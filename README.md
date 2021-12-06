Rodando um hello world para Game Boy clássico usando
[gbdk](https://github.com/gbdk-2020/gbdk-2020). Também um setup básico para testes unitários.

## Requisitos

Uso o [sameboy](https://sameboy.github.io/) para emular o jogo em desenvolvimento.

Após compilar corretamente o jogo, é possível rodar: 

```bash
$ make rodar_jogo
```

## Setup

Baixar o gbdk:

```bash
$ make baixar_gbdk
```

## Estrutura

- **build**: arquivos de compilação
- **src**: código do jogo
- **tests**: testes usando [CppUTest](https://github.com/cpputest/cpputest)