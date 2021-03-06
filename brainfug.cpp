#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include "conio.h"

#define DEFUG false

using namespace std;

void Error(string message, signed int position)
{
	cout <<"Error: "<< message <<" Character: "<< position << endl;
}

void Error(string message)
{
	cout <<"Error: "<< message << endl;
}

/*
	>	Move the pointer to the right
	<	Move the pointer to the left
	+	Increment the memory cell under the pointer
	-	Decrement the memory cell under the pointer
	.	Output the character signified by the cell at the pointer
	,	Input a character and store it in the cell at the pointer
	[	Jump past the matching ] if the cell under the pointer is 0
	]	Jump back to the matching [ if the cell under the pointer is nonzero
*/
void Brainfug(string code)
{
	vector< signed int > stuff;
	stuff.push_back(0);
	
	signed int position = 0;
	
	map< unsigned int, unsigned int > loops;
	
	signed int loop = 0;
	
	int lCode = code.length();
	
	for(int i = 0; i < lCode; i++)
	{
		char c = code.at(i);
		
		if(c == '[')
		{
			loop++;
			
			loops[i] = loop;
		}
		else if(c == ']')
		{
			loop--;
			
			loops[i] = loop;
		}
	}
	
	if(loop != 0)
	{
		Error("Unopened or unclosed loop detected.");
		return;
	}
	
	for(signed int i = 0; i < lCode; i++)
	{
		char c = code.at(i);
		
		switch(c)
		{
			case '>':
			{
				position++;
				
				if(position >= stuff.size())
				{
					stuff.push_back(0);
				}
				
				break;
			}
			
			case '<':
			{
				position--;
				
				if(position < 0)
				{
					Error("Cannot point to a position before 0.", i);
					return;
				}
				
				break;
			}
			
			case '+':
			{
				stuff[position]++;
				
				break;
			}
			
			case '-':
			{
				stuff[position]--;
				
				break;
			}
			
			case '.':
			{
				cout << (char)stuff[position];
				
				break;
			}
			
			case ',':
			{
				stuff[position] = getch();
				
				break;
			}
			
			case '[':
			{
				if(stuff[position] == 0)
				{
					unsigned int targetLoopLevel = loops[position] - 1;
					unsigned int xLoop = loops[position];
					
					while(xLoop != targetLoopLevel)
					{
						i++;
						
						if(i >= lCode)
						{
							Error("Went beyond code.", i);
							
							return;
						}
						
						if(code.at(i) == '[')
						{
							xLoop++;
						}
						else if(code.at(i) == ']')
						{
							xLoop--;
						}
					}
				}
				
				break;
			}
			
			case ']':
			{
				if(stuff[position] != 0)
				{
					unsigned int targetLoopLevel = loops[position] + 1;
					unsigned int xLoop = loops[position];
					
					while(xLoop != targetLoopLevel)
					{
						i--;
						
						if(code.at(i) == '[')
						{
							xLoop++;
						}
						else if(code.at(i) == ']')
						{
							xLoop--;
						}
					}
				}
				
				break;
			}
			
			case '#': // traditional debug command - not part of language specification
			{
				unsigned long nStuff = stuff.size();
				
				cout <<"=== DEFUG ==="<< endl <<" There are "<< nStuff <<" cells."<< endl;
				
				for(unsigned long iStuff = 0; iStuff < nStuff; iStuff++)
				{
					cout <<"["<< (iStuff + 1) <<"]\t"<< stuff[iStuff] << endl;
				}
				
				cout <<"============="<< endl;
				
				break;
			}
			
			default:
			{
				break;
			}
		}
	}
	
	if(DEFUG)
	{
		cout << endl <<"| ";
		for(int i = 0; i < stuff.size(); i++)
		{
			cout << stuff[i] <<" | ";
		}
	}
}

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		cout <<"Usage: "<< argv[0] <<" FILENAME.bf"<< endl;
		
		return 0;
	}
	
	ifstream fugFile(argv[1]);
	
	if(!fugFile.good())
	{
		fugFile.close();
		
		cout<<"File '"<< argv[1] <<"' not found."<< endl;
		
		return 0;
	}
	
	string code = "";
	string line = "";
	int nLines = 0;
	
	while(getline(fugFile, line))
	{
		nLines++;
		
		code += ((nLines > 1) ? "\n"+ line : line);
	}
	
	fugFile.close();
	
	Brainfug(code);
	
	cout << endl;
	
	return 0;
}







