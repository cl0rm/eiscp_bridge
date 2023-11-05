/*******************************************************************************
 * @file tcp_server.h
 *
 * @author Clemens Haffner
 *
 * @brief TCP Server for eISCP
 *
 * @version 0.1
 * @date 2023-11-05
 *
 ******************************************************************************/

#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <stdint.h>

int tcp_init(void);
void tcp_task(void);
int tcp_send(uint8_t *pData, uint16_t u16Len);

#endif /* TCP_SERVER_H */
