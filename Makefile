SRC_DIR := source
OBJ_DIR := build

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

INCLUDESPATHS := -Iheaders
LDFLAGS :=
CPPFLAGS := -g -std=c++17 -O0 -Wall
CC := g++
main: $(OBJ_FILES)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CPPFLAGS) $(CXXFLAGS) $(INCLUDESPATHS) -c -o $@ $<

clean:
	rm build/*.o main.exe