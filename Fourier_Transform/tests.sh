#!/bin/csh

#foreach t  (1 2 3 4 5 6 7 8 9)
#	echo "1" > roiFile
#	echo "0 0 900 900 $t" >> roiFile
#	foreach f (`ls testCases/imgs/*.ppm`)
#		./ipTools LPF $f roiFile out/lpf_`basename $f .ppm`.i.${t}
#		convert out/lpf_`basename $f .ppm`.i.${t}.pgm out/lpf_`basename $f .ppm`.i.${t}.jpg
#		rm out/lpf_`basename $f .ppm`.i.${t}.ppm -f
#	end
#	foreach f (`ls testCases/imgs/*.pgm`)
#		./ipTools LPF $f roiFile out/lpf_`basename $f .pgm`.${t}
#		convert out/lpf_`basename $f .pgm`.${t}.pgm out/lpf_`basename $f .pgm`.${t}.jpg
#		rm out/lpf_`basename $f .ppm`.${t}.pgm -f
#	end
#	foreach f (`ls testCases/imgs/*.ppm`)
#		./ipTools HPF $f roiFile out/hpf_`basename $f .ppm`.i.${t}
#		convert out/hpf_`basename $f .ppm`.i.${t}.pgm out/hpf_`basename $f .ppm`.i.${t}.jpg
#		rm out/hpf_`basename $f .ppm`.i.${t}.ppm -f
#	end
#	foreach f (`ls testCases/imgs/*.pgm`)
#		./ipTools HPF $f roiFile out/hpf_`basename $f .pgm`.${t}
#		convert out/hpf_`basename $f .pgm`.${t}.pgm out/hpf_`basename $f .pgm`.${t}.jpg
#		rm out/hpf_`basename $f .ppm`.${t}.pgm -f
#	end
#end
#foreach t  (1 5 10 15 30 50)
#    foreach dt  (10 30 50 200)
#	echo "1" > roiFile
#	echo "0 0 900 900 `expr $t + $dt` $t" >> roiFile
#	foreach f (`ls testCases/imgs/*.ppm`)
#		./ipTools BPF $f roiFile out/bpf_`basename $f .ppm`.i.${t}.${dt}
#		convert out/bpf_`basename $f .ppm`.i.${t}.${dt}.pgm out/bpf_`basename $f .ppm`.i.${t}.${dt}.jpg
#		rm out/bpf_`basename $f .ppm`.i.${t}.ppm -f
#	end
#	foreach f (`ls testCases/imgs/*.pgm`)
#		./ipTools BPF $f roiFile out/bpf_`basename $f .pgm`.${t}.${dt}
#		convert out/bpf_`basename $f .pgm`.i.${t}.${dt}.pgm out/bpf_`basename $f .pgm`.${t}.${dt}.jpg
#		rm out/bpf_`basename $f .ppm`.${t}.pgm -f
#	end
#   end
#end
foreach c (`ls ../../ass2/clips/air.mpeg`)
  foreach t  (1 5 10 30)
    echo "1" > roiFile
    echo "0 0 900 900 $t" >> roiFile
    ./ipTools HPFVideo $c roiFile out/`basename $c`.$t.
  end
end

