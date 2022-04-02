#include<bits/stdc++.h>
using namespace std;

int main()
{
	cout<<"Enter number of features : ";
	int f;
	cin>>f;
	cout<<"Enter bias : ";
	vector<double> w(f+1);
	vector<double> del_w(f+1);
	vector<double> val(f+1);
	cin>>val[0];
	cout<<"Enter learning rate : ";
	double eta;
	cin>>eta;
	cout<<"Enter initial w : ";
	for(int i=0;i<=f;i++)
		cin>>w[i];
	while(1)
	{
		cout<<"Enter feature values : ";
		for(int i=1;i<=f;i++)
			cin>>val[i];
		double tar;
		cout<<"Enter target : ";
		cin>>tar;
		double y=0;
		for(int i=0;i<=f;i++)
			y+=w[i]*val[i];
		double o=1/(1+exp(-y));
		cout<<"Forward: y = "<<y<<"; o = "<<o<<"\n";
		double del=o*(1-o)*(tar-o);
		cout<<"Backward: Delta = "<<del<<"; Del_w = ";
		for(int i=0;i<=f;i++)
		{
			del_w[i]=eta*del*val[i];
			cout<<del_w[i]<<", ";
		}
		cout<<"; w = ";
		for(int i=0;i<=f;i++)
		{
			w[i]=w[i]+del_w[i];
			cout<<w[i]<<", ";
		}
		cout<<"\n";
	}
}