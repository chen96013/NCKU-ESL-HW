#include "systemc.h"

SC_MODULE(timer) {
    sc_in<bool> start;
    sc_out<bool>timeout;
    sc_in<bool> clock;

    int count;
    const int init_count;
    void runtimer();

    timer(sc_module_name name, int init_count = 5) : init_count(init_count) {
        SC_HAS_PROCESS(timer);
        SC_CTHREAD(runtimer, clock.pos());
        count = init_count;
    }
};