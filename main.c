#include "cliente_banco.h"
#include <stdio.h>

int main() {
  ler_arquivo_clientes();
  ler_arquivo_extrato();

  int opcao;
  while (1) {
    printf("\n 1 Novo cliente\n 2 Apagar cliente\n 3.Listar clientes\n 4 "
           "Débito\n "
           "5 Depósito\n 6 Extrato\n 7 Transferência entre contas\n 8 Sair\n");
    printf("\nEscolha uma opção: ");
    scanf("%d", &opcao);

    switch (opcao) {
    case 1:
      novo_cliente();
      break;
    case 2:
      apagar_cliente();
      break;
    case 3:
      listar_clientes();
      break;
    case 4:
      debito();
      break;
    case 5:
      deposito();
      break;
    case 6:
      extrato();
      break;
    case 7:
      transferencia();
      break;
    case 8:
      arquivo_clientes();
      arquivo_extrato();
      printf("\nAção encerrada\n\n");
      return 0;
    default:
      printf("\nOpção inválida\n\n");
    }
  }

  return 0;
}
