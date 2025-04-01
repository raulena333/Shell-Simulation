import matplotlib.pyplot as plt
import numpy as np
import matplotlib.pylab as pylab

params = {
    'xtick.labelsize': 12,    
    'ytick.labelsize': 12,      
    'axes.titlesize' : 12,
    'axes.labelsize' : 12,
    'legend.fontsize': 12,
}
pylab.rcParams.update(params)  # Apply changes

data = np.loadtxt('build/bin/Debug/ProjectileTrajectory.txt', delimiter='\t', skiprows=1)

time = data[:, 0]
posX = data[:, 1]
posY = data[:, 2]
posZ = data[:, 3]
vX = data[:, 4]
vY = data[:, 5]
vZ = data[:, 6]
machNumber = data[:, 7]
terminalVelocity = data[:, 8]
k_e = data[:, 9]
k_p = data[:, 10]
k_t = data[:, 11]
momentum = data[:, 12]


fig, ax= plt.subplots(2, 2, figsize=(12, 10))
ax[0,0].plot(time, posX, linewidth=2, color='black')
ax[0,0].set_xlabel('Time (s)')
ax[0,0].set_ylabel('PositionX (m)')

ax[0,1].plot(time, posY, linewidth=2, color='black')
ax[0,1].set_xlabel('Time (s)')
ax[0,1].set_ylabel('PositionY (m)')

ax[1,0].plot(time, posZ, linewidth=2, color='black')
ax[1,0].set_xlabel('Time (s)')
ax[1,0].set_ylabel('PositionZ (m)')

ax[1,1].plot(time, machNumber, linewidth=2, color='black')
ax[1,1].set_xlabel('Time (s)')
ax[1,1].set_ylabel('Mach Number')

# plt.savefig('OutputAltitude.pdf')
plt.show()

fig1, ax1= plt.subplots(2, 2, figsize=(12, 10))
ax1[0,0].plot(time, vX, linewidth=2, color='black')
ax1[0,0].set_xlabel('Time (s)')
ax1[0,0].set_ylabel('VelocityX (m/s)')

ax1[0,1].plot(time, vY, linewidth=2, color='black')
ax1[0,1].set_xlabel('Time (s)')
ax1[0,1].set_ylabel('VelocityY (m/s)')

ax1[1,0].plot(time, vZ, linewidth=2, color='black')
ax1[1,0].set_xlabel('Time (s)')
ax1[1,0].set_ylabel('VelocityZ (m/s)')

ax1[1,1].plot(time, terminalVelocity, linewidth=2, color='black')
ax1[1,1].set_xlabel('Time (s)')
ax1[1,1].set_ylabel('Terminal Velocity (m/s)')

# plt.savefig('OutputAltitude.pdf')
plt.show()

fig2, ax2= plt.subplots(2, 2, figsize=(12, 10))
ax2[0,0].plot(time, k_e, linewidth=2, color='black')
ax2[0,0].set_xlabel('Time (s)')
ax2[0,0].set_ylabel('Kinetic Energy (J)')

ax2[0,1].plot(time, k_p, linewidth=2, color='black')
ax2[0,1].set_xlabel('Time (s)')
ax2[0,1].set_ylabel('Potential Energy (J)')

ax2[1,0].plot(time, k_t, linewidth=2, color='black')
ax2[1,0].set_xlabel('Time (s)')
ax2[1,0].set_ylabel('Total Energy (J)')

ax2[1,1].plot(time, momentum, linewidth=2, color='black')
ax2[1,1].set_xlabel('Time (s)')
ax2[1,1].set_ylabel('Momentum (kg m)')

# plt.savefig('OutputAltitude.pdf')
plt.show()