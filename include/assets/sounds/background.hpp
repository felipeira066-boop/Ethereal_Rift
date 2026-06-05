#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <iostream>

class AudioManager {
private:
    Mix_Music* musicaDeFundo;
    bool somInicializado;

public:
    // Construtor: Inicializa os ponteiros
    AudioManager() : musicaDeFundo(nullptr), somInicializado(false) {}

    // Destrutor: Garante que a memória vai ser limpa se você esquecer de fechar
    ~AudioManager() {
        encerrar();
    }

    // Inicializa o sistema de som
    bool inicializar() {
        if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
            std::cerr << "Erro SDL_Audio: " << SDL_GetError() << std::endl;
            return false;
        }

        // Abre os canais de áudio
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            std::cerr << "Erro Mix_OpenAudio: " << Mix_GetError() << std::endl;
            return false;
        }

        somInicializado = true;
        return true;
    }

    // Carrega e toca a música em loop
    bool reproduzirMusica(const std::string& caminho) {
        if (!somInicializado) {
            std::cerr << "Erro: O sistema de áudio não foi inicializado!" << std::endl;
            return false;
        }

        // Se já tiver uma música tocando, para e libera ela primeiro
        pararMusica();

        musicaDeFundo = Mix_LoadMUS(caminho.c_str());
        if (!musicaDeFundo) {
            std::cerr << "Erro ao carregar musica: " << Mix_GetError() << std::endl;
            return false;
        }

        // Toca em loop infinito (-1)
        Mix_PlayMusic(musicaDeFundo, -1);
        return true;
    }

    void pararMusica() {
        if (musicaDeFundo) {
            Mix_HaltMusic();
            Mix_FreeMusic(musicaDeFundo);
            musicaDeFundo = nullptr;
        }
    }

    // Fecha o sistema completamente
    void encerrar() {
        pararMusica();
        if (somInicializado) {
            Mix_CloseAudio();
            somInicializado = false;
        }
    }
};