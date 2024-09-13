SSE_ARCH_FLAGS=-msse3

CC=g++
ARCH=-m64 $(SSE_ARCH_FLAGS)
CFLAGS= -O2 $(ARCH) -g -Wall -Iinc
LDFLAGS=$(ARCH)
LIBS=-lrt

# Targets for building all configurations
all: build_matmul_single build_matmul_double

# Build single-precision matrix multiplication programs for all loop orderings
build_matmul_single: build/matmul_single_IJK build/matmul_single_IKJ build/matmul_single_JIK build/matmul_single_JKI build/matmul_single_KIJ build/matmul_single_KJI

build/matmul_single_IJK: build/matmul_single_IJK.o
	$(CC) $(LDFLAGS) -o build/matmul_single_IJK build/matmul_single_IJK.o $(LIBS)

build/matmul_single_IJK.o: src/matmul.cpp
	$(CC) -c -o build/matmul_single_IJK.o $(CFLAGS) -DSINGLE -DORDER_IJK src/matmul.cpp

build/matmul_single_IKJ: build/matmul_single_IKJ.o
	$(CC) $(LDFLAGS) -o build/matmul_single_IKJ build/matmul_single_IKJ.o $(LIBS)

build/matmul_single_IKJ.o: src/matmul.cpp
	$(CC) -c -o build/matmul_single_IKJ.o $(CFLAGS) -DSINGLE -DORDER_IKJ src/matmul.cpp

build/matmul_single_JIK: build/matmul_single_JIK.o
	$(CC) $(LDFLAGS) -o build/matmul_single_JIK build/matmul_single_JIK.o $(LIBS)

build/matmul_single_JIK.o: src/matmul.cpp
	$(CC) -c -o build/matmul_single_JIK.o $(CFLAGS) -DSINGLE -DORDER_JIK src/matmul.cpp

build/matmul_single_JKI: build/matmul_single_JKI.o
	$(CC) $(LDFLAGS) -o build/matmul_single_JKI build/matmul_single_JKI.o $(LIBS)

build/matmul_single_JKI.o: src/matmul.cpp
	$(CC) -c -o build/matmul_single_JKI.o $(CFLAGS) -DSINGLE -DORDER_JKI src/matmul.cpp

build/matmul_single_KIJ: build/matmul_single_KIJ.o
	$(CC) $(LDFLAGS) -o build/matmul_single_KIJ build/matmul_single_KIJ.o $(LIBS)

build/matmul_single_KIJ.o: src/matmul.cpp
	$(CC) -c -o build/matmul_single_KIJ.o $(CFLAGS) -DSINGLE -DORDER_KIJ src/matmul.cpp

build/matmul_single_KJI: build/matmul_single_KJI.o
	$(CC) $(LDFLAGS) -o build/matmul_single_KJI build/matmul_single_KJI.o $(LIBS)

build/matmul_single_KJI.o: src/matmul.cpp
	$(CC) -c -o build/matmul_single_KJI.o $(CFLAGS) -DSINGLE -DORDER_KJI src/matmul.cpp

# Build double-precision matrix multiplication programs for all loop orderings
build_matmul_double: build/matmul_double_IJK build/matmul_double_IKJ build/matmul_double_JIK build/matmul_double_JKI build/matmul_double_KIJ build/matmul_double_KJI

build/matmul_double_IJK: build/matmul_double_IJK.o
	$(CC) $(LDFLAGS) -o build/matmul_double_IJK build/matmul_double_IJK.o $(LIBS)

build/matmul_double_IJK.o: src/matmul.cpp
	$(CC) -c -o build/matmul_double_IJK.o $(CFLAGS) -DDOUBLE -DORDER_IJK src/matmul.cpp

build/matmul_double_IKJ: build/matmul_double_IKJ.o
	$(CC) $(LDFLAGS) -o build/matmul_double_IKJ build/matmul_double_IKJ.o $(LIBS)

build/matmul_double_IKJ.o: src/matmul.cpp
	$(CC) -c -o build/matmul_double_IKJ.o $(CFLAGS) -DDOUBLE -DORDER_IKJ src/matmul.cpp

build/matmul_double_JIK: build/matmul_double_JIK.o
	$(CC) $(LDFLAGS) -o build/matmul_double_JIK build/matmul_double_JIK.o $(LIBS)

build/matmul_double_JIK.o: src/matmul.cpp
	$(CC) -c -o build/matmul_double_JIK.o $(CFLAGS) -DDOUBLE -DORDER_JIK src/matmul.cpp

build/matmul_double_JKI: build/matmul_double_JKI.o
	$(CC) $(LDFLAGS) -o build/matmul_double_JKI build/matmul_double_JKI.o $(LIBS)

build/matmul_double_JKI.o: src/matmul.cpp
	$(CC) -c -o build/matmul_double_JKI.o $(CFLAGS) -DDOUBLE -DORDER_JKI src/matmul.cpp

build/matmul_double_KIJ: build/matmul_double_KIJ.o
	$(CC) $(LDFLAGS) -o build/matmul_double_KIJ build/matmul_double_KIJ.o $(LIBS)

build/matmul_double_KIJ.o: src/matmul.cpp
	$(CC) -c -o build/matmul_double_KIJ.o $(CFLAGS) -DDOUBLE -DORDER_KIJ src/matmul.cpp

build/matmul_double_KJI: build/matmul_double_KJI.o
	$(CC) $(LDFLAGS) -o build/matmul_double_KJI build/matmul_double_KJI.o $(LIBS)

build/matmul_double_KJI.o: src/matmul.cpp
	$(CC) -c -o build/matmul_double_KJI.o $(CFLAGS) -DDOUBLE -DORDER_KJI src/matmul.cpp

# Commands to run the executables
run_matmul_single_IJK: build_matmul_single
	./build/matmul_single_IJK 4096

run_matmul_single_IKJ: build_matmul_single
	./build/matmul_single_IKJ 4096

run_matmul_single_JIK: build_matmul_single
	./build/matmul_single_JIK 4096

run_matmul_single_JKI: build_matmul_single
	./build/matmul_single_JKI 4096

run_matmul_single_KIJ: build_matmul_single
	./build/matmul_single_KIJ 4096

run_matmul_single_KJI: build_matmul_single
	./build/matmul_single_KJI 4096

run_matmul_double_IJK: build_matmul_double
	./build/matmul_double_IJK 4096

run_matmul_double_IKJ: build_matmul_double
	./build/matmul_double_IKJ 4096

run_matmul_double_JIK: build_matmul_double
	./build/matmul_double_JIK 4096

run_matmul_double_JKI: build_matmul_double
	./build/matmul_double_JKI 4096

run_matmul_double_KIJ: build_matmul_double
	./build/matmul_double_KIJ 4096

run_matmul_double_KJI: build_matmul_double
	./build/matmul_double_KJI 4096

# Clean up build artifacts
clean:
	$(RM) build/*.o build/matmul_single_* build/matmul_double_*

.PHONY: clean all run_matmul_single_IJK run_matmul_single_IKJ run_matmul_single_JIK run_matmul_single_JKI run_matmul_single_KIJ run_matmul_single_KJI run_matmul_double_IJK run_matmul_double_IKJ run_matmul_double_JIK run_matmul_double_JKI run_matmul_double_KIJ run_matmul_double_KJI
