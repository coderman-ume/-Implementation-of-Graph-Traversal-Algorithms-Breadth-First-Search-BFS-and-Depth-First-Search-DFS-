from collections import deque


class Graph:
    def __init__(self, vertex_count):
        self.vertex_count = vertex_count
        self.adj = [[] for _ in range(vertex_count)]

    def is_valid(self, v):
        return 0 <= v < self.vertex_count

    def add_edge(self, u, v):
        if not (self.is_valid(u) and self.is_valid(v)):
            return False
        self.adj[u].append(v)
        self.adj[v].append(u)
        return True

    def sort_neighbours(self):
        for neighbours in self.adj:
            neighbours.sort()

    def display(self):
        for v in range(self.vertex_count):
            print(f"{v}:" + "".join(f" -> {n}" for n in self.adj[v]))

    def bfs(self, source):
        visited = [False] * self.vertex_count
        queue = deque([source])
        visited[source] = True
        order = []
        while queue:
            u = queue.popleft()
            order.append(u)
            for v in self.adj[u]:
                if not visited[v]:
                    visited[v] = True
                    queue.append(v)
        return order

    def dfs(self, source):
        visited = [False] * self.vertex_count
        stack = [source]
        order = []
        while stack:
            u = stack.pop()
            if visited[u]:
                continue
            visited[u] = True
            order.append(u)
            for v in reversed(self.adj[u]):
                if not visited[v]:
                    stack.append(v)
        return order

    def unreachable(self, order):
        seen = set(order)
        return [v for v in range(self.vertex_count) if v not in seen]


def read_int(prompt, low, high):
    while True:
        raw = input(prompt).strip()
        if raw.lstrip("-").isdigit():
            value = int(raw)
            if low <= value <= high:
                return value
        print(f"Invalid. Enter an integer from {low} to {high}.")


def main():
    vertices = read_int("Enter number of vertices V: ", 1, 10**9)
    edges = read_int("Enter number of edges E: ", 0, 10**9)

    graph = Graph(vertices)
    for i in range(1, edges + 1):
        while True:
            parts = input(f"Enter edge {i} (u v): ").split()
            if len(parts) == 2 and all(p.lstrip("-").isdigit() for p in parts):
                u, v = int(parts[0]), int(parts[1])
                break
            print("Enter exactly two integers.")
        if not graph.add_edge(u, v):
            print(f"Rejected: vertex out of range (valid range 0 to {vertices - 1}).")

    graph.sort_neighbours()
    print("\nAdjacency list:")
    graph.display()

    source = read_int(f"\nEnter source vertex (0 to {vertices - 1}): ", 0, vertices - 1)

    bfs_order = graph.bfs(source)
    print("\nBFS traversal order:", " ".join(map(str, bfs_order)))
    for v in graph.unreachable(bfs_order):
        print(f"Vertex {v} is not reachable from {source} (BFS).")

    dfs_order = graph.dfs(source)
    print("\nDFS traversal order:", " ".join(map(str, dfs_order)))
    for v in graph.unreachable(dfs_order):
        print(f"Vertex {v} is not reachable from {source} (DFS).")


if __name__ == "__main__":
    main()
