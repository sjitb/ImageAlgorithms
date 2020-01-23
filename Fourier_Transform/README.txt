Assignment 4: Fourier Transform and Filtering 
CAP5400 -  Digital Image Processing

Usage:
   ./ipTools <filterName> <srcFile> <roiFile> <tgtFile> [<tgtFile2>]

   (<tgtFile2> is used for FTT operation since frequency domain image and reult of IFFT is returned..)

   <filter Name> can be (note, Video can be added ot the end of any filter name for video processing):
         FTT:
	   Transforms inamge into frequency domain; stores freqeuency image into <tgtFile2>, and result of inverse into <tgtFile1>.
         LPF:
	   Applies mask to frequency domain so that only low frequency intensities survive.
         HPF:
	   Applies mask to frequency domain so that only high frequency intensities survive.
         BPF:
	   Applies mask to frequency domain so that only a band of frequency intensities survive.

ROI File Format:
   1
   <x> <y> <sx> <sy> {filter options}

   Where {filter otions} can be one of:
      LPF, HPF:
        <thresh>  
      BPF:
        <LP thresh> <HP thresh> 


----------------------------------------
Old Usage (still valid):

Usage:
   ./ipTools <filterName> <srcFile> <roiFile> <soiFile> <tgtFile> [<tgtFile2>]

   (<tgtFile2> is used for Sobel operations, since both sobel <tgtFile> and thresholded sobel <tgtFile2> images are output.)

   <filter Name> can be:
         AdaptiveThresh:
	   Threshold by the average intensity level within a window w plus a constant T0.
         GrayArithmetic:
	   Add a specified value to all intensity levels.
         ROIThresh:
	   Threshold according to supplied T.
         ColorThresh:
	   Given an R,G,B, and D, for all pixels with intensity within D of (R,G,B) in <R,G,B> space, threshold.
         ColorArithmetic:
	   Add specified values to the R,G, and B, channels.

	(All Sobel functions can be called with "Video" at the end for mpeg processing, i.e. "SobelVideo")
         Sobel:
	   Use Sobel in dx and dy f`to find edges, output image and thresholded image.
         SobelR:
	   Apply sobel as above to R channel. Output image and thresholded image.
         SobelG:
	   Apply sobel as above to G channel. Output image and thresholded image.
         SobelB:
	   Apply sobel as above to B channel. Output image and thresholded image.
         SobelRGB:
	   Apply sobel to R, G and B channels, and combine into one image. Output image and thresholded image.
         SobelH:
	   Apply sobel as above to H channel. Output image and thresholded image.
         SobelS:
	   Apply sobel as above to S channel. Output image and thresholded image.
         SobelI:
	   Apply sobel as above to I channel. Output image and thresholded image.
         SobelHSI:
	   Apply sobel to H, S and I channels, and combine into one image. Output image and thresholded image.


ROI File Format:
   <# of ROI>
   <x> <y> <sx> <sy> {filter options}
   ...
   <x> <y> <sx> <sy> {filter options}

   Where {filter otions} can be one of:
      AdaptiveThresh:
        <thresh> <winSize> 
      GrayArithmetic:
        <toAdd> 
      ROIThresh:
        <thresh> 
      ColorThresh:
        <thresh> <cRed> <cGreen> <cBlue>
      ColorArithmetic: 
        <cRed> <cGreen> <cBlue>
      Sobel:
        <thresh>
      SobelR:
        <thresh>
      SobelG:
        <thresh>
      SobelB:
        <thresh>
      SobelRGB:
        <redThresh> <greenThresh> <blueThresh>
      SobelH:
        <thresh>
      SobelS:
        <thresh>
      SobelI:
        <thresh>
      SobelHSI:
        <hThresh> <sThresh> <iThresh>


SOI File Format:
   <# of SOI>
   <Start> <End>
   ...
   <start> <End>


