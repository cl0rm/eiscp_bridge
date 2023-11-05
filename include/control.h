#ifndef CONTROL_H
#define CONTROL_H
#include <stdint.h>

void control_tcp_rx(uint8_t *pData, uint16_t u16Len);
void control_serial_rx(uint8_t *pData, uint16_t u16Len);

#endif /* CONTROL_H */
