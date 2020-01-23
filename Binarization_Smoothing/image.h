#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#ifndef _IMAGE_HPP_
#define _IMAGE_HPP_ 

#define RED 0
#define GREEN 1
#define BLUE 2

class Image 
{
private:
  int  *IM_r, *IM_g, *IM_b;
  int getint(FILE *fp);
  int initialize (int **IM, int nr, int nc);
public:
  int NR, NC;
  Image();
  ~Image();
  int initialize (int nr, int nc);

  int save (char file[100]);
  int read (char file[100]);

  int inboundp (const int i, const int j);

  int& operator()(const int i, const int j);
  int& operator()(const int i, const int j, const int rgb);

  int operator()(const int i, const int j) const;
  int operator()(const int i, const int j, const int rgb)const ;


};

#endif

