/**************************************************************************************************
NOM			Prenom		N°etudiant 

HAMADI      Rayen		22000116
FARADONI	Louis		22002260
VILLEMUR	Zoè			22009358
BOUYA		Jolivet		22003862
***************************************************************************************************/

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<time.h> 

/**************************************************************************************************
											macro #define
***************************************************************************************************/

#define dim 100
#define bit_Y0 12
#define bit_B 17
#define bit_DL 19
#define bit_Lo 26
#define precision 10000
#define limite 100 
#define seuil 0.01
/**************************************************************************************************
							déclaration des nouveaux types (typedef) 
***************************************************************************************************/
typedef struct {
	int G1[bit_Y0] ; int G2[bit_B] ; 
	int G3[bit_DL] ; int G4[bit_Lo] ; 
	double cout;
} individu; 

typedef double datas [1024];

typedef double vect[dim] ;



/**************************************************************************************************
											zone de prototype 
***************************************************************************************************/

double echantillon (double a , double b) ; 

void lecture(char *filepath, datas x, datas y, datas z, int *n);

void ecriture( char *nom_du_fichier , double x1 , double x2 , double x3 , double x4 ) ; 

double decodage(individu x , datas X , datas Y) ;

double f(double Y0 , double B , double DL , double Lo , double L) ;

individu croisement (individu f , individu g) ;

individu tournoi (individu population[dim]) ;

individu mutation(individu x) ;

int tri (individu population[dim]) ;

void binaire(int x , int bit , int a[bit]) ;

double conversion_binaire_entier(individu population[dim] , int nb_bit , int indice_min) ;

individu clone(individu x) ;

double densite(double x) ;

int main(void)
{
	/**************************************************************************************************
											zone de declaration 
	***************************************************************************************************/
	
	int i , j , N , n , g , min;
	vect Y0 , B , DL , Lo ;
	datas X , Y , Z;
	double g1 , g2 , g3 , g4 , p , d ;
	time_t t;
	individu A ;
	individu population[dim] ; 
	individu enfants[dim] ; 
	
	j=0 ;
	
	lecture("Data_Genetique.txt",X,Y,Z,&N);
	
	
	
	srand(time(&t));
	for(i=0 ; i<100 ; i++)
	{
		

		//echantillonage des parametres
		
		Y0[i] = echantillon (2100 , 2300) ;
		B[i] = echantillon (40000 , 120000) ; 
		DL[i] = echantillon (100000 , 300000) ;
		Lo[i] = echantillon (65600000 , 65640000) ;
		
		
		//conversion entier --> binaire 
		binaire(Y0[i],bit_Y0,population[i].G1); 
		binaire(B[i],bit_B,population[i].G2);
		binaire(DL[i],bit_DL,population[i].G3);
		binaire(Lo[i],bit_Lo,population[i].G4);
		
		//conversion binaire --> entier  + cout  
		population[i].cout = decodage(population[i] , X , Z ) ;
	}
	
	
	
	
	/*on genere à chaque fois une nouvelle generation par tournoi 
	et on refait le processus tant que la valeur du cout min est superieur à un certein seuil. */
	do
	{
		//on genere les descendants de la premiere population qu'on appelle enfants.  
		for (i=0 ; i < dim ; i++)
		{
			enfants[i] = tournoi(population) ;
		}
		
		// on stocke le rang de l'individu qui nous donne le coup le plus petit.
		min = tri(enfants) ;
		
		//on ecrase les individus contenu dans la premiere population et on stoke les nouvelles individus à leurs places.  
		for (i=0 ; i<dim ; i++)
		{	for(n=0  ; n<bit_Y0 ; n++)
				population[i].G1[n] = enfants[i].G1[n] ;
			for(n=0  ; n<bit_B ; n++)
				population[i].G2[n] = enfants[i].G2[n];
			for(n=0  ; n<bit_DL ; n++)			
				population[i].G3[n] = enfants[i].G3[n] ; 
			for(n=0  ; n<bit_Lo ; n++)
				population[i].G4[n] = enfants[i].G4[n] ; 
		} 
		printf("Iteration num = %d\n",j);
		j++ ;
	}while(enfants[min].cout >= seuil && j!=limite) ;
	
	
	//conversion binaire --> entier 
	g1 = conversion_binaire_entier(enfants , bit_Y0 , min) ;
	g2 = conversion_binaire_entier(enfants , bit_B , min) ;
	g3 = conversion_binaire_entier(enfants , bit_DL , min) ;
	g4 = conversion_binaire_entier(enfants , bit_Lo , min) ;
	
	ecriture( "valeur.txt" , g1 , g2 , g3 , g4 ) ;
	//affichage des parametres 
	printf("La valeur de Y0       vaut : %lf\n",g1) ;
	printf("La valeur de B        vaut : %lf\n",g2) ;
	printf("La valeur de D_lambda vaut : %lf\n",g3) ;
	printf("La valeur de Lambda_0 vaut : %lf\n",g4) ;
	
	d = densite(g3) ;
	printf("la densite electronique vaut : %lf m^-3\n",d) ;
	
	return 0 ;
}


/**************************************************************************************************
										Définition des fonctions 
***************************************************************************************************/

void lecture(char *filepath, datas x, datas y, datas z, int *n)
{

	*n = 0;
	FILE *nom = fopen(filepath,"r");  
	if(nom == NULL)  
	{
		printf("\nERREUR d'ouverture du fichier en lecture\nEXIT\n\n");
		exit(1); 
	}
	do
	{
		fscanf(nom,"%lf\t%lf\t%lf",&x[*n],&y[*n],&z[*n]);
		(*n)++;
		
	}while(!feof(nom));
	fclose(nom);
	(*n)--;
}


double echantillon (double a , double b)
{
	double x ;
	x = rand()%(int)(((b-a)+1))+a ;  //Renvoie une valeur qui appartient à l'intervalle [a,b]. 
	
	return x ;
}

double f(double Y0 , double B , double DL , double Lo , double L)
{	
	double x ; 
	x = Y0 + ((B)/(pow((L-Lo),2.0)+(pow((DL/2.0),2.0)))) ; //fonction Lorentzienne
	
	return x ;
}

double decodage(individu x , datas X , datas Y)
{
	int i , j , n ; 
	double Y0 , B , DL , Lo , p;
	double S; 
	
	Y0 = 0 ;
	B = 0 ;
	DL = 0 ;
	Lo = 0 ; 
	S = 0 ;

	for(j = 0 ; j < bit_Y0 ; j++ )
	{	p = pow(2,j) ;
		Y0 = Y0+p*x.G1[j] ;
	}
	for(j = 0 ; j < bit_B ; j++ )
	{	p = pow(2,j) ;
		B = B + p*x.G2[j] ;
	}
	
	for(j = 0 ; j < bit_DL ; j++ )
	{	p = pow(2,j) ;
		DL = DL + p*x.G3[j] ;
	}
	
	for(j = 0 ; j < bit_Lo ; j++ )
	{	p = pow(2,j) ;
		Lo = Lo + p*x.G4[j] ;
	}
	
	
	
	Y0 = Y0/precision ;
	B = B/precision ;
	DL = DL/precision ; 
	Lo = Lo/precision ; 
	
	for(n = 0 ; n<1024 ; n++)
	{
		S = S + pow((Y[n]-f(Y0 , B , DL , Lo , X[n] )),2) ; //le cout = erreur quadratique = ∑(y[i]-f[i])^2. 
	}
	
	
	return S ; 
} 


void ecriture( char *nom_du_fichier , double x1 , double x2 , double x3 , double x4 )
{	
	int i , j;

	FILE *nom = fopen(nom_du_fichier,"w+");
	if(nom == NULL)
	{
		printf("\nERREUR d'ouverture de fichier en écriture\nEXIT\n\n");
		exit(1); 
	} 
	
	fprintf(nom,"%lf\t%lf\t%lf\t%lf\n", x1 , x2 , x3 , x4);
	fclose(nom);

}



/* 
On choisit aleatoirement 4 individus et on compare leurs couts deux à deux et on croise les deux qui possedent le cout le plus faible. 
*/
individu tournoi (individu population[dim])
{
	int i1 , i2 , i3 , i4 ;

	i1 = rand()%100 ; 
	i2 = rand()%100 ; 
	i3 = rand()%100 ; 
	i4 = rand()%100 ; 
	
	if(population[i1].cout < population[i2].cout && population[i3].cout < population[i4].cout )
		return croisement(population[i1],population[i3]) ; 
	
	if(population[i1].cout < population[i2].cout && population[i3].cout > population[i4].cout )
		return croisement(population[i1],population[i4]) ;

	if(population[i1].cout > population[i2].cout && population[i3].cout < population[i4].cout )
		return croisement(population[i2],population[i3]) ;

	if(population[i1].cout > population[i2].cout && population[i3].cout > population[i4].cout )
		return croisement(population[i2],population[i4]) ;

}

/*
on fixe le seuil de mutation à 1%. 
*/
individu mutation(individu x)
{
	int proba_mutation;
	int m , n;
	proba_mutation=rand()%100;
	
	
	if(proba_mutation==1 ) // proba de mutation de l'ordre de 1%
	{
		n=rand()%4 ;  // gene à muter 
		switch(n)
		{
			case 0 :
			m=rand()%bit_Y0 ; //position dans le gene à muter 
			if(x.G1[m]==0)
				x.G1[m]=1 ;
			else
				x.G1[m]=0 ;
			break ;
			case 1 :
			m=rand()%bit_B ;
			if(x.G2[m]==0)
				x.G2[m]=1 ;
			else
				x.G2[m]=0 ;
			break ;
			case 2 : 
			m=rand()%bit_DL ;
			if(x.G3[m]==0)
				x.G3[m]=1 ;
			else
				x.G3[m]=0 ;
			break ;
			case 3 :
			m=rand()%bit_Lo ;
			if(x.G4[m]==0)
				x.G4[m]=1 ;
			else
				x.G4[m]=0 ;
			break ;
		}
	}
	return x ;
}


individu croisement(individu f , individu g)
{
	int r1 , r2 , r3 , r4 , lim1 , lim2 , lim3 , lim4 , i , j , N;
	individu F , G ;
	datas X , Y , Z; 
	lecture("Data_Genetique.txt",X,Y,Z,&N) ;
	
	
	
	//clone Y0/G1
	for(i=0 ; i<bit_Y0 ; i++)
	{
		G.G1[i]=g.G1[i] ;
		F.G1[i]=f.G1[i] ;
	}
	
	//clone B/G2
	for(i=0 ; i<bit_B ; i++)
	{
		G.G2[i]=g.G2[i] ;
		F.G2[i]=f.G2[i] ;
	}
	
	//clone DL/G3
	for(i=0 ; i<bit_DL ; i++)
	{
		G.G3[i]=g.G3[i] ;
		F.G3[i]=f.G3[i] ;
	}
	
	//clone Lo/G4 
	for(i=0 ; i<bit_Lo ; i++)
	{
		G.G4[i]=g.G4[i] ;
		F.G4[i]=f.G4[i] ;
	}
	
	//G1 
	r1=rand()%(int)bit_Y0 ;  
	lim1 = rand()%(int)bit_Y0 ;
	
	if(r1>lim1)
	{
		while(i>=r1)
		{
			f.G1[i]=g.G1[i];
			f.G2[i]=g.G2[i];
			f.G3[i]=g.G3[i];
			f.G4[i]=g.G4[i];
			g.G1[i]=F.G1[i];
			g.G2[i]=F.G2[i];
			g.G3[i]=F.G3[i];
			g.G4[i]=F.G4[i];
			i--;
		}
	}
	else
	{
		for(i=r1;i<=lim1;i++)
		{
			f.G1[i]=g.G1[i];
			f.G2[i]=g.G2[i];
			f.G3[i]=g.G3[i];
			f.G4[i]=g.G4[i];
			g.G1[i]=F.G1[i];
			g.G2[i]=F.G2[i];
			g.G3[i]=F.G3[i];
			g.G4[i]=F.G4[i];
		}
	}
	
	
	//G2
	r2=rand()%(int)bit_B ;
	lim2 = rand()%(int)bit_B;
	
	if(r2>lim2)
	{
		while(i>=r2)
		{
			f.G1[i]=g.G1[i];
			f.G2[i]=g.G2[i];
			f.G3[i]=g.G3[i];
			f.G4[i]=g.G4[i];
			g.G1[i]=F.G1[i];
			g.G2[i]=F.G2[i];
			g.G3[i]=F.G3[i];
			g.G4[i]=F.G4[i];
			i--;
		}
	}
	else
	{
		for(i=r2;i<=lim2;i++)
		{
			f.G1[i]=g.G1[i];
			f.G2[i]=g.G2[i];
			f.G3[i]=g.G3[i];
			f.G4[i]=g.G4[i];
			g.G1[i]=F.G1[i];
			g.G2[i]=F.G2[i];
			g.G3[i]=F.G3[i];
			g.G4[i]=F.G4[i];
		}
	}
	
	//G3
	r3=rand()%(int)bit_DL ;
	lim3 = rand()%(int)bit_DL;
	
	if(r3>lim3)
	{
		while(i>=r3)
		{
			f.G1[i]=g.G1[i];
			f.G2[i]=g.G2[i];
			f.G3[i]=g.G3[i];
			f.G4[i]=g.G4[i];
			g.G1[i]=F.G1[i];
			g.G2[i]=F.G2[i];
			g.G3[i]=F.G3[i];
			g.G4[i]=F.G4[i];
			i--;
		}
	}
	else
	{
		for(i=r3;i<=lim3;i++)
		{
			f.G1[i]=g.G1[i];
			f.G2[i]=g.G2[i];
			f.G3[i]=g.G3[i];
			f.G4[i]=g.G4[i];
			g.G1[i]=F.G1[i];
			g.G2[i]=F.G2[i];
			g.G3[i]=F.G3[i];
			g.G4[i]=F.G4[i];
		}
	}
	
	//G4
	r4=rand()%(int)bit_Lo ;
	lim4 = rand()%(int)bit_Lo;
	
	if(r4>lim4)
	{
		while(i>=r4)
		{
			f.G1[i]=g.G1[i];
			f.G2[i]=g.G2[i];
			f.G3[i]=g.G3[i];
			f.G4[i]=g.G4[i];
			g.G1[i]=F.G1[i];
			g.G2[i]=F.G2[i];
			g.G3[i]=F.G3[i];
			g.G4[i]=F.G4[i];
			i--;
		}
	}
	else
	{
		for(i=r4;i<=lim4;i++)
		{
			f.G1[i]=g.G1[i];
			f.G2[i]=g.G2[i];
			f.G3[i]=g.G3[i];
			f.G4[i]=g.G4[i];
			g.G1[i]=F.G1[i];
			g.G2[i]=F.G2[i];
			g.G3[i]=F.G3[i];
			g.G4[i]=F.G4[i];
		}
	}
	
	//mutation 
	f = mutation(f) ;
	g = mutation(g) ;
	
	
	f.cout = decodage(f , X , Z) ;
	g.cout = decodage(g , X , Z) ;
	
	if (f.cout>g.cout)
		return g ;
	else 
		return f ; 
}


void binaire(int x , int bit , int a[bit])
{
	int i=0 ;
	int b[bit] ;
	do
	{
		a[i] = x%2; //renvoi 1 ou 0 
		x=x/2;      // le reste de la division 
		i++;
	}
	while(x!=0);
}

int tri (individu population[dim])
{
	int i , j ; 
	double min ;
	
	min = population[1].cout ; //on fixe la premiere valeur de min 
	for(i=1 ; i<dim ; i++)
	{
		if(population[i].cout <= min)
		{
			min = population[i].cout ; 
			/* on compare la valeur de min aux couts de la population et à chaque fois le cout est plus petit le min prend sa valeur.*/
			j=i ;
		}
	}
	return j ; //on renvoi le rang de l'individu qui a le plus petit cout. 
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

double densite(double x)
{
	double y ;
	
	y = pow(10,21.78515 + 1.2614*log10(x)) ;
	
	return y ;
}
