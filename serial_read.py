import serial
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

ser = serial.Serial('COM3', 9600)

x_data, y_data, z_data = [], [], []

fig, ax = plt.subplots()
ax.set_ylim(-1023, 1023) 
line_x, = ax.plot([], [], label='X-axis')
line_y, = ax.plot([], [], label='Y-axis')
line_z, = ax.plot([], [], label='Z-axis')

ax.legend()

def update(frame):
    try:
        data = ser.readline().decode('utf-8').strip()
        if data:
            val_x, val_y, val_z = map(int, data.split(','))

            x_data.append(val_x)
            y_data.append(val_y)
            z_data.append(val_z)

            line_x.set_data(range(len(x_data)), x_data)
            line_y.set_data(range(len(y_data)), y_data)
            line_z.set_data(range(len(z_data)), z_data)

            ax.relim()
            ax.autoscale_view()

    except KeyboardInterrupt:
        ser.close()
        print("Serial port closed.")
        plt.close()

ani = FuncAnimation(fig, update, blit=False)

plt.show()
