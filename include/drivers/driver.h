#ifndef __KERNEL__DRIVERS__DRIVER_H
#define __KERNEL__DRIVERS__DRIVER_H

#include <common/types.h>

class Driver {
public:
    Driver();
    ~Driver();
    virtual int Reset();
    virtual void Activate();
    virtual void Deactivate();
};

class DriverManager {
private:
    Driver* drivers[256];
    int numDrivers;
public:
    DriverManager();
    ~DriverManager();
    void AddDriver(Driver*);
    void ActivateAll();
};

#endif