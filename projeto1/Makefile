FLEX = flex
BISON = bison
CC = clang++
CFLAGS = -std=c++11 -fcxx-exceptions -Wno-deprecated-register -Wall
EXECUTABLE = lukasiewicz

SRC_DIR = src
HEADERS_DIR = include
SRC_FILES += $(wildcard $(SRC_DIR)/*.cpp)
OBJS += $(SRC_FILES:.cpp=.o)

$(EXECUTABLE): parser scanner $(SRC_DIR)/Main.cpp
	@echo 'Creating executable...'
	${CC} -o ${EXECUTABLE} $(SRC_DIR)/*.cpp ${CFLAGS} -I$(HEADERS_DIR)
	@echo 'Creating executable... Done!'
	@echo ''

scanner: $(SRC_DIR)/Scanner.l parser
	@echo 'Generating lexical analyzer...'
	${FLEX} -o $(SRC_DIR)/Scanner.cpp $(SRC_DIR)/Scanner.l 
	@echo 'Generating lexical analyzer... Done!'
	@echo ''

parser: $(SRC_DIR)/Parser.y
	@echo 'Generating bottom-up parser...'
	${BISON} --defines=$(HEADERS_DIR)/Parser.h --output=$(SRC_DIR)/Parser.cpp $(SRC_DIR)/Parser.y
	@echo 'Generating bottom-up parser... Done!'
	@echo ''

all: clean $(EXECUTABLE) test
    
test:
	@echo 'Executing tests...'
	@echo ''
	@echo "     Ops... there's nothing here!"
	@echo ''
	@echo 'Executing tests... Done!'
	@echo ''

clean:
	@echo 'Cleaning project...'
	-@rm $(SRC_DIR)/Parser.cpp $(HEADERS_DIR)/Parser.h $(SRC_DIR)/Scanner.cpp ${EXECUTABLE}
	@echo 'Cleaning project... Done!'
	@echo ''
