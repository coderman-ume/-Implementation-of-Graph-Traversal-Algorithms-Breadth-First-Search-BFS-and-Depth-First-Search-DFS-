using System;
using System.Collections.Generic;
using System.Linq;

class Graph
{
    private readonly int _n;
    private readonly List<int>[] _adj;

    public Graph(int n)
    {
        _n = n;
        _adj = new List<int>[n];
        for (int i = 0; i < n; i++) _adj[i] = new List<int>();
    }

    public bool IsValid(int v) => v >= 0 && v < _n;

    public bool AddEdge(int u, int v)
    {
        if (!IsValid(u) || !IsValid(v)) return false;
        _adj[u].Add(v);
        _adj[v].Add(u);
        return true;
    }

    public void SortNeighbours()
    {
        foreach (var list in _adj) list.Sort();
    }

    public void Display()
    {
        for (int v = 0; v < _n; v++)
            Console.WriteLine($"{v}:" + string.Concat(_adj[v].Select(x => $" -> {x}")));
    }

    public List<int> Bfs(int src)
    {
        var visited = new bool[_n];
        var queue = new Queue<int>();
        var order = new List<int>();
        visited[src] = true;
        queue.Enqueue(src);
        while (queue.Count > 0)
        {
            int u = queue.Dequeue();
            order.Add(u);
            foreach (int v in _adj[u])
            {
                if (!visited[v])
                {
                    visited[v] = true;
                    queue.Enqueue(v);
                }
            }
        }
        return order;
    }

    public List<int> Dfs(int src)
    {
        var visited = new bool[_n];
        var stack = new Stack<int>();
        var order = new List<int>();
        stack.Push(src);
        while (stack.Count > 0)
        {
            int u = stack.Pop();
            if (visited[u]) continue;
            visited[u] = true;
            order.Add(u);
            for (int i = _adj[u].Count - 1; i >= 0; i--)
            {
                int v = _adj[u][i];
                if (!visited[v]) stack.Push(v);
            }
        }
        return order;
    }

    public List<int> Unreachable(List<int> order)
    {
        var seen = new bool[_n];
        foreach (int v in order) seen[v] = true;
        return Enumerable.Range(0, _n).Where(v => !seen[v]).ToList();
    }

    static int ReadInt(string prompt, int low, int high)
    {
        while (true)
        {
            Console.Write(prompt);
            if (int.TryParse(Console.ReadLine()?.Trim(), out int x) && x >= low && x <= high)
                return x;
            Console.WriteLine($"Invalid. Enter an integer from {low} to {high}.");
        }
    }

    static void Main()
    {
        int vertices = ReadInt("Enter number of vertices V: ", 1, 1000000);
        int edges = ReadInt("Enter number of edges E: ", 0, 1000000);

        var graph = new Graph(vertices);
        for (int i = 1; i <= edges; i++)
        {
            while (true)
            {
                Console.Write($"Enter edge {i} (u v): ");
                var parts = (Console.ReadLine() ?? "").Split((char[])null, StringSplitOptions.RemoveEmptyEntries);
                if (parts.Length == 2 && int.TryParse(parts[0], out int u) && int.TryParse(parts[1], out int v))
                {
                    if (!graph.AddEdge(u, v))
                        Console.WriteLine($"Rejected: vertex out of range (valid range 0 to {vertices - 1}).");
                    break;
                }
                Console.WriteLine("Enter exactly two integers.");
            }
        }

        graph.SortNeighbours();
        Console.WriteLine("\nAdjacency list:");
        graph.Display();

        int source = ReadInt($"\nEnter source vertex (0 to {vertices - 1}): ", 0, vertices - 1);

        var bfs = graph.Bfs(source);
        Console.WriteLine("\nBFS traversal order: " + string.Join(" ", bfs));
        foreach (int v in graph.Unreachable(bfs))
            Console.WriteLine($"Vertex {v} is not reachable from {source} (BFS).");

        var dfs = graph.Dfs(source);
        Console.WriteLine("\nDFS traversal order: " + string.Join(" ", dfs));
        foreach (int v in graph.Unreachable(dfs))
            Console.WriteLine($"Vertex {v} is not reachable from {source} (DFS).");
    }
}
