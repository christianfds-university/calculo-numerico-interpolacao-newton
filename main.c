#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//3 -1 4 0 1 2 -1
//P(x) = +0.667(x^2) -2.333x +1.000

void multiplicaPolinomio(int n1, int n2, double *polinomio1, double *polinomio2);
void somaPolinomio(int n, double *polinomio1, double *polinomio2);
double *calculaResultado(int n, double *vetorx, double *vetorFx);
void lerPontos(int n, double *vetorx, double *vetorFx);
void imprimeResultado(int n, double *resultado);
void testaValores(int n, double *resultado);

int main()
{
	int n;
	double *vetorx, *vetorFx, *resultado;

	printf("Quantos pontos serão lidos?\n");
	scanf("%d", &n);

	vetorx    = (double *) calloc(n,sizeof(double));
	vetorFx   = (double *) calloc(n,sizeof(double));

	lerPontos(n,vetorx,vetorFx);
	putchar('\n');
	resultado = calculaResultado(n,vetorx,vetorFx);
	imprimeResultado(n,resultado);

	testaValores(n,resultado);

	free(vetorx); free(vetorFx); free(resultado);
}

void imprimeResultado(int n, double *resultado)
{//Imprime o Polinomio interpolador encontrado
	int i;
	printf("Polinômio Interpolador:\nP(x)= ");
	for(i = n-1; i >= 0; i--)
	{
		if(i==0) printf("%+2.3lf\n", resultado[i]);
		else 
		if(i==1) printf("%+2.3lfx ", resultado[i]);
		else     printf("%+2.3lf(x^%d) ", resultado[i], i);
	}
}


void multiplicaPolinomio(int n1, int n2, double *polinomio1, double *polinomio2)//Multiplica dois polinomios
{ //Resultado será devolvido no polinômio 1
	double resultado[n1+n2-1];
	int i,j;

	for(i = 0; i < (n1+n2-1); i++)
		resultado[i] = 0;

	for(i = 0; i < n1; i++)
		for(j = 0; j < n2; j++)
			resultado[i+j] += polinomio1[i] * polinomio2[j];

	for(i = 0; i < (n1+n2-1); ++i)
		polinomio1[i] = resultado[i];
}

void somaPolinomio(int n, double *polinomio1, double *polinomio2)//Soma dois polinomios
{ // Resultado será devolvido no polinômio 1
	int i;
	for(i = 0; i < n; ++i)
		polinomio1[i] += polinomio2[i];
}

double *calculaResultado(int n, double *vetorx, double *vetorFx)//Realiza todos os calculos referentes
{//Ao método de newton e retorna o polinômio interpolador
	double m[n+n-1][n], aux[n+n-1], diferenca[n], polinomios[n][n], polinomio[2], a;
	double *resultado = calloc(n,sizeof(double));
	int auxiliar = n, flag = 1, i,j;

	for(i = 0; i < n; i++)
	{//inicializa aux e m
		aux[i*2]  = vetorx[i];
		m[i*2][0] = vetorFx[i];
	}

	for(i = 1; i < n; i++)
		for (j = i; j < n+n-2; j+=2)
		{//Realiza os calculos em m
			m[j][i] = m[j+1][i-1] - m[j-1][i-1];
			a = (aux[j+i])-(aux[j-i]);
			m[j][i]/=a;
		}

	for(i = 0; i < n; i++)//Inicializa o vetor diferenca
		diferenca[i] = m[i][i];

	polinomios[0][0]=vetorFx[0];
	polinomio[1] = 1;

	for(i = 1; i < n; i++)
	{//Realiza as multiplicações entre os polinomios
		polinomios[i][0] = -vetorx[0];
		polinomios[i][1] = 1;
		for(j = 1; j < i; j++)
		{
			polinomio[0] = -vetorx[j];
			multiplicaPolinomio(n,n,polinomios[i],polinomio);
		}
	}

	for (i = 1; i < n; i++)//Multiplica cada elemento do polinomio pelo valor da diferença
		for(j = 0; j < n; j++)
			polinomios[i][j] *= diferenca[i];
	
	for (i = 0; i < n; i++)//Soma todos os polinomios, gerando o resultado final
		somaPolinomio(n,resultado,polinomios[i]);

	return resultado;
}


void lerPontos(int n, double *vetorx, double *vetorFx)
{//Le pontos do console do usuário
	int i;
	for(i = 0; i < n; i++)
	{
		printf("x%d: ", i);
		scanf("%lf",vetorx+i);
		printf("f(x%d): ", i);
		scanf("%lf",vetorFx+i);
	}
}

void testaValores(int n, double *polinomio)
{//Testa valores inseridos pelo usuário, no polinomio
	char a;
	double x;
	double resultado;
	int i;
	getchar();
	printf("\n\nDeseja testar algum valor? s/n\n");
	a = getchar();
	while(a=='s')
	{
		scanf("%lf", &x);
		resultado = 0;
		for(i = 0; i < n; i++)
		{
			if(i==0) resultado += polinomio[i];
			else     resultado += polinomio[i] * pow(x,i);
		}
		printf("\nResultado para f(%2.3lf): %5.5lf\n\n", x, resultado);
		getchar();
		printf("Deseja testar algum valor? s/n\n");
		a = getchar();
	}
}
