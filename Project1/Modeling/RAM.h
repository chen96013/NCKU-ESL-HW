#ifndef	RAM_H
#define RAM_H
// #define SC_INCLUDE_DYNAMIC_PROCESSES

#include "systemc.h"
#include "tlm.h" 
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"
#include <iomanip>

using namespace sc_core; 
using namespace sc_dt; 
using namespace std; 

SC_MODULE(RAM) {
	sc_in<bool> clk, reset;	
	
	bool mrw;
	int addr;
    uint mem[1024];
	int* wdata;
		
	//tlm_utils::simple_target_socket<RAM> slave;
	tlm_utils::simple_target_socket<RAM> slave_p; // RAM's slave port
	
	void ram_p();
	void b_transport(tlm::tlm_generic_payload&, sc_time&);
	
	SC_CTOR(RAM): slave_p("slave_p"){
		slave_p.register_b_transport(this, &RAM::b_transport);
		SC_CTHREAD(ram_p, clk.pos());
		reset_signal_is( reset, false ); /// active low			
	}
};


#endif