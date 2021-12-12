#include <iostream>
#include <map>
#include <vector>
// #include <pair>
#include <algorithm>

#include "utf8//utf8.h"

using namespace std;
class Bigram {
 public:
  int bigram[2];
  explicit Bigram(int first = 0, int second = 0) {
    bigram[0] = first;
    bigram[1] = second;
  }
  bool operator<(const Bigram& b) const {
    int c1 = compareRussianLetters(bigram[0], b.bigram[0]);
    if (c1 < 0) return true;
    if (c1 > 0) return false;
    assert(c1 == 0);
    int c2 = compareRussianLetters(bigram[1], b.bigram[1]);
    return c2 < 0;
    //    return (bigram[0] < b.bigram[0] ||
    //            (bigram[0] == b.bigram[0] && bigram[1] < b.bigram[1]));
  }
};
bool compareCharPairs(const pair<int, int>& p0, const pair<int, int>& p1) {
  if (p0.first > p1.first) return true;
  if (p0.first < p1.first) return false;
  assert(p0.first == p1.first);
  int c1 = compareRussianLetters(p0.second, p1.second);
  return c1 < 0;
}
bool compareBigramPairs(const pair<int, Bigram>& p0,
                        const pair<int, Bigram>& p1) {
  if (p0.first > p1.first) return true;
  if (p0.first < p1.first) return false;
  assert(p0.first == p1.first);
  return p0.second < p1.second;
}

int main() {
  int totalChars = 0;
  map<int, int> alphabet;
  map<Bigram, int> bigrams;
  bool fails = false;
  int previousLetter = -1;
  int bigramsDetected = 0;
  while (!fails) {
    int codePoint = get_utf8_code_point(cin, fails);
    if (isRussianLetter(codePoint)) {
      ++totalChars;
      codePoint = toUpperLetter(codePoint);
      if (alphabet.find(codePoint) == alphabet.end()) {
        // This is a new letter,
        // add it to the alphabet
        alphabet[codePoint] = 1;
      } else {
        // The letter is already in the alphabet
        ++(alphabet[codePoint]);
      }
      if (previousLetter >= 0) {
        ++bigramsDetected;
        Bigram currentBigram(previousLetter, codePoint);
        if (bigrams.count(currentBigram) != 0)
          ++(bigrams[currentBigram]);
        else
          bigrams[currentBigram] = 1;
      }
      previousLetter = codePoint;
    } else {
      previousLetter = -1;
    }

  }  // end of while

  vector<pair<int, int>> pairs;
  auto i = alphabet.begin();
  while (i != alphabet.end()) {
    pairs.emplace_back(i->second, i->first);
    ++i;
  }
  stable_sort(pairs.begin(), pairs.end(), compareCharPairs);

  // Print results
  for (auto& pair : pairs) {
    // Print a letter
    output_utf8(cout, pair.second);
    // Print its frequence
    cout << ' ' << pair.first << ' ' << (double)pair.first / (double)totalChars
         << endl;
  }

  cout << endl << "------------" << endl;
  cout << "Bigrmas in the text:" << endl;
  vector<pair<int, Bigram>> bigramPairs;
  auto j = bigrams.begin();
  while (j != bigrams.end()) {
    bigramPairs.emplace_back(j->second, j->first);
    ++j;
  }

  stable_sort(bigramPairs.begin(), bigramPairs.end(), compareBigramPairs);

  // Print results
  for (auto& pair : bigramPairs) {
    // Print a bigram
    output_utf8(cout, pair.second.bigram[0]);
    output_utf8(cout, pair.second.bigram[1]);
    // Print its frequence
    cout << ' ' << pair.first << ' '
         << (double)pair.first / (double)bigramsDetected << endl;
  }

  return 0;
}
