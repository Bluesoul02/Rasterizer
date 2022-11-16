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
                    throw runtime_error("list size bigger than N");
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

            template T Vector<T,N>::at( size_t i) const {
                if (i > N)
                    throw runtime_error("index out of range");
                return vector[i];
            }

            T Vector<T,N>::operator[]( size_t i) const {
                at(i);
            }

            T & Vector<T,N>::operator[]( size_t i) {
                
            }

            Vector<T,N> & Vector::operator+=( const Vector<T,N> & );

            Vector<T,N> cross( const Vector<T,N> &, const Vector<T,N> & );
            T dot( const Vector<T,N> &, const Vector<T,N> & );
            bool isnan( const Vector & );
            bool is_unit( const Vector<T,N> & );
            bool nearly_equal( const Vector<T,N> &, const Vector<T,N> & );
            T Vector<T,N>::norm();
            bool operator==( const Vector<T,N> &, const Vector<T,N> & );
            bool operator!=( const Vector<T,N> & v1, const Vector<T,N> & v2 );
            std::ostream operator<<( std::ostream &, const Vector<T,N> v );
            Vector<T,N> operator+( const Vector<T,N> &, const Vector<T,N> & );
            Vector<T,N> operator-( const Vector<T,N> & );
            Vector<T,N> operator-( const Vector<T,N> &, const Vector<T,N> & );
            Vector<T,N> operator*( const T &, const vector<T,N> & );
            Vector<T,N> operator*( const vector<T,N> &, const T & );
            Vector<T,N> operator*( const vector<T,N> &, const Vector<T,N> & );
            Vector<T,N> operator/( const Vector<T,N> & v, const T & s );
            T Vector<T,N>::sq_norm();
            std::string to_string( const Vector<T,N> & );
            Vector<T,N> unit_vector( const Vector<T,N> & v );
    };
}