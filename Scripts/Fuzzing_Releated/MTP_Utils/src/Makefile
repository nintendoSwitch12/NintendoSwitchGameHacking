CC = g++
CXXFLAGS = -Wall -O3
INCLUDES = -I/usr/libmtp/include
LDFLAGS = -L/usr/libmtp/lib -lmtp

SRC_DIR = ./src
OBJ_DIR = ./obj

TARGET = ./bin/main

HPP_SRCS = $(notdir $(wildcard $(SRC_DIR)/*.hpp)) 
CPP_SRCS = $(notdir $(wildcard $(SRC_DIR)/*.cpp))
GCHS = $(HPP_SRCS:=.gch) 
OBJS = $(CPP_SRCS:.cpp=.o)

PRECOMPILED_HEADERS = $(patsubst %.hpp.gch,$(OBJ_DIR)/%.hpp.gch,$(GCHS)) 
OBJECTS = $(patsubst %.o,$(OBJ_DIR)/%.o,$(OBJS)) 
DEPS = $(OBJECTS:.o=.d) $(PRECOMPILED_HEADERS:.gch=.d)

all : $(TARGET)

# $(OBJ_DIR)/%.hpp.gch : $(SRC_DIR)/%.hpp
# 	$(CC) $(CXXFLAGS) $(INCLUDES) -c $< -o $@ -MD $(LDFLAGS) 

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CC) $(CXXFLAGS) $(INCLUDES) -c $< -o $@ -MD $(LDFLAGS) 

# $(TARGET) : $(PRECOMPILED_HEADERS) $(OBJECTS)
# 	$(CC) $(CXXFLAGS) $(OBJECTS) $(INCLUDES) -o $(TARGET) $(LDFLAGS)

$(TARGET) : $(OBJECTS)
	$(CC) $(CXXFLAGS) $(OBJECTS) $(INCLUDES) -o $(TARGET) $(LDFLAGS)

.PHONY : clean all
clean:
	rm -f $(OBJECTS) $(PRECOMPILED_HEADERS) $(DEPS) $(TARGET)

-include $(DEPS)
