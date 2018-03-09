#include "q1tallyvotes.h"
#include "q1printer.h"

TallyVotes::TallyVotes( unsigned int voters, unsigned int group, Printer & printer ) :
 group_sem(0), mutex_sem(1), num_voters(voters), group_size(group), printer(printer),
 pictureCount(0), statueCount(0), giftShopCount(0), completedVoters(0), voteCount(0),
 failed(false) {}

TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ) {
  if (failed) {
    return TallyVotes::Tour::Failed;
  }

  mutex_sem.P(); // begin critical section

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
    printer.print(id, Voter::States::Block, voteCount);
    group_sem.P(mutex_sem); // baton pass
    voteCount--;
    printer.print(id, Voter::States::Unblock, voteCount);
  }

  if (failed) { // check for failure again after unblock
    return TallyVotes::Tour::Failed;
  }

  // determine which tour to attend
  // priority on ties is gift shop, pictures, then statues
  Tour winner;
  if (giftShopCount >= statueCount && giftShopCount >= pictureCount) { // attend gift shop
    winner = Tour::GiftShop;
  } else if (pictureCount >= statueCount && pictureCount >= giftShopCount) { // attend picture
    winner = Tour::Picture;
  } else if (statueCount >= giftShopCount && statueCount >= pictureCount) { // attend statue
    winner = Tour::Statue;
  }

  if (group_sem.empty()) {
    // reset votes
    pictureCount = 0;
    statueCount = 0;
    giftShopCount = 0;

    mutex_sem.V();
  } else {
    group_sem.V();
  }

  return winner;
}

void TallyVotes::done() {
  completedVoters++;

  mutex_sem.P();
  unsigned int remaining_voters = num_voters - completedVoters;
  if (remaining_voters < group_size) {
    failed = true;
    group_sem.V();
  }
  mutex_sem.V();
}
