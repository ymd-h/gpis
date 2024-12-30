#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/stl.h>

#include "GPShape.hpp"

namespace py = pybind11;
using namespace pybind11::literals;


PYBIND11_MAKE_OPAQUE(cnt);
PYBIND11_MAKE_OPAQUE(cnts);
PYBIND11_MAKE_OPAQUE(Emx);
PYBIND11_MAKE_OPAQUE(Evx);


PYBIND11_MODULE(_gpis, m) {
  m.doc() = "GPIS internal C++ extension module";
  py::class_<cnt>(m, "Contour")
    .def(py::init<>())
    .def(py::init([](const py::iterable &it){
      auto v = std::unique_ptr<cnt>(new cnt());
      v->reserve(py::len_hint(it));
      for(auto h : it){
        v->push_back(h.cast<std::vector<double>>());
      }
      return v.release();
    }))
    .def("__getitem__", [](const cnt& self, std::size_t i){ return self[i]; })
    .def("__len__", [](const cnt& self){ return self.size(); })
    .def("append", [](cnt& self, std::vector<double> v){ self.push_back(v); });

  py::class_<cnts>(m, "ContourList")
    .def(py::init<>())
    .def(py::init<std::vector<cnt>>())
    .def("__getitem__", [](const cnts& self, std::size_t i){ return self[i]; })
    .def("__len__", [](const cnts& self){ return self.size(); })
    .def("append", [](cnts& self, cnt v){ self.push_back(v); });

  py::class_<Emx>(m, "EigenMatrix")
    .def(py::init<>())
    .def(py::init<Eigen::Index, Eigen::Index>());

  py::class_<Evx>(m, "EigenVector")
    .def(py::init<>())
    .def(py::init<Eigen::Index>())
    .def_static("Zero3d", []() -> Evx { return Eigen::Vector3d::Zero(); })
    .def_static("copyFrom", [](const Eigen::Ref<Evx>& v) -> Evx { return Evx(v); })
    .def("__getitem__", [](const Evx& self, std::size_t i){ return self(i); })
    .def("__getitem__", [](const Evx& self, py::slice slice){
      std::size_t start{0}, stop{0}, step{0}, length{0};
      if(!slice.compute(self.size(), &start, &stop, &step, &length)){
        throw py::error_already_set();
      }
      auto v = Evx(length);
      for(std::size_t i=0; i<length; i++){
        v(i) = self(start);
        start += step;
      }
      return v;
    })
    .def("__setitem__", [](Evx& self, std::size_t i, double v){ self(i) = v; });

  py::class_<GPShape, std::shared_ptr<GPShape>>(m, "GPShape")
    .def(py::init<std::vector<double>, std::vector<double>, std::string, const double&, const double&, const int&, bool>(),
         "var"_a, "prior_var"_a,
         "kernel"_a, "test_lim"_a, "res"_a, "gp_count"_a,
         "localExperts"_a = true, R"**(
     Initialize GPShape

     Parameters
     ----------
     var : iterable of float
         Variance
     prior_var : iterable of float
         Prior Variance
     kernel : "thinplate", "gaussian", "matern"
         Kernel Method
     test_lim : float
         Grid Limit for ``test()``.
         Square region ``(x, y)`` in ``[-test_lim, test_lim]`` will be used.
     res : float
         Grid Resolution (step size) for ``test()``.
     gp_count : int
         Number of Local Gaussian Processes
     localExperts : bool
         Whether to use local experts. Default is ``True``
)**")
    .def("buildAndQueryTree", &GPShape::buildAndQueryTree)
    .def("initGPs", &GPShape::initGPs)
    .def("updateKxx", &GPShape::updateKxx)
    .def("updateAlpha", &GPShape::updateAlpha)
    .def("getKxStar2", static_cast<void (GPShape::*)(const Emx&, Emx&) const>(&GPShape::getKxStar2))
    .def("getKxStar2", static_cast<void (GPShape::*)(const std::vector<double>&, Emx&) const>(&GPShape::getKxStar2))
    .def("getKxStar", static_cast<void (GPShape::*)(const int&, const Emx&, Emx&) const>(&GPShape::getKxStar))
    .def("getKxStar", static_cast<void (GPShape::*)(const int&, const std::vector<double>&, Emx&) const>(&GPShape::getKxStar))
    .def("addKernelNoise", &GPShape::addKernelNoise)
    .def("getKxCheck", &GPShape::getKxCheck)
    .def("getCenters", &GPShape::getCenters)
    .def("proximityCheck", &GPShape::proximityCheck)
    .def("kernelBlock", &GPShape::kernelBlock)
    .def("kernelBlockThinPlate", &GPShape::kernelBlockThinPlate)
    .def("kernelBlockGaussian", &GPShape::kernelBlockGaussian)
    .def("kernelBlockMatern", &GPShape::kernelBlockMatern)
    .def("solveCholesky", &GPShape::solveCholesky)
    .def("solveCholeskyWithL", &GPShape::solveCholeskyWithL)
    .def("getCholeskyL", &GPShape::getCholeskyL)
    .def("addMeasurement", &GPShape::addMeasurement)
    .def("printSize", &GPShape::printSize)
    .def("printTiming", &GPShape::printTiming)
    .def("preFilter", &GPShape::preFilter)
    .def("update", &GPShape::update, R"**(
     Update local GPs

     Parameters
     ----------
     contact_pt : EigenVector
         Position Vector ``{x, y}`` of newly measured Contact.
     contact_normal : EigenVector
         Normal Vector ``{nx, ny}`` of newly measured Contact.
     p : EigenVector
         Pose Vector ``{x, y, theta}`` of planar object.
)**")
    .def("updateLocal", &GPShape::updateLocal)
    .def("addPrior", &GPShape::addPrior, R"**(
     Add Initial Prior Distribution

     Parameters
     ----------
     r : float
         Radius
     n : int
         Number of Points
     p : EigenVector
         Pose Vector ``{x, y, theta}``
)**")
    .def("test", &GPShape::test, R"**(
     Estimate contour

     First sample from posterior distribution
     over grid test points to get signed distance field (SDF).
     Then estimate contour from them.

     Parameters
     ----------
     contour : Contour
         Estimated contour will be stored here.
     m_test : EigenMatrix
         Mean value at test points will be stored.
     c_test : EigenMatrix
         Variance at test points will be stored.
     controurVar : EigenVector
         Estimated contour variance will be stored here.
)**")
    .def("testThread", &GPShape::testThread)
    .def("testMeasurement",
         [](const GPShape& self, int idx, const Evx& p, const Evx& n){
           double v, n_dot, d;
           self.testMeasurement(idx, p, n, v, n_dot, d);
           return std::make_tuple(v, n_dot, d);
         })
    .def("getContourVariance", &GPShape::getContourVariance)
    .def("transformContour", &GPShape::transformContour)
    .def("transformPointFrom", &GPShape::transformPointFrom)
    .def("transformPointTo", &GPShape::transformPointTo)
    .def("Norm", &GPShape::Norm)
    .doc() = R"**(
    GPShape estimating shape of 2D planar objects from tactile sensor.

    Notes
    -----
    Shape is estimated with multiple local Gaussian Prpcesses (GPs)
    instead of single full large GP to reduce computation cost.
    Each local GP learns function from points ``{x, y}``
    to signed distance field (SDF) ``{d, nx, ny}``.
    Estimated shape is initialized with circular pior distribution
    and updated with newly measured contacts ``{x, y, nx, ny, d=0}``.
    By sampling over mesh grid points from posterior distribution,
    contour (``d=0``) is estimated.

    This is a part of the paper [1]_ and pose estimation is (probably)
    not included.

    References
    ----------
    .. [1] S. Sudharshan et al., "Tactile SLAM: Real-time inference of
       shape and pose from planar pushing", arXiv preprint
       arXiv:2011.07044 (2020).
)**";
}
