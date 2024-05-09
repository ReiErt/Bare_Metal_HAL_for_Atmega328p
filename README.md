# Bare_Metal_HAL_for_Atmega328p
Hardware abstraction layer on bare metal for abstracting away the atmega328p's hardware and software interrupts. In the command line, simply give the length of time you require, and whether the timer should be event based or realised with callbacks.

## Why Hardware Abstraction Layer (HAL) at all?
Using the chip's hardware features (ports/DAC/ADC/timers/uart/interrupts/etc.) requires looking through the chip's datasheet to enable these features. This takes valuable time from the programmer's schedule. With a HAL, hardware settings can be abstracted away into a command line argument or GUI. This makes activating / deactivating certain hardware features very easy, regardless of the physical hardware on the host machine.

## How does it work
A HAL_Generation module generates a file "TimerHAL_cfg.h", which contains macros that either deactive or active certain microcontroller features. The microcontroller is then instantiated according to the macros at compile time. Note that the various features of the Microcontroll (ports/DAC/ADC/timers/uart/etc.) are contained in the various .h files you have already written for your microcontroller.



![General_Idea](/images/HAL_about.PNG)

## Drawback
The main drawback to HAL is extra code that must be written. Writing a HAL_Generation file is tidious and time consuming. But once it is written, you never need to deal with the hardware implementation again.

![Example execution](/images/terminal_example.PNG)