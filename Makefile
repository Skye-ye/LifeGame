CXX = g++
CXXFLAGS = -Wall -std=c++2b -O2 -I include/SDL2
LDFLAGS = -L lib -lSDL2-2.0.0 -lSDL2_image-2.0.0 -lSDL2_ttf-2.0.0
SCRIPTS = ./scripts
SRCS = $(wildcard $(SCRIPTS)/*.cpp)
OBJS = $(SRCS:$(SCRIPTS)/%.cpp=%.o)

edit : $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o LifeGame $(LDFLAGS)

%.o: $(SCRIPTS)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY : cleanall cleanobj cleanexe

cleanall : cleanobj cleanexe

cleanobj :
	rm -f $(OBJS)

cleanexe :
	rm -f LifeGame
