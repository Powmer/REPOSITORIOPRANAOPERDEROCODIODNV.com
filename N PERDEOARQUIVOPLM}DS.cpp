#include <iostream>
#include <cstring>
using namespace std;

#define TABLESPACE 2000

struct agenda {
    bool livre;
    char nome[50];
};
//VARIAVEIS DO RELATORIO
int colisoesInsertNewHash = 0;
int colisoesInsertOldHash = 0;
int colisoesSortNew = 0;
int movimentosSortNew = 0;
int colisoesSortOld = 0;
int movimentosSortOld = 0;

int NewHash(char *chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        soma += (chave[i] * (i + 1));
    }
    return soma % TABLESPACE;
}

int minhaHash(char *chave) {
    return (chave[0] - 65) % TABLESPACE; 
}

void iniciaAgenda(agenda a[]) {
    for (int i = 0; i < TABLESPACE; i++) {
        a[i].livre = true;
        strcpy(a[i].nome, "");
  
    }
}

void merge(char nomes[][50], int esq, int meio, int dir, int &colisoes, int &movimentos) {
    int n1 = meio - esq + 1;
    int n2 = dir - meio;

    char L[n1][50];
    char R[n2][50];

    for (int i = 0; i < n1; i++) {
        strcpy(L[i], nomes[esq + i]);
        movimentos++;
    }
    for (int j = 0; j < n2; j++) {
        strcpy(R[j], nomes[meio + 1 + j]);
        movimentos++;
    }

    int i = 0, j = 0, k = esq;

    while (i < n1 && j < n2) {
        colisoes++;
        if (strcmp(L[i], R[j]) <= 0) {
            strcpy(nomes[k], L[i]);
            i++;
        } else {
            strcpy(nomes[k], R[j]);
            j++;
        }
        k++;
        movimentos++;
    }

    while (i < n1) {
        strcpy(nomes[k], L[i]);
        i++;
        k++;
        movimentos++;
    }

    while (j < n2) {
        strcpy(nomes[k], R[j]);
        j++;
        k++;
        movimentos++;
    }
}

void mergeSort(char nomes[][50], int esq, int dir, int &colisoes, int &movimentos) {
    if (esq < dir) {
        int meio = esq + (dir - esq) / 2;
        mergeSort(nomes, esq, meio, colisoes, movimentos);
        mergeSort(nomes, meio + 1, dir, colisoes, movimentos);
        merge(nomes, esq, meio, dir, colisoes, movimentos);
    }
}

int main() {
    agenda aNewHash[TABLESPACE];
    agenda aOldHash[TABLESPACE];
    iniciaAgenda(aNewHash);
    iniciaAgenda(aOldHash);

    char nome[50];

    cout << "Digite nomes (0 para parar):\n";
    while (true) {
        cout << "Nome: ";
        cin.getline(nome, 50);

        if (strcmp(nome, "0") == 0)
            break;

        // FUNÇÂO DE HASH NOVA
        int posNew = NewHash(nome);
        if (aNewHash[posNew].livre) {
            strcpy(aNewHash[posNew].nome, nome);
            aNewHash[posNew].livre = false;
        } else {
            colisoesInsertNewHash++;
            cout << "Colisão Insert (NewHash) em posicao " << posNew << " (" << aNewHash[posNew].nome << " já existe).\n";
        }

        //  HASH VELHA 
        int posOld = minhaHash(nome);
        if (aOldHash[posOld].livre) {
            strcpy(aOldHash[posOld].nome, nome);
            aOldHash[posOld].livre = false;
        } else {
            colisoesInsertOldHash++;
            cout << "Colisão Insert (minhaHash) em posicao " << posOld << " (" << aOldHash[posOld].nome << " já existe).\n";
        }
    }

    char listaNewHash[TABLESPACE][50];
    int totalNew = 0;

    char listaOldHash[TABLESPACE][50];
    int totalOld = 0;

    for (int i = 0; i < TABLESPACE; i++) {
        if (!aNewHash[i].livre) {
            strcpy(listaNewHash[totalNew], aNewHash[i].nome);
            totalNew++;
        }
        if (!aOldHash[i].livre) {
            strcpy(listaOldHash[totalOld], aOldHash[i].nome);
            totalOld++;
        }
    }

    mergeSort(listaNewHash, 0, totalNew - 1, colisoesSortNew, movimentosSortNew);
    mergeSort(listaOldHash, 0, totalOld - 1, colisoesSortOld, movimentosSortOld);

    // MPRINT LISTA ORDENADA
    cout << "\n Lista ORDENADA com nomes  (ASH NOVA)\n";
    for (int i = 0; i < totalNew; i++) {
        int hashNew = NewHash(listaNewHash[i]);
        cout << listaNewHash[i] << " | Hash Nova: " << hashNew << endl;
    }

    cout << "\n Lista ORDENADA com nomes (Hash Antiga) \n";
    for (int i = 0; i < totalOld; i++) {
        int hashOld = minhaHash(listaOldHash[i]);
        cout << listaOldHash[i] << " | Hash Antiga: " << hashOld << endl;
    }

    // PRINTAR AS LISTAS
    cout << "\n Lista final na agenda (Hash Nova) n";
    for (int i = 0; i < TABLESPACE; i++) {
        if (!aNewHash[i].livre) {
            cout << "Posição " << i << ": " << aNewHash[i].nome << endl;
        }
    }

    cout << "\n Lista final na agenda (Hash Antiga) \n";
    for (int i = 0; i < TABLESPACE; i++) {
        if (!aOldHash[i].livre) {
            cout << "Posição " << i << ": " << aOldHash[i].nome << endl;
        }
    }

    // RESULTADO DO RELATORIO
    cout << "\nColisões Insert (NewHash): " << colisoesInsertNewHash << endl;
    cout << "Colisões Insert (minhaHash): " << colisoesInsertOldHash << endl;

    cout << "\nOrdenação Hash Nova - Colisões: " << colisoesSortNew << " | Movimentos: " << movimentosSortNew << endl;
    cout << "Ordenação Hash Antiga - Colisões: " << colisoesSortOld << " | Movimentos: " << movimentosSortOld << endl;

    return 0;
}
