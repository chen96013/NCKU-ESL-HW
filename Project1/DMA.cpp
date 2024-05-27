#include "DMA.h"
#include <iostream>
using namespace std;

void DMA::DMA_Master() {
    interrupt.write(0);
    while (true) {
        wait();
        if (START_CLEAR.read() == 1) {
            // DMA start
            cout << "\n    DMA start new mission" << endl;
            cout << "=============================" << endl;
            int length = 1;
            sc_dt::uint64 source_addr = SOURCE.read();
            sc_dt::uint64 target_addr = TARGET.read();
            for (int i = 0; i < SIZE.read()/4; i++) {
                tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
                sc_time delay = sc_time(10, SC_NS);
                tlm::tlm_command cmd = tlm::TLM_READ_COMMAND;
                // DMA read from SOURCE
                trans->set_command(cmd);
                trans->set_address(source_addr + i*4);
                trans->set_data_ptr(data);
                trans->set_data_length(length);
                trans->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
                master->b_transport(*trans, delay);
                cout << "DMA <--- SOURCE["<< source_addr + i*4 <<"]: " << *((int*)data) << endl;
                // DMA write to TARGET
                cmd = tlm::TLM_WRITE_COMMAND;
                trans->set_command(cmd);
                trans->set_address(target_addr + i * 4);
                trans->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
                master->b_transport(*trans, delay);
                cout << "DMA ---> TARGET["<< target_addr + i * 4 <<"]: " << *((int*)data) << endl;
            }
            interrupt.write(true);
            cout << "DMA finish, interrupt -> CPU" << endl;
            cout << "=============================" << endl;
            while(START_CLEAR.read() == 1) {
                wait();
            }
            interrupt.write(false);
        }
    }
}

void DMA::DMA_Registers() {
    SOURCE.write(0);
    TARGET.write(0);
    SIZE.write(0);
    START_CLEAR.write(0);
    while (true) {
        wait();
        if (cmd_slave == tlm::TLM_WRITE_COMMAND) {
            if (addr_slave == 0 && START_CLEAR.read() == 0) { // protect registers when DMA is running
                SOURCE.write(*((int*)data_slave));
                cout << "SOURCE write by CPU: " << *((int*)data_slave) << endl;
            } else if (addr_slave == 4 && START_CLEAR.read() == 0) { // protect registers when DMA is running
                TARGET.write(*((int*)data_slave));
                cout << "TARGET write by CPU: " << *((int*)data_slave) << endl;
            } else if (addr_slave == 8 && START_CLEAR.read() == 0) { // protect registers when DMA is running
                SIZE.write(*((int*)data_slave));
                cout << "SIZE write by CPU: " << *((int*)data_slave) << endl;
            } else if (addr_slave == 12) {
                START_CLEAR.write(*((int*)data_slave));
                if (*((int*)data_slave) != START_CLEAR.read()) {
                    cout << "START_CLEAR write by CPU: " << *((int*)data_slave) << endl;
                }
            }
        }
    }
}


void DMA::DMA_Slave(tlm::tlm_generic_payload &trans, sc_time &delay) {
    if (reset.read() == 1) {
        cmd_slave = trans.get_command();
        if (cmd_slave == tlm::TLM_READ_COMMAND) {
            sc_dt::uint64       addr = trans.get_address();
            unsigned char*      ptr  = trans.get_data_ptr();
            switch(addr/4) {
                case 0: *((int*)ptr) = SOURCE.read(); break;
                case 1: *((int*)ptr) = TARGET.read(); break;
                case 2: *((int*)ptr) = SIZE.read(); break;
                case 3: *((int*)ptr) = START_CLEAR.read(); break;
            }
        } else if (cmd_slave == tlm::TLM_WRITE_COMMAND) {
            data_slave = trans.get_data_ptr();
            addr_slave = trans.get_address();
        }
    }
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
    wait(delay);
}