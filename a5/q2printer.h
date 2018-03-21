#ifndef __Q2PRINTER_H__
#define __Q2PRINTER_H__

#include <string>
#include "q2voter.h"
#include "q2tallyVotes.h"

// internal representation of voter state for buffer
class State {
  unsigned int id;
  Voter::States state;
  TallyVotes::Tour tour;
  TallyVotes::Ballot ballot;
  unsigned int numBlocked;
public:
  State( unsigned int id, Voter::States state ); // S, b, C, X
  State( unsigned int id, Voter::States state, TallyVotes::Tour tour ); // F
  State( unsigned int id, Voter::States state, TallyVotes::Ballot ballot ); // V
  State( unsigned int id, Voter::States state, unsigned int numBlocked ); // B, U
  std::string format(); // formats the voter state into a string for printing based on state type
};

_Monitor Printer {
    unsigned int voters;
    State **buffer; // holds internal representations for state of all the voters
    void flush(); // flush contents of buffer to stdout
  public:
    Printer( unsigned int voters );
    ~Printer();
    void print( unsigned int id, Voter::States state );
    void print( unsigned int id, Voter::States state, TallyVotes::Tour tour );
    void print( unsigned int id, Voter::States state, TallyVotes::Ballot ballot );
    void print( unsigned int id, Voter::States state, unsigned int numBlocked );
};

#endif
