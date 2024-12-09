# use alpine as base image
FROM alpine:latest as build-env

# install meta packages inside build-env container
RUN apk add --no-cache build-base libgomp

# change directory to /nn
WORKDIR /nn

# copy all files from current directory inside the build-env container
COPY ./src/* .

# Compile the source code and generate nearest-neighbor binary executable file
RUN gcc -o nearest_neighbor main.c nearest-neighbor.c -lm -fopenmp

COPY nearest_neighbor ./build/nearest_neighbor

# # use another container to run the program
# FROM alpine:latest as run-env

# # install meta packages inside build-env container
# RUN apk add --no-cache libgomp

# # copy binary executable to new container
# COPY --from=build-env /nn/nearest_neighbor /nn/nearest_neighbor

# WORKDIR /nn

# # at last run the program
# CMD ["time", "./nearest_neighbor"]
