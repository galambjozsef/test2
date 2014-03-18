#----------------------------------------------------------------------------
# # Released by KORG ITALY S.P.A.
# # Author: Stefano Tinti
# # Date:   09/05/2012
#
#----------------------------------------------------------------------------

#CXX è passato da build.py
#CXX=/data/development/build-tools/linux/arm/cs-arm-2009q3-lite/bin/arm-none-linux-gnueabi-g++
#CXX=gcc

CFLAGS  = -D_GNU_SOURCE

PROG_NAME=test

all: test

test: test.o myclass.o utility.o  thread.o iworker.o workerthread.o timestamp.o  psemaphore.o commander.o korgdevice.o korgcmd.o criticalsection.o criticalsectionlock.o korgsink.o invoker.o dokorgcmd.o
	$(CXX) test.o utility.o myclass.o thread.o iworker.o workerthread.o timestamp.o  psemaphore.o commander.o korgdevice.o korgcmd.o criticalsection.o criticalsectionlock.o korgsink.o invoker.o dokorgcmd.o -D_GNU_SOURCE -o $(BUILD_DIR)$(PROG_NAME) $(CFLAGS) -lpthread

myclass.o: myclass.cpp
	$(CXX) -c myclass.cpp -O3 $(CFLAGS) -lpthread -o $@

thread.o: thread.cpp
	$(CXX) -c thread.cpp -O3 $(CFLAGS) -lpthread -o $@

iworker.o: iworker.cpp
	$(CXX) -c iworker.cpp -O3 $(CFLAGS) -lpthread -o $@

workerthread.o: workerthread.cpp
	$(CXX) -c workerthread.cpp -O3 $(CFLAGS) -lpthread -o $@

timestamp.o: timestamp.cpp
	$(CXX) -c timestamp.cpp -O3 $(CFLAGS) -lpthread -o $@

utility.o: utility.cpp
	$(CXX) -c utility.cpp -O3 $(CFLAGS) -lpthread -o $@

psemaphore.o: psemaphore.cpp
	$(CXX) -c psemaphore.cpp -O3 $(CFLAGS) -lpthread -o $@

commander.o: commander.cpp
	$(CXX) -c commander.cpp -O3 $(CFLAGS) -lpthread -o $@	

korgdevice.o: korgdevice.cpp
	$(CXX) -c korgdevice.cpp -O3 $(CFLAGS) -lpthread -o $@	
	
korgcmd.o: korgcmd.cpp
	$(CXX) -c korgcmd.cpp -O3 $(CFLAGS) -lpthread -o $@	
	
criticalsection.o: criticalsection.cpp
	$(CXX) -c criticalsection.cpp -O3 $(CFLAGS) -lpthread -o $@		

criticalsectionlock.o: criticalsectionlock.cpp
	$(CXX) -c criticalsectionlock.cpp -O3 $(CFLAGS) -lpthread -o $@		

korgsink.o: korgsink.cpp
	$(CXX) -c korgsink.cpp -O3 $(CFLAGS) -lpthread -o $@		

invoker.o: invoker.cpp
	$(CXX) -c invoker.cpp -O3 $(CFLAGS) -lpthread -o $@

dokorgcmd.o: dokorgcmd.cpp
	$(CXX) -c dokorgcmd.cpp -O3 $(CFLAGS) -lpthread -o $@	
				
test.o: test.cpp
	$(CXX) -c test.cpp -O3 $(CFLAGS) -lpthread -o $@
	
clean:
#	rm -f test.o
#	rm -f myclass.o
#	rm -f thread.o
#	rm -f iworker.o
#	rm -f workerthread.o
#	rm -f commander.o
#	rm -f utility.o	
	rm -f *.o
	rm -f $(BUILD_DIR)$(PROG_NAME)

#install:
#	cp $(BUILD_DIR)/$(PROG_NAME) /data/development/lfo/target/build/andoria/rootfs/full/
	
	
	
	
	
	
	
	
	
