#ifndef __Q1HEXFILTER_H__
#define __Q1HEXFILTER_H__ 1

#include <string>
#include "q1filter.h"

_Coroutine HexFilter : public Filter {
    std::string charToHex(char ch); // converts char to a length 2 string of its hex representation
    void handleTwoChars(); // helper that accepts 2 characters and puts the hex rep. to the next filter
    void main();
  public:
    HexFilter(Filter *next);
};

#endif
