#include <algorithm>
#include "truck.h"
#include "MPRNG.h"

#include <iostream>
using namespace std;

extern MPRNG mprng;

Truck::Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant, unsigned int numVendingMachines, unsigned int maxStockPerFlavour ) :
  printer(prt),
  nameServer(nameServer),
  plant(plant),
  numVendingMachines(numVendingMachines),
  maxStockPerFlavour(maxStockPerFlavour) {}

void Truck::main() {
  printer.print(Printer::Truck, 'S');

  VendingMachine **machines = nameServer.getMachineList();
  unsigned int lastMachineIndex = -1; // last machine the truck restocked
  unsigned int cargo[numFlavours]; // cargo for shipment

  for (;;) {
    yield(mprng(1, 10)); // get coffee

    // obtain a new shipment
    try {
      plant.getShipment(cargo);
    } catch (BottlingPlant::Shutdown) {
      break;
    }

    unsigned int total = 0; // keep track of total sodas
    for (unsigned int i = 0; i < numFlavours; ++i) {
      total += cargo[i];
    }

    printer.print(Printer::Truck, 'P', total);

    // restock vending machine in cyclic order
    for (unsigned int i = 0; i < numVendingMachines; ++i) {
      if (total == 0) break; // stop restocking if out of cargo

      lastMachineIndex = (lastMachineIndex + 1) % numVendingMachines;
      unsigned int *inventory = machines[lastMachineIndex]->inventory(); // get machine's inventory

      printer.print(Printer::Truck, 'd', lastMachineIndex, total); // begin delivery

      // perform restock
      unsigned int missing = 0; // keep track of unreplenished bottles
      for (unsigned int f = 0; f <numFlavours; ++f) {
        unsigned int numRestocked = min(maxStockPerFlavour - inventory[f], cargo[f]);
        inventory[f] += numRestocked;
        cargo[f] -= numRestocked;
        total -= numRestocked;
        missing += ( maxStockPerFlavour - inventory[f] );
      }

      machines[lastMachineIndex]->restocked(); // indicate restock completed

      if (missing > 0) { // print machines that are not fully stocked
        printer.print(Printer::Truck, 'U', lastMachineIndex, missing);
      }

      printer.print(Printer::Truck, 'D', lastMachineIndex, total); // complete delivery
    }
  }

  printer.print(Printer::Truck, 'F');
}
