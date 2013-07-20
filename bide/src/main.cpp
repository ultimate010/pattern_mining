#include "zthread/Thread.h"
#include "zthread/Runnable.h"
#include "zthread/ThreadedExecutor.h"
#include "zthread/PoolExecutor.h"
#include "zthread/SynchronousExecutor.h"
#include "zthread/ConcurrentExecutor.h"
#include "zthread/CountedPtr.h"
#include "zthread/Cancelable.h"
#include "DataShared.h"
#include "BideThread.h"
#include <iostream>
#include <fstream>
using namespace std;
using namespace ZThread;



int main(){

CountedPtr<DataShared> countPtr(new DataShared);
	try{
		Thread t1(new BideThread(1,countPtr));
		Thread t2(new BideThread(2,countPtr));
		Thread t3(new BideThread(3,countPtr));
		Thread t4(new BideThread(4,countPtr));
		Thread t5(new BideThread(5,countPtr));
		Thread t6(new BideThread(6,countPtr));
		Thread t7(new BideThread(7,countPtr));
		Thread t8(new BideThread(8,countPtr));
		Thread t9(new BideThread(9,countPtr));
		Thread t10(new BideThread(10,countPtr));
		Thread t11(new BideThread(11,countPtr));
		Thread t12(new BideThread(12,countPtr));
		Thread t13(new BideThread(13,countPtr));
		Thread t14(new BideThread(14,countPtr));
		Thread t15(new BideThread(15,countPtr));
		Thread t16(new BideThread(16,countPtr));
	}catch(Synchronization_Exception &e){
		cerr <<e.what() <<endl;
	}
}