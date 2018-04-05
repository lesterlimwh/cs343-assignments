#include "student.h"
#include "MPRNG.h"

#include <iostream>
using namespace std;

extern MPRNG mprng;

Student::Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff, unsigned int id, unsigned int maxPurchases ) :
  printer(prt),
  nameServer(nameServer),
  office(cardOffice),
  groupoff(groupoff),
  id(id),
  maxPurchases(maxPurchases) {}

void Student::main() {
  unsigned int numBottlesToPurchase = mprng(1, maxPurchases); // random # of bottles to buy
  VendingMachine::Flavours preferredFlavour = static_cast<VendingMachine::Flavours>(mprng(numFlavours - 1));
  WATCard::FWATCard watCard = office.create(id, 5); // create watcard with balance of $5
  WATCard::FWATCard giftCard = groupoff.giftCard(); // create gift card
  
  printer.print(Printer::Student, id, 'S', preferredFlavour, numBottlesToPurchase);

  VendingMachine *vm = nameServer.getMachine(id); // get initial vending machine

  printer.print(Printer::Student, id, 'V', vm->getId());

  for (unsigned int b = 0; b < numBottlesToPurchase; ++b) {
    yield(mprng(1,10));

    for (;;) {
      _Select(giftCard) {
        try {
          vm->buy(preferredFlavour, *giftCard());
          printer.print(Printer::Student, id, 'G', preferredFlavour, giftCard()->getBalance());
          giftCard.reset(); // make gift card unusable
          break;
        } catch (VendingMachine::Stock) { // out of stock => try on different machine
          vm = nameServer.getMachine(id);
          printer.print(Printer::Student, id, 'V', vm->getId());
        } catch (VendingMachine::Free) {
          yield(4); // watch advertisement
          printer.print(Printer::Student, id, 'a', preferredFlavour, giftCard()->getBalance());
          break;
        }
      } or _Select (watCard) {
        try {
          vm->buy(preferredFlavour, *watCard());
          printer.print(Printer::Student, id, 'B', preferredFlavour, watCard()->getBalance());
          break;
        } catch (VendingMachine::Stock) { // out of stock => try on different machine
          vm = nameServer.getMachine(id);
          printer.print(Printer::Student, id, 'V', vm->getId());
        } catch (VendingMachine::Free) {
          yield(4); // watch advertisement
          printer.print(Printer::Student, id, 'A', preferredFlavour, watCard()->getBalance());
          break;
        } catch (VendingMachine::Funds) {
          watCard = office.transfer(id, 5 + vm->cost(), watCard()); // transfer money to watcard 
        } catch (WATCardOffice::Lost) {
          printer.print(Printer::Student, id, 'L');
          watCard = office.create(id, 5); // create new card with balance of $5
        }
      }
    }
  }

  try {
    delete watCard();
  } catch (...) {}

  // delete giftCard(); // why does program not end if I uncomment this

  printer.print(Printer::Student, id, 'F');
}

