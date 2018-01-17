#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctype.h>
using namespace std;

_Coroutine Money {
    char ch; // currently processed char
    char decimal_ch; // expected decimal char
    char separator_ch; // expected separator char

    void handleFailure() {
      cout << "nope" << endl;
    }

    void main() { // coroutine main
      int DIGITS_PER_GROUP = 3; // number of digits allowed in a digit grouping

      // check that it starts with $ or E
      if (ch == '$') {
        decimal_ch = '.'; 
        separator_ch = ','; 
      } else if (ch == 'E') {
        decimal_ch = ','; 
        separator_ch = '.'; 
      } else {
        handleFailure();
      }
      suspend();

      // check for optional minus
      if (ch != '-' && !isdigit(ch)) {
        handleFailure();
      } else if (ch == '-') {
        suspend();
      }

      int max_groups = 0;
      Groups: for (int i=0; i<max_groups; ++i) {
        if (i == 0) { // parse first digit group 
          for (int j=0; j<DIGITS_PER_GROUP; ++j) {
            if (!isdigit(ch) && ch != separator_ch && ch != decimal_ch) {
              handleFailure();
            } else if (ch == decimal_ch) {
              suspend();
        break Groups;
            } else if (ch == separator_ch) {
              suspend();
        continue Groups;
            } else { // it's a digit
              suspend();
              continue;
            }
          }
        } else { // not the first grouping 
          // parse 3 digits
          for (int j=0; j<DIGITS_PER_GROUP; ++j) {
            if (!isdigit(ch)) {
              handleFailure();
            }
            suspend();
          }
          // parse decimal or separator
          if (ch == decimal_ch) {
            suspend();
        break Groups;
          } else if (ch == separator_ch) {
            suspend();
        continue Groups;
          } else {
            handleFailure();
          }
        }
      }
      
      // parse decimal digits
      for (int i=0; i<2; ++i) {
        if (!isdigit(ch)) {
          handleFailure();
        }
        suspend();
      }

      cout << "valid!" << endl;
    }
  public:
    Money() {
      cout << " constructed " << endl;
    }
    _Event Match {}; // last character match
    _Event Error {}; // last character invalid
    void next(char c) {
      ch = c;
    	resume();
    }
};

void usage(char *argv[]) {
  cerr << "Usage: " << argv[0]
    << " [ input-file ]" << endl;
  exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
	istream *in = &cin;

  if (argc == 2) {
    try {
      in = new ifstream(argv[1]);
    } catch(uFile::Failure) {
      cerr << "Error! Could not open input file \"" << argv[1] << "\"" << endl;
      exit(EXIT_FAILURE);
    }
  } else if (argc != 1) {
    usage(argv);
  }

  *in >> noskipws; // turn off white space skipping during input

  string str;
  while (std::getline(*in, str)) { // read each line in input stream
    cout << "lala" << endl;
    Money m;
    for (char& ch : str) { // iterate through characters per line
      m.next(ch);
    }
  }

  cout << "ending" << endl;

  // for (;;) {
    // *in >> ch; // read a character
    // // cout << ch << endl;

    // if (ch == '\n') {
      // cout << "new line" << endl;
    // }

    // if (in->fail()) {
      // break; // end program when EOF or error
    // }
  // }

  if (in != &cin) {
    delete in; // close input stream if it's not cin
  }
}
