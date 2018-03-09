#include "q1tallyvotes.h"
#include "q1printer.h"

TallyVotes::TallyVotes( unsigned int voters, unsigned int group, Printer & printer ) :
 uBarrier(group), num_voters(voters), group_size(group), printer(printer),
 pictureCount(0), statueCount(0), giftShopCount(0), completedVoters(0),
 num_winner_calculations(0) {}

TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ) {
  printer.print(id, Voter::States::Vote, ballot);

  // force waiting voters to unblock
  unsigned int remaining_voters = num_voters - completedVoters;
  if (remaining_voters < group_size) {
    uBarrier::block();
    return TallyVotes::Tour::Failed;
  }

  // add votes
  pictureCount += ballot.picture;
  statueCount += ballot.statue;
  giftShopCount += ballot.giftshop;

  // wait for enough voters to vote before continuing
  if (uBarrier::waiters() + 1 != group_size) {
    printer.print(id, Voter::States::Block, uBarrier::waiters() + 1);
    uBarrier::block();
    printer.print(id, Voter::States::Unblock, uBarrier::waiters());
  } else {
    uBarrier::block();
    printer.print(id, Voter::States::Complete);
  }

  remaining_voters = num_voters - completedVoters;
  if (remaining_voters < group_size) {
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

  num_winner_calculations++;

  // reset counters after winner is calculated by all threads
  if (num_winner_calculations == group_size) {
    pictureCount = 0;
    statueCount = 0;
    giftShopCount = 0;
    num_winner_calculations = 0;
  }

  return winner;
}

void TallyVotes::done() {
  completedVoters += 1;

  unsigned int remaining_voters = num_voters - completedVoters;
  if (remaining_voters < group_size) {
    uBarrier::block();
  }
}
