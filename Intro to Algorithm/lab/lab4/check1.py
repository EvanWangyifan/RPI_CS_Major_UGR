import random
import time

def div_conquer(arr, n):
    v = random.choice(arr)
    l = []
    m = []
    r = []
    for item in arr:
        if (item < v):
            l.append(item)
        elif (item==v):
            m.append(item)
        else:
            r.append(item)
    if (n<=len(l)):
        return div_conquer(l,n)
    elif (n>(len(l)+len(m))):
        return div_conquer(r,n-len(l)-len(m))
    else:
        return v
        #print("the value is: ",v)


arr = [7, 2, 4, 6, 9, 11, 2, 6, 10, 6, 15, 6, 14, 2, 7, 5, 13, 9, 12, 15]
t0 = time.time()
print(div_conquer(arr,10))
t1 = time.time()
print("time taken: ",t1-t0)

'''
l = []
for i in range(0, 10**7):
    l.append(random.randint(0, 10**5))
t2 = time.time()
div_conquer(l, 0, len(l)-1)
t3 = time.time()
print("time taken: ", t3-t2)
print(l[10])
'''