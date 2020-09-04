CXX=g++
CXXFLAGS=-Iinclude -Wall -Wextra -Wpedantic -Wshadow -Wno-narrowing -Wno-sign-compare -Werror -std=c++20
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

obj/notes.o: src/notes.cpp include/notes.hpp
objd/notes.o: src/notes.cpp include/notes.hpp
obj/menu.o: src/menu.cpp include/menu.hpp include/window.hpp include/coords.hpp
objd/menu.o: src/menu.cpp include/menu.hpp include/window.hpp include/coords.hpp
obj/window.o: src/window.cpp include/window.hpp include/coords.hpp
objd/window.o: src/window.cpp include/window.hpp include/coords.hpp
obj/main.o: src/main.cpp include/window.hpp include/coords.hpp include/session.hpp include/menu.hpp include/notes.hpp include/view.hpp
objd/main.o: src/main.cpp include/window.hpp include/coords.hpp include/session.hpp include/menu.hpp include/notes.hpp include/view.hpp
obj/coords.o: src/coords.cpp include/coords.hpp
objd/coords.o: src/coords.cpp include/coords.hpp
obj/view.o: src/view.cpp include/view.hpp include/window.hpp include/coords.hpp include/menu.hpp include/notes.hpp include/session.hpp
objd/view.o: src/view.cpp include/view.hpp include/window.hpp include/coords.hpp include/menu.hpp include/notes.hpp include/session.hpp
obj/session.o: src/session.cpp include/session.hpp
objd/session.o: src/session.cpp include/session.hpp
