import math as m

readIn = open("mapData.txt", "r")


xPos = []
yPos = []
conns = []

maxConnCount = 0

xMin = -1
xMax = -1

yMin = -1
yMax = -1

for line in readIn.readlines():
	splitLine = line.split(' ')

	xFoo = int(splitLine[1])
	xPos.append(xFoo)
	if xFoo > xMax or xMax==-1: xMax = xFoo
	if xFoo < xMin or xMin==-1: xMin = xFoo

	yFoo = int(splitLine[2])
	yPos.append(yFoo)
	if yFoo > yMax or yMax==-1: yMax = yFoo
	if yFoo < yMin or yMin==-1: yMin = yFoo

	fooConnCount = int(splitLine[3])

	if fooConnCount > maxConnCount: maxConnCount = fooConnCount

	fooConns = []
	for i in range(fooConnCount):
		fooConns.append(int(splitLine[4 +i]))


	conns.append(fooConns)

readIn.close()

print(xPos)
print(yPos)
print(conns)





if xMax - xMin > yMax - yMin:
	scale = 255/(xMax - xMin)
else:
	scale = 255/(yMax - yMin)

for i in range(len(xPos)):
	xPos[i] = m.floor((xPos[i] - xMin) * scale)
	yPos[i] = m.floor((yPos[i] - yMin) * scale)



print(xPos)
print(yPos)




printOut = open("LedMap.h", "w")

readTemplate = open("LedMapTemplate.h")

while True:
	line = readTemplate.readline()
	if line == "//INSERT\n": break

	printOut.write(line)

printOut.write(f"#define NUM_LEDS {len(xPos)}\n")
printOut.write(f"const uint16_t nodeCount = {len(xPos)};\n")

printOut.write("const uint8_t xPos[] = {")
for x in xPos: printOut.write(f"{str(x).rjust(3, ' ')},")
printOut.write("};\n")

printOut.write("const uint8_t yPos[] = {")
for y in yPos: printOut.write(f"{str(y).rjust(3, ' ')},")
printOut.write("};\n")


for line in readTemplate.readlines():
	printOut.write(line)


	# printOut.write(str(splt[1]).ljust(5, ' '))
	# printOut.write(",")
	# printOut.write(str(splt[2]).ljust(5, ' '))
	# printOut.write("\n")

printOut.close()