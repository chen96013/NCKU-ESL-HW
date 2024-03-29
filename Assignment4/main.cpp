#include "systemc.h"
#include "HalfAdder.h"
#include "FullAdder.h"

int sc_main (int argc, char** argv) {
    sc_signal<bool>A, B, Cin, S_HA, S_FA, C_HA, C_FA;

    HalfAdder HA0("Half-adder");
    HalfAdder *HA0_ptr = &HA0;
    HA0_ptr->A(A);
    HA0_ptr->B(B);
    HA0_ptr->Sum(S_HA);
    HA0_ptr->Carry(C_HA);

    FullAdder FA0("Full-adder");
    FullAdder *FA0_ptr = &FA0;
    FA0_ptr->A(A);
    FA0_ptr->B(B);
    FA0_ptr->Cin(Cin);
    FA0_ptr->S(S_FA);
    FA0_ptr->Cout(C_FA);

    // Create signal tracing file (vcd format)
    sc_trace_file * tf = sc_create_vcd_trace_file("RESULT");
    // Add signal to trace file by reference (using pointer)
    sc_trace(tf, A, "Half-adder_A");
    sc_trace(tf, B, "Half-adder_B");
    sc_trace(tf, S_HA, "Half-adder_Sum");
    sc_trace(tf, C_HA, "Half-adder_Carry");

    sc_trace(tf, A, "Full-adder_A");
    sc_trace(tf, B, "Full-adder_B");
    sc_trace(tf, Cin, "Full-adder_Cin");
    sc_trace(tf, S_FA, "Full-adder_S");
    sc_trace(tf, C_FA, "Full-adder_Cout");

    // use a nest loop to let all 2^3 type of status run once
    A.write(0);B.write(0);Cin.write(0);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                sc_start(10, SC_NS);
                A.write(!A.read());
            }
            B.write(!B.read());
        }
        Cin.write(!Cin.read());
    }

    // close wave tracing
    sc_close_vcd_trace_file(tf);

    // done !!!   d(=U=)b good job
    return 0;
}
