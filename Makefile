CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall

SRC = src/main.cpp \
      src/RandomStreamGen.cpp \
      src/HashFuncGen.cpp \
      src/ExactCounter.cpp \
      src/HyperLogLog.cpp \
      src/HyperLogLogImproved.cpp

TARGET = hll_app

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET) results/results.csv
