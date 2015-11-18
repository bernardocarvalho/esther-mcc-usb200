#!/usr/bin/env gnuplot
#
#
clear
# set term png
set term png enhanced font '/usr/share/fonts/liberation/LiberationSans-Regular.ttf' 12
# from pulse 46 Sampling rate is 100KSKS
pulse = 46
Fs = 100e3
set output 'voltData-'.pulse.'.png'

#set xlabel 'Time (s)'
#set xlabel 'Samp'
set ylabel 'Bar'
set xlabel 'Time / Sec'

set title 'KISTLER ADC Data, Pulse #'.pulse

dfile='voltdata-'.pulse.'.bin'

plot_dec = 6 
firstl = 0
endl = 8e3


plot dfile binary format='%double' every plot_dec::firstl:0:endl using ($0/Fs):($1*20) with lines lt 1  title 'ADC Ch 0'
#set term x11

#set term qt
set term wxt
replot
pause -1 "Hit return to continue"

