#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/stl_bind.h>
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

  py::bind_vector<cnt>(m, "Contour");
  py::bind_vector<cnts>(m, "ContourList");

  py::class_<Emx>(m, "EigenMatrix")
    .def(py::init<>());

  py::class_<Evx>(m, "EigenVector")
    .def(py::init<>())
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
    });

  py::class_<GPShape, std::shared_ptr<GPShape>>(m, "GPShape")
    .def(py::init<std::vector<double>, std::vector<double>, std::string, const double&, const double&, const int&, bool>(),
         "var"_a, "prior_var"_a,
         "kernel"_a, "test_lim"_a, "res"_a, "gp_count"_a,
         "localExperts"_a = true,
         R"(
GPShape

Parameters
----------
var : Vector1d
prior_var : Vector1d
kernel : "thinplate", "gaussian", "matern"
test_lim : double
res : double
gp_count : int
localExpert : bool
    Default is True
)")
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
    .def("updateThread", &GPShape::updateThread)
    .def("update", &GPShape::update)
    .def("updateLocal", &GPShape::updateLocal)
    .def("addPrior", &GPShape::addPrior)
    .def("test", &GPShape::test)
    .def("testThread", &GPShape::testThread)
    .def("testMeasurement", &GPShape::testMeasurement)
    .def("getContourVariance", &GPShape::getContourVariance)
    .def("transformContour", &GPShape::transformContour)
    .def("transformPointFrom", &GPShape::transformPointFrom)
    .def("transformPointTo", &GPShape::transformPointTo)
    .def("Norm", &GPShape::Norm);
}
