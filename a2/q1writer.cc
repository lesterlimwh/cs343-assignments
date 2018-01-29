#include "q1writer.h"
#include <iostream>

using namespace std;

void Writer::main() {
  try {
    _Enable {
      for (;;) {
        outputCount++;
        (*out) << ch; // outputs char to stream
        suspend();
      }
    }
  } catch(Eof&) {
    cout << outputCount << " characters" << endl;
  }
}

Writer::Writer(ostream *o) {
  out = o;
  outputCount = 0;
}
