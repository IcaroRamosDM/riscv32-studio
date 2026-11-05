# Limites dos perfis ISA

O RISC-V Studio usa um modelo funcional para executar programas, inspecionar o estado da máquina e reproduzir o histórico. As extensões selecionadas definem as instruções e os registradores que o programa pode usar.

## Tempo, memória e recursos

- **Zkt e Zvkt** descrevem a independência entre dados e tempo de execução para determinadas instruções. Os contadores do Studio medem passos virtuais. O tempo do programa no computador depende também do sistema operacional e das bibliotecas do anfitrião.
- **RVWMO e Ztso** tratam da ordem dos acessos à memória. O escalonador executa os harts em série. Esse modelo permite observar fences, operações atômicas e reservas. Em hardware, buffers de escrita e reordenação especulativa podem produzir outras sequências.
- **Ssqosid** expõe os identificadores de recursos de `srmcfg`. A disputa por largura de banda e um controlador de QoS externo ficam fora do modelo.
- **Zicbop, Zihintntl e Zihintpause** permitem que o modelo execute hints sem alterar o cache ou o consumo de energia. Uma espera com **Zawrs** pode terminar imediatamente por um despertar espúrio.

## Ponteiros e sementes

Em RV64, **Sspm e Supm** usam `Smnpm` e `menvcfg.PMM` para S/HS, e `Ssnpm` e `senvcfg.PMM` para U. Em um sistema MU, `menvcfg.PMM` também controla U. O registrador `henvcfg.PMM` controla VS. Cabe ao software salvar e restaurar o contexto de `senvcfg` para VU. O endereço `0x20a` não contém um CSR `vsenvcfg`. O mascaramento depende do mecanismo habilitado e está disponível apenas em RV64.

**Zkr** obtém amostras do sistema anfitrião. O simulador aplica o formato ES16 e as permissões USEED/SSEED. Cada leitura efetiva fica no histórico para que Voltar, Avançar e reabrir um projeto reproduzam o valor original. A simulação não mede a entropia de um gerador físico.

## Contadores, permissões e históricos

Contadores HPM ausentes podem retornar zero (WARL). Os bits OF expõem a interface de overflow. Os seletores indiretos ausentes podem gerar traps nos casos reservados ou não especificados pela arquitetura.

Antes de modificar a RAM, `cbo.zero` confere as permissões do bloco inteiro. A ISA deixa alguns casos de permissões PMP mistas sem comportamento definido. Se uma mudança de semântica impedir a reprodução de um histórico antigo, abra o projeto com a versão do motor que o gravou. O Studio preserva o arquivo original.

## Referências

As especificações descrevem as extensões de [criptografia escalar](https://docs.riscv.org/reference/isa/v20260120/unpriv/scalar-crypto.html),
[criptografia vetorial](https://docs.riscv.org/reference/isa/v20260120/unpriv/vector-crypto.html),
[RVWMO](https://docs.riscv.org/reference/isa/v20260120/unpriv/rvwmo.html),
[Ztso](https://docs.riscv.org/reference/isa/v20260120/unpriv/ztso-st-ext.html),
[máscara de ponteiros](https://docs.riscv.org/reference/isa/v20260120/priv/zpm.html),
[hipervisor](https://docs.riscv.org/reference/isa/v20260120/priv/hypervisor.html)
e [CSRs indiretos](https://docs.riscv.org/reference/isa/v20260120/priv/indirect-csr.html).
