CXX = g++
MKDIR = mkdir -p
INCLUDE = include
PARAM = -I ${INCLUDE} -lpthread -lboost_system

CHAT = chat
CLIENT = client
SERVER = server

SRC = src
SRC_CHAT = ${SRC}/${CHAT}
SRC_CLIENT = ${SRC}/${CLIENT}
SRC_SERVER = ${SRC}/${SERVER}

BIN = bin
BIN_CHAT = ${BIN}/${CHAT}
BIN_CHAT_PATHS = ${BIN} ${BIN_CHAT}
BIN_CHAT_OUT_FILES = ${BIN_CHAT}/Connection.o \
                     ${BIN_CHAT}/Server.o

BIN_CLIENT = ${BIN}/${CLIENT}
BIN_CLIENT_PATHS = ${BIN_CHAT_PATHS} ${BIN_CLIENT}
BIN_CLIENT_OUT_FILES = ${BIN_CHAT}/Connection.o
BIN_CLIENT_MAIN = ${BIN_CLIENT}/main

BIN_SERVER = ${BIN}/${SERVER}
BIN_SERVER_PATHS = ${BIN_CHAT_PATHS} ${BIN_SERVER}
BIN_SERVER_OUT_FILES = ${BIN_CHAT_OUT_FILES}
BIN_SERVER_MAIN = ${BIN_SERVER}/main

all: ${CHAT} ${CLIENT} ${SERVER}

# Rules for creating directories
${BIN}:
	${MKDIR} $@

${BIN_CHAT}:
	${MKDIR} $@

${BIN_CLIENT}:
	${MKDIR} $@

${BIN_SERVER}:
	${MKDIR} $@

# Rules for building the chat library
${CHAT}: ${BIN_CHAT_PATHS} ${BIN_CHAT_OUT_FILES}

${BIN_CHAT}/%.o: ${SRC_CHAT}/%.cpp ${INCLUDE}/%.hpp
	${CXX} $< -o $@ ${PARAM} -c

# Rules for building the client app
${CLIENT}: ${BIN_CLIENT_PATHS} ${BIN_CLIENT_MAIN}

${BIN_CLIENT_MAIN}: ${SRC_CLIENT}/main.cpp ${BIN_CLIENT_OUT_FILES}
	${CXX} $< ${BIN_CLIENT_OUT_FILES} -o $@ ${PARAM}

# Rules for building the server app
${SERVER}: ${BIN_SERVER_PATHS} ${BIN_SERVER_MAIN}

${BIN_SERVER_MAIN}: ${SRC_SERVER}/main.cpp ${BIN_SERVER_OUT_FILES}
	${CXX} $< ${BIN_SERVER_OUT_FILES} -o $@ ${PARAM}

# Clean the previous build
clean:
	rm -R bin
