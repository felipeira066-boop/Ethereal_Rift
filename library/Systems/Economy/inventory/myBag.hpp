#pragma once

#include "library/Systems/Itens/Effects.hpp"
#include <iostream>
#include <map>
#include <iomanip>

class Inventory : virtual public setEffects {
private:
    // Map que armazena: chave = ID (combinado), valor = quantidade
    std::map<std::string, int> consumables;  // "consumable_1", "consumable_2", etc
    std::map<std::string, int> potions;      // "potion_1", "potion_2", etc
    
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

    // ========== ADICIONAR ITEMS ==========
    bool addConsumable(ConsumableID id, int quantity = 1) {
        if (!setConsumable(id) || id == ConsumableID::NONE) {
            std::cout << "  Item consumível inválido!" << std::endl;
            std::cin.get();
            return false;
        }

        std::string key = getConsumableKey(id);
        int currentQty = consumables[key];
        
        if (currentQty + quantity > maxCapacity) {
            std::cout << "  Inventário cheio!" << std::endl;
            std::cin.get();
            return false;
        }

        consumables[key] += quantity;
        std::cout << "  "<< quantity << "x " << getConsumableName(id) << " adicionado!" << std::endl;
        return true;
    }

    bool addPotion(PotionID id, int quantity = 1) {
        if (!setPotion(id)) {
            std::cout << "  Poção inválida!" << std::endl;
            std::cin.get();
            return false;
        }

        std::string key = getPotionKey(id);
        int currentQty = potions[key];
        
        if (currentQty + quantity > maxCapacity) {
            std::cout << "  Inventário cheio!" << std::endl;
            std::cin.get();
            return false;
        }

        potions[key] += quantity;
        std::cout << "  " << quantity << "x " << getPotionName(id) << " adicionado!" << std::endl;
        std::cin.get();
        return true;
    }

    // ========== REMOVER ITEMS ==========
    bool removeConsumable(ConsumableID id, int quantity = 1) {
        std::string key = getConsumableKey(id);
        
        if (consumables.find(key) == consumables.end() || consumables[key] == 0) {
            std::cout << "  Você não possui este item!" << std::endl;
            std::cin.get();
            return false;
        }

        if (consumables[key] < quantity) {
            std::cout << "  Quantidade insuficiente! (Você tem " << consumables[key] << ")" << std::endl;
            std::cin.get();
            return false;
        }

        consumables[key] -= quantity;
        if (consumables[key] == 0) {
            consumables.erase(key);
        }

        std::cout << "  " << quantity << "x " << getConsumableName(id) << " a menos!" << std::endl;
        std::cin.get();
        return true;
    }

    bool removePotion(PotionID id, int quantity = 1) {
        std::string key = getPotionKey(id);
        
        if (potions.find(key) == potions.end() || potions[key] == 0) {
            std::cout << "  Você não possui esta poção!" << std::endl;
            std::cin.get();
            return false;
        }

        if (potions[key] < quantity) {
            std::cout << "  Quantidade insuficiente! (Você tem " << potions[key] << ")" << std::endl;
            std::cin.get();
            return false;
        }

        potions[key] -= quantity;
        if (potions[key] == 0) {
            potions.erase(key);
        }

        std::cout << "  " << quantity << "x " << getPotionName(id) << " a menos!" << std::endl;
        std::cin.get();
        return true;
    }

    // ========== USAR ITEMS ==========
    bool useConsumable(ConsumableID id, finalStats &fs, int &critical_rate, HP &hp, Mana &mn) {
        if (!removeConsumable(id, 1)) {
            return false;
        }

        applyConsumable(id, fs, critical_rate, hp, mn);
        std::cout << "  Efeito de " << getConsumableName(id) << " aplicado!" << std::endl;
        std::cin.get();
        return true;
    }

    bool usePotion(PotionID id, finalStats &fs, int &critical_rate, HP &hp, Mana &mn) {
        if (!removePotion(id, 1)) {
            return false;
        }

        applyPotion(id, fs, critical_rate, hp, mn);
        std::cout << "  Efeito de " << getPotionName(id) << " aplicado!" << std::endl;
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
        std::cout << "║          INVENTÁRIO                  ║" << std::endl;
        std::cout << "║  Itens: " << std::setw(2) << total << "/" << std::setw(2) << maxCapacity 
                  << "                        ║" << std::endl;
        std::cout << "╚══════════════════════════════════════╝" << std::endl;

        if (consumables.empty() && potions.empty()) {
            std::cout << "\n  (vazio)" << std::endl;
            return;
        }

        // Consumíveis
        if (!consumables.empty()) {
            std::cout << "\n┌─ CONSUMÍVEIS:" << std::endl;
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
            std::cout << "\n┌─ POÇÕES:" << std::endl;
            int count = 0;
            for (int i = 1; i <= 9; i++) {
                std::string key = getPotionKey(static_cast<PotionID>(i));
                auto it = potions.find(key);
                if (it != potions.end() && it->second > 0) {
                    std::cout << "│  [" << (i + 4) << "] " << getPotionName(static_cast<PotionID>(i)) 
                              << " x" << it->second << std::endl;
                    count++;
                }
            }
            std::cout << "└" << std::endl;
        }

        std::cout << std::endl;
    }

    // ========== SELECIONAR E USAR ITEM ==========
    void selectAndUseItem(effectsBonus &eff, finalStats &fs, int &critical_rate, HP &hp, Mana &mn) {
        if (consumables.empty() && potions.empty()) {
            std::cout << "\n  Inventário vazio! Nada para usar.\n";
            std::cin.get();
            return;
        }

        displayInventory();
        std::cout << "  Qual item você deseja usar? (ou 0 para cancelar)\n";
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
                std::cout << "  Você não possui este item!\n";
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
                std::cout << "  Você não possui esta poção!\n";
                std::cin.ignore();
            }
            return;
        }

        std::cout << "  Opção inválida!\n";
        std::cin.ignore();
    }

    void displayDetailed() const {
        // Exibir detalhes com efeitos
        std::cout << "\n DETALHES DOS ITENS:\n" << std::endl;
        
        for (int i = 1; i <= 4; i++) {
            ConsumableID id = static_cast<ConsumableID>(i);
            if (getConsumableQuantity(id) > 0) {
                auto* effect = getEffect(id);
                std::cout << "▪ " << getConsumableName(id) << " x" << getConsumableQuantity(id) << std::endl;
                if (effect->Bonus.HealAmount > 0)
                    std::cout << "  ├─ Cura: +" << effect->Bonus.HealAmount << " HP" << std::endl;
                if (effect->Bonus.DamageBonus > 0)
                    std::cout << "  ├─ Dano: +" << effect->Bonus.DamageBonus << std::endl;
                if (effect->Bonus.LeftTurns > 0)
                    std::cout << "  └─ Duração: " << effect->Bonus.LeftTurns << " turnos" << std::endl;
            }
        }

        for (int i = 1; i <= 9; i++) {
            PotionID id = static_cast<PotionID>(i);
            if (getPotionQuantity(id) > 0) {
                auto* potion = getPotion(id);
                std::cout << "▪ " << getPotionName(id) << " x" << getPotionQuantity(id) << std::endl;
                if (potion->Bonus.HealAmount > 0)
                    std::cout << "  ├─ Cura: +" << potion->Bonus.HealAmount << " HP" << std::endl;
                if (potion->Bonus.ManaBonus > 0)
                    std::cout << "  ├─ Mana: +" << potion->Bonus.ManaBonus << std::endl;
                if (potion->Bonus.DamageBonus > 0)
                    std::cout << "  └─ Dano: +" << potion->Bonus.DamageBonus << std::endl;
            }
        }

        std::cout << "  Você quer usar algum item? (Digite o número correspondente ou 0 para sair)" << std::endl;

        

        std::cout << "  Você tem " << getRemainingCapacity() << " espaços restantes no inventário." << std::endl;
        std::cin.ignore();
    }

    // ========== LIMPAR INVENTÁRIO ==========
    void clear() {
        consumables.clear();
        potions.clear();
        std::cout << "  Inventário limpo!" << std::endl;
        std::cin.get();
    }

    // Menu do inventário
    void menuBag(effectsBonus &eff, finalStats &fs, int &critical_rate, HP &hp, Mana &mn) {
    
        while (true) {
            displayInventory();
            std::cout << "\n=== MENU DO INVENTÁRIO ===" << std::endl;
            std::cout << "1. Ver detalhes dos itens" << std::endl;
            std::cout << "2. Usar item" << std::endl;
            std::cout << "3. Sair" << std::endl;
            std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━" << std::endl;
            std::cout << "Escolha: ";

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
                    std::cout << "Saindo do inventário..." << std::endl;
                    return;
                default:
                    std::cout << "Opção inválida!" << std::endl;
                    break;
            }
        }   
    }


    // ========== GETTERS ==========
    int getCapacity() const { return maxCapacity; }
    int getRemainingCapacity() const { return maxCapacity - getTotalItems(); }
};