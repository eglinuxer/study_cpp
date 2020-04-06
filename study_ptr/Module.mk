PWD = study_ptr

CFLAGS   = $(COMMON_FLAG) $(COMMON_HEADE) -I./study_ptr
CXXFLAGS = $(COMMON_FLAG) $(COMMON_HEADE) -I./study_ptr

SOURCESCPP = $(COMMON_SORCE) $(PWD)/main.cpp $(wildcard $(PWD)/*.cpp)
OBJECTSCPP = $(SOURCESCPP:.cpp=.o)

TARGET := ptr

$(TARGET):$(OBJECTSCPP) $(OBJECTSC)
	$(CXX) $(CXXFLAGS) -o $@ $^