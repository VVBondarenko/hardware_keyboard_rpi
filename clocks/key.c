/*
             define from  wiringPi.h                     define from Board DVK511
                 3.3V | | 5V               ->                 3.3V | | 5V
                8/SDA | | 5V               ->                  SDA | | 5V
                9/SCL | | GND              ->                  SCL | | GND
                    7 | | 15/TX            ->                  IO7 | | TX
                  GND | | 16/RX            ->                  GND | | RX
                    0 | | 1                ->                  IO0 | | IO1
                    2 | | GND              ->                  IO2 | | GND
                    3 | | 4                ->                  IO3 | | IO4
                  VCC | | 5                ->                  VCC | | IO5
              MOSI/12 | | GND              ->                 MOSI | | GND
              MISO/13 | | 6                ->                 MISO | | IO6
               SCK/14 | | 10/CE0           ->                  SCK | | CE0
                  GND | | 11/CE1           ->                  GND | | CE1
              30/ID_SD| | 31/ID_SC         ->                ID_SD | | ID_SC
                    21| | GND              ->                  SDA | | 5V
                    22| | 26               ->                  SCL | | GND
                    23| | GND              ->                  IO7 | | TX
                    24| | 27               ->                  GND | | RX
                    25| | 28               ->                  IO0 | | IO1
                   GND| | 29               ->                  IO2 | | GND
                  */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <wiringPi.h>

char lines[]={25,24,23,22};
char number[]={26,27,29,28};

int i,j,n;
int Gx=0,Gy=0;

int main(int argc, char **argv){
	if (wiringPiSetup() < 0)
	{
	    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
	    return 1 ;
	}
    time_t rawtime;
    struct tm * ptm;
//    int delay_time = 6;//atoi(argv[1]);
//    int inputs = atoi(argv[2]);
//    int ins[] = {(inputs/1000), (inputs/100)%10, (inputs/10)%10, inputs%10};
    int ins[] = {(ptm->tm_hour+3)%24/10,(ptm->tm_hour+3)%24%10,
                ptm->tm_min/10,ptm->tm_min%10};

	for(i=0;i<4;i++) {
		pinMode     (lines  [i],OUTPUT);
		pinMode     (number [i],OUTPUT);
		//pinMode (KEY[1], OUTPUT) ;
		digitalWrite(lines  [i],LOW);
		digitalWrite(number [i],LOW);
	}
    for(n=0; n<=1000; n++)
    {
	    for(i=0;i<4;i++)
	    {
	        //retriving current time from system
            time ( &rawtime );
            ptm = gmtime ( &rawtime );
            ins[0] = (ptm->tm_hour+3)%24/10;
            ins[1] = (ptm->tm_hour+3)%24%10;
            ins[2] = ptm->tm_min/10;
            ins[3] = ptm->tm_min%10;
            
            //drawing current time on nixie
		    digitalWrite(lines[i],HIGH);
	        for(j=0;j<4;j++) {
		        digitalWrite(number [j],LOW);
	        }
            if(ins[i]==1)
            {
            	digitalWrite(number [0],HIGH);
            }
            if(ins[i]==2)
            {
            	digitalWrite(number [1],HIGH);
            }
            if(ins[i]==3)
            {
            	digitalWrite(number [1],HIGH);
                digitalWrite(number [0],HIGH);
            }
            if(ins[i]==4)
            {
            	digitalWrite(number [2],HIGH);
            }
            if(ins[i]==5)
            {
                digitalWrite(number [0],HIGH);
                digitalWrite(number [2],HIGH);
            }
            if(ins[i]==6)
            {
                //digitalWrite(number [0],HIGH);
                digitalWrite(number [1],HIGH);
                digitalWrite(number [2],HIGH);
            }
            if(ins[i]==7)
            {
                digitalWrite(number [0],HIGH);
                digitalWrite(number [1],HIGH);
                digitalWrite(number [2],HIGH);
            }
            if(ins[i]==8)
            {
            	digitalWrite(number [3],HIGH);
            }          
            if(ins[i]==9)
            {
                digitalWrite(number [0],HIGH);
                digitalWrite(number [3],HIGH);
            }                    
			delay(6);
		    digitalWrite(lines[i],LOW);
	    }
    }


}
