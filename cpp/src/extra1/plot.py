import matplotlib.pyplot as plt
import re

with open("out.txt", "r") as f:
#(8, 11) (1, 24) 
    pat = []
    for line in f.readlines():
        mtch = re.match("(\d+),(\d+)", line) 
        if mtch:
            pat.append((mtch.group(1), mtch.group(2)))
    y_max = max([int(y) for _,y in pat])
    plt.plot([x for x,_ in pat], [y_max - int(y) for _,y in pat], 'r.')
    plt.show()
    
