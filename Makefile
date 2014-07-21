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

test: test.o myclass.o utility.o  thread.o iworker.o workerthread.o timestamp.o  psemaphore.o commander.o korgdevice.o korgcmd.o criticalsection.o criticalsectionlock.o korgsink.o invoker.o dokorgcmd.o statistics.o socketclient.o socketserver.o clientconnection.o proxy.o iproxy.o dodevmem2.o
	$(CXX) test.o utility.o myclass.o thread.o iworker.o workerthread.o timestamp.o  psemaphore.o commander.o korgdevice.o korgcmd.o criticalsection.o criticalsectionlock.o korgsink.o invoker.o dokorgcmd.o statistics.o socketclient.o socketserver.o clientconnection.o proxy.o iproxy.o dodevmem2.o  -D_GNU_SOURCE -o $(BUILD_DIR)$(PROG_NAME) $(CFLAGS) -lpthread -rdynamic

myclass.o: myclass.cpp
	$(CXX) -c myclass.cpp -O3 $(CFLAGS) -lpthread -rdynamic -o $@

thread.o: thread.cpp
	$(CXX) -c thread.cpp -O3 $(CFLAGS) -lpthread -rdynamic -o $@

iworker.o: iworker.cpp
	$(CXX) -c iworker.cpp -O3 $(CFLAGS) -lpthread -rdynamic -o $@

workerthread.o: workerthread.cpp
	$(CXX) -c workerthread.cpp -O3 $(CFLAGS) -lpthread -rdynamic -o $@

timestamp.o: timestamp.cpp
	$(CXX) -c timestamp.cpp -O3 $(CFLAGS) -lpthread -rdynamic -o $@

utility.o: utility.cpp
	$(CXX) -c utility.cpp -O3 $(CFLAGS) -lpthread -rdynamic -o $@

psemaphore.o: psemaphore.cpp
	$(CXX) -c psemaphore.cpp -O3 $(CFLAGS) -lpthread -rdynamic -o $@

commander.o: commander.cpp
	$(CXX) -c commander.cpp -O3 $(CFLAGS) -lpthread -rdynamic -o $@	

korgdevice.o: korgdevice.cpp
	$(CXX) -c korgdevice.cpp -O3 $(CFLAGS) -lpthread -rdynamic -o $@	
	
korgcmd.o: korgcmd.cpp
	$(CXX) -c korgcmd.cpp -O3 $(CFLAGS) -lpthread -rdynamic -o $@	
	
criticalsection.o: criticalsection.cpp
	$(CXX) -c criticalsection.cpp -O3 $(CFLAGS) -lpthread -rdynamic -o $@		

criticalsectionlock.o: criticalsectionlock.cpp
	$(CXX) -c criticalsectionlock.cpp -O3 $(CFLAGS) -lpthread -rdynamic -o $@		

korgsink.o: korgsink.cpp
	$(CXX) -c korgsink.cpp -O3 $(CFLAGS) -lpthread -rdynamic -o $@		

invoker.o: invoker.cpp
	$(CXX) -c invoker.cpp -O3 $(CFLAGS) -lpthread -rdynamic -o $@

dokorgcmd.o: dokorgcmd.cpp
	$(CXX) -c dokorgcmd.cpp -O3 $(CFLAGS) -lpthread -rdynamic -o $@	

statistics.o: statistics.cpp
	$(CXX) -c statistics.cpp -O3 $(CFLAGS) -lpthread -rdynamic -o $@	
	
socketclient.o: socketclient.cpp
	$(CXX) -c socketclient.cpp -O3 $(CFLAGS) -lpthread -rdynamic -o $@	

socketserver.o: socketserver.cpp
	$(CXX) -c socketserver.cpp -O3 $(CFLAGS) -lpthread -rdynamic -o $@	

clientconnection.o: clientconnection.cpp
	$(CXX) -c clientconnection.cpp -O3 $(CFLAGS) -lpthread -rdynamic -o $@	

proxy.o: proxy.cpp
	$(CXX) -c proxy.cpp -O3 $(CFLAGS) -lpthread -rdynamic -o $@				

iproxy.o: iproxy.cpp
	$(CXX) -c iproxy.cpp -O3 $(CFLAGS) -lpthread -rdynamic -o $@		
				
				
dodevmem2.o: dodevmem2.cpp
	$(CXX) -c dodevmem2.cpp -O3 $(CFLAGS) -lpthread -rdynamic -o $@						
				
								
				
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
	
	
	
	
	
	
	
	
	
