<img align="right" width="90" height="40" src="https://user-images.githubusercontent.com/61190134/103407124-a0fe6380-4b5d-11eb-9692-a7a372f816a2.png">

# HandsOn: Pipes

## About 

|            |                                              |  
| ---------- | -------------------------                    |  
| Title:     | HandsOn: Pipes       |  
| Instructor and Author:| [Jordi Mateo Fornés](http:jordimateofornes.com)                           |  
| *Fall*:      | 2021-2022                                   |  
| *Course*:    |    102377 - Operating Systems - [Grau en Tècniques d'Interacció Digital i de Computació](http://www.grauinteraccioicomputacio.udl.cat/ca/index.html) |
| University:     | [University of Lleida](http://www.udl.cat/ca/) - [Campus Igualada](http://www.campusigualada.udl.cat/ca/) - [Escola Politècnica Superior](http://www.eps.udl.cat/ca/)       |  
| Copyright: | Copyright © 2019-2022 Jordi Mateo Fornés     |  
| Version:   | 1.0

## Introducció

Aquest és **HandsOn** tracta sobre la comunicació entre processos (IPC) a Linux. En aquest HandsOn analitzarem els pipes, que són canals que permeten la comunicació entre processos. Un canal té un final d'escriptura per escriure bytes i un final de lectura per llegir aquests bytes en ordre FIFO (primer en entrar, primer en sortir). En l'ús habitual, un procés escriu al canal i un procés diferent llegeix des d'aquest mateix canal. Els propis bytes poden representar qualsevol cosa.

Les canonades es presenten en dos tipus, amb nom i sense nom, i es poden utilitzar de manera interactiva des de la línia d'ordres o dins dels programes; els exemples estan disponibles. Aquest **HandsOn** tractarà únicament les canonades sense nom. 


![Julia Evans: Summary](https://pbs.twimg.com/media/DaROZeQW4AEMUl5?format=jpg&name=large)

## Objectius

En aquesta **HandsOn**, ens centrarem principalment en com emprar les pipes a Linux. Revisarem els conceptes de descriptor de fitxer i veurem com s'implementen les pipes en C i les seves peculiaritats. Finalment, farem un simulador de batalles pokémon utilitzant pipes.

![Pokemon battle](https://static.wikia.nocookie.net/essentialsdocs/images/7/70/Battle.png/revision/latest?cb=20190219202514)

També plantejarem alguns exercicis de nivell incremental per assegurar la bona comprensió.

## Contingut

1. **Solutions.md**: Conté un document amb les solucions a les activitats proposades al *HandsOn.md*.
2. Les carpetes **act0X**, contenen els codis per solucionar les activitats suggerides.
