#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <complex>
#include <valarray>

typedef std::complex<double> Cpx;
typedef std::valarray<Cpx> CArray;

void computeFFT(CArray& x);

#endif
