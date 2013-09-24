all:
	g++ -D_ERROR -D_WARNING -g -o bin/test  src/BideThread.cpp  src/main.cpp  src/DataShared.cpp src/commonFunc.cpp  -I./include
allthread:
	g++ -D_ERROR -D_WARNING -D_THREAD -g -o bin/testthread  src/BideThread.cpp  src/mainthread.cpp src/DataShared.cpp src/commonFunc.cpp  -I./include -lpthread
mpi:
	mpic++ -D_ERROR -D_WARNING -g  src/mpimain.cpp  src/BideThread.cpp  src/DataShared.cpp src/commonFunc.cpp -I./include -Wl,-rpath /usr/lib64/openmpi/lib/ -o bin/mpimining
mpithread:
	mpic++ -D_ERROR -D_WARNING -D_THREAD -g  src/mpimainThread.cpp  src/BideThread.cpp  src/DataShared.cpp src/commonFunc.cpp -I./include -Wl,-rpath /usr/lib64/openmpi/lib/ -o bin/mpiminingthread -lpthread
