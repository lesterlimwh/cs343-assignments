#ifndef TRUCK_H
#define TRUCK_H

#include "bottlingplant.h"
#include "nameserver.h"
#include "vendingmachine.h"

_Task BottlingPlant;

_Task Truck {
    static const unsigned int numFlavours = 4; // number of soda flavours
    Printer &printer;
    NameServer &nameServer;
    BottlingPlant &plant;
    unsigned int numVendingMachines;
    unsigned int maxStockPerFlavour;
    void main();
  public:
    Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
      unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
};

#endif
