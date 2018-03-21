#ifndef __Q2VOTER_H__
#define __Q2VOTER_H__

#include "MPRNG.h"
#include "q2tallyVotes.h"

_Monitor Printer; // forward declare or else circular dependency

extern MPRNG mprng;

_Task Voter {
  TallyVotes::Ballot cast() { // cast 3-way vote
    // O(1) random selection of 3 items without replacement using divide and conquer.
    static unsigned int voting[3][2][2] = { { {2,1}, {1,2} }, { {0,2}, {2,0} }, { {0,1}, {1,0} } };
    unsigned int picture = mprng( 2 ), statue = mprng( 1 );
    return (TallyVotes::Ballot){ picture, voting[picture][statue][0], voting[picture][statue][1] };
  }
  unsigned int id;
  unsigned int num_votes;
  TallyVotes &voteTallier;
  Printer &printer;
  void main();
public:
  enum States { Start = 'S', Vote = 'V', Block = 'B', Unblock = 'U', Barging = 'b',
               Complete = 'C', Finished = 'F', Failed = 'X' };
  Voter( unsigned int id, unsigned int nvotes, TallyVotes & voteTallier, Printer & printer );
};

#endif
