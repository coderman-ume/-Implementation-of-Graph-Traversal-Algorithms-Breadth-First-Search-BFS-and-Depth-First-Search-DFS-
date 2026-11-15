#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int vertexCount;
    int *degree;
    int *capacity;
    int **neighbors;
} Graph;

static void *allocate(size_t count, size_t size) {
    void *memory = calloc(count ? count : 1, size);
    if (memory == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    return memory;
}

static Graph createGraph(int vertexCount) {
    Graph graph;

    graph.vertexCount = vertexCount;
    graph.degree = allocate((size_t)vertexCount, sizeof *graph.degree);
    graph.capacity = allocate((size_t)vertexCount, sizeof *graph.capacity);
    graph.neighbors = allocate((size_t)vertexCount, sizeof *graph.neighbors);
    return graph;
}

static void releaseGraph(Graph *graph) {
    for (int v = 0; v < graph->vertexCount; v++)
        free(graph->neighbors[v]);
    free(graph->neighbors);
    free(graph->degree);
    free(graph->capacity);
}

static void insertNeighbor(Graph *graph, int from, int to) {
    int count = graph->degree[from];
    int position = 0;
    int *list;

    while (position < count && graph->neighbors[from][position] < to)
        position++;
    if (position < count && graph->neighbors[from][position] == to)
        return;

    if (count == graph->capacity[from]) {
        int newCapacity = graph->capacity[from] > 0 ? graph->capacity[from] * 2 : 4;

        list = realloc(graph->neighbors[from], (size_t)newCapacity * sizeof *list);
        if (list == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }
        graph->neighbors[from] = list;
        graph->capacity[from] = newCapacity;
    }

    for (int i = count; i > position; i--)
        graph->neighbors[from][i] = graph->neighbors[from][i - 1];
    graph->neighbors[from][position] = to;
    graph->degree[from]++;
}

static void addEdge(Graph *graph, int u, int v) {
    insertNeighbor(graph, u, v);
    if (u != v)
        insertNeighbor(graph, v, u);
}

static void printAdjacencyList(const Graph *graph) {
    printf("\nAdjacency list:\n");
    for (int v = 0; v < graph->vertexCount; v++) {
        printf("%d:", v);
        for (int i = 0; i < graph->degree[v]; i++)
            printf(" %d", graph->neighbors[v][i]);
        printf("\n");
    }
}

static void printUnreachable(const int *visited, int count, int source) {
    int unreachableFound = 0;

    for (int v = 0; v < count; v++) {
        if (!visited[v]) {
            printf("Vertex %d is not reachable from source %d.\n", v, source);
            unreachableFound = 1;
        }
    }
    if (!unreachableFound)
        printf("Every vertex is reachable from source %d.\n", source);
}

static void breadthFirstSearch(const Graph *graph, int source) {
    int count = graph->vertexCount;
    int *visited = allocate((size_t)count, sizeof *visited);
    int *queue = allocate((size_t)count, sizeof *queue);
    int head = 0;
    int tail = 0;

    visited[source] = 1;
    queue[tail++] = source;

    printf("\nBFS order from %d:", source);
    while (head < tail) {
        int current = queue[head++];

        printf(" %d", current);
        for (int i = 0; i < graph->degree[current]; i++) {
            int next = graph->neighbors[current][i];

            if (!visited[next]) {
                visited[next] = 1;
                queue[tail++] = next;
            }
        }
    }
    printf("\n");
    printUnreachable(visited, count, source);

    free(visited);
    free(queue);
}

static void depthFirstSearch(const Graph *graph, int source) {
    int count = graph->vertexCount;
    int stackCapacity = 1;
    int *visited = allocate((size_t)count, sizeof *visited);
    int *stack;
    int top = 0;

    for (int v = 0; v < count; v++)
        stackCapacity += graph->degree[v];
    stack = allocate((size_t)stackCapacity, sizeof *stack);

    stack[top++] = source;

    printf("\nDFS order from %d:", source);
    while (top > 0) {
        int current = stack[--top];

        if (visited[current])
            continue;
        visited[current] = 1;
        printf(" %d", current);

        for (int i = graph->degree[current] - 1; i >= 0; i--) {
            int next = graph->neighbors[current][i];

            if (!visited[next])
                stack[top++] = next;
        }
    }
    printf("\n");
    printUnreachable(visited, count, source);

    free(visited);
    free(stack);
}

static int readInt(const char *prompt) {
    int value;

    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) == 1)
            return value;
        printf("Please enter a whole number.\n");
        while (getchar() != '\n' && !feof(stdin))
            ;
        if (feof(stdin))
            exit(EXIT_FAILURE);
    }
}

static int isInRange(int vertex, int vertexCount) {
    return vertex >= 0 && vertex < vertexCount;
}

static int readVertex(const char *prompt, int vertexCount) {
    int vertex;

    do {
        vertex = readInt(prompt);
        if (!isInRange(vertex, vertexCount))
            printf("Vertex must be between 0 and %d.\n", vertexCount - 1);
    } while (!isInRange(vertex, vertexCount));
    return vertex;
}

int main(void) {
    int vertexCount = readInt("Number of vertices V: ");
    int edgeCount = readInt("Number of edges E: ");
    int acceptedEdges = 0;
    Graph graph;

    if (vertexCount < 1 || edgeCount < 0) {
        printf("Vertices must be at least 1 and edges cannot be negative.\n");
        return EXIT_FAILURE;
    }

    graph = createGraph(vertexCount);

    for (int i = 0; i < edgeCount; i++) {
        int u;
        int v;

        printf("Edge %d (u v): ", i + 1);
        u = readInt("");
        v = readInt("");
        if (!isInRange(u, vertexCount) || !isInRange(v, vertexCount)) {
            printf("Rejected: both endpoints must be between 0 and %d.\n", vertexCount - 1);
            continue;
        }
        addEdge(&graph, u, v);
        acceptedEdges++;
    }
    printf("Accepted %d of %d edges.\n", acceptedEdges, edgeCount);

    printAdjacencyList(&graph);

    printf("\nTask 2: Breadth First Search\n");
    breadthFirstSearch(&graph, readVertex("Source vertex: ", vertexCount));

    printf("\nTask 3: Depth First Search\n");
    depthFirstSearch(&graph, readVertex("Source vertex: ", vertexCount));

    releaseGraph(&graph);
    return EXIT_SUCCESS;
}
