#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
using namespace std;

//we'll be using 1-indexed vector in the custom priority queue for convenience
class Node {
    private:
        string name;
        long long int dist = 1e18; //this is the shortest distance
        long long int od_dist = 1e18; //this is the shortest odd distance
        long long int ev_dist = 1e18; //this is the shortest even distance
    public:
        Node(string s = "Start=>"): name(s) {}
        string getN() {
            return name;
        }
        long long int getD() {
            return dist;
        }
        void setD(long long int d) {
            dist = d;
        }
        long long int getO() {
            return od_dist;
        }
        void setO(long long int d) {
            od_dist = d;
        } 
        long long int getE() {
            return ev_dist;
        }
        void setE(long long int d) {
            ev_dist = d;
        }  
};

std::ostream& operator<< (std::ostream& stream, Node& C) { //overloading << for printing complex numbers
    return stream << C.getN() << " " << C.getD() << " ";
}

template <typename T>
class Comp {
    public:
    bool operator()(T a, T b) {
        return a > b;
    }
};

template <>
class Comp<Node> {
    public:
        bool operator()(Node a, Node b) {
            return a.getD() > b.getD();
        }
};

template <typename T>
class min_heap {
    private:
        int curr_size;

    public:
        vector<T> pq;
        min_heap() {
            pq.resize(1);
            curr_size = 0;
        }
        void push(const T& t); //inserts new element
        void pop(); //removes the top element
        const size_t size() { //gives the current number of elements in the priority queue
            return (size_t)curr_size;
        }
        T top() { //retrives top element of the priority queue without removing it
            return pq[1];
        }
        const bool empty() const { //checks if whether the piority queue is empty
            return curr_size == 0;
        }   
};

template <typename T>
void min_heap<T>::push(const T& t) {
    pq.push_back(t);
    curr_size++;
    //cout << "Pushed into pq: " << pq[curr_size] << endl;
    int i = curr_size;
    Comp<Node> c;
    while (i > 1 && c(pq[i/2], pq[i])) {
        swap(pq[i/2], pq[i]);
        i = i/2;
    }
}

template <typename T>
void min_heap<T>::pop() {
    if (curr_size == 0) {
        cout << "No elements to be popped" << endl;
        return;
    }
    
    pq[1] = pq[curr_size];
    pq.pop_back();
    curr_size--;

    int i = 1;
    Comp<Node> c;
    while (2*i + 1 <= curr_size || 2*i <= curr_size) {
        if (2*i + 1 > curr_size) {
            if (c(pq[i], pq[2*i])) {
                swap(pq[i], pq[2*i]);
                break;
            }
        }
        if (c(pq[i], pq[2*i]) && c(pq[2*i + 1], pq[i])) { //pq[2i + 1] > pq[i] > pq[2i]
            swap(pq[i], pq[2*i]);
            i = 2*i;    
        }

        else if (c(pq[2*i], pq[i]) && c(pq[i], pq[2*i + 1])) { //pq[2i] > pq[i] > pq[2i + 1]
            swap(pq[i], pq[2*i + 1]);
            i = 2*i + 1;  
        }

        else if (c(pq[i], pq[2*i]) && c(pq[i], pq[2*i + 1])) {
            if (c(pq[2*i], pq[2*i + 1])) { //pq[i] > pq[2i] > pq[2i + 1]
                swap(pq[i], pq[2*i + 1]);
                i *= 2;
                i++; 
            }
            else {  //pq[i] > pq[2i + 1] > pq[2i]
                swap(pq[i], pq[2*i]);
                i *= 2;
            }
        }

        else break;
    }
}

class Marauder { //this is not templatized
    public:
        map<string, Node*> roomName;
        unordered_map<string, vector<string>> adj;
        map<pair<string, string>, int> edges;
        int dijkstra(string source, string target);
};

int Marauder::dijkstra(string s, string t) {
    if (s == t) return 0;
    //code
    min_heap<Node> pq;
    roomName[s]->setD(0);
    roomName[s]->setE(0);
    pq.push(*roomName[s]);
    while (!pq.empty()) {
        Node y = pq.top();
        pq.pop();
        for (auto x: adj[y.getN()]) { //x is a string
            long long int sumE = y.getO() + edges[{x, y.getN()}]; //this gives a possible path of even length
            long long int sumO = y.getE() + edges[{x, y.getN()}]; //this gives a possible path of odd length
            if (sumE < roomName[x]->getE() || sumO < roomName[x]->getO()) {
                long long int sum = min(sumE, sumO);
                if (sum < roomName[x]->getD()) {
                    roomName[x]->setD(sum);
                }
                if (sumE < roomName[x]->getE()) {
                    roomName[x]->setE(sumE);
                }
                if (sumO < roomName[x]->getO()) {
                    roomName[x]->setO(sumO);
                }
                Node temp = *roomName[x];
                pq.push(temp);
            }
        }
    }
    
    long long int x = roomName[t]->getE();
    if (x != 1e18) return x;
    else return -1;
}

int main() {
    int n, m;
    cin >> n >> m;
    Marauder M;
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;
        Node* temp = new Node(s);
        M.roomName[s] = temp;
    }

    for (int i = 0; i < m; i++) {
        int x;
        string s1, s2;
        cin >> s1 >> s2 >> x;
        M.edges[{s1, s2}] = x;
        M.edges[{s2, s1}] = x;
        M.adj[s1].push_back(s2);
        M.adj[s2].push_back(s1);
    }

    string source, target;
    cin >> source >> target;
    cout << M.dijkstra(source, target) << endl;
}
