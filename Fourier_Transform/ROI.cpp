// ROI.cpp: implementation of the ROI class.
//
//////////////////////////////////////////////////////////////////////

#include "ROI.h"
#include "stdio.h"
#include "stdlib.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void BaseROI::print()
{
  printf("%i %i %i %i -- %i %lf %i %i %i %i\n",x,y,sx,sy,thresh,radius,winSize,cRed,cGreen,cBlue);
}

ROI::ROI()
{
	this->roiList=0;
	this->numBaseROI=0;

}

ROI::~ROI()
{

}

int ROI::InROI(int px, int py)
{
	int x,y,sx,sy;
	for (int i=0;i<this->numBaseROI;i++)
	{
		x=roiList[i].x;
		y=roiList[i].y;
		sx=roiList[i].sx;
		sy=roiList[i].sy;
		if (px>x && py>y && px<x+sx && py<y+sy)
		{
			return i;
		}
	}

	return -1;

}

void ROI::CreateROIList(char* filename,Filter filter)
{
	FILE* file=fopen(filename,"rt+");
 	if(file==NULL) {
		printf("%s not valid ROI file.\n",filename);
		exit(0);
	}	
	fscanf(file,"%d",&this->numBaseROI);
	this->roiList=new BaseROI[numBaseROI];

	for (int i=0;i<numBaseROI;i++)
	{
		switch(filter) {
                        case AdaptiveThresh:
		        	fscanf(file,"%d %d %d %d %d %d\n",&roiList[i].x,&roiList[i].y,&roiList[i].sx,&roiList[i].sy,&roiList[i].thresh,&roiList[i].winSize);
                        break;
			case GrayArithmetic:
				fscanf(file,"%d %d %d %d %d\n",&roiList[i].x,&roiList[i].y,&roiList[i].sx,&roiList[i].sy,&roiList[i].thresh);
				roiList[i].winSize = -1; 
                        break;
			case Sobel:
			case SobelR:
			case SobelG:
			case SobelB:
			case SobelRGB:
			case SobelH:
			case SobelS:
			case SobelI:
			case SobelHSI:
                        case ROIThresh:
                        case LPF:
                        case HPF:
				fscanf(file,"%d %d %d %d %d\n",&roiList[i].x,&roiList[i].y,&roiList[i].sx,&roiList[i].sy,&roiList[i].thresh);
				roiList[i].winSize = -1; 
                        break;
                        case BPF:
				fscanf(file,"%d %d %d %d %d %d\n",&roiList[i].x,&roiList[i].y,&roiList[i].sx,&roiList[i].sy,&roiList[i].thresh,&roiList[i].winSize);
                        break;
                        case ColorThresh:
				fscanf(file,"%d %d %d %d %lf %d %d %d\n",&roiList[i].x,&roiList[i].y,&roiList[i].sx,&roiList[i].sy,&roiList[i].radius,&roiList[i].cRed,&roiList[i].cGreen,&roiList[i].cBlue);
				roiList[i].winSize = -1; 
                        break;
                        case ColorArithmetic: 
				fscanf(file,"%d %d %d %d %d %d %d\n",&roiList[i].x,&roiList[i].y,&roiList[i].sx,&roiList[i].sy,&roiList[i].cRed,&roiList[i].cGreen,&roiList[i].cBlue);
				roiList[i].winSize = -1; 
                        break;
                        case OutputROI:
				fscanf(file,"%d %d %d %d\n",&roiList[i].x,&roiList[i].y,&roiList[i].sx,&roiList[i].sy);
			break; 
		}
	}
}

void ROI::ReleaseROIList()
{
}
