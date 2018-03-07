#ifndef __Q1TALLYVOTES_H__
#define __Q1TALLYVOTES_H__

// #include "q1printer.h"
_Monitor Printer;

#if defined( MC ) // mutex/condition solution
// includes for this kind of vote-tallier
class TallyVotes {
  // private declarations for this kind of vote-tallier
#elif defined( SEM ) // semaphore solution
// includes for this kind of vote-tallier
class TallyVotes {
  // private declarations for this kind of vote-tallier
#elif defined( BAR ) // barrier solution
// includes for this kind of vote-tallier
_Cormonitor TallyVotes : public uBarrier {
  // private declarations for this kind of vote-tallier
#else
  #error unsupported voter type
#endif
  // common declarations
  public: // common interface
    TallyVotes( unsigned int voters, unsigned int group, Printer & printer );
    struct Ballot { unsigned int picture, statue, giftshop; };
    enum Tour { Picture = 'p', Statue = 's', GiftShop = 'g', Failed = 'f' };
    Tour vote( unsigned int id, Ballot ballot );
    void done();
};

#endif
