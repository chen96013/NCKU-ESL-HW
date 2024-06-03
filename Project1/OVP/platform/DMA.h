#ifndef SC_INCLUDE_DYNAMIC_PROCESSES
#define SC_INCLUDE_DYNAMIC_PROCESSES
#endif
#include "systemc.h"
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/simple_initiator_socket.h"

#ifndef DMA_H
#define DMA_H

using namespace sc_core;
using namespace sc_dt;
using namespace std;

SC_MODULE(DMA) {
    tlm_utils::simple_target_socket<DMA>    slave;
    tlm_utils::simple_initiator_socket<DMA> master;

    sc_in   < bool>  clk;
    sc_in   < bool>  reset;
    sc_out  < bool>  interrupt;

    sc_uint<32> SOURCE;
    sc_uint<32> TARGET;
    sc_uint<32> SIZE;
    bool        START_CLEAR;

    unsigned char* data_slave;
    sc_dt::uint64 addr_slave;
    tlm::tlm_command cmd_slave;
    unsigned char *data;
    sc_dt::uint64 addr;

    void DMA_Master();
    void DMA_Slave(tlm::tlm_generic_payload &trans, sc_time &delay);

    SC_CTOR(DMA) :
    slave("slave"),
    master("master") {
        slave.register_b_transport(this, &DMA::DMA_Slave);

        SC_CTHREAD(DMA_Master, clk.pos());
        reset_signal_is(reset, false);
        data = new unsigned char[4];
    }
};

#endif
