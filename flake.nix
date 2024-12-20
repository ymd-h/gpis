{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }: flake-utils.lib.eachDefaultSystem (system:
    let
      pkgs = nixpkgs.legacyPackages.${system};
      deps = with pkgs; [
          cmake
          eigen
          (python3.withPackages (ps: with ps; [
            numpy
            matplotlib
          ]))
        ];
    in {
      packages.default = pkgs.stdenv.mkDerivation {
        name = "gpis";
        src = ./.;
        dontUseCmakeConfigure=true;

        nativeBuildInputs = deps;
        buildInputs = deps;

        buildPhase = ''
          mkdir -p mex/build
          cd mex/build
          cmake ../
          make -j
        '';

        installPhase = ''
          mkdir -p $out
          cp test_gp test_kd $out/
        '';
      };

      devShells.default = pkgs.mkShell {
        packages = deps;
      };
    }
  );
}
