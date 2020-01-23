// ipTool.cpp: implementation of the ipTool class.
//
//////////////////////////////////////////////////////////////////////

#include "ipTool.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ipTool::ipTool()
{

}

ipTool::~ipTool()
{

}

void ipTool::binarize(Image &src, Image &tgt, ROI roi)
{
	
	for (int i=0; i<roi.numBaseROI; i++)
	{
		
			for (int m=roi.roiList[i].x; m<roi.roiList[i].x+roi.roiList[i].sx; m++)
			{
				for (int n=roi.roiList[i].y; n<roi.roiList[i].y+roi.roiList[i].sy; n++)
				{
					src(m,n,RED)=int((src(m,n,RED)+src(m,n,GREEN)+src(m,n,BLUE))/3); // Save grayscale ROI in RED layer. 
					if (src(m,n,RED)>roi.roiList[i].thresh)
					{
						src(m,n,RED)=255;
						src(m,n,GREEN)=255;
						src(m,n,BLUE)=255;
					}
					else
					{
						src(m,n,RED)=0;
						src(m,n,GREEN)=0;
						src(m,n,BLUE)=0;
					}
				}
			}
		//}
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



void ipTool::GreyThresh(Image& src, Image& tgt, ROI roi, int winsize)
{
	
	int sum = 0;//, count = 0;
	for (int i=0; i<roi.numBaseROI; i++)

	{

	//for (int f=roi.roiList[i].fs ; f<=roi.roiList[i].fe ; f++)

	//{
		for (int m=roi.roiList[i].x; m<roi.roiList[i].x+roi.roiList[i].sx; m++)
		{
			for (int n=roi.roiList[i].y; n<roi.roiList[i].y+roi.roiList[i].sy; n++)
			{
				src(m,n,RED)=int((src(m,n,RED)+src(m,n,GREEN)+src(m,n,BLUE))/3);
				for( int row=m + winsize/2;row <=winsize ;row++)
				{
		 			for(int col=n + winsize/2;col<=winsize;col++)
		    			{
		      						      				
			      				sum += (src(row,col,RED));
			      						      				
					}
		      				
		    		}
				int winmean = sum/(winsize*winsize);
	        		int varThresh = winmean + roi.roiList[i].thresh;
				if (src(m,n,RED)>varThresh)
				{
					src(m,n,RED)=255;
					src(m,n,GREEN)=255;
					src(m,n,BLUE)=255;
				}
				else
				{
					src(m,n,RED)=0;
					src(m,n,GREEN)=0;
					src(m,n,BLUE)=0;
				}
	        	}

		}		
	}
	//}
}


			 
void ipTool::Colorbinarize(Image& src, Image& tgt, ROI roi)
{

	for (int i=0; i<roi.numBaseROI; i++)

	{
	
		for (int m=roi.roiList[i].x; m<roi.roiList[i].x+roi.roiList[i].sx; m++)
		{
			for (int n=roi.roiList[i].y; n<roi.roiList[i].y+roi.roiList[i].sy; n++)
			{
					long distsqr = (roi.roiList[i].Cred-src(m,n,RED)) * (roi.roiList[i].Cred-src(m,n,RED))   + 
		      					 (roi.roiList[i].Cgreen-src(m,n,GREEN)) * (roi.roiList[i].Cgreen-src(m,n,GREEN)) +
		      					 (roi.roiList[i].Cblue-src(m,n,BLUE)) * (roi.roiList[i].Cblue-src(m,n,BLUE)) ;
					int dist = ceil(pow(distsqr, 0.5));
					
					if(dist < roi.roiList[i].thresh)
					{
						src(m,n,RED)=255;
						src(m,n,GREEN)=255;
						src(m,n,BLUE)=255;
					}
					else
					{
						src(m,n,RED)=0;
						src(m,n,GREEN)=0;
						src(m,n,BLUE)=0;
					}
			}
		}
	//}
	}
}	
	
void ipTool::Smoothing(Image& src,Image& tgt,ROI roi,int size)
{
	int sum=0;
	int average=0;
	//int smoothresh=roi.roiList[i].thresh;
	
	
	
	for (int i=0; i<roi.numBaseROI; i++)
	{

		for (int m=roi.roiList[i].x; m<roi.roiList[i].x+roi.roiList[i].sx; m++)
		{
			for (int n=roi.roiList[i].y; n<roi.roiList[i].y+roi.roiList[i].sy; n++)
			{
				src(m,n,RED)=int((src(m,n,RED)+src(m,n,GREEN)+src(m,n,BLUE))/3);
				for (int x=m + size/2;m <=size ;x++){
					for (int y=n + size/2;y<=size;y++)
						{
							sum+=(src(m,n,RED));
						}
				}
				int smoothresh=roi.roiList[i].thresh;
				average= sum/size*size;
				int ab_diff= (src(m,n,RED)-average)*(src(m,n,RED)-average);
				if (ab_diff < (smoothresh*smoothresh))
					src(m,n,RED)=average;
				else
					tgt(m,n,RED)=src(m,n,RED);
			}
			//else{
			//		tgt(i,j,RED)=src(i,j,RED);
			//}
		}
	//}
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

