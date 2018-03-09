#include "q1tallyvotes.h"
#include "q1printer.h"

TallyVotes::TallyVotes( unsigned int voters, unsigned int group, Printer & printer ) :
 uBarrier(group), num_voters(voters), group_size(group), printer(printer),
 pictureCount(0), statueCount(0), giftShopCount(0) {}

TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ) {
  printer.print(id, Voter::States::Vote, ballot);

  // add votes
  pictureCount += ballot.picture;
  statueCount += ballot.statue;
  giftShopCount += ballot.giftshop;

  uBarrier::block(); // wait for enough voters to vote before continuing

  // determine which tour to attend
  // priority on ties is gift shop, pictures, then statues
  if (giftShopCount >= statueCount && giftShopCount >= pictureCount) { // attend gift shop
    return Tour::GiftShop;
  } else if (pictureCount >= statueCount && pictureCount >= giftShopCount) { // attend picture
    return Tour::Picture;
  } else if (statueCount >= giftShopCount && statueCount >= pictureCount) { // attend statue
    return Tour::Statue;
  }
}

// need to handle F

void TallyVotes::done() {

}
