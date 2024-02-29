#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    int* intrari;
    int iesire;
    char poarta;
} Poarta;

int main(int argc, char* argv[]) {

    FILE* inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
        perror("Error opening input file");
        return 1;
    }

    FILE* outputFile = fopen(argv[2], "w");
    if (outputFile == NULL) {
        perror("Error opening output file");
        fclose(inputFile);
        return 1;
    }

    int nr_intrari, var_iesire;
    fscanf(inputFile, "%d %d", &nr_intrari, &var_iesire);

    Poarta* circuit = (Poarta*)malloc(100 * sizeof(Poarta));

    int clauze = 0, i = 0;

    fprintf(outputFile, "p cnf %d %d\n", var_iesire, clauze);

    while(fscanf(inputFile, " %c", &circuit[i].poarta) == 1) {
        //cuvantul ramans pana la primul spatiu
        char word[2];
        fscanf(inputFile, "%99s", word);

        circuit[i].intrari = (int*)calloc(60, sizeof(int));

        int j = 0, index_intrare;

        while (fscanf(inputFile, "%d", &index_intrare) == 1) {
            //acum avem numarul in variabila 'number'
            //poti sa procesezi sau sa afisezi numarul aici

            if (fgetc(inputFile) == '\n' || feof(inputFile)) {
                //s-a citit ultimul numar pe linie
                circuit[i].iesire = index_intrare;
            } else {
                //inca exista alte numere pe aceeasi linie
                circuit[i].intrari[j] = index_intrare;
            }
            j++;
        }

        if(circuit[i].poarta == 'N') {
            fprintf(outputFile, "-%d ", circuit[i].intrari[0]);
            fprintf(outputFile, "-%d ", circuit[i].iesire);
            fprintf(outputFile, "0\n");

            
            fprintf(outputFile, "%d ", circuit[i].intrari[0]);
            fprintf(outputFile, "%d ", circuit[i].iesire);
            fprintf(outputFile, "0\n");

            clauze += 2;
        } else if (circuit[i].poarta == 'A') {
            for (int k = 0; k < j - 1; ++k)
                fprintf(outputFile, "-%d ", circuit[i].intrari[k]);
            fprintf(outputFile, "%d ", circuit[i].iesire);
            fprintf(outputFile, "0\n");

            for (int k = 0; k < j - 1; ++k) {
                fprintf(outputFile, "%d ", circuit[i].intrari[k]);
                fprintf(outputFile, "-%d ", circuit[i].iesire);
                fprintf(outputFile, "0\n");
            }

            clauze = clauze + 1 + j - 1;

        } else if (circuit[i].poarta == 'O') {
            for (int k = 0; k < j - 1; ++k)
                fprintf(outputFile, "%d ", circuit[i].intrari[k]);
            fprintf(outputFile, "-%d ", circuit[i].iesire);
            fprintf(outputFile, "0\n");

            for (int k = 0; k < j - 1; ++k) {
                fprintf(outputFile, "-%d ", circuit[i].intrari[k]);
                fprintf(outputFile, "%d ", circuit[i].iesire);
                fprintf(outputFile, "0\n");
            }

            clauze = clauze + 1 + j - 1;
        }
        i++;
    }

    fprintf(outputFile, "%d 0\n", var_iesire);
    clauze++;

    //ne intoarcem la inceputul fisierului
    fseek(outputFile, 0, SEEK_SET);
    fprintf(outputFile, "p cnf %d %d\n", var_iesire, clauze);

    for (int i = 0; i < nr_intrari; ++i) {
        free(circuit[i].intrari);
    }
    free(circuit);

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
