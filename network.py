# Cayden Knight
# 12/6/2023
# Colab wit the goat Musa
import sys
from collections import defaultdict, deque
from heap import heap

def broadcast(src, graph, ttl):
    stack = deque([(src, ttl)])
    prev = {src: None}
    
    while stack:
        node, current_ttl = stack.pop()
        if current_ttl > 0:
            for neighbor in graph[node]:
                if neighbor not in prev:
                    prev[neighbor] = node
                    stack.append((neighbor, current_ttl - 1))
    
    return prev

def direct(src, graph):
    # Initialize empty lists for heap creation
    nodes = [src]
    distances = [0]
    queue = heap(nodes, distances)
    dist = {node: float('inf') for node in graph}
    dist[src] = 0
    prev = {}

    while not queue.is_empty():
        d, node = queue.pop()  # Get the node with the minimum distance
        for neighbor, weight in graph[node].items():
            new_dist = dist[node] + weight
            if new_dist < dist[neighbor]:
                dist[neighbor] = new_dist
                prev[neighbor] = node
                # Update the heap with new distance
                if neighbor in nodes:
                    idx = nodes.index(neighbor)
                    distances[idx] = new_dist
                else:
                    nodes.append(neighbor)
                    distances.append(new_dist)
                queue = heap(nodes, distances)  # Re-create the heap

    return prev




def mst(src, graph):
    # Prims to find MST -> already has a path
    node = []
    cost = []
    res = defaultdict(dict)
    for n in graph.keys():
        node.append(n)
        if n == src:
            cost.append(0)
        else:
            cost.append(float('inf'))

    q = heap(node, cost)
    while not q.is_empty():
        u = q.pop()[1]
        for w in graph[u].keys():
            c = graph[u][w]  # getting the relative cost from parent to destination
            if c < q.prio(w):
                q.decrease_key(w, c)
                res[w] = u
    return res



def print_paths(graph, prev, msg, src):
    for v in graph.keys():
        if v == src:
            continue
        if not v in prev:
            print("Could not find path to", v)
            continue
        path = v
        cost = 0
        u = v
        while u != src:
            past = u
            u = prev[u]
            cost += graph[u][past]
            path = u + " " + path
        print(v, "received", msg, "along path", path, "with cost", cost)

if __name__ == "__main__":
    graph = defaultdict(dict)
    
    infile = open(sys.argv[1], "r")

    for line in infile:
        data = line.split(" ")
        start = data[0]
        end = data[1]
        cost = int(data[2])

        graph[start][end] = cost
        graph[end][start] = cost
        
    cmd = input()
    while cmd != "exit":
        print("Command:", cmd)
        method, msg, src = cmd.split(" ")[:3]
        prev = None
        if method == "broadcast":
            ttl = int(cmd.split(" ")[-1])
            prev = broadcast(src, graph, ttl)
        elif method == "direct":
            prev = direct(src, graph)
        elif method == "MST":
            prev = mst(src, graph)
        else:
            print("Invalid command!")
        if prev != None:
            print_paths(graph, prev, msg, src)
        cmd = input()
    print("Goodbye!")
