CXX = g++

# C preprocessor flags. These are the only flags used in automatic dependency
# generation. Do not confuse with C++ compiler flags (CXXFLAGS).
CPPFLAGS = \
	-std=c++14 \
	-I../lib/json \

CXXFLAGS = \
	-g \

LDFLAGS = \
	-g \
	-lSDL2 \
	-lSDL2_image \
	-lSDL2_ttf \

sources = \
	client/app.cpp \
	client/client_game.cpp \
	client/exceptions.cpp \
	client/field.cpp \
	client/geometry.cpp \
	client/gui.cpp \
	client/log.cpp \
	client/resources.cpp \
	client/sdl_wrapper.cpp \
	client/view.cpp \
	client/window.cpp \
	core/Cell.cpp \
	core/ChangeMap.cpp \
	core/GameState.cpp \
	core/JackalGame.cpp \
	core/Point.cpp \
	core/TimeState.cpp \
	main.cpp \

objects = $(sources:.cpp=.o)

all : jackal

jackal : $(objects)
	$(CXX) -o $@ $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $^

# Automatic scanning of C++ sources for #include's:
#   https://www.gnu.org/software/make/manual/make.html#Automatic-Prerequisites
%.d : %.cpp
	@set -e; rm -f $@; \
	 $(CXX) -M $(CPPFLAGS) $< > $@.$$$$; \
	 sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	 rm -f $@.$$$$

include $(sources:.cpp=.d)

.PHONY : clean
clean :
	find . \( -name '*.d' -o -name '*.o' \) \
		-exec sh -c 'echo removing {}; rm {}' \;
