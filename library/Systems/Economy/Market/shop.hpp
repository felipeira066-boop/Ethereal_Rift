#pragma once
#include <iostream>
#include "library/assets/NPCs/NPCs-City/Merchanters.hpp"
#include "library/Systems/Itens/Effects.hpp"
#include "library/Systems/Economy/inventory/Bank.hpp"
#include "library/Systems/Economy/inventory/myBag.hpp"

using namespace std;

int op;
/* ═══════════════════════════════════════════════════
   Shop — onde o jogador pode comprar e vender itens
   (ainda em desenvolvimento)
═══════════════════════════════════════════════════ */
class Buy : protected setEffects{
   protected:

      Inventory& bag;

      void HeavyWeapons(){
         cout << "  > Você escolheu comprar armas pesadas.\n";
         cin.ignore();
      }

      void LightWeapons(){
         cout << "  > Você escolheu comprar armas leves.\n";
         cin.ignore();
      }

      void MagicWeapons(){
         cout << "  > Você escolheu comprar armas mágicas.\n";
         cin.ignore();
      }

      void HeavyArmors(){
         cout << "  > Você escolheu comprar armaduras pesadas.\n";
         cin.ignore();
      }

      void LightArmors(){
         cout << "  > Você escolheu comprar armaduras leves.\n";
         cin.ignore();
      }

      void MagicArmors(){
         cout << "  > Você escolheu comprar armaduras mágicas.\n";
         cin.ignore();
      }

      void HealthPotions(){
         cout << "  > Você escolheu comprar poções de vida.";
         cin.ignore();
         cout << "  > Essas poções restauram sua vida instantaneamente e são essenciais para sobreviver às batalhas mais difíceis.";
         cin.ignore();
         cout << "  > Use-as com sabedoria: o estoque é limitado e os inimigos podem ser implacáveis.";
         cin.ignore();
         cout << "  > Boa sorte em sua aventura. Que as poções de vida sejam suas aliadas mais valiosas.";
         cin.ignore();
         cout << "  — Tenho algumas opções no estoque. Escolha a que mais ajuda você:";
         cin.ignore();
         cout << "  > Você tem " << CurrentCoin << " moedas de prata.\n"
              << "  > Qual poção de vida você deseja comprar?\n"
              << "  > Lembre-se: cada poção tem um preço diferente, então escolha com cuidado!\n"
              << "  [1] Poção de Vida Pequena (Restaura 20 HP) - 10 moedas\n"
              << "  [2] Poção de Vida Média (Restaura 50 HP) - 25 moedas\n"
              << "  [3] Poção de Vida Grande (Restaura 100 HP) - 50 moedas\n"
              << "  [4] Voltar\n"
              << "  > ";
         cin >> op;

         if(op == 1 && setPotion(PotionID::HEALTH_P) == true) {
            if(CurrentCoin > 10){
               lostCoin(10);
               cout << "  > Você comprou uma Poção de Vida Pequena!\n";
               bag.addPotion(PotionID::HEALTH_P);
               return;
            }
            cout << "  > Você não tem moedas suficientes para comprar essa poção.\n";
            cin.ignore();
            
         }

         if(op == 2 && setPotion(PotionID::HEALTH_M) == true) {
            if(CurrentCoin > 25){
               lostCoin(25);
               cout << "  > Você comprou uma Poção de Vida Média!\n";
               bag.addPotion(PotionID::HEALTH_M);
               return;
            }
            cout << "  > Você não tem moedas suficientes para comprar essa poção.\n";
            cin.ignore();
         }

         if(op == 3 && setPotion(PotionID::HEALTH_G) == true ) {
            if(CurrentCoin > 50){
               lostCoin(50);
               cout << "  > Você comprou uma Poção de Vida Grande!";
               bag.addPotion(PotionID::HEALTH_G);
               return;
            } 
            cout << "  > Você não tem moedas suficientes para comprar essa poção.\n";
            cin.ignore();
         }

         buyPotions();
      }

      void ManaPotions(){
         cout << "  > Você escolheu comprar poções de mana.\n";
         cin.ignore();
      }

      void StrengthPotions(){
         cout << "  > Você escolheu comprar poções de força.\n";
         cin.ignore();
      }

   public:
      Buy(Inventory& inv) : bag(inv) {
      }

      void buyWeapons(){
         cout << "  Qual categoria você deseja?\n"
            << "  [1] Armas Pesadas\n"
            << "  [2] Armas Leves\n"
            << "  [3] Armas Mágicas\n"
            << "  [4] Voltar\n"
            << "  > ";
      
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
         cout << "  Qual categoria você deseja?\n"
            << "  [1] Armaduras Pesadas\n"
            << "  [2] Armaduras Leves\n"
            << "  [3] Armaduras Mágicas\n"
            << "  [4] Voltar\n"
            << "  > ";
      
         cin >> op;

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
         cout << "  Qual categoria você deseja?\n"
            << "  [1] Poções de Vida\n"
            << "  [2] Poções de Mana\n"
            << "  [3] Poções de Força\n"
            << "  [4] Voltar\n"
            << "  > ";
      
         cin >> op;

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
   Shop(Inventory& playerInv) : Buy(playerInv) {}

   bool initShop() {
      cout  << "  > Bem-vindo à loja. Meu nome é "
            << genName() << " e tenho os melhores itens da região.\n";
      cin.ignore();
      setShop();

      return true;
   }

   bool setShop(){
      cout << "  [1] Comprar\n"
           << "  [2] Vender\n"
           << "  [3] Sair\n"
         << "  > ";
   
      cin >> op;

      if(op == 1) setBuy();
      if(op == 2) setSell();
      if(op == 3) return false;

      return true;
   }

   void setBuy(){
      cout << "  O que você deseja comprar?\n"
         << "  [1] Armas\n"
         << "  [2] Armaduras\n"
         << "  [3] Poções\n"
         << "  [4] Voltar\n"
         << "  > ";
   
      cin >> op;

      if(op == 1) buyWeapons();  
      if(op == 2) buyArmors();
      if(op == 3) buyPotions();
      if(op == 4) setShop();
   }

   void setSell(){
      cout << "  O que você deseja vender?\n"
           << "  > ";
      cin.ignore();
   }
};
