#include <iostream>
#include <vector>
#include <stack>
using namespace std;

class Card {
    private:
        int str; //this is the strength of the card i.e., its area
    public:

        int getS() {
            return str;
        }
        void setS(int area) {
            str = area;
        }

    //overloading comparators
        bool operator>(Card* c) {
            return str > c->getS();
        }
        bool operator<(Card* c) {
            return str < c->getS();
        }

    //overloading streams
    virtual ostream& operator<< (ostream& stream) = 0; //the = 0 part makes it an abstract class
    virtual istream& operator>> (istream& stream) = 0;
};

//constructors of derived objects set their strengths
//istream and ostream are separately overloaded for derived classes
class Rectangle: public Card {
    private:
        int l, b;
    public:
        Rectangle(int len, int bre): l(len), b(bre) {
            setS(l*b);
        }
        void nothing() {return;}
        ostream& operator<< (ostream& stream) {
            return (stream << "RECTANGLE " << l << " " << b);
        }
        istream& operator>> (istream& stream) {
            return (stream >> l >> b);
        }
};

class Square: public Card {
    private:
        int s;
    public:
        Square(int sid): s(sid) {
            setS(s*s);
        }
        ostream& operator<< (ostream& stream) {
            return (stream << "SQUARE " << s);
        }
        istream& operator>> (istream& stream) {
            return (stream >> s);
        }
};

class Triangle: public Card {
    private:
        int b, h;
    public:
        Triangle(int bas, int hei): b(bas), h(hei) {
            setS((b*h)/2);
        }
        ostream& operator<< (ostream& stream) {
            return (stream << "TRIANGLE " << b << " " << h);
        }
        istream& operator>> (istream& stream) {
            return (stream >> b >> h);
        }
};

class CardOrganizer {
    private:
        vector<Card*> cards; //these are the cards that we have
        vector<Card*> piles;
        vector<int> pre, tails;
    public:
        void addCard(Card* c); //Finds a suitable pile and inserts the card in it.
        size_t getPileCount(); //Returns the number of piles created so far.
        vector<Card*> getLIS(); //Returns a valid LIS based on pile tops.
        int lwr_bound(Card* c, vector<Card*> v); //this finds this first card whose strengh is greater than or equal to the strength of c in the vector
};      

void CardOrganizer::addCard(Card* c) {
    cards.push_back(c);
    pre.push_back(-1);
    int i = cards.size() - 1;
    int x = lwr_bound(c, piles);
    if (x == piles.size()) {
        piles.push_back(cards[i]);
        tails.push_back(i);
    }
    else {
        piles[x] = cards[i];
        tails[x] = i;
    }

    if (x > 0) pre[i] = tails[x - 1];
}
size_t CardOrganizer::getPileCount() {
    return piles.size();
}
vector<Card*> CardOrganizer::getLIS() {
    int n = cards.size();
    stack<Card*> stk;
    vector<Card*> fin;
    int tail = tails[tails.size() - 1];
    while (tail != -1) {
        stk.push(cards[tail]);
        tail = pre[tail];
    }
    while (!stk.empty()) {
        fin.push_back(stk.top());
        stk.pop();
    }
    return fin;
}

int CardOrganizer::lwr_bound(Card* c, vector<Card*> v) {
    int n = v.size();
    int l = 0, r = n;
    while (l < r) {
        int m = l + (r - l)/2;
        if (*v[m] < c) l = m + 1;
        else r = m ;
    }
    return l;
}

int main() {
    int q;
    cin >> q;

    CardOrganizer card_organizer;

    for (int i = 0; i < q; i++) {
        int query;
        cin >> query;

        if (query == 1) {
            string s;
            cin >> s;

            if (s == "SQUARE") {
                int side;
                cin >> side;

                Square* temp = new Square(side);
                card_organizer.addCard(temp);
            }

            else if (s == "RECTANGLE") {
                int l, b;
                cin >> l >> b;

                Rectangle* temp = new Rectangle(l, b);
                card_organizer.addCard(temp);
            }

            else if (s == "TRIANGLE") {
                int b, h;
                cin >> b >> h;

                Triangle* temp = new Triangle(b, h);
                card_organizer.addCard(temp);
            }
        }

        else if (query == 2) {
            cout << card_organizer.getPileCount() << "\n";
        }

        else if (query == 3) {
            vector<Card*> v = CO.getLIS();
            cout << v.size() << endl;
            for (auto x: v) {
                *x << cout << '\n';
            }
        }
    }
}
