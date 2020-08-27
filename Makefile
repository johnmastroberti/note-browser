CXX=g++
CXXFLAGS=-Iinclude -Wall -Wextra -Wpedantic -Wshadow -std=c++20
DEBUG_FLAGS=-g
RELEASE_FLAGS=-O3 -flto
LD=g++
LDFLAGS=-lncurses
sources=$(shell ls src/*.cpp)
objects=$(sources:src/%.cpp=obj/%.o)
objects_d=$(sources:src/%.cpp=objd/%.o)

.EXTRA_PREREQS = Makefile

.PHONY: default
default: release

.PHONY: release
release: app

app: $(objects)
	$(LD) -o $@ $^ $(RELEASE_FLAGS) $(LDFLAGS)

obj/%.o: src/%.cpp
	$(CXX) -o $@ $(RELEASE_FLAGS) $(CXXFLAGS) -c $^

.PHONY: debug
debug: appd

appd: $(objects_d)
	$(LD) -o $@ $^ $(DEBUG_FLAGS) $(LDFLAGS)

objd/%.o: src/%.cpp
	$(CXX) -o $@ $(DEBUG_FLAGS) $(CXXFLAGS) -c $^
