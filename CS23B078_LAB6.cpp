#include <iostream>
#include <vector>
#include <cassert>
#define int long long
using namespace std;
//the code precomputes primes using the sieve of eratosthenes
//fibonacci numbers are calculated using matrix exponentiation
//the current code uses quicksort with end element as pivot each time
//partition code is based on the pseudocode given in the textbook
class NumberAnalyzer;
class CurrencySorter {
    private:
        int partition(vector<int>& v, int l, int r); //gives the index of the pivot
        void quickSort(vector<int>& v, int l, int r); //for quick sorting 
    public:
        void operator()(vector<int>& v) { //functor for sorting
            quickSort(v, 0, v.size() - 1);
        }
};

class FibonacciGenerator {
    private:
        vector<int> mat = {1, 1, 1, 0};
        vector<int> matExp(int n); //for exponentiating a matrix
        int fib(int n);
        int MOD = 1e9 + 7;

    public:
        int operator()(int n) { //functor for nth fibonacci
            if (n == 0) return 0;
            else if (n == 1) return 1;
            return fib(n);
        } 
};

class PrimeCalculator {
    private:
        vector<int> sieve;//this currenlty only has primes from 1 to 1e6
    public:
        void makesieve();
        void printPrimes(int l, int r); //prints primes from l to r
        int printSum(int l, int r); //prints sum of primes from l to r
        friend class NumberAnalyzer;
};
class NumberAnalyzer {
    public:
        bool isSquareFree(int x); //checks if a number is squarefree
        bool isDivisor(int D, int x); //checks if x is a factor of D
        int countDivisors(int x); //counts divisors
        int sumDivisors(int x); //gives sum of divisors
};

//CurrencySorter
int CurrencySorter::partition(vector<int>& v, int l, int r) {
    if (l == r) return l;
    int L = l - 1;
    for (int i = l; i < r; i++) {
        if (v[i] < v[r]) {
            L++;
            swap(v[L], v[i]);
        }
    }
    swap(v[L + 1], v[r]);
    return L + 1;
}

void CurrencySorter::quickSort(vector<int>& v, int l, int r) {
    if (l >= r) return;
    int p = partition(v, l, r);
    quickSort(v, l, p - 1);
    quickSort(v, p + 1, r);
}
//fibonacciGenerator
vector<int> FibonacciGenerator::matExp(int n) {
    if (n == 1) return mat;
    vector<int> t1 = matExp(n/2);
    vector<int> t2(4);
    t2[0] = (t1[0]*t1[0])%MOD + (t1[1]*t1[2])%MOD; t2[0] %= MOD;
    t2[1] = (t1[0]*t1[1])%MOD + (t1[1]*t1[3])%MOD; t2[1] %= MOD;
    t2[2] = (t1[2]*t1[0])%MOD + (t1[2]*t1[3])%MOD; t2[2] %= MOD;
    t2[3] = (t1[2]*t1[1])%MOD + (t1[3]*t1[3])%MOD; t2[3] %= MOD;
    
    if (n%2 == 0) return t2;
    else {
        vector<int> t3 = {(t2[0] + t2[2])%(int)MOD, (t2[1] + t2[3])%(int)MOD, t2[0], t2[1]};
        return t3;
    }
}

int FibonacciGenerator::fib(int n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    vector<int> temp = matExp(n - 1);

    return temp[0];
}
//PrimeCalculator
void PrimeCalculator::makesieve() {
    vector<bool> v(1e6 + 1, true);
    for (int i = 2; i <= 1e3; i++) {
        if (v[i]) {
            for (int j = i*i; j <= 1e6; j += i) {
                v[j] = false;
            }
        }
    }
    for (int i = 2; i <= 1e6; i++) {
        if (v[i]) sieve.push_back(i);
    }
}

void PrimeCalculator::printPrimes(int l, int r) {
    vector<bool> temp(r - l + 1, true);
    for (auto x: sieve) {
        if (x > r) break; 
        for (int i = x*(l/x + 1 - (l%x == 0)) - l; i < r - l + 1; i += x) {
            if (i + l == x) continue;
            temp[i] = false;
        }
    }
    for (int i = 0; i < r - l + 1; i++) {
        if (temp[i] && i + l != 1) cout << l + i << " ";
    }
    cout << endl;
}

int PrimeCalculator::printSum(int l, int r) {
    vector<bool> temp(r - l + 1, true);
    int sum = 0;
    for (auto x: sieve) {
        if (x > r) break;
        
        for (int i = x*(l/x + 1 - (l%x == 0)) - l; i < r - l + 1; i += x) {
            if (i + l == x) continue;
            temp[i] = false;
        }
    }
    for (int i = 0; i < r - l + 1; i++) {
        if (temp[i] && i + l != 1) sum += (l + i);
    }
    return sum;
}

//these may still need to be optimized
bool NumberAnalyzer::isSquareFree(int x) {
    if (x == 1) return false; 
    for (int i = 2; i*i <= x ; i++) {
        if (isDivisor(x, i)) x = x/i;
        if (isDivisor(x, i)) return false;
    }
    return true;
}
bool NumberAnalyzer::isDivisor(int D, int x) {
    return D%x == 0;
}
int NumberAnalyzer::countDivisors(int x) {
    int count = 0;
    for (int i = 1; i*i <= x; i++) {
        if (i*i == x) count++;
        else if (isDivisor(x, i)) count += 2;
    }
    return count;
}
int NumberAnalyzer::sumDivisors(int x) {
    int sum = 0;
    for (int i = 1; i*i <= x; i++) {
        if (i*i == x) sum += i;
        else if (isDivisor(x, i)) {
            sum += i;
            sum += (x/i);
        }
    }
    return sum;
}

int32_t main() {
    int x, t;
    cin >> x;
    if (x == 1) {
        cin >> t;
        for (int i = 0; i < t; i++) {
            int n;
            cin >> n;
            vector<int> v(n);
            for (int j = 0; j < n; j++) {
                cin >> v[j];
            }

            CurrencySorter sorter;
            sorter(v);

            for (auto x: v) {
                cout << x << " ";
            }
            cout << endl;
        }
    }

    else if (x == 2) {
        cin >> t;
        for (int i = 0; i < t; i++) {
            int n;
            cin >> n;
            FibonacciGenerator fib;
            cout << fib(n) << endl;
        }
    }

    else if (x == 3) {
        cin >> t;
        PrimeCalculator p;
        p.makesieve();
        for (int i = 0; i < t; i++) {
            int l, r;
            string s;
            cin >> s >> l >> r;
            if (s == "printPrimes") {
                p.printPrimes(l, r);
            }
            else if (s == "printPrimeSum"){
                cout << p.printSum(l, r) << endl;
            }
        }
    }

    else if (x == 4) {
        cin >> t;
        NumberAnalyzer N;
        for (int i = 0; i < t; i++) {
            string s;
            int n;
            cin >> s >> n;

            if (s == "isSquareFree") {
                if (N.isSquareFree(n)) cout << "yes" << endl;
                else cout << "no" << endl;
            }

            else if (s == "countDivisors") {
                cout << N.countDivisors(n) << endl;
            }

            else if (s == "sumOfDivisors") {
                cout << N.sumDivisors(n) << endl;
            }
        }
    }
} 