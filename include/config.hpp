/**
 * @file config.hpp
 * @brief Configurações e constantes globais do Ethereal Rift
 */

#ifndef CONFIG_HPP
#define CONFIG_HPP

// ============================================================================
// VERSÃO DO PROJETO
// ============================================================================
#define ETHEREAL_RIFT_VERSION "1.0.0"
#define ETHEREAL_RIFT_BUILD_DATE "2026-06-05"

// ============================================================================
// INCLUDES PADRÃO C++
// ============================================================================
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <cstdlib>
#include <ctime>

// ============================================================================
// CONSTANTES DE JOGO
// ============================================================================

// Limites de inventário
constexpr int MAX_INVENTORY_SIZE = 20;
constexpr int MAX_BANK_SIZE = 50;

// Limites de personagem
constexpr int MAX_LEVEL = 99;
constexpr int MIN_LEVEL = 1;

// Valores base de HP e Mana
constexpr int BASE_HP = 100;
constexpr int BASE_MANA = 50;

// Chance de crítico
constexpr float BASE_CRITICAL_CHANCE = 0.15f; // 15%
constexpr float MAX_CRITICAL_CHANCE = 0.50f;  // 50%

// Dano
constexpr int BASE_DAMAGE = 10;
constexpr int MAX_DAMAGE = 999;

// Economía
constexpr int STARTING_GOLD = 100;
constexpr int MAX_GOLD = 999999;

// ============================================================================
// MACROS ÚTEIS
// ============================================================================

// Debug
#ifdef DEBUG
    #define LOG(x) std::cout << "[LOG] " << x << std::endl
    #define GAME_ERROR(x) std::cerr << "[ERROR] " << x << std::endl
#else
    #define LOG(x)
    #define GAME_ERROR(x)
#endif

// Clamp (limita valor entre min e max)
#define CLAMP(val, min, max) ((val) < (min) ? (min) : ((val) > (max) ? (max) : (val)))

// ============================================================================
// ENUMS GLOBAIS
// ============================================================================

/**
 * Tipos de equipamento
 */
enum class ItemType {
    WEAPON,      // Arma
    ARMOR,       // Armadura
    MATERIAL,    // Material
    CONSUMABLE,  // Item de consumo
    QUEST,       // Item de quest
    UNKNOWN
};

/**
 * Direções no mundo
 */
enum class Direction {
    NORTH,
    SOUTH,
    EAST,
    WEST,
    STAY
};

/**
 * Estados de batalha
 */
enum class BattleState {
    RUNNING,
    PLAYER_TURN,
    ENEMY_TURN,
    PLAYER_WON,
    PLAYER_LOST,
    ENDED
};

// ============================================================================
// ESTRUTURAS GLOBAIS
// ============================================================================

/**
 * Estrutura para representar posição no mapa
 */
struct Position {
    int x;
    int y;
    
    Position() : x(0), y(0) {}
    Position(int x_val, int y_val) : x(x_val), y(y_val) {}
    
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

// ============================================================================
// FIM DE CONFIGURAÇÕES
// ============================================================================

#endif // CONFIG_HPP
