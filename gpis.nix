{ pkgs, lib, ps }: ps.buildPythonPackage {
  pname = "gpis";

  version = "0.0.1";

  doCheck = false;

  pyproject = true;

  dontUseCmakeConfigure=true;

  nativeBuildInputs = [
    pkgs.cmake
    pkgs.boost
    pkgs.eigen
  ];

  configurePhase = ''
    export Boost_DIR=${pkgs.boost.dev}
    export Eigen3_DIR=${pkgs.eigen}
  '';

  build-system = [
    ps.scikit-build-core
    ps.ninja
    ps.pybind11
  ];

  src = lib.fileset.toSource {
    root = ./.;
    fileset = lib.fileset.unions [
      ./pyproject.toml
      ./README.md
      ./CMakeLists.txt
      ./src
    ];
  };
}
