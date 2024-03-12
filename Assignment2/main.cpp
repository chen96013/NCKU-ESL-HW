#include "systemc.h"
#include "timer.h"

bool check_timer(int*, int);

int sc_main (int argc, char* argv[]) {
    // clock spec declaration and clock instantiate
    sc_time  ClkPrd1(10, SC_NS), ClkDly1(0, SC_NS);
    sc_clock Clock1("clock1", ClkPrd1, 0.5, ClkDly1, true);
    sc_time  ClkPrd2(5, SC_NS), ClkDly2(0, SC_NS);
    sc_clock Clock2("clock2", ClkPrd2, 0.5, ClkDly2, true);
    sc_time  ClkPrd3(2, SC_NS), ClkDly3(0, SC_NS);
    sc_clock Clock3("clock3", ClkPrd3, 0.5, ClkDly3, true);

    // wire instantiate
    sc_signal<bool>start1, timeout1;
    sc_signal<bool>start2, timeout2;
    sc_signal<bool>start3, timeout3;

    // timer1 instantiate and port connection
    timer * timer_pointer1;
    timer Timer1("timer1");
    timer_pointer1 = &Timer1;
    timer_pointer1->start(start1);
    timer_pointer1->timeout(timeout1);
    timer_pointer1->clock(Clock1);

    // timer2 instantiate and port connection
    timer * timer_pointer2;
    timer Timer2("timer2",10);
    timer_pointer2 = &Timer2;
    timer_pointer2->start(start2);
    timer_pointer2->timeout(timeout2);
    timer_pointer2->clock(Clock2);

    // timer3 instantiate and port connection
    timer * timer_pointer3;
    timer Timer3("timer3",25);
    timer_pointer3 = &Timer3;
    timer_pointer3->start(start3);
    timer_pointer3->timeout(timeout3);
    timer_pointer3->clock(Clock3);

    // Create signal tracing file (vcd format)
    sc_trace_file * tf = sc_create_vcd_trace_file("RESULT");
    // Add signal to trace file by reference (using pointer)
    sc_trace(tf, Clock1, "clock1");
    sc_trace(tf, start1,  "start1");
    sc_trace(tf, timeout1, "timeout1");
    sc_trace(tf, timer_pointer1->count, "count1");
    
    sc_trace(tf, Clock2, "clock2");
    sc_trace(tf, start2,  "start2");
    sc_trace(tf, timeout2, "timeout2");
    sc_trace(tf, timer_pointer2->count, "count2");

    sc_trace(tf, Clock3, "clock3");
    sc_trace(tf, start3,  "start3");
    sc_trace(tf, timeout3, "timeout3");
    sc_trace(tf, timer_pointer3->count, "count3");


    // ||=================||
    // ||timer controlling||
    // ||=================||

    // cycle counter, cotrolling the cycle limitation of wave file
    int pass_cycles = 0;

    // reset timer for 3 cycles (30 ns)
    start1.write(1); start2.write(1); start3.write(1);
    sc_start(30, SC_NS); pass_cycles+=3;
    start1.write(0); start2.write(0); start3.write(0);

    // repeat timer passing until timer count == 1
    // -> reset before timer count reaches 0
    while (pass_cycles != 300) {
        sc_start(1, SC_NS); pass_cycles++;
        //reset when count meet zero
        start1.write(check_timer(&(timer_pointer1->count),0));
        start2.write(check_timer(&(timer_pointer2->count),0));
        start3.write(check_timer(&(timer_pointer3->count),0));
    }

    // close wave tracing
    sc_close_vcd_trace_file(tf);

    // done !!!   d(=U=)b good job
    return 0;
}

bool check_timer(int* count, int checkpoint){
    return (*count==checkpoint);
};
