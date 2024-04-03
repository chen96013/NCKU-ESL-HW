#include "systemc.h"
#include "FIR16.h"
#include <string>
#include <math.h>
#include <time.h>
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

int sc_main (int argc, char** argv) {
    srand(time(NULL));
    
    // clock spec declaration and clock instantiate
    sc_time  ClkPrd(10, SC_NS), ClkDly(0, SC_NS);
    sc_clock CLK("clock0", ClkPrd, 0.5, ClkDly, true);
    sc_signal<bool>RST;

    sc_signal<sc_uint<32> >input, output;
    
    FIR16 Filter("movingAVE");
    FIR16* Filter_ptr = &Filter;
    Filter_ptr->clk(CLK);
    Filter_ptr->rst(RST);
    Filter_ptr->x(input);
    Filter_ptr->y(output);


    // Create signal tracing file (vcd format)
    sc_trace_file * tf = sc_create_vcd_trace_file("RESULT");
    // Add signal to trace file by reference (using pointer)
    sc_trace(tf, Filter_ptr->clk, "clk");
    sc_trace(tf, Filter_ptr->rst, "rst");
    sc_trace(tf, Filter_ptr->x, "x");
    sc_trace(tf, Filter_ptr->y, "y");
    
    RST.write(0);
    input.write(0);
    
    sc_start(20, SC_NS);
    
    RST.write(1);
    
    sc_start(5, SC_NS);

    ifstream TestWave;
    TestWave.open("firdata", ios::in);
    if (TestWave.fail()) {
        ofstream WaveFile;
        WaveFile.open("firdata", ios::app);

        for (int i = 0; i < 1000; i++) {
            unsigned int In_Signal;
            In_Signal = (unsigned int)((sin((float)i * 0.01 * 2 * M_PI) + 1) * exp2(16)) + rand() % 10 * exp2(12);
            WaveFile << hex << setw(8) << setfill('0') << In_Signal;
            input.write(In_Signal);
            sc_start(1, SC_NS);
            if (i!=999) {WaveFile << endl;}
        }
        WaveFile.close();
    }
    else {
        while(!TestWave.eof()) {
            unsigned int In_Signal;
            TestWave >> hex >> In_Signal;
            input.write(In_Signal);
            sc_start(1, SC_NS);
        }
        
    }
    // Input Signal is the sine wave with uniformal noise loaded
    TestWave.close();
    
    // close wave tracing
    sc_close_vcd_trace_file(tf);

    // done !!!   d(=U=)b good job
    return 0;
}
