#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <cctype>
#include <cassert>
#include "utf8/utf8.h"

using namespace std;

typedef int unicodeChar;
typedef basic_string<unicodeChar> unicodeString;

bool comparePairs(      // Return true iff x < y
    const pair<unicodeString, int>& x,
    const pair<unicodeString, int>& y
);

ostream& operator<<(ostream& s, const unicodeString& txt);

bool operator<(const unicodeString& s1, const unicodeString& s2);   

int main() {
    int state = 0;  // 0 between words, 1 inside a word
    unicodeString word;
    map<unicodeString, int> words;
    while (true) {
        bool readFails = false;
        unicodeChar c;
        // cin >> c; 
        c = unicodeChar(get_utf8_code_point(cin, readFails));
        if (readFails)
            break;
        bool letter = isRussianLetter(c);
        if (state == 0) {   // Between words
            if (letter) {
                word.clear();
                word += unicodeChar(toLowerLetter(c));
                state = 1;  // Reading a word
            } else {
                // Nothing to do.
            }            
        } else {
            assert(state == 1); // Reading a word
            if (letter) {
                word += unicodeChar(toLowerLetter(c));
            } else {
                // End of word reading
                if (words.count(word) == 0) {
                    // Add the word to the dictionary
                    words[word] = 1;
                } else {
                    // Increment the number of word occurencies in the text
                    ++words[word];
                }
                state = 0;  // State = between words
            }
        }
    }     
    if (state == 1) {
        // Process the last word            
        if (words.count(word) == 0) {
            // Add the word to the dictionary
            words[word] = 1;
        } else {
            // Increment the number of word occurencies in the text
            ++words[word];
        }
    }
    
    // Our dictionary is ready!
    // Sort the words by their numbers in the text
    vector< pair<unicodeString, int> > wordsFreq;
    auto i = words.cbegin();
    while (i != words.end()) {
        wordsFreq.push_back(*i);
        ++i;
    }
    
    sort(wordsFreq.begin(), wordsFreq.end(), comparePairs); 
    
    // Print the results
    for (size_t i = 0; i < wordsFreq.size(); ++i) {
        cout << wordsFreq[i].second << "\t" << wordsFreq[i].first << endl;
    }
    
    cout << "Total number of different words in a text: " << 
        wordsFreq.size() << endl;
    return 0;
}
    
bool comparePairs(      // Return true iff x < y
    const pair<unicodeString, int>& x,
    const pair<unicodeString, int>& y
) {
    if (x.second > y.second) {
        return true;
    } else if (x.second < y.second) {
        return false;
    }
    assert(x.second == y.second);
    return (x.first < y.first);
}

bool operator<(const unicodeString& s1, const unicodeString& s2) {
    size_t l1 = s1.length();
    size_t l2 = s2.length();
    for (size_t i = 0; i < l1; ++i) {
        if (i >= l2) {
            // s2 is a beginning of s1
            return false;
        }
        int c = compareRussianLetters(s1[i], s2[i]);
        if (c < 0) {
            return true;
        } else if (c > 0) {
            return false;
        }
        assert(s1[i] == s2[i]);
    }
    if (l2 > l1) {
        return true;
    }
    assert(l1 == l2);
    return false;
}

ostream& operator<<(ostream& s, const unicodeString& txt) {
    size_t l = txt.length();
    for (size_t i = 0; i < l; ++i) {
        output_utf8(s, txt[i]);     
    }
    return s;    
}
