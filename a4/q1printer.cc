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
  switch (state) {
    case Voter::States::Start:
    case Voter::States::Barging:
    case Voter::States::Complete:
    case Voter::States::Failed:
      return string(1, state);
    case Voter::States::Finished:
      return string(1, state) + " " + string(1, tour);
    case Voter::States::Vote:
      return string(1, state) + " " + to_string(ballot.picture) + "," + to_string(ballot.statue) + "," + to_string(ballot.giftshop);
    case Voter::States::Block:
    case Voter::States::Unblock:
      return string(1, state) + " " + to_string(numBlocked);
    default:
      throw; // invalid state
  }
}

Printer::Printer( unsigned int voters ) { 
  this->voters = voters;
  buffer = new State*[voters]; // 1 buffer slot for each voter
  for (unsigned int i = 0; i < voters; ++i) {
    buffer[i] = nullptr; // initialize to nullptr
  }

  // output header
  for (unsigned int i = 0; i < voters; ++i) {
    cout << setw(8) << left << "V" + to_string(i);
  }
  cout << endl;
  for (unsigned int i = 0; i < voters; ++i) {
    cout << setw(8) << left << "*******";
  }
  cout << endl;
}

Printer::~Printer() {
  flush();
  for (unsigned int i = 0; i < voters; ++i) {
    delete buffer[i];
  }
  delete[] buffer;
  cout << "*****************" << endl;
  cout << "All tours started" << endl;
}

void Printer::flush() {
  for (unsigned int id = 0; id < voters; ++id) { // output contents of all buffer voters
    if (buffer[id] != nullptr) {
      cout << setw(8) << left << buffer[id]->format();

      // clear buffer
      delete buffer[id];
      buffer[id] = nullptr;
    } else {
      cout << setw(8) << left << "";
    }
  }
  cout << endl;
}

void Printer::print( unsigned int id, Voter::States state ) {
  if (buffer[id] != nullptr) {
    flush();
  }
  buffer[id] = new State(id, state);
}

void Printer::print( unsigned int id, Voter::States state, TallyVotes::Tour tour ) {
  if (buffer[id] != nullptr) {
    flush();
  }
  buffer[id] = new State(id, state, tour);
}

void Printer::print( unsigned int id, Voter::States state, TallyVotes::Ballot ballot ) {
  if (buffer[id] != nullptr) {
    flush();
  }
  buffer[id] = new State(id, state, ballot);
}

void Printer::print( unsigned int id, Voter::States state, unsigned int numBlocked ) {
  buffer[id] = new State(id, state, numBlocked);
}

