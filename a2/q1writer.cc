#include "q1writer.h"
#include <iostream>

using namespace std;

void Writer::main() {
  cout << "writer main has begun" << endl; 
  suspend();
}

Writer::Writer(ostream *o) {
  cout << "Writer constructed" << endl;
}
