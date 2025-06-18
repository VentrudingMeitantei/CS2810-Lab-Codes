#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <ios>
using namespace std;

string conc(string& s, int a) {
    string temp;
    for (int i = 0; i < a; i++) {
        temp += s;
    }
    return temp;
}
class Complex {
    public:
        long long int real, imag;
        Complex(long long int r = 0, long long int i = 0): real(r), imag(i) {}

        Complex operator+(Complex &b) {
            long long int r = real + b.real;
            long long int i = imag + b.imag;
            Complex c(r, i);
            return c;
        }

        Complex operator-(Complex &b) {
            long long int r = real - b.real;
            long long int i = imag - b.imag;
            Complex c(r, i);
            return c;
        }

        Complex operator*(Complex &b) {
            long long int r = (real)*(b.real) - (imag)*(b.imag);
            long long int i = (real)*(b.imag) + (imag)*(b.real);
            Complex c(r, i);
            return c;
        }
};

std::ostream& operator<< (std::ostream& stream, Complex& C) { //overloading << for printing complex numbers
    return stream << C.real << " " << C.imag;
}

template <typename T>
class Polynomial {
    public:
        int deg = 0;
        vector<T> Poly;
        Polynomial(vector<T> p): Poly(p) {
            deg = p.size();
        }
        Polynomial<T> operator*(Polynomial<T>& P); //operator overload
        void print();
};

template <typename T>
class Multiply {
    public:
        Polynomial<T> make(Polynomial<T>& P1, Polynomial<T>& P2) {
            P1.Poly.resize(16); //since the bound on the degree is 10
            P2.Poly.resize(16);
            Polynomial<T> Res(mult(P1.Poly, P2.Poly));
            Res.deg = P1.deg + P2.deg - 1;
            return Res;
        }
        vector<T> mult(vector<T>& v1, vector<T>& v2); //multiplies two polynomials we shall use karatsuba's algorithm here (algorithm yet to be written)
       
};

template <typename T>
class Solve {
    public:
        T sol(Polynomial<T>& P, int a); //Solves the polynomial P at the value a
};

template <typename T>
class Differentiate {
    public:
        Polynomial<T> diff(Polynomial<T>& P); //differentiates the polynomial
};

//fuctions of Polynomial
template <typename T>
Polynomial<T> Polynomial<T>::operator*(Polynomial<T>& P) { //this isthe operator overloading part
            Multiply<T> M;
            return M.make(*this, P);
}
template <typename T>
void Polynomial<T>::print() {
    for (int i = 0; i < deg; i++) {
        cout << Poly[i] << " ";
    }
    cout << endl;
}

template <>
void Polynomial<long double>::print() {
    for (int i = 0; i < deg; i++) {
        cout << fixed << setprecision(6) << Poly[i] << " ";
    }
    cout << endl;
}

//functions of Differentiate
template <typename T>
Polynomial<T> Differentiate<T>::diff(Polynomial<T>& P) {
    vector<T> v;
    for (int i = 1; i < P.Poly.size(); i++) {
        v.push_back(((T)(i))*(P.Poly[i]));
    }

    Polynomial<T> Res(v);
    return Res;
}

//functions of Solve
template <>
string Solve<string>::sol(Polynomial<string>& P, int a) {
    int ex = P.Poly.size();
    long long int x = pow(a, ex - 1);
    string s;
    for (int i = ex - 1; i >= 0; i--) {
        s += conc(P.Poly[i], x); 
        x = x/a;
    }

    return s;
}
template <typename T> //this is for int and float for which * operator is defined
T Solve<T>::sol(Polynomial<T>& P, int a) {
    int ex = P.Poly.size();
    long long int x = pow(a, ex - 1);
    T sum{};
    for (int i = ex - 1; i >= 0; i--) {
        sum += P.Poly[i]*x;
        x /= a;
    }
    return sum;
}
//functions for Multiply
template <typename T>
vector<T> Multiply<T>::mult(vector<T>& v1, vector<T>& v2) {
    int n = v1.size();
    if (n == 1) { //at each level v1 and v2 have the same size
        return {v1[0]*v2[0], 0};
    }
    vector<T> res(2*n);
    vector<T> a(v1.begin(), v1.begin() + n/2), b(v1.begin() + n/2, v1.end());
    vector<T> c(v2.begin(), v2.begin() + n/2), d(v2.begin() + n/2, v2.end());
    vector<T> temp1(n/2), temp2(n/2);
    vector<T> ac, bd, combo;
    for (int i = 0; i < n; i++) {
        if (i < n/2) temp1[i] = a[i] + b[i];
        else temp2[i - n/2] = c[i - n/2] + d[i - n/2];
    }
    ac = mult(a, c);
    bd = mult(b, d);
    combo = mult(temp1, temp2); // combo = (a + b)(c + d)
    for (int i = 0; i < 2*n; i++) {
        if (i < n) {
            res[i] = res[i] + ac[i];
        }
        if (i >= n) {
            res[i] = res[i] + bd[i - n];
        }
        if (i >= n/2 && i < (3*n)/2) {
            res[i] = res[i] + combo[i - n/2] - ac[i - n/2] - bd[i - n/2]; 
        }
    }
    
    return res;
}
int main() {
    int q, x;
    string s;
    cin >> q;
    for (int i = 0; i < q; i++) {
        cin >> x;
        if (x == 1) {
            int deg1, deg2;
            cin >> s;
            if (s == "complex") {
                vector<Complex> v1, v2;
                cin >> deg1;
                for (int j = 0; j < deg1; j++) {
                    Complex temp;
                    cin >> temp.real >> temp.imag;
                    v1.push_back(temp);
                }
                cin >> deg2;
                for (int j = 0; j < deg2; j++) {
                    Complex temp;
                    cin >> temp.real >> temp.imag;
                    v2.push_back(temp);
                }
                Polynomial<Complex> P1(v1), P2(v2);
                (P1*P2).print();
            }

            else if (s == "integer") {
                vector<long long int> v1, v2;
                cin >> deg1;
                for (int j = 0; j < deg1; j++) {
                    long long int temp;
                    cin >> temp;
                    v1.push_back(temp);
                }
                cin >> deg2;
                for (int j = 0; j < deg2; j++) {
                    long long int temp;
                    cin >> temp;
                    v2.push_back(temp);
                }
                Polynomial<long long int> P1(v1), P2(v2);
                (P1*P2).print();
            }

            else if (s == "float") {
                vector<long double> v1, v2;
                cin >> deg1;
                for (int j = 0; j < deg1; j++) {
                    long double temp;
                    cin >> temp;
                    v1.push_back(temp);
                }
                cin >> deg2;
                for (int j = 0; j < deg2; j++) {
                    long double temp;
                    cin >> temp;
                    v2.push_back(temp);
                }
                Polynomial<long double> P1(v1), P2(v2);
                (P1*P2).print();
            }
        }

        else if (x == 2) {
            int deg;
            string s;
            cin >> s;

            if (s == "integer") {
                cin >> deg;
                vector<long long int> v;
                for (int i = 0; i < deg; i++) {
                    long long int temp;
                    cin >> temp;
                    v.push_back(temp);
                }
                Solve<long long int> E;
                int a;
                cin >> a;
                Polynomial<long long int> P(v);
                cout << E.sol(P, a) << endl;
            }

            else if (s == "float") {
                cin >> deg;
                vector<long double> v;
                for (int i = 0; i < deg; i++) {
                    long double temp;
                    cin >> temp;
                    v.push_back(temp);
                }
                Solve<long double> E;
                int a;
                cin >> a;
                Polynomial<long double> P(v);
                cout << fixed << setprecision(6) << E.sol(P, a) << endl;
            }

            else if (s == "string") {
                cin >> deg;
                vector<string> v;
                for (int i = 0; i < deg; i++) {
                    string temp;
                    cin >> temp;
                    v.push_back(temp);
                }
                Solve<string> E;
                int a;
                cin >> a;
                Polynomial<string> P(v);
                cout << E.sol(P, a) << endl;
            }
        }

        else if (x == 3) {
            int deg;
            string s;
            cin >> s;

            if (s == "integer") {
                cin >> deg;
                vector<long long int> v;
                for (int i = 0; i < deg; i++) {
                    long long int temp;
                    cin >> temp;
                    v.push_back(temp);
                }
                Differentiate<long long int> D;
                Polynomial<long long int> P(v);
                P = D.diff(P);
                P.print();
            }

            else if (s == "float") {
                cin >> deg;
                vector<long double> v;
                for (int i = 0; i < deg; i++) {
                    long double temp;
                    cin >> temp;
                    v.push_back(temp);
                }
                Differentiate<long double> D;
                Polynomial<long double> P(v);
                P = D.diff(P);
                P.print();
            }
        }
    }    
}