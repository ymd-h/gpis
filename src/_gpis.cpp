#include <pybind11/pybind11.h>

#include "GPShape.hpp"

namespace py = pybind11;


PYBIND11_MODULE(_gpis, m) {
  m.doc() = "GPIS internal C++ extension module";
  py::class_<GPShape, std::shared_ptr<GPShape>>(m, "GPShape")
    .def(py::init<std::vector<double>, std::vector<double>, std::string,
         const double&, const double&, const int&, bool>())
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
