#########################################################################
# File Name: convert.sh
# Author: sandy
# mail: sandy@luo.bo
# Created Time: 2014年03月25日 星期二 12时24分01秒
#########################################################################
#!/bin/bash
database=$1

cd $database
for files in *
do
	ncfile=$(basename $files)
	suffix=".*"
	pfile=${ncfile%$suffix}".bmp"
	convert $ncfile -type truecolor $pfile
done
mv *.bmp ~/Iris/bmp/
