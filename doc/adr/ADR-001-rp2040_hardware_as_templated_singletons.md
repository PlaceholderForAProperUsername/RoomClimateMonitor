# 1. RP2040 Hardware as templated Singletons

Date: 2026-06-15

## Status

Accepted


## Context (Issue)

The RP2040's hardware must be modeled in software in order to be usable. To ensure safe usage of the hardware, status flags
should be used to make sure the hardware is configured for the use case (e.g. the uart is configured to transmit data when
data is to be sent, etc.). To keep the software model consistent with the hardware configuration, at most one instance per
controller should be allowed. 

As there are no hard time requirements in this project and the Raspberry Pi Pico has enough memory to be it not a concern, 
performance aspects are secondary.

The hardware is used to interact with external devices. To ensure proper usage, an interface needs to be defined to make
sure the right hardware for the device is used with the correct methods.


## Decision

The options are to use Singletons with the hardware address as a template parameter or to use static classes with templates 
and global/static status flags.

A Singleton guarantees there will be only one instance and with the hardware address as a template parameter, each controller
will be its own type, meaning each UART, SPI, etc. controller will have its own instance (UART0 will be a different type
than UART1 due to the template parameter). This makes the implementation of status flags easier, as every instance will
have its own status flags. The interface can be defined with abstract base classes, making the interface easy to read and
understand, but will introduce a performance cost due to runtime polymorphism.

The other option is to use completely static classes and define the interface via concepts and make the specific RP2040 
hardware controller a template parameter of the external device using that controller. The status flags could be implemented
as either global variables in a namespace or static variables of the static class, making it more complicated to get the
status flag for a specific instance.

The decision fell on the Singleton with the hardware address as a template parameter.

## Consequences

A Singleton with the hardware address as a template parameter should be easier to read and understand than static classes 
with concepts as an interface. The status flags can be easier implemented with the Singleton approach. There will be a 
performance cost due to runtime polymorphism, but this is unimportant in this project.


