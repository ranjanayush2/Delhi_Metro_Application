
// Written by : Ayush Ranjan

#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;
typedef long long ll;
map<string, ll> m; //each station is given a numerical value.
vector<pair<ll, float>> v[10001]; //store the weighted graph.
map<ll, string> station; 
map<ll,string> clr; //to store colour corrosponding to each stations.
set<ll> intersec; //stores indices of station having intersections.
vector<string> v1 = {"red.txt", "blue1.txt", "blue2.txt", "green1.txt", "green2.txt", "airport_express.txt", "aqualine.txt", "gray.txt", "magenta.txt", "pink.txt", "rapid.txt", "violet.txt", "yellow.txt"};
class comparedis
{
public:
	bool operator()(pair<ll, ll> &p, pair<ll, ll> &q)
	{
		return (p.second > q.second); // For min heap use > sign
	}
};
struct Pair
{
	std::string key;
	float value;
};
void consmap() // To assign values to each metro stations.  [ ok,tested ]
{
	ll cnt = 0;
	for (ll i = 0; i < v1.size(); i++)
	{   string trc="";
	    for(ll j=0;j<v1[i].size();j++)
		{
         if(v1[i][j]=='.'){break;}
		 trc.push_back(v1[i][j]);
		}
		std::ifstream file(v1[i]); // Open the file
		if (file.is_open())
		{
			std::vector<Pair> pairs;
			std::string line;

			while (std::getline(file, line))
			{  
				// Read lines until the end of file
				Pair pair;
				size_t lastSpace = line.find_last_of(' ');
				if (lastSpace != std::string::npos)
				{
					pair.value = std::stof(line.substr(lastSpace + 1)); // Extract float part
					pair.key = line.substr(0, lastSpace);				// Extract string part
					if (m.find(pair.key) == m.end())
					{
						m[pair.key] = cnt;
						station[cnt] = pair.key;
						clr[cnt]=trc; //to assign colour to each stations.
						cnt++;
						//clr[cnt]=trc; 
					}
				}
			}

			file.close(); // Close the file when done
		}
	}
} // [ok ,tested ]
 
void addedge()  // to add edge in the graph.
{
	for (ll i = 0; i < v1.size(); i++)
	{   ll cn = 0;
		std::ifstream file(v1[i]); // Open the file
	    if (file.is_open())
		{
			std::vector<Pair> pairs;
			std::string line; 
			string prev_string ="";
			float prev_value = 0.0;
			while (std::getline(file, line))
			{ // Read lines until the end of file
				Pair pair;
				size_t lastSpace = line.find_last_of(' ');
				if (lastSpace != std::string::npos)
				{   
					pair.value = std::stof(line.substr(lastSpace + 1)); // Extract float part.
					pair.key = line.substr(0, lastSpace);				// Extract string part.
                    if (cn>=1)
					{
						v[m[prev_string]].push_back({m[pair.key], pair.value - prev_value});
						v[m[pair.key]].push_back({m[prev_string], pair.value - prev_value});
						
					}
						prev_string = pair.key;
						prev_value = pair.value;
						cn++;
			
				}

			}

			file.close(); // Close the file when done.
			
		}
	}

	
}  

// [ok,tested]

void disp(ll src,ll dest,ll par[])
{   vector<ll> v3;
	ll i,x,y,cn=0,ci=0;
	stack<ll> st;
	st.push(dest);
	i=dest;
	while(par[i]!=-1)
	{
		i=par[i];
		st.push(i);				
	}
	//char col='\0';
	cout<<"Below is the detailed route"<<endl;
	while(!st.empty())
	{
		x=st.top();
		st.pop();
		cout<<station[x]<<"->";
		v3.push_back(x);	
	}
	cout<<endl;
    ll interchange=0;
	for(ll i=1;i<v3.size();i++)
	{
     if(intersec.find(v3[i])!=intersec.end())
	 {  string c1=clr[v3[i-1]];
		string c2=clr[v3[i+1]];
		if(i+1<v3.size() && c1!=c2)
		{
		 interchange++;
		}
	 }
	}
     
	 cout<<"number of intersections is ";
	 cout<<interchange<<endl;


	for(ll i=1;i<v3.size();i++)
	{
     if(intersec.find(v3[i])!=intersec.end())
	 {
		if(i+1<v3.size())
		{
		 string c1=clr[v3[i-1]];
		 string c2=clr[v3[i+1]];
		 if(c1!=c2)
		 {
		 cout<<"change from ";
		 cout<<c1<<" "<<"line to "<<c2<<" line at "<<station[v3[i]]<<endl;
		 }
	
		}
	 }
	}

	
}

void dijkstra(ll src,ll dest)
{   
   
   long double dist[10001];
	ll vis[10001];
	ll par[10001];
	for(ll i=0;i<10001;i++)
	{   vis[i]=0;
		dist[i]=1e17;
		par[i]=-1;
	}
	 
	set<pair<long double,ll>> st;

	st.insert({0.0,src});
	dist[src]=0.0;
 
	while(st.size()>0)
	{
		auto node=*st.begin();
		ll v1=node.second;
		long double v_dist=node.first;
		st.erase(st.begin());
		if(vis[v1])continue;
		vis[v1]=1;
		for(auto child : v[v1])
		{
		  ll child_v=child.first;
		  long double wt=child.second;
		  if(dist[v1]+wt<dist[child_v])
		  {
            dist[child_v]=dist[v1]+wt;
			st.insert({dist[child_v],child_v});
			par[child_v]=v1;
		  }
		}

	}

   disp(src,dest,par);
	
}

void bfs(ll src,ll dest)
{ 
 ll vis[10001];
 ll par[10001];
// ll level[10001];
 for(ll i=0;i<10001;i++){par[i]=-1;vis[i]=0;}
 queue<ll> q;
 q.push(src);
 vis[src]=1;
 while(!q.empty())
 {
	ll cur_v=q.front();
	q.pop();
	for(auto child : v[cur_v])
	{
      ll chl=child.first;
	  if(!vis[chl])
	  {
		q.push(chl);
		vis[chl]=1;
        par[chl]=cur_v;
	  }
	}
 }

 disp(src,dest,par);
 
}
int main()
{   
	string source,destination;
	ll i,x,y,w,src,dest,k,choice,dec;
	char ch='y';
	consmap();
	addedge();
	
	//adding all stations which has intersections from line no 254 to 278.
   
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
	bool is_val=true;
	while(ch=='y' || ch=='Y')
	{
	char input;
	cout<<"*Welcome to Delhi metro*"<<endl;;
	cout<<"please enter source station : ";
	getline(cin,source);
	cout<<"please enter Destination station : ";
	getline(cin,destination);
	//managing incorrect inputs.
	if(m.find(source)==m.end() || m.find(destination)==m.end())
	{
		cout<<"Invalid input"<<endl;
		cout<<"The stations you have entered does not exist,please enter valid inputs."<<endl;
		is_val=false;
	}
	if(!is_val){continue;}
	cout<<"How can I help you ?"<<endl;
	cout<<"Press 1 to get the shortest and cheapest route between two stations\n";
	cout<<"Press 2 to get the route having minimum number of stations in between\n";
    cin>>choice;
	src=m[source];
	dest=m[destination];
	if(choice==1){dijkstra(src,dest);}
	else{bfs(src,dest);}
	cout<<"Do you wish to go back to main menu\n";
	cin>>input;
	ch=input;
	}	
	cout<<"Thanks for using Delhi Metro.Have a good day";
}