#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define MAXSIZE 1000
#define N 100

int queue[MAXSIZE][3];
int DomainInValues[N];

int front = -1;
int rear = -1;
int size = -1;

int isempty();
void enqueue(int v1, int v2, int v3);
int dequeue();
int Front_1();
int Front_2();
int Front_3();
int ConstraintToNumber(char c);
bool CheckIfVariableExists(int a[],int b);
bool CheckIfSymbolExists(char b);
int CheckEmptyDomain(int x);
bool RestrictionCheck(int a1, int a2, int domain[N][N], int j, int s, int u2);
bool arc_reduce(int u1, int u2, int u3, int d[N][N]);
int AC3(int v[], int d[][N], int b[][3]);

int isempty(){
	
    return size==0;
}

void enqueue(int v1, int v2, int v3){
	// From left to right, left variable, constraint, right variable
    if(size<MAXSIZE)
    {
        if(size<0)
        {
            queue[0][0] = v1;
            queue[0][1] = v2;
            queue[0][2] = v3;
            front = rear = 0;
            size = 1;
        }
        else if(rear == MAXSIZE-1)
        {
            queue[0][0] = v1;
            queue[0][1] = v2;
            queue[0][2] = v3;
            rear = 0;
            size++;
        }
        else
        {
            queue[rear+1][0] = v1;
            queue[rear+1][1] = v2;
            queue[rear+1][2] = v3;
            rear++;
            size++;
        }
    }
    else
    {
        printf("Queue is full\n");
    }
}

int dequeue(){
	
    if(size==0)
    {
        printf("Queue is empty\n");
    }
    else
    {
        size--;
        front++;
    }
}

int Front_1(){
	
    return queue[front][0];
}

int Front_2(){
	
    return queue[front][1];
}

int Front_3(){
	
    return queue[front][2];
}

int ConstraintToNumber(char c){
	// > --> 1, < --> 2, = -->3 , / --> 4
	if(c=='>')
		return 1;
	else if(c=='<')
		return 2;
	else if(c=='=')
		return 3;
	else
		return 4;
}

bool CheckIfVariableExists(int a[],int b){
	
	int i=0;
	
	while(a[i]!=-1){
		
		if(b == a[i])
			return true;
		
		i++;
	}
	
	return false;
}

bool CheckIfSymbolExists(char b){
	
	int i=0;
	char symbol[]={'>','<','=','/'};
	
	while(symbol[i]!='\0'){
		
		if(b == symbol[i])
			return true;
		
		i++;
	}
	
	return false;
}

int CheckEmptyDomain(int x){
	
	return DomainInValues[x];
}

bool RestrictionCheck(int a1, int a2, int d[][N], int j, int s, int u2){
	
	if(u2==1)
		return (d[a1][j]<d[a2][s] || d[a1][j]==d[a2][s]);
	else if(u2==2)
		return (d[a1][j]>d[a2][s] || d[a1][j]==d[a2][s]);
	else if(u2==3)
		return (d[a1][j]!=d[a2][s]);
	else
		return (d[a1][j]==d[a2][s]);
	
}

bool arc_reduce(int u1, int u2, int u3, int d[][N]){

	int j,s,counter,k;	
	bool change=false;
	bool a;
	
	j=0;
	
	// Empty Value --> -1
	while(d[u1][j]!=-1){
			
		if(d[u1][j]==-2){
			j++;
			continue;
		}
		
		k=0;			
		s=0;
		counter=0;
		
		while(d[u3][s]!=-1){
			
			if(d[u3][s]==-2){
				counter++;
				s++;
				continue;
			}
			
			a=RestrictionCheck(u1,u3,d,j,s,u2);
			
			if(a==true){
				counter++;
			}
			else{
				k++; 
				s++;
				break;
			}
			
			s++;
		}
		
		// Deleted Value --> -2
		if(counter==s && k==0){
			printf("Value %d of variable %d's domain has been deleted\n",d[u1][j],u1);
			d[u1][j]=-2;
			change=true;
			
			if(DomainInValues[u1]!=0)
				DomainInValues[u1]--;
		}
		
		j++;
	}
	
	return change;
}

int AC3(int v[], int d[][N], int b[][3]){
	
	int i,j,n,u1,u2,u3;
	bool a;
	
	i=0;
	while(isempty()==0){
		
		u1=Front_1();
		u2=Front_2();
		u3=Front_3();
		dequeue();

		a=arc_reduce(u1,u2,u3,d);
			
		if(a==true){
			if(CheckEmptyDomain(u1)==0)
				return 1;
			else{
				n=0;
				while(b[n][0]!=-1){
					if(b[n][0]==v[u1] || b[n][2]==v[u1]){
						enqueue(b[n][0],b[n][1],b[n][2]);
					}
			
					n++;
				}
			}	
		}			
	}
	
	return 0;
}

int main(){
	
	int i,j,varnumber,rangemax, variables[N], domain[N][N], constraints[N][3], a1, a2; 
	char c, answer;
	bool repeat=true, check;
	
	// Initializing the arrays
	for(i=0; i<N; i++){
		
		variables[i]=-1;
		DomainInValues[i]=-1;
		
		for(j=0; j<N; j++){
			domain[i][j]=-1;
		}
	}
	
	// Number of variables
	do{
		printf("How many variables do you want(min 2)? ");
		scanf("%d",&varnumber);
		
	}while(varnumber<2);
	
	// Range of variables
	do{
		printf("Range between 1 and (min 2)? ");
		scanf("%d",&rangemax);
		
	}while(rangemax<2);
		
	for(i=0; i<varnumber; i++){
		
		variables[i]=i;
		DomainInValues[i]=rangemax-1;
		
		for(j=0; j<rangemax; j++){
			domain[i][j]=j+1;
		}
	}
	
	printf("Give the constraints between %d-%d \n",variables[0],variables[varnumber-1]);
	
	i=0;
	j=0;
	
	while(repeat){
		
		do{
			check=false;
			printf("Acceptable choices: >,<,=,/(this one for !=) \n");
			printf("Examples: x>y, x<y, x=y, x/y (x,y variable names) \n");
			fflush(stdin);
			scanf("%d",&a1);
			scanf("%c",&c);
			scanf("%d",&a2);

		if((a1==a2) || (CheckIfVariableExists(variables,a1)==false) || (CheckIfSymbolExists(c)==false) || (CheckIfVariableExists(variables,a2)==false)){	
			check=true;
			printf("Invalid inputs, try again\n");
		}
			
		}while(check==true);
		
		// > --> 1, < --> 2, = -->3 , / --> 4
		int test=ConstraintToNumber(c);
		constraints[j][0] = a1;
		constraints[j][1] = test;
		constraints[j][2] = a2;
		enqueue(a1,test,a2);
		j++;
		
		if(test==1)
			test=2;
		else if(test==2)
			test=1;
		
		constraints[j][0] = a2;
		constraints[j][1] = test;
		constraints[j][2] = a1;
		enqueue(a2,test,a1);
		j++;	
		
		do{
			printf("Do you wish to continue(Y/y or N/n)\n");
			fflush(stdin);
			scanf("%c",&answer);
			
			if(answer=='N' || answer=='n') 
				repeat=false;			
			
			if(answer!='N' && answer!='n' && answer!='Y' && answer!='y')
				printf("Wrong answer, try again \n");
			
		}while(answer!='N' && answer!='n' && answer!='Y' && answer!='y');

	}
	
	for(i=0; i<varnumber; i++){
		
		printf("\nVariable: %d\nDomain \n",variables[i]);
		
		for(j=0; j<rangemax+1; j++){
			
			if(domain[i][j]!=-1 && domain[i][j]!=-2)
				printf("%d ",domain[i][j]);
		}
		printf("\n");
	}
	
	// AC3 call
	int ac_return;
	printf("Initializing AC3...\n");	
	ac_return = AC3(variables,domain,constraints);	
	
	if(ac_return==1)
		printf("Domain of a variable was empty, AC3 stopped \n");
	else{
		printf("AC3 finished\n");
		for(i=0; i<varnumber; i++){
		
			printf("\nVariable: %d\nDomain \n",variables[i]);
			
			for(j=0; j<rangemax+1; j++){
				
				if(domain[i][j]!=-1 && domain[i][j]!=-2)
					printf("%d ",domain[i][j]);
			}
			printf("\n");
		}
	}
	

	return 0;
}