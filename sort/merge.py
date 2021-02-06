import random

def test_sort(sort, list):
	start = time.time()
	sort(list)
	end = time.time()
	return end - start

#-------- Quicksort -------------

def median_of_three(list):
	x1 = list[0]
	x2 = list[(len(list) - 1) // 2] 
	x3 = list[len(list) - 1]
	return x1 + x2 + x3 - min(x1, x2, x3) - max(x1, x2, x3)

def quicksort(list):
	if not list:
		return list
	pivot  = median_of_three(list)
	lower  = quicksort([x for x in list if x < pivot])
	same   = [x for x in list if x == pivot]
	higher = quicksort([x for x in list if x > pivot])
	return lower + same + higher


# -------  Merge sort  ---------

def merge(l1, l2):
	res = []
	l1_it = 0
	l2_it = 0
	while l1_it < len(l1) or l2_it < len(l2):
		if(len(l1) == l1_it):
			return res + l2[l2_it:]
		elif(len(l2) == l2_it):
			return res + l1[l1_it:]
		elif(l1[l1_it] < l2[l2_it]):
			res.append(l1[l1_it])
			l1_it += 1
		else:
			res.append(l2[l2_it])
			l2_it += 1
	return res 

def merge_sort(l):
	if(len(l) <= 1):
		return l
	left = merge_sort(l[:len(l) // 2])
	right = merge_sort(l[len(l) // 2:])
	return merge(left, right)

#-------- Insert sort ----------

def insert_sort(l):
	res = l.copy()
	for i in range(len(res)):
		smallest = i
		for j in range(i, len(res)):
			if(res[smallest] > res[j]):
				smallest = j
		res[smallest], res[i] = res[i], res[smallest]
	return res






############################### Unit tests #########################################
for sort in [quicksort, insert_sort, merge_sort]:
	print("Testing {}".format(sort.__name__))
	assert sort([1,2,3]) == [1,2,3]
	assert sort([3,2,1]) == [1,2,3]
	assert sort([]) == []
	assert sort([3,2,16,4,3,6,4,3,5,2,4,3]) == [2,2,3,3,3,3,4,4,4,5,6,16]
	print("{} tests were successfull\n".format(sort.__name__))





############################## Benchmarks ##########################################

import time
list = [random.randint(0, 10000) for x in range(0,10000)]

res = test_sort(insert_sort,list)
print("Insert sort on {} items: {:.2f}s".format(len(list), res))

list = [random.randint(0, 10000) for x in range(0,1000000)]

res = test_sort(merge_sort, list)
print("Merge sort on {} items: {:.2f}s".format(len(list), res))

res = test_sort(quicksort, list)
print("Quicksort on {} items: {:.2f}s".format(len(list), res))

res = test_sort(sorted, list)
print("Python sort function on {} items: {:.2f}s".format(len(list), res))
