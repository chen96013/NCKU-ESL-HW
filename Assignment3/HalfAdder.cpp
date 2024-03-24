#include "systemc.h"
#include "HalfAdder.h"

void HalfAdder::ADD() {
    Sum.write(A.read() ^ B.read());
    Carry.write(A.read() & B.read());
}