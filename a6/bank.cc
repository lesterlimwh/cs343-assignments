#include "bank.h"

Bank::Bank(unsigned int numStudents) :
	numStudents(numStudents), balances(new unsigned int[numStudents]) {
  for (unsigned int i = 0; i < numStudents; ++i) {
  	balances[i] = 0;
  }
}

Bank::~Bank() {
  delete[] balances;
}

// deposit money to student account
void Bank::deposit(unsigned int id, unsigned int amount) {
  balances[id] += amount;
}

void Bank::withdraw(unsigned int id, unsigned int amount) {
  // block until sufficient balance
  for (;;) {
    if (balances[id] < amount) {
      _Accept(Bank::deposit);
    } else {
      break;
    }
  }

  balances[id] -= amount;
}
