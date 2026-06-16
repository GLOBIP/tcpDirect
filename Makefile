SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)
FLAGS = -g -I $(HOME)/boostasio/boost_1_82_0
COMPILER = g++

%.o: %.cpp
	$(COMPILER) $(FLAGS) -c $< -o $@
all: ${OBJS}
	${COMPILER} ${OBJS} -o main ${FLAGS}


run: all
	./main

.PHONY : clean
clean :
	-rm -f *.o $(OBJS)
