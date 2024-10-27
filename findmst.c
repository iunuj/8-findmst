#include <stdio.h>
#include <stdlib.h>

#define MAX_EDGES 100

typedef struct {
	int start, end, weight;
} Edge;

typedef struct {
	int num_vertices;
	int num_edges;
	Edge edges[MAX_EDGES];
} GraphType;

void graph_init(GraphType* g) {
	g->num_vertices = 0;
	g->num_edges = 0;
}

int find(int parent[], int x) {
	if (parent[x] == x) return x;
	return parent[x] = find(parent, parent[x]);
}

void union_sets(int parent[], int rank[], int u, int v) {
	u = find(parent, u);
	v = find(parent, v);
	if (rank[u] < rank[v]) {
		parent[u] = v;
	}
	else if (rank[u] > rank[v]) {
		parent[v] = u;
	}
	else {
		parent[v] = u;
		rank[u]++;
	}
}

int compare(const void* a, const void* b) {
	Edge* edgeA = (Edge*)a;
	Edge* edgeB = (Edge*)b;
	return edgeA->weight - edgeB->weight;
}

void QuickKruskal(GraphType* g) {
	int parent[MAX_EDGES], rank[MAX_EDGES];
	for (int i = 0; i < g->num_vertices; i++) {
		parent[i] = i;
		rank[i] = 0;
	}

	qsort(g->edges, g->num_edges, sizeof(Edge), compare);

	printf("Quick Based Kruskal\n");
	for (int i = 0; i < g->num_edges; i++) {
		int u = g->edges[i].start;
		int v = g->edges[i].end;
		int weight = g->edges[i].weight;
		if (find(parent, u) != find(parent, v)) {
			printf("Edge (%d, %d) select %d\n", u, v, weight);
			union_sets(parent, rank, u, v);
		}
	}
}

typedef struct {
	Edge heap[MAX_EDGES];
	int size;
} MinHeap;

void minHeapInit(MinHeap* h) { h->size = 0; }

void insertMinHeap(MinHeap* h, Edge edge) {
	h->heap[h->size] = edge;
	int i = h->size;
	while (i > 0 && h->heap[(i - 1) / 2].weight > h->heap[i].weight) {
		Edge temp = h->heap[(i - 1) / 2];
		h->heap[(i - 1) / 2] = h->heap[i];
		h->heap[i] = temp;
		i = (i - 1) / 2;
	}
	h->size++;
}

Edge deleteMinHeap(MinHeap* h) {
	Edge minEdge = h->heap[0];
	h->size--;
	h->heap[0] = h->heap[h->size];
	int i = 0;
	while (i * 2 + 1 < h->size) {
		int minIdx = i * 2 + 1;
		if (minIdx + 1 < h->size && h->heap[minIdx + 1].weight < h->heap[minIdx].weight)
			minIdx++;
		if (h->heap[i].weight < h->heap[minIdx].weight) break;
		Edge temp = h->heap[i];
		h->heap[i] = h->heap[minIdx];
		h->heap[minIdx] = temp;
		i = minIdx;
	}
	return minEdge;
}

void MinHeapKruskal(GraphType* g) {
	int parent[MAX_EDGES], rank[MAX_EDGES];
	for (int i = 0; i < g->num_vertices; i++) {
		parent[i] = i;
		rank[i] = 0;
	}

	MinHeap heap;
	minHeapInit(&heap);
	for (int i = 0; i < g->num_edges; i++) {
		insertMinHeap(&heap, g->edges[i]);
	}
	printf("\n");
	printf("MinHeap Based Kruskal\n");
	while (heap.size > 0) {
		Edge edge = deleteMinHeap(&heap);
		int u = edge.start;
		int v = edge.end;
		if (find(parent, u) != find(parent, v)) {
			printf("Edge (%d, %d) select %d\n", u, v, edge.weight);
			union_sets(parent, rank, u, v);
		}
	}
}

void GenerateGraph(GraphType* g) {
	g->num_vertices = 10;
	g->num_edges = 9;
	g->edges[0] = (Edge){ 0, 5, 1 };  
	g->edges[1] = (Edge){ 3, 4, 2 };
	g->edges[2] = (Edge){ 1, 2, 3 };
	g->edges[3] = (Edge){ 2, 4, 4 };
	g->edges[4] = (Edge){ 3, 8, 5 };
	g->edges[5] = (Edge){ 3, 7, 6 };
	g->edges[6] = (Edge){ 2, 6, 7 };
	g->edges[7] = (Edge){ 8, 9, 10 }; 
	g->edges[8] = (Edge){ 7, 9, 15 }; 
}

int main(void) {
	GraphType* g;
	g = (GraphType*)malloc(sizeof(GraphType));
	graph_init(g);

	GenerateGraph(g);

	QuickKruskal(g);
	MinHeapKruskal(g);

	free(g);

	return 0;
}
