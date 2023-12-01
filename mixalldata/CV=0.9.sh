#!/usr/bin/gnuplot

set title "CV=0.9" 
set xlabel "SE [unit]"
set xrange [0:1]
set xtics 0,0.1,1
set yrange [0:1]
set ytics 0,0.1,1
set ylabel "RDV [unit]"
#set zlabel "NF=1"
set key outside # Puts the legend outside the plot
set grid xtics ytics
set pointsize 0.5 
#set y2tics font "Helvetica:12"
set terminal postscript eps "Times-New-Roman" 10 color
set output "CV=0.9.eps"#
plot "WIFI-CV=0.9-UP=0.1.txt" using 3:4 lw 3 lt 9 lc rgb "#85C1E9" title "WiFi-UP=0.1" with linespoints,\
     "WIFI-CV=0.9-UP=0.5.txt" using 3:4 lw 3 lt 9 lc rgb "#2E86C1" title "WiFi-UP=0.5" with linespoints,\
     "WIFI-CV=0.9-UP=0.9.txt" using 3:4 lw 3 lt 5 lc rgb "#21618C" title "WiFi-UP=0.9" with linespoints,\
     "4G-CV=0.9-UP=0.1.txt" using 3:4 lw 3 lt 9 lc rgb "#EC7063" title "4G-UP=0.1" with linespoints,\
     "4G-CV=0.9-UP=0.5.txt" using 3:4 lw 3 lt 9 lc rgb "#CB4335" title "4G-UP=0.5" with linespoints,\
     "4G-CV=0.9-UP=0.9.txt" using 3:4 lw 3 lt 5 lc rgb "#943126" title "4G-UP=0.9" with linespoints,\
     "5G-CV=0.9-UP=0.1.txt" using 3:4 lw 3 lt 9 lc rgb "#2ECC71" title "5G-UP=0.1" with linespoints,\
     "5G-CV=0.9-UP=0.5.txt" using 3:4 lw 3 lt 9 lc rgb "#239B56" title "5G-UP=0.5" with linespoints,\
     "5G-CV=0.9-UP=0.9.txt" using 3:4 lw 3 lt 5 lc rgb "#186A3B" title "5G-UP=0.9" with linespoints

set output
#set output "new1.eps"
#plot "16" using 1:2 lw 5 title "n0-n6" with lines,"17" using 1:2 lw 5 title "n0-n7" with lines,"18" using 1:2 lw 5 title "n0-n8" with lines,"19" using 1:2 lw 5 title "n0-n9" with lines,"110" using 1:2 lw 5 title "n0-n10" with lines,"111" using 1:2 lw 5 title "n0-n11" with lines
#set output

