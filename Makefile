CXX           := g++
CXXFLAGS      := -std=c++17 -Wall -Wextra -Wpedantic -Werror

SRCDIR        := src
OBJDIR        := build
BINDIR        := bin

SRCS          := $(shell ls -R ${SRCDIR}/*.cpp 2> /dev/null)
HEADS         := $(shell ls -R ${SRCDIR}/*.hpp 2> /dev/null)
OBJS          := ${SRCS:${SRCDIR}/%.cpp=${OBJDIR}/%.o}

PANDOC_OUTPUT := "Report.pdf"

EXEC          := PA5

# TODO: Add tests

.PHONY: help clean DEBUG_MAKE run-test
help:
	@echo "Usage: make <options> [<optional_arg>=<value>] ...}"
	@echo "options:"
	@printf "%5s %-10s\t-- %s\n" "" "all" "makes all files"
	@printf "%5s %-10s\t-- %s\n" "" "clean" "cleans this directory"
	@printf "%5s %-10s\t-- %s\n" "" "${BINDIR}/${EXEC}" "builds the executable"
	@printf "%5s %-10s\t-- %s\n" "" "report" "Runs pandoc and compiles the Report.md to the file defined by PANDOC_OUTPUT"
	@printf "%5s %-10s\t-- %s\n" "" "DEBUG_MAKE" "debugs make variables"
	@echo "optional args:"
	@printf "%5s %-10s\t-- %s\n" "" "DEBUG [0,1]" "compiles files with '-g', allowing for gdb debugging"
	@printf "%5s %-10s\t-- %s\n" "" "VERBOSE [0,1]" "enables the '-v' flag on all operations"
	@printf "%5s %-10s\t-- %s\n" "" "COMPILE_ARGS" "options to be used while COMPILING"
	@printf "%5s %-10s\t-- %s\n" "" "LINKING_ARGS" "options to be used while LINKING"
	@printf "%5s %-10s\t-- %s\n" "" "DEBUG_COMMAND" "the command to be used when debugging make variables"

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

