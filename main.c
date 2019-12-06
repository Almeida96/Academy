#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//dados dos alunos
 struct aluno{
  int id;
  char nome[300];
  char cpf[15];
  char telefone[50];
  char email[100];
  };

//dados do prof
struct prof{
  char nome[50];
  char cpf[15];
  char tel[20];
  int id_prof;
  } ;

//dados da aula
struct aula{
  int id;
  char tipo[50]; 
  int dia;
  char horario[6];
  int minimo;
  int maximo;
  char faixa_etaria[6];
  struct aluno aluno_aula[100];
  struct prof prof_aula[100];
};

//funções doideras 
void cadasAlu();
void cadasProf();
void cadasAula();
void insAlu();

//void tira(struct aluno beyblade[]);//beyblade é UMA VARIAVEL do tipo struc aluno
void tiraNovaLinha(char *string);

//void salvarAula(struct aula[]);
struct prof prof[100];
struct aluno aluno[100];
struct aula aula[100];

void tiraNovaLinha(char *string);



int main(void) {
   // Programa Principal 
   
    		
  int opcao=1;
        char *teste; // TIRARRRR
  do{
      puts("Academia superação\nDigite a opcao: ");
      printf("\n 1-Cadastrar Aluno.\n 2-Cadastrar Professor \n 3-Cadastrar Turma \n 4-Inscrever Aluno \n 5-Cancelar Inscrição \n 6-Fechar Turmas \n 7-Confirmar Turmas \n 8-Fechar Programa\nR:");
      scanf("%d",&opcao);
     
      //opcao = lerOpcao();
      switch (opcao){
        case 1: // 1- Cadastra alun
          cadasAlu();
        break;
    
        case 2: // 2-  Cadastrar professor
          cadasProf();
        /* APAGAR
        getchar();
        teste = malloc(300);
        fgets(teste, 300, stdin);
        tiraNovaLinha(teste);
        puts(teste);*/
        break;
    
    	  case 3: // 3- Cadastrar turma
          cadasAula();
        break;
                 
        case 4:  // 4- Inscrever aluno
          insAlu();
          
        break;
                 
        case 5:  // 5- Cancelar inscrição
        break;
           
        case 6:  // 6- Fechar turmas
        break;
               
        case 7:  // 7- Confirmar turmas
        break;
    
    	  case 8:  // 8- Fechar o programa
          //salvarAula(aula);
    	    exit(0);
        break;
    
    	  default:
    	    printf("Opcao invalida, tente novamente.\n");
      	break;
                          
      }
    }while (opcao>0 && opcao<9);
    
    
  return 0;
} // fim do main

/*void tira(struct aluno beyblade[]){
  
  if(aluno[i].beyblade[strlen(aluno[i].beyblade)-1]=='\n')
    {
      aluno[i].beyblade[strlen(aluno[i].beyblade)-1]='\ 0';
    }
}//função tira \n*/

void tiraNovaLinha(char *string) {
  int tam = strlen(string);

  if(string[tam - 1] == '\n') {
    string[tam - 1] = '\0';
  }
}

void cadasAlu(){
  int res,i=0,c;

  FILE*fp=fopen("alunoCadastro.txt","a");
  if(fp == NULL){
    printf("\n deu ruim \n");
    exit(1);
    }
  do{
    printf("\n*********Entre com suas informações*********\n");
    
    printf("\n Seu Id:");
    scanf(" %d",&aluno[i].id);
    setbuf(stdin, NULL);
    
    printf("\n Seu nome:");
    fgets(aluno[i].nome,300,stdin);
    tiraNovaLinha(aluno[i].nome);
    
    printf("\n Seu cpf:");
    fgets(aluno[i].cpf,15,stdin);
    tiraNovaLinha(aluno[i].cpf);

    printf("\n Seu telefone:");
    fgets(aluno[i].telefone,50,stdin);
    tiraNovaLinha(aluno[i].telefone);
    
    printf("\n Seu email:");
    fgets(aluno[i].email,100,stdin);
    tiraNovaLinha(aluno[i].email);
    
    fprintf(fp,"%d || %s || %s || %s || %s \n ",aluno[i].id,aluno[i].nome,aluno[i].cpf,aluno[i].telefone,aluno[i].email);
    
    setbuf(stdin, NULL);
    printf("Gostaria de continuar?\n Sim=1 / Não=2");
    scanf("%d",&res);
    i++;
  }while(res==1);
  fclose(fp);
}

//cadastro do prof 

void cadasProf(){
  int res,i=0,c;

  FILE*fp=fopen("profCadastro.txt","a");
  if(fp == NULL){
    printf("\n deu ruim \n");
    exit(1);
    }

  do{
    printf("\n*********Entre com suas informações*********\n");
    
    printf("\n Seu Id:");
    scanf(" %d",&prof[i].id_prof);
    setbuf(stdin, NULL);
    
    printf("\n Seu nome:");
    fgets(prof[i].nome,50,stdin);
    tiraNovaLinha(prof[i].nome);
    
    printf("\n Seu cpf:");
    fgets(prof[i].cpf,15,stdin);
    tiraNovaLinha(prof[i].cpf);

    printf("\n Seu telefone:");
    fgets(prof[i].tel,20,stdin);
    tiraNovaLinha(prof[i].tel);
    
    
    fprintf(fp,"%d || %s || %s || %s \n ",prof[i].id_prof,prof[i].nome,prof[i].cpf,prof[i].tel);
    
    setbuf(stdin, NULL);
    printf("Gostaria de continuar?\n Sim=1 / Não=2");
    setbuf(stdin, NULL);
    scanf("%d",&res);
    i++;
  }while(res==1);
  fclose(fp);
}

void cadasAula(){
  int res,i=0,c;

  FILE*fp=fopen("aulaCadastro.txt","a");
  if(fp == NULL){
    printf("\n deu ruim \n");
    exit(1);
  }

  
  do{
    printf("\n*********Entre com as informações*********\n");
    
    
    printf("\n Id da aula:");
    scanf(" %d",&aula[i].id);
    setbuf(stdin, NULL);
    
    printf("\n Tipo de aula:");
    fgets(aula[i].tipo,50,stdin);
    tiraNovaLinha(aula[i].tipo);

    printf("\n Dia da aula:");
    scanf(" %d",&aula[i].dia);
    setbuf(stdin, NULL);
    
    printf("\n Horário da aula:");
    fgets(aula[i].horario,6,stdin);
    tiraNovaLinha(aula[i].horario);

    printf("\n Mínimo de alunos:");
    scanf(" %d",&aula[i].minimo);
    setbuf(stdin, NULL);

    printf("\n Máximo de Alunos:");
    scanf(" %d",&aula[i].maximo);
    setbuf(stdin, NULL);

    printf("\n Faixetária:");
    fgets(aula[i].faixa_etaria,6,stdin);
    tiraNovaLinha(aula[i].faixa_etaria);
    
    
    fprintf(fp,"%d || %s || %d || %s || %d || %d || %s \n ",aula[i].id,aula[i].tipo,aula[i].dia,aula[i].horario,aula[i].minimo,aula[i].maximo,aula[i].faixa_etaria);
    
    setbuf(stdin, NULL);
    printf("Gostaria de continuar?\n Sim=1 / Não=2");
    setbuf(stdin, NULL);
    scanf("%d",&res);
    setbuf(stdin, NULL);
    i++;
  }while(res==1);
  fclose(fp);
}

void insAlu(){
  int cadas,cadas2,tam,i=0,j=0;
  int aula,aula1;
  printf("Deseja cadastrar qual aluno?(entre com sua id)\nR:");
  scanf("%d",&cadas);
  setbuf(stdin, NULL);
  printf("Deseja cadastrar em qual aula?");
  scanf("%d",&aula);

  FILE*fp=fopen("alunoCadastro.txt","r");
  if(fp == NULL)exit(1);
  
  FILE*fp1=fopen("aula.txt","w");
  if(fp == NULL)exit(1);
  
  while(aula[i].id,aula[i].tipo,aula[i].dia,aula[i].horario,aula[i].minimo,aula[i].maximo,aula[i].faixa_etaria)!=EOF{
    if(aula==aula[i].tipo){
      while(fscanf(fp,"%d || %s || %s || %s || %s \n ",aluno[i].id,aluno[i].nome,aluno[i].cpf,aluno[i].telefone,aluno[i].email)!=EOF){
        cadas2=aluno[i].id;
        if(cadas==cadas2){
          fputc(aluno[i].nome,fp1);
        }
      i++;
      }
    }
  }

    
} 