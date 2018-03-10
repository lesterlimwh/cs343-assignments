#include "q1tallyvotes.h"
#include "q1printer.h"

TallyVotes::TallyVotes( unsigned int voters, unsigned int group, Printer & printer ) :
 num_voters(voters), group_size(group), printer(printer), completedVoters(0),
 pictureCount(0), statueCount(0), giftShopCount(0), voteCount(0),
 failed(false), barge_signal(false) {}

TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ) {
  if (failed) {
    return TallyVotes::Tour::Failed;
  }

  mutex_lock.acquire();

  // handle barging
  if (barge_signal) {
    printer.print(id, Voter::States::Barging);
    barge_lock.wait(mutex_lock);
    barge_signal = false;
  }

	printer.print(id, Voter::States::Vote, ballot);

  // add votes
  voteCount++;
  pictureCount += ballot.picture;
  statueCount += ballot.statue;
  giftShopCount += ballot.giftshop;

	if (voteCount == group_size) {
    barge_signal = true;
    printer.print(id, Voter::States::Complete);
    group_lock.broadcast(); // unblock other voters

    if (failed) {
      winner = TallyVotes::Tour::Failed;
      return TallyVotes::Tour::Failed;
    }

    // determine which tour to attend
    // priority on ties is gift shop, pictures, then statues
    if (giftShopCount >= statueCount && giftShopCount >= pictureCount) { // attend gift shop
      winner = Tour::GiftShop;
    } else if (pictureCount >= statueCount && pictureCount >= giftShopCount) { // attend picture
      winner = Tour::Picture;
    } else if (statueCount >= giftShopCount && statueCount >= pictureCount) { // attend statue
      winner = Tour::Statue;
    }
    
    // reset votes
    voteCount = 0;
    pictureCount = 0;
    statueCount = 0;
    giftShopCount = 0;
    
    barge_signal = false;
    if (!barge_lock.empty()) {
      barge_signal = true;
      barge_lock.signal();
    }
  } else {
    printer.print(id, Voter::States::Block, voteCount);

    if (!barge_lock.empty()) {
      barge_signal = true;
      barge_lock.signal();
    }

    group_lock.wait(mutex_lock);

    printer.print(id, Voter::States::Unblock, voteCount);
  }

  mutex_lock.release();

	return winner;
}

void TallyVotes::done() {
  completedVoters++;

  mutex_lock.acquire();
  unsigned int remaining_voters = num_voters - completedVoters;
  if (remaining_voters < group_size) {
    failed = true;
    group_lock.broadcast();
  }

  mutex_lock.release();

}
