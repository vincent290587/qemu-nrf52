//
// Created by vince on 09/05/2022.
//

#ifndef QEMU_NRF52_NRF_RTC_H
#define QEMU_NRF52_NRF_RTC_H

#include "hw/sysbus.h"
#include "qemu/timer.h"
#include "qom/object.h"
#include "hw/registerfields.h"

#define NRF_RTC_PER_SIZE   0x600
#define NRF5_RTC_REG_COUNT 4

REG32(RTC_TASKS_START, 0x000)
REG32(RTC_TASKS_STOP, 0x004)
REG32(RTC_TASKS_CLEAR, 0x008)
REG32(RTC_TRIGOVRFLW, 0x00C)

REG32(RTC_EVENT_TICK, 0x100)
REG32(RTC_EVENT_OVRFLW, 0x104)

// clear event X
REG32(RTC_EVENT_CMP0, 0x140)
REG32(RTC_EVENT_CMP1, 0x144)
REG32(RTC_EVENT_CMP2, 0x148)
REG32(RTC_EVENT_CMP3, 0x14C)

REG32(RTC_INTEN, 0x300)
    FIELD(RTC_INTEN, TICK, 0, 1)
    FIELD(RTC_INTEN, OVRFLW, 1, 1)
    FIELD(RTC_INTEN, CC0, 16, 1)
    FIELD(RTC_INTEN, CC1, 17, 1)
    FIELD(RTC_INTEN, CC2, 18, 1)
    FIELD(RTC_INTEN, CC3, 19, 1)
REG32(RTC_INTENSET, 0x304)
REG32(RTC_INTENCLR, 0x308)

REG32(RTC_EVTEN, 0x340)
    FIELD(RTC_EVTEN, TICK, 0, 1)
    FIELD(RTC_EVTEN, OVRFLW, 1, 1)
    FIELD(RTC_EVTEN, CC0, 16, 1)
    FIELD(RTC_EVTEN, CC1, 17, 1)
    FIELD(RTC_EVTEN, CC2, 18, 1)
    FIELD(RTC_EVTEN, CC3, 19, 1)
REG32(RTC_EVTENSET, 0x344)
REG32(RTC_EVTENCLR, 0x348)

REG32(RTC_CTR, 0x504)
REG32(RTC_PRESCALER, 0x508)

REG32(RTC_CC0, 0x540)
REG32(RTC_CC1, 0x544)
REG32(RTC_CC2, 0x548)
REG32(RTC_CC3, 0x54C)

#define TYPE_NRF_RTC "nrf_soc.rtc"
OBJECT_DECLARE_SIMPLE_TYPE(NRF5RtcState, NRF_RTC)

struct NRF5RtcState {
    SysBusDevice parent_obj;

    MemoryRegion iomem;
    qemu_irq irq;

    uint8_t id;
    
    QEMUTimer tick;

    bool enabled;
    bool running;

    uint32_t regs[NRF_RTC_PER_SIZE];

    MemoryRegion *downstream;
    AddressSpace downstream_as;
};

#endif //QEMU_NRF52_NRF_RTC_H
