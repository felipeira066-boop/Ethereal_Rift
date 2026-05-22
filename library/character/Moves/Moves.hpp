#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "library/Stats/Stats_point.hpp"

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
    { MoveID::PORRADA,        "Porrada",        1,  5,  0, "Um soco direto no inimigo.", 0 },
    { MoveID::FURIA,          "Furia",          5,  10, 0, "Ataque furioso com toda a forca.", 0 },
    { MoveID::ECLIPSE_MORTAL, "Eclipse Mortal", 15, 28, 0, "Golpe devastador que ofusca tudo.", 2 }
};

inline const std::vector<MoveData> hunterMoves = {
    { MoveID::TIRO,           "Tiro",           1,  8,  0, "Disparo preciso a distancia.", 0 },
    { MoveID::FLECHA_DE_OURO, "Flecha de Ouro", 5,  18, 0, "Flecha encantada de alto dano.", 0 },
    { MoveID::MIRA_DA_MORTE,  "Mira da Morte",  15, 35, 0, "Tiro certeiro em ponto vital.", 0 }
};

inline const std::vector<MoveData> mageMoves = {
    { MoveID::EVIDENCIA,      "Evidencia",      1,  9,  0, "Feitico basico de energia arcana.", 6 },
    { MoveID::CONJURACAO,     "Conjuracao",     5,  16, 0, "Invoca forcas magicas poderosas.", 15 },
    { MoveID::BOLA_DE_FOGO,   "Bola de Fogo",   15, 35, 0, "Esfera de fogo explosiva.", 34 }
};

inline const std::vector<MoveData> clericMoves = {
    { MoveID::GOLPE_SAGRADO,  "Golpe Sagrado",  1,  7,  0,  "Golpe abencado pela luz divina.",  3  },
    { MoveID::CURA_DIVINA,    "Cura Divina",    5,  0,  20, "Restaura HP com poder sagrado.",  10  },
    { MoveID::JUIZO_FINAL,    "Juizo Final",    15, 30, 0,  "Poder divino que julga o inimigo.",  25  }
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

        std::cout << "\n  Escolha seu ataque:\n";
        for (int i = 0; i < (int)moveTable->size(); i++) {
            const MoveData &m = (*moveTable)[i];

            if (nivel >= m.nivelMinimo) {

                std::cout << "    " << (i + 1) << ". "
                          << m.nome
                          << "  (dano: " << m.danoBase << ")"
                          << "  (cura: " << m.curaBase << ")"
                          << "  (Mana: " << m.costMana << ")\n";

            } else {
                std::cout << "    " << (i + 1) << ". "
                          << m.nome
                          << " (bloqueado — nivel " << m.nivelMinimo << ")"
                          << "\n";
                          
            }
        }
        std::cout << "  > ";
    }

    /* retorna o move escolhido pelo jogador */
    const MoveData *escolherMove(uint16_t nivel) const {
        if (!moveTable) return nullptr;

        exibirMoves(nivel);

        int op;
        std::cin >> op;
        op--;

        if (op < 0 || op >= (int)moveTable->size()) op = 0;

        const MoveData &m = (*moveTable)[op];

        if (nivel < m.nivelMinimo) {
            std::cout << "  Move bloqueado! Usando o primeiro disponivel.\n";
            return &(*moveTable)[0];
        }
        if(m.costMana > fs.CurrentMana){
            std::cout << "  Você não tem mana suficiente para usar este move.\n";
            std::cout << "  Nunhum move usado! \n" << std::endl;
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
                std::cout << "  --> Novo move desbloqueado: "
                          << m.nome << "!\n";
            }
        }
    }
};

