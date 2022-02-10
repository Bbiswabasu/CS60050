/*
Name - Bbiswabasu Roy
Roll - 19EC30008
Assignment - 1
No specific flags required
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cmath>
using namespace std;

//loads training or test data into vectors
void load_data(vector<vector<int>> &x,vector<int> &y, bool isTraining)
{
	fstream data_file;
	if(isTraining)
		data_file.open("project1.data",ios::in);
	else
		data_file.open("project1_test.data",ios::in);
	string line;
	while(getline(data_file,line))
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

	double initialEntropy,maxInfoGain=-1e9;
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
	adj.push_back(vector<int>(7,-1));

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

void display_tree(vector<vector<int>>& adj,vector<int> &nodeAttribute,int depth,int node,vector<string> &attrNames,vector<string> &attrValues,vector<string> &outputClass,fstream &dec_tree)
{
	bool isLeaf=1;
	for(int i=0;i<7;i++)
	{
		if(adj[node][i]!=-1)
		{
			isLeaf=0;
			for(int cnt=0;cnt<depth;cnt++)
			{
				dec_tree<<"|";
				dec_tree<<"   ";
			}
			dec_tree<<attrNames[nodeAttribute[node]]<<" = "<<((nodeAttribute[node]!=2 && nodeAttribute[node]!=3 )? attrValues[i] : to_string(i))<<"\n";
			display_tree(adj,nodeAttribute,depth+1,adj[node][i],attrNames,attrValues,outputClass,dec_tree);
		}
	}
	if(isLeaf)
	{
		for(int cnt=0;cnt<depth;cnt++)
			{
				dec_tree<<"|";
				dec_tree<<"   ";
			}
		dec_tree<<outputClass[nodeAttribute[node]-10]<<"\n";
	}
}

int predict(vector<vector<int>> &adj,vector<int> &x,vector<int> &nodeAttribute,int node)
{
	if(nodeAttribute[node]>=10)
		return nodeAttribute[node]-10;
	return predict(adj,x,nodeAttribute,adj[node][x[nodeAttribute[node]]]);
}

int main()
{
	vector<vector<int>> x_train(6); //feature vector of training set
	vector<int> y_train; //output vector of training set
	load_data(x_train,y_train,true);

	vector<vector<int>> adj; //stores adjacency list of the decision tree
	vector<int> values(6,-1); //stores values which are already fixed at current node
	int nodeID=-1; //assigns unique ID to each node
	vector<int> nodeAttribute; //stores which attribute to split on at some node
	build_model(adj,x_train,y_train,values,nodeID,nodeAttribute,-1);
	cout<<"Decision Tree built and stored in decision_tree.txt\n";

	vector<string> attrNames({"price","maint","doors","persons","lug_boot","safety"});
	vector<string> attrValues({"vhigh","high","med","low","small","big"});
	vector<string> outputClass({"unacc","acc","good","vgood"});
	fstream dec_tree;
	dec_tree.open("decision_tree.txt",ios::out);
	display_tree(adj,nodeAttribute,0,0,attrNames,attrValues,outputClass,dec_tree);

	vector<vector<int>> x_test(6); //feature vector of test set
	vector<int> y_test; //actual output vector of test set
	load_data(x_test,y_test,false);

	vector<int> prediction;
	for(int i=0;i<x_test[0].size();i++)
	{
		vector<int> currentTest;
		for(int j=0;j<6;j++)
			currentTest.push_back(x_test[j][i]);
		prediction.push_back(predict(adj,currentTest,nodeAttribute,0));
	}
	cout<<"Prediction completed and stored in prediction.txt\nError : ";
	double error=0;

	fstream output;
	output.open("prediction.txt",ios::out);
	for(int i=0;i<prediction.size();i++)
	{
		output<<outputClass[prediction[i]]<<"\n";
		if(prediction[i]!=y_test[i])
			error++;
	}
	error/=(double)y_test.size();
	cout<<error*100<<"%\n";
}