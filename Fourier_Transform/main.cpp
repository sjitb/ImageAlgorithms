
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <rfftw.h>

#include<string>
using namespace std;

#include "image.h"
#include "ipTool.h"
#include "mpeg.h"

#define WIDTH  256
#define HEIGHT 256

#define MAXLEN 256
//global varialbles;
//function declaration;
#define LINE_LENGTH 1024

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

int saveFrame (int, int, char*, Image&);

int main(int argc, char* argv[]){

	if (argc<2)
	{
		fprintf(stderr, "Please tell me the filter's name ^^\n");
		exit(0);

	}

	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Filtering in Fourier Domain;
	  Note that Convolution in Space Domain equals multiplication in Fourier Domain,
	  Filtering: 
		(1). a (image in Space Domain)-->FFT-->A (image in Fourier Domain);
		(2). A*B=C (A multiply the filter in Fourier Domain);
		(3). C (the result of filtering in Fourier Domain) --> IFFT --> c (the result of filtering in Space Domain, an image);
			src is a *.pgm image */

	if(strncasecmp(argv[1],"FFT",MAXLEN)==0)
	{
		if(argc!=1+1+1+1+1+1) {
			printf("Fourier Transform require 4 arguments: <in> <roi> <out1> <out2>\n");
			exit(0);
		}
		char srcname[MAXLEN];
		char tgtname[MAXLEN];
		char tgtname2[MAXLEN];
		Image src;
		Image tgt,tgtFinal,tgtFinal2;
		ROI roi;
		strcpy(srcname,argv[2]);
		src.read(srcname);
		tgt.initialize(src.NR,src.NC);
		tgtFinal.initialize(src.NR,src.NC);
		tgtFinal2.initialize(src.NR,src.NC);

  		if (strstr(argv[1], "FFT") != NULL)
		  roi.CreateROIList(argv[3],ROI::LPF);

  		if (strstr(argv[2], ".ppm") != NULL) {/* PPM Color File*/
			ipTool::ppm2i(src, tgt);
			ipTool::PassFilter(tgt,roi,tgtFinal,tgtFinal2,argv[1]);  
		} 
		else {	
			ipTool::PassFilter(src,roi,tgtFinal,tgtFinal2,argv[1]);   
		}

		/* save 'tgt', the output image is the same with input image in this case */
		sprintf(tgtname,"%s.pgm", argv[4]);
		sprintf(tgtname2,"%s.pgm", argv[5]);
		tgtFinal.save(tgtname);
		tgtFinal2.save(tgtname2);
		exit(0);
	}

	if(strncasecmp(argv[1],"HPF",MAXLEN)==0||strncasecmp(argv[1],"BPF",MAXLEN)==0||strncasecmp(argv[1],"LPF",MAXLEN)==0)
	{
		if(argc!=1+1+1+1+1) {
			printf("Low, Band and High Pass Filters require 3 arguments: <in> <roi> <out>\n");
			exit(0);
		}
		char srcname[MAXLEN];
		char tgtname[MAXLEN];
		char tgtname2[MAXLEN];
		Image src;
		Image tgt,tgtFinal,tgtFinal2;
		ROI roi;
		strcpy(srcname,argv[2]);
		strcpy(tgtname,argv[4]);
		//strcpy(tgtname2,argv[5]);
		src.read(srcname);
		tgt.initialize(src.NR,src.NC);
		tgtFinal.initialize(src.NR,src.NC);
		tgtFinal2.initialize(src.NR,src.NC);

  		if (strstr(argv[1], "BPF") != NULL)
		  roi.CreateROIList(argv[3],ROI::BPF);
  		if (strstr(argv[1], "HPF") != NULL)
		  roi.CreateROIList(argv[3],ROI::HPF);
  		if (strstr(argv[1], "LPF") != NULL)
		  roi.CreateROIList(argv[3],ROI::LPF);

  		if (strstr(argv[2], ".ppm") != NULL) {/* PPM Color File*/
			ipTool::ppm2i(src, tgt);
			ipTool::PassFilter(tgt,roi,tgtFinal,tgtFinal2,argv[1]);  
		} 
		else {	
			ipTool::PassFilter(src,roi,tgtFinal,tgtFinal2,argv[1]);   
		}

		/* save 'tgt', the output image is the same with input image in this case */
		sprintf(tgtname,"%s.pgm", argv[4]);
		tgtFinal.save(tgtname);
		//sprintf(tgtname,"tmp.pgm");
		//tgtFinal2.save(tgtname);

		exit(0);
	}

	if (strncasecmp(argv[1],"FFTVideo",MAXLEN)==0)
	{
		// prog filt vidIn roi vidOut vidOut
		if (argc!=6)
		{
			fprintf(stderr, "FFTVideo requires 3 arguments.\n");
			exit(0);
		}
		//init:
		FILE 		*mpeg;
		ImageDesc 	img;
		Boolean		moreframes = TRUE;
		char 		*pixels;
		Image		src,tgt,tgt2,tgt3,tgt4,tgt5;
		int			count=0;
		char		tgtname[100];
		mpeg = fopen (argv[2], "rb");
		ROI roi;
		roi.CreateROIList(argv[3],ROI::LPF);
		SetMPEGOption (MPEG_DITHER, FULL_COLOR_DITHER);
		OpenMPEG (mpeg, &img);
		src.initialize(img.Height, img.Width);
		tgt.initialize(img.Height, img.Width);
		tgt2.initialize(img.Height, img.Width);
		tgt3.initialize(img.Height, img.Width);
		tgt4.initialize(img.Height, img.Width);
		tgt5.initialize(img.Height, img.Width);
		pixels = (char *) malloc (img.Size);
		fprintf(stderr,"\nBeginning to extract frames...\n");
		while (moreframes)
		{
			moreframes = GetMPEGFrame(pixels);
			if (saveFrame (img.Height, img.Width, pixels, src) != 0) 
			{
				(void) fprintf (stderr,"\nError Saving Frame\nExitting...\n");
				exit (-2);
			}
			ipTool::ppm2pgm(src,tgt);
			ipTool::PassFilter(tgt,roi,tgt2,tgt3,"FFT"); 
			ipTool::pgm2ppm(tgt2,tgt4);
			ipTool::pgm2ppm(tgt3,tgt5);
			sprintf(tgtname,"%s%d.ppm", argv[4], count);
			tgt4.save(tgtname);
			sprintf(tgtname,"%s%d.ppm", argv[5], count++);
			tgt5.save(tgtname);
		}
		fprintf(stderr,"\n***DONE***\n\n");

		char ccount[10000];
		sprintf(ccount,"%i",count-1);
		string encodeCmd = ("./encodeframes_grad 0 "+string(ccount)+" "+string(argv[4])+" "+string(argv[4])+".mpeg"); 
		printf("%s\n",encodeCmd.c_str());
		system(encodeCmd.c_str()); 
		encodeCmd = ("./encodeframes_grad 0 "+string(ccount)+" "+string(argv[5])+" "+string(argv[5])+".mpeg"); 
		printf("%s\n",encodeCmd.c_str());
		system(encodeCmd.c_str()); 

		CloseMPEG ();
		fclose (mpeg);
		src.flushMemory();
		exit(0);
	}

	if (strncasecmp(argv[1],"LPFVideo",MAXLEN)==0||strncasecmp(argv[1],"HPFVideo",MAXLEN)==0||strncasecmp(argv[1],"BPFVideo",MAXLEN)==0)
	{
		// prog filt vidIn roi vidOut
		if (argc!=5)
		{
			fprintf(stderr, "[L,B,H]PFVideo requires 3 arguments.\n");
			exit(0);
		}
		//init:
		FILE 		*mpeg;
		ImageDesc 	img;
		Boolean		moreframes = TRUE;
		char 		*pixels;
		Image		src,tgt,tgt2,tgt3,tgt4,tgt5;
		int			count=0;
		char		tgtname[100];
		mpeg = fopen (argv[2], "rb");
		ROI roi;
		roi.CreateROIList(argv[3],ROI::BPF);
		SetMPEGOption (MPEG_DITHER, FULL_COLOR_DITHER);
		OpenMPEG (mpeg, &img);
		src.initialize(img.Height, img.Width);
		tgt.initialize(img.Height, img.Width);
		tgt2.initialize(img.Height, img.Width);
		tgt3.initialize(img.Height, img.Width);
		tgt4.initialize(img.Height, img.Width);
		tgt5.initialize(img.Height, img.Width);
		pixels = (char *) malloc (img.Size);
		fprintf(stderr,"\nBeginning to extract frames...\n");
		while (moreframes)
		{
			moreframes = GetMPEGFrame(pixels);
			if (saveFrame (img.Height, img.Width, pixels, src) != 0) 
			{
				(void) fprintf (stderr,"\nError Saving Frame\nExitting...\n");
				exit (-2);
			}
			ipTool::ppm2pgm(src,tgt);
			char filt[3];
			for(int i = 0; i < 3; i++)
				filt[i] = argv[1][i];
			ipTool::PassFilter(tgt,roi,tgt2,tgt3,filt); 
			ipTool::pgm2ppm(tgt2,tgt4);
			sprintf(tgtname,"%s%d.ppm", argv[4], count++);
			tgt4.save(tgtname);
		}
		fprintf(stderr,"\n***DONE***\n\n");

		char ccount[10000];
		sprintf(ccount,"%i",count-1);
		string encodeCmd = ("./encodeframes_grad 0 "+string(ccount)+" "+string(argv[4])+" "+string(argv[4])+".mpeg"); 
		printf("%s\n",encodeCmd.c_str());
		system(encodeCmd.c_str()); 

		CloseMPEG ();
		fclose (mpeg);
		src.flushMemory();
		exit(0);
	}

//Image Filters for Assignment 3. See below for video filters.
	if (strncasecmp(argv[1],"Sobel",MAXLEN)==0)
	{
		if (argc!=6) {
			fprintf(stderr, "Sobel require 4 arguments: <src> <roiFile> <tgt> <tgt>\n");
				exit(0);
			}
		Image		src, tgt, tgtFinal, tgtFinal2;
		char		tgtname[100];
		ROI 		roi;
		src.read(argv[2]);
		tgt.initialize(src.NR,src.NC);
		tgtFinal.initialize(src.NR,src.NC);
		tgtFinal2.initialize(src.NR,src.NC);
		roi.CreateROIList(argv[3],ROI::Sobel);
  		if (strstr(argv[2], ".ppm") != NULL) {/* PPM Color File*/
			ipTool::Sobel(src,roi,tgtFinal,tgtFinal2);  
		} 
		else {	
			ipTool::pgm2ppm(src, tgt);
			ipTool::Sobel(tgt,roi,tgtFinal,tgtFinal2);   
		}
		sprintf(tgtname,"%s.pgm", argv[4]);
		tgtFinal.save(tgtname);
		sprintf(tgtname,"%s.pgm", argv[5]);
		tgtFinal2.save(tgtname);
		exit(0);
	}
	if (strncasecmp(argv[1],"SobelR",MAXLEN)==0)
	{
		if (argc!=6) {
			fprintf(stderr, "SobelR require 4 arguments: <src> <roiFile> <tgt> <tgt>\n");
			exit(0);
		}
		Image		src, tgt, tgtFinal,tgtFinal2;
		char		tgtname[100];
		ROI 		roi;
		src.read(argv[2]);
		tgt.initialize(src.NR,src.NC);
		tgtFinal.initialize(src.NR,src.NC);
		tgtFinal2.initialize(src.NR,src.NC);
		roi.CreateROIList(argv[3],ROI::SobelR);
  		if (strstr(argv[2], ".ppm") != NULL) {/* PPM Color File*/
			ipTool::SobelR(src,roi,tgtFinal,tgtFinal2);  
			sprintf(tgtname,"%s.ppm", argv[4]);
			tgtFinal.save(tgtname);
			sprintf(tgtname,"%s.ppm", argv[5]);
			tgtFinal2.save(tgtname);
		} 
		else {	
			ipTool::pgm2ppm(src, tgt);
			ipTool::SobelR(tgt,roi,tgtFinal,tgtFinal2);
			sprintf(tgtname,"%s.pgm", argv[4]);
			tgtFinal.save(tgtname);
			sprintf(tgtname,"%s.pgm", argv[5]);
			tgtFinal2.save(tgtname);
		}
		exit(0);
	}
	if (strncasecmp(argv[1],"SobelG",MAXLEN)==0)
	{
		if (argc!=6) {
			fprintf(stderr, "SobelG require 4 arguments: <src> <roiFile> <tgt> <tgt>\n");
			exit(0);
		}
		Image		src, tgt, tgtFinal, tgtFinal2;
		char		tgtname[100];
		ROI 		roi;
		src.read(argv[2]);
		tgt.initialize(src.NR,src.NC);
		tgtFinal.initialize(src.NR,src.NC);
		tgtFinal2.initialize(src.NR,src.NC);
		roi.CreateROIList(argv[3],ROI::SobelG);
  		if (strstr(argv[2], ".ppm") != NULL) {/* PPM Color File*/
			ipTool::SobelG(src,roi,tgtFinal,tgtFinal2);  
			sprintf(tgtname,"%s.ppm", argv[4]);
			tgtFinal.save(tgtname);
			sprintf(tgtname,"%s.ppm", argv[5]);
			tgtFinal2.save(tgtname);
		} 
		else{
			ipTool::pgm2ppm(src, tgt);
			ipTool::SobelG(tgt,roi,tgtFinal,tgtFinal2);
			sprintf(tgtname,"%s.pgm", argv[4]);
			tgtFinal.save(tgtname);
			sprintf(tgtname,"%s.pgm", argv[5]);
			tgtFinal2.save(tgtname);
		}   
		exit(0);
	}
	if (strncasecmp(argv[1],"SobelB",MAXLEN)==0)
	{
		if (argc!=6) {
			fprintf(stderr, "SobelB require 4 arguments: <src> <roiFile> <tgt> <tgt>\n");
			exit(0);
		}
		Image		src, tgt, tgtFinal,tgtFinal2;
		char		tgtname[100];
		ROI 		roi;
		src.read(argv[2]);
		tgt.initialize(src.NR,src.NC);
		tgtFinal.initialize(src.NR,src.NC);
		tgtFinal2.initialize(src.NR,src.NC);
		roi.CreateROIList(argv[3],ROI::SobelB);
  		if (strstr(argv[2], ".ppm") != NULL) {/* PPM Color File*/
			ipTool::SobelB(src,roi,tgtFinal,tgtFinal2);  
			sprintf(tgtname,"%s.ppm", argv[4]);
			tgtFinal.save(tgtname);
			sprintf(tgtname,"%s.ppm", argv[5]);
			tgtFinal2.save(tgtname);
		} 
		else{
			ipTool::pgm2ppm(src, tgt);
			ipTool::SobelG(tgt,roi,tgtFinal,tgtFinal2);
			sprintf(tgtname,"%s.pgm", argv[4]);
			tgtFinal.save(tgtname);
			sprintf(tgtname,"%s.pgm", argv[5]);
			tgtFinal2.save(tgtname);
		}   
		exit(0);
	}
	if (strncasecmp(argv[1],"SobelRGB",MAXLEN)==0)
	{
		if (argc!=6) {
			fprintf(stderr, "SobelRGB require 4 arguments: <src> <roiFile> <tgt> <tgt>\n");
			exit(0);
		}
		Image		src, tgt, tgtFinal,tgtFinal2;
		char		tgtname[100];
		ROI 		roi;
		src.read(argv[2]);
		tgt.initialize(src.NR,src.NC);
		tgtFinal.initialize(src.NR,src.NC);
		tgtFinal2.initialize(src.NR,src.NC);
		roi.CreateROIList(argv[3],ROI::SobelRGB);
  		if (strstr(argv[2], ".ppm") != NULL) {/* PPM Color File*/
			ipTool::SobelRGB(src,roi,tgtFinal,tgtFinal2);  
			sprintf(tgtname,"%s.ppm", argv[4]);
			tgtFinal.save(tgtname);
			sprintf(tgtname,"%s.ppm", argv[5]);
			tgtFinal2.save(tgtname);
		} 
		else{
			ipTool::pgm2ppm(src, tgt);
			ipTool::SobelG(tgt,roi,tgtFinal,tgtFinal2);
			sprintf(tgtname,"%s.pgm", argv[4]);
			tgtFinal.save(tgtname);
			sprintf(tgtname,"%s.pgm", argv[5]);
			tgtFinal2.save(tgtname);
		}   
		exit(0);
	}
	if (strncasecmp(argv[1],"SobelH",MAXLEN)==0)
	{
		if (argc!=6) {
			fprintf(stderr, "SobelH require 4 arguments: <src> <roiFile> <tgt> <tgt>\n");
			exit(0);
		}
		Image		src, tgt, tgtFinal,tgtFinal2;
		char		tgtname[100];
		ROI 		roi;
		src.read(argv[2]);
		tgt.initialize(src.NR,src.NC);
		tgtFinal.initialize(src.NR,src.NC);
		tgtFinal2.initialize(src.NR,src.NC);
		roi.CreateROIList(argv[3],ROI::SobelH);
  		if (strstr(argv[2], ".ppm") != NULL) {/* PPM Color File*/
			ipTool::SobelH(src,roi,tgtFinal,tgtFinal2);  
			sprintf(tgtname,"%s.ppm", argv[4]);
			tgtFinal.save(tgtname);
			sprintf(tgtname,"%s.ppm", argv[5]);
			tgtFinal2.save(tgtname);
		} 
		else{
			ipTool::pgm2ppm(src, tgt);
			ipTool::SobelG(tgt,roi,tgtFinal,tgtFinal2);
			sprintf(tgtname,"%s.pgm", argv[4]);
			tgtFinal.save(tgtname);
			sprintf(tgtname,"%s.pgm", argv[5]);
			tgtFinal2.save(tgtname);
		}   
		exit(0);
	}
	if (strncasecmp(argv[1],"SobelS",MAXLEN)==0)
	{
		if (argc!=6) {
			fprintf(stderr, "SobelS require 4 arguments: <src> <roiFile> <tgt> <tgt>\n");
			exit(0);
		}
		Image		src, tgt, tgtFinal,tgtFinal2;
		char		tgtname[100];
		ROI 		roi;
		src.read(argv[2]);
		tgt.initialize(src.NR,src.NC);
		tgtFinal.initialize(src.NR,src.NC);
		tgtFinal2.initialize(src.NR,src.NC);
		roi.CreateROIList(argv[3],ROI::SobelS);
  		if (strstr(argv[2], ".ppm") != NULL) {/* PPM Color File*/
			ipTool::SobelS(src,roi,tgtFinal,tgtFinal2);  
			sprintf(tgtname,"%s.ppm", argv[4]);
			tgtFinal.save(tgtname);
			sprintf(tgtname,"%s.ppm", argv[5]);
			tgtFinal2.save(tgtname);
		} 
		else{
			ipTool::pgm2ppm(src, tgt);
			ipTool::SobelG(tgt,roi,tgtFinal,tgtFinal2);
			sprintf(tgtname,"%s.pgm", argv[4]);
			tgtFinal.save(tgtname);
			sprintf(tgtname,"%s.pgm", argv[5]);
			tgtFinal2.save(tgtname);
		}   
		exit(0);
	}
	if (strncasecmp(argv[1],"SobelI",MAXLEN)==0)
	{
		if (argc!=6) {
			fprintf(stderr, "SobelI require 4 arguments: <src> <roiFile> <tgt> <tgt>\n");
			exit(0);
		}
		Image		src, tgt, tgtFinal,tgtFinal2;
		char		tgtname[100];
		ROI 		roi;
		src.read(argv[2]);
		tgt.initialize(src.NR,src.NC);
		tgtFinal.initialize(src.NR,src.NC);
		tgtFinal2.initialize(src.NR,src.NC);
		roi.CreateROIList(argv[3],ROI::SobelI);
  		if (strstr(argv[2], ".ppm") != NULL) {/* PPM Color File*/
			ipTool::SobelI(src,roi,tgtFinal,tgtFinal2);  
			sprintf(tgtname,"%s.ppm", argv[4]);
			tgtFinal.save(tgtname);
			sprintf(tgtname,"%s.ppm", argv[5]);
			tgtFinal2.save(tgtname);
		} 
		else{
			ipTool::pgm2ppm(src, tgt);
			ipTool::SobelG(tgt,roi,tgtFinal,tgtFinal2);
			sprintf(tgtname,"%s.pgm", argv[4]);
			tgtFinal.save(tgtname);
			sprintf(tgtname,"%s.pgm", argv[5]);
			tgtFinal2.save(tgtname);
		}   
		exit(0);
	}
	if (strncasecmp(argv[1],"SobelHSI",MAXLEN)==0)
	{
		if (argc!=6) {
			fprintf(stderr, "SobelHSI require 4 arguments: <src> <roiFile> <tgt1> <tgt2>\n");
			exit(0);
		}
		Image		src, tgt, tgtFinal,tgtFinal2;
		char		tgtname[100];
		ROI 		roi;
		src.read(argv[2]);
		tgt.initialize(src.NR,src.NC);
		tgtFinal.initialize(src.NR,src.NC);
		tgtFinal2.initialize(src.NR,src.NC);
		roi.CreateROIList(argv[3],ROI::SobelHSI);
  		if (strstr(argv[2], ".ppm") != NULL) {/* PPM Color File*/
			ipTool::SobelHSI(src,roi,tgtFinal,tgtFinal2);  
			sprintf(tgtname,"%s.ppm", argv[4]);
			tgtFinal.save(tgtname);
			sprintf(tgtname,"%s.ppm", argv[5]);
			tgtFinal2.save(tgtname);
		} 
		else{
			ipTool::pgm2ppm(src, tgt);
			ipTool::SobelG(tgt,roi,tgtFinal,tgtFinal2);
			sprintf(tgtname,"%s.pgm", argv[4]);
			tgtFinal.save(tgtname);
			sprintf(tgtname,"%s.pgm", argv[5]);
			tgtFinal2.save(tgtname);
		}   
		exit(0);
	}

//Image filters from Assignment 2 - Video Filters for Assignments 2 and 3 are below.

	if (strncasecmp(argv[1],"ROIThresh",MAXLEN)==0)
	{
		if (argc!=5)
		{
			fprintf(stderr, "ROIThresh require 3 arguments: <src> <roiFile> <tgt>\n");
			exit(0);
		}
		//init:
		Image		src;
		Image 		tgt;
		Image 		tgtFinal;
		char		tgtname[100];

		src.read(argv[2]);
		tgt.initialize(src.NR,src.NC);
		tgtFinal.initialize(src.NR,src.NC);

		ROI roi;
		roi.CreateROIList(argv[3],ROI::ROIThresh);


  		if (strstr(argv[2], ".ppm") != NULL) {/* PPM Color File*/
			ipTool::ppm2pgm(src, tgt);
			ipTool::ROIThresh(tgt,roi,tgtFinal);  
		} 
		else
			ipTool::ROIThresh(src,roi,tgtFinal);   

		sprintf(tgtname,"%s.pgm", argv[4]);
		tgtFinal.save(tgtname);

		exit(0);
	}
	if (strncasecmp(argv[1],"OutputROI",MAXLEN)==0)
	{
		if (argc!=5)
		{
			fprintf(stderr, "OutputROI require 4 arguments: <src> <roiFile> <tgt>\n");
			exit(0);
		}

		Image		src;
		Image 		tgt;
		Image 		tgtFinal;
		char		tgtname[100];
		ROI 		roi;

		src.read(argv[2]);

		roi.CreateROIList(argv[3],ROI::OutputROI);
		
		tgt.initialize(src.NR,src.NC);
		tgtFinal.initialize(src.NR,src.NC);

  		if (strstr(argv[2], ".ppm") != NULL) { /* PPM Color File*/
			sprintf(tgtname,"%s.ppm", argv[4]);
			ipTool::OutputROI(src,roi,tgt);   
			tgt.save(tgtname);
		} else {
			sprintf(tgtname,"%s.pgm", argv[4]);
			ipTool::pgm2ppm(src,tgt);
			ipTool::OutputROI(tgt,roi,tgtFinal);   
			tgtFinal.save(tgtname);
		}

		exit(0);
	}
	
	if (strncasecmp(argv[1],"GrayArithmetic",MAXLEN)==0)
	{
		if (argc!=5)
		{
			fprintf(stderr, "GrayArithmetic require 4 arguments: <src> <roiFile> <tgt>\n");
			exit(0);
		}
		//init:
		Image		src;
		ROI 		roi;
		Image 		tgt;
		Image 		tgtFinal;
		char		tgtname[100];

		src.read(argv[2]);

		roi.CreateROIList(argv[3],ROI::GrayArithmetic);

		tgt.initialize(src.NR,src.NC);
		tgtFinal.initialize(src.NR,src.NC);

		sprintf(tgtname,"%s.pgm", argv[4]);
  		if (strstr(argv[2], ".ppm") != NULL) { /* PPM Color File*/
			ipTool::ppm2pgm(src, tgt);
			ipTool::GrayArithmetic(tgt,roi,src);
			src.save(tgtname);
		} else {
			ipTool::GrayArithmetic(src,roi,tgt);
			tgt.save(tgtname);
    		}

		exit(0);
	}

	//static void AdaptiveThresh(Image &src,ROI roi,Image &tgt);
	if (strncasecmp(argv[1],"AdaptiveThresh",MAXLEN)==0)
	{
		if (argc!=5)
		{
			fprintf(stderr, "AdaptiveThresh require 4 arguments: <src> <roiFile> <tgt>\n");
			exit(0);
		}
		//init:
		Image		src;
		Image 		tgt;
		Image 		tgtFinal;
		char		tgtname[100];
		
		src.read(argv[2]);

		ROI roi;
		roi.CreateROIList(argv[3],ROI::AdaptiveThresh);

		tgt.initialize(src.NR,src.NC);
		tgtFinal.initialize(src.NR,src.NC);

		sprintf(tgtname,"%s.pgm", argv[4]); 
  		if (strstr(argv[2], ".ppm") != NULL) { /* PPM Color File*/
			ipTool::ppm2pgm(src, tgt);
			ipTool::AdaptiveThresh(tgt,roi,src);   
			src.save(tgtname);

		} else {
			ipTool::AdaptiveThresh(src,roi,tgt);   
			tgt.save(tgtname);
		}

		exit(0);
	}

	//static void ColorThresh(Image &src,ROI roi,Image &tgt);
	if (strncasecmp(argv[1],"ColorThresh",MAXLEN)==0)
	{
		if (argc!=5)
		{
			fprintf(stderr, "ColorThresh require 4 arguments: <src> <roiFile> <tgt>\n");
			exit(0);
		}
		//init:
		Image		src;
		Image 		tgt;
		Image 		tgtFinal;
		char		tgtname[100];

		src.read(argv[2]);

		ROI roi;
		roi.CreateROIList(argv[3],ROI::ColorThresh);

		tgt.initialize(src.NR,src.NC);
		tgtFinal.initialize(src.NR,src.NC);

		sprintf(tgtname,"%s.ppm", argv[4]); 
  		if (strstr(argv[2], ".ppm") != NULL) { /* PPM Color File*/
			ipTool::ColorThresh(src,roi,tgt);   
			tgt.save(tgtname);
		} else {
			ipTool::pgm2ppm(src,tgt);
			ipTool::ColorThresh(tgt,roi,tgtFinal); 
			tgtFinal.save(tgtname);
		}

		exit(0);
	}

	//static void ColorArithmetic(Image &src,ROI roi,Image &tgt);
	if (strncasecmp(argv[1],"ColorArithmetic",MAXLEN)==0)
	{
		if (argc!=5)
		{
			fprintf(stderr, "ColorArithmetic require 4 arguments: <src> <roiFile> <tgt>\n");
			exit(0);
		}
		//init:
		Image		src;
		ROI 		roi;
		Image 		tgt;
		Image 		tgtFinal;
		char		tgtname[100];
		
		src.read(argv[2]);
		roi.CreateROIList(argv[3],ROI::ColorArithmetic);

		tgt.initialize(src.NR,src.NC);
		tgtFinal.initialize(src.NR,src.NC);

		sprintf(tgtname,"%s.ppm", argv[4]);
  		if (strstr(argv[2], ".ppm") != NULL) { /* PPM Color File*/
			ipTool::ColorArithmetic(src,roi,tgt);   
			tgt.save(tgtname);
		} else {
			ipTool::pgm2ppm(src,tgt);
			ipTool::ColorArithmetic(tgt,roi,tgtFinal);   
			tgtFinal.save(tgtname);
		}

		exit(0);
	}

//Video Filters for assignment 3

	if (strncasecmp(argv[1],"SobelVideo",MAXLEN)==0)
	{
		if (argc!=7)
		{
			fprintf(stderr, "SobelVideo requires 5 arguments: <inFile> <roi> <soi> <out1> <out2>\n");
			exit(0);
		}
		//init:
		FILE 		*mpeg;
		ImageDesc 	img;
		Boolean		moreframes = TRUE;
		char 		*pixels;
		Image		src,tgt;
		int			count=0;
		char		tgtname[100];
		int		frameNum=0;
		SOI soi;
		ROI roi;
		soi.CreateSOIList(argv[4]);
		roi.CreateROIList(argv[3],ROI::Sobel);
		mpeg = fopen (argv[2], "rb");
		SetMPEGOption (MPEG_DITHER, FULL_COLOR_DITHER);
		OpenMPEG (mpeg, &img);
		src.initialize(img.Height, img.Width);
		tgt.initialize(img.Height, img.Width);
		pixels = (char *) malloc (img.Size);
		fprintf(stderr,"\nBeginning to extract frames...\n");
		int latestTs = 0;
		for(int i = 0; i < soi.numBaseSOI; i++)
			if( soi.soiList[i].end > latestTs)
				latestTs = soi.soiList[i].end; 

		while (moreframes && frameNum < latestTs)
		{
			moreframes = GetMPEGFrame(pixels);
			if (saveFrame (img.Height, img.Width, pixels, src) != 0) 
			{
				(void) fprintf (stderr,"\nError Saving Frame\nExitting...\n");
				exit (-2);
			}

			if(soi.InSOI(frameNum++)!=-1) {
				Image tgt2;
				tgt2.initialize(img.Height,img.Width);
				ipTool::Sobel(src,roi,tgt,tgt2);
				sprintf(tgtname,"%s%d.ppm", argv[5], count);
				tgt.save(tgtname);
				sprintf(tgtname,"%s%d.ppm", argv[6], count++);
				tgt2.save(tgtname);
			}
		}
		fprintf(stderr,"\n***DONE***\n\n");

		char ccount[10000];
		sprintf(ccount,"%i",count-1);
		string encodeCmd = ("./encodeframes_grad 0 "+string(ccount)+" "+string(argv[5])+" "+string(argv[5])+".mpeg"); 
		printf("%s\n",encodeCmd.c_str());
		system(encodeCmd.c_str()); 
		encodeCmd = ("./encodeframes_grad 0 "+string(ccount)+" "+string(argv[6])+" "+string(argv[6])+".mpeg"); 
		printf("%s\n",encodeCmd.c_str());
		system(encodeCmd.c_str()); 

		CloseMPEG ();
		fclose (mpeg);
		src.flushMemory();
		exit(0);
	}
	if (strncasecmp(argv[1],"SobelRVideo",MAXLEN)==0)
	{
		if (argc!=7)
		{
			fprintf(stderr, "SobelRVideo requires 5 arguments: <inFile> <roi> <soi> <out1> <out2>\n");
			exit(0);
		}
		//init:
		FILE 		*mpeg;
		ImageDesc 	img;
		Boolean		moreframes = TRUE;
		char 		*pixels;
		Image		src,tgt;
		int			count=0;
		char		tgtname[100];
		int		frameNum=0;
		mpeg = fopen (argv[2], "rb");
		SetMPEGOption (MPEG_DITHER, FULL_COLOR_DITHER);
		OpenMPEG (mpeg, &img);
		src.initialize(img.Height, img.Width);
		tgt.initialize(img.Height, img.Width);
		pixels = (char *) malloc (img.Size);
		fprintf(stderr,"\nBeginning to extract frames...\n");
		SOI soi;
		soi.CreateSOIList(argv[4]);
		ROI roi;
		roi.CreateROIList(argv[3],ROI::SobelR);
		int latestTs = 0;
		for(int i = 0; i < soi.numBaseSOI; i++)
			if( soi.soiList[i].end > latestTs)
				latestTs = soi.soiList[i].end; 

		while (moreframes && frameNum < latestTs)
		{
			moreframes = GetMPEGFrame(pixels);
			if (saveFrame (img.Height, img.Width, pixels, src) != 0) 
			{
				(void) fprintf (stderr,"\nError Saving Frame\nExitting...\n");
				exit (-2);
			}

			if(soi.InSOI(frameNum++)!=-1) {
				Image tgt2;
				tgt2.initialize(img.Height,img.Width);
				ipTool::Sobel(src,roi,tgt,tgt2);
				sprintf(tgtname,"%s%d.ppm", argv[5], count);
				tgt.save(tgtname);
				sprintf(tgtname,"%s%d.ppm", argv[6], count++);
				tgt2.save(tgtname);
			}
		}
		fprintf(stderr,"\n***DONE***\n\n");

		char ccount[10000];
		sprintf(ccount,"%i",count-1);
		string encodeCmd = ("./encodeframes_grad 0 "+string(ccount)+" "+string(argv[5])+" "+string(argv[5])+".mpeg"); 
		printf("%s\n",encodeCmd.c_str());
		system(encodeCmd.c_str()); 
		encodeCmd = ("./encodeframes_grad 0 "+string(ccount)+" "+string(argv[6])+" "+string(argv[6])+".mpeg"); 
		printf("%s\n",encodeCmd.c_str());
		system(encodeCmd.c_str()); 

		CloseMPEG ();
		fclose (mpeg);
		src.flushMemory();
		exit(0);
	}
	if (strncasecmp(argv[1],"SobelGVideo",MAXLEN)==0)
	{
		if (argc!=7)
		{
			fprintf(stderr, "SobelGVideo requires 5 arguments: <inFile> <roi> <soi> <out1> <out2>\n");
			exit(0);
		}
		//init:
		FILE 		*mpeg;
		ImageDesc 	img;
		Boolean		moreframes = TRUE;
		char 		*pixels;
		Image		src,tgt;
		int			count=0;
		char		tgtname[100];
		int		frameNum=0;
		mpeg = fopen (argv[2], "rb");
		SetMPEGOption (MPEG_DITHER, FULL_COLOR_DITHER);
		OpenMPEG (mpeg, &img);
		src.initialize(img.Height, img.Width);
		tgt.initialize(img.Height, img.Width);
		pixels = (char *) malloc (img.Size);
		fprintf(stderr,"\nBeginning to extract frames...\n");
		SOI soi;
		ROI roi;
		soi.CreateSOIList(argv[4]);
		roi.CreateROIList(argv[3],ROI::SobelG);
		int latestTs = 0;
		for(int i = 0; i < soi.numBaseSOI; i++)
			if( soi.soiList[i].end > latestTs)
				latestTs = soi.soiList[i].end; 

		while (moreframes && frameNum < latestTs)
		{
			moreframes = GetMPEGFrame(pixels);
			if (saveFrame (img.Height, img.Width, pixels, src) != 0) 
			{
				(void) fprintf (stderr,"\nError Saving Frame\nExitting...\n");
				exit (-2);
			}
			if(soi.InSOI(frameNum++)!=-1) {
				Image tgt2;
				tgt2.initialize(img.Height,img.Width);
				ipTool::Sobel(src,roi,tgt,tgt2);
				sprintf(tgtname,"%s%d.ppm", argv[5], count);
				tgt.save(tgtname);
				sprintf(tgtname,"%s%d.ppm", argv[6], count++);
				tgt2.save(tgtname);
			}
		}
		fprintf(stderr,"\n***DONE***\n\n");

		char ccount[10000];
		sprintf(ccount,"%i",count-1);
		string encodeCmd = ("./encodeframes_grad 0 "+string(ccount)+" "+string(argv[5])+" "+string(argv[5])+".mpeg"); 
		printf("%s\n",encodeCmd.c_str());
		system(encodeCmd.c_str()); 
		encodeCmd = ("./encodeframes_grad 0 "+string(ccount)+" "+string(argv[6])+" "+string(argv[6])+".mpeg"); 
		printf("%s\n",encodeCmd.c_str());
		system(encodeCmd.c_str()); 

		CloseMPEG ();
		fclose (mpeg);
		src.flushMemory();
		exit(0);
	}
	if (strncasecmp(argv[1],"SobelBVideo",MAXLEN)==0)
	{
		if (argc!=7)
		{
			fprintf(stderr, "SobelBVideo requires 5 arguments: <inFile> <roi> <soi> <out1> <out2>\n");
			exit(0);
		}
		//init:
		FILE 		*mpeg;
		ImageDesc 	img;
		Boolean		moreframes = TRUE;
		char 		*pixels;
		Image		src,tgt;
		int			count=0;
		char		tgtname[100];
		int		frameNum=0;
		mpeg = fopen (argv[2], "rb");
		SetMPEGOption (MPEG_DITHER, FULL_COLOR_DITHER);
		OpenMPEG (mpeg, &img);
		src.initialize(img.Height, img.Width);
		tgt.initialize(img.Height, img.Width);
		pixels = (char *) malloc (img.Size);
		fprintf(stderr,"\nBeginning to extract frames...\n");
		SOI soi;
		soi.CreateSOIList(argv[4]);
		ROI roi;
		roi.CreateROIList(argv[3],ROI::SobelB);
		int latestTs = 0;
		for(int i = 0; i < soi.numBaseSOI; i++)
			if( soi.soiList[i].end > latestTs)
				latestTs = soi.soiList[i].end; 

		while (moreframes && frameNum < latestTs)
		{
			moreframes = GetMPEGFrame(pixels);
			if (saveFrame (img.Height, img.Width, pixels, src) != 0) 
			{
				(void) fprintf (stderr,"\nError Saving Frame\nExitting...\n");
				exit (-2);
			}

			if(soi.InSOI(frameNum++)!=-1) {
				Image tgt2;
				tgt2.initialize(img.Height,img.Width);
				ipTool::Sobel(src,roi,tgt,tgt2);
				sprintf(tgtname,"%s%d.ppm", argv[5], count);
				tgt.save(tgtname);
				sprintf(tgtname,"%s%d.ppm", argv[6], count++);
				tgt2.save(tgtname);
			}
		}
		fprintf(stderr,"\n***DONE***\n\n");

		char ccount[10000];
		sprintf(ccount,"%i",count-1);
		string encodeCmd = ("./encodeframes_grad 0 "+string(ccount)+" "+string(argv[5])+" "+string(argv[5])+".mpeg"); 
		printf("%s\n",encodeCmd.c_str());
		system(encodeCmd.c_str()); 
		encodeCmd = ("./encodeframes_grad 0 "+string(ccount)+" "+string(argv[6])+" "+string(argv[6])+".mpeg"); 
		printf("%s\n",encodeCmd.c_str());
		system(encodeCmd.c_str()); 

		CloseMPEG ();
		fclose (mpeg);
		src.flushMemory();
		exit(0);
	}
	if (strncasecmp(argv[1],"SobelRGBVideo",MAXLEN)==0)
	{
		if (argc!=7)
		{
			fprintf(stderr, "SobelRGBVideo requires 5 arguments: <inFile> <roi> <soi> <out1> <out2>\n");
			exit(0);
		}
		//init:
		FILE 		*mpeg;
		ImageDesc 	img;
		Boolean		moreframes = TRUE;
		char 		*pixels;
		Image		src,tgt;
		int			count=0;
		char		tgtname[100];
		int		frameNum=0;
		mpeg = fopen (argv[2], "rb");
		SetMPEGOption (MPEG_DITHER, FULL_COLOR_DITHER);
		OpenMPEG (mpeg, &img);
		src.initialize(img.Height, img.Width);
		tgt.initialize(img.Height, img.Width);
		pixels = (char *) malloc (img.Size);
		fprintf(stderr,"\nBeginning to extract frames...\n");
		SOI soi;
		ROI roi;
		soi.CreateSOIList(argv[4]);
		roi.CreateROIList(argv[3],ROI::SobelRGB);
		int latestTs = 0;
		for(int i = 0; i < soi.numBaseSOI; i++)
			if( soi.soiList[i].end > latestTs)
				latestTs = soi.soiList[i].end; 

		while (moreframes && frameNum < latestTs)
		{
			moreframes = GetMPEGFrame(pixels);
			if (saveFrame (img.Height, img.Width, pixels, src) != 0) 
			{
				(void) fprintf (stderr,"\nError Saving Frame\nExitting...\n");
				exit (-2);
			}
			if(soi.InSOI(frameNum++)!=-1) {
				Image tgt2;
				tgt2.initialize(img.Height,img.Width);
				ipTool::Sobel(src,roi,tgt,tgt2);
				sprintf(tgtname,"%s%d.ppm", argv[5], count);
				tgt.save(tgtname);
				sprintf(tgtname,"%s%d.ppm", argv[6], count++);
				tgt2.save(tgtname);
			}
		}
		fprintf(stderr,"\n***DONE***\n\n");

		char ccount[10000];
		sprintf(ccount,"%i",count-1);
		string encodeCmd = ("./encodeframes_grad 0 "+string(ccount)+" "+string(argv[5])+" "+string(argv[5])+".mpeg"); 
		printf("%s\n",encodeCmd.c_str());
		system(encodeCmd.c_str()); 
		encodeCmd = ("./encodeframes_grad 0 "+string(ccount)+" "+string(argv[6])+" "+string(argv[6])+".mpeg"); 
		printf("%s\n",encodeCmd.c_str());
		system(encodeCmd.c_str()); 

		CloseMPEG ();
		fclose (mpeg);
		src.flushMemory();
		exit(0);
	}
	if (strncasecmp(argv[1],"SobelHVideo",MAXLEN)==0)
	{
		if (argc!=7)
		{
			fprintf(stderr, "SobelHVideo requires 5 arguments: <inFile> <roi> <soi> <out1> <out2>\n");
			exit(0);
		}
		//init:
		FILE 		*mpeg;
		ImageDesc 	img;
		Boolean		moreframes = TRUE;
		char 		*pixels;
		Image		src,tgt;
		int			count=0;
		char		tgtname[100];
		int		frameNum=0;
		mpeg = fopen (argv[2], "rb");
		SetMPEGOption (MPEG_DITHER, FULL_COLOR_DITHER);
		OpenMPEG (mpeg, &img);
		src.initialize(img.Height, img.Width);
		tgt.initialize(img.Height, img.Width);
		pixels = (char *) malloc (img.Size);
		fprintf(stderr,"\nBeginning to extract frames...\n");
		SOI soi;
		ROI roi;
		soi.CreateSOIList(argv[4]);
		roi.CreateROIList(argv[3],ROI::SobelH);
		int latestTs = 0;
		for(int i = 0; i < soi.numBaseSOI; i++)
			if( soi.soiList[i].end > latestTs)
				latestTs = soi.soiList[i].end; 

		while (moreframes && frameNum < latestTs)
		while (moreframes)
		{
			moreframes = GetMPEGFrame(pixels);
			if (saveFrame (img.Height, img.Width, pixels, src) != 0) 
			{
				(void) fprintf (stderr,"\nError Saving Frame\nExitting...\n");
				exit (-2);
			}
			if(soi.InSOI(frameNum++)!=-1) {
				Image tgt2;
				tgt2.initialize(img.Height,img.Width);
				ipTool::Sobel(src,roi,tgt,tgt2);
				sprintf(tgtname,"%s%d.ppm", argv[5], count);
				tgt.save(tgtname);
				sprintf(tgtname,"%s%d.ppm", argv[6], count++);
				tgt2.save(tgtname);
			}
		}
		fprintf(stderr,"\n***DONE***\n\n");

		char ccount[10000];
		sprintf(ccount,"%i",count-1);
		string encodeCmd = ("./encodeframes_grad 0 "+string(ccount)+" "+string(argv[5])+" "+string(argv[5])+".mpeg"); 
		printf("%s\n",encodeCmd.c_str());
		system(encodeCmd.c_str()); 
		encodeCmd = ("./encodeframes_grad 0 "+string(ccount)+" "+string(argv[6])+" "+string(argv[6])+".mpeg"); 
		printf("%s\n",encodeCmd.c_str());
		system(encodeCmd.c_str()); 

		CloseMPEG ();
		fclose (mpeg);
		src.flushMemory();
		exit(0);
	}
	if (strncasecmp(argv[1],"SobelSVideo",MAXLEN)==0)
	{
		if (argc!=7)
		{
			fprintf(stderr, "SobelSVideo requires 5 arguments: <inFile> <roi> <soi> <out1> <out2>\n");
			exit(0);
		}
		//init:
		FILE 		*mpeg;
		ImageDesc 	img;
		Boolean		moreframes = TRUE;
		char 		*pixels;
		Image		src,tgt;
		int			count=0;
		char		tgtname[100];
		int		frameNum=0;
		mpeg = fopen (argv[2], "rb");
		SetMPEGOption (MPEG_DITHER, FULL_COLOR_DITHER);
		OpenMPEG (mpeg, &img);
		src.initialize(img.Height, img.Width);
		tgt.initialize(img.Height, img.Width);
		pixels = (char *) malloc (img.Size);
		SOI soi;
		ROI roi;
		soi.CreateSOIList(argv[4]);
		roi.CreateROIList(argv[3],ROI::SobelS);
		fprintf(stderr,"\nBeginning to extract frames...\n");
		int latestTs = 0;
		for(int i = 0; i < soi.numBaseSOI; i++)
			if( soi.soiList[i].end > latestTs)
				latestTs = soi.soiList[i].end; 

		while (moreframes && frameNum < latestTs)
		{
			moreframes = GetMPEGFrame(pixels);
			if (saveFrame (img.Height, img.Width, pixels, src) != 0) 
			{
				(void) fprintf (stderr,"\nError Saving Frame\nExitting...\n");
				exit (-2);
			}
			if(soi.InSOI(frameNum++)!=-1) {
				Image tgt2;
				tgt2.initialize(img.Height,img.Width);
				ipTool::Sobel(src,roi,tgt,tgt2);
				sprintf(tgtname,"%s%d.ppm", argv[5], count);
				tgt.save(tgtname);
				sprintf(tgtname,"%s%d.ppm", argv[6], count++);
				tgt2.save(tgtname);
			}
		}
		fprintf(stderr,"\n***DONE***\n\n");

		char ccount[10000];
		sprintf(ccount,"%i",count-1);
		string encodeCmd = ("./encodeframes_grad 0 "+string(ccount)+" "+string(argv[5])+" "+string(argv[5])+".mpeg"); 
		printf("%s\n",encodeCmd.c_str());
		system(encodeCmd.c_str()); 
		encodeCmd = ("./encodeframes_grad 0 "+string(ccount)+" "+string(argv[6])+" "+string(argv[6])+".mpeg"); 
		printf("%s\n",encodeCmd.c_str());
		system(encodeCmd.c_str()); 

		CloseMPEG ();
		fclose (mpeg);
		src.flushMemory();
		exit(0);
	}
	if (strncasecmp(argv[1],"SobelIVideo",MAXLEN)==0)
	{
		if (argc!=7)
		{
			fprintf(stderr, "SobelIVideo requires 5 arguments: <inFile> <roi> <soi> <out1> <out2>\n");
			exit(0);
		}
		//init:
		FILE 		*mpeg;
		ImageDesc 	img;
		Boolean		moreframes = TRUE;
		char 		*pixels;
		Image		src,tgt;
		int			count=0;
		char		tgtname[100];
		int		frameNum=0;
		mpeg = fopen (argv[2], "rb");
		SetMPEGOption (MPEG_DITHER, FULL_COLOR_DITHER);
		OpenMPEG (mpeg, &img);
		src.initialize(img.Height, img.Width);
		tgt.initialize(img.Height, img.Width);
		pixels = (char *) malloc (img.Size);
		fprintf(stderr,"\nBeginning to extract frames...\n");
		SOI soi;
		ROI roi;
		soi.CreateSOIList(argv[4]);
		roi.CreateROIList(argv[3],ROI::SobelI);
		int latestTs = 0;
		for(int i = 0; i < soi.numBaseSOI; i++)
			if( soi.soiList[i].end > latestTs)
				latestTs = soi.soiList[i].end; 

		while (moreframes && frameNum < latestTs)
		{
			moreframes = GetMPEGFrame(pixels);
			if (saveFrame (img.Height, img.Width, pixels, src) != 0) 
			{
				(void) fprintf (stderr,"\nError Saving Frame\nExitting...\n");
				exit (-2);
			}
			if(soi.InSOI(frameNum++)!=-1) {
				Image tgt2;
				tgt2.initialize(img.Height,img.Width);
				ipTool::Sobel(src,roi,tgt,tgt2);
				sprintf(tgtname,"%s%d.ppm", argv[5], count);
				tgt.save(tgtname);
				sprintf(tgtname,"%s%d.ppm", argv[6], count++);
				tgt2.save(tgtname);
			}
		}
		fprintf(stderr,"\n***DONE***\n\n");

		char ccount[10000];
		sprintf(ccount,"%i",count-1);
		string encodeCmd = ("./encodeframes_grad 0 "+string(ccount)+" "+string(argv[5])+" "+string(argv[5])+".mpeg"); 
		printf("%s\n",encodeCmd.c_str());
		system(encodeCmd.c_str()); 
		encodeCmd = ("./encodeframes_grad 0 "+string(ccount)+" "+string(argv[6])+" "+string(argv[6])+".mpeg"); 
		printf("%s\n",encodeCmd.c_str());
		system(encodeCmd.c_str()); 

		CloseMPEG ();
		fclose (mpeg);
		src.flushMemory();
		exit(0);
	}
	if (strncasecmp(argv[1],"SobelHSIVideo",MAXLEN)==0)
	{
		if (argc!=7)
		{
			fprintf(stderr, "SobelHSIVideo requires 5 arguments: <inFile> <roi> <soi> <out1> <out2>\n");
			exit(0);
		}
		//init:
		FILE 		*mpeg;
		ImageDesc 	img;
		Boolean		moreframes = TRUE;
		char 		*pixels;
		Image		src,tgt;
		int			count=0;
		char		tgtname[100];
		int		frameNum=0;
		mpeg = fopen (argv[2], "rb");
		SetMPEGOption (MPEG_DITHER, FULL_COLOR_DITHER);
		OpenMPEG (mpeg, &img);
		src.initialize(img.Height, img.Width);
		tgt.initialize(img.Height, img.Width);
		pixels = (char *) malloc (img.Size);
		SOI soi;
		ROI roi;
		soi.CreateSOIList(argv[4]);
		roi.CreateROIList(argv[3],ROI::SobelHSI);
		fprintf(stderr,"\nBeginning to extract frames...\n");
		int latestTs = 0;
		for(int i = 0; i < soi.numBaseSOI; i++)
			if( soi.soiList[i].end > latestTs)
				latestTs = soi.soiList[i].end; 

		while (moreframes && frameNum < latestTs)
		{
			moreframes = GetMPEGFrame(pixels);
			if (saveFrame (img.Height, img.Width, pixels, src) != 0) 
			{
				(void) fprintf (stderr,"\nError Saving Frame\nExitting...\n");
				exit (-2);
			}
			if(soi.InSOI(frameNum++)!=-1) {
				Image tgt2;
				tgt2.initialize(img.Height,img.Width);
				ipTool::Sobel(src,roi,tgt,tgt2);
				sprintf(tgtname,"%s%d.ppm", argv[5], count);
				tgt.save(tgtname);
				sprintf(tgtname,"%s%d.ppm", argv[6], count++);
				tgt2.save(tgtname);
			}
		}
		fprintf(stderr,"\n***DONE***\n\n");

		char ccount[10000];
		sprintf(ccount,"%i",count-1);
		string encodeCmd = ("./encodeframes_grad 0 "+string(ccount)+" "+string(argv[5])+" "+string(argv[5])+".mpeg"); 
		printf("%s\n",encodeCmd.c_str());
		system(encodeCmd.c_str()); 
		encodeCmd = ("./encodeframes_grad 0 "+string(ccount)+" "+string(argv[6])+" "+string(argv[6])+".mpeg"); 
		printf("%s\n",encodeCmd.c_str());
		system(encodeCmd.c_str()); 

		CloseMPEG ();
		fclose (mpeg);
		src.flushMemory();
		exit(0);
	}



// The following are from assignment 2
	if (strncasecmp(argv[1],"decodeVideo",MAXLEN)==0)
	{
		if (argc!=4)
		{
			fprintf(stderr, "decoding require 2 arguments\n");
			exit(0);
		}
		//init:
		FILE 		*mpeg;
		ImageDesc 	img;
		Boolean		moreframes = TRUE;
		char 		*pixels;
		Image		src,tgt;
		int			count=0;
		char		tgtname[100];
		mpeg = fopen (argv[2], "rb");
		SetMPEGOption (MPEG_DITHER, FULL_COLOR_DITHER);
		OpenMPEG (mpeg, &img);
		src.initialize(img.Height, img.Width);
		tgt.initialize(img.Height, img.Width);
		pixels = (char *) malloc (img.Size);
		fprintf(stderr,"\nBeginning to extract frames...\n");
		while (moreframes)
		{
			moreframes = GetMPEGFrame(pixels);
			if (saveFrame (img.Height, img.Width, pixels, src) != 0) 
			{
				(void) fprintf (stderr,"\nError Saving Frame\nExitting...\n");
				exit (-2);
			}
			sprintf(tgtname,"%s%d.ppm", argv[3], count);
			src.save(tgtname);
			ipTool::ppm2pgm(src,tgt);
			sprintf(tgtname,"%s%d.pgm", argv[3], count++);
			tgt.save(tgtname);
		}
		fprintf(stderr,"\n***DONE***\n\n");
		CloseMPEG ();
		fclose (mpeg);
		src.flushMemory();
		exit(0);
	}

	if (strncasecmp(argv[1],"ROIThreshVideo",MAXLEN)==0)
	{
		if (argc!=6)
		{
			fprintf(stderr, "ROIThresh require 4 arguments: <src> <roiFile> <soiFile> <tgt>\n");
			exit(0);
		}
		//init:
		FILE 		*mpeg;
		ImageDesc 	img;
		Boolean		moreframes = TRUE;
		char		*pixels;
		Image		src;
		int		count=0;
		int		frameNum=0;
		char		tgtname[100];
		mpeg = fopen (argv[2], "rb");
		SetMPEGOption (MPEG_DITHER, FULL_COLOR_DITHER);
		OpenMPEG (mpeg, &img);
		src.initialize(img.Height, img.Width);
		pixels = (char *) malloc (img.Size);
		printf("size: %i h: %i w: %i\n",img.Size,img.Height, img.Width);
		ROI roi;
		roi.CreateROIList(argv[3],ROI::ROIThresh);
		Image tgt;
		tgt.initialize(src.NR,src.NC);
		Image tgtFinal;
		tgtFinal.initialize(src.NR,src.NC);
		SOI soi;
		soi.CreateSOIList(argv[4]);
		fprintf(stderr,"\nBeginning to extract frames...\n");
		int latestTs = 0;
		for(int i = 0; i < soi.numBaseSOI; i++)
			if( soi.soiList[i].end > latestTs)
				latestTs = soi.soiList[i].end; 

		while (moreframes && frameNum < latestTs)
		{
			moreframes = GetMPEGFrame (pixels);
			if (saveFrame (img.Height, img.Width, pixels, src) != 0) 
			{
				(void) fprintf (stderr,"\nError Saving Frame\nExitting...\n");
				exit (-2);
			}
                        printf("Frame Number: %i\n",frameNum);
			if(soi.InSOI(frameNum++)!=-1) 
			{
				ipTool::ppm2pgm(src, tgt);
				ipTool::ROIThresh(tgt,roi,tgtFinal);   
				ipTool::pgm2ppm(tgtFinal, tgt);
				sprintf(tgtname,"%s%d.ppm", argv[5], count++);
				tgt.save(tgtname);
			}
		}
		fprintf(stderr,"\n***DONE***\n\n");

		char ccount[10000];
		sprintf(ccount,"%i",count-1);
		string encodeCmd = ("./encodeframes_grad 0 "+string(ccount)+" "+string(argv[5])+" "+string(argv[5])+".mpeg"); 
		printf("%s\n",encodeCmd.c_str());
		system(encodeCmd.c_str()); 

		CloseMPEG ();
		fclose(mpeg);
		src.flushMemory();
		exit(0);
	}

	if (strncasecmp(argv[1],"OutputROIVideo",MAXLEN)==0)
	{
		if (argc!=6)
		{
			fprintf(stderr, "OutputROI require 4 arguments: <src> <roiFile> <soiFile> <tgt>\n");
			exit(0);
		}
		//init:
		FILE 		*mpeg;
		ImageDesc 	img;
		Boolean		moreframes = TRUE;
		char		*pixels;
		Image		src;
		int		count=0;
		int		frameNum=0;
		char		tgtname[100];

		mpeg = fopen (argv[2], "rb");
		SetMPEGOption (MPEG_DITHER, FULL_COLOR_DITHER);
		OpenMPEG (mpeg, &img);
		
		src.initialize(img.Height, img.Width);
		pixels = (char *) malloc (img.Size);

		ROI roi;
		roi.CreateROIList(argv[3],ROI::OutputROI);
		SOI soi;
		soi.CreateSOIList(argv[4]);
		Image tgtFinal;
		tgtFinal.initialize(src.NR,src.NC);

		fprintf(stderr,"\nBeginning to extract frames...\n");

		int latestTs = 0;
		for(int i = 0; i < soi.numBaseSOI; i++)
			if( soi.soiList[i].end > latestTs)
				latestTs = soi.soiList[i].end; 

		while (moreframes && frameNum < latestTs)
		{
			moreframes = GetMPEGFrame (pixels);
			if (saveFrame (img.Height, img.Width, pixels, src) != 0) 
			{
				(void) fprintf (stderr,"\nError Saving Frame\nExitting...\n");
				exit (-2);
			}
                        printf("Frame Number: %i\n",frameNum);
			if(soi.InSOI(frameNum++)!=-1) 
			{
				sprintf(tgtname,"%s%d.ppm", argv[5], count++);
				ipTool::OutputROI(src,roi,tgtFinal);   
				tgtFinal.save(tgtname);
			}
		}
		fprintf(stderr,"\n***DONE***\n\n");

		char ccount[10000];
		sprintf(ccount,"%i",count-1);
		string encodeCmd = ("./encodeframes_grad 0 "+string(ccount)+" "+string(argv[5])+" "+string(argv[5])+".mpeg"); 
		printf("%s\n",encodeCmd.c_str());
		system(encodeCmd.c_str()); 

		CloseMPEG ();
		fclose(mpeg);
		src.flushMemory();

		exit(0);
	}
	
	if (strncasecmp(argv[1],"GrayArithmeticVideo",MAXLEN)==0)
	{
		if (argc!=6)
		{
			fprintf(stderr, "GrayArithmetic require 4 arguments: <src> <roiFile> <soiFile> <tgt>\n");
			exit(0);
		}
		//init:
		FILE 		*mpeg;
		ImageDesc 	img;
		Boolean		moreframes = TRUE;
		char		*pixels;
		Image		src;
		int		count=0;
		int		frameNum=0;
		char		tgtname[100];

		mpeg = fopen (argv[2], "rb");
		SetMPEGOption (MPEG_DITHER, FULL_COLOR_DITHER);
		OpenMPEG (mpeg, &img);
		
		src.initialize(img.Height, img.Width);
		pixels = (char *) malloc (img.Size);

		ROI roi;
		roi.CreateROIList(argv[3],ROI::GrayArithmetic);
		SOI soi;
		soi.CreateSOIList(argv[4]);

		Image tgt;
		tgt.initialize(src.NR,src.NC);
		Image tgtFinal;
		tgtFinal.initialize(src.NR,src.NC);

		fprintf(stderr,"\nBeginning to extract frames...\n");
		
		int latestTs = 0;
		for(int i = 0; i < soi.numBaseSOI; i++)
			if( soi.soiList[i].end > latestTs)
				latestTs = soi.soiList[i].end; 

		while (moreframes && frameNum < latestTs)
		{
			moreframes = GetMPEGFrame (pixels);
			if (saveFrame (img.Height, img.Width, pixels, src) != 0) 
			{
				(void) fprintf (stderr,"\nError Saving Frame\nExitting...\n");
				exit (-2);
			}
                        printf("Frame Number: %i\n",frameNum);
			if(soi.InSOI(frameNum++)!=-1) 
			{
				sprintf(tgtname,"%s%d.ppm", argv[5], count++);
				ipTool::ppm2pgm(src, tgt);
				ipTool::GrayArithmetic(tgt,roi,src);
				ipTool::pgm2ppm(src, tgtFinal);
				tgtFinal.save(tgtname);
			}
		}
		fprintf(stderr,"\n***DONE***\n\n");

		char ccount[10000];
		sprintf(ccount,"%i",count-1);
		string encodeCmd = ("./encodeframes_grad 0 "+string(ccount)+" "+string(argv[5])+" "+string(argv[5])+".mpeg"); 
		printf("%s\n",encodeCmd.c_str());
		system(encodeCmd.c_str()); 
	
		CloseMPEG ();
		fclose(mpeg);
		src.flushMemory();

		exit(0);
	}
	//static void AdaptiveThresh(Image &src,ROI roi,Image &tgt);
	if (strncasecmp(argv[1],"AdaptiveThreshVideo",MAXLEN)==0)
	{
		if (argc!=6)
		{
			fprintf(stderr, "AdaptiveThresh require 4 arguments: <src> <roiFile> <soiFile> <tgt>\n");
			exit(0);
		}
		//init:
		FILE 		*mpeg;
		ImageDesc 	img;
		Boolean		moreframes = TRUE;
		char		*pixels;
		Image		src;
		int		count=0;
		int		frameNum=0;
		char		tgtname[100];

		mpeg = fopen (argv[2], "rb");
		SetMPEGOption (MPEG_DITHER, FULL_COLOR_DITHER);
		OpenMPEG (mpeg, &img);
		
		src.initialize(img.Height, img.Width);
		pixels = (char *) malloc (img.Size);

		ROI roi;
		roi.CreateROIList(argv[3],ROI::AdaptiveThresh);
		SOI soi;
		soi.CreateSOIList(argv[4]);
		Image tgt;
		tgt.initialize(src.NR,src.NC);
		Image tgtFinal;
		tgtFinal.initialize(src.NR,src.NC);

		fprintf(stderr,"\nBeginning to extract frames...\n");

		int latestTs = 0;
		for(int i = 0; i < soi.numBaseSOI; i++)
			if( soi.soiList[i].end > latestTs)
				latestTs = soi.soiList[i].end; 

		while (moreframes && frameNum < latestTs)
		{
			moreframes = GetMPEGFrame (pixels);
			if (saveFrame (img.Height, img.Width, pixels, src) != 0) 
			{
				(void) fprintf (stderr,"\nError Saving Frame\nExitting...\n");
				exit (-2);
			}
                        printf("Frame Number: %i\n",frameNum);
			if(soi.InSOI(frameNum++)!=-1) 
			{
				sprintf(tgtname,"%s%d.ppm", argv[5], count++);
				ipTool::ppm2pgm(src, tgt);
				ipTool::AdaptiveThresh(tgt,roi,src);   
				ipTool::pgm2ppm(src, tgtFinal);
				tgtFinal.save(tgtname);
			}
		}
		fprintf(stderr,"\n***DONE***\n\n");

		char ccount[10000];
		sprintf(ccount,"%i",count-1);
		string encodeCmd = ("./encodeframes_grad 0 "+string(ccount)+" "+string(argv[5])+" "+string(argv[5])+".mpeg"); 
		printf("%s\n",encodeCmd.c_str());
		system(encodeCmd.c_str()); 

		CloseMPEG ();
		fclose(mpeg);
		src.flushMemory();

		exit(0);
	}

	//static void ColorThresh(Image &src,ROI roi,Image &tgt);
	if (strncasecmp(argv[1],"ColorThreshVideo",MAXLEN)==0)
	{
		if (argc!=6)
		{
			fprintf(stderr, "ColorThresh require 4 arguments: <src> <roiFile> <soiFile> <tgt>\n");
			exit(0);
		}
		//init:
		FILE 		*mpeg;
		ImageDesc 	img;
		Boolean		moreframes = TRUE;
		char		*pixels;
		Image		src;
		int		count=0;
		int		frameNum=0;
		char		tgtname[100];

		mpeg = fopen (argv[2], "rb");
		SetMPEGOption (MPEG_DITHER, FULL_COLOR_DITHER);
		OpenMPEG (mpeg, &img);
		
		src.initialize(img.Height, img.Width);
		pixels = (char *) malloc (img.Size);

		ROI roi;
		roi.CreateROIList(argv[3],ROI::ColorThresh);
		SOI soi;
		soi.CreateSOIList(argv[4]);
		Image tgt;

		tgt.initialize(src.NR,src.NC);

		fprintf(stderr,"\nBeginning to extract frames...\n");

		int latestTs = 0;
		for(int i = 0; i < soi.numBaseSOI; i++)
			if( soi.soiList[i].end > latestTs)
				latestTs = soi.soiList[i].end; 

		while (moreframes && frameNum < latestTs)
		{
			moreframes = GetMPEGFrame (pixels);
			if (saveFrame (img.Height, img.Width, pixels, src) != 0) 
			{
				(void) fprintf (stderr,"\nError Saving Frame\nExitting...\n");
				exit (-2);
			}
                        printf("Frame Number: %i\n",frameNum);
			if(soi.InSOI(frameNum++)!=-1) 
			{
				sprintf(tgtname,"%s%d.ppm", argv[5], count++);
				ipTool::ColorThresh(src,roi,tgt);   
				tgt.save(tgtname);
			}
		}
		fprintf(stderr,"\n***DONE***\n\n");

		char ccount[10000];
		sprintf(ccount,"%i",count-1);
		string encodeCmd = ("./encodeframes_grad 0 "+string(ccount)+" "+string(argv[5])+" "+string(argv[5])+".mpeg"); 
		printf("%s\n",encodeCmd.c_str());
		system(encodeCmd.c_str()); 

		CloseMPEG ();
		fclose(mpeg);
		src.flushMemory();

		exit(0);
	}

	//static void ColorArithmetic(Image &src,ROI roi,Image &tgt);
	if (strncasecmp(argv[1],"ColorArithmeticVideo",MAXLEN)==0)
	{
		if (argc!=6)
		{
			fprintf(stderr, "ColorArithmetic require 4 arguments: <src> <roiFile> <soiFile> <tgt>\n");
			exit(0);
		}
		//init:
		FILE 		*mpeg;
		ImageDesc 	img;
		Boolean		moreframes = TRUE;
		char		*pixels;
		Image		src;
		int		count=0;
		int		frameNum=0;
		char		tgtname[100];

		mpeg = fopen (argv[2], "rb");
		SetMPEGOption (MPEG_DITHER, FULL_COLOR_DITHER);
		OpenMPEG (mpeg, &img);
		
		src.initialize(img.Height, img.Width);
		pixels = (char *) malloc (img.Size);

		ROI roi;
		roi.CreateROIList(argv[3],ROI::ColorArithmetic);
		SOI soi;
		soi.CreateSOIList(argv[4]);
		Image tgt;

		tgt.initialize(src.NR,src.NC);

		fprintf(stderr,"\nBeginning to extract frames...\n");

		int latestTs = 0;
		for(int i = 0; i < soi.numBaseSOI; i++)
			if( soi.soiList[i].end > latestTs)
				latestTs = soi.soiList[i].end; 

		while (moreframes && frameNum < latestTs)
		{
			moreframes = GetMPEGFrame (pixels);
			if (saveFrame (img.Height, img.Width, pixels, src) != 0) 
			{
				(void) fprintf (stderr,"\nError Saving Frame\nExitting...\n");
				exit (-2);
			}
                        printf("Frame Number: %i\n",frameNum);
			if(soi.InSOI(frameNum++)!=-1) 
			{
				sprintf(tgtname,"%s%d.ppm", argv[5], count++);
				ipTool::ColorArithmetic(src,roi,tgt);   
				tgt.save(tgtname);
			}
		}
		fprintf(stderr,"\n***DONE***\n\n");

		char ccount[10000];
		sprintf(ccount,"%i",count-1);
		string encodeCmd = ("./encodeframes_grad 0 "+string(ccount)+" "+string(argv[5])+" "+string(argv[5])+".mpeg"); 
		printf("%s\n",encodeCmd.c_str());
		system(encodeCmd.c_str()); 

		CloseMPEG ();
		fclose(mpeg);
		src.flushMemory();

		exit(0);
	}

	fprintf(stderr, "Unknown Filter Name\n");
	return 0;

}

int saveFrame (int nr, int nc, char *pixels, Image &frame)
{
	for(int i=0; i<nr; i++)
		for(int j=0; j<nc; j++)
		{
			//printf("%i %i %i %i\n",pixels[0],pixels[1],pixels[2],pixels[3]);
			frame(i,j,RED) = (unsigned char)(*(pixels+3));
			frame(i,j,GREEN) = (unsigned char)(*(pixels+2));
			frame(i,j,BLUE) = (unsigned char)(*(pixels+1));
			pixels=pixels+4;
		}

		return 0;
}
/* this reads the image into a ImVfb and returns a pointer to it */

