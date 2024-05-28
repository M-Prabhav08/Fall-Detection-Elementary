import serial
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

ser = serial.Serial('COM3', 9600)

x_data, y_data, z_data = [], [], []
speed_threshold = 50 

fig, ax = plt.subplots()
ax.set_ylim(-1023, 1023)
line_x, = ax.plot([], [], label='X-axis')
line_y, = ax.plot([], [], label='Y-axis')
line_z, = ax.plot([], [], label='Z-axis')

ax.legend()

def calculate_speed(data_list):
    if len(data_list) < 2:
        return 0
    return abs(data_list[-1] - data_list[-2])

def update(frame):
    try:
        data = ser.readline().decode('utf-8').strip()
        if data:
            val_x, val_y, val_z = map(int, data.split())

            x_data.append(val_x)
            y_data.append(val_y)
            z_data.append(val_z)

            speed_x = calculate_speed(x_data)
            speed_y = calculate_speed(y_data)
            speed_z = calculate_speed(z_data)

            line_x.set_data(range(len(x_data)), x_data)
            line_y.set_data(range(len(y_data)), y_data)
            line_z.set_data(range(len(z_data)), z_data)

            ax.relim()
            ax.autoscale_view()

            if speed_x > speed_threshold or speed_y > speed_threshold or speed_z > speed_threshold:
                print("Fault detected! ")

    except KeyboardInterrupt:
        ser.close()
        plt.close()

ani = FuncAnimation(fig, update, blit=False)

plt.show()
