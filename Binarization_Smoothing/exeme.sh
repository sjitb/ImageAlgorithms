
# binarize all *.pgm files in /home/csee1/satrajit/dip/images/
for src  in /home/csee1/satrajit/dip/images/*.pgm;
do
# set target file name;
tgt=`basename $src` ;
echo converting $src to $tgt;
# run "binarize" function with threshold=100;
$1 binarize $src $tgt 100;
# run "GreyThresh" function with thresh=140, winsize=8;
$2 GreyThresh $src roi $tgt 140 8
# run "Smoothing" function with threshold=10, winsize=3;
$3 Smoothing $src roi $tgt 10 3

done