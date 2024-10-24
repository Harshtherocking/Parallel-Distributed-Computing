#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

#define INF (int)1e9
#define START 0
#define END 5

void dijkstra ( int start, int end, std::vector<std::vector<int>> &graph, std::vector<bool> & visited, std::vector<int> & distance, std::vector<int>path){
	visited[start] = true;
	distance[start] = 0;

	int curr = start;

	while (curr != end){
		visited[curr] = true;
		// printf("Exploring node : %d\n", curr);

		#pragma omp parallel for
		for (int i = 0; i < graph[curr].size(); i++){

			// check for unvisited neighbouring node nodes
			if (! visited[i] && graph[curr][i] != INF && i != curr){

				// update the value
				if (distance[i] > distance[curr] + graph[curr][i]){
					path[i] = curr;
					distance[i] = distance[curr]  + graph[curr][i];
				}
			}
		}

		// choosing miniumum distant node to explore (can be done better with priority queue with log(n) time complexity)
		int min_distance = INF;
		for (int i =0 ; i < distance.size(); i++ ){
			if (distance[i] < min_distance && ! visited[i]){
				min_distance = distance[i];
				curr = i;
			}
		}
		if (min_distance == INF) break;
	}


	printf("Distance : %d\n", distance[end]);
	printf("Path : ");

	int p = end;
	while (p != start){
		printf("%d ",p);
		p = path[p];
	}
	printf("%d", start);


}

int main() {
    std::vector<std::vector<int>> graph = {
	// A   B   C   D   E   F
	{  0,  7, 12, INF, INF, INF }, // A
	{ INF,  0,  2,   9, INF, INF }, // B
	{ INF, INF,  0, INF,  10, INF }, // C
	{ INF, INF, INF,  0,   INF,   1 }, // D
	{ INF, INF, INF,  4,   0,   5 }, // E
	{ INF, INF, INF, INF, INF,   0 }  // F
    };

	std::vector<bool> visited (graph.size(), false);
	std::vector<int> distance (graph.size(), INF);
	std::vector<int> path (graph.size(), -1);

	dijkstra(START,END, graph, visited, distance, path);


}
