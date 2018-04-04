#ifndef BOTTLINGPLANT_H
#define BOTTLINGPLANT_H

#include "printer.h"
#include "vendingmachine.h"
#include "nameserver.h"
#include "truck.h"

_Task Truck;

_Task BottlingPlant {
    static const unsigned int numFlavours = 4; // number of soda flavours
    Printer &printer;
    NameServer &nameServer;
    unsigned int numVendingMachines;
    unsigned int maxShippedPerFlavour;
    unsigned int maxStockPerFlavour;
    unsigned int timeBetweenShipments;
    unsigned int *stock; // stock for each flavour
    bool closed; // true if plant has shut down
    Truck *truck;
    void main();
  public:
    _Event Shutdown {};                       // shutdown plant
    BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
      unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
      unsigned int timeBetweenShipments );
    void getShipment( unsigned int cargo[] );
};

#endif
