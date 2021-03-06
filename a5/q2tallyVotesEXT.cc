#include "q2tallyVotes.h"
#include "q2printer.h"

TallyVotes::TallyVotes( unsigned int voters, unsigned int group, Printer & printer ) :
	num_voters(voters),
	group_size(group),
	printer(printer),
	num_waiters(0),
	pictureCount(0),
	statueCount(0),
	giftShopCount(0),
	completedVoters(0) {}

// vote tallier with external scheduling
TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ) {
  // force waiting voters to unblock
  unsigned int remaining_voters = num_voters - completedVoters;
  if (remaining_voters < group_size) {
    return TallyVotes::Tour::Failed;
  }

  printer.print(id, Voter::States::Vote, ballot);

  // add votes
  pictureCount += ballot.picture;
  statueCount += ballot.statue;
  giftShopCount += ballot.giftshop;

	num_waiters++;

  if (num_waiters == group_size) { // enough voters to form group
    printer.print(id, Voter::States::Complete);

		// determine which tour to attend
		// priority on ties is gift shop, pictures, then statues
		if (giftShopCount >= statueCount && giftShopCount >= pictureCount) { // attend gift shop
			winner = Tour::GiftShop;
		} else if (pictureCount >= statueCount && pictureCount >= giftShopCount) { // attend picture
			winner = Tour::Picture;
		} else if (statueCount >= giftShopCount && statueCount >= pictureCount) { // attend statue
			winner = Tour::Statue;
		}

    // reset counters
    pictureCount = 0;
    statueCount = 0;
    giftShopCount = 0;

  } else { // wait for more voters
    printer.print(id, Voter::States::Block, num_waiters);

    for (;;) {
      _Accept(vote) {
        break;
      } or _Accept(done) { // if done is received, check if error, but we still need to wait for vote()
        unsigned int remaining_voters = num_voters - completedVoters;
        if (remaining_voters < group_size) {
          return TallyVotes::Tour::Failed;
        }
      }
    }

    remaining_voters = num_voters - completedVoters;
    if (remaining_voters < group_size) {
      return TallyVotes::Tour::Failed;
    }

    printer.print(id, Voter::States::Unblock, num_waiters - 1);
  }

	num_waiters--;

  return winner;
}

void TallyVotes::done() {
  completedVoters += 1;
}
