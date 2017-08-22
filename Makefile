#vpath %.h headers/
INCLUDE := headers
TARGET   = mo

.PHONY : all clean

all:$(TARGET)

$(TARGET).o: $^ sample_sdk.cpp
	@echo "===================Объектный_файл========================="
	@sleep 2
	gcc -c -I$(INCLUDE) -o $(TARGET).o sample_sdk.cpp

$(TARGET): $^ $(TARGET).o
	@echo "===================Исполняемый_файл======================="
	sleep 2
	gcc -o $(TARGET) -I$(INCLUDE) $(TARGET).o  

clean:
	rm -rf $(TARGET)*.o

