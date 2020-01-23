// ROI.cpp: implementation of the ROI class.
//
//////////////////////////////////////////////////////////////////////

#include "ROI.h"
#include "stdio.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ROI::ROI()
{
	this->roiList=0;
	this->numBaseROI=0;

}

ROI::~ROI()
{

}

bool ROI::InROI(int px, int py)
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
			return true;
		}
		
	}

	return false;

}

void ROI::CreateROIList(char* filename)
{
	FILE* file=fopen(filename,"rt+");
	fscanf(file,"%d",&this->numBaseROI);
	this->roiList=new BaseROI[numBaseROI];

	for (int i=0;i<numBaseROI;i++)
	{
		fscanf(file,"%d %d %d %d\n",&roiList[i].x,&roiList[i].y,&roiList[i].sx,&roiList[i].sy);
	}

}

void ROI::ReleaseROIList()
{
}
