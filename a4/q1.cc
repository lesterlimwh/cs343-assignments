#include <iostream>
#include "MPRNG.h"
#include "q1printer.h"
#include "q1tallyvotes.h"
#include "q1voter.h"

using namespace std;

MPRNG mprng; // globally defined mprng

// prints usage information
void usage(char *arg) {
	cerr << "Usage: " << arg
		<< " [ voters (> 0 & voters mod group = 0) | 'x' (default 6)"
		<< " [ group (> 0) | 'x' (default 3) [ votes (> 0) | 'x' (default 1)"
		<< " [ seed (> 0), | 'x' (default random) ] ] ] ]	" << endl;
  exit(EXIT_FAILURE);
}

int tryParseStringToNumber(char* arg) {
  char* endptr;

  long val = strtol(arg, &endptr, 10);

  if (*endptr == 0) { // successfully parsed string
    return int(val);
  } else {
    return -1; // will never get here, but gets rid of compiler warning
  }
}

int main( int argc, char *argv[] ) {
  // initialize defaults
  int num_voters = 6;
  int group_size = 3;
  int num_votes = 1;
  int seed = getpid();

  // parse command line arguments
  switch (argc) {
    case 5: // read seed
      if (argv[4][0] != 'x') {
        seed = tryParseStringToNumber(argv[4]);
        if (seed < 0) {
          usage(argv[0]);
        }
      }
    case 4:
      if (argv[3][0] != 'x') {
        num_votes = tryParseStringToNumber(argv[3]);
        if (num_votes < 0) {
          usage(argv[0]);
        }
      }
    case 3:
      if (argv[2][0] != 'x') {
        group_size = tryParseStringToNumber(argv[2]);
        if (group_size < 0) {
          usage(argv[0]);
        }
      }
    case 2:
      if (argv[1][0] != 'x') {
        num_voters = tryParseStringToNumber(argv[1]);
        if (num_voters < 0 || num_voters % group_size != 0) {
          usage(argv[0]);
        }
      }
    case 1:
      break;
    default:
      usage(argv[0]);
  }

  mprng.set_seed(seed); // set seed

  // begin voting simulation
  Printer printer(num_voters);
  Voter* voters[num_voters];
  TallyVotes voteTallier(num_voters, group_size, printer);

  for (int i = 0; i < num_voters; ++i) {
    voters[i] = new Voter(i, num_votes, voteTallier, printer);
  }

  cout << "========" << endl;

  for (int i = 0; i < num_voters; ++i) {
    delete voters[i];
  }
}
