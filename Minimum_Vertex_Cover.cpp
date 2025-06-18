#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <queue>
using namespace std;

template <typename T> class Kingdom;
class Sentinel {
    private:
        int id, ir;
        string name, rank;
    public:
        virtual int countHigherRanked() = 0; //counts the number of sentinels ranked higher than a given sentinel, it's abstract here
        int getIR() { //returns the integer equivelent of rank
            return ir;
        }
        int getID() { //returns the id
            return id;
        }
        //setters
        void setID(int i) {
            id = i;
        }
        void setIR(int i) {
            ir = i;
        }
        void setN(string n) {
            name = n;
        }
        void setR(string r) {
            rank = r;
        }
};

ostream& operator<<(ostream& stream, Sentinel& s) {
    return (stream << s.getID());
}
vector<Sentinel*> v; //global vector containing Sentinels
int countS = 0, countD = 0; //used in countHigherRanked
unordered_map<int, Sentinel*> m; //maps indices to Sentinels

template <typename T> //functor for comparison of Sentinels
class Comp {
    public:
        bool operator()(T* a, T* b) {
            return *a < *b;
        }
}; 

template <>
bool Comp<Sentinel>:: operator()(Sentinel* a, Sentinel* b) {
    if (a->getIR() == b->getIR()) return a->getID() < b->getID();
    return a->getIR() < b->getIR();
}

class Senapati: public Sentinel {
    public:
        Senapati(int i, string n) {
            setID(i);
            setN(n);
            setR("SENAPATI");
            setIR(1);
        }
        int countHigherRanked() {
            return 0; //because this is the highest rank
        }
};

class Dandanayaka: public Sentinel {
    public:
        Dandanayaka(int i, string n) {
            setID(i);
            setN(n);
            setR("DANDANAYAKA");
            setIR(2);
        }
        int countHigherRanked() {
            return countS;
        }
};

class Chaturangini: public Sentinel {
    public:
        Chaturangini(int i, string n) {
            setID(i);
            setN(n);
            setR("CHATURANGINI");
            setIR(3);
        }
        int countHigherRanked() {
            return countD + countS;
        }
}; 

template <typename T> 
class Kingdom {
    private:
        vector<T*> sentinels;
        vector<vector<int>> adj; //this is the adjcency list
        vector<vector<int>> dp; //this is for minimum vertex cover
    public:
        Kingdom(vector<T*>& v, vector<vector<int>>& a) {
            sentinels = v;
            adj  = a;
            sort(sentinels.begin(), sentinels.end(), Comp<T>());
            dp.resize(2, vector<int> (v.size()));
            for (int i = 0; i < v.size(); i++) {
                dp[0][i] = -1;
                dp[1][i] = -1;
            }

        }

        pair<int, int> MVC(int source, int parent);

        void Query1();
        void Query2() {
            for (auto x: sentinels) {
                cout << *x << " ";
            }
            cout << '\n';
        }

        void Query3(int i) {
            cout << m[i]->countHigherRanked() << '\n';
        }
};

template <typename T>
void Kingdom<T>::Query1() {
    cout << MVC(0, -1).first << '\n';
}

template <typename T>
pair<int, int> Kingdom<T>::MVC(int s, int p) {
    if (dp[0][s] != -1) return {dp[0][s], dp[1][s]};

    int isCount = 0;
    int sum = 0;
    for (auto x: adj[s]) {
        if (x == p) continue;
        pair<int, int> res = MVC(x, s);
        sum += res.first;
        if (res.second == 0) isCount = 1;
    }

    if (isCount) sum++;

    dp[0][s] = sum;
    dp[1][s] = isCount;

    return {dp[0][s], dp[1][s]};
}


int main() {
    int n;
    cin >> n;
    vector<vector<int>> edges(n);

    for (int i = 0; i < n - 1; i++) { //these are the edges
        int a, b;
        cin >> a >> b;
        edges[a].push_back(b);
        edges[b].push_back(a);
    }

    for (int i = 0; i < n; i++) {
        string a, b;
        cin >> a >> b;

        if (b == "SENAPATI") {
            Sentinel* t = new Senapati(i, a);
            v.push_back(t);
            m[i] = t;
            countS++;
        }

        else if (b == "DANDANAYAKA") {
            Sentinel* t = new Dandanayaka(i, a);
            v.push_back(t);
            m[i] = t;
            countD++;
        }
        else if (b == "CHATURANGINI") {
            Sentinel* t= new Chaturangini(i, a);
            m[i] = t;
            v.push_back(t);
        }
    }

    int q;
    cin >> q;

    Kingdom<Sentinel> kingdom(v, edges);
    for (int i = 0; i < q; i++) {
        int a;
        cin >> a;

        if (a == 1) {
            kingdom.Query1();
        }
        else if (a == 2) kingdom.Query2();
        else if (a == 3) {
            cin >> a;
            kingdom.Query3(a);
        }
    }

}
