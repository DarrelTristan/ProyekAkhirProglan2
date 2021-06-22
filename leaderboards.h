#include <stdio.h>  
#include <stdlib.h>
#include <string.h>

struct node{  
    int data;  
    char s[10];
    float time;
    struct node *next;  
};      
   
struct node *head, *tail = NULL;  
 
void addNode(char s[10], int data, float time) {  
    //Create a new node  
    struct node *newNode = (struct node*)malloc(sizeof(struct node));  
    newNode->data = data;  
    strcpy(newNode->s,s);
    newNode->time = time;
    newNode->next = NULL;  
      
    //Checks if the list is empty  
    if(head == NULL) {  
        head = newNode;  
        tail = newNode;  
    }  
    else {  
        tail->next = newNode;  
        tail = newNode;  
    }  
}  
   
void sortList() {  
    //Node current will point to head  
    struct node *current = head, *index = NULL;  
    int temp;
	float temp1;
    char temp2[100];
          
    if(head == NULL) {  
        return;  
    }  
    else {  
        while(current != NULL) {  
                  
            index = current->next;  
                
            while(index != NULL) {  
                    
                if(current->data < index->data) {  
                    temp = current->data;  
                    current->data = index->data;  
                    index->data = temp;  
                    
                    temp1 = current->time;  
                    current->time = index->time;  
                    index->time = temp1; 
                        
                    strcpy(temp2,current->s);
            		strcpy(current->s,index->s);
            		strcpy(index->s,temp2);
                }
				else if(current->data == index->data && strcmpi(current->s,index->s) > 0) {  
                    temp = current->data;  
                    current->data = index->data;  
                    index->data = temp;
					
					temp1 = current->time;
					current->time = index->time;
					index->time = temp1;  
                
                    strcpy(temp2,current->s);
            		strcpy(current->s,index->s);
            		strcpy(index->s,temp2);
                }  
                index = index->next;  
            }  
            current = current->next;  
        }      
    }  
}  

void openf(){
 	FILE *fptr;
 	int nomor, score; 
 	int i;
	float waktu;
	char nama[10];
	fptr = fopen("ScoreText.txt","r");
	if(fptr == NULL){
		fclose(fptr);
	 	fopen("ScoreText.txt","w");
	 	fclose(fptr);
	}
	else {
		int c = getc(fptr);
		while (c != EOF) {	
   			fscanf(fptr, "%s %d %f\n", nama, &score, &waktu);
			addNode(nama, score, waktu);
			c = getc(fptr);	
		}
		
		if (feof(fptr))
			printf("\n Reached to the end of file.");
		else
			printf("\n Failure.");
		fclose(fptr);
	}
 }

void write (FILE *fptr){
	fptr = fopen ("ScoreText.txt", "w");    
    struct node *current = head;  
    if(head == NULL) {  
        printf("Tidak ada list \n");  
        return;  
    }
    while(current != NULL) { 
		fprintf(fptr, "- %10s\t %d\t %.2f\n", current->s, current->data, current->time);
        current = current->next;  
    } 
	fclose(fptr);   
} 

void display (FILE *fptr){

	struct node *current = head;

	printf ("%s\t%s\t%s\n", "NAME", "SCORE", "TIME");
	printf ("======================\n");

	while(current != NULL) {
		printf("%s\t", current->s);
    	printf("%d\t", current->data);
		printf("%.2f\n", current->time);
		current = current->next;	
	}  
} 
      
void leaderboard (char name[10], float *time, int *globalScore){
	FILE *fptr;  
	
	struct node *current = head;
	head = NULL;
	openf();

	addNode(name, *globalScore, *time);   //linked list

      
    sortList();    				             //Sorting list (descending)
	write(fptr);    			             //Write leaderboard to the text
    printf("\n>>Leaderboard:\n");  
    display(fptr); 	 //Displaying leaderboard from the text 
    free(head);
    getch();
    system("cls");
}  
