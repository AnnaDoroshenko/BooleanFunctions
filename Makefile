# The name of the main file and executable
mainFileName = main
# Files that have .h and .cpp versions
classFiles = Function
# Files that only have the .h version
justHeaderFiles =
# Compilation flags
OPTIMIZATION_FLAG = -O2
LANGUAGE_LEVEL = -std=c++17
COMPILER_FLAGS = -Wall -Wextra -Wno-unused-parameter -pthread
LINKER_FLAGS =


# Auxiliary
filesObj = $(addsuffix .o, $(mainFileName) $(classFiles))
filesH = $(addsuffix .h, $(classFiles) $(justHeaderFiles))


all: cleanExe $(mainFileName)


# Compiler
%.o: %.cpp $(filesH)
	g++ $(COMPILER_FLAGS) $(OPTIMIZATION_FLAG) $(LANGUAGE_LEVEL) -c $<

# Linker
$(mainFileName): $(filesObj)
	g++ $(COMPILER_FLAGS) $(OPTIMIZATION_FLAG) $(LANGUAGE_LEVEL) $(LINKER_FLAGS) $^ -o $@


# Utils
clean:
	rm -f *.o $(mainFileName)

cleanExe:
	rm -f $(mainFileName)
