#define IR_INPUT_PIN    4
#define NO_LED_FEEDBACK_CODE // activating saves 14 bytes program space

#include "TinyIRReceiver.hpp"

/*
   Helper macro for getting a macro definition as string
*/
#if !defined(STR_HELPER)
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#endif

#define A 0
#define B 1
#define C 2
#define D 3
#define NUMBER_OF_STEPS_PER_REV 512

volatile struct TinyIRReceiverCallbackDataStruct sCallbackData;
int state = 1;
int speed = 2;

void setup() {
  DDRB =  DDRB | B1111;
  initPCIInterruptForTinyReceiver();
}

void write(int a, int b, int c, int d) {
  digitalWrite(A, a);
  digitalWrite(B, b);
  digitalWrite(C, c);
  digitalWrite(D, d);
}

void onestepcw(uint8_t speed) {
  write(1, 1, 0, 0);
  delay(speed);
  write(0, 1, 1, 0);
  delay(speed);
  write(0, 0, 1, 1);
  delay(speed);
  write(1, 0, 0, 1);
  delay(speed);
}

void onestepccw(uint8_t speed) {
  PORTB = B1001;
  delay(speed);
  PORTB = B1100;
  delay(speed);
  PORTB = B0110;
  delay(speed);
  PORTB = B0011;
  delay(speed);
}

void loop() {
  //for(int i=0;i<NUMBER_OF_STEPS_PER_REV;i++){
  //        onestepcw(speed);
  //      }
  switch (state) {
    case 1:
      onestepcw(speed);
      break;
    case 2:
      onestepccw(speed);
      break;
    default:
      break;
  }

}

void handleReceivedTinyIRData(uint16_t aAddress, uint8_t aCommand, bool isRepeat)
{
  /*
     This is not allowed in ISR context for any kind of RTOS
     For Mbed we get a kernel panic and "Error Message: Semaphore: 0x0, Not allowed in ISR context" for Serial.print()
     for ESP32 we get a "Guru Meditation Error: Core  1 panic'ed" (we also have an RTOS running!)
  */
  switch (aCommand) {
    case 0x44:
      state = 1;
      break;
    case 0x40:
      state = 2;
      break;
    case 0x43:
      state = 0;
      break;
    case 0xC:
      speed = 9;
      break;
    case 0x18:
      speed = 8;
      break;
    case 0x5E:
      speed = 7;
      break;
    case 0x8:
      speed = 6;
      break;
    case 0x1C:
      speed = 5;
      break;
    case 0x5A:
      speed = 4;
      break;
    case 0x42:
      speed = 3;
      break;
    case 0x52:
      speed = 2;
      break;
    case 0x4A:
      speed = 1;
      break;
    default:
      break;
  }
}
