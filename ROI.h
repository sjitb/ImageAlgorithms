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

};
class ROI  
{
public:
	int numBaseROI;
	BaseROI* roiList;
	void CreateROIList(char* filename);
	void ReleaseROIList();
	bool InROI(int px,int py);
	ROI();
	virtual ~ROI();
};

#endif // !defined(AFX_ROI_H__8438A090_F2DD_446B_99A9_8C03BAF40432__INCLUDED_)
