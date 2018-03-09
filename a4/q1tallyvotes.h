#ifndef __Q1TALLYVOTES_H__
#define __Q1TALLYVOTES_H__

_Monitor Printer;

#if defined( MC ) // mutex/condition solution
class TallyVotes {
  unsigned int num_voters;
  unsigned int group_size;
  Printer &printer;
#elif defined( SEM ) // semaphore solution
class TallyVotes {
#elif defined( BAR ) // barrier solution
#include <uBarrier.h>
_Cormonitor TallyVotes : public uBarrier {
  unsigned int num_voters;
  unsigned int group_size;
  Printer &printer;
  unsigned int pictureCount;
  unsigned int statueCount;
  unsigned int giftShopCount;
  unsigned int completedVoters;
  unsigned int num_winner_calculations;
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
