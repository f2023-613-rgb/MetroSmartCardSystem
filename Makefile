CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -Iinclude

TARGET = metro

SOURCES = \
	src/Card.cpp \
	src/Journey.cpp \
	src/Station.cpp \
	src/OperationCounter.cpp \
	src/GateQueue.cpp \
	src/TopUpStack.cpp \
	src/TransactionLog.cpp \
	src/JourneyHistory.cpp \
	src/CardHashTable.cpp \
	src/BlockedCardHashTable.cpp \
	src/MetroSystem.cpp \
	src/CommandProcessor.cpp \
	src/main.cpp

OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) $(TARGET).exe

run: $(TARGET)
	./$(TARGET)

commands: $(TARGET)
	./$(TARGET) data/commands.txt

performance:
	$(CXX) $(CXXFLAGS) \
	tests/performance_test.cpp \
	src/Card.cpp \
	src/Journey.cpp \
	src/JourneyHistory.cpp \
	src/OperationCounter.cpp \
	src/CardHashTable.cpp \
	-o performance_test

test:
	$(CXX) $(CXXFLAGS) \
	tests/system_test.cpp \
	src/Card.cpp \
	src/Journey.cpp \
	src/Station.cpp \
	src/OperationCounter.cpp \
	src/GateQueue.cpp \
	src/TopUpStack.cpp \
	src/TransactionLog.cpp \
	src/JourneyHistory.cpp \
	src/CardHashTable.cpp \
	src/BlockedCardHashTable.cpp \
	src/MetroSystem.cpp \
	-o system_test
	./system_test

