# C17 no ambiente avançado

O workspace inclui GCC e Clang para RISC-V, SDKs medlow e medany e GDB 17.1 para depuração de variáveis locais de thread. As variantes do SDK com Anexo K ficam em diretórios próprios.

## Execução e terminal

O ambiente avançado distingue dois modelos de execução:

- **C17 hosted threads**: chama `main(argc, argv)` uma vez e disponibiliza
  `threads.h`. Os demais harts executam as threads criadas por `thrd_create`.
- **Parallel main on each hart**: preserva a extensão anterior, chamando
  `main(hart, count)` em cada hart quando há mais de um. Com um hart,
  `main` recebe `argc=1`, `argv[0]="program"` e `argv[1]=NULL`.

A escolha fica nas configurações de compilação. Assembly controla o próprio
startup e não recebe automaticamente a inicialização C. O modelo escolhido
para C fica indisponível enquanto a linguagem selecionada é Assembly.

O **Terminal**, na parte inferior, mostra stdout/stderr e recebe stdin. É
possível reabri-lo em **Exibir > Painéis > Terminal**. Enquanto uma thread está
em scanf/getchar, outra pode escrever em stdout. A espera por entrada não
executa repetidamente a mesma instrução. EOF é um evento explícito do terminal.

A saída visível é limitada à janela recente de 64 KiB; **Salvar saída** exporta
os bytes ainda retidos no log, cujo limite total é de 64 MiB.

## Threads, atomics e esperas

As 25 APIs de `threads.h` incluem criação/join/detach, mutexes simples,
recursivos e temporizados, condições, `call_once`, sleep/yield e armazenamento
específico de thread com destrutores. Cada reutilização de um hart reinicializa
seu TLS e herda o ambiente de ponto flutuante do criador. Construtores também
podem criar e aguardar threads.

Os limites atuais são explícitos: até quatro harts e uma thread em execução
por hart; 32 registros de threads aguardando join, 32 chaves TSS, 256 mutexes e
256 condições. Sem um hart livre, `thrd_create` retorna `thrd_nomem`. IDs têm
uma geração para impedir que um handle antigo passe a identificar um objeto
recém-criado. Os limites são de recursos desta máquina simulada.

Retornar de `main` encerra o processo com seu resultado. `thrd_exit` encerra
apenas a thread chamadora, inclusive a inicial. A última thread encerra o
processo normalmente com `EXIT_SUCCESS`. Os destrutores TSS seguem até quatro
passagens e não são executados pelo exit normal do processo.

A plataforma 5 acrescentou atomics C17 também a I/E sem A: o serviço do modelo
realiza cada operação de forma indivisível. Isso não inventa a extensão A na
ISA selecionada. Há operações escalares de 1/2/4/8/16 bytes e operações
compostas genéricas, sujeitas aos limites de RAM. Interferências invalidam as
reservas LR/SC dos harts afetados.

A plataforma 6 acrescenta esperas cooperativas por alteração de palavras,
prazo virtual e interrupção habilitada. O registro reavalia as condições para
não perder um despertar. Quando todos os harts aguardam um evento futuro, o
relógio virtual avança sem executar instruções de CPU. Esse avanço faz parte
do histórico, dos projetos e do replay. **Execução > Acordar hart selecionado**
permite interromper uma espera explicitamente durante a depuração.

## Arquivos em RAM

**Exibir > Painéis > Arquivos do programa** lista os arquivos do filesystem
virtual do C, seus tamanhos e descritores abertos. A prévia mostra texto e hex,
limitados a 4 KiB. **Salvar arquivo** exporta todo o conteúdo em blocos limitados,
inclusive bytes NUL e dados binários; a operação protege fontes, cabeçalhos,
bibliotecas capturadas e arquivos do projeto contra sobrescrita.

As escritas em buffer aparecem depois de `fflush`/`fclose`. A inspeção não abre
arquivos do sistema operacional do notebook e não modifica a máquina simulada.
O painel precisa do símbolo do SDK `__rv_fs_nodes`; um ELF arbitrário sem esse
contrato não é interpretado como se tivesse o mesmo filesystem.

A libc31 instalada mantém locks recursivos por FILE. O registro de arquivos tem
proteção própria; o scanner de `fflush(NULL)` usa try-lock e libera a proteção
da lista durante callbacks. A posse do FILE mantém o nó vivo até que o scanner
reobtenha a proteção da lista. Nomes e ponteiros corrompidos são tratados com
limites, sem executar HTML de nomes ou prévias.

A receita 25 acrescenta I/O buffered em blocos. Ela
preserva posições reais de append também no caminho direto de `fwrite`.
Não interpretar esta otimização como mudança no nível de otimização escolhido
para o código do usuário: o SDK usa O2 por padrão, independentemente de O0/O2/etc.

## Seleção do compilador

**Automático (recomendado)** usa GCC normalmente e o Clang privado verificado
quando GCC não aceita um target vetorial válido sem M/Zmmul. O perfil solicitado
não ganha instruções extras. GCC explícito explica essa limitação; Assembly
bruto passa diretamente pelo GNU as e .S usa o frontend selecionado.

Clang21 precisa conservar frame pointer nos frames vetoriais E. Seus helpers
internos do SDK não usam auto-vectorização para evitar um erro reproduzido em
RV32/VLEN65536. Intrínsecos e auto-vectorização do programa do usuário continuam
habilitados. As decisões e flags ficam nos manifestos de compilação.


## TLS, depuração e conversões atuais

O TLS estático é inicializado por hart antes dos construtores. O GDB inspeciona
variáveis `_Thread_local` em I/E32/64 com GCC/Clang, incluindo os harts dos modelos
suportados, sem alterar estado/histórico nas consultas. ELF inconsistente com seu
template TLS é recusado antes de substituir o programa carregado. O campo reservado
p_paddr do PT_TLS não se confunde com o endereço de carga de PT_LOAD.

As bases revisadas otimizam a divisão decimal por 1e9 nos adaptadores de I/O,
preservam o ambiente de ponto flutuante e mantêm um fallback inteiro exato.
O helper Clang preserva os
bytes do Assembly intermediário, inclusive comentários não UTF-8 emitidos pelo
LLVM.
