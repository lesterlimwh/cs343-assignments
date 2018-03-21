#include "q2tallyVotes.h"
#include "q2printer.h"

void TallyVotes::wait() {
	bench.wait(); // wait until signalled
	while ( rand() % 2 == 0 ) { // multiple bargers allowed
		_Accept( vote ) { // accept barging callers
		} Else { // do not wait if no callers
		} // Accept
	} // while
}

void TallyVotes::signalAll() { // also useful
	while ( ! bench.empty() ) bench.signal(); // drain the condition
}

TallyVotes::TallyVotes( unsigned int voters, unsigned int group, Printer & printer ) :
	num_voters(voters),
	group_size(group),
	printer(printer),
	num_waiters(0),
	pictureCount(0),
	statueCount(0),
	giftShopCount(0),
  ticket(0),
  currentTicket(0),
	completedVoters(0) {}

// vote tallier with internal scheduling, simulating Java monitor
TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ) {
	unsigned int myTicket = ticket; // assign ticket
	ticket++; // bump global ticket counter

	while (myTicket > currentTicket) {
    printer.print(id, Voter::States::Barging);
    wait();
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

    signalAll();
  } else { // wait for more voters
    printer.print(id, Voter::States::Block, num_waiters);
    wait();
    printer.print(id, Voter::States::Unblock, num_waiters - 1);
  }

	num_waiters--;

  currentTicket++; // serve next ticket

  return winner;
}

void TallyVotes::done() {
  completedVoters += 1;

  unsigned int remaining_voters = num_voters - completedVoters;
  if (remaining_voters < group_size) {
    // uBarrier::block();
  }
}
