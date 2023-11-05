#ifndef SERIAL_H
#define SERIAL_H

#include "stdint.h"

int serial_init(void);
void serial_task(void);
int serial_send(uint8_t *pData, uint16_t u16Len);

#endif /* SERIAL_H */