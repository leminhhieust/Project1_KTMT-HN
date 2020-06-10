#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <stdlib.h>
#include "QInt.h"

using namespace std;

//get a vector of token from string
vector<string> getToken(string str)
{
	char delim = ' ';
	vector <string> token;
	stringstream check1(str);
	string intermediate;

	while (getline(check1, intermediate, delim))
	{
		token.push_back(intermediate);
	}
	return token;
}


//push each line of the input file into a vector of string
vector<string> inputFile(string path) {
	ifstream f(path);
	vector<string> s;

	if (!f) {
		cout << "Can not open file!";
		return s;
	}

	while (!f.eof()) {
		string s0;
		getline(f, s0);
		s.push_back(s0);
	}

	return s;
}

int main(int argc, char** argv) //.exe .in .out
{
	fstream fOutput(argv[2], fstream::app);
	if (!fOutput) { cout << "Open failed" << endl; return 0; }

	vector<string> lines = inputFile(argv[1]);
	int n = lines.size();
	int i = 0;

	while (i < n)
	{
		vector<string> tokens = getToken(lines[i]);

		if (tokens.size() == 3)
		{
			QInt q(tokens[2], stoi(tokens[0]));
			if (tokens[1] == "10")
			{
				fOutput << q.toDecimal() << endl;
			}
			else if (tokens[1] == "2")
			{
				fOutput << q.toBinary() << endl;
			}
			else if (tokens[1] == "16")
			{
				fOutput << q.toHexa() << endl;
			}
			else if (tokens[1] == "rol")
			{
				q.ROL();
				fOutput << q.printQINT(stoi(tokens[0])) << endl;
			}
			else if (tokens[1] == "ror")
			{
				q.ROR();
				fOutput << q.printQINT(stoi(tokens[0])) << endl;
			}
			else if (tokens[1] == "~")
			{
				~q;
				fOutput << q.printQINT(stoi(tokens[0])) << endl;
			}
		}
		else if (tokens.size() == 4)
		{
			if (tokens[2] != "<<" && tokens[2] != ">>")
			{
				QInt q1(tokens[1], stoi(tokens[0]));
				QInt q2(tokens[3], stoi(tokens[0]));
				if (tokens[2] == "+")
				{
					QInt q = q1 + q2;
					fOutput << q.printQINT(stoi(tokens[0])) << endl;
				}
				else if (tokens[2] == "-")
				{
					QInt q = q1 - q2;
					fOutput << q.printQINT(stoi(tokens[0])) << endl;
				}
				else if (tokens[2] == "*")
				{
					QInt q = q1 * q2;
					fOutput << q.printQINT(stoi(tokens[0])) << endl;
				}
				else if (tokens[2] == "/")
				{
					QInt q = q1 / q2;
					fOutput << q.printQINT(stoi(tokens[0])) << endl;
				}
				else if (tokens[2] == "&")
				{
					QInt q = q1 & q2;
					fOutput << q.printQINT(stoi(tokens[0])) << endl;
				}
				else if (tokens[2] == "^")
				{
					QInt q = q1 ^ q2;
					fOutput << q.printQINT(stoi(tokens[0])) << endl;
				}
				else if (tokens[2] == "|")
				{
					QInt q = q1 | q2;
					fOutput << q.printQINT(stoi(tokens[0])) << endl;
				}
			}
			else
			{
				QInt q(tokens[1], stoi(tokens[0]));
				if (tokens[2] == "<<")
				{
					q << stoi(tokens[3]);
					fOutput << q.printQINT(stoi(tokens[0])) << endl;
				}
				else if (tokens[2] == ">>")
				{
					q >> stoi(tokens[3]);
					fOutput << q.printQINT(stoi(tokens[0])) << endl;
				}
			}
		}
		i++;
	}

	return 0;
}
