# -MMD -MP instruct g++ to automatically generate a .d file alongside each .o 
# file. That .d file lists exactly which headers that specific .cpp includes.
# 'make' then reads those .d files to know the true dependencies.
CPPFLAGS = -std=c++20 -Wall -Werror -pedantic -ggdb -pthread -MMD -MP 
OBJS = Terminal.o PixelMatrix.o RGB.o Object.o BouncingBall.o SpaceShip.o Manager.o 

all : P1.bin

%.o : %.cpp
	g++ $(CPPFLAGS) -c $< -o $@

P1.bin : $(OBJS) P1.o
	g++ -lncurses -lpthread $^ -o $@
	
	
# For linux checking memory leak
valgrind:
	valgrind -s --leak-check=full ./P1.bin

# For mac checking memory leak
check: all
	leaks --atExit -- ./P1.bin

clean :
	rm -f *.o *.bin *.d

# Tell `make` to actually read and use those .d files on the next incremental 
# build(already has .o and .d)
# The leading '-' before 'include' tells 'make' to silently ignore missing .d 
# files, which is important because on a fresh build they don't exist yet.(for 
# first ever build)
ALL_OBJS = $(OBJS) P1.o
-include $(ALL_OBJS:.o=.d)
# Other explanation
# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want those
# errors to show up.