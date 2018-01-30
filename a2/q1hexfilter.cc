#include <iostream>
#include <iomanip>
#include <sstream>
#include "q1hexfilter.h"

using namespace std;

string HexFilter::charToHex(char ch) {
  stringstream ss;
  ss << setfill('0') << setw(2) << hex << int(ch);
  return ss.str();
}

void HexFilter::handleTwoChars() {
  string hexString;
  for (int i = 0; i < 2; i++) {
    hexString = charToHex(ch);
    next->put(hexString[0]);
    next->put(hexString[1]);
    suspend();
  }
}

void HexFilter::main() {
  try {
    _Enable {
      for (;;) {
        for (int g = 0; g < 4; g++) { // pattern repeated 4 times a line
          handleTwoChars();
          next->put(' '); // 1 space
          handleTwoChars();
          if (g == 3) {
            next->put('\n');
          } else {
            // 3 spaces
            next->put(' ');
            next->put(' ');
            next->put(' ');
          }
        }
      }
    }
  } catch(Eof& eof) {
    _Resume eof _At *next;
    next->put('\n'); // resume next filter with arbitrary char
  }
}

HexFilter::HexFilter(Filter *f) {
  next = f; // set next filter
}
