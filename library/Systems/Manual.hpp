#pragma once

#include <iostream>

using std::cout;
using std::cin;

std::string openManual;
std::string inManual;

void openManual(){
    cout << "  Você quer ver o Manual do Jogo?\n";
    cout << "  [Y/n]\n";
    cin << openManual;

    if(openManual == 'y' || openManual == 'Y')
        Manual();
}

void Manual(){
    cout << "  [Q] — | Instruções aos Status  |\n";
    cout << "  [W] — | Instruções às Classes  |\n";
    cout << "  [R] — | Instruções às Batalhas |\n";

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