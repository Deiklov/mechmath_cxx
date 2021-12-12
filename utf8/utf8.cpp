#include "utf8.h"

#include <cassert>
#include <cctype>
#include <iostream>

using namespace std;

static int toUpperRussianLetter(int code_point);
static int toUpperEnglishLetter(int code_point);
static int toLowerRussianLetter(int code_point);
static int toLowerEnglishLetter(int code_point);

static const int RUSSIAN_LETTERS_ORDER[] = {
    // Upper case russian letters in alphabet order
    0x410, 0x411, 0x412, 0x413, 0x414,  // A   B  V   G  D
    0x415, 0x401, 0x416, 0x417, 0x418,  // E   Yo Zhe Z  I
    0x419, 0x41A, 0x41B, 0x41C, 0x41D,  // Yot K  L   M  N
    0x41E, 0x41F, 0x420, 0x421, 0x422,  // O   P  R   S  T
    0x423, 0x424, 0x425, 0x426, 0x427,  // Ou  F  Kh  Ts Che
    0x428, 0x429, 0x42A, 0x42B, 0x42C,  // Sha Tsche HardSign Y SoftSign
    0x42D, 0x42E, 0x42F,                // Ea  Yu Ya
    // Lower case letters
    0x430, 0x431, 0x432, 0x433, 0x434,  // a   b  v   g  d
    0x435, 0x451, 0x436, 0x437, 0x438,  // e   yo zhe z  i
    0x439, 0x43A, 0x43B, 0x43C, 0x43D,  // yot k  l   m  n
    0x43E, 0x43F, 0x440, 0x441, 0x442,  // o   p  r   s  t
    0x443, 0x444, 0x445, 0x446, 0x447,  // ou  f  kh  ts che
    0x448, 0x449, 0x44A, 0x44B, 0x44C,  // sha tsche hard_sign y soft_sign
    0x44D, 0x44E, 0x44F,                // ea  yu ya
    0                                   // terminator
};

static const int RUS_CODE_BASE = 0x400;

static int russianLettersOrder[256];
static bool russianLettersOrder_initialized = false;
static void initializeRussianLettersOrder();

int get_utf8_code_point(istream& stream, bool& readFails) {
  unsigned char leadingByte, continuationByte;
  int result = 0;
  // stream >> leadingByte;
  leadingByte = (unsigned char)stream.get();
  // cout << "Leading byte = " << (unsigned int) leadingByte << endl;

  if (stream.fail()) {
    readFails = true;
    return (-1);
  }
  readFails = false;
  if ((0x80 & leadingByte) == 0) {
    // ASCII character
    return leadingByte;
  }

  // Count the number of continuation bytes
  int k = 0;
  unsigned int bit = 0x80;
  unsigned int mask = 0x7F;  // Mask of data bits in leading byte
  while (k < 6 && (leadingByte & bit) != 0) {
    ++k;
    bit >>= 1;
    mask >>= 1;
  }
  if (k == 6 && (leadingByte & bit) != 0) {
    // Illegal character (maybe, BOM)
    return (-1);
  }
  result |= (leadingByte & mask);  // Data bits in leading byte
  --k;                             // k is the number of continuation bytes

  while (k > 0) {
    // stream >> continuationByte;
    continuationByte = (unsigned char)stream.get();

    if (stream.fail()) {
      readFails = true;
      return (-1);
    }
    if ((continuationByte & 0xC0) != 0x80) {
      return (-1);
    }
    result <<= 6;
    result |= (0x3F & continuationByte);
    --k;
  }
  return result;
}

bool output_utf8(std::ostream& stream, int code_point) {
  if ((code_point & 0x80000000) != 0) return false;

  // Count a number of bits (from non-zero leading bit)
  int bit = 0x40000000;
  int bitNumber = 30;
  while (bitNumber >= 0 && (bit & code_point) == 0) {
    bit >>= 1;
    --bitNumber;
  }

  // Theoretical maximal is 6-byte sequence:
  // 1 bit in leading byte,
  // 5*6 bits in continuation bytes
  // But now maximal is 21 bits (4-byte sequence)

  int numBits = bitNumber + 1;
  //... cout << std::dec << "numBits = " << numBits << endl;

  assert(0 <= numBits && numBits <= 31);
  if (numBits > 21) return false;

  if (numBits <= 7) {
    // ASCII
    stream << ((char)code_point);
    return stream.good();
  }

  // 1 continuation byte: 5 + 6 = 11 bits
  // 2 continuation bytes: 4 + 6 + 6 = 16 bits
  // 3 continuation bytes: 3 + 6 + 6 + 6 = 21 bits
  int numContBytes = 1;
  if (numBits > 11) ++numContBytes;
  if (numBits > 16) ++numContBytes;
  //... cout << "numContBytes = " << numContBytes << endl;

  unsigned char leadingByte;
  if (numBits > 16) {
    // 4-byte sequence
    assert(numContBytes == 3);
    int mask = 0x07;  // 3 bits in leading byte
    leadingByte = (unsigned char)((code_point >> (6 * 3)) & mask);
    leadingByte |= (unsigned char)0xF0;
  } else if (numBits > 11) {
    // 3-byte sequence
    assert(numContBytes == 2);
    int mask = 0x0F;  // 4 bits in leading byte
    leadingByte = (char)((code_point >> (6 * 2)) & mask);
    leadingByte |= (unsigned char)0xE0;
  } else {
    // 2-byte sequence
    assert(numContBytes == 1);
    int mask = 0x1F;  // 5 bits in leading byte
    leadingByte = (char)((code_point >> (6 * 1)) & mask);
    leadingByte |= (unsigned char)0xC0;
  }

  // Write the leading byte
  //... cout << "leadingByte = " << std::hex <<
  //...     ((unsigned int) leadingByte & 0xFF) << endl;
  stream << leadingByte;
  if (stream.fail()) return false;

  // Write continuation bytes
  for (int i = 1; i <= numContBytes; ++i) {
    unsigned char contByte = (unsigned char)0x80;
    contByte |=
        (unsigned char)((code_point >> (6 * (numContBytes - i))) & 0x3F);
    stream << contByte;
    if (stream.fail()) return false;
  }
  return true;
}

// Upper Russian letters: 1040..1071 plus Yo 1025
// Lower Russian letters: 1072..1103 plus yo 1105

bool isRussianLetter(int code_point) {
  return ((1040 <= code_point && code_point <= 1103) ||
          code_point == 1025 ||  // YO
          code_point == 1105     // yo
  );
}

bool isEnglishLetter(int code_point) {
  return (0 <= code_point && code_point < 128 && isalpha(code_point));
}

int toUpperLetter(int code_point) {
  if (isRussianLetter(code_point)) {
    return toUpperRussianLetter(code_point);
  } else if (isEnglishLetter(code_point)) {
    return toUpperEnglishLetter(code_point);
  } else {
    return code_point;
  }
}

int toLowerLetter(int code_point) {
  if (isRussianLetter(code_point)) {
    return toLowerRussianLetter(code_point);
  } else if (isEnglishLetter(code_point)) {
    return toLowerEnglishLetter(code_point);
  } else {
    return code_point;
  }
}

static int toLowerRussianLetter(int code_point) {
  if (isUpperRussianLetter(code_point)) {
    if (code_point != 1025) {  // Yo
      return (1072 + (code_point - 1040));
    } else {
      return 1105;  // yo
    }
  }
  return code_point;
}

static int toLowerEnglishLetter(int code_point) {
  if (0 < code_point && code_point < 128 && isalpha(code_point) &&
      isupper(code_point))
    return tolower(code_point);
  return code_point;
}

static int toUpperRussianLetter(int code_point) {
  if (isLowerRussianLetter(code_point)) {
    if (code_point != 1105) {  // yo
      return (1040 + (code_point - 1072));
    } else {
      return 1025;  // Yo
    }
  }
  return code_point;
}

static int toUpperEnglishLetter(int code_point) {
  if (0 < code_point && code_point < 128 && isalpha(code_point) &&
      islower(code_point))
    return toupper(code_point);
  return code_point;
}

bool isUpperRussianLetter(int code_point) {
  return ((1040 <= code_point && code_point <= 1071) ||
          code_point == 1025  // Yo
  );
}

bool isLowerRussianLetter(int code_point) {
  return ((1072 <= code_point && code_point <= 1103) ||
          code_point == 1105  // yo
  );
}

bool isUpperEnglishLetter(int code_point) {
  return (0 < code_point && code_point < 128 && isalpha(code_point) &&
          isupper(code_point));
}

bool isLowerEnglishLetter(int code_point) {
  return (0 < code_point && code_point < 128 && isalpha(code_point) &&
          islower(code_point));
}

static void initializeRussianLettersOrder() {
  if (russianLettersOrder_initialized) return;
  for (int i = 0; i < 256; ++i) {
    russianLettersOrder[i] = (-1);  // undefined
  }

  // Order of upper case letters is even, lower case is odd
  for (int i = 0; i < NUM_RUSSIAN_UPPER_LETTERS; ++i) {
    int upperLetter = RUSSIAN_LETTERS_ORDER[i];
    assert(isUpperRussianLetter(upperLetter));
    int lowerLetter = RUSSIAN_LETTERS_ORDER[i + NUM_RUSSIAN_UPPER_LETTERS];
    assert(isLowerRussianLetter(lowerLetter));

    russianLettersOrder[upperLetter - RUS_CODE_BASE] = 2 * i;
    russianLettersOrder[lowerLetter - RUS_CODE_BASE] = 2 * i + 1;
  }
  russianLettersOrder_initialized = true;
}

int compareRussianLetters(int c0, int c1) {
  if (!russianLettersOrder_initialized) initializeRussianLettersOrder();

  if (isRussianLetter(c0) && isRussianLetter(c1)) {
    int order0 = russianLettersOrder[c0 - RUS_CODE_BASE];
    assert(order0 >= 0);
    int order1 = russianLettersOrder[c1 - RUS_CODE_BASE];
    assert(order1 >= 0);
    return (order0 - order1);
  } else {
    return (c0 - c1);
  }
}

int compareRussianLetters_NoCase(int c0, int c1) {
  if (!russianLettersOrder_initialized) initializeRussianLettersOrder();

  int c0_lower = toLowerLetter(c0);
  int c1_lower = toLowerLetter(c1);

  if (isRussianLetter(c0_lower) && isRussianLetter(c1_lower)) {
    // Upper letters have even orders, lower have odd orders
    // (Order of lower 'a') = (Order of upper 'A')*2 + 1

    int order0 = (russianLettersOrder[c0_lower - RUS_CODE_BASE] / 2);
    assert(order0 >= 0);
    int order1 = (russianLettersOrder[c1_lower - RUS_CODE_BASE] / 2);
    assert(order1 >= 0);
    return (order0 - order1);
  } else {
    return (c0_lower - c1_lower);
  }
}

int upperRussianLetter(int orderInAlphabet) {  // order = 0..32
  assert(0 <= orderInAlphabet && orderInAlphabet < NUM_RUSSIAN_UPPER_LETTERS);
  int c = RUSSIAN_LETTERS_ORDER[orderInAlphabet];
  assert(isUpperRussianLetter(c));
  return c;
}

int lowerRussianLetter(int orderInAlphabet) {  // order = 0..32
  assert(0 <= orderInAlphabet && orderInAlphabet < NUM_RUSSIAN_LOWER_LETTERS);
  int c = RUSSIAN_LETTERS_ORDER[orderInAlphabet + NUM_RUSSIAN_UPPER_LETTERS];
  assert(isLowerRussianLetter(c));
  return c;
}

// Alphabet is "A a B b ... Ya ya"
int russianLetter(int orderInAlphabet) {  // order = 0..65
  assert(0 <= orderInAlphabet && orderInAlphabet < NUM_RUSSIAN_LETTERS);
  if (orderInAlphabet % 2 == 0) {
    // This is upper case letter
    int c = upperRussianLetter(orderInAlphabet / 2);
    assert(isUpperRussianLetter(c));
    return c;
  } else {
    // This is lower case letter
    int c = lowerRussianLetter(orderInAlphabet / 2);
    assert(isLowerRussianLetter(c));
    return c;
  }
}

int russianLetterOrder(int codePoint) {  // order = 0..65
  if (!russianLettersOrder_initialized) initializeRussianLettersOrder();

  int idx = codePoint - RUS_CODE_BASE;
  if (idx < 0 || idx >= 256) return (-1);
  int order = russianLettersOrder[idx];
  assert(0 <= order && order < NUM_RUSSIAN_LETTERS);
  return order;
}

int russianLetterOrder_NoCase(int codePoint) {  // order = 0..32
  int order = russianLetterOrder(codePoint);
  if (order < 0) return order;
  order /= 2;
  assert(0 <= order && order < NUM_RUSSIAN_UPPER_LETTERS);
  return order;
}