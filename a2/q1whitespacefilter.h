#ifndef __Q1WHITESPACEFILTER_H__
#define __Q1WHITESPACEFILTER_H__ 1

#include <string>
#include "q1filter.h"

_Coroutine WhitespaceFilter : public Filter {
    void main();
  public:
    WhitespaceFilter(Filter *next);
};

#endif
