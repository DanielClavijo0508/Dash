#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct{
	char *PGN;
	char *data;
}trabajadores;

trabajadores *trab;

void vaciar(char temp[])
{
	for(int i=0; i<50; i++)
	{
		temp[i]='\0';
	}
}
void copiarPGN(char temp[], int i)
{
	int N =strlen(temp)+1;
	trab[i].PGN=(char*)malloc(N*sizeof(char));
	if(trab[i].PGN==NULL)
	{
		printf("no se ha podido reservar memoria \n");
		exit(1);
	}
	strcpy(trab[i].PGN,temp);
}

void copiardata(char temp[], int i)
{
	int N =strlen(temp)+1;
	trab[i].data=(char*)malloc(N*sizeof(char));
	if(trab[i].data==NULL)
	{
		printf("no se ha podido reservar memoria \n");
		exit(1);
	}
	strcpy(trab[i].data,temp);
}


int main ()
{
while(1){
	char temp[50];
	char aux;
	int cont =0;
	FILE *f;
	f=fopen("PGN.txt","r");
	if(f==NULL)
	{
		printf("no se puede acceder al fichero \n");
		exit(1);
	}
	while (!feof(f))
	{
		fgets(temp, 50 , f);
		cont++;
	}
	rewind(f);
	trab =(trabajadores*)malloc(cont*sizeof(trabajadores));
	if(trab==NULL)
	{
		printf("no se ha podido reservar en memoria \n");
		exit(1);
	}
	for(int i=0; !feof(f);i++)
	{
		vaciar(temp);
		aux='0';
		for(int j=0;aux!='-';j++)
		{
			aux=fgetc(f);
			if(aux!='-')
			{
				temp[j]=aux;
			}
		}		
		copiarPGN(temp,i);
		fgets(temp, 17, f);
		copiardata(temp,i);
		printf("PGN: %s, Data: %s \n", trab[i].PGN, trab[i].data);
	}
//	system("pause");
//	return 0;
}
}
