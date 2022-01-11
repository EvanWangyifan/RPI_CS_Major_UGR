# Checkpoint 1
def binary_search(arr,left,right,target):
    while left <= right:
        mid = (left + right)//2
        if arr[mid] == target:
            return mid
        elif arr[mid] < target:
            left = mid + 1
        else:
            right = mid - 1
    return -1

def find_missing(arr):
    arr.sort()
    length = len(arr)
    ideal_sum = (arr[0]+arr[length-1])*(length+1)/2
    real_sum = sum(arr)
    return int(ideal_sum-real_sum)
    

elements1 = [2,4,6,8,10]
elements2 = [1,2,3,4,5,6,6,6]

print(binary_search(elements1,0,len(elements1)-1,6))
print(binary_search(elements1,0,len(elements1)-1,10))
print(binary_search(elements1,0,len(elements1)-1,3))
print(binary_search(elements1,0,len(elements1)-1,9))
print(binary_search(elements1,0,len(elements1)-1,100))
print(binary_search(elements1,0,len(elements1)-1,0))
print()

print(binary_search(elements2,0,len(elements2)-1,6))
print(binary_search(elements2,0,len(elements2)-1,4))
print(binary_search(elements2,0,len(elements2)-1,5))
print(binary_search(elements2,0,len(elements2)-1,0))
print()

unsorted = [2,3,6,1,5,7]

print(find_missing(unsorted))