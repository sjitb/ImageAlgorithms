
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

//function body;
int main(int argc, char* argv[]){

	if (argc<2)
	{
		fprintf(stderr, "Please tell me the filter's name ^^\n");
		exit(0);

	}


	if (strncasecmp(argv[1],"decode",MAXLEN)==0)
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
		char		*pixels;
		Image		src;
		int			count=0;
		char		tgtname[100];

		mpeg = fopen (argv[2], "rb");
		SetMPEGOption (MPEG_DITHER, FULL_COLOR_DITHER);
		OpenMPEG (mpeg, &img);
		
		src.initialize(img.Height, img.Width);
		pixels = (char *) malloc (img.Size);

		fprintf(stderr,"\nBeginning to extract frames...\n");

		while (moreframes)
		{
			moreframes = GetMPEGFrame (pixels);
			if (saveFrame (img.Height, img.Width, pixels, src) != 0) 
			{
				(void) fprintf (stderr,"\nError Saving Frame\nExitting...\n");
				exit (-2);
			}

			sprintf(tgtname,"%s%d.pgm", argv[3], count++);		

			src.save(tgtname);
		}
		fprintf(stderr,"\n***DONE***\n\n");

		CloseMPEG ();
		fclose (mpeg);
		src.flushMemory();

		exit(0);
	}

	if (strncasecmp(argv[1],"binarize",MAXLEN)==0)
	{
		if (argc!=5)
		{
			fprintf(stderr, "binarize require 3 arguments\n");
			exit(0);
		}
		//init:
		FILE 		*mpeg;
		ImageDesc 	img;
		Boolean		moreframes = TRUE;
		char		*pixels;
		Image		src;
		int			count=0;
		char		tgtname[100];
		int 		framenum;

		mpeg = fopen (argv[2], "rb");
		SetMPEGOption (MPEG_DITHER, FULL_COLOR_DITHER);
		OpenMPEG (mpeg, &img);
		
		src.initialize(img.Height, img.Width);
		pixels = (char *) malloc (img.Size);

		ROI roi;
		roi.CreateROIList(argv[4]);
		Image tgt;

		tgt.initialize(src.NR,src.NC);

		fprintf(stderr,"\nBeginning to extract frames...\n");

		while (moreframes)
		{
			moreframes = GetMPEGFrame (pixels);
			if (saveFrame (img.Height, img.Width, pixels, src) != 0) 
			{
				(void) fprintf (stderr,"\nError Saving Frame\nExitting...\n");
				exit (-2);
			}

			sprintf(tgtname,"%s%d.pgm", argv[3], count++);	
			framenum = count -1;
			if (roi.InSOI(framenum))
				ipTool::binarize(src, tgt, roi);   
			ipTool::pgm2ppm(src, tgt);
			src.save(tgtname);
			
		}
		
		fprintf(stderr,"\n***DONE***\n\n");

		CloseMPEG ();
		fclose (mpeg);
		src.flushMemory();

		exit(0);

	}


/* Adaptive Thresholding for Grey Level */

		if (strncasecmp(argv[1],"GreyThresh",MAXLEN)==0)
	{
		if (argc!=6)
		{
			fprintf(stderr, "GreyThresh require 4 arguments\n");
			exit(0);
		}
		//init:
		FILE 		*mpeg;
		ImageDesc 	img;
		Boolean		moreframes = TRUE;
		char		*pixels;
		Image		src;
		int		count=0;
		char		tgtname[100];
		int 		winsize;
		int 		framenum;


		mpeg = fopen (argv[2], "rb");
		SetMPEGOption (MPEG_DITHER, FULL_COLOR_DITHER);
		OpenMPEG (mpeg, &img);
		
		src.initialize(img.Height, img.Width);
		pixels = (char *) malloc (img.Size);

		ROI roi;
		roi.CreateROIList(argv[4]);
		winsize = atoi(argv[5]);
		Image tgt;

		tgt.initialize(src.NR,src.NC);

		fprintf(stderr,"\nBeginning to extract frames...\n");

		while (moreframes)
		{
			moreframes = GetMPEGFrame (pixels);
			if (saveFrame (img.Height, img.Width, pixels, src) != 0) 
			{
				(void) fprintf (stderr,"\nError Saving Frame\nExitting...\n");
				exit (-2);
			}

			sprintf(tgtname,"%s%d.ppm", argv[3], count++);  //check back
			framenum = count -1;
			if (roi.InSOI(framenum))
	
				ipTool::GreyThresh(src, tgt, roi, winsize);   
			ipTool::pgm2ppm(src, tgt);
			src.save(tgtname);

		}

		//ipTool::pgm2ppm(src, tgt);
		fprintf(stderr,"\n***DONE***\n\n");

		CloseMPEG ();
		fclose (mpeg);
		src.flushMemory();

		exit(0);

	}
/* colorbinarize */

		if (strncasecmp(argv[1],"Colorbinarize",MAXLEN)==0)
	{
		if (argc!=5)
		{
			fprintf(stderr, "decoding require 3 arguments\n");
			exit(0);
		}
		//init:
		FILE 		*mpeg;
		ImageDesc 	img;
		Boolean		moreframes = TRUE;
		char		*pixels;
		Image		src;
		int			count=0;
		char		tgtname[100];
		int 		framenum;



		mpeg = fopen (argv[2], "rb");
		SetMPEGOption (MPEG_DITHER, FULL_COLOR_DITHER);
		OpenMPEG (mpeg, &img);
		
		src.initialize(img.Height, img.Width);
		pixels = (char *) malloc (img.Size);

		ROI roi;
		roi.CreateROIList(argv[4]);
		Image tgt;

		tgt.initialize(src.NR,src.NC);

		fprintf(stderr,"\nBeginning to extract frames...\n");

		while (moreframes)
		{
			moreframes = GetMPEGFrame (pixels);
			if (saveFrame (img.Height, img.Width, pixels, src) != 0) 
			{
				(void) fprintf (stderr,"\nError Saving Frame\nExitting...\n");
				exit (-2);
			}

			sprintf(tgtname,"%s%d.ppm", argv[3], count++);	
			framenum = count -1;
			if (roi.InSOI(framenum))

				ipTool::Colorbinarize(src, tgt, roi);   

			src.save(tgtname);
		}
		fprintf(stderr,"\n***DONE***\n\n");

		CloseMPEG ();
		fclose (mpeg);
		src.flushMemory();

		exit(0);

	}

/* Smoothing */

		if (strncasecmp(argv[1],"Smoothing",MAXLEN)==0)
	{
		if (argc!=6)
		{
			fprintf(stderr, "Smoothing require 4 arguments\n");
			exit(0);
		}
		//init:
		FILE 		*mpeg;
		ImageDesc 	img;
		Boolean		moreframes = TRUE;
		char		*pixels;
		Image		src;
		int			count=0;
		char		tgtname[100];
		int		size;
		int 		framenum;



		mpeg = fopen (argv[2], "rb");
		SetMPEGOption (MPEG_DITHER, FULL_COLOR_DITHER);
		OpenMPEG (mpeg, &img);
		
		src.initialize(img.Height, img.Width);
		pixels = (char *) malloc (img.Size);

		ROI roi;
		roi.CreateROIList(argv[4]);
		size = atoi(argv[5]);
		Image tgt;

		tgt.initialize(src.NR,src.NC);

		fprintf(stderr,"\nBeginning to extract frames...\n");

		while (moreframes)
		{
			moreframes = GetMPEGFrame (pixels);
			if (saveFrame (img.Height, img.Width, pixels, src) != 0) 
			{
				(void) fprintf (stderr,"\nError Saving Frame\nExitting...\n");
				exit (-2);
			}

			sprintf(tgtname,"%s%d.ppm", argv[3], count++);
			framenum = count -1;	
			if (roi.InSOI(framenum))

				ipTool::Smoothing(src, tgt, roi, size);   
			ipTool::pgm2ppm(src, tgt);
			src.save(tgtname);
			ipTool::pgm2ppm(src, tgt);
		}

		//ipTool::pgm2ppm(src, tgt);

		fprintf(stderr,"\n***DONE***\n\n");

		CloseMPEG ();
		fclose (mpeg);
		src.flushMemory();

		exit(0);
	}



	fprintf(stderr, "Unknow Filter Name\n");
	return 0;
}


int saveFrame (int nr, int nc, char *pixels, Image &frame){
	for(int i=0; i<nr; i++)
		for(int j=0; j<nc; j++)
		{
			frame(i,j,RED) = *(pixels+3);
			frame(i,j,GREEN) = *(pixels+2);
			frame(i,j,BLUE) = *(pixels+1);
			pixels=pixels+4;
		}

		return 0;
}
/* this reads the image into a ImVfb and returns a pointer to it */
