#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define Taille_De_Quai 5
#define Taille_DUne_Chaine 30
#define MaxTrain 20
#define MaxParkingRails 4
#define MapSize 8


typedef char chaine[Taille_DUne_Chaine];

   typedef struct passager_t *passagers_t;
   typedef struct passager_t{
    int id;
    chaine destination;
    passagers_t suivant;
    }passager_t;



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


typedef struct quai{
    int id;
    listeTrain_t listeTrain;
    int premierTrain;
    int capacite;
}quai_t;

passagers_t CreerPassager(int id, chaine dest){
    passagers_t p=(passagers_t)malloc(sizeof(passagers_t));
    if(p==NULL){
        printf("Error d'allocation, on ne peut pas creer un passager maintenemt.\n");
        return p;
    }
    p->id=id;
    strcpy(p->destination, dest);
    p->suivant=NULL;
    return p;
}

int SiListeVide(passagers_t ListePassager){
   if(ListePassager==NULL)
   {
       return 1 ;
   }
   return 0;
}

passagers_t AvoirDernierElementList(passagers_t ListePassager)
{
    while(ListePassager->suivant!=NULL)
        ListePassager=ListePassager->suivant;

    return ListePassager;
}

passagers_t ReservationPlace(passagers_t ListePassagers, train_t * Trains, int NumTrain, chaine Destination)
{
	int SiDestinationTrouvee=0,i;
	for (i=0;i<NumTrain;i++)
	{
    	if ((SiDestinationTrouvee==0)&&(strcmp(Trains[i].destination,Destination)==0))
        {
			SiDestinationTrouvee=1;
        }
	}
	if(SiDestinationTrouvee)
	{
    	if(ListePassagers==NULL)
		{
        	ListePassagers=CreerPassager(0,Destination);
        	return ListePassagers;
    	}
		passagers_t DernierElmnt=AvoirDernierElementList(ListePassagers);
    	DernierElmnt->suivant=CreerPassager(DernierElmnt->id+1,Destination);
	}
	else
    printf("La destination %s n'est pas disponible dans notre gare\n",Destination);
return ListePassagers;
}

int ExistIdDansListe(int IDRecherche,passagers_t ListePassager)
{
    passagers_t p=ListePassager;

    while(p!=NULL)
    {
        if(p->id==IDRecherche)
            return 1;
        p=p->suivant;
    }
    return 0;
}


passagers_t AnnulerReservation(int IDRecherche,passagers_t ListePassager)
{
    passagers_t parent,courant;

    if (!ExistIdDansListe(IDRecherche,ListePassager))
    {
        printf("le passager n'existe pas !!\n");
        return ListePassager;
    }


    if(ListePassager->id==IDRecherche)
        {
            ListePassager=ListePassager->suivant;
            return ListePassager;
        }
    else {
              courant=ListePassager->suivant;
              parent=ListePassager;

        while (!SiListeVide(courant)&&(courant->id!=IDRecherche))
               {
                   parent=courant;
                   courant=courant->suivant ;
               }
            if(courant!=NULL){
                parent->suivant=courant->suivant ;
                free(courant);
            }
    }

      return ListePassager ;
}

int TailleListe(passagers_t ListePassager)
{ int Nb=0 ;
   passagers_t courant=ListePassager;
   while(courant!=NULL)
   {
        Nb=Nb+1 ;
        courant=courant->suivant ;
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
                for(i=0;i<(taille-1);i++)
                {
                  courant=courant->suivant;
                }
                courant->suivant=&PassagerAAjouter;

        }

        return ListeDesPassagers;
    }

passager_t SupprimerPassagerListeAttenteEtLeRetourner(passagers_t *ListePassagers,int iDPassager){

    passager_t PassagerARetourner;

    if ((*ListePassagers)->id==iDPassager){
        PassagerARetourner=**(ListePassagers);
        *ListePassagers=(*ListePassagers)->suivant;
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

    PassagerParent->suivant=PassagerCourant->suivant;
    PassagerARetourner=*PassagerCourant;
    free(PassagerCourant);

return PassagerARetourner;

}


void AfficherPassagers(passagers_t ListePassagers){

passagers_t ParcoursPassager=ListePassagers;

if (TailleListe(ParcoursPassager)==0){
    printf("\n**Aucune reservation faite !**\n");
    return;

}
printf("\nListe des réservations \n");
while (ParcoursPassager!=NULL){
    printf("Le passager avec l'ID %d a reserve la destination %s ! \n",ParcoursPassager->id,ParcoursPassager->destination);
    ParcoursPassager=ParcoursPassager->suivant;
}

printf("\n");
}



  quai_t CreeUnParkingRail(int id ,int capacite)
{
    quai_t PK;

    PK.id=id;
    PK.capacite=capacite;
    PK.premierTrain=-1;

    return PK;
}

    int PileRemplie(quai_t PK)
{
  if (PK.premierTrain==PK.capacite)
    return 1 ;
    else
        return 0;
}

  int Empiler (train_t TrainCourant,quai_t *q){
    if (PileRemplie(*q) == 1){
        return 0;
    }
    else{
        q->premierTrain=q->premierTrain+1;
        q->listeTrain[q->premierTrain]=TrainCourant;
        return 1;
    }
}

    int PileVide(quai_t *q)
    {
    return (q->premierTrain == - 1);
    }


  train_t Depiler(quai_t *q)
  {
      train_t t;

    if(PileVide(q)==0)
    {
    t = q->listeTrain[q->premierTrain];
    q->premierTrain--;
    }else
        t.id=-1;


    return t;
  }

   quai_t TrouverDesTrainDisponible(quai_t ParkingForTrain,train_t * Trains, int numTrain){

     for (int i=0;i<numTrain;i++)
        {
       if((Trains[i].kmRestant==-1)&&(!PileRemplie(ParkingForTrain)))
       {
        Trains[i].kmRestant=0;
        Empiler(Trains[i],&ParkingForTrain);

     }
  }
         return ParkingForTrain;
}

    int AjouterQuais(quai_t *Quais,int NumQuai,int IdentifiantQuai, int capacity,train_t *trains,int NumTrain){

  Quais[NumQuai]=CreeUnParkingRail(IdentifiantQuai,capacity);
  Quais[NumQuai]=TrouverDesTrainDisponible(Quais[NumQuai],trains,NumTrain);

    NumQuai=NumQuai+1;

    return NumQuai;
}

void UpdateQuais(quai_t * Quais, int NumQuai, train_t * Trains, int NumTrain ){

for (int j=0;j<NumTrain;j++){
    for(int i=0;i<NumQuai;i++){
        if((Trains[j].kmRestant==-1)){
            if(!PileRemplie(Quais[i])){
                Trains[j].kmRestant=0;
                Empiler(Trains[j],&Quais[i]);
            }
        }
    }
}

}

     void AfficherParkingRail(quai_t *ParkingRails, int NumParkingRail)
    {
    printf("Le nombre de Quais dans la gare est de %d\n \n",NumParkingRail);

for (int i=0;i<NumParkingRail;i++){

    printf("Le nombre de train dans le quai %d est de %d \n",ParkingRails[i].id,ParkingRails[i].premierTrain+1);

        for (int j=ParkingRails[i].premierTrain;j>=0;j--){
            printf("le train ID=%d prendra le départ en %d position\n",ParkingRails[i].listeTrain[j].id,ParkingRails[i].premierTrain-j+1);
            }
        }
    }






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

    train_t ViderTrain(train_t T)
    {
    passagers_t courant;

      if (T.kmRestant==0)
      {
        courant=T.listeDesPassagers;
        while(SiListeVide(T.listeDesPassagers))
        {
            T.listeDesPassagers=T.listeDesPassagers->suivant;
            free(courant);
            courant=T.listeDesPassagers;
        }

      }
      return T ;
    }

    void AfficherTrains(train_t * Trains, int nbrTrains){

   printf("Il y'a %d trains dans la gare \n\n", nbrTrains);
for (int i=0;i<nbrTrains;i++){

    float Pourcentage=((float)Trains[i].tauxDeRemplissage/(float)Trains[i].capaciteMAX)*100;
    if (Trains[i].kmRestant==-1)
        printf("Train ID= %d est disponible pour %s \n",Trains[i].id, Trains[i].destination);
    else if (Trains[i].kmRestant==0)
        printf("Train ID= %d dans le quai, remplis a %.2f %%, en attente de départ vers %s \n",Trains[i].id,Pourcentage, Trains[i].destination);
    else
        printf("Train ID= %d en Trajet passagers: %d, %.2f %%, ===== reste:%d Km ====> %s\n\n",Trains[i].id,Trains[i].tauxDeRemplissage,Pourcentage,Trains[i].kmRestant,Trains[i].destination);

    }

}

    int AjouterTrain(train_t * Trains,int Id, int Capacitee, chaine Destination)
    {
        Trains[Id]=CreerTrain(Id,Capacitee,Destination);
        Id=Id+1;

        return Id;
    }


    int SiTrainRemplis(train_t TrainCourant)
    {
        if(TrainCourant.tauxDeRemplissage==TrainCourant.capaciteMAX) return 1 ;
        return 0;
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
        if(TrainPourDepart.id==-1)
            printf("Le quai %d est vide \n",Quaies[i].id);
        else{
        printf("Prochain train,ID=%d, a destination de %s dans le quai %d\n",TrainPourDepart.id,TrainPourDepart.destination,Quaies[i].id);

            for(int i=0;i<(NumDestination+1);i++){
                if(strcmp(TrainPourDepart.destination,Destinations[i][0])==0){
                    TrainPourDepart.kmRestant=atoi(Destinations[i][1]);

                }
            }


            ParcourirPassager=PassagerPretPourEmbarquement;
            while(ParcourirPassager!=NULL){
                if((strcmp(ParcourirPassager->destination,TrainPourDepart.destination)==0)&&(!SiTrainRemplis(TrainPourDepart)))
                {
                    PassagerTmp=SupprimerPassagerListeAttenteEtLeRetourner(&PassagerPretPourEmbarquement,ParcourirPassager->id);

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


    int InitialiserLesDestinationsDisponibles(chaine Destinations[MapSize][2])
    {
        strcpy(Destinations[0][0],"Tokyo");
        strcpy(Destinations[0][1],"3000");

        strcpy(Destinations[1][0],"Helsinki");
        strcpy(Destinations[1][1],"2500");

        strcpy(Destinations[2][0],"Berlin");
        strcpy(Destinations[2][1],"1500");

        strcpy(Destinations[3][0],"Nairobi");
        strcpy(Destinations[3][1],"1800");

        strcpy(Destinations[4][0],"Denver");
        strcpy(Destinations[4][1],"2000");

        strcpy(Destinations[5][0],"Ouergla");
        strcpy(Destinations[5][1],"50");

        strcpy(Destinations[6][0],"Rio");
        strcpy(Destinations[6][1],"900");

        strcpy(Destinations[7][0],"moscou");
        strcpy(Destinations[7][1],"2500");

        return 7;
    }


int main()
{
    train_t Trains[MaxTrain];
    quai_t Quais[MaxParkingRails];
    int NumTrain=0;
    int NumQuai=0;
    int NbrHeur=0;

    passagers_t ListeDesPassagerEnAttente=NULL;

    chaine Destinations[MapSize][2];

    int NumDestinations= InitialiserLesDestinationsDisponibles(Destinations);



        NumTrain=AjouterTrain(Trains,NumTrain,15,"Tokyo");
        NumTrain=AjouterTrain(Trains,NumTrain,10,"Helsinki");
        NumTrain=AjouterTrain(Trains,NumTrain,10,"Helsinki");
        NumTrain=AjouterTrain(Trains,NumTrain,10,"Tokyo");
        NumTrain=AjouterTrain(Trains,NumTrain,10,"Berlin");
        NumTrain=AjouterTrain(Trains,NumTrain,8,"Nairobi");
        NumTrain=AjouterTrain(Trains,NumTrain,10,"Denver");
        NumTrain=AjouterTrain(Trains,NumTrain,10,"Nairobi");
        NumTrain=AjouterTrain(Trains,NumTrain,10,"Berlin");
        NumTrain=AjouterTrain(Trains,NumTrain,10,"Ouergla");


        NumQuai=AjouterQuais(Quais,NumQuai,1,2,Trains, NumTrain);
        NumQuai=AjouterQuais(Quais,NumQuai,2,1,Trains, NumTrain);
        NumQuai=AjouterQuais(Quais,NumQuai,3,1,Trains,NumTrain);



    while(1)
        {
    AfficherParkingRail(Quais,NumQuai);
    NbrHeur++;

    printf("************Heure %d***********\n\n",NbrHeur);

    srand(time(NULL));

    int NombreAleatoireDeReservations=rand()%10+5;
    int NombreAleatoireDAnulations=rand()%5;

    printf("Nombre de reservations :%d\n",NombreAleatoireDeReservations);
    printf("Nombre d'annulations : %d\n",NombreAleatoireDAnulations);

    for (int i=0;i<NombreAleatoireDeReservations;i++)
        {
            int ChoisirUneDestinationAleatoire=rand()%(NumDestinations+1);
            ListeDesPassagerEnAttente=ReservationPlace(ListeDesPassagerEnAttente,Trains,NumTrain,Destinations[ChoisirUneDestinationAleatoire][0]);
        }

    for (int i=0;i<NombreAleatoireDAnulations;i++)
        {
            int ChoisirUnIDPourAnulationAleatoire=rand()%TailleListe(ListeDesPassagerEnAttente);
            ListeDesPassagerEnAttente=AnnulerReservation(ChoisirUnIDPourAnulationAleatoire,ListeDesPassagerEnAttente);
        }


    printf("\n Nombre de passagers en attente:\t%d\n",TailleListe(ListeDesPassagerEnAttente));

    AfficherPassagers(ListeDesPassagerEnAttente);

    ListeDesPassagerEnAttente=Embarquement(Quais, NumQuai,ListeDesPassagerEnAttente, Destinations,NumDestinations, Trains,NumTrain );
    AfficherTrains(Trains,NumTrain);



    break;
    }

}
