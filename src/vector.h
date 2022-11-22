#include <iostream>
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
                if (list.size() > N)
                    throw std::runtime_error("list size bigger than N");
                for (int i = 0; i < N; i++) {
                    if (i > list.size())
                        vector[i] = 0;
                    else
                        vector[i] = list[i];
                    }
            }

            Vector( const Vector<T,N> &v ) {
                for (int i = 0; i < N; i++) {
                    vector[i] = v[i];
                }
            }

            template <T> T at(size_t i) const {
                if (i > N)
                    throw std::runtime_error("index out of range");
                return vector[i];
            }

            T operator[]( size_t i) const {
                at(i);
            }

            T & operator[]( size_t i) {
                at(i);
            }

            Vector<T,N> & operator+=(const Vector<T,N> & v) {
                vector += v;
            }

            Vector<T,N> cross( const Vector<T,N> &v1, const Vector<T,N> &v2) {

            }

            T dot( const Vector<T,N> &v1, const Vector<T,N> &v2) {

            }

            bool isnan( const Vector &v) {
                for (int i = 0; i < N; i++)
                    if (isnan(vector[i])) return true;
            }

            bool is_unit( const Vector<T,N> &v) {
                return false;
            }

            bool nearly_equal( const Vector<T,N> &v1, const Vector<T,N> &v2) {
                return false;
            }

            T norm() {

            }

            bool operator==(const Vector<T,N> &v) {
                return false;
            }

            bool operator!=(const Vector<T,N> & v) {
                return false;
            }

            std::ostream operator<<(std::ostream &s) {

            }

            Vector<T,N> operator+(const Vector<T,N> &v) {

            }

            Vector<T,N> operator-() {

            }

            Vector<T,N> operator-(const Vector<T,N> &v) {

            }

            Vector<T,N> operator*(const T &t) { // The product of a scalar and a vector.

            }

            Vector<T,N> operator*(const T &t) { // The product of a vector and a scalar.

            }

            Vector<T,N> operator*(const Vector<T,N> &v) {

            }

            Vector<T,N> operator/(const T &t) {

            }

            template <T> T sq_norm() {
                return 0;
            }

            std::string to_string() {
                return "";
            }

            Vector<T,N> unit_vector(const Vector<T,N> &v) {

            }
    };
}