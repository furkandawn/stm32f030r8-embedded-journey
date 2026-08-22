# STM32F030R8 Embedded Journey

Studying Microcontrollers with an ARM Cortex-M0 on the STM32F030R8 (NUCLEO-F030R8).

---

### [01-led-blink-hal](01-led-blink-hal)
First contact with the chip through STM32CubeMX and HAL.  
Exposes the black boxes and the abstraction that comes with it.

`HAL` `CubeMX` `GPIO`

### [02-led-blink-register](02-led-blink-register)
Direct register-level GPIO control.  
RCC clock gates, MODER, BSRR, bit manipulation, and why `volatile` exists.

`memory-maps` `RM0091` `GPIO` `RCC` `register-level`

### [03-boot](03-boot)
What actually happens before `main()`.  
Custom startup file, vector table, Reset_Handler, `.data`/`.bss` initialization, linker script, and a complete toolchain without any IDE.

`build process` `startup` `linker-script` `vector-table` `Reset_Handler` `arm-none-eabi` `Make` `OpenOCD` `GDB`

### [04-system-init](04-system-init)
SystemInit() from scratch. Explores clock tree.  
HSI -> PLL, FLASH latency, SystemCoreClock, and the exact sequence required to switch the system clock safely.

`RCC` `PLL` `FLASH latency` `SystemInit` `clock-tree`

### [05-timers](05-timers)
Driving 4 LEDs with a single advanced timer. Interrupt-driven state-machine mini project with 2 timers.  
PWM, output compare toggle, alternate function mapping, interrupt-driven state machine, CPU sleep mode `__WFI()`.

`PWM` `Timer Channels` `Alternate Function` `ISR` `state-machine`

---

**Currently working on: I2C, NVIC/EXTI, and the analog side of the pins (push-pull vs open-drain, MOSFET drivers).**

*Each folder contains its own documentation and the source code.*

*This repository is a personal learning log, not a tutorial series.*