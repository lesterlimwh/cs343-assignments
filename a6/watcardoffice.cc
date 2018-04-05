#include "watcardoffice.h"
#include "MPRNG.h"

#include <iostream>
using namespace std;

extern MPRNG mprng;

void WATCardOffice::main() {
  printer.print(Printer::WATCardOffice, 'S');

  for (;;) {
    _Accept(~WATCardOffice) {
      for (;;) {
        if (jobs.empty()) break;
        delete jobs.front();
        jobs.pop();
      }
      for (unsigned int i = 0; i < numCouriers; ++i) {
        _Accept(requestWork);
      }
      // clean up
      for (unsigned int i = 0; i < numCouriers; ++i) {
        delete couriers[i];
      }
      delete[] couriers;
      break;
    } or _Accept(create, transfer) {
    } or _When(!jobs.empty()) _Accept(requestWork) {}
  }

  printer.print(Printer::WATCardOffice, 'F');
}

WATCardOffice::~WATCardOffice() {}

WATCardOffice::WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers ) :
 printer(prt), bank(bank), numCouriers(numCouriers) {
   couriers = new Courier*[numCouriers];

   for (unsigned int i = 0; i < numCouriers; ++i) {
     couriers[i] = new Courier(i, *this, printer, bank);
   }
}

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
  WATCard *card = new WATCard();
  Job *job = new Job(sid, amount, card);
  jobs.push(job);

  printer.print(Printer::WATCardOffice, 'C', sid, amount);

  return job->result;
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard * card ) {
  Job *job = new Job(sid, amount, card);
  jobs.push(job);

  printer.print(Printer::WATCardOffice, 'T', sid, amount);

  return job->result;
}

WATCardOffice::Job * WATCardOffice::requestWork() {
  if (jobs.empty()) {
    return nullptr;
  }

  Job *job = jobs.front(); // get front of queue
  jobs.pop();

  printer.print(Printer::WATCardOffice, 'W');

  return job;
}

WATCardOffice::Courier::Courier(unsigned int id, WATCardOffice &office, Printer &printer, Bank &bank) :
  id(id), office(office), printer(printer), bank(bank) {}

void WATCardOffice::Courier::main() {
  printer.print(Printer::Courier, id, 'S');

  for (;;) {
    _Accept(~Courier) {
      break;
    } _Else {
      Job *job = office.requestWork();

      if (job == nullptr) break;

      printer.print(Printer::Courier, id, 't', job->sid, job->amount);

      bank.withdraw(job->sid, job->amount);
      job->watcard->deposit(job->amount);

      printer.print(Printer::Courier, id, 'T', job->sid, job->amount);

      // deliver watcard
      if (mprng(5) == 0) { // 1/6th chance of losing card during delivery
        printer.print(Printer::Courier, id, 'L', job->sid);
        job->result.exception(new WATCardOffice::Lost);
      } else {
        job->result.delivery(job->watcard);
      }

      delete job; // reference no longer needed
    }
  }

  printer.print(Printer::Courier, id, 'F');
}
