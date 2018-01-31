#include "q2.h"

using namespace std;

extern PRNG prng; // global declared in q2.cc

Potato::Potato(unsigned int maxTicks) {
  reset();
}

void Potato::reset(unsigned int maxTicks) {
  currentTick = 0;
  this->maxTicks = prng(1, maxTicks);
  cout << "  POTATO goes off after " << this->maxTicks
    << (this->maxTicks == 1 ? " toss" :  " tosses") << endl;
}

void Potato::countdown() {
  currentTick += 1;
  if (currentTick == maxTicks) {
    _Throw Explode();
  }
}
