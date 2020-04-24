#!/usr/bin/python3
import sys
import os
try:
    import matplotlib.pyplot as plt
except:
    os.system("pip install matplotlib")
    try:
        import matplotlib.pyplot as plt
    except:
        print("Instale matplot para python3")
        os._exit(-1)

xCores = [1,2,4,6,8]
yTiempo = [0.54, 0.67, 0.98, 0.78, 0.45]

plt.plot( xCores , yTiempo )
plt.show()

