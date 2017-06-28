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
	ifstream ifs("input_0.csv");

	if (ifs.fail()) {
		cout << "fail" << endl;
	}
	else{
		cout<<"success"<<endl;
		string first;
		ifs>>first;

		string str;
		double d;

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

void outputresult(vector<city> cycle)
{
	ofstream ofs("solution_yours_0.csv");

	ofs << "index" << endl;
	for (int i = 0; i != cycle.size(); i++) {
		ofs << cycle[i].citynumber << endl;
	}

	ofs.close();
}

int binarysearch(vector<city> a, int k)
{
	int ret = -1;

	int low = 0; int high = a.size() - 1;

	while (low <= high) {
		int middle = (low + high) / 2;
		if (k == a[middle].citynumber) {
			ret = middle;
			break;
		}
		else if (k > a[middle].citynumber)low = middle + 1;
		else high = middle - 1;	
	}
	return ret;
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

pair<int,int> min_citygroups(vector<city> cities_a, vector<city> cities_b, vector<vector<double> > distance)
{
	pair<int, int> min_between_groups;

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
	min_between_groups.first = mincity_a;
	min_between_groups.second = mincity_b;

	return min_between_groups;
}



//visited:0, unvisited:それ以外でstart
void nearest_insert(vector<city>& unvisited_cities, vector<city>& visited_cities,vector<city>& cities,vector<vector<double> >& distance)
{
	while (unvisited_cities.size() != 0) {
		pair<int, int> min_visited_unvisited;
		min_visited_unvisited = min_citygroups(visited_cities, unvisited_cities,distance);

		int newcitynumber=min_visited_unvisited.second;
		int nearest_new_number=min_visited_unvisited.first;

		city newcity = unvisited_cities[min_visited_unvisited.second];
		city nearest_new = visited_cities[min_visited_unvisited.first];



		visited_cities.insert(visited_cities.begin() + nearest_new_number + 1, newcity);
		unvisited_cities.erase(unvisited_cities.begin() + newcitynumber);

		nearest_insert(unvisited_cities, visited_cities, cities,distance);
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

	outputresult(visited_cities);

	double totaldistance=0.0;

	for (int i = 1; i != visited_cities.size(); i++) {
		totaldistance+=distance[visited_cities[i].citynumber][visited_cities[i - 1].citynumber];
	}

	totaldistance+=distance[visited_cities[visited_cities.size()-1].citynumber][0];
	cout << totaldistance << endl;
	
	return 0;
}