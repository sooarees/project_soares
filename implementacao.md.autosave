# Implementação

Processo de construção do jogo, da base gráfica até os recursos finais.

## Estrutura inicial

O desenvolvimento do **Royal Knight** começou com a criação temporária da `QGraphicsView` e da `QGraphicsScene`, utilizadas inicialmente para testar os elementos básicos do jogo.

## Jogador e movimentação

Em seguida, foi criado o personagem e implementada sua movimentação. Nessa etapa foram adicionados o deslocamento horizontal, o pulo, a gravidade e o fast fall, buscando deixar os controles mais responsivos.

Depois, foram desenvolvidas as colisões com as plataformas. Também foram adicionados os sprites e as animações do personagem, junto com os ajustes de sua hitbox para que as colisões correspondessem melhor à parte visível do jogador.

## Obstáculos e colisões

Na sequência, foram implementadas as armadilhas. Foram criados os espinhos e as serras, cada um com suas próprias características visuais e formas de colisão. A serra utiliza uma hitbox circular, enquanto o espinho possui uma área de colisão ajustada ao seu formato.

## Cena e fases

Com o funcionamento básico da fase pronto, a `QGraphicsView` e a `QGraphicsScene` foram ajustadas para a resolução utilizada pelo jogo e para a execução em tela cheia.

Posteriormente, foram adicionados os portais, responsáveis pela passagem entre as fases carregadas pelo arquivo `fases.json`. Depois disso, foi desenvolvido o menu inicial, contendo os botões para iniciar e fechar o jogo.

A princesa foi adicionada como objetivo final. Ao encontrá-la, o sistema de fim de jogo é executado, apresentando a conclusão da partida. Também foi implementado um cronômetro para registrar o tempo utilizado pelo jogador.

## Recursos visuais e sonoros

Após a estrutura funcional estar pronta, foram adicionados os elementos gráficos do ambiente, como o fundo das fases e as texturas das plataformas.

Por fim, foram implementados o sistema de vidas, o HUD para sua exibição e a trilha sonora reproduzida durante a partida. O melhor tempo é armazenado com `QSettings`, enquanto os recursos visuais e sonoros são carregados pelo arquivo `res.qrc`.

## Ferramenta de apoio

Também foi criado o arquivo `preview_fases.html`, usado como ferramenta de apoio para visualizar e ajustar as fases. Ele permite carregar o JSON das fases, visualizar os elementos no cenário, testar posições e exportar o conteúdo atualizado para o arquivo `fases.json`.

## Organização do código

Durante o desenvolvimento, o código foi reorganizado em classes com responsabilidades específicas, aplicando conceitos de programação orientada a objetos, como herança, encapsulamento e polimorfismo.

---

<div align="center">

[Retroceder](projeto.md) | [Avançar](testes.md)

</div>
