#include "q1reader.h"
#include <iostream>

using namespace std;

void Reader::main() {
  cout << "reader main has begun" << endl; 

  for (;;) {
    *in >> ch;

    if (in->fail()) {
      _Resume Eof() _At *next;
      next->put('\n'); // resume next filter with arbitrary char
      break;
    }

    next->put(ch);
  }
}

Reader::Reader(Filter *f, istream *i) {
  // set private members
  next = f;
  in = i;
  resume(); // start reader as soon as it's declared
}
