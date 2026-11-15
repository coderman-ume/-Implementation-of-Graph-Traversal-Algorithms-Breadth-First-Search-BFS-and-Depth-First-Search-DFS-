const readline = require('readline/promises');
const { stdin, stdout } = require('process');

class Graph {
  constructor(vertexCount) {
    this.vertexCount = vertexCount;
    this.adj = Array.from({ length: vertexCount }, () => []);
  }

  isValid(v) {
    return Number.isInteger(v) && v >= 0 && v < this.vertexCount;
  }

  addEdge(u, v) {
    if (!this.isValid(u) || !this.isValid(v)) return false;
    this.adj[u].push(v);
    this.adj[v].push(u);
    return true;
  }

  sortNeighbours() {
    for (const neighbours of this.adj) {
      neighbours.sort((a, b) => a - b);
    }
  }

  display() {
    this.adj.forEach((neighbours, v) => {
      console.log(`${v}:` + neighbours.map((n) => ` -> ${n}`).join(''));
    });
  }

  bfs(source) {
    const visited = new Array(this.vertexCount).fill(false);
    const queue = [source];
    const order = [];
    let head = 0;
    visited[source] = true;
    while (head < queue.length) {
      const u = queue[head++];
      order.push(u);
      for (const v of this.adj[u]) {
        if (!visited[v]) {
          visited[v] = true;
          queue.push(v);
        }
      }
    }
    return order;
  }

  dfs(source) {
    const visited = new Array(this.vertexCount).fill(false);
    const stack = [source];
    const order = [];
    while (stack.length > 0) {
      const u = stack.pop();
      if (visited[u]) continue;
      visited[u] = true;
      order.push(u);
      const neighbours = this.adj[u];
      for (let i = neighbours.length - 1; i >= 0; i--) {
        const v = neighbours[i];
        if (!visited[v]) stack.push(v);
      }
    }
    return order;
  }

  unreachable(order) {
    const seen = new Set(order);
    const result = [];
    for (let v = 0; v < this.vertexCount; v++) {
      if (!seen.has(v)) result.push(v);
    }
    return result;
  }
}

async function main() {
  const rl = readline.createInterface({ input: stdin, output: stdout });

  const readInt = async (prompt, low, high) => {
    while (true) {
      const raw = (await rl.question(prompt)).trim();
      const value = raw === '' ? NaN : Number(raw);
      if (Number.isInteger(value) && value >= low && value <= high) return value;
      console.log(`Invalid. Enter an integer from ${low} to ${high}.`);
    }
  };

  const vertices = await readInt('Enter number of vertices V: ', 1, Number.MAX_SAFE_INTEGER);
  const edges = await readInt('Enter number of edges E: ', 0, Number.MAX_SAFE_INTEGER);

  const graph = new Graph(vertices);
  for (let i = 1; i <= edges; i++) {
    let u;
    let v;
    while (true) {
      const parts = (await rl.question(`Enter edge ${i} (u v): `)).trim().split(/\s+/).map(Number);
      if (parts.length === 2 && parts.every(Number.isInteger)) {
        [u, v] = parts;
        break;
      }
      console.log('Enter exactly two integers.');
    }
    if (!graph.addEdge(u, v)) {
      console.log(`Rejected: vertex out of range (valid range 0 to ${vertices - 1}).`);
    }
  }

  graph.sortNeighbours();
  console.log('\nAdjacency list:');
  graph.display();

  const source = await readInt(`\nEnter source vertex (0 to ${vertices - 1}): `, 0, vertices - 1);

  const bfsOrder = graph.bfs(source);
  console.log(`\nBFS traversal order: ${bfsOrder.join(' ')}`);
  for (const v of graph.unreachable(bfsOrder)) {
    console.log(`Vertex ${v} is not reachable from ${source} (BFS).`);
  }

  const dfsOrder = graph.dfs(source);
  console.log(`\nDFS traversal order: ${dfsOrder.join(' ')}`);
  for (const v of graph.unreachable(dfsOrder)) {
    console.log(`Vertex ${v} is not reachable from ${source} (DFS).`);
  }

  rl.close();
}

main();
