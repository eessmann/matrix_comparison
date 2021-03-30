#!/usr/env/python

import matplotlib.pyplot as plt
from pathlib import Path
import csv

def plot_results(filename):
    size = []
    BLAZE = []
    GSL = []
    Eigen = []

    csvfile = open(filename)

    reader = csv.reader(csvfile, delimiter
    =',')

    for i, row in enumerate(reader):
        if i == 0:
            continue
        size.append(float(row[0]))
        Eigen.append(float(row[1]))
        BLAZE.append(float(row[2]))
        GSL.append(float(row[3]))

    filename = Path(filename)

    fig_path = filename.with_suffix('.pdf')

    plt.figure(1)
    plt.title("Asymmetric Eigenvalue Decomposition")
    ax = plt.subplot(111)
    plt.plot(size, BLAZE, label="Blaze")
    plt.plot(size, GSL, label="GSL")
    plt.plot(size, Eigen, label="Eigen")
    plt.legend()
    ax.set_yscale("log")
    plt.ylabel('$\mu$s')
    plt.xlabel('matrix size')
    plt.savefig(fig_path)


    #plt.figure(2)
    #plt.title("Asymmetric Eigenvalue Decomposition (Small matrices)")
    #ax = plt.subplot(111)
    #plt.plot(size[:25], BLAZE[:25], label="Blaze")
    #plt.plot(size[:25], GSL[:25], label="GSL")
    #plt.plot(size[:25], Eigen[:25], label="Eigen")
    #plt.legend()
    #ax.set_yscale("log")
    #plt.ylabel('$\mu$s')
    #plt.xlabel('matrix size')



if __name__ == '__main__':
    plot_results("../cmake-build-release/csv_symm_results.csv")
    plot_results("../cmake-build-release/csv_asymm_results.csv")
