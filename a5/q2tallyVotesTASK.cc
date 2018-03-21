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

void TallyVotes::main() {
  for (;;) {
    _Accept(~TallyVotes) {
      break; // exit thread when destructor is called
    } or _Accept(vote) {
      printer.print(id, Voter::States::Vote, ballot);

      // add votes
      pictureCount += ballot.picture;
      statueCount += ballot.statue;
      giftShopCount += ballot.giftshop;

      num_waiters++;
      printer.print(id, Voter::States::Block, num_waiters);

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

        while (!groupComplete.empty()) { // unblock all other voters
          num_waiters--;
          printer.print(groupComplete.front(), Voter::States::Unblock, num_waiters);
          groupComplete.signalBlock();
        }
      }
    }
  }
}

// vote tallier with server task
TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ) {
  TallyVotes::id = id;
  TallyVotes::ballot = ballot;

  groupComplete.wait(id); // wait for group to be formed

  return winner;
}

void TallyVotes::done() {
  completedVoters += 1;
}
