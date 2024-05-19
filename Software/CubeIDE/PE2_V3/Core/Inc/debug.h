/*
 * debug.h
 *
 *  Created on: Apr 17, 2024
 *      Author: joran
 */

#ifndef INC_DEBUG_H_
#define INC_DEBUG_H_

#include "stm32f3xx_hal.h"
#include <errno.h>
#include <unistd.h>

int _write(int file, char *ptr, int len);


#endif /* INC_DEBUG_H_ */
