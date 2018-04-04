#ifndef VENDINGMACHINE_H
#define VENDINGMACHINE_H

#include "printer.h"
#include "nameserver.h"
#include "watcard.h"

_Task NameServer;

_Task VendingMachine {
    static const unsigned int numFlavours = 4; // number of flavours
    Printer &printer;
    NameServer &nameServer;
    unsigned int id;
    unsigned int sodaCost;
    unsigned int maxStockPerFlavour;
    unsigned int stocks[numFlavours]; // contains amount of each type of soda
    bool restocking; // indicates truck is restocking machine
    void main();
  public:
    enum Flavours { Lime, BubbleGum, Cream, Orange }; // flavours of soda
    _Event Free {};                           // free, advertisement
    _Event Funds {};                          // insufficient funds
    _Event Stock {};                          // out of stock for particular flavour
    VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost, unsigned int maxStockPerFlavour );
    void buy( Flavours flavour, WATCard & card );
    unsigned int * inventory();
    void restocked();
    _Nomutex unsigned int cost();
    _Nomutex unsigned int getId();
};

#endif
