/*
 * Lista de testes para o módulo ButtonHandler
 * Objetivo: Detectar quando um botão troca de estado e enviar a nota sendo pressionada/solta
 * para o módulo synth
 * 
 * Iniciar ButtonReader (parte que le os pinos no hardware) durante inicializacao (open)
 * Fechar ButtonReader durante fechamento
 * 
 * 8 Botoes (0-7) iniciados como abertos
 * Botoes inexistentes (8-...) retornam estado de erro
 * 
 * Pressionar botao troca estado para pressionado
 * Soltar botao troca estado para aberto
 * Pressionar N botoes troca os estados corretamente
 * Soltar N botoes trocam o estado corretamente
 * 
 * Retorna estado alterado (1)o apos pressionar botao da primeira vez que funcao e chamada
 * Retorna estado inalterado (0) da segunda vez que funcao e chamada
 * Retorna estado alterado apos soltar botao
 * Retorna estado inalterado apos segunda chamada da funcao
 * 
 * 
*/