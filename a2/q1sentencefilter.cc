#include <iostream>
#include <vector>
#include "q1sentencefilter.h"

using namespace std;

// checks whether c is a valid closing punctuation for sentence
bool isValidSentenceEnd(char c) {
  return c == '.' || c == '!' || c == '?';
}

// filters out lines that don't end with a period, question mark, or exclamation mark
void SentenceFilter::main() {
  try {
    _Enable {
      for (;;) {
        vector<char> sentence;

        while (ch != '\n') {
          sentence.push_back(ch); // accumulate line
          suspend();
        }

        if (!sentence.empty() && isValidSentenceEnd(sentence.back())) {
          // output line
          for (auto c : sentence) {
            next->put(c);
          }
          next->put('\n');
        }

        sentence.clear();
        suspend();
      }
    }
  } catch(Eof& eof) {
    _Resume eof _At *next;
    next->put('\n'); // put arbitrary char to resume next filter
  }
}

SentenceFilter::SentenceFilter(Filter *f) {
  next = f; // set next filter
}
