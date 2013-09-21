all:
	g++ -D_ERROR -D_WARNING -D_INFO  -g -o bin/test  src/BideThread.cpp  src/main.cpp  src/DataShared.cpp src/commonFunc.cpp  -I./include
mpi:
	mpic++ -D_ERROR -D_WARNING -g  src/mpimain.cpp  src/BideThread.cpp  src/DataShared.cpp src/commonFunc.cpp -I./include -Wl,-rpath /usr/lib64/openmpi/lib/ -o bin/mpimining
