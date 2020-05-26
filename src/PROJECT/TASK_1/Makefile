TARGET_EXEC ?= main.out

BUILD_DIR ?= ./build
SRC_DIRS ?= ./
INC_DIRS ?= ./
SRCS := $(shell find $(SRC_DIRS) -name "*.cpp")
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP -lwiringPi -Wall

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	g++ $(OBJS) -o $@ $(LDFLAGS)



# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

	
#all:

#	g++ $(CPPFLAGS) -o main.out $(SRCS)


run:
	./main.out 

.PHONY: clean

clean:
	$(RM) $(BUILD_DIR)/$(TARGET_EXEC)

-include $(DEPS)



