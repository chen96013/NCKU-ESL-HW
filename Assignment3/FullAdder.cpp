#include "systemc.h"
#include "FullAdder.h"

void FullAdder::Cout_calculate() {
    Cout.write(C_HA_1.read() | C_HA_2.read());
}