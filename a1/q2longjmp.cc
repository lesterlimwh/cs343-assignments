#include <iostream>
#include <cstdlib>
using namespace std;
#include <unistd.h>                     // access: getpid
#include <setjmp.h>

#ifdef NOOUTPUT
#define PRT( stmt )
#else
#define PRT( stmt ) stmt
#endif // NOOUTPUT

PRT( struct T { ~T() { cout << "~"; } }; )

struct E {};                            // exception type
long int freq = 5;                      // exception frequency
jmp_buf exception_env;

long int Ackermann( long int m, long int n ) {
    jmp_buf local_env;
    memcpy(local_env, exception_env, sizeof(jmp_buf));
    PRT( T t; )
    if ( m == 0 ) {
        if ( rand() % freq == 0 ) longjmp(exception_env, 1);
        return n + 1;
    } else if ( n == 0 ) {
        if ( rand() % freq == 0 ) longjmp(exception_env, 1);
        // catch
        if (setjmp(exception_env)) {
            PRT( cout << "E1 " << m << " " << n << endl );
        } else { // try
            return Ackermann( m - 1, 1 );
        }
    } else {
        // catch
        if (setjmp(exception_env)) {
            PRT( cout << "E2 " << m << " " << n << endl );
        } else { // try
            memcpy(local_env, exception_env, sizeof(jmp_buf));
            long int result = Ackermann( m, n - 1 );
            memcpy(exception_env, local_env, sizeof(jmp_buf));
            return Ackermann( m - 1, result);
        }
    } // if
    memcpy(exception_env, local_env, sizeof(jmp_buf));
    return 0;                           // recover by returning 0
}
int main( int argc, char *argv[] ) {
    long int m = 4, n = 6, seed = getpid(); // default values
    try {				// process command-line arguments
        switch ( argc ) {
          case 5: freq = stoi( argv[4] ); if ( freq <= 0 ) throw 1;
          case 4: seed = stoi( argv[3] ); if ( seed <= 0 ) throw 1;
          case 3: n = stoi( argv[2] ); if ( n < 0 ) throw 1;
          case 2: m = stoi( argv[1] ); if ( m < 0 ) throw 1;
          case 1: break;                // use defaults
          default: throw 1;
        } // switch
    } catch( ... ) {
        cout << "Usage: " << argv[0] << " [ m (> 0) [ n (> 0) [ seed (> 0)"
            " [ freq (> 0) ] ] ] ]" << endl;
        exit( 1 );
    } // try
    srand( seed );			// seed random number
    // begin program
    if (setjmp(exception_env)) {
        PRT( cout << "E3" << endl );
    } else {
        PRT( cout << m << " " << n << " " << seed << " " << freq << endl );
        long int val = Ackermann( m, n );
        PRT( cout << val << endl );
    }
}
