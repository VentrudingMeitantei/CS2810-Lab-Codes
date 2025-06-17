#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include <set>
using namespace std;
class Island;

int countSetBits(int n)
{
    int count = 0;
    while (n) {
        count += n & 1;
        n >>= 1;
    }
    return count;
}

long double dist(double x1, double y1, double x2, double y2) {
    return sqrtl((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

class Island {
    private:
        long double furthest; //this is the distance to the furthest point on the island from the center
        double cenX, cenY; //coordinates of the center
    public:
        string id; //made this public to make insertion and extreaction easier
        //setters
        void setCen(double x, double y);
        void setF(long double f);
        //getters
        long double getF();
        double getCX();
        double getCY();

        virtual void make() = 0; //this sets up the center and furthest distance
        //overloading insertion and extraction
        ostream& operator<<(ostream& stream); //this doesn't really need to be virtual
        virtual istream& helper(istream& stream) = 0; 
        //we declare overloaded operators as friends
        friend ostream& operator<<(ostream& stream, Island* a);
        friend istream& operator>>(istream& stream, Island* a);
};

ostream& operator<<(ostream& stream, Island* a) {
    return stream << a->id;
}

istream& operator>>(istream& stream, Island* a) {
    return a->helper(stream);
}

class Rectangle: public Island {
    private:
        int x1, x2, x3, x4;
        int y1, y2, y3, y4;
    public:
        void make();
        istream& helper(istream& stream);
};

class Triangle: public Island {
    private:
        int x1, x2, x3;
        int y1, y2, y3;
    public:
        void make();
        istream& helper(istream& stream);
};

class Circle: public Island {
    public:
        int x, y, r;
    public:
        void make();
        istream& helper(istream& stream);
};

class Archipelago {
    private:
        vector<Island*> Isles; //list of islands (we don't really need this)
        map<int, Island*> i_i; //index to island map
        vector<vector<int>> adj; //adjacency list for the graph 
        vector<vector<int>> dp; //for calculating the path
    public:
        Archipelago(vector<Island*>& v) {
            Isles = v;
            for (int i = 0; i < Isles.size(); i++) {
                i_i[i] = Isles[i];
            }
            adj.resize(Isles.size());
            dp.resize((1 << Isles.size()), vector<int> (Isles.size(), -1));
        }
        void makeAdj();
        bool solve(); 
        void printPath();
        void printPath(int mask);
};

//for Island
ostream& Island::operator<<(ostream& stream) {
    return stream << id;
}

void Island::setCen(double x, double y) {
    cenX = x;
    cenY = y;
}

void Island::setF(long double f) {
    furthest = f;
}

long double Island::getF() {
    return furthest;
}

double Island::getCX() {
    return cenX;
}

double Island::getCY() {
    return cenY;
}

//for Rectangle
istream& Rectangle::helper(istream& stream) {
    return stream >> id >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> x4 >> y4;
}

void Rectangle::make() {
    double sumX = x1 + x2 + x3 + x4;
    double sumY = y1 + y2 + y3 + y4;
    double x_c = (double)sumX/4;
    double y_c = (double)sumY/4;
    setCen(x_c, y_c);
    setF(dist(getCX(), getCY(), x1, y1)); //center is equidistant to all vertices, so any distance works
}

//for Triangle
istream& Triangle::helper(istream& stream) {
    return stream >> id >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
}

void Triangle::make() {
    double sumX = x1 + x2 + x3;
            double sumY = y1 + y2 + y3;
            double x_c = (double)sumX/3;
            double y_c = (double)sumY/3;
            setCen(x_c, y_c);
            long double d1 = dist(getCX(), getCY(), x1, y1);
            long double d2 = dist(getCX(), getCY(), x2, y2);
            long double d3 = dist(getCX(), getCY(), x3, y3);
            setF(max(d1, max(d2, d3))); // we basically set the distnce to the furthest vertex as the furthest distance
}

//for Circle
istream& Circle::helper(istream& stream) {
    return stream >> id >> x >> y >> r;
}

void Circle::make() {
    setCen(x, y);
    setF(r);
}

//for Archipelago
bool Archipelago::solve() {
    int n = Isles.size();
    for (int i = 0; i < n; i++) {
        dp[(1 << i)][i] = -2; //these are the roots
    }

    for (int i = 1; i < (1 << n); i++) {
        for (int j = 0; j < n; j++) {
            if (!(i & (1 << j))) continue; //if j is not in the mask, skip
            if (dp[i][j] == -1) continue;
            for (auto x: adj[j]) {
                if (i & (1 << x)) continue; //i.e., if this neighbour is already present, you don't need it
                dp[i | (1 << x)][x] = j;
            }
        }
    }

    bool check = false;
    for (int i = 0; i < n; i++) {
        check |= (dp[(1 << n) - 1][i] != -1);
    }

    return check;
}

void Archipelago::makeAdj() {
    int n = Isles.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) continue;
            if(Isles[i]->getF() + Isles[j]->getF() >= dist(Isles[i]->getCX(), Isles[i]->getCY(), Isles[j]->getCX(), Isles[j]->getCY())) {
                adj[i].push_back(j);
            }
        }
    }
}

void Archipelago:: printPath() {
    int n = Isles.size();
    bool check = false;
    for (int i = 0; i < n; i++) {
        if (dp[(1 << n) - 1][i] != -1) {
            check = true;
            break;
        }
    }

    if (check) {
        printPath((1 << n) - 1);
        return;
    }
    int count = 0, mask;
    for (int i = 0; i < (1 << n) - 1; i++) {
        bool check = false;
        for (int j = 0; j < n; j++) {
            if (dp[i][j] != -1) {
                check = true;
                break;
            }
        }

        if (check) {
            int c = countSetBits(i);
            if (count < c) {
                count = c;
                mask = i;
            }
        }
    }

    cout << count << endl;
    printPath(mask);
}

void Archipelago::printPath(int mask) {
    int n = Isles.size();
    vector<int> v;
    int x;
    for (int i = 0; i < n; i++) {
        if (dp[mask][i] != -1) {
            x = i;
            break;
        }
    }
    int temp;
    while (x != -2) {
        v.push_back(x);
        temp = mask;
        mask  = mask & ~(1 << x);
        x = dp[temp][x];
    }

    for (int i = 0; i < v.size(); i++) {
        *i_i[v[i]] << cout << " ";
    }
    cout << endl;
}

int main() {
    int t;
    cin >> t;
    //Archipelago A;
    vector<Island*> v;
    for (int i = 0; i < t; i++) {
        string s;
        cin >> s;
        if (s == "RECTANGLE") {
            Island* temp = new Rectangle;
            cin >> temp;
            temp->make();
            v.push_back(temp);
        }

        else if (s == "TRIANGLE") {
            Island* temp = new Triangle;
            cin >> temp;
            temp->make();
            v.push_back(temp);
        }

        else if (s == "CIRCLE") {
            Island* temp = new Circle;
            cin >> temp;
            temp->make();
            v.push_back(temp);
        }
    }

    Archipelago A(v);
    A.makeAdj();
    if(A.solve()) {
        cout << "YES" << endl;
    }
   else cout << "NO" << endl;
    A.printPath();
}
