CC=g++

CFLAGS=-c -Wall -O3 -s -fpic -std=c++0x -Wno-strict-aliasing -DNDEBUG \
       -I $(HOME)/local-gcc-trunk/include \
       -I $(HOME)/dev/tmp/libev_run/include \
       -I $(HOME)/dev/repo/http-parser \
       -I $(HOME)/local-gcc-trunk/boost-trunk-ctx/include

LDFLAGS=-Wl,-rpath,$(HOME)/local-gcc-trunk/lib64 \
        -Wl,-rpath,$(HOME)/local-gcc-trunk/lib \
        -Wl,-rpath,$(HOME)/dev/tmp/libev_run/lib \
        -Wl,-rpath,$(HOME)/local-gcc-trunk/boost-trunk-ctx/lib \
        -L $(HOME)/local-gcc-trunk/lib64 \
        -L $(HOME)/local-gcc-trunk/lib \
        -L $(HOME)/dev/tmp/libev_run/lib \
        -L $(HOME)/local-gcc-trunk/boost-trunk-ctx/lib \
        -lstdc++ -lpthread -lev -lboost_context \
        -O3 -s

EXE=ratel

vpath %.cpp src

SRC=main.cpp server.cpp connection.cpp

OBJ=$(patsubst %.cpp,objects/%.o,$(SRC)) 

objects/%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) $(HOME)/dev/repo/http-parser/http_parser.o -o $@

all: odir $(EXE)

odir:
	mkdir -p objects

clean:
	rm -rf objects/*.o $(EXE)

