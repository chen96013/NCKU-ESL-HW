#ifndef SC_INCLUDE_DYNAMIC_PROCESSES
#define SC_INCLUDE_DYNAMIC_PROCESSES
#endif
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "systemc.h"

#ifndef RAM_H
#define RAM_H

#define MEM_SIZE 1024
#define MEM_WIDTH 32

using namespace std;
using namespace sc_dt;
using namespace sc_core;

SC_MODULE(RAM_SINGLE_PORT) {
    tlm_utils::simple_target_socket<RAM_SINGLE_PORT>Slave;

    sc_in <bool> clk;
    sc_in <bool> reset; // active low
    sc_signal<sc_uint<MEM_WIDTH> >Mem[MEM_SIZE];
    bool write_enable;
    uint64 addr;
    unsigned char *data;
    
    void RAM();
    void RAM_Wrapper(tlm::tlm_generic_payload &trans, sc_time &delay);

    SC_CTOR(RAM_SINGLE_PORT) {
        Slave.register_b_transport(this, &RAM_SINGLE_PORT::RAM_Wrapper);

        SC_CTHREAD(RAM, clk.pos());
        reset_signal_is(reset, false);
    }
};

#endif