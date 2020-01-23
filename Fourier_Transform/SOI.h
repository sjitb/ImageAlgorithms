// SOI.h: interface for the SOI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOI_H__8438A090_F2DD_446B_99A9_8C03BAF40432__INCLUDED_)
#define AFX_SOI_H__8438A090_F2DD_446B_99A9_8C03BAF40432__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class BaseSOI
{
public:
        int start;
        int end;
};

class SOI  
{
public:
	int numBaseSOI;
	BaseSOI* soiList;
	void CreateSOIList(char* filename);
	void ReleaseSOIList();
	int InSOI(int frame);
	SOI();
	virtual ~SOI();
};

#endif // !defined(AFX_SOI_H__8438A090_F2DD_446B_99A9_8C03BAF40432__INCLUDED_)
