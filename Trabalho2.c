#include <stdio.h>
#include <stdlib.h>
#define MM  1 //Ordem das paginas internas
#define MM2 2 //Ordem das paginas externas
#define FALSE 0
#define TRUE  1

typedef int TipoChave;
typedef struct TipoRegistro {
	TipoChave Chave;
	/* outros componentes */
} TipoRegistro;
typedef enum {
	Interna, Externa
} TipoIntExt;
typedef struct TipoPagina *TipoApontador;
typedef struct TipoPagina *TipoAcessoSequencial;
typedef struct TipoPagina {
	TipoIntExt Pt;
	union {
		struct {
			int ni;
			TipoChave ri[MM];
			TipoApontador pi[MM + 1];
		} U0;
		struct {
			int ne;
			TipoRegistro re[MM2];
			TipoAcessoSequencial prox;
		} U1;
	} UU;
} TipoPagina;

void Inicializa(TipoApontador *Dicionario) {
      *Dicionario = NULL;
}

void ImprimeCrescente(TipoAcessoSequencial lista){
	while(lista != NULL){
		int i;
		for(i = 0; i < lista->UU.U1.ne; i++){
			printf("%d\t" , lista->UU.U1.re[i].Chave);
			//Acrescente aqui os outros campos do registro
		}
		lista = lista->UU.U1.prox;
	}
}

void ImprimeDecrescente(TipoAcessoSequencial lista){
	while(lista->UU.U1.prox != NULL)
		ImprimeDecrescente(lista->UU.U1.prox);		
	int i;
	for(i = lista->UU.U1.ne; i >= 0; i--){
		printf("%d\t" , lista->UU.U1.re[i].Chave);
		//Acrescente aqui os outros campos do registro
	}
	return;
}

void ImprimeIntervalo (TipoApontador Ap, TipoChave ChaveIni, TipoChave ChaveFim){
	if (Ap->Pt == Interna){ 
		int i = 1;
		while (i < Ap->UU.U0.ni && ChaveIni > Ap->UU.U0.ri[i - 1]) 
			i++;
		if (ChaveIni < Ap->UU.U0.ri[i - 1]) 
			ImprimeIntervalo(Ap->UU.U0.pi[i - 1], ChaveIni, ChaveFim);
		else 
			ImprimeIntervalo(Ap->UU.U0.pi[i], ChaveIni, ChaveFim); 
		return;
	}
	
	if (Ap->Pt == Externa){
		while (Ap != NULL){
			int i;
			for(i = 0; i < Ap->UU.U1.ne; i++){
				if (Ap->UU.U1.re[i].Chave >= ChaveIni && Ap->UU.U1.re[i].Chave <= ChaveFim)
					printf("%d", Ap->UU.U1.re[i].Chave);
			}
			Ap = Ap->UU.U1.prox;
		}
		return;
	}
}

void Pesquisa(TipoRegistro *x, TipoApontador *Ap) {
	int i;
	TipoApontador Pag;
	Pag = *Ap;
	if ((*Ap)->Pt == Interna) { 
		i = 1;
		while (i < Pag->UU.U0.ni && x->Chave > Pag->UU.U0.ri[i - 1]) 
			i++;
		if (x->Chave < Pag->UU.U0.ri[i - 1]) 
			Pesquisa(x, &Pag->UU.U0.pi[i - 1]);
		else 
			Pesquisa(x, &Pag->UU.U0.pi[i]); 
		return;
	}
	i = 1;
	while (i < Pag->UU.U1.ne && x->Chave > Pag->UU.U1.re[i - 1].Chave) 
		i++;
	if (x->Chave == Pag->UU.U1.re[i - 1].Chave) 
		*x = Pag->UU.U1.re[i - 1];
	else 
		printf("TipoRegistro nao esta presente na arvore\n");
}


void InsereNaPagina(TipoApontador Ap, TipoRegistro Reg, TipoApontador ApDir){ 
	short NaoAchouPosicao;
	int k;
	if (Ap->Pt == Externa){
		k = Ap->UU.U1.ne;  
		NaoAchouPosicao = (k > 0);
		while (NaoAchouPosicao) { 
			if (Reg.Chave >= Ap->UU.U1.re[k-1].Chave) { 
				NaoAchouPosicao = FALSE;
				break;
			}
			Ap->UU.U1.re[k] = Ap->UU.U1.re[k-1];
			k--;
			if (k < 1) 
				NaoAchouPosicao = FALSE;
		}
		Ap->UU.U1.re[k] = Reg; 
		Ap->UU.U1.ne++;
	} else {
		k = Ap->UU.U0.ni;  
		NaoAchouPosicao = (k > 0);
		while (NaoAchouPosicao) { 
			if (Reg.Chave >= Ap->UU.U0.ri[k-1]) { 
				NaoAchouPosicao = FALSE;
				break;
			}
			Ap->UU.U0.ri[k] = Ap->UU.U0.ri[k-1];
			Ap->UU.U0.pi[k+1] = Ap->UU.U0.pi[k];
			k--;
			if (k < 1) 
				NaoAchouPosicao = FALSE;
		}
		Ap->UU.U0.ri[k] = Reg.Chave; 
		Ap->UU.U0.pi[k+1] = ApDir;
		Ap->UU.U0.ni++;
	}
} 

void Ins(TipoRegistro Reg, TipoApontador Ap, short *Cresceu, TipoRegistro *RegRetorno,  TipoApontador *ApRetorno){ 
	long i = 1; 
	long j;
	TipoApontador ApTemp;
	
	//Verifica se a pagina esta vazia:
	if (Ap == NULL){ 
		*Cresceu = TRUE; 
		(*RegRetorno) = Reg; 
		(*ApRetorno) = NULL;
		return;
	}
	
	//Pesquisa posição do elemento nas paginas internas
	if (Ap->Pt == Interna) { 
		i = 1;
		while (i < Ap->UU.U0.ni && Reg.Chave > Ap->UU.U0.ri[i - 1]) 
			i++;
		if (Reg.Chave < Ap->UU.U0.ri[i - 1]) 
			Ins(Reg, Ap->UU.U0.pi[i - 1], Cresceu, RegRetorno, ApRetorno);
		else 
			Ins(Reg, Ap->UU.U0.pi[i], Cresceu, RegRetorno, ApRetorno);
		return;
	}
	
	//Ao encontrar pagina externa onde o elemento deveria estar, faz a 
	//busca pelo elemento dentro da pagina
	i = 1;
	while (i < Ap->UU.U1.ne && Reg.Chave > Ap->UU.U1.re[i - 1].Chave) 
		i++;
	
	//Elemento esta presente na arvore
	if (Reg.Chave == Ap->UU.U1.re[i - 1].Chave){	
		printf(" Erro: Registro ja esta presente\n"); 
		*Cresceu = FALSE;
		return;
	}

	//Verifica a posicao do elemento na pagina
	if (Reg.Chave < Ap->UU.U1.re[i-1].Chave) 
		i--;
	
	if (!*Cresceu) 
		return;
	
	//Pagina tem espaco, entao insere o elemento
	if (Ap->UU.U1.ne < MM2){ 
		InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
		*Cresceu = FALSE;
		return;
	}
	
	//Overflow: Pagina tem que ser dividida
	*Cresceu = TRUE;
	if (Ap->Pt == Externa){	

		//Cria pagina externa temporaria
		ApTemp = (TipoApontador)malloc(sizeof(TipoPagina));
		ApTemp->Pt = Externa;
		ApTemp->UU.U1.ne = 0;
		
		//Verifica se é necessário emprestar um elemento para a pagina a direita
		//ou se deve inseri-lo direto na pagina nova
		if (i < MM2/2 + 1) { 
			InsereNaPagina(ApTemp, Ap->UU.U1.re[MM2-1], NULL);
			Ap->UU.U1.ne--;
			InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
		} else 
			InsereNaPagina(ApTemp, *RegRetorno, *ApRetorno);
		
		//Transfere a metade da direita da pagina que encheu para a pagina nova
		for (j = MM2/2 + 1; j <= MM2; j++) 
			InsereNaPagina(ApTemp, Ap->UU.U1.re[j-1], NULL);
		Ap->UU.U1.ne = MM2/2;  
		
		//Retorna o primeiro elemento da pagina nova uma recursao acima, para inserir 
		//o seu indice na pagina pai.
		*RegRetorno = Ap->UU.U1.re[MM2/2];  
		*ApRetorno = ApTemp;

	} else {

		//Cria pagina interna temporaria
		ApTemp = (TipoApontador)malloc(sizeof(TipoPagina));
		ApTemp->Pt = Interna;
		ApTemp->UU.U0.ni = 0; 

		//Verifica se é necessário emprestar um elemento para a pagina a direita
		//ou se deve inseri-lo direto na pagina nova
		if (i < MM/2 + 1){
			TipoRegistro RegTemp;
			RegTemp.Chave = Ap->UU.U0.ri[MM-1]; 
			InsereNaPagina(ApTemp, RegTemp, *ApRetorno);
			Ap->UU.U0.ni--;
			InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
		} else
			InsereNaPagina(ApTemp, *RegRetorno, *ApRetorno);

		//Transfere a metade da direita da pagina que encheu para a pagina nova
		for (j = MM/2 + 1; j <= MM; j++){ 
			TipoRegistro RegTemp;
			RegTemp.Chave = Ap->UU.U0.ri[j-1];
			InsereNaPagina(ApTemp, RegTemp , *ApRetorno);
		}        
		Ap->UU.U0.ni = MM/2;  

		//Retorna o primeiro elemento da pagina nova uma recursao acima, para inserir 
		//o seu indice na pagina pai.
		//ApTemp->UU.U0.pi[0] = Ap;
		TipoRegistro RegTemp;
		RegTemp.Chave = ApTemp->UU.U0.ri[0];
		*RegRetorno = RegTemp;  
		*ApRetorno = ApTemp;

	}
}

void Insere(TipoRegistro Reg, TipoApontador *Ap){ 
	short Cresceu;
	TipoRegistro RegRetorno;
	TipoPagina *ApRetorno, *ApTemp;

	Ins(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno);

	//Arvore cresce na altura pela raiz
	if (Cresceu){
		//Checa se a arvore esta vazia e cria a pagina inicial
		if (*Ap == NULL){
			ApTemp = (TipoPagina *)malloc(sizeof(TipoPagina));
			ApTemp->Pt = Externa;
			ApTemp->UU.U1.ne = 1;
			ApTemp->UU.U1.re[0] = Reg; 
			ApTemp->UU.U1.prox = NULL;
			*Ap = ApTemp;
			return;
		}
		//Senao cresce uma pagina interna
		ApTemp = (TipoPagina *)malloc(sizeof(TipoPagina));
		ApTemp->UU.U0.ni = 1; 
		ApTemp->UU.U0.ri[0] = RegRetorno.Chave;
		ApTemp->UU.U0.pi[1] = ApRetorno;
		ApTemp->UU.U0.pi[0] = *Ap;  
		*Ap = ApTemp;
	}
}















void GeraArvore(TipoApontador *Ap, long nNos){
	TipoRegistro Reg;
	long i;

	for(i=0;i<nNos;i++){
		Reg.Chave= i;
		Insere(Reg,Ap);
	}
	
}

void MostraMenu (){
	printf("\n\nMenu Principal\n\n\
			1)  Gera a arvore;\n\
			2)  Insere um registro na arvore;\n\
			3)  Remove um registro da árvore;\n\
			4)  Pesquisa por um registro na árvore;\n\
			5)  Imprime os dados de todos os registros da árvore em ordem crescente;\n\
			6)  Imprime os dados de todos os registros da árvore em ordem descrescente;\n\
			7)  Imprime os dados de todos os registros da árvore ue se encontram dentro de um intervalo;\n\
			8)  Altura da arvore;\n\
			0)  Sair\n\
			\nDigite uma opcao : \n");
}

int main(){
	TipoApontador Ap;
	Inicializa(&Ap);
	MostraMenu();
    int op;
    while (op != 0){
        MostraMenu();
        scanf("%d",&op);
        switch (op){
            case 1: {
				long nNos;
				printf("\n Entre com o numero de nos: ");
				scanf("%ld",&nNos);
				GeraArvore(&Ap,nNos);
				printf("\n Arvore criada");				
                break;
            }    
            case 2: {
				int elem;
				TipoRegistro Reg;
				printf("\n Entre com o elemento a ser inserido: ");
				scanf("%d",&elem);
				Reg.Chave = elem;
				Insere(Reg,&Ap);
				printf("\n Elemento inserido");	
                break;
            }    
            default :
				printf("\nOpcao invalida!\n");
                break;
            
        }
    }
	return 0;
}
/*
int main(int argc, char *argv[]){ 
    TipoApontador Ap;
    TipoRegistro Reg;
    Inicializa(&Ap);
    int i;
    Reg.Chave = 50;
    Insere(Reg,&Ap);
    
    for (i = 1; i <= 100; i++){
        Reg.Chave = i;
		Insere(Reg,&Ap);
		printf ("Registro inserido: %d\n",Reg.Chave);
		Pesquisa(&Reg,&Ap);
	}
    
    //ImprimeCrescente(Lista);
    system("PAUSE");
    return 0; 
} */
