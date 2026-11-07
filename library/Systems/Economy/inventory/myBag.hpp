#pragma once

#include "../../Itens/Effects.hpp"
#include <iostream>
#include <map>
#include <iomanip>
#include <set>

class Inventory : virtual public setEffects {
private:
    // Map que armazena: chave = ID (combinado), valor = quantidade
    std::map<std::string, int> consumables;  // "consumable_1", "consumable_2", etc
    std::map<std::string, int> potions;      // "potion_1", "potion_2", etc
    std::set<int> weapons;
    std::set<int> armors;
    
    int maxCapacity;
    
    // Gera uma chave única para identificar item
    std::string getConsumableKey(ConsumableID id) const {
        return "c_" + std::to_string(static_cast<int>(id));
    }
    
    std::string getPotionKey(PotionID id) const {
        return "p_" + std::to_string(static_cast<int>(id));
    }

public:
    Inventory(int capacity = 50) : maxCapacity(capacity) {}

    bool addWeapon(int id) {
        return id >= 1 && id <= 3 && this->weapons.insert(id).second;
    }

    bool addArmor(int id) {
        return id >= 1 && id <= 3 && this->armors.insert(id).second;
    }

    bool hasWeapon(int id) const {
        return this->weapons.find(id) != this->weapons.end();
    }

    bool hasArmor(int id) const {
        return this->armors.find(id) != this->armors.end();
    }

    // ========== ADICIONAR ITEMS ==========
    bool addConsumable(ConsumableID id, int quantity = 1) {
        if (!setConsumable(id) || id == ConsumableID::NONE) {
            std::cout << "  Invalid consumable item!" << std::endl;
            return false;
        }

        std::string key = getConsumableKey(id);
        int currentQty = consumables[key];
        
        if (currentQty + quantity > maxCapacity) {
            std::cout << "  Inventory is full!" << std::endl;
            return false;
        }

        consumables[key] += quantity;
        std::cout << "  "<< quantity << "x " << getConsumableName(id) << " added!" << std::endl;
        return true;
    }

    bool addPotion(PotionID id, int quantity = 1) {
        if (!setPotion(id)) {
            std::cout << "  Invalid potion!" << std::endl;
            return false;
        }

        std::string key = getPotionKey(id);
        int currentQty = potions[key];
        
        if (currentQty + quantity > maxCapacity) {
            std::cout << "  Inventory is full!" << std::endl;
            return false;
        }

        potions[key] += quantity;
        std::cout << "  " << quantity << "x " << getPotionName(id) << " added!" << std::endl;
        return true;
    }

    // ========== REMOVER ITEMS ==========
    bool removeConsumable(ConsumableID id, int quantity = 1) {
        std::string key = getConsumableKey(id);
        
        if (consumables.find(key) == consumables.end() || consumables[key] == 0) {
            std::cout << "  You do not have this item!" << std::endl;
            std::cin.get();
            return false;
        }

        if (consumables[key] < quantity) {
            std::cout << "  Not enough items! (You have " << consumables[key] << ")" << std::endl;
            std::cin.get();
            return false;
        }

        consumables[key] -= quantity;
        if (consumables[key] == 0) {
            consumables.erase(key);
        }

        std::cout << "  " << quantity << "x " << getConsumableName(id) << " removed!" << std::endl;
        std::cin.get();
        return true;
    }

    bool removePotion(PotionID id, int quantity = 1) {
        std::string key = getPotionKey(id);
        
        if (potions.find(key) == potions.end() || potions[key] == 0) {
            std::cout << "  You do not have this potion!" << std::endl;
            std::cin.get();
            return false;
        }

        if (potions[key] < quantity) {
            std::cout << "  Not enough potions! (You have " << potions[key] << ")" << std::endl;
            std::cin.get();
            return false;
        }

        potions[key] -= quantity;
        if (potions[key] == 0) {
            potions.erase(key);
        }

        std::cout << "  " << quantity << "x " << getPotionName(id) << " removed!" << std::endl;
        std::cin.get();
        return true;
    }

    bool consumeConsumable(ConsumableID id) {
        const std::string key = getConsumableKey(id);
        auto item = consumables.find(key);
        if (item == consumables.end() || item->second <= 0) {
            return false;
        }

        if (--item->second == 0) {
            consumables.erase(item);
        }
        return true;
    }

    bool consumePotion(PotionID id) {
        const std::string key = getPotionKey(id);
        auto item = potions.find(key);
        if (item == potions.end() || item->second <= 0) {
            return false;
        }

        if (--item->second == 0) {
            potions.erase(item);
        }
        return true;
    }

    // ========== USAR ITEMS ==========
    bool useConsumable(ConsumableID id, finalStats &fs, int &critical_rate, HP &hp, Mana &mn) {
        if (!removeConsumable(id, 1)) {
            return false;
        }

        applyConsumable(id, fs, critical_rate, hp, mn);
        std::cout << "  Effect of " << getConsumableName(id) << " applied!" << std::endl;
        std::cin.get();
        return true;
    }

    bool usePotion(PotionID id, finalStats &fs, int &critical_rate, HP &hp, Mana &mn) {
        if (!removePotion(id, 1)) {
            return false;
        }

        applyPotion(id, fs, critical_rate, hp, mn);
        std::cout << "  Effect of " << getPotionName(id) << " applied!" << std::endl;
        std::cin.get();
        return true;
    }

    // ========== CONSULTAR ITEMS ==========
    int getConsumableQuantity(ConsumableID id) const {
        std::string key = getConsumableKey(id);
        auto it = consumables.find(key);
        return (it != consumables.end()) ? it->second : 0;
    }

    int getPotionQuantity(PotionID id) const {
        std::string key = getPotionKey(id);
        auto it = potions.find(key);
        return (it != potions.end()) ? it->second : 0;
    }

    int getTotalItems() const {
        int total = 0;
        for (const auto& item : consumables) total += item.second;
        for (const auto& item : potions) total += item.second;
        return total;
    }

    // ========== EXIBIR INVENTÁRIO ==========
    void displayInventory() const {
        int total = getTotalItems();
        
        std::cout << "\n╔══════════════════════════════════════╗" << std::endl;
        std::cout << "║          INVENTORY                   ║" << std::endl;
        std::cout << "║  Items: " << std::setw(2) << total << "/" << std::setw(2) << maxCapacity
                  << "                        ║" << std::endl;
        std::cout << "╚══════════════════════════════════════╝" << std::endl;

        if (consumables.empty() && potions.empty()) {
            std::cout << "\n  (empty)" << std::endl;
            return;
        }

        // Consumíveis
        if (!consumables.empty()) {
            std::cout << "\n┌─ CONSUMABLES:" << std::endl;
            for (int i = 1; i <= 4; i++) {
                std::string key = getConsumableKey(static_cast<ConsumableID>(i));
                auto it = consumables.find(key);
                if (it != consumables.end() && it->second > 0) {
                    std::cout << "│  [" << i << "] " << getConsumableName(static_cast<ConsumableID>(i)) 
                              << " x" << it->second << std::endl;
                }
            }
            std::cout << "└" << std::endl;
        }

        // Poções
        if (!potions.empty()) {
            std::cout << "\n┌─ POTIONS:" << std::endl;
            for (int i = 1; i <= 9; i++) {
                std::string key = getPotionKey(static_cast<PotionID>(i));
                auto it = potions.find(key);
                if (it != potions.end() && it->second > 0) {
                    std::cout << "│  [" << (i + 4) << "] " << getPotionName(static_cast<PotionID>(i)) 
                              << " x" << it->second << std::endl;
                }
            }
            std::cout << "└" << std::endl;
        }

        std::cout << std::endl;
    }

    // ========== SELECIONAR E USAR ITEM ==========
    void selectAndUseItem(effectsBonus &eff, finalStats &fs, int &critical_rate, HP &hp, Mana &mn) {
        if (consumables.empty() && potions.empty()) {
            std::cout << "\n  Inventory is empty! Nothing to use.\n";
            std::cin.get();
            return;
        }

        displayInventory();
        std::cout << "  Which item would you like to use? (or 0 to cancel)\n";
        std::cout << "  > ";
        
        int escolha;
        std::cin >> escolha;

        if (escolha == 0) {
            std::cout << "  Cancelado.\n";
            std::cin.ignore();
            return;
        }

        // Consumíveis (1-4)
        if (escolha >= 1 && escolha <= 4) {
            ConsumableID id = static_cast<ConsumableID>(escolha);
            if (getConsumableQuantity(id) > 0) {
                useConsumable(id, fs, critical_rate, hp, mn);
            } else {
                std::cout << "  You do not have this item!\n";
                std::cin.ignore();
            }
            return;
        }

        // Poções (5-13)
        if (escolha >= 5 && escolha <= 13) {
            PotionID id = static_cast<PotionID>(escolha - 4);
            if (getPotionQuantity(id) > 0) {
                usePotion(id, fs, critical_rate, hp, mn);
            } else {
                std::cout << "  You do not have this potion!\n";
                std::cin.ignore();
            }
            return;
        }

        std::cout << "  Invalid option!\n";
        std::cin.ignore();
    }

    void displayDetailed() const {
        // Exibir detalhes com efeitos
        std::cout << "\n ITEM DETAILS:\n" << std::endl;
        
        for (int i = 1; i <= 4; i++) {
            ConsumableID id = static_cast<ConsumableID>(i);
            if (getConsumableQuantity(id) > 0) {
                auto* effect = getEffect(id);
                std::cout << "▪ " << getConsumableName(id) << " x" << getConsumableQuantity(id) << std::endl;
                if (effect->Bonus.HealAmount > 0)
                    std::cout << "  ├─ Healing: +" << effect->Bonus.HealAmount << " HP" << std::endl;
                if (effect->Bonus.DamageBonus > 0)
                    std::cout << "  ├─ Damage: +" << effect->Bonus.DamageBonus << std::endl;
                if (effect->Bonus.LeftTurns > 0)
                    std::cout << "  └─ Duration: " << effect->Bonus.LeftTurns << " turns" << std::endl;
            }
        }

        for (int i = 1; i <= 9; i++) {
            PotionID id = static_cast<PotionID>(i);
            if (getPotionQuantity(id) > 0) {
                auto* potion = getPotion(id);
                std::cout << "▪ " << getPotionName(id) << " x" << getPotionQuantity(id) << std::endl;
                if (potion->Bonus.HealAmount > 0)
                    std::cout << "  ├─ Healing: +" << potion->Bonus.HealAmount << " HP" << std::endl;
                if (potion->Bonus.ManaBonus > 0)
                    std::cout << "  ├─ Mana: +" << potion->Bonus.ManaBonus << std::endl;
                if (potion->Bonus.DamageBonus > 0)
                    std::cout << "  └─ Damage: +" << potion->Bonus.DamageBonus << std::endl;
            }
        }

        std::cout << "  Would you like to use an item? (Enter its number or 0 to exit)" << std::endl;

        

        std::cout << "  You have " << getRemainingCapacity() << " inventory slots remaining." << std::endl;
        std::cin.ignore();
    }

    // ========== LIMPAR INVENTÁRIO ==========
    void clear() {
        consumables.clear();
        potions.clear();
        std::cout << "  Inventory cleared!" << std::endl;
        std::cin.get();
    }

    // Menu do inventário
    void menuBag(effectsBonus &eff, finalStats &fs, int &critical_rate, HP &hp, Mana &mn) {
    
        while (true) {
            displayInventory();
            std::cout << "\n=== INVENTORY MENU ===" << std::endl;
            std::cout << "1. View item details" << std::endl;
            std::cout << "2. Use an item" << std::endl;
            std::cout << "3. Exit" << std::endl;
            std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━" << std::endl;
            std::cout << "Choose: ";

            int opcao;
            std::cin >> opcao;
            std::cin.ignore();

            switch (opcao) {
                case 1:
                    displayDetailed();
                    break;

                case 2:
                    selectAndUseItem(eff, fs, critical_rate, hp, mn);
                    break;

                case 3:
                    std::cout << "Leaving inventory..." << std::endl;
                    return;
                default:
                    std::cout << "Invalid option!" << std::endl;
                    break;
            }
        }   
    }


    // ========== GETTERS ==========
    int getCapacity() const { return maxCapacity; }
    int getRemainingCapacity() const { return maxCapacity - getTotalItems(); }
};