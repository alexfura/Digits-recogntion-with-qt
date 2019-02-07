# Digits-recogntion-with-qt

A small tutorial how to run this project.

## Prerequisites
  
  Armadillo(last version) + Lapack and Blas - http://arma.sourceforge.net/ \
  HDF5 lib - https://www.hdfgroup.org/downloads/hdf5/ \
  Qt + Qt Creator(last version) 
  
## Configuration of project

1) Download and install all libraries.
2) In armadillo folder find "/include/armadillo_bits", open config.hpp and uncomment "ARMA_USE_HDF5" to enable .h5 format(here will be stored out trained model).
3) Edit .pro file.
4) Place "w1.h5" and "w2.h5" files in build directory.

## Usage

Run project and have fun

## License
[MIT](https://choosealicense.com/licenses/mit/)
