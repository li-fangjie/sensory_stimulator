import serial
import numpy as np
from matplotlib import pyplot as plt
import time


class Variables:
    def __init__(self, variable_name, y_lab, y_r):
        self.y_lim = y_r[:]
        self.y_label = y_lab
        self.name = variable_name

    def get_y_lim(self):
        return self.y_lim

    def get_y_label(self):
        return self.y_label

    def get_name(self):
        return self.name


force_var = Variables("force", "Force(kg)", [-1, 20])
vel_var = Variables("velocity", "Velocity(mm/s)", [-200, 200])

cur_var: Variables = force_var


def read_line(port_obj):
    line = port_obj.readline()
    try:
        if line[0] == "-":
            data = float(line[1:])
            data = -data
        else:
            data = float(line)
    except:
        print(line)
        return 0
    return data


def init_graph():
    plt.title("PID Control Reading: %s" % cur_var.get_name())
    plt.xlabel("Time(s)")
    plt.ylabel(cur_var.get_y_label())
    axes = plt.gca()
    line, = plt.plot([], [], "b")
    # axes.set_xlim(0, 100)
    axes.set_ylim(cur_var.get_y_lim()[0], cur_var.get_y_lim()[1])
    axes.set_xlim(0, 10)
    # axes.set_autoscalex_on(True)
    return line, axes


def update_graph(t, dat, line, axes):
    line.set_xdata(t)
    line.set_ydata(dat)
    # axes.relim()
    # axes.autoscale_view()
    if t[len(t)-1] - 10 >= 0:
        axes.set_xlim(t[len(t)-1]-10, t[len(t)-1])
    plt.draw()
    plt.pause(1e-17)


dat_size = 100
t_arr = []
dat_arr = []
lin, axs = init_graph()

i = -1
with serial.Serial('/dev/cu.usbmodem14101', 9600) as port:
    start_t = 0
    cur_t = 0

    while True:
        if len(dat_arr) == dat_size:
            dat_arr[:-1] = dat_arr[1:]
            t_arr[:-1] = t_arr[1:]
        # time_1 = time.time()
        dat = read_line(port)
        # print(time.time()- time_1)
        dat_arr.append(dat)

        if cur_t == 0:
            start_t = time.time()
        cur_t = time.time() - start_t

        t_arr.append(cur_t)
        # i = min(i + 1, dat_size - 1)
        update_graph(t_arr, dat_arr, lin, axs)
        print(dat, cur_t)
