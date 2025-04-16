# -*- coding: utf-8 -*-
#use python3
import numpy as np
import matplotlib.pyplot as plt

samples = 16
Vmax = 2047

s_sin_full = np.zeros(samples)


# Sinusoidal Assembly
# Full wave Sinusoidal Assembly
for i in range(np.size(s_sin_full)):
    # full wave rectified sinusoidal
    s_sin_full[i] = np.sin(2*np.pi*(i+1)/samples) * Vmax
        
s_sin_full = s_sin_full.astype(int)

### print on C code format ###
qtty_per_line = 10
    
print ()
print ()
print ('--- SINUSOIDAL FULL WAVE ---')
line = 1
print ("{",end='')
for i in range(np.size(s_sin_full)):
    if i < ((line * qtty_per_line) - 1):
        print (str(s_sin_full[i]) + ",",end='')
    else:
        if i == (np.size(s_sin_full) - 1):
            print (str(s_sin_full[i]),end='')
        else:                
            print (str(s_sin_full[i]) + ",\n",end='')
            line += 1
        
print ("};")
