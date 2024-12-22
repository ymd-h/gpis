from __future__ import annotations

from typing import Iterable, Literal


class GPShape:
    def __init__(self,
                 var: Iterable[float],
                 prior_var: Iterable[float],
                 kernel: Lteral["thinplate", "gaussian", "matern"],
                 test_lim: float,
                 res: float,
                 gp_count: int,
                 localExperts: bool = True): ...

    def buildAndQueryTree(self) -> None: ...
    def initGPs(self) -> None: ...

    def updateKxx(self, idx: int) -> None: ...
    def updateAlpha(self, idx: int) -> None: ...

    def getKxStar2(self, x_star: Iterable[float] | EigenMatrix, K_xStar_xStar: EigenMatrix) -> None: ...

    def getKxStar(self, idx: int, x_star: Iterable[float] | EigenMatrix, KxStar: EigenMatrix) -> None: ...

    def addKernelNoise(self, kernel: EigenMatrix, num_prior: int) -> None: ...
    def getKxCheck(self, idx: int, x_check: EigenMatrix, KuStar: EigenMatrix) -> None: ...

    def getCenters(self) -> Contour: ...

    def proximityCheck(self, idx: int, x: EigenVector) -> bool: ...

    def kernelBlock(self, x1: Iterable[float], x2: Iterable[float]) -> EigenMatrix: ...
    def kernelBlockThinPlatex(self, x: Iterable[float], x2: Iterable[float]) -> EigenMatrix: ...
    def kernelBlockGaussian(self, x: Iterable[float], x2: Iterable[float]) -> EigenMatrix: ...
    def kernelBlockMatern(self, x: Iterable[float], x2: Iterable[float]) -> EigenMatrix: ...

    def solveCholesky(self, A: EigenMatrix, b: EigenMatrix) -> EigenMatrix: ...
    def solveCholeskyWithL(self, L: EigenMatrix, b: EigenMatrix) -> EigenMatrix: ...
    def getCholeskyL(self, A: EigenMatrix) -> EigenMatrix: ...
    def updateCholeskyL(self, idx: int, A: EigenMatrix) -> None: ...

    def addMeasurement(self, idx: int, c: EigenVector, n: EigenVector) -> None: ...

    def printSize(self, M: EigenMatrix, M_name: str) -> None: ...

    def preFilter(self, idx: int, contact_pt: EigenVector, contact_normal: EigenVector, p: EigenVector) -> bool: ...
    def updateThread(self, start_id: int, end_id: int, contact_pt: EigenVector, contact_normal: EigenVector, p: EigenVector, isUpdated: VectorBool) -> None: ...
    def update(self, contact_pt: EigenVector, contact_normal: EigenVector, p: EigenVector) -> bool: ...
    def updateLocal(self, idx: int, contact_pt: EigenVector, contact_normal: EigenVector, p: EigenVector, isPrior: bool = False) -> bool: ...
    def addPrior(self, r: float, n: int, p: EigenVector) -> None: ...
    def test(self, contour: Contour, m_test: EigenMatrix, c_test: EigenMatrix, contourVar: EigenVector) -> None: ...
    def testThread(self, thread_id: int, start_id: int, end_id: int, sz: int, m_: EigenMatrix, c_: EigenMatrix) -> None: ...
    def testMeasurement(self, idx: int, p: EigenVector, n: EigenMatrix) -> [float, float, float]: ...
    def getContourVariance(self, contour: Contour, c_test: EigenMatrix) -> EigenVector: ...

    def transformContour(self, contours: ContourList, p: EigenVector) -> None: ...
    def transformPointFrom(self, pt: EigenVector, pose2: EigenVector) -> EigenVector: ...
    def transformPointTo(self, pt: EigenVector, pose2: EigenVector) -> EigenVector: ...
    def Norm(self, x: Iterable[float], y: Iterable[float]) -> float: ...


class Contour:
    @overload
    def __init__(self): ...

    @overload
    def __init__(self, v: Iterable[Iterable[float]]): ...

    def __getitem__(self, i: int) -> list[float]: ...
    def __len__(self) -> int: ...

    def append(v: Iterable[float]) -> None: ...

class ContourList:
    @overload
    def __init__(self): ...

    @overload
    def __init__(self, v: Iterable[Contour] | Iterable[Iterable[Iterable[float]]]): ...

    def __getitem__(self, i: int) -> Contour: ...
    def __len__(self) -> int: ...

    def append(v: Contour) -> None: ...


class EigenVector:
    @overload
    def __init__(self): ...

    @overload
    def __init__(self, n: int): ...

    @classmethod
    def Zero3d(cls) -> EigenVector: ...

    @classmethod
    def copyFrom(cls, v: EigenVector | Iterable[float]) -> EigenVector: ...

    @overload
    def __getitem__(self, i: int) -> float: ...

    @overload
    def __getitem__(self, s: slice) -> EigenVector: ...

    def __setitem__(self, i: int, v: float) -> None: ...


class EigenMatrix:
    @overload
    def __init__(self): ...

    @overload
    def __init__(self, rows: int, columns: int): ...
