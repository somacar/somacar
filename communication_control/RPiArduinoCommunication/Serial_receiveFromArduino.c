#include <stdio.h>
#include <wiringSerial.h>

int sp;
int cnt;

int main () {
	printf("\nRaspberry Pi -  wiringPi blink test\n"); 
	printf("Interrupt program with ctrl-c\n");

	sp = serialOpen ("/dev/ttyAMA0", 9600) ;

	serialPrintf(sp,"This is from Raspberry serial\n\n");
	char status;
	char dTstr;
	cnt = 0;
	while (1) { //
		status = serialGetchar(sp);

		printf("PC > RPi = %c \n", status); // print string or number
		if (status == '6') { // if tracking off
			printf("Detected from arduino (sent 6)\n") ;
		}

		// serialPutchar(sp, '1'); (send to arduino)
		delay(100);
	}

}
