CXX           := g++
CXXFLAGS      := -std=c++17 -Wall -Wextra -Wpedantic -Werror

SRCDIR        := src
OBJDIR        := build
BINDIR        := bin
TESTDIR       := tests
INCDIR        := include

SRCS          := $(shell ls -R ${SRCDIR}/*.cpp 2> /dev/null)
HEADS         := $(shell ls -R ${SRCDIR}/*.hpp 2> /dev/null)
OBJS          := ${SRCS:${SRCDIR}/%.cpp=${OBJDIR}/%.o}

TEST_SRCS     := $(shell ls -R ${TESTDIR}/*.cpp 2> /dev/null)
TEST_BINDIR   := ${BINDIR}/tests
TEST_INCLUDES := -I${INCDIR} -I${SRCDIR}
TEST_BINS     := ${TEST_SRCS:${TESTDIR}/%.cpp=${TEST_BINDIR}/%}

PANDOC_OUTPUT := "Report.pdf"

EXEC          := PA5

ifeq "${DEBUG}" "1"
	COMPILE_ARGS        += -g
	LINKING_ARGS        += -g
endif
ifeq "${VERBOSE}" "1"
	COMPILE_ARGS        += -v
	LINKING_ARGS        += -v
endif

.PHONY: help clean DEBUG_MAKE
help:
	@echo "Usage: make <options> [<optional_arg>=<value>] ...}"
	@echo "options:"
	@printf "%5s %-10s\t-- %s\n" "" "all" "makes all files"
	@printf "%5s %-10s\t-- %s\n" "" "clean" "cleans this directory"
	@printf "%5s %-10s\t-- %s\n" "" "${BINDIR}/${EXEC}" "builds the executable"
	@printf "%5s %-10s\t-- %s\n" "" "report" "Runs pandoc and compiles the Report.md to the file defined by PANDOC_OUTPUT"
	@printf "%5s %-10s\t-- %s\n" "" "DEBUG_MAKE" "debugs make variables"
	@printf "%5s %-10s\t-- %s\n" "" "tests" "compiles all tests"
	@echo "optional args:"
	@printf "%5s %-10s\t-- %s\n" "" "DEBUG [0,1]" "compiles files with '-g', allowing for gdb debugging"
	@printf "%5s %-10s\t-- %s\n" "" "VERBOSE [0,1]" "enables the '-v' flag on all operations"
	@printf "%5s %-10s\t-- %s\n" "" "COMPILE_ARGS" "options to be used while COMPILING"
	@printf "%5s %-10s\t-- %s\n" "" "LINKING_ARGS" "options to be used while LINKING"
	@printf "%5s %-10s\t-- %s\n" "" "DEBUG_COMMAND" "the command to be used when debugging make variables"

tests: ${TEST_BINS}

${TEST_BINDIR}/%: ${TESTDIR}/%.cpp ${SRCDIR}/%.hpp ${SRCDIR}/%.cpp ${INCDIR}/catch.hpp
	@mkdir -pv ${TEST_BINDIR}
	@echo "Linking and Compiling $@ with ${CXXFLAGS} ${COMPILE_ARGS} ${TEST_INCLUDES}..."
	${CXX} ${CXXFLAGS} ${COMPILE_ARGS} ${TEST_INCLUDES} -o $@ $(filter %.cpp, $^)
	@echo "...Done"

report: ${PANDOC_OUTPUT}

DEBUG_MAKE:
	${DEBUG_COMMAND}

${INCDIR}/catch.hpp:
	@echo "Installing catch2 library to $@..."
	@mkdir -pv ${INCDIR}/catch/
	@curl -o $@ "https://raw.githubusercontent.com/catchorg/Catch2/v2.10.2/single_include/catch2/catch.hpp" > /dev/null
	@echo "...Done"

all: ${TEST_BINS} ${BINDIR}/${EXEC} report

${OBJDIR}/%.o: ${SRCDIR}/%.cpp
	@mkdir -pv ${OBJDIR}
	@echo "Compiling $@ with ${CXXFLAGS} ${COMPILE_ARGS}..."
	${CXX} ${CXXFLAGS} ${COMPILE_ARGS} -c -o $@ $<
	@echo "...Done"

${BINDIR}/${EXEC}: ${OBJS} ${HEADS}
	@mkdir -pv ${BINDIR}
	@echo "Linking $@ with ${CXXFLAGS} ${LINKING_ARGS}..."
	${CXX} ${CXXFLAGS} ${LINKING_ARGS} -o $@ $(filter-out %.hpp, $^)
	@echo "...Done"

${PANDOC_OUTPUT}: Report.md
	@echo "Compiling $@..."
	@pandoc -o $@ $<
	@echo "...Done"

clean:
	@echo "Cleaning Directory."
	@echo "Removing ${OBJDIR}, ${BINDIR}, ${INCDIR}..."
	@rm -rf ${OBJDIR} ${BINDIR} ${INCDIR}
	@echo "...Done"
	@echo "Removing ${PANDOC_OUTPUT}..."
	@rm -f ${PANDOC_OUTPUT}
	@echo "...Done"

