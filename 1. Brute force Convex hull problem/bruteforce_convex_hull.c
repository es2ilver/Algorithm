#include <stdlib.h> // atoi, rand, malloc, realloc
#include <stdio.h>
#include <time.h> //time

#define RANGE 10000

typedef struct
{
	int x;
	int y;
} t_point;

typedef struct
{
	t_point from;
	t_point to;
} t_line;

////////////////////////////////////////////////////////////////////////////////
void print_header( char *filename)
{
	printf( "#! /usr/bin/env Rscript\n");
	printf( "png(\"%s\", width=700, height=700)\n", filename);
	
	printf( "plot(1:%d, 1:%d, type=\"n\")\n", RANGE, RANGE);
}
////////////////////////////////////////////////////////////////////////////////
void print_footer( void)
{
	printf( "dev.off()\n");
}

////////////////////////////////////////////////////////////////////////////////




int compare(int x, int y){
	if (x > y)
		return 1;
	else return -1;
	return 0;
}

/*
#points
points(2247,7459)
points(616,2904)
points(5976,6539)
points(1246,8191)
*/
void print_points( t_point *points, int num_point);

/*
#line segments
segments(7107,2909,7107,2909)
segments(43,8,5,38)
segments(43,8,329,2)
segments(5047,8014,5047,8014)
*/
void print_line_segments( t_line *lines, int num_line);



// [input] points : set of points
// [input] num_point : number of points
// [output] num_line : number of line segments that forms the convex hull
// return value : set of line segments that forms the convex hull
t_line *convex_hull( t_point *points, int num_point, int *num_line);



////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	int x, y;
	int num_point; // number of points
	int num_line=0; // number of lines
	
	if (argc != 2)
	{
		printf( "%s number_of_points\n", argv[0]);
		return 0;
	}

	num_point = atoi( argv[1]); //점의 개수 부분을 실제 숫자로 바꾸기
	if (num_point <= 0)
	{
		printf( "The number of points should be a positive integer!\n");
		return 0;
	}



	t_point *points = (t_point *) malloc( num_point * sizeof( t_point));
		
	t_line *lines;




	// making n points
	srand( time(NULL));
	for (int i = 0; i < num_point; i++)
	{
		x = rand() % RANGE + 1; // 1 ~ RANGE random number
		y = rand() % RANGE + 1;
		
		points[i].x = x;
		points[i].y = y;
 	}
 	fprintf( stderr, "%d points created!\n", num_point);

	

	print_header( "convex.png"); //헤더부분 출력
	
	print_points( points, num_point);
	

	lines = convex_hull( points, num_point, &num_line); //convex hull 계산


	fprintf( stderr, "%d lines created!\n", num_line);

	print_line_segments( lines, num_line);
	
	print_footer();
	


	free( points);
	free( lines);
	
	return 0;
}



void print_points( t_point *points, int num_point){

	int i = 0;
	printf("#points\n");
	for(i=0; i<num_point; i++){

		printf("points(%d,%d)", points[i].x, points[i].y);
		printf("\n");
	}
	printf("\n");
	

}


void print_line_segments( t_line *lines, int num_line){

	int i = 0;
	printf("#line segments\n");
	for(i=0; i<num_line; i++){

		printf("segments(%d,%d,%d,%d)", lines[i].from.x, lines[i].from.y, lines[i].to.x, lines[i].to.y);
		printf("\n");
	}
	printf("\n");


}



t_line *convex_hull( t_point *points, int num_point, int *num_line){


	t_line *lines = (t_line *) malloc( num_point * sizeof( t_line)); //일단 점 개수만큼 할당 .. ;;
	int i, j, k, l=0;
	double a,b, c;
	int add = 0;
	int first  = 1;
	int check;

	for(i=0; i<num_point; i++){
		for(j=i+1; j<num_point; j++){
			//if(i == j) continue;


			a = points[j].y-points[i].y;
			b = points[i].x-points[j].x;
			c = points[i].x*points[j].y-points[j].x*points[i].y;
			// a = (points[i].y - points[j].y)/(points[i].x - points[j].x);
			// p = points[i].y - a*(points[i].x);

			add=1;
			for(k=0; k <num_point-1; k++){

				if(k==i || k==j) continue;
				if(k+1==i || k+1==j) continue;



				

				if((a*points[k].x + b*points[k].y - c ) * (a*points[k+1].x + b*points[k+1].y - c ) < 0 )
					add=0;

			}

			if(add==1){
				add=0;

				(*num_line)++;
	 			lines[l].from.x = points[i].x;
	 			lines[l].from.y = points[i].y;	
				lines[l].to.x = points[j].x;
				lines[l].to.y = points[j].y;
	 			l++;
	 		}



			
		}



					

		
	}



	

	 return lines;
}


//오답노트
// k로 if문 돌릴떄 k, k+1이 ij랑 같은경우까지 모두 제외해야 함 ㅠㅠ 안하면 수가 엄청 적을때 이상해짐 ㅠㅠ
// 직선 기울기 구하는 과정에서 나누기 쓰면 자꾸 플로팅 에러 발생해서 곱하는 쪽으로 함






// t_line *convex_hull( t_point *points, int num_point, int *num_line){

// 	t_line *lines;

// 	int i, j, k, x, y;
// 	float a, b ;
// 	int first, check, p=0;
// 	int l =0;

// 	for(i=0; i<num_point; i++){
// 		for(j=i+1; j<num_point; j++){

// 			a = (points[i].x - points[j].x)/(points[i].y - points[j].y);
// 			b = points[i].y + a*(-1)*(points[i].x);

			

			


// 			for(k=0; k<num_point; k++){ //비교하는 점들

// 				if(k==i || k==j) continue;

// 				x = points[k].x;
// 				y = points[k].y;




// 				while(first==1){
// 					first = 0 ;
// 					if(a*x+b >= y)
// 						check=1;
// 					else
// 						check=0;
// 						}
					

				
// 				if(check){
// 					if(a*x+b >= y)
// 						continue;
// 					else{

// 						p=1;
// 						break;
// 					}
// 				}
							
// 				else{
// 					if(a*x+b < y)
// 						continue;
// 					else{

// 						p=1;
// 						break;
// 					}
// 				}


// 				if(p==1)
// 					break;

// 				else{
// 					p==0;
// 					num_line++;
// 					lines[l].from.x = points[i].x;
// 					lines[l].from.y = points[i].y;	
// 					lines[l].to.x = points[j].x;
// 					lines[l].to.y = points[j].y;
// 					l++;

// 				}
				
// 				first = 1;	
// 			}




// 		}
// 	}
	





// return lines;

// }