#pragma once
#include <iostream>
#include "library/assets/NPCs/NPCs-City/Merchanters.hpp"

using namespace std;

/* ═══════════════════════════════════════════════════
   Shop — onde o jogador pode comprar e vender itens
   (ainda em desenvolvimento)
═══════════════════════════════════════════════════ */

class Buy {
    private:
       void HeavyWeapons(){
          cout << "  > Você escolheu comprar armas pesadas!\n";
          cout << "  > Pressione ENTER para continuar...\n";
          cin.ignore();
       }

       void LightWeapons(){
          cout << "  > Você escolheu comprar armas leves!\n";
          cout << "  > Pressione ENTER para continuar...\n";
          cin.ignore();
       }

       void MagicWeapons(){
          cout << "  > Você escolheu comprar armas mágicas!\n";
          cout << "  > Pressione ENTER para continuar...\n";
          cin.ignore();
       }

       void HeavyArmors(){
          cout << "  > Você escolheu comprar armaduras pesadas!\n";
          cout << "  > Pressione ENTER para continuar...\n";
          cin.ignore();
       }

       void LightArmors(){
          cout << "  > Você escolheu comprar armaduras leves!\n";
          cout << "  > Pressione ENTER para continuar...\n";
          cin.ignore();
       }

       void MagicArmors(){
          cout << "  > Você escolheu comprar armaduras mágicas!\n";
          cout << "  > Pressione ENTER para continuar...\n";
          cin.ignore();
       }

       void HealthPotions(){
          cout << "  > Você escolheu comprar poções de vida!\n";
          cout << "  > Pressione ENTER para continuar...\n";
          cin.ignore();
       }

       void ManaPotions(){
          cout << "  > Você escolheu comprar poções de mana!\n";
          cout << "  > Pressione ENTER para continuar...\n";
          cin.ignore();
       }

       void StrengthPotions(){
          cout << "  > Você escolheu comprar poções de força!\n";
          cout << "  > Pressione ENTER para continuar...\n";
          cin.ignore();
       }

    public:
       void buyWeapons(){
          cout << "  Qual categoria você quer ? \n"
               << "  [1] Armas Pesadas\n"
               << "  [2] Armas Leves\n"
               << "  [3] Armas Magicas\n"
               << "  [4] Voltar \n" 
               << "  > ";
          int op;
          cin >> op;
          cin.ignore();

          switch (op) {
             case 1:
                HeavyWeapons();
                break;
             case 2:
                LightWeapons();
                break;
             case 3:
                MagicWeapons();
                break;
             case 4:
                break;
          }
       }

       void buyArmors(){
          cout << "  Qual categoria você quer ? \n"
               << "  [1] Armaduras Pesadas\n"
               << "  [2] Armaduras Leves\n"
               << "  [3] Armaduras Magicas\n"
               << "  [4] Voltar \n" 
               << "  > ";
          int op;
          cin >> op;
          cin.ignore();

          switch (op) {
             case 1:
                HeavyArmors();
                break;
             case 2:
                LightArmors();
                break;
             case 3:
                MagicArmors();
                break;
             case 4:
                break;
          }
       }

       void buyPotions(){
          cout << "  Qual categoria você quer ? \n"
               << "  [1] Poções de Vida\n"
               << "  [2] Poções de Mana\n"
               << "  [3] Poções de Força\n"
               << "  [4] Voltar \n" 
               << "  > ";
          int op;
          cin >> op;
          cin.ignore();

          switch (op) {
             case 1:
                HealthPotions();
                break;
             case 2:
                ManaPotions();
                break;
             case 3:
                StrengthPotions();
                break;
             case 4:
                break;
          }
       }
};

class Shop : public Buy{
public:
    bool initShop() {
       genName();
       cout  << "  > Bem-vindo à minha loja! Meu nome é "
             << name << " e eu vendo os melhores itens da região!\n";
       cout << "  > Pressione ENTER para continuar...\n";
       cin.ignore();
       return true;
    }

    void setShop(){
       cout << "  [1] Comprar\n"
            << "  [2] Vender\n"
            << "  [3] Sair\n"
            << "  > ";
       int op;
       cin >> op;
       cin.ignore();

       if(op == 1) return setBuy();
       if(op == 2) return setSell();
       if(op == 3) return;  
    }

    void setBuy(){
       cout << "  O que você deseja comprar ? \n"
            << "  [1] Armas\n"
            << "  [2] Armaduras\n"
            << "  [3] Poções\n"
            << "  [4] Voltar \n" 
            << "  > ";
       int op;
       cin >> op;
       cin.ignore();

       if(op == 1) return Buy::buyWeapons();
       if(op == 2) return Buy::buyArmors();
       if(op == 3) return Buy::buyPotions();
       if(op == 4) return setShop();
    }

    void setSell(){
       cout << "  O que você quer vender? \n";
       cout << "  > Pressione ENTER para continuar...\n";
       cin.ignore();
    }
};
