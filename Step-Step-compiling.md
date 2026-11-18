# passo à passo para compilar com make
## Abra o Perfil
notepad $PROFILE

## (se falhar)
New-Item -ItemType Directory -Force -Path (Split-Path $PROFILE)
New-Item -ItemType File -Force -Path $PROFILE
notepad $PROFILE

## Conteúdo
function make { mingw32-make @args }

## Carregue o perfil
. $PROFILE