#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>
#include <string.h>

// máximo de cadastros
#define NUM_MAX_CAD 50

// máximo de ingredientes por cadastro
#define MAX_INGREDIENTES 30




//----------------------------------------Struct que vai ter as informações da receita---------------------------------//
typedef struct Receita {
    int ocupado;
    char nome_receita[256];
    char cod_receita[3]; // O codigo tem 2 caracteres, sobra espaço para '\0'
    char tipo_rendimento[256];
    float rendimento;  // Virou float pra dar pra fazer conta


    float preco;

    float quant_ingred[MAX_INGREDIENTES];
    char nome_ingrediente[MAX_INGREDIENTES][256]; //Matriz de string
    int count_ingrediente;
    char medida[20][256]; //Armazena a unidade de medida de cada ingrediente
} Receita;
//--------------------------------------------------------------------------------------------------------------------//



//-------------------------------------Função que gera o arquivo .txt-------------------------------------------------//

void gerarArquivoReceitas(Receita vetor_receita[], int total_receita) {

    //comando para gerar o arquivo
    FILE *file = fopen("receitas.txt", "w");
    if (file == NULL) {
        printf("Erro ao criar o arquivo.\n");
        return;
    }


    fprintf(file, "Total de Receitas Cadastradas: %d\n\n", total_receita);

    // Percorre todas as receitas e copia pro arquivo
    for (int i = 0; i < total_receita; i++) {
        if (vetor_receita[i].ocupado) {
            fprintf(file, "Código da receita: %s\n", vetor_receita[i].cod_receita);
            fprintf(file, "Nome da receita: %s\n", vetor_receita[i].nome_receita);
            fprintf(file, "Ingredientes e quantidades:\n");

            //laço para imprimir todos os ingredientes salvos na receita
            for (int j = 0; j < vetor_receita[i].count_ingrediente; j++) {
                fprintf(file, "%s: %.2f %s\n",
                        vetor_receita[i].nome_ingrediente[j],
                        vetor_receita[i].quant_ingred[j],
                        vetor_receita[i].medida[j]);
            }

            fprintf(file, "Preço: R$ %.2f\n", vetor_receita[i].preco);
            fprintf(file, "Rendimento: %.1f %s\n", vetor_receita[i].rendimento, vetor_receita[i].tipo_rendimento);
            fprintf(file, "\n------------------------------------------\n\n");
        }
    }

    //fecha o arquivo depois de todas as receitas da memória terem sido adicionadas
    fclose(file);
    printf("Arquivo 'receitas.txt' gerado com sucesso!\n");
}

//-----------------------------------------------------------------------------------------------------------------------//




//------------------------------------------Função que calcula a proporção-----------------------------------------------//
float proporcao(float total,float desejo, float ingrediente)
{

    return (ingrediente*desejo)/total;
}
//-----------------------------------------------------------------------------------------------------------------------//




int main() {

//system("chcp 1252 > nul");
  setlocale(LC_ALL, "pt_BR.UTF-8");
    system("chcp 65001 > nul");

//-----------------------------------------------------------------------------------//
    Receita vetor_receita[NUM_MAX_CAD];
    int total_receita = 0; //número de receitas cadastradas

    int sair = 0; // controla quando o programa deve ser encerrado
    int opcao;
    int continuar;
    char ch; //armazena as opções sim e não no código
    float desejo=0;

    // Inicializa o vetor de receitas
    for (int i = 0; i < NUM_MAX_CAD; i++) {
        vetor_receita[i].ocupado = 0;
        vetor_receita[i].count_ingrediente = 0;
    }

    // Váriaveis locais
    char nome_receita[256];
    char cod_receita[4];
    char tipo_rendimento[256];
    float rendimento; //Virou float pra dar pra fazer conta
    float ingrediente;
    float venda_sugestao;


    float preco;
    int count_ingrediente;

    int idxPesquisa = -1;
//-----------------------------------------------------------------------------------//


    //menu de opções
    do {
        system("cls");
        printf("Menu de Opções \n");
        printf("1 - Cadastrar receita\n");
        printf("2 - Pesquisar receita\n");
        printf("3 - Remover receita\n");
        printf("4 - Consulta de códigos das receitas\n");
        printf("5 - Exportar receitas para documento de texto\n");
        printf("6 - Sair \n\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

//----------------------------------------------------------Switch case das opções---------------------------------------------------------------------//
        //execução de acordo com a opção escolhida
        switch (opcao) {
            case 1:
                do {
                    system("cls");
                    printf("1 - Cadastrar\n");

                    if (total_receita >= NUM_MAX_CAD) {
                        printf("\nErro: O cadastro já atingiu sua capacidade máxima!\n");
                        system("pause");
                        break;
                    }

                    //nome da receita
                    printf("\nDigite o nome que voc� deseja dar para a receita: ");
                    scanf(" %255[^\n]", nome_receita);

                    //código da receita
                    printf("\nDigite o código para identificar a receita (até 2 dígitos): ");
                    scanf(" %2[^\n]", cod_receita);

                    //contador de ingredientes
                    count_ingrediente = 0;

//----------------------------------------------------------Bloco de entrada de vários ingredientes----------------------------------------------------------//
                    do {
                        if (count_ingrediente >= MAX_INGREDIENTES) {
                            printf("Erro: Número máximo de ingredientes atingido!\n");
                            break;
                        }

                        printf("\nNome do ingrediente a ser adicionado na receita: ");
                        scanf(" %255[^\n]", vetor_receita[total_receita].nome_ingrediente[count_ingrediente]);

                        printf("\nDigite a quantidade do ingrediente: ");
                        scanf("%f", &vetor_receita[total_receita].quant_ingred[count_ingrediente]);

                        printf("\nDigite a unidade de medida do ingrediente: ");
                        scanf(" %255[^\n]", vetor_receita[total_receita].medida[count_ingrediente]);



                        vetor_receita[total_receita].count_ingrediente = ++count_ingrediente;

                        printf("\nVocê deseja adicionar mais algum ingrediente? (s/n) ");
                       scanf(" %c", &ch);
                       ch = toupper(ch);

                    } while (ch == 'S');


                    //preço da receita
                    printf("\nDigite o preço de venda da receita: R$ ");
                    scanf("%f", &preco);

                    printf("\nDigite a porção que a receita rende: ");
                    scanf(" %f", &rendimento);

                    printf("\nDigite a unidade de medida da porção: ");
                    scanf(" %255[^\n]", tipo_rendimento);



                    //verificação de existência do código
                    idxPesquisa = -1;
                    for (int i = 0; i < NUM_MAX_CAD; i++) {
                        if (vetor_receita[i].ocupado == 1 && strcmp(vetor_receita[i].cod_receita, cod_receita) == 0) {
                            idxPesquisa = i;
                            break;
                        }
                    }

//------------------------------------------------------------------Verificação do código----------------------------------------------------------------------//
                    //se já existir uma receita com o código, o programa imprime a receita do código
                    if (idxPesquisa != -1) {
                        printf("\nErro: Já existe uma receita com o mesmo código no cadastro.\n");
                        printf("\nReceita existente: \n");
                        printf("Código da receita: %s\n", vetor_receita[idxPesquisa].cod_receita);
                        printf("Nome da receita: %s\n", vetor_receita[idxPesquisa].nome_receita);

                        //laço que vai imprimir todos os ingredientes
                        for (int i = 0; i < vetor_receita[idxPesquisa].count_ingrediente; i++) {
                            printf("%s: %.2f\n", vetor_receita[idxPesquisa].nome_ingrediente[i], vetor_receita[idxPesquisa].quant_ingred[i]);
                        }
                        printf("Preço: R$ %.2f\n\n", vetor_receita[idxPesquisa].preco);
                        system("pause");
//--------------------------------------------------------------Copia os dados para as variáveis da struc-------------------------------------------------------------------------//
                    } else {

                        //se o código não for o mesmo, copia as informações para as várias da struct
                        for (int i = 0; i < NUM_MAX_CAD; i++) {
                            if (vetor_receita[i].ocupado == 0) {
                                vetor_receita[i].ocupado = 1;
                                strcpy(vetor_receita[i].nome_receita, nome_receita); // garante que o valor seja copiado para a struct
                                strcpy(vetor_receita[i].cod_receita, cod_receita); // garante que o valor seja copiado para a struct
                                strcpy(vetor_receita[i].tipo_rendimento, tipo_rendimento); // garante que o valor seja copiado para a struct
                                vetor_receita[i].rendimento = rendimento;
                                vetor_receita[i].preco = preco;
                                vetor_receita[i].count_ingrediente = count_ingrediente;
                                total_receita++;
                                printf("\nDados cadastrados com sucesso!\n");
                                system("pause");
                                break;
                            }
                        }
                    }
                    //pergunta se quer adicionar outro ingrediente
                    system("cls");
                    printf("\nGostaria de cadastrar outra receita? (s/n): ");
                    scanf(" %c", &ch);
                    ch = toupper(ch);
                    continuar = (ch == 'S');

                } while (continuar);

                break;
//----------------------------------------------------------------------Pesquisa de receita por código----------------------------------------------------------------------//
            case 2:
                do {
                    system("cls");
                    printf("2 - Pesquisar receita\n");

                    printf("\nDigite o código da receita: ");
                    scanf(" %2s", cod_receita); //lê 2 caracteres e o '\0'

                    idxPesquisa = -1;
                    for (int i = 0; i < NUM_MAX_CAD; i++) {
                        if (vetor_receita[i].ocupado == 1 && strcmp(vetor_receita[i].cod_receita, cod_receita) == 0) {
                            idxPesquisa = i;
                            break;
                        }
                    }


                    if (idxPesquisa != -1) {
                        printf("\nReceita Encontrada: \n");
                        printf("Código da receita: %s\n", vetor_receita[idxPesquisa].cod_receita);
                        printf("Nome da receita: %s\n\n", vetor_receita[idxPesquisa].nome_receita);
                         printf("Ingredientes e quantidades: \n");
                        for (int i = 0; i < vetor_receita[idxPesquisa].count_ingrediente; i++) {
                            printf("%s: %.2f %s\n", vetor_receita[idxPesquisa].nome_ingrediente[i], vetor_receita[idxPesquisa].quant_ingred[i], vetor_receita[idxPesquisa].medida[i]);
                        }
                        printf("Preço: R$ %.2f\n", vetor_receita[idxPesquisa].preco);
                          printf("Rendimento da receita: %.2f %s\n\n", vetor_receita[idxPesquisa].rendimento,vetor_receita[idxPesquisa].tipo_rendimento);
                        system("pause");

                        printf("\nVocê deseja fazer essa receita? (s/n)");      //define se o usuario piloto vai querer fazer a receira
                      scanf(" %c",&ch);
                      ch=toupper(ch);
                      if(ch=='S')
                      {

                        printf("\nDigite a porção que deseja fazer dessa receita: ");       //entra com a porção
                        scanf("%f",&desejo);

                        printf("\nPara fazer %.2f %s de %s , a receita será:\n",desejo,vetor_receita[idxPesquisa].tipo_rendimento,vetor_receita[idxPesquisa].nome_receita);

                        for(int i=0;i<vetor_receita[idxPesquisa].count_ingrediente;i++)
                        {
                            ingrediente = proporcao(vetor_receita[idxPesquisa].rendimento,desejo, vetor_receita[idxPesquisa].quant_ingred[i]);
                            printf("%s: %.2f %s\n",vetor_receita[idxPesquisa].nome_ingrediente[i],ingrediente,vetor_receita[idxPesquisa].medida[i]);
                        }
                        venda_sugestao = proporcao(vetor_receita[idxPesquisa].rendimento,desejo,vetor_receita[idxPesquisa].preco);
                            printf("\nPreço sugerido para venda: %.2f reais.",venda_sugestao);
                        system("pause");

                    }
                    } else {
                        printf("\nNão foi encontrada uma receita com o código informado!\n\n");
                        system("pause");
                    }



                    printf("\nGostaria de pesquisar outra receita? (s/n): ");
                    scanf(" %c", &ch);
                    ch = toupper(ch);
                    continuar = (ch == 'S');

                } while (continuar);

                break;
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//



//----------------------------------------------------------------------Remove uma receita escolhida pelo usuário piloto----------------------------------------------------------------------//
            case 3:
                do {
                    system("cls");
                    printf("3 - Remover Receita\n\n");

                    printf("\nDigite o código da receita: ");
                    scanf(" %2s", cod_receita); // Corrigido para ler at� 2 caracteres + '\0'

                    idxPesquisa = -1;
                    for (int i = 0; i < NUM_MAX_CAD; i++) {
                        if (vetor_receita[i].ocupado == 1 && strcmp(vetor_receita[i].cod_receita, cod_receita) == 0) {
                            idxPesquisa = i;
                            break;
                        }
                    }

                    if (idxPesquisa != -1) {
                        vetor_receita[idxPesquisa].ocupado = 0;
                        total_receita--;
                        printf("\nReceita removida com sucesso!\n");
                    } else {
                        printf("\nNão foi possível remover a receita do cadastro!\n");
                    }

                    system("pause");

                    printf("\nGostaria de remover outra receita? (s/n): ");
                    scanf(" %c", &ch);
                    ch = toupper(ch);
                    continuar = (ch == 'S');

                } while (continuar);

                break;
//-------------------------------------------------------------------------------------------------------------------------------//





//------------------------------------Mostra as informações contidas no código da receita---------------------------------//
            case 4:
                system("cls");
                printf("4 - Consultar códigos\n\n");

                printf("Total de Receitas Cadastradas: %d\n\n", total_receita);
                printf("Código da receita\tNome da receita\n");

                for (int i = 0; i < NUM_MAX_CAD; i++) {
                    if (vetor_receita[i].ocupado == 1) {
                        printf("\n");
                        printf("\t%s \t\t%s\n", vetor_receita[i].cod_receita, vetor_receita[i].nome_receita);
                    }
                }

                system("pause");

                break;
//------------------------------------------------------------------------------------------------------------------------//



//--------------------------------------Gera um arquvivo de texto com todas as receitas-----------------------------------//

            case 5:
            gerarArquivoReceitas( vetor_receita, total_receita); // Chama a função que gera um arquivo txt
            system("pause");
            break;
//------------------------------------------------------------------------------------------------------------------------//



//-----------------------------------------------------Sai do programa----------------------------------------------------//
            case 6:
                sair = 1;
                break;

            default:
                printf("\nOpção inválida! Tente novamente.\n");
                system("pause");
                break;
        }

    } while (!sair);
//------------------------------------------------------Fim do switch case das opções---------------------------------------------------------------------//

    return 0;
}







