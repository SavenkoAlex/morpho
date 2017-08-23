INC_H := headers/
INC_L := lib/
T   = mo
CC=g++

#all:$(T)

$(T): $(T).o
	@echo ========EXEC============
	$(CC) $^  -Wl,rpath,$(INC_L) -o $@

$(T).o: sample_sdk.cpp
	@echo ========Object==========
	$(CC) -I$(INC_H) -c sample_sdk.cpp -o mo.o

clean:
	rm -rf $(T)*.o
