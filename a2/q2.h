#ifndef __Q2_H__
#define __Q2_H__ 1

#include <iostream>
#include <vector>
#include "PRNG.h"

class Potato {
    int maxTicks; // maximum ticks
    int currentTick; // current tick #
  public:
    _Event Explode {};
    Potato( unsigned int maxTicks = 10 );
    void reset( unsigned int maxTicks = 10 );
    void countdown();
};

_Coroutine Player {
    _Event Terminate {
      public:
        Player * victim;                      // delete player
        Terminate( Player * victim ) : victim( victim ) {}
    };
    _Event Election {
      public:
        int id;                               // highest id seen so far
        Election( int id ) : id( id ) {}
    };
    int id;                                   // player id
    Player * partner[2];                      // left and right player
    Potato * potato;
    bool firstToss;                           // specifies whether it's the umpire's first toss
    static Player * oldUmpire;                // old umpire for election traversal stopping
    void main();
    void unlink();                            // unlink self from circle
  public:
    static Player * umpire;                   // current umpire

    Player( int id, Potato & potato );
    void start( Player & lp, Player & rp );   // supply partners
    void toss();                              // tossed the potato
    void vote( int id );                      // resume partner to vote
    void terminate();                         // resume umpire to delete
};

#endif
