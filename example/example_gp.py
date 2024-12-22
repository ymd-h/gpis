# /// script
# requires-python = ">=3.9"
# dependencies = [
#     "matplotlib",
#     "numpy",
# ]
# ///
"""
Example Code for GPIS

Original Code: https://github.com/suddhu/gpis-touch-public/blob/53558687a9de9ef131eb51071eb817cf98802099/mex/test_gp.cpp
"""
from __future__ import annotations

import argparse
import sys
from typing import Literal, TypeAlias

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

import gpis

ShapeID: TypeAlias = Literal[
    "rect1", "rect2", "rect3",
    "tri1", "tri2", "tri3",
    "ellip1", "ellip2", "ellip3",
    "hex",
]

def ground_truth_shape(shape: ShapeID) -> gpis.Contour:
    """
    Get Ground Truth Shape

    Parameters
    ----------
    shape

    Returns
    -------
    gpis.Contour

    Notes
    -----
    Original Code: https://github.com/suddhu/gpis-touch-public/blob/53558687a9de9ef131eb51071eb817cf98802099/cpp/src/get_shapes.cpp
    """
    if shape in ["rect1", "rect2", "rect3"]:
        dim = {
            "rect1": [0.090, 0.090],
            "rect2": [0.08991, 0.11258],
            "rect3": [0.13501, 0.08994],
        }[shape]

        a = dim[0] / 2
        b = dim[1] / 2

        return gpis.Contour([
            [ a, b],
            [-a, b],
            [-a,-b],
            [ a,-b],
        ])

    if shape in ["tri1", "tri2", "tri3"]:
        dim = {
            "tri1": [0.12587, 0.12590, 0.178],
            "tri2": [0.12587, 0.15100, 0.1962],
            "tri3": [0.12561, 0.1765, 0.2152],
        }[shape]

        a = dim[0]
        b = dim[1]
        c = dim[2]
        d = 0.090 / 2.0

        return gpis.Contour([[d, d], [d-b, d], [d, d-a]])

    if shape in ["ellip1", "ellip2", "ellip3"]:
        dim = {
            "ellip1": [0.105, 0.105],
            "ellip2": [0.105, 0.13089],
            "ellip3": [0.105, 0.157],
        }[shape]

        ax1 = dim[0] / 2
        ax2 = dim[1] / 2

        gamma = np.linspace(0, 2*np.pi, 100)
        x = ax1*np.cos(gamma)
        y = ax2*np.sin(gamma)

        return gpis.Contour([[xi, yi] for xi, yi in zip(x, y)])

    if shape == "hex":
        dim = {
            "hex": [0.06050, 6],
        }[shape]

        side = dim[0]
        n = int(dim[1])

        theta = np.arange(n) * 2 * np.pi;
        x = side * np.cos(theta)
        y = side * np.sin(theta)

        return gpis.Contour([[xi, yi] for xi, yi in zip(x, y)])

    raise ValueError(f"Unknown Shape: ${shape}")


def example_gp(data: str,
               shape: ShapeID,
               kernel: Literal["thinplate", "gaussian", "matern"],
               interval: int) -> int:
    """
    Example GP

    Parameters
    ----------
    data : str
        Input Data File. './data/contacts/contacts-rect1-20200810-1811.txt'
    shape : "rect1" or "ellip2"
        Shape ID for Ground Truth
    kernel : "thinplate", "gaussian", or "matern"
        Kernel
    interval : int
        Plot frame interval in ms

    Returns
    -------
    int
        Return Code
    """
    ground_truth = ground_truth_shape(shape)

    pvar = [1e-5, 1e-5, 1e-5]
    priorNoise = [1e-2, 1e-2, 1e-2]
    testLim = 0.1
    testRes = testLim * 0.1
    priorRad = 0.03
    gp_count = 16 # 4*4

    gpshape = gpis.GPShape(pvar, priorNoise, kernel, testLim, testRes, gp_count, True)
    gpshape.addPrior(priorRad, 4, gpis.EigenVector.Zero3d())

    contour = gpis.Contour()
    fMean = gpis.EigenMatrix()
    fVar = gpis.EigenMatrix()
    contourVar = gpis.EigenVector()

    fig = plt.figure(figsize=(10, 10))

    plt.xlim(-0.09, 0.09)
    plt.ylim(-0.09, 0.09)

    ground_x = [ground_truth[i][0] for i in range(len(ground_truth))]
    ground_y = [ground_truth[i][1] for i in range(len(ground_truth))]

    ground_x.append(ground_x[0])
    ground_y.append(ground_y[0])

    ground_line = plt.plot(ground_x, ground_y, "k--")
    frames = np.loadtxt(data, delimiter=",")

    def plot(step: tuple[int, np.ndarray], lines: list) -> tuple:
        t = step[0]

        contact_point = gpis.EigenVector.copyFrom(step[1][:2])
        contact_normal = gpis.EigenVector.copyFrom(step[1][2:])

        updated = gpshape.update(contact_point, contact_normal, gpis.EigenVector.Zero3d())
        if(updated):
            gpshape.test(contour, fMean, fVar, contourVar)

        while len(lines) > 0:
            lines.pop().remove()

        plt.title(f"t = {t} / {frames.shape[0]}")

        shape_x = [contour[i][0] for i in range(len(contour))]
        shape_y = [contour[i][1] for i in range(len(contour))]

        shape_x.append(shape_x[0])
        shape_y.append(shape_y[0])

        lines.extend(plt.plot(shape_x, shape_y, "g-"))

        c_x = [contact_point[0]]
        c_y = [contact_point[1]]
        n_x = [contact_normal[0]]
        n_y = [contact_normal[1]]

        lines.extend(plt.plot(c_x, c_y, "bx"))
        lines.append(plt.quiver(c_x, c_y, n_x, n_y))

        return tuple()


    ani = FuncAnimation(fig, plot,
                        frames = enumerate(frames),
                        interval=interval,
                        fargs=([], ),
                        repeat=False)
    plt.show()
    return 0


if __name__ == "__main__":
    p = argparse.ArgumentParser("Example for GP")
    p.add_argument("data", help="Input Data. e.g. './data/contacts/contacts-rect1-20200810-1811.txt'")
    p.add_argument("--shape", choices=["rect1", "rect2", "rect3", "tri1", "tri2", "tri3", "ellip1", "ellip2", "ellip3", "hex"], default="rect1")
    p.add_argument("--kernel", choices=["thinplate", "gaussian", "matern"], default="thinplate")
    p.add_argument("--interval", type=int, help="Plot frame interval in ms", default=10)
    args = p.parse_args()

    sys.exit(example_gp(args.data, args.shape, args.kernel, args.interval))
