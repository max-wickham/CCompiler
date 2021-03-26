import sys
import re

inputData = []
typeDefs = {}
run = False
#first read the lines into an array
for line in sys.stdin:
    inputData.append(line)
    if "typedef" in line:
        run = True
#read through each line and check if it has a type def and if so add the type def to teh dictionary
types = ["int",
        "unsigned int",
        "long int",
        "long",
        "char",
        "unsigned char",
        "float",
        "unsigned float",
        "double",
        "unsigned double",]
if run:
    for typeKey in types:
        prog = re.compile("typedef(\s|\t)+" + typeKey + "(\s|\t)+[_a-zA-Z][_a-zA-Z0-9]*")
        for line in inputData:
            result = prog.search(line)
            if result:
                data = line[result.regs[0][0]:result.regs[0][1]]
                data = data.replace(" ","")
                data = data.replace("   ","")
                a = typeKey
                a = a.replace(" ","")
                a = a.replace("   ","")
                #print(a)
                typeDefs[data[7 + len(a):]] = typeKey
                #print(data[7 + len(typeKey):])
    #replace the instances of the type def in the file and print out
    #print(typeDefs)
    for typeKey in typeDefs.keys():
        for j in range(len(inputData)):
            line = inputData[j]
            if "typedef" in line:
                inputData[j] = ""
            else:
                x = True
                while x:
                    x = False
                    length = len(typeKey)
                    for i in range((len(line)-length-1)):
                        #print(line[i:i+length]+ "---" + typeKey)
                        if inputData[j][i:i+length] == typeKey:
                            if i == 0:
                                match1 = True
                            else:
                                match1 = re.match("\s|\t|\(|\{",line[i-1])
                            match2 = re.match("\s|\t|\(|\{",line[i+length])
                            if match1 and match2:
                                x = True
                                inputData[j] = line[:i] + typeDefs[typeKey] + line[i+length:]
                                #print(inputData)
                                break
                #inputData[i] = line.replace(typeKey,typeDefs[typeKey])

for data in inputData:
    print(data,end="")