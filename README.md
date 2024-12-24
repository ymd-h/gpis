# gpis: GP implicit surfaces for shape estimation

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

This repository is a 3rd party unofficial fork of [suddhu/gpis-touch-public](https://github.com/suddhu/gpis-touch-public) which provides shape estimation with Gaussian Process Implicit Surface (GPIS).


We changed the followings;

- Add Python Bindings
  - We can install as Python Library
- Make CMakeList.txt more portable
  - We don't need modify library path manually


## Requirements

- [CMake](https://cmake.org/) >= 3.30
- [Boost](https://www.boost.org/) >= 1.70
- [Eigen](https://eigen.tuxfamily.org/) 3
- Python >= 3.9 (?)


To find Boost installation correctly,
we requires relatively new CMake and Boost.
See https://cmake.org/cmake/help/latest/policy/CMP0167.html


## Example Usage

Clone this repository and install `gpis` library
(Currently we don't publish this library to PyPI.)

```shell
git clone https://github.com/ymd-h/gpis.git

cd gpis
pip install .
```

Run a example.
(For this example, you need [NumPy](https://numpy.org/) and [Matplotlib](https://matplotlib.org/), too)


```shell
python example/example_gp.py data/contacts/contacts-rect1-20200810-1811.txt

python example/example_gp.py data/contacts/contacts-ellip2-20200115-2208.txt --shape ellip2 --interval 50
```


## Limitations

We made thin wrappers of `GPShape` class and some container classes like Eigen matrix by using [pybind11](https://pybind11.readthedocs.io/).

What we have achived is to make the example code work.
We haven't investigate other APIs well.

Keyword arguments (probably) doesn't work properly,
and docstring are missing.

If you pass wrong type arguments,
`TypeError` is shown on its console, however, it is not raised.


## Aknoledgements derived from Upstream
- The contouring functions are from [conrec](http://paulbourke.net/papers/conrec/).
- C++ library to generate mesh grid: [meshgen](https://github.com/xiaohongchen1991/meshgen)
- Shape models taken from the [MIT push dataset](https://mcube.mit.edu/push-dataset/index.html)
- `kdtree_eigen.h` is developed by Fabian Meyer based on "Analysis of Approximate Nearest Neighbor Searching with Clustered Point Sets" by Songrit Maneewongvatana and  David M. Mount


## ToDo
- Clean up `data` directory which still contains unknown MATLAB codes etc.
- Organize (re-design) public API.
- Improve implementation if possible.
  - i.e. Does `std::vector<std::vector>>` for contour is sufficient? Should be one of the matrices classes of Eigen or NumPy?
