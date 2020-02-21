 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
/*-------Déclaration des structures-------*/
#define b 20
#define b2 20
typedef struct Tenreg
{
int Eff;
int cle;
//char *Nom;
//int N_tel;
} Tenreg;

typedef struct Tbloc
{
Tenreg T[b];
int Nb;
int lien;

} Tbloc;

typedef struct Tbloc2 
 { 
 Tenreg T[b2];
 int Nb ; 
 int lien ;
 }Tbloc2;

typedef struct Entete
{
int adrDerBloc;
// on peut rajouter ici toute information sur le fichier jugée utile
} Entete;

typedef struct Tcouple 
{
	int c ;
	int adr ;
} Tcouple;




typedef struct TObF
{
FILE *fichier;
Entete entete;
} TObF;

TObF *f;

/*--------------Fonction d'ouverture d'un fichier--------------*/
int ouvrir(TObF **f,char *chemin,char mode)
{
*f = malloc(sizeof(TObF));
char s[3];
if((mode == 'A') || (mode == 'a')) sprintf(s,"rb+");
else if ((mode == 'N') || (mode == 'n')) sprintf(s,"wb+");
else return 0;
(*f)->fichier = fopen(chemin, s);
if((*f)->fichier == NULL) return 0;
if((mode == 'A') || (mode == 'a'))
{
fread(&((*f)->entete), sizeof(Entete), 1, (*f)->fichier);
}
else if ((mode == 'N') || (mode == 'n'))
{
(*f)->entete.adrDerBloc = 0;
}
return 1;
}
/*--------------------------------------------------------------*/
/*--------------Fonction de fermeture d'un fichier--------------*/
void fermer(TObF *f)
{
//Sauvegarde de l'entête
rewind(f->fichier);
fwrite(&(f->entete), sizeof(Entete), 1, f->fichier);
fclose(f->fichier);
free(f);
}
/*--------------------------------------------------------------*/
/*-----------------Fonction de lecture d'un bloc----------------*/
void lireDir(TObF *f,int N_Bloc,Tbloc *buffer)
{
if(N_Bloc <= (f->entete).adrDerBloc)
{
fseek(f->fichier, sizeof(Entete) + (N_Bloc-1) * sizeof(Tbloc),SEEK_SET);
fread(buffer, 1, sizeof(Tbloc), f->fichier);
}
}
/*--------------------------------------------------------------*/
/*-----------------Fonction d'écriture d'un bloc----------------*/
void ecrireDir(TObF *f,int N_Bloc,Tbloc *buffer)
{
if(N_Bloc <= (f->entete).adrDerBloc)
{
fseek(f->fichier, sizeof(Entete) + (N_Bloc-1) * sizeof(Tbloc),SEEK_SET);
fwrite(buffer, 1, sizeof(Tbloc), f->fichier);
}
}
/*--------------------------------------------------------------*/
int entete(TObF *f,int i)
{
if(i == 1) return (f->entete).adrDerBloc;
// else if(i == 2) return (f->entete).nbEnreg;
// else if(i == 3) return (f->entete).indice_libre;
else return -1;
}
/*--------------------------------------------------------------*/
/*-------------Fonction de modification de l'entête-------------*/
void aff_entete(TObF *f, int i, int val)
{
if(i == 1) (f->entete).adrDerBloc = val;
}
/*--------------------------------------------------------------*/
/*--------------------L'allocation d'un bloc--------------------*/

int alloc_bloc(TObF *f)
{
aff_entete(f,1,entete(f,1)+1);
return entete(f,1);
}


void Charger(int i)
{
Tbloc buf;
ouvrir(&f,"test.bin",'N');
int Num=alloc_bloc(f);
int k;
for (k=0;k<10;k++)
{
buf.T[k].cle=k+100;
buf.T[k].Eff=0;

//buf.T[k].N_tel=k+100000;
}
buf.Nb=10;
ecrireDir(f,Num,&buf);
fermer(f);
printf("fin de charger\n");
}

void Afficher()
{
Tbloc buf;
ouvrir(&f,"test.txt",'A');
printf("\nAffichage-----------------------------------------------------------\n");
lireDir(f,entete(f,1),&buf);
printf("\nNb = %d\n",buf.Nb);
int k;
printf("les enreg : \n");
printf("______________________________\n");
for (k=1;k<13;k++)
{

printf("|cle : %d| eff: %d|\n",buf.T[k].cle, buf.T[k].Eff);

}
printf("______________________________\n");
fermer(f);
}


void supprimer(int i,int j)
{ Tbloc buf;
ouvrir(&f,"test.bin",'A');
lireDir(f,entete(f,i),&buf);
buf.T[j].Eff=1;
ecrireDir(f,i,&buf);
fermer(f);
}



int main()
{
	int n = 3;
	int i,j,k,NbI,cle = 0;; 
	Tenreg e;
	Tbloc buf1;
	TObF *fichier1 ;
	FILE *fichier2;
//	fichier2 = fopen("Index.txt","w");
	ouvrir(&fichier1,"test.txt",'n');
	i=1;
	j=1;
	NbI=-1;
	
	Tcouple *Index;
//	Index[0].c = 0;
	for(int k = 0; k<12;k++)
	{ 
		printf("entrer la cle \n");
		scanf("%d" , &(e.cle));
		e.Eff = 0;
	
		if ( j<=5)
		{ 
			
			buf1.T[j] = e;
			printf("la cle dans le fichier est %d \n",buf1.T[j].cle);
			j++;
		}
		else 
		{ 
			NbI ++ ;
			Index[NbI].c = buf1.T[j-1].cle ;
			printf("la cle insere dans l'index est de %d \n",Index[NbI].c);
			Index[NbI].adr = i;
			buf1.Nb = j-1;
			buf1.lien = -1;
			
			ecrireDir(fichier1,i,&buf1);
			
			i++;
			buf1.T[1] = e;
			j=2;
		}
		
	}
	
	NbI ++ ;
	Index[NbI].c = buf1.T[j-1].cle;
	buf1.Nb = j-1;
	buf1.lien = -1;
	
	aff_entete(fichier1,1,i);
	ecrireDir(fichier1,1,&buf1);
	fermer(fichier1);
	printf("la taille d'un element est de : %d\n",sizeof(Index[0]));
	printf("la taille du tableau est de : %d\n",sizeof(Index));
	int el = sizeof(Index[0]);
	int tablEl = sizeof(Index);
	printf("la taille du tableau est de : %d\n", tablEl/el );
			for(int l=0;l<3;l++) 
			{ 
				printf("|%d  | ",Index[l].c);
			}
fichier2 = fopen("Index.txt","w");
	for(int l=0;l<3;l++) 
			{ 
				fprintf(fichier2,"%d | ",Index[l].c);
			}
			
	Afficher();
	
	
	 
	

return 0;
}
