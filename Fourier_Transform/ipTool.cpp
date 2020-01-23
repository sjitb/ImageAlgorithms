// ipTool.cpp: implementation of the ipTool class.
//  Please see the README.txt for comments
//////////////////////////////////////////////////////////////////////

#include "ipTool.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ipTool::ipTool(){}
ipTool::~ipTool(){}


void ipTool::ppm2i(Image &src, Image &tgt) {

	
	tgt.initialize(src.NR,src.NC);
	for(int i = 0; i < src.NR; i++)
		for(int j = 0; j < src.NC; j++) {
			int r,g,b;
			double h,s,in;
			r=src(i,j,RED);
			g=src(i,j,GREEN);
			b=src(i,j,BLUE);
			rgb2hsi(r,g,b,h,s,in);
			tgt(i,j,RED) = (int)in;
			tgt(i,j,BLUE) = (int)in;
			tgt(i,j,GREEN) = (int)in;
		}

}

void ipTool::PassFilter(Image &src, ROI &roi, Image &tgt, Image &tgt2, char* filt) {

	int M=src.NR;
	int N=src.NC;

	fftw_real a[M][2*(N/2+1)], b[M][2*(N/2+1)], c[M][N];
	fftw_complex *A, *B, C[M][N/2+1],D[M][N];
	rfftwnd_plan p, pinv;
	fftw_real scale = 1.0 / (M * N);

	int i, j;

	/* create plan for FFT; */
	p    = rfftw2d_create_plan(M, N, FFTW_REAL_TO_COMPLEX,
					FFTW_ESTIMATE | FFTW_IN_PLACE);

	/* create plan for IFFT; */
	pinv = rfftw2d_create_plan(M, N, FFTW_COMPLEX_TO_REAL,
					FFTW_ESTIMATE);

	/* aliases for accessing complex transform outputs: */
	A = (fftw_complex*) &a[0][0];
	B = (fftw_complex*) &b[0][0];

	/* let a[i][j]=src(i,j,RED), then image data is stored in 'a'  */  
	for (i = 0; i < M; ++i)
		for (j = 0; j < N; ++j) {
			a[i][j] =src(i,j,RED) ;
		}
	/* Fast Fourier Transform: save the result of FFT in 'A' */  
	rfftwnd_one_real_to_complex(p, &a[0][0], NULL);

	/********************************************************************/
	int thresh = roi.roiList[0].thresh;
	if (strncasecmp(filt,"FFT",MAXLEN)==0)
		for (i = 0; i < M; ++i)
			for (j = 0; j < N/2+1; ++j) {
				int ij = i*(N/2+1) + j;
				B[ij].re=1;
				B[ij].im=0;
			}
	if (strncasecmp(filt,"LPF",MAXLEN)==0)
		for (i = 0; i < M; ++i)
			for (j = 0; j < N/2+1; ++j) {
				double distTop = 0;
				double distBot = 0;
				int ij = i*(N/2+1) + j;
				distTop = sqrt(pow(i,2)+pow(j,2));
				distBot = sqrt(pow(M-i,2)+pow(j,2));
				if( distTop < thresh || distBot < thresh) {
					B[ij].re=1;
					B[ij].im=0;
				} else {
					B[ij].re=0;
					B[ij].im=0;
				}
			}
	if (strncasecmp(filt,"HPF",MAXLEN)==0)
		for (i = 0; i < M; ++i)
			for (j = 0; j < N/2+1; ++j) {
				double distTop = 0;
				double distBot = 0;
				int ij = i*(N/2+1) + j;
				distTop = sqrt(pow(i,2)+pow(j,2));
				distBot = sqrt(pow(M-i,2)+pow(j,2));
				if( distTop  > thresh && distBot > thresh) {
					B[ij].re=1;
					B[ij].im=0;
				} else {
					B[ij].re=0;
					B[ij].im=0;
				}
		
		  	}
	if (strncasecmp(filt,"BPF",MAXLEN)==0) {
		int thresh2 = roi.roiList[0].winSize;
		for (i = 0; i < M; ++i)
			for (j = 0; j < N/2+1; ++j) {
				double distTop = 0;
				double distBot = 0;
				int ij = i*(N/2+1) + j;
				distTop = sqrt(pow(i,2)+pow(j,2));
				distBot = sqrt(pow(M-i,2)+pow(j,2));
				if( (distTop > thresh2 && distBot > thresh2) && (distTop  < thresh || distBot < thresh) ) {
					B[ij].re=1;
					B[ij].im=0;
				} else {
					B[ij].re=0;
					B[ij].im=0;
				}
			}
	}
     	/********************************************************************/

    	/* Mutiply image and filter in Fourier Domain */
 	for (i = 0; i < M; ++i)
          for (j = 0; j < N/2+1; ++j) {
               int ij = i*(N/2+1) + j;
               C[i][j].re = (A[ij].re * B[ij].re
                             - A[ij].im * B[ij].im) * scale;
               C[i][j].im = (A[ij].re * B[ij].im
                             + A[ij].im * B[ij].re) * scale;
	  }
 
   	/* Inverse transform to get c, the convolution of a and b;
        	this has the side effect of overwriting C */
	rfftwnd_one_complex_to_real(pinv, &C[0][0], &c[0][0]);
	rfftwnd_destroy_plan(p);
	rfftwnd_destroy_plan(pinv);

	/* Save Fourier Amplitude Image */
	for (i=0; i<M; ++i)
		for (j=0; j<N; ++j) {
			int ij = abs((M/2-i))*(N/2+1) + abs(N/2+1-j);
			tgt2(i,j,RED)=abs((int)(a[abs(M/2-i)][abs(N/2-j)]*B[ij].re*scale*255.0));
		}
	/* Save the output image in 'tgt' */
	for (i=0; i<M; ++i)
		for (j=0; j<N; ++j)
			if(roi.InROI(i,j)!=-1)
				tgt(i,j,RED)=(int)c[i][j];
			else
				tgt(i,j,RED)=src(i,j,RED);

}




//Assignment 2. See below for Assignment 3.
void ipTool::pgm2ppm(Image &src, Image &tgt) {
  for(int i=0;i<tgt.NR;i++){
    for(int j=0;j<tgt.NC;j++){     
      tgt(i,j,RED)=src(i,j,RED);
      tgt(i,j,GREEN)=src(i,j,RED);
      tgt(i,j,BLUE)=src(i,j,RED);
    }
  }  
}
void ipTool::ppm2pgm(Image &src, Image &tgt) {
  for(int i=0;i<tgt.NR;i++)
    for(int j=0;j<tgt.NC;j++)
      tgt(i,j,RED)=(src(i,j,RED)+src(i,j,GREEN)+src(i,j,BLUE))/3;
}

//static void ROIThresh(Image& src, ROI roi, Image& tgt);
void ipTool::ROIThresh(Image &src, ROI &roi, Image &tgt) {
  for(int i=0;i<tgt.NR;i++)
    for(int j=0;j<tgt.NC;j++)
      if(roi.InROI(i,j)!=-1)
        if( src(i,j,RED) > roi.roiList[roi.InROI(i,j)].thresh )
	  tgt(i,j,RED)=255;
	else
	  tgt(i,j,RED)=0;
	else
	  tgt(i,j,RED)=src(i,j,RED);
}

//static void OutputROI(Image& src, ROI roi, Image& tgt);
void ipTool::OutputROI(Image &src, ROI &roi, Image &tgt) {
  for(int i=0;i<tgt.NR;i++)
    for(int j=0;j<tgt.NC;j++)
      if(roi.InROI(i,j)!=-1) {
        tgt(i,j,RED)=src(i,j,RED);
	tgt(i,j,BLUE)=src(i,j,BLUE);
	tgt(i,j,GREEN)=src(i,j,GREEN);
      } else {
	tgt(i,j,RED)=0;
	tgt(i,j,BLUE)=0;
	tgt(i,j,GREEN)=0;
      }
}

//static void GrayArithmetic(Image &src,ROI &roi,Image &tgt);
void ipTool::GrayArithmetic(Image &src,ROI &roi,Image &tgt) {
  for(int i=0;i<tgt.NR;i++)
    for(int j=0;j<tgt.NC;j++)
      if (roi.InROI(i,j)!=-1)
        if(src(i,j,RED)+roi.roiList[roi.InROI(i,j)].thresh>255||src(i,j,RED)+roi.roiList[roi.InROI(i,j)].thresh<0)
          if(src(i,j,RED)+roi.roiList[roi.InROI(i,j)].thresh>255)
            tgt(i,j,RED)=255;
          else
            tgt(i,j,RED)=0;
        else
          tgt(i,j,RED)=src(i,j,RED)+roi.roiList[roi.InROI(i,j)].thresh;
      else
        tgt(i,j,RED)=src(i,j,RED);
}

//static void AdaptiveThresh(Image &src,ROI roi,Image &tgt);
void ipTool::AdaptiveThresh(Image &src,ROI &roi,Image &tgt)
{
	for(int i=0;i<tgt.NR;i++){
		for(int j=0;j<tgt.NC;j++){
			if (roi.InROI(i,j)!=-1)
			{
				int thresh=roi.roiList[roi.InROI(i,j)].thresh;
				int winsize=roi.roiList[roi.InROI(i,j)].winSize;
				double avg = 0;
				double nThresh = 0;
				int valid = 0;
				for(int di = (winsize-1)/-2;di<(winsize-1)/2;di++) 
					for(int dj = (winsize-1)/-2;dj<(winsize-1)/2;dj++)
					{
						if(!(i+di>tgt.NR||i+di<0||j+dj>tgt.NC||j+dj<0))
						{
							if(roi.InROI(i+di,j+dj)!=-1) 
							{
								avg += src(i+di,j+dj,RED);
								valid++;
							}
						}
					}
				avg /= valid;
				nThresh = (double)(thresh)+avg;
				if (src(i,j,RED)>nThresh)
					tgt(i,j,RED)=255;
				else
					tgt(i,j,RED)=0;
				
			}
			else
				tgt(i,j,RED)=src(i,j,RED);
		}
	}	

}

//static void ColorThresh(Image &src,ROI roi,Image &tgt);
void ipTool::ColorThresh(Image &src,ROI &roi,Image &tgt)
{
	for(int i=0;i<tgt.NR;i++){
		for(int j=0;j<tgt.NC;j++){
			if (roi.InROI(i,j)!=-1)
			{
				double radius=roi.roiList[roi.InROI(i,j)].radius;
				int cred=roi.roiList[roi.InROI(i,j)].cRed;
				int cgreen=roi.roiList[roi.InROI(i,j)].cGreen;
				int cblue=roi.roiList[roi.InROI(i,j)].cBlue;
				double dist = sqrt(pow(src(i,j,RED)-cred,2)+pow(src(i,j,GREEN)-cgreen,2)+pow(src(i,j,BLUE)-cblue,2));
				if(dist < radius) {
					tgt(i,j,RED)=0;
					tgt(i,j,BLUE)=0;
					tgt(i,j,GREEN)=0;
				}else{
					tgt(i,j,RED)=255;
					tgt(i,j,BLUE)=255;
					tgt(i,j,GREEN)=255;
				}
			}
			else
			{
				tgt(i,j,RED)=src(i,j,RED);
				tgt(i,j,GREEN)=src(i,j,GREEN);
				tgt(i,j,BLUE)=src(i,j,BLUE);
			}
		}
	}	

}

//static void ColorArithmetic(Image &src,ROI roi,Image &tgt);
void ipTool::ColorArithmetic(Image &src, ROI &roi,Image &tgt)
{
	for(int i=0;i<tgt.NR;i++){
		for(int j=0;j<tgt.NC;j++){
			if (roi.InROI(i,j)!=-1)
			{
				int cred=roi.roiList[roi.InROI(i,j)].cRed;
				int cgreen=roi.roiList[roi.InROI(i,j)].cGreen;
				int cblue=roi.roiList[roi.InROI(i,j)].cBlue;
				if(src(i,j,RED)+cred>255||src(i,j,RED)+cred<0)
					if(src(i,j,RED)+cred>255)
						tgt(i,j,RED)=255;
					else
						tgt(i,j,RED)=0;
				else
					tgt(i,j,RED)=src(i,j,RED)+cred;
				if(src(i,j,GREEN)+cgreen>255||src(i,j,GREEN)+cgreen<0)
					if(src(i,j,GREEN)+cgreen>255)
						tgt(i,j,GREEN)=255;
					else
						tgt(i,j,GREEN)=0;
				else
					tgt(i,j,GREEN)=src(i,j,GREEN)+cgreen;
				if(src(i,j,BLUE)+cblue>255||src(i,j,BLUE)+cblue<0)
					if(src(i,j,BLUE)+cblue>255)
						tgt(i,j,BLUE)=255;
					else
						tgt(i,j,BLUE)=0;
				else
					tgt(i,j,BLUE)=src(i,j,BLUE)+cblue;
			}
			else
			{
				tgt(i,j,RED)=src(i,j,RED);
				tgt(i,j,GREEN)=src(i,j,GREEN);
				tgt(i,j,BLUE)=src(i,j,BLUE);
			}
		}
	}	

}


//Assignment 3
// Sobel for Gray and Color

//  rgb2hsi used for color space conversion 
void ipTool::rgb2hsi(int r, int g, int b, double &h, double &s, double &i) {
	//Compute Intensity
	i=double(r+g+b)/3.0;


	double red = double(r)/255.0;
	double green = double(g)/255.0;
	double blue = double(b)/255.0;

	if(!(r==0&&b==0&&g==0)) {
		red=red/(red+green+blue);
		green=green/(red+green+blue);
		blue=blue/(red+green+blue);
	//Compute saturation
		if(r<=g&&r<=b)
			s=1.0-3.0*red;
		else if(g<=r&&g<=b)
			s=1.0-3.0*green;
		else
			s=1.0-3.0*blue;
		s*=255.0;
	//Compute Hue
		h=acos(0.5*(double(red-green)+double(red-blue))/sqrt(pow(red-green,2)+double(red-blue)*double(green-blue)));
		if(b>g)
			h=2.0*M_PI-h;
		h=h/(2*M_PI)*255.0;
	} else { //Gray image
		h=0;
		s=0;
	}

}

void ipTool::hsi2rgb(double h, double s, double i, int &r, int &g, int &b) {
	double rt,gt,bt;
	if(h>0&&h<2.0/3.0*M_PI) {
		rt=((1+(double(s)*cos(h))/(cos(1.0/3.0*M_PI-double(h))))/3.0);
		bt=(1-s)/3;
		gt=1-b-r;
	} else if(h<2.0/3.0*M_PI&&h>4.0/3.0*M_PI) {
		gt=(1+(s*cos(h))/(cos(1.0/3.0*M_PI-h)))/3.0;
		rt=(1-s)/3;
		gt=1-b-r;
	} else {
		bt=(1+(s*cos(h))/(cos(1.0/3.0*M_PI-h)))/3.0;
		gt=(1-s)/3;
		rt=1-g-b;
	}		

	r=int(rt*3.0*i);	
	g=int(gt*3.0*i);	
	b=int(bt*3.0*i);
	
}


//static void Sobel(Image& src, ROI &roi, Image& tgt, Image& tgt2);
void ipTool::Sobel(Image& src, ROI &roi, Image& tgt, Image& tgt2) {
	int * mask [3];
	for(int i = 0; i < 3; i++)
		mask[i] = new int[3];
	mask[0][0] = -1;
	mask[0][1] = 0;
	mask[0][2] = 1;
	mask[1][0] = -2;
	mask[1][1] = 0;
	mask[1][2] = 2;
	mask[2][0] = -1;
	mask[2][1] = 0;
	mask[2][2] = 1;
	for(int i=0;i<tgt.NR;i++){
		for(int j=0;j<tgt.NC;j++){
			if (roi.InROI(i,j)!=-1){
				int dx = 0;
				int dy = 0;
				for(int x = -1; x <=1; x++)
				  for(int y=-1; y<=1; y++)
				    if(i+x&&j+y) {
				      dx+=src(i+x,j+y,RED)*mask[x+1][y+1];
				      dy+=src(i+x,j+y,RED)*mask[y+1][x+1];
				    }
				double amp = sqrt(pow(dx,2)+pow(dy,2));
				tgt(i,j,RED)=int(amp);
				tgt(i,j,GREEN)=int(amp);
				tgt(i,j,BLUE)=int(amp);
				if(amp > roi.roiList[roi.InROI(i,j)].thresh) { 
					tgt2(i,j,RED)=int(amp);
					tgt2(i,j,GREEN)=int(amp);
					tgt2(i,j,BLUE)=int(amp);
				} else {
					tgt2(i,j,RED)=0;
					tgt2(i,j,GREEN)=0;
					tgt2(i,j,BLUE)=0;
				}
			}
			else
			{
				tgt(i,j,RED)=src(i,j,RED);
				tgt(i,j,GREEN)=src(i,j,GREEN);
				tgt(i,j,BLUE)=src(i,j,BLUE);
				tgt2(i,j,RED)=src(i,j,RED);
				tgt2(i,j,GREEN)=src(i,j,GREEN);
				tgt2(i,j,BLUE)=src(i,j,BLUE);
			}
		}
	}			
}

//static void SobelR(Image& src, ROI &roi, Image& tgt, Image& tgt2);
void ipTool::SobelR(Image& src, ROI &roi, Image& tgt, Image& tgt2){ 
	int * mask [3];
	for(int i = 0; i < 3; i++)
		mask[i] = new int[3];
	mask[0][0] = -1;
	mask[0][1] = 0;
	mask[0][2] = 1;
	mask[1][0] = -2;
	mask[1][1] = 0;
	mask[1][2] = 2;
	mask[2][0] = -1;
	mask[2][1] = 0;
	mask[2][2] = 1;
	for(int i=0;i<tgt.NR;i++){
		for(int j=0;j<tgt.NC;j++){
			if (roi.InROI(i,j)!=-1){
				int dx = 0;
				int dy = 0;
				for(int x = -1; x <=1; x++)
				  for(int y=-1; y<=1; y++)
				    if(i+x&&j+y) {
				      dx+=src(i+x,j+y,RED)*mask[x+1][y+1];
				      dy+=src(i+x,j+y,RED)*mask[y+1][x+1];
				    }
				double amp = sqrt(pow(dx,2)+pow(dy,2));
				tgt(i,j,RED)=int(amp);
				tgt(i,j,GREEN)=int(amp);
				tgt(i,j,BLUE)=int(amp);
				if(amp > roi.roiList[roi.InROI(i,j)].thresh) {
					tgt2(i,j,RED)=int(amp);
					tgt2(i,j,GREEN)=int(amp);
					tgt2(i,j,BLUE)=int(amp);
				 }else{
					tgt2(i,j,RED)=0;
					tgt2(i,j,GREEN)=0;
					tgt2(i,j,BLUE)=0;
				}
			}
			else
			{
				tgt(i,j,RED)=src(i,j,RED);
				tgt(i,j,GREEN)=src(i,j,GREEN);
				tgt(i,j,BLUE)=src(i,j,BLUE);
				tgt2(i,j,RED)=src(i,j,RED);
				tgt2(i,j,GREEN)=src(i,j,GREEN);
				tgt2(i,j,BLUE)=src(i,j,BLUE);
			}
		}
	}			
}
//static void SobelG(Image& src, ROI &roi, Image& tgt, Image& tgt2);
void ipTool::SobelG(Image& src, ROI &roi, Image& tgt, Image& tgt2){ 
	int * mask [3];
	for(int i = 0; i < 3; i++)
		mask[i] = new int[3];
	mask[0][0] = -1;
	mask[0][1] = 0;
	mask[0][2] = 1;
	mask[1][0] = -2;
	mask[1][1] = 0;
	mask[1][2] = 2;
	mask[2][0] = -1;
	mask[2][1] = 0;
	mask[2][2] = 1;
	for(int i=0;i<tgt.NR;i++){
		for(int j=0;j<tgt.NC;j++){
			if (roi.InROI(i,j)!=-1){
				int dx = 0;
				int dy = 0;
				for(int x = -1; x <=1; x++)
				  for(int y=-1; y<=1; y++)
				    if(i+x&&j+y) {
				      dx+=src(i+x,j+y,GREEN)*mask[x+1][y+1];
				      dy+=src(i+x,j+y,GREEN)*mask[y+1][x+1];
				    }
				double amp = sqrt(pow(dx,2)+pow(dy,2));
				tgt(i,j,RED)=int(amp);
				tgt(i,j,GREEN)=int(amp);
				tgt(i,j,BLUE)=int(amp);
				if(amp > roi.roiList[roi.InROI(i,j)].thresh) {
					tgt2(i,j,RED)=int(amp);
					tgt2(i,j,GREEN)=int(amp);
					tgt2(i,j,BLUE)=int(amp);
				 }else{
					tgt2(i,j,RED)=0;
					tgt2(i,j,GREEN)=0;
					tgt2(i,j,BLUE)=0;
				}
			}
			else
			{
				tgt(i,j,RED)=src(i,j,RED);
				tgt(i,j,GREEN)=src(i,j,GREEN);
				tgt(i,j,BLUE)=src(i,j,BLUE);
				tgt2(i,j,RED)=src(i,j,RED);
				tgt2(i,j,GREEN)=src(i,j,GREEN);
				tgt2(i,j,BLUE)=src(i,j,BLUE);
			}
		}
	}			
}
//static void SobelB(Image& src, ROI &roi, Image& tgt, Image& tgt2);
void ipTool::SobelB(Image& src, ROI &roi, Image& tgt, Image& tgt2){ 
	int * mask [3];
	for(int i = 0; i < 3; i++)
		mask[i] = new int[3];
	mask[0][0] = -1;
	mask[0][1] = 0;
	mask[0][2] = 1;
	mask[1][0] = -2;
	mask[1][1] = 0;
	mask[1][2] = 2;
	mask[2][0] = -1;
	mask[2][1] = 0;
	mask[2][2] = 1;
	for(int i=0;i<tgt.NR;i++){
		for(int j=0;j<tgt.NC;j++){
			if (roi.InROI(i,j)!=-1){
				int dx = 0;
				int dy = 0;
				for(int x = -1; x <=1; x++)
				  for(int y=-1; y<=1; y++)
				    if(i+x&&j+y) {
				      dx+=src(i+x,j+y,BLUE)*mask[x+1][y+1];
				      dy+=src(i+x,j+y,BLUE)*mask[y+1][x+1];
				    }
				double amp = sqrt(pow(dx,2)+pow(dy,2));
				tgt(i,j,RED)=int(amp);
				tgt(i,j,GREEN)=int(amp);
				tgt(i,j,BLUE)=int(amp);
				if(amp > roi.roiList[roi.InROI(i,j)].thresh) {
					tgt2(i,j,RED)=int(amp);
					tgt2(i,j,GREEN)=int(amp);
					tgt2(i,j,BLUE)=int(amp);
				 }else{
					tgt2(i,j,RED)=0;
					tgt2(i,j,GREEN)=0;
					tgt2(i,j,BLUE)=0;
				}
			}
			else
			{
				tgt(i,j,RED)=src(i,j,RED);
				tgt(i,j,GREEN)=src(i,j,GREEN);
				tgt(i,j,BLUE)=src(i,j,BLUE);
				tgt2(i,j,RED)=src(i,j,RED);
				tgt2(i,j,GREEN)=src(i,j,GREEN);
				tgt2(i,j,BLUE)=src(i,j,BLUE);
			}
		}
	}			
}
//static void SobelRGB(Image& src, ROI &roi, Image& tgt, Image& tgt2);
void ipTool::SobelRGB(Image& src, ROI &roi, Image& tgt, Image& tgt2){ 
	int * mask [3];
	for(int i = 0; i < 3; i++)
		mask[i] = new int[3];
	mask[0][0] = -1;
	mask[0][1] = 0;
	mask[0][2] = 1;
	mask[1][0] = -2;
	mask[1][1] = 0;
	mask[1][2] = 2;
	mask[2][0] = -1;
	mask[2][1] = 0;
	mask[2][2] = 1;
	for(int i=0;i<tgt.NR;i++){
		for(int j=0;j<tgt.NC;j++){
			if (roi.InROI(i,j)!=-1){
				int dx[3] = {0,0,0};
				int dy[3] = {0,0,0};
				for(int x = -1; x <=1; x++)
				  for(int y=-1; y<=1; y++)
				    if(i+x&&j+y) {
				      dx[0]+=src(i+x,j+y,RED)*mask[x+1][y+1];
				      dy[0]+=src(i+x,j+y,RED)*mask[y+1][x+1];
				      dx[1]+=src(i+x,j+y,GREEN)*mask[x+1][y+1];
				      dy[1]+=src(i+x,j+y,GREEN)*mask[y+1][x+1];
				      dx[2]+=src(i+x,j+y,BLUE)*mask[x+1][y+1];
				      dy[2]+=src(i+x,j+y,BLUE)*mask[y+1][x+1];
				    }
				double amp = 0;
				for(int l = 0; l < 3; l++)
					amp += sqrt(pow(dx[l],2)+pow(dy[l],2))/3.0;
				tgt(i,j,RED)=int(amp);
				tgt(i,j,GREEN)=int(amp);
				tgt(i,j,BLUE)=int(amp);
				if(amp > roi.roiList[roi.InROI(i,j)].thresh) {
					tgt2(i,j,RED)=int(amp);
					tgt2(i,j,GREEN)=int(amp);
					tgt2(i,j,BLUE)=int(amp);
				 }else{
					tgt2(i,j,RED)=0;
					tgt2(i,j,GREEN)=0;
					tgt2(i,j,BLUE)=0;
				}
			}
			else
			{
				tgt(i,j,RED)=src(i,j,RED);
				tgt(i,j,GREEN)=src(i,j,GREEN);
				tgt(i,j,BLUE)=src(i,j,BLUE);
				tgt2(i,j,RED)=src(i,j,RED);
				tgt2(i,j,GREEN)=src(i,j,GREEN);
				tgt2(i,j,BLUE)=src(i,j,BLUE);
			}
		}
	}			
}
//static void SobelH(Image& src, ROI &roi, Image& tgt, Image& tgt2);
void ipTool::SobelH(Image& src, ROI &roi, Image& tgt, Image& tgt2){ 
	double * mask [3];
	for(int i = 0; i < 3; i++)
		mask[i] = new double[3];
	mask[0][0] = -1;
	mask[0][1] = 0;
	mask[0][2] = 1;
	mask[1][0] = -2;
	mask[1][1] = 0;
	mask[1][2] = 2;
	mask[2][0] = -1;
	mask[2][1] = 0;
	mask[2][2] = 1;

	double h,s,in;
	for(int i=0;i<tgt.NR;i++){
		for(int j=0;j<tgt.NC;j++){
			if (roi.InROI(i,j)!=-1){
				double dx[3] = {0,0,0};
				double dy[3] = {0,0,0};
				for(int x = -1; x <=1; x++)
				  for(int y=-1; y<=1; y++)
				    if(i+x&&j+y) {
				      int r,g,b;
				      r=src(i+x,j+y,RED);
				      g=src(i+x,j+y,GREEN);
				      b=src(i+x,j+y,BLUE);
				      rgb2hsi(r,g,b,h,s,in);
				      dx[0]+=h*mask[x+1][y+1];
				      dy[0]+=h*mask[y+1][x+1];
				    }
				double amp = 0;
				for(int l = 0; l < 1; l++)
					amp += sqrt(pow(dx[l],2)+pow(dy[l],2))/1.0;
				tgt(i,j,RED)=int(amp);
				tgt(i,j,GREEN)=int(amp);
				tgt(i,j,BLUE)=int(amp);
				if(amp > roi.roiList[roi.InROI(i,j)].thresh) {
					tgt2(i,j,RED)=int(amp);
					tgt2(i,j,GREEN)=int(amp);
					tgt2(i,j,BLUE)=int(amp);
				 }else{
					tgt2(i,j,RED)=0;
					tgt2(i,j,GREEN)=0;
					tgt2(i,j,BLUE)=0;
				}
			}
			else
			{
				tgt(i,j,RED)=src(i,j,RED);
				tgt(i,j,GREEN)=src(i,j,GREEN);
				tgt(i,j,BLUE)=src(i,j,BLUE);
				tgt2(i,j,RED)=src(i,j,RED);
				tgt2(i,j,GREEN)=src(i,j,GREEN);
				tgt2(i,j,BLUE)=src(i,j,BLUE);
			}
		}
	}			
}
//static void SobelS(Image& src, ROI &roi, Image& tgt, Image& tgt2);
void ipTool::SobelS(Image& src, ROI &roi, Image& tgt, Image& tgt2){ 
	double * mask [3];
	for(int i = 0; i < 3; i++)
		mask[i] = new double[3];
	mask[0][0] = -1;
	mask[0][1] = 0;
	mask[0][2] = 1;
	mask[1][0] = -2;
	mask[1][1] = 0;
	mask[1][2] = 2;
	mask[2][0] = -1;
	mask[2][1] = 0;

	double h,s,in;
	for(int i=0;i<tgt.NR;i++){
		for(int j=0;j<tgt.NC;j++){
			if (roi.InROI(i,j)!=-1){
				double dx[3] = {0,0,0};
				double dy[3] = {0,0,0};
				for(int x = -1; x <=1; x++)
				  for(int y=-1; y<=1; y++)
				    if(i+x&&j+y) {
				      int r,g,b;
				      r=src(i+x,j+y,RED);
				      g=src(i+x,j+y,GREEN);
				      b=src(i+x,j+y,BLUE);
				      rgb2hsi(r,g,b,h,s,in);
				      
				      dx[0]+=s*mask[x+1][y+1];
				      dy[0]+=s*mask[y+1][x+1];
				    }
				double amp = 0;
				for(int l = 0; l < 1; l++)
					amp += sqrt(pow(dx[l],2)+pow(dy[l],2))/1.0;
				
				tgt(i,j,RED)=int(amp);
				tgt(i,j,GREEN)=int(amp);
				tgt(i,j,BLUE)=int(amp);


				if(amp > roi.roiList[roi.InROI(i,j)].thresh) {
					tgt2(i,j,RED)=int(amp);
					tgt2(i,j,GREEN)=int(amp);
					tgt2(i,j,BLUE)=int(amp);
				 }else{
					tgt2(i,j,RED)=0;
					tgt2(i,j,GREEN)=0;
					tgt2(i,j,BLUE)=0;
				}
			}
			else
			{
				tgt(i,j,RED)=src(i,j,RED);
				tgt(i,j,GREEN)=src(i,j,GREEN);
				tgt(i,j,BLUE)=src(i,j,BLUE);
				tgt2(i,j,RED)=src(i,j,RED);
				tgt2(i,j,GREEN)=src(i,j,GREEN);
				tgt2(i,j,BLUE)=src(i,j,BLUE);
			}
		}
	}			
}
//static void SobelI(Image& src, ROI &roi, Image& tgt, Image& tgt2);
void ipTool::SobelI(Image& src, ROI &roi, Image& tgt, Image& tgt2) { 
	double * mask [3];
	for(int i = 0; i < 3; i++)
		mask[i] = new double[3];
	mask[0][0] = -1;
	mask[0][1] = 0;
	mask[0][2] = 1;
	mask[1][0] = -2;
	mask[1][1] = 0;
	mask[1][2] = 2;
	mask[2][0] = -1;
	mask[2][1] = 0;
	mask[2][2] = 1;

	double h,s,in;
	for(int i=0;i<tgt.NR;i++){
		for(int j=0;j<tgt.NC;j++){
			if (roi.InROI(i,j)!=-1){ 
				double dx[3] = {0,0,0};
				double dy[3] = {0,0,0};
				for(int x = -1; x <=1; x++)
				  for(int y=-1; y<=1; y++)
				    if(i+x&&j+y) {
				      int r,g,b;
				      r=src(i+x,j+y,RED);
				      g=src(i+x,j+y,GREEN);
				      b=src(i+x,j+y,BLUE);
				      rgb2hsi(r,g,b,h,s,in);
				      
				      dx[0]+=in*mask[x+1][y+1];
				      dy[0]+=in*mask[y+1][x+1];
				    }
				double amp = 0;
				for(int l = 0; l < 1; l++)
					amp += sqrt(pow(dx[l],2)+pow(dy[l],2))/1.0;
				tgt(i,j,RED)=int(amp);
				tgt(i,j,GREEN)=int(amp);
				tgt(i,j,BLUE)=int(amp);
				if(amp > roi.roiList[roi.InROI(i,j)].thresh) {
					tgt2(i,j,RED)=int(amp);
					tgt2(i,j,GREEN)=int(amp);
					tgt2(i,j,BLUE)=int(amp);
				 }else{
					tgt2(i,j,RED)=0;
					tgt2(i,j,GREEN)=0;
					tgt2(i,j,BLUE)=0;
				}
			} else {
				tgt(i,j,RED)=src(i,j,RED);
				tgt(i,j,GREEN)=src(i,j,GREEN);
				tgt(i,j,BLUE)=src(i,j,BLUE);
				tgt2(i,j,RED)=src(i,j,RED);
				tgt2(i,j,GREEN)=src(i,j,GREEN);
				tgt2(i,j,BLUE)=src(i,j,BLUE);
			}
		}
	}
}
//static void SobelHSI(Image& src, ROI &roi, Image& tgt, Image& tgt2);
void ipTool::SobelHSI(Image& src, ROI &roi, Image& tgt, Image& tgt2) { 
	double * mask [3];
	for(int i = 0; i < 3; i++)
		mask[i] = new double[3];
	mask[0][0] = -1;
	mask[0][1] = 0;
	mask[0][2] = 1;
	mask[1][0] = -2;
	mask[1][1] = 0;
	mask[1][2] = 2;
	mask[2][0] = -1;
	mask[2][1] = 0;
	mask[2][2] = 1;
	
	double h,s,in;
	for(int i=0;i<tgt.NR;i++){
		for(int j=0;j<tgt.NC;j++){
			if (roi.InROI(i,j)!=-1) {
				double dx[3] = {0,0,0};
				double dy[3] = {0,0,0};
				for(int x = -1; x <=1; x++)
				  for(int y=-1; y<=1; y++)
				    if(i+x&&j+y) {
				      int r,g,b;
				      r=src(i+x,j+y,RED);
				      g=src(i+x,j+y,GREEN);
				      b=src(i+x,j+y,BLUE);
				      rgb2hsi(r,g,b,h,s,in);
				      
				      dx[0]+=h*mask[x+1][y+1];
				      dy[0]+=h*mask[y+1][x+1];
				      dx[1]+=s*mask[x+1][y+1];
				      dy[1]+=s*mask[y+1][x+1];
				      dx[2]+=i*mask[x+1][y+1];
				      dy[2]+=i*mask[y+1][x+1];
				    }
				double amp = 0;
				for(int l = 0; l < 3; l++)
					amp += sqrt(pow(dx[l],2)+pow(dy[l],2))/3.0;
				tgt(i,j,RED)=int(amp);
				tgt(i,j,GREEN)=int(amp);
				tgt(i,j,BLUE)=int(amp);
				if(amp > roi.roiList[roi.InROI(i,j)].thresh) {
					tgt2(i,j,RED)=int(amp);
					tgt2(i,j,GREEN)=int(amp);
					tgt2(i,j,BLUE)=int(amp);
				 }else{
					tgt2(i,j,RED)=0;
					tgt2(i,j,GREEN)=0;
					tgt2(i,j,BLUE)=0;
				}
		
			}
			else
			{
				tgt(i,j,RED)=src(i,j,RED);
				tgt(i,j,GREEN)=src(i,j,GREEN);
				tgt(i,j,BLUE)=src(i,j,BLUE);
				tgt2(i,j,RED)=src(i,j,RED);
				tgt2(i,j,GREEN)=src(i,j,GREEN);
				tgt2(i,j,BLUE)=src(i,j,BLUE);
			}
		}
	}			
}

