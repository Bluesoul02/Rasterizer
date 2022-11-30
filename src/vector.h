#include <iostream>
#include <float.h>
#include <string>
#include <cmath>
#include <sstream>

namespace aline {
    template <class T, int N>
    class Vector {
        private:
            T vector[N];
        public:
            Vector() {
                for (int i = 0; i < N; i++) {
                    vector[i] = 0;
                }
            }

            Vector(std::initializer_list<T> list) {
                if (list.size() > (size_t) N)
                    throw std::runtime_error("list size bigger than N");
                T l[list.size()];
                int s = 0;
                for (auto& i : list) {
                    l[s++] = i;
                }
                for (int i = 0; i < N; i++) {
                    if ((size_t) i >= list.size())
                        vector[i] = 0;
                    else
                        vector[i] = l[i];
                }
            }

            Vector( const Vector<T,N> &v ) {
                for (int i = 0; i < N; i++) {
                    vector[i] = v[i];
                }
            }

            T at(size_t i) const {
                if (i >= N)
                    throw std::runtime_error("index out of range");
                return vector[i];
            }

            T operator[](size_t i) const {
                return at(i);
            }

            T & operator[](size_t i) {
                if (i > N)
                    throw std::runtime_error("index out of range");
                return vector[i];
            }

            Vector<T,N> & operator+=(const Vector<T,N> & v) {
                for (int i = 0; i < N; i++) {
                    vector[i] += v[i];
                }
                return *this;
            }
    };

    template <class T, int N> Vector<T, N> cross(const Vector<T, N>& u, const Vector<T, N>& v) {
        if (N < 3)
            throw std::runtime_error("vector need at least 3 elements");
        Vector<T, N> cross_P;
        cross_P[0] = u[1] * v[2] - u[2] * v[1];
        cross_P[1] = u[2] * v[0] - u[0] * v[2];
        cross_P[2] = u[0] * v[1] - u[1] * v[0];
        return cross_P;
    }

    template <class T, int N> T dot(const Vector<T, N>& u, const Vector<T, N>& v) {
        T product = 0;

        for (int i = 0; i < N; i++)
            product += u[i] * v[i];

        return product;
    }

    template <class T, int N> bool isnan(const Vector<T, N>& v) {
        for (int i = 0; i < N; i++)
            if (std::isnan(v[i])) return true;
        return false;
    }

    template <class T, int N> bool is_unit(const Vector<T, N>& v) {
        return std::round(norm(v)) == 1; // using round because double aren't as precise as floats
    }

    template <class T, int N> bool nearly_equal(const Vector<T, N>& u, const Vector<T, N>& v) {
        float A;
        float B;
        for (int i = 0; i < N; i++) {
            A = u[i];
            B = v[i];
            float diff = fabs(A - B);
            A = fabs(A);
            B = fabs(B);
            float largest = (B > A) ? B : A;

            if (diff > largest * FLT_EPSILON)
                return false;
        }
        return true;
    }

    template <class T, int N> T norm(const Vector<T, N>& v) {
        return sqrt(dot(v, v));
    }

    template <class T, int N> bool operator==(const Vector<T, N>& u, const Vector<T, N>& v) {
        for (int i = 0; i < N; i++) {
            if (u[i] != v[i])
                return false;
        }
        return true;
    }

    template <class T, int N> bool operator!=(const Vector<T, N>& u, const Vector<T, N>& v) {
        for (int i = 0; i < N; i++) {
            if (u[i] == v[i])
                return false;
        }
        return true;
    }

    template <class T, int N> std::ostream& operator<<(std::ostream& out, const Vector<T, N>& v) {
        out << to_string(v);
        return out;
    }

    template <class T, int N> Vector<T, N> operator+(const Vector<T, N>& u, const Vector<T, N>& v) {
        Vector<T, N> res;
        for (int i = 0; i < N; i++) {
            res[i] = u[i] + v[i];
        }
        return res;
    }

    template <class T, int N> Vector<T, N> operator-(const Vector<T, N>& v) {
        Vector<T, N> res;
        for (int i = 0; i < N; i++) {
            res[i] = -v[i];
        }
        return res;
    }

    template <class T, int N> Vector<T, N> operator-(const Vector<T, N>& u, const Vector<T, N>& v) {
        Vector<T, N> res;
        for (int i = 0; i < N; i++) {
            res[i] = u[i] - v[i];
        }
        return res;
    }

    template <class T, int N> Vector<T, N> operator*(const T& t, const Vector<T, N>& v) {
        Vector<T, N> res;
        for (int i = 0; i < N; i++) {
            res[i] = t * v[i];
        }
        return res;
    }

    template <class T, int N> Vector<T, N> operator*(const Vector<T, N>& v, const T& t) {
        Vector<T, N> res;
        for (int i = 0; i < N; i++) {
            res[i] = v[i] * t;
        }
        return res;
    }

    template <class T, int N> Vector<T, N> operator*(const Vector<T, N>& u, const Vector<T, N>& v) {
        Vector<T, N> res;
        for (int i = 0; i < N; i++) {
            res[i] = u[i] * v[i];
        }
        return res;
    }

    template <class T, int N> Vector<T, N> operator/(const Vector<T, N>& v, const T& t) {
        if (t == 0)
            throw std::runtime_error("div by zero");
        Vector<T, N> res;
        for (int i = 0; i < N; i++) {
            res[i] = v[i] / t;
        }
        return res;
    }

    template <class T, int N> T sq_norm(const Vector<T, N>& v) {
        return norm(v) * norm(v);
    }

    template <class T, int N> std::string to_string(const Vector<T, N>& v) {
        std::stringstream s;
        s << "(";
        for (int i = 0; i < N; i++) {
            s << std::fixed << v[i] << (i != (N - 1) ? ", " : "");
        }
        s << ")";
        return s.str();
    }

    template <class T, int N> Vector<T, N> unit_vector(const Vector<T, N>& v) {
        Vector<T, N> res;
        for (int i = 0; i < N; i++) {
            res[i] = v[i] / norm(v);
        }
        return res;
    }
}

using uint = unsigned int;
using real = double;
using Vec2i = aline::Vector<int,2ul>;
using Vec2r = aline::Vector<real,2ul>;