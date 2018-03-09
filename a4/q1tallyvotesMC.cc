#include "q1tallyvotes.h"


TallyVotes::TallyVotes( unsigned int voters, unsigned int group, Printer & printer ) :
 num_voters(voters), group_size(group), printer(printer) {
   
 }

TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ) {

}

void TallyVotes::done() {

}
