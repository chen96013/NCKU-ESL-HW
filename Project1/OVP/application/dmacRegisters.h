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

//////////////////////// Registers for DMA Controller based on ARM PL081 ////////////////////////////

#ifndef DMAC_REGISTERS_H
#define DMAC_REGISTERS_H

#define DMA_BASE                  0x10000000
#define RAM1_BASE                 0x00000000
#define RAM2_BASE                 0xd0000000


#define DMA_INT_STATUS            0x00
#define DMA_INT_TC_STATUS         0x04
#define DMA_INT_TC_CLEAR          0x04
#define DMA_INT_ERROR_STATUS      0x0c
#define DMA_INT_ERROR_CLEAR       0x0c
#define DMA_RAW_INT_TC_STATUS     0x14
#define DMA_RAW_INT_ERROR_STATUS  0x18
#define DMA_EMBLD_CHNS            0x1c
#define DMA_CONFIGURATION         0x30

#define DMA_C0_SRC_ADDR           0x100
#define DMA_C0_DST_ADDR           0x104
#define DMA_C0_LL1                0x108
#define DMA_C0_CONTROL            0x10c
#define DMA_C0_CONFIGURATION      0x110

#define DMA_CHANNEL_STRIDE        0x20

#define DMA_C1_SRC_ADDR           (DMA_C0_SRC_ADDR      + DMA_CHANNEL_STRIDE)
#define DMA_C1_DST_ADDR           (DMA_C0_DST_ADDR      + DMA_CHANNEL_STRIDE)
#define DMA_C1_LL1                (DMA_C0_LL1           + DMA_CHANNEL_STRIDE)
#define DMA_C1_CONTROL            (DMA_C0_CONTROL       + DMA_CHANNEL_STRIDE)
#define DMA_C1_CONFIGURATION      (DMA_C0_CONFIGURATION + DMA_CHANNEL_STRIDE)

// Bit manipulation
#define MASK(_BITS)                     ((_BITS) == 32 ? -1U : (1U<<MIN((_BITS),31))-1)
#define PART_SELECT(_V,_LEFT,_RIGHT)    (((_V)>>(_RIGHT)) & MASK((_LEFT)-(_RIGHT)+1))
#define BIT_SELECT(_V,_BIT)             (((_V)>>(_BIT)) & 1)

// Control Bits
#define DMA_CONTROL_I(_R)         BIT_SELECT(_R,31)
#define DMA_CONTROL_TRAN_SIZE(_R) PART_SELECT(_R,11,0)

// Configuration bits
#define DMA_CONFIG_HALT(_R)       BIT_SELECT(_R,18)
#define DMA_CONFIG_ACT(_R)        BIT_SELECT(_R,17)
#define DMA_CONFIG_ITC(_R)        BIT_SELECT(_R,15)
#define DMA_CONFIG_ENA(_R)        BIT_SELECT(_R,0)

#endif
