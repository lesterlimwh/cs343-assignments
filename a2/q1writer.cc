#include "q1writer.h"
#include <iostream>

using namespace std;

void Writer::main() {
  cout << "writer main has begun" << endl; 
  try {
    _Enable {
      for (;;) {
        (*out) << ch;
        suspend();
      }
    }
  } catch(Eof&) {
    cout << "eof caught in writer" << endl;
  }
}

Writer::Writer(ostream *o) {
  out = o;
}
