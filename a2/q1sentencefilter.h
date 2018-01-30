#ifndef __Q1SENTENCEFILTER_H__
#define __Q1SENTENCEFILTER_H__ 1

#include <string>
#include "q1filter.h"

_Coroutine SentenceFilter : public Filter {
    void main();
  public:
    SentenceFilter(Filter *next);
};

#endif
