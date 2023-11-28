#!/usr/bin/gnuplot

set title "CV=0.5" 
set xlabel "SE [unit]"
set xrange [0:1]
set xtics 0,1,100
set yrange [0:1]
set ytics 0,0.1,1
set ylabel "RDV [unit]"
#set zlabel "NF=1"
set key left top
set grid xtics ytics
set pointsize 1.5 
#set y2tics font "Helvetica:12"
set terminal postscript eps "Times-New-Roman" 20 color
set output "CV=0.5.eps"#
plot "WIFI-CV=0.5-UP=0.1.txt" using 3:4 lw 6 lt 7 lc rgb "#A70D2A" title "WiFi-UP=0.1" with linespoints,\
     "WIFI-CV=0.5-UP=0.5.txt" using 3:4 lw 6 lt 9 lc rgb "#A63E0D" title "WiFi-UP=0.5" with linespoints,\
     "WIFI-CV=0.5-UP=0.9.txt" using 3:4 lw 6 lt 9 lc rgb "#EC8453" title "WiFi-UP=0.9" with linespoints,\
     "4G-CV=0.5-UP=0.1.txt" using 3:4 lw 6 lt 9 lc rgb "#00A36C" title "4G-UP=0.1" with linespoints,\
     "4G-CV=0.5-UP=0.5.txt" using 3:4 lw 6 lt 9 lc rgb "#00A36C" title "4G-UP=0.5" with linespoints,\
     "4G-CV=0.5-UP=0.9.txt" using 3:4 lw 6 lt 5 lc rgb "#50F3BC" title "4G-UP=0.9" with linespoints

set output
#set output "new1.eps"
#plot "16" using 1:2 lw 5 title "n0-n6" with lines,"17" using 1:2 lw 5 title "n0-n7" with lines,"18" using 1:2 lw 5 title "n0-n8" with lines,"19" using 1:2 lw 5 title "n0-n9" with lines,"110" using 1:2 lw 5 title "n0-n10" with lines,"111" using 1:2 lw 5 title "n0-n11" with lines
#set output
