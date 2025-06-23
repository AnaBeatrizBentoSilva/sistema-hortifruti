#include <stdio.h>
#include <string.h>

// Variáveis globais para o sistema de estoque
char nomesProdutos[100][50];
char categorias[100][20];
float precos[100];
float pesos[100];
int unidades[100];
char validade[100][11];
int quantidades[100];

char nomesFornecedores[100][50];
char contatos[100][50];
char enderecos[100][100];

int numProdutos = 0, numFornecedores = 0;

// Funções do sistema de estoque
void cadastrarProduto() {
    printf("Digite o nome do produto: ");
    fgets(nomesProdutos[numProdutos], sizeof(nomesProdutos[numProdutos]), stdin);
    strtok(nomesProdutos[numProdutos], "\n"); // Remove a nova linha

    printf("Digite o preço: ");
    scanf("%f", &precos[numProdutos]);
    getchar(); // Consome o '\n'

    printf("Digite se o produto é vendido por [0]unidade ou [1]peso: ");
    scanf("%d", &unidades[numProdutos]);
    getchar(); // Consome o '\n'

    if (unidades[numProdutos] == 0) {
        printf("Digite a quantidade disponível: ");
        scanf("%d", &quantidades[numProdutos]);
        pesos[numProdutos] = 1; // Não aplica para produtos vendidos por unidade
    } else {
        printf("Digite o peso (em kg): ");
        scanf("%f", &pesos[numProdutos]);
        quantidades[numProdutos] = 0; // Não aplica para produtos vendidos por peso
    }

    printf("Digite a data de validade (dd/mm/aaaa): ");
    scanf("%s", validade[numProdutos]);
    getchar(); // Consome o '\n'

    numProdutos++;
    printf("Produto cadastrado com sucesso!\n");
}

void listarProdutos() {
    printf("\n--- Lista de Produtos ---\n");
    for (int i = 0; i < numProdutos; i++) {
        printf("Nome: %s, Preço: R$ %.2f, Validade: %s, ",
               nomesProdutos[i], precos[i], validade[i]);
        
        if (unidades[i] == 0) { // Se é por unidade
            printf("Quantidade: %d unidades\n", quantidades[i]);
        } else { // Se é por peso
            printf("Peso: %.2f kg\n", pesos[i]);
        }
    }
}

// Sistema de caixa (compras)
void realizarCompra() {
    int produto, quantidade, escolhapag;
    float total = 0.0, valorPago, troco, precoFinal, peso;
    char listaCompras[1000] = "";  // Armazena a lista de produtos comprados

    // Processo de adicionar produtos até que o cliente encerre a compra
    while (1) {
        printf("\nSelecione um produto da lista abaixo:\n");
        printf("0 - Encerrar a compra\n");
        for (int i = 0; i < numProdutos; i++) {
            printf("%d - %s\n", i + 1, nomesProdutos[i]);
        }
        
        printf("Digite o número do produto: ");
        scanf("%d", &produto);

        if (produto == 0) {
            // Encerra a compra
            printf("\nCompra encerrada!\n");
            break;
        }

        // Verifica se o produto está disponível
        if (produto < 1 || produto > numProdutos) {
            printf("Produto inválido! Tente novamente.\n");
            continue;
        }

        // Verifica se o produto é vendido por unidade ou por peso
        if (unidades[produto - 1] == 0) {
            // Produto vendido por unidade
            printf("Digite a quantidade de unidades: ");
            scanf("%d", &quantidade);

            if (quantidade > quantidades[produto - 1]) {
                printf("Quantidade insuficiente em estoque!\n");
                continue;
            }

            precoFinal = precos[produto - 1] * quantidade;
            printf("\nAdicionado %d unidades de %s - Preço: R$ %.2f\n", quantidade, nomesProdutos[produto - 1], precoFinal);

            // Atualiza a lista de compras e o total
            char item[100];
            sprintf(item, "%d unidades de %s - R$ %.2f\n", quantidade, nomesProdutos[produto - 1], precoFinal);
            strcat(listaCompras, item);

            // Atualiza a quantidade em estoque
            quantidades[produto - 1] -= quantidade;
        } else {
            // Produto vendido por peso (100g)
            printf("Digite o peso em porções de 100g: ");
            scanf("%f", &peso);

            if (peso > (pesos[produto - 1] * 10)) {
                printf("Peso insuficiente em estoque!\n");
                continue;
            }

            // Preço final deve multiplicar pelo preço unitário e pelo número de porções
            precoFinal = (precos[produto - 1] / 10) * peso;  // Divide o preço por 10 para converter para preço por 100g
            printf("Adicionado %.2f kg de %s - Preço: R$ %.2f\n", peso / 10, nomesProdutos[produto - 1], precoFinal);

            // Atualiza a lista de compras e o total
            char item[100];
            sprintf(item, "%.2f kg de %s - R$ %.2f\n", peso / 10, nomesProdutos[produto - 1], precoFinal);
            strcat(listaCompras, item);

            // Atualiza a quantidade em estoque
            pesos[produto - 1] -= peso / 10; // Atualiza o peso em kg
        }

        // Soma o preço ao total
        total += precoFinal;
    }

    // Mostra a lista completa de compras e o valor total
    printf("\n--- Lista de Compras ---\n%s", listaCompras);
    printf("\nValor total da compra: R$ %.2f\n", total);

    // Forma de pagamento
    do {
        printf("\nEscolha a forma de pagamento [1] Dinheiro ou [2] Cartão: ");
        scanf("%d", &escolhapag);

        if (escolhapag == 1) {
            do {
                printf("\nDigite o valor pago pelo cliente: ");
                scanf("%f", &valorPago);

                if (valorPago >= total) {
                    troco = valorPago - total;
                    printf("Troco: R$ %.2f\n", troco);
                } else {
                    printf("Valor pago é insuficiente! Tente novamente.\n");
                }
            } while (valorPago < total);
        } else if (escolhapag == 2) {
            printf("Siga as instruções da máquina de cartão.\n");
        } else {
            printf("Opção inválida! Tente novamente.\n");
        }
    } while (escolhapag < 1 || escolhapag > 2);
}

// Função para cadastrar fornecedor
void cadastrarFornecedor() {
    printf("Digite o nome do fornecedor: ");
    fgets(nomesFornecedores[numFornecedores], sizeof(nomesFornecedores[numFornecedores]), stdin);
    strtok(nomesFornecedores[numFornecedores], "\n"); // Remove a nova linha

    printf("Digite o contato: ");
    fgets(contatos[numFornecedores], sizeof(contatos[numFornecedores]), stdin);
    strtok(contatos[numFornecedores], "\n"); // Remove a nova linha

    printf("Digite o endereço: ");
    fgets(enderecos[numFornecedores], sizeof(enderecos[numFornecedores]), stdin);
    strtok(enderecos[numFornecedores], "\n"); // Remove a nova linha

    numFornecedores++;
    printf("Fornecedor cadastrado com sucesso!\n");
}

// Função para listar fornecedores
void listarFornecedores() {
    printf("\n--- Lista de Fornecedores ---\n");
    for (int i = 0; i < numFornecedores; i++) {
        printf("Nome: %s, Contato: %s, Endereço: %s\n",
               nomesFornecedores[i], contatos[i], enderecos[i]);
    }
}

// Menu principal
void menu() {
    int opcao;
    do {
        printf("\n--- Escolha o sistema ---\n");
        printf("1 - Sistema de Compras (Caixa)\n");
        printf("2 - Sistema de Estoque\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // Consome o '\n'

        switch (opcao) {
            case 1: realizarCompra(); break; // Sistema de compras
            case 2: {
                int opcaoEstoque;
                do {
                    printf("\n--- Menu de Estoque ---\n");
                    printf("1 - Cadastrar Produto\n");
                    printf("2 - Listar Produtos\n");
                    printf("3 - Cadastrar Fornecedor\n");
                    printf("4 - Listar Fornecedores\n");
                    printf("0 - Voltar\n");
                    printf("Escolha uma opção: ");
                    scanf("%d", &opcaoEstoque);
                    getchar(); // Consome o '\n'

                    switch (opcaoEstoque) {
                        case 1: cadastrarProduto(); break;
                        case 2: listarProdutos(); break;
                        case 3: cadastrarFornecedor(); break;
                        case 4: listarFornecedores(); break;
                        case 0: break; // Voltar ao menu principal
                        default: printf("Opção inválida! Tente novamente.\n"); break;
                    }
                } while (opcaoEstoque != 0);
                break; // Volta ao menu principal
            }
            case 0: printf("Saindo do sistema...\n"); break;
            default: printf("Opção inválida! Tente novamente.\n"); break;
        }
    } while (opcao != 0);
}

int main() {
    menu();
    return 0;
}
