#include <stdio.h>
#include <stdlib.h>

#define N	6

char gameboard[N][N];


typedef struct {	

	char player[2];			//Player
	int row;				//현재 Player가 둔 오델로의 행
	int col;				//현재 Player가 둔 오델로의 열
	int turn;				//현재 Player턴 -> 0 : W, 1 : B
	int turnoverNB;			//뒤집는 방향 갯수
	int final_pos[8][2];	//알 뒤집을때 마지막 알의 위치	
	int offset[8][2];		//현재 Player의 알을 찾을때 행과 열의 offset
	
}Player_state;


extern char init_othello();

extern void print_othello(char gameboard[][N]);

int check_blank(char gameboard[][N])
{
	int blankNB = 0;
	
	for(int i=0; i<N ; i++)
	{
		for(int j=0; j<N ; j++)
		{
			if(gameboard[i][j] == '.')
				blankNB ++;
		}
	}			
	return blankNB;
}


int check_InputVaild(char gameboard[][N], Player_state *state)
{	
	state->turnoverNB = 0;
	
	for(int row = state->row-1 ; row <= state->row+1 ; row++)
	{		
		for(int col = state->col-1 ; col <= state->col+1 ; col++)
		{
			if(row >= 0 && row<=5 && col >= 0 && col<=5)	//찾는 주변의 상대 오델로는 보드안에 있어야함
			{
				if(gameboard[row][col] == state->player[(state->turn+1)%2])	//내 주변에 상대 오델로가 존재할때		
				{
					state->offset[state->turnoverNB][1] = col - state->col;
					state->offset[state->turnoverNB][0] = row - state->row;
						
					for(int i = row, j = col; ;i += state->offset[state->turnoverNB][0], j += state->offset[state->turnoverNB][1])
					{						
						if(i > 5 || j > 5 || i < 0 || j < 0)	break;	//board범위를 넘은 경우
						
						if(gameboard[i][j] == state->player[state->turn])	//현재의 player와 같은 색의 알이 있는 경우
						{
							state->final_pos[state->turnoverNB][0] = i;							
							state->final_pos[state->turnoverNB][1] = j;
							state->turnoverNB++;
							break;
						}						
					}					
				}
			}				
		}		
	}

	return state->turnoverNB;
	
}

void check_color(char gameboard[][N], int BW_NB[])
{			
	for(int i=0; i<N ; i++)
	{
		for(int j=0; j<N ; j++)
		{
			if(gameboard[i][j] == 'W')
				BW_NB[0]++;
			else if(gameboard[i][j] == 'B')
				BW_NB[1]++;
		}
	}
}

int check_Turnover(char gameboard[][N], Player_state *state)
{
	for(int i=0; i<N ; i++)
	{
		state->row = i;
		
		for(int j=0; j<N ; j++)
		{
			state->col = j;
			
			if(gameboard[i][j] == '.')
			{
				if(check_InputVaild(gameboard, state))		
					return 1;	//뒤집을 수 있는 알이 존재하는 경우
			}
		}
	}	

	return 0;	
}


int isGameEnd(char gameboard[][N], Player_state *state)	
{
	int count = 0;
	int BW_NB[2] = {0,};	//BW_NB[0] : 'W'개수, BW_NB[1] : 'B'개수

	// ------'B', 'W'둘다 뒤집을 수 있는 알이 없을때
	for(int k=0; k<2 ; k++)
	{
		state->turn = (state->turn + 1)%2;
		if(!check_Turnover(gameboard, state))
		{
			count ++;
		}
	}	
	if(count == 2)	
	{
		return 1;
	}

	// ------ 보드가 꽉 찼을때
	if(check_blank(gameboard)==0)	
	{
		return 1;
	}	

	// ------ 한가지 색의 알만 있는 경우
	check_color(gameboard, BW_NB);
	printf("\n----------------(White : %d, Black : %d)----------------\n", BW_NB[0], BW_NB[1]);
	if(BW_NB[0] == BW_NB[0]+BW_NB[1] || BW_NB[1] == BW_NB[0]+BW_NB[1]) 
	{
		return 1;
	}
	
	return 0;	
}


int Turnover(char gameboard[][N], Player_state *state)
{
	int Turnover_num = 0; //뒤집은 알의 개수

	for(int k = 0 ; k<state->turnoverNB ; k++)
	{
		for(int i = state->row + state->offset[k][0] , j = state->col + state->offset[k][1]; ;i += state->offset[k][0], j += state->offset[k][1])
		{
			gameboard[i][j] = state->player[state->turn];			
			if(i == state->final_pos[k][0] && j == state->final_pos[k][1])	break;				
			Turnover_num ++;
		}
	}
	return Turnover_num;
}

 
int main(int argc, char *argv[]) {

	Player_state *state = (Player_state *)malloc(sizeof(Player_state));
		
	state->player[0] = 'W';	//White player
	state->player[1] = 'B';	//Black player
	state->turn = 0;		//turn = 0-> White player turn , turn = 1-> Black player turn 
	
	int turn_num = 0;		//뒤집은 알의 개수
	int BW_NB[2] = {0,};	//BW_NB[0] : 'W'개수, BW_NB[1] : 'B'개수

	
	gameboard[N][N] = init_othello();


	while(isGameEnd(gameboard, state) == 0)
 	{		
		state->turnoverNB = 0; 			
		
		print_othello(gameboard);

 		if(!check_Turnover(gameboard, state)) // 현재 player가 뒤집을 수 있는 알이 없는 경우 -> 다음 turn으로 자동으로 넘김
		{
			printf("현재 player가 뒤집을 수 있는 알이 존재하지 않습니다! ---- turn pass\n");
 			state->turn = (state->turn+1)%2;
			continue;
		}

		printf("put a new %c othello (ex : row=2, column=3 -> 2,3) : ", state->player[state->turn]);
		scanf("%i, %i", &state->row, &state->col);

		if(gameboard[state->row][state->col] != '.')	//이미 알이 있는 곳에 둔 경우
		{
			printf("already filled!! retry!!\n");
			continue;
		}		
				
 		if(check_InputVaild(gameboard, state))	//player가 둔 알에 의해 뒤집히는 알이 있는지 확인
 		{			
			gameboard[state->row][state->col] = state->player[state->turn];	//현재 turn의 player의 오델로 두기
			turn_num = Turnover(gameboard, state);	//뒤집기
			printf("\n======> %c가 뒤집은 알의 개수 : %d\n",state->player[state->turn], turn_num);						
			state->turn = (state->turn+1)%2;
		}
		else //뒤집을 수 있는 알이 없는 경우
		{
			printf("뒤집을 수 있는 알이 없다! 다시 입력하십시오.\n");			
		}			
	}

	print_othello(gameboard);
	check_color(gameboard, BW_NB);
	
	printf("총 알의 개수 : W = %d, B = %d\n", BW_NB[0], BW_NB[1]);
	
	if(BW_NB[0]>BW_NB[1])	
		printf("승자는 W입니다.\n");
	else if(BW_NB[0]<BW_NB[1])	
		printf("승자는 B입니다.\n");
	else 
		printf("동점입니다.\n");

	free(state);
	
	return 0;
}

