#include "DMA.h"
#include "CPU.h"
#include "RAM.h"

int sc_main(int argc, char* argv[]) {
	
	sc_clock clock("clock", 10, SC_NS);
	sc_signal<bool> reset, Interrupt;
	
	sc_signal<sc_uint<32> > addr; 
	sc_signal<sc_uint<32> > data; 

	
	CPU *cpu; // name: cpu
	DMA *dma;   
	RAM *ram;

	cpu = new CPU("cpu");
	dma = new DMA("dma");
	ram = new RAM("ram");
	
	// Binding 
	cpu->master_p.bind(dma->slave);
	dma->master.bind(ram->slave_p);
	
	
	cpu->clk(clock);
	cpu->reset(reset);
	cpu->Interrupt(Interrupt);
	
	dma->clk(clock);
	dma->reset(reset);
	dma->interrupt(Interrupt);	
	
	ram->clk(clock);
	ram->reset(reset);
	

	
	
	// Tracing: 
	sc_trace_file *tf = sc_create_vcd_trace_file("Result");
		
	sc_trace(tf, clock, "clock");
	sc_trace(tf, reset, "reset");
	sc_trace(tf, Interrupt, "Interrupt");
	
	// CPU
	sc_trace(tf, cpu->SOURCE_C, "SOURCE_C");
	sc_trace(tf, cpu->TARGET_C, "TARGET_C");
	sc_trace(tf, cpu->SIZE_C, "SIZE_C");
	sc_trace(tf, cpu->START_C, "START_C");
	//sc_trace(tf, cpu->transfer, "transfer");
	
	// DMA
	sc_trace(tf, dma->SOURCE, "SOURCE");
	sc_trace(tf, dma->TARGET, "TARGET");
	sc_trace(tf, dma->SIZE, "SIZE");	
	sc_trace(tf, dma->START_CLEAR, "START");
	sc_trace(tf, dma->data, "data_m");
	
	// sc_trace(tf, dma->temp_size, "temp_size");
	// sc_trace(tf, dma->temp_source, "temp_source");
	// sc_trace(tf, dma->temp_target, "temp_target");
	
	
	/*
	// RMA
	// 1
	sc_trace(tf, ram->mem[4], "mem[4]");
	sc_trace(tf, ram->mem[100], "mem[100]");

	// 2
	sc_trace(tf, ram->mem[32], "mem[32]");	
	sc_trace(tf, ram->mem[36], "mem[36]");
	sc_trace(tf, ram->mem[20], "mem[20]");
	sc_trace(tf, ram->mem[24], "mem[24]");
		
	// 3
	sc_trace(tf, ram->mem[40], "mem[40]");
	sc_trace(tf, ram->mem[44], "mem[44]");
	sc_trace(tf, ram->mem[48], "mem[48]");
	sc_trace(tf, ram->mem[60], "mem[60]");
	sc_trace(tf, ram->mem[64], "mem[64]");
	sc_trace(tf, ram->mem[68], "mem[68]");	
	*/
	//sc_trace(tf, dma->srw, "srw");
	
	
	
	reset.write(0);
	sc_start(10,SC_NS);	
	reset.write(1);
	sc_start(410,SC_NS); // 500
	
	
	sc_close_vcd_trace_file(tf);
	return 0;
}