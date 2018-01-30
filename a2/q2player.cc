#include "q2.h"

using namespace std;

void Player::main() {
  cout << "activated player!" << endl;
}

Player::Player(int id, Potato &potato) {
  cout << "player created" << endl;
}

void Player::start(Player &lp, Player& rp) {
  cout << "player starting" << endl;
}

void Player::toss() {
  cout << "player toss" << endl;
}

void Player::vote(int id) {
  cout << "player vote" << endl;
} 

void Player::terminate() {
  cout << "player terminate" << endl;
}
