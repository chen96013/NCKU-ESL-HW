#include "CPU.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

void CPU::cpu_p() {
	ready = 1;
	transfer = 0;
	while(1) {
		wait();
		
		tlm::tlm_generic_payload* trans_m = new tlm::tlm_generic_payload;
		tlm::tlm_command cmd_m;
		sc_time delay = sc_time(10,SC_NS);  // set any delay you have
		
		if(Interrupt.read()==1&& ready==1){
			transfer += 1;
			ready = 0;			

			qq = 0; //START_C = 0;
			cmd_m = tlm::TLM_WRITE_COMMAND; 
			trans_m->set_command(cmd_m);
			trans_m->set_address(0x0000000c); // addr_c
			trans_m->set_data_ptr(reinterpret_cast<unsigned char*>(&qq)); // &data_c
			trans_m->set_data_length(4); 
			trans_m->set_streaming_width(4);
			trans_m->set_byte_enable_ptr(0);
			trans_m->set_dmi_allowed(false);
			trans_m->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
					
			master_p->b_transport(*trans_m, delay);				
		}
		
		else if (Interrupt.read()==0/* && ready==1*/){	
			ready = 1;
			if(transfer==0){
				SOURCE_C = 4;
				TARGET_C = 100;
				SIZE_C = 4;
				START_C = 1;
			}		
			else if(transfer==1){
				SOURCE_C = 32;
				TARGET_C = 20;
				SIZE_C = 8;
				START_C = 1;			
			}
			else if(transfer==2){
				SOURCE_C = 40;
				TARGET_C = 60;
				SIZE_C = 12;
				START_C = 1;
			}
			
			else {
				SOURCE_C = 0;
				TARGET_C = 0;
				SIZE_C = 0;
				START_C = 0;			
			}			
			// source and target addresses and data setting 
			
			int i;
			for(i=1; i<=4; i++){			
					switch(i){
						case 1: { addr_c = 0x00000000; data_c = SOURCE_C;  } break;
						case 2: { addr_c = 0x00000004; data_c = TARGET_C;  } break;
						case 3: { addr_c = 0x00000008; data_c = SIZE_C;     } break;
						case 4: { addr_c = 0x0000000c; data_c = START_C;    } break;
						default:{ addr_c = 0x00000000; data_c = 0x00000000; } break;
					}
						
					cmd_m = tlm::TLM_WRITE_COMMAND; 
					trans_m->set_command(cmd_m);
					trans_m->set_address(addr_c);
					trans_m->set_data_ptr(reinterpret_cast<unsigned char*>(&data_c));
					trans_m->set_data_length(4); 
					trans_m->set_streaming_width(4);
					trans_m->set_byte_enable_ptr(0);
					trans_m->set_dmi_allowed(false);
					trans_m->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
							
					master_p->b_transport(*trans_m, delay);	
				}								
		}
	}
} 

