#ifndef __Q1HEXFILTER_H__
#define __Q1HEXFILTER_H__ 1

#include <string>
#include "q1filter.h"

_Coroutine HexFilter : public Filter {
    std::string charToHex(char ch);
    void handleTwoChars();
    void main();
  public:
    HexFilter(Filter *next);
};

#endif
