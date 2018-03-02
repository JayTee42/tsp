CFLAGS = -O3 -fopenmp -DUSE_OMP -DUSE_CTZ_MACRO

OS := $(shell uname)

ifeq ($(OS),Darwin)
CC = /usr/local/opt/llvm/bin/clang
CFLAGS += -I/usr/local/opt/llvm/include
LDFLAGS += -L/usr/local/opt/llvm/lib
endif

tsp: tsp.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

.PHONY: clean

clean:
	rm -f tsp *.o
