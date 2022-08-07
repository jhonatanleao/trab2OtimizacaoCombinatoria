#include <iostream>
#include "main.hpp"
#include <memory.h>
#include <time.h> 
#include <stdlib.h> 

using namespace std;


int main(int argc, char *argv[]){
    srand(time(NULL));
    qtdVetCandidatas = 0;
    qtdAtvInicial = 0;
    qtdvetAlocada = 0;
    lerArquivo();
    qtdMaquinaUsada = 7;
    for(int i=0; i< qtdMaquinaUsada; i++){
        soluc[i].FO = 0;
    }
    int flag2;
    heuGulosa();

    for(int i=0; i < qtdMaquinaUsada; i++){
        solucaoAux[i] = soluc[i];
    }
    //ajusteVetPosAlocadas(1);

    // for(int i=0; i < qtdvetAlocada; i++){
    //     printf("%d ", vetAtivAlocadas[i]+1);
    // }
    // printf("\n");

    // flag2 = ajustaVetSucessor(12, 9);
    // printf("%d\n", flag2);

    for(int i=0; i < qtdMaquinaUsada; i++){
        solucaoAux[i] = soluc[i];
    }
    for(int i=0; i < 1000000; i++){
        geraVizinha();
    }
    heuBLMM();
    printSolucao2();
    
}

void lerArquivo()
{
    int flag = 0, predecessor, sucessor;
    FILE *f = fopen("GUNTHER.txt", "r");

    fscanf(f, "%d", &qtdTarefas);
    
    for(int i=0; i < qtdTarefas; i++){
        vetTarefa[i].idTarefa=i;
        vetTarefa[i].qtdPredecessor=0;
        vetTarefa[i].qtdSucessor=0;
        fscanf(f, "%d", &vetTarefa[i].custo);       
    }

    do
    {
        fscanf(f, "%d", &predecessor);
        fscanf(f, "%d", &sucessor);
        if(predecessor == -1){
            flag = 1;
            
        }else{
            predecessor--;
            sucessor--;
            vetTarefa[predecessor].vetSucessor[vetTarefa[predecessor].qtdSucessor] = sucessor;
            vetTarefa[predecessor].qtdSucessor++;
            vetTarefa[sucessor].vetPredecessor[vetTarefa[sucessor].qtdPredecessor] = predecessor;
            vetTarefa[sucessor].qtdPredecessor++;

        }       
    } while (flag == 0);
}

void heuGulosa()
{
    int divisaoTarefa, resto, contador=0, id, flag;

    divisaoTarefa = qtdTarefas / qtdMaquinaUsada;
    resto = qtdTarefas % qtdMaquinaUsada;

    ativInicial();
    for(int i=0; i<qtdMaquinaUsada; i++){
        for(int j=0; j < divisaoTarefa; j++){
            if(contador == qtdAtvInicial){
                id = maiorPeso();
                flag = insereTarefa(vetPosCandidatas[id]);
                if(flag ==1){
                    soluc[i].atividadeAlocada[soluc[i].qtdReqAlocada] = vetPosCandidatas[id];                
                    organizaVetorCandidata(id);
                }
            } else {
                soluc[i].atividadeAlocada[soluc[i].qtdReqAlocada] = vetAtvInicial[contador];               
                insereTarefa(vetAtvInicial[contador]);
                contador++;                         
            }
            soluc[i].qtdReqAlocada++;
        }
    }

    for(int j=0; j < resto; j++){
        id = maiorPeso();
        flag = insereTarefa(vetPosCandidatas[id]);
        if(flag == 1){
            soluc[qtdMaquinaUsada-1].atividadeAlocada[soluc[qtdMaquinaUsada-1].qtdReqAlocada] = vetPosCandidatas[id];
            soluc[qtdMaquinaUsada-1].qtdReqAlocada++;
            organizaVetorCandidata(id);
        }
    }

    FOSolucao = calculaFO(soluc);

}

void geraVizinha(){
	int maquina1, maquina2, FOaux, flag, tarefa, idTarefa, flag2;

    for(int i=0; i < qtdMaquinaUsada; i++){
        solucaoAux[i] = soluc[i];
    }
		
	maquina1 = rand() % qtdMaquinaUsada; //maquina que vai perder uma atividade

	do
		maquina2 = rand() % qtdMaquinaUsada; //maquina que vai ganhar uma atividade
	while(maquina2 == maquina1);
	
	tarefa = rand() % solucaoAux[maquina1].qtdReqAlocada;  //tarefa que vai ser trocada
    idTarefa = solucaoAux[maquina1].atividadeAlocada[tarefa];

	excluirTarefa(tarefa, maquina1);
    memset(&vetAtivAlocadas, 0, sizeof(vetAtivAlocadas));
    ajusteVetPosAlocadas(maquina2);
    flag = verificaViabilidadeAtv(idTarefa);
    flag2 = ajustaVetSucessor(idTarefa, tarefa);
	if(flag == 1 && flag2 == 1){
        solucaoAux[maquina2].atividadeAlocada[solucaoAux[maquina2].qtdReqAlocada] = idTarefa;
        solucaoAux[maquina2].qtdReqAlocada++;
		FOaux = calculaFO(solucaoAux);
		if(FOaux < FOSolucao){
            printf("Nova FO: %d\n", FOaux);
            printf("\n");
            for(int i=0; i < qtdMaquinaUsada; i++){
                soluc[i] = solucaoAux[i];
            }
            FOSolucao = FOaux;
		}
	}

}

int ajustaVetSucessor(int idTarefa, int tarefa){
    int flag;
    for(int i=0; i < vetTarefa[idTarefa].qtdSucessor; i++){
        flag = buscaid(vetTarefa[idTarefa].vetSucessor[i]);
        if(flag == 1)
            return -1;
    }
    return 1;
}

// int ajustaVetSucessor(int idTarefa, int tarefa){
//     int qtdVetSucessor =0;

//     for(int i=0; i < qtdvetAlocada; i++){
//         for(int j=0; j < vetTarefa[idTarefa].qtdSucessor; j++){
//             if(vetTarefa[idTarefa].vetSucessor[j] == vetAtivAlocadas[i]){
//                 return -1;
//             }
//         }
//     }
//     return 1;   
// }

void excluirTarefa(int idAtividade, int idMaquina){
    int i, posicaoRetirada;

    //posicaoRetirada = buscaIdAtvSoluc(idAtividade, idMaquina);
    solucaoAux[idMaquina].atividadeAlocada[idAtividade] = 0;

    for(i = idAtividade; i < solucaoAux[idMaquina].qtdReqAlocada; i++){
        solucaoAux[idMaquina].atividadeAlocada[i] = solucaoAux[idMaquina].atividadeAlocada[i+1];
    }
    solucaoAux[idMaquina].qtdReqAlocada--;
    solucaoAux[idMaquina].atividadeAlocada[i] = 0;

}

void ajusteVetPosAlocadas(int idMaquina){
    qtdvetAlocada = 0;
    for(int i=0; i <= idMaquina; i++){
        for(int j=0; j < solucaoAux[i].qtdReqAlocada; j++){
            preencheVetPosAlocadas(solucaoAux[i].atividadeAlocada[j]);
        }
    }
}

int buscaIdAtvSoluc(int idAtividade, int idMaquina){
    for(int i =0; i < solucaoAux[idMaquina].qtdReqAlocada; i++){
        if(solucaoAux[idMaquina].atividadeAlocada[i] == idAtividade)
            return i;
    }
}

void printSolucao(){
    FILE *f;
    f = fopen("resultado.txt", "w");

    for(int i=0; i < qtdMaquinaUsada; i++){
        fprintf(f, "Maquina %d: ", i);
        for(int j=0; j < soluc[i].qtdReqAlocada; j++){
            fprintf(f,"%d  ", soluc[i].atividadeAlocada[j]+1);
        }
        

        fprintf(f, "\n");
    }
    fprintf(f,"FO: %d", FOSolucao);
    fclose(f);
}

void printSolucao2(){

    for(int i=0; i < qtdMaquinaUsada; i++){
        printf("Maquina %d: ", i);
        for(int j=0; j < soluc[i].qtdReqAlocada; j++){
            printf("%d  ", soluc[i].atividadeAlocada[j]+1);
        }
        

        printf("\n");
    }
}

int maiorPeso(){
    int maiorFo = 0, maiorId;
    for (int i = 0; i < qtdVetCandidatas; i++)
    {
        //printf("%d\n", vetTarefa[i].custo);
        if(maiorFo < vetTarefa[i].custo){
            maiorFo = vetTarefa[i].custo;
            //maiorId = vetPosCandidatas[i];
            maiorId = i;
        }   
    }     
    //printf("\n");
    return maiorId; 
}

void organizaVetorCandidata(int posicaoRetirada){
    int i;
    vetPosCandidatas[posicaoRetirada] = 0;

    for(i = posicaoRetirada; i < qtdVetCandidatas; i++){
        vetPosCandidatas[i] = vetPosCandidatas[i+1];
    }
    qtdVetCandidatas--;
    vetPosCandidatas[i] = 0;
}

void preencheVetPosAlocadas(int idAtivade){
    vetAtivAlocadas[qtdvetAlocada] = idAtivade;
    qtdvetAlocada++;
}

int verificaViabilidadeAtv(int idAtividade){
    int flag, contador=0;
    for(int i=0; i < vetTarefa[idAtividade].qtdPredecessor; i++){
        flag = buscaid(vetTarefa[idAtividade].vetPredecessor[i]);
        if(flag == 1)
            contador++;
    }
    if(vetTarefa[idAtividade].qtdPredecessor == contador){
        return 1;
    } else {
        return -1;
    }
    
}

int buscaid(int idAtividade){ 
    for(int i=0; i < qtdvetAlocada; i++){
        if(vetAtivAlocadas[i] == idAtividade) 
            return 1;
    }
    return -1;
}

int verificaVetAlocada(int idAtividade){
    for(int i=0; i < qtdvetAlocada; i++){
        if(vetAtivAlocadas[i] == idAtividade) 
            return 1;
    }
    return -1;    
}

void preencheVetCandidatas(int idAtividade){
    vetPosCandidatas[qtdVetCandidatas] = idAtividade;
    qtdVetCandidatas++;
}

void ativInicial(){
    for (int i = 0; i < qtdTarefas; i++)
    {
        if(vetTarefa[i].qtdPredecessor == 0){
            vetAtvInicial[qtdAtvInicial] = i;
            qtdAtvInicial++;
        }
    }
}

int insereTarefa(int idAtividade)
{
    int flag=0;
    flag = verificaViabilidadeAtv(idAtividade);
    if(flag == 1){
        flag = 0;
        preencheVetPosAlocadas(idAtividade);
        for(int i=0; i< vetTarefa[idAtividade].qtdSucessor; i++){
            if(verificaVetAlocada(vetTarefa[idAtividade].vetSucessor[i]) == -1){
                flag = verificaViabilidadeAtv(vetTarefa[idAtividade].vetSucessor[i]);
                if(flag == 1){
                    preencheVetCandidatas(vetTarefa[idAtividade].vetSucessor[i]);
                }
            }
        }
        return 1;
    }
    return -1;
}

int calculaFO(solucao s[MAX_REQ]){
    int maior = 0;

    for(int i=0; i < qtdMaquinaUsada; i++){
        s[i].FO = 0;
        for(int j=0; j < s[i].qtdReqAlocada; j++){
            s[i].FO += vetTarefa[s[i].atividadeAlocada[j]].custo;
        }
        if(maior < s[i].FO){
            maior = s[i].FO;
        }
    }

    return maior;
}

void heuBLMM()
{
    int flag, flag2, i, j, idMaquina, contador =0;

    for(int i=0; i < qtdMaquinaUsada; i++){
        solucaoAux[i] = soluc[i];
        melhor[i] = soluc[i];
    }

    FOMelhor = FOSolucao;
    FOaux = FOSolucao;

    while (true)
    {
        flag = 0;
        for (i = 0; i < qtdTarefas; i++)
        {
            for (j = 0; j < qtdMaquinaUsada; j++)
            {
            	
                idMaquina = encontraMaquina(j, i);
                //printf("idMaquina: %d\n", idMaquina);
                if (idMaquina != -1) // verificar se a tarefa não está na maquina que o for ta
                {
                    //printf("%d\n", contador);
                    
					excluirTarefa(i, idMaquina);
				    memset(&vetAtivAlocadas, 0, sizeof(vetAtivAlocadas));
				    ajusteVetPosAlocadas(idMaquina);
				    flag = verificaViabilidadeAtv(i);
				    flag2 = ajustaVetSucessor(i, i);
				    
					if(flag == 1 && flag2 == 1){
	                   	solucaoAux[j].atividadeAlocada[solucaoAux[j].qtdReqAlocada] = i;
	                    solucaoAux[j].qtdReqAlocada++;
	                    FOaux = calculaFO(solucaoAux);  
	                    printf("FOaux: %d, FOMelhor: %d\n", FOaux, FOMelhor);
	                    if (FOaux < FOMelhor && FOaux != 0)
	                    {
	                        printf("FOaux: %d, FOMelhor: %d\n", FOaux, FOMelhor);
	                        for(int i=0; i < qtdMaquinaUsada; i++){
	                            melhor[i] = solucaoAux[i];
	                        }
	                        FOMelhor = FOaux;
	                        flag = 1;
	                    }
					}
                                        
                    //excluirTarefa(i, idMaquina);
                    //solucaoAux[j].atividadeAlocada[solucaoAux[j].qtdReqAlocada] = i;
                    //solucaoAux[j].qtdReqAlocada++;
                    //FOaux = calculaFO(solucaoAux);               
                    // retiro a tarefa da maquina que ela ta e coloco na maquina que o for ta
                    // calculo a FO e verifico se é menor e se é diferente de 0
                    // se tiver um resultado melhor, eu mudo a flag para quando terminar, ele mudar a solução original
                    // eu vou precisar criar mais uma estrutura de solução auxiliar.
					
                }
                //contador++;
            }
            for(int i=0; i < qtdMaquinaUsada; i++){
                solucaoAux[i] = soluc[i];
            }
            FOaux = FOSolucao;
        }
        if (flag == 1)
        {
            for(int i=0; i < qtdMaquinaUsada; i++){
                soluc[i] = melhor[i];
            }
            FOSolucao = FOMelhor;
        }
        
        break;
    }
}

int estaNaMaquina(int idMaquina, int idTarefa){
    for(int i =0; i < solucaoAux[idMaquina].qtdReqAlocada; i++){
        if(solucaoAux[idMaquina].atividadeAlocada[i] == idTarefa)
            return idMaquina;
    }
    return -1;
}

int encontraMaquina(int idMaquina, int idTarefa){
    for(int i =0; i < solucaoAux[idMaquina].qtdReqAlocada; i++){
        if(solucaoAux[idMaquina].atividadeAlocada[i] == idTarefa)
            return idMaquina;
    }
    return -1;
}
