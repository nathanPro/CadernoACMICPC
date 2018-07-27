CXX=clang++
CXXFLAGS = -g -O2 -fsanitize=address,undefined -fno-omit-frame-pointer
fuzzflags = -fsanitize=fuzzer

test:
	$(CXX) $(CXXFLAGS) $(fuzzflags) code.cpp -o fuzz
