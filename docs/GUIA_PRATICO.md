# Do primeiro programa à depuração em RISC-V

O espaço avançado abre por padrão. **Aprenda na prática**, acima do editor,
abre os **Tutoriais e exemplos**. Você também os encontra em **Ajuda**.
Escolher um tutorial mostra as instruções e uma prévia; **Abrir exemplo editável**
cria o rascunho e seleciona sua ISA, ambiente e quantidade de harts.
Fontes não salvas são confirmadas antes da substituição.

## Imprima uma mensagem

[Abra o tutorial Hello World](tutorial:hello). Use **Compilar e carregar**
(**Ctrl+Enter**) e depois **Executar** (**F5**). A mensagem aparece no
**Terminal**, junto à parte inferior da interface.

Compilar prepara o programa; executar produz a saída. O painel **Log de
compilação** contém mensagens do compilador. `printf`, `puts` e `putchar`
escrevem no Terminal. O programa do tutorial termina com código de saída **0**.
Altere a mensagem e repita a compilação para executar a nova versão.

Se o painel estiver fechado, use **Exibir > Painéis > Terminal**. O botão
**Abrir Terminal**, no tutorial, também o revela.

## Converse com scanf

[Abra o tutorial de entrada](tutorial:input). Execute e espere a pergunta.
No campo abaixo da saída do Terminal, digite `Ada` e pressione **Enter**.
Use um nome sem espaços: neste exemplo, `%31s` lê uma palavra.
Na segunda pergunta, digite `7` e pressione Enter. A execução continua
quando a resposta chega. O resultado termina com:

```
Hello, Ada! Next year: 8.
```

O exemplo usa `scanf` com limites de tamanho e valida o texto numérico antes
da conversão. Reinicie e experimente `abc`, `-1` ou `1000` no segundo campo:
o programa explica que esperava um inteiro. **Enviar EOF** informa que não
haverá mais entrada. No primeiro campo, isso encerra o exemplo com código 1;
no segundo, com código 2. Use **Reiniciar** para começar com a entrada aberta.

O Terminal é a entrada e saída do programa simulado. A UART é outro dispositivo;
seu painel mostra comunicação serial, filas e erros de transmissão.

## Veja o que uma linha de C faz

[Abra o tutorial de alterações reversíveis](tutorial:reverse). Com otimização
**0**, compile e carregue. Em **Símbolos**, clique duas vezes em `result` para
revelar seu endereço no painel **Memória**. Use **Avançar instrução** (**F6**)
para acompanhar as escritas.

**Voltar** (**F7**) desfaz uma instrução registrada. **Reproduzir adiante**
(**F8**) reaplica o que já está no histórico. Compare os registradores e os
bytes de `result` antes e depois. Executar até o fim imprime `result = 42`.

Clique duas vezes em uma instrução na tabela Assembly para criar ou remover
um ponto de parada. O programa para antes de executá-la. Para acompanhar as
funções e variáveis locais em C, inicie o depurador C: **F11** entra na função,
**F10** passa por ela e **Shift+F11** retorna ao chamador.

Alterar o editor não altera as instruções já carregadas. O título do painel
informa quando a fonte difere da compilação carregada; compile e carregue de
novo para usar a alteração. O guia só confirma o resultado de referência
quando a fonte correspondente foi carregada.

## Programe diretamente em Assembly

[Abra Hello World em Assembly](tutorial:assembly). O exemplo fornece `_start`
e usa `lbu`, `addi`, desvio e `ecall`. Acompanhe `t0`, `t1`, `a0` e `a7`.
O serviço didático **0x100** imprime o byte em `a0`; o serviço **93** encerra
com o código em `a0`. O número do serviço vai em `a7` neste perfil RV64I.

Esses serviços pertencem ao ambiente **Didático** do Studio. Uma chamada de
sistema Linux como `write` não é automaticamente um serviço deste ambiente.
Uma imagem de firmware pode instalar seu próprio tratamento de traps e usar
os periféricos mapeados em memória.

O montador também expande pseudoinstruções como `la` e `li`. Compare o texto
com a tabela de instruções para enxergar a expansão. Altere a mensagem: a
constante definida pela diferença entre os rótulos acompanha seu tamanho.

[No próximo exemplo, C chama uma instrução Assembly](tutorial:mixed). As
restrições do Assembly inline descrevem os operandos ao compilador. Para
separar o código em um arquivo `.S`, adicione-o ao projeto e marque-o para
compilação. A ISA selecionada precisa conter as instruções utilizadas.

## Explore os dispositivos com um resultado concreto

- [Arquivos virtuais](tutorial:files): cria `result.txt` na RAM simulada. Abra
  **Exibir > Painéis > Arquivos do programa** para ver seus bytes. **Salvar
  arquivo** exporta uma cópia para o local escolhido no notebook.
- [Tela gráfica](tutorial:display): desenha uma diagonal de 64 pixels em uma
  tela de 64 por 32. O botão **Abrir painel de inspeção** revela a tela.
  Avance pelo laço e use Voltar/Reproduzir para acompanhar os pixels na RAM.
- [Threads C17](tutorial:threads): dois harts cooperam com `threads.h` e
  `stdatomic.h`. O modelo **Threads C17** inicia `main` uma vez e o programa
  termina com contador **200**, após a junção da thread.
- [Reserva LR/SC](tutorial:atomic): acompanhe a reserva e a escrita
  condicional. Editar os bytes reservados entre LR e SC invalida a reserva;
  o programa pode então produzir um resultado diferente do de referência.
- [Ponto flutuante](tutorial:floating): compare o resultado **5,25** com os
  bits binary64 e os indicadores de arredondamento. A saída do programa usa
  ponto decimal, como está escrito no código.
- [Vetores](tutorial:vector): carregue quatro elementos, some dez e observe
  `[11, 12, 13, 14]` nos registradores vetoriais e na memória.
- [Interrupções](tutorial:timer): no ambiente **Firmware**, o tratador recebe
  três interrupções do temporizador e imprime `123`. Inspecione `mepc`,
  `mcause` e `mstatus` e volte pela entrada da interrupção.

Os harts são entrelaçados de forma determinística. Isso permite repetir a
execução; uma execução não representa todos os resultados possíveis de uma
máquina física com memória fraca. O temporizador acompanha o progresso
simulado das instruções, não o relógio de um circuito físico.

## Organize e guarde o trabalho

Os controles pequenos na barra de título de cada painel permitem fechá-lo,
mover ou desacoplar para outra janela. Reabra painéis em **Exibir > Painéis**.
**Exibir > Restaurar layout padrão** recupera a disposição inicial;
**Desfazer restauração do layout** recupera sua disposição anterior.

Abra **Exibir > Painéis > Histórico** para consultar os registros recentes.
**Anteriores** e **Próximos** percorrem a lista; **Mais recentes** acompanha as
novas ações. Cada trecho tem até 256 registros e uma linha pode agrupar várias
instruções. O contador informa o intervalo exibido e a posição da máquina.
Use **Voltar** (**F7**) e **Reproduzir adiante** (**F8**) para restaurar estados.
Ao executar ou editar a partir de uma posição anterior, o novo caminho substitui
o futuro registrado.

**Salvar código-fonte** (**Ctrl+S**) guarda o texto do editor. **Salvar projeto
do espaço de trabalho** (**Ctrl+Shift+S**) guarda fontes, configurações,
programa carregado e histórico retido em um arquivo `.rvstudio`. Reabra-o
para continuar a inspeção. Uma cópia exportada para o notebook é um arquivo
independente: Voltar no simulador não modifica essa cópia.

O botão **Próximo exemplo** apenas navega. Leia **Experimente**, execute o
resultado original e depois altere um detalhe por vez em **Explore mais**.
O código continua seu; o guia não o executa nem o substitui automaticamente.
