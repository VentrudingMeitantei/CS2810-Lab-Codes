#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
using namespace std;
//using Kahn's algorithm for lexicographical toposort, reference: geeksforgeeks
class Comparator {
    public:
        bool operator()(int a, int b) {
            return a > b;
        }
};

class GraphAlgorithm {
    private:
        bool cyclic;
    public:
        vector<pair<int, int>> time; //these are complete times of different vertices
        vector<vector<int>> adjList, tradj; //adjList: for main graph, tradJ: for transpose graph
        vector<set<int>> adjSCC; //adjSCC: adjacency matrix for SCC graph
        vector<vector<int>> SCC; //SCCs
        vector<int> hype;
        void makeT(vector<vector<int>>& adjList);
        void sccDFS(); //function for making SCCs
        bool transcendentDFS(bool k);
        int count = 0;
        map<int, int> v_scc; //maps each index to the scc it belongs to
        bool dfs(int i, vector<vector<int>>& adjList, vector<int>&visite, int& t, bool test, vector<int>& v, bool k);
        bool getCy() {
            return cyclic;
        }
        void setCy(bool val) {
            cyclic = val;
        }
        virtual void Query() = 0;

};

class isCycle: public GraphAlgorithm {
    public:
        isCycle(vector<vector<int>> adj, vector<int> hyp) {
            adjList = adj;
            hype = hyp;
            makeT(adjList);
        }
        void Query(); //query called from here detects cycles
};

class indepComponent: public GraphAlgorithm {
    public:
        indepComponent(vector<vector<int>> adj, vector<int> hyp) {
            adjList = adj;
            hype = hyp;
            makeT(adjList);
        }
        void Query(); //query called from here counts SCCS
};

class validOrder: public GraphAlgorithm {
    public:
        validOrder(vector<vector<int>> adj, vector<int> hyp) {
            adjList = adj;
            hype = hyp;
            makeT(adjList);
        }
        void Query(); //query called from here gives toposort
};

class maxhype: public GraphAlgorithm {
    public:
        maxhype(vector<vector<int>> adj, vector<int> hyp) {
            adjList = adj;
            hype = hyp;
            makeT(adjList);
        }
        void Query(); //query called from here Max hype score
};

//-1 for unprocessed, 0 for processing, 1 for processed
bool GraphAlgorithm::transcendentDFS(bool k) {
    int n = hype.size();
    vector<int> visited(n, -1);
    bool check = false;
    int t = 0;
    vector<int> v;
    for (int i = 0; i < n; i++) {
        if (visited[i] == -1) check |= dfs(i, adjList, visited, t, true, v, k);
        if (check) {
            setCy(check);
            return true;
        }
    }

    setCy(check);
    return false;
}

void GraphAlgorithm::sccDFS() {
    int n = hype.size();
    vector<int> visited(n, -1);
    sort(time.begin(), time.end(), [](pair<int, int> a, pair<int, int> b) {return a.first > b.first;});
    vector<int> v;
    int t = 0;
    for (int i = 0; i < n; i++) {
        if (visited[time[i].second] == -1) {
            dfs(time[i].second, tradj, visited, t, false, v, false);
            SCC.push_back(v);
            count++;
            v.clear();
        }
    }
}
bool GraphAlgorithm::dfs(int i, vector<vector<int>>& adjList, vector<int>&visited, int& t, bool test, vector<int>& v, bool k) {
    if (visited[i] == 0) return true;
    else if (visited[i] == 1) return false;
    visited[i] = 0;
    v_scc[i] = count;
    v.push_back(i);
    bool check = false;
    t++;
    for (auto x: adjList[i]) {
        check |= dfs(x, adjList, visited, t, test, v, k);
        if (k && check) return true;
    }
    visited[i] = 1;
    t++;
    if (test) time.push_back({t, i});
    return false;
}

void GraphAlgorithm::makeT(vector<vector<int>>& adjList) {
    int n = adjList.size();
    tradj.resize(n);
    for (int i = 0; i < n; i++) {
        for (auto x: adjList[i]) {
            tradj[x].push_back(i);
        }
    }
}

void isCycle::Query() {
    transcendentDFS(true);
    if (getCy()) cout << "YES" << endl;
    else cout << "NO" << endl;
}

void validOrder::Query() {
    vector<int> toposort;
    int count = 0, n = hype.size();
    vector<int> indeg(n);
    for (int i = 0; i < n; i++) {
        for (auto x: adjList[i]) {
            indeg[x]++; //counts indegrees
        }
    }

    priority_queue<int, vector<int>, greater<int>> p;

    for (int i = 0; i < n; i++) {
        if (indeg[i] == 0) p.push(i); //push in all sinks and have the one with the least value at the top
    }

    //this basically keeps removing sources sequentially
    while (!p.empty()) {
        count++;
        int x = p.top();
        toposort.push_back(x);
        p.pop();
        for (auto y: adjList[x]) {
            indeg[y]--;
            if (indeg[y] == 0) {
                p.push(y);
            }
        }
    }

    if (count != n) {
        setCy(true);
        cout << "NO" << endl;
        return;
    }
    setCy(false);
    for (auto x: toposort) cout << x + 1 << " ";
    cout << endl;
}

void indepComponent::Query() {
    transcendentDFS(false);
    sccDFS();
    int M = 0;
    for (auto x: SCC) {
        M = max(M, (int)x.size());
    }
    cout << SCC.size() << " " << M << endl;
}

void maxhype::Query() {
    transcendentDFS(false);
    sccDFS();
    adjSCC.resize(count);
    vector<int> hypeVal(count);
    int n = hype.size();
    for (int i = 0; i < n; i++) {
        hypeVal[v_scc[i]] += hype[i];
        for (int x: adjList[i]) {
            if (v_scc[x] != v_scc[i]) {
                adjSCC[v_scc[i]].insert(v_scc[x]);
            }
        }
    }

    vector<int> toposort;
    vector<int> indeg(n);
    for (int i = 0; i < count; i++) {
        for (auto x: adjSCC[i]) {
            indeg[x]++;
        }
    }

    priority_queue<int, vector<int>, greater<int>> p;

    for (int i = 0; i < n; i++) {
        if (indeg[i] == 0) p.push(i);
    }

    while (!p.empty()) {
        int x = p.top();
        toposort.push_back(x);
        p.pop();
        for (auto y: adjList[x]) {
            indeg[y]--;
            if (indeg[y] == 0) {
                p.push(y);
            }
        }
    }

    vector<int> dp(count, 0);
    int finalVal = 0;
    for (int i = count - 1; i >= 0; i--) {
        int M = 0;
        for (auto x: adjSCC[i]) {
            M = max(M, dp[x]);
        }
        dp[i] += hypeVal[i] + M;
        finalVal = max(dp[i], finalVal);
    }

    cout << finalVal << endl;
}

int main() {
    int N, M;
    cin >> N >> M;

    vector<vector<int>> adj(N);
    vector<int> hype(N);
    
    for (int i = 0; i < N; i++) {
        cin >> hype[i];
    }

    for (int i = 0; i < M; i++) {
        int u, v;
        cin >> u >> v;
        adj[u - 1].push_back(v - 1);
    }

    Comparator c;
    for (int i = 0; i < N; i++) {
        sort(adj[i].begin(), adj[i].end(), c);
    }

    GraphAlgorithm* isC = new isCycle(adj, hype);
    GraphAlgorithm* val = new validOrder(adj, hype);
    GraphAlgorithm* scc = new indepComponent(adj, hype);
    GraphAlgorithm* mhy = new maxhype(adj, hype);

    int q;
    cin >> q;
    
    for (int i = 0; i < q; i++) {
        int x;
        cin >> x;
        if (x == 1) isC->Query();
        else if (x == 3) val->Query();
        else if (x == 2) scc->Query();
        else {
            mhy->Query();
        }
    }
}
