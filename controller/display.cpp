#include "display.h"

const byte digitCodes[10] = {
    B01111110,  // 0: a b c d e f
    B00110000,  // 1:   b c
    B01101101,  // 2: a b   d e   g
    B01111001,  // 3: a b c d     g
    B00110011,  // 4:   b c     f g
    B01011011,  // 5: a   c d   f g
    B01011111,  // 6: a   c d e f g
    B01110000,  // 7: a b c
    B01111111,  // 8: a b c d e f g
    B01111011   // 9: a b c d   f g
};

/**
 * Handle displaying digits on the board for both LED
 */
void displayDigits(const int dsPin, const int shcpPin, const int stcpPin,
                   const int npnPin1, const int npnPin2,
                   const int digit1, const int digit2)
{
  enableLED(dsPin, shcpPin, stcpPin, npnPin1, digit1);
  enableLED(dsPin, shcpPin, stcpPin, npnPin2, digit2);
}

/**
 * Handle LED displaying
 */
void enableLED(const int dsPin, const int shcpPin, const int stcpPin,
               const int npnPin, const int digit)
{
  digitalWrite(npnPin, HIGH);

  digitalWrite(stcpPin, LOW);
  shiftOut(dsPin, shcpPin, LSBFIRST, digitCodes[digit]);
  digitalWrite(stcpPin, HIGH);
  
  delay(5);
  digitalWrite(npnPin, LOW);
  delay(5);
}

