#pragma once

#include "library/Stats/Stats_point.hpp"
/*Armaduras do Guerreiro*/


enum class Warrior_armorID{
    NONE            = 0,
    SILVER_GARMENTS = 1,
    UNIFORM_VIKING  = 2,
    BEAR_SKINS      = 3
};

class Warrior_armor{

    private:
    struct dataArmor_War{
        Warrior_armorID     id;
        std::string         name;
        stats               VIG;
        stats               DEF;
        stats               VEL;
        stats               FOR;
        stats               INT;
    };

    public:

        dataArmor_War Current_Armor= {Warrior_armorID::NONE,  "NADA",  {0,  0,  0,  0,  0}};

        std::vector<dataArmor_War> Armor_warriorTable{
            /*          id                       nome              vig def vel for int*/
            {Warrior_armorID::NONE,             "NADA",             {0,  0,  0,  0,  0}},
            {Warrior_armorID::SILVER_GARMENTS,  "Vestes de Prata",  {2,  5,  0,  1,  0}},
            {Warrior_armorID::UNIFORM_VIKING,   "Uniforme Viking",  {5,  1,  0,  3,  0}},
            {Warrior_armorID::BEAR_SKINS,       "Pelos de Urso",    {2,  2,  0,  2,  0}}
        };

        bool equippedArmor(Warrior_armorID  wrAmo){
            long unsigned int Amodex = static_cast <int> (wrAmo);
            if(Amodex < 0|| Amodex >= Armor_warriorTable.size()) return false;

            dataArmor_War select = Armor_warriorTable[Amodex];

            Current_Armor = select;
            std::cout << "  Equipado: " << Current_Armor.name;
            return true;
    }
};


/*Armaduras do caçador*/


enum class Arrow_armorID{
    NONE            = 0,
    ASSASINS_KIT    = 1,
    HUNTER_COAT     = 2,
    NINJAS_GEDGETS   = 3
};

class Arrow_armor : public HP{

    private:
    struct dataArmor_Arr{
        Arrow_armorID       id;
        std::string         name;
        stats               VIG;
        stats               DEF;
        stats               VEL;
        stats               FOR;
        stats               INT;
       
    };

    public:

        dataArmor_Arr Current_Armor= {Arrow_armorID::NONE,  "NADA",  0,  0,  0,  0,  0};

        std::vector<dataArmor_Arr> Armor_arrowTable{
            /*          ID                   nome                  vig def vel for int*/
            {Arrow_armorID::NONE,             "NADA",               0,  0,  0,  0,  0},
            {Arrow_armorID::ASSASINS_KIT,     "Kit de Assasinos",   0,  1,  3,  1,  0},
            {Arrow_armorID::HUNTER_COAT,      "Casaco do Caçador",  1,  3,  5,  0,  0},
            {Arrow_armorID::NINJAS_GEDGETS,   "Tralhas Ninjas",     0,  1,  2,  1,  0}
        };

    bool equippedArmor(Arrow_armorID  aAmo){
        long unsigned int Amodex = static_cast <int> (aAmo);
        if(Amodex < 0||Amodex >= Armor_arrowTable.size()) return false;

        dataArmor_Arr select = Armor_arrowTable[Amodex];

        Current_Armor = select;
        std::cout << "  Equipado: " << Current_Armor.name;
            return true;
    }
};


/*Armaduras do Mago*/

enum class Wizard_armorID{
    NONE            = 0,
    WIZARD_ROBE     = 1,
    STARS_HAT       = 2,
    GLITTER_TUNIC   = 3
};

class Wizard_armor{

    private:
    struct dataArmor_Wz{
        Wizard_armorID      id;
        std::string         name;
        stats               VIG;
        stats               DEF;
        stats               VEL;
        stats               FOR;
        stats               INT;
    };
    public:

        dataArmor_Wz Current_Armor= {Wizard_armorID::NONE,  "NADA",  0,  0,  0,  0,  0};

        std::vector<dataArmor_Wz> Armor_wizardTable{
            /*        id                         nome                 vig def vel for int*/
            {Wizard_armorID::NONE,             "NADA",                 0,  0,  0,  0,  0},
            {Wizard_armorID::WIZARD_ROBE,      "Roupão do Bruxo",      1,  2,  0,  0,  3},
            {Wizard_armorID::STARS_HAT,        "Chapéu das Estrelas",  0,  1,  0,  0,  2},
            {Wizard_armorID::GLITTER_TUNIC,    "Tunica de Glitter",    2,  2,  0,  0,  4}
        };

        bool equippedArmor(Wizard_armorID  wzAmo){
            long unsigned int Amodex = static_cast <int> (wzAmo);
            if(Amodex < 0 || Amodex >= Armor_wizardTable.size()) return false;

            dataArmor_Wz select = Armor_wizardTable[Amodex];

            Current_Armor = select;
            std::cout << "  Equipado: " << Current_Armor.name;
            return true;
    }
};

/*Armaduras do clerigo*/

enum class Cleric_armorID{
    NONE            = 0,
    CHAIN_MAIL      = 1,
    SCALE_MAIL      = 2,
    STUDDED_LEATHER = 3
};

class Cleric_armor {

    private: 
        struct dataArmor_Cl{
        Cleric_armorID      id;
        std::string         name;
        stats               VIG;
        stats               DEF;
        stats               VEL;
        stats               FOR;
        stats               INT;
        };
    public:
        dataArmor_Cl Current_Armor= {Cleric_armorID::NONE,    "NADA",  0,  0,  0,  0,  0};

        std::vector<dataArmor_Cl> Armor_clericTable{
            /*      id                             nome            vig def vel for int*/
            {Cleric_armorID::NONE,              "NADA",             0,  0,  0,  0,  0},
            {Cleric_armorID::CHAIN_MAIL,        "Cota de Malha",    2,  0,  0,  0,  1},
            {Cleric_armorID::SCALE_MAIL,        "Cota de Escamas",  3,  0,  0,  0,  2},
            {Cleric_armorID::STUDDED_LEATHER,   "Couro Batido",     5,  2,  0,  0,  0},
        };
        bool equippedArmor(Cleric_armorID  clAmo){
            long unsigned int Amodex =  static_cast <int> (clAmo);
            if(Amodex < 0 || Amodex >= Armor_clericTable.size()) return false;

            dataArmor_Cl select = Armor_clericTable[Amodex];

            Current_Armor = select;
            std::cout << "  Equipado: " << Current_Armor.name;
            return true;
    }
};