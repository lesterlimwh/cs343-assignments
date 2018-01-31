#include "q2.h"

using namespace std;

extern PRNG prng; // global declared in q2.cc

Player* Player::umpire = nullptr;
Player* Player::oldUmpire = nullptr;

void Player::main() {
  suspend();

  for (;;) {
    try {
      _Enable {
        if (this == Player::umpire && firstToss) {
          cout << "U " << id;
          firstToss = false;
        } else {
          cout << " -> " << id;
        }

        // check for explosion
        potato->countdown();

        // toss to next player
        partner[prng(1)]->toss();
      }
    } catch (Potato::Explode &e) {
      cout << " is eliminated" << endl;

      if (this == Player::umpire) {
        try {
          _Enable {
            // start election process
            cout << "E " << id;
            Player::oldUmpire = Player::umpire;
            _Resume Election(partner[1]->id) _At *partner[1];
            partner[1]->vote(partner[1]->id);
          }
        } catch (Player::Election &e) {
          cout << " : umpire " << e.id << endl;
        }
      }

      unlink(); // player unlinks itself .... 3 should point to itself
      _Resume Terminate(this) _At *Player::umpire;
      Player::umpire->terminate();
    } catch (Player::Terminate &e) {
      potato->reset();
      if (partner[0] == this && partner[1] == this) { // only player left, so game is finished
        break;
      }
      this->firstToss = true;
      this->toss();
    } catch (Player::Election &e) {
      cout << " -> " << id;
      int higherId = this->id > e.id ? this->id : e.id;
      _Resume Election(higherId) _At *partner[1];
      partner[1]->vote(higherId);
    }
  }
  cout << "U " << id << " wins the Match!" << endl;
}

Player::Player(int id, Potato &p) {
  this->id = id;
  this->potato = &p;
  this->firstToss = true;
}

void Player::unlink() {
  Player *left = partner[0];
  Player *right = partner[1];
  left->partner[1] = right;
  right->partner[0] = left;
}

void Player::start(Player &lp, Player& rp) {
  partner[0] = &lp;
  partner[1] = &rp;
  resume();
}

void Player::toss() {
  resume();
}

void Player::vote(int highestId) {
  if (this->id >= highestId && this != Player::oldUmpire) {
    Player::umpire = this;
  }
  resume();
} 

void Player::terminate() {
  resume();
}
