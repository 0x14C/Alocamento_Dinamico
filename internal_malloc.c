#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

// ============================================== ESTRUTURA DA LISTA ENCADEADA ==============================================

typedef struct encadeada_block{

 bool ocupado;
 int tamanho;
 struct encadeada_block *proximo;
 struct encadeada_block *fim;
}block;

block *heap_head;
bool  heap_initialised = false; //variavel responsavel por verificar se há um node criado 

// -------------------------------------------------------------------------------------------------------------------



// ============================================== PROTOTIPO DAS FUNÇÕES ==============================================

void inicial_heap();
void debug_heap();
void novo_bloco_deMemoria(int tamanho, block *ptrAtual, block *novo_bloco);
void divisao_doBloco(int alloc_size, block *ptrAtual);
void *alloc(int tamanho);
void frag();
void freeZ(void *ptr);
// -------------------------------------------------------------------------------------------------------------------



// ============================================== FUNÇÃO PRINCIPAL(ENTRYPOINT) ==============================================
int main(){

printf("oi");

int *ptrzor = NULL;

ptrzor = alloc(sizeof(int));

if(ptrzor == NULL){

	printf("erro ao alocar memoria");
}else{
	printf("MEMORIA: %x\n\n", ptrzor);
	*ptrzor = 50;
	printf("VALOR: %d\n", *ptrzor);
	freeZ(ptrzor);
}

return 0;
}

// -------------------------------------------------------------------------------------------------------------------


// ============================================== INICIO DAS FUNÇÕES ==============================================

void inicial_heap()
{
 if(!heap_initialised){
	heap_head 	   	   = sbrk(sizeof(block)); //a função sbrk e da biblioteca unistd.h, ela será responsável por redimensionar o tamanho da heap no espaço de memoria que esta em execução.
	heap_head->ocupado = true; 
	heap_head->tamanho = sizeof(block);
	heap_head->proximo = NULL;
	heap_head->fim 	   = heap_head + sizeof(block);

	heap_initialised = true; /*quando for verdadeiro o condicional not vai mudar a saida para false e nao vai executar mais. Essa função servirá apenas para criar 
	o primeiro node da lista encadeada.*/
		     	    }
}

void debug_heap()
{
 block *ptrAtual = heap_head;
 while(ptrAtual != NULL){
	if(ptrAtual == heap_head)
	{
		fprintf(stderr, ">>> HEAD <<<\n");
	}
	fprintf(stderr, "=== BLOCO ATUAL DE MEMORIA: %p ===\n", ptrAtual);
	fprintf(stderr, "\ttotal tamanho: %d\n", ptrAtual->tamanho);
	fprintf(stderr, "\ttamanho alocado: %ld\n", ptrAtual->tamanho - sizeof(block));
	fprintf(stderr, "\tocupado: %d\n", ptrAtual->ocupado);
	fprintf(stderr, "\tproximo: %p\n", ptrAtual->proximo);
	fprintf(stderr, "\tfim: %p\n", ptrAtual->fim);
	fprintf(stderr, "=== FIM DO BLOCO DE MEMORIA ===\n");
	
	ptrAtual = ptrAtual->proximo;
}
	fprintf(stderr, ">>> VAZIO <<<\n\n");
}//fim debug_heap()

void novo_bloco_deMemoria(int tamanho, block *ptrAtual, block *novo_bloco)
{//novo node na lista encadeada
	int novo_tamanho_bloco = sizeof(block) + tamanho;
	novo_bloco->tamanho = novo_tamanho_bloco;
	novo_bloco->ocupado = true;
	novo_bloco->fim = novo_bloco + sizeof(block);
	novo_bloco->proximo = NULL;

	ptrAtual->proximo = novo_bloco; //o ultimo node anterior apontará agora para o novo ultimo node que esta contido em novo_bloco.
		
}

void divisao_doBloco(int alloc_size, block *ptrAtual)
{
	if(ptrAtual->tamanho == alloc_size + sizeof(block))
	{
	ptrAtual->ocupado = true;
	ptrAtual->fim = ptrAtual + sizeof(block);
	return;
	} 
	else if(ptrAtual->tamanho >= alloc_size + sizeof(block))
	{
		block *novo_bloco = alloc_size + ptrAtual + sizeof(block);
		novo_bloco->proximo = ptrAtual->proximo;
		novo_bloco->ocupado = false;
		novo_bloco->fim = novo_bloco + sizeof(block);
		novo_bloco->tamanho = ptrAtual->tamanho - alloc_size - sizeof(block);

		ptrAtual->proximo = novo_bloco;
		ptrAtual->tamanho = alloc_size + sizeof(block);
		ptrAtual->ocupado = true;
	}
}//fim divisao_doBloco

void *alloc(int tamanho)
{
 inicial_heap(); //Vamos chamar essa função para verificar se há um node existente que já contém um endereço de memória da heap alocado.
 block *ptrAtual = heap_head; //heap_head conterá o endereço de memoria alocado.
 block *novo_bloco;

 while(ptrAtual->proximo != NULL)
 {
	/*
		Como heap_head contém o endereço de memória do primeiro alocamento, vamos verificar se nessa lista encadeada em block se temos outros nodes, vamos fazer
		essa verificação na variavel de ponteiro proximo que sempre apontará para o próximo node ou seja para proximo node que contém endereço de memoria alocado 
		na heap.
	*/
	if(ptrAtual->ocupado == false && ptrAtual->tamanho >= tamanho + sizeof(block))
	{
		fprintf(stderr, "Memoria liberada para uso: %p!\n", ptrAtual);
		break; //encerra o loop while caso for encontrado um bloco livre.
	}
	ptrAtual = ptrAtual->proximo; //"incrementa" para que while consiga ir para outro node.

 } 
 
 if(ptrAtual->proximo == NULL)
 /*
 	Caso o primeiro node a ser consultado e seu elemento proximo estiver contendo o valor NULL, significa que aquele node é o unico da lista encadeada.
*/
 {
	novo_bloco = sbrk(sizeof(block) + tamanho);
	novo_bloco_deMemoria(tamanho, ptrAtual, novo_bloco);
 }
 else if(ptrAtual->ocupado == false && ptrAtual->tamanho >= tamanho + sizeof(block)){
	divisao_doBloco(tamanho, ptrAtual);
	novo_bloco = ptrAtual;
}else{
	fprintf(stderr, "Erro.\n");
}

 return novo_bloco->fim;

} //fim alloc

void frag(){
 block *ptrAtual = heap_head;
 block *prev = NULL;

 while(ptrAtual != NULL)
 {
	if(ptrAtual != heap_head && (prev->ocupado == false && ptrAtual->ocupado == false))
	{
		prev->tamanho = prev->tamanho + ptrAtual->tamanho;
		prev->proximo = ptrAtual->proximo;
		
		ptrAtual = prev;
		ptrAtual = ptrAtual->proximo;
	}else   {
		prev = ptrAtual;
		ptrAtual = ptrAtual->proximo;
		}
	}
}//fim frag

void freeZ(void *ptr)
{

 block *ptrAtual = heap_head;
 while(ptrAtual->proximo != NULL && ptrAtual->fim != ptr)
 {
	ptrAtual = ptrAtual->proximo;
 }

 if(ptrAtual->fim == ptr){
	ptrAtual->ocupado = false;
	fprintf(stderr, "esvaziando o bloco %p com os dados %p com sucesso!\n", ptrAtual, ptrAtual->fim);
} else {
	fprintf(stderr, "Falha - verifique se o endereco %p eh um alocamento feito pelo ponteiro?\n", ptr);
}

frag();

} //fim free


// -------------------------------------------------------------------------------------------------------------------
