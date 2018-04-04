#ifndef WATCARDOFFICE_H
#define WATCARDOFFICE_H

#include <queue>
#include "watcard.h"
#include "printer.h"
#include "bank.h"

_Task WATCardOffice {
    struct Job {                              // marshalled arguments and return future
      unsigned int sid;
      unsigned int amount;
      WATCard *watcard;
      WATCard::FWATCard result;             // return future

      Job( unsigned int sid, unsigned int amount, WATCard *watcard ) : 
        sid(sid),
        amount(amount),
        watcard(watcard) {}
    };

    _Task Courier {                          // communicates with bank
        unsigned int id;
        WATCardOffice &office;
        Printer &printer;
        Bank &bank;
        void main();
      public:
        Courier( unsigned int id, WATCardOffice &office, Printer &printer, Bank &bank );
    };

    Printer &printer;
    Bank &bank;
    unsigned int numCouriers;
    std::queue<Job*> jobs;
    Courier **couriers;

    void main();
  public:
    _Event Lost {};                           // lost WATCard
    WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers );
    ~WATCardOffice();
    WATCard::FWATCard create( unsigned int sid, unsigned int amount ); 
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard * card );
    Job * requestWork();
};

#endif
