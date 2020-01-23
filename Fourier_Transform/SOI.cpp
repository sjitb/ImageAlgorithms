// SOI.cpp: implementation of the SOI class.
//
//////////////////////////////////////////////////////////////////////

#include "SOI.h"
#include "stdio.h"
#include "stdlib.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SOI::SOI()
{
	this->soiList=0;
	this->numBaseSOI=0;
}

SOI::~SOI()
{

}

int SOI::InSOI(int frame)
{
        int s, e;
	for (int i=0;i<this->numBaseSOI;i++)
	{
		s=soiList[i].start;
		e=soiList[i].end;
		if (frame>=s && frame<e)
		{
			return i;
		}
	}
	return -1;
}

void SOI::CreateSOIList(char* filename)
{
	FILE* file=fopen(filename,"rt+");
 	if(file==NULL) {
		printf("%s not valid ROI file.",filename);
		exit(0);
	}	
	fscanf(file,"%d",&this->numBaseSOI);
	this->soiList=new BaseSOI[numBaseSOI];

	for (int i=0;i<numBaseSOI;i++)
	{
		fscanf(file,"%d %d\n",&soiList[i].start,&soiList[i].end);
	}
}

void SOI::ReleaseSOIList()
{

}
