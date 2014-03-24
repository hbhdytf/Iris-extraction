#!/bin/sh
echo "\033[33m============================================================== \033[0m"
mkdir -p ~/Iris/bmp/
mkdir -p ~/Iris/Output/Segmentation/Snake/
mkdir -p ~/Iris/Output/Segmentation/Hough/
sh genlist.sh
cd ~/Iris/OSIRIS/src
./osiris -s ../scripts/Segmentation/Script_Segmentation.txt
echo "\033[33m============================================================== \033[0m"
mkdir -p ~/Iris/Output/Normalisation/
cp ~/Iris/bmp/* ~/Iris/Output/Segmentation
cd ~/Iris/OSIRIS/src
./osiris -s ../scripts/Normalisation/Script_normalisation1.txt
echo "\033[33m============================================================== \033[0m"
mkdir -p ~/Iris/Output/Matching/
cd ~/Iris/OSIRIS/src
./osiris -s ../scripts/Matching/Script_feature.txt
echo "\033[33m============================================================== \033[0m"
