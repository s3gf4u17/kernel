#include <drivers/keyboard.h>

KeyboardDriver::KeyboardDriver(InterruptManager* manager) : InterruptHandler(0x21,manager), dataport(0x60), commandport(0x64) {
}

void KeyboardDriver::Activate() {
    while(commandport.Read()&0x1) {
        dataport.Read();
    }
    commandport.Write(0xAE); // start sending keyboard interrupts
    commandport.Write(0x20); // gives current state
    uint8_t status = (dataport.Read() | 1) & ~0x10;
    commandport.Write(0x60); // set state
    dataport.Write(status);
    dataport.Write(0xF4);
}

void printf(const int8_t *str);
void printfHex(uint8_t hex);

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp) {
    uint8_t key = dataport.Read();
    if (key>=0x80) return esp; // ignore release key codes
    switch (key) {
        case 0x45: case 0xc5: case 0xfa: break;
        case 0x10: printf("q"); break;
        case 0x11: printf("w"); break;
        case 0x12: printf("e"); break;
        case 0x13: printf("r"); break;
        case 0x14: printf("t"); break;
        case 0x15: printf("y"); break;
        case 0x16: printf("u"); break;
        case 0x17: printf("i"); break;
        case 0x18: printf("o"); break;
        case 0x19: printf("p"); break;
        case 0x1e: printf("a"); break;
        case 0x1f: printf("s"); break;
        case 0x20: printf("d"); break;
        case 0x21: printf("f"); break;
        case 0x22: printf("g"); break;
        case 0x23: printf("h"); break;
        case 0x24: printf("j"); break;
        case 0x25: printf("k"); break;
        case 0x26: printf("l"); break;
        case 0x2c: printf("z"); break;
        case 0x2d: printf("x"); break;
        case 0x2e: printf("c"); break;
        case 0x2f: printf("v"); break;
        case 0x30: printf("b"); break;
        case 0x31: printf("n"); break;
        case 0x32: printf("m"); break;
        case 0x39: printf(" "); break;
        case 0x1c: printf("\n"); break;
        default:
            printf("key 0x");
            printfHex(key);
            break;
    }
    return esp;
}