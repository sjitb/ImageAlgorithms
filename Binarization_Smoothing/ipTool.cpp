// ipTool.cpp: implementation of the ipTool class.
//
//////////////////////////////////////////////////////////////////////

#include "ipTool.h"
#include "math.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ipTool::ipTool()
{

}

ipTool::~ipTool()
{

}

void ipTool::binarize(Image &src, Image &tgt, int thresh, ROI roi)
{

		for(int i=0;i<tgt.NR;i++){
			for(int j=0;j<tgt.NC;j++){		
				if (roi.InROI(i,j)){
					if (src(i,j,RED)>thresh)
						tgt(i,j,RED)=255;
					else
						tgt(i,j,RED)=0;
					}
				else
					tgt(i,j,RED)=src(i,j,RED);
		}
	}	

}

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


void ipTool::GreyThresh(Image& src, ROI roi,Image& tgt, int thresh, int winsize)
{
	
	int sum = 0;//, count = 0;
	for(int i=0;i<tgt.NR;i++){
		for(int j=0;j<tgt.NC;j++){	
			if (roi.InROI(i,j)){
				for( int row=i + winsize/2;row <=winsize ;row++)
					{
		 			for(int col=j + winsize/2;col<=winsize;col++)
		    				{
		      						      				
			      				sum += (src(row,col,RED));
			      						      				
					}
		      				
		    		}
				int winmean = sum/(winsize*winsize);
	        		int varThresh = winmean + thresh;
				if (src(i,j,RED)>varThresh)
					tgt(i,j,RED)=255;
				else
					tgt(i,j,RED)=0;
	        	}
			else
				tgt(i,j,RED)=src(i,j,RED);

		}		
	}
}

  
	       /* int winmean = sum/count;
	        int varThresh = winmean + thresh;	
		 for(int i=0;i<tgt.NR;i++){
			for(int j=0;j<tgt.NC;j++){	
				if (roi.InROI(i,j)){
						if (src(i,j,RED)>varThresh)
							tgt(i,j,RED)=255;
						else
							tgt(i,j,RED)=0;
					}
				else
					tgt(i,j,RED)=src(i,j,RED);
}}
} */

			 
void ipTool::Colorbinarize(Image& src, Image& tgt, int threshold, ROI roi,int Cred,int Cgreen,int Cblue)
{
	
	for(int i=0;i<tgt.NR;i++){
			for(int j=0;j<tgt.NC;j++){		
				if (roi.InROI(i,j)){
					long distsqr = (Cred-src(i,j,RED)) * (Cred-src(i,j,RED))   + 
		      					 (Cgreen-src(i,j,GREEN)) * (Cgreen-src(i,j,GREEN)) +
		      					 (Cblue-src(i,j,BLUE)) * (Cblue-src(i,j,BLUE)) ;
					int dist = ceil(pow(distsqr, 0.5));
					
					if(dist < threshold)
						tgt(i,j,RED)=tgt(i,j,GREEN)=tgt(i,j,BLUE)=255;
					else
						tgt(i,j,RED)=tgt(i,j,GREEN)=tgt(i,j,BLUE)=0;
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
	
void ipTool::Smoothing(Image& src,ROI roi,Image& tgt,int smoothresh,int size)
{
	int sum=0;
	int average=0;
	
	
	
	for(int i=0;i<tgt.NR;i++){
		for(int j=0;j<tgt.NC;j++){	
			if (roi.InROI(i,j)){
				for (int m=i + size/2;m <=size ;m++){
					for (int n=j + size/2;n<=size;n++)
						{
							sum+=(src(i,j,RED));
						}
				}
				average= sum/size*size;
				int ab_diff= (src(i,j,RED)-average)*(src(i,j,RED)-average);
				if (ab_diff < (smoothresh*smoothresh))
					tgt(i,j,RED)=average;
				else
					tgt(i,j,RED)=src(i,j,RED);
			}
			else{
					tgt(i,j,RED)=src(i,j,RED);
			}
		}
	}
	//printf(" average: %d",average);
}

				


