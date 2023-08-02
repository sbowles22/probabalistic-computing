# Basics
WARNING = -Wall -Wshadow -Wno-unused-parameter --pedantic
ERROR = -Wvla -Werror
GCC = gcc -std=c99 -g $(WARNING) $(ERROR) -fopenmp -lm
VAL = valgrind --tool=memcheck --log-file=memcheck.txt --leak-check=full --verbose

# Defines number of threads
NUM_THREADS = 4

# Dependent on program
TESTFLAGS = -DTEST_MAIN 

# EXEC_NAME is the name of the final executable
EXEC_NAME = pcomp

# TESTSUITE1 is the name of the first set of tests
EXEC_TEST_NAME = testsuites

# SRCS is the names of all source dependenies
SRCS = main.c graphs.c utils.c network.c
OBJS = $(SRCS:%.c=%.o)

#SRCS_TEST is the source dependencies for the testsuites
TESTS_C = testsuites.c graph_testsuite.c utils_testsuite.c
SRCS_TEST = $(TESTS_C) graphs.c utils.c
OBJS_TEST = $(SRCS_TEST:%.c=%.o)

$(EXEC_NAME): $(OBJS)
	$(GCC) $(TESTFLAGS) $(OBJS) -o $(EXEC_NAME)
# echo $(OBJS)

.c.o:
	$(GCC) $(TESTFLAGS) -c $*.c

testmemory: $(EXEC_NAME)
	$(VAL) ./$(EXEC_NAME) 
# testing only input3 and output3

run: $(EXEC_NAME)
	./$(EXEC_NAME)

$(EXEC_TEST_NAME): $(OBJS_TEST)
	$(GCC) $(TESTFLAGS) $(OBJS_TEST) -o $(EXEC_TEST_NAME)

run_tests: $(EXEC_TEST_NAME) $(EXEC_TEST_NAME).o
	./$(EXEC_TEST_NAME)

# TESTING PROCESS
# make testsuites.o
# make testsuites
# make run_tests
#
# Testing graohs.c is in separate file from utils.c tests
# For list of tests check .h file


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
