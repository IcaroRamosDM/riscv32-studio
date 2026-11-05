# Terminal, painéis e ferramentas de investigação

## Hello World e entrada interativa

1. Abra **Arquivo > Novo projeto a partir de modelo > Hello World e entrada
   interativa**.
2. Use **Compilar e carregar** e depois **Executar** (F5).
3. Veja a saída em **Terminal**, na parte inferior da interface. O botão
   **Terminal** da barra ou **Exibir > Terminal** (Ctrl+Alt+J) abre esse painel.
4. Digite seu nome no campo inferior e pressione Enter. Depois digite um inteiro
   e pressione Enter novamente. A opção **Adicionar nova linha** vem marcada.
5. Abra **Exibir > Display gráfico** (Ctrl+Alt+D) para ver a saudação na tela
   virtual. Essa tela também aparece na bancada virtual.

O **Registro de compilação** contém mensagens do compilador. A saída de
`printf`, `puts` e `putchar` aparece no **Terminal**. O display gráfico tem
buffer próprio na RAM; `printf` não desenha automaticamente nessa tela.
Use `<rv_display.h>` para configurar um display de texto ou bitmap monocromático.
Não há janelas gráficas arbitrárias de bibliotecas do sistema operacional.

Exemplo mínimo:

```c
#include <stdio.h>

int main(void)
{
    int numero = 0;
    puts("Hello World!");
    printf("Digite um inteiro: ");
    if (scanf("%d", &numero) == 1)
        printf("Recebido: %d\n", numero);
    return 0;
}
```

Quando a fila acaba, `getchar` e `scanf` aguardam sem executar uma tentativa,
alterar histórico, avançar relógios ou consumir o limite de execução. O Terminal
é aberto e seu campo recebe foco. Uma execução iniciada com Executar continua
ao enviar a entrada; um Passo apenas recebe os dados para o próximo passo.
**Pausar** cancela essa continuação automática. Em vários harts, a espera do
hart selecionado pausa a execução conjunta até receber entrada.

Entradas são bytes UTF-8. O display de texto usa caracteres Latin-1 por célula.
Back/Replay restaura consumo de entrada, saída e o caractere de antecipação de
`scanf`. Reiniciar reutiliza a entrada gravada. Com histórico desativado, o
Terminal continua funcionando, sem reversão de suas alterações.

A versão 1.7 usa a [biblioteca padrão C17](C_STANDARD_LIBRARY.md), incluindo
formatação completa de printf/scanf, ponto flutuante, FILE, arquivos em RAM,
memória dinâmica, strings, Unicode, matemática real e complexa. O modelo
**Biblioteca padrão C17** demonstra essas famílias. **Exibir > Arquivos do
programa** (Ctrl+Alt+F) mostra os arquivos e permite exportá-los.

Use **EOF** ou **Ctrl+D** dentro do Terminal para terminar a entrada. O texto
pendente no campo é enviado antes do EOF. `clearerr(stdin)` permite retomar a
leitura. Use largura em `%s`: `char nome[32]; scanf("%31s", nome);`.
Para validar conversões fora da faixa, use `fgets` com `strtol`/`strtod`, endptr
e errno; a biblioteca padrão não mantém a antiga regra especial de overflow do
SDK mínimo. `rv_getchar` continua não bloqueante, retornando -1 sem dados.

O ambiente didático usa ECALL `0x102` para a leitura bloqueante. Firmware usa
uma leitura de palavra em `0xffff00a0`; a leitura não bloqueante antiga continua
em `0xffff000c`. Uma interrupção habilitada pendente tem prioridade sobre a
espera. No C executável exportado, o estado chama-se `waiting-input`; o runner
aceita `--input arquivo.txt`. A API pode acrescentar dados e retomar `studio_run`.

## Painéis móveis e janelas separadas

Cada setor principal e auxiliar possui uma barra com **…**, **↗** e **×**.
Esses controles são discretos (18–24 pixels lógicos) e respeitam a escala da tela.
Arraste a barra para rearranjar e acoplar painéis ou use **↗** para desafixar.
A janela solta pode ir para outro monitor. **…** oferece desafixar e fechar;
**×** fecha o painel. **Exibir** lista os painéis com marcação de visibilidade.
Fechar um painel preserva seu conteúdo e o estado do programa.

Posição, tamanho, agrupamentos em abas, visibilidade e janelas soltas são salvos
no projeto e nas preferências. Os modos Essencial e Completo preservam suas
disposições separadamente. O layout compacto agrupa os cinco setores principais
e o Terminal acoplado em abas, dando mais espaço a cada um. O botão Terminal
continua abrindo a entrada diretamente. Se um monitor desaparecer, a restauração
reposiciona janelas que ficaram
fora das telas disponíveis. Preferências antigas migram para o novo layout.

## Origem de valores

Selecione um registrador, uma região escalar de memória ou uma variável e use
**Explicar origem do valor...** no botão ou menu de contexto. A árvore mostra a
última escrita registrada e os operandos usados, seguindo dependências anteriores.
Escritas parciais na RAM são examinadas byte a byte; escritas de outros harts
também aparecem. Edições manuais, entradas e leituras de dispositivos são limites
identificados. **Salvar origem em JSON** conserva o relatório.

A análise não modifica a máquina. Examina apenas o histórico retido e aplicado,
até o cursor atual. Estado inicial, eventos descartados, valores sem registro,
operações vetoriais e limites de busca são explicitamente incompletos. Não
reconstrói dependências de controle nem prova a causa lógica de todo o programa.
Há limites de nós, profundidade e eventos examinados. Variáveis compostas,
calculadas ou eliminadas pela otimização não recebem uma origem inventada.

## Primeira divergência entre compilações

Na janela de comparação, pause ambas as máquinas e escolha **Comparação >
Encontrar primeira divergência...**. Reinicie ambas antes para buscar desde o
começo. A busca usa cópias isoladas dos estados pausados e mantém os originais.

- **Bytes de saída do Terminal:** compara o primeiro byte, o segundo e assim
  por diante, mesmo que as versões executem quantidades diferentes de instruções.
- **Marcador semântico:** use uma expressão escalar, por exemplo uma variável
  global `volatile unsigned etapa`, incrementada depois de cada operação
  significativa. Os próximos valores devem coincidir e crescer estritamente.

As observações selecionadas podem incluir expressões, `$stdout`, `$exit`,
`$display`, `$leds`, `$buttons`, `$uart` e `$peripherals`. Sem seleção explícita,
a busca usa saída e código de saída; `$exit` só é comparado após ambas terminarem.
Contagens de instruções nunca são usadas para alinhar programas otimizados.

O relatório localiza o primeiro ponto com diferença nas observações escolhidas,
com PC, núcleo, arquivo/linha disponíveis e tentativas de cada lado. Ele é salvo
com a comparação e no relatório JSON. **Pausar ambas** cancela a busca.
Falta de entrada, observação indisponível, marcador ambíguo, exceção ou limites
atingidos produzem resultado inconclusivo. Ausência de diferença vale somente
para as observações e pontos comparados; não prova equivalência dos programas.

## Bancada virtual

Abra **Exibir > Bancada virtual**. Use **Adicionar componente...** para conectar
um componente a um canal/registrador. Arraste os cartões pela alça **⠿**;
o menu **…** permite renomear, mover por coordenadas ou remover. A configuração
fica no projeto e nas preferências. **Restaurar disposição** refaz os cartões
padrão e não reinicia os dispositivos.

| Componente | Conexão |
|---|---|
| LED | Bit 0–7 da saída GPIO |
| Botão | Bit 0–7 da entrada GPIO; manter ou pressionar temporariamente |
| Potenciômetro ADC | Entrada 0–3, nível inteiro 0–4095 |
| Indicador PWM | Duty, período e nível do canal PWM |
| Sensor SPI | Byte de resposta do alvo virtual |
| Sensor I²C | Um dos 16 registradores de um byte do alvo |
| Display | Buffer de texto/bitmap configurado pelo programa na RAM |

Confirme campos numéricos com Enter ou ao sair do campo; o potenciômetro também
aceita arraste ou teclas. As alterações são eventos de histórico: Voltar desfaz,
Repetir restaura. Transferências ocupadas podem rejeitar mudanças no alvo.
São até 32 cartões com conexões válidas; não se trata de um simulador elétrico
de fios, tensão, ruído ou dispositivos comerciais.

## Interrupções dos periféricos

Abra o modelo **Interrupções de ADC, SPI e I2C**. Ele configura um tratador
M-mode e recebe quatro conclusões, incluindo uma transação I²C com NACK.
O painel **Terminal > PWM / ADC / SPI / I2C > IRQs de conclusão** mostra máscaras,
sinais pendentes e núcleo de destino. Os detalhes e o mapa estão em PERIPHERALS.md.

Conclusões ficam pendentes mesmo quando mascaradas. O SDK oferece
`rv_peripheral_irq_enable`, `rv_peripheral_irq_pending`, `rv_peripheral_irq_ack`
e `rv_peripheral_irq_route`. Firmware entrega a combinação habilitada como
interrupção externa MEIP, causa 11; o tratador deve confirmar os sinais e o
programa deve configurar `mtvec`, `mie.MEIE` e `mstatus.MIE`. No ambiente didático
é possível consultar os sinais, sem entrega de traps de firmware.

## Restaurar a organização da interface (1.8)

**Exibir > Restaurar layout padrão** reacopla os painéis e restaura a posição
e a visibilidade padrão do modo atual. No modo Essencial, mantém as abas e as
configurações recolhidas; no modo Completo, adapta a disposição ao tamanho da
janela. O programa, o texto em edição, a RAM e o histórico são preservados.
**Exibir > Desfazer restauração do layout** recupera a disposição imediatamente
anterior. A opção de desfazer vale para o modo atual e é encerrada ao trocar
de modo. A organização restaurada é salva nas preferências desse modo.

**Exibir > Painéis** e **Exibir > Barras de ferramentas** reabrem elementos
fechados ou ocultos. O botão ↗ desacopla um painel; ↙ o acopla novamente.
As janelas desacopladas seguem a ordem normal de ativação e são minimizadas
junto com a janela principal. Menus e confirmações podem receber foco e ficar
à frente delas.

Em **Arquivos do programa**, a tabela mantém a seleção quando os arquivos não
mudaram. Durante a execução, é preciso pausar para atualizar ou exportar.
Ao pausar, editar a RAM ou usar Voltar/Repetir, os bytes são lidos novamente.
