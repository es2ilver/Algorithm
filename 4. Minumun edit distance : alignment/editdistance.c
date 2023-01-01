#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define INSERT_OP      0x01 //0001
#define DELETE_OP      0x02 //0010
#define SUBSTITUTE_OP  0x04 //0100
#define MATCH_OP       0x08 //1000
#define TRANSPOSE_OP   0x10 //10000

#define INSERT_COST	1
#define DELETE_COST	1
#define SUBSTITUTE_COST	1
#define TRANSPOSE_COST	1

// 재귀적으로 연산자 행렬을 순회하며, 두 문자열이 최소편집거리를 갖는 모든 가능한 정렬(alignment) 결과를 출력한다.
// op_matrix : 이전 상태의 연산자 정보가 저장된 행렬 (1차원 배열임에 주의!)
// col_size : op_matrix의 열의 크기
// str1 : 문자열 1
// str2 : 문자열 2
// n : 문자열 1의 길이
// m : 문자열 2의 길이
// level : 재귀호출의 레벨 (0, 1, 2, ...)
// align_str : 정렬된 문자쌍들의 정보가 저장된 문자열 배열 예) "a - a", "a - b", "* - b", "ab - ba"
static void backtrace_main( int *op_matrix, int col_size, char *str1, char *str2, int n, int m, int level, char align_str[][8]);



// 강의 자료의 형식대로 op_matrix를 출력 (좌하단(1,1) -> 우상단(n, m))
// 각 연산자를 다음과 같은 기호로 표시한다. 삽입:I, 삭제:D, 교체:S, 일치:M, 전위:T
//op(i*colsize+j)
void print_matrix( int *op_matrix, int col_size, char *str1, char *str2, int n, int m);

// 두 문자열 str1과 str2의 최소편집거리를 계산한다.
// return value : 최소편집거리
// 이 함수 내부에서 print_matrix 함수와 backtrace 함수를 호출함
int min_editdistance( char *str1, char *str2);

////////////////////////////////////////////////////////////////////////////////
// 세 정수 중에서 가장 작은 값을 리턴한다.
static int __GetMin3( int a, int b, int c)
{
	int min = a;
	if (b < min)
		min = b;
	if(c < min)
		min = c;
	return min;
}

////////////////////////////////////////////////////////////////////////////////
// 네 정수 중에서 가장 작은 값을 리턴한다.
static int __GetMin4( int a, int b, int c, int d)
{
	int min = __GetMin3( a, b, c);
	return (min > d) ? d : min;
}

////////////////////////////////////////////////////////////////////////////////
// 정렬된 문자쌍들을 출력
void print_alignment( char align_str[][8], int level)
{
	int i;
	
	for (i = level; i >= 0; i--)
	{
		printf( "%s\n", align_str[i]);
	}
}

////////////////////////////////////////////////////////////////////////////////
// backtrace_main을 호출하는 wrapper 함수
// str1 : 문자열 1
// str2 : 문자열 2
// n : 문자열 1의 길이
// m : 문자열 2의 길이

void backtrace( int *op_matrix, int col_size, char *str1, char *str2, int n, int m)
{
	char align_str[n+m][8]; // n+m strings
	
	backtrace_main( op_matrix, col_size, str1, str2, strlen(str1), strlen(str2), 0, align_str);
	
}


////////////////////////////////////////////////////////////////////////////////
int main()
{
	char str1[30];
	char str2[30];
	
	int distance;
	
	fprintf( stderr, "INSERT_COST = %d\n", INSERT_COST);
	fprintf( stderr, "DELETE_COST = %d\n", DELETE_COST);
	fprintf( stderr, "SUBSTITUTE_COST = %d\n", SUBSTITUTE_COST);
	fprintf( stderr, "TRANSPOSE_COST = %d\n", TRANSPOSE_COST);
	
	while( fscanf( stdin, "%s\t%s", str1, str2) != EOF)
	{
		printf( "\n==============================\n");
		printf( "%s vs. %s\n", str1, str2);
		printf( "==============================\n");
		
		distance = min_editdistance( str1, str2);
		
		printf( "\nMinEdit(%s, %s) = %d\n", str1, str2, distance);
	}
	return 0;
}




static void backtrace_main( int *op_matrix, int col_size, char *str1, char *str2, int n, int m, int level, char align_str[][8])
{

	
	
	static int count = 1; //static int로 선언해야 한다
	if(level==0) count =1;

	if ( (n == 0) && (m == 0)) {
		
		printf("\n[%d] ==============================\n", count++);
		print_alignment(align_str, level-1);
		return;
	}
	

	int op = op_matrix[n * col_size + m];
	

	if( (op & MATCH_OP) || (op & SUBSTITUTE_OP)){
		
		align_str[level][0] = str1[n-1];
	    align_str[level][1] = ' ';
	    align_str[level][2] = '-';
	    align_str[level][3] = ' ';
	    align_str[level][4] = str2[m-1];
	    align_str[level][5] = '\0';
	    backtrace_main(op_matrix, col_size, str1, str2, n - 1, m - 1, level+1, align_str);
	}


	if(op & DELETE_OP){

		align_str[level][0] = str1[n-1];
	    align_str[level][1] = ' ';
	    align_str[level][2] = '-';
	    align_str[level][3] = ' ';
	    align_str[level][4] = '*';
	    align_str[level][5] = '\0';
		backtrace_main(op_matrix, col_size, str1, str2, n-1 , m, level+1, align_str);
	}


	if(op & INSERT_OP){

		align_str[level][0] = '*';
	    align_str[level][1] = ' ';
	    align_str[level][2] = '-';
	    align_str[level][3] = ' ';
	    align_str[level][4] = str2[m-1];
	    align_str[level][5] = '\0';
		
		backtrace_main(op_matrix, col_size, str1, str2, n, m-1 , level+1, align_str);

	}

	

	if(op & TRANSPOSE_OP){

		align_str[level][0] = str1[n - 2];
	    align_str[level][1] = str1[n-1];
	    align_str[level][2] = ' ';
	    align_str[level][3] = '-';
	    align_str[level][4] = ' ';
	    align_str[level][5] = str2[m - 2];
	    align_str[level][6] = str2[m-1];
	    align_str[level][7] = '\0';
		backtrace_main(op_matrix, col_size, str1, str2, n - 2, m - 2, level+1, align_str);
	}


	return;

}
// 강의 자료의 형식대로 op_matrix를 출력 (좌하단(1,1) -> 우상단(n, m))
// 각 연산자를 다음과 같은 기호로 표시한다. 삽입:I, 삭제:D, 교체:S, 일치:M, 전위:T
//op(i*colsize+j)
void print_matrix( int *op_matrix, int col_size, char *str1, char *str2, int n, int m)
{

	for(int i = n; i > 0 ; --i){
		printf("%c\t", str1[i-1]);
        for(int j = 1; j < m + 1; ++j){
            if(op_matrix[i * col_size + j] & SUBSTITUTE_OP) printf("S");
            if(op_matrix[i * col_size + j] & MATCH_OP) printf("M");
            if(op_matrix[i * col_size + j] & INSERT_OP) printf("I");
            if(op_matrix[i * col_size + j] & DELETE_OP) printf("D");
            if(op_matrix[i * col_size + j] & TRANSPOSE_OP) printf("T");
            printf("\t");
        }
        printf("\n");
    }



    printf("\t");
    for(int i=0; i<m; i++){
    	printf("%c\t", str2[i]);
    }
    printf("\n");
}

// 두 문자열 str1과 str2의 최소편집거리를 계산한다.
// return value : 최소편집거리
// 이 함수 내부에서 print_matrix 함수와 backtrace 함수를 호출함
int min_editdistance( char *str1, char *str2){


	int n = strlen(str1);
	int m = strlen(str2);
 	int col_size = m+1;


	
	int d[n + 1][m + 1]; 
	int op_matrix[(n+1)*(m+1)]; 
	memset(d, 0, sizeof(d));
	memset(op_matrix, 0, sizeof(op_matrix));

	for (int i = 0; i <= n;i++) {
		d[i][0] = i;
		op_matrix[i*(m+1)]= DELETE_OP;
	}

	for (int j = 0; j <= m; j++) {
		d[0][j] = j;
		op_matrix[j] = INSERT_OP;
		
	}

	op_matrix[0] = 0;
	//실수1 : 연산자도 초기화 해주어야 한다는 것을...
	//실수2 : 그리고 0,0 부분은 0으로 set 해줘야 한다는 것을 ...




    for(int i=1; i< n+1; i++){
    	for(int j=1; j< m+1; j++){

    		//실수3: 계속 valgrind  오류 나길래 왜그런가 했는데 (i>1) && (j>1) 넣어줘야 하는 거였음ㅠㅠ 
    		//transpose 하려면 글자 두개를 비교해야 하는데 i가 1인 경우엔 -1까지 비교하게되니까...

    		//두 문자의 Transpose 혹은 Match가 가능한 경우 *글자 두개를 비교
    		if( (i>1) && (j>1) && (str1[i-1]==str2[j-2]) && (str1[i-2]==str2[j-1]) ){

    			if( (str1[i-1]==str2[j-1]) && (str1[i-2]==str2[j-1])){ //MATCH가 가능한 경우

    				d[i][j] = __GetMin4(d[i][j-1]+INSERT_COST, d[i-1][j]+DELETE_COST, d[i-1][j-1]+ 0 , d[i-2][j-2]+TRANSPOSE_COST);
    			
					if (d[i][j] == d[i][j - 1] + INSERT_COST) op_matrix[i * (m + 1) + j] |= INSERT_OP;
					if (d[i][j] == d[i - 1][j] + DELETE_COST) op_matrix[i * (m + 1) + j] |= DELETE_OP;
					if (d[i][j] == d[i - 2][j - 2] + TRANSPOSE_COST) op_matrix[i * (m + 1) + j] |= TRANSPOSE_OP;
					if (d[i][j] == d[i - 1][j - 1] + 0 ) op_matrix[i * (m + 1) + j] |= MATCH_OP;

    			}
    			else{

    				d[i][j] = __GetMin4(d[i][j-1]+INSERT_COST, d[i-1][j]+DELETE_COST, d[i-1][j-1]+SUBSTITUTE_COST, d[i-2][j-2]+TRANSPOSE_COST);
    			
					if (d[i][j] == d[i][j - 1] + INSERT_COST) op_matrix[i * (m + 1) + j] |= INSERT_OP;
					if (d[i][j] == d[i - 1][j] + DELETE_COST) op_matrix[i * (m + 1) + j] |= DELETE_OP;
					if (d[i][j] == d[i - 2][j - 2] + TRANSPOSE_COST) op_matrix[i * (m + 1) + j] |= TRANSPOSE_OP;
					if (d[i][j] == d[i - 1][j - 1] + SUBSTITUTE_COST) op_matrix[i * (m + 1) + j] |= SUBSTITUTE_OP;
    			}
    		}

    		
    		else{


    			if(str1[i-1] == str2[j-1]){ //문자열 비교할 때는 하나씩 빼서한다 -> 같은 경우 transpose의 비용이 0

					d[i][j] = __GetMin3(d[i][j-1]+INSERT_COST, d[i-1][j]+DELETE_COST, d[i-1][j-1]+ 0 );
		    		if (d[i][j] == d[i][j - 1] + INSERT_COST) op_matrix[i * (m + 1) + j] |= INSERT_OP;
					if (d[i][j] == d[i - 1][j] + DELETE_COST) op_matrix[i * (m + 1) + j] |= DELETE_OP;
					if (d[i][j] == d[i - 1][j - 1] + 0 ) op_matrix[i * (m + 1) + j] += MATCH_OP;
				}
	    		else{

	    			d[i][j] = __GetMin3(d[i][j-1]+INSERT_COST, d[i-1][j]+DELETE_COST, d[i-1][j-1]+ SUBSTITUTE_COST);
		    		if (d[i][j] == d[i][j-1] + INSERT_COST) op_matrix[i * (m + 1) + j] |= INSERT_OP;
					if (d[i][j] == d[i - 1][j] + DELETE_COST) op_matrix[i * (m + 1) + j] |= DELETE_OP;
					if (d[i][j] == d[i - 1][j - 1] + SUBSTITUTE_COST) op_matrix[i * (m + 1) + j] |= SUBSTITUTE_OP;
	    		}

	    	}


    	}
    	
    }

    print_matrix(op_matrix, col_size, str1, str2, n, m);
    backtrace(op_matrix, col_size, str1, str2, n, m);

    return d[n][m];
}

