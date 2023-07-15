#include <Eigen/Dense>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace Eigen;

/**
 The function reads CSV file and returns a vector of doubles
*/
vector<double> readData(string fileName, int startIndex, int columnIndex) {
  vector<double> data;
  ifstream dataFile(fileName);
  // declares a string variable 'line'
  string line;
  int lineIndex = 0;

  // reiterate across the file
  while (getline(dataFile, line)) {
    lineIndex++;
    if (lineIndex < startIndex) {
      continue;
    }
    int commaIndex = line.find(",");
    int column = 1;
    while (commaIndex != string::npos) {
      if (column == columnIndex) {
        data.push_back(stod(line.substr(0, commaIndex)));
        break;
      }
      line = line.substr(commaIndex + 1);
      commaIndex = line.find(",");
      column++;
    }
  }
  return data;
}
/**
 Calculate common spatial patterns for two data sets
*/
MatrixXd csp(MatrixXd &CL, MatrixXd &CR) {
  // Creating a zero matrix
  MatrixXd S = MatrixXd::Zero(CL.rows(), CL.rows());
  // compute the covariance matrix for first dataset
  for (int i = 0; i < CL.cols(); i++) {
    S = S + CL.col(i) * CL.col(i).transpose();
  }
  S = S / CL.cols();  // normalize the matrix
  // repeat the same procedure for the second dataset
  MatrixXd R = MatrixXd::Zero(CR.rows(), CR.rows());
  for (int i = 0; i < CR.cols(); i++) {
    R = R + CR.col(i) * CR.col(i).transpose();
  }
  // Perform eigenvalue decomposition
  R = R / CR.cols();
  // Eigenvalue decomposition
  SelfAdjointEigenSolver<MatrixXd> eigensolver(S.inverse() * R);
  MatrixXd W = eigensolver.eigenvectors();
  VectorXd lambda = eigensolver.eigenvalues();
  MatrixXd A(CL.rows(), 2);
  A.col(0) = W.col(0);
  A.col(1) = W.col(1);
  return A;  // getting the vectors
}
