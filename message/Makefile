# protobuf messages
CXX=g++
CFLAGS_COMMON=-std=c++0x -g -O3 -Wall
INCLUDE_PROTOBUF=./google/protobuf

all : AppendEntries.pb.o AppendEntriesResult.pb.o RegisterMessage.pb.o RequestVote.pb.o Vote.pb.o Commit.pb.o \
      PSPushVec.pb.o PSPullVec.pb.o PSResponse.pb.o PSControl.pb.o GraphMessage.pb.o GraphResp.pb.o GraphControl.pb.o
.PHONY: all

%.pb.o : %.proto
	protoc --cpp_out=./ $<
	${CXX} -c ${CFLAGS_COMMON} ${@:.o=.cc} -o $@

clean:
	rm -rf *.o *.pb.cc *.pb.h

