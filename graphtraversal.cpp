#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

class Graph {
public:
    explicit Graph(int vertexCount)
        : adjacencyList_(vertexCount) {
    }

    void addUndirectedEdge(int from, int to) {
        addDirectedEdge(from, to);
        if (from != to) {
            addDirectedEdge(to, from);
        }
    }

    void printAdjacencyList() const {
        std::cout << "\nAdjacency list:\n";
        for (int vertex = 0; vertex < static_cast<int>(adjacencyList_.size()); ++vertex) {
            std::cout << vertex << ":";
            for (int neighbor : adjacencyList_[vertex]) {
                std::cout << " " << neighbor;
            }
            std::cout << '\n';
        }
    }

    void breadthFirstSearch(int source) const {
        validateVertex(source);

        std::vector<bool> visited(adjacencyList_.size(), false);
        std::queue<int> queue;

        visited[source] = true;
        queue.push(source);

        std::cout << "\nBFS order from " << source << ":";
        while (!queue.empty()) {
            int current = queue.front();
            queue.pop();
            std::cout << " " << current;

            for (int next : adjacencyList_[current]) {
                if (!visited[next]) {
                    visited[next] = true;
                    queue.push(next);
                }
            }
        }
        std::cout << '\n';

        printReachabilityReport(visited, source);
    }

    void depthFirstSearch(int source) const {
        validateVertex(source);

        std::vector<bool> visited(adjacencyList_.size(), false);
        std::stack<int> stack;

        stack.push(source);

        std::cout << "\nDFS order from " << source << ":";
        while (!stack.empty()) {
            int current = stack.top();
            stack.pop();

            if (visited[current]) {
                continue;
            }

            visited[current] = true;
            std::cout << " " << current;

            for (auto it = adjacencyList_[current].rbegin(); it != adjacencyList_[current].rend(); ++it) {
                int next = *it;
                if (!visited[next]) {
                    stack.push(next);
                }
            }
        }
        std::cout << '\n';

        printReachabilityReport(visited, source);
    }

private:
    void validateVertex(int vertex) const {
        if (vertex < 0 || vertex >= static_cast<int>(adjacencyList_.size())) {
            throw std::out_of_range("Vertex is outside the valid range.");
        }
    }

    void addDirectedEdge(int from, int to) {
        validateVertex(from);
        validateVertex(to);

        std::vector<int>& neighbors = adjacencyList_[from];
        auto position = std::lower_bound(neighbors.begin(), neighbors.end(), to);

        if (position != neighbors.end() && *position == to) {
            return;
        }

        neighbors.insert(position, to);
    }

    void printReachabilityReport(const std::vector<bool>& visited, int source) const {
        bool hasUnreachableVertex = false;

        for (int vertex = 0; vertex < static_cast<int>(visited.size()); ++vertex) {
            if (!visited[vertex]) {
                std::cout << "Vertex " << vertex << " is not reachable from source " << source << ".\n";
                hasUnreachableVertex = true;
            }
        }

        if (!hasUnreachableVertex) {
            std::cout << "Every vertex is reachable from source " << source << ".\n";
        }
    }

    std::vector<std::vector<int>> adjacencyList_;
};

int readInt(const std::string& prompt) {
    int value = 0;

    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            return value;
        }

        std::cout << "Please enter a whole number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

int readVertex(const std::string& prompt, int vertexCount) {
    while (true) {
        int vertex = readInt(prompt);
        if (vertex >= 0 && vertex < vertexCount) {
            return vertex;
        }
        std::cout << "Vertex must be between 0 and " << vertexCount - 1 << ".\n";
    }
}

int main() {
    int vertexCount = readInt("Number of vertices V: ");
    int edgeCount = readInt("Number of edges E: ");

    if (vertexCount < 1 || edgeCount < 0) {
        std::cout << "Vertices must be at least 1, and edges cannot be negative.\n";
        return 1;
    }

    Graph graph(vertexCount);
    int acceptedEdges = 0;

    for (int edgeIndex = 0; edgeIndex < edgeCount; ++edgeIndex) {
        std::cout << "Edge " << edgeIndex + 1 << " (u v): ";
        int from = readInt("");
        int to = readInt("");

        if (from < 0 || from >= vertexCount || to < 0 || to >= vertexCount) {
            std::cout << "Rejected: both endpoints must be between 0 and " << vertexCount - 1 << ".\n";
            continue;
        }

        graph.addUndirectedEdge(from, to);
        ++acceptedEdges;
    }

    std::cout << "Accepted " << acceptedEdges << " of " << edgeCount << " edges.\n";
    graph.printAdjacencyList();

    std::cout << "\nTask 2: Breadth-First Search\n";
    graph.breadthFirstSearch(readVertex("Source vertex: ", vertexCount));

    std::cout << "\nTask 3: Depth-First Search\n";
    graph.depthFirstSearch(readVertex("Source vertex: ", vertexCount));

    return 0;
}
