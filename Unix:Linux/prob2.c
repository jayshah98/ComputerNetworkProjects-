//Jay Shah
//1-9-20
//Lab 1 Part 2
//implements quantitative comparisons between circuit switching and packet switching


#include <stdio.h>
#include <stdlib.h>
#include <math.h> 

double fact(int); 
int main(int argc, char *argv[]){
	double linkBandwidth =atoi(argv[1]);
    double userBandwidth = atoi(argv[2]);
    double tPSuser = atof(argv[3]);
    double nPSusers = atoi(argv[4]);

    
	double nCSusers = linkBandwidth/userBandwidth; 
	printf("%lf circuit switched users are supported\n" ,nCSusers); 

	double pPSusers = tPSuser; 
	printf("%lg is the probability that a user is busy trasnmiting\n" ,pPSusers); 

	double pPSusersNotBusy = 1-pPSusers; 
	printf("%lg is the probability that one specific other user is not busy\n",pPSusersNotBusy); 

	double all = pow((1-pPSusers),(nPSusers-1)); 
	printf("%lg is the probability that all of the other specific other users are not busy\n",all); 
	
	double one_user = pPSusers*(pow(pPSusersNotBusy,(nPSusers-1))); 
	printf("%lg is the probability that one user is transmitting and remaining users are not transmitting\n",one_user); 
	
	double exact = nPSusers*pPSusers*(pow(pPSusersNotBusy,(nPSusers-1))); 
	printf("%lg is the probability that exactly one of the nPSusers is busy\n",exact); 

	double ten = (pow(pPSusers,10))*(pow(pPSusersNotBusy,(nPSusers-10)));
	printf("%lg is the probability that 10 specific users of nPSusers are transmitting and the others are idle\n",ten); 

	double binomial_coeff = ((fact(nPSusers))/((fact(10))*(fact(nPSusers- 10))));
	binomial_coeff = binomial_coeff*(pow(pPSusers,10))*(pow(pPSusersNotBusy,nPSusers-10)); 
	printf("%lg is the probability that any 10 users of nPSusers are transmitting and the others are idle\n",binomial_coeff);
 
	int i; 
	double prob1=0;
	for(i=11;i<nPSusers;i++) { 
        prob1 += fact(nPSusers)/(fact(i)*fact(nPSusers-i))*pow(pPSusers, i) * pow(pPSusersNotBusy,nPSusers-i);
	}
	printf("%lg is the probability that more than 10 users of nPSusers are transmitting and the others are idle\n",prob1); 
	return 0; 
	
} 
double fact(int n) {
	 if(n>=1) 
		return n*fact(n-1); 
	else 
		return 1;
} 

