TARGET   = main
CXX      = g++
INCLUDES = -I./mock
CPPFLAGS = -std=c++11
LDFLAGS  =

.PHONY: default clean test
.PRECIOUS: $(TARGET) $(OBJECTS)

ifeq ($(debug),1)
CPPFLAGS += -g -DDEBUG=1
endif

OBJECTS = main.o EspWiFi.o Arduino.o
HEADERS = $(wildcard *.h) $(wildcard ../*.h) $(wildcard mock/*.h)

default: $(TARGET)

clean:
	rm -f *.o
	rm -f $(TARGET)

test:
	./$(TARGET)

Arduino.o: mock/Arduino.cpp $(HEADERS)
	$(CXX) $(CPPFLAGS) $(INCLUDES) -c $< -o $@

EspWiFi.o: ../EspWiFi.cpp $(HEADERS)
	$(CXX) $(CPPFLAGS) $(INCLUDES) -c $< -o $@

main.o: main.cpp $(HEADERS)
	$(CXX) $(CPPFLAGS) $(INCLUDES) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)
