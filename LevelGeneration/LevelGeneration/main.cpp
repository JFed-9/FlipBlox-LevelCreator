#include <cstdlib>
#include <iostream>
#include <limits>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

int main()
{
	int dim;
	cout << "Dimensions: ";
	
	while (!(cin >> dim))
	{
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin.clear();
		cout << "Error, please try again: ";
	}
	
	vector<vector<vector<int> > > grids;
	vector<vector<int> > grid;
	ifstream in;
	string file = "templates.txt";
	in.open(file.c_str());
	string input;
	while (!in.eof())
	{
		getline(in, input);
		parse(grid, input);
		for (int i = 0; i < input.length() - 1; i++)
		{
			getline(in, input);
			parse(grid, input);
		}
	}
	in.close();

}

void parse(vector<vector<int> > &grid, string input)
{
	vector<int> line;
	for (char ch : input)
	{
		line.push_back(ch - '0');
	}
	grid.push_back(line);
}