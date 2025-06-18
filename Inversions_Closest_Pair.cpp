#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <map>
#include <cmath>
#define int long long
#define point pair<int, int>
using namespace std;

int sqDist(pair<point, point> x) {
    return (x.first.first - x.second.first)*(x.first.first - x.second.first) + (x.first.second - x.second.second)*(x.first.second - x.second.second);
}

pair<point, point> tiebreaker(pair<point, point> a, pair<point, point> b) {
    int arr1[] = {a.first.first, a.first.second, a.second.first, a.second.second};
    int arr2[] = {b.first.first, b.first.second, b.second.first, b.second.second};
    for (int i = 0; i < 4; i++) {
        if (arr1[i] < arr2[i]) return a;
        else if (arr2[i] < arr1[i]) return b;
    }
    return a; //both pairs of points are same if the function reaches this
}

pair<point, point> tiebreaker(pair<point, point> p1, pair<point, point> p2, pair<point, point> p3) {
    int d1 = sqDist(p1), d2 = sqDist(p2), d3 = sqDist(p3);
    pair<point, point> res;
    if (d1 < d2) {
        if (d1 < d3) res = p1; //d1 < d3 , d2
        else if (d3 < d1) res = p3; //d3 < d1 < d2
        else {
            res = tiebreaker(p1, p3);
        }
    }
    else if (d1 == d2) {
        if (d3 < d1) res = p3;
        else if (d3 > d1) res = tiebreaker(p1, p2);
        else {
            res = tiebreaker(p1, p2);
            res = tiebreaker(res, p3);
        }
    }
    else {
        if (d2 < d3) res = p2;
        else if (d3 < d2) res = p3;
        else {
            res = tiebreaker(p2, p3);
        }
    }
    return res;
}

class Comparator {
    private:
        bool increasing = true;
    public:
        void setComp(bool val) { //increasing is true is we want ascending order and false otherwise
            increasing = val;
        }

        bool operator()(int a, int b) {
            if (increasing) return a <= b;
            else return b <= a;
        }
};

int mergeSort(vector<int>& v, Comparator& c);
int merge(vector<int>& l, vector<int>& r, vector<int>& result, Comparator& c); //for the merge step

class Board {
    private:
        int n;
        vector<vector<int>> board;
    public:
        void create(int sz); //this creates the matrix
        void sortRows(Comparator comparator); //sorts the matrix row-wise
        int countInversions(); //counts inversions
        void display(); //displays matrix
};

class Players {
    private:
        int n;
        vector<point> players, y;
        map<point, int> m;
    public:
        Players(int i): n(i) {
            players.resize(n);
            y.resize(n);
        }

        void input(); //after this players will be sorted wrt x and y is y_sorted
        void showCP() { //displays closest pair
            pair<point, point> p = closestPair(players);
            cout << p.first.first << " " << p.first.second << " " << p.second.first << " " << p.second.second << endl;
        }
        pair<point, point> closestPair(vector<point> &v); //for calculating the closest pair of points
};

int mergeSort(vector<int>& v, Comparator& c) {
    if (v.size() == 1) return 0;
    int n = v.size();
    vector<int> L(n/2), R(n - n/2), res(n);
    for (int i = 0; i < n/2; i++) {
        L[i] = v[i];
    }
    for (int i = n/2; i < n; i++) {
        R[i - n/2] = v[i];
    }
    
    int x = mergeSort(L, c);
    int y = mergeSort(R, c);
    int count = merge(L, R, res, c);
    v = res;
    return x + y + count;
}

int merge(vector<int>& l, vector<int>& r, vector<int>& result, Comparator& c) {
    int count = 0;
    int L = l.size(), R = r.size(), a = 0, b = 0, x = 0;
    while (a < L && b < R) {
        if (c(l[a], r[b])) result[x++] = l[a++];
        else {
            result[x++] = r[b++];
            count += (L - a); 
        }
    }
    while (a < L) {
        result[x++] = l[a++];
    }
    while (b < R) {
        result[x++] = r[b++];
        count += (L - a); 
    }
    return count;
}

//functions pertaining to Board
void Board::create(int sz) {
    n = sz;
    vector<int>row(n);
    board.resize(n);
    board.assign(n,row);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> board[i][j];
        }
    }
}

void Board::display() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

void Board::sortRows(Comparator c) {
    for (int i = 0; i < n; i++) {
        mergeSort(board[i], c);
    }
}

int Board::countInversions() {
    vector<int> t(n*n);
    int x = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            t[x++] = board[i][j];
        }
    }
    Comparator c;
    return mergeSort(t, c);
}

//functions pertaining to Player
void Players::input() {
    for (int i = 0; i < n; i++) {
        cin >> players[i].first >> players[i].second;
        m[players[i]] = i;
    }
    y = players;
    sort(players.begin(), players.end(), [](point a, point b) {return a.first < b.first;});
    sort(y.begin(), y.end(), [](point a, point b) {if (a.second != b.second) return a.second < b.second; else return a.first < b.first;});
}

pair<point, point> Players::closestPair(vector<point>& v) {
    if (v.size() == 2) {
        pair<point, point> p1;
        if (m[v[0]] < m[v[1]]) p1 = {v[0], v[1]};
        else p1 = {v[1], v[0]};
        return p1;
    }
    if (v.size() == 3) {
        pair<point, point> p1, p2, p3;
        if (m[v[0]] < m[v[1]]) p1 = {v[0], v[1]};
        else p1 = {v[1], v[0]};
        if (m[v[1]] < m[v[2]]) p2 = {v[1], v[2]};
        else p2 = {v[2], v[1]};
        if (m[v[2]] < m[v[0]]) p3 = {v[2], v[0]};
        else p3 = {v[0], v[2]};
        return tiebreaker(p1, p2, p3);
    }
    int mid = v.size()/2;
    vector<point> L(mid), R(v.size() - mid);
    for (int i = 0; i < mid; i++) {
        L[i] = v[i];
    }
    pair<point, point> p1 = closestPair(L);

    for (int i = mid; i < v.size(); i++) {
        R[i - mid] = v[i];
    }
    pair<point, point> p2 = closestPair(R);
    int a = sqDist(p1), b = sqDist(p2);
    int d = min(a, b);
    int x = v[mid - 1].first;
    vector<point> PSY;
    bool first = true, check = false;
    pair<point, point> p3, ptemp;
    
    for (int i = 0; i < v.size(); i++) {
        if (d >= (x - v[i].first)*(x - v[i].first)) PSY.push_back(v[i]);
    }
    for (int i = 0; i < PSY.size(); i++) {
        for (int j = 1; j <= 7 && (i + j < PSY.size()); j++) {
            if (m[PSY[i]] < m[PSY[i + j]]) ptemp = {PSY[i], PSY[i + j]};
            else ptemp = {PSY[i + j], PSY[i]};
            int dist = sqDist(ptemp);
            if (dist == d) {
                check = true;
                if (first) {
                    p3 = ptemp;
                    first = false;
                }
                else p3 = tiebreaker(p3, ptemp);
            }
            else if (dist < d) {
                check = true;
                p3 = ptemp;
                d = dist;
                first = false;
            }
        }
    }
    if (check) return tiebreaker(p1, p2, p3);
    if (a < b) return p1;
    else if (a > b) return p2;
    else return tiebreaker(p1, p2);
    
}

int32_t main() {
    string s;
    cin >> s;
    Board prime;
    while (s != "END") {
        if (s == "CREATE_2D") {
            int n;
            cin >> n;
            prime.create(n);
        }
        else if (s == "DISPLAY_2D") prime.display();
        else if (s == "INVERSION_2D") cout << prime.countInversions() << endl;
        else if (s == "SORT_2D") {
            cin >> s;
            Comparator c;
            if (s == "ascending") c.setComp(true);
            else c.setComp(false);
            prime.sortRows(c);
        }
        else if (s == "CLOSEST_2D") {
            int n;
            cin >> n;
            Players play(n);
            play.input();
            play.showCP();
        }
        cin >> s;
    }
}
