<?php

class Graph
{
    private int $n;
    private array $adj;

    public function __construct(int $n)
    {
        $this->n = $n;
        $this->adj = array_fill(0, $n, []);
    }

    public function valid(int $v): bool
    {
        return $v >= 0 && $v < $this->n;
    }

    public function addEdge(int $u, int $v): bool
    {
        if (!$this->valid($u) || !$this->valid($v)) {
            return false;
        }
        $this->adj[$u][] = $v;
        $this->adj[$v][] = $u;
        return true;
    }

    public function sortNeighbours(): void
    {
        foreach ($this->adj as &$list) {
            sort($list);
        }
        unset($list);
    }

    public function display(): void
    {
        for ($v = 0; $v < $this->n; $v++) {
            $line = "$v:";
            foreach ($this->adj[$v] as $x) {
                $line .= " -> $x";
            }
            echo $line, PHP_EOL;
        }
    }

    public function bfs(int $src): array
    {
        $visited = array_fill(0, $this->n, false);
        $queue = [$src];
        $head = 0;
        $order = [];
        $visited[$src] = true;
        while ($head < count($queue)) {
            $u = $queue[$head++];
            $order[] = $u;
            foreach ($this->adj[$u] as $v) {
                if (!$visited[$v]) {
                    $visited[$v] = true;
                    $queue[] = $v;
                }
            }
        }
        return $order;
    }

    public function dfs(int $src): array
    {
        $visited = array_fill(0, $this->n, false);
        $stack = [$src];
        $order = [];
        while ($stack) {
            $u = array_pop($stack);
            if ($visited[$u]) {
                continue;
            }
            $visited[$u] = true;
            $order[] = $u;
            foreach (array_reverse($this->adj[$u]) as $v) {
                if (!$visited[$v]) {
                    $stack[] = $v;
                }
            }
        }
        return $order;
    }

    public function unreachable(array $order): array
    {
        $seen = array_fill(0, $this->n, false);
        foreach ($order as $v) {
            $seen[$v] = true;
        }
        $res = [];
        for ($v = 0; $v < $this->n; $v++) {
            if (!$seen[$v]) {
                $res[] = $v;
            }
        }
        return $res;
    }
}

function readLineOrExit(): string
{
    $line = fgets(STDIN);
    if ($line === false) {
        exit(1);
    }
    return trim($line);
}

function isInt(string $s): bool
{
    return preg_match('/^-?\d+$/', $s) === 1;
}

function readInt(string $prompt, int $low, int $high): int
{
    while (true) {
        echo $prompt;
        $raw = readLineOrExit();
        if (isInt($raw)) {
            $value = (int)$raw;
            if ($value >= $low && $value <= $high) {
                return $value;
            }
        }
        echo "Invalid. Enter an integer from $low to $high.", PHP_EOL;
    }
}

$vertices = readInt("Enter number of vertices V: ", 1, 1000000);
$edges = readInt("Enter number of edges E: ", 0, 1000000);

$graph = new Graph($vertices);
for ($i = 1; $i <= $edges; $i++) {
    while (true) {
        echo "Enter edge $i (u v): ";
        $parts = preg_split('/\s+/', readLineOrExit(), -1, PREG_SPLIT_NO_EMPTY);
        if (count($parts) === 2 && isInt($parts[0]) && isInt($parts[1])) {
            $u = (int)$parts[0];
            $v = (int)$parts[1];
            break;
        }
        echo "Enter exactly two integers.", PHP_EOL;
    }
    if (!$graph->addEdge($u, $v)) {
        echo "Rejected: vertex out of range (valid range 0 to ", $vertices - 1, ").", PHP_EOL;
    }
}

$graph->sortNeighbours();
echo PHP_EOL, "Adjacency list:", PHP_EOL;
$graph->display();

$source = readInt("\nEnter source vertex (0 to " . ($vertices - 1) . "): ", 0, $vertices - 1);

$bfs = $graph->bfs($source);
echo PHP_EOL, "BFS traversal order: ", implode(' ', $bfs), PHP_EOL;
foreach ($graph->unreachable($bfs) as $v) {
    echo "Vertex $v is not reachable from $source (BFS).", PHP_EOL;
}

$dfs = $graph->dfs($source);
echo PHP_EOL, "DFS traversal order: ", implode(' ', $dfs), PHP_EOL;
foreach ($graph->unreachable($dfs) as $v) {
    echo "Vertex $v is not reachable from $source (DFS).", PHP_EOL;
}
