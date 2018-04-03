#include "parent.h"
#include "MPRNG.h"

extern MPRNG mprng;

Parent::Parent( Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay ) :
  printer(prt), bank(bank), numStudents(numStudents), delay(parentalDelay) {}

void Parent::main() {
  printer.print(Printer::Parent, 'S');

  for (;;) {
    _Accept(Parent::~Parent) {
      printer.print(Printer::Kind::Parent, 'F');
      break;
    } _Else {
      yield(delay); // wait before giving money to random student

      unsigned int luckyStudentId = mprng(numStudents - 1); // student to give money to
      unsigned int amountToGive = mprng(1, 3); // amount of money to give

      printer.print(Printer::Parent, 'D', luckyStudentId, amountToGive);
      bank.deposit(luckyStudentId, amountToGive);
    }
  }
}
