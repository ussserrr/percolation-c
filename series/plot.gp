set terminal qt

set style data lines

set title "Зависимость перколяции от вероятности заполнения ячейки\n\
		   при различных размерах решетки"

set key left
set keytitle 'Размер решетки'

set xlabel 'Вероятность заполнения ячейки'
# set ylabel "Доля экспериментов,\nокончившихся перколяцией"

plot '100x100'  # , '16x16', '24x24'

pause -1