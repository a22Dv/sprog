TARGET = generator

CXX = clang++
CXXFLAGS = -std=c++20 -stdlib=libc++ -g -O0 -Iinclude 
LLVM_LIB_PATH = /opt/llvm/lib/x86_64-unknown-linux-gnu
LDFLAGS = -L$(LLVM_LIB_PATH) -rpath $(LLVM_LIB_PATH)

all: ./src/main.cpp ./src/lexer.cpp 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) ./src/main.cpp ./src/lexer.cpp  -o $(TARGET)
clean:
	rm $(TARGET)