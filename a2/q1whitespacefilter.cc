#include <iostream>
#include "q1whitespacefilter.h"

using namespace std;

void WhitespaceFilter::main() {
  try {
    _Enable {
      for (;;) {
        // eat all spaces in beginning
        while (isblank(ch)) {
          suspend();
        }
        
        // echo the middle while eating through all spaces
        for (;;) {
          // send char to next filter if it's not blank
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
    _Resume eof _At *next;
    next->put('\n'); // put arbitrary char to resume next filter
  }
}

WhitespaceFilter::WhitespaceFilter(Filter *f) {
  next = f; // set next filter
}
