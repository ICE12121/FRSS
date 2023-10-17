#!/usr/bin/gnuplot
set title "CV=0.1" 
set xlabel "SE [%]"
set xrange [0:1]
set xtics 0,0.1,100
set yrange [0:1]
set ytics 0,0.1,1
set ylabel "RDV [unit]"
#set zlabel "NF=1"
set key left top
set grid xtics ytics
set pointsize 1.5 
#set y2tics font "Helvetica:12"
set terminal postscript eps "Times-New-Roman" 20 color
set output "CV=10%.eps"#
plot "CV=0.1-UP=0.1" using 1:2 lw 6 lt 7 lc rgb "#A70D2A" title "UP=10%" with linespoints,\
     "CV=0.1-UP=0.5" using 1:2 lw 6 lt 9 lc rgb "#00A36C" title "UP=50%" with linespoints,\
     "CV=0.1-UP=0.9" using 1:2 lw 6 lt 5 lc rgb "#3090C7" title "UP=90%" with linespoints

set output
#set output "new1.eps"
#plot "16" using 1:2 lw 5 title "n0-n6" with lines,"17" using 1:2 lw 5 title "n0-n7" with lines,"18" using 1:2 lw 5 title "n0-n8" with lines,"19" using 1:2 lw 5 title "n0-n9" with lines,"110" using 1:2 lw 5 title "n0-n10" with lines,"111" using 1:2 lw 5 title "n0-n11" with lines
#set output
