#!/usr/bin/env gnuplot
#
#
clear
# set term png
set term png enhanced font '/usr/share/fonts/liberation/LiberationSans-Regular.ttf' 12
set output 'dac16Data.png'

#set xlabel 'Time (s)'
#set xlabel 'Samp'
set xlabel 'Time / Sec'

set title 'ADC 16 bit out Data'

dfile='rawdata.bin'
plot dfile binary format='%int16' using ($0/50e3):($1) with lines lt 1  title 'DAC Ch 0'
#set term x11

set term qt
replot
pause -1 "Hit return to continue"
