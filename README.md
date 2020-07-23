### Simulateur d'ordonnancement dynamique utilisant l'algorithme de Tomasulo
Code de départ atelier 11 ELE749 ETS

Vous devez transformer ce prototype afin de le rendre paramétrable.

Éléments à considérer :
- Jeu d'instructions
- Registres
- Unités d'exécution / Stations d'attente

Le programme est déjà chargé d'un fichier. 

Exemple d'exécution :
```
***********************************************************************
*  ELE749 Out-of-order execution demo using Tomasulo's algorithm      *
***********************************************************************

Cycle : 6 
|---------------------------------------------------------------------|
| Instruction         | Issue     | Execute   | Writeback | Retired   |
|---------------------------------------------------------------------|
|       ld F6, 34(R2) |         1 |         2 |         3 |         4 |
|       ld F2, 45(R3) |         2 |         3 |         4 |         5 |
|     muld F0, F2, F4 |         3 |         5 |         0 |         0 |
|     subd F8, F6, F2 |         4 |         5 |         0 |         0 |
|    divd F10, F0, F6 |         5 |         0 |         0 |         0 |
|     addd F6, F8, F2 |         6 |         0 |         0 |         0 |
|---------------------------------------------------------------------|

|---------------------------------------------------------------------|
| Reservation stations                                                |
|---------------------------------------------------------------------|
| Name     |  Busy  |    Op   |   Vj    |    Vk   |    Qj   |    Qk   |
|---------------------------------------------------------------------|
|     Add1 |    yes |    subd |      F6 |   Load2 |         |         |
|     Add2 |    yes |    addd |         |      F2 |    Add1 |         |
|     Add3 |     no |         |         |         |         |         |
|     Mul1 |    yes |    muld |   Load2 |      F4 |         |         |
|     Mul2 |    yes |    divd |         |      F6 |    Mul1 |         |
|    Load1 |     no |         |         |         |         |         |
|    Load2 |     no |         |         |         |         |         |
|---------------------------------------------------------------------|

|-----------------------------------------------------------------------|
| Register wait queues (Qi)                                             |
|-----------------------------------------------------------------------|
|   F0   |   F2   |   F4   |   F6   |   F8   |  F10   |  F12   |  F14   |
|   Mul1 |        |        |   Add2 |   Add1 |   Mul2 |        |        |
|-----------------------------------------------------------------------|
(c)ontinue, (a)bort
```
