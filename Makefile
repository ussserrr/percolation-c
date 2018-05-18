single_experiment:
	gcc-8 single_experiment.c percolation.c -o single_experiment -Ofast -std=c17 -Wall -lsodium

series_experiments:
	gcc-8 series_experiments.c percolation.c -o series_experiments -Ofast -std=c17 -Wall -lsodium -lpthread -DSERIES -DMULTITHREADING

clean:
	rm -rf single_experiment series_experiments percolation
