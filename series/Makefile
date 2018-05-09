single_experiment:
	gcc single_experiment.c percolation.c -o single_experiment -std=c11 -Wall -lsodium

multi_experiments:
	gcc multi_experiments.c percolation.c -o multi_experiments -O3 -std=c11 -Wall -lsodium

clean:
	rm -rf single_experiment multi_experiments percolation
