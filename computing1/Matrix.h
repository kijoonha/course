#pragma once
#include <iostream>
#include <vector>
using namespace std;

/* Your code here */
class Matrix
{
public:
  Matrix(); // constructor
  Matrix(const int &nRow, const int &nCol);
  Matrix(vector<vector<double>> &aMat);
  Matrix operator+(Matrix &aMat);
  Matrix operator-(Matrix &aMat);
  Matrix operator*(Matrix &aMat);
  Matrix Transpose();
  Matrix GetSubVectorbyColumn(int column);
  void Print();
  int GetNumRow();
  int GetNumColumn();
  double GetVal(int x, int y);

  void setMatrix(const vector<vector<double>> &aVvec);
  vector<vector<double>> getMatrix();

protected: // 멤버변수는 .. matrix의..
  vector<vector<double>> vvec;
  int nRow;
  int nCol;
};

class DataFrame : public Matrix
{

public:
  int ReadData(std::string FileName, char sep, char comment, bool IsHeader);
  Matrix GetMatrix(int index[], int nColumn);

private: // 멤버변수는 뭐가있지?
  vector<vector<double>> dataVV;
};

Matrix Cor(Matrix &mat, int method = 1);
Matrix SimpleLinearRegression(Matrix &X, Matrix &Y);

double squareSum(Matrix &mat1, Matrix &mat2);
double discorP(Matrix &mat1, Matrix &mat2);