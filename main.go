package main

import (
	"bufio"
	"fmt"
	"os"
	"sort"
	"strings"
)

type Graph struct {
	n   int
	adj [][]int
}

func NewGraph(n int) *Graph {
	return &Graph{n: n, adj: make([][]int, n)}
}

func (g *Graph) valid(v int) bool {
	return v >= 0 && v < g.n
}

func (g *Graph) AddEdge(u, v int) bool {
	if !g.valid(u) || !g.valid(v) {
		return false
	}
	g.adj[u] = append(g.adj[u], v)
	g.adj[v] = append(g.adj[v], u)
	return true
}

func (g *Graph) SortNeighbours() {
	for _, list := range g.adj {
		sort.Ints(list)
	}
}

func (g *Graph) Display() {
	for v := 0; v < g.n; v++ {
		fmt.Printf("%d:", v)
		for _, x := range g.adj[v] {
			fmt.Printf(" -> %d", x)
		}
		fmt.Println()
	}
}

func (g *Graph) BFS(src int) []int {
	visited := make([]bool, g.n)
	queue := []int{src}
	visited[src] = true
	order := []int{}
	for len(queue) > 0 {
		u := queue[0]
		queue = queue[1:]
		order = append(order, u)
		for _, v := range g.adj[u] {
			if !visited[v] {
				visited[v] = true
				queue = append(queue, v)
			}
		}
	}
	return order
}

func (g *Graph) DFS(src int) []int {
	visited := make([]bool, g.n)
	stack := []int{src}
	order := []int{}
	for len(stack) > 0 {
		u := stack[len(stack)-1]
		stack = stack[:len(stack)-1]
		if visited[u] {
			continue
		}
		visited[u] = true
		order = append(order, u)
		for i := len(g.adj[u]) - 1; i >= 0; i-- {
			v := g.adj[u][i]
			if !visited[v] {
				stack = append(stack, v)
			}
		}
	}
	return order
}

func (g *Graph) Unreachable(order []int) []int {
	seen := make([]bool, g.n)
	for _, v := range order {
		seen[v] = true
	}
	res := []int{}
	for v := 0; v < g.n; v++ {
		if !seen[v] {
			res = append(res, v)
		}
	}
	return res
}

func readLine(r *bufio.Reader) string {
	line, err := r.ReadString('\n')
	if err != nil && line == "" {
		os.Exit(1)
	}
	return strings.TrimSpace(line)
}

func readInt(r *bufio.Reader, prompt string, low, high int) int {
	for {
		fmt.Print(prompt)
		var x int
		if _, err := fmt.Sscan(readLine(r), &x); err == nil && x >= low && x <= high {
			return x
		}
		fmt.Printf("Invalid. Enter an integer from %d to %d.\n", low, high)
	}
}

func join(v []int) string {
	parts := make([]string, len(v))
	for i, x := range v {
		parts[i] = fmt.Sprint(x)
	}
	return strings.Join(parts, " ")
}

func main() {
	reader := bufio.NewReader(os.Stdin)

	vertices := readInt(reader, "Enter number of vertices V: ", 1, 1000000)
	edges := readInt(reader, "Enter number of edges E: ", 0, 1000000)

	graph := NewGraph(vertices)
	for i := 1; i <= edges; i++ {
		for {
			fmt.Printf("Enter edge %d (u v): ", i)
			var u, v int
			if n, err := fmt.Sscan(readLine(reader), &u, &v); err == nil && n == 2 {
				if !graph.AddEdge(u, v) {
					fmt.Printf("Rejected: vertex out of range (valid range 0 to %d).\n", vertices-1)
				}
				break
			}
			fmt.Println("Enter exactly two integers.")
		}
	}

	graph.SortNeighbours()
	fmt.Println("\nAdjacency list:")
	graph.Display()

	source := readInt(reader, fmt.Sprintf("\nEnter source vertex (0 to %d): ", vertices-1), 0, vertices-1)

	bfs := graph.BFS(source)
	fmt.Printf("\nBFS traversal order: %s\n", join(bfs))
	for _, v := range graph.Unreachable(bfs) {
		fmt.Printf("Vertex %d is not reachable from %d (BFS).\n", v, source)
	}

	dfs := graph.DFS(source)
	fmt.Printf("\nDFS traversal order: %s\n", join(dfs))
	for _, v := range graph.Unreachable(dfs) {
		fmt.Printf("Vertex %d is not reachable from %d (DFS).\n", v, source)
	}
}
