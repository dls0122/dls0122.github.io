from os import listdir 

def disconnect(line,link,verticality=True):
    x=line
    y=x
    x1=link[0][0]
    y1=link[0][1]
    x2=link[1][0]
    y2=link[1][1]
    if verticality: 
        if(x<x1 and x>x2)or (x>x1 and x<x2):
            return True
        else:
            return False
    else:
        if (y<y1 and y>y2) or (y>y1 and y<y2):
            return True
        else:
            return False

def function(line,lineCur,vertical):
    cut = 0
    cutMax = 0
    for link in links:
        if link!=None:
            if disconnect(line, link, verticality=vertical):
                cut = cut + 1
    if (cut >= cutMax):
        cutMax = cut
        lineCur = line
    return(cutMax)

def findlinewithmostcuts(links,left,right,down,up):
    cutMax=0
    lineCur_h=0
    lineCur_v = 0
    lineCur = 0
    line_orientation ="h"
    h_cutmax = 0
    v_cutmax = 0
    h1 = down +0.5
    v1 = left + 0.5
    while (True):
        if h1 > up:
            break
        h_line_cutmax = function(h1, lineCur, vertical=False)
        if h_cutmax<h_line_cutmax:
            h_cutmax = h_line_cutmax
            lineCur_h = h1
        h1 = h1 + 1

    while(True):
        if v1 > right:
            break
        v_line_cutmax = function(v1,lineCur,vertical=True)
        if v_cutmax < v_line_cutmax:
            v_cutmax = v_line_cutmax
            lineCur_v = v1
        v1 = v1 +1
    lineCur = lineCur_h
    cutMax = h_cutmax
    if h_cutmax <= v_cutmax:
        cutMax = v_cutmax
        line_orientation = "v"
        lineCur = lineCur_v
    return(lineCur,cutMax,line_orientation)



if __name__ == '__main__':
    directory = listdir("input")  
    for t in directory:  
        points = []  
        fread = open("input/" + t, 'r')  
        fread.readline()  
        for L in fread:
            pnt = L.split(" ")
            points.append([int(pnt[0]), int(pnt[1])])

        res = []  
        links = []  
        left, right, up, down = 0, 0, 0, 0  

        for p in points:
            left = min(left, p[0])  
            right = max(right, p[1])  
            up = max(up, p[1])
            down = min(down, p[0])

        for i,p1 in enumerate(points):
            for j in range(i,len(points)):
                if p1!=points[j]:
                    links.append((p1, points[j])) 

        sum =0
        while((len(links)-sum)!= 0):
            line,cutMax,line_orientation=findlinewithmostcuts(links,left,right,down,up)
            res.append((line,line_orientation))

            if line_orientation == 'v':
                vertical = True
            else:
                vertical = False

            sum = sum + cutMax
            for s in range(len(links)):
                if links[s] != None:
                    if disconnect(line,links[s],verticality=vertical):
                        links[s] = None


        t=t.split(".")
        solnfiles = "greedyoutput/greedy_solution" + t[0][-2]+t[0][-1]+'.txt'
        f=open(solnfiles,'w')
        f.write("%d \n"%(len(res)))
        for line in res:
            f.write("%s %f\n"%(line[1],line[0]))
        f.close()