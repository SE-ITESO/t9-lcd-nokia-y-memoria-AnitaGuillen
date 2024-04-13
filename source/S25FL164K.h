/*
 * S25FL164K.h
 *
 *  Created on: 12 abr. 2024
 *      Author: prome
 */

#ifndef S25FL164K_H_
#define S25FL164K_H_

#include <stdint.h>

#define S25FL_READ_OPCODE         ((uint32_t)0x03U)
#define S25FL_TRANSFER_BAUDRATE           2000000U

void s25fl_read_data(uint32_t address, uint8_t * readData, uint32_t dataSize);
void s25fl_init(void);

#endif /* S25FL164K_H_ */
