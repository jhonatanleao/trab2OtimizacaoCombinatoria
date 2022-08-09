#ifndef MAIN_HPP_INCLUDED
#define MAIN_HPP_INCLUDED

#define MAX_REQ 100
#define INF 999999999

typedef struct tsolucao
{
	int atividadeAlocada[MAX_REQ];
	int FO;
	int qtdReqAlocada;
}solucao;

typedef struct tmatTarefa
{
	int idTarefa;
	int vetPredecessor[MAX_REQ];
	int vetSucessor[MAX_REQ];
	int custo;
	int qtdPredecessor;
	int qtdSucessor;
}matTarefa;

int matSucessor[MAX_REQ][MAX_REQ];
int matPredecessor[MAX_REQ][MAX_REQ], matPredecessorAux[MAX_REQ][MAX_REQ];
int vetPosCandidatas[MAX_REQ], qtdVetCandidatas, vetAtivAlocadas[MAX_REQ], qtdvetAlocada, vetAtivSucessor[MAX_REQ], qtdVetSucessor;
int qtdMaquinaUsada, FOaux, FOSolucao, FOSolucao2, qtdTarefas, FOMelhor;
int vetAtvInicial[MAX_REQ], qtdAtvInicial;
solucao soluc[MAX_REQ], solucaoAux[MAX_REQ], solucaoAux2[MAX_REQ], melhor[MAX_REQ];
matTarefa vetTarefa[MAX_REQ];

void lerArquivo();
void heuGulosa();
int buscaid(int idAtividade);
void ativInicial();
void organizaVetorCandidata(int posicaoRetirada);
void organizaVetor(int posicaoRetirada);
int insereTarefa(int idAtividade);
int maiorPeso();
void printSolucao();
int calculaFO(solucao s[MAX_REQ]);
int buscaIdAtvSoluc(int idAtividade, int idMaquina);
void ajusteVetPosAlocadas(int idMaquina);
void excluirTarefa(int idAtividade, int idMaquina);
void preencheVetPosAlocadas(int idAtivade);
void geraVizinha();
int verificaViabilidadeAtv(int idAtividade);
void printSolucao2();
int ajustaVetSucessor(int idTarefa, int tarefa);
int heuBLMM();
int estaNaMaquina(int idMaquina, int idTarefa);
int encontraMaquina(int idTarefa);
int encontraId(int idTarefa);

#endif // MAIN_HPP_INCLUDED
