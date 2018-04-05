#include "groupoff.h"
#include "MPRNG.h"

#include <iostream>
using namespace std;

extern MPRNG mprng;

Groupoff::Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay ) :
  printer(prt),
  numStudents(numStudents),
  sodaCost(sodaCost),
  delay(groupoffDelay) {}

Groupoff::~Groupoff() {
  for (unsigned int i = 0; i < cards.size(); ++i) {
    delete cards[i];
  }
}

WATCard::FWATCard Groupoff::giftCard() {
  WATCard::FWATCard futureCard;
  futureCards.push_back(futureCard);
  return futureCard;
}

void Groupoff::main() {
  printer.print(Printer::Groupoff, 'S');

  for (unsigned int times = 0; times < numStudents; ++times) {
    _Accept(giftCard);
  }

  for (;;) {
    _Accept(~Groupoff) {
      break;
    } _Else {
      if (futureCards.empty()) break;

      yield(delay);

      // deliver card to random student
      unsigned int random_idx = mprng(futureCards.size() - 1);
      WATCard *card = new WATCard();
      card->deposit(sodaCost);
      cards.push_back(card);

      futureCards[random_idx].delivery(card);
      printer.print(Printer::Groupoff, 'D', sodaCost);

      futureCards.erase(futureCards.begin() + random_idx);
    }
  }

  printer.print(Printer::Groupoff, 'F');
}
