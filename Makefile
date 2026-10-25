OBJS1	= Point.o min_Point.o typical_functions.o common_functions.o cube.o main_cube.o
OBJS2	= Point.o min_Point.o typical_functions.o common_functions.o lsh.o main_lsh.o
OBJS3	= Point.o typical_functions.o common_functions.o cluster_functions.o cluster_main.o
OUT1	= cube
OUT2	= lsh
OUT3	= cluster
CC		= g++
FLAGS   = -std=c++11 -g -c -O3
# -g option enables debugging mode 
# -c flag generates object code for separate files

cube: $(OBJS1)
	$(CC) -g -o $(OUT1) $(OBJS1)

lsh: $(OBJS2)
	$(CC) -g -o $(OUT2) $(OBJS2)

cluster: $(OBJS3)
	$(CC) -g -o $(OUT3) $(OBJS3)

# create/compile the individual files >>separately<<
Point.o: Point.cpp Point.hpp
	$(CC) $(FLAGS) Point.cpp

min_Point.o: min_Point.cpp min_Point.hpp
	$(CC) $(FLAGS) min_Point.cpp

typical_functions.o: typical_functions.cpp typical_functions.hpp Point.hpp
	$(CC) $(FLAGS) typical_functions.cpp

common_functions.o: common_functions.cpp common_functions.hpp Point.hpp
	$(CC) $(FLAGS) common_functions.cpp	

cube.o: cube.cpp cube.hpp
	$(CC) $(FLAGS) cube.cpp

main_cube.o: main_cube.cpp cube.hpp
	$(CC) $(FLAGS) main_cube.cpp

lsh.o: lsh.cpp lsh.hpp
	$(CC) $(FLAGS) lsh.cpp

main_lsh.o: main_lsh.cpp lsh.hpp
	$(CC) $(FLAGS) main_lsh.cpp

cluster_functions: cluster_functions.cpp cluster_functions.hpp
	$(CC) $(FLAGS) cluster_functions.cpp

cluster_main: cluster_main.cpp cluster_functions.hpp
	$(CC) $(FLAGS) cluster_main.cpp

# clean house
clean:
	rm -f $(OBJS1) $(OUT1)
	rm -f $(OBJS2) $(OUT2)
	rm -f $(OBJS3) $(OUT3)
