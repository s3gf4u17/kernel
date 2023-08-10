#ifndef __KERNEL__DRIVERS__KEYBOARD_H
#define __KERNEL__DRIVERS__KEYBOARD_H

#include <common/types.h>
#include <hardware/interrupts.h>
#include <hardware/port.h>
#include <drivers/driver.h>

class KeyboardDriver : public InterruptHandler, public Driver {
    Port8Bit dataport;
    Port8Bit commandport;
public:
    KeyboardDriver(InterruptManager* manager);
    ~KeyboardDriver();
    virtual uint32_t HandleInterrupt(uint32_t esp);
    virtual void Activate();
};

#endif