# make dsf
CFLAGS_COMMON = -std=c++0x -g -O3 -Wall
#INCLUDE_PROTOBUF=./google/protobuf

# message directory
MESSAGE = ./message
# util directory
UTIL = ./util

# message .o files
MSG_OBJ = ${wildcard ${MESSAGE}/*.o}
# message headers
MSG_HEAD = ${wildcard ${MESSAGE}/*.h}
# util headers
UTIL_HEAD = ${wildcard ${UTIL}/*.h}

dsf :  StateMachine.o ParameterServer.o MessageSocket.o main.o ${MSG_OBJ}
	g++ ${CFLAGS_COMMON} $^ -lprotobuf -lpthread -o $@

ParameterServer.o : ParameterServer.cc ParameterServer.h MessageSocket.h ${MSG_HEAD} ${UTIL_HEAD}
	g++ -c ${CFLAGS_COMMON} -lpthread $< -o $@

StateMachine.o : StateMachine.cc StateMachine.h MessageSocket.h ${MSG_HEAD} ${UTIL_HEAD}
	g++ -c $(CFLAGS_COMMON) -lpthread $< -o $@

MessageSocket.o : MessageSocket.cc MessageSocket.h ${MSG_HEAD} ${UTIL_HEAD}
	g++ -c $(CFLAGS_COMMON) $< -o $@

main.o : main.cc StateMachine.h ParameterServer.h
	g++ -c $(CFLAGS_COMMON) $< -o $@

#--------------------------------------------------
# clean up
#--------------------------------------------------

clean:
	rm -rf dsf *.o *.pb-c.[ch]