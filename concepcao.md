# Análise do Projeto

## 1. Visão Geral

### Nome do Projeto
Royal Knight (nome temporário)

### Descrição
Jogo de plataforma 2D focado em movimentação fluida e precisa, no qual o jogador deve superar obstáculos utilizando habilidades de parkour e alcançar o final da fase no menor tempo possível para salvar a princesa.

### Objetivo do Jogador
Completar a fase superando obstáculos e alcançar a meta no menor tempo possível, tentando bater seu próprio recorde.

---

## 2. Problema a Ser Resolvido

Desenvolver um jogo de plataforma 2D em que o desafio e a diversão estejam concentrados na precisão dos controles e no domínio da movimentação do personagem.

O foco principal não está em combate, mas em:

- Controle responsivo
- Timing
- Reflexos
- Precisão nos movimentos

---

## 3. Público

Jogadores que apreciam jogos desafiadores com foco em movimentação e exploração, como:

- Celeste
- Hollow Knight
- Super Meat Boy

---

## 4. Características do Jogo

- Movimentação responsiva
- Física precisa
- Ausência de inimigos na versão inicial
- Obstáculos estáticos e móveis
- Cronômetro de tempo
- Armazenamento do melhor tempo (recorde)
- Uma fase inicial

---

## 5. Requisitos Funcionais

### 5.1 Movimentação do Jogador

O jogador deve ser capaz de:

- Mover-se para a esquerda e para a direita
- Pular
- Utilizar coyote time
- Utilizar jump buffering
- Utilizar Fast Fall

### 5.2 Obstáculos

O jogo deve possuir:

- Espinhos
- Buracos
- Plataformas móveis
- Espinhos móveis (opcional)

### 5.3 Progressão

O jogo deve permitir:

- Reinício completo da fase após a morte
- Conclusão da fase ao alcançar a meta
- Registro do melhor tempo

### 5.4 Interface

O jogo deve exibir:

- Menu principal
- Temporizador da fase
- Melhor tempo registrado

---

## 6. Requisitos Não Funcionais

- Desenvolvido em C++ utilizando Qt Creator
- Código modular e orientado a objetos
- Fácil manutenção e expansão
- Estrutura organizada em repositório Git
- Execução fluida e com alta responsividade

---

## 7. Regras do Jogo

- Ao tocar em espinhos, o jogador morre.
- Ao cair em um buraco, o jogador morre.
- Ao morrer, a fase é reiniciada desde o início.
- Ao alcançar a meta, a fase é concluída.
- Se o tempo atual for menor que o recorde armazenado, o novo recorde é salvo.

---

## 8. Elementos do Domínio

### Entidades Principais

- Jogador
- Plataforma
- Plataforma móvel
- Espinho
- Espinho móvel
- Meta da fase
- Fase
- Temporizador
- Recorde

---

## 9. Funcionalidades Futuras

As seguintes funcionalidades poderão ser adicionadas em versões futuras:

- Wall jump
- Dash
- Pogo
- Múltiplas fases
- Sistema de seleção de fases
- Efeitos sonoros e trilha sonora
- Animações avançadas