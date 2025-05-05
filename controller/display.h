#include <Arduino.h>

#ifndef DISPLAY_H
#define DISPLAY_H

/**
 * Handle displaying digits on the board for both LED
 *
 * @param[in] dsPin Serial Data pin
 * @param[in] shcpPin SR Clock pin
 * @param[in] stcpPin R Clock pin
 * @param[in] npnPin1 Toggle LED1 transistor pin
 * @param[in] npnPin2  Toggle LED2 transistor pin
 * @param[in] digit1 Digit to display for LED 1
 * @param[in] digit2 Digit to display for LED 2
 */
void displayDigits(const int dsPin, const int shcpPin, const int stcpPin,
                   const int npnPin1, const int npnPin2,
                   const int digit1, const int digit2);
/**
 * Handle LED displaying
 *
 * @param[in] dsPin Serial Data pin
 * @param[in] shcpPin SR Clock pin
 * @param[in] stcpPin R Clock pin
 * @param[in] npnPin Toggle LED transistor pin
 * @param[in] digit Digit to display for LED
 */
void enableLED(const int dsPin, const int shcpPin, const int stcpPin,
               const int npnPin, const int digit);
#endif
