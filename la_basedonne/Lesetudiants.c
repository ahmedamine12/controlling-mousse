#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// la structure qui contient les informations des etudiants
typedef struct
{
    char nom[25];
    char prenom[25];
    char date_naiss[11];
    char CNE[11];
    float Tab_note[12];
    float moyenne;
}info_etud;

 
// la structure de la liste qui se trouve dans chaque case du tableau de la base
typedef struct cel
{
    info_etud etud; //un étudiant
    struct cel* svt; 
}liste_base;

liste_base  *Tab_BaseEtud[26][26];
/**********************/

/**********************/
typedef struct cell
{
    char nom[25];
    char CNE[11];
    struct cell* svt;
}etud_2info;
/*cette structure va contenir deux liste une pour 
les etudiants qui ont valide et une autre pour les etudiants qui ont echoues*/
typedef struct
{
    etud_2info* valide_liste;//liste des etudiants qui ont valides
    etud_2info* echoue_liste;//liste des etudiants qui n'ont pas valides
}liste_validation;
 liste_validation *liste_valid_echoue;
/**********************
   statistique des modules
/**********************/
typedef struct
{
    int valide;
    int compense;
    int echoue;
}statisModule;
statisModule tableau_statis[12];







// initialisation de toue les liste de la base de donnee
void initialisation_base()
{
    // parcourir le tableau de deux dimension
    for(int i=0;i<26;i++)
   {
       for(int j=0;j<26;j++)
       {
           Tab_BaseEtud[i][j]=NULL;
       }
   }
}///fin fonction

/**********************
Nom du fonction : cherche_cles
Entrées         : le nom d'etudiant
sortie:         :on va avoir les deux cles
Description     : chercher les deux cles pour acceder a la base
***********************/
void cherche_cles(char nom[25],int *cle1,int *cle2)
{
      char car;
      car=nom[0];
     (*cle1)=(int)car-'A';
      car=nom[1];
     (*cle2)=(int)car-'A';
}
/***********************
Nom du fonction : est_ce_que_valide
Entrées         : info d'etudiant
Sorties         : retourne 1 si l'étudiant valide l'année et 0 sinon
Description     : cette fonction pemet savoir est ce que l'etudiant
                  passe on argument a validé
************************/
int est_ce_que_valide(info_etud etu)
{
    int modu_nnvalide=0,// nombres des modules non valides
    modu_compense=0;// nombres des modules  valides
    //parcourir la table des notes
    for(int i=0 ; i<12 ; i++)
    {
        //si le module a une note
        if(etu.Tab_note[i]!=-1)
        {
          ///si elle est inferieur a 10 incrementer modu_nnvalide
          if( etu.Tab_note[i] < 10 ) modu_nnvalide++;
          ///si elle est entre 10 et 12  incrementer modu_compense
          else if( ( etu.Tab_note[i]>= 10) && (etu.Tab_note[i] < 12) )modu_compense++;
        }
    }
    // pour q'un etudiant valide il doit verifier ces conditions
    if( (etu.moyenne>=12) && (modu_nnvalide==0) && (modu_compense<=2) )
            return((int)1);// donc il a validé
    return((int)0);
}///fin fonction

/***********************
Nom du fonction : creer_cellule_etud
Entrées         : (info_etud etu) : un étudiant
Sorties         : retourne la cellule crée
Description     : creer un info_etud
**********************/
liste_base* creer_info_etud(info_etud etu)
{
    liste_base* nw_etud;//nouvelle cellule
     //allocation memoire
    nw_etud=(liste_base*)malloc(sizeof(liste_base));
    ///teste d'allocation
    if(!nw_etud)
    {
        printf("\nErreur d'allocation");
        exit(-1);
    }
    ///initialisation
    nw_etud->etud=etu;
    nw_etud->svt=NULL;
return((liste_base*)nw_etud);
}///fin fonction
/***********************
 Insertion ordonne des informations d'un etudiants
/************************/
liste_base* insertion_ord_nom(liste_base* liste,info_etud etu,int* etat_inser)
{
    liste_base *info_etud,*crt;// nouvelle cellule
     ///creation
    info_etud=creer_info_etud(etu);
    (*etat_inser)=0;// le resultat de l'insertion bien faites(1) ou non(0)
    ///si la liste est vide
    ///alors retourner la nouvelle cellule
    if(!liste)return((liste_base*)info_etud);
    ///sinon, inserer ordonner le nouveau etudiant
    //cas d'insertion au debut
    // comparaison avec la tete de la liste
    if(strcmp(info_etud->etud.nom,liste->etud.nom)<0)
    {
        info_etud->svt=liste;
        liste=info_etud;
        (*etat_inser=1);///l'insertion est biens faite
        ///retourner la liste
        return((liste_base*)liste);
    }
    //cas d'insertion au milieu, parcourir la liste des info_etud
    crt=liste;
    while(crt->svt)
    {
        if(strcmp(info_etud->etud.nom,crt->svt->etud.nom)<0)
         {
           info_etud->svt=crt->svt;
           crt->svt=info_etud;
           (*etat_inser)=1;///l'insertion est biens faite
           return((liste_base*)liste);
         }
        crt=crt->svt;
    }
    //cas d'insertion a la fin

    if(strcmp(info_etud->etud.nom,crt->etud.nom)>0)
        {
            crt->svt=info_etud;
           (*etat_inser)=1;///l'insertion est biens faite
        }
    return((liste_base*)liste);
        
}///fin fonction

  //****creation de la cellule qui va contenir deux infos le nom et le cne

etud_2info* creer_etud_2info(char nom[25],char CNE[11])
{
    etud_2info *info_etud;
    ///allocation memoire
    info_etud=(etud_2info*)malloc(sizeof(etud_2info));
    if(!info_etud)
    {
         
         printf("erreur d'allocation\n");
         exit(-1);
    }
   ///si l'allocation est bien faite on insere les donnés
   strcpy(info_etud->CNE,CNE);
   strcpy(info_etud->nom,nom);
   info_etud->svt=NULL;
   return((etud_2info*)info_etud);
}///fin fonction
/******
Nom du fonction : moyenne
Entrées         : (etud_2info* cellule) : une cellule
Sorties         : retourne la moyenne de l'étudiant associée à la cellule -1 s'il ,n'existe pas
Description     : cette fonction permet de retourner la moyenne d'un étudiant
******/
float moyenne(etud_2info* cellule)
{
    // un pointeur qui va parcourir la liste
    liste_base* crt;
    
    int cle1,cle2;
    //récupérer la clé/
    cherche_cles(cellule->nom,&cle1,&cle2);
    //acceder à la base des étudiant 
    crt=Tab_BaseEtud[cle1][cle2];
    while(crt)
    {
        //parcourir la liste on testant sur le cne, si on trouve en retourne la moyenne/
        if(strcmp(crt->etud.CNE,cellule->CNE)==0)
            return((float)crt->etud.moyenne);
        crt=crt->svt;
    }
    //dans le cas ou l'etudiant n'existe pas
    return((float)-1);
}
/****************************************************
Nom du fonction :insertion_par_moyenne
Entrées         :(etud_2info* liste) : tete de la liste
  (info_etud etu) : un étudiant
                 (int* etat_inser) pour enregistrer si l'insertion bien passée ou pas
Sorties         : retourne le nouvelle tete de la liste
Description     : cette fonction fait une insertion ordonne par moyenne
***********************************************************/
etud_2info* insertion_par_moyenne(etud_2info* liste,info_etud etu,int* etat_inser)
{
    //déclaration d'un nouveau élément/
    etud_2info* cel_etud,*crt;
    *etat_inser=0;// pour savoir est ce que l'insertion est bien faite
    //creer une cellule pour l'étudiant/
    cel_etud=creer_etud_2info(etu.nom,etu.CNE);
    /* si la liste n'existe pas*/
    if(!liste)return((etud_2info*)cel_etud);
    //si l'étudiant à insérer à la plus grande moyenne/
    if(etu.moyenne >= moyenne(liste))
    {
        //cas d'inserertion en tete
        cel_etud->svt=liste;
        liste=cel_etud;
        *etat_inser=1;
        return((etud_2info*)liste);
    }
    // cas d'insertion au milieu
   //parcourir la liste/
    crt=liste;
    while(crt->svt)
    {
       
        if(etu.moyenne > moyenne(crt->svt))
         {
             //insérer au milieu
           cel_etud->svt=crt->svt;
           crt->svt=cel_etud;
           *etat_inser=1;
           return((etud_2info*)liste);
         }
        crt=crt->svt;
    }
    //si l'étudiant à insérer à la plus petite moyenne
   
        //cas d'insérertion à la fin
        if(etu.moyenne < moyenne(crt))
      {
          crt->svt=cel_etud;
        *etat_inser=1;
     }
    return((etud_2info*)liste);

}

/**************************************
Nom du fonction : creer_liste_valid_echoue
Sorties         : cellule de type (liste_validation*)
Description     : cette fonction permet de creer une liste qui va contenir 
                  les etudiants qui ont valides et qui n'ont pas valides
******************************************/
liste_validation* creer_liste_valid_echoue()
{
    //délacaration d'un nouveau élément
    liste_validation* liste_valid_echoue;
    //allouer de l'éspace mémoire
    liste_valid_echoue=(liste_validation*)malloc(sizeof(liste_validation));
    if(!liste_valid_echoue)
    {
        printf("erreur d'allocation");
        exit(-1);
    }
    //initialiser des deux sous liste
    liste_valid_echoue->valide_liste=NULL;
    liste_valid_echoue->echoue_liste=NULL;
    return((liste_validation*)liste_valid_echoue);
}



/******
Nom du fonction : chargement_base
Entrées         : le nom du ficher
Description     : cette fonction va lire le contenu du fichier ,l'insére dans 
                  la base de donne par ordre selon le nom et aussi inserer les information
                  dans le tableau des stastiques  
******/
void chargement_base(char* nom_fichier)
{
       
       // pointeur sur le fichoer et l'ouverure de ce dernier en mode lecture
       FILE* f=fopen(nom_fichier,"r");
  //déclaration des indices/
  int j,test;
  int ind1,ind2;
  //déclaration d'un étudiant/
  info_etud etud;
  // pointeur sur la liste convenable dans la base ou on doit inserer
  liste_base* liste;
  //déclaration d'une variable pour la moyenne/
  float moyenne=0;
  //creer la liste des étudiants qui va contenir les deux sous liste 
  liste_valid_echoue=creer_liste_valid_echoue();
  char c;// nous aides dans la lecture de fichier

  if(f!=NULL)
 {
    
  do
     {
      moyenne=0;
      /* récupérer les informations d'un étudiant à partir du fichier*/
     fscanf(f,"%s %s %s %s",etud.nom,etud.prenom,etud.CNE,etud.date_naiss);
     for(j=0;j<12;j++)
       {
         // remplissage du tableau des  12 notes
         fscanf(f,"%f",&etud.Tab_note[j]);
         //si la note valide 
         if(etud.Tab_note[j]!=-1) moyenne+=etud.Tab_note[j];
         //remplir les informations par rapport à chaque module
         //remplissage du taleau de stastique
         if(etud.Tab_note[j]>=12)
            tableau_statis[j].valide++;
      
         else if(etud.Tab_note[j]<10)
                    tableau_statis[j].echoue++;
         else if(etud.Tab_note[j]>=10 && etud.Tab_note[j]<12)
                tableau_statis[j].compense++;
       }
   // calcule de la moyenne d'etudiant
   moyenne/=12;

   //affecter a la structure la moyenne calculer
   etud.moyenne=moyenne;
//chercher les cles
  cherche_cles(etud.nom,&ind1,&ind2);
  //accéder a la liste convenables pour l'insertion
  liste=Tab_BaseEtud[ind1][ind2];
  //insérer à la bonne place l'étudiant selon son nom
  liste=insertion_ord_nom(liste,etud,&test);
  Tab_BaseEtud[ind1][ind2]=liste;
  //si l'étudiant a validé l'année , on va l'insérer à la liste des étudiants qui ont validé 
  if(est_ce_que_valide(etud))
        liste_valid_echoue->valide_liste=insertion_par_moyenne(liste_valid_echoue->valide_liste,etud,&test);
  else//on va l'insérer dans la liste des étudiants qui ont échoué
     liste_valid_echoue->echoue_liste=insertion_par_moyenne(liste_valid_echoue->echoue_liste,etud,&test);
   c=fgetc(f);// pour passer a la deuxieme ligne
   
   //tant qu'on a pas encore arrivée à la fin du fichier
  }while(c!=EOF);
}
else
    printf("ouverture echoué ");
    //fermer le fichier/
fclose(f);
}
/******
Nom du fonction : affiche_liste
Entrées         : la tete de la liste
Description     : cette fonction permet l'affichage de la liste
******/
void affiche_liste(etud_2info* liste)
{
    //déclaration d'un courant/
    etud_2info* crt;
    crt=liste;
    //parcourir la liste/
    while(crt)
    {
        //afficher le nom - Cne de chaque étudiant/
        printf("\n\nNOM : %s\tCNE : %s\n",crt->nom,crt->CNE);
        crt=crt->svt;
    }
}
/***
Nom du fonction : afficher_info_etud
Entrées         : un étudiant
Description     : cette fonction permet d'afficher le nom,
                  le prénom , la date de naissance ,le CNE ,
                  les notes des 12 modules de l'année et
                  la moyenne d'un étudiant.
****/
void afficher_info_etud(info_etud etud)
{
    int i;
    printf("\nNOM : %s ",etud.nom);//le nom de l'étudiant
    printf("\tPRENOM : %s ",etud.prenom);//le prenom de l'étudiant
     printf("\tCNE : %s ",etud.CNE);//le Code National de l'etudiant
    printf("\tDate naissance : %s \n",etud.date_naiss);//la date de naissance de l'etudiant
   
    printf("\nNOTES :  ");
    for(i=0;i<12;i++)
    //on parcourut le tableau de note de taille 12
    //12 car ona 12 notes de 12 modules
    {
        //si l'étudiant n'a pas de note dans un module en la remplace par -1
        if(etud.Tab_note[i]!=-1)
        //affichage de la note de chaque module a part
        printf(" %.2f\t",etud.Tab_note[i]);
    }
    //la moyenne d'un étudiant dans une année d'étude
    printf("\n");
    printf("\nMOYENNE : %.2f \n",etud.moyenne);
}
/******************************
Nom du fonction : afficher_liste
Entrées         : une liste de type liste_base*
Description     : cette fonction permet d'afficher tous
                  les étudiants de la liste 
****/
void afficher_liste(liste_base* liste)
{
    liste_base* crt;
    //le pointeur courant qui pointe sur la tete de la liste
    crt=liste;
    while(crt)
    //on parcourut la liste
    {
        printf("\n********************************************\n");
        //affichage de l'info_etud de la cellule courante
        afficher_info_etud(crt->etud);
        crt=crt->svt;
    }
}
/***********************************
Nom du fonction : afficher_la_base
Entrées         : un tableau de 2 dimensions 
Description     : cette fonction permet d'afficher toute 
                  les informationsdes étudiants
*******************************************/


void afficher_la_base(liste_base* Tab[26][26])
{
    liste_base* crt;// qui va parcourir tous les listes de la base
    int i,j;
    for(i=0;i<26;i++)
    {
        for(j=0;j<26;j++)
        {
            
            crt=Tab[i][j];
            afficher_liste(crt);
        }
    }
}
/********************************
Nom du fonction : recherche_result_etud
Entrées         : le nom et le CNE de l'étudiant
Sorties         : retourne 1 si l'etudiant existe et 0 sinon
Description     : cette fonction permet d'afficher les
                 notes et la moyenne de l'etudiants rechercher
**********************************/
int recherche_result_etud(char nom[25],char CNE[11])
{
    liste_base* crt;
    int cle1,cle2;
    // trouvant les deux cle
    cherche_cles(nom,&cle1,&cle2);
    // pointer sur la liste trouve par les cles
    crt=Tab_BaseEtud[cle1][cle2];
    while(crt)
    {
       /*faire une comparaison entre l'etudiant donne et et 
       les etudiant qui sont dans la liste*/
        // s'il est trouver
        if(strcmp(CNE,crt->etud.CNE)==0)
        {
            for(int i=0;i<12;i++)
            {
                if(crt->etud.Tab_note[i]!=-1)
                    printf("\nNote %d = %2.3f\n",i+1,crt->etud.Tab_note[i]);
            }
            printf("\nMoyenne = %2.3f\n",crt->etud.moyenne);
            return((int)1);
        }
        crt=crt->svt;
    }
    // si l'etudiant donne n'existe pas
    return((int)0);
}

/********************************
Nom du fonction : affiche_etud_compensation
Entrées         : le tableau de deux dimensions de type
                  liste_base*
Description     : cette fonction permet d'afficher tous
                  les étudiants ayant des modules inférieur
                  à 12 ou à 10.
*************************************/
void affiche_etud_compensation(liste_base* Tab_BaseEtud[26][26])
{
 liste_base* crt;
 int i,j,k;
 for(i=0;i<26;i++)
 {
   for(j=0;j<26;j++)
    {
     crt=Tab_BaseEtud[i][j];
     while(crt)
      {
       for(k=0;k<12;k++)
        {
         if((crt->etud.Tab_note[k] < 10 ||crt->etud.Tab_note[k] < 12)
            && crt->etud.Tab_note[k]!=-1)    
          {
                     afficher_info_etud(crt->etud);
                     /* une fois on a trouve une note 
                     qui est inferieur on l'affiche et on sort de la boucle*/
                     break;
           }
         }
            crt=crt->svt;
       }
    }
 }
}
/********************************
Nom du fonction : afficher_statistique_modules

Description     : cette fonction permet d'afficher
                  les statistique de chaque modules
*************************************/
void afficher_statistique_modules()
{
    int i;
    for(i=0;i<12;i++)
    {
        printf("\nle module %d : nombre de validation---> %d nombre de compensation---> %d nombre de non validation---> %d\n",
        i+1,tableau_statis[i].valide,tableau_statis[i].compense,tableau_statis[i].echoue);
    }
}
/********************************
Nom du fonction : chercher_etudiant
Entrer           :une chanie de caractere "le nom"

Description     : cette fonction permet d'afficher
                  toutes les informations sur l'etudiant 
                  rechercher
*************************************/
void chercher_etudiant(char nom[25])
{ int cle1,cle2,i;
liste_base*crt;

    cherche_cles(nom,&cle1,&cle2);
    crt=Tab_BaseEtud[cle1][cle2];
    while(strcmp(crt->etud.nom,nom))
       crt=crt->svt;
        printf("\nprenom :%s  CNE :%s  date de naissannce : %s\n",crt->etud.prenom,crt->etud.CNE,crt->etud.date_naiss);
        printf("NOTES: ");
            for(int i=0;i<12;i++)
            {
                if(crt->etud.Tab_note[i]!=-1)
                    printf("\t%2.2f",crt->etud.Tab_note[i]);
            }
            printf("\nMoyenne = %2.3f\n",crt->etud.moyenne);
}
/******************************
Nom du fonction : classement


Description     : cette fonction permet d'afficher
                  les etudiants en ordre par moyen
     
************************************/
 void classement()
 {
     etud_2info*crt1;
     
     crt1=liste_valid_echoue->valide_liste;
    
     while(crt1)
     {
         
             
         printf("\t%s\t%2.3f\n",crt1->nom,moyenne(crt1));
         crt1=crt1->svt;
       }
        crt1=liste_valid_echoue->echoue_liste;

      while(crt1)
     {
        
         printf("\t%s\t%2.3f\n",crt1->nom,moyenne(crt1));
         crt1=crt1->svt;
     }
 
 }// fin de fonction



 

void affich_qlq(char*nomfichier)
{
    info_etud etud;
    int j;
    char c;// nous aides dans la lecture de fichier
    FILE* f=fopen(nomfichier,"r");
    if(f!=NULL)
 {
    
  do
     {
          fscanf(f,"%s %s %s %s",etud.nom,etud.prenom,etud.CNE,etud.date_naiss);
          printf("%s \t%s \t%s \t%s",etud.nom,etud.prenom,etud.CNE,etud.date_naiss);
     for(j=0;j<12;j++)
       {
         // remplissage du tableau des  12 notes
         fscanf(f,"%f",&etud.Tab_note[j]);
          printf("\t%2.2f",etud.Tab_note[j]);
       }
       printf("\n\n");
        c=fgetc(f);// pour passer a la deuxieme ligne
   
   //tant qu'on a pas encore arrivée à la fin du fichier
  }while(c!=EOF);
}
else
    printf("ouverture echoué ");
    //fermer le fichier/
fclose(f);
}
       
 

int main()
{
  
  int choix ; 
  char demande,nom[25];
  
  initialisation_base();
  ///lire le fichier
  chargement_base("ficher_info.txt");
affich_qlq("ficher_info.txt");

  
  
       
  do
  {
        printf("\nsi vous voulez: \n");
        printf("\n  **la base de donnes des etudiants--------> 1 \n");
        printf("\n  **les etudiant qui ont valides--------> 2 \n");
        printf("\n  **les etudiants  qui ont echoue --------> 3\n");
        printf("\n  **les etudiants qui peut compenser--------> 4 \n");
        printf("\n  **les statistique de chaque module ------> 5\n");
        printf("\n  ** chercher les informations d'un etudiant par son nom-------> 6\n");
        printf("\n  **affichage par ordre de merite-------->7");
        printf("\n  **sortir-------->8");
        do
        {
           printf("\n\n   donner votre choix:  ");
           scanf("%d",&choix);
        }while(choix<1 || choix>8);
     switch(choix)
     {
        case 1:afficher_la_base(Tab_BaseEtud); break;
        case 2 : affiche_liste(liste_valid_echoue->valide_liste);break;
        case 3 : affiche_liste(liste_valid_echoue->echoue_liste);break;
        case 4 : affiche_etud_compensation(Tab_BaseEtud);break;
        case 5 : afficher_statistique_modules();break;
        case 6 :  printf("\ndonner le nom d'etudiant rechercher : ");
                  scanf("%s",nom);    
                   chercher_etudiant(nom); break;
        case 7 : classement();break;
        case 8 : return((int)0);


     }
    printf("\n\nVoulez-vous une autres information  Y= yes et N=non ");
    getchar();
    scanf("%c",&demande);
    }while(demande=='Y'||demande=='y');
  
return((int)0);
}