// ipTool.cpp: implementation of the ipTool class.
//
//////////////////////////////////////////////////////////////////////

#include "ipTool.h"
//#include "iostream.h"
#include <sys/ddi.h>
#include <math.h>



#define ROW 1
#define COL 2

#define round(x) ( (x) >= 0           ? \
		      (int)((x) + 0.5): \
		      (int)((x) - 0.5)  \
		 )



int Gy[]={-1,-2,-1,0,0,0,1,2,1};

int Gx[]={-1,0,1,-2,0,2,-1,0,1};




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ipTool::ipTool()
{

}

ipTool::~ipTool()
{

}

int pixeltohsi(int r, int g, int b, Image& tgt, int m, int n, int count);
int convolve(Image& src, int x, int y, int color);

void ipTool::OutputROI(Image &src, ROI roi, Image &tgt)
{

	for(int i=0;i<tgt.NR;i++){
		for(int j=0;j<tgt.NC;j++){		
			if (roi.InROI(i,j))
			{
				tgt(i,j,RED)=src(i,j,RED);
			}
			else
				tgt(i,j,RED)=0;
		}
	}	


}


void ipTool::pgm2ppm(Image &src, Image &tgt)
{
    for(int i=0;i<tgt.NR;i++){
       for(int j=0;j<tgt.NC;j++){     
           tgt(i,j,RED)=src(i,j,RED);
           tgt(i,j,GREEN)=src(i,j,RED);
           tgt(i,j,BLUE)=src(i,j,RED);
       }
    }  
}

//void ipTool::Sobel(Image& src, Image& tgt0, Image& tgt, ROI roi)
/*{

	//Image amplitude;
	//tgt0.initialize(src.NR,src.NC);

	for(int i=0;i<tgt.NR;i++){
		for(int j=0;j<tgt.NC;j++){		
				tgt0(i,j,RED)= int (src(i,j,RED)+src(i,j,BLUE)+src(i,j,GREEN))/3;
				tgt(i,j,RED)=src(i,j,RED);
				tgt(i,j,GREEN)=src(i,j,GREEN);
				tgt(i,j,BLUE)=src(i,j,BLUE);	
			}
	}

	
	for (int i=0; i<roi.numBaseROI; i++)

	{
		for (int m=roi.roiList[i].x; m<roi.roiList[i].x+roi.roiList[i].sx; m++)
		{
			for (int n=roi.roiList[i].y; n<roi.roiList[i].y+roi.roiList[i].sy; n++)
			{
				tgt(m,n,RED) =convolve(tgt0,m,n,RED) ;
				

	        	}

		}		
	}
*/

void ipTool::Sobelbin(Image& src, Image& tgt0, Image& tgt, ROI roi)
{

	//Image tgt0;
	//tgt0.initialize(src.NR,src.NC);

	for(int i=0;i<tgt.NR;i++){
		for(int j=0;j<tgt.NC;j++){		
				tgt0(i,j,RED)= src(i,j,RED);
				tgt(i,j,RED)=src(i,j,RED);
					
			}
	}

	
	for (int i=0; i<roi.numBaseROI; i++)

	{
		for (int m=roi.roiList[i].x; m<roi.roiList[i].x+roi.roiList[i].sx; m++)
		{
			for (int n=roi.roiList[i].y; n<roi.roiList[i].y+roi.roiList[i].sy; n++)
			{
				tgt0(m,n,RED) =convolve(src,m,n,RED) ;
				tgt(m,n,RED) =convolve(src,m,n,RED) ;
		  		if(tgt(m,n,RED) > roi.roiList[i].thresh )
					tgt(m,n,RED)= 255;
				else
					tgt(m,n,RED)= 0;
			
	        	}

		}		
	}
} 


void ipTool::SobelColbin(Image& src, Image& tgt0, Image& tgt, ROI roi)
{
	
	Image tmp;
	tmp.initialize(src.NR,src.NC);
	for(int i=0;i<tgt.NR;i++){
		for(int j=0;j<tgt.NC;j++){		
				tgt0(i,j,RED)=src(i,j,RED);
				tgt0(i,j,GREEN)=src(i,j,GREEN);
				tgt0(i,j,BLUE)=src(i,j,BLUE);	

				tmp(i,j,RED)= int (src(i,j,RED)+src(i,j,BLUE)+src(i,j,GREEN))/3;
				tgt(i,j,RED)=src(i,j,RED);
				tgt(i,j,GREEN)=src(i,j,GREEN);
				tgt(i,j,BLUE)=src(i,j,BLUE);	
		}
	}

	
	for (int i=0; i<roi.numBaseROI; i++)

	{
		for (int m=roi.roiList[i].x; m<roi.roiList[i].x+roi.roiList[i].sx; m++)
		{
			for (int n=roi.roiList[i].y; n<roi.roiList[i].y+roi.roiList[i].sy; n++)
			{
				tgt0(m,n,RED)= int (tgt0(m,n,RED)+tgt0(m,n,BLUE)+tgt0(m,n,GREEN))/3;

				tgt(m,n,RED)= int (tgt(m,n,RED)+tgt(m,n,BLUE)+tgt(m,n,GREEN))/3;

				tgt0(m,n,RED) =convolve(tmp,m,n,RED) ;
				tgt0(m,n,GREEN) =tgt0(m,n,BLUE) =tgt0(m,n,RED) ;
				tgt(m,n,RED) =convolve(tmp,m,n,RED) ;
		  		if(tgt(m,n,RED) > roi.roiList[i].thresh )
					tgt(m,n,RED)= tgt(m,n,GREEN)=tgt(m,n,BLUE)=255;
				else
					tgt(m,n,RED)= tgt(m,n,GREEN)=tgt(m,n,BLUE)=0;
	        	}

		}		
	}
} 
 

void ipTool::SobelR(Image& src, Image& tgt0, Image& tgt, ROI roi)
{

	for(int i=0;i<tgt.NR;i++){
		for(int j=0;j<tgt.NC;j++){		
				tgt0(i,j,RED)=src(i,j,RED);
				tgt0(i,j,GREEN)=src(i,j,GREEN);
				tgt0(i,j,BLUE)=src(i,j,BLUE);	


				tgt(i,j,RED)=src(i,j,RED);
				tgt(i,j,GREEN)=src(i,j,GREEN);
				tgt(i,j,BLUE)=src(i,j,BLUE);	
		}
	}

	for (int i=0; i<roi.numBaseROI; i++)

	{

		for (int m=roi.roiList[i].x; m<roi.roiList[i].x+roi.roiList[i].sx; m++)
		{
			for (int n=roi.roiList[i].y; n<roi.roiList[i].y+roi.roiList[i].sy; n++)
			{
				//tgt0(i,j,RED)=src(i,j,RED);
				//tgt0(i,j,GREEN)=src(i,j,GREEN);
				//tgt0(i,j,BLUE)=src(i,j,BLUE);	

				tgt(m,n,RED) =convolve(src,m,n,RED) ;
		  		//if(tgt0(m,n,RED) > roi.roiList[i].Cred )
		      			tgt(m,n,GREEN) =tgt(m,n,BLUE)= 0;
		  		//else
		      		//	tgt(m,n,RED) = 0;
 
	        	}

		}		
	}
} 
void ipTool::SobelG(Image& src, Image& tgt0, Image& tgt, ROI roi)
{
	//Image tgt0;
	//tgt0.initialize(src.NR,src.NC);
	for(int i=0;i<tgt.NR;i++){
		for(int j=0;j<tgt.NC;j++){		
				tgt0(i,j,RED)=src(i,j,RED);
				tgt0(i,j,GREEN)=src(i,j,GREEN);
				tgt0(i,j,BLUE)=src(i,j,BLUE);	

				tgt(i,j,RED)=src(i,j,RED);
				tgt(i,j,GREEN)=src(i,j,GREEN);
				tgt(i,j,BLUE)=src(i,j,BLUE);	
		}
	}



	for (int i=0; i<roi.numBaseROI; i++)

	{

		for (int m=roi.roiList[i].x; m<roi.roiList[i].x+roi.roiList[i].sx; m++)
		{
			for (int n=roi.roiList[i].y; n<roi.roiList[i].y+roi.roiList[i].sy; n++)
			{
				tgt(m,n,GREEN) =convolve(src,m,n,GREEN) ;
		  		//if(tgt0(m,n,GREEN) > roi.roiList[i].Cgreen )
		      		//	tgt(m,n,GREEN) = 255;
		  		//else
		      			tgt(m,n,RED) = tgt(m,n,BLUE)=0;
 
	        	}

		}		
	}
} 
void ipTool::SobelB(Image& src, Image& tgt0, Image& tgt, ROI roi)
{

	//Image tgt0;
	//tgt0.initialize(src.NR,src.NC);
	for(int i=0;i<tgt.NR;i++){
		for(int j=0;j<tgt.NC;j++){		
				tgt0(i,j,RED)=src(i,j,RED);
				tgt0(i,j,GREEN)=src(i,j,GREEN);
				tgt0(i,j,BLUE)=src(i,j,BLUE);	

				tgt(i,j,RED)=src(i,j,RED);
				tgt(i,j,GREEN)=src(i,j,GREEN);
				tgt(i,j,BLUE)=src(i,j,BLUE);	
		}
	}

	
	for (int i=0; i<roi.numBaseROI; i++)

	{

		for (int m=roi.roiList[i].x; m<roi.roiList[i].x+roi.roiList[i].sx; m++)
		{
			for (int n=roi.roiList[i].y; n<roi.roiList[i].y+roi.roiList[i].sy; n++)
			{
				tgt(m,n,BLUE) =convolve(tgt0,m,n,BLUE) ;
		  		//if(tgt0(m,n,BLUE) > roi.roiList[i].Cblue )
		      		//	tgt(m,n,BLUE) = 255;
		  		//else
		      			tgt(m,n,RED)=tgt(m,n,GREEN) = 0;
	        	}

		}		
	}
} 


int convolve( Image& temp, int m, int n, int color)
{
    	int dx = 0;
	int dy = 0;
	int t=0;
     

	for(int i = -1; i <= 1; i++)
    	{
		for(int j = -1; j <= 1; j++)
		{
			dx=dx + (temp(m+i,n+j,color)*Gx[t]);
			dy=dy + (temp(m+i,n+j,color)*Gy[t]);
			//printf(" value of dx =%d, dy=%d, \n",dx,dy);
			t++;
			//printf(" value of t =%d \n",t);
			
		}
    	}
	dx=dx;
	dy=dy;
	int value =sqrt( (dx*dx) + (dy*dy) ) ;
	return value;

}


void ipTool::SobelI(Image& src, Image& tgt0, Image& tgt, ROI roi)
{
	//int chanI;
	int count=3;
 	Image tmp;
	tmp.initialize(src.NR,src.NC);
	for(int i=0;i<tgt.NR;i++){
		for(int j=0;j<tgt.NC;j++){
				tgt0(i,j,RED)=src(i,j,RED);	
				tgt0(i,j,GREEN)=src(i,j,GREEN);	
				tgt0(i,j,BLUE)=src(i,j,BLUE);

				tgt(i,j,RED)=src(i,j,RED);
				tgt(i,j,GREEN)=src(i,j,GREEN);
				tgt(i,j,BLUE)=src(i,j,BLUE);	
		}
	}


	for (int i=0; i<roi.numBaseROI; i++)

	{

		for (int m=roi.roiList[i].x; m<roi.roiList[i].x+roi.roiList[i].sx; m++)
		{
			for (int n=roi.roiList[i].y; n<roi.roiList[i].y+roi.roiList[i].sy; n++)
			{
			  
				int r = src(m,n,RED);
				int g = src(m,n,GREEN);
				int b = src(m,n,BLUE);
				tgt0(m,n,RED)=tgt0(m,n,GREEN)=tgt0(m,n,BLUE)= pixeltohsi(r, g, b, tgt0, m, n, count); //, chanS);
				tgt(m,n,RED)=pixeltohsi(r,g,b,tgt,m,n,count);
				//tgt(m,n,RED)=tgt(m,n,GREEN)=tgt(m,n,BLUE)=pixeltohsi(r,g,b,tgt,m,n,count); //chanS);
				tgt(m,n,RED) =convolve(tgt0,m,n,RED) ;
		  		if(tgt(m,n,RED) > roi.roiList[i].thresh )
		      			tgt(m,n,RED) = tgt(m,n,GREEN)=tgt(m,n,BLUE)=255;
				else
		      			tgt(m,n,RED) = tgt(m,n,GREEN)=tgt(m,n,BLUE)=0;

				
				

	        	}

		}		
	}
} 



void ipTool::SobelH(Image& src, Image& tgt0, Image& tgt, ROI roi)
{
	//int chanH;
	int count=1;
	Image tmp;
	tmp.initialize(src.NR,src.NC);
	for(int i=0;i<tgt.NR;i++){
		for(int j=0;j<tgt.NC;j++){
				tgt0(i,j,RED)=src(i,j,RED);	
				tgt0(i,j,GREEN)=src(i,j,GREEN);	
				tgt0(i,j,BLUE)=src(i,j,BLUE);

				tmp(i,j,RED)=int(src(i,j,GREEN)+src(i,j,BLUE)+src(i,j,RED))/3;

				tgt(i,j,RED)=src(i,j,RED);
				tgt(i,j,GREEN)=src(i,j,GREEN);
				tgt(i,j,BLUE)=src(i,j,BLUE);	
		}
	}


	for (int i=0; i<roi.numBaseROI; i++)

	{

		for (int m=roi.roiList[i].x; m<roi.roiList[i].x+roi.roiList[i].sx; m++)
		{
			for (int n=roi.roiList[i].y; n<roi.roiList[i].y+roi.roiList[i].sy; n++)
			{
			  
				int r = src(m,n,RED);
				int g = src(m,n,GREEN);
				int b = src(m,n,BLUE);
				tgt0(m,n,RED)=tgt0(m,n,GREEN)=tgt0(m,n,BLUE)= pixeltohsi(r, g, b, tgt0, m, n, count); 
				tmp(m,n,RED)=pixeltohsi(r,g,b,tgt,m,n,count); 
				tgt(m,n,RED)=tgt(m,n,GREEN)=tgt(m,n,BLUE)=pixeltohsi(r,g,b,tgt,m,n,count); 
				printf("values : %d, %d, %d :\n", tgt(m,n,RED),tgt(m,n,BLUE),tgt(m,n,GREEN));
				tgt(m,n,RED) =convolve(tmp,m,n,RED) ;
				printf("values : %d \n", tgt(m,n,RED));
		  		if(tgt(m,n,RED) > roi.roiList[i].thresh)
		      			tgt(m,n,GREEN)=tgt(m,n,BLUE)=tgt(m,n,RED)=255 ;
		  		else
		      			tgt(m,n,RED) = tgt(m,n,GREEN)=tgt(m,n,BLUE)=0;
				printf(" 2 values : %d, %d, %d :\n", tgt(m,n,RED),tgt(m,n,BLUE),tgt(m,n,GREEN));

	        	}

		}		
	}
} 



void ipTool::SobelS(Image& src, Image& tgt0, Image& tgt, ROI roi)
{

	int count=2;
	//int chanS;
	Image tmp;
	tmp.initialize(src.NR,src.NC);
	for(int i=0;i<tgt.NR;i++){
		for(int j=0;j<tgt.NC;j++){
				tgt0(i,j,RED)=src(i,j,RED);	
				tgt0(i,j,GREEN)=src(i,j,GREEN);	
				tgt0(i,j,BLUE)=src(i,j,BLUE);

				tgt(i,j,RED)=src(i,j,RED);
				tgt(i,j,GREEN)=src(i,j,GREEN);
				tgt(i,j,BLUE)=src(i,j,BLUE);	
		}
	}


	for (int i=0; i<roi.numBaseROI; i++)

	{

		for (int m=roi.roiList[i].x; m<roi.roiList[i].x+roi.roiList[i].sx; m++)
		{
			for (int n=roi.roiList[i].y; n<roi.roiList[i].y+roi.roiList[i].sy; n++)
			{
			  
				int r = src(m,n,RED);
				int g = src(m,n,GREEN);
				int b = src(m,n,BLUE);
				tgt0(m,n,RED)=tgt0(m,n,GREEN)=tgt0(m,n,BLUE)= pixeltohsi(r, g, b, tgt0, m, n, count); //, chanS);
				tmp(m,n,RED)=pixeltohsi(r,g,b,tgt,m,n,count); //chanS);
				tgt(m,n,RED) =convolve(tmp,m,n,RED) ;
		  		if(tgt(m,n,RED) > roi.roiList[i].thresh)
		      			tgt(m,n,RED) = tgt(m,n,GREEN)=tgt(m,n,BLUE)=255;
		  		else
		      			tgt(m,n,RED) = tgt(m,n,GREEN)=tgt(m,n,BLUE)=0;

	        	}

		}		
	}
} 



int pixeltohsi(int r, int g, int b, Image& temp, int m, int n, int count)
{
    
    float fr = r;
    float fg = g;
    float fb = b;

    int max, min; //, chanH, chanS, chanI;
    if(r > g && r > b)
    {
       max = r;
	if(g > b)	
       	min = b;
	else
		min = g;
    }
    else
    {
        if(g > b)
        {
            max = g;
            
		if(r > b)	
       		min = b;
		else
			min = r;

        }
        else
        {
            max = b;
		if(r > g)	
       		min = g;
		else
			min = r;

            
        }
    
    }

    fr = r / 255.0;
    fg = g / 255.0;   
    fb = b / 255.0;
    
    float intensity = (fr + fg + fb)/3.0;
    float saturation = 1 - 1.0/intensity * min/255.0;
    float hue = 0;

    float huetmp = 0.5 * (fr - fg + fr - fb)/(sqrt((double)((fr - fg) * (fr - fg) + (fr-fb) * (fg - fb))));
    hue = acos(huetmp) * 57.296; //Convert to degrees
    

    if(b > g)
        hue = 360 - hue;
    if (count==1) {
	//chanH=chan;
    	//temp(m,n, chan) = 
	return int(hue / 360.0 * 255);
	}
    if (count==2){
	//chan= chanS;
	//temp(m,n, chanS) = 
	return int(saturation * 255);
	}
    if (count==3){
	//chan= chanI;
    	//temp(m,n, chanI)= 
	return int(intensity * 255);
	}
}


