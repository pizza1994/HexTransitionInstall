# HexTransitionInstall

This is the code we used to validate the schemes introduced in the paper "**Optimal Dual Schemes for 
Adaptive Grid Based Hexmeshing**" by [M. Livesu](http://pers.ge.imati.cnr.it/livesu/),  L. Pitzalis and [G. Cherchi](http://www.gianmarcocherchi.com).

## Dependencies

This project is built on top of some external libraries. Make sure to have them installed on your machine before proceeding with the building process.
In addition to [Cinolib](https://github.com/mlivesu/cinolib.git) which is already a submodule of this repository, it is necessary to install 
[Gurobi](https://www.gurobi.com). We use Gurobi to solve an ILP that finds the transition vertices of the grid.
Follow the instructions at this [link](https://www.gurobi.com/documentation/6.5/quickstart_linux/software_installation_guid.html) to 
install the library.

## Building
Clone this repository, including submodules, with:
```
git clone --recursive https://github.com/pizza1994/HexTransitionInstall.git
```
Build the executable by running the following commands:
```
cd HexTransitionInstall 
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=<build type> ..
make
```

## Usage

You can install the the transition schemes in a balanced and paired adaptive grid by running the following command:
```
./HexTransitionInstall <input_grid.mesh> <output_grid.mesh>
```
|:warning: Every hexahedron of the input grid must have its level of refinement as the label |
 | --- |
 
If you use our code in your academic projects, please cite our paper using the following BibTeX entry:
```
@article{PLC21,
  title   = {Optimal Dual Schemes for Adaptive Grid Based Hexmeshing},
  author  = {Pitzalis, Luca and Livesu, Marco and Cherchi, Gianmarco},
  journal = {ACM Transactions on Graphics},
  year    = {2021},
  volume  = {},
  number  = {},
  doi     = {}
}
```
