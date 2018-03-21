#include "q2voter.h"
#include "q2printer.h"

Voter::Voter( unsigned int id, unsigned int nvotes, TallyVotes & voteTallier, Printer & printer ) : voteTallier(voteTallier), printer(printer), id(id), num_votes(nvotes) {}

void Voter::main() {
  yield(mprng(19)); // yield 0 - 19 times inclusive

  for (unsigned int i = 0; i < num_votes; ++i) {
    printer.print(id, States::Start);

    yield(mprng(4)); // yield 0 - 4 times inclusive

    TallyVotes::Ballot ballot = cast(); // cast vote onto ballot 
    TallyVotes::Tour tour = voteTallier.vote(id, ballot); // submit vote for tallying

    // exit on failure to complete all tours
    if (tour == TallyVotes::Tour::Failed) {
      printer.print(id, States::Failed);
      return;
    }

    yield(mprng(4)); // yield 0 - 4 times inclusive

    printer.print(id, States::Finished, tour);
  }

  voteTallier.done();
}

