#include "image.h"

Image::Image(){
  NR = NC = 0; 
  IM_r = IM_g = IM_b = NULL;
};
/*-----------------------------------------------------------------------**/
Image::~Image() {
    if (IM_r != NULL) free(IM_r);
    if (IM_g != NULL) free(IM_g);
    if (IM_b != NULL) free(IM_b);
};
/*-----------------------------------------------------------------------**/
int Image::initialize (int **IM, int nr, int nc)
{
  if (((*IM) == NULL) || (((*IM) != NULL) && (nr*nc > NC*NR))) {
    if ((*IM) != NULL) delete (*IM);
    if (((*IM) = new int [nr*nc]) == NULL){
      fprintf(stderr, "\n Could not allocate space(%d %d) at line %d in file %s\n",
	      nr, nc, __LINE__, __FILE__);
      return(1);
    }
  }
  NR = nr; NC = nc;
  for (int i = 0; i < nr*nc; i++) (*IM)[i] = 0;
  return(1);
}
/*-----------------------------------------------------------------------**/
int Image::initialize (int nr, int nc)
{
  initialize (&(IM_r), nr, nc);
  initialize (&(IM_g), nr, nc);
  initialize (&(IM_b), nr, nc);
  return 0;
}
/*-------------------------------------------------------------------*/
int& Image::operator()(const int i, const int j) {return *(IM_r + i*NC+j);};
/*-------------------------------------------------------------------*/
int& Image::operator()(const int i, const int j, const int rgb) {
  switch (rgb) {
  case RED: return *(IM_r + i*NC+j); break;
  case GREEN: return *(IM_g + i*NC + j); break;
  case BLUE: return *(IM_b + i*NC + j); break;
  }
  return *(IM_r + i*NC+j);
};
/*-------------------------------------------------------------------*/
int Image::operator()(const int i, const int j) const {return *(IM_r + i*NC+j);};
/*-------------------------------------------------------------------*/
int Image::operator()(const int i, const int j, const int rgb) const {
  switch (rgb) {
  case RED:   return *(IM_r + i*NC+j); break;
  case GREEN: return *(IM_g + i*NC + j); break;
  case BLUE:  return *(IM_b + i*NC + j); break;
  }
  return *(IM_r + i*NC+j);
};

/*-------------------------------------------------------------------*/
int Image::save (char file[100])
{
  FILE *outfile1;
  int i, j, Flag;

  fprintf(stderr, "\n Saving edge image in %s...\n", file);
  if ((outfile1 = fopen(file, "w")) == NULL)
    {
      fprintf(stderr, "Cannot open file, %s for writing.\n", file) ;
      return(0) ;
    }
  if (strstr(file, ".ppm") != NULL) {/* PPM Color File*/
    Flag = 0;
  }
  else Flag = 1;

  if (Flag)
    fprintf(outfile1, "P5\n%d %d\n255\n", NC, NR);
  else 
    fprintf(outfile1, "P6\n%d %d\n255\n", NC, NR);

  if (Flag) {
    for (i=0; i < NR; i++) {
      for (j=0; j < NC; j++) {
	putc((unsigned char) (((*(IM_r+i*NC+j))>255)?255:(*(IM_r+i*NC+j))), outfile1) ;
      }
    }
  }
  else {
     for (i=0; i < NR; i++) {
      for (j=0; j < NC; j++) {
	putc((unsigned char)(((*(IM_r+i*NC+j))>255)?255:(*(IM_r+i*NC+j))), outfile1) ;
	putc((unsigned char)(((*(IM_g+i*NC+j))>255)?255:(*(IM_g+i*NC+j))), outfile1) ;
	putc((unsigned char)(((*(IM_b+i*NC+j))>255)?255:(*(IM_b+i*NC+j))), outfile1) ;
      }
    }
  }
  fclose(outfile1) ;
  
  return(1) ;
}

/*-----------------------------------------------------------------------**/
int Image::inboundp (int i, int j) 
{
  if ((i < 0) || (j < 0) || (j >= NC) || (i >= NR)) return(0);
  else return(1);
}
/*-----------------------------------------------------------------------**/
int Image::getint(FILE *fp)
{
  int item, i, flag;

  /* skip forward to start of next number */
  item  = getc(fp); flag = 1;
  do {

    if (item =='#') {   /* comment*/
      while (item != '\n' && item != EOF) item=getc(fp);
    }

    if (item ==EOF) return 0;
    if (item >='0' && item <='9') 
      {flag = 0;  break;}

    /* illegal values */
    if ( item !='\t' && item !='\r' 
	&& item !='\n' && item !=',') return(-1);

    item = getc(fp);
  } while (flag == 1);


  /* get the number*/
  i = 0; flag = 1;
  do {
    i = (i*10) + (item - '0');
    item = getc(fp);
    if (item <'0' || item >'9') {
      flag = 0; break;}
    if (item==EOF) break; 
  } while (flag == 1);
  
  return i;
} 
/*-------------------------------------------------------------------*/
int Image::read (char file[100])
{
  FILE *fp;
  char str[50];
  unsigned char r, g, b;
  int i, Flag;

  if (strstr(file, ".ppm") != NULL) {/* PPM Color File*/
    Flag = 0;
  }
  else Flag = 1;

  if ((fp = fopen(file, "r")) == NULL) {
    fprintf(stderr, "\nCannot open image file %s\n", file);
    exit(1);
  }
  
  fscanf(fp, "%s", str) ;
  if ((Flag == 0) && (strcmp (str, "P6") != 0)) {
    fprintf(stderr, "\n Image file %s not in PPM format", file);
    return (0);
  }
  if ((Flag == 1) && (strcmp (str, "P5") != 0)) {
    fprintf(stderr, "\n Image file %s not in PGM format", file);
    return (0);
  }
  NC = getint(fp);  NR = getint(fp);
  fscanf(fp, "%d", &i) ;  (getc(fp));/* gets the carriage return*/

  if (Flag) initialize (&(IM_r), NR, NC); /* just the Red array for gray level values */
  else initialize (NR, NC);

  if (Flag) {
    for (i=0; i < NR*NC; i++) {
      r = (unsigned char) (getc(fp));  
      *(IM_r + i) = r;
    }
  }
  else {
    for (i=0; i < NR*NC; i++) {
      r = (unsigned char) (getc(fp));  
      g = (unsigned char) (getc(fp));  
      b = (unsigned char) (getc(fp));
      *(IM_r + i) = r;
      *(IM_g + i) = g;
      *(IM_b + i) = b;
    }
  } 
  fclose(fp);
  return(1);
}

void Image::flushMemory() {
    if (IM_r != NULL) free(IM_r);
    if (IM_g != NULL) free(IM_g); 
    if (IM_b != NULL) free(IM_g); 
  	NR = NC = 0; 
	  IM_r = IM_g = IM_b = NULL;
};
  

