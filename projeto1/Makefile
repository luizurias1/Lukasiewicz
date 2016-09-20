FLEX = flex
BISON = bison
CC = clang++
CFLAGS = -std=c++11 -fcxx-exceptions -Wno-deprecated-register -Wall
EXECUTABLE = lukacompiler
TEST_EXECUTABLE = test/test.sh

# Directories and Files
SRC_DIR = src
HEADERS_DIR = include
SRC_FILES += $(wildcard $(SRC_DIR)/*.cpp)
HEADER_FILES += $(wildcard $(HEADERS_DIR)/*.h)

# Bison and Flex Files
PARSER_HEADER = $(HEADERS_DIR)/Parser.h
PARSER_CPP = tmp/Parser.cpp
SCANNER_CPP = tmp/Scanner.cpp

# Colors
GREEN=\033[0;32m
LIGHT_GREEN=\033[1;32m
WHITE=\033[0;37m

$(EXECUTABLE): tmp parser scanner $(SRC_FILES) $(PARSER_CPP) $(SCANNER_CPP)
	@echo "${GREEN}Creating executable...${WHITE}"
	${CC} -o ${EXECUTABLE} $(SRC_FILES) $(PARSER_CPP) $(SCANNER_CPP) ${CFLAGS} -I$(HEADERS_DIR)
	@echo "${GREEN}Creating executable... ${LIGHT_GREEN}Done!${WHITE}"
	@echo ''

scanner: $(SRC_DIR)/Scanner.l parser
	@echo "${GREEN}Generating lexical analyzer...${WHITE}"
	${FLEX} -o $(SCANNER_CPP) $(SRC_DIR)/Scanner.l 
	@echo "${GREEN}Generating lexical analyzer... ${LIGHT_GREEN}Done!${WHITE}"
	@echo ''

parser: $(SRC_DIR)/Parser.y
	@echo "${GREEN}Generating bottom-up parser...${WHITE}"
	${BISON} --defines=$(PARSER_HEADER) --output=$(PARSER_CPP) $(SRC_DIR)/Parser.y
	@echo "${GREEN}Generating bottom-up parser... ${LIGHT_GREEN}Done!${WHITE}"
	@echo ''

all: clean $(EXECUTABLE) test
    
test: $(EXECUTABLE)
	@echo "${GREEN}Executing tests...${WHITE}"
	@bash ./$(TEST_EXECUTABLE)
	@echo "${GREEN}Executing tests... ${LIGHT_GREEN}Done!${WHITE}"
	@echo ''

clean:
	@echo "${GREEN}Cleaning project...${WHITE}"
	-@rm -Rf tmp
	-@rm $(PARSER_HEADER) ${EXECUTABLE}
	@echo "${GREEN}Cleaning project... ${LIGHT_GREEN}Done!${WHITE}"
	@echo ''

tmp:
	@echo "${GREEN}Creating tmp directory...${WHITE}"
	@mkdir tmp
	@echo "${GREEN}Creating tmp directory... ${LIGHT_GREEN}Done!${WHITE}"
	@echo ''