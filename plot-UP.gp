# This is a gnuplot script file
# set terminal pngcairo  # Sets the output to PNG. Change as needed.
# set output 'output_plot.png'  # Filename for the output image.

set title "Comparison Plot"
set xlabel "UP [unit]"
set xrange [0:1]
set xtics 0,0.1,100
set yrange [0:1]
set ylabel "RDV [unit]"
set key left top
set grid xtics ytics
set pointsize 1.5 
set terminal postscript eps "Times-New-Roman" 20 color
set output "result-UP.eps"#

# If your data has headers, you might need to use 'set key autotitle columnheader'
set key outside # Puts the legend outside the plot

plot "data_wifi.txt" using 2:4 lw 6 lt 7 lc rgb "#A70D2A" with linespoints title 'WiFi', \
     "data_4G.txt" using 2:4 lw 6 lt 9 lc rgb "#00A36C" with linespoints title '4G'
set output