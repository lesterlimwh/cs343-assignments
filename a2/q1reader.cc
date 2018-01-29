#include "q1reader.h"
#include <iostream>

using namespace std;

void Reader::main() {
  for (;;) {
    *in >> ch; // read a char

    if (in->fail()) {
      _Resume Eof() _At *next; // notify end of file
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
