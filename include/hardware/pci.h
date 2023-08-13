#ifndef __KERNEL__HARDWARE__PCI_H
#define __KERNEL__HARDWARE__PCI_H

#include <hardware/port.h>
#include <drivers/driver.h>
#include <common/types.h>
#include <hardware/interrupts.h>

class PeripheralComponentInterconnectDeviceDescriptor {
public:
    uint32_t portbase;
    uint32_t interrupt;
    uint16_t bus;
    uint16_t device;
    uint16_t function;
    uint16_t vendor_id;
    uint16_t device_id;
    uint8_t class_id; 
    uint8_t subclass_id;
    uint8_t interface_id;
    uint8_t revision;
    PeripheralComponentInterconnectDeviceDescriptor();
    ~PeripheralComponentInterconnectDeviceDescriptor();
};

class PeripheralComponentInterconnectController {
    Port32Bit dataport;
    Port32Bit commandport;
public:
    PeripheralComponentInterconnectController();
    ~PeripheralComponentInterconnectController();
    uint32_t Read(uint16_t bus, uint16_t device, uint16_t function, uint32_t offset);
    void Write(uint16_t bus, uint16_t device, uint16_t function, uint32_t offset, uint32_t value);
    bool DeviceHasFunctions(uint16_t bus, uint16_t device);
    void SelectDrivers(DriverManager* driverManager);
    PeripheralComponentInterconnectDeviceDescriptor GetDeviceDescriptor(uint16_t bus, uint16_t device, uint16_t function);
};

#endif