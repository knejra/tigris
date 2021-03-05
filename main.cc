#include <string.h>

#include "ParameterServer.h"
#include "StateMachine.h"
//#include "Graph.h"

#define RAFT  0x1
#define PS    0x2
#define GRAPH 0x4

// StateMachine: Raft consensus protocol
StateMachine stateMachine;
// ParameterServer: distributed ML/DL frame
ParameterServer paramServer;
// Graph: Graph Processing
Graph<double, double, GraphMessage> graph;

int main(int argc, char *argv[])
{
    int start = atoi(argv[1]);
    int machineId = atoi(argv[2]);
    int role;
    
    if((start & RAFT) == RAFT)
    {
        stateMachine.init(machineId);
        stateMachine.destroy();
    }

    if((start & PS) == PS)
    {
        role = (strcmp(argv[3], "server") == 0) ? SERVER : WORKER;
        if(role == WORKER)
        {
            role = (strcmp(argv[3], "scheduler") == 0) ? W_SCHEDULER : role;
        }
        paramServer.init(machineId, role);
        paramServer.destroy();
    }

    // if((start & GRAPH) == GRAPH)
    // {
    //     role = (strcmp(argv[3], "master") == 0) ? MASTER : SLAVE;
    //     graph.init(machineId, role);
    //     graph.destroy();
    // }

    return 0;
}