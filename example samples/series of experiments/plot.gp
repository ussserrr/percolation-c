set terminal qt

set style data lines

set title "Percolation threshold"

set grid

set key left
set keytitle 'Grid size, number of experiments'

set xlabel 'Probability of the cell occupation'
set ylabel "Fraction of experiments\nended with percolation"

plot '10x10, N=100', '15x15, N=100', '20x20, N=100', '100x100, N=100'

pause -1
