arr1 = ['r', 'b', 'r', 'r', 'b', 'r', 'b']
arr2 = ['b', 'r', 'b', 'b', 'r', 'b', 'r', 'r']

def swap(arr, i, j):
    x = arr[j]
    arr[j] = arr[i]
    arr[i] = x

def rearrange(arr):
    left = 0
    right = len(arr)-1
    while (left!=right) and (left!=right-1):
        while (arr[left] != 'b') and (left < right):
            left+=1
        while (arr[right] != 'r') and (left < right):
            right-=1
        swap(arr,left,right)
    return arr

print(rearrange(arr1))
print(rearrange(arr2))
