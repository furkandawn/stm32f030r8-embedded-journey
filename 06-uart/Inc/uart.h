#include "stdbool.h"

void USART2_SendChar(char c);
char USART2_ReceiveChar(void);
void USART2_SendString(const char *str);
void USART2_init(void);

bool RingBuffer_Get(uint8_t *data);

extern volatile bool buffer_flag;
