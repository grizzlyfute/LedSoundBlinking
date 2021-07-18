#ifndef CONSTANTS_H
#define CONSTANTS_H

// General constants purpose

// Should be power of two
#define SAMPLE_SIZE 64

// #define SYS_TICKS_FREQ 2083.3 Hz

#define ANALOG_LOWAMP  (1 << 4)
#define ANALOG_HIGHAMP (1 << 3)

#define LED_GREEN (1 << 2)
#define LED_RED   (1 << 1)
#define LED_BLUE  (1 << 0)
#define LED_ALL (LED_GREEN | LED_RED | LED_BLUE)

#endif // CONSTANTS_H
