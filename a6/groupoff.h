#ifndef GROUPOFF_H
#define GROUPOFF_H

#include <vector>
#include "printer.h"
#include "watcard.h"

_Task Groupoff {
    Printer &printer;
    unsigned int numStudents;
    unsigned int sodaCost;
    unsigned int delay; // groupoff delay
    std::vector<WATCard::FWATCard> futureCards; // gift cards
    std::vector<WATCard*> cards; // keep track of cards
    void main();
  public:
    Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
    ~Groupoff();
    WATCard::FWATCard giftCard();
};

#endif
