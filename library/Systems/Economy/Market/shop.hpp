#pragma once
#include <cstdint>
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "library/assets/Monsters/Enemy.hpp"
#include "library/Systems/Itens/Armaments.hpp"

using std::uint16_t;
using std::out;
using std::cin;
using std::string;

class Money : public Enemies{

    protected:
        uint16_t CurrentCoin;
        long int MaxCoin = 9999;
        uint16_t valor_to_pay;

    public:

        void setMaxCoin(){
            if(CurrentCoin >= MaxCoin) {
                CurrentCoin = MaxCoin;
                cout << "  Limite máximo de moedas atingido:" << MaxCoin << "\n";
            }
        }

        void getMoney(dataEnemies dE){
            CurrentCoin += dE.earnCoin;
            cout << "  Você recebeu " << dE.earnCoin << " Moedas\n";
            setMaxCoin();
            cout << "  Saldo atual: " << CurrentCoin << std::endl;
            
        }

};

enum class MerchantID{
    PABLO        = 0,
    LIVIA       = 1,
    JOAO        = 2,
    MARIA       = 3,
    BRUNO       = 4,
    ERICK       = 5,
    OTAVIO      = 6,
    JUAN        = 7,
    BRITHNAY    = 8,
    MALCON      = 9
};

class Market : public Money{
    
    private:
        char enter = 'n';
        srand (time(NULL));
        std::string typeBuy;

        static struct Merchanter{
            MerchantID  id;
            std::string name;
        }

        std::vector<Merchanter> nameTable{
            {MerchantID::PABLO,    "Pablo"},
            {MerchantID::LIVIA,    "Lívia"},
            {MerchantID::JOAO,     "João"},
            {MerchantID::MARIA,    "Maria"},
            {MerchantID::BRUNO,    "Bruno"},
            {MerchantID::ERICK,    "Erick"},
            {MerchantID::OTAVIO,   "Otávio"},
            {MerchantID::JUAN,     "Juan"},
            {MerchantID::BRITHNAY, "Brithney"},
            {MerchantID::MALCON,   "Malcon"}

        };

    public:
        

        int buy;

        char buyOther;
        char continueBuy;
        void enterShop(){
            cout << "  Você encontrou um Mercador!\n  Ele te convida à entrar <y/n>";
            cin >> enter;
            if(enter == 's' || enter == 'S'){
                inShop();
            } 
            return;
        }

        Merchanter Merc;
        
        void inShop(){
            int i = rand MerchanterGen()%10;
            Merc.name[i];

            cout << "  — Olá! Meu nome é " << Merc.name << "\n";
            cout << "  — Eu estou vendendo algumas armas você. Que tal dar uma olhadinha\n";
            cout << "  — Eu Tenho armas de Guerreiro, Arqueiro e Magos. Qual você quer ver primeiro?\n  <w/a/m>";
            cin >> typeBuy;
            cout << "  — Exelente escolha1\n"; 

            switch (typeBuy){
                case 'w' || 'W':
                    WarriorBuy();
                    break;
                case 'a' || 'A':
                    ArrowBuy();
                    break;
                case 'm' || 'M':
                    MageBuy();
                    break;

                default:
                    WarriorBuy();
                    break;
            }
        }

        void WarriorBuy(){
            cout << "  — Eu tenho alguns Machados de Madeiras e um Martelo do Aprendiz.\n";
            cout << "  Saldo atual: " << CurrentCoin << "\n";
            cout << "  [1] - Machado de Madeira ————————————— C$32.00\n";
            cout << "  [2] - Martelo do Aprendiz ———————————— C$28.00\n";

            cin >> buy;

            if(buy == '1'){
                if(CurrentCoin >= 32){
                    cout << "  Compra realizada com sucesso!\n";

                    CurrentCoin -= 32;

                    cout << "  Saldo atual: " << CurrentCoin;

                    WarriorTable(ArmamentsTable[2]);
                    
                } else {
                    cout << "  — Você não tem dinheiro o suficiente\n";
                }   
            } else {
                if(CurrentCoin >= 28){
                    cout << "  Compra realizada com sucesso!\n";

                    CurrentCoin -= 28;

                    cout << "  Saldo atual: " << CurrentCoin;
                    WarriorTable(ArmamentsTable[3]);

                } else {
                    cout << "  Você não tem dinheiro o suficiente\n";
                }
            }
            cout << "  — Você quer ver os meus outros itens?\n";
            cout << "  <Y/n>";
            cin << continueBuy;
            if(continueBuy == "y" || continueBuy == "Y"){
                cout << "  > digite A para a área do Arqueiro\n  > digite M para a área do Mago\n";
                cin >> buyOther;
            
                (buyOther == 'A' || buyOther == 'a') ? ArrowBuy(); : MageBuy();
            } else return;
        }

        void ArrowBuy(){
            cout << "  — Eu tenho algumas Facas de Aprendiz e ums Arma Velha.\n";
            cout << "  Saldo atual: " << CurrentCoin << "\n";
            cout << "  [1] - Faca de Aprendiz ————————————— C$38.00\n";
            cout << "  [2] - Arma Velha        ———————————— C$25.00\n";

            cin >> buy;

            if(buy == '1'){
                if(CurrentCoin >= 38){
                    cout << "  Compra realizada com sucesso!\n";

                    CurrentCoin -= 38;

                    cout << "  Saldo atual: " << CurrentCoin;

                    HunterTable(ArmamentsTable[2]);

                } else {
                    cout << "  — Você não tem dinheiro o suficiente\n";
                }   
            } else {
                if(CurrentCoin >= 25){
                    cout << "  Compra realizada com sucesso!\n";

                    CurrentCoin -= 25;

                    cout << "  Saldo atual: " << CurrentCoin;

                    HunterTable(ArmamentsTable[3]);
                } else {
                    cout << "  Você não tem dinheiro o suficiente\n";
                }
            }
            cout << "  — Você quer ver os meus outros itens?\n";
            cout << "  <Y/n>";
            cin << continueBuy;
            if(continueBuy == "y" || continueBuy == "Y"){
                cout << "  > digite W para a área do Guerreiro\n  > digite M para a área do Mago\n";
                cin >> buyOther;
            
                if (buyOther == 'W' || buyOther == 'w') ? WarriorBuy(); : MageBuy();
            } else return;
        }

        void MageBuy(){
            cout << "  — Eu tenho algumas Facas de Aprendiz e ums Arma Velha.\n";
            cout << "  Saldo atual: " << CurrentCoin << "\n";
            cout << "  [1] - Tomo do Aprendiz ————————————— C$34.00\n";
            cout << "  [2] - Orb dos Ventos   ————————————— C$45.00\n";

            cin >> buy;

            if(buy == '1'){
                if(CurrentCoin >= 34){
                    cout << "  Compra realizada com sucesso!\n";

                    CurrentCoin -= 34;

                    cout << "  Saldo atual: " << CurrentCoin;
                    
                    WizardTable(ArmamenstTable[2]);
                } else {
                    cout << "  — Você não tem dinheiro o suficiente\n";
                }   
            } else {
                if(CurrentCoin >= 45){
                    cout << "  Compra realizada com sucesso!\n";

                    CurrentCoin -= 45;

                    cout << "  Saldo atual: " << CurrentCoin;

                    WizardTable(ArmamenstTable[3]);
                } else {
                    cout << "  Você não tem dinheiro o suficiente\n";
                }
            }
            cout << "  — Você quer ver os meus outros itens?\n";
            cout << "  <Y/n>";
            cin << continueBuy;
            if(continueBuy == "y" || continueBuy == "Y"){
                cout << "  > digite W para a área do Guerreiro\n  > digite A para a área do Arqueiro\n";
                cin >> buyOther;
                
                (buyOther == 'W' || buyOther == 'w') ? WarriorBuy(); : ArrowBuy();
            } else return;
        }

};