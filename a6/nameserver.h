#ifndef NAMESERVER_H
#define NAMESERVER_H

#include "printer.h"
#include "vendingmachine.h"

_Task VendingMachine; // forward declare VendingMachine

_Task NameServer {
    Printer &printer;
    unsigned int numVendingMachines;
    unsigned int numStudents;
    VendingMachine ** vendingMachines; // list of registered vending machines
    unsigned int * machineForStudent; // maps student id to machine id
    void main();
  public:
    NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents );
    ~NameServer();
    void VMregister( VendingMachine * vendingmachine );
    VendingMachine * getMachine( unsigned int id ); // get specific machine
    VendingMachine ** getMachineList(); // return list of machines
};

#endif
