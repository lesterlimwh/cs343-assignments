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
  }

	printer.print(id, Voter::States::Vote, ballot);

  // add votes
  voteCount++;
  pictureCount += ballot.picture;
  statueCount += ballot.statue;
  giftShopCount += ballot.giftshop;

	if (voteCount == group_size) {
    printer.print(id, Voter::States::Complete);
    voteCount--;
  } else {
    if (barge_lock.empty()) {
      barge_signal = false;
    } else {
      barge_signal = true;
      barge_lock.signal();
    }

    printer.print(id, Voter::States::Block, voteCount);

    group_lock.wait(mutex_lock);

    voteCount--;

    printer.print(id, Voter::States::Unblock, voteCount);
  }

  Tour winner;
  if (failed) {
    return TallyVotes::Tour::Failed;
  } else {
    // determine which tour to attend
    // priority on ties is gift shop, pictures, then statues
    if (giftShopCount >= statueCount && giftShopCount >= pictureCount) { // attend gift shop
      winner = Tour::GiftShop;
    } else if (pictureCount >= statueCount && pictureCount >= giftShopCount) { // attend picture
      winner = Tour::Picture;
    } else if (statueCount >= giftShopCount && statueCount >= pictureCount) { // attend statue
      winner = Tour::Statue;
    }
  }

  if (!group_lock.empty()) {
    barge_signal = true;
    group_lock.signal();
  } else if (!barge_lock.empty()) {
    barge_signal = true;
    barge_lock.signal();
    // reset votes
    pictureCount = 0;
    statueCount = 0;
    giftShopCount = 0;
  } else {
    barge_signal = false;
    // reset votes
    pictureCount = 0;
    statueCount = 0;
    giftShopCount = 0;
  }

  mutex_lock.release();

	return winner;
}

void TallyVotes::done() {
  mutex_lock.acquire();
  completedVoters++;
  unsigned int remaining_voters = num_voters - completedVoters;
  if (remaining_voters < group_size) {
    if (barge_signal) {
      barge_lock.wait(mutex_lock);
    }
    failed = true;
    barge_lock.broadcast();
    group_lock.broadcast();
  }

  mutex_lock.release();
}
