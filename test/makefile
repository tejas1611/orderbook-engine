CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

BOOST_LIB = -L /usr/lib
BOOST_INCLUDE = -I /usr/include
BOOST_UNIT_TEST_LIB = -lboost_unit_test_framework

INCLUDE_DIR = ../include
SRC_DIR = ../src
OBJ_DIR = ../obj
TEST_DIR = .

HEADERS = $(wildcard $(INCLUDE_DIR)/*.hpp)

OBJS = $(OBJ_DIR)/OrderBook.o $(OBJ_DIR)/Party.o $(TEST_DIR)/maintest.o
TARGET = test

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(BOOST_LIB) $(BOOST_UNIT_TEST_LIB)

$(OBJ_DIR)/OrderBook.o: $(SRC_DIR)/OrderBook.cpp $(SRC_DIR)/Party.cpp $(INCLUDE_DIR)/OrderBook.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(BOOST_INCLUDE)

$(OBJ_DIR)/Party.o: $(SRC_DIR)/Party.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(BOOST_INCLUDE)

$(TEST_DIR)/maintest.o: $(TEST_DIR)/maintest.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(BOOST_INCLUDE)

clean:
	$(RM) $(OBJS) $(TARGET)