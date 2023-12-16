/////////////////////////////////////////////
/// Projet de Niveau 2 :
/// char nom1[] = "TAHAR";
/// char prenom2] = "Elias";
/// char nom2[] = "GHIAT";
/// char prenom2[] = "Mohammed";
/// char nom3[] = "ABDEL-MADJID";
/// char prenom3[] = "Hamid-Ousman";
/// char groupe[] = "A12";
/////////////////////////////////////////////



#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<time.h> 

/**************************************************************************************************
											macro #define
***************************************************************************************************/

#define dim 100
#define tailleBinaireY0 12
#define tailleBinaireB 17
#define tailleBinaireDL 19
#define tailleBinaireLo 26
#define precision 10000
#define nombreMaximumIteration 80 
#define chanceMutation 0.01
/**************************************************************************************************
							déclaration des nouveaux types (typedef) 
***************************************************************************************************/
struct Individu
{
	int chromosome1[tailleBinaireY0] ; int chromosome2[tailleBinaireB] ; 
	int chromosome3[tailleBinaireDL] ; int chromosome4[tailleBinaireLo] ; 
	double fitnessScore;
}; //une structure  nommée individu. La structure contient plusieurs champs, dont chacun est un tableau d'entiers

typedef struct Individu individu; // on renomme la structure.

typedef double ValeursExperimentales[1024];//crée un nouveau type de données appelé ValeursExperimentales qui est un tableau de nombres de type double avec une taille spécifiée par la constante 1024

typedef double vect[dim] ;//crée un nouveau type de données appelé vect qui est un tableau de nombres de type double avec une taille spécifiée par la constante dim

/**************************************************************************************************
											zone de prototype 
***************************************************************************************************/

double genererValeurAleatoire (double a , double b); 

void extraireDonneesFichier(char *filepath, ValeursExperimentales x, ValeursExperimentales y, ValeursExperimentales z, int *n);

void ecrireResultatsFichier(char nom_du_fichier[], double X_f, double Y_f); 

double calculerErreurSolution(individu x , ValeursExperimentales X , ValeursExperimentales Y);

double fonctionLorentzienne(double Y0 , double B , double delta_lambda , double lambda_zero , double lambda);

individu croisementGenetique (individu parent1 , individu parent2);

individu selectionnerMeilleursIndividus (individu population[dim]);

individu mutationGenetiqueAleatoire(individu x);

int identifierIndividuOptimal (individu population[dim]);

void convertionEntierBinaire(int x , int bit , int a[bit]);

double convertionBinaireEntier(individu population[dim] , int nb_bit , int indice_min);

individu clone(individu x);

double densitePlasma(double x);

void calculerNouveauxPointsDonnees(char nom_du_fichier[], double Y0, double B, double delta_lambda, double lambda_zero, ValeursExperimentales x);

int main(void)
{
    // Déclaration des variables
    int i, iteration = 0, N, indiceBit, indiceGenetique, indiceMeilleur;
    vect ValeursY0, ValeursB, ValeursDeltaLambda, ValeursLambdaZero;
    ValeursExperimentales DonneesX, DonneesY, DonneesZ;
    double Y0_Decode, B_Decode, DL_Decode, Lo_Decode, puissanceDeux, densiteElectronique;
    time_t t;
    individu A;
    individu population[dim]; // Tableau contenant la population initiale d'individus
    individu enfants[dim]; // Tableau pour stocker les individus de la nouvelle génération

    // Extraction des données expérimentales à partir d'un fichier
    extraireDonneesFichier("Data_Genetique.txt", DonneesX, DonneesY, DonneesZ, &N);

    // Initialisation du générateur de nombres aléatoires
    srand(time(&t));

    // Boucle pour initialiser la population avec des paramètres aléatoires
    for(i = 0; i < 100; i++)
    {
        // Génération aléatoire des paramètres pour chaque individu
        ValeursY0[i] = genererValeurAleatoire(2000, 2300);
        ValeursB[i] = genererValeurAleatoire(100000, 150000);
        ValeursDeltaLambda[i] = genererValeurAleatoire(100000, 200000);
        ValeursLambdaZero[i] = genererValeurAleatoire(65600000, 65650000);

        // Conversion des paramètres en binaire pour chaque individu
        convertionEntierBinaire(ValeursY0[i], tailleBinaireY0, population[i].chromosome1);
        convertionEntierBinaire(ValeursB[i], tailleBinaireB, population[i].chromosome2);
        convertionEntierBinaire(ValeursDeltaLambda[i], tailleBinaireDL, population[i].chromosome3);
        convertionEntierBinaire(ValeursLambdaZero[i], tailleBinaireLo, population[i].chromosome4);

        // Calcul de l'erreur (fitness score) pour chaque individu
        population[i].fitnessScore = calculerErreurSolution(population[i], DonneesX, DonneesZ);
    }

    // Boucle principale pour générer de nouvelles générations jusqu'à atteindre le chanceMutation de fitness ou la nombreMaximumIteration d'itérations
    do
    {
        // Génération des enfants pour la nouvelle génération
        for (i = 0; i < dim; i++)
        {
            enfants[i] = selectionnerMeilleursIndividus(population);
        }

        // Trouver l'individu avec le meilleur fitness dans la génération actuelle
        indiceMeilleur = identifierIndividuOptimal(enfants);

        // Mise à jour de la population avec les nouveaux individus
        for (i = 0; i < dim; i++)
        {
            for(indiceBit = 0; indiceBit < tailleBinaireY0; indiceBit++)
                population[i].chromosome1[indiceBit] = enfants[i].chromosome1[indiceBit];

            for(indiceBit = 0; indiceBit < tailleBinaireB; indiceBit++)
                population[i].chromosome2[indiceBit] = enfants[i].chromosome2[indiceBit];

            for(indiceBit = 0; indiceBit < tailleBinaireDL; indiceBit++)
                population[i].chromosome3[indiceBit] = enfants[i].chromosome3[indiceBit];

            for(indiceBit = 0; indiceBit < tailleBinaireLo; indiceBit++)
                population[i].chromosome4[indiceBit] = enfants[i].chromosome4[indiceBit];
        }
        
        printf("Iteration num = %d\n", iteration);
        iteration++;
    } while(enfants[indiceMeilleur].fitnessScore >= chanceMutation && iteration != nombreMaximumIteration);

    // Décodage des paramètres de l'individu avec le meilleur fitness
    Y0_Decode = convertionBinaireEntier(enfants, tailleBinaireY0, indiceMeilleur);
    B_Decode = convertionBinaireEntier(enfants, tailleBinaireB, indiceMeilleur);
    DL_Decode = convertionBinaireEntier(enfants, tailleBinaireDL, indiceMeilleur);
    Lo_Decode = convertionBinaireEntier(enfants, tailleBinaireLo, indiceMeilleur);

    // Calcul et sauvegarde des nouveaux points basés sur les paramètres décodés
    calculerNouveauxPointsDonnees("calculated_points.txt", Y0_Decode, B_Decode, DL_Decode, Lo_Decode, DonneesX);

    // Affichage des valeurs des paramètres décodés
    printf("La valeur de Y0 vaut : %lf\n", Y0_Decode);
    printf("La valeur de B vaut : %lf\n", B_Decode);
    printf("La valeur de Delta Lambda vaut : %lf\n", DL_Decode);
    printf("La valeur de Lambda Zero vaut : %lf\n", Lo_Decode);

    // Calcul et affichage de la densité électronique
    densiteElectronique = densitePlasma(DL_Decode);
    printf("la densite electronique vaut : %lf m^-3\n", densiteElectronique);
    return 0;
}

/**************************************************************************************************
										Définition des fonctions 
***************************************************************************************************/

void extraireDonneesFichier(char *filepath, ValeursExperimentales x, ValeursExperimentales y, ValeursExperimentales z, int *n) {
    *n = 0;
    FILE *nom = fopen(filepath, "r");

    if (nom == NULL) {
        fprintf(stderr, "\nERREUR d'ouverture du fichier en extraireDonneesFichier\nEXIT\n\n");
        exit(1);
    }

    while (fscanf(nom, "%lf\t%lf\t%lf", &x[*n], &y[*n], &z[*n]) == 3) //Utilisation de == 3 pour s'assurer que les trois éléments ont été lus correctement à chaque itération.
	{
        (*n)++;
    }
    fclose(nom);
    (*n)--;
}

                                                                        // deja optimiser
double genererValeurAleatoire (double a , double b)
{
	double x ;
	x = rand()%(int)(((b-a)+1))+a ;  //Renvoie une valeur qui appartient à l'intervalle [a,b]. 
	return x ;
}
                                                                       // deja optimiser

// Fonction retournant la valeur de J(lambda)
double fonctionLorentzienne(double Y0, double B, double delta_lambda, double lambda_zero, double lambda)
{
    double denominator = pow((lambda - lambda_zero), 2.0) + pow((delta_lambda / 2.0), 2.0);
    double J = Y0 + B / denominator;
    return J;
}

double calculerErreurSolution(individu x, ValeursExperimentales X, ValeursExperimentales Y) {
    // Déclaration des variables pour les paramètres décodés et l'erreur cumulée
    double Y0 = 0, B = 0, delta_lambda = 0, lambda_zero = 0, puissance_deux, S = 0;

    // Décodage de Y0 à partir de la représentation binaire
    for (int j = 0; j < tailleBinaireY0; j++) {
        puissance_deux = pow(2, j); // Calcul de la puissance de 2 pour la position de bit actuelle
        Y0 += puissance_deux * x.chromosome1[j]; // Ajout de la valeur binaire pondérée au total
    }

    // Décodage de B de la même manière
    for (int j = 0; j < tailleBinaireB; j++) {
        puissance_deux = pow(2, j);
        B += puissance_deux * x.chromosome2[j];
    }

    // Décodage de delta_lambda
    for (int j = 0; j < tailleBinaireDL; j++) {
        puissance_deux = pow(2, j);
        delta_lambda += puissance_deux * x.chromosome3[j];
    }

    // Décodage de lambda_zero
    for (int j = 0; j < tailleBinaireLo; j++) {
        puissance_deux = pow(2, j);
        lambda_zero += puissance_deux * x.chromosome4[j];
    }

    // Normalisation des paramètres décodés
    Y0 /= precision;
    B /= precision;
    delta_lambda /= precision;
    lambda_zero /= precision;

    // Calcul de l'erreur quadratique totale par rapport aux données expérimentales
    for (int n = 0; n < 1024; n++) {
        // Calcul de l'erreur pour chaque point de données
        double erreur = Y[n] - fonctionLorentzienne(Y0, B, delta_lambda, lambda_zero, X[n]);
        S += erreur * erreur; // Addition de l'erreur quadratique à l'erreur totale
    }

    // Retour de l'erreur totale pour cet individu
    return S;
}


void ecrireResultatsFichier(char nom_du_fichier[], double X_f, double Y_f)
{
    // Ouvre le fichier en mode écriture
    FILE* fichier = fopen(nom_du_fichier, "w+");
    
    // Vérifie si l'ouverture du fichier a réussi
    if (fichier == NULL) {
        printf("\nERREUR d'ouverture de fichier en écriture\nEXIT\n\n");
        exit(1);
    } 
    
    // Écrit les valeurs dans le fichier
    fprintf(fichier, "%lf\t%lf\n", X_f, Y_f);
    
    // Ferme le fichier
    fclose(fichier);
}

                                                                                       // deja optimiser

/* 
On choisit aleatoirement 4 individus et on compare leurs couts deux à deux et on croise les deux qui possedent le fitnessScore le plus faible. 
*/
individu selectionnerMeilleursIndividus(individu population[dim]) {
    int i1 = rand() % dim;
    int i2 = rand() % dim;
    int i3 = rand() % dim;
    int i4 = rand() % dim;

    int meilleur1 = (population[i1].fitnessScore < population[i2].fitnessScore) ? i1 : i2;
    int meilleur2 = (population[i3].fitnessScore < population[i4].fitnessScore) ? i3 : i4;

    return croisementGenetique(population[meilleur1], population[meilleur2]);
}

/*
on fixe le chanceMutation de mutationGenetiqueAleatoire à 1%.                                                      //deja optimiser 
*/
individu mutationGenetiqueAleatoire(individu x) {
    // Vérification si une mutation doit se produire (1% de chance)
    if (rand() % 100 == 1) {
        int n = rand() % 4; // Choix aléatoire du gène (chromosome) à muter
        int m; // Indice du bit à muter dans le chromosome sélectionné

        // Détermination du chromosome à muter et mutation d'un bit aléatoire
        switch (n) {
            case 0:
                m = rand() % tailleBinaireY0; // Choix aléatoire du bit à muter dans chromosome1
                x.chromosome1[m] = 1 - x.chromosome1[m]; // Inversion du bit sélectionné
                break;
            case 1:
                m = rand() % tailleBinaireB; // Choix aléatoire du bit à muter dans chromosome2
                x.chromosome2[m] = 1 - x.chromosome2[m]; // Inversion du bit
                break;
            case 2:
                m = rand() % tailleBinaireDL; // Choix aléatoire du bit à muter dans chromosome3
                x.chromosome3[m] = 1 - x.chromosome3[m]; // Inversion du bit
                break;
            case 3:
                m = rand() % tailleBinaireLo; // Choix aléatoire du bit à muter dans chromosome4
                x.chromosome4[m] = 1 - x.chromosome4[m]; // Inversion du bit
                break;
        }
    }

    return x;
}


individu croisementGenetique(individu parent1 , individu parent2)
{
    // Initialisation des variables pour la sélection aléatoire des points de croisement
    int debutCroisementY0, debutCroisementB, debutCroisementDL, debutCroisementLo, finCroisementY0, finCroisementB, finCroisementDL, finCroisementLo, i, j, N;
    individu clone_parent1, clone_parent2;
    ValeursExperimentales X, Y, Z;

    // Chargement des données expérimentales pour l'évaluation des individus
    extraireDonneesFichier("Data_Genetique.txt", X, Y, Z, &N);

    // Clonage des chromosomes des deux parents pour éviter la modification directe
    // Clone de Y0/chromosome1
    for(i = 0; i < tailleBinaireY0; i++)
    {
        clone_parent2.chromosome1[i] = parent2.chromosome1[i];
        clone_parent1.chromosome1[i] = parent1.chromosome1[i];
    }
    
    // Clone de B/chromosome2
    for(i = 0; i < tailleBinaireB; i++)
    {
        clone_parent2.chromosome2[i] = parent2.chromosome2[i];
        clone_parent1.chromosome2[i] = parent1.chromosome2[i];
    }
    
    // Clone de delta_lambda/chromosome3
    for(i = 0; i < tailleBinaireDL; i++)
    {
        clone_parent2.chromosome3[i] = parent2.chromosome3[i];
        clone_parent1.chromosome3[i] = parent1.chromosome3[i];
    }
    
    // Clone de lambda_zero/chromosome4
    for(i = 0; i < tailleBinaireLo; i++)
    {
        clone_parent2.chromosome4[i] = parent2.chromosome4[i];
        clone_parent1.chromosome4[i] = parent1.chromosome4[i];
    }
    
    // Croisement des chromosomes
    // Croisement de chromosome1
    debutCroisementY0 = rand() % tailleBinaireY0;  
    finCroisementY0 = rand() % tailleBinaireY0;
    if(debutCroisementY0 > finCroisementY0)
    {
        for(i = finCroisementY0; i <= debutCroisementY0; i++)
        {
            parent1.chromosome1[i] = clone_parent2.chromosome1[i];
            parent2.chromosome1[i] = clone_parent1.chromosome1[i];
        }
    }
    else
    {
        for(i = debutCroisementY0; i <= finCroisementY0; i++)
        {
            parent1.chromosome1[i] = clone_parent2.chromosome1[i];
            parent2.chromosome1[i] = clone_parent1.chromosome1[i];
        }
    }
    
    // Croisement de chromosome2
    debutCroisementB = rand() % tailleBinaireB;
    finCroisementB = rand() % tailleBinaireB;
    if(debutCroisementB > finCroisementB)
    {
        for(i = finCroisementB; i <= debutCroisementB; i++)
        {
            parent1.chromosome2[i] = clone_parent2.chromosome2[i];
            parent2.chromosome2[i] = clone_parent1.chromosome2[i];
        }
    }
    else
    {
        for(i = debutCroisementB; i <= finCroisementB; i++)
        {
            parent1.chromosome2[i] = clone_parent2.chromosome2[i];
            parent2.chromosome2[i] = clone_parent1.chromosome2[i];
        }
    }
    
    // Croisement de chromosome3
    debutCroisementDL = rand() % tailleBinaireDL;
    finCroisementDL = rand() % tailleBinaireDL;
    if(debutCroisementDL > finCroisementDL)
    {
        for(i = finCroisementDL; i <= debutCroisementDL; i++)
        {
            parent1.chromosome3[i] = clone_parent2.chromosome3[i];
            parent2.chromosome3[i] = clone_parent1.chromosome3[i];
        }
    }
    else
    {
        for(i = debutCroisementDL; i <= finCroisementDL; i++)
        {
            parent1.chromosome3[i] = clone_parent2.chromosome3[i];
            parent2.chromosome3[i] = clone_parent1.chromosome3[i];
        }
    }
    
    // Croisement de chromosome4
    debutCroisementLo = rand() % tailleBinaireLo;
    finCroisementLo = rand() % tailleBinaireLo;
    if(debutCroisementLo > finCroisementLo)
    {
        for(i = finCroisementLo; i <= debutCroisementLo; i++)
        {
            parent1.chromosome4[i] = clone_parent2.chromosome4[i];
            parent2.chromosome4[i] = clone_parent1.chromosome4[i];
        }
    }
    else
    {
        for(i = debutCroisementLo; i <= finCroisementLo; i++)
        {
            parent1.chromosome4[i] = clone_parent2.chromosome4[i];
            parent2.chromosome4[i] = clone_parent1.chromosome4[i];
        }
    }
    
    // Application d'une mutation aléatoire
    parent1 = mutationGenetiqueAleatoire(parent1);
    parent2 = mutationGenetiqueAleatoire(parent2);
    
    // Calcul du score de fitness pour chaque parent après le croisement
    parent1.fitnessScore = calculerErreurSolution(parent1, X, Z);
    parent2.fitnessScore = calculerErreurSolution(parent2, X, Z);
    
    // Retour de l'individu avec le meilleur score de fitness
    if (parent1.fitnessScore > parent2.fitnessScore)
        return parent2;
    else 
        return parent1; 
}


                                                             //deja optimiser 
void convertionEntierBinaire(int x, int bit, int a[bit]) {
    for (int i = 0; i < bit; i++) {
        a[i] = x % 2; // Renvoie 1 ou 0 (c'est à dire les diffrents restent des divisions euclidiennes successives)
        x = x / 2;    // Puis on divise x par 2, de nouveau et on recommence
    }
}

                                                             //deja optimiser
int identifierIndividuOptimal(individu population[dim]) {
    int meilleur = 0; // On suppose que le premier individu a le coût minimum

    for (int i = 1; i < dim; i++) {
        if (population[i].fitnessScore < population[meilleur].fitnessScore) {
            meilleur = i;
        }
    }

    return meilleur;
}

                                                             //deja optimiser
double convertionBinaireEntier(individu population[dim], int nb_bit, int indice_min) {
    double parent2 = 0;
    double puissance_deux = 0;

    switch (nb_bit) {
        case 12:
            for (int j = 0; j < nb_bit; j++) {
                puissance_deux = pow(2, j);
                parent2 += puissance_deux * population[indice_min].chromosome1[j];
            }
            break;

        case 17:
            for (int j = 0; j < nb_bit; j++) {
                puissance_deux = pow(2, j);
                parent2 += puissance_deux * population[indice_min].chromosome2[j];
            }
            break;

        case 19:
            for (int j = 0; j < nb_bit; j++) {
                puissance_deux = pow(2, j);
                parent2 += puissance_deux * population[indice_min].chromosome3[j];
            }
            break;

        case 26:
            for (int j = 0; j < nb_bit; j++) {
                puissance_deux = pow(2, j);
                parent2 += puissance_deux * population[indice_min].chromosome4[j];
            }
            break;
    }

    return parent2 / precision;
}

                                                             //deja optimiser
double densitePlasma(double x) {
    return exp(21.78515 + 1.2614 * log(x) / log(10));
}

void calculerNouveauxPointsDonnees(char nom_du_fichier[], double Y0, double B, double delta_lambda, double lambda_zero, ValeursExperimentales x)
{
	// Ouvre le fichier en mode écriture
    FILE* fichier = fopen(nom_du_fichier, "w+");
    
    // Vérifie si l'ouverture du fichier a réussi
    if (fichier == NULL) {
        printf("\nERREUR d'ouverture de fichier en écriture\nEXIT\n\n");
        exit(1);
    } 

	// Calculs nouveaux points
	for (int n = 0; n < 1024; n++)
	{
		// calcul de l'image de chaque point
		double image_f = fonctionLorentzienne(Y0, B, delta_lambda, lambda_zero, x[n]);

		// Écrit les valeurs dans le fichier
		fprintf(fichier, "%lf\t%lf\n", x[n], image_f);
    }

	// Ferme le fichier
    fclose(fichier);
}