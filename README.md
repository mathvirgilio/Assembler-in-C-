# SB_TP1
# Assembler-in-C++
Implementation in C/C++ of an assembler and linker for the custom assembly defined in class. The project includes source code and a README with authors, OS, and instructions to compile and run. It must use GCC on Linux or CodeBlocks on Windows, with no additional librarie

Repositório para o Trabalho Prático 1 da disciplina de Software Basico da Universidade de Brasília (UnB)

Alunos:
- `190125985` José Henrique de Lucas Zeferino
- `170053032` Matheus Virgilio Ferreira

SO utilizado: Linux Kubuntu 24.04
versão do kernel: `6.8.0-51-generic (64-bit)`

instruções para compilar os arquivos **do montador** separadamente e depois ligá-los:
separadamente:
```
gcc  -c  montador.cpp  -o  objs/montador.o
gcc  -c  src/preprocess_asm_stream.cpp  -o  objs/preprocess_asm_stream.o
gcc  -c  src/assemble.cpp  -o  objs/assemble.o
gcc  objs/montador.o  objs/preprocess_asm_stream.o  objs/assemble.o  -lstdc++  -o  objs/montador
```

```
./objs/montador  asm/exs_slides/ex2_primeira_passagem.asm
./objs/montador  asm/exs_slides/ex2_primeira_passagem.pre
```

com um único comando:
```
gcc  montador.cpp  src/preprocess_asm_stream.cpp  src/assemble.cpp  -lstdc++  -o  montador
```


instruções para compilar os arquivos **do ligador** separadamente e depois ligá-los:
separadamente:
```
gcc  -c  ligador.cpp  -o  objs/ligador.o
gcc  -c  src/preprocess_asm_stream.cpp  -o  objs/preprocess_asm_stream.o
gcc  -c  src/assemble.cpp  -o  objs/assemble.o
gcc  objs/ligador.o  objs/preprocess_asm_stream.o  objs/assemble.o  -lstdc++  -o  objs/ligador
```

```
./objs/ligador  file_1.obj  file_2.obj
./objs/ligador  asm/exs/ex_obj_com_ligacao.obj asm/exs/ex_obj_com_ligacao2.obj
```

- **OBS.:** às vezes, é necessária uma quebra de linha extra para cada arquivo objeto (uma a mais que a da ultima linha do .obj)
- **OBS.:** as LABELS do arquivo ligado não estão no .obj final (chamado asm/exs/objeto_final.obj), mas serão exibidas no terminal


****** ****** ****** ****** ****** ****** ****** ****** ****** ****** ****** ******

# Suposições:

- assume-se que o arquivo `.asm` sempre irá apresentar as linhas `SECTION TEXT\n` e `SECTION DATA\n`
- assume-se que a versão correta do mnemônico da instrução que multiplica o registrador `ACC` é `MUL`
- assume-se que código assembly não irá apresentar operações com vetores - **ex:** `STORE Y + 2`
- assume-se que a diretiva `END` deve vir antes da linha `SECTION DATA`

- montador funciona seguindo o padrão dos slides, i.e. considera-se que na linha `BEGIN: MOD_A`, do exemplo fornecido, há erros e que a sua versão correta deve ser `MOD_A: BEGIN`

- **OBS.:** arquivos de saída serão criados no mesmo diretório em que se encontrava o arquivo de entrada no momento da execução do montador
- **OBS.:** montador não confere se linha que contém diretiva `END`, caso exista, é a linha imediatamente acima da linha `SECTION DATA`

****** ****** ****** ****** ****** ****** ****** ****** ****** ****** ****** ******

## notas de aula:

> **OBS:** implementação do suporte à macros conta como os 3,5 pts da prova.

- apenas usar o `gcc` ou `g++`, para compilar o trabalho
- montador não é *case-sensitive*
- epera-se que o arquivo `.asm` será ligado à outros arquivos `.asm` caso apresente as diretivas `BEGIN` e `END`
- diretiva `SPACE` deve ser representada, no `.obj`, com `0` ou `00` e não `XX`
- maneiras esperadas de executar o montador:
    - se arquivo de entrada for `.asm` - i.e. `./montador file.asm` -, espera-se arq de saída `file.pre` (pré-processado)
    - se arquivo de entrada for `.pre` - i.e. `./montador file.pre` -, espera-se arq de saída `file.obj` (objeto)
- maneira esperada para executar o ligador: `./ligador file1.obj file2.obj file3.obj`


- **OBS.:** é permitido e recomendado alterar o conteúdo lido do arquivo de entrada
- **LEMBRETE:** ler lista de erros

- sobre `MACROS`:
- **OBS.:** assume-se que que, no arquivo de entrada, há um máximo de até 3 definições de `MACROS`
- **OBS.:** assume-se que essas `MACROS` recebem um valor máximo de até 8 argumentos `MACROS`
- **OBS.:** assume-se que, caso haja múltiplas definições de `MACROS` no arquivo `.asm`:
            - uma `MACRO` pode ser invocada dentro do escopo de outra se, e somente se, a `MACRO` invocada tiver sido definida antes daquela que a está invocando
            - o nome dos parâmetros no espoco de definição de uma `MACRO` podem ser repetidos no escopo de definição de outra. ex: `&op1` é repetido em:
            ```
            BEQ:  MACRO &op1, &op2, &LABEL ; branch to &LABEL if &op1 is equal to &op2
                  ...
                  ENDMACRO

            BGEZ: MACRO &op1, &LABEL       ; branch to &LABEL if &op1 is greater than or equal to 0
                  ...
                  ENDMACRO
            ```


****** ****** ****** ****** ****** ****** ****** ****** ****** ****** ****** ******
