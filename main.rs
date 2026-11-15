use std::collections::VecDeque;
use std::io::{self, BufRead, Write};

struct Graph {
    n: usize,
    adj: Vec<Vec<usize>>,
}

impl Graph {
    fn new(n: usize) -> Self {
        Graph { n, adj: vec![Vec::new(); n] }
    }

    fn add_edge(&mut self, u: i64, v: i64) -> bool {
        if u < 0 || v < 0 || u as usize >= self.n || v as usize >= self.n {
            return false;
        }
        let (u, v) = (u as usize, v as usize);
        self.adj[u].push(v);
        self.adj[v].push(u);
        true
    }

    fn sort_neighbours(&mut self) {
        for list in self.adj.iter_mut() {
            list.sort();
        }
    }

    fn display(&self) {
        for v in 0..self.n {
            let parts: String = self.adj[v].iter().map(|x| format!(" -> {}", x)).collect();
            println!("{}:{}", v, parts);
        }
    }

    fn bfs(&self, src: usize) -> Vec<usize> {
        let mut visited = vec![false; self.n];
        let mut queue = VecDeque::new();
        let mut order = Vec::new();
        visited[src] = true;
        queue.push_back(src);
        while let Some(u) = queue.pop_front() {
            order.push(u);
            for &v in &self.adj[u] {
                if !visited[v] {
                    visited[v] = true;
                    queue.push_back(v);
                }
            }
        }
        order
    }

    fn dfs(&self, src: usize) -> Vec<usize> {
        let mut visited = vec![false; self.n];
        let mut stack = vec![src];
        let mut order = Vec::new();
        while let Some(u) = stack.pop() {
            if visited[u] {
                continue;
            }
            visited[u] = true;
            order.push(u);
            for &v in self.adj[u].iter().rev() {
                if !visited[v] {
                    stack.push(v);
                }
            }
        }
        order
    }

    fn unreachable(&self, order: &[usize]) -> Vec<usize> {
        let mut seen = vec![false; self.n];
        for &v in order {
            seen[v] = true;
        }
        (0..self.n).filter(|&v| !seen[v]).collect()
    }
}

fn read_line(stdin: &io::Stdin) -> String {
    let mut line = String::new();
    stdin.lock().read_line(&mut line).unwrap();
    line
}

fn read_int(stdin: &io::Stdin, prompt: &str, low: i64, high: i64) -> i64 {
    loop {
        print!("{}", prompt);
        io::stdout().flush().unwrap();
        if let Ok(x) = read_line(stdin).trim().parse::<i64>() {
            if x >= low && x <= high {
                return x;
            }
        }
        println!("Invalid. Enter an integer from {} to {}.", low, high);
    }
}

fn join(v: &[usize]) -> String {
    v.iter().map(|x| x.to_string()).collect::<Vec<_>>().join(" ")
}

fn main() {
    let stdin = io::stdin();

    let vertices = read_int(&stdin, "Enter number of vertices V: ", 1, 1_000_000) as usize;
    let edges = read_int(&stdin, "Enter number of edges E: ", 0, 1_000_000);

    let mut graph = Graph::new(vertices);
    for i in 1..=edges {
        loop {
            print!("Enter edge {} (u v): ", i);
            io::stdout().flush().unwrap();
            let line = read_line(&stdin);
            let parsed: Result<Vec<i64>, _> = line.split_whitespace().map(|t| t.parse::<i64>()).collect();
            match parsed {
                Ok(p) if p.len() == 2 => {
                    if !graph.add_edge(p[0], p[1]) {
                        println!("Rejected: vertex out of range (valid range 0 to {}).", vertices - 1);
                    }
                    break;
                }
                _ => println!("Enter exactly two integers."),
            }
        }
    }

    graph.sort_neighbours();
    println!("\nAdjacency list:");
    graph.display();

    let source = read_int(
        &stdin,
        &format!("\nEnter source vertex (0 to {}): ", vertices - 1),
        0,
        (vertices - 1) as i64,
    ) as usize;

    let bfs = graph.bfs(source);
    println!("\nBFS traversal order: {}", join(&bfs));
    for v in graph.unreachable(&bfs) {
        println!("Vertex {} is not reachable from {} (BFS).", v, source);
    }

    let dfs = graph.dfs(source);
    println!("\nDFS traversal order: {}", join(&dfs));
    for v in graph.unreachable(&dfs) {
        println!("Vertex {} is not reachable from {} (DFS).", v, source);
    }
}
