# nearest-neighbor-poc

Simple implementation of the nearest neighbor algorithm, in C. Based on the original implementation of [Luciano2302](https://github.com/Luciano2302).

## Quick start up

To build, just run:

```bash
gcc -o nearest_neighbor src/main.c src/nearest-neighbor.c -lm -fopenmp
```

To run it, gathering info concerning execution time:

```bash
time ./nearest_neighbor
```

## Related documentation

- An [article](https://medium.com/swlh/k-nearest-neighbor-ca2593d7a3c4) on Medium, explaining the K-NN algorithm in a simple way;
