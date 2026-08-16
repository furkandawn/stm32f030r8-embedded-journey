extern uint32_t SystemCoreClock;          /* System Clock Frequency (Core Clock) */

extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);

const uint8_t AHBPrescTable[16];
const uint8_t APBPrescTable[8];

uint32_t ClockGetHCLK(void);
uint32_t ClockGetPCLK(void);