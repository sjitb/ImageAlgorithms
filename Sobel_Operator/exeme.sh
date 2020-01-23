
# decode all *.pgm files in /home/csee1/satrajit/dip/sobel
for src  in /home/csee1/satrajit/dip/sobel/*.pgm;
do
# set gradient target file name;
tgt0=`basename $src` ;
# set binarized target file name;
tgt='basename $src' ;
echo converting $src to $tgt;
# run "Sobelbin" function ;
$1 Sobelbin $src $tgt0 $tgt roi;
done