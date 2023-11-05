/*******************************************************************************
 * @file udp_server.h
 *
 * @author Clemens Haffner
 *
 * @brief UDP Auto-Discovery for eISCP
 *
 * @version 0.1
 * @date 2023-11-05
 *
 ******************************************************************************/

#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include <stdint.h>

int udp_init(void);
void udp_task(void);

#endif /* UDP_SERVER_H */
