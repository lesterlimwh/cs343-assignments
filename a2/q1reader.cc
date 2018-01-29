#include "q1reader.h"
#include <iostream>

using namespace std;

void Reader::main() {
  cout << "reader main has begun" << endl; 
  suspend();
}

Reader::Reader(Filter *f, istream *i) {
  cout << "reader constructed" << endl;
}
