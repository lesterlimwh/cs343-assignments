#ifndef __Q1WRITER_H__
#define __Q1WRITER_H__ 1

#include "q1filter.h"
#include <iostream>

_Coroutine Writer : public Filter {
    void main();
  public:
    Writer(std::ostream *o);
};

#endif
