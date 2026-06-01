# Análise orientada a objeto

## Descrição Geral do domínio do problema

O projeto consiste no desenvolvimento de um jogo de plataforma 2D com foco em movimentação precisa e responsiva. O jogador deve percorrer uma fase repleta de obstáculos e alcançar o ponto final no menor tempo possível para salvar a princesa.

A proposta do jogo é priorizar a movimentação, inspirado em jogos como Celeste, Hollow Knight e Super Meat Boy. Na versão inicial, o jogo não contará com inimigos, concentrando o desafio em obstáculos como espinhos, buracos e plataformas.

Ao morrer, o jogador retorna ao início da fase e pode tentar novamente até completar o percurso. O sistema registra o melhor tempo obtido, permitindo ao jogador buscar novos recordes.

### Requisitos Funcionais

- Permitir movimentação horizontal do personagem.
- Permitir que o personagem realize saltos.
- Aplicar gravidade ao personagem.
- Detectar colisões com plataformas.
- Detectar colisões com obstáculos.
- Reiniciar a fase quando o jogador morrer.
- Encerrar a fase ao alcançar a meta.
- Exibir o tempo atual da fase.
- Armazenar e exibir o melhor tempo obtido.

### Requisitos Não Funcionais

- Desenvolvido em C++ utilizando Qt Creator.
- Utilizar programação orientada a objetos.
- Código modular.
- Alta responsividade dos controles.
- Possibilidade de expansão futura para novas mecânicas.

### Funcionalidades Futuras

- Wall jump.
- Dash.
- Fast Fall
- Múltiplas fases.
- Efeitos sonoros e música.
- Sistema de seleção de fases.

---

## Diagrama de Casos de Uso

### Ator

- Jogador

### Casos de Uso

- Iniciar jogo
- Controlar personagem
- Reiniciar fase
- Concluir fase
    - Salvar Melhor Tempo
- Visualizar melhor tempo


### Representação visual

<img src="img/CasosdeUso.png" 
     width="70%" 
     style="padding: 10px">



## Classes

Primeira análise: 

<img src="img/ClassAnalysis.png" 
     width="70%" 
     style="padding: 10px">


<div align="center">

[Retroceder](README.md) | [Avançar](projeto.md)

</div>
