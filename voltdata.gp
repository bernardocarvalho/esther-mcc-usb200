#!/usr/bin/env gnuplot
#
#
clear
# set term png
set term png enhanced font '/usr/share/fonts/liberation/LiberationSans-Regular.ttf' 12
set output 'voltData.png'

#set xlabel 'Time (s)'
#set xlabel 'Samp'
set ylabel 'Volt'
set xlabel 'Time / Sec'

set title 'ADC  Data'

dfile='voltdata.bin'
plot dfile binary format='%double' using ($0/50e3):($1) with lines lt 1  title 'ADC Ch 0'
#set term x11

set term qt
replot
pause -1 "Hit return to continue"
