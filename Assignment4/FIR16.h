#ifndef FIR16_H
#define FIR16_H
#include "systemc.h"

SC_MODULE(FIR16) {
    sc_in<bool>CLK, RST;
    sc_out<bool>Sum, Carry;

    void MAC();

    SC_CTOR(FIR16) {
        SC_CTHREAD(MAC, CLK);
        sensitive << A << B;
    }
};
#endif