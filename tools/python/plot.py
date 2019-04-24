# -*- coding: utf-8 -*-

import json
import matplotlib.pyplot as plt
import numpy as np


file = '../../log/veh_controller.json'

with open(file) as json_file:  
    data = json.load(json_file)
    
    
time = np.zeros(len(data))
values = {}

i = 0
for d in data:
    
    # get time and data
    time[i] = d['time']
    da = d['data']
    
    # iterate over field names
    for name,dict_ in da.items():

        # create array
        if i == 0:
            values[name] = np.zeros(len(data))

        # save to values            
        values[name][i] = da[name]

    i += 1
    
    
# plot position    
plt.figure(1)
plt.clf()
plt.grid()
plt.axis('equal')
plt.plot(values['x'], values['y'])
plt.title('trajectory')
plt.xlabel('x [m]')
plt.ylabel('y [m]')


# plot acceleration
plt.figure(2)
plt.clf()
plt.grid()
plt.plot(time, values['a'])
plt.plot(time, values['aDes'], '--')
plt.title('acceleration')
plt.xlabel('t [s]')
plt.ylabel('a [m/s^2]')


# plot velocity
plt.figure(3)
plt.clf()
plt.grid()
plt.plot(time, values['v'])

if hasattr(values, 'vDes'):
    plt.plot(time, values['vDes'], '--')

plt.title('velocity')
plt.xlabel('t [s]')
plt.ylabel('v [m/s]')


# plot velocity
plt.figure(4)
plt.clf()
plt.grid()
plt.plot(time, values['pedal'])
plt.title('pedal')
plt.xlabel('t [s]')
plt.ylabel('p [-]')


# plot aux values
plt.figure(5)
plt.clf()
plt.grid()

if hasattr(values, 'aux'):
    plt.plot(time, values['aux'], '--')

plt.title('aux')
plt.xlabel('t [s]')
plt.ylabel('aux [-]')

plt.show(block=True)
