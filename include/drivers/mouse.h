#ifndef __KERNEL__DRIVERS__MOUSE_H
#define __KERNEL__DRIVERS__MOUSE_H

#include <common/types.h>
#include <hardware/interrupts.h>
#include <hardware/port.h>
#include <drivers/driver.h>

class MouseDriver : public InterruptHandler, public Driver {
    Port8Bit dataport;
    Port8Bit commandport;
    uint8_t buffer[3];
    uint8_t offset;
    uint8_t buttons;
public:
    MouseDriver(InterruptManager* manager);
    ~MouseDriver();
    virtual uint32_t HandleInterrupt(uint32_t esp);
    virtual void Activate();
};


#endif