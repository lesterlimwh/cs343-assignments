#ifndef __Q1TALLYVOTES_H__
#define __Q1TALLYVOTES_H__

_Monitor Printer;

#if defined( MC ) // mutex/condition solution
class TallyVotes {
  unsigned int num_voters;
  unsigned int group_size;
  Printer &printer;
  unsigned int pictureCount; // counts number of picture votes
  unsigned int statueCount; // counts number of statue votes
  unsigned int giftShopCount; // counts number of gift shop votes
  unsigned int voteCount; // counts number of votes
  unsigned int completedVoters;
  uOwnerLock mutex_lock;
  uCondLock group_lock;
  uCondLock barge_lock;
  bool barge_signal; // signals barging
  bool failed;

#elif defined( SEM ) // semaphore solution
#include <uSemaphore.h>
class TallyVotes {
  unsigned int num_voters;
  unsigned int group_size;
  Printer &printer;
  unsigned int pictureCount; // counts number of picture votes
  unsigned int statueCount; // counts number of statue votes
  unsigned int giftShopCount; // counts number of gift shop votes
  unsigned int voteCount; // counts number of votes
  unsigned int completedVoters; // number of voters who are done
  uSemaphore group_sem;
  uSemaphore mutex_sem;
  bool failed; // indicates that not enough remaining voters to form a group

#elif defined( BAR ) // barrier solution
#include <uBarrier.h>
_Cormonitor TallyVotes : public uBarrier {
  unsigned int num_voters;
  unsigned int group_size;
  Printer &printer;
  unsigned int pictureCount; // counts number of picture votes
  unsigned int statueCount; // counts number of statue votes
  unsigned int giftShopCount; // counts number of gift shop votes
  unsigned int completedVoters; // number of voters who are done
  unsigned int num_winner_calculations; // number of times winner was calculated

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
