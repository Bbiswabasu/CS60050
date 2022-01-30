#include <iostream>
#include <vector>
#include <map>
#include <cmath>
using namespace std;

//loads training or test data into vectors
void load_data(vector<vector<int>> &x,vector<int> &y, bool isTraining)
{
	if(isTraining)
		freopen("project1.data","r",stdin);
	else
		freopen("project1_test.data","r",stdin);
	string line;
	while(getline(cin,line))
	{
		line+=",";
		int cnt=0;
		string value="";
		for(int i=0;i<line.size();i++)
		{
			if(line[i]==',')
			{
				if(cnt==6)
				{
					if(value=="unacc")
						y.push_back(0);
					else if(value=="acc")
						y.push_back(1);
					else if(value=="good")
						y.push_back(2);
					else
						y.push_back(3);
				}
				else
				{
					if(value=="vhigh")
						x[cnt].push_back(0);
					else if(value=="high")
						x[cnt].push_back(1);
					else if(value=="med")
						x[cnt].push_back(2);
					else if(value=="low")
						x[cnt].push_back(3);
					else if(value=="small")
						x[cnt].push_back(4);
					else if(value=="big")
						x[cnt].push_back(5);
					else
						x[cnt].push_back(stoi(value));
				}
				cnt++;
				value="";
			}
			else
				value+=line[i];
		}
	}
}

//calculates entropy of given distribution
double calculate_entropy(vector<int> &dist)
{
	map<int,int> count;
	for(auto &it:dist)
		count[it]++;
	double entropy=0,probablity;
	for(auto &it:count)
	{
		probablity=(it.second)*1.0/(long long int)dist.size();
		entropy-=probablity*log2(probablity);
	}
	return entropy;
}

void build_model(vector<vector<int>> &adj,vector<vector<int>> &x,vector<int> &y,vector<int> &values,int &nodeID,vector<int> &attribute)
{
	nodeID++;
	double initialEntropy,maxInfoGain=-1;
	int bestAttribute=-1;
	vector<int> S;
	for(int i=0;i<x[0].size();i++)
	{
		bool ok=1;
		for(int j=0;j<values.size();j++)
		{
			if(values[j]!=-1 && x[j][i]!=values[j])
			{
				ok=0;
				break;
			}
		}
		if(ok)
		S.push_back(y[i]);		
	}
	initialEntropy=calculate_entropy(S);
	cerr<<initialEntropy<<"\n";
}

int main()
{
	vector<vector<int>> x_train(6); //feature vector
	vector<int> y_train; //output vector
	load_data(x_train,y_train,true);
	vector<vector<int>> adj;
	vector<int> values(6,-1);
	int nodeID=-1;
	vector<int> attribute;
	build_model(adj,x_train,y_train,values,nodeID,attribute);
}