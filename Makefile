CC = gcc
CXX = g++

INC_PATH := -Iinc/ -Iinc/linux
CFLAGS = $(INC_PATH)

OSTYPE = $(shell uname)
ifneq (${OSTYPE}, Linux)
CFLAGS += -DHOLYBSD
else
CFLAGS += -DOS_LINUX -DFN_DEBUG
endif

CPP_DIRS := lib/linux app
CPP_SRCS := $(wildcard *.cpp) $(wildcard $(addsuffix /*.cpp, $(CPP_DIRS)))
CPP_OBJS := $(patsubst %.cpp, %.o, $(CPP_SRCS))

APP_DIRS := app/
APP_SRCS := $(wildcard *.cpp) $(wildcard $(addsuffix /*.cpp, $(APP_DIRS)))
APP_OBJS := $(patsubst %.cpp, %.o, $(APP_SRCS))
APP_BINS := $(patsubst %.cpp, %.bin, $(APP_SRCS))

EXECUTABLE := $(APP_BINS)

$(EXECUTABLE) : $(CPP_OBJS)
	$(CXX) -o $(EXECUTABLE) $(CPP_OBJS) $(CFLAGS)


all: $(EXECUTABLE)

clean:
	-rm -f $(shell find . -name "*.[o]")
	-rm -f *.bin

prepare:
	-find . -name "*.cpp" | xargs  astyle --style=allman
	-find . -name "*.h" | xargs  astyle --style=allman
	-find . -name "*.cpp" | xargs dos2unix
	-find . -name "*.h" | xargs dos2unix

finish:
	-find . -name "*.cpp" | xargs dos2unix
	-find . -name "*.h" | xargs dos2unix
	-find . -name "*.cpp" | xargs chmod a+w
	-find . -name "*.h" | xargs chmod a+w

