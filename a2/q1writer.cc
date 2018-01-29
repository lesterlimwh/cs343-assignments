#include "q1writer.h"
#include <iostream>

using namespace std;

void Writer::main() {
  cout << "writer main has begun" << endl; 
  try {
    _Enable {
      for (;;) {
        outputCount++;
        (*out) << ch;
        suspend();
      }
    }
  } catch(Eof&) {
    cout << "eof caught in writer" << endl;
    cout << outputCount << "characters" << endl;
  }
}

Writer::Writer(ostream *o) {
  out = o;
  outputCount = 0;
}
