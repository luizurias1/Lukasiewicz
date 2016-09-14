FLEX = flex
BISON = bison
CC = clang++
CFLAGS = -std=c++11 -fcxx-exceptions -Wno-deprecated-register -Wall
EXECUTABLE = lukasiewicz
TEST_EXECUTABLE = test/test.sh

SRC_DIR = src
HEADERS_DIR = include
SRC_FILES += $(wildcard $(SRC_DIR)/*.cpp)
HEADER_FILES += $(wildcard $(HEADERS_DIR)/*.h)

PARSER_HEADER = $(HEADERS_DIR)/Parser.h
PARSER_CPP = tmp/Parser.cpp
SCANNER_CPP = tmp/Scanner.cpp


$(EXECUTABLE): tmp parser scanner $(SRC_FILES) $(PARSER_CPP) $(SCANNER_CPP)
	@echo 'Creating executable...'
	${CC} -o ${EXECUTABLE} $(SRC_FILES) $(PARSER_CPP) $(SCANNER_CPP) ${CFLAGS} -I$(HEADERS_DIR)
	@echo 'Creating executable... Done!'
	@echo ''

scanner: $(SRC_DIR)/Scanner.l parser
	@echo 'Generating lexical analyzer...'
	${FLEX} -o $(SCANNER_CPP) $(SRC_DIR)/Scanner.l 
	@echo 'Generating lexical analyzer... Done!'
	@echo ''

parser: $(SRC_DIR)/Parser.y
	@echo 'Generating bottom-up parser...'
	${BISON} --defines=$(PARSER_HEADER) --output=$(PARSER_CPP) $(SRC_DIR)/Parser.y
	@echo 'Generating bottom-up parser... Done!'
	@echo ''

all: clean $(EXECUTABLE) test
    
test: $(EXECUTABLE)
	@echo 'Executing tests...'
	bash ./$(TEST_EXECUTABLE)
	@echo 'Executing tests... Done!'
	@echo ''

clean:
	@echo 'Cleaning project...'
	-@rm -Rf tmp
	-@rm $(PARSER_HEADER) ${EXECUTABLE}
	@echo 'Cleaning project... Done!'
	@echo ''

tmp:
	@mkdir tmp