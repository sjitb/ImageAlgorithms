// ipTool.h: interface for the ipTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPTOOL_H__65F5F541_0319_42DF_9ABC_143FAB67972C__INCLUDED_)
#define AFX_IPTOOL_H__65F5F541_0319_42DF_9ABC_143FAB67972C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define MAXLEN 256

#include "image.h"
#include "ROI.h"
#include "SOI.h"
#include <math.h>
#include <rfftw.h>
class ipTool  
{
public:
	static void pgm2ppm(Image &src, Image &tgt);
	static void ppm2pgm(Image &src, Image &tgt);

//Assignment 2
	static void OutputROI(Image& src, ROI &roi, Image& tgt);
	static void ROIThresh(Image& src, ROI &roi, Image& tgt);
	static void GrayArithmetic(Image &src,ROI &roi,Image &tgt);
	static void AdaptiveThresh(Image &src,ROI &roi,Image &tgt);
	static void ColorThresh(Image &src,ROI &roi,Image &tgt);
	static void ColorArithmetic(Image &src,ROI &roi,Image &tgt);

//Assignment 3
	static void rgb2hsi(int, int, int, double&, double&, double&);
	static void hsi2rgb(double, double, double, int&,int&,int&);
	static void Sobel(Image& src, ROI &roi, Image& tgt, Image& tgt2);
	static void SobelR(Image& src, ROI &roi, Image& tgt, Image& tgt2);
	static void SobelG(Image& src, ROI &roi, Image& tgt, Image& tgt2);
	static void SobelB(Image& src, ROI &roi, Image& tgt, Image& tgt2);
	static void SobelRGB(Image& src, ROI &roi, Image& tgt, Image& tgt2);
	static void SobelH(Image& src, ROI &roi, Image& tgt, Image& tgt2);
	static void SobelS(Image& src, ROI &roi, Image& tgt, Image& tgt2);
	static void SobelI(Image& src, ROI &roi, Image& tgt, Image& tgt2);
	static void SobelHSI(Image& src, ROI &roi, Image& tgt, Image& tgt2);

//Assignment 4
	static void PassFilter(Image& src, ROI& roi, Image& tgt, Image& tgt2, char * filt);
	static void ppm2i(Image &src, Image &tgt);

	ipTool();
	virtual ~ipTool();
};

#endif // !defined(AFX_IPTOOL_H__65F5F541_0319_42DF_9ABC_143FAB67972C__INCLUDED_)
