// ****************************************************
//
// Authors: P.J. Bones UCECE, Lex Harrison, Michael Zhu
// Date: 07/02/2018 - Updated: 18/05/2023
// Description: A module of the ENCE361 project for
// button initialization, de-bouncing and state control
//
// ****************************************************

#ifndef BUTTONS_H_
#define BUTTONS_H_

// STD
#include <stdint.h>
#include <stdbool.h>

// UP button
#define UP_BUT_PERIPH  SYSCTL_PERIPH_GPIOE
#define UP_BUT_PORT_BASE  GPIO_PORTE_BASE
#define UP_BUT_PIN  GPIO_PIN_0
#define UP_BUT_NORMAL  false

// DOWN button
#define DOWN_BUT_PERIPH  SYSCTL_PERIPH_GPIOD
#define DOWN_BUT_PORT_BASE  GPIO_PORTD_BASE
#define DOWN_BUT_PIN  GPIO_PIN_2
#define DOWN_BUT_NORMAL  false

// LEFT button
#define LEFT_BUT_PERIPH  SYSCTL_PERIPH_GPIOF
#define LEFT_BUT_PORT_BASE  GPIO_PORTF_BASE
#define LEFT_BUT_PIN  GPIO_PIN_4
#define LEFT_BUT_NORMAL  true

// RIGHT button
#define RIGHT_BUT_PERIPH  SYSCTL_PERIPH_GPIOF
#define RIGHT_BUT_PORT_BASE  GPIO_PORTF_BASE
#define RIGHT_BUT_PIN  GPIO_PIN_0
#define RIGHT_BUT_NORMAL  true

// SWITCH button
#define SWITCH_PERIPH  SYSCTL_PERIPH_GPIOA
#define SWITCH_PORT_BASE  GPIO_PORTA_BASE
#define SWITCH_PIN  GPIO_PIN_7
#define SWITCH_NORMAL  false

// RESET button
#define RESET_PERIPH  SYSCTL_PERIPH_GPIOA
#define RESET_PORT_BASE  GPIO_PORTA_BASE
#define RESET_PIN  GPIO_PIN_6
#define RESET_NORMAL  true

// Number of polls needed before the button updates state
#define NUM_BUT_POLLS 3

// Constants
enum butNames {UP = 0, DOWN, LEFT, RIGHT, SWITCH, RESET, NUM_BUTS};
enum butStates {RELEASED = 0, PUSHED, NO_CHANGE};

// Initialize button peripherals and pins
void initButtons (void);

// Update the button states and run the de-bounce algorithm
void updateButtons (void);

// Check for changes in the button using a flag system
uint8_t checkButton (uint8_t butName);

// Check if a button is down/up regardless of if the state has changed
bool checkButtonState(uint8_t butName);

#endif
