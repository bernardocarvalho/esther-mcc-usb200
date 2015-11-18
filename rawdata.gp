#!/usr/bin/env gnuplot
#
#
clear
# set term png
set term png enhanced font '/usr/share/fonts/liberation/LiberationSans-Regular.ttf' 12
set output 'voltData.png'

#set xlabel 'Time (s)'
#set xlabel 'Samp'
set ylabel 'Bar'
set xlabel 'Time / Sec'

set title 'KISTLER ADC Data'

dfile='voltdata-36.bin'

plot_dec = 10 
firstl = 69.5e3
endl = 71e3


plot dfile binary format='%double' every plot_dec::firstl:0:endl using ($0/50e3):($1*-20) with lines lt 1  title 'ADC Ch 0'
#set term x11

#set term qt
set term wxt
replot
pause -1 "Hit return to continue"

