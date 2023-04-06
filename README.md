## Bipartitní souvislý podgraf hranově ohodnoceného grafu s maximální vahou

### Vstupní data:
- n = přirozené číslo představující počet uzlů grafu G, 150 > n >= 10
- k = přirozené číslo představující průměrný stupeň uzlu grafu G, 3n/4 > k >= 5
- G(V,E) = jednoduchý neorientovaný hranově ohodnocený souvislý graf o n uzlech a průměrném stupni k, váhy hran jsou z intervalu <80,120>

### Definice:
Graf G(V,E) je bipartitní, jestliže můžeme rozdělit množinu uzlů V na disjunktní pomnožiny U a W tak, že každá hrana v G spojuje uzel z U s uzlem z W. Jinak řečeno, bipartitní graf je možné uzlově obarvit 2 barvami.

### Úkol:
Nalezněte podmnožinu hran F takovou, že podgraf G(V,F) je bipartitní, souvislý a součet ohodnocení jeho hran je maximální mezi všemi bipartitními souvislými podgrafy grafu G(V,E).

### Výstup algoritmu:
Výpis obou podmnožin uzlů bipartitního podgrafu a výpis množiny jeho hran F.