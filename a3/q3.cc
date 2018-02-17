#include <iostream>
#include "MPRNG.h"
#include "q3buffer.h"
#include "q3cons.h"
#include "q3prod.h"

using namespace std;

MPRNG mprng; // globally defined mprng

// prints usage information
void usage(char *arg) {
  cerr << "Usage: " << arg
    << " [ Cons (> 0) [ Prods (> 0) [ Produce (> 0)"
    << " [ BufferSize (> 0) [ Delay (> 0) ] ] ] ] ]" << endl;
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
  // initialize default options
  int cons = 5;
  int prods = 3;
  int produce = 10;
  int bufferSize = 10;
  bool didSetDelay = false;
  int delays;

  int sum = 0; // accumulates consumed values
  queue<Producer*> prodQueue;
  queue<Consumer*> consQueue;

  // parse command line arguments
  switch (argc) {
    case 6:
			delays = tryParseStringToNumber(argv[5]);
      didSetDelay = true;
			if (delays < 1) {
        usage(argv[0]);
			}
    case 5:
      bufferSize = tryParseStringToNumber(argv[4]);
			if (bufferSize < 1) {
        usage(argv[0]);
			}
    case 4:
      produce = tryParseStringToNumber(argv[3]);
      if (produce < 1) {
        usage(argv[0]);
      }
    case 3:
      prods = tryParseStringToNumber(argv[2]);
      if (prods < 1) {
        usage(argv[0]);
      }
    case 2:
      cons = tryParseStringToNumber(argv[1]);
      if (cons < 1) {
        usage(argv[0]);
      }
  }

  if (!didSetDelay) {
    delays = cons + prods; // calculate default delays if not explicitly set
  }

#ifdef __U_MULTI__
  uProcessor p[3] __attribute__((unused));  // create 3 kernel thread for a total of 4
#endif

  // initialize buffer, producers and consumers
  BoundedBuffer<int> buffer(bufferSize);
  for (int i = 0; i < prods; ++i) {
    prodQueue.push(new Producer(buffer, produce, delays));
  }
  for (int i = 0; i < cons; ++i) {
    consQueue.push(new Consumer(buffer, delays, SENTINEL, sum));
  }

  // wait for the producers to finish
  while (!prodQueue.empty()) {
    delete prodQueue.front();
    prodQueue.pop();
  }

  // stop consumers
  for (int i = 0; i < cons; ++i) {
    buffer.insert(SENTINEL);
  }

  // clean up consumers
  while (!consQueue.empty()) {
    delete consQueue.front();
    consQueue.pop();
  }

  // output sum
  cout << "total: " << sum << endl;
}
