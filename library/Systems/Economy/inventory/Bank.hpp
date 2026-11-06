#pragma once 

#include <iostream>

using namespace std;

int CurrentCoin = 0;

int CurrentBank = 0;

int recompensa = 0; //dinheiro recebido com baús, premios e moedas openworld

bool openMyWallet(){
    int op;
    int valor;
     cout << "  > You opened your wallet...\n"
         << "  > Would you like to check your balance? [Y/n]\n"
         << "  > ";

     cin >> op;

    if(op == 'N' || op == 'n'){
          cout << "  > You closed your wallet.";
          cin.ignore();
        return false;
    }

     cout << "  > You have " << CurrentCoin << " coins!";
     cin.ignore();
    
     cout << "  > Would you like to deposit or withdraw money? [D/w]\n"
         << "  > ";

     cin >> op;

    if(op == 'D' || op == 'd'){
          cout << "  > How much would you like to deposit? (Enter a positive amount)\n"
             << "  > ";
          cin >> valor;

        if(valor > CurrentCoin){
               cout << "  > You do not have enough money to deposit that amount.";

               cin.ignore();
            return false;
        }
        CurrentCoin -= valor;
        CurrentBank += valor;
          cout << "  > You deposited " << valor
             << " silver coins. Current wallet balance: " << CurrentCoin
             << " silver coins.";
          cin.ignore();
          cout << " Saldo atual no banco: " << CurrentBank 
             << " silver coins.";
          cin.ignore();
    } else {
          cout << "  > Saldo atual no banco: " << CurrentBank 
             << " silver coins.";
          cin.get();
          cout << "  > How much would you like to withdraw? (Enter a positive amount)\n"
             << "  > ";
          cin >> valor;
          cin.ignore();

        if(valor > CurrentBank){
               cout << "  > You do not have enough money to withdraw that amount.";

               cin.ignore();
            return false;
        } else {
            CurrentBank -= valor;
            CurrentCoin += valor;
               cout << "  > You withdrew " << valor
                 << " silver coins. ";
               cin.get();
               cout << "Current wallet balance: " << CurrentCoin
                 << " silver coins.";
               cin.get();
               cout << " Saldo atual no banco: " << CurrentBank 
                 << " silver coins.";

               cin.ignore();
        }
    }
    return true;
}

void gainCoin(int recompensa){
    CurrentCoin += recompensa;
     cout << "  > You earned " << recompensa
         << " silver coins! \n  Current balance: " << CurrentCoin
         << " silver coins.";
     cin.ignore();
}

void lostCoin(int valor){
    CurrentCoin -= valor;
     cout << "  > You lost " << valor
         << " silver coins! \n  Current balance: " << CurrentCoin
         << " silver coins.";
     cin.ignore();
}