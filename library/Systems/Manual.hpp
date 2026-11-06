#pragma once

#include <iostream>

using std::cout;
using std::cin;

std::string openManual;
std::string inManual;

void openManual(){
    cout << "  Would you like to view the game manual?\n";
    cout << "  [Y/n]\n";
    cin << openManual;

    if(openManual == 'y' || openManual == 'Y')
        Manual();
}

void Manual(){
    cout << "  [Q] — | Stats Guide   |\n";
    cout << "  [W] — | Class Guide   |\n";
    cout << "  [R] — | Battle Guide  |\n";

    cint << inManual;

    if (inManual == 'Q' || inManual == 'q')
        InstrucoesStatus();
    if(inManual == 'W' || inManual == 'w')
        InstrucoesClasses();
    if(inManual == 'R' || inManual == 'r')
        InstrucoesBatalhas();
}

void InstrucoesStatus()

void InstrucoesClasses()

void InstrucoesBatalhas()