#ifndef __Q1READER_H__
#define __Q1READER_H__ 1

#include "q1filter.h"
#include <iostream>

_Coroutine Reader : public Filter {
    std::istream *in; // input stream to read from
    void main();
  public:
    Reader(Filter *f, std::istream *i);
};

#endif
