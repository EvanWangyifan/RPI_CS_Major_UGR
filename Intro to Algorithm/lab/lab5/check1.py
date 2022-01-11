import math
import random

def find_lps(arr):
    #initialize the match
    row = []
    result = []
    match = []
    for i in range(len(arr)):
        row.append(0)
    for i in range(len(arr)):
        match.append(row.copy())
    for i in range(len(arr)):
        match[i][i] = 1

    for i in range(2, len(arr)+1):  # i is the leng of the sequence
        temp = len(arr)-i+1
        for j in range(temp):
            start = j+i-1
            if arr[j] == arr[start] and i == 2:
                match[j][start] = 2
                if match[j][start] > len(result):
                    result.insert(0, arr[j])
                    result.append(arr[start])
            elif arr[j] == arr[start]:
                match[j][start] = match[j+1][start-1]+2
                if match[j+1][start-1] == 1 and match[j][start] > len(result):
                    result.append(arr[start-1])
                    result.insert(0, arr[j])
                    result.append(arr[start])
                elif match[j][start] > len(result):
                    result.insert(0, arr[j])
                    result.append(arr[start])
            else:
                match[j][start] = max(match[j][start-1], match[j+1][start])
    return result

arr = [7, 2, 4, 6, 9, 11, 2, 6, 10, 6, 15, 6, 14, 2, 7, 5, 13, 9, 12, 15]

print(find_lps(arr))
rand = []
for i in range(0, 1000):
    num = random.randint(0, 100)
    rand.append(num)
print(find_lps(rand))

