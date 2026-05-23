#pragma once 

#include <iostream>

using namespace std;

int CurrentCoin;

int CurrentBank;

int recompensa; //dinheiro recebido com baús, premios e moedas openworld

bool openMyWallet(){
    int op;
    int valor;
    cout << "  > Você abriu a sua carteira...\n"
         << "  > Você deseja ver o seu dinheiro? [Y/n]\n"
         << "  > ";

    cin >> op;
    cin.ignore();

    if(op == 'N' || op == 'n'){
        cout << "  > Você fechou a carteira.\n";
        cout << "  > Pressione ENTER para continuar...\n";
        cin.ignore();
        return false;
    }

    cout << "  > Você tem " << CurrentCoin << " moedas!\n";
    cout << "  > Pressione ENTER para continuar...\n";
    cin.ignore();
    
    cout << "  > Você deseja depositar/sacar seu dinheiro? [D/s]\n"
         << "  > ";

    cin >> op;
    cin.ignore();

    if(op == 'D' || op == 'd'){
        cout << "  > Quanto você deseja depositar? (Digite um valor positivo)\n"
             << "  > ";
        cin >> valor;
        cin.ignore();

        if(valor > CurrentCoin){
            cout << "  > Você não tem dinheiro suficiente para depositar essa quantia.\n";
            cout << "  > Pressione ENTER para continuar...\n";
            cin.ignore();
            return false;
        }
        CurrentCoin -= valor;
        CurrentBank += valor;
        cout << "  > Você depositou " << valor
             << " moedas de prata. Saldo atual na carteira: " << CurrentCoin 
             << " moedas de prata. Saldo atual no banco: " << CurrentBank 
             << " moedas de prata.\n";
        cout << "  > Pressione ENTER para continuar...\n";
        cin.ignore();
    } else {
        cout << "  > Saldo atual no banco: " << CurrentBank 
             << " moedas de prata.\n";
        cout << "  > Quanto você deseja sacar? (Digite um valor positivo)\n"
             << "  > ";
        cin >> valor;
        cin.ignore();

        if(valor > CurrentBank){
            cout << "  > Você não tem dinheiro suficiente para sacar essa quantia.\n";
            cout << "  > Pressione ENTER para continuar...\n";
            cin.ignore();
            return false;
        } else {
            CurrentBank -= valor;
            CurrentCoin += valor;
            cout << "  > Você sacou " << valor
                 << " moedas de prata. Saldo atual na carteira: " << CurrentCoin 
                 << " moedas de prata. Saldo atual no banco: " << CurrentBank 
                 << " moedas de prata.\n";
            cout << "  > Pressione ENTER para continuar...\n";
            cin.ignore();
        }
    }
    return true;
}

void gainCoin(int recompensa){
    CurrentCoin += recompensa;
    cout << "  > Você ganhou " << recompensa 
         << " moedas de prata! Saldo atual: " << CurrentCoin 
         << " moedas de prata.\n";
    cout << "  > Pressione ENTER para continuar...\n";
    cin.ignore();
}

void lostCoin(int valor){
    CurrentCoin -= valor;
    cout << "  > Você perdeu " << valor 
         << " moedas de prata! Saldo atual: " << CurrentCoin 
         << " moedas de prata.\n";
    cout << "  > Pressione ENTER para continuar...\n";
    cin.ignore();
}