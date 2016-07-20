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

#include <wiringPi.h>
//#include "xdo.h"
char KEY[]={29,25,24,23,22,21,14,13,12,7,11,10};
char lines[]={1,16,15};
int i,j,n;
int Gx=0,Gy=0;

//char qwerty[12][12]={
char qwerty[3][12][12]={
			{"q","w","e","r","t","y","u","i","o","p","0","0"},
			{"0","a","s","d","f","g","h","j","k","l","0","0"},
			{"0","0","z","x","c","v","b","n","m","0","0","0"}
		};
int main(){
	//xdo_t *xdo=xdo_new(NULL);
	//xdo_keysequence(xdo, CURRENTWINDOW,"",)
	if (wiringPiSetup() < 0)
	{
	    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
	    return 1 ;
	}

	for(i=0;i<12;i++) {
		pinMode (KEY[i],INPUT) ;//pinMode (KEY[1], OUTPUT) ;
		pullUpDnControl(KEY[i], PUD_DOWN);
	}
	for(i=0;i<3;i++) {
		pinMode (lines[i],OUTPUT) ;//pinMode (KEY[1], OUTPUT) ;
		digitalWrite(lines[i],LOW);
	}
	int position[12][3]={{0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0}},
	prevpos[12][3]={{0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0}};
	while(1)
	{
		for(j=0;j<3;j++){
			digitalWrite(lines[j],HIGH);
			for(i=0;i<12;i++){
				position[i][j]=digitalRead(KEY[i]);
				if(position[i][j]!=prevpos[i][j]){
					printf("key %d %d",i,j);
				}
				prevpos[i][j]=position[i][j];

			}
			digitalWrite(lines[j],LOW);
		}
		delay(50);
	}
}
/*
ToDO:

Add something like "trigger" buttons (Shift as one of them, Num)(one of them Fn if needed)
stop using xdotool and use directly X11
find something that will look like better keyboard
*/
