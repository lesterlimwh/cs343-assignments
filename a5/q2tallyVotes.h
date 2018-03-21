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
// includes for this kind of vote-tallier
_Monitor TallyVotes {
	// private declarations for this kind of vote-tallier
	uCondition bench; // only one condition variable (you may change the variable name)
	void wait(); // barging version of wait
	void signalAll(); // unblock all waiting tasks

#elif defined( AUTO ) // automatic-signal monitor solution
// includes for this kind of vote-tallier
_Monitor TallyVotes {

#elif defined( TASK ) // internal/external scheduling task solution
_Task TallyVotes {

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
};

#endif
