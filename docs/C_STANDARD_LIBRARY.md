# Biblioteca C17 do RISC-V Studio

O SDK privado do workspace avançado acrescenta oito ABIs, TLS e locks reais
para uso em vários harts. Veja [Plataforma avançada](ADVANCED_PLATFORM.md) para
a interface paralela, periféricos e evidências. O [runtime C17 avançado](ADVANCED_C17_RUNTIME.md)
implementa threads.h no modelo hosted e atomics também em I/E sem A, com
limites de recursos explícitos e oito ABIs.
As instruções introdutórias abaixo referem-se ao workspace clássico.

A versão 1.7 substitui o stdio mínimo por Picolibc 1.8.12, incluído no projeto
com fontes, licenças, receita reproduzível e hashes. A compilação usa essa
biblioteca convidada em GCC e Clang, com ABI ILP32 (RV32) ou LP64 (RV64).
Não é preciso instalar uma libc separada para escrever os programas.

## Experimentar

Abra **Arquivo > Novo projeto a partir de modelo > Biblioteca padrão C17**.
Use **Compilar e carregar**, **Executar** e digite um número decimal com ponto.
O exemplo usa scanf para double, sqrt/fabs, snprintf, malloc/free, strings e
fopen/fputs/fgets/fclose. Veja o arquivo em **Exibir > Arquivos do programa**
(Ctrl+Alt+F); selecione-o e use **Salvar arquivo virtual...** para exportar os bytes.

Os controles **… / ↗ / ×** dos painéis têm agora tamanho próprio de 18–24 pixels
lógicos, sem o espaçamento dos botões de ação. O dimensionamento do sistema
continua aplicado para telas com escala maior.

Executar inicia na velocidade Máxima. Para acompanhar cada instrução, use Passo
ou escolha uma velocidade menor. O limite inicial é de um milhão de instruções;
ao atingi-lo, Executar continua do ponto atual.

Novos projetos usam **256 KiB de RAM e 16 KiB de pilha**. Projetos existentes
preservam suas escolhas. Se o compilador informar que código/dados e pilha se
sobrepõem, aumente a RAM nas configurações de máquina e recompile. A pilha não
é uma área protegida; recursão e arrays locais precisam de espaço suficiente.

## Famílias implementadas

| Cabeçalho | Funções e recursos |
|---|---|
| stdio.h | printf/fprintf/snprintf e variantes va_list; scanf/fscanf/sscanf e variantes; FILE, stdin/stdout/stderr, leitura, escrita, buffering, posição, EOF e erros |
| stdlib.h | malloc/calloc/realloc/free/aligned_alloc, conversões inteiras e reais, qsort/bsearch, rand/srand, exit/atexit/quick_exit/at_quick_exit/_Exit/abort, ambiente e conversões multibyte |
| string.h / ctype.h | Memória, strings, busca, comparação, classificação e transformação de caracteres |
| math.h / tgmath.h | Funções reais em float, double e long double; classificações e seleção por tipo |
| complex.h | Funções complexas nos três tipos reais |
| fenv.h | Ambiente de ponto flutuante; capacidades conforme a ISA e as limitações descritas abaixo |
| inttypes.h | Conversões e formatos dos inteiros de largura definida |
| wchar.h / wctype.h / uchar.h | Strings e I/O largos, classificação, multibyte e UTF-16/UTF-32 |
| locale.h | Locale C/POSIX e C.UTF-8; seleção e consulta |
| time.h | Calendário, conversões, formatação, clock/time/difftime/timespec_get |
| setjmp.h / signal.h | setjmp/longjmp, signal/raise e sinais C |
| stdatomic.h | Operações C11/C17 nos perfis com A, incluindo tipos largos e agregados por fallback LLVM |
| Demais cabeçalhos C17 | assert, limites, tipos, bool, alignas/alignof, va_list/va_copy, noreturn e operadores alternativos |

Os SDKs derivados incluem as 68 interfaces opcionais do Anexo K. As APIs de threads dependem do modelo hosted do ambiente avançado.
Declaração, ligação e contratos de execução são evidências distintas;
setjmp, que pode ser macro, é verificado por execução. Isso complementa os
contratos de comportamento e não constitui certificação formal ISO.

## Terminal e formatação

O botão **Terminal** ou Ctrl+Alt+J abre stdout/stderr e o campo de entrada.
A saída dos dois streams aparece na mesma área, mantendo objetos FILE distintos.
`printf` aceita flags, largura, precisão, comprimentos, inteiros largos, ponto
flutuante, hex float, caracteres/strings largos e `%n`. A família scanf inclui
float/double/long double, scansets `%[...]`, larguras, supressão e `%n`.

Quando stdin precisa de dados, a máquina aguarda sem consumir instruções,
tempo virtual ou histórico. Enviar entrada retoma uma execução que estava
aguardando. Use **EOF** ou **Ctrl+D** dentro do Terminal para encerrar a leitura.
O evento é representado pelo byte EOT (0x04) na fila do console; o stdio o
interpreta como EOF. `clearerr(stdin)` permite uma nova leitura posteriormente.
As funções de baixo nível rv_getchar/rv_service continuam vendo bytes brutos.

`fgets` permite leitura limitada por tamanho. Para validar números fora da faixa,
use fgets com strtol/strtoll/strtod e verifique endptr e errno. O scanf padrão
não promete o comportamento especial de rejeição de overflow do antigo SDK
mínimo. Use largura com `%s` e scansets para respeitar o tamanho do destino.

## Arquivos e memória

Os nomes usados em fopen referem-se ao filesystem em RAM da máquina simulada.
Há até 64 arquivos/slots e 61 descritores de arquivo, além de stdin/out/err;
comprimentos e quantidades também ficam limitados pela RAM disponível. São
implementados modos r/w/a, +, b e x, truncamento, append, seeks, posição,
remove/rename, arquivos temporários, buffering e erros.

**Arquivos do programa** mostra nomes, tamanhos e prévia de texto ou bytes.
Escritas buffered aparecem no armazenamento depois de fflush/fclose. Salvar
exporta todos os bytes, mesmo quando a prévia é truncada. Arquivos, cursores,
FILE, heap e buffers fazem parte da RAM: projetos salvos os conservam;
Back/Replay os reverte no histórico retido; Reiniciar volta ao estado inicial.

O heap ocupa o espaço entre a imagem carregada e a reserva de pilhas. Falhas de
alocação retornam NULL/errno. A execução continua sujeita aos limites de RAM,
saída (64 KiB), histórico e instruções do simulador.

## Ambiente e limites declarados

- main recebe argc=1 e argv[0]="program" no modo de um hart; retornar de main
  passa por exit, callbacks e finalização dos streams. O modo de vários harts
  mantém a extensão de startup anterior, com main(hart, count) e saída por hart.
- O ambiente convidado contém LANG=C.UTF-8, TZ=UTC0 e RISCV_STUDIO=1. O locale
  inicial C permanece o exigido pela linguagem; setlocale(LC_ALL, "") escolhe
  o ambiente virtual. Número decimal usa ponto. Outros locales podem falhar.
- O tempo é determinístico: época UTC 2000-01-01, um tick/instrução aposentada
  equivale a um microssegundo didático. Datas e clock seguem esse relógio.
- system(NULL) informa ausência de processador de comandos; system(comando)
  retorna -1/ENOSYS. Sinais são entregues por raise, sem sinais externos do host.
- Nos perfis sem D, double usa operações de software de 64 bits; com D, o
  compilador pode emitir operações binary64. long double usa software de 128 bits.
  As bibliotecas pré-compiladas de matemática preservam o ABI soft-float e podem
  usar software internamente. Variantes da libc com F usam operações binary32. rint/nearbyint/lrint
  e llrint respeitam a direção anunciada também nos tipos de software. A libc
  não promete conformidade completa com os anexos IEC 60559 F/G nem mudança
  de arredondamento de toda operação soft-float do compilador.
- Atomics requerem A; nos demais perfis __STDC_NO_ATOMICS__ é definido. O fallback
  largo usa locks em RAM, sem threads do host. Harts são hardware virtual,
  não uma implementação do scheduler ou da API threads.h de C11.
- Threads/TLS opcionais de C11 não são oferecidos pelo workspace clássico
  (__STDC_NO_THREADS__=1). O modelo hosted do workspace avançado oferece essas APIs.
  A biblioteca clássica usa estado compartilhado;
  programas com vários harts devem sincronizar chamadas que compartilham esse estado.
- A referência é ISO C17. APIs POSIX, WinAPI, conio.h, bibliotecas gráficas do
  sistema e extensões de outros padrões têm contratos próprios. O display do
  simulador continua disponível por rv_display.h e Exibir > Display gráfico.

## Interfaces opcionais do Anexo K

Os SDKs atuais acrescentam as 68 interfaces do Anexo K de C17. Para obter suas
declarações, defina `__STDC_WANT_LIB_EXT1__` como 1 antes de incluir os cabeçalhos.
O valor deve permanecer igual em todas as inclusões. O compilador da IDE anuncia
`__STDC_LIB_EXT1__=201112L`; `RSIZE_MAX` limita os tamanhos aceitos por essas APIs.

```c
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>

int main(void)
{
    char name[32];

    printf_s("Your name: ");
    fflush(stdout);
    if (scanf_s("%31s", name, (rsize_t)sizeof(name)) != 1) {
        puts("No name was read.");
        return 1;
    }

    printf_s("Hello, %s!\n", name);
    return 0;
}
```

Em `scanf_s`, cada conversão não suprimida `%c`, `%s` ou `%[` recebe um ponteiro
e, logo depois, o número real de elementos do destino, com tipo `rsize_t`.
Um buffer insuficiente causa falha de correspondência; não autoriza escrever
fora dele. O tamanho de um destino `wchar_t` conta elementos, não bytes.
Essas interfaces não conseguem descobrir o tamanho verdadeiro de um objeto a
partir de um ponteiro; o programa deve informar esse tamanho corretamente.

`snprintf_s` e `snwprintf_s` permitem truncamento e retornam o comprimento que
seria necessário. `sprintf_s` e `swprintf_s` tratam falta de espaço como violação
de contrato. O especificador `%n` é proibido na saída formatada dessas APIs;
permanece disponível na entrada. `%%n` representa texto literal.

As funções de cópia, concatenação, tokenização, memória, conversão multibyte,
tempo, ambiente, ordenação e arquivos também seguem os contratos do Anexo K.
As assinaturas são as de ISO C17, incluindo `gmtime_s(timer, result)` e
`localtime_s(timer, result)`, que retornam `struct tm *`.

Violações chamam o tratador registrado por `set_constraint_handler_s`. A limpeza
exigida do destino ocorre antes dessa chamada. O tratador padrão encerra o
programa convidado; `ignore_handler_s` permite tratar o valor de retorno e
`abort_handler_s` solicita encerramento. Um tratador pode retornar ou usar
`longjmp`; os locks internos são liberados antes da chamada.

Os arquivos continuam pertencendo ao sistema de arquivos virtual isolado do
programa. O modificador `u` dos modos de abertura é aceito, sem criar usuários
ou permissões do sistema operacional dentro desse ambiente. Prefira `tmpfile_s`
a criar um arquivo posteriormente a partir de `tmpnam_s`: entre obter o nome e
criá-lo, outro código do programa pode usá-lo.

As bibliotecas anteriores e os SDKs capturados em projetos continuam preservados.
As implementações novas estão em `runtime/annex_k`; o gerador verificável é
`tools/prepare_annex_k.py`. O inventário e os relatórios distinguem os testes
finitos realizados de uma certificação universal da norma.

Fontes e proveniência: `third_party/picolibc/`, `third_party/compiler-rt/`,
`tools/prepare_libc.py`, `tools/prepare_atomics.py` e os manifests em runtime/libc.
O uso normal da biblioteca é offline. A receita corrige flush/append, reserva espaço para callbacks internos e o macro I
para Clang C17 estrito; a camada convidada corrige arredondamento dinâmico dos
reais de software e implementa os serviços da máquina virtual.
