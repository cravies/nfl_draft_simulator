from matplotlib import pyplot as plt 
import numpy as np

def plot_results(fname):
    # grab results from file
    f = open(fname, "r")
    lines = f.readlines()
    lines = [line.replace('\n','') for line in lines]
    x = [float(line.split(',')[0]) for line in lines]
    y = [float(line.split(',')[1]) for line in lines]
    print("x: ", x)
    print("y: ", y)
    plt.title("Results")
    plt.plot(x,y)
    plt.show()
    f.close()

if __name__=="__main__":
    plot_results("out.txt")