# 📚 Guia de Headers - Include/

Este arquivo explica como usar os headers do projeto Ethereal Rift.

## 🎯 Quick Start

```cpp
#include "bibliotecas.hpp"  // Inclui TUDO automaticamente!

int main() {
    // Agora você tem acesso a todas as classes e funções
    return 0;
}
```

**Pronto!** Você não precisa incluir cada header individualmente.

---

## 📁 Estrutura de Headers

```
include/
├── bibliotecas.hpp      ← INCLUA ESTE (todas as bibliotecas)
├── config.hpp           ← Configurações globais
│
├── character/           ← Sistema de personagem
│   ├── Races/           ├─ Raças (Human, Elfo, Dwarf)
│   ├── Classes/         ├─ Classes (Warrior, Mage, etc)
│   └── Moves/           └─ Habilidades
│
├── Battle.hpp           ← Sistema de batalha
│
├── Stats/               ← Estatísticas
│   ├── HP.hpp           ├─ Pontos de vida
│   ├── Mana.hpp         ├─ Mana
│   ├── level.hpp        ├─ Nível
│   ├── Stats_point.hpp  ├─ Pontos de atributo
│   └── Critical.hpp     └─ Sistema de críticos
│
├── Systems/             ← Sistemas complementares
│   ├── Economy/         ├─ Economia
│   │   ├── Market/      │   └─ Loja (shop.hpp)
│   │   └── inventory/   │   ├─ Mochila (myBag.hpp)
│   │       └─ Banco (Bank.hpp)
│   ├── Itens/           ├─ Tipos de itens
│   │   ├── Armaments.hpp│   ├─ Armas
│   │   ├── Armors.hpp   │   ├─ Armaduras
│   │   ├── Materials.hpp│   ├─ Materiais
│   │   └── Effects.hpp  │   └─ Efeitos
│   ├── World/           ├─ Mundo
│   │   └── Treasures/   │   └─ Tesouros
│   │       └─ Chests.hpp
│   └── Manual.hpp       └─ Manual do jogo
│
├── assets/              ← Recursos do jogo
│   ├──                  ├
│   │                    │ 
│   ├── NPCs/            ├─ Personagens não-jogáveis
│   │   └── NPCs-City/   │   └─ Merchanters.hpp
│   └── sounds/          └─ Áudio
│       ├── background.hpp
│       └── background.cpp
```

---

## 💡 Como Usar

### Opção 1: Incluir Tudo (Recomendado para main.cpp)

```cpp
#include "bibliotecas.hpp"

int main() {
    // Acesso a TUDO
    Player player;
    Battle battle;
    Item sword;
    // ... etc
}
```

### Opção 2: Incluir Módulo Específico (Não recomendado)

```cpp
#include "character/Races/Human.hpp"
#include "Battle.hpp"

// Apenas estes headers
```

### Opção 3: Incluir Categoria

```cpp
// Se só precisa de personagem
#include "character/Races/allRaces.hpp"
#include "character/Classes/allClasses.hpp"
```

**Nota**: A opção 1 é mais limpa e organizada. Use nos arquivos principais.

---

## 🔧 Configuração Global

Arquivo: `config.hpp`

Contém:
- Versão do projeto
- Constantes de jogo (MAX_LEVEL, BASE_HP, etc)
- Macros úteis (LOG, CLAMP)
- Enums globais
- Estruturas comuns

**Exemplo de uso:**
```cpp
#include "config.hpp"

// Agora disponível:
MAX_INVENTORY_SIZE       // Constante
LOG("Debug message")     // Macro
ItemType::WEAPON         // Enum
Position pos(10, 20)     // Struct
```

---

## 📋 Ordem de Inclusão (Automática com bibliotecas.hpp)

1. **config.hpp** → Configurações base
2. **character/** → Sistema de personagem
3. **Battle.hpp** → Batalhas
4. **Stats/** → Estatísticas
5. **Systems/** → Sistemas (Economia, Itens, Mundo)
6. **assets/** → Recursos (Inimigos, NPCs, Sons)

**Respeitada automaticamente**. Não precisa se preocupar!

---

## ✅ Checklist de Headers

Ao criar um novo arquivo:

- [ ] Arquivo em `include/` (não em src/)
- [ ] Nome descritivo (ex: `PlayerInventory.hpp`, não `pi.hpp`)
- [ ] Header guards: `#ifndef ARQUIVO_HPP` / `#define ARQUIVO_HPP`
- [ ] Incluído em `bibliotecas.hpp`
- [ ] Sem código de implementação (.cpp) em headers
- [ ] Comentários explicativos em código complexo
- [ ] Testado compilando com `make`

---

## 🚫 Erros Comuns

### Erro: "undefined reference to..."
**Causa**: Header incluído mas implementação (.cpp) não compila
**Solução**: 
- Verifique se `.cpp` existe
- Atualize Makefile se necessário
- Recompile com `make clean && make`

### Erro: "redefinition of..."
**Causa**: Header incluído múltiplas vezes
**Solução**:
- Adicione header guards: `#ifndef`, `#define`, `#endif`
- Ou use: `#pragma once` (moderno, recomendado)

### Erro: Função/classe não encontrada
**Causa**: Tipo não incluído
**Solução**:
- Verifique se `#include "bibliotecas.hpp"` está em seu arquivo
- Ou inclua manualmente: `#include "character/Races/Human.hpp"`

---

## 🎓 Boas Práticas

### ✅ FAÇA

```cpp
// Em main.cpp
#include "bibliotecas.hpp"
using namespace std;

int main() {
    // Código limpo
    Player p;
    return 0;
}
```

### ❌ NÃO FAÇA

```cpp
// Não inclua tudo sem necessidade
#include "character/Races/Human.hpp"
#include "character/Races/Elfo.hpp"
#include "character/Races/Dwarf.hpp"
#include "character/Classes/Warrior.hpp"
// ... 30 linhas de includes

// Use bibliotecas.hpp em vez disso!
```

---

## 🔗 Dependências Entre Headers

```
bibliotecas.hpp
  ├→ config.hpp (sem dependências)
  ├→ character/
  │   ├→ Stats/ (lê config.hpp)
  │   └→ Moves/ (lê Stats/)
  ├→ Battle.hpp (lê Stats/ e character/)
  ├→ Systems/ (lê tudo acima)
  └→ assets/ (lê tudo acima)
```

**Importante**: Acíclico (sem ciclos circulares). Seguro incluir.

---

## 📝 Template para Novo Header

```cpp
/**
 * @file NomeDoArquivo.hpp
 * @brief Descrição breve do que faz
 * 
 * Descrição mais detalhada (opcional)
 */

#ifndef NOME_DO_ARQUIVO_HPP
#define NOME_DO_ARQUIVO_HPP

#include "config.hpp"  // Se precisa constantes globais
// Outros includes conforme necessário

class MinhaClasse {
public:
    MinhaClasse();
    void metodoPublico();
    
private:
    int atributoPrivado_;
    void metodoPrivado();
};

#endif // NOME_DO_ARQUIVO_HPP
```

---

## 🚀 Próximas Etapas

1. **Use `#include "bibliotecas.hpp"`** em seus arquivos
2. **Compile com `make`**
3. **Desenvolva!**

---

**Dúvidas?** Consulte [FAQ.md](../FAQ.md) no raiz do projeto.
