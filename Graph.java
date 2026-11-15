package com.mahatab.graph;

import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Deque;
import java.util.List;
import java.util.Scanner;

public class Graph {
    private final int vertexCount;
    private final List<List<Integer>> adj;

    public Graph(int vertexCount) {
        this.vertexCount = vertexCount;
        this.adj = new ArrayList<>();
        for (int i = 0; i < vertexCount; i++) {
            adj.add(new ArrayList<>());
        }
    }

    public boolean isValid(int v) {
        return v >= 0 && v < vertexCount;
    }

    public boolean addEdge(int u, int v) {
        if (!isValid(u) || !isValid(v)) {
            return false;
        }
        adj.get(u).add(v);
        adj.get(v).add(u);
        return true;
    }

    public void sortNeighbours() {
        for (List<Integer> neighbours : adj) {
            Collections.sort(neighbours);
        }
    }

    public void display() {
        for (int v = 0; v < vertexCount; v++) {
            StringBuilder line = new StringBuilder(v + ":");
            for (int n : adj.get(v)) {
                line.append(" -> ").append(n);
            }
            System.out.println(line);
        }
    }

    public List<Integer> bfs(int source) {
        boolean[] visited = new boolean[vertexCount];
        Deque<Integer> queue = new ArrayDeque<>();
        List<Integer> order = new ArrayList<>();
        visited[source] = true;
        queue.offer(source);
        while (!queue.isEmpty()) {
            int u = queue.poll();
            order.add(u);
            for (int v : adj.get(u)) {
                if (!visited[v]) {
                    visited[v] = true;
                    queue.offer(v);
                }
            }
        }
        return order;
    }

    public List<Integer> dfs(int source) {
        boolean[] visited = new boolean[vertexCount];
        Deque<Integer> stack = new ArrayDeque<>();
        List<Integer> order = new ArrayList<>();
        stack.push(source);
        while (!stack.isEmpty()) {
            int u = stack.pop();
            if (visited[u]) {
                continue;
            }
            visited[u] = true;
            order.add(u);
            List<Integer> neighbours = adj.get(u);
            for (int i = neighbours.size() - 1; i >= 0; i--) {
                int v = neighbours.get(i);
                if (!visited[v]) {
                    stack.push(v);
                }
            }
        }
        return order;
    }

    public List<Integer> unreachable(List<Integer> order) {
        boolean[] seen = new boolean[vertexCount];
        for (int v : order) {
            seen[v] = true;
        }
        List<Integer> result = new ArrayList<>();
        for (int v = 0; v < vertexCount; v++) {
            if (!seen[v]) {
                result.add(v);
            }
        }
        return result;
    }

    private static int readInRange(Scanner in, String prompt, int low, int high) {
        while (true) {
            System.out.print(prompt);
            if (in.hasNextInt()) {
                int x = in.nextInt();
                if (x >= low && x <= high) {
                    return x;
                }
            } else {
                in.next();
            }
            System.out.println("Invalid. Enter an integer from " + low + " to " + high + ".");
        }
    }

    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        int vertices = readInRange(in, "Enter number of vertices V: ", 1, Integer.MAX_VALUE);
        int edges = readInRange(in, "Enter number of edges E: ", 0, Integer.MAX_VALUE);

        Graph graph = new Graph(vertices);
        for (int i = 1; i <= edges; i++) {
            System.out.print("Enter edge " + i + " (u v): ");
            int u = in.nextInt();
            int v = in.nextInt();
            if (!graph.addEdge(u, v)) {
                System.out.println("Rejected: vertex out of range (valid range 0 to " + (vertices - 1) + ").");
            }
        }

        graph.sortNeighbours();
        System.out.println("\nAdjacency list:");
        graph.display();

        int source = readInRange(in, "\nEnter source vertex (0 to " + (vertices - 1) + "): ", 0, vertices - 1);

        List<Integer> bfsOrder = graph.bfs(source);
        System.out.println("\nBFS traversal order: " + bfsOrder);
        for (int v : graph.unreachable(bfsOrder)) {
            System.out.println("Vertex " + v + " is not reachable from " + source + " (BFS).");
        }

        List<Integer> dfsOrder = graph.dfs(source);
        System.out.println("\nDFS traversal order: " + dfsOrder);
        for (int v : graph.unreachable(dfsOrder)) {
            System.out.println("Vertex " + v + " is not reachable from " + source + " (DFS).");
        }

        in.close();
    }
}
