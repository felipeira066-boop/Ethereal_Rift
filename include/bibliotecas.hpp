/**
 * @file bibliotecas.hpp
 * @brief Header central que inclui todos os módulos do projeto
 * 
 * Este arquivo centraliza todos os includes do Ethereal Rift.
 * Inclua apenas este arquivo em main.cpp para ter acesso a tudo.
 */

#ifndef BIBLIOTECAS_HPP
#define BIBLIOTECAS_HPP

// ============================================================================
// CONFIGURAÇÃO E TIPOS BASE
// ============================================================================
#include "config.hpp"

// ============================================================================
// SISTEMA DE PERSONAGEM
// ============================================================================

// Raças
#include "character/Races/allRaces.hpp"
#include "character/Races/Human.hpp"
#include "character/Races/Elfo.hpp"
#include "character/Races/Dwarf.hpp"

// Classes
#include "character/Classes/allClasses.hpp"
#include "character/Classes/Warrior.hpp"
#include "character/Classes/Mage.hpp"
#include "character/Classes/Cleric.hpp"
#include "character/Classes/Hunter.hpp"

// Habilidades e movimentos
#include "character/Moves/Moves.hpp"

// ============================================================================
// SISTEMA DE ESTATÍSTICAS
// ============================================================================
#include "Stats/HP.hpp"
#include "Stats/Mana.hpp"
#include "Stats/level.hpp"
#include "Stats/Stats_point.hpp"
#include "Stats/Critical.hpp"

// ============================================================================
// SISTEMA DE BATALHA
// ============================================================================
#include "Battle.hpp"

// ============================================================================
// SISTEMA DE ITENS E ECONOMIA
// ============================================================================

// Tipos de itens
#include "Systems/Itens/Armaments.hpp"
#include "Systems/Itens/Armors.hpp"
#include "Systems/Itens/Materials.hpp"
#include "Systems/Itens/Effects.hpp"

// Economia
#include "Systems/Economy/Market/shop.hpp"
#include "Systems/Economy/inventory/myBag.hpp"
#include "Systems/Economy/inventory/Bank.hpp"

// ============================================================================
// SISTEMA DE MUNDO
// ============================================================================

// Tesouros e itens do mundo
#include "Systems/World/Treasures/Chests.hpp"

// Manual e documentação
#include "Systems/Manual.hpp"

// ============================================================================
// SISTEMA DE NPCs
// ============================================================================

// NPCs
#include "assets/NPCs/NPCs-City/Merchanters.hpp"

// ============================================================================
// SISTEMA DE ÁUDIO
// ============================================================================


// ============================================================================
// FIM - Todos os módulos estão inclusos!
// ============================================================================

#endif // BIBLIOTECAS_HPP
