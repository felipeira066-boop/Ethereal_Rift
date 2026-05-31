#pragma once
#include <iostream>
#include "library/assets/NPCs/NPCs-City/Merchanters.hpp"
#include "library/character/Classes/allClasses.hpp"

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
         // cin.ignore(); // Removed unnecessary ignore to avoid confusing the user
         cout << "  > As armas pesadas são ideais para os guerreiros que buscam causar danos massivos em seus inimigos.\n";
         cin.ignore();
         cout << "  > Elas são mais lentas, mas cada golpe é devastador, capaz de derrubar até os adversários mais resistentes.\n";
         cin.ignore();
         cout << "  > Se você valoriza o poder bruto e não se importa em sacrificar um pouco de velocidade, as armas pesadas são a escolha perfeita para sua jornada.\n";
         cin.ignore();
         cout << "  > Prepare-se para esmagar seus inimigos com força implacável e deixar um rastro de destruição por onde passar!\n";
         cin.ignore();
         cout << "  > Você tem " << CurrentCoin << " moedas de prata.\n"
              << "  > Qual arma pesada você deseja comprar?\n"
              << "  > Lembre-se: cada arma tem um preço diferente, então escolha com cuidado!\n"
              << "==================== Guerreiro ====================\n"
              << "  [1] Lamina de Grama     (FOR: +2 | VEL: +1 )           - 50 moedas\n"
              << "  [2] Machado de Madeira  (FOR: +2 | DEF: +2 )           - 75 moedas\n"
              << "  [3] Lança               (FOR: +4 | DEF: +1 | VEL: +2 ) - 120 moedas\n"
              << "==================== Clerico ====================\n"
              << "  [4] Maça                (DEF: +4 | VIG: +2 )           - 70 moedas\n"
              << "  [5] Martelo de Guerra   (FOR: +2 | DEF: +3 | VIG: +2)  - 90 moedas\n"
              << "  [6] Mangual             (FOR: +1 | DEF: +2 | VIG: +3)  - 150 moedas\n"
              << "  [7] Voltar\n"
              << "  > ";
         cin >> op;
         Warrior guerreiro;
         Cleric clerico;
         switch (op) {
            case 1:
               if(CurrentCoin > 50){
                  if(guerreiro.equipar(warrior_ArmamentID::GRASS_BLADE) == false){
                     cout << "  > Você não é um guerreiro\n";
                     break;
                  }
                  lostCoin(50);
                  cout << "  > Você comprou a Lamina de Grama!\n";
                  guerreiro.equipar(warrior_ArmamentID::GRASS_BLADE);
                  return;
               } else{
                  cout << "  > Você não tem moedas suficientes para comprar essa arma.\n";
                  cin.ignore();
                  buyWeapons();
               }
               break;
            case 2:
               if(CurrentCoin > 75){
                  if(guerreiro.equipar(warrior_ArmamentID::WOOD_AXE) == false){ 
                     cout << "  > Você não é um guerreiro\n"; 
                     break;
                  }
                  lostCoin(75);
                  cout << "  > Você comprou o Machado de Madeira!\n";
                  guerreiro.equipar(warrior_ArmamentID::WOOD_AXE);
                  return;
               } else{
                  cout << "  > Você não tem moedas suficientes para comprar essa arma.\n";
                  cin.ignore();
                  buyWeapons();
               }
               break;
            case 3:
               if(CurrentCoin > 120){
                  if(guerreiro.equipar(warrior_ArmamentID::SPEAR) == false) {
                     cout << "  > Você não é um guerreiro\n";  
                     break;
                  }
                  lostCoin(120);
                  cout << "  > Você comprou a Lança!\n";
                  guerreiro.equipar(warrior_ArmamentID::SPEAR);
                  return;
               } else{
                  cout << "  > Você não tem moedas suficientes para comprar essa arma.\n";
                  cin.ignore();
                  buyWeapons();
               }
               break;
            case 4:
               if(CurrentCoin > 70){
                  if(clerico.equipar(cleric_ArmamentID::MACE) == false) {
                     cout << "  > Você não é um Clérico\n"; 
                     break;
                  }
                  lostCoin(70);
                  cout << "  > Você comprou a Maça\n";
                  clerico.equipar(cleric_ArmamentID::MACE);
                  return;
               }
               else{
                  cout << "  > Você não tem moedas suficientes para comprar essa arma.\n";
                  cin.ignore();
                  buyWeapons();
               }
               break;
            case 5:
               if(CurrentCoin > 90){
                  if(clerico.equipar(cleric_ArmamentID::WAR_HAMMER) == false) {
                     cout << "  > Você não é um Clérico\n"; 
                     break;
                  }
                  lostCoin(90);
                  cout << "  > Você comprou um Martelo de Guerra\n";
                  clerico.equipar(cleric_ArmamentID::WAR_HAMMER);
                  return;
               } else{
                  cout << " você não tem moedas o suficiente para comprar essa arma\n";
                  cin.ignore();
                  buyWeapons();
               }
               break;
            
            case 6:
               if(CurrentCoin > 150){
                  if(clerico.equipar(cleric_ArmamentID::FLAIL) == false) {
                     cout << "  > Você não é um Clérico\n"; 
                     break;
                  }
                  lostCoin(150);
                  cout << "  > Você comprou um Mangual\n";
                  clerico.equipar(cleric_ArmamentID::FLAIL);
                  return;
               } else{
                  cout << " você não tem moedas o suficiente para comprar essa arma\n";
                  cin.ignore();
                  buyWeapons();
               }
               break;
            default:
               buyWeapons();
               break;
         }

      }

      void LightWeapons(){
         cout << "  > Você escolheu comprar armas leves.\n";
         cin.ignore();
         cout << "  > As armas leves são perfeitas para os aventureiros que \n";
         cin.ignore();
         cout << "  > Valorizam a velocidade e a agilidade em combate.\n";
         cin.ignore();
         cout << "  > Elas permitem ataques rápidos e precisos, \n";
         cin.ignore();
         cout << "  > Ideal para aqueles que preferem esquivar e contra-atacar com destreza.\n";
         cin.ignore(); 
         cout << "  > Se você busca uma abordagem mais ágil e estratégica,\n";
         cin.ignore();
         cout << "  > As armas leves são a escolha certa para sua jornada.\n";
         cin.ignore();
         cout << "  > Prepare-se para desferir golpes rápidos e mortais, \n";
         cin.ignore();
         cout << "  > Deixando seus inimigos atordoados e incapazes de acompanhar seu ritmo frenético!\n";
         cin.ignore();
         cout << "  > Você tem " << CurrentCoin << " moedas de prata.\n"
              << "  > Qual arma leve você deseja comprar?\n"
              << "  > Lembre-se: cada arma tem um preço diferente, então escolha com cuidado!\n"
              << "  [1] Arco de Madeira      (FOR: +2 | VEL: +2 )           - 40 moedas\n"
              << "  [2] Facas do Aprendiz    (FOR: +1 | VEL: +5 )           - 60 moedas\n"
              << "  [3] Arma Velha           (FOR: +3 | VEL: +3 )           - 90 moedas\n"
              << "  [4] Voltar\n"
              << "  > ";
         cin >> op;
         Hunter cacador;
         switch (op) {
            case 1:
               if(CurrentCoin > 40){
                  if(cacador.equipar(hunter_ArmamentID::WOOD_BOW) == false) {
                     cout << "  > Você não é um caçador"; 
                     break;
                  }
                  lostCoin(40);
                  cout << "  > Você comprou o Arco de Madeira!\n";
                  cacador.equipar(hunter_ArmamentID::WOOD_BOW);
                  return;
               } else{
                  cout << "  > Você não tem moedas suficientes para comprar essa arma.\n";
                  cin.ignore();
                  buyWeapons();
               }
               break;
            case 2:
               if(CurrentCoin > 60){
                  if(cacador.equipar(hunter_ArmamentID::WOOD_BOW) == false) {
                     cout << "  > Você não é um caçador"; 
                     break;
                  }
                  lostCoin(60);
                  cout << "  > Você comprou as Facas do Aprendiz!\n";
                  cacador.equipar(hunter_ArmamentID::APRENTICE_KNIVES);
                  return;
               } else{
                  cout << "  > Você não tem moedas suficientes para comprar essa arma.\n";
                  cin.ignore();
                  buyWeapons();
               }
               break;
            case 3:
               if(CurrentCoin > 90){
                  if(cacador.equipar(hunter_ArmamentID::WOOD_BOW) == false) {
                     cout << "  > Você não é um caçador"; 
                     break;
                  }
                  lostCoin(90);
                  cout << "  > Você comprou o Chicote!\n";
                  cacador.equipar(hunter_ArmamentID::OLD_GUN);
                  return;
               } else{
                  cout << "  > Você não tem moedas suficientes para comprar essa arma.\n";
                  cin.ignore();
                  buyWeapons();
               }
               break;
            default:
               buyWeapons();
               break;
         }

      }

      void MagicWeapons(){
         cout << "  > Você escolheu comprar armas mágicas.\n";
         cin.ignore();
         cout << "  > As armas mágicas são a escolha ideal para os magos\n";
         cin.ignore();
         cout << "  > Elas canalizam o poder arcano, permitindo que você lance feitiços\n";
         cin.ignore();
         cout << "  > E habilidades mágicas devastadoras contra seus inimigos.\n";
         cin.ignore();
         cout << "  > Se você busca dominar as artes místicas e causar danos mágicos massivos,\n";
         cin.ignore();
         cout << "  > As armas mágicas são a escolha perfeita para sua jornada.\n";
         cin.ignore();
         cout << "  > Prepare-se para desencadear feitiços poderosos e habilidades mágicas incríveis,\n";
         cin.ignore();
         cout << "  > Deixando seus inimigos em ruínas e estabelecendo seu domínio sobre as forças arcanas!\n";
         cin.ignore();
         cout << "  > Você tem " << CurrentCoin << " moedas de prata.\n"
              << "  > Qual arma mágica você deseja comprar?\n"
              << "  > Lembre-se: cada arma tem um preço diferente, então escolha com cuidado!\n"
              << "  [1] Cajado de Madeira    ( DEF: +1 | INT: +2 )           - 45 moedas\n"
              << "  [2] Tomo do Bruxo        ( VIG: +1 | INT: +4 )           - 80 moedas\n"
              << "  [3] Orb dos Ventos       ( DEF: +2 | VEL: +1 | INT: +5 ) - 130 moedas\n"
              << "  [4] Voltar\n"
              << "  > ";
         cin >> op;
         Mage mago;
         switch(op){
            case 1:
               if(CurrentCoin > 45){
                  if(mago.equipar(mage_ArmamentID::WOOD_STAFF) == false) {
                     cout << "  > Você não é um mago\n"; 
                     break;
                  }
                  lostCoin(45);
                  cout << "  > Você comprou o Cajado de Madeira!\n";
                  mago.equipar(mage_ArmamentID::WOOD_STAFF);
                  return;
               } else{
                  cout << "  > Você não tem moedas suficientes para comprar essa arma.\n";
                  cin.ignore();
                  buyWeapons();
               }
               break;
            case 2:
               if(CurrentCoin > 80){
                  if(mago.equipar(mage_ArmamentID::mage_TOME) == false) {
                     cout << "  > Você não é um mago\n"; 
                     break;
                  }
                  lostCoin(80);
                  cout << "  > Você comprou o Tomo do Bruxo!\n";
                  mago.equipar(mage_ArmamentID::mage_TOME);
                  return;
               } else{
                  cout << "  > Você não tem moedas suficientes para comprar essa arma.\n";
                  cin.ignore();
                  buyWeapons();
               }
               break;
            case 3:
               if(CurrentCoin > 130){
                  if(mago.equipar(mage_ArmamentID::WIND_ORB) == false) {
                     cout << "  > Você não é um mago\n"; 
                     break;
                  };
                  lostCoin(130);
                  cout << "  > Você comprou a Orb dos Ventos!\n";
                  mago.equipar(mage_ArmamentID::WIND_ORB);
                  return;
               } else{
                  cout << "  > Você não tem moedas suficientes para comprar essa arma.\n";
                  cin.ignore();
                  buyWeapons();
               }
               break;
            default:
               buyWeapons();
               break;
         }
      }

      void HeavyArmors(){
         cout << "  > Você escolheu comprar armaduras pesadas.\n";
         cin.ignore();
         cout << "  > Armaduras pesadas são perfeitas para quem quer durar bastante na batalha\n";
         cin.ignore();
         cout << "  > Elas aumentam a sua defesa e te faz se sentir mais saudável e protegido\n";
         cin.ignore();
         cout << "  > Seus inimigos terão dificuldades para feri-lo\n";
         cin.ignore();
         cout << "  Eu tenho algumas armaduras que você possa se interessar\n";
         cin.ignore();
         cout << "  > Você tem " << CurrentCoin << " moedas de prata.\n"
              << "  > Qual armadura pesada você deseja comprar?\n"
              << "  > Lembre-se: cada armadura tem um preço diferente, então escolha com cuidado!\n"
              << "==================== Guerreiro ====================\n"
              << "  [1] Pele de Urso         (VIG: +2 | DEF: +2 | FOR: +2 )           - 30 moedas\n"
              << "  [2] Vestes de Prata      (VIG: +2 | DEF: +5 | FOR: +1 )           - 60 moedas\n"
              << "  [3] Uniforme Viking      (VIG: +5 | DEF: +1 | FOR: +3 )           - 130 moedas\n"
              << "==================== Clérico ====================\n"
              << "  [4] Cota de Malha        (VIG: +2 | INT: +1 )                     - 25 moedas\n"
              << "  [5] Cota de Escamas      (VIG: +3 | INT: +2 )                     - 50 moedas\n"
              << "  [5] Couro Batido         (VIG: +5 | DEF: +2 | INT: +1)            - 110 moedas\n"
              << "  [7] Voltar\n"
              << "  > ";
         cin.ignore();
         cin >> op;
         Warrior guerreiro;
         Cleric clerico;
         switch(op){
            case 1:
               if(CurrentCoin >= 30){
                  if(guerreiro.Armor(Warrior_armorID::BEAR_SKINS) == false) {
                     cout << "  > Você não é um guerreiro \n"; 
                     break;
                  }
                  lostCoin(30);
                  cout << "  > Você comprou uma Pele de Urso\n";
                  guerreiro.Armor(Warrior_armorID::BEAR_SKINS);
                  return;
               }
               else{
                  cout << "  > Você não tem moedas o suficiente\n";
                  cin.ignore();
                  buyArmors();
               }
               break;
            case 2:
               if(CurrentCoin >= 70){
                  if(guerreiro.Armor(Warrior_armorID::SILVER_GARMENTS) == false) {
                     cout << "  > Você não é um guerreiro \n"; 
                     break;
                  }
                  lostCoin(70);
                  cout << "  > Você comprou uma Vestes de Prata\n";
                  guerreiro.Armor(Warrior_armorID::SILVER_GARMENTS);
                  return;
               }
               else{
                  cout << "  > Você não tem moedas o suficiente\n";
                  cin.ignore();
                  buyArmors();
               }
               break;
            case 3:
               if(CurrentCoin >= 130){
                  if(guerreiro.Armor(Warrior_armorID::UNIFORM_VIKING) == false) {
                     cout << "  > Você não é um guerreiro \n"; 
                     break;
                  }
                  lostCoin(130);
                  cout << "  > Você comprou uma Uniforme Viking\n";
                  guerreiro.Armor(Warrior_armorID::UNIFORM_VIKING);
                  return;
               }
               else{
                  cout << "  > Você não tem moedas o suficiente\n";
                  cin.ignore();
                  buyArmors();
               }
               break;
            case 4:
               if(CurrentCoin >= 25){
                  if(clerico.Armor(Cleric_armorID::CHAIN_MAIL) == false) {
                     cout << "  > Você não é um clérigo \n"; 
                     break;
                  }
                  lostCoin(25);
                  cout << "  > Você comprou uma Cota de Malha\n";
                  clerico.Armor(Cleric_armorID::CHAIN_MAIL);
                  return;
               }
               else{
                  cout << "  > Você não tem moedas o suficiente\n";
                  cin.ignore();
                  buyArmors();
               }
               break;
            case 5:
               if(CurrentCoin >= 50){
                  if(clerico.Armor(Cleric_armorID::SCALE_MAIL) == false) {
                     cout << "  > Você não é um clérigo \n"; 
                     break;
                  }
                  lostCoin(50);
                  cout << "  > Você comprou uma Cota de Escamas\n";
                  clerico.Armor(Cleric_armorID::SCALE_MAIL);
                  return;
               }
               else{
                  cout << "  > Você não tem moedas o suficiente\n";
                  cin.ignore();
                  buyArmors();
               } 
               break;
            case 6:
               if(CurrentCoin >= 110){
                  if(clerico.Armor(Cleric_armorID::STUDDED_LEATHER) == false) {
                     cout << "  > Você não é um clérigo \n"; 
                     break;
                  }
                  lostCoin(110);
                  cout << "  > Você comprou uma Couro Batido\n";
                  clerico.Armor(Cleric_armorID::STUDDED_LEATHER);
                  return;
               }
               else{
                  cout << "  > Você não tem moedas o suficiente\n";
                  cin.ignore();
                  buyArmors();
               }
               break;
            default:
               buyArmors();
               break;
         }
      }

      void LightArmors(){
         cout << "  > Você escolheu comprar armaduras leves.\n";
         cin.ignore();
         cout << "  > Armaduras leves são perfeitas para quem valoriza velocidade e agilidade\n";
         cin.ignore();
         cout << "  > Elas permitem movimentos rápidos e precisos em combate\n";
         cin.ignore();
         cout << "  > Ideais para aqueles que preferem esquivar e contra-atacar com destreza\n";
         cin.ignore();
         cout << "  > Se você busca uma abordagem ágil e estratégica, as armaduras leves são perfeitas\n";
         cin.ignore();
         cout << "  > Prepare-se para dançar no campo de batalha com graça e velocidade!\n";
         cin.ignore();
         cout << "  > Você tem " << CurrentCoin << " moedas de prata.\n"
              << "  > Qual armadura leve você deseja comprar?\n"
              << "  > Lembre-se: cada armadura tem um preço diferente, então escolha com cuidado!\n"
              << "==================== Caçador ====================\n"
              << "  [1] Kit de Assasinos     (VEL: +3 | DEF: +1 | FOR: +1 )     - 35 moedas\n"
              << "  [2] Casaco do Caçador    (VIG: +1 | DEF: +3 | VEL: +5 )     - 75 moedas\n"
              << "  [3] Tralhas Ninjas       (VEL: +2 | DEF: +1 | FOR: +1 )     - 55 moedas\n"
              << "  [4] Voltar\n"
              << "  > ";
         cin.ignore();
         cin >> op;
         Hunter cacador;
         switch(op){
            case 1:
               if(CurrentCoin >= 35){
                  if(cacador.Armor(Arrow_armorID::ASSASINS_KIT) == false) {
                     cout << "  > Você não é um caçador \n"; 
                     break;
                  }
                  lostCoin(35);
                  cout << "  > Você comprou um Kit de Assasinos\n";
                  cacador.Armor(Arrow_armorID::ASSASINS_KIT);
                  return;
               }
               else{
                  cout << "  > Você não tem moedas o suficiente\n";
                  cin.ignore();
                  buyArmors();
               }
               break;
            case 2:
               if(CurrentCoin >= 75){
                  if(cacador.Armor(Arrow_armorID::HUNTER_COAT) == false) {
                     cout << "  > Você não é um caçador \n"; 
                     break;
                  }
                  lostCoin(75);
                  cout << "  > Você comprou um Casaco do Caçador\n";
                  cacador.Armor(Arrow_armorID::HUNTER_COAT);
                  return;
               }
               else{
                  cout << "  > Você não tem moedas o suficiente\n";
                  cin.ignore();
                  buyArmors();
               }
               break;
            case 3:
               if(CurrentCoin >= 55){
                  if(cacador.Armor(Arrow_armorID::NINJAS_GEDGETS) == false) {
                     cout << "  > Você não é um caçador \n"; 
                     break;
                  }
                  lostCoin(55);
                  cout << "  > Você comprou Tralhas Ninjas\n";
                  cacador.Armor(Arrow_armorID::NINJAS_GEDGETS);
                  return;
               }
               else{
                  cout << "  > Você não tem moedas o suficiente\n";
                  cin.ignore();
                  buyArmors();
               }
               break;
            default:
               buyArmors();
               break;
         }
      }

      void MagicArmors(){
         cout << "  > Você escolheu comprar armaduras mágicas.\n";
         cin.ignore();
         cout << "  > Armaduras mágicas canalizam o poder arcano\n";
         cin.ignore();
         cout << "  > Elas aumentam sua inteligência e potência mágica\n";
         cin.ignore();
         cout << "  > Perfeitas para magos que buscam dominar as artes místicas\n";
         cin.ignore();
         cout << "  > Se você quer aumentar seu poder mágico, as armaduras mágicas são a escolha certa\n";
         cin.ignore();
         cout << "  > Prepare-se para desencadear feitiços poderosos com potência arcana!\n";
         cin.ignore();
         cout << "  > Você tem " << CurrentCoin << " moedas de prata.\n"
              << "  > Qual armadura mágica você deseja comprar?\n"
              << "  > Lembre-se: cada armadura tem um preço diferente, então escolha com cuidado!\n"
              << "==================== Mago ====================\n"
              << "  [1] Roupão do Bruxo      (VIG: +1 | DEF: +2 | INT: +3 )     - 65 moedas\n"
              << "  [2] Chapéu das Estrelas  (DEF: +1 | INT: +2 )              - 40 moedas\n"
              << "  [3] Túnica de Glitter    (VIG: +2 | DEF: +2 | INT: +4 )    - 100 moedas\n"
              << "  [4] Voltar\n"
              << "  > ";
         cin.ignore();
         cin >> op;
         Mage mago;
         switch(op){
            case 1:
               if(CurrentCoin >= 65){
                  if(mago.Armor(Wizard_armorID::WIZARD_ROBE) == false) {
                     cout << "  > Você não é um mago \n"; 
                     break;
                  }
                  lostCoin(65);
                  cout << "  > Você comprou um Roupão do Bruxo\n";
                  mago.Armor(Wizard_armorID::WIZARD_ROBE);
                  return;
               }
               else{
                  cout << "  > Você não tem moedas o suficiente\n";
                  cin.ignore();
                  buyArmors();
               }
               break;
            case 2:
               if(CurrentCoin >= 40){
                  if(mago.Armor(Wizard_armorID::STARS_HAT) == false) {
                     cout << "  > Você não é um mago \n"; 
                     break;
                  }
                  lostCoin(40);
                  cout << "  > Você comprou um Chapéu das Estrelas\n";
                  mago.Armor(Wizard_armorID::STARS_HAT);
                  return;
               }
               else{
                  cout << "  > Você não tem moedas o suficiente\n";
                  cin.ignore();
                  buyArmors();
               }
               break;
            case 3:
               if(CurrentCoin >= 100){
                  if(mago.Armor(Wizard_armorID::GLITTER_TUNIC) == false) {
                     cout << "  > Você não é um mago \n"; 
                     break;
                  }
                  lostCoin(100);
                  cout << "  > Você comprou uma Túnica de Glitter\n";
                  mago.Armor(Wizard_armorID::GLITTER_TUNIC);
                  return;
               }
               else{
                  cout << "  > Você não tem moedas o suficiente\n";
                  cin.ignore();
                  buyArmors();
               }
               break;
            default:
               buyArmors();
               break;
         }
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
         cout << "  > As poções de mana são essenciais para os magos e\n";
         cin.ignore();
         cout << "  > Qualquer aventureiro que dependa de habilidades especiais.\n";
         cin.ignore();
         cout << "  > Elas restauram sua energia mágica, permitindo que você continue lançando feitiços poderosos\n";
         cin.ignore();
         cout << "  > Ou usando habilidades únicas mesmo nas batalhas mais intensas.\n";
         cin.ignore();
         cout << "  > Use-as com sabedoria, pois o estoque é limitado e os inimigos podem ser implacáveis.\n";
         cin.ignore();
         cout << "  > Boa sorte em sua aventura. Que as poções de mana sejam suas aliadas mais valiosas.\n";
         cin.ignore();
         cout << "  — Tenho algumas opções no estoque. Escolha a que mais ajuda você:\n";
         cin.ignore();
         cout << "  > Você tem " << CurrentCoin << " moedas de prata.\n"
              << "  > Qual poção de mana você deseja comprar?\n"
              << "  > Lembre-se: cada poção tem um preço diferente, então escolha com cuidado!\n"
              << "  [1] Poção de Mana Pequena (Restaura 10 Mana) - 10 moedas\n"
              << "  [2] Poção de Mana Média (Restaura 50 Mana) - 25 moedas\n"
              << "  [3] Poção de Mana Grande (Restaura 100 Mana) - 60 moedas\n"
              << "  [4] Voltar\n"
              << "  > ";
         cin >> op;

         switch (op) {
            case 1:
               if(CurrentCoin > 10 && setPotion(PotionID::MANA_P) == true){
                  lostCoin(10);
                  cout << "  > Você comprou uma Poção de Mana Pequena!\n";
                  bag.addPotion(PotionID::MANA_P);
               } else {
                  cout << "  > Você não tem moedas suficientes para comprar essa poção.\n";
                  cin.ignore();
               }
               break;
            case 2:
               if(CurrentCoin > 25 && setPotion(PotionID::MANA_M) == true){
                  lostCoin(25);
                  cout << "  > Você comprou uma Poção de Mana Média!\n";
                  bag.addPotion(PotionID::MANA_M);
               } else {
                  cout << "  > Você não tem moedas suficientes para comprar essa poção.\n";
                  cin.ignore();
               }
               break;
            case 3:
               if(CurrentCoin > 60 && setPotion(PotionID::MANA_G) == true){
                  lostCoin(60);
                  cout << "  > Você comprou uma Poção de Mana Grande!\n";
                  bag.addPotion(PotionID::MANA_G);
               } else {
                  cout << "  > Você não tem moedas suficientes para comprar essa poção.\n";
                  cin.ignore();
               }
               break;
            default:
               buyPotions();
               break;
         }

      }

      void StrengthPotions(){
         cout << "  > Você escolheu comprar poções de força.\n";
         cin.ignore();
         cout << "  > As poções de força são essenciais para os guerreiros e \n";
         cin.ignore();
         cout << "  > Qualquer aventureiro que dependa de ataques físicos poderosos.\n";
         cin.ignore();
         cout << "  > Elas aumentam temporariamente sua força, permitindo que você cause mais \n";
         cin.ignore();
         cout << "  > Dano em seus inimigos mesmo nas batalhas mais intensas.\n";
         cin.ignore();
         cout << "  > Use-as com sabedoria, pois o estoque é limitado e os inimigos podem ser implacáveis.\n";
         cin.ignore();
         cout << "  > Boa sorte em sua aventura. Que as poções de força sejam suas aliadas mais valiosas.\n";
         cin.ignore();
         cout << "  — Tenho algumas opções no estoque. Escolha a que mais ajuda você:\n";
         cin.ignore();
         cout << "  > Você tem " << CurrentCoin << " moedas de prata.\n"
              << "  > Qual poção de força você deseja comprar?\n"
              << "  > Lembre-se: cada poção tem um preço diferente, então escolha com cuidado!\n"
              << "  [1] Poção de Força Pequena (Aumenta 5 de Dano por 3 turnos) - 15 moedas\n"
              << "  [2] Poção de Força Média (Aumenta 10 de Dano por 5 turnos) - 35 moedas\n"
              << "  [3] Poção de Força Grande (Aumenta 20 de Dano por 7 turnos) - 70 moedas\n"
              << "  [4] Voltar\n"
              << "  > ";
         cin >> op;
         switch(op){
            case 1:
               if(CurrentCoin > 15 && setPotion(PotionID::STRENGTH_P) == true){
                  lostCoin(15);
                  cout << "  > Você comprou uma Poção de Força Pequena!\n";
                  bag.addPotion(PotionID::STRENGTH_P);
               } else {
                  cout << "  > Você não tem moedas suficientes para comprar essa poção.\n";
                  cin.ignore();
               }
               break;
            case 2:
               if(CurrentCoin > 35 && setPotion(PotionID::STRENGTH_M) == true){
                  lostCoin(35);
                  cout << "  > Você comprou uma Poção de Força Média!\n";
                  bag.addPotion(PotionID::STRENGTH_M);
               } else {
                  cout << "  > Você não tem moedas suficientes para comprar essa poção.\n";
                  cin.ignore();
               }
               break;
            case 3:
               if(CurrentCoin > 70 && setPotion(PotionID::STRENGTH_G) == true){
                  lostCoin(70);
                  cout << "  > Você comprou uma Poção de Força Grande!\n";
                  bag.addPotion(PotionID::STRENGTH_G);
               } else {
                  cout << "  > Você não tem moedas suficientes para comprar essa poção.\n";
                  cin.ignore();
               }
               break;
            case 4:
               buyPotions();
               break;
         }
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
