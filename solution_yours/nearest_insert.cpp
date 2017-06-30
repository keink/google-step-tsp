#include<cstdio>
#include<iostream>
#include<sstream>
#include<fstream>
#include<vector>
#include<math.h>
#include<assert.h>
#include<algorithm>
#include<string>

using namespace std;

class city
{
public:
	city()
		:citynumber(0),x(0.0),y(0.0)
	{
	};

	int citynumber; 
	double x;
	double y;

};

void split(vector<string>& str,string& input,char delimeter)
{
	istringstream stream(input);

	string field;

	while(getline(stream,field,delimeter)){
		str.push_back(field);
	}
}

void inputfile(vector<city>& cities)
{
	ifstream ifs("input_6.csv");

	if (ifs.fail()) {
		cout << "fail" << endl;
	}
	else{
		cout<<"success"<<endl;
		string first;
		ifs>>first;

		string str;

		int i=0;
		
		while(ifs>>str){
			vector<string> citydata;
			split(citydata,str,',');
			city a;
			a.x=stod(citydata[0]);
			a.y=stod(citydata[1]);
			a.citynumber=i;
			cities.push_back(a);
			i++;
		}
	}
}

void outputresult(vector<city> tour)
{
	ofstream ofs("solution_yours_6.csv");

	ofs << "index" << endl;
	for (int i = 0; i != tour.size(); i++) {
		ofs << tour[i].citynumber << endl;
	}

	ofs.close();
}

void ij_range_swap(vector<city>& swapcities,int i,int j){
	int k=j-i;
	
	for(int l=0;l<=k/2;l++){
		city a=swapcities[i+l];
		swapcities[i+l]=swapcities[j-l];
		swapcities[j-l]=a;		
	}
}

double two_cities_distance(city a, city b)
{
	double distance;
	distance = sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
	return distance;
}

void all_cities_distance(vector<vector<double> >& distance,vector<city> cities)
{
	distance.resize(cities.size());
	for (int i=0; i != cities.size(); i++) {
		distance[i].resize(cities.size());
		for (int j=0; j != cities.size(); j++) {
			double citydistance = two_cities_distance(cities[i] , cities[j]);
			distance[i][j]=citydistance;
		}
	}
}

pair<int,int> shortest_btw_citygroups(vector<city> cities_a, vector<city> cities_b, vector<vector<double> > distance)
{
	pair<int, int> shortestedge;

	double min = 100000000;
	int mincity_a = 0; int mincity_b = 0;

	for (int i = 0; i != cities_a.size(); i++) {
		for (int j = 0; j != cities_b.size(); j++) {
			double distance_ij = distance[cities_a[i].citynumber][cities_b[j].citynumber];
			if (distance_ij < min) {
				min = distance_ij;
				mincity_a = i;
				mincity_b = j;
			}
		}
	}
	shortestedge.first = mincity_a;
	shortestedge.second = mincity_b;

	return shortestedge;
}



//visited:0, unvisited:それ以外でstart
void nearest_insert(vector<city>& unvisited_cities, vector<city>& visited_cities,vector<city>& cities,vector<vector<double> >& distance)
{
	while (unvisited_cities.size() != 0) {
		pair<int, int> min_visited_unvisited;
		min_visited_unvisited = shortest_btw_citygroups(visited_cities, unvisited_cities,distance);

		int newcitynumber=min_visited_unvisited.second;
		int nearest_new_number=min_visited_unvisited.first;

		city newcity = unvisited_cities[min_visited_unvisited.second];
		city nearest_new = visited_cities[min_visited_unvisited.first];

		visited_cities.insert(visited_cities.begin() + nearest_new_number + 1, newcity);
		unvisited_cities.erase(unvisited_cities.begin() + newcitynumber);

		nearest_insert(unvisited_cities, visited_cities, cities,distance);
	}
}

void local_search(vector<city>& current_tour,vector<vector<double> >& distance){
	for(int i=0;i!=current_tour.size()-3;i++){
		for(int j=i+2;j!=current_tour.size()-1;j++){

			int i_city=current_tour[i].citynumber;
			int i_nextcity=current_tour[i+1].citynumber;
			int j_city=current_tour[j].citynumber;
			int j_nextcity=current_tour[j+1].citynumber;

			double current_edge_distance=distance[i_city][i_nextcity]+distance[j_city][j_nextcity];
			double swapped_edge_distance=distance[i_city][j_city]+distance[i_nextcity][j_nextcity];
			
			if(swapped_edge_distance<current_edge_distance){
				ij_range_swap(current_tour,i+1,j);
			}
		}
	}
}

int main()
{
	vector<city> cities;
	inputfile(cities);

	cout<<"input finished"<<endl;

	vector<vector<double> > distance;
	all_cities_distance(distance,cities);

	cout<<"distance finished"<<endl;


	vector<city> visited_cities;
	vector<city> unvisited_cities = cities;

	visited_cities.push_back(cities[0]);
	unvisited_cities.erase(unvisited_cities.begin());
	nearest_insert(unvisited_cities, visited_cities, cities, distance);

	cout<<"nearest_insert finished"<<endl;


	visited_cities.push_back(cities[0]);
	cout<<"local search start"<<endl;

	//local_search一回だけではまだ交差が存在する可能性があるので繰り返す
	for(int i=0;i<=5;i++){
		local_search(visited_cities,distance);
	}

	double new_totaldistance=0.0;

	cout<<"new tour"<<endl;
	for (int i = 1; i != visited_cities.size(); i++) {
		new_totaldistance+=distance[visited_cities[i].citynumber][visited_cities[i - 1].citynumber];
	}
	cout<<new_totaldistance<<endl;

	visited_cities.pop_back();

	outputresult(visited_cities);
	
	return 0;
}