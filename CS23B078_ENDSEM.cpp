#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
using namespace std;

class Events {
    private:
        int startTime, endTime;
        double profit;
    public:
        //setter attributes
        void setStart(int s);
        void setEnd(int e);
        void setProfit(double d);
        //getter attributes
        int getStart();
        int getEnd();
        double getProfit();
        virtual void calculateProfit() = 0; //for calculating profit
        virtual istream& input(istream& stream) = 0; //helps overloading extraction
};

class comp { 
    public:
        bool operator()(Events* a, Events* b) { //custom comparator functor
            return a->getEnd() < b->getEnd();
        }
};

istream& operator>>(istream& stream, Events* E) { //overloading extraction operator
    return E->input(stream);
}

class EventScheduler {
    private:
        vector<Events*> events;
    public:
        void add(Events*& e);
        double maxProfit(); //returns the maximum profit
        double solve(int i, vector<int>& ends, vector<double>& dp); //helps in solving for maxProfit
};

//classes derived from events
class Concerts: public Events {
    private:
        int ticket_price, tickets_sold, fee, log_cost;
    public:
        void calculateProfit();
        istream& input(istream& stream);
};

class TheaterShow: public Events {
    private:
        int base_price, seats, ven_cost;
    public:
        void calculateProfit();
        istream& input(istream& stream);
};

class Wedding: public Events {
    private:
        int base_price, dec_cost, guest, ven_cost;
    public:
        void calculateProfit();
        istream& input(istream& stream);
};

//function definitios of Events class (getters and setters)
void Events::setProfit(double d) {
    profit = d;
}

double Events::getProfit() {
    return profit;
}

void Events::setStart(int s) {
    startTime = s;
}

int Events::getStart() {
    return startTime;
}

void Events::setEnd(int s) {
    endTime = s;
}

int Events::getEnd() {
    return endTime;
}

//overloading virtual function calculateProfit and input for derived classes
void Concerts::calculateProfit() {
    int cost = fee + log_cost;
    double revenue = (0.82)*(tickets_sold*ticket_price);
    double profit = revenue - cost;
    profit = (profit > 2*cost)? (0.7)*profit: profit;
    setProfit(profit);
}

istream& Concerts::input(istream& stream) {
    int s, e;
    stream >> s >> e >> ticket_price >> tickets_sold >> fee >> log_cost;
    setStart(s);
    setEnd(e);
    calculateProfit();
    return stream;
}

void TheaterShow::calculateProfit() {
    double tic_rev = (0.82)*(1.25)*(seats*base_price);
    double pop_rev = (0.25)*(seats)*(150);
    double profit = tic_rev + pop_rev - ven_cost;
    setProfit(profit);
}

istream& TheaterShow::input(istream& stream) {
    int s, e;
    stream >> s >> e >> base_price >> seats >> ven_cost;
    setStart(s);
    setEnd(e);
    calculateProfit();
    return stream;
}

void Wedding::calculateProfit() {
    double tot_ven_cost = (guest > 200)? (3*ven_cost):ven_cost;
    double cat_cost = (guest > 100)? (70*guest):100*guest;
    double profit = base_price - tot_ven_cost - dec_cost - cat_cost;
    setProfit(profit);
}

istream& Wedding::input(istream& stream) {
    int s, e;
    stream >> s >> e >> base_price >> dec_cost >> guest >> ven_cost;
    setStart(s);
    setEnd(e);
    calculateProfit();
    return stream;
}

//helps append new event pointer to the events schedule
void EventScheduler::add(Events*& e) {
    events.push_back(e);
}

//calculating maxProfit
double EventScheduler::maxProfit() {
    int n = events.size();
    comp c;
    sort(events.begin(), events.end(), c);
    vector<int> ends(n);
    for (int i = 0; i < n; i++) ends[i] = events[i]->getEnd();
    vector<double> dp(n, -1);
    return solve(n - 1, ends, dp);
}

double EventScheduler::solve(int i, vector<int>& ends, vector<double>& dp) {
    //base cases
    if (i == 0) {
        dp[0] = events[i]->getProfit();
        return dp[0];
    }
    if (i < 0) return 0;
    //using memoization
    if (dp[i] != -1) return dp[i];
    //finding previous compatible interval
    int x = upper_bound(ends.begin(), ends.begin() + i, events[i]->getStart()) - ends.begin() - 1;
    dp[i] = events[i]->getProfit() + solve(x, ends, dp);
    double y = solve(i - 1, ends, dp);
    dp[i] = max(dp[i], y); //recurrence relation
    return dp[i]; 
}

int main() {
    int n;
    cin >> n;
    EventScheduler scheduler;
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        if (x == 1) {
            Events* temp = new Concerts;
            cin >> temp;
            scheduler.add(temp);
        }

        else if (x == 2) {
            Events* temp = new TheaterShow;
            cin >> temp;
            scheduler.add(temp);
        }

        else if (x == 3) {
            Events* temp = new Wedding;
            cin >> temp;
            scheduler.add(temp);
        }
    }
    
    cout << fixed << setprecision(2) << scheduler.maxProfit() << endl;
}