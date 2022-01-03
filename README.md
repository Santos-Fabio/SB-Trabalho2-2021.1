# Trabalho 2 - Arualização do Montador, Carregador e Calculadora

Aluno: Fábio Rodrigues de Andrade Santos - 16/0151783

## Introdução
O trabalho consiste em implementar em C/C++ uma alteração do montador do trabalho anterior
para uma saída com cabeçalho mais completo e criar um carregador que possa carregar 1 a 3
arquivos objetos. Por fim a implementação de uma calculadora em Assembly IA-32.

## Objetivo
Fixar‌ ‌o‌s conceitos aprendidos na aula sobre programação em Assembly e processos de carregamento.

## Compilação
O desenvolvimento do código foi realizado no ambiente linux 64 bits, com o SO Ubuntu versão 18.04.5 LTS. O compilador utilizado nos dois códigos, montador.cpp e carrgador.cpp, foi o g++, seguindo o seguintes comandos respectivamente:

```bash
g++ montador.cpp -o montador
```

```bash
g++ carregador.cpp -o carregador
```

Caso o computador utilizado não tenha suporte para versões mais recentes do c++, será necessário a seguinte diretiva de compilação:

```bash
-std=c++17
```

Para o código em Assembly foram utilizados os comandos de compilação e ligação, respectivamente:
```bash
nasm -f elf -o calculadora.o calculadora.asm
```

```bash
ld -m elf_i386 -o calculadora calculadora.o
```
Para computadores Linux nativos de 32 bits, pode ser usado:

```bash
ld -o calculadora calculadora.o
```
## Execução
Execução do Montador:
```bash
./montador -r 1 myprogram.asm
```
Onde "-r x" deve estar antes do nome do programa e x só pode assumir os valores 0, caso a informação de realocação seja mapa de bits e 1 para lista de endereços.

Carregador:
```bash
./carregador‌ ‌myprogram1.obj‌ ‌myprogram2.obj‌ ‌3‌ ‌15‌ ‌10‌ ‌12‌ ‌1000‌ ‌2500‌ ‌5000
```
 Os programas são passados antes do conjunto de números, neste caso são 2 programas a serem carregados. Os números seguem a ordem: Quantidade de chuks de memória
disponível; Tamanhos de cada um dos chunks disponíveis; Endereços onde começa cada chunk. Para todos os casos são usados números decimais 

Calculadora:
```bash
./calculadora
```
A calculadora executa, porém só foi implementado as operações de soma, subtração e finalização do programa
