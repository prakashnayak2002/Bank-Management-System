SRC := $(wildcard *.cpp)
OBJ := $(patsubst %.cpp , %.opp , $(SRC))
cal.out : $(OBJ) 
	g++ -o $@ $^
clean :
	rm *.o