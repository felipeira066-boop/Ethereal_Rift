#pragma once
#include <ctime>
#include <cstdlib>
#include <map>
#include <string>

using std::string;

std::map<short,string> Merc = {
    {1, "Marcos"},
    {2, "Lívia"},
    {3, "Érik"}
};  

string genName(){
    string name;
    int Merchanter = rand ()%3;

    if (Merchanter == 0) name = Merc[1];
    if (Merchanter == 1) name = Merc[2];
    if (Merchanter == 2) name = Merc[3];

    return name;
}