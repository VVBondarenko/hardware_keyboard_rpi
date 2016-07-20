#include <stdio.h>
#include "xdo.h"

int main(){
	xdo_t *xdo=xdo_new(NULL);
	xdo_keysequence(xdo,CURRENTWINDOW, "hello, world",1000);
	return 0;
}

