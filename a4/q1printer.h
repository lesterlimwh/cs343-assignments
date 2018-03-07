#ifndef __Q1PRINTER_H__
#define __Q1PRINTER_H__

#include "q1voter.h"
#include "q1tallyvotes.h"

_Monitor Printer {

  public:
    Printer( unsigned int voters );
    void print( unsigned int id, Voter::States state );
    void print( unsigned int id, Voter::States state, TallyVotes::Tour tour );
    void print( unsigned int id, Voter::States state, TallyVotes::Ballot ballot );
    void print( unsigned int id, Voter::States state, unsigned int numBlocked );
};

#endif
