#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define Taille_De_Quai 5
#define Taille_DUne_Chaine 30
#define MapSize 8

//défenire les tableau
typedef char chaine[Taille_DUne_Chaine];

   typedef struct passager_t *passagers_t;
   typedef struct passager_t{
    int id;
    chaine destination;
    passagers_t suivant;
    }passager_t;


//défenire un train
typedef struct train{
    int id;
    int capaciteMAX;
    int tauxDeRemplissage;
    passagers_t listeDesPassagers;
    chaine destination;
    int kmRestant;
    int vitesse;
}train_t;

typedef train_t listeTrain_t[Taille_De_Quai];

//défenire un quai
typedef struct quai{
    int id;
    listeTrain_t listeTrain;
    int premierTrain; //sommet de la pile
    int capacite;
}quai_t;


    train_t CreerTrain(int IDTrain, int PlacesSupportees, chaine Destination)
    {
        train_t P ;
        P.id=IDTrain;
        P.capaciteMAX=PlacesSupportees ;
        strcpy(P.destination,Destination);
        P.kmRestant=-1;
        P.tauxDeRemplissage=0;
        P.vitesse=0;
        P.listeDesPassagers=NULL;
        return P ;
    }


    int AjouterTrain(train_t * Trains, int NumTrain, int Id, int Capacitee, chaine Destination)
    {
        Trains[NumTrain]=CreerTrain(Id,Capacitee,Destination);
        NumTrain=NumTrain+1;

        return NumTrain;
    }


    int PileVide(quai_t *q)
    {
    return (q->premierTrain == - 1);
    }


  train_t Depiler(quai_t *q)
  {
      train_t t;

    if(!PileVide(q))
    {
    t = q->listeTrain[q->premierTrain];
    q->premierTrain--;
    }else
        t.id=-1;


    return t;
  }

      int SiTrainRemplis(train_t TrainCourant)
    {
        if(TrainCourant.tauxDeRemplissage==TrainCourant.capaciteMAX) return 1 ;
        return 0;
    }


    int TailleListe(passagers_t ListePassager)
{ int Nb=0 ;
   while(ListePassager!=NULL)
   {
        Nb=Nb+1 ;
        ListePassager=ListePassager->suivant ;
   }
  return Nb;
}

passagers_t AjouterPassagerDansLaListe(passager_t PassagerAAjouter,passagers_t ListeDesPassagers)
{
        int taille,i;
        taille=TailleListe(ListeDesPassagers);
        passagers_t courant=ListeDesPassagers;

        if(taille==0)
            ListeDesPassagers=&PassagerAAjouter;
        else{
                for(i=0;i<taille-1;i++)
                {
                  courant=courant->suivant;
                }
                courant->suivant=&PassagerAAjouter;

        }

        return ListeDesPassagers;
    }


    int SiListeVide(passagers_t ListePassager){
   if(ListePassager==NULL)
   {
       return 1 ;   //la list est vide
   }
   return 0;
}

    passager_t SupprimerPassagerListeAttenteEtLeRetourner(passagers_t *ListePassagers,int iDPassager){

    passager_t PassagerARetourner;

    if ((*ListePassagers)->id==iDPassager){
        PassagerARetourner=**(ListePassagers);
        *ListePassagers=(*ListePassagers)->suivant;
        //printf("cas 0");
        return PassagerARetourner;

    }

    passagers_t PassagerParent=*ListePassagers;
    passagers_t PassagerCourant=(*ListePassagers)->suivant;

    while ((!SiListeVide(PassagerCourant))&&(PassagerCourant->id!=iDPassager)){
        PassagerParent=PassagerCourant;
        PassagerCourant=PassagerParent->suivant;
    }

    if(SiListeVide(PassagerCourant)){
        printf("aucune suppression Faite !!! faite ! ");
        return;
    }

    //printf("suppression de passager %d",PassagerCourant->ID);
    PassagerParent->suivant=PassagerCourant->suivant;
    PassagerARetourner=*PassagerCourant;
    free(PassagerCourant);

//printf("je suis dans la suppression taille liste est : %d", TailleListe(*ListePassagers));
return PassagerARetourner;

}


    void MiseAJourTableauDesTrain(train_t TrainPourDepart,train_t * Trains,int NumTrain)
    {
     for (int i=0;i<NumTrain;i++)
        {
    if (TrainPourDepart.id==Trains[i].id)
            {
            Trains[i]=TrainPourDepart;
            return;
            }
        }
    }

passagers_t Embarquement(quai_t * Quaies, int NumQuaie, passagers_t PassagerPretPourEmbarquement, chaine Destinations[MapSize][2], int NumDestination, train_t * Trains, int NumTrain)
{
    train_t TrainPourDepart;
    passager_t PassagerTmp;
    passagers_t ParcourirPassager;

    printf("Liste Des Prochains Departs\n");
    for (int i=0;i<NumQuaie;i++)
    {
        TrainPourDepart=Depiler(&Quaies[i]);
        if(TrainPourDepart.id==-1) //
            printf("Le quai %d est vide \n",Quaies[i].id);
        else{
        printf("Prochain train,ID=%d, a destination de %s dans le quai %d\n",TrainPourDepart.id,TrainPourDepart.destination,Quaies[i].id);

            //RécupérerLaDistance
            for(int i=0;i<(NumDestination+1);i++){
                if(strcmp(TrainPourDepart.destination,Destinations[i][0])==0){
                    TrainPourDepart.kmRestant=atoi(Destinations[i][1]);

                }
            }


            ParcourirPassager=PassagerPretPourEmbarquement;
            while(ParcourirPassager!=NULL){
                if((strcmp(ParcourirPassager->destination,TrainPourDepart.destination)==0)&&(!SiTrainRemplis(TrainPourDepart))){

                    //Enlever de la liste des passager en attente et le mettre dans la liste des passagers dans le train
                    PassagerTmp=SupprimerPassagerListeAttenteEtLeRetourner(&PassagerPretPourEmbarquement,ParcourirPassager->id);

                    //Le mettre dans le train

                    TrainPourDepart.listeDesPassagers=AjouterPassagerDansLaListe(PassagerTmp,TrainPourDepart.listeDesPassagers);
                    TrainPourDepart.tauxDeRemplissage=TrainPourDepart.tauxDeRemplissage+1;
                }
                ParcourirPassager=ParcourirPassager->suivant;
            }

            MiseAJourTableauDesTrain(TrainPourDepart,Trains,NumTrain);
        }

    }
    printf("\n");
return PassagerPretPourEmbarquement;
}

int main()
{
    printf("Hello world!\n");
    return 0;
}
