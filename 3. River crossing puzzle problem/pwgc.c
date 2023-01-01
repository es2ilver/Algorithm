#include <stdio.h>
#include <stdlib.h>

#define PEASANT 0x08
#define WOLF	0x04
#define GOAT	0x02
#define CABBAGE	0x01

// 주어진 상태 state의 이름(마지막 4비트)을 화면에 출력
// 예) state가 7(0111)일 때, "<0111>"을 출력
static void print_statename( FILE *fp, int state);

// 주어진 상태 state에서 농부, 늑대, 염소, 양배추의 상태를 각각 추출하여 p, w, g, c에 저장
// 예) state가 7(0111)일 때, p = 0, w = 1, g = 1, c = 1
static void get_pwgc( int state, int *p, int *w, int *g, int *c);

// 허용되지 않는 상태인지 검사
// 예) 농부없이 늑대와 염소가 같이 있는 경우 / 농부없이 염소와 양배추가 같이 있는 경우
// return value: 1 허용되지 않는 상태인 경우, 0 허용되는 상태인 경우
static int is_dead_end( int state);

// state1 상태에서 state2 상태로의 전이 가능성 점검
// 농부 또는 농부와 다른 하나의 아이템이 강 반대편으로 이동할 수 있는 상태만 허용
// 허용되지 않는 상태(dead-end)로의 전이인지 검사
// return value: 1 전이 가능한 경우, 0 전이 불이가능한 경우 
static int is_possible_transition( int state1,	int state2);

// 상태 변경: 농부 이동
// return value : 새로운 상태
static int changeP( int state);

// 상태 변경: 농부, 늑대 이동
// return value : 새로운 상태, 상태 변경이 불가능한 경우: -1
static int changePW( int state);

// 상태 변경: 농부, 염소 이동
// return value : 새로운 상태, 상태 변경이 불가능한 경우: -1
static int changePG( int state);

// 상태 변경: 농부, 양배추 이동
// return value : 새로운 상태, 상태 변경이 불가능한 경우: -1 
static int changePC( int state);

// 주어진 state가 이미 방문한 상태인지 검사
// return value : 1 visited, 0 not visited
static int is_visited( int visited[], int depth, int state);

// 방문한 경로(상태들)을 차례로 화면에 출력
static void print_path( int visited[], int depth);

// recursive function
static void dfs_main( int initial_state, int goal_state, int depth, int visited[]);

////////////////////////////////////////////////////////////////////////////////
// 상태들의 인접 행렬을 구하여 graph에 저장
// 상태간 전이 가능성 점검
// 허용되지 않는 상태인지 점검 
void make_adjacency_matrix( int graph[][16]);

// 인접행렬로 표현된 graph를 화면에 출력
void print_graph( int graph[][16], int num);

// 주어진 그래프(graph)를 .net 파일로 저장
// pgwc.net 참조
void save_graph( char *filename, int graph[][16], int num);

////////////////////////////////////////////////////////////////////////////////
// 깊이 우선 탐색 (초기 상태 -> 목적 상태)
void depth_first_search( int initial_state, int goal_state);



////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	int graph[16][16] = {0,};
	
	// 인접 행렬 만들기
	make_adjacency_matrix( graph);

	// 인접 행렬 출력 (only for debugging)
	print_graph( graph, 16);
	
	// .net 파일 만들기
	save_graph( "pwgc.net", graph, 16);

	// 깊이 우선 탐색
	depth_first_search( 0, 15); // initial state, goal state
	
	return 0;
}








// 주어진 상태 state의 이름(마지막 4비트)을 화면에 출력
// 예) state가 7(0111)일 때, "<0111>"을 출력
static void print_statename( FILE *fp, int state)
{
	int i;
	printf("<");
	for(i=3; i>=0; i--)
	{
		printf("%d", state >> i & 1);
	}//비트 연산 활용 (자릿수만큼 >> 연산 후 우측 끝 비트와 1을 비교함. 8421 순서로...)
	printf(">");


}

// 주어진 상태 state에서 농부, 늑대, 염소, 양배추의 상태를 각각 추출하여 p, w, g, c에 저장
// 예) state가 7(0111)일 때, p = 0, w = 1, g = 1, c = 1
static void get_pwgc( int state, int *p, int *w, int *g, int *c)
{
	if(state & PEASANT)
		*p=1;
	if(state & WOLF)
		*w=1;
	if(state & GOAT)
		*g=1;
	if(state & CABBAGE)
		*c=1;

}

// 허용되지 않는 상태인지 검사
// 예) 농부없이 늑대와 염소가 같이 있는 경우 / 농부없이 염소와 양배추가 같이 있는 경우
// return value: 1 허용되지 않는 상태인 경우, 0 허용되는 상태인 경우
static int is_dead_end( int state)
{
	int p=0,w=0,g=0,c=0;
	get_pwgc(state, &p, &w, &g, &c);

	if(p) //농부 있음-> 반대편 검사, 농부쪽에 없음
	{
		if(w+g==0)
			return 1;
		if(g+c==0)
			return 1;
	}
	else
	{
		if(w+g==2)
			return 1;
		if(g+c==2)
			return 1;
	}
	return 0;
}

// state1 상태에서 state2 상태로의 전이 가능성 점검
// 농부 또는 농부와 다른 하나의 아이템이 강 반대편으로 이동할 수 있는 상태만 허용
// 허용되지 않는 상태(dead-end)로의 전이인지 검사
// return value: 1 전이 가능한 경우, 0 전이 불이가능한 경우 
static int is_possible_transition( int state1,	int state2)
{
	
	if(state2==changeP(state1) || state2==changePW(state1)
		|| state2==changePG(state1)|| state2==changePC(state1))
		return 1;
	else
		return 0;
}

// 상태 변경: 농부 이동
// return value : 새로운 상태
static int changeP( int state)
{
	int p=0,w=0,g=0,c=0;
	get_pwgc(state, &p, &w, &g, &c);

	if(p==1)//농부 있음
		return state-PEASANT;
	else
		return state+PEASANT;

}

// 상태 변경: 농부, 늑대 이동
// return value : 새로운 상태, 상태 변경이 불가능한 경우: -1
static int changePW( int state)
{	
	
	int p=0,w=0,g=0,c=0;
	get_pwgc(state, &p, &w, &g, &c);

	if(p+w==2)//농부 있음, 늑대 있음
		return state-PEASANT-WOLF;
	else if(p+w==0)
		return state+PEASANT+WOLF;
	else return -1;

}

// 상태 변경: 농부, 염소 이동
// return value : 새로운 상태, 상태 변경이 불가능한 경우: -1
static int changePG( int state)
{	

	int p=0,w=0,g=0,c=0;
	get_pwgc(state, &p, &w, &g, &c);

	if(p+g==2)//농부 있음, 염소 있음
		return state-PEASANT-GOAT;
	else if(p+g==0)
		return state+PEASANT+GOAT;
	else return -1;

}

// 상태 변경: 농부, 양배추 이동
// return value : 새로운 상태, 상태 변경이 불가능한 경우: -1 
static int changePC( int state)
{
	int p=0,w=0,g=0,c=0;
	get_pwgc(state, &p, &w, &g, &c);

	if(p+c==2)//농부 있음, 양배추 있음
		return state-PEASANT-CABBAGE;
	else if(p+c==0)
		return state+PEASANT+CABBAGE;
	else return -1;

}


// 주어진 state가 이미 방문한 상태인지 검사
// return value : 1 visited, 0 not visited
static int is_visited( int visited[], int depth, int state)
{
	int i;
	
	for(i=0; i<depth; i++)
	{	
		if(state == visited[i])
			return 1;
	}
	return 0;
}

// 방문한 경로(상태들)을 차례로 화면에 출력
static void print_path( int visited[], int depth)
{
	int i;
	FILE *fp;

	for(i=0; i<=depth; i++)
	{	
		print_statename(fp, visited[i]);
		printf("\n");
	}

}


////////////////////////////////////////////////////////////////////////////////
// 상태들의 인접 행렬을 구하여 graph에 저장
// 상태간 전이 가능성 점검
// 허용되지 않는 상태인지 점검 
void make_adjacency_matrix( int graph[][16])
{	
	//i에서 j로의 가능성이라고 이해했음
	int i, j;

	for(i=0; i<16; i++)
	{
		if(is_dead_end(i))
			continue;

		for(j=0; j<16; j++)
		{		
			if(i==j)
				continue;
			if(is_possible_transition(i, j))
			{
				if(!is_dead_end(j))
				{
					graph[i][j]=1;
				}
			}
			

				
		}
		
	}

}

// 인접행렬로 표현된 graph를 화면에 출력
void print_graph( int graph[][16], int num)
{
	int i, j;
	for(i=0; i<16; i++)
	{
		for(j=0; j<16; j++)
		{
			printf("%d\t", graph[i][j]);
		}
		printf("\n");
	}
}

// 주어진 그래프(graph)를 .net 파일로 저장
// pgwc.net 참조
void save_graph( char *filename, int graph[][16], int num)
{

	FILE *fp;
	fp = fopen(filename, "w");
	int i, j;


	fprintf(fp, "*Vertices 16\n");

	for(i=0; i<16; i++)
	{
		fprintf(fp, "%d \"<", i+1);
		for(j=3; j>=0; j--)
			fprintf(fp, "%d", i >> j & 1);
		fprintf(fp,">\"\n");
			
	}



	fprintf(fp, "*Edges\n");
	
	for(i=0; i<8; i++)
	{
		for(j=0; j<16; j++)
		{
			if(graph[i][j])
			fprintf(fp, "  %d  %d\n", i+1, j+1);
		}
	
	}






	fclose(fp);




}

////////////////////////////////////////////////////////////////////////////////
// 깊이 우선 탐색 (초기 상태 -> 목적 상태)
void depth_first_search( int initial_state, int goal_state)
{
	int depth = 0;
	int visited[16] = {0,}; // 방문한 정점을 저장
	
	dfs_main( initial_state, goal_state, depth, visited); 
}
// recursive function
static void dfs_main( int initial_state, int goal_state, int depth, int visited[])
{

	int i;
	int state = initial_state;
	FILE *fp;

	printf("current state is ");
	print_statename(fp, state);
	printf(" (depth %d)\n", depth);
	



	for(i=0; i<4; i++)
	{	
		
		if(state == goal_state)
		{
			printf("Goal-state found!\n");
			
			break;
		}


		int nowstate;

		switch(i)
		{
			case 0:
				nowstate = changeP(state);
				break;
			case 1:
				nowstate = changePW(state);
				break;
			case 2:
				nowstate = changePG(state);
				break;
			case 3: 
				nowstate = changePC(state);
				break;
			default: -1;
		} if(nowstate<0) continue;
	

			if(is_possible_transition(state, nowstate))
			{	
				
				 
				if(is_dead_end(nowstate))
				{					
					printf("\tnext state ");
					print_statename(fp, nowstate);
					printf(" is dead-end\n");
				}
				else
				{	
					if(is_visited(visited,depth,nowstate))
					{
						printf("\tnext state ");
						print_statename(fp, nowstate);
						printf(" has been visited\n");

					}
					else
					{

						
						visited[++depth]=nowstate;
						if(nowstate==goal_state)
						{	
							printf("current state is <1111> (depth %d)\n", depth);	

							printf("Goal-state found!\n");
							print_path(visited,depth);
							printf("\n");

						}
						else
						{
							

							dfs_main(nowstate, goal_state, depth, visited);
						}

						printf("back to ");
						print_statename(fp, state);
						printf(" (depth %d)\n", --depth);
						

						
					}

					

				}
			}
			else continue;


			

			
	}

	depth--;
	if(depth>=0)
	{	
		

		int k;
		for(k=15; k>depth+1; k--)
		visited[k]=0;

	}

	
}




					
				


			


		
		


	
