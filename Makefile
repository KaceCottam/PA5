CXX                   =  g++
CXXFLAGS              =  -std=c++14 -Wall -Wextra -Wpedantic -Werror

SRCDIR                =  src
OBJDIR                =  build
BINDIR                =  bin

SRCS                  := $(shell ls -R ${SRCDIR}/*.cpp 2> /dev/null)
HEADS                 := $(shell ls -R ${SRCDIR}/*.hpp 2> /dev/null)
OBJS                  := ${SRCS:${SRCDIR}/%.cpp=${OBJDIR}/%.o}

PANDOC_OUTPUT         := "Report.pdf"

EXEC                  := PA5

.PHONY: help clean DEBUG_MAKE run-test
help:
	@echo "Usage: make <options> [<optional_arg>=<value>] ...}"
	@echo "options:"
	@echo "\tall\t\t-- makes all files"
	@echo "\tclean\t\t-- cleans this directory"
	@echo "\t${BINDIR}/${EXEC}\t\t-- builds the executable"
	@echo "\treport\t\t-- Runs pandoc and compiles the Report.md to the file defined by PANDOC_OUTPUT"
	@echo "\tDEBUG_MAKE\t-- debugs make variables"
	@echo "optional args:"
	@echo "\tDEBUG [0,1]\t-- compiles files with '-g', allowing for gdb debugging"
	@echo "\tVERBOSE [0,1]\t-- enables the '-v' flag on all operations"
	@echo "\tCOMPILE_ARGS\t-- options to be used while COMPILING"
	@echo "\tLINKING_ARGS\t-- options to be used while LINKING"
	@echo "\tPANDOC_OUTPUT\t-- the output test file. default: Report.pdf"
	@echo "\tDEBUG_COMMAND\t-- the command to be used when debugging make variables"


report: ${PANDOC_OUTPUT}

DEBUG_MAKE:
	${DEBUG_COMMAND}

all: ${BINDIR}/${EXEC} report

${OBJDIR}/%.o: ${SRCDIR}/%.cpp
	@mkdir -pv ${OBJDIR}
	@echo "Compiling $@ with ${CXXFLAGS} ${COMPILE_ARGS}..."
	@${CXX} ${CXXFLAGS} ${COMPILE_ARGS} -c -o $@ $<
	@echo "...Done"

${BINDIR}/${EXEC}: ${OBJS} ${HEADS}
	@mkdir -pv ${BINDIR}
	@echo "Compiling $@ with ${CXXFLAGS} ${LINKING_ARGS}..."
	@${CXX} ${CXXFLAGS} ${LINKING_ARGS} -o $@ $^
	@echo "...Done"

${PANDOC_OUTPUT}: Report.md
	@echo "Compiling $@..."
	@pandoc -o $@ $<
	@echo "...Done"

clean:
	@echo "Cleaning Directory."
	@echo "Removing ${OBJDIR}, ${BINDIR}..."
	@rm -rf ${OBJDIR} ${BINDIR}
	@echo "...Done"
	@echo "Removing ${PANDOC_OUTPUT}..."
	@rm -f ${PANDOC_OUTPUT}
	@echo "...Done"

