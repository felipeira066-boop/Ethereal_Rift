#pragma once
#include <ctime>
#include <cstdlib>
#include <map>

using std::string;

std::map<string, short> mapMerc{
    mapMerc["Marcos"], 1;
    mapMerc["Lívia"],  2;
    mapMerc["Érik"],   3;
};

srand (time(NULL));

void genName(mapMerc M){
    rand Merchanter()%3;
    mapMerc(Merchanter);
}