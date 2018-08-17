# Nephesh

Nephesh é um jogo de aventura, focado em lutas contra chefes (boss fights). 

Historia:
O jogo se passa em um universo fantástico, onde as civilizações habitam seres colossais e tem seus cotidianos atrelados à eles. A existência
dos gigantes está diretamente atrelada a magia, e as civilizações de diferentes estão em constante guerra entre si, devido à motivos religiosos.
O personagem é um ser misterioso, que o jogador não sabe nem seus objetivos e nem a sua origem, a principio. O objetivo encontrado na fase desenvolvida
é destruir a consciência do colosso (boss da fase), conforme explicado por um NPC.

Programadores:
	- Fernando Nunes: sistema de layers, sistema de mapas renderizados dinamicamente, projetil do player, dash, algoritmo de colisão do player com o mapa, efeitos de fade da tela.
	- Fabio Marques: sistema de movimentação do personagem, ataque de palmas do boss (colunas que se erguem do chão), melhorias no sistema de musica (fade da musica dependendo do contexto do jogo).
	- Bruno Rodrigues: Melhorias no sistema de colisão. Sistema de colisão com o mapa. Ataques de slam (pedras que caem do ceu) e slap (ataque proximo).

	Todos os programadores participaram das fases de integração das animações e sons com os sistemas implementados e da elaboração dos sistemas apresentados acima.

Dependencias:
	- Bibliotecas SDL utilizadas no desenvolvimento dos 7 trabalhos iniciais. O programa pode ser compilado com o makefile fornecido pela disciplina. Compatibilidade com o Windows não é garantida.

Controles:
	- Shift esquerdo: dash
	- WASD: movimentação
	- Espaço: falar com NPCs
	- Botão direito do mouse: Ataque mágico
	- Botão esquerdo do mouse: Ataque com a espada

Hacks disponiveis:
	Invencibilidade do player: tecla I -> player não toma dano
	Easy mode: tecla L -> o ataque magico pode acertar o boss enquanto ele não está atacando

Breve descrição da batalha com o Boss:
	É possivel acertar o ataque mágico somente enquanto o boss está usando o ataque de clap, se você acerta-lo nessa janela de tempo, ele vai ficar vulneravel, podendo tomar dano pela espada.
	No easy mode, é possível acertar o ataque mágico enquanto ele está em IDLE (ou seja, na maior parte do tempo).

Link do github: https://github.com/ftfnunes/Nephesh