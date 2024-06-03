#include "RAM.h"
#include <iostream>

void RAM_SINGLE_PORT::RAM() {
    while (true) {
        wait();
        if (write_enable) {
            cout << "RAM[" << addr << "] = " << *((int*)data) << endl;
            Mem[addr/4].write(*((int*)data));
            cout << "RAM[" << addr << "] = " << *((int*)data) << endl;
        }
    }
}

void RAM_SINGLE_PORT::RAM_Wrapper(tlm::tlm_generic_payload &trans, sc_time &delay) {
    tlm::tlm_command cmd = trans.get_command();
    addr = trans.get_address();
    if (addr > MEM_SIZE || addr < 0) {
        cout << "Error -> RAM: Address out of range" << endl;
        trans.set_command(tlm::TLM_IGNORE_COMMAND);
        trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
    } else if (cmd == tlm::TLM_READ_COMMAND) {
        unsigned char* ptr = trans.get_data_ptr();
        data = (unsigned char *)&Mem[addr/4].read();
        ptr = data;
        trans.set_response_status(tlm::TLM_OK_RESPONSE);
    } else if (cmd == tlm::TLM_WRITE_COMMAND) {
        cout << "水喔" << endl;
        write_enable = true;
        data = trans.get_data_ptr();
        trans.set_response_status(tlm::TLM_OK_RESPONSE);
    }
    wait(delay);
    write_enable = false;
    return;
}