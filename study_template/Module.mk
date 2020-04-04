PWD = study_template

CFLAGS   = $(COMMON_FLAG) $(COMMON_HEADE) -I./study_template
CXXFLAGS = $(COMMON_FLAG) $(COMMON_HEADE) -I./study_template

SOURCESCPP = $(COMMON_SORCE) $(PWD)/main.cpp $(wildcard $(PWD)/*.cpp)
OBJECTSCPP = $(SOURCESCPP:.cpp=.o)

TARGET := template

$(TARGET):$(OBJECTSCPP) $(OBJECTSC)
	$(CXX) $(CXXFLAGS) -o $@ $^