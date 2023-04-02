# Basics
WARNING = -Wall -Wshadow --pedantic
ERROR = -Wvla -Werror
GCC = gcc -std=c99 -g $(WARNING) $(ERROR) -fopenmp
VAL = valgrind --tool=memcheck --log-file=memcheck.txt --leak-check=full --verbose

# Defines number of threads
NUM_THREADS = 4

# Dependent on program
TESTFALGS = -DTEST_MAIN 

# EXEC_NAME is the name of the final executable
EXEC_NAME = pcomp
# SRCS is the names of all source dependenies
SRCS = main.c graphs.c utils.c
OBJS = $(SRCS:%.c=%.o)

$(EXEC_NAME): $(OBJS)
	$(GCC) $(TESTFALGS) $(OBJS) -o $(EXEC_NAME)
# echo $(OBJS)

.c.o:
	$(GCC) $(TESTFALGS) -c $*.c

testmemory: $(EXEC_NAME)
	$(VAL) ./$(EXEC_NAME) 
# testing only input3 and output3

test1: $(EXEC_NAME)
	./$(EXEC_NAME)

# TODO Write unit tests, 
# note that MAX_CUT of fully connected size 4 graph is 4, and of size 5 is 6
# # Tests
# testall: test1
#
# test1: test_max_cut
#   ./test_main
# 	@echo OK!
# 
# test_max_cut

# Clean compiled and outputs
clean: # remove all machine generated files
	rm -f $(EXEC_NAME) *.o output?
