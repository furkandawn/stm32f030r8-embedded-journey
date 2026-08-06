/**************************************************************************
*
* This file is created to demonstrate the functionality of the startup file
* The vector table at the bottom is copy-pasted from the vendor startup file.
*
* Startup File for NUCLEO-F030R8 Board embedding STM32F030R8Tx Device from stm32f0 series
*
* 29 July 2026
* furkandawn
***************************************************************************/

/*
* The four instructions below for the assembler, CPU never sees them.
*/
/* Use the unified ARM/Thumb syntax, bypasses the ARM and Thumb syntax differences. */
.syntax unified

/* Generate instructions valid for Cortex-M0 */
.cpu cortex-m0

/* Cortex-M0 has no floating-point hardware unit. This line tells the GCC that floating point operations are implemented in software. */
.fpu softvfp

/* Tells the assembler that everything after this line is a Thumb instruction. Cortex-M0 only executes 16-bit Thumb instructions. */
.thumb

/*
* .global exports a symbol so it can be referenced by other object files and resolved by the linker.
* in Assembly everything is hidden/private by default.
*/

/* global pointer to function vector table (ST's naming convention) */
.global g_pfnVectors


/*
* Every interrupt is initially made an alias of "Default_Handler".
* The default handler by default is nothing but an infinite while loop "while(1)".
* The interrupts can be overrode by a custom ISR created by the developer, until then it is set to Default_Handler.
* Why? -> To prevent the execution from branching to an undefined address, thus causing hard-faults or undefined behavior.
*/
.global Default_Handler


/*
* Start and End addresses of the sections in RAM and FLASH memory
* .data section is set to their initialized values, .bss section is cleared out to 0.
*/

/* start address of the initialized values at FLASH memory */
.word _sidata

/* .data section start address */
.word _sdata

/* .data section end address */
.word _edata

/* .bss section start address */
.word _sbss

/* .bss section end address */
.word _ebss

/*
* The very first code that runs on boot, Reset_Handler() itself, it prepares the environment by:
* fetching initialized variable values from FLASH and loading them into .data section inside RAM
* zero-initializing the .bss section, clearing out the garbage values.
*/

.section .text.Reset_Handler
.weak Reset_Handler
.type Reset_Handler, %function
Reset_Handler:
    /* Initialize the MSP, redundant in cortex-M0 since this is done by hardware automatically*/
    //ldr r0, =_estack
    //mov sp, r0

    /* branch to MCU specific hardware initialization */
    bl SystemInit

    /* Copy the initialized values from FLASH to RAM .data section */
    ldr r0, =_sdata
    ldr r1, =_edata
    ldr r2, =_sidata
    movs r3, #0 /* offset, copy one 32-bit word (4 bytes) per iteration */
    b LoopCopyDataCheck

LoopCopyData:
    ldr r4, [r2, r3]
    str r4, [r0, r3]
    adds r3, r3, #4

LoopCopyDataCheck:
    adds r4, r0, r3
    cmp r4, r1
    bcc LoopCopyData

/* zero-fill the whole .bss segment */
    ldr r2, =_sbss
    ldr r4, =_ebss
    movs r3, #0
    b LoopZerobssCheck

LoopZerobss:
    str r3, [r2]
    adds r2, r2, #4

LoopZerobssCheck:
    cmp r2, r4
    bcc LoopZerobss

/* Call the application's entry point.*/
    bl main

LoopForever:
    b LoopForever

    .size Reset_Handler, .-Reset_Handler

/*
* This is the code that gets called when the processor receives an
* unexpected interrupt.  This simply enters an infinite loop, preserving
* the system state for examination by a debugger.
*/
    .section .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
  b Infinite_Loop
  .size Default_Handler, .-Default_Handler

/* "a" means allocatable, "x" means executable. */
/******************************************************************************
*
* The STM32F030R8Tx vector table.  Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
*
******************************************************************************/
  .section .isr_vector,"a",%progbits
  .type g_pfnVectors, %object

g_pfnVectors:
  .word _estack
  .word Reset_Handler
  .word NMI_Handler
  .word HardFault_Handler
  .word	0
  .word	0
  .word	0
  .word	0
  .word	0
  .word	0
  .word	0
  .word	SVC_Handler
  .word	0
  .word	0
  .word	PendSV_Handler
  .word	SysTick_Handler
  .word	WWDG_IRQHandler               			/* Window Watchdog interrupt                             */
  .word	PVD_IRQHandler                			/* PVD and VDDIO2 supply comparator interrupt            */
  .word	RTC_IRQHandler                			/* RTC interrupts                                        */
  .word	FLASH_IRQHandler              			/* Flash global interrupt                                */
  .word	RCC_IRQHandler                			/* RCC global interruptr                                 */
  .word	EXTI0_1_IRQHandler            			/* EXTI Line[1:0] interrupts                             */
  .word	EXTI2_3_IRQHandler            			/* EXTI Line[3:2] interrupts                             */
  .word	EXTI4_15_IRQHandler           			/* EXTI Line15 and EXTI4 interrupts                      */
  .word	0                             			/* Reserved                                              */
  .word	DMA1_CH1_IRQHandler           			/* DMA1 channel 1 interrupt                              */
  .word	DMA1_CH2_3_IRQHandler         			/* DMA1 channel 2 and 3 interrupt                        */
  .word	DMA1_CH4_5_IRQHandler         			/* DMA1 channel 4 and 5 interrupt                        */
  .word	ADC_IRQHandler                			/* ADC interrupt                                         */
  .word	TIM1_BRK_UP_TRG_COM_IRQHandler			/* TIM1 break, update, trigger and commutation interrupt */
  .word	TIM1_CC_IRQHandler            			/* TIM1 Capture Compare interrupt                        */
  .word	0                             			/* Reserved                                              */
  .word	TIM3_IRQHandler               			/* TIM3 global interrupt                                 */
  .word	TIM6_IRQHandler               			/* TIM6 global interrupt                                 */
  .word	0                             			/* Reserved                                              */
  .word	TIM14_IRQHandler              			/* TIM14 global interrupt                                */
  .word	TIM15_IRQHandler              			/* TIM15 global interrupt                                */
  .word	TIM16_IRQHandler              			/* TIM16 global interrupt                                */
  .word	TIM17_IRQHandler              			/* TIM17 global interrupt                                */
  .word	I2C1_IRQHandler               			/* I2C1 global interrupt                                 */
  .word	I2C2_IRQHandler               			/* I2C2 global interrupt                                 */
  .word	SPI1_IRQHandler               			/* SPI1_global_interrupt                                 */
  .word	SPI2_IRQHandler               			/* SPI2 global interrupt                                 */
  .word	USART1_IRQHandler             			/* USART1 global interrupt                               */
  .word	USART2_IRQHandler             			/* USART2 global interrupt                               */
  .word	USART3_4_5_6_IRQHandler       			/* USART3, USART4, USART5, USART6 global interrupt       */
  .word	0                             			/* Reserved                                              */
  .word	USB_IRQHandler                			/* USB global interrupt                                  */
  .size g_pfnVectors, .-g_pfnVectors

/*******************************************************************************
*
* Provide weak aliases for each Exception handler to the Default_Handler.
* As they are weak aliases, any function with the same name will override
* this definition.
*
*******************************************************************************/

	.weak	NMI_Handler
	.thumb_set NMI_Handler,Default_Handler

	.weak	HardFault_Handler
	.thumb_set HardFault_Handler,Default_Handler

	.weak	SVC_Handler
	.thumb_set SVC_Handler,Default_Handler

	.weak	PendSV_Handler
	.thumb_set PendSV_Handler,Default_Handler

	.weak	SysTick_Handler
	.thumb_set SysTick_Handler,Default_Handler

	.weak	WWDG_IRQHandler
	.thumb_set WWDG_IRQHandler,Default_Handler

	.weak	PVD_IRQHandler
	.thumb_set PVD_IRQHandler,Default_Handler

	.weak	RTC_IRQHandler
	.thumb_set RTC_IRQHandler,Default_Handler

	.weak	FLASH_IRQHandler
	.thumb_set FLASH_IRQHandler,Default_Handler

	.weak	RCC_IRQHandler
	.thumb_set RCC_IRQHandler,Default_Handler

	.weak	EXTI0_1_IRQHandler
	.thumb_set EXTI0_1_IRQHandler,Default_Handler

	.weak	EXTI2_3_IRQHandler
	.thumb_set EXTI2_3_IRQHandler,Default_Handler

	.weak	EXTI4_15_IRQHandler
	.thumb_set EXTI4_15_IRQHandler,Default_Handler

	.weak	DMA1_CH1_IRQHandler
	.thumb_set DMA1_CH1_IRQHandler,Default_Handler

	.weak	DMA1_CH2_3_IRQHandler
	.thumb_set DMA1_CH2_3_IRQHandler,Default_Handler

	.weak	DMA1_CH4_5_IRQHandler
	.thumb_set DMA1_CH4_5_IRQHandler,Default_Handler

	.weak	ADC_IRQHandler
	.thumb_set ADC_IRQHandler,Default_Handler

	.weak	TIM1_BRK_UP_TRG_COM_IRQHandler
	.thumb_set TIM1_BRK_UP_TRG_COM_IRQHandler,Default_Handler

	.weak	TIM1_CC_IRQHandler
	.thumb_set TIM1_CC_IRQHandler,Default_Handler

	.weak	TIM3_IRQHandler
	.thumb_set TIM3_IRQHandler,Default_Handler

	.weak	TIM6_IRQHandler
	.thumb_set TIM6_IRQHandler,Default_Handler

	.weak	TIM14_IRQHandler
	.thumb_set TIM14_IRQHandler,Default_Handler

	.weak	TIM15_IRQHandler
	.thumb_set TIM15_IRQHandler,Default_Handler

	.weak	TIM16_IRQHandler
	.thumb_set TIM16_IRQHandler,Default_Handler

	.weak	TIM17_IRQHandler
	.thumb_set TIM17_IRQHandler,Default_Handler

	.weak	I2C1_IRQHandler
	.thumb_set I2C1_IRQHandler,Default_Handler

	.weak	I2C2_IRQHandler
	.thumb_set I2C2_IRQHandler,Default_Handler

	.weak	SPI1_IRQHandler
	.thumb_set SPI1_IRQHandler,Default_Handler

	.weak	SPI2_IRQHandler
	.thumb_set SPI2_IRQHandler,Default_Handler

	.weak	USART1_IRQHandler
	.thumb_set USART1_IRQHandler,Default_Handler

	.weak	USART2_IRQHandler
	.thumb_set USART2_IRQHandler,Default_Handler

	.weak	USART3_4_5_6_IRQHandler
	.thumb_set USART3_4_5_6_IRQHandler,Default_Handler

	.weak	USB_IRQHandler
	.thumb_set USB_IRQHandler,Default_Handler

	.weak	SystemInit
    