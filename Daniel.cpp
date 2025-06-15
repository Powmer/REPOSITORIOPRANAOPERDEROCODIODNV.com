#include <iostream>
#include <cstring>
using namespace std;

#define TABLESPACE 2000

struct agenda {
    bool livre;
    char nome[50];
};

// VARIAVEIS DO RELATORIO
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

void bubbleSort(char nomes[][50], int total, int &colisoes, int &movimentos) {
    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - i - 1; j++) {
            colisoes++;  // Comparação
            if (strcmp(nomes[j], nomes[j + 1]) > 0) {
                char temp[50];
                strcpy(temp, nomes[j]);
                strcpy(nomes[j], nomes[j + 1]);
                strcpy(nomes[j + 1], temp);
                movimentos += 3;
            }
        }
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

        // HASH NOVA
        int posNew = NewHash(nome);
        if (aNewHash[posNew].livre) {
            strcpy(aNewHash[posNew].nome, nome);
            aNewHash[posNew].livre = false;
        } else {
            colisoesInsertNewHash++;
            cout << "Colisão Insert (NewHash) em posição " << posNew << " (" << aNewHash[posNew].nome << " já existe).\n";
        }

        // HASH ANTIGA
        int posOld = minhaHash(nome);
        if (aOldHash[posOld].livre) {
            strcpy(aOldHash[posOld].nome, nome);
            aOldHash[posOld].livre = false;
        } else {
            colisoesInsertOldHash++;
            cout << "Colisão Insert (minhaHash) em posição " << posOld << " (" << aOldHash[posOld].nome << " já existe).\n";
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

    bubbleSort(listaNewHash, totalNew, colisoesSortNew, movimentosSortNew);
    bubbleSort(listaOldHash, totalOld, colisoesSortOld, movimentosSortOld);

    cout << "\n--- Relatório ---\n";
    cout << "Colisões Insert NewHash: " << colisoesInsertNewHash << endl;
    cout << "Colisões Insert minhaHash: " << colisoesInsertOldHash << endl;
    cout << "Colisões Sort NewHash: " << colisoesSortNew << ", Movimentos: " << movimentosSortNew << endl;
    cout << "Colisões Sort minhaHash: " << colisoesSortOld << ", Movimentos: " << movimentosSortOld << endl;

    return 0;
}
