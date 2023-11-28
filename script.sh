chmod +x CV=*.sh
./CV=*.sh

rm -r Results/
mkdir "Results"

chmod +x CV=*.eps

mv CV=*.eps Results

rm -r Rawdata/
mkdir "Rawdata"

chmod +x WIFI-CV=*-UP=*
mv WIFI-CV=*-UP=*  Rawdata

chmod +x 4G-CV=*-UP=*
mv WIFI-CV=*-UP=*  Rawdata
