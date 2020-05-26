#include <stdio.h>
#include <wiringPi.h>

// LED Pin - wiringPi pin 0 is BCM_GPIO 17.

#define LED_LEFT  29
#define LED_RIGHT 28
bool led_state = false;
int main (void)
{
  printf ("Raspberry Pi - Gertboard Blink\n") ;

  wiringPiSetup () ;

  pinMode (LED_LEFT, OUTPUT);
  pinMode(LED_RIGHT, OUTPUT);

  for (;;)
  {
    led_state = !led_state;
    digitalWrite(LED_LEFT, led_state); // On
    digitalWrite(LED_RIGHT, !led_state); // On
    delay (500) ;               
   
  }
  return 0 ;
}
