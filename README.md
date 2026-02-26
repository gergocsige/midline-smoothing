# Midline Smoothing

A C++ implementation of a few smoothing algorithms for general purpose point-defined midline trajectory smoothing.

## Moving-average smoothing

$$
P=\frac{P_{i-1}+P_i+P_{i+1}}{3}
$$

## Weighted smoothing

$$
P'_i=\frac{\sum_{j=i-3}^{i+3}P_j*\omega_{j-i+3}}{\sum_{j=i-3}^{i+3}\omega_{j-i+3}}
$$

### Other considerations
- Input validation for meshing with other, possibly error-prone modules/programs.