#ifndef EXTRATO_H
#define EXTRATO_H

struct Extrato {
    char cpf[12];
    char data[20];
    double valor;
    double tarifa;
    double saldo;
};

void arquivo_extrato();
void ler_arquivo_extrato();
void debito();
void deposito();
void extrato();
void transferencia();

#endif
