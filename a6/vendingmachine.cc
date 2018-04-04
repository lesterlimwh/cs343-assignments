#include "vendingmachine.h"
#include "MPRNG.h"

extern MPRNG mprng;

VendingMachine::VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost, unsigned int maxStockPerFlavour ) :
  printer(prt),
  nameServer(nameServer),
  id(id),
  sodaCost(sodaCost),
  maxStockPerFlavour(maxStockPerFlavour),
  restocking(false) {
    for (unsigned int i = 0; i < numFlavours; ++i) {
      stocks[i] = 0; // new vending machine is empty
    }

    nameServer.VMregister(this); // register with name server
  }

void VendingMachine::buy( Flavours flavour, WATCard & card ) {
  if (card.getBalance() < sodaCost) { // not enough money
    _Throw Funds();
  }

  if (mprng(4) == 0) { // 1/5 chance of free soda
    stocks[(unsigned int)flavour]--;
    _Throw Free();
  }

  if (stocks[(unsigned int)flavour] == 0) { // out of stock
    _Throw Stock();
  }

  // perform transaction
  card.withdraw(sodaCost);
  stocks[(unsigned int)flavour]--;

  printer.print(Printer::Vending, id, 'B', (unsigned int)flavour, stocks[(unsigned int)flavour]);
}

unsigned int * VendingMachine::inventory() {
  printer.print(Printer::Vending, id, 'r'); // only called when truck reloading
  return stocks;
}

void VendingMachine::restocked() {
  printer.print(Printer::Vending, id, 'R');
}

_Nomutex unsigned int VendingMachine::cost() {
  return sodaCost;
}

_Nomutex unsigned int VendingMachine::getId() {
  return id;
}

void VendingMachine::main() {
  printer.print(Printer::Vending, id, 'S', sodaCost);

  for (;;) {
    try {
      _Accept(~VendingMachine) {
        break;
      } or _Accept(restocked) {
        restocking = false;
      } or _Accept(inventory) {
        restocking = true;
      } or _When (!restocking) _Accept(buy); 
    } catch (uMutexFailure::RendezvousFailure) {}
  }

  printer.print(Printer::Vending, id, 'F');
}
