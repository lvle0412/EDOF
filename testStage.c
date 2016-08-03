#include <stdio.h>
#include <math.h>
#include <windows.h>
#include "MyLibs/Talk2Stage.h"


int main(){
	printf("Testing the Ludl stage.\nWait\n");
	HANDLE stage=InitializeUsbStage();
	
	
	int k=1;
	int key=0;
	int vx, vy;
	int px=0;
	int py=0;
	
	float x;
	float y;
	float t;
	
	int commandInterval=10;
	
	printf("Sending commands with %d ms interval\n",commandInterval);
	while (1) {
		Sleep(commandInterval);
		t=(float) k;
		
		//Generate circle in velocity space
		x=1000*sin(t/300);
		y=1000*cos(t/300);
		
		// Convert floats to int

		vx=(int) x;
		vy= (int) y;
		
		//Send velocity command to stage
		spinStage(stage,vx,vy);
		
		
		if (k%20==0){
			printf("\n%d commands sent",k);	
			printf(" current velocity: %d, %d\n",vx, vy);
			findStagePosition(stage,&px,&py);
			printf("current stage position: %d, %d\n",px,py);
		}
		printf(".");
		k=k+1;
	}



	return 0;
}
