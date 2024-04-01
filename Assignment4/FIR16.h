#ifndef FIR16_H
#define FIR16_H
#include "systemc.h"

SC_MODULE(FIR16) {
    sc_in<bool> clk, rst;

    sc_in<sc_uint<32> >x;
    sc_out<sc_uint<32> >y;

    sc_signal<sc_uint<32> >Reg[16];
    sc_signal<sc_uint<32> >Weight[17];
    sc_uint<64>temp;
    
    void Pipline_reg();
    void Adder_Tree();

    SC_CTOR(FIR16) {
        SC_CTHREAD(Pipline_reg, clk.pos());
        reset_signal_is(rst, false);

        for (int i = 0; i < 17; i++){
            Weight[i] = 0x00000F0F;
        }

        SC_METHOD(Adder_Tree);        
        sensitive << x;
    }
};
#endif