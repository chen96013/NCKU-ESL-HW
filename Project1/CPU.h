#ifndef	CPU_H
#define CPU_H
// #define SC_INCLUDE_DYNAMIC_PROCESSES

#include "systemc.h"
#include "tlm.h" 
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"

using namespace sc_core; 
using namespace sc_dt; 
using namespace std; 

SC_MODULE(CPU) {	
	sc_in<bool> clk, reset, Interrupt;
	
	tlm_utils::simple_initiator_socket<CPU> master_p; //  CPU's master port

    uint SOURCE_C, TARGET_C, SIZE_C, START_C;

    sc_uint<32> addr_c;
	unsigned char data_c, qq;
	
    sc_uint<32> transfer, ready;

	void cpu_p();
	
	SC_CTOR(CPU): master_p("master_p"){		
		SC_CTHREAD(cpu_p, clk.pos());
		reset_signal_is( reset, false ); /// active low		
	}
};

#endif