#include "q1voter.h"
#include "q1printer.h"

Voter::Voter( unsigned int id, unsigned int nvotes, TallyVotes & voteTallier, Printer & printer ) : voteTallier(voteTallier), printer(printer), id(id), num_votes(nvotes) {}

void Voter::main() {
  yield(mprng(19)); // yield 0 - 19 times inclusive

  for (unsigned int i = 0; i < num_votes; ++i) {
    printer.print(id, States::Start);

    yield(mprng(4)); // yield 0 - 4 times inclusive

    TallyVotes::Ballot ballot = cast(); // cast vote onto ballot 
    TallyVotes::Tour tour = voteTallier.vote(id, ballot); // submit vote for tallying

    yield(mprng(4)); // yield 0 - 4 times inclusive

    printer.print(id, States::Finished, tour);
  }
}

