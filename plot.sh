#!/bin/sh
gnuplot << EOF
set terminal postscript eps color enhanced
set output "$1.eps"
set xlabel "Energy [MeV]"
set ylabel "Cross Section [b]"
set title "(n,2n) reaction"
set xrange [ 0 : 20 ]
set yrange [ 0 : 2 ]
set mxtics 5
set mytics 5
set xtics 5
set ytics 0.5
plot "$1.dat" using 1:2 notitle w l
EOF

