
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
		fprintf(stderr, "Please tell me the operator's name ^^\n");
		exit(0);

	}


	if (strncasecmp(argv[0],"videoprocess",MAXLEN)==0)
	{
		if (argc!=6)
		{
			fprintf(stderr, "Sobel Operators require 3 arguments\n");
                        exit(0);
                }
                //init:
                FILE            *mpeg;
                ImageDesc       img;
                Boolean         moreframes = TRUE;
                char            *pixels;
                Image           src;
                int                     count=0;
                char            tgtname[100];
		char		tgt0name[100];
                int             framenum;

                mpeg = fopen (argv[2], "rb");
                SetMPEGOption (MPEG_DITHER, FULL_COLOR_DITHER);
                OpenMPEG (mpeg, &img);

                src.initialize(img.Height, img.Width);
                pixels = (char *) malloc (img.Size);

                ROI roi;
                roi.CreateROIList(argv[5]);
                Image tgt;
		Image tgt0;

                tgt.initialize(src.NR,src.NC);
                tgt0.initialize(src.NR,src.NC);

                fprintf(stderr,"\nBeginning to extract frames...\n");

                while (moreframes)
                {
                        moreframes = GetMPEGFrame (pixels);
                        if (saveFrame (img.Height, img.Width, pixels, src) != 0)                                                                                        
                        {
                                (void) fprintf (stderr,"\nError Saving Frame\nExitting...\n");
                                exit (-2);
                        }

                        sprintf(tgt0name,"%s%d.ppm", argv[3], count++);
                        sprintf(tgtname,"%s%d.ppm", argv[4], count++);

                        framenum = count -1;
                        if (roi.InSOI(framenum))
                        {
                                //if (strncasecmp(argv[1],"Sobel",MAXLEN)==0)
                                  //      ipTool::Sobel(src, tgt0, tgt, roi);
                                if (strncasecmp(argv[1],"Sobelbin",MAXLEN)==0)
                                        ipTool::Sobelbin(src, tgt0, tgt, roi);
                                if (strncasecmp(argv[1],"SobelColbin",MAXLEN)==0)
                                        ipTool::SobelColbin(src, tgt0, tgt, roi);
                                if (strncasecmp(argv[1],"SobelR",MAXLEN)==0)
                                        ipTool::SobelR(src, tgt0, tgt, roi);
                                if (strncasecmp(argv[1],"SobelG",MAXLEN)==0)
                                        ipTool::SobelG(src, tgt0, tgt, roi);
                                if (strncasecmp(argv[1],"SobelB",MAXLEN)==0)
                                        ipTool::SobelB(src, tgt0, tgt, roi);
			        	if (strncasecmp(argv[1],"SobelH",MAXLEN)==0)
              				  ipTool::SobelH(src, tgt0,tgt, roi);
					if (strncasecmp(argv[1],"SobelS",MAXLEN)==0)
                				ipTool::SobelS(src, tgt0, tgt, roi);   
        				if (strncasecmp(argv[1],"SobelI",MAXLEN)==0)
                				ipTool::SobelI(src, tgt0, tgt, roi);
					
					
                        }
                        if (strncasecmp(argv[1],"Sobelbin",MAXLEN)==0)
                                ipTool::pgm2ppm(src, tgt);
                        tgt0.save(tgt0name);
			   tgt.save(tgtname);

                }

                fprintf(stderr,"\n***DONE***\n\n");

                CloseMPEG ();
                fclose (mpeg);
                src.flushMemory();

                exit(0);



//      fprintf(stderr, "Unknow Operator Name\n");
//      return 0;
    }







if (strncasecmp(argv[0],"outputROI",MAXLEN)==0)
{
        if (argc!=6)
        {
                fprintf(stderr, "Sobel Operators require 4 arguments\n");
                exit(0);
        }
        char srcname[MAXLEN];
        char tgtname[MAXLEN];
        char tgt0name[MAXLEN];
        int threshhold;

        Image src;
        Image tgt,tgt0;
        ROI roi;

        strcpy(srcname,argv[2]);        //source
        strcpy(tgt0name,argv[3]);        //temp-target
        strcpy(tgtname,argv[4]);
        roi.CreateROIList(argv[5]);     //roi
        src.read(srcname);
        tgt.initialize(src.NR,src.NC);
        tgt0.initialize(src.NR,src.NC);


        //if (strncasecmp(argv[1],"Sobel",MAXLEN)==0)
                //ipTool::Sobel(src, tgt0, tgt, roi);
        if (strncasecmp(argv[1],"Sobelbin",MAXLEN)==0)
                ipTool::Sobelbin(src, tgt0, tgt, roi);
        if (strncasecmp(argv[1],"SobelColbin",MAXLEN)==0)
                ipTool::SobelColbin(src, tgt0, tgt, roi);
        if (strncasecmp(argv[1],"SobelR",MAXLEN)==0)
                ipTool::SobelR(src, tgt0, tgt, roi);
        if (strncasecmp(argv[1],"SobelG",MAXLEN)==0)
                ipTool::SobelG(src, tgt0, tgt, roi);
        if (strncasecmp(argv[1],"SobelB",MAXLEN)==0)
                ipTool::SobelB(src, tgt0, tgt, roi);
        if (strncasecmp(argv[1],"SobelH",MAXLEN)==0)
                ipTool::SobelH(src, tgt0, tgt, roi);
      	 if (strncasecmp(argv[1],"SobelS",MAXLEN)==0)
                ipTool::SobelS(src, tgt0, tgt, roi);			
        if (strncasecmp(argv[1],"SobelI",MAXLEN)==0)
                ipTool::SobelI(src, tgt0, tgt, roi);
	
		
	 tgt0.save(tgt0name);
        tgt.save(tgtname);
                exit(0);


}

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
