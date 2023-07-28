// ****************************************************
//
// Authors: Lex Harrison, Michael Zhu
// Date: 18/05/2023
// Description: A module of the ENCE361 project for
// UART functions
//
// ****************************************************

#ifndef UART_H_
#define UART_H_

// Initialize the UART port
void initUART(void);

// Sends a character buffer over the UART channel
void UARTSend(char *buffer);

// Sends/receives serial data
void debugSerial(void);

#endif
