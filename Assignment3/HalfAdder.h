#ifndef HALFADDER_H
#define HALFADDER_H
#include "systemc.h"

SC_MODULE(HalfAdder) {
    sc_in<bool>A, B;
    sc_out<bool>Sum, Carry;

    void ADD();

    SC_CTOR(HalfAdder) {
        SC_METHOD(ADD);
        sensitive << A << B;
    }
};
#endif