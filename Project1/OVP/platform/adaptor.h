#ifndef ADAPTOR_H
#define ADAPTOR_H

#include "systemc.h"
#include "tlm.h"

using namespace std;
using namespace sc_core;
using namespace sc_dt;
SC_MODULE (ADAPTOR){

    sc_in<bool>interrupt_in;
    tlm::tlm_analysis_port<unsigned int>interrupt_out;
    int interrupt;

    void run();
    SC_CTOR(ADAPTOR){
        SC_METHOD(run);
            sensitive << interrupt_in;
        }
    };

#endif
