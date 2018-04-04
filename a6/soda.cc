#include <iostream>
#include "config.h"
#include "MPRNG.h"
#include "printer.h"
#include "parent.h"
#include "bank.h"
#include "watcardoffice.h"

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
  Parent parent(printer, bank, config.numStudents, config.parentalDelay);
  WATCardOffice office(printer, bank, config.numCouriers);
  // end tasks
}
