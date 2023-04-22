#include "Matrix.h"
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
/* Your code here */
int DataFrame::ReadData(std::string FileName, char sep, char comment, bool IsHeader)
{
    std::fstream myfile;
    myfile.open(FileName);
    std::string line;
    if (myfile.is_open())
    {

        while (getline(myfile, line))
        {
            vector<double> dataRow;

            if (line.empty() || line[0] == comment)
            {
                continue;
            }
            if (IsHeader)
            {
                IsHeader = false;
                continue; // header는 그냥 넘기면 돼?
            }
            // 아래.. line별로 읽어온 데이터를 벡터 i번째에 추가함
            std::stringstream ss(line); // sep구분자로 읽어오는거 고쳐야함
            std::string data;
            while (getline(ss, data, sep))
            {
                if (data != string(1, sep))
                {
                    dataRow.push_back(stod(data)); // 문자열을 double형으로 변환?
                }
            }
            dataVV.push_back(dataRow);
        }
    }

    return 0;
}

Matrix DataFrame::GetMatrix(int index[], int nColumn)
{

    vector<vector<double>> temp(dataVV.size(), vector<double>(nColumn));
    for (int j = 0; j < nColumn; j++)
    {
        for (int i = 0; i < dataVV.size(); i++)
        {
            temp[i][j] = dataVV[i][index[j]];
        }
    }

    return Matrix(temp);
}

Matrix::Matrix() {}
Matrix::Matrix(const int &aMM, const int &aNN)
{

    nRow = aMM;
    nCol = aNN;
    vector<vector<double>> vvec(nRow, vector<double>(nCol, 0));

} // 이것도 과연필요할지? 벡터 초기화
Matrix::Matrix(vector<vector<double>> &aMat) : vvec(aMat)
{
    nRow = vvec.size();
    if (nRow == 0)
    {
        nCol = 0;
    }
    else
    {
        nCol = vvec[0].size();
    }
}
void Matrix::setMatrix(const vector<vector<double>> &aVvec)
{
    vvec = aVvec;
    nRow = vvec.size();
    nCol = vvec[0].size();
}

vector<vector<double>> Matrix::getMatrix()
{
    return vvec;
}

Matrix Matrix::operator+(Matrix &aMat)
{
    vector<vector<double>> temp(nRow, vector<double>(nCol));
    for (int i = 0; i < nRow; i++)
    {
        for (int j = 0; j < nCol; j++)
        {
            temp[i][j] = vvec[i][j] + aMat.GetVal(i, j);
        }
    }
    return Matrix(temp);
}

Matrix Matrix::operator-(Matrix &aMat)
{
    vector<vector<double>> temp(nRow, vector<double>(nCol));
    for (int i = 0; i < nRow; i++)
    {
        for (int j = 0; j < nCol; j++)
        {
            temp[i][j] = vvec[i][j] - aMat.GetVal(i, j);
        }
    }
    return Matrix(temp);
}

Matrix Matrix::operator*(Matrix &aMat)
{
    int mm = nRow;
    int nn = aMat.GetNumColumn();
    vector<vector<double>> temp(mm, vector<double>(nn));
    for (int i = 0; i < mm; i++)
    {
        for (int j = 0; j < nn; j++)
        {
            for (int k = 0; k < aMat.GetNumRow(); k++)
            {
                temp[i][j] += vvec[i][k] * aMat.GetVal(k, j);
            }
        }
    }
    return Matrix(temp);
}

Matrix Matrix::Transpose()
{
    vector<vector<double>> temp(nCol, vector<double>(nRow));
    for (int i = 0; i < nRow; i++)
    {
        for (int j = 0; j < nCol; j++)
        {
            temp[j][i] = vvec[i][j];
        }
    }

    return Matrix(temp);
}

Matrix Matrix::GetSubVectorbyColumn(int column)
{
    vector<vector<double>> temp(nRow, vector<double>(1));
    for (int i = 0; i < vvec.size(); i++)
    {
        temp[i][0] = vvec[i][column];
    }
    return Matrix(temp);
}

void Matrix::Print()
{
    for (int i = 0; i < nRow; i++)
    {
        for (int j = 0; j < nCol; j++)
        {
            cout << vvec[i][j] << ' ';
        }
        cout << endl;
    }
}
int Matrix::GetNumRow()
{
    return nRow;
}
int Matrix::GetNumColumn()
{
    return nCol;
}

double Matrix::GetVal(int x, int y)
{
    return vvec[x][y];
}

Matrix SimpleLinearRegression(Matrix &X, Matrix &Y)
{

    int nn = X.GetNumRow();
    vector<vector<double>> XaV(nn, vector<double>(2, 1));
    for (int i = 0; i < nn; i++)
    {
        XaV[i][1] = X.GetVal(i, 0);
    }
    Matrix Xa(XaV);
    Matrix X_t = Xa.Transpose();
    Matrix X_tX = X_t * Xa;

    Matrix X_tY = X_t * Y;
    // X_txX가 항상 2by2임을 전제(simple linearR이므로)
    vector<vector<double>> abcd;
    abcd = X_tX.getMatrix();
    // inverse구하기
    double det;
    det = abcd[0][0] * abcd[1][1] - abcd[0][1] * abcd[1][0];

    vector<vector<double>> abcd_inv(2, vector<double>(0));
    abcd_inv[0].push_back(abcd[1][1] / det);
    abcd_inv[0].push_back(-abcd[1][0] / det);
    abcd_inv[1].push_back(-abcd[0][1] / det);
    abcd_inv[1].push_back(abcd[0][0] / det);

    Matrix inverseXtX;
    inverseXtX.setMatrix(abcd_inv);
    return inverseXtX * X_tY;
}
double squareSum(Matrix &mat1, Matrix &mat2)
{ // t사이즈 같은  칼럼하나짜리matrix 2개 받아 squaresum구한다
    double mean1, mean2;
    double sum1 = 0;
    double sum2 = 0;
    int nn = mat1.GetNumRow();
    double ssS = 0;
    for (int i = 0; i < nn; i++)
    {
        sum1 += mat1.GetVal(i, 0);
    }
    for (int i = 0; i < nn; i++)
    {
        sum2 += mat2.GetVal(i, 0);
    }
    mean1 = sum1 / static_cast<double>(nn);
    mean2 = sum2 / static_cast<double>(nn);
    for (int i = 0; i < nn; i++)
    {
        ssS += (mat1.GetVal(i, 0) - mean1) * (mat2.GetVal(i, 0) - mean2);
    }
    return ssS;
}

double discorP(Matrix &mat1, Matrix &mat2)
{
    int disN = 0;
    int nn = mat1.GetNumRow();
    for (int i = 0; i < nn; i++)
    {
        for (int j = i; j < nn; j++)
        {
            double x_i = mat1.GetVal(i, 0);
            double y_i = mat2.GetVal(i, 0);
            // i와 j를 비교할 예정
            double x_j = mat1.GetVal(j, 0);
            double y_j = mat2.GetVal(j, 0);
            if ((x_i > x_j && y_i < y_j) || (x_i < x_j && y_i > y_j))
            {
                disN += 1;
            }
            else
            {
                continue;
            }
        }
    }
    return static_cast<double>(disN);
}

Matrix Cor(Matrix &mat, int method)
{
    int mm = mat.GetNumColumn();
    vector<vector<double>> corV(mm, vector<double>(mm));

    if (method == 1)

    {
        for (int i = 0; i < mm; i++)
        {

            for (int j = 0; j < mm; j++)
            {
                double pearson;
                Matrix sub_i = mat.GetSubVectorbyColumn(i);
                Matrix sub_j = mat.GetSubVectorbyColumn(j);
                pearson = squareSum(sub_i, sub_j) / sqrt(squareSum(sub_j, sub_j) * squareSum(sub_i, sub_i));
                corV[i][j] = pearson;
            }
        }
        return Matrix(corV);
    }
    else
    {
        for (int i = 0; i < mm; i++)
        {
            for (int j = 0; j < mm; j++)
            {
                double tau;
                Matrix sub_i = mat.GetSubVectorbyColumn(i);
                Matrix sub_j = mat.GetSubVectorbyColumn(j);
                double disN = discorP(sub_i, sub_j);
                int n = mat.GetNumRow();

                tau = 1 - static_cast<double>(4 * disN / (n * (n - 1)));
                corV[i][j] = tau;
            }
        }
        return Matrix(corV);
    }
}