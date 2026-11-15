$stdout.sync = true

class Graph
  attr_reader :n

  def initialize(n)
    @n = n
    @adj = Array.new(n) { [] }
  end

  def valid?(v)
    v >= 0 && v < @n
  end

  def add_edge(u, v)
    return false unless valid?(u) && valid?(v)
    @adj[u] << v
    @adj[v] << u
    true
  end

  def sort_neighbours
    @adj.each(&:sort!)
  end

  def display
    @n.times { |v| puts "#{v}:" + @adj[v].map { |x| " -> #{x}" }.join }
  end

  def bfs(src)
    visited = Array.new(@n, false)
    queue = [src]
    visited[src] = true
    order = []
    until queue.empty?
      u = queue.shift
      order << u
      @adj[u].each do |v|
        next if visited[v]
        visited[v] = true
        queue << v
      end
    end
    order
  end

  def dfs(src)
    visited = Array.new(@n, false)
    stack = [src]
    order = []
    until stack.empty?
      u = stack.pop
      next if visited[u]
      visited[u] = true
      order << u
      @adj[u].reverse_each { |v| stack.push(v) unless visited[v] }
    end
    order
  end

  def unreachable(order)
    seen = Array.new(@n, false)
    order.each { |v| seen[v] = true }
    (0...@n).reject { |v| seen[v] }
  end
end

def read_int(prompt, low, high)
  loop do
    print prompt
    value = Integer($stdin.gets.to_s.strip, exception: false)
    return value if value && value.between?(low, high)
    puts "Invalid. Enter an integer from #{low} to #{high}."
  end
end

vertices = read_int("Enter number of vertices V: ", 1, 1_000_000)
edges = read_int("Enter number of edges E: ", 0, 1_000_000)

graph = Graph.new(vertices)
1.upto(edges) do |i|
  loop do
    print "Enter edge #{i} (u v): "
    parts = $stdin.gets.to_s.split.map { |t| Integer(t, exception: false) }
    if parts.length == 2 && parts.none?(&:nil?)
      puts "Rejected: vertex out of range (valid range 0 to #{vertices - 1})." unless graph.add_edge(*parts)
      break
    end
    puts "Enter exactly two integers."
  end
end

graph.sort_neighbours
puts "\nAdjacency list:"
graph.display

source = read_int("\nEnter source vertex (0 to #{vertices - 1}): ", 0, vertices - 1)

bfs = graph.bfs(source)
puts "\nBFS traversal order: #{bfs.join(' ')}"
graph.unreachable(bfs).each { |v| puts "Vertex #{v} is not reachable from #{source} (BFS)." }

dfs = graph.dfs(source)
puts "\nDFS traversal order: #{dfs.join(' ')}"
graph.unreachable(dfs).each { |v| puts "Vertex #{v} is not reachable from #{source} (DFS)." }
