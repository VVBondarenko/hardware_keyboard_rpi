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

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <linux/uinput.h>

#define die(str, args...) do { \
        perror(str); \
        exit(EXIT_FAILURE); \
    } while(0)

__u16 key(int i, int j)
{
    switch(j){
        case 0:
            switch(i)
            {
                case 0: return KEY_Q;
                case 1: return KEY_W;
                case 2: return KEY_E;
                case 3: return KEY_R;
                case 4: return KEY_T;
                case 5: return KEY_Y;
                case 6: return KEY_U;
                case 7: return KEY_I;
                case 8: return KEY_O;
                case 9: return KEY_P;
                case 10:return KEY_0;
                case 11:return KEY_0;
            }
        case 1:
            switch(i)
            {
                case 0: return KEY_LEFTSHIFT;
                case 1: return KEY_A;
                case 2: return KEY_S;
                case 3: return KEY_D;
                case 4: return KEY_F;
                case 5: return KEY_G;
                case 6: return KEY_H;
                case 7: return KEY_J;
                case 8: return KEY_K;
                case 9: return KEY_L;
                case 10:return KEY_0;
                case 11:return KEY_0;
            }
        case 2:
            switch(i)
            {
                case 0: return KEY_LEFTCTRL;
                case 1: return KEY_LEFTALT;
                case 2: return KEY_Z;
                case 3: return KEY_X;
                case 4: return KEY_C;
                case 5: return KEY_V;
                case 6: return KEY_B;
                case 7: return KEY_N;
                case 8: return KEY_M;
                case 9: return KEY_0;
                case 10:return KEY_SPACE;
                case 11:return KEY_ENTER;
            }
    }
    return -1;
}

char KEY[]={29,25,24,23,22,21,14,13,12,7,11,10};
char lines[]={1,16,15};
int k,n;
int                    fd;
struct uinput_user_dev uidev;
struct input_event     evK,Syn;
int                    dx, dy;
int                    i,j;


void init()
{
    fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if(fd < 0)
        die("error: open");

    if(ioctl(fd, UI_SET_EVBIT, EV_KEY) < 0)
        die("error: ioctl");

    if(ioctl(fd, UI_SET_EVBIT, EV_SYN) < 0)
        die("error: ioctl");

    for(j=0;j<3;j++)for(i=0;i<12;i++)
        if(ioctl(fd, UI_SET_KEYBIT, key(i,j)) < 0)
            die("error: ioctl");

    memset(&uidev, 0, sizeof(uidev));
    snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "uinput-sample");
    uidev.id.bustype = BUS_USB;
    uidev.id.vendor  = 0x1;
    uidev.id.product = 0x1;
    uidev.id.version = 1;

    if(write(fd, &uidev, sizeof(uidev)) < 0)
        die("error: write");

    if(ioctl(fd, UI_DEV_CREATE) < 0)
        die("error: ioctl");

	memset(&Syn, 0, sizeof(Syn));                   //sync
	Syn.type  = EV_SYN;
	Syn.code  = SYN_REPORT;
	Syn.value = 0;

    usleep(6000);
}

void keyEvent(int i, int j, int val)
{
    memset(&evK, 0, sizeof(struct input_event));    //key down
    evK.type = EV_KEY;
    evK.code = key(i,j);
    evK.value = val;
    if(write(fd, &evK, sizeof(evK)) < 0)
        die("error: write");

    if(write(fd, &Syn, sizeof(Syn)) < 0)
        die("error: write");
}





int main(){
	init();
	if (wiringPiSetup() < 0)
	{
	    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
	    return 1 ;
	}

	for(k=0;k<12;k++) {
		pinMode (KEY[k],INPUT) ;//pinMode (KEY[1], OUTPUT) ;
		pullUpDnControl(KEY[k], PUD_DOWN);
	}
	for(k=0;k<3;k++) {
		pinMode (lines[k],OUTPUT) ;//pinMode (KEY[1], OUTPUT) ;
		digitalWrite(lines[k],LOW);
	}
	int position[12][3]={{0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0}},
	prevpos[12][3]={{0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0}};
	while(1)
	{
		for(n=0;n<3;n++)
		{
			digitalWrite(lines[n],HIGH);
			for(k=0;k<12;k++)
			{
				position[k][n]=digitalRead(KEY[k]);
				if(position[k][n]!=prevpos[k][n]){
					keyEvent(k, n, position[k][n]);printf("key %d %d",k,n);
				}
				prevpos[k][n]=position[k][n];
			}
			digitalWrite(lines[n],LOW);
			delay(50);
		}
		delay(10);
	}
	if(ioctl(fd, UI_DEV_DESTROY) < 0)
		die("error: ioctl");
	close(fd);
	return 0;
}
/*
ToDO:

Add something like "trigger" buttons (Shift as one of them, Num)(one of them Fn if needed)
stop using xdotool and use directly X11
find something that will look like better keyboard
*/
