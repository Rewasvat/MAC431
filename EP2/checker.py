#!/usr/bin/python

import sys

def GetDataLines(fileName):
    f = open(fileName, "r")
    l = f.readlines()
    f.close()
    return l
    
def Map(data):
    d = []
    hc = ""
    author = ""
    totalAdd = 0
    totalRem = 0
    lineCount = 0
    for line in data:
        lineCount += 1
        if line == "\n":    continue
        params = line.split()
        n = len(params)
        if n == 2:
            if hc != "":
                total = totalAdd + totalRem;
                d.append( ("modlinesauthor_"+author, total) )
                d.append( ("modlinescommit_"+hc, total) )
                d.append( ("numcommits_"+author, 1) )
            
            hc, author = params
            totalAdd = totalRem = 0
        elif n == 3:
            ladd, lrem, fileName = params
            if ladd != "-": ladd = int(ladd)
            else:   ladd = 0
            if lrem != "-": lrem = int(lrem)
            else:   lrem = 0
            totalAdd += ladd
            totalRem += lrem
            d.append( ("modlinesfile_"+fileName, ladd+lrem ) )
        else:
            print "WRONG DATA FORMAT (line#%s): %s" % (lineCount, line)
            return []
    if hc != "":
        total = totalAdd + totalRem;
        d.append( ("modlinesauthor_"+author, total) )
        d.append( ("modlinescommit_"+hc, total) )
        d.append( ("numcommits_"+author, 1) )
    return d
    
def Reduce(pairList):
    d = {}
    for key, value in pairList:
        if d.has_key(key):
            d[key] += value
        else:
            d[key] = value
    return d

def Execute(argList):
    if len(argList) != 1:
        print "Wrong program call. Use:   ./checker.py <input data file>"
        return
        
    data = GetDataLines(argList[0])
    pairList = Map(data)
    stats = Reduce(pairList)
    
    items = stats.items()
    items.sort()
    for key, value in items:
        print "%s\t%s" % (key, value)

if __name__ == "__main__":
    Execute( sys.argv[1:] )
