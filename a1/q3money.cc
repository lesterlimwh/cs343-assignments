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
      _Resume Error() _At resumer();
      for (;;) suspend();
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

      int max_groups = 5;
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
          // parse separator or decimal
          if (ch == decimal_ch) {
            suspend();
        break Groups;
          } else if (ch == separator_ch) {
            suspend();
        continue Groups;
          } else {
            handleFailure();
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
          } else if (ch == separator_ch && i < max_groups - 1) { // reject separator in last group
            suspend();
        continue Groups;
          } else {
            handleFailure();
          }
        }
      }

      // parse first decimal digit
      if (!isdigit(ch)) {
        handleFailure();
      }
      suspend();

      // parse second decimal digit
      if (!isdigit(ch)) {
        handleFailure();
      }
      suspend();

      if (ch != '\n') {
        handleFailure();
      }

      _Resume Match() _At resumer();
    }
  public:
    Money() {}
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
    size_t i;
  
    // special case for newlines
    if (str == "") {
      cout <<  "'' : Warning! Blank line." << endl;
      continue;
    }

    try {
      _Enable{
        Money m;

        for (i=0; i<str.length(); ++i) { // iterate through characters per line
          m.next(str[i]);
        }
        i--; // decrement counter to account extra increment at for completion
        m.next('\n'); // send newline to resume and indicate completion to Money
      }
    } catch(Money::Match) {
      cout << "'" << str << "' : '" << str.substr(0, i+1)
        << "' yes" << endl;
    } catch(Money::Error) {
      cout << "'" << str << "' : '" << str.substr(0, i+1)
        << "' no";

      if (i != str.length() - 1) {
        cout << " -- extraneous characters '" << str.substr(i+1) << "'" << endl;
      } else {
        cout << endl;
      }
      
    }
  }

  if (in != &cin) {
    delete in; // close input stream if it's not cin
  }
}
