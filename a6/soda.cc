#include <iostream>
#include "config.h"
#include "MPRNG.h"
#include "printer.h"
#include "bank.h"

using namespace std;

MPRNG mprng; // globally defined mprng

// prints usage information
void usage(char *arg) {
  cerr << "Usage: " << arg
		<< " [ config-file [ random-seed (> 0) ] ]" << endl;
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
  string configFile = "soda.config";
  int seed = getpid();

  // read in and parse args
  switch (argc) {
    case 3:
			seed = tryParseStringToNumber(argv[2]);
			if (seed < 0) {
				usage(argv[0]);
			}
    case 2:
      configFile = argv[1];
    case 1:
			break;
    default:
			usage(argv[0]);
  }

	mprng.set_seed(seed);

	// parse config file
	ConfigParms config;
	processConfigFile(configFile.c_str(), config);

	// start tasks
	Printer printer(config.numStudents, config.numVendingMachines, config.numCouriers);
  Bank bank(config.numStudents);
  printer.print(Printer::Parent, 'S');
  printer.print(Printer::Parent, 'D', 0, 1);
  printer.print(Printer::Groupoff, 'D', 2);
  printer.print(Printer::Student, 0, 'S', 2, 4);
  printer.print(Printer::Student, 1, 'V', 1);
  printer.print(Printer::Student, 1, 'F');
  bank.deposit(0, 20);
  bank.withdraw(0, 20); 
  


	// end tasks
}
