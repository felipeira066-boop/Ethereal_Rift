# Ethereal Rift - RPG em C++

Um jogo RPG 2D em desenvolvimento, criado com C++ e SDL2. Você cria um personagem, escolhe sua classe e raça, e explora um mundo de fantasia repleto de batalhas, NPCs e um sistema completo de economia.

## 🎮 Características

- **Sistema de Personagem**: Classes (Guerreiro, Mago, Clérigo, Caçador) e Raças (Humano, Elfo, Anão)
- **Sistema de Batalha**: Combate por turnos com críticos, mana e habilidades especiais
- **Economia**: Ouro, inventário, loja e banco
- **Mundo**: Cidades, florestas, NPCs e monstros variados
- **Sistema de Progressão**: Níveis, pontos de status e equipamentos

## 📋 Pré-requisitos

- **C++17** ou superior
- **SDL2** e **SDL2_mixer** (para áudio)
- **GNU Make**
- **g++** ou compilador C++ compatível

### Instalação de dependências

#### Linux (Ubuntu/Debian)
```bash
sudo apt-get install libsdl2-dev libsdl2-mixer-dev build-essential
```

#### macOS
```bash
brew install sdl2 sdl2_mixer
```

#### Windows (MinGW)
Instale SDL2 e SDL2_mixer conforme descrito em https://wiki.libsdl.org/

## 🚀 Como Compilar e Executar

```bash
# Compilar o projeto
make

# Executar o jogo
./builds/Ethereal_Rift.exe

# Limpar arquivos compilados
make clean
```

## 📁 Estrutura do Projeto

```
Ethereal_Rift/
├── README.md                 # Este arquivo
├── Makefile                  # Configuração de compilação
├── .gitignore               # Arquivos ignorados pelo Git
├── STRUCTURE.md             # Documentação da estrutura
│
├── include/                 # Headers C++ (.hpp)
│   ├── core/                # Classes base e componentes centrais
│   ├── character/           # Sistema de personagem
│   ├── battle/              # Sistema de batalha
│   ├── systems/             # Sistemas de jogo
│   ├── world/               # Mundo, NPCs e inimigos
│   └── audio/               # Gerenciamento de áudio
│
├── src/                     # Implementações C++ (.cpp)
│   └── main.cpp             # Ponto de entrada do programa
│
├── assets/                  # Recursos do jogo
│   └── audio/               # Arquivos de som
│       ├── city/            # Músicas/sons da cidade
│       └── forest/          # Músicas/sons da floresta
│
├── builds/                  # Executáveis compilados
│   └── Ethereal_Rift.exe    # Executável do jogo
│
└── test/                    # Testes e código experimental
```

## 🏗️ Arquitetura

O projeto é organizado em **módulos temáticos**:

- **Character**: Gerencia classes, raças, stats e movimentos
- **Battle**: Sistema de combate e críticos
- **Systems**: Economia, inventário e manual do jogo
- **World**: NPCs, monstros e ambientes
- **Audio**: Gerenciamento de sons e músicas

## 📝 Desenvolvimento

Este projeto está em **desenvolvimento ativo**. Consulte [STRUCTURE.md](STRUCTURE.md) para detalhes técnicos sobre cada módulo.

### Próximos passos
- [ ] Interface gráfica (SDL2 graphics)
- [ ] Mapas maiores e mais exploração
- [ ] Mais tipos de inimigos
- [ ] Sistema de missões
- [ ] Multiplayer local

## 📜 Licença

[Defina sua licença aqui]

## 👨‍💻 Autor

Criado por [Seu Nome]

---

**Nota**: O jogo está em desenvolvimento e pode ter bugs. Reporte problemas na seção Issues do repositório.

