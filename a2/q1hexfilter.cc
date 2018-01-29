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
  cout << "hexfilter main has begun" << endl; 
  try {
    _Enable {
      for (;;) {
        for (int g = 0; g < 4; g++) {
          handleTwoChars();
          next->put(' ');
          handleTwoChars();
          if (g == 3) {
            next->put('\n');
          } else {
            next->put(' ');
            next->put(' ');
            next->put(' ');
          }
        }
      }
    }
  } catch(Eof& eof) {
    cout << "eof caught in hexfilter" << endl;
    // rethrow instead
    _Resume eof _At *next;
    next->put('\n');
  }
}

HexFilter::HexFilter(Filter *f) {
  next = f; // set next filter
}
