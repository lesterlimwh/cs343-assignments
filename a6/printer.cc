#include "printer.h"
#include <iostream>

using namespace std;

State::State( Printer::Kind kind, char state, int value1, int value2 ) :
  kind(kind), state(state), value1(value1), value2(value2) {}

State::State( Printer::Kind kind, unsigned int lid, char state, int value1, int value2 ) :
  kind(kind), lid(lid), state(state), value1(value1), value2(value2) {}

std::string State::format() {
  // if values are not negative, use them
  if (value1 != -1 && value2 != -1) {
    return string(1, state) + to_string(value1) + "," + to_string(value2);
  } else if (value1 != -1) {
    return string(1, state) + to_string(value1);
  } else {
    return string(1, state);
  }
}

Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers ) :
	numStudents(numStudents),
	numVendingMachines(numVendingMachines),
	numCouriers(numCouriers)
{
  buffer = new State*[numStudents + numVendingMachines + numCouriers + 6]; // 1 buffer slot for each student, vending machine, courier, and other entities
  for (unsigned int i = 0; i < numStudents + numVendingMachines + numCouriers + 6; ++i) {
    buffer[i] = nullptr; // initialize to nullptr
  }

  // output header
	cout << "Parent" << '\t';
	cout << "Gropoff" << '\t';
	cout << "WATOff" << '\t';
	cout << "Names" << '\t';
	cout << "Truck" << '\t';
	cout << "Plant" << '\t';

  for (unsigned int i = 0; i < numStudents; ++i) {
    cout << "Stud" << i << '\t';
  }

  for (unsigned int i = 0; i < numVendingMachines; ++i) {
    cout << "Mach" << i << '\t';
  }

  for (unsigned int i = 0; i < numCouriers; ++i) {
    cout << "Cour" << i;

		if (i != numCouriers - 1) {
			cout << '\t';
		}
  }

  cout << endl;

  cout << "*******";
  for (unsigned int i = 1; i < numStudents + numVendingMachines + numCouriers + 6; ++i) {
    cout << "\t*******";
  }
  cout << endl;
}

Printer::~Printer() {
  flush();
  for (unsigned int i = 0; i < numStudents + numVendingMachines + numCouriers + 6; ++i) {
    delete buffer[i];
  }
  delete[] buffer;
	cout << "***********************" << endl;
}

void Printer::flush() {
  // figure out index to stop printing at
  unsigned int highest = 0;
	unsigned int lastBufferIndex = numStudents + numVendingMachines + numCouriers + 6;
  for (unsigned int id = lastBufferIndex; id > 0; --id) {
    if (buffer[id - 1] != nullptr) {
      highest = id - 1;
      break;
    }
  }

  if (buffer[0] != nullptr) {
    cout << buffer[0]->format();
    // clear buffer
    delete buffer[0];
    buffer[0] = nullptr;
  }
  for (unsigned int id = 1; id <= highest; ++id) { // output contents of all buffer voters
    if (buffer[id] != nullptr) {
      cout << "\t" << buffer[id]->format();

      // clear buffer
      delete buffer[id];
      buffer[id] = nullptr;
    } else if (id != highest) {
      cout << "\t";
    }
  }

  cout << endl;
}

unsigned int Printer::getBufferIndex( Kind kind, unsigned int lid ) {
  if (kind != Student && kind != Vending && kind != Courier) {
    return (int) kind;
  }

  switch (kind) {
    case Student:
      return 6 + lid;
    case Vending:
      return 6 + numStudents + lid;
    case Courier:
      return 6 + numStudents + numVendingMachines + lid;
  }
}

void Printer::print( Kind kind, char state ) {
  unsigned int idx = getBufferIndex(kind);
  if (buffer[idx] != nullptr) {
    flush();
  }

	buffer[idx] = new State(kind, state);
}

void Printer::print( Kind kind, char state, int value1 ) {
  unsigned int idx = getBufferIndex(kind);
  if (buffer[idx] != nullptr) {
    flush();
  }

	buffer[idx] = new State(kind, state, value1);
}

void Printer::print( Kind kind, char state, int value1, int value2 ) {
  unsigned int idx = getBufferIndex(kind);
  if (buffer[idx] != nullptr) {
    flush();
  }

	buffer[idx] = new State(kind, state, value1, value2);
}

void Printer::print( Kind kind, unsigned int lid, char state ) {
  unsigned int idx = getBufferIndex(kind, lid);
  if (buffer[idx] != nullptr) {
    flush();
  }

	buffer[idx] = new State(kind, lid, state);
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1 ) {
  unsigned int idx = getBufferIndex(kind, lid);
  if (buffer[idx] != nullptr) {
    flush();
  }

	buffer[idx] = new State(kind, lid, state, value1);
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1, int value2 ) {
  unsigned int idx = getBufferIndex(kind, lid);
  if (buffer[idx] != nullptr) {
    flush();
  }

	buffer[idx] = new State(kind, lid, state, value1, value2);
}

