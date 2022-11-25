
#include <iostream>
#include <float.h>
#include <string>
#include <cmath>
#include <sstream>
#include "vector.h"

namespace aline {
    template <class T, int M,int N>
    class Matrix {
    private:
        Vector<T, N> matrix[M];
    public:
        Matrix() {
            for (int i = 0; i < M; i++) {
                matrix[i] = Vector<T, N>();
            }
        }

        Matrix(std::initializer_list<Vector<T, N>> list) {
            if (list.size() > (size_t) M)
                throw std::runtime_error("list size bigger than M");
            Vector<T,N> l[list.size()];
            int s = 0;
            for (auto& i : list) {
                l[s++] = Vector<T, N>(i);
            }
            for (int i = 0; i < M; i++) {
                if ((size_t) i >= list.size())
                    matrix[i] = Vector<T, N>();
                else
                    matrix[i] = Vector<T, N>(l[i]);
            }
        }

        Matrix(const Matrix<T, M, N>&m) {
            for (int i = 0; i < N; i++) {
                matrix[i] = m[i];
            }
        }

        Vector<T, N> at(size_t i) {
            if (i >= M)
                throw std::runtime_error("index out of range");
            return matrix[i];
        }

        T at(size_t i, size_t j) {
            if (i >= M || j >=N)
                throw std::runtime_error("index out of range");
            return matrix[i][j];
        }

        Vector<T, N> operator[](size_t i) const {
            return matrix[i];
        }

        Vector<T, N>& operator[](size_t i) {
            return matrix[i];
        }

        Matrix<T, M, N>& operator+=(const Matrix<T, M, N>&m) {
            for (int i = 0; i < M; i++) {
                matrix[i] += m[i];
            }
            return *this;
        }
    };

    //template <class T, int M, int N> int det(const Matrix<T, M, N>& m) {
    //    int d = 0;
    //    Matrix<T, (M - 1), (N - 1)> sub;
    //    if (M == 2)
    //        return ((m[0][0] * m[1][1]) - (m[1][0] * m[0][1]));
    //    else {
    //        for (int x = 0; x < M; x++) {
    //            int subi = 0;
    //            for (int i = 1; i < N; i++) {
    //                int subj = 0;
    //                for (int j = 0; j < N; j++) {
    //                    if (j == x)
    //                        continue;
    //                    sub[subi][subj] = m[i][j];
    //                    subj++;
    //                }
    //                subi++;
    //            }
    //            d = d + (std::pow(-1, x) * m[0][x] * det(sub));
    //        }
    //    }
    //    return d;
    //}

    template <class T, int M, int N> Matrix<T, M, N> inverse(const Matrix<T, M, N>&m) {
        Matrix<T, M, N> res;
        //Matrix<T, M, N> temp;
        if (M != N) {
            for (int i = 0; i < M; i++)
                for (int j = 0; j < N; j++)
                    res[i][j] = nan("");
            return res;
        }

        //int determinant = det(m);

        //if (determinant == 0) {
        //    for (int i = 0; i < M; i++)
        //        for (int j = 0; j < N; j++)
        //            res[i][j] = nan("");
        //    return res;
        //}

        //int row = 0, col = 0, sign = 1;
        //for (int p = 0; p < M; p++) {
        //    for (int q = 0; q < N; q++) {
        //        for (int i = 0; i < M; i++) {
        //            for (int j = 0; j < N; j++) {
        //                if (i != p && j != q) {
        //                    temp[row][col++] = m[i][j];

        //                    if (col == N - 1) {
        //                        col = 0;
        //                        row++;
        //                    }
        //                }
        //            }
        //        }
        //        sign = ((p + q) % 2 == 0) ? 1 : -1;

        //        res[q][p] = (sign) * (det(temp, N - 1));
        //    }
        //}
        return res;
    }

    template <class T, int M, int N> bool isnan(const Matrix<T, M, N>&m) {
        for (int i = 0; i < M; i++) {
            if (isnan(m[i]))
                return true;
        }
        return false;
    }

    template <class T, int M, int N> bool nearly_equal(const Matrix<T, M, N>& m, const Matrix<T, M, N>& n) {
        for (int i = 0; i < M; i++) {
            if (!nearly_equal(m[i], n[i]))
                return false;
        }
        return true;
    }

    template <class T, int M, int N> bool operator==(const Matrix<T, M, N>&m, const Matrix<T, M, N>&n) {
        for (int i = 0; i < M; i++) {
            if (m[i] != n[i])
                return false;
        }
        return true;
    }

    template <class T, int M, int N> bool operator!=(const Matrix<T, M, N>& m, const Matrix<T, M, N>& n) {
        for (int i = 0; i < M; i++) {
            if (m[i] == n[i])
                return false;
        }
        return true;
    }

    template <class T, int M, int N> std::ostream& operator<<(std::ostream& out, const Matrix<T, M, N>&m) {
        out << to_string(m) << std::endl;
        return out;
    }

    template <class T, int M, int N> Matrix<T, M, N> operator+(const Matrix<T, M, N>&m, const Matrix<T, M, N>&n) {
        Matrix<T, M, N> res;
        for (int i = 0; i < M; i++) {
            res[i] = m[i] + n[i];
        }
        return res;
    }

    template <class T, int M, int N> Matrix<T, M, N> operator-(const Matrix<T, M, N>&m) {
        Matrix<T, M, N> res;
        for (int i = 0; i < M; i++) {
            res[i] = -m[i];
        }
        return res;
    }

    template <class T, int M, int N> Matrix<T, M, N> operator-(const Matrix<T, M, N>&m, const Matrix<T, M, N>&n) {
        Matrix<T, M, N> res;
        for (int i = 0; i < M; i++) {
            res[i] = m[i] - n[i];
        }
        return res;
    }

    template <class T, int M, int N> Matrix<T, M, N> operator*(const T& t, const Matrix<T, M, N>& m) {
        Matrix<T, M, N> res;
        for (int i = 0; i < M; i++) {
            res[i] = t * m[i];
        }
        return res;
    }

    template <class T, int M, int N> Matrix<T, M, N> operator*(const Matrix<T, M, N>& m, const T& t) {
        Matrix<T, M, N> res;
        for (int i = 0; i < M; i++) {
            res[i] = Vector<T, N>(m[i] * t);
        }
        return res;
    }

    template <class T, int M, int N> Vector<T, M> operator*(const Matrix<T, M, N>& m, const Vector<T, N>& v) {
        Vector<T, M> res;
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                res[i] += (m[i][j] * v[j]);
            }
        }
        return res;
    }

    template <class T, int M, int N, int O> Matrix<T, M, O> operator*(const Matrix<T, M, N>& m, const Matrix<T, N, O>& n) {
        Matrix<T, M, O> res  = Matrix<T, M, O>();
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < O; j++) {
                for (int k = 0; k < N; k++) {
                    res[i][j] += m[i][k] * n[k][j];
                }
            }
        }
        return res;
    }

    template <class T, int M, int N> Matrix<T, M, N> operator/(const Matrix<T, M, N>& m, const T& s) {
        Matrix<T, M, N> res;
        for (int i = 0; i < M; i++) {
            res[i] = 1 / s * m[i];
        }
        return res;
    }

    template <class T, int M, int N> std::string to_string(const Matrix<T, M, N>& m) {
        std::stringstream s;
        s << "(";
        for (int i = 0; i < M; i++) {
            s << m[i] << (i != (M - 1) ? ", " : "");
        }
        s << ")";
        return s.str();
    }

    template <class T, int M, int N> Matrix<T, M, N> transpose(const Matrix<T, M, N>&m) {
        Matrix<T, M, N> res;
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                res[j][i] = m[i][j];
            }
        }
        return res;
    }
}
