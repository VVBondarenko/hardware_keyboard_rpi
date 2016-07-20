#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <errno.h>
char KEY[]={29,25,24,23,22,21,14,13,12,7,11,10};
char lines[]={1,16,15};
int i,j;
int Gx=0,Gy=0;

char qwerty[12][12]={
			{'q','w','e','r','t','y','u','i','o','p','0','0'},
			{'0','a','s','d','f','g','h','j','k','l','0','0'},
			{'0','0','z','x','c','v','b','n','m','0','0','0'}
		};
int main()
{

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
//pullUpDnControl(KEY[1], PUD_DOWN);digitalWrite(KEY[1],HIGH);
    int position[12][3]={{0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0}},
	prevpos[12][3]={{0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0}};
	while(1)
	{
		for(j=0;j<3;j++)
		{
			digitalWrite(lines[j],HIGH);
			for(i=0;i<12;i++)
			{
				position[i][j]=digitalRead(KEY[i]);
				if(position[i][j]!=prevpos[i][j] && position[i][j]==0){
					if(j==1 && i==0)printf("keyup Shift\n");
					else if(j==2 && i==0)printf("keyup Ctrl\n");
					else if(j==2 && i==1)printf("keyup Alt\n");
					else if(j==0 && i==10)printf("key BackSpace\n");
					else if(j==2 && i==10)printf("key Space\n");
					else if(j==2 && i==11)printf("key Return\n");
					else printf("key %c\n",qwerty[j][i]);
				}
                		if(position[i][j]!=prevpos[i][j] && position[i][j]==1){
                                        if(j==1 && i==0)printf("keydown Shift\n");
                                        else if(j==2 && i==0)printf("keydown Ctrl\n");
					else if(j==2 && i==1)printf("keydown Alt\n");				}
				prevpos[i][j]=position[i][j];
				//digitalWrite(lines[j],LOW);
			}digitalWrite(lines[j],LOW);

		}
//		digitalWrite(KEY[1],LOW);
//		printf("switch off\n");
		delay(50);
	}
}
