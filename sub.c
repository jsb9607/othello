#include <stdio.h>
#include <stdlib.h>

#define N	6

extern char gameboard[N][N];

char init_othello(void)
{
	for(int i=0; i<N ; i++)
	{
		for(int j=0; j<N ; j++)
		{
			if(i == 2 && j == 2 || i == 3 && j == 3)
				gameboard[i][j] = 'W';			
			else if(i == 2 && j == 3 || i == 3 && j == 2)
				gameboard[i][j] = 'B';
			else				
				gameboard[i][j] = '.';
		}
	}		
}

void print_othello(char gameboard[][N])
{	
	printf("\n\n    0     1    2    3    4    5\n");
	
	for(int i=0; i<N ; i++)
	{
		printf("\n\n%d", i);		
		for(int j=0; j<N ; j++)
		{
			printf("    %c", gameboard[i][j]);
		}
		printf("\n");
	}	
	printf("\n\n");
}

