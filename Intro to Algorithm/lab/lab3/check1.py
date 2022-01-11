import random

def Graph(n, p):
    result = []
    for i in range(n):
        l = []
        for j in range(n):
            if i == j:
                l.append(0)
                continue
            if j < len(result):
                l.append(result[j][i])
                continue
            if (random.random() < p):
                l.append(1)
            else:
                l.append(0)
        result.append(l)
    return result

def print_graph(graph):
    for i in range(len(graph)):
        print(graph[i])


num = int(input("Give an integer which is greater than 1 => "))
pro = float(input("Give the probability => "))
g = Graph(num, pro)
print_graph(g)

#############################################################
# Check 2
#############################################################


def BFS(graph, s):
    visited, stack = set(), [s]
    while stack:
        vertex = stack.pop()
        if vertex not in visited:
            visited.add(vertex)
            for i in range(len(graph[vertex])):
                if(graph[vertex][i] == 1 and i not in visited):
                    stack.append(i)
    return visited


def get_largest(graph, t):
    allsize = []
    for i in range(len(graph)):
        vi = BFS(graph, i)
        #print(vi)
        allsize.append(len(vi))
    if (max(allsize) >= t):
        return 1
    return 0


def make_graph(n, p):
    c = 2.0
    p = c/n
    array = []

    for i in range(0, n):
        array.append([0]*n)

    for i in range(0, n):
        for j in range(0, n):
            if i != j:
                if array[i][j] != 1:
                    if random.random() <= p:
                        array[i][j] = 1
                        array[j][i] = 1

    return array


x = 0
for i in range(500):
    array = make_graph(40, 0.2)
    result = get_largest(array, 30)
    if (result == 1):
        x = x+1
print(x/500)
'''
for i in range(len(array)):
    for j in range(len(array[i])):
        print(array[i][j], end = ' ')
    print()
result = get_largest(array, 5)
print(result)
'''
