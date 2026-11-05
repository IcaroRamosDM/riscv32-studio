# Plataforma do workspace avançado

Abra **Arquivo > Workspace RISC-V avançado...**, ou execute o launcher com
`--advanced`. Este workspace usa o modelo Spike isolado; o workspace clássico
continua disponível e conserva seus projetos e biblioteca anteriores.

No painel de registradores, escolha o hart a inspecionar e o escalonamento:
**Todos os harts** alterna entre os núcleos; **Somente o hart selecionado**
executa apenas esse núcleo. Passo avança o hart inspecionado. Voltar e Replay
seguem o histórico global, incluindo RAM, entrada/saída e periféricos.
A preferência de escalonamento também é salva nos projetos.

Use **Exibir > Painéis** para abrir Terminal, Display, UART ou Periféricos.
Todos usam os controles compactos de desacoplamento e fechamento. Restaurar
layout padrão e Desfazer restauração continuam disponíveis. O display aparece
quando o programa configura seu framebuffer; texto e bitmap usam `<rv_display.h>`.

O Terminal recebe `printf`/`puts` e oferece entrada para `scanf`/`getchar`, EOF,
limpeza reversível e exportação da saída retida. A UART de `<rv_uart.h>` é um
periférico separado: configure-a no código, pause, enfileire bytes no painel e
execute para transmitir os quadros. É possível usar hexadecimal e injetar erros
de paridade ou stop. FIFOs, linhas e contadores acompanham o histórico.

`<rv_peripherals.h>` oferece PWM, ADC, SPI, I2C e interrupções de conclusão.
O painel mostra os registradores e permite editar os graváveis enquanto pausado.
Botões virtuais são uma máscara de 32 bits; LEDs são mostrados em hexadecimal.
Os dispositivos são modelos educacionais determinísticos, não medições de hardware.

## C em vários harts

Escolha o modelo de execução antes de compilar:

- **C17 hosted threads** chama `main(argc, argv)` uma vez. Os demais harts
  executam funções criadas por `thrd_create`. Há criação/join/detach, mutexes,
  condições, `call_once`, sleep/yield, TSS e reinicialização de TLS/fenv na
  reutilização de um hart. Até quatro threads executam simultaneamente, uma por
  hart; sem hart livre, a criação retorna `thrd_nomem`.
- **Parallel main on each hart** preserva `main(hart, count)` nos projetos
  paralelos anteriores. Retornar de main ou chamar `rv_hart_exit` conclui um
  hart; `exit`/`_Exit` concluem o processo. Com um hart, o startup usa
  `argc=1`, `argv[0]="program"`.

A escolha fica salva no projeto. Pilha e ELF TLS são particulares de cada hart.
O modo hosted oferece as25 APIs de threads.h e o modelo paralelo continua sendo
uma extensão própria. Veja [Runtime C17](ADVANCED_C17_RUNTIME.md) para limites,
Terminal, atomics e contratos de finalização do processo.

A libc25 usa TLS para errno/estado, locks recursivos por FILE e proteção breve
para a lista de streams. `fflush(NULL)` libera a proteção da lista durante
callbacks e não mantém ponteiros sem posse para FILEs que podem ser fechados.
Por isso, scanf bloqueado em stdin não impede outra thread de escrever stdout.
O IO em blocos preserva posicionamento de append também no fwrite direto.

A plataforma5 oferece atomics escalares e genéricos em I/E sem A, por serviços
indivisíveis do modelo, sem inventar A no perfil escolhido. A plataforma6 oferece
esperas cooperativas por palavras, prazo virtual ou interrupção. Harts aguardando
não executam busy-wait; o avanço virtual de prazo faz parte do histórico.
**Execução > Acordar hart selecionado** permite interromper uma espera depurando.

**Exibir > Painéis > Arquivos do programa** mostra arquivos da RAM do convidado,
com prévia de texto/hex e exportação binária completa. Dados aparecem após flush
ou fechamento; arquivos abertos e removidos também ficam identificados. Isso
não abre arquivos do sistema operacional do notebook.

O relógio C avançado lê o contador de 64 bits sem estado de rollover compartilhado.
Seu tempo é virtual: época UTC 2000-01-01 e um microssegundo por passo do modelo;
não é o relógio de parede nem uma medida de desempenho de hardware.

## Compatibilidade de projetos e SDKs

A revisão de plataforma fica no projeto. Arquivos anteriores mantêm sua semântica;
a revisão4 também compacta steps escalonados com máscara de harts elegíveis.
Assim mais de um milhão de alternâncias não precisa de um registro por instrução.
Os limites de RAM, payload e eventos continuam aplicados; a reexecução reconstrói
cada passo e verifica RAM, registradores, dispositivos e futuro retido.

Para validar um SDK privado sem substituí-lo, o workspace aceita a variável
`RV32_STUDIO_RUNTIME_ROOT` com a raiz que contém runtime/. O compilador verifica
os manifests e registra a raiz no build; o projeto portátil preserva os inputs
do usuário e reconstrói com o SDK selecionado, sem reintroduzir cópias de arquivos
internos da instalação antiga.
