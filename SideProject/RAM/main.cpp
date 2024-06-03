#include "RAM.h"
#include <iostream>
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "systemc.h"

using namespace std;
using namespace sc_dt;
using namespace sc_core;

SC_MODULE(TEST) {
    tlm_utils::simple_initiator_socket<TEST> Master;

    sc_in<bool> clk;
    sc_in<bool> reset;

    void Write() {
        int test_data = 0x26124989;
        int test_addr = 0x00000000;
        while(true) {
            wait();
            tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
            sc_time delay = sc_time(10, SC_NS);
            tlm::tlm_command cmd = tlm::TLM_WRITE_COMMAND;
            trans->set_command(cmd);
            trans->set_address(test_addr);
            trans->set_data_ptr((unsigned char*)&test_data);
            trans->set_data_length(4);
            trans->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
            Master->b_transport(*trans, delay);
            test_addr += 4;
            test_data ++;
        }
    };

    SC_CTOR(TEST) {
        SC_CTHREAD(Write, clk.pos());
        reset_signal_is(reset, false);
    }
};

int sc_main(int argc, char* argv[]) {
    sc_clock clk("clk", sc_time(10, SC_NS));
    sc_signal<bool> reset;

    RAM_SINGLE_PORT ram("ram");
    ram.clk(clk);
    ram.reset(reset);

    TEST test("test");
    test.clk(clk);
    test.reset(reset);

    test.Master.bind(ram.Slave);

    sc_trace_file *wf = sc_create_vcd_trace_file("wave");
    sc_trace(wf, clk, "clk");
    sc_trace(wf, reset, "reset");

    sc_trace(wf, ram.Mem[0], "ram.Mem[0]");
    sc_trace(wf, ram.Mem[1], "ram.Mem[1]");
    sc_trace(wf, ram.Mem[2], "ram.Mem[2]");
    sc_trace(wf, ram.Mem[3], "ram.Mem[3]");

    reset.write(false);
    sc_start(10, SC_NS);
    reset.write(true);
    sc_start(100, SC_NS);

    return 0;
}