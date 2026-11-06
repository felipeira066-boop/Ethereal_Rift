#pragma once
#include "Stats/HP.hpp"
#include "Arena.hpp"
#include "character/Moves/Moves.hpp"
#include "Stats/Mana.hpp"
#include "Systems/Itens/Effects.hpp"
#include "Systems/Economy/inventory/myBag.hpp"
#include <iostream>
#include <string>

using std::cout;
using std::cin;

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
                << " — healed " << move->curaBase << " HP!\n";

            jogador.useMana(move->costMana);
            cout << "  > Mana remaining: " << jogador.getCurrentMana() << "\n";
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
                << " dealt " << danoComShield
                << " damage to " << inimigo.nome << "!\n";

            jogador.useMana(move->costMana);
            cout << "  > Mana remaining: " << jogador.getCurrentMana() << "\n";
        }
    } else {
        cout << "  > No move used!\n";
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

    cout << "  > " << inimigo.nome << " attacks!\n";
    jogador.takeDamage(iniStats);

    return true;
}

template <typename B>

bool battle(B &jogador, EnemyData &inimigo, const std::string &nomeJogador, Inventory& bag) {
    int turno = 1;
    effectsBonus eff;

    if (inimigo.ID == 3 || inimigo.ID == 6 || inimigo.ID == 9 || inimigo.ID == 12) {
            cout << "  > Would you like to visit the city shop? (y/n)\n"
            << "  > ";
            char resposta;
            cin >> resposta;

            if (resposta == 'y' || resposta == 'Y') {
                Shop shop(bag);
                shop.initShop();
            }
        }

         cout << "  > Press Enter to start the battle...";
         cin.get();

    cout << "\n========================================\n";
    cout << "  BATTLE: " << nomeJogador << " vs " << inimigo.nome << "\n";
    cout << "========================================\n";

    while (jogador.isAlive() && inimigo.isAlive()) {

        cout << "\n  -- Turn: " << turno << " --\n";
        printBarraHP(nomeJogador,  jogador.getHP(), jogador.getMaxHP());
        printBarraMana( jogador.getCurrentMana(), jogador.getMaxMana());
        printBarraExp( jogador.getExp(), jogador.getExpMax());

        printBarraHP(inimigo.nome, inimigo.hp, inimigo.hpmax);

        cout << "  1. Fight\n";
        cout << "  2. Open Inventory\n";
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
        cout << "  You defeated " << inimigo.nome << "!\n";
        cout << "  + " << inimigo.expRecompensa  << " EXP\n";
        gainCoin(inimigo.coinRecompensa);
        return true;
    }

    cout << "  You were defeated by " << inimigo.nome << ". Game Over.\n";
    cin.get();
    cout << "  RECORD: " << inimigo.ID <<  " enemies defeated.\n";

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
        

        cout << "\nNext challenge: " << inimigo.nome
             << " (HP: " << inimigo.hp
             << " | Damage: " << inimigo.damage
             << " | Shield: " << inimigo.shield << ")\n";

        

        if (!battle(jogador, inimigo, nomeJogador, playerInventory))
            return;

        /* level up após batalha */
        jogador.gainExp(inimigo.expRecompensa);

        cout << "========================================\n";
        cout << "  " << nomeJogador
             << " | Level " << jogador.getNivel()
            << " | HP " << jogador.getHP()
            << "/" << jogador.getMaxHP() << "\n";
        cout << "========================================\n";
    }

    cout << "\n========================================\n";
    cout << "  " << nomeJogador << " completed the arena!\n";
    cout << "========================================\n";
}