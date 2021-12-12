#ifndef UTF8_H
#define UTF8_H

#include <iostream>

int get_utf8_code_point(std::istream& stream, bool& readFails);
bool output_utf8(std::ostream& stream, int code_point);
bool isRussianLetter(int code_point);
bool isEnglishLetter(int code_point);

bool isUpperRussianLetter(int code_point);
bool isLowerRussianLetter(int code_point);

bool isUpperEnglishLetter(int code_point);
bool isLowerEnglishLetter(int code_point);

int toUpperLetter(int code_point);
int toLowerLetter(int code_point);

int compareRussianLetters(int c0, int c1);
int compareRussianLetters_NoCase(int c0, int c1);

const int NUM_RUSSIAN_LETTERS = 66;  // Including upper and lower letters
const int NUM_RUSSIAN_UPPER_LETTERS = 33;
const int NUM_RUSSIAN_LOWER_LETTERS = 33;

int upperRussianLetter(int orderInAlphabet);  // order = 0..32
int lowerRussianLetter(int orderInAlphabet);  // order = 0..32

// Alphabet is "A a B b ... Ya ya"
int russianLetter(int orderInAlphabet);  // order = 0..65

// The following functions return (-1) for any characters
// that are not Russian letters
int russianLetterOrder(int codePoint);         // order = 0..65
int russianLetterOrder_NoCase(int codePoint);  // order = 0..32

#endif