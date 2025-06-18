#include <iostream>
#include <iomanip>
#include <cassert>
#include <cmath>
#define MOD 1000000007
using namespace std;

long long gcd(long long a, long long b);

class Complex {
    private:
        double real, imag;
    public:
        Complex(): real(0), imag(0) {}
        Complex(double r, double i): real(r) , imag(i) {}

        void add(Complex& C);
        void sub(Complex& C);
        void mult(Complex& C);

        double getR();
        double getI();

        virtual void print();
};

class Rational: public Complex {
    private:
        long long p, q;
    public:
        Rational(long long P, long long Q): Complex(((double)P/Q), 0) {
            assert(Q != 0);
            if ((P < 0 && Q < 0) || (P >= 0 && Q > 0)) {
                p = abs(P);
                q = abs(Q);
            }
            else {
                p = -1*abs(P);
                q = abs(Q);
            }
        }

        void reduce();
        void print();
};

class Natural: Rational {
    private:
        int n;
    public:
        Natural(int N): Rational(N, 1) {
            n = N;
        }
        
        bool checkPrime();
        long long inverseModulo();
        long long binexp(int x);
        void print();
};

void Complex::add(Complex& C) {
    real += C.real;
    imag += C.imag;
}

void Complex::sub(Complex& C) {
    real -= C.real;
    imag -= C.imag;
}

void Complex::mult(Complex& C) {
    double r = real, i = imag;
    real = r*(C.real) - i*(C.imag);
    imag = r*(C.imag) + i*(C.real);
}

void Complex::print() {
    cout << fixed << setprecision(3) << real << " " << imag << endl;
}

double Complex::getR() {
    return real;
}

double Complex::getI() {
    return imag;
}

void Rational::reduce() {
    long long x = gcd(abs(p), q);
    p /= x;
    q /= x;
    cout << p << " " << q << endl;
}

long long gcd(long long a, long long b) {
    if (b == 0) return a;
    return gcd(b , a%b);
}

void Rational::print() {
    cout << fixed << setprecision(3) << getR() << endl;
}

bool Natural::checkPrime() {
    int x = sqrt(n);
    if (n == 1) return false;
    for (int i = 2; i <= x; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

void Natural::print() {
    cout << n << endl;
}

long long Natural::inverseModulo() {
    return binexp(MOD - 2);
}

long long Natural::binexp(int x) {
    if (x == 0) return 1;
    long long y = binexp(x/2)%(MOD);
    if (x%2 == 0) return (y*y)%(MOD);
    return (((n*y)%(MOD))*y)%(MOD);
}

int main() {
    int t;
    cin >> t;
    for (int i = 0; i < t; i++) {
        string s1, s2;
        cin >> s1 >> s2;
        if (s1 == "complex") {
            double r , c;
            cin >> r >> c;
            Complex C1(r , c);
            if (s2 == "add") {
                cin >> r >> c;
                Complex C2(r , c);
                C1.add(C2);
                C1.print();
            }

            else if (s2 == "sub") {
                cin >> r >> c;
                Complex C2(r , c);

                C1.sub(C2);
                C1.print();
            }

            else if (s2 == "mult") {
                cin >> r >> c;
                Complex C2(r , c);

                C1.mult(C2);
                C1.print();
            }
        }

        else if (s1 == "rational") {
            int p, q;
            cin >> p >> q;
            Rational R1(p, q);

            if (s2 == "add") {
                cin >> p >> q;
                Rational R2(p , q);

                R1.add(R2);
                R1.print();
            }

            else if (s2 == "sub") {
                cin >> p >> q;
                Rational R2(p , q);

                R1.sub(R2);
                R1.print();
            }

            else if (s2 == "mult") {
                cin >> p >> q;
                Rational R2(p , q);

                R1.mult(R2);
                R1.print();
            }

            else if (s2 == "reduce") {
                R1.reduce();
            }
        }

        else if (s1 == "natural") {
            int n;
            cin >> n;
            Natural N1(n);

            if (s2 == "isprime") {
                cout << N1.checkPrime() << endl;
            }

            else if (s2 == "inverse") {
                cout << N1.inverseModulo() << endl;
            }
        }
    }
}
