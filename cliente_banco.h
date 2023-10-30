#ifndef CLIENTES_H
#define CLIENTES_H

struct Cliente {
    char nome[50];
    char cpf[12];
    int tipo;
    double saldo;
    char senha[20];
};

struct Cliente *buscar_cliente(const char *cpf);
void novo_cliente();
void apagar_cliente();
void listar_clientes();
void arquivo_clientes();
void ler_arquivo_clientes();

#endif
