#include <cstdlib>
#include <iostream>
#include <limits>
#include <fstream>
#include <vector>
#include <string>
#include <time.h>
#include <algorithm>

#define dbg(x) cout << #x << ": " << x << endl


using namespace std;

int MIN_MIX = 4;
int MAX_MIX = 10;
int NUM_LEVELS = 0;

vector<vector<int> > solution;

vector<vector<vector<int> > > used;

int startRow;
int startColumn;
int dist;

bool compare(vector<vector<int> > grid1)
{
	/**/

	for (int use = 0; use < used.size(); use++)
	{
		int identical = true;
		for (int i = 0; i < grid1.size(); i++)
		{
			for (int j = 0; j < grid1.size(); j++)
			{
				if (grid1.at(i).at(j) != used.at(use).at(i).at(j))
				{
					identical = false;
				}
			}
		}
		if (identical)
		{
			for (int x = 0; x < grid1.size(); x++)
			{
				for (int y = 0; y < grid1.size(); y++)
				{
					//cout << grid1.at(x).at(y);
				}
				//cout << endl;
			}
			//cout << "Is the same as\n";
			for (int x = 0; x < used.at(use).size(); x++)
			{
				for (int y = 0; y < used.at(use).size(); y++)
				{
					//cout << used.at(use).at(x).at(y);
				}
				//cout << endl;
			}
			//cout << endl;
			return true;
		}
	}
	return false;
}

bool mixUp(vector<vector<int> > &grid)
{
	used.push_back(grid);
	startRow = rand() % grid.size();
	startColumn = rand() % grid.size();
	
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
		//cout << "Trying again - corner\n";
		return false;
	}
	if (dir == 0)
	{
		if (grid.size() - startColumn == 1)
			dist = 1;
		else
			dist = rand() % (grid.size() - startColumn - 1) + 1;
		//dbg(startRow);
		//dbg(startColumn);
		//dbg(dist);
		//cout << "dir: " << "horizontal" << endl;
		if (startColumn + dist > grid.size() - 1)
		{
			//cout << "Trying again - out of range\n";
			
			return false;
		}
		reverse(grid.at(startRow).begin() + startColumn, grid.at(startRow).begin() + startColumn + dist);
	}
	else
	{
		if (grid.size() - startRow == 1)
			dist = 1;
		else
			dist = rand() % (grid.size() - startRow - 1) + 1;
		//dbg(startRow);
		//dbg(startColumn);
		//dbg(dist);
		//cout << "dir: " << "vertical" << endl;

		int begin = startRow;
		int end = dist + startRow;
		if (end > grid.size() - 1)
		{
			//cout << "Trying again - out of range 2\n";
			return false;
		}
		for (int i = 0; i < (dist+1)/2; i++)
		{
			int temp = grid.at(begin + i).at(startColumn);
			grid.at(i + begin).at(startColumn) = grid.at(end - i).at(startColumn);
			grid.at(end - i).at(startColumn) = temp;
		}
	}
	if (compare(grid))
	{
		return false;
	}
	vector<int> nextMove;
	int size = grid.size();
	if (dir == 0)
		nextMove = vector<int>({ startColumn, size - 1 - startRow, startColumn + dist - 1, size - 1 - startRow});
	else							//[2, 4, 2, 7 ],
		nextMove = vector<int>({ startColumn, size - 1 - startRow, startColumn, size - 1 - startRow - dist });
	/*cout << "Move: ";
	for (int i : nextMove)
		cout << i << " ";
	cout << endl;*/
	solution.push_back(nextMove);
	return true;
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
	int easy_index = 0;
	int medium_index = 0;
	int hard_index = 0;

	cout << "What level are we on (easy): ";
	cin >> easy_index;
	cout << "What level are we on (medium): ";
	cin >> medium_index;
	cout << "What level are we on (hard): ";
	cin >> hard_index;

	cout << "How many levels do you want?\n>";
	cin >> NUM_LEVELS;

	while (!in.eof())
	{
		getline(in, input);
		while (input == "" || input == "\n" || input.size() < 4) getline(in, input);
		parse(grid, input);
		for (int i = 0; i < input.length() - 1; i++)
		{
			getline(in, input);
			while (input == "" || input == "\n" || input.size() < 4) getline(in, input);
			parse(grid, input);
		}
		grids.push_back(grid);
		grid.clear();
	}
	in.close();

	MIN_MIX = 3;
	MAX_MIX = 6;
	ofstream out;
	bool ren = false;
	if (easy_index != 1)
	{
		in.open("flipblox-Easy-export.json");
		out.open("flipblox-Easy-export-new.json");
		string line;
		getline(in,line);
		for (std::string tmp; std::getline(in,tmp); line.swap(tmp)) {
			out << line << '\n';
		}
		in.close();
		out << "}, ";
		ren = true;
	}
	else {
		out.open("flipblox-Easy-export.json");
		out << "[ null, ";
	}
	for (int i = 0; i < NUM_LEVELS; i++)
	{
		used.clear();
		grid = grids.at(rand() % grids.size());
		while (grid.size() != 4) grid = grids.at(rand() % grids.size());
		vector<vector<int> > mixed;
		mixed = grid;

		int mixes = rand() % (MAX_MIX - MIN_MIX) + MIN_MIX;

		for (int i = 0; i < mixes; i++)
		{
			while (!mixUp(mixed)) {}
		}

		reverse(solution.begin(), solution.end());

		out << "{" << endl;
		int* index = (grid.size() == 4) ? &easy_index : ((grid.size() == 5) ? &medium_index : &hard_index);
		out << "\t\"levelID\" : " << (*index)++ << "," << endl;
		out << endl;
		out << "\t\"size\" : " << grid.size() << "," << endl;
		out << "\t\"title\" : " << "\"System\"," << endl;
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

		out << "\t\"solution\" : [ ";
		for (int j = 0; j < solution.size(); j++)
		{
			if (j != 0)
			{
				out << "\t\t";
			}
			out << "[" << solution.at(j).at(0);
			for (int i = 1; i < 4; i++)
			{
				out << ", " << solution.at(j).at(i);
			}
			out << " ]";
			if (j != solution.size() - 1)
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
		if (i < NUM_LEVELS - 1)
			out << ",";
		out << " ";

		solution.clear();
	}
	out << " ]";
	out.close();
	if (ren) {
		remove("flipblox-Easy-export.json");
		rename("flipblox-Easy-export-new.json", "flipblox-Easy-export.json");
	}
	cout << "Easy levels done...\n";

	MIN_MIX = 7;
	MAX_MIX = 10;
	ren = false;
	if (medium_index != 1)
	{
		in.open("flipblox-Medium-export.json");
		out.open("flipblox-Medium-export-new.json");
		string line;
		getline(in, line);
		for (std::string tmp; std::getline(in, tmp); line.swap(tmp)) {
			out << line << '\n';
		}
		in.close();
		out << "}, ";
		ren = true;
	}
	else {
		out.open("flipblox-Medium-export.json");
		out << "[ null, ";
	}
	for (int i = 0; i < NUM_LEVELS; i++)
	{
		used.clear();
		grid = grids.at(rand() % grids.size());
		while (grid.size() != 5) grid = grids.at(rand() % grids.size());
		vector<vector<int> > mixed;
		mixed = grid;

		int mixes = rand() % (MAX_MIX - MIN_MIX) + MIN_MIX;

		for (int i = 0; i < mixes; i++)
		{
			while (!mixUp(mixed)) {}
		}

		reverse(solution.begin(), solution.end());

		out << "{" << endl;
		int* index = (grid.size() == 4) ? &easy_index : ((grid.size() == 5) ? &medium_index : &hard_index);
		out << "\t\"levelID\" : " << (*index)++ << "," << endl;
		out << endl;
		out << "\t\"size\" : " << grid.size() << "," << endl;
		out << "\t\"title\" : " << "\"System\"," << endl;
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
			}
			else
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

		out << "\t\"solution\" : [ ";
		for (int j = 0; j < solution.size(); j++)
		{
			if (j != 0)
			{
				out << "\t\t";
			}
			out << "[" << solution.at(j).at(0);
			for (int i = 1; i < 4; i++)
			{
				out << ", " << solution.at(j).at(i);
			}
			out << " ]";
			if (j != solution.size() - 1)
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
		if (i < NUM_LEVELS - 1)
			out << ",";
		out << " ";

		solution.clear();
	}
	out << " ]";
	out.close();
	if (ren) {
		remove("flipblox-Medium-export.json");
		rename("flipblox-Medium-export-new.json", "flipblox-Medium-export.json");
	}
	cout << "Medium levels done...\n";

	MIN_MIX = 10;
	MAX_MIX = 15;
	ren = false;
	if (hard_index != 1)
	{
		in.open("flipblox-Hard-export.json");
		out.open("flipblox-Hard-export-new.json");
		string line;
		getline(in, line);
		for (std::string tmp; std::getline(in, tmp); line.swap(tmp)) {
			out << line << '\n';
		}
		in.close();
		out << "}, ";
		ren = true;
	}
	else {
		out.open("flipblox-Hard-export.json");
		out << "[ null, ";
	}
	for (int i = 0; i < NUM_LEVELS; i++)
	{
		used.clear();
		grid = grids.at(rand() % grids.size());
		while (grid.size() != 6) grid = grids.at(rand() % grids.size());
		vector<vector<int> > mixed;
		mixed = grid;

		int mixes = rand() % (MAX_MIX - MIN_MIX) + MIN_MIX;

		for (int i = 0; i < mixes; i++)
		{
			while (!mixUp(mixed)) {}
		}

		reverse(solution.begin(), solution.end());

		out << "{" << endl;
		int* index = (grid.size() == 4) ? &easy_index : ((grid.size() == 5) ? &medium_index : &hard_index);
		out << "\t\"levelID\" : " << (*index)++ << "," << endl;
		out << endl;
		out << "\t\"size\" : " << grid.size() << "," << endl;
		out << "\t\"title\" : " << "\"System\"," << endl;
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
			}
			else
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

		out << "\t\"solution\" : [ ";
		for (int j = 0; j < solution.size(); j++)
		{
			if (j != 0)
			{
				out << "\t\t";
			}
			out << "[" << solution.at(j).at(0);
			for (int i = 1; i < 4; i++)
			{
				out << ", " << solution.at(j).at(i);
			}
			out << " ]";
			if (j != solution.size() - 1)
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
		if (i < NUM_LEVELS - 1)
			out << ",";
		out << " ";

		solution.clear();
	}
	out << " ]";
	out.close();
	if (ren) {
		remove("flipblox-Hard-export.json");
		rename("flipblox-Hard-export-new.json", "flipblox-Hard-export.json");
	}
	cout << "Hard levels done...\n";

	//cout << "Hope it worked, good luck!\nPress any key to continue...";
	system("pause");
	return 0;
}


/*

11211
12221
11211
12221
11221

*/
