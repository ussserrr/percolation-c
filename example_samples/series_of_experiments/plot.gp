set terminal qt size 1024,768

set style data lines

set title "Percolation threshold"
set title font "Helvetica,24"

set grid

set key left
set key font "Helvetica,14"
# set key title 'Grid size, number of experiments'
# set key title font "Helvetica,18"
set key spacing 2

set xlabel 'Probability of the cell occupation'
set xlabel font "Helvetica,14"
set ylabel "Fraction of experiments ended with percolation"
set ylabel font "Helvetica,14"
set tics font "Helvetica,14"

plot '10x10, N=1000', '15x15, N=1000', '20x20, N=1000'

pause -1
