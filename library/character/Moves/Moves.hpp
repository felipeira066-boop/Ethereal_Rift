#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "../../Stats/Stats_point.hpp"

/* ═══════════════════════════════════════════════════
   Enum de todas as habilidades do jogo
   Cada classe tem as suas — separadas por bloco
═══════════════════════════════════════════════════ */

enum class MoveID {
    NONE = 0,

    /* Warrior */
    PORRADA         = 1,
    FURIA           = 2,
    ECLIPSE_MORTAL  = 3,

    /* Hunter */
    TIRO            = 4,
    FLECHA_DE_OURO  = 5,
    MIRA_DA_MORTE   = 6,

    /* Mage */
    EVIDENCIA       = 7,
    CONJURACAO      = 8,
    BOLA_DE_FOGO    = 9,

    /* Cleric */
    GOLPE_SAGRADO   = 10,
    CURA_DIVINA     = 11,
    JUIZO_FINAL     = 12
};

/* ═══════════════════════════════════════════════════
   Struct de um move — dados fixos
═══════════════════════════════════════════════════ */

struct MoveData {
    MoveID      id;
    std::string nome;
    uint16_t    nivelMinimo;   /* nível necessário para desbloquear */
    uint16_t    danoBase;      /* dano base do move */
    uint16_t    curaBase;      /* cura (usado pelo Cleric) */
    std::string descricao;
    uint16_t    costMana;
};

/* ═══════════════════════════════════════════════════
   Tabelas de moves por classe — definidas globalmente
═══════════════════════════════════════════════════ */

inline const std::vector<MoveData> warriorMoves = {
    { MoveID::PORRADA,        "Punch",          1,  5,  0, "A direct punch at the enemy.", 0 },
    { MoveID::FURIA,          "Fury",           5,  10, 0, "A furious attack with all your strength.", 0 },
    { MoveID::ECLIPSE_MORTAL, "Mortal Eclipse", 15, 28, 0, "A devastating strike that eclipses all.", 2 }
};

inline const std::vector<MoveData> hunterMoves = {
    { MoveID::TIRO,           "Shot",           1,  8,  0, "A precise ranged attack.", 0 },
    { MoveID::FLECHA_DE_OURO, "Golden Arrow",   5,  18, 0, "An enchanted arrow that deals heavy damage.", 0 },
    { MoveID::MIRA_DA_MORTE,  "Deadly Aim",     15, 35, 0, "A precise shot at a vital point.", 0 }
};

inline const std::vector<MoveData> mageMoves = {
    { MoveID::EVIDENCIA,      "Arcane Bolt",    1,  9,  0, "A basic arcane energy spell.", 6 },
    { MoveID::CONJURACAO,     "Conjuration",    5,  16, 0, "Summons powerful magical forces.", 15 },
    { MoveID::BOLA_DE_FOGO,   "Fireball",       15, 35, 0, "An explosive sphere of fire.", 34 }
};

inline const std::vector<MoveData> clericMoves = {
    { MoveID::GOLPE_SAGRADO,  "Holy Strike",     1,  7,  0,  "A strike blessed by divine light.",  3  },
    { MoveID::CURA_DIVINA,    "Divine Heal",     5,  0,  20, "Restores HP with sacred power.",  10  },
    { MoveID::JUIZO_FINAL,    "Final Judgment", 15, 30, 0,  "Divine power that judges the enemy.",  25  }
};

/* ═══════════════════════════════════════════════════
   Classe base de sistema de moves
   Cada classe de personagem herda desta
═══════════════════════════════════════════════════ */

class MoveSystem {
protected:
    const std::vector<MoveData> *moveTable = nullptr;   /* aponta para a tabela da classe */

public:

    /* exibe moves disponíveis e bloqueados */
    void exibirMoves(uint16_t nivel) const {
        if (!moveTable) return;

        std::cout << "\n  Choose your attack:\n";
        for (int i = 0; i < (int)moveTable->size(); i++) {
            const MoveData &m = (*moveTable)[i];

            if (nivel >= m.nivelMinimo) {

                std::cout << "    " << (i + 1) << ". "
                          << m.nome
                          << "  (damage: " << m.danoBase << ")"
                          << "  (healing: " << m.curaBase << ")"
                          << "  (Mana: " << m.costMana << ")\n";

            } else {
                std::cout << "    " << (i + 1) << ". "
                          << m.nome
                          << " (locked — level " << m.nivelMinimo << ")"
                          << "\n";
                          
            }
        }
        std::cout << "  > ";
    }

    template<typename P>
    /* retorna o move escolhido pelo jogador */
    const MoveData *escolherMove(uint16_t nivel, const P &jogador) const {
        if (!moveTable) return nullptr;

        exibirMoves(nivel);

        int op;
        std::cin >> op;
        op--;

        if (op < 0 || op >= (int)moveTable->size()) op = 0;

        const MoveData &m = (*moveTable)[op];

        if (nivel < m.nivelMinimo) {
            std::cout << "  Move locked! Using the first available move.\n";
            return &(*moveTable)[0];
        }
        if(m.costMana > jogador.getCurrentMana()){
            std::cout << "  You do not have enough mana to use this move.\n";
            std::cout << "  No move used.\n" << std::endl;
            return nullptr;
        }

        return &m;
    }

    /* verifica se um move está disponível */
    bool moveDisponivel(MoveID id, uint16_t nivel) const {
        if (!moveTable) return false;

        for (const auto &m : *moveTable) {
            if (m.id == id)
                return nivel >= m.nivelMinimo;
        }
        return false;
    }

    /* notifica moves desbloqueados ao subir de nível */
    void verificarNovosMovesDesbloqueados(uint16_t novoNivel) const {
        if (!moveTable) return;

        for (const auto &m : *moveTable) {
            if (m.nivelMinimo == novoNivel) {
                std::cout << "  --> New move unlocked: "
                          << m.nome << "!\n";
            }
        }
    }
};
