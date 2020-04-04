
.PHONY: all clean

CC = gcc
CXX = g++

COMMON_FLAG  = -ggdb -std=c++11 -Wall -I./common
COMMON_HEADE = 
COMMON_SORCE = 

SOURCESC   = 
OBJECTSC   = $(SOURCESC:.c=.o)


#all: 
all: template

#SRCDIRS	:= 
SRCDIRS	:= study_template

include $(SRCDIRS:%=%/Module.mk)

.cpp.o:
	$(CXX) $(CFLAGS) $< -c -o $@
.c.o:
	$(CXX) $(CFLAGS) $< -c -o $@

clean:
	rm -rf $(SRCDIRS:%=%/*.o)
	rm -rf ./*.o $(SRCDIRS)/*.o
	rm -rf template
