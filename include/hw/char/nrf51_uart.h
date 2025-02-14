/*
 * nRF51 SoC UART emulation
 *
 * Copyright (c) 2018 Julia Suvorova <jusual@mail.ru>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 or
 * (at your option) any later version.
 */

#ifndef NRF51_UART_H
#define NRF51_UART_H

#include "hw/sysbus.h"
#include "hw/ptimer.h"
#include "chardev/char-fe.h"
#include "hw/registerfields.h"
#include "qom/object.h"

#define UART_FIFO_LENGTH 6
#define UART_SIZE 0x1000

#define TYPE_NRF51_UART "nrf51_soc.uart"
OBJECT_DECLARE_SIMPLE_TYPE(NRF51UARTState, NRF51_UART)

#define NRF52832_UART_PER_SIZE 0x600

REG32(UART_STARTRX, 0x000)
REG32(UART_STOPRX, 0x004)
REG32(UART_STARTTX, 0x008)
REG32(UART_STOPTX, 0x00C)
REG32(UART_SUSPEND, 0x01C)

REG32(UART_CTS, 0x100)
REG32(UART_NCTS, 0x104)
REG32(UART_RXDRDY, 0x108)
REG32(UART_ENDRX, 0x110)
REG32(UART_TXDRDY, 0x11C)
REG32(UART_ENDTX, 0x120)
REG32(UART_ERROR, 0x124)
REG32(UART_RXTO, 0x144)

REG32(UART_TXSTARTED, 0x150)
REG32(UART_TXSTOPPED, 0x158)

REG32(UART_SHORTS, 0x200)

REG32(UART_INTEN, 0x300)
    FIELD(UART_INTEN, CTS, 0, 1)
    FIELD(UART_INTEN, NCTS, 1, 1)
    FIELD(UART_INTEN, RXDRDY, 2, 1)
    FIELD(UART_INTEN, ENDRX, 4, 1)
    FIELD(UART_INTEN, TXDRDY, 7, 1)
    FIELD(UART_INTEN, ENDTX, 8, 1)
    FIELD(UART_INTEN, ERROR, 9, 1)
    FIELD(UART_INTEN, RXTO, 17, 1)
    FIELD(UART_INTEN, TX_STARTED, 20, 1)
    FIELD(UART_INTEN, TX_STOPPED, 22, 1)
REG32(UART_INTENSET, 0x304)
REG32(UART_INTENCLR, 0x308)

REG32(UART_ERRORSRC, 0x480)
REG32(UART_ENABLE, 0x500)

REG32(UART_PSELRTS, 0x508)
REG32(UART_PSELTXD, 0x50C)
REG32(UART_PSELCTS, 0x510)
REG32(UART_PSELRXD, 0x514)

REG32(UART_RXD, 0x518)
REG32(UART_TXD, 0x51C)

REG32(UART_BAUDRATE, 0x524)

REG32(UART_RXD_PTR, 0x534)
REG32(UART_RXD_MAXCNT, 0x538)
REG32(UART_RXD_AMOUNT, 0x53C)

REG32(UART_TXD_PTR, 0x544)
REG32(UART_TXD_MAXCNT, 0x548)
REG32(UART_TXD_AMOUNT, 0x54C)

REG32(UART_CONFIG, 0x56C)

struct NRF51UARTState {
    SysBusDevice parent_obj;

    MemoryRegion iomem;
    CharBackend chr;
    qemu_irq irq;
    guint watch_tag;

    uint8_t tx_dma[64*1024];

    uint8_t rx_fifo[UART_FIFO_LENGTH];
    unsigned int rx_fifo_pos;
    unsigned int rx_fifo_len;

    uint32_t reg[NRF52832_UART_PER_SIZE];

    bool is_uarte;
    bool pending_tx_byte;
    bool enabled;

    ptimer_state *ptimer;

    MemoryRegion *downstream;
    AddressSpace downstream_as;
};

#endif
