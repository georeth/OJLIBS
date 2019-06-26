LDLIBES = -lm
CXX = clang++
CXXFLAGS = -I${HOME}/usr/include -std=c++14 -DLOCAL_JUDGE -Wall -Wpedantic -Werror -Wno-unused-variable -Wno-unused-function -Wshorten-64-to-32 -g

prog: main.cpp
	$(CXX) $(CXXFLAGS) $^ $(LOADLIBES) $(LDLIBES) -o $@
check: prog test
	./check.sh
run: prog
	./prog
