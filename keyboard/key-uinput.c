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

