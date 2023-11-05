#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <stdint.h>

int tcp_init(void);
void tcp_task(void);
int tcp_send(uint8_t *pData, uint16_t u16Len);

#endif /* TCP_SERVER_H */
