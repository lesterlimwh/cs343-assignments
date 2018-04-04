#ifndef STUDENT_H
#define STUDENT_H

#include "printer.h"
#include "nameserver.h"
#include "vendingmachine.h"
#include "watcard.h"
#include "bank.h"
#include "groupoff.h"
#include "watcardoffice.h"
#include "groupoff.h"

_Task Student {
    static const unsigned int numFlavours = 4;
    Printer &printer;
    NameServer &nameServer;
    WATCardOffice &office;
    Groupoff &groupoff;
    unsigned int id;
    unsigned int maxPurchases;
    void main();
  public:
    Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff, unsigned int id, unsigned int maxPurchases );
};

#endif
