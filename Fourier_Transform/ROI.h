// ROI.h: interface for the ROI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROI_H__8438A090_F2DD_446B_99A9_8C03BAF40432__INCLUDED_)
#define AFX_ROI_H__8438A090_F2DD_446B_99A9_8C03BAF40432__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class BaseROI
{
    public:
	int x;
	int y;
	int sx;
	int sy;

        int thresh;
        double radius;
	int winSize;
        int cRed;
        int cGreen;
        int cBlue;

	void print();
};

class ROI  
{
    public:
        enum Filter {LPF, HPF, BPF, GrayArithmetic, ROIThresh, AdaptiveThresh, ColorThresh, ColorArithmetic, OutputROI, Sobel, SobelR, SobelG, SobelB, SobelRGB,SobelH, SobelS, SobelI, SobelHSI}; 
	int numBaseROI;
	BaseROI* roiList;
	void CreateROIList(char* filename, Filter filter);
	void ReleaseROIList();
	int InROI(int px,int py);
	ROI();
	virtual ~ROI();
};

#endif // !defined(AFX_ROI_H__8438A090_F2DD_446B_99A9_8C03BAF40432__INCLUDED_)
