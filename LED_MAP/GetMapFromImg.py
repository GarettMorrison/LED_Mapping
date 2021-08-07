from PIL import Image
import math as m
import os
import sys
import subprocess as sp
import time



inImg = Image.open("map.png").convert('RGB')
inPix = inImg.load()
iW, iH = inImg.size

dataArr = []


# dataArr key:
# 0: null
# 1: orange, start pos
# 2: red, node
# 3: teal, wire and adjacent
# 4: blue, just wire
# 5: green, just adjacent
#10+: Part of set node, n-10 indicates node ID

startPts = []

for x in range(iW):
	dataArr.append([])
	for y in range(iH):
		fooPix = inPix[x, y]

		dataArr[x].append(0)

		if fooPix[0] == 255: #Red

			# print(fooPix)
			if fooPix[1] == 127: #Orange, start node
				dataArr[x][y] = 1
				startPts.append((x,y))
			else:	#Just red, node
				dataArr[x][y] = 2

		elif fooPix[2] == 255: #Blue 
			if fooPix[1] == 255: #Teal, adjacent & wire
				dataArr[x][y] = 3
			else: #Just Blue, just wire
				dataArr[x][y] = 4

		elif fooPix[1] == 255: #Just green
			dataArr[x][y] = 5

inImg.close()


nodes = []
#index 0 is ID
#2 is x
#3 is y
#4 is set of adjacent nodes

setOfAdjacents = ((0,1), (0,-1), (1,0), (-1,0))

def ptOk(pos):
	global iW, iH
	if x < 0 or x >= iW: return False
	if y < 0 or y >= iH: return False	

	return True


# print(startPts)

checkedMap = []

for x in range(iW):
	checkedMap.append([])
	for y in range(iH):
		checkedMap[x].append(-1)


print("Starting loop")

while len(startPts) > 0: #Loop until no next point found
	nodeID = len(nodes)
	# print(nodes)
	nodes.append([nodeID])

	print(f"Working on Node {nodeID} Len {len(nodes[nodeID])}")

	nextNodePts = []

	bothPts = []
	wirePts = []
	adjaPts = []

	adjacentNodes = []

	startPtsCounted = 0
	nodeX = 0
	nodeY = 0

	while len(startPts) > 0: #Loop starting points, get initial area
		# print(f"Checking Node {nodeID} in startPts Len {len(nodes[nodeID])}")

		pos = startPts.pop(0)

		dataArr[pos[0]][pos[1]] = nodeID +10

		# print(f"x:{nodeX} y:{nodeY}")

		nodeX = (nodeX*startPtsCounted	+ pos[0])/(startPtsCounted +1)
		nodeY = (nodeY*startPtsCounted	+ pos[1])/(startPtsCounted +1)
		startPtsCounted += 1

		for offset in setOfAdjacents:
			x = pos[0] + offset[0]
			y = pos[1] + offset[1]

			if not ptOk((x,y)): continue

			if checkedMap[x][y] == nodeID: continue
			else:	checkedMap[x][y] = nodeID

			fooPt = dataArr[x][y]

			if fooPt == 2 and fooPt not in startPts:	startPts.append((x,y))

			if fooPt == 3 and fooPt not in bothPts:	bothPts.append((x,y))

			if fooPt == 4 and fooPt not in wirePts:	wirePts.append((x,y))

			if fooPt == 5 and fooPt not in adjaPts:	adjaPts.append((x,y))



	nodes[nodeID].append(m.floor(nodeX))
	nodes[nodeID].append(m.floor(nodeY))


	startPts = [] #Reset for next loop

	#Loop and find next pt

	while len(bothPts) > 0:	#Check bothPts
		# print(f"Checking Node {nodeID} in bothPts Len {len(nodes[nodeID])}")

		pos = bothPts.pop(0)

		for offset in setOfAdjacents:
			x = pos[0] + offset[0]
			y = pos[1] + offset[1]

			if not ptOk((x,y)): continue

			if checkedMap[x][y] == nodeID: continue
			else:	checkedMap[x][y] = nodeID

			fooPt = dataArr[x][y]


			if fooPt == 2 and fooPt not in startPts:	startPts.append((x,y))

			if fooPt == 3 and fooPt not in bothPts:	bothPts.append((x,y))

			if fooPt == 4 and fooPt not in wirePts:	wirePts.append((x,y))

			if fooPt == 5 and fooPt not in adjaPts:	adjaPts.append((x,y))

			if fooPt >= 10:
				if fooPt -10 not in adjacentNodes: adjacentNodes.append(fooPt -10)
				if nodeID not in nodes[fooPt -10][3]: nodes[fooPt -10][3].append(nodeID)



				#NO DOUBLE SAVE DETECTION FUCK AAAAAAAAAAAAAA
	while len(wirePts) > 0:	#Check just wire
		# print(f"Checking Node {nodeID} in wirePts Len {len(nodes[nodeID])}")
		pos = wirePts.pop(0)

		for offset in setOfAdjacents:
			x = pos[0] + offset[0]
			y = pos[1] + offset[1]

			if not ptOk((x,y)): continue

			if checkedMap[x][y] == nodeID: continue
			else:	checkedMap[x][y] = nodeID

			fooPt = dataArr[x][y]

			if fooPt == 2 and fooPt not in startPts:	startPts.append((x,y))

			if (fooPt == 4 or fooPt == 3) and fooPt not in wirePts:	wirePts.append((x,y))

	while len(adjaPts) > 0:	#Check just adja
		# print(f"Checking Node {nodeID} in adjaPts Len {len(nodes[nodeID])}")

		pos = adjaPts.pop(0)

		for offset in setOfAdjacents:
			x = pos[0] + offset[0]
			y = pos[1] + offset[1]

			if not ptOk((x,y)): continue

			if checkedMap[x][y] == nodeID: continue
			else:	checkedMap[x][y] = nodeID

			fooPt = dataArr[x][y]

			if (fooPt == 3 or fooPt == 5) and fooPt not in adjaPts:	adjaPts.append((x,y))

			if fooPt >= 10:
				if fooPt -10 not in adjacentNodes: adjacentNodes.append(fooPt -10)
				if nodeID not in nodes[fooPt -10][3]: nodes[fooPt -10][3].append(nodeID)

	nodes[nodeID].append(adjacentNodes)

	print(f"\nOn Node {nodeID}   -----\n")
	for fooNode in nodes:
		print(f"Node:{fooNode[0]}")
		print(f"   X:{fooNode[1]}")
		print(f"   Y:{fooNode[2]}")
		print(f"   adj:{fooNode[3]}")


	# for i in nodes: print(i)
	# print(nodes[nodeID])
	# print(startPts)


	oImg = Image.new("RGB", inImg.size, (0,0,0))
	oPix = oImg.load()
	for x in range(iW):
		for y in range(iH):
			if checkedMap[x][y] == -1: continue
			foo = m.floor(255*(checkedMap[x][y]+6)/(len(nodes) +5))
			oPix[x,y] = (foo,foo,foo)


	for pt in nodes:
		# print(pt)
		oPix[pt[1], pt[2]] = (255, 0, 0)
	oImg.save("demo.png")
	oImg.save(f"DemoDump/out_{nodeID}.png")


outData = open("mapData.txt", "w")

for pt in nodes:
	outData.write(f"{pt[0]} {pt[1]} {pt[2]} {len(pt[3])}")
	for adj in pt[3]:
		outData.write(f" {adj}")
	outData.write("\n")

