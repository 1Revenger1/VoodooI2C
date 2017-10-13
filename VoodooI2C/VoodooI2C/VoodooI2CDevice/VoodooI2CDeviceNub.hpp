//
//  VoodooI2CDeviceNub.hpp
//  VoodooI2C
//
//  Created by Alexandre on 07/08/2017.
//  Copyright © 2017 Alexandre Daoud. All rights reserved.
//

#ifndef VoodooI2CDeviceNub_hpp
#define VoodooI2CDeviceNub_hpp

#include <IOKit/IOLib.h>
#include <IOKit/IOKitKeys.h>
#include <IOKit/IOService.h>
#include <IOKit/acpi/IOACPIPlatformDevice.h>
#include "../../../Dependencies/VoodooGPIO/VoodooGPIO/VoodooGPIO.h"

class VoodooI2CControllerDriver;

/* Implements a device nub to which an instance of a device driver may attach. Examples include <VoodooI2CHIDDevice>
 *
 * The members of this class are responsible for low-level interfacing with an I2C slave device. The public
 * member functions that are not inherited from <IOService> collectively form the so-called 'device API'.
 * Device drivers access the API in order to perform low-level hardware operations such as receiving interrupts
 * and I2C protocol messaging.
 */

class VoodooI2CDeviceNub : public IOService {
  OSDeclareDefaultStructors(VoodooI2CDeviceNub);

 public:
    /* Attaches <VoodooI2CController> class
     * @provider A <VoodooI2CControllerDriver> object
     * @child An <IOACPIPlatformDevice> representing the physical ACPI slave device.
     *
     * This function is called by <VoodooI2CControllerDriver> in order for the nub to attach itself
     * and perform its initialisation routine.
     *
     * @return *true* if the successfully attached and initialised, *false* otherwise
     */

    bool attach(IOService* provider, IOService* child);

    /* Disables an interrupt source
     * @source An <int> representing the index of the interrupt source in the case of APIC interrupts
     *
     * This function disables the interrupt source via the GPIO controller or passing it onto the APCI device
     * depending on the configuration
     *
     * @return *kIOReturnSuccess* if the interrupt is sucessfully disabled, *kIOReturnNoInterrupt* if the interrupt source
     * is invalid
     */

    IOReturn disableInterrupt(int source);
    IOReturn enableInterrupt(int source);
    IOReturn getInterruptType(int source, int *interruptType);
    IOWorkLoop* getWorkLoop();
    IOReturn readI2C(UInt8* values, UInt16 length);
    IOReturn registerInterrupt(int source, OSObject *target, IOInterruptAction handler, void *refcon);
    bool start(IOService* provider);
    void stop(IOService* provider);
    IOReturn unregisterInterrupt(int source);
    IOReturn writeI2C(UInt8* values, UInt16 length);
    IOReturn writeReadI2C(UInt8* write_buffer, UInt16 write_length, UInt8* read_buffer, UInt16 read_length);

 private:
    IOACPIPlatformDevice* acpi_device;
    IOCommandGate* command_gate;
    VoodooI2CControllerDriver* controller;
    const char* controller_name;
    VoodooGPIO* gpio_controller;
    int gpio_irq;
    UInt16 gpio_pin;
    UInt8 i2c_address;
    bool has_gpio_interrupts;
    bool use_10bit_addressing;
    IOWorkLoop* work_loop;

    IOReturn getDeviceResources();
    VoodooGPIO* getGPIOController();
    IOReturn readI2CGated(UInt8* values, UInt16* length);
    void releaseResources();
    IOReturn writeI2CGated(UInt8* values, UInt16* length);
    IOReturn writeReadI2CGated(UInt8* write_buffer, UInt16* write_length, UInt8* read_buffer, UInt16* read_length);
};


#endif /* VoodooI2CDeviceNub_hpp */
