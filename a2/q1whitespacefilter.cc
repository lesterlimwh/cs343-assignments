#include <iostream>
#include "q1whitespacefilter.h"

using namespace std;

void WhitespaceFilter::main() {
  cout << "whitespacefilter main has begun" << endl; 
  try {
    _Enable {
      for (;;) {
        // eat all spaces in beginning
        while (isblank(ch)) {
          suspend();
        }
        
        // echo the middle while eating through all spaces
        for (;;) {
          // echo character if it's not blank
          if (!isblank(ch)) {
            next->put(ch);
            suspend();
            continue;
          }

          // eat up all blank characters
          while (isblank(ch)) {
            suspend();
          }
          if (ch == '\n') { // case where it's at the end of the line
            next->put('\n');
          } else {
            next->put(' '); // case where it's in the middle
            next->put(ch);
          }

          suspend();
        }
      }
    }
  } catch(Eof& eof) {
    cout << "eof caught in whitespacefilter" << endl;
    // rethrow instead
    _Resume eof _At *next;
    next->put('\n');
  }
}

WhitespaceFilter::WhitespaceFilter(Filter *f) {
  next = f; // set next filter
}
