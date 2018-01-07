#include <iostream>
#include <cstdlib>                      // access: rand, srand
using namespace std;
#include <unistd.h>                     // access: getpid

int times = 1000;                       // default value

int rtn1( int i ) {
    bool flag = false;
    int val = -1;
    for ( int j = 0; j < times && !flag; j += 1 ) {
        if ( rand() % 10000 == 42 ) {
            val = j;
            flag = true;
        }
    }
    return val;
}
int rtn2( int i ) {
    bool flag = false;
    int val = -1;
    for ( int j = times; j >= 0 && !flag; j -= 1 ) {
        if ( rand() % 10000 == 42 ) {
            val = j;
            flag = true;
        }
    }
    return val; 
}
int g( int i ) {
    int flag = false;
    int val; // value to return (simulates what was previously thrown by rtn functions) 
    for ( int j = 0; j < times && flag == false; j += 1 ) {
        if ( rand() % 2 == 0 ) {
            val = rtn1( i );
            if (val != -1) {
                flag = true;
            }
        } else {
            val = rtn2( i );
            if (val != -1) {
                flag = true;
            }
        }
    }
    if ( i % 2 && !flag) {
        val = rtn2( i );
        if (val != -1) {
            flag = true;
        }
    }

    if (!flag) {
        val = rtn1( i );
    }

    return val; // either returns -1 or a positive value
}
int f( int i ) {
    bool flag = false;
    int val;

    for ( int j = 0; j < times && flag == false; j += 1 ) {
        val = g( i );
        if (val != -1) {
            flag = true;
        }
    }
    if ( i % 2 && !flag ) {
        val = g( i );
        if (val != -1) {
            flag = true;
        }
    }
    if (!flag) {
        val = g( i );
    }

    return val; // either returns -1 or a positive value
}
int main( int argc, char *argv[] ) {
    int seed = getpid();                // default value
    try {				// process command-line arguments
        switch ( argc ) {
          case 3: times = stoi( argv[2] ); if ( times <= 0 ) throw 1;
          case 2: seed = stoi( argv[1] ); if ( seed <= 0 ) throw 1;
          case 1: break;                // use defaults
          default: throw 1;
        } // switch
    } catch( ... ) {
        cout << "Usage: " << argv[0] << " [ seed (> 0) [ times (> 0) ] ]" << endl;
        exit( 1 );
    } // try
    srand( seed );			// set random-number seed
    // begin program
    int rc = f( 3 );
    if (rc == -1) {
        cout << "seed:" << seed << " times:" << times << " complete" << endl;
    } else {
        cout << "seed:" << seed << " times:" << times << " rc:" << rc << endl;
    }
}
