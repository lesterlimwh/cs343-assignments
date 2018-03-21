#include "q2tallyVotes.h"
#include "q2printer.h"

TallyVotes::TallyVotes( unsigned int voters, unsigned int group, Printer & printer ) :
	num_voters(voters),
	group_size(group),
	printer(printer),
	num_waiters(0),
  groupComplete(false),
	pictureCount(0),
	statueCount(0),
	giftShopCount(0),
	completedVoters(0) {}

// vote tallier simulating general automatic-signal monitor 
TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ) {

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

    groupComplete = true;
  } else { // wait for more voters
    printer.print(id, Voter::States::Block, num_waiters);
    WAITUNTIL(groupComplete,,);
    printer.print(id, Voter::States::Block, num_waiters - 1);
  }

  num_waiters--;
  if (num_waiters == 0) { // last voter of this group resets flag for next group
    groupComplete = false;
  }

  RETURN(winner);
}

void TallyVotes::done() {
  completedVoters += 1;

  unsigned int remaining_voters = num_voters - completedVoters;
  if (remaining_voters < group_size) {
    // uBarrier::block();
  }
}
