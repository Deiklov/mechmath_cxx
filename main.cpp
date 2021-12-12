#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

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
  }
};


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


  cout << endl << "------------" << endl;
  cout << "Bigrams in the text:" << endl;
  vector<pair<int, Bigram>> bigramPairs;
  auto j = bigrams.begin();
  while (j != bigrams.end()) {
    bigramPairs.emplace_back(j->second, j->first);
    ++j;
  }

  stable_sort(bigramPairs.begin(), bigramPairs.end(), compareBigramPairs);

  // Print results
  for (int i = 0; i < 100 && i < bigramPairs.size(); ++i) {
    // Print a bigram
    output_utf8(cout, bigramPairs[i].second.bigram[0]);
    output_utf8(cout, bigramPairs[i].second.bigram[1]);
    // Print its frequence
    cout << ' ' << bigramPairs[i].first << ' '
         << (double)bigramPairs[i].first / (double)bigramsDetected << endl;
  }

  return 0;
}
