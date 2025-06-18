#include <iostream>
#include <vector>
#include <set>
using namespace std;

class Graph {
    private:
        int n, m;    
        vector<set<int>> adj;
    public:
        Graph(int v, int e) {
            n = v;
            m = e;
            adj.resize(n);
        }

        void addEdge(int x, int y) {
            adj[x].insert(y);
            adj[y].insert(x);
        }

        void removeEdge(int x, int y) {
            adj[x].erase(y);
            adj[y].erase(x);
        }

        bool isReachable(int x, int y) {
            vector<bool> visited(n);
            return isReachable(x, y, adj, visited);
        }

        bool isReachable(int x, int y, const vector<set<int>>& adj, vector<bool>& visited) {
            if (x == y) return true;
            if (visited[x]) return false;
            visited[x] = true;
            bool check = false;

            for (auto i: adj[x]) {
                check |= isReachable(i, y, adj, visited);
            }

            return check;
        }

        Graph& operator!() {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (adj[i].count(j) > 0) adj[i].erase(j);
                    else if (i != j) adj[i].insert(j);
                }
            }

            return *this;
        }

        Graph& operator+(Graph &G) {
            int v = G.n;
            for (int i = 0; i < v; i++) {
                if (i >= n) {
                    adj.push_back(G.adj[i]);
                }
                else {
                    for (auto x: G.adj[i]) {
                        adj[i].insert(x);
                    }
                }
            }
            n = max(n, G.n);
            return *this;
        }

        Graph& operator-(Graph &G) {
            int v = G.n;
            if (v > n) {
                set<int> s;
                for (int i = 0; i < v - n; i++) {
                    adj.push_back(s);
                }
            }
            n = max(n, v);
            for (int i = 0; i < n; i++) {
                if (i >= v) adj[i].clear();
                else {
                    set<int> s;
                    for (auto x: adj[i]) {
                        if (G.adj[i].count(x) > 0) s.insert(x);
                    }
                    adj[i] = s;
                }
            }

            return *this;
        }

        friend std::istream& operator >> (std::istream& stream, Graph &G);
        friend std::ostream& operator << (std::ostream& stream, Graph &G);
};

std::istream& operator >> (std::istream& stream, Graph &G) {
    int m = G.m;
    int x, y;
    for (int i = 0; i < m; i++) {
        stream >> x >> y;
        G.adj[x].insert(y);
        G.adj[y].insert(x);
    }
    return stream;
}

std::ostream& operator << (std::ostream& stream, Graph &G) {
    int n = G.n;
    for (int i = 0; i < n; i++) {
        stream << "Vertex " << i << ": ";
        for (auto x: G.adj[i]) {
            stream << x << " ";
        }
        stream << endl;
    }
    return stream;
}


int main() {
    string s;
    int n, m;
    cin >> s >> n >> m;
    Graph G1(n, m);
    cin >> G1;
    cin >> s;
    while (s != "end") {
        if (s == "printGraph") {
            cout << G1;
        }

        else if (s == "add_edge") {
            int x, y;
            cin >> x >> y;
            G1.addEdge(x, y);
        }

        else if (s == "remove_edge") {
            int x, y;
            cin >> x >> y;
            G1.removeEdge(x, y);
        }

        else if (s == "isReachable") {
            int x, y;
            cin >> x >> y;
            if (G1.isReachable(x, y)) cout << "Yes" << endl;
            else cout << "No" << endl;
        }

        else if (s == "complement") {
            !G1;
        }

        else if (s == "union") {
            cin >> s >> n >> m;
            Graph G2(n, m);
            cin >> G2;
            G1 + G2;
        }

        else if (s == "intersection") {
            cin >> s >> n >> m;
            Graph G2(n, m);
            cin >> G2;
            G1 - G2;
        }

        cin >> s;
    }
}
