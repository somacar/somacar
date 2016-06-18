#include <stdio.h>
#include <wiringPi.h>
#include <wiringSerial.h>


int sp;
int cnt;

int main () {
	printf("\nRaspberry Pi -  wiringPi blink test\n"); 
	printf("Interrupt program with ctrl-c\n");

	if (wiringPiSetup() < 0) {
		fprintf(stderr, "Unable  to initialise wiringPi\n");
		return 1;
	} 

	sp = serialOpen ("/dev/ttyACM1", 9600) ;

	serialPrintf(sp,"This is from Raspberry serial\n\n");

	cnt = 0;
	while (1) {
		printf("Serial string was sent\n");
		serialPutchar(sp,'2'); // Send to Arduino number 2 
		delay(1200);
		cnt++;
	}

	for(;;);

}
