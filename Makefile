CXX=g++
CXXFLAGS=-Iinclude -Wall -Wextra -Wpedantic -Wshadow -Werror -std=c++20
DEBUG_FLAGS=-g
RELEASE_FLAGS=-O3 -flto
LD=g++
LDFLAGS=-lncurses -lmenu
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
	$(CXX) -o $@ $(RELEASE_FLAGS) $(CXXFLAGS) -c $<

.PHONY: debug
debug: appd

appd: $(objects_d)
	$(LD) -o $@ $^ $(DEBUG_FLAGS) $(LDFLAGS)

objd/%.o: src/%.cpp
	$(CXX) -o $@ $(DEBUG_FLAGS) $(CXXFLAGS) -c $<

.PHONY: clean
clean:
	-rm -f obj/*
	-rm -f objd/*
	-rm app
	-rm appd

obj/menu.o: src/menu.cpp include/menu.hpp include/window.hpp include/coords.hpp
objd/menu.o: src/menu.cpp include/menu.hpp include/window.hpp include/coords.hpp
obj/window.o: src/window.cpp include/window.hpp include/coords.hpp
objd/window.o: src/window.cpp include/window.hpp include/coords.hpp
obj/main.o: src/main.cpp include/window.hpp include/coords.hpp include/session.hpp include/menu.hpp
objd/main.o: src/main.cpp include/window.hpp include/coords.hpp include/session.hpp include/menu.hpp
obj/coords.o: src/coords.cpp include/coords.hpp
objd/coords.o: src/coords.cpp include/coords.hpp
obj/session.o: src/session.cpp include/session.hpp
objd/session.o: src/session.cpp include/session.hpp
