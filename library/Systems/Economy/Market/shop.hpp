#pragma once
#include <iostream>
#include "library/assets/NPCs/NPCs-City/Merchanters.hpp"
#include "library/Systems/Itens/Effects.hpp"

using namespace std;

int op;
/* ═══════════════════════════════════════════════════
   Shop — onde o jogador pode comprar e vender itens
   (ainda em desenvolvimento)
═══════════════════════════════════════════════════ */
class Buy : protected setEffects{
   private:
      void HeavyWeapons(){
         cout << "  > Você escolheu comprar armas pesadas!\n";
         cin.ignore();
      }

      void LightWeapons(){
         cout << "  > Você escolheu comprar armas leves!\n";
         cin.ignore();
      }

      void MagicWeapons(){
         cout << "  > Você escolheu comprar armas mágicas!\n";
         cin.ignore();
      }

      void HeavyArmors(){
         cout << "  > Você escolheu comprar armaduras pesadas!\n";
         cin.ignore();
      }

      void LightArmors(){
         cout << "  > Você escolheu comprar armaduras leves!\n";
         cin.ignore();
      }

      void MagicArmors(){
         cout << "  > Você escolheu comprar armaduras mágicas!\n";
         cin.ignore();
      }

      void HealthPotions(){
         cout << "  > Você escolheu comprar poções de vida!\n";
         cin.get();
         cout << "  > Essas poções restauram sua vida instantaneamente. Elas são essenciais para sobreviver às batalhas mais difíceis!\n";
         cin.get();
         cout << "  > Lembre-se de usá-las com sabedoria, pois o estoque é limitado e os inimigos podem ser implacáveis!\n";
         cin.get();
         cout << "  > Enfim, boa sorte em suas aventuras, e que as poções de vida sejam suas aliadas mais valiosas!\n";
         cin.get();
         cout << "  Eu tenho algumas poções de vida no estoque, veja se você quer alguma delas: \n";
         cin.ignore();
         cout << "  [1] Poção de Vida Pequena (Restaura 20 HP) - 10 moedas\n"
              << "  [2] Poção de Vida Média (Restaura 50 HP) - 25 moedas\n"
              << "  [3] Poção de Vida Grande (Restaura 100 HP) - 50 moedas\n"
              << "  [4] Voltar \n" 
              << "  > ";

         cin >> op;

         if(op == 1) {
            setPotion(PotionID::HEALTH_P);

            cout << "  > Você comprou uma Poção de Vida Pequena!\n";
            cin.ignore();
         }

         if(op == 2) {
            setPotion(PotionID::HEALTH_M);
            cout << "  > Você comprou uma Poção de Vida Média!\n";
            cin.ignore();
         }

         if(op == 3) {
            setPotion(PotionID::HEALTH_G);

            cout << "  > Você comprou uma Poção de Vida Grande!\n";
            cin.ignore();
         }

         buyPotions();
      }

      void ManaPotions(){
         cout << "  > Você escolheu comprar poções de mana!\n";
         cin.ignore();
      }

      void StrengthPotions(){
         cout << "  > Você escolheu comprar poções de força!\n";
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
         cout << "  Qual categoria você quer ? \n"
            << "  [1] Poções de Vida\n"
            << "  [2] Poções de Mana\n"
            << "  [3] Poções de Força\n"
            << "  [4] Voltar \n" 
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
   bool initShop() {
      cout  << "  > Bem-vindo à minha loja! Meu nome é "
            << genName() << " e eu vendo os melhores itens da região!\n";
      cin.get();
      setBuy();

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
      Buy buy;

      cout << "  O que você deseja comprar ? \n"
         << "  [1] Armas\n"
         << "  [2] Armaduras\n"
         << "  [3] Poções\n"
         << "  [4] Voltar \n" 
         << "  > ";
   
      cin >> op;

      if(op == 1) buy.buyWeapons();
      if(op == 2) buy.buyArmors();
      if(op == 3) buy.buyPotions();
      if(op == 4) setShop();
   }

   void setSell(){
      cout << "  O que você quer vender? \n";
      cin.ignore();
   }
};
