#include "systemc.h"
#include "HalfAdder.h"

SC_MODULE(FullAdder) {
    HalfAdder ha1;
    HalfAdder ha2;

    sc_in<bool>A, B, Cin;
    sc_signal<bool>S_HA1, C_HA_1;
    sc_signal<bool>C_HA_2;
    sc_out<bool>S, Cout;   

    void Cout_calculate();

    FullAdder(sc_module_name name) : ha1("ha1"), ha2("ha2") {
        SC_HAS_PROCESS(FullAdder);
        SC_METHOD(Cout_calculate);
        sensitive << A << B << Cin << C_HA_1 << C_HA_2;

        ha1.A(A);
        ha1.B(B);
        ha1.Sum(S_HA1);
        ha1.Carry(C_HA_1);

        ha2.A(S_HA1);
        ha2.B(Cin);
        ha2.Sum(S);
        ha2.Carry(C_HA_2);
    }
};