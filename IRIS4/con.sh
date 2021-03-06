#########################################################################
# File Name: con.sh
# Author: sandy
# mail: sandy@luo.bo
# Created Time: 2014年03月24日 星期一 16时53分09秒
#########################################################################
#!/bin/bash
trash-put -rf ~/Iris/out/
mkdir -p ~/Iris/out/{tiff,SegmentedImages,NormalizedMasks,NormalizedImages,Masks,IrisCodes,CircleParameters}
for files in ~/Iris/bmp/*.bmp
do
	ncfile=$(basename $files)
	suffix=".*"
	pfile=${ncfile%$suffix}".tiff"
	echo $pfile
	convert ${files} ~/Iris/out/tiff/${pfile} 
done

File="/home/sandy/Iris/IRIS4/scripts/Template/ImageList.txt"
cd ~/Iris/out/tiff
find *.tiff -type f > $File
cd -
cd src
./osiris ../scripts/Template/configuration.ini
#cd ~/Iris/IRIS4/

File="/home/sandy/Iris/IRIS4/scripts/Matching/ImageList.txt"
echo ""> $File
for files1 in ~/Iris/out/tiff/*.tiff
do
	ncfile1=$(basename $files1)
	for files2 in ~/Iris/out/tiff/*.tiff
	do
		ncfile2=$(basename $files2)
		echo $ncfile1 $ncfile2 >> $File
	done
done

./osiris ../scripts/Matching/configuration.ini

