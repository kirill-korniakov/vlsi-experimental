#!/usr/bin/env python
if __name__ == '__main__':
  import sys
  sys.path.insert(0, '/home/kirill/Work/vlsi-deps/petsc-2.3.3-p16/config')
  import configure
  configure_options = ['--with-mpi=0', '--with-shared=0', '--download-c-blas-lapack=yes', '--PETSC_ARCH=ubuntu-64', '--with-clanguage=C++', '--with-fc=0']
  configure.petsc_configure(configure_options)
