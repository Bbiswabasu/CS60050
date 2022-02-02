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

void build_model(vector<vector<int>> &adj,vector<vector<int>> &x,vector<int> &y,vector<int> &values,int &nodeID,vector<int> &nodeAttribute,int parent)
{
	double eps=1e-6;

	double initialEntropy,maxInfoGain=-1;
	int bestAttribute=-1;
	vector<int> S;
	vector<vector<int>> filteredX(6);

	//calculate initial entropy
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
		{
			S.push_back(y[i]);
			for(int j=0;j<values.size();j++){
				filteredX[j].push_back(x[j][i]);
			}
		}
	}
	if(S.size()==0)
		return;
	initialEntropy=calculate_entropy(S);

	nodeID++;
	int myID=nodeID;
	adj.push_back(vector<int>(6,-1));

	if(parent!=-1){
		int parentBranch=values[nodeAttribute[parent]];
		adj[parent][parentBranch]=myID;
	}

	nodeAttribute.push_back(-1);

	//check if it the box is pure
	if(initialEntropy<eps)
	{
		nodeAttribute[myID]=S[0]+10;
		return;
	}

	//choose best attribute
	for(int i=0;i<values.size();i++)
	{
		if(values[i]!=-1)
			continue;
		//calculate gain for i-th attribute
		vector<vector<int>> Sv(7);
		for(int j=0;j<S.size();j++)
		{
			Sv[filteredX[i][j]].push_back(S[j]);
		}
		double newEntropy=0;
		for(int j=0;j<7;j++)
		{
			if(Sv[j].size()!=0)
			{
				newEntropy+=calculate_entropy(Sv[j])*Sv[j].size();
			}
		}
		newEntropy/=(double)S.size();
		if(initialEntropy-newEntropy>maxInfoGain)
		{
			bestAttribute=i;
			maxInfoGain=initialEntropy-newEntropy;
		}
	}

	nodeAttribute[nodeID]=bestAttribute;

	for(int i=0;i<7;i++)
	{
		values[bestAttribute]=i;
		build_model(adj,x,y,values,nodeID,nodeAttribute,myID);
		values[bestAttribute]=-1;
	}
}

void display_tree(vector<vector<int>>& adj,vector<int> &nodeAttribute,int depth,int node,vector<string> &attrNames,vector<string> &attrValues,vector<string> &outputClass)
{
	bool isLeaf=1;
	for(int i=0;i<6;i++)
	{
		if(adj[node][i]!=-1)
		{
			isLeaf=0;
			for(int cnt=0;cnt<depth*4;cnt++)
				cout<<" ";
			cout<<attrNames[nodeAttribute[node]]<<" = "<<((nodeAttribute[node]!=2 && nodeAttribute[node]!=3 )? attrValues[i] : to_string(i))<<"\n";
			display_tree(adj,nodeAttribute,depth+1,adj[node][i],attrNames,attrValues,outputClass);
		}
	}
	if(isLeaf)
	{
		for(int cnt=0;cnt<depth*4;cnt++)
				cout<<" ";
		cout<<outputClass[nodeAttribute[node]-10]<<"\n";
	}
}

int main()
{
	vector<vector<int>> x_train(6); //feature vector
	vector<int> y_train; //output vector
	load_data(x_train,y_train,true);

	vector<vector<int>> adj;
	vector<int> values(6,-1);
	int nodeID=-1;
	vector<int> nodeAttribute;
	build_model(adj,x_train,y_train,values,nodeID,nodeAttribute,-1);

	vector<string> attrNames({"price","maint","doors","persons","lug_boot","safety"});
	vector<string> attrValues({"vhigh","high","med","low","small","big"});
	vector<string> outputClass({"unacc","acc","good","vgood"});
	display_tree(adj,nodeAttribute,0,0,attrNames,attrValues,outputClass);

}