#include "systemc.h"
#include "FIR16.h"

void FIR16::Pipline_reg() {
    while(1) {
        wait();
        if (!rst.read()) {
            for (int i = 15; i >= 0; i--) {
                Reg[i].write(0);
            }
        }
        else {
            for (int i = 15; i >= 0; i--) {
                Reg[i].write( (i)? Reg[i-1].read() : x.read());
            }
        }
    }
}

void FIR16::Adder_Tree() {
    temp = x.read() * Weight[0].read();
    for (int i = 15; i >= 0; i--) {
        temp += Reg[i].read() * Weight[i+1].read();
    }
    y.write(temp(47,16));
}