#include <vector>
#include <iostream>
#include <limits.h>

struct Edge {
    int src, dest, weight;
};
struct Graph {
    int V, E;
    struct Edge* edge;
};
struct Graph* createGraph(int V, int E)
{
    struct Graph* graph = new Graph;
    graph->V = V;
    graph->E = E;
    graph->edge = new Edge[E];
    return graph;
}

void printArr(int dist[], int n)
{
    printf("Vertex Distance from Source\n");
    for (int i = 0; i < n; ++i)
        printf("%d  %d\n", i, dist[i]);
}

void BellmanFord(struct Graph* graph, int src, int bfValues[], int bfPrevious[]) {
    int V = graph->V;
    int E = graph->E;

    // Step 1: Initialize distances from src to all other
    // vertices as INFINITE
    for (int i = 0; i < V; i++) {
        bfValues[i] = INT_MAX;
        bfPrevious[i] = -1;
    }
    bfValues[src] = 0;

    // Step 2: Relax all edges |V| - 1 times. A simple
    // shortest path from src to any other vertex can have
    // at-most |V| - 1 edges
    for (int i = 1; i <= V - 1; i++) {
        for (int j = 0; j < E; j++) {
            int u = graph->edge[j].src;
            int v = graph->edge[j].dest;
            int weight = graph->edge[j].weight;
            if (bfValues[u] != INT_MAX
                && bfValues[u] + weight < bfValues[v]) {
                bfValues[v] = bfValues[u] + weight;
                bfPrevious[v] = u;
            }
        }
    }

    // Step 3: check for negative-weight cycles.  The above
    // step guarantees shortest distances if graph doesn't
    // contain negative weight cycle.  If we get a shorter
    // path, then there is a cycle.
    for (int i = 0; i < E; i++) {
        int u = graph->edge[i].src;
        int v = graph->edge[i].dest;
        int weight = graph->edge[i].weight;
        if (bfValues[u] != INT_MAX
            && bfValues[u] + weight < bfValues[v]) {
            // printf("Graph contains negative weight cycle");
            return; // If negative cycle is detected, simply
                    // return
        }
    }

    // Print the updated Bellman-Ford Value array and Previous matrix
    // printf("Updated Bellman-Ford Value array:\n");
    // for (int i = 0; i < V; i++) {
    //     printf("%d ", bfValues[i]);
    // }
    // printf("\n");

    // printf("Updated Bellman-Ford Previous matrix:\n");
    // for (int i = 0; i < V; i++) {
    //     printf("%d ", bfPrevious[i]);
    // }
    // printf("\n");

    return;
}

int countEdges(int Input_graph[20][20], int numVertices) {
    int edgeCount = 0;
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            if (Input_graph[i][j]!= 0) {
                edgeCount++;
            }
        }
    }
    return edgeCount;
}
void BF(int Input_graph[20][20], int numVertices, char startVertex, int bfValues[20], int bfPrevious[20]) {
    int startIndex = startVertex - 'A'; // Convert character to integer index
    Graph* graph = createGraph(numVertices, countEdges(Input_graph, numVertices));
    int edgeIndex = 0;
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            if (Input_graph[i][j]!= 0) {
                graph->edge[edgeIndex].src = i;
                graph->edge[edgeIndex].dest = j;
                graph->edge[edgeIndex].weight = Input_graph[i][j];
                edgeIndex++;
            }
        }
    }
    BellmanFord(graph, startIndex, bfValues, bfPrevious); // Assuming the source vertex is 0
}

int main() {
    int Input_graph[20][20] = {{0, 4, 0, 0, 0, 0, 0, 0},
                             {4, 0, 8, 0, 0, 0, 0, 0},
                             {0, 8, 0, 7, 0, 4, 0, 0},
                             {0, 0, 7, 0, 9, 14, 0, 0},
                             {0, 0, 0, 9, 0, 10, 0, 0},
                             {0, 0, 4, 14, 10, 0, 2, 0},
                             {0, 0, 0, 0, 0, 2, 0, 1},
                             {0, 0, 0, 0, 0, 0, 1, 0}};

    int numVertices = 8;
    char startVertex = 'D';
    int bfValues[20];
    int bfPrevious[20];
    BF(Input_graph, numVertices, startVertex, bfValues, bfPrevious);
    return 0;
}