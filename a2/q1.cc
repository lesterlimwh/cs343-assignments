#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "q1filter.h"
#include "q1hexfilter.h"
// #include "q1whitespacefilter.h"
#include "q1reader.h"
#include "q1writer.h"

using namespace std;

/*
_Coroutine filter-name : public Filter {
    // YOU MAY ADD PRIVATE MEMBERS
    void main();
  public:
    filter-name( Filter * f, ... );
};
*/

// prints usage information
void usage(char *argv[]) {
  cerr << "Usage: " << argv[0]
    << " [ -filter-options ... ] [ infile [outfile] ]" << endl;
  exit(EXIT_FAILURE);
}

int main( int argc, char *argv[] ) {
  // initialize streams to stdin and stdout
  istream *in = &cin;
  ostream *out = &cout;
  vector<char*> filterOptions; // holds filter options

  int i = 1;

  // parse filter options
  while (i < argc && argv[i][0] == '-') {
    filterOptions.push_back(argv[i]);
    i++;
  }

  // parse possible input and output files
  int remainingArgCount = argc - i;
  switch(remainingArgCount) {
    case 2:
      try {
        out = new ofstream(argv[i + 1]);
      } catch(uFile::Failure) {
        cerr << "Error! Could not open output file \"" << argv[i + 1] << "\"" << endl;
        exit(EXIT_FAILURE);
      }
    case 1:
      try {
        in = new ifstream(argv[i]);
      } catch(uFile::Failure) {
        cerr << "Error! Could not open input file \"" << argv[i] << "\"" << endl;
        exit(EXIT_FAILURE);
      }
    case 0:
      break;
    default:
      usage(argv);
  }

  // initialize filter system
  Writer writer(out);
  Filter* filters[filterOptions.size()]; // keep track of polymorphic filters
  Filter* prev = &writer; // keep track of previous filter to build chain
  for (size_t i = filterOptions.size(); i-- > 0;) { // traverse options in reverse order
    switch (filterOptions.at(i)[1]) {
      case 'h':
        cout << "-h specified" << endl;
        filters[i] = new HexFilter(prev);
        prev = filters[i];
        break;
      case 'w':
        cout << "-w specified" << endl;
        break;
      default:
        cout << "unknown filter " << filterOptions.at(i) << endl;
        exit(EXIT_FAILURE);
    }
  }
  *in >> noskipws; // turn off white space skipping during input
  Reader reader(prev, in); // activate reader

  // close file streams
	if (in != &cin) {
		delete in;
	}
	if (out != &cout) {
		delete out;
	}

  // clean up filters in heap
  for (auto f : filters) {
    delete f;
  }
}
