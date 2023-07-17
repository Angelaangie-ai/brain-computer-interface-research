#ifndef CSP_UTILS_H
#define CSP_UTILS_H

#include <Eigen/Dense>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace Eigen;

/**
 * The function reads a CSV file and returns a vector of doubles
 */
vector<double> readData(string fileName, int startIndex, int columnIndex);

/**
 * Calculate common spatial patterns for two data sets
 */
MatrixXd csp(MatrixXd& CL, MatrixXd& CR);

#endif  // CSP_UTILS_H
