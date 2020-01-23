All the operations were implemented using C++ Language. All the major functions were written with the class ipTools, in a manner that follows from the previous assignments.

The two binary files outputROI and videoprocess are used for images and video input respectively. 

For that the scripts compile1.sh and compile.sh need to be run respectively.

The user inputs the operation to be performed, the source filename, the target filename for the gradient edge detection and the target file for the binarized form and the roi file.

outputROI <operator> <source file> <targetfile1> <targetfile2> roi

videoprocess <operator> <source file> <targetfile1_name> <targetfile2_name> roi

runme.sh script file runes the program for images by invoking outputROI and runmevid.sh runs for video input. but neccessay changes need to be made in the exeme.sh script file for running the program each time.

It is simpler to run this by entering throught the command line.

The roi files contains all the user defined parameters. The format of the roi file is as follows:


3
4 5 70 70 64 20 60 120 120 120
100 300 100 100 100 100 150 130 130 130
220 110 200 200 120 200 250 100 100 100

The first element represents the number of ROIs to be considered.

The part highlighted in yellow is the x, y, Sx, Sy respectively for the given image or for a single 
frame of a video input.
The green highlighted part is the threshold input.

The blue highlighted parts are the start and end frames respectively for a video input.

The grey part represents the threshold values for Red Green and Blue respectively as Cred, Cgreen and Cblue.

 For each decoded image for video, it is checked whether the frame is part of the specified SOI. If it falls within the SOI, the operation will be performed only on those frames which fall within the SOI .

The different operators called, handle ROI and perform respective operations within the ROI of the image and save the output.

The different operators called are invoked from the main function as they are called by the user. 
The functionality of each can be found within the ipTools class.

The different operators defined are:

Sobelbin: this function is called to perform edge detection on grey-level image and videos. It saves both the amplitude/gradient as well as the binarized edge images.

SobelColbin: this function is called to perform Edge-detection on Color images with the output being in both grey-level as well as black and white.

SobelR, SobelG, SobelB : these three functions are called to perform edge detection for each of the Red, Green and Blue Channels.

SobelH, SobelS, SobelI : these functions perfom sobel operations on individual Hue, Saturation and intensity channels.

pixeltohsi function present in the ipTools class converts RGB to HSI.

convolve function in the ipTools class perform the Convolution and returns the gradient.  

For video the output frames are encoded to video using the encodeframes_grad file.
