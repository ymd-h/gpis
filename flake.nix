{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }: flake-utils.lib.eachDefaultSystem (system:
    let
      pkgs = nixpkgs.legacyPackages.${system};
      lib = nixpkgs.lib;
    in rec {
      packages = rec {
        python-311 = pkgs.callPackage ./gpis.nix {
          inherit pkgs;
          inherit lib;
          ps = pkgs.python311Packages;
        };

        python-312 = pkgs.callPackage ./gpis.nix {
          inherit pkgs;
          inherit lib;
          ps = pkgs.python312Packages;
        };

        default = python-312;
      };

      devShells.default = pkgs.mkShell {
        packages = [
          (pkgs.python312.withPackages(ps: [
            packages.python-312
          ]))
        ];
      };
    }
  );
}
