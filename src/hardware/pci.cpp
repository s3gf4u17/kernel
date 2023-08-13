#include <hardware/pci.h>

void printf(const int8_t *str);
void printfHex(uint8_t hex);

PeripheralComponentInterconnectController::PeripheralComponentInterconnectController() : dataport(0xcfc), commandport(0xcf8) {

}

PeripheralComponentInterconnectController::~PeripheralComponentInterconnectController() {

}

uint32_t PeripheralComponentInterconnectController::Read(uint16_t bus, uint16_t device, uint16_t function, uint32_t offset) {
    uint32_t id = (0x1<<31)|((bus&0xff)<<16)|((device&0x1f)<<11)|((function&0x07)<<8)|(offset&0xfc);
    commandport.Write(id);
    uint32_t result = dataport.Read();
    return result>>(8*(offset%4));
}

void PeripheralComponentInterconnectController::Write(uint16_t bus, uint16_t device, uint16_t function, uint32_t offset, uint32_t value) {
    uint32_t id = (0x1<<31)|((bus&0xff)<<16)|((device&0x1f)<<11)|((function&0x07)<<8)|(offset&0xfc);
    commandport.Write(id);
    dataport.Write(value);
}

bool PeripheralComponentInterconnectController::DeviceHasFunctions(uint16_t bus, uint16_t device) {
    return Read(bus,device,0,0x0e)&(1<<7);
}

void PeripheralComponentInterconnectController::SelectDrivers(DriverManager* driverManager) {
    for (int bus=0;bus<8;bus++) {for (int device=0;device<32;device++) {
        int functions = DeviceHasFunctions(bus,device)?8:1;
        for (int function=0;function<functions;function++) {
            PeripheralComponentInterconnectDeviceDescriptor dev = GetDeviceDescriptor(bus,device,function);
            if (dev.vendor_id==0||dev.vendor_id==0xffff) break;
            printf("PCI BUS ");
            printfHex(bus&0xff);
            printf(", DEVICE ");
            printfHex(device&0xff);
            printf(", FUNCTION ");
            printfHex(function&0xff);
            printf(" = VENDOR ");
            printfHex((dev.vendor_id&0xff00)>>8);
            printfHex(dev.vendor_id&0xff);
            printf(", DEVICE ");
            printfHex((dev.device_id&0xff00)>>8);
            printfHex(dev.device_id&0xff);
            printf("\n");
        }
    }}
}

PeripheralComponentInterconnectDeviceDescriptor::PeripheralComponentInterconnectDeviceDescriptor() {

}

PeripheralComponentInterconnectDeviceDescriptor::~PeripheralComponentInterconnectDeviceDescriptor() {
    
}

PeripheralComponentInterconnectDeviceDescriptor PeripheralComponentInterconnectController::GetDeviceDescriptor(uint16_t bus, uint16_t device, uint16_t function) {
    PeripheralComponentInterconnectDeviceDescriptor result;
    result.bus = bus;
    result.device = device;
    result.function = function;
    result.vendor_id = Read(bus,device,function,0x00);
    result.device_id = Read(bus,device,function,0x02);
    result.class_id = Read(bus,device,function,0x0b);
    result.subclass_id = Read(bus,device,function,0x0a);
    result.interface_id = Read(bus,device,function,0x09);
    result.revision = Read(bus,device,function,0x08);
    result.interrupt = Read(bus,device,function,0x3c);
    return result;
}