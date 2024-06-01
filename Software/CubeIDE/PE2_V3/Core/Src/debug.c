/*
 * debug.c
 *
 *  Created on: 18 May 2024
 *      Author: joran
 */

#include "debug.h"

// Declare external UART handle
extern UART_HandleTypeDef huart3;

int _write(int file, char *ptr, int len) {
    HAL_StatusTypeDef xStatus;
    switch (file) {
    case STDOUT_FILENO: /*stdout*/
    case STDERR_FILENO: /*stderr*/
        xStatus = HAL_UART_Transmit(&huart3, (uint8_t*)ptr, len, HAL_MAX_DELAY);
        if (xStatus != HAL_OK) {
            errno = EIO;
            return -1;
        }
        break;
    default:
        errno = EBADF;
        return -1;
    }
    return len;
}


