#include <drivers/driver.h>

int Driver::Reset() {
    return 0;
}

Driver::Driver() {

}

Driver::~Driver() {
    
}

void Driver::Activate() {

}

void Driver::Deactivate() {

}

DriverManager::DriverManager() {
    numDrivers = 0;
}

void DriverManager::AddDriver(Driver* drv) {
    drivers[numDrivers] = drv;
    numDrivers++;
}

void DriverManager::ActivateAll() {
    for (uint16_t i = 0 ; i < numDrivers ; i++) {
        drivers[i]->Activate();
    }
}