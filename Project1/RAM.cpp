#include "RAM.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

// RAM's Slave Port
void RAM::b_transport(tlm::tlm_generic_payload& trans, sc_time& delay){
	
	tlm::tlm_command cmd_s = trans.get_command();
	sc_dt::uint64 addr_s = trans.get_address();
	unsigned char* data_s = trans.get_data_ptr();
	unsigned int len = trans.get_data_length();       
	
	addr = addr_s;       
   
	if (cmd_s == tlm::TLM_WRITE_COMMAND) { // write 
		mrw=1;				
        wdata = (reinterpret_cast<int*>(data_s));
	} 
   
	else { // read 
		mrw=0;

        data_s[0] = *(reinterpret_cast<int*>(&mem[addr]));		
	}
	
	wait(delay);		
	trans.set_response_status(tlm::TLM_OK_RESPONSE);	
}  

void RAM::ram_p() {

	for(int i=0; i<1024; i++){
		mem[i]=i;
	}
	
    while (1) {
		wait();
		if(mrw==1) // write
			mem[addr]=*wdata;
    }
}
