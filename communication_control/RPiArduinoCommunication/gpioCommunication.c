#include <stdio.h>
#include <wiringPi.h>

#define GPIO1 4
#define GPIO2 5
#define GPIO3 6



int main (void) {
  if (wiringPiSetup () == -1)
    return 1 ;

  pinMode (GPIO1, OUTPUT);
  pinMode (GPIO2, OUTPUT);
  pinMode (GPIO3, OUTPUT);

  for (;;) {
    digitalWrite (GPIO1, HIGH) ; // On
    digitalWrite (GPIO2, HIGH) ; // On
    digitalWrite (GPIO3, HIGH) ; // On
    delay (1000) ; // ms
    digitalWrite (GPIO1, LOW) ; // Off
    digitalWrite (GPIO2, LOW) ; // Off
    digitalWrite (GPIO3, LOW) ; // Off
    delay (1000) ;
  }
  return 0 ;
}
