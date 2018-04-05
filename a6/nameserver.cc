#include "nameserver.h"

#include <iostream>
using namespace std;

NameServer::NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents ) :
  printer(prt),
  numVendingMachines(numVendingMachines),
  numStudents(numStudents) {
    vendingMachines = new VendingMachine*[numVendingMachines];
    machineForStudent = new unsigned int[numStudents];

    // round robin distribution of student to machine
    for (unsigned int i = 0; i < numStudents; ++i) {
      machineForStudent[i] = i % numVendingMachines;
    }
  }

NameServer::~NameServer() {}

void NameServer::VMregister( VendingMachine * vendingmachine ) {
  printer.print(Printer::NameServer, 'R', vendingmachine->getId());

  vendingMachines[vendingmachine->getId()] = vendingmachine;
}

VendingMachine * NameServer::getMachine( unsigned int id ) {
  // get vm corresponding to student id
  unsigned int index = machineForStudent[id];
  VendingMachine *vm = vendingMachines[index];

  printer.print(Printer::NameServer, 'N', id, vm->getId());

  // set next machine student should use
  machineForStudent[id] = (index + 1) % numVendingMachines;

  return vm;
}

VendingMachine ** NameServer::getMachineList() {
  return vendingMachines;
}

void NameServer::main() {
  printer.print(Printer::NameServer, 'S');

  for (unsigned int i = 0; i < numVendingMachines; ++i) {
    _Accept(VMregister); // wait for all vending machines to be registered
  }

  for (;;) {
    _Accept(~NameServer) {
      break;
    } or _Accept(getMachineList, getMachine);
  }

  // clean up
  delete[] vendingMachines;
  delete[] machineForStudent;

  printer.print(Printer::NameServer, 'F');
}
