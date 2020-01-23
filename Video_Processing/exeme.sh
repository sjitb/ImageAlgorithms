
# decode all *.m1v files in /home/csee1/satrajit/dip/asgn2/videos/
for src  in /home/csee1/satrajit/dip/asgn2/videos/*.m1v;
do
# set target file name;
tgt=`basename $src` ;
echo converting $src to $tgt;
# run "decode" function with threshold=100;
$1 decode $src $tgt;
done