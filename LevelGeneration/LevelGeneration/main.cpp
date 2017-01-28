#include <cstdlib>
#include <iostream>
#include <limits>
#include <fstream>
#include <vector>
#include <string>
#include <time.h>
#include <algorithm>

#define dbg(x) //cout << #x << ": " << x << endl
using namespace std;

const int MIN_MIX = 4;
const int MAX_MIX = 10;
const int NUM_LEVELS = 30;

bool compare(vector<vector<int> > grid1, vector<vector<int> > grid2)
{
	for (int i = 0; i < grid1.size(); i++)
	{
		for (int j = 0; j < grid1.size(); j++)
		{
			if (grid1.at(i).at(j) != grid2.at(i).at(j))
				return false;
		}
	}
	return true;
}

void mixUp(vector<vector<int> > &grid)
{
	vector<vector<int> > prev;
	prev = grid;
	int startRow = rand() % grid.size();
	int startColumn = rand() % grid.size();

	int dir = rand() % 2;
	if (startRow == grid.size() - 1)
	{
		dir = 1;
	}
	if (startColumn == grid.size() - 1)
	{
		dir = 0;
	}
	if (startColumn == grid.size() - 1 && startRow == grid.size() - 1)
	{
		mixUp(grid);
		return;
	}
	if (dir == 0)
	{
		int dist;
		if (grid.size() - startColumn == 1)
			dist = 1;
		else
			dist = rand() % (grid.size() - startColumn - 1) + 1;
		dbg(startRow);
		dbg(startColumn);
		dbg(dist);
		//cout << "dir: " << "horizontal" << endl;
		if (startColumn + dist > grid.size() - 1)
		{
			mixUp(grid);
			return;
		}
		reverse(grid.at(startRow).begin() + startColumn, grid.at(startRow).begin() + startColumn + dist);
	}
	else
	{
		int dist;
		if (grid.size() - startRow == 1)
			dist = 1;
		else
			dist = rand() % (grid.size() - startRow - 1) + 1;
		dbg(startRow);
		dbg(startColumn);
		dbg(dist);
		//cout << "dir: " << "vertical" << endl;

		int begin = startRow;
		int end = dist + startRow;
		if (end > grid.size() - 1)
		{
			mixUp(grid);
			return;
		}
		for (int i = 0; i < (dist+1)/2; i++)
		{
			int temp = grid.at(begin + i).at(startColumn);
			grid.at(i + begin).at(startColumn) = grid.at(end - i).at(startColumn);
			grid.at(end - i).at(startColumn) = temp;
		}
	}
	if (compare(grid,prev))
	{
		mixUp(grid);
		return;
	}
	for (int i = 0; i < grid.size(); i++)
	{
		for (int j = 0; j < grid.at(i).size(); j++)
		{
			//cout << grid.at(i).at(j) << " ";
		}
		//cout << endl;
	}
	
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

int main()
{
	srand(time(NULL));
	
	vector<vector<vector<int> > > grids;
	vector<vector<int> > grid;
	ifstream in;
	string file = "templates.txt";
	in.open(file.c_str());
	string input;
	int index = 0;
	while (!in.eof())
	{
		getline(in, input);
		parse(grid, input);
		for (int i = 0; i < input.length() - 1; i++)
		{
			getline(in, input);
			parse(grid, input);
		}
		grids.push_back(grid);
		grid.clear();
	}
	in.close();
	system("mkdir LEVELS");
	for (int i = 0; i < NUM_LEVELS; i++)
	{
		grid = grids.at(rand() % grids.size());

		vector<vector<int> > mixed;
		mixed = grid;

		int mixes = rand() % (MAX_MIX - MIN_MIX) + MIN_MIX;

		for (int i = 0; i < mixes; i++)
		{
			mixUp(mixed);
		}
		ofstream out;
		file = "./LEVELS/Hard_";
		if (grid.size() == 4)
			file = "./LEVELS/Easy_";
		if (grid.size() == 5)
			file = "./LEVELS/Medium_";
		file += to_string(index);
		file += ".json";
		out.open(file.c_str());


		out << "{" << endl;
		out << "\t\"level\" : " << 99 << "," << endl;
		out << endl;
		out << "\t\"spaces\" : [ ";
		for (int j = 0; j < grid.size(); j++)
		{
			if (j != 0)
			{
				out << "\t\t";
			}
			out << "[1";
			for (int i = 0; i < grid.size() - 1; i++)
			{
				out << ", 1";
			}
			out << " ]";
			if (j != grid.size() - 1)
			{
				out << "," << endl;
			} else
			{
				out << " ]," << endl << endl;
			}

		}


		out << "\t\"tiles\" : [ ";
		for (int j = 0; j < grid.size(); j++)
		{
			if (j != 0)
			{
				out << "\t\t";
			}
			out << "[" << mixed.at(j).at(0);
			for (int i = 1; i < grid.size(); i++)
			{
				out << ", " << mixed.at(j).at(i);
			}
			out << " ]";
			if (j != grid.size() - 1)
			{
				out << "," << endl;
			}
			else
			{
				out << " ]," << endl << endl;
			}

		}

		out << "\t\"complete\" : [ ";
		for (int j = 0; j < grid.size(); j++)
		{
			if (j != 0)
			{
				out << "\t\t";
			}
			out << "[" << grid.at(j).at(0);
			for (int i = 1; i < grid.size(); i++)
			{
				out << ", " << grid.at(j).at(i);
			}
			out << " ]";
			if (j != grid.size() - 1)
			{
				out << "," << endl;
			}
			else
			{
				out << " ]," << endl << endl;
			}

		}

		out << "\t\"oneStar\" : " << (int)(mixes * 2) << "," << endl;
		out << "\t\"twoStar\" : " << (int)(mixes * 1.5) << "," << endl;
		out << "\t\"threeStar\" : " << mixes << endl;
		out << "}";

		out.close();
		index++;
	}
	return 0;
}


/*

11211
12221
11211
12221
11221

*/
