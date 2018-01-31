#include "q2.h"

using namespace std;

PRNG prng; // globally defined prng

// prints usage information
void usage(char *arg) {
  cerr << "Usage: " << arg
    << " [ games (>= 0) | \"x\" [ players (>= 2) | "
    << "\"x\" [ random-seed (> 0) | \"x\" ] ] ]" << endl;
  exit(EXIT_FAILURE);
}

// parses string to a number, and exits program if it fails
int tryParseStringToNumber(char* arg) {
  char* endptr;

  long val = strtol(arg, &endptr, 10);

  if (*endptr == 0) { // successfully parsed string
    return int(val);
  } else {
    usage(arg); 
    return -1; // will never get here, but gets rid of compiler warning
  }
}

int main( int argc, char *argv[] ) {
  int numGames = 5;
  int numPlayers;
  bool numPlayersSpecified = false; // generates players each game if false

  // parse user input
  switch (argc) {
    case 4: // read seed
      if (argv[3][0] == 'x') {
        prng.seed(getpid()); // default
      } else {
        prng.seed(tryParseStringToNumber(argv[3]));
      }
    case 3: // read players
      if (argv[2][0] != 'x') {
        numPlayersSpecified = true;
        numPlayers = tryParseStringToNumber(argv[2]);
        if (numPlayers < 2) {
          usage(argv[0]);
        }
      }
    case 2: // read games
      if (argv[1][0] == 'x') {
        numGames = 5; 
      } else {
        numGames = tryParseStringToNumber(argv[1]);
        if (numGames < 0) {
          usage(argv[0]);
        }
      }
    case 1:
      break;
    default:
      usage(argv[0]);
  }

  // create potato
  Potato potato;

  for (int g = 0; g < numGames; ++g) {
    if (!numPlayersSpecified) { // generate numPlayers if it isn't specified in args
      numPlayers = prng(2, 10);
    }
    cout << numPlayers << " players in the game" << endl;

    // store vector of players
    vector<Player*> players;
    for (int i = 0; i < numPlayers; ++i) {
      players.push_back(new Player(i, potato));
    }

    // create circle of players
    for (int i = 0; i < numPlayers; ++i) {
      Player * left = players.at((i + players.size() - 1) % players.size());
      Player * right = players.at((i + players.size() + 1) % players.size());
      players.at(i)->start(*left, *right);
    }

    // set global umpire
    Player::umpire = players.at(0);

    // toss potato to it to start game
    Player::umpire->toss();

    // clean up
    for (auto player : players) {
      delete player;
    }
  }
}
