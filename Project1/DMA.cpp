#include "DMA.h"
#include <iostream>
using namespace std;

void DMA::DMA_Master() {
    // SOURCE.write(0);
    // TARGET.write(0);
    // SIZE.write(0);
    // START_CLEAR.write(0);
    interrupt.write(0);
    while (true) {
        wait();
        if (START_CLEAR.read() == 1) {
            int length = 1;
            sc_dt::uint64 source_addr = SOURCE.read();
            sc_dt::uint64 target_addr = TARGET.read();
            for (int i = 0; i < SIZE.read(); i++) {
                tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
                sc_time delay = sc_time(10, SC_NS);
                tlm::tlm_command cmd = tlm::TLM_READ_COMMAND;
                // DMA read from SOURCE
                trans->set_command(cmd);
                trans->set_address(source_addr + i*4);
                trans->set_data_ptr(data);
                trans->set_data_length(length);
                master->b_transport(*trans, delay);
                cout << "DMA read from SOURCE["<< source_addr + i*4 <<"]: " << *((int*)data) << endl;
                // DMA write to TARGET
                cmd = tlm::TLM_WRITE_COMMAND;
                trans->set_command(cmd);
                trans->set_address(target_addr + i * 4);
                master->b_transport(*trans, delay);
                cout << "DMA write to TARGET["<< target_addr + i * 4 <<"]: " << *((int*)data) << endl;
            }
            interrupt.write(true);
            while(START_CLEAR.read() == 1) {
                wait();
            }
            interrupt.write(false);
        }
    }
    
}

void DMA::DMA_Slave(tlm::tlm_generic_payload &trans, sc_time &delay) {
    if (reset.read() == 1) {
        tlm::tlm_command    cmd  = trans.get_command();
        sc_dt::uint64       addr = trans.get_address();
        unsigned char*      ptr  = trans.get_data_ptr();
        if (cmd == tlm::TLM_READ_COMMAND) {
            if (addr/4 == 0) {
                *((int*)ptr) = SOURCE.read();
            } else if (addr/4 == 1) {
                *((int*)ptr) = TARGET.read();
            } else if (addr/4 == 2) {
                *((int*)ptr) = SIZE.read();
            } else if (addr/4 == 3) {
                *((int*)ptr) = START_CLEAR.read();
            } 
        } else if (cmd == tlm::TLM_WRITE_COMMAND) {
            if (addr/4 == 0) {
                SOURCE.write(*((int*)ptr));
            } else if (addr/4 == 1) {
                TARGET.write(*((int*)ptr));
            } else if (addr/4 == 2) {
                SIZE.write(*((int*)ptr));
            } else if (addr/4 == 3) {
                START_CLEAR.write(*((int*)ptr));
            }
        }
    }
}