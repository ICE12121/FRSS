#!/bin/sh

cmrb FLC.fis rules

# g++ Main_SDN_controller.cpp -o main -lambigu

./main

# mkpdt filelist result 0 100 10

chmod +x CV=*.sh
./CV=*.sh

rm -r Results/
mkdir "Results"

chmod +x CV=*.eps

mv CV=*.eps Results

rm -r Rawdata/
mkdir "Rawdata"

chmod +x CV=*-UP=*
mv CV=*-UP=*  Rawdata

