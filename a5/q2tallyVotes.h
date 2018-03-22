#ifndef __Q2TALLYVOTES_H__
#define __Q2TALLYVOTES_H__

_Monitor Printer;

#if defined( EXT ) // external scheduling monitor solution
_Monitor TallyVotes {
  unsigned int num_voters;
  unsigned int group_size;
  Printer &printer;
	unsigned int num_waiters; // counts number of waiters
  unsigned int pictureCount; // counts number of picture votes
  unsigned int statueCount; // counts number of statue votes
  unsigned int giftShopCount; // counts number of gift shop votes
  unsigned int completedVoters; // number of voters who are done

#elif defined( INT ) // internal scheduling monitor solution
_Monitor TallyVotes {
  unsigned int num_voters;
  unsigned int group_size;
  Printer &printer;
	unsigned int num_waiters; // counts number of waiters
  unsigned int pictureCount; // counts number of picture votes
  unsigned int statueCount; // counts number of statue votes
  unsigned int giftShopCount; // counts number of gift shop votes
  unsigned int completedVoters; // number of voters who are done
  uCondition groupComplete; // blocks when group not formed

#elif defined( INTB ) // internal scheduling monitor solution with barging
_Monitor TallyVotes {
	uCondition bench; // only one condition variable (you may change the variable name)
	void wait(); // barging version of wait
	void signalAll(); // unblock all waiting tasks
  unsigned int num_voters;
  unsigned int group_size;
  Printer &printer;
	unsigned int num_waiters; // counts number of waiters
  unsigned int pictureCount; // counts number of picture votes
  unsigned int statueCount; // counts number of statue votes
  unsigned int giftShopCount; // counts number of gift shop votes
  unsigned int completedVoters; // number of voters who are done
  unsigned int ticket; // counts all tickets
  unsigned int currentTicket; // ticket that is being served

#elif defined( AUTO ) // automatic-signal monitor solution
#include "AutomaticSignal.h"
_Monitor TallyVotes {
  unsigned int num_voters;
  unsigned int group_size;
  Printer &printer;
  AUTOMATIC_SIGNAL;
  bool groupComplete; // flag that indicates group is completed
	unsigned int num_waiters; // counts number of waiters
  unsigned int pictureCount; // counts number of picture votes
  unsigned int statueCount; // counts number of statue votes
  unsigned int giftShopCount; // counts number of gift shop votes
  unsigned int completedVoters; // number of voters who are done

#elif defined( TASK ) // internal/external scheduling task solution
_Task TallyVotes {
  uCondition groupComplete; // blocks when group not formed
  unsigned int id; // current id server is working on
  unsigned int num_voters;
  unsigned int group_size;
  Printer &printer;
	unsigned int num_waiters; // counts number of waiters
  unsigned int pictureCount; // counts number of picture votes
  unsigned int statueCount; // counts number of statue votes
  unsigned int giftShopCount; // counts number of gift shop votes
  unsigned int completedVoters; // number of voters who are done
  void main();

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
private:
	Tour winner; // holds winner of vote
  Ballot ballot; // holds current ballot for server task
};

#endif
