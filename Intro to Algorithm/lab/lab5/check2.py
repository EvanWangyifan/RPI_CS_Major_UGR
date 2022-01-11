
def greedy_sort(data):
    temp = []
    temp2 = []
    for i in data:
        temp.append(i[1]/i[0])
        temp2.append([i[1]/i[0], i[0], i[1]])
    new = []
    count = 0
    while count<len(data):
        t = max(temp)
        for m in temp2:
            if m[0] == t:
                new.append([m[1],m[2]])
        temp.remove(t)
        count+=1
    return new
    

def greedy(data,total):
    ndata = greedy_sort(data)
    result = []
    now = 0
    for i in ndata:
        if i[0]>(total-now):
            continue
        else:
            result.append(i[1])
            now += i[0]
    return sum(result)

data = [[96, 91], [96, 92], [96, 92], [97, 94], [98, 95], [100, 94], [100, 96], [102, 97], [103, 97], [104, 99], [
    106, 101], [107, 101], [106, 102], [107, 102], [109, 104], [109, 106], [110, 107], [111, 108], [113, 107], [114, 110]]
test = [[10, 60], [20, 100], [30, 120]]
print(greedy(data, 100))
print(greedy(data, 200))
print(greedy(data, 300))


def dynamic(data, total):
    row = []
    result = []
    for i in range(total+1):
        row.append(0)
    for i in range(len(data)+1):
        result.append(row.copy())
    for i in range(len(data)+1):
        for j in range(total+1):
            if data[i-1][0] <= j:
                result[i][j] = max(data[i-1][1]+result[i-1]
                                  [j-data[i-1][0]], result[i-1][j])
            else:
                result[i][j] = result[i-1][j]
    print(result[len(data)][total])


dynamic(data, 100)
dynamic(data, 200)
dynamic(data, 300)
