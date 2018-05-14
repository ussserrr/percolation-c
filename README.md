# percolation-c
![20x20](/example%20samples/single%20experiment/20x20.png)

See [percolation-python](https://github.com/ussserrr/percolation-python) for Python version of this program.


# Overview
Model of the two-dimensional percolation phenomena. In a nutshell: we have a 2D grid with the given size (Y,X) and we randomly fill it with the given probability P. After some time we can get the cluster - aggregate of cells - that connects two opposite borders of the grid. In this case we can say that *percolation happened* and call the cluster as *percolated cluster*. Of course, there are may be more than one such cluster but in practice we do not have many of them due to random character of the phenomena. This model can represent (at some level of approximation) a variety of real-world physical systems such as electrical conductivity, fluid flows and so on. We can research such models in different ways, for example find a *percolation threshold* - value of the probability at which at least 60% of all experiments ends with percolation.


# Architecture
Main library - `percolation.h`/`percolation.c`. It contains algorithm' functions and the function representing one whole experiment. Grid is a two-dimensional array of integers where `0` and `1` mean empty and filled cells respectively. There are few macros controlling some functionality but there is no need to change them by default.

Two modes of operation are present:
  - **Single experiment**: performs one run, prints grid and result in a terminal. Also percolated clusters are highlighted by colors. In the given example, `single_experiment.c`, we generate grids again and again until there is no one with 2 percolated clusters at once appears and then display it;
  - **Series of experiments**: quietly runs without displaying the grids (only progress). At each step we increment the probability of cell occupation and also for every probability perform N experiments. Finally, we will get the file named like '25x25, N=100', using that, we can plot the relation between some parameter (e.g., fraction of percolated cluster in all occupied cells or fraction of experiments ended with percolation) and probability.
  ![gnuplot](/example%20samples/series%20of%20experiments/plot.png)


# Dependencies
  - [libsodium](https://download.libsodium.org/doc) - random-number generation library
  - GCC, Clang or some C compiler
  - Make - for Makefile
  - [gnuplot](http://www.gnuplot.info) - optional, used for plotting the data produced in "series of experiments" mode


# Building
Use Makefile to build
```bash
$ make single_experiment
```
or
```bash
$ make series_experiments
```
and clean
```bash
$ make clean
```


# Usage
To start, run:
```bash
$ ./single_experiment
```
or
```bash
$ ./series_experiments
```
For last case, after completing you can run
```bash
$ gnuplot plot.gp
```
(see "./example samples/series of experiments") to plot the data.
