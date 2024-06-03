/*
 *
 * Copyright (c) 2005-2019 Imperas Software Ltd., www.imperas.com
 *
 * The contents of this file are provided under the Software License
 * Agreement that you accepted before downloading this file.
 *
 * This source forms part of the Software and can be used for educational,
 * training, and demonstration purposes but cannot be used for derivative
 * works except in cases where the derivative works require OVP technology
 * to run.
 *
 * For open source models released under licenses that you can use for
 * derivative works, please visit www.OVPworld.org or www.imperas.com
 * for the location of the open source models.
 *
 */

#include "tlm/tlmModule.hpp"
#include "tlm/tlmDecoder.hpp"
#include "tlm/tlmMemory.hpp"
//#include "ovpworld.org/processor/or1k/1.0/tlm/processor.igen.hpp"
#include "/usr/cad/imperas/cur/ImperasLib/source/andes.ovpworld.org/processor/riscv/1.0/tlm/processor.igen.hpp"
#include "DMA.h"
#include "adaptor.h"

// alternative processors:
// #include "arm.ovpworld.org/processor/arm/1.0/tlm/processor.igen.hpp"
// #include "mips.ovpworld.org/processor/mips32/1.0/tlm/processor.igen.hpp"

using namespace sc_core;
#define IMPERAS_TLM_CPU_TRACE 1
////////////////////////////////////////////////////////////////////////////////
//                      BareMetal Class                                       //
////////////////////////////////////////////////////////////////////////////////

class BareMetal : public sc_module {
    public:
    BareMetal (sc_module_name name);
    sc_in <bool>          clk;
    sc_in <bool>          rst;
    tlmModule             Platform;
    tlmDecoder            bus1;
    tlmRam                ram1;
    tlmRam                ram2;
    riscv                 cpu1;
    DMA*                  dma1;
    ADAPTOR*		  adaptor1;

    sc_signal <bool> inter;

    // alternative processors:
    // arm                   cpu1;
    // mips32                cpu1;

    private:
    params paramsForcpu1() {
        params p;
        p.set("defaultsemihost", true);
        return p;
    }

}; /* BareMetal */


BareMetal::BareMetal (sc_module_name name)
    : sc_module (name)
    , Platform ("")
    , bus1 (Platform, "bus1", 3, 3)
    , ram1 (Platform, "ram1", 0x0fffffff)
    , ram2 (Platform, "ram2", 0xdfffffff)
    , cpu1 (Platform, "cpu1",  paramsForcpu1())
{

    dma1 = new DMA("DMA1");
    dma1 -> clk(clk);
    dma1 -> reset(rst);

    adaptor1 = new ADAPTOR("ADAPTOR");
    adaptor1->interrupt_in(inter);
    dma1->interrupt(inter);



    adaptor1 -> interrupt_out(cpu1.MExternalInterrupt);

    bus1.connect(cpu1.INSTRUCTION);
    bus1.connect(cpu1.DATA);
    dma1->master(*bus1.nextTargetSocket());

    bus1.connect(ram1.sp1, 0x0, 0x0fffffff);
    bus1.connect(ram2.sp1, 0x20000000, 0xffffffff);
    bus1.nextInitiatorSocket(0x10000000,0x1000000C)->bind(dma1->slave);
    }

int sc_main (int argc, char *argv[]) {

    sc_time clkprd(10,SC_NS),clkDly(0,SC_NS);
    sc_clock clk("clk",clkprd,0.50,clkDly,false);
    sc_signal<bool> rst;

    // start the CpuManager session
    session s;

    // create a standard command parser and parse the command line
    parser  p(argc, (const char**) argv);

    // create an instance of the platform
    BareMetal top ("top");

    top.clk(clk);
    top.rst(rst);

    rst=true;
 // start SystemC
    sc_start();


    return 0;
}

