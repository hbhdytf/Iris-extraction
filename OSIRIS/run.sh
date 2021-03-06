#!/bin/sh
echo "\033[33m============================================================== \033[0m"
mkdir -p ~/Iris/bmp/
mkdir -p ~/Iris/Output/Segmentation/Snake/
mkdir -p ~/Iris/Output/Segmentation/Hough/

#转换图片格式
#dabase="/home/sandy/CASIA/001/"
#cd ~/
#cp ${database+'/*'} ~/Iris/bmp/

sh convert.sh "/home/sandy/CASIA/001/L"
sh convert.sh "/home/sandy/CASIA/002/L"
#sh convert.sh "/home/sandy/CASIA/003/L"
sh genlist.sh
cd ~/Iris/OSIRIS/src
./osiris -s ../scripts/Segmentation/Script_Segmentation.txt
echo "\033[33m============================================================== \033[0m"
File="/home/sandy/Iris/OSIRIS/scripts/Normalisation/Script_normalisation1.txt"
echo "Options: -w -k -p
Filter: /home/sandy/Iris/OSIRIS/OsirisParam/GaborFilterBank.txt
Points: /home/sandy/Iris/OSIRIS/OsirisParam/Point.txt
Path_results: /home/sandy/Iris/Output/Normalisation/
Result_file: Resultat_Norm.txt
Path_source: /home/sandy/Iris/Output/Segmentation/" >$File
suffix=".*"
for files in ~/Iris/bmp/*.bmp
do
	ncfile=$(basename $files)
	echo  "Match:\t"$ncfile"\t"${ncfile%$suffix}"_mask.bmp\t"${ncfile%$suffix}".Param\t"${ncfile%$suffix}".Param" >> $File
done
mkdir -p ~/Iris/Output/Normalisation/
cp -r ~/Iris/bmp/* ~/Iris/Output/Segmentation
cd ~/Iris/OSIRIS/src
./osiris -s ../scripts/Normalisation/Script_normalisation1.txt
echo "\033[33m============================================================== \033[0m"
File="/home/sandy/Iris/OSIRIS/scripts/Matching/Script_feature.txt"
echo "Options: -e -c
Filter: /home/sandy/Iris/OSIRIS/OsirisParam/GaborFilterBank.txt
Points: /home/sandy/Iris/OSIRIS/OsirisParam/Point.txt
Path_results: /home/sandy/Iris/Output/Matching/
Path_source: /home/sandy/Iris/Output/Normalisation/
Result_file: Script_feature.txt"> $File
suffix=".*"
for files1 in ~/Iris/bmp/*.bmp
do
	ncfile1=$(basename $files1)
	for files2 in ~/Iris/bmp/*.bmp
	do
		ncfile2=$(basename $files2)
		echo  "Match:\t"${ncfile1%$suffix}"_Norm.bmp\t"${ncfile2%$suffix}"_Norm.bmp\t"${ncfile1%$suffix}"_mask_Norm.bmp\t"${ncfile2%$suffix}"_mask_Norm.bmp" >> $File
	done
done
mkdir -p ~/Iris/Output/Matching/
cd ~/Iris/OSIRIS/src
./osiris -s ../scripts/Matching/Script_feature.txt
echo "\033[33m============================================================== \033[0m"
