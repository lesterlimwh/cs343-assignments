#include <iostream>
#include <iomanip>

#include "q1printer.h"

using namespace std;

State::State( unsigned int id, Voter::States state ) : id(id), state(state) {}

State::State( unsigned int id, Voter::States state, TallyVotes::Tour tour ) : 
  id(id), state(state), tour(tour) {}

State::State( unsigned int id, Voter::States state, TallyVotes::Ballot ballot ) :
  id(id), state(state), ballot(ballot) {}

State::State( unsigned int id, Voter::States state, unsigned int numBlocked ) 
  : id(id), state(state), numBlocked(numBlocked) {}

std::string State::format() {
  return "lol";  
}

Printer::Printer( unsigned int voters ) { 
  this->voters = voters;
  buffer = new State*[voters]; // 1 buffer slot for each voter
  for (unsigned int i = 0; i < voters; ++i) {
    buffer[i] = nullptr; // initialize to nullptr
  }

  // output header
  for (unsigned int i = 0; i < voters; ++i) {
    cout << setw(8) << left << "Voter" + to_string(i);
  }
  cout << endl;
  for (unsigned int i = 0; i < voters; ++i) {
    cout << setw(8) << left << "*******";
  }
  cout << endl;
}

Printer::~Printer() {
  for (unsigned int i = 0; i < voters; ++i) {
    delete buffer[i];
  }
  delete[] buffer;
}

void Printer::flush() {
  cout << "flushing" << endl;
  for (unsigned int id = 0; id < voters; ++id) { // output contents of all buffer voters


    // clear buffer
    delete buffer[id];
    buffer[id] = nullptr;
  }
}

void Printer::print( unsigned int id, Voter::States state ) {
  cout << "print1" << state << endl;
  cout << "what" << endl;
  if (buffer[id] != nullptr) {
    flush();
  }
  buffer[id] = new State(id, state);
}

void Printer::print( unsigned int id, Voter::States state, TallyVotes::Tour tour ) {
  cout << "print2" << endl;
  if (buffer[id] != nullptr) {
    flush();
  }
  buffer[id] = new State(id, state, tour);
}

void Printer::print( unsigned int id, Voter::States state, TallyVotes::Ballot ballot ) {
  cout << "print3" << endl;
  if (buffer[id] != nullptr) {
    flush();
  }
  buffer[id] = new State(id, state, ballot);
}

void Printer::print( unsigned int id, Voter::States state, unsigned int numBlocked ) {
  cout << "print4" << endl;
  buffer[id] = new State(id, state, numBlocked);
}

