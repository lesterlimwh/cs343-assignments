#ifndef __Q1HEXFILTER_H__
#define __Q1HEXFILTER_H__ 1

#include "q1filter.h"

_Coroutine HexFilter : public Filter {
    void main();
  public:
    HexFilter(Filter *next);
};

#endif
