#ifndef __Q1WRITER_H__
#define __Q1WRITER_H__ 1

#include "q1filter.h"
#include <iostream>

_Coroutine Writer : public Filter {
    std::ostream *out;
    int outputCount; // keeps track of number of characters output
    void main();
  public:
    Writer(std::ostream *o);
};

#endif
