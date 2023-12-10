#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float randomFloat1(float a, float b) {
    float Random = (float)rand();
    float N = Random / RAND_MAX * (b - a) + a;
    printf("Le nombre tiré au sort est... %f\n", Random);
    return N;
}

float randomFloat2(float a , float b)
{
    int Random = rand();
	float N = Random %(int)(((b-a)+1))+a ;
	printf("Le nombre tiré au sort est... %d\n", Random);
	return N;
}

int main() {
    // Initialisation de la graine pour rand()
    srand((unsigned int)time(NULL));
    
    float a = 1.0; // Valeur minimale
    float b = 10.0; // Valeur maximale
    
    float randomValue1 = randomFloat1(a,b);
    float randomValue2 = randomFloat2(a,b);
    
    printf("Nombre aléatoire entre %f et %f est :\n->fonction ChatGPT : %f\n-> fonction Projet : %f\n\n", a, b, randomValue1, randomValue2);
    return 0;
}


double conversion_binaire_entier(individu population[dim] , int nb_bit , int indice_min)
{
	int j ;
	double p , g; 
	
	g = 0 ;
	p = 0 ; 
	
	switch(nb_bit)
	{
		case 12 :
		for(j = 0 ; j < nb_bit ; j++ ) 
		{	p = pow(2,j) ;
			g = g + p*population[indice_min].G1[j] ;
		}
		break ;
		
		case 17 : 
		for(j = 0 ; j < nb_bit ; j++ )   ///// 
		{	p = pow(2,j) ;
			g = g + p*population[indice_min].G2[j] ; 
		}
		break ;

		case 19 :
		for(j = 0 ; j < nb_bit ; j++ )   ///// 
		{	p = pow(2,j) ;
			g = g + p*population[indice_min].G3[j] ; 
		}
		break ;
		
		case 26 :
		for(j = 0 ; j < nb_bit ; j++ )   
		{	p = pow(2,j) ;
			g = g + p*population[indice_min].G4[j] ;
		}
		break ; 
	}
	
	return g/precision ; 
}