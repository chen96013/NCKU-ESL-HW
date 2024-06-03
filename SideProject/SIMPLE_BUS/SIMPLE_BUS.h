#ifndef SC_INCLUDE_DYNAMIC_PROCESSES
#define SC_INCLUDE_DYNAMIC_PROCESSES
#endif
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "systemc.h
#include "vector"

#ifndef SIMPLE_BUS_H
#define SIMPLE_BUS_H

using namespace std;
using namespace sc_dt;
using namespace sc_core;

SC_MODULE(SIMPLE_BUS) {
    tlm_utils::simple_target_socket<SIMPLE_BUS>* BUS_Slave;

    tlm_utils::simple_initiator_socket<SIMPLE_BUS>* BUS_Master;

    sc_in <bool> clk;
    sc_in <bool> reset; // active low
    
    void BUS2Master(tlm::tlm_generic_payload &trans, sc_time &delay);
    void BUS2Slave();

    SC_CTOR(SIMPLE_BUS): 
    BUS_Slave0("BUS_Slave0"),
    BUS_Master0("BUS_Master0"),
    BUS_Slave1("BUS_Slave1"),
    BUS_Master1("BUS_Master1") {
        BUS_Slave0.register_b_transport(this, &SIMPLE_BUS::BUS2Master);
        BUS_Slave1.register_b_transport(this, &SIMPLE_BUS::BUS2Master);

        SC_CTHREAD(BUS2Slave, clk.pos());
        reset_signal_is(reset, false);
    }

};

#endif