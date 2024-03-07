#include "systemc.h"
#include "timer.h"

int sc_main (int argc, char** argv) {
    // clock spec declaration and clock instantiate
    sc_time  ClkPrd(10, SC_NS), ClkDly(0, SC_NS);
    sc_clock Clock0("clock0", ClkPrd, 0.5, ClkDly, true);

    // wire instantiate
    sc_signal<bool>start, timeout;

    // timer instantiate and port connection
    timer * timer_pointer0;
    timer Timer0("timer0");
    timer_pointer0 = &Timer0;
    timer_pointer0->start(start);
    timer_pointer0->timeout(timeout);
    timer_pointer0->clock(Clock0);

    // Create signal tracing file (vcd format)
    sc_trace_file * tf = sc_create_vcd_trace_file("RESULT");
    // Add signal to trace file by reference (using pointer)
    sc_trace(tf, Clock0, "clock");
    sc_trace(tf, start,  "start");
    sc_trace(tf, timeout, "timeout");
    sc_trace(tf, timer_pointer0->count, "count");

    // timer controlling

    // cycle counter, cotrolling the cycle limitation of wave file
    int pass_cycles = 0;

    // reset timer for 3 cycles (30 ns)
    start.write(1);
    sc_start(30, SC_NS); pass_cycles+=3;
    start.write(0);

    // repeat timer passing until timer count == 1
    // -> reset before timer count reaches 0
    while (timer_pointer0->count != 1) {
        sc_start(10, SC_NS); pass_cycles++;
    }
    // reset timer for 3 cycles (30 ns)
    start.write(1);
    sc_start(30, SC_NS); pass_cycles+=3;
    start.write(0);

    // repeat timer passing until timer count == 0
    // -> reset after timer count reaches 0
    while (timer_pointer0->count != 0) {
        sc_start(10, SC_NS); pass_cycles++;
    }
    // reset timer for 3 cycles (30 ns)
    start.write(1);
    sc_start(30, SC_NS); pass_cycles+=3;
    start.write(0);

    // time passing until total time reaches 30 cycles
    while (pass_cycles!=30) {
        sc_start(10, SC_NS); pass_cycles++;
    }

    // close wave tracing
    sc_close_vcd_trace_file(tf);

    // done !!!   d(=U=)b good job
    return 0;
}
