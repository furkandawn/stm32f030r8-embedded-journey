/*
******************************************************************************
*
* This file is created to understand the linking process and the linker's job.
*
* Linker script for NUCLEO-F030R8 Board embedding STM32F030R8Tx Device from stm32f0 series
* 64KBytes FLASH | 8KBytes RAM
*
* The linker script plays a "Reference Manual" role for the startup file --"API" between Linker and Startup.
*
* 28 July 2026
* furkandawn
******************************************************************************
*/

/*
* ENTRY POINT
* Purpose: Find the symbol named "Reset_Handler" across all compiled relocatable object files.
* Note: The linker records "Reset_Handler" as the first instruction executed after reset.
*/
ENTRY(Reset_Handler)

/*
* MEMORY DEFINITION
* Purpose: Tell the linker the origin of memory addresses and their lengths.
* Note: (xrw) stands for execute-read-write, (rx) stands for read-execute only.
* Note2: The origin addresses are defined in the Reference Manual as well as their lengths.
*/
MEMORY
{
    RAM (xrw) : ORIGIN = 0x20000000, LENGTH = 8K
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 64K
}

/*
* SETTING STACK AND HEAP MEMORY
* Purpose: Calculate and define _estack (end of stack) address. The stack grows downwards.
* The .data and the .bss sections live on the bottom of the RAM.
* Purpose2: Min heap size and Min stack size is defined and it is used to check the stack and the heap size on build time.
* This way it is ensured that the data section doesn't take up the whole RAM, and there is enough space for stack.
* Note: The heap is almost never used, dynamic memory allocation isn't practical (also banned on some standards) .
*/
_estack = ORIGIN(RAM) + LENGTH(RAM); /* End of RAM, 0x20002000 */
_Min_Stack_Size = 0x400; /* Required amount of stack size, 1024 Bytes. */
_Min_Heap_Size = 0x200; /* Required amount of heap size, 512 Bytes. */

/*
* SETTING UP THE SECTIONS
* Sections are aligntd by 4 bytes. ALIGN(4) does this job, because Cortex-M0 is a 32-bit architecture.
* (>FLASH) used to write onto FLASH memory as well as (>RAM) used that the section goes into RAM.
* KEEP(*(.section_name)) -> Prevents linker from deleting the section under any circumstances.
* PROVIDE(symbol = address) -> Defines a symbol only if it hasn't been defined by an object file in the application project.
*/
SECTIONS
{
    /* interrupt service routine vector table, right at the start of FLASH memory */
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } >FLASH


    /*
    * The program code and other data into FLASH memory
    * glue code is to change CPU state so that ARM instruction set and THUMB instruction set function calls can be executed on the same program.
    */
    .text :
    {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        *(.glue_7)
        *(.glue_7t)

        . = ALIGN(4);
        _etext = .;
    } >FLASH


    /* Constant data into FLASH memory */
    .rodata :
    {
        . = ALIGN(4);
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
    } >FLASH


    /*
    * Initialized Data Section, saved on FLASH memory and loaded to RAM at the start of runtime
    * _sdata and _edata is used for RAM, VMA (Virtual Memory Address)
    * _sidata is used for FLASH memory, LMA (Load Memory Address)
    * "> RAM AT > FLASH" -> Defines the load address at FLASH and the execution address at RAM. (>RAM) for VMA, (AT > FLASH) for LMA.
    */
    .data :
    {
        . = ALIGN(4);
        _sdata = .;

        *(.data)
        *(.data*)

        . = ALIGN(4);
        _edata = .;
    } > RAM AT > FLASH

    /*
    * The starting address of initialized variables inside FLASH memory
    * The address is later used in startup file to copy the variables into RAM
    */
    _sidata = LOADADDR(.data);


    /*
    * Uninitialized Data Section, not saved on FLASH memory
    * The reason for duplicated _sbss and _ebss addresses, purely a historical naming convention across different compilers and C standard libraries.
    * COMMON is something about Legacy C, not so important.
    */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        __bss_start__ = _sbss;
        *(.bss)
        *(.bss*)
        *(COMMON)

        . = ALIGN(4);
        _ebss = .;
        __bss_end__ = _ebss;
    } >RAM


    /* User_heap_stack section, used to check that there is enough RAM left */
    ._user_heap_stack :
    {
    . = ALIGN(8);
    PROVIDE (end = .);
    PROVIDE (_end = .);
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
    } >RAM


    /* Create metadata (CPU architecture flags, ABI version), never loaded into target memory, stays in the .elf file for debugging. */
    .ARM.attributes 0 : { *(.ARM.attributes) }
}