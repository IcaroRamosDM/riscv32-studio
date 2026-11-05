# Periféricos virtuais: PWM, ADC, SPI e I²C

Abra **Arquivo > Novo projeto > PWM, ADC, SPI e I2C**, compile e consulte
**Terminal e E/S virtual > PWM / ADC / SPI / I2C**. O exemplo também está em
`demos/c/peripherals.c`. Resultados esperados: ADC 3072, SPI 165, I²C 90,
um alvo ausente detectado por NACK, saída `OK` e código de saída zero.

O painel tem uma página por dispositivo, valores atuais separados dos campos
de configuração e botões para aplicar/iniciar. Uma alteração manual é um evento
reversível. Configurar não executa o programa: use Passo ou Executar para avançar.
Os controles de transferência/configuração ficam indisponíveis durante uma
operação ativa. As páginas rolam em janelas pequenas e com fonte ampliada.

## Contrato do modelo

- Um tick por instrução simulada concluída. ECALL atendido pelo ambiente
  didático, edição manual, trap e entrada de IRQ não avançam esses dispositivos.
  O `mtime` de firmware usa outro relógio: uma tentativa/entrada de IRQ.
- Os dispositivos são compartilhados pelos harts. A ordem de execução determina
  os acessos; programas concorrentes devem sincronizar sequências de configuração,
  teste de disponibilidade e transferência. O SDK não torna essas sequências atômicas.
- Voltar/Reproduzir restauram configuração, entrada capturada, linhas SPI,
  fase I²C, resultados e contadores. Reiniciar zera todos esses dispositivos,
  inclusive entradas ADC e o alvo I²C. Salvar preserva operações em andamento,
  futuro de histórico e histórico com eventos antigos descartados.
- Escritas inválidas, dispositivo desativado ou ocupado: nenhuma alteração.
  No ambiente didático retornam falha; acesso MMIO inválido gera a exceção de
  armazenamento correspondente. O SDK verifica argumentos e disponibilidade,
  mas firmware concorrente ainda precisa sincronização.
- Todos os acessos são palavras alinhadas de 32 bits. Leituras/inspeção não
  consomem resultados nem limpam flags. Iniciar a próxima operação limpa DONE/NACK;
  reconfigurar um controlador limpa seu progresso/contadores. Contadores saturam
  em `UINT32_MAX`. Valores de entrada/fixture permanecem ao reconfigurar.
- São modelos didáticos próprios. Não representam um microcontrolador comercial,
  frequências físicas, ruído analógico, tensão ou desempenho de barramento real.
  ADC, SPI e I²C oferecem flags de conclusão e IRQ externa opcional de firmware.

## Interrupções de conclusão

Abra o modelo **Interrupções de ADC, SPI e I2C** ou
`demos/c/peripheral_interrupts.c`. Cada conclusão ADC, última borda SPI ou STOP
I²C registra um sinal pendente: ADC=1, SPI=2, I²C=4. NACK também conclui a
transação I²C e gera seu sinal. Sinais iguais se combinam; não há uma fila de
interrupções para contar conclusões repetidas. Use os contadores dos dispositivos
quando precisar dessa contagem.

Os sinais ficam pendentes mesmo quando mascarados. Escrever 1 em um bit de
IRQ PENDING confirma e limpa somente esse bit (W1C). Leituras não os consomem.
Desabilitar uma máscara ou reconfigurar um dispositivo não limpa sinais antigos.
O reset zera máscaras e pendências e seleciona o hart zero.

`rv_peripheral_irq_enable(mask)`, `rv_peripheral_irq_pending()`,
`rv_peripheral_irq_ack(mask)` e `rv_peripheral_irq_route(hart)` expõem esse
contrato. O destino deve existir entre os harts configurados. No firmware,
`enable & pending` é combinado com a IRQ externa manual do destino em MEIP,
causa 11. Configure `mtvec`, `mie.MEIE` e `mstatus.MIE`; confirme os sinais no
tratador antes de MRET. No ambiente didático há flags, sem traps de firmware.

Máscaras, pendências, roteamento e CSRs derivados seguem Voltar/Repetir, projetos
e C executável exportado. A aba **IRQs de conclusão** do painel permite consultar
e editar esses registradores com histórico habilitado e execução pausada.

## Dispositivos

**PWM:** um canal, período 1–65535 ticks, tempo alto 0–período. A saída é alta
quando fase < tempo alto. Configurar reinicia fase e contador; 0/0 desativa.
Duty zero e duty igual ao período dão níveis constantes.

**ADC:** quatro entradas inteiras 0–4095, resolução de 12 bits. A conversão
captura o canal no comando START; alterações posteriores da entrada não mudam
a amostra pendente. Após 1–65535 ticks, RESULT e DONE são atualizados. O resultado
anterior continua legível durante BUSY. Tempo zero desativa o conversor.

**SPI:** 8 bits, MSB primeiro, quatro modos CPOL/CPHA, um alvo virtual.
Cada uma das 16 bordas ocorre após 1–65535 ticks. CPHA=0 amostra nas bordas
iniciais; CPHA=1 nas finais. CPOL define o nível de repouso. Loopback recebe
MOSI, ou a resposta de oito bits configurada no alvo é capturada ao iniciar.
Não há fila, vários alvos ou sinais elétricos de seleção. O clock volta ao
repouso ao completar. CONFIG=0 desativa (modo zero, loopback desligado).

**I²C:** controlador de transações e um alvo com endereço de 7 bits, entre
0x08 e 0x77, e 16 registradores de um byte. Uma escrita percorre START,
endereço+W, índice de registrador, DATA e STOP. Uma leitura usa START,
endereço+W, índice, START repetido, endereço+R, DATA e STOP; o NACK normal
do controlador que encerra a leitura está incluído na fase de dados e não
é contado como erro. Cada fase dura o número configurado de ticks.

Alvo ausente/endereço diferente ou índice >=16 produzem NACK e STOP, sem
escrever dados. O contador de transações inclui operações rejeitadas pelo alvo.
O fixture do alvo fica bloqueado durante BUSY. A fase DATA conclui a leitura
ou escrita antes da fase STOP. Não são modeladas linhas SDA/SCL, tempos elétricos,
arbitragem entre controladores, clock stretching, endereçamento de 10 bits,
general call ou sequências arbitrárias de bytes. O mapa do alvo e a duração
por fase são convenções deste simulador. A ordem de transação usa como referência
as seções 3.1.4–3.1.6 e 3.1.10 do
[manual UM10204 da NXP](https://www.nxp.com/docs/en/user-guide/UM10204.pdf).

## SDK e mapa

Use `<rv_peripherals.h>` em C RV32/RV64, com GCC ou Clang. As funções são
não bloqueantes; consulte `rv_adc_status`, `rv_spi_status` e `rv_i2c_status`.
Bits de estado: BUSY=1, DONE=2, NACK=4. `rv_i2c_start(..., read=1)` lê um
registrador, incluindo o START repetido. `rv_adc_input`, `rv_spi_reply` e
`rv_i2c_target[_register]` configuram fontes/alvos virtuais para experimentos.

No ambiente didático, ECALL 0x150 lê o offset em a0; 0x151 escreve a1 no
offset a0 e retorna zero/falha (todos os bits de XLEN em 1). Em firmware,
use a base MMIO **0xffff0100**, sem extensão por sinal no RV64. O mapa é
esparso: offsets omitidos são inválidos. A tabela Timer/MMIO exibe os
endereços reais e copia expressões corretas. `periph32(offset)` inspeciona
o mesmo mapa nos dois ambientes; `io32(endereço)` funciona em firmware.

| Offset | Registro | Conteúdo/acesso |
|---|---|---|
| 0x00 | PWM CONFIG | período bits 0–15, alto 16–31; R/W |
| 0x04 | PWM STATUS | fase 0–15, saída bit 16, habilitado 17; R |
| 0x08 | PWM CYCLES | períodos completos; R |
| 0x0c | IRQ ENABLE | máscara ADC=1, SPI=2, I²C=4; R/W |
| 0x10 | ADC TICKS | duração; R/W |
| 0x14 | ADC START | canal 0–3; W inicia, R último canal |
| 0x18 | ADC STATUS | flags 0–1, canal 8–9; R |
| 0x1c | ADC RESULT | última conversão; R |
| 0x20–0x2c | ADC INPUT0–3 | passo de 4 bytes, 12 bits; R/W |
| 0x30 | ADC COUNT | conversões; R |
| 0x34 | IRQ PENDING | sinais de conclusão; R / W1C |
| 0x38 | IRQ HART | núcleo de destino, 0 a harts−1; R/W |
| 0x40 | SPI CONFIG | ticks 0–15, modo 16–17, loopback 18; R/W |
| 0x44 | SPI TRANSFER | W inicia byte, R último TX |
| 0x48 | SPI STATUS | flags 0–1, CLK 8, MOSI 9, MISO 10, bordas 16–20; R |
| 0x4c | SPI RX | último byte recebido; R |
| 0x50 | SPI REPLY | byte do alvo; R/W |
| 0x54 | SPI COUNT | transferências; R |
| 0x60 | I2C TICKS | duração por fase; R/W |
| 0x64 | I2C TARGET | endereço 0–6, presente 8; R/W |
| 0x68 | I2C REQUEST | endereço 0–6, índice 8–15, byte 16–23, leitura 31; W inicia, R último pedido |
| 0x6c | I2C STATUS | flags 0–2, fase 8–10 (1–7; zero inativo); R |
| 0x70 | I2C RX | último byte lido; R |
| 0x74 / 0x78 | I2C COUNT / NACKS | transações / NACKs do alvo; R |
| 0x80–0xbc | I2C REGISTER0–15 | passo de 4 bytes, valor de 8 bits; R/W |

## Persistência, comparação e C exportado

O estado `peripherals` é opcional em projetos antigos (padrão zerado). O formato
v2 contém exatamente 29 palavras decimais canônicas; valida limites, fases,
contagens, progresso e IRQs antes de substituir a sessão. Não grava padding ou
endereços do host. Cada estado ocupa 116 bytes no núcleo. Projetos v1, com 26
palavras, continuam aceitos: os novos registradores iniciam zerados e sinais
pendentes são reconstruídos pelo histórico retido, sem modificar as 26 palavras
antigas ou habilitar interrupções que não existiam.

`$peripherals` compara o estado exato, incluindo temporização. Compilações
O0/O2 podem produzir fases PWM/tempos diferentes e resultados finais iguais;
para comparar resultados use `periph32(0x1c)`, `periph32(0x4c)` e
`periph32(0x70)`. Cada painel da comparação mantém sua própria máquina.
CLI JSON e relatórios da comparação incluem o estado completo.

O C executável exportado usa o mesmo módulo público independente
`runtime/linked/studio_peripherals.[ch]`; não incorpora os fontes privados
do núcleo. O backend executável atende RV32/RV64 e múltiplos harts, incluindo
as IRQs de conclusão de firmware e o mesmo estado de periféricos v2.
