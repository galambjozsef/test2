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

test: test.o myclass.o
	$(CXX) test.o myclass.o -D_GNU_SOURCE -o $(BUILD_DIR)/$(PROG_NAME) $(CFLAGS)

myclass.o: myclass.cpp
	$(CXX) -c myclass.cpp -O3 $(CFLAGS) -o $@


test.o: test.cpp
	$(CXX) -c test.cpp -O3 $(CFLAGS) -o $@
	
clean:
	rm -f test.o
	rm -f myclass.o
	rm -f $(BUILD_DIR)/$(PROG_NAME)

#install:
#	cp $(BUILD_DIR)/$(PROG_NAME) /data/development/lfo/target/build/andoria/rootfs/full/
	
	
	
	
	
	
	
	
	
