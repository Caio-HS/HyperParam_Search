# Laboratório de Comportamento de Redes Neurais MLP Feedforward

## Resumo 

Este projeto visa construir uma aplicação capaz de testar sistematicamente diversas configurações de redes neurais contra um mesmo problema, a fim de encontrar uma configuração ideal frente a algum critério (Não definido no projeto, no momento). Esse critério deve ser definido como alguma relação entre algumas métricas, sendo estas: tempo de treino, tempo de execução, tamanho da rede (número de parâmetros), erro quadrático médio (MSE) e erro de categorização (Bit error).

## Arquitetura do sistema

A aplicação será desenvolvida em 3 camadas principais, podendo ser nomeadas de Execução (Worker), Orquestração (Orquestrador de workers) e Registro (Banco de dados).

### O Worker:

O Worker é a unidade básica do sistema; é ele quem treinará a rede e coletará as métricas. Ele, até o momento, fará isso uma vez por instância. Isso quer dizer que será iniciado com um conjunto de parâmetros, treinará essa rede específica coletando certas métricas e retornará o resultado das métricas no fim da execução.

C foi a linguagem escolhida pois é a que dá o máximo de controle da execução e o menor overhead, o que é importante para a eficiência e, mais importante ainda, tendo em vista o que será mencionado na seção "Ambiente".

### O Orquestrador

O Orquestrador será quem definirá os parâmetros, orquestrará os Workers e intermediará o salvamento dos dados no banco. Ele é mais próximo de uma estrutura intermediária, criada para isolar o trabalho do Worker e garantir foco completo em eficiência no seu desenvolvimento. 

Como ele representará, idealmente, uma pequena fração do processo computacional, Python foi a linguagem escolhida devido à sua simplicidade e variedade de ferramentas (a despeito do overhead do GIL), sendo adequado para uma "camada de controle" por ser fácil de expandir no futuro em qualquer direção. Se algum dia alguém, além de mim, vier a trabalhar nesse projeto, é ideal que não seja necessário amplos conhecimentos de C ou gerenciamento de memória para ser capaz de adicionar algo ao workflow.

### O Banco de Dados

O banco de dados será onde os resultados serão salvos, junto com os parâmetros que os geraram. O objetivo disso é manter o histórico da execução para análise futura (como é possível observar, a análise não está incluída no escopo do projeto). 

Devido à simplicidade dos dados e do responsável por guardá-los (sendo apenas um único orquestrador em Python) e questões que serão mencionadas na seção "Ambiente", o SQLite3 foi escolhido como o banco de dados.

### Expansões

A ideia das camadas é para que expansões futuras tenham complexidade controlada. Por exemplo, não existe inteligência na escolha dos parâmetros a se testar prevista no projeto; isso pode vir de uma nova camada que consome dados do banco de dados e alimenta o orquestrador que interage com o C.

Ou talvez computação em cluster seja uma opção adotada no desenvolvimento desse projeto. Dessa forma, o código terá que alterar o que o orquestrador orquestra (deixará de ser os Workers, e agora serão nós com Workers), mas isso pode ser feito sem alterar o banco de dados, a lógica na escolha dos parâmetros e nem mesmo o próprio Worker.

## Interfaces existentes:

Para a arquitetura em camadas funcionar, é necessário que exista um "contrato" entre as camadas, de forma que uma camada possa ser substituída por outra implementação sem afetar como as camadas adjacentes são implementadas, desde que essa nova implementação respeite o "contrato" antigo.

### Orquestrador X Banco de dados



### Orquestrador X Worker

O orquestrador deve passar, por parametro, para o worker os parametros da rede a ser treinada. Listarei eles em ordem:
1. O Numero de camadas, contando com a de entrada e a de saída. (unsigned int)
2. Grau de densidade da rede (quantos % das conexões possiveis entre neuronios vão existir), acima de 99.5% a rede é considerada densa (Arredonda para 100% internamente). (float)
3. Tipo de rede, existe no momento a em camadas ("Layers"), onde os neuronios de cada camada só se comunicam com a acamada seguinte, e a "Shortcut", onde os neuronios de uma camada podem se comunicar com todas as camadas a frente (0 é a Layers e 1 é a Shortcut). (int)
4. O algoritmo de treino, como isso é um tópico por si só, não irei entrar em detalhes: 0 é trein incremental, 1 é treino em batch, 2 é Resilient Back Propagation (RPROP), 3 é o Quick Propagation e 4 é o Simulated Annealing Resilient Back Propagation (SARPROP). (int)
5. A função de erro, 0 é linear (geralmente a desejável) e 1 é a tangente (boa para punir desproporcionalmente mais desvios maiores). (int)
6. A seed aleatória da rede, como os valores inicias da rede são gerados aleatóriamente, esse valor é a seed do gerador de numeros pseudo aleatórios, para garantir 100% de reprodutibilidade. (int)
7. O numero de neuronios em cada camada (essa seção deve ter tantos argumentos quanto o numero de camadas, a primeira camada deve casar com o numero de entradas do problema, a ultima camada deve casar com o numero de saidas do problema). (unsigned int)
8. A função de ativação de cada camada, 0 é linear, 1 é a função limite, 2 é a função limite simétrica, 3 é a função sigmoid, 4 é a função sigmoid por etapas, 5 é a função sigmoid simétrica, 6 é a função sigmoid simétrica por etapas, 7 é a função gaussiana, 8 é a função gaussiana simétrica, 9 é a função gaussiana por etapas, 10 é a função Elliot, 11 é a função Elliot simétrica, 12 é a função linear por partes, 13 é a função linear por partes simétrica, 14 é a função seno simétrica, 15 é a função cosseno simétrica, 16 é a função seno e 17 é a função cosseno. (int)

## Ambiente

Como mencionado acima, o ambiente influenciou, e influencia, muito em como esse projeto está sendo e como será desenvolvido. 

* **Sistema Operacional:** Linux, Debian 13.
* **Hardware:** * Placa-Mãe: IPXPV-03 (Não possui extensão para GPU, CPU soldada).
    * CPU: Intel Atom D425 (Single-Core, Hyper-Thread).
    * RAM: 4GB DDR3 (Não é possível pôr mais por limitação da CPU).
    * HD Mecânico: 500GB.
* **Localidade:** * A localidade onde o computador está disposto — minha casa — sofre com quedas de energia intermitentes e eu não disponho de um nobreak. Portanto, o sistema pode ser desligado repentinamente.

Não é difícil observar que o hardware é limitado, o ambiente é instável e o workload é longo. Um dos desafios é manter o progresso (o máximo possível) entre desligamentos abruptos (quedas de energia) e garantir que dados não sejam corrompidos, tudo isso sem criar um overhead grande o suficiente a ponto de afetar significativamente o desempenho do Worker. Tirar cada gota de performance desse hardware também é necessário e um desafio técnico interessantíssimo.

### Utilidade

Apesar de a realidade ser que eu não estou desenvolvendo este projeto por questões utilitárias, e sim apenas para descobrir se eu consigo fazer algo interessante com um hardware tão limitado, eu acredito que esta aplicação pode possuir usos reais. Listo três abaixo:

1. **MLP Feedforward eficiente para embarcados:** Redes neurais geralmente são associadas a questões maiores e hardwares mais potentes, mas as redes que a biblioteca FANN treina são pensadas para serem treinadas e executadas em CPU de forma eficiente, tornando-as adequadas para aplicações embarcadas. Porém, problemas diferentes requerem arquiteturas diferentes e a descoberta manual de uma boa configuração para a rede pode ser um processo trabalhoso e lento, e um erro nesse processo pode levar a uma rede maior e mais computacionalmente custosa do que o necessário. Esse ambiente pode automatizar o processo de busca por uma arquitetura eficiente.
2. **Estudo sobre o comportamento de redes:** Grande parte do que é feito hoje em relação a redes neurais é empírico — teste, medição e repetição —, principalmente porque a análise de redes gigantes com milhões ou bilhões de parâmetros é inviável. Porém, em redes menores, com alguns milhares de parâmetros, pode ser possível analisar como a rede se comporta frente a diversos layouts de dados e como diferentes algoritmos de treino influenciam em como a rede generaliza. Neste problema, a aplicação aqui desenvolvida seria uma engrenagem no workflow de análise automatizada de redes.
3. **Base de dados de comportamento de redes neurais:** Para universitários, seria interessante existir uma base de dados sobre o comportamento de diversas redes neurais em relação a problemas clássicos. Talvez não seja eu quem vai conseguir fazer algum bom uso disso, mas talvez alguma outra pessoa possa aproveitar.

## Melhorias futuras fora do escopo

To-Do

---

_Creio que isso constitui justificativa suficiente para a existência do projeto e documentação adequadamente simplificada da sua arquitetura geral e seus objetivos, sendo o principal deles se divertir._
