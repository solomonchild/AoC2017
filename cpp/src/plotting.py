import matplotlib.pyplot as plt
import re

with open("out.txt", "r") as f:

    pat1 = []
    for line in f.readlines():
        mtch = re.match("(\d+),(\d+)", line) 
        if mtch:
            pat1.append((mtch.group(1), mtch.group(2)))
    plt.plot([x for x,_ in pat1], [y for _,y in pat1], 'r.')
    plt.show()
