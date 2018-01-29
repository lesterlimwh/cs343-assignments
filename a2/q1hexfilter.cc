#include <iostream>
#include "q1hexfilter.h"

using namespace std;

void HexFilter::main() {
  cout << "hexfilter main has begun" << endl; 
  try {
    _Enable {
      for (;;) {
        next->put(ch);
        suspend();
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
