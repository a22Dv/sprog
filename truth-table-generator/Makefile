TARGET = generator

CXX = clang++
CXXFLAGS = -std=c++20 -stdlib=libc++ -g -O0
LLVM_LIB_PATH = /opt/llvm/lib/x86_64-unknown-linux-gnu
LDFLAGS = -L$(LLVM_LIB_PATH) -rpath $(LLVM_LIB_PATH)

all: generator.cpp generator.hpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET) generator.cpp
clean:
	rm $(TARGET)