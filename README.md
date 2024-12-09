# nearest-neighbor-poc

Simple implementation of the nearest neighbor algorithm, in C. Based on the original implementation of [Luciano2302](https://github.com/Luciano2302).

To build, just run:

```bash
gcc -o nearest_neighbor src/main.c src/nearest-neighbor.c -lm -fopenmp
```

To run it, gathering info concerning execution time:

```bash
time ./nearest_neighbor
```
