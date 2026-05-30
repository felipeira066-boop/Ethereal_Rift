#pragma once
#include "library/Stats/HP.hpp"
#include "library/character/Moves/Moves.hpp"
#include "library/Stats/Mana.hpp"
#include "library/Systems/Itens/Effects.hpp"
#include "library/Systems/Economy/inventory/myBag.hpp"
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
    int         ID;

};

/* ═══════════════════════════════════════════════════
   Arena progressiva — igual ao código C original
═══════════════════════════════════════════════════ */

inline std::vector<EnemyData> arenaInimigos = {
    /* nome               hp  hpmax dmg shl exp coin  vel*/
    { "Goblin",           25,  25,  6,   0,  40,  10,   4 , 1},
    { "Goblin Guerreiro", 28,  28,  8,   1,  44,  12,   5 , 2},
    { "Goblin Arqueiro",  25,  25,  8,   0,  44,  12,   5 , 3},
    { "Goblin de Elite",  32,  32,  12,  1,  55,  18,   6 , 4},
    { "Lobo Selvagem",    40,  40,  14,  2,  60,  25,   7 , 5},
    { "Javali Selvagem",  52,  52,  18,  3,  98,  50,  10 , 6},
    { "Lobo Alfa",        48,  48,  18,  3,  70,  30,   8 , 7},
    { "Javali Selvagem",  52,  52,  18,  3,  98,  50,  10 , 8},
    { "Ogro",             60,  60,  20,  3,  95,  50,  11 , 9},
    { "Ogro",             67,  67,  20,  3,  95,  50,  11 , 10},
    { "Ogro Chefe",       74,  74,  23,  4, 150,  80,  15 , 11},
    { "Dragao",           82,  82,  30,  5, 200, 150,  20 , 12}
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

bool battle(B &jogador, EnemyData &inimigo, const std::string &nomeJogador, Inventory& bag) {
    int turno = 1;
    effectsBonus eff;

    if (inimigo.ID == 3 || inimigo.ID == 6 || inimigo.ID == 9 || inimigo.ID == 12) {
            cout << "  > Você quer visitar a loja da cidade? (s/n)\n"
            << "  > ";
            char resposta;
            cin >> resposta;

            if (resposta == 's' || resposta == 'S') {
                Shop shop(bag);
                shop.initShop();
            }
        }

         cout << "  > Pressione Enter para iniciar a batalha...";
         cin.get();

    cout << "\n========================================\n";
    cout << "  BATALHA: " << nomeJogador << " vs " << inimigo.nome << "\n";
    cout << "========================================\n";

    while (jogador.isAlive() && inimigo.isAlive()) {

        cout << "\n  -- Turno: " << turno << " --\n";
        printBarraHP(nomeJogador,  jogador.getHP(), jogador.getMaxHP());
        printBarraMana( jogador.getCurrentMana(), jogador.getMaxMana());
        printBarraExp( jogador.getExp(), jogador.getExpMax());

        printBarraHP(inimigo.nome, inimigo.hp, inimigo.hpmax);

        cout << "  1. Entrar em batalha\n";
        cout << "  2. Abrir Inventario\n";
        cout << "  > ";

        int esc;
        cin >> esc;

        if (esc == 2) {
            finalStats fs = jogador.getStats();
            int critical_rate = 0;
            bag.menuBag(eff, fs, critical_rate, static_cast<HP&>(jogador), static_cast<Mana&>(jogador));
            
            MonsterTurn(jogador, inimigo);
            jogador.CurrentMana += 2;
            turno++;
            continue;
        }

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
        gainCoin(inimigo.coinRecompensa);
        return true;
    }

    cout << "  Voce foi derrotado por " << inimigo.nome << ". Game Over.\n";
    cin.get();
    cout << "  RECORD: " << inimigo.ID <<  " inimigos derrotados.\n";

    return false;
}

/* ═══════════════════════════════════════════════════
   Loop da arena — percorre todos os inimigos
═══════════════════════════════════════════════════ */

template <typename T>
void arena(T &jogador, const std::string &nomeJogador) {
    jogador.getStats(); // Ou jogador.hp.getStats() se mudar a arquitetura
    Inventory playerInventory; // Criar inventário compartilhado

    for (auto &inimigo : arenaInimigos) {
        

        cout << "\nProximo desafio: " << inimigo.nome
             << " (HP: " << inimigo.hp
             << " | Dano: " << inimigo.damage
             << " | Shield: " << inimigo.shield << ")\n";

        

        if (!battle(jogador, inimigo, nomeJogador, playerInventory))
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