# Determinação do Escalonamento de Carga de Trabalho usando Rate Monotonic

* Alexsandra Macedo Souto 
* Ana Flávia Regis Macêdo 

## Descrição geral
Esse projeto visa desenvolver um sistema para analisar e determinar o escalonamento de uma carga de trabalho em tempo real utilizando o algoritmo Rate Monotonic (RM). A carga de trabalho será fornecida em formato JSON, contendo informações detalhadas sobre um conjunto de tarefas periódicas. O sistema deverá analisar a viabilidade do escalonamento utilizando o RM e sugerir um possível escalonamento baseado em uma heurística apropriada.

## Objetivos do projeto
1. Determinar se o escalonamento de uma carga de trabalho específica é viável usando o algoritmo Rate Monotonic.
2. Sugerir um escalonamento possível para a carga de trabalho utilizando uma heurística baseada na análise das tarefas fornecidas.

## Como rodar

1. Download do projeto:
 ```bash
    git clone -b main https://github.com/alexsandrasouto/str-projeto02
```
2. Navegue até a pasta do projeto:
  ```bash
    cd str-projeto02
```
3. Adicione o arquivo "tasks.json" ao diretório e rode os comandos:
  ```bash
    cmake ..
    make
```
  ```bash
    ./scheduler 
```
* IMPORTANTE: O arquivo "tasks.json" deve estar no meso diretório que o executável.

### Demonstração:
```bash
https://youtu.be/srtsqIlVD2o
```

