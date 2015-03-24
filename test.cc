////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <czmq.h>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>

using namespace std;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::string;



string findDirection(string name, map<string,string> map)
	{
		string direction = "";
		for(auto o: map)
			{
				if (o.first.compare(name) == 0)
					{
						direction = o.second;
					}
			}
		return direction;
	} 


void fill_map(vector<string> data , map<string,string>& map)
	{
		for (int i = 1; i <= data.size()/2;  i++)
			{
				map[data[2*i-2]] = data[2*i-1];
			}
		
	}


void do_enemy_move(string name,map<string,string> map)
	{

		string direction;
		direction = findDirection(name, map);
		cout << direction << endl;


		if (direction.compare("left") == 0)
			{
				cout << "move left"<<endl;
			}
		if (direction.compare("right") == 0)
			{
				cout << "move right"<<endl;
			}
		if (direction.compare("up") == 0)
			{
				cout << "move up"<<endl;
			}
		if (direction.compare("down") == 0)
			{
				cout << "move down"<<endl;
			}
	}


int main (int argc, char** argv)
{

	///ARMAR MAPA CON 6 MENSAJES Q LLEGNA COMO CADENAS.
	vector<string> datos = {"gabriel","left","david","right","pepe","up","juanito","down"}; 

	map<string, string> map;
	fill_map(datos , map);

	for (auto o : map)
		{
			cout << o.first << "=>" << o.second <<endl;
		}

	

	do_enemy_move("pepe",map);
	return 0;
}