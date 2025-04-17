import socket
import matplotlib.pyplot as plt
import numpy as np

s = socket.create_connection(("192.168.1.1", 288))
x = []
y = []

while(True):
    data = s.recv(1024).decode()
    
    if(data == "help"):
        break
    
    pair = data.split(',')
    x.append(int(pair[0]))
    y.append(int(pair[1]))

plt.scatter(np.array(x), np.array(y))
plt.show()

s.close()