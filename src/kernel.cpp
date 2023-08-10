#include <common/types.h>
#include <gdt.h>
#include <hardware/interrupts.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>

void printf(const int8_t *string) {
    uint16_t *VideoMemory = (uint16_t*)0xb8000; // pointer to video output memory

    static uint8_t x = 0 , y = 0;

    for (int32_t i = 0 ; string[i] != '\0' ; ++i) {
        switch (string[i]) {
            case '\n':
                y++;
                x=0;
                break;
            default:
                VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | string[i]; // save first 2 high bits (color information) - fun 0xC000
                x++;
                break;
        }
        if (x>=80) {
            x=0;
            y++;
        }
        if (y>=25) {
            for (y = 0 ; y < 25 ; y++) {
                for (x = 0 ; x < 80 ; x++) {
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | 0x0000;
                }
            }
            x = 0; y = 0;
        }
    }
}

void printfHex(uint8_t key) {
    char* foo = "00";
    char* hex = "0123456789abcdef";
    foo[0] = hex[(key>>4)&0xF];
    foo[1] = hex[key&0xF];
    printf(foo);
}

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;

extern "C" void callConstructors() {
    for (constructor* i = &start_ctors ; i != &end_ctors ; i++) (*i)();
}

extern "C" void kernelMain(void *multiboot_structure, uint32_t magic_number) {
    printf("hello kernel.\nmy name is Simon and i am looking for a job as a kernel engineer :)\n");
    GlobalDescriptorTable gdt;
    InterruptManager interrupts(&gdt);

    DriverManager drvManager;
    KeyboardDriver keyboard(&interrupts);
    drvManager.AddDriver(&keyboard);
    MouseDriver mouse(&interrupts);
    drvManager.AddDriver(&mouse);
    drvManager.ActivateAll();

    interrupts.Activate();
    while(1);
}