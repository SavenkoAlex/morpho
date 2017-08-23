INC_H := headers
#INC_L := so
T   = mo
CC=g++

.PHONY: all clean

all:$(T)

$(T).o: sample_sdk.cpp
	@echo === $^===
	$(CC) -I$(INC_H) -c $^ -o $(T).o

$(T): $(T).o
	@echo ====$^====
#	$(CC) $^ -Wl,rpath -L$(INC_L) -o ^@ -lmorphosdk -lstdc++ -lpthread -ldl -lgcc_s -lc
	$(CC) $^ -o $(T) -L/usr/local/lib/morphosmartsdk/ -lmorphosdk -lstdc++ -lpthread -ldl -lgcc_s -lc

clean:
	@echo rem $(T)
	rm -rf $(T)*.o
