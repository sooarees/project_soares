# Implementação

O desenvolvimento do **Royal Knight** começou com a criação temporária da `QGraphicsView` e da `QGraphicsScene`, utilizadas inicialmente para testar os elementos básicos do jogo.

Em seguida, foi criado o personagem e implementada sua movimentação. Nessa etapa foram adicionados o deslocamento horizontal, o pulo, a gravidade e o fast fall, buscando deixar os controles mais responsivos.

Depois, foram desenvolvidas as colisões com as plataformas. Também foram adicionados os sprites e as animações do personagem, junto com os ajustes de sua hitbox para que as colisões correspondessem melhor à parte visível do jogador.

Na sequência, foram implementadas as armadilhas. Foram criados os espinhos e as serras, cada um com suas próprias características visuais e formas de colisão. A serra utiliza uma hitbox circular, enquanto o espinho possui uma área de colisão ajustada ao seu formato.

Com o funcionamento básico da fase pronto, a `QGraphicsView` e a `QGraphicsScene` foram ajustadas para a resolução utilizada pelo jogo e para a execução em tela cheia.

Posteriormente, foram adicionados os portais, responsáveis pela passagem entre as fases carregadas pelo arquivo `fases.json`. Depois disso, foi desenvolvido o menu inicial, contendo os botões para iniciar e fechar o jogo.

A princesa foi adicionada como objetivo final. Ao encontrá-la, o sistema de fim de jogo é executado, apresentando a conclusão da partida. Também foi implementado um cronômetro para registrar o tempo utilizado pelo jogador.

Após a estrutura funcional estar pronta, foram adicionados os elementos gráficos do ambiente, como o fundo das fases e as texturas das plataformas.

Por fim, foram implementados o sistema de vidas, o HUD para sua exibição e a trilha sonora reproduzida durante a partida. O melhor tempo é armazenado com `QSettings`, enquanto os recursos visuais e sonoros são carregados pelo arquivo `res.qrc`.

Durante o desenvolvimento, o código foi reorganizado em classes com responsabilidades específicas, aplicando conceitos de programação orientada a objetos, como herança, encapsulamento e polimorfismo.

O projeto foi desenvolvido em C++ utilizando Qt Widgets, Qt Creator e CMake.

<div align="center">

[Retroceder](projeto.md) | [Avançar](testes.md)

</div>
