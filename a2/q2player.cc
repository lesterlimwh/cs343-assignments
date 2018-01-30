#include "q2.h"

using namespace std;

extern PRNG prng; // global declared in q2.cc

Player* Player::umpire = NULL;

void Player::main() {
  cout << "activated player!" << endl;
  suspend();

  for (;;) {
    cout << " -> " << id;

    // check for explosion

    // toss to next player
    holdingPotato = false;
    partner[prng(1)]->toss();
  }
}

Player::Player(int id, Potato &p) {
  cout << "player " << id << " created" << endl;
  this->id = id;
  this->potato = &p;
}

void Player::start(Player &lp, Player& rp) {
  partner[0] = &lp;
  partner[1] = &rp;
  resume();
}

void Player::toss() {
  cout << "player " << id << " tossed" << endl;
  holdingPotato = true;
  resume();
}

void Player::vote(int id) {
  cout << "player vote" << endl;
} 

void Player::terminate() {
  cout << "player terminate" << endl;
}
