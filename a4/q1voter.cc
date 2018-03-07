#include "q1voter.h"
#include "q1printer.h"

Voter::Voter( unsigned int id, unsigned int nvotes, TallyVotes & voteTallier, Printer & printer ) : voteTallier(voteTallier), printer(printer), id(id), num_votes(nvotes) {}

void Voter::main() {
  printer.print(id, States::Start);
}

