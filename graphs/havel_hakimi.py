# Ze souboru stupnu urci jestli je to validni graf nebo nikoliv.

for x in range(3, 10):
	degrees = [2,4,5,5,7,8,8,9,10,10]
	degrees.append(x)
	degrees.sort()
	print("Solving for list", degrees)
	while len(degrees) >= 2:
		for i in range(len(degrees) - 1 - degrees[-1], len(degrees) - 1):
			degrees[i] -= 1
		degrees.pop()
		degrees = list(filter(lambda a: a != 0, degrees))
		degrees.sort()
		print(degrees)
		if len(degrees) != 0 and (len(degrees) - 1 < degrees[-1] or degrees[0] < 0):
			print("Not valid graph.")
			break
		elif (len(degrees)) == 0:
			print("Valid graph.")
			break
	print("--------------")