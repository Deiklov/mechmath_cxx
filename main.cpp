#include <cassert>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <list>
#include <string>
#include <utility>
#include <vector>

using namespace std;

void directSort(vector<string>::iterator first, vector<string>::iterator last);

void radixSort(vector<string>::iterator first, vector<string>::iterator last,
               int compareLength = 64);

void printArray(vector<string>::iterator first, vector<string>::iterator last);

void generateRandomString(string& s, int minLength = 1, int maxLength = 70);

int main() {
  time_t tim;
  tim = time(&tim);
  srand(int(tim));
  vector<string> a;
  while (true) {
    int n;
    cout << "n: ";
    cin >> n;
    if (n <= 0) break;

    int minLen = 0, maxLen = 0;
    cout << "Minimal and maximal lengths of strings: " << endl;
    cin >> minLen >> maxLen;
    if (minLen == 0 || maxLen < minLen) break;

    int sortAlg;
    cout << "Enter number of sorting algorithm:" << endl;
    cout << "1 DirectSort, 2 RadixSort" << endl;
    cin >> sortAlg;

    int compareLength = 0;
    if (sortAlg == 2) {
      cout << "Enter string length to compare: " << endl;
      cin >> compareLength;
      if (compareLength == 0) break;
    }

    // Generate a random array of size n
    a.resize(n);
    for (int i = 0; i < n; ++i) {
      generateRandomString(a[i], minLen, maxLen);
    }
    cout << "Random array:" << endl;
    printArray(a.begin(), a.end());
    cout << "-------------" << endl;

    if (sortAlg == 1)
      directSort(a.begin(), a.end());
    else if (sortAlg == 2)
      radixSort(a.begin(), a.end(), compareLength);
    else
      continue;
    cout << "Sorted array:" << endl;
    printArray(a.begin(), a.end());
    cout << "-------------" << endl;
  }
  return 0;
}

void printArray(vector<string>::iterator a,    // Beginning of array segment
                vector<string>::iterator last  // After-end of array segment
) {
  size_t n = last - a;
  if (n <= 1000) {
    // Print all array
    for (size_t i = 0; i < n; ++i) {
      cout << a[i] << endl;
    }
  } else {
    // The array is too big, print its beginning and end only
    // (100 first and 100 last elements)
    for (size_t i = 0; i < 100; ++i) {
      cout << a[i] << endl;
    }
    cout << ". . ." << endl;
    for (size_t i = n - 100; i < n; ++i) {
      cout << a[i] << endl;
    }
  }
}

void directSort(vector<string>::iterator a,    // Beginning of array segment
                vector<string>::iterator last  // After-end of array segment
) {
  size_t n = last - a;
  for (size_t i = 0; i < n - 1; ++i) {
    size_t indMin = i;
    for (size_t j = i + 1; j < n; ++j) {
      if (a[j] < a[indMin]) indMin = j;
    }
    if (indMin != i) swap(a[i], a[indMin]);
  }
}

static const char* const LETTERS = "abcdefghijklmnopqrstuvwxyz";
// "ABCDEFGHIJKLMNOPQRSTUVWXYZ_";
static const char* const DIGITS = "0123456789";

void generateRandomString(string& s, int minLength /* = 1 */,
                          int maxLength /* = 70 */
) {
  //... int numLetters = int(strlen(LETTERS));
  int numLetters = 0;
  int numDigits = int(strlen(DIGITS));
  s.clear();

  // Select a random length
  int len = minLength + rand() % (maxLength + 1 - minLength);

  for (int i = 0; i < len; ++i) {
    int r = rand() % (numLetters + numDigits);
    char c;
    if (r < numLetters) {
      // Add a random letter
      c = LETTERS[rand() % numLetters];
    } else {
      // Add a random digit
      c = DIGITS[rand() % numDigits];
    }
    s.push_back(c);
  }
}

void radixSort(vector<string>::iterator first, vector<string>::iterator last,
               int compareLength /* = 64 */
) {
  //... to be implemented...
  cout << "radixSort is not implemented yet..." << endl;
}
void mergeSortRecursive(
    vector<double>::iterator first, // Beginning of array segment
    vector<double>::iterator last,  // After-end of array segment
    vector<double>* tmpMemory = 0   // Temporary memory to use
);