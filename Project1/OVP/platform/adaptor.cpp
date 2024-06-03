#include "adaptor.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;


void ADAPTOR::run(){
    interrupt = interrupt_in.read();
    interrupt_out.write(interrupt);
}
