#!/usr/bin/env bash

#dot g0.dot > g0_formatted.dot
#cat g0_formatted.dot | gvpack -g -m1000000 -o g0-pack.dot
#dot -Tpng g0-pack.dot -o g0.png

graphs=$(find . -name "g*.dot" | sort)

rm -f *.png


for f in $graphs; do
  dot $f -Tpng -o ${f}.png
done


images=$(find . -name "g*.dot.png" | sort)

echo $images

montage *.dot.png -geometry 100x100% -frame 5 -shadow -tile x1 out.png

eog out.png