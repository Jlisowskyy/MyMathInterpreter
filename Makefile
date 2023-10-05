.PHONY: clean all cleanAll

BUILD_DIR=makebuild/
TARGET=main

FILES= ${BUILD_DIR}main.o ${BUILD_DIR}ParserUnit.o ${BUILD_DIR}LexerUnit.o ${BUILD_DIR}InterpretingUnit.o ${BUILD_DIR}AST.o ${BUILD_DIR}operatorsProc.o
COMPILER=g++
C_FLAGS=-O3 -Wall -pedantic -std=c++20 -march=native -Wextra
L_FLAGS=

all: ${TARGET}
clean:
	-rm -f ${FILES}
cleanAll: clean
	-rm -f ${TARGET}
${TARGET}: ${FILES}
	${COMPILER} -o ${TARGET} ${FILES} ${L_FLAGS}

${BUILD_DIR}main.o: main.cpp include/ParserUnit.h ${BUILD_DIR}
	${COMPILER} -c -o ${BUILD_DIR}main.o main.cpp ${C_FLAGS}

${BUILD_DIR}ParserUnit.o: include/ParserUnit.h include/errors.h include/globalValues.h include/LexerUnit.h include/InterpretingUnit.h src/ParserUnit.cpp ${BUILD_DIR}
	${COMPILER} -c -o ${BUILD_DIR}ParserUnit.o src/ParserUnit.cpp ${C_FLAGS}

${BUILD_DIR}LexerUnit.o: include/LexerUnit.h include/Token.h include/globalValues.h include/errors.h src/LexerUnit.cpp ${BUILD_DIR}
	${COMPILER} -c -o ${BUILD_DIR}LexerUnit.o src/LexerUnit.cpp ${C_FLAGS}

${BUILD_DIR}InterpretingUnit.o: include/InterpretingUnit.h include/Token.h include/AST.h include/StdBasedHashTable.h include/operatorsProc.h src/InterpretingUnit.cpp ${BUILD_DIR}
	${COMPILER} -c -o ${BUILD_DIR}InterpretingUnit.o src/InterpretingUnit.cpp ${C_FLAGS}

${BUILD_DIR}AST.o: include/AST.h include/operatorsProc.h include/globalValues.h src/AST.cpp ${BUILD_DIR}
	${COMPILER} -c -o ${BUILD_DIR}AST.o src/AST.cpp ${C_FLAGS}

${BUILD_DIR}operatorsProc.o: include/operatorsProc.h include/globalValues.h include/Token.h src/operatorsProc.cpp ${BUILD_DIR}
	${COMPILER} -c -o ${BUILD_DIR}operatorsProc.o src/operatorsProc.cpp ${C_FLAGS}

${BUILD_DIR}:
	-mkdir ${BUILD_DIR}
