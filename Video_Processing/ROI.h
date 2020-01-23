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
	int fs;
	int fe;
	//bool process;	//check back
	int Cred;
	int Cgreen;
	int Cblue;
};
class ROI  
{
public:
	int numBaseROI;
	BaseROI* roiList;
	void CreateROIList(char* filename);
	//void CreateSOIROIList(char* filename, int maxrows, int maxcols, int frame);
	void ReleaseROIList();
	bool InROI(int px,int py);
	bool InSOI(int framenum);
	ROI();
	virtual ~ROI();
};

#endif // !defined(AFX_ROI_H__8438A090_F2DD_446B_99A9_8C03BAF40432__INCLUDED_)
