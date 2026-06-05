#pragma once

#include <iostream>

using std::cout;
using std::cin;

std::string manualChoice;
std::string inManual;

// Forward declarations
void Manual();
void InstrucoesStatus();
void InstrucoesClasses();
void InstrucoesBatalhas();

void openManualFunc(){
    cout << "  Você quer ver o Manual do Jogo?\n";
    cout << "  [Y/n]\n";
    cin >> manualChoice;

    if(manualChoice == "y" || manualChoice == "Y")
        Manual();
}

void Manual(){
    cout << "  [Q] — | Instruções aos Status  |\n";
    cout << "  [W] — | Instruções às Classes  |\n";
    cout << "  [R] — | Instruções às Batalhas |\n";

    cin >> inManual;

    if (inManual == "Q" || inManual == "q")
        InstrucoesStatus();
    if(inManual == "W" || inManual == "w")
        InstrucoesClasses();
    if(inManual == "R" || inManual == "r")
        InstrucoesBatalhas();
}

void InstrucoesStatus(){
    cout << "  [Status Info]\n";
}

void InstrucoesClasses(){
    cout << "  [Classes Info]\n";
}

void InstrucoesBatalhas(){
    cout << "  [Battle Info]\n";
}