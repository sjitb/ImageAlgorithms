

#include <stdio.h>
#include "image.h"
#include "ipTool.h"

#define WIDTH  256
#define HEIGHT 256

#define MAXLEN 256
//global varialbles;
//function declaration;


//function body;
int main(int argc, char* argv[]){

	if (argc<2)
	{
		  fprintf(stderr, "Please tell me the filter's name ^^\n");
		  exit(0);
		
	}
	if (strncasecmp(argv[1],"binarize",MAXLEN)==0)
	{
		if (argc!=6)
		{
		  fprintf(stderr, "binarization require 4 arguments\n");
		  exit(0);
		}
		//binarize filter:
		char srcname[MAXLEN];
		char tgtname[MAXLEN];
		int threshhold;
		
		Image src;
		Image tgt;
		ROI roi;

		strcpy(srcname,argv[2]);	//source	
		strcpy(tgtname,argv[3]);	//target
		threshhold=atoi(argv[4]);	//thresh
		roi.CreateROIList(argv[5]);	//roi
		src.read(srcname);
		tgt.initialize(src.NR,src.NC);
		ipTool::binarize(src,tgt,threshhold,roi); //calls binarize in ipTools
		tgt.save(tgtname);
		exit(0);
	}

	//demo: output ROI
	if (strncasecmp(argv[1],"outputROI",MAXLEN)==0)
	{
		if (argc!=5)
		{
		  fprintf(stderr, "outputroi require 3 arguments\n");
		  exit(0);
		}
		//binarize filter:
		char srcname[MAXLEN];
		char tgtname[MAXLEN];
		
		ROI roi;
		Image src;
		Image tgt;

		
		strcpy(srcname,argv[2]);
		strcpy(tgtname,argv[4]);
		roi.CreateROIList(argv[3]);
		src.read(srcname);
		tgt.initialize(src.NR,src.NC);

		ipTool::OutputROI(src,roi,tgt);
		tgt.save(tgtname);
		exit(0);
	}
	

	if (strncasecmp(argv[1],"GreyThresh",MAXLEN)==0)
	{
		
		if (argc!=7)
		{
		  fprintf(stderr, "GreyThresh requires 5 arguments\n");
		  exit(0);
		}
		//GreyLevelThreshhold filter:
		char srcname[MAXLEN];
		char tgtname[MAXLEN];
		
		ROI roi;
		Image src;
		Image tgt;
		int threshold;
		int winsize;
		

		
		strcpy(srcname,argv[2]);
		strcpy(tgtname,argv[4]);
		threshold= atoi(argv[5]);
		winsize=atoi(argv[6]);
		roi.CreateROIList(argv[3]);
		src.read(srcname);
		tgt.initialize(src.NR,src.NC);
		ipTool::GreyThresh(src,roi,tgt,threshold,winsize);
		
		tgt.save(tgtname);
		exit(0);
	}
	

	if (strncasecmp(argv[1],"ColorBinarize",MAXLEN)==0)
	{
		if (argc!=9)
		{
		  fprintf(stderr, "ColorBinarization require 7 arguments\n");
		  exit(0);
		}
		//color-binarize filter:
		char srcname[MAXLEN];
		char tgtname[MAXLEN];
		int threshhold;
		
		Image src;
		Image tgt;
		ROI roi;
		int Cred;
		int Cgreen;
		int Cblue;


		strcpy(srcname,argv[2]);	//source	
		strcpy(tgtname,argv[3]);	//target
		threshhold=atoi(argv[4]);	//thresh
		roi.CreateROIList(argv[5]);	//roi
		Cred=atoi(argv[6]);
		Cgreen=atoi(argv[7]);
		Cblue=atoi(argv[8]);
		src.read(srcname);
		tgt.initialize(src.NR,src.NC);
		ipTool::Colorbinarize(src,tgt,threshhold,roi,Cred,Cgreen,Cblue); //calls color-binarize in ipTools
		tgt.save(tgtname);
		exit(0);
	}
	
	if (strncasecmp(argv[1],"Smoothing",MAXLEN)==0)
	{
		
		if (argc!=7)
		{
		  fprintf(stderr, "Smoothing requires 5 arguments\n");
		  exit(0);
		}
		//Smoothing filter:
		char srcname[MAXLEN];
		char tgtname[MAXLEN];
		
		ROI roi;
		Image src;
		Image tgt;
		int smoothresh;
		int size;
		

		
		strcpy(srcname,argv[2]);
		strcpy(tgtname,argv[4]);
		smoothresh= atoi(argv[5]);
		size=atoi(argv[6]);
		roi.CreateROIList(argv[3]);
		src.read(srcname);
		tgt.initialize(src.NR,src.NC);
		ipTool::Smoothing(src,roi,tgt,smoothresh,size);
		
		tgt.save(tgtname);
		exit(0);
	}


	fprintf(stderr, "Unknown Filter Name\n");
	return 0;

}
