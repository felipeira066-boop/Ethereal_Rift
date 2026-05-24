#pragma once
#include "library/Stats/HP.hpp"
#include "library/character/Moves/Moves.hpp"
#include "library/Stats/Mana.hpp"
#include "library/Systems/Itens/Effects.hpp"
#include <iostream>
#include <string>

using std::cout;
using std::cin;

/* ═══════════════════════════════════════════════════
   Struct de inimigo — dados completos
═══════════════════════════════════════════════════ */

struct EnemyData {
    std::string nome;
    int         hp;
    int         hpmax;
    uint32_t    damage;
    uint32_t    shield;
    uint32_t    expRecompensa;
    uint32_t    coinRecompensa;
    int16_t     speed;
    int         isAlive() const { return hp > 0; }

};

/* ═══════════════════════════════════════════════════
   Arena progressiva — igual ao código C original
═══════════════════════════════════════════════════ */

inline std::vector<EnemyData> arenaInimigos = {
    /* nome               hp  hpmax dmg shl exp coin  vel*/
    { "Goblin",           25,  25,  6,   0,  40,  10,   4 },
    { "Goblin Arqueiro",  25,  25,  8,  0,  44,  12,   5 },
    { "Goblin Guerreiro", 28,  28,  8,  1,  44,  12,   5 },
    { "Goblin de Elite",  32,  32,  12,  1,  55,  18,   6 },
    { "Lobo Selvagem",    40,  40,  14,  2,  60,  25,   7 },
    { "Lobo Alfa",        48,  48,  18,  3,  70,  30,   8 },
    { "Javali Selvagem",  52,  52,  18,  3,  98,  50,  10 },
    { "Javali Selvagem",  52,  52,  18,  3,  98,  50,  10 },
    { "Ogro",             60,  60,  20,  3,  95,  50,  11 },
    { "Ogro",             67,  67,  20,  3,  95,  50,  11 },
    { "Ogro Chefe",       74,  74,  23,  4, 150,  80,  15 },
    { "Dragao",           82,  82,  30,  5, 200, 150,  20 }
};

/* ═══════════════════════════════════════════════════
   Barra de HP visual
═══════════════════════════════════════════════════ */

inline void printBarraHP(const std::string &nome, int current, int max) {
    int total  = 20;
    int cheios = (max > 0) ? (current * total / max) : 0;

    cout << "  " << nome << "\n   HP-[";
    for (int i = 0; i < total; i++)
        cout << (i < cheios ? "\33[31m❤\33[0m " : "▪ ");
    cout << "] " << current << "/" << max << std::endl;
}

/* ═══════════════════════════════════════════════════
   Barra de Mana visual
═══════════════════════════════════════════════════ */

inline void printBarraMana( int current, int max) {
    int total  = 40;
    int cheios = (max > 0) ? (current * total / max) : 0;

    cout << "   Mana-[";
    for (int i = 0; i < total; i++)
        cout << (i < cheios ? "\33[36m◆\33[0m" : "▪ ");
    cout << "] " << current << "/" << max << std::endl;
}

/* ═══════════════════════════════════════════════════
   Barra de Exp visual
═══════════════════════════════════════════════════ */

inline void printBarraExp( int current, int max) {
    int total  = 40;
    int cheios = (max > 0) ? (current * total / max) : 0;

    cout << "   Exp-[";
    for (int i = 0; i < total; i++)
    cout << (i < cheios ? "\33[32m=\33[0m" : "-");
    cout << "] " << current << "/" << max << std::endl;
}

/* ═══════════════════════════════════════════════════
   Batalha principal — integra moves + HP + crítico
═════════════════════════════════════════════════ */

template <typename P>

bool PlayerTurn(P &jogador, EnemyData &inimigo, const std::string &nomeJogador){
    /* turno do jogador — escolhe move */
    const MoveData *move = jogador.escolherMove(jogador.getNivel(), jogador);

    if (move) {
        /* cura (para Cleric) */
        if (move->curaBase > 0) {
            jogador.getHeal(move->curaBase);
            cout << "  > " << move->nome
                << " — curou " << move->curaBase << " HP!\n";

            jogador.useMana(move->costMana);
            cout << "  > Mana restante: " << jogador.getCurrentMana() << "\n";
        }

        /* dano no inimigo */
        if (move->danoBase > 0) {
            /* aplica stats do jogador no dano */
            finalStats fs = jogador.getStats();
            jogador.CriticalDamage(fs);
            int danoFinal = move->danoBase + (int)fs.Damage;
            int danoComShield = danoFinal - (int)(inimigo.shield);
            if (danoComShield < 0) danoComShield = 0;

            inimigo.hp -= danoComShield;
            if (inimigo.hp < 0) inimigo.hp = 0;

            cout << "  > " << move->nome
                << " causou " << danoComShield
                << " de dano em " << inimigo.nome << "!\n";

            jogador.useMana(move->costMana);
            cout << "  > Mana restante: " << jogador.getCurrentMana() << "\n";
        }
    } else {
        cout << "  > Nenhum move usado!\n";
    }
    return true;
}

template <typename M>

bool MonsterTurn(M &jogador, EnemyData &inimigo){
    finalStats iniStats;
    iniStats.Damage = inimigo.damage;
    iniStats.Shield = inimigo.shield;
    iniStats.MaxHP  = inimigo.hpmax;
    iniStats.Speed  = inimigo.speed;
    iniStats.MaxMana = 0;

    cout << "  > " << inimigo.nome << " ataca!\n";
    jogador.takeDamage(iniStats);

    return true;
}

template <typename B>

bool battle(B &jogador, EnemyData &inimigo, const std::string &nomeJogador) {
    int turno        = 1;

    cout << "\n========================================\n";
    cout << "  BATALHA: " << nomeJogador << " vs " << inimigo.nome << "\n";
    cout << "========================================\n";

    while (jogador.isAlive() && inimigo.isAlive()) {

        cout << "\n  -- Turno: " << turno << " --\n";
        printBarraHP(nomeJogador,  jogador.getHP(), jogador.getMaxHP());
        printBarraMana( jogador.getCurrentMana(), jogador.getMaxMana());
        printBarraExp( jogador.getExp(), jogador.getExpMax());

        printBarraHP(inimigo.nome, inimigo.hp, inimigo.hpmax);
        
        if(jogador.getStats().Speed > (uint32_t)inimigo.speed){
            PlayerTurn(jogador, inimigo, nomeJogador);

            if(!inimigo.isAlive()) break;

            MonsterTurn(jogador, inimigo);
        } else {
            MonsterTurn(jogador, inimigo);
            if(!jogador.isAlive()) break;

            PlayerTurn(jogador, inimigo, nomeJogador);
        }

        jogador.CurrentMana += 2;

        turno++;
    }

    cout << "\n";
    printBarraHP(nomeJogador, jogador.getHP(), jogador.getMaxHP());
    cout << "\n";

    if (!inimigo.isAlive()) {
        cout << "  Voce derrotou " << inimigo.nome << "!\n";
        cout << "  + " << inimigo.expRecompensa  << " EXP\n";
        cout << "  + " << inimigo.coinRecompensa << " moedas\n";
        return true;
    }

    cout << "  Voce foi derrotado por " << inimigo.nome << ". Game Over.\n";
    cin.get();
    return false;
}

/* ═══════════════════════════════════════════════════
   Loop da arena — percorre todos os inimigos
═══════════════════════════════════════════════════ */

template <typename T>
void arena(T &jogador, const std::string &nomeJogador) {
    finalStats fs = jogador.getStats(); // Ou jogador.hp.getStats() se mudar a arquitetura
    int crtRate = 0; // Ou o valor vindo do sistema de status do seu jogador
    for (auto &inimigo : arenaInimigos) {

        cout << "\nProximo desafio: " << inimigo.nome
             << " (HP: " << inimigo.hp
             << " | Dano: " << inimigo.damage
             << " | Shield: " << inimigo.shield << ")\n";

        cout << "O que deseja fazer?\n";
        cin.get(); // Limpa o buffer para evitar problemas com getline() depois
        cout << "  1. Entrar em batalha\n";
        cout << "  2. Usar item\n";
        cout << "  3. Ir para a loja\n";
        cout << "  > ";

        int esc;
        cin >> esc;

        if (esc == 2) {
            jogador.exibirinventory();
            cout << "Digite o numero do item que deseja usar (1-" << jogador.inventory.size() << "): ";
            int itemOp;
            cin >> itemOp;

            if (itemOp >= 1 && itemOp <= static_cast<int>(jogador.inventory.size())) {
                ConsumableID itemId = static_cast<ConsumableID>(itemOp);
                jogador.usarConsumivel(itemId, fs, crtRate);
            } else {
                cout << "  Opção inválida!\n";
                cin.get(); // Limpa o buffer
            }
        }

        if(esc == 3){
            Shop shop;
            shop.initShop();
        }

        if (!battle(jogador, inimigo, nomeJogador))
            return;

        /* level up após batalha */
        jogador.gainExp(inimigo.expRecompensa);

        cout << "========================================\n";
        cout << "  " << nomeJogador
             << " | Nivel " << jogador.getNivel()
            << " | HP " << jogador.getHP()
            << "/" << jogador.getMaxHP() << "\n";
        cout << "========================================\n";
    }

    cout << "\n========================================\n";
    cout << "  " << nomeJogador << " completou a arena!\n";
    cout << "========================================\n";
}