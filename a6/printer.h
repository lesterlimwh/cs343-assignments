#ifndef PRINTER_H
#define PRINTER_H

#include <string>

class State;

_Monitor Printer {
    unsigned int numStudents;
    unsigned int numVendingMachines;
    unsigned int numCouriers;
    State **buffer; // holds internal reresentations for state of all the voters
    void flush(); // flush contents of buffer to stdout
  public:
    enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
    Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
    ~Printer();
    void print( Kind kind, char state );
    void print( Kind kind, char state, int value1 );
    void print( Kind kind, char state, int value1, int value2 );
    void print( Kind kind, unsigned int lid, char state );
    void print( Kind kind, unsigned int lid, char state, int value1 );
    void print( Kind kind, unsigned int lid, char state, int value1, int value2 );
  private:
    unsigned int getBufferIndex( Kind kind, unsigned int lid = 0 );
};

// internal representation of state for buffer
class State {
    Printer::Kind kind;
    char state;
    int value1;
    int value2;
    unsigned int lid;
  public:
    State( Printer::Kind kind, char state, int value1 = -1, int value2 = -1);
    State( Printer::Kind kind, unsigned int lid, char state, int value1 = -1, int value2 = -1 );
    std::string format(); // formats state into a string for printing based on Kind and state
};

#endif
