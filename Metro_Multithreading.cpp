// Written by : Ayush Ranjan
#include <bits/stdc++.h>
#include <thread>
#include <mutex>
#include <sstream>
using namespace std;

typedef long long ll;
map<string, ll> m; 
vector<pair<ll, float>> v[10001];
map<ll, string> station; 
map<ll, string> clr; 
set<ll> intersec; 
vector<string> v1 = {"red.txt", "blue1.txt", "blue2.txt", "green1.txt", "green2.txt", "airport_express.txt", "aqualine.txt", "gray.txt", "magenta.txt", "pink.txt", "rapid.txt", "violet.txt", "yellow.txt"};
std::mutex print_mtx; // For thread-safe printing

struct Pair {
    std::string key;
    float value;
};

void consmap() {
    ll cnt = 0;
    for (ll i = 0; i < v1.size(); i++) {
        string trc = "";
        for (ll j = 0; j < v1[i].size(); j++) {
            if (v1[i][j] == '.') break;
            trc.push_back(v1[i][j]);
        }
        std::ifstream file(v1[i]);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                Pair pair;
                size_t lastSpace = line.find_last_of(' ');
                if (lastSpace != std::string::npos) {
                    pair.value = std::stof(line.substr(lastSpace + 1));
                    pair.key = line.substr(0, lastSpace);
                    if (m.find(pair.key) == m.end()) {
                        m[pair.key] = cnt;
                        station[cnt] = pair.key;
                        clr[cnt] = trc;
                        cnt++;
                    }
                }
            }
            file.close();
        }
    }
}

void addedge() {
    for (ll i = 0; i < v1.size(); i++) {
        ll cn = 0;
        std::ifstream file(v1[i]);
        if (file.is_open()) {
            std::string line; 
            string prev_string = "";
            float prev_value = 0.0;
            while (std::getline(file, line)) {
                Pair pair;
                size_t lastSpace = line.find_last_of(' ');
                if (lastSpace != std::string::npos) {   
                    pair.value = std::stof(line.substr(lastSpace + 1));
                    pair.key = line.substr(0, lastSpace);
                    if (cn >= 1) {
                        v[m[prev_string]].push_back({m[pair.key], pair.value - prev_value});
                        v[m[pair.key]].push_back({m[prev_string], pair.value - prev_value});
                    }
                    prev_string = pair.key;
                    prev_value = pair.value;
                    cn++;
                }
            }
            file.close();
        }
    }
}

void disp(ll src, ll dest, ll par[], std::ostringstream &out) {
    vector<ll> v3;
    stack<ll> st;
    st.push(dest);
    ll i = dest;
    while (par[i] != -1) {
        i = par[i];
        st.push(i);
    }
    out << "Below is the detailed route\n";
    while (!st.empty()) {
        out << station[st.top()] << "->";
        v3.push_back(st.top());
        st.pop();
    }
    out << "\n";

    ll interchange = 0;
    for (ll i = 1; i < v3.size(); i++) {
        if (intersec.find(v3[i]) != intersec.end()) {
            string c1 = clr[v3[i - 1]];
            string c2 = clr[v3[i + 1]];
            if (i + 1 < v3.size() && c1 != c2) interchange++;
        }
    }
    out << "Number of intersections is " << interchange << "\n";

    for (ll i = 1; i < v3.size(); i++) {
        if (intersec.find(v3[i]) != intersec.end()) {
            if (i + 1 < v3.size()) {
                string c1 = clr[v3[i - 1]];
                string c2 = clr[v3[i + 1]];
                if (c1 != c2) {
                    out << "Change from " << c1 << " line to " << c2 << " line at " << station[v3[i]] << "\n";
                }
            }
        }
    }
}

void dijkstra(ll src, ll dest, std::ostringstream &out) {
    long double dist[10001];
    ll vis[10001];
    ll par[10001];
    for (ll i = 0; i < 10001; i++) {
        vis[i] = 0;
        dist[i] = 1e17;
        par[i] = -1;
    }
    set<pair<long double, ll>> st;
    st.insert({0.0, src});
    dist[src] = 0.0;

    while (!st.empty()) {
        auto node = *st.begin();
        ll v1 = node.second;
        st.erase(st.begin());
        if (vis[v1]) continue;
        vis[v1] = 1;
        for (auto child : v[v1]) {
            ll child_v = child.first;
            long double wt = child.second;
            if (dist[v1] + wt < dist[child_v]) {
                dist[child_v] = dist[v1] + wt;
                st.insert({dist[child_v], child_v});
                par[child_v] = v1;
            }
        }
    }
    disp(src, dest, par, out);
}

void bfs(ll src, ll dest, std::ostringstream &out) {
    ll vis[10001], par[10001];
    for (ll i = 0; i < 10001; i++) { par[i] = -1; vis[i] = 0; }
    queue<ll> q;
    q.push(src);
    vis[src] = 1;
    while (!q.empty()) {
        ll cur_v = q.front();
        q.pop();
        for (auto child : v[cur_v]) {
            ll chl = child.first;
            if (!vis[chl]) {
                q.push(chl);
                vis[chl] = 1;
                par[chl] = cur_v;
            }
        }
    }
    disp(src, dest, par, out);
}

void run_dijkstra_thread(ll src, ll dest) {
    std::ostringstream buffer;
    dijkstra(src, dest, buffer);
    std::lock_guard<std::mutex> lock(print_mtx);
    std::cout << buffer.str();
}

void run_bfs_thread(ll src, ll dest) {
    std::ostringstream buffer;
    bfs(src, dest, buffer);
    std::lock_guard<std::mutex> lock(print_mtx);
    std::cout << buffer.str();
}

int main() {
    consmap();
    addedge();

    intersec.insert(m["Kashmiri Gate"]);
    intersec.insert(m["Rajiv Chowk"]);
    intersec.insert(m["Inderlok"]);
    intersec.insert(m["Central Secretariat"]);
    intersec.insert(m["Dwarka Sector 21"]);
    intersec.insert(m["Kirti Nagar"]);
    intersec.insert(m["Yamuna Bank"]);
    intersec.insert(m["Ashok Park Main"]);
    intersec.insert(m["Mandi House"]);
    intersec.insert(m["Azadpur"]);
    intersec.insert(m["Netaji Subhash Place"]);
    intersec.insert(m["Rajouri Garden"]);
    intersec.insert(m["Lajpat Nagar"]);
    intersec.insert(m["Janak Puri West"]);
    intersec.insert(m["Hauz Khas"]);
    intersec.insert(m["Kalkaji Mandir"]);
    intersec.insert(m["Botanical Garden"]);
    intersec.insert(m["Anand Vihar"]);
    intersec.insert(m["Karkar Duma"]);
    intersec.insert(m["Welcome"]);
    intersec.insert(m["Maujpur"]);
    intersec.insert(m["Mayur Vihar Phase-1"]);
    intersec.insert(m["Dwarka"]);
    intersec.insert(m["Noida Sector 52"]);
    intersec.insert(m["Dhaula Kuan"]);

    char ch = 'y';
    while (ch == 'y' || ch == 'Y') {
        string source, destination;
        cout << "*Welcome to Delhi Metro*\n";
        cin.ignore();
        cout << "Please enter source station: ";
        getline(cin, source);
        cout << "Please enter destination station: ";
        getline(cin, destination);

        if (m.find(source) == m.end() || m.find(destination) == m.end()) {
            cout << "Invalid input. Please enter valid stations.\n";
            continue;
        }

        cout << "Press 1 for shortest and cheapest route\n";
        cout << "Press 2 for route with minimum number of stations\n";
        int choice;
        cin >> choice;

        if (choice == 1) {
            std::thread t(run_dijkstra_thread, m[source], m[destination]);
            t.join();
        } else {
            std::thread t(run_bfs_thread, m[source], m[destination]);
            t.join();
        }

        cout << "Do you wish to go back to main menu? (y/n): ";
        cin >> ch;
    }
    cout << "Thanks for using Delhi Metro. Have a good day!\n";
}
