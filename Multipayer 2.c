#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <windows.h>
#include <omp.h>
#include <conio.h>

int main()
{
	int refresh = 50;
    int sizey = 23;
    int sizex = 40;
    int x, y, yi;
    char world[sizey][sizex];
    char world2[sizey][sizex];
    char player = 'A';
    char playerLaser = '^';
    char enemy = 'M';
    char enemyShielded = 'O';
    char enemyLaser = 'v';
    char explosion = 'X';
    int score = 0, score2 = 0;
    int victory = 1, victory2 = 1;
    int laserReady = 1;
    int enemyReady = 0;
	double t1, t2;
	int j;

    srand(time(NULL)); //number randomizer

    printf("\n \n \n \n Press any key to start...");
    getch();
	
	
	
	
	t1 = omp_get_wtime();
	
	int totalEnemies = 0;
	//arena
    for (x = 0; x < sizex; x ++) {
        for (y = 0; y < sizey; y ++) {
            if ((y+1) % 2 == 0 && y < 7 && x > 4
            && x < sizex - 5 && x % 2 ==0) {
                world[y][x] = enemy;
                totalEnemies ++;
            }
            else if ((y+1) % 2 == 0 && y >= 7 && y < 9 && x > 4
            && x < sizex - 5 && x % 2 ==0){
                world[y][x] = enemyShielded;
                totalEnemies = totalEnemies + 2;
            }
            else {
                world[y][x] = ' ';
            }
        }
    }
    world[sizey - 1][sizex / 2] = player;
    int i = 1;
    char direction = 'l';
    int currentEnemies = totalEnemies;
    //copy world
    #pragma omp for
    
    for (j = 0; j < sizey; j++){
    	strcpy(world2[j], world[j]);
	}
	

    
    
	#pragma omp parallel
	{	
		
		#pragma omp master
		{
			
			do{
				char keyPress;
				i++;
				if(kbhit()){
				keyPress = tolower(getch());
				#pragma omp task shared (world, world2, keyPress)
				{
				
		        	if (keyPress == 'a') {
		        		#pragma omp critical
		        		{
		        			for (x = 0; x < sizex; x = x+1) {
				                if ( world[sizey-1][x+1] == player) {
				                    world[sizey-1][x] = player;
				                    world[sizey-1][x+1] = ' ';
				                }
				            }
						}
			            
			        }
				
		        	if (keyPress == 'd') {
		        		#pragma omp critical
		        		{
			        		for (x = sizex - 1; x > 0; x = x-1) {
				                if ( world[sizey-1][x-1] == player) {
				                    world[sizey-1][x] = player;
				                    world[sizey-1][x-1] = ' ';
				                }
				            }	
						}
			            
			        }
				
				if (keyPress == 'w'){
					#pragma omp critical
					{
						if (laserReady > 2) {
				            for (x = 0; x < sizex; x = x+1) {
				                if ( world[sizey-1][x] == player) {
				                    world[sizey - 2][x] = playerLaser;
				                    laserReady = 0;
				                }
				            }
				        }
					}
				}
				
				
					if (keyPress == 'j') {
						#pragma omp critical
						{
							for (x = 0; x < sizex; x = x+1) {
				                if ( world2[sizey-1][x+1] == player) {
				                    world2[sizey-1][x] = player;
				                    world2[sizey-1][x+1] = ' ';
				                }
				            }
						}
			            
			        }
				
		        
			    if (keyPress == 'l') {
			        #pragma omp critical
			        {
				        for (x = sizex - 1; x > 0; x = x-1) {
				            if ( world2[sizey-1][x-1] == player) {
				                world2[sizey-1][x] = player;
				                world2[sizey-1][x-1] = ' ';
				            }
				        }	
					}	
			    }
				
				if (keyPress == 'i')
				{
					#pragma omp critical
					{
						if (laserReady > 2) {
			            	for (x = 0; x < sizex; x = x+1) {
				                if ( world2[sizey-1][x] == player) {
				                    world2[sizey - 2][x] = playerLaser;
				                    laserReady = 0;
				                }
				            }
				        }
					}
				}
				
		}
	} else {
		keyPress = ' ';
	}
			
			
			
			#pragma omp task shared (world, score, victory) 
			{
				int drop = 0;
		        int enemySpeed = 1 + 10 * currentEnemies / totalEnemies;
		        laserReady ++;
		        
				#pragma omp critical
				{
		        /*waktu laser*/
		        //enemy laser down
		        for (x = 0; x < sizex; x ++) {
		            for (y = sizey-1; y >= 0; y --) {
		                if (i%2 == 0 && world[y][x] == enemyLaser){
		                	if(world[y+1][x] != enemy && world[y+1][x] != enemyShielded){
				                world[y+1][x] = enemyLaser;
				                world[y][x] = ' ';
			                } else if (world[y+1][x] == enemy || world[y+1][x] == enemyShielded){
		                    world[y][x] = ' ';
		                	}
						}
		            }
		        }
		    	}
		    	#pragma omp critical
				{
		        for (x = 0; x < sizex; x ++) {
		            for (y = 0; y < sizey; y ++) {
		                if ((i % 5) == 0 && (world[y][x] == enemyShielded
		                || world[y][x] == enemy) && (rand() % 15) > 13
		                && world[y+1][x] != playerLaser) {
		                    for (yi = y+1; yi < sizey; yi ++) {
		                        if (world[yi][x] == enemy
		                        || world[yi][x] == enemyShielded) {
		                            enemyReady = 0;
		                            break;
		                        }
		                        enemyReady = 1;
		                    }
		                    if (enemyReady) {
		                        world[y+1][x] = enemyLaser;
		                    }
		                }
		                if (world[y][x] == playerLaser && world[y-1][x] == enemy) {
		                    world[y][x] = ' ';
		                    world[y-1][x] = explosion;
		                    currentEnemies --;
		                    score = score + 50;
		                }
		                else if (world[y][x] == playerLaser
		                && world[y-1][x] == enemyShielded) {
		                    world[y][x] = ' ';
		                    world[y-1][x] = enemy;
		                    currentEnemies --;
		                    score = score + 50;
		                }
		                else if (world[y][x] == playerLaser
		                && world[y-1][x] == enemyLaser) {
		                    world[y][x] = ' ';
		                }
		                else if (world[y][x] == explosion) {
		                    world[y][x] = ' ';
		                }
		                else if ((i+1) % 2 == 0 && world[y][x] == enemyLaser
		                && world[y+1][x] == player) {
		                    world[y+1][x] = explosion;
		                    world[y][x] = ' ';
		                    victory = 0;
		                }
		                else if (world[y][x] == playerLaser
		                && world[y-1][x] != enemyLaser) {
		                    world[y-1][x] = playerLaser;
							world[y][x] = ' ';
		                }
		            }
		        }
			}
			#pragma omp critical
			{
		         /*refresh arah gerak musuh*/
		        /* fungsi dibawah adalah untuk refresh/gerak kesamping pada sumbu x*/
		        for (y = 0; y < sizey; y ++) {
		            if (world[y][0] == enemy) {
		                direction = 'r';
		                drop = 1;
		                break;
		            }
		            if (world[y][sizex-1] == enemy){
		                direction = 'l';
		                drop = 1;
		                break;
		            }
		        }
			}
			#pragma omp critical
			{
		        /*update board*/
		        /* fungsi dibawah adalah untuk refresh/gerak kebawah jika board sudah mencapai batasan x sehingga tidak menumpuk*/
		        if (i % enemySpeed == 0) {
		            if (direction == 'l') {
		                for (x = 0; x < sizex - 1; x ++) {
		                    for (y = 0; y < sizey; y ++) {
		                        if (drop && (world[y-1][x+1] == enemy
		                            || world[y-1][x+1] == enemyShielded)){
		                            world[y][x] = world[y-1][x+1];
		                            world[y-1][x+1] = ' ';
		                        }
		                        else if (!drop && (world[y][x+1] == enemy
		                            || world[y][x+1] == enemyShielded)) {
		                            world[y][x] = world[y][x+1];
		                            world[y][x+1] = ' ';
		                        }
		                    }
		                }
		            }
		            else {
		                for (x = sizex; x > 0; x --) {
		                    for (y = 0; y < sizey; y ++) {
		                        if (drop && (world[y-1][x-1] == enemy
		                            || world[y-1][x-1] == enemyShielded)) {
		                            world[y][x] = world[y-1][x-1];
		                            world[y-1][x-1] = ' ';
		                        }
		                        else if (!drop && (world[y][x-1] == enemy
		                            || world[y][x-1] == enemyShielded)) {
		                            world[y][x] = world[y][x-1];
		                            world[y][x-1] = ' ';
		                        }
		                    }
		                }
		            }
		            for (x = 0; x < sizex; x ++) {
		                if (world[sizey - 1][x] == enemy) {
		                    victory = 0;
		                }
		            }
		        }
			}
		   		
		    }
		
			
			#pragma omp task shared (world2, score2, victory2) 
			{
				int drop = 0;
		        int enemySpeed = 1 + 10 * currentEnemies / totalEnemies;
		        laserReady ++;
		        
				#pragma omp critical
				{	
		        /*waktu laser*/
		        for (x = 0; x < sizex; x ++) {
		            for (y = sizey-1; y >= 0; y --) {
		                if (i%2 == 0 && world2[y][x] == enemyLaser
		                && (world2[y+1][x] != enemy && world2[y+1][x] != enemyShielded)){
		                world2[y+1][x] = enemyLaser;
		                world2[y][x] = ' ';
		                }
		                else if (i%2 == 0 && world2[y][x] == enemyLaser
		                && (world2[y+1][x] == enemy || world2[y+1][x] == enemyShielded)){
		                    world2[y][x] = ' ';
		                }
		            }
		        }
		        for (x = 0; x < sizex; x ++) {
		            for (y = 0; y < sizey; y ++) {
		                if ((i % 5) == 0 && (world2[y][x] == enemyShielded
		                || world2[y][x] == enemy) && (rand() % 15) > 13
		                && world2[y+1][x] != playerLaser) {
		                    for (yi = y+1; yi < sizey; yi ++) {
		                        if (world2[yi][x] == enemy
		                        || world2[yi][x] == enemyShielded) {
		                            enemyReady = 0;
		                            break;
		                        }
		                        enemyReady = 1;
		                    }
		                    if (enemyReady) {
		                        world2[y+1][x] = enemyLaser;
		                    }
		                }
		                if (world2[y][x] == playerLaser && world2[y-1][x] == enemy) {
		                    world2[y][x] = ' ';
		                    world2[y-1][x] = explosion;
		                    currentEnemies --;
		                    score2 = score2 + 50;
		                }
		                else if (world2[y][x] == playerLaser
		                && world2[y-1][x] == enemyShielded) {
		                    world2[y][x] = ' ';
		                    world2[y-1][x] = enemy;
		                    currentEnemies --;
		                    score2 = score2 + 50;
		                }
		                else if (world2[y][x] == playerLaser
		                && world2[y-1][x] == enemyLaser) {
		                    world2[y][x] = ' ';
		                }
		                else if (world2[y][x] == explosion) {
		                    world2[y][x] = ' ';
		                }
		                else if ((i+1) % 2 == 0 && world2[y][x] == enemyLaser
		                && world2[y+1][x] == player) {
		                    world2[y+1][x] = explosion;
		                    world2[y][x] = ' ';
		                    victory2 = 0;
		                }
		                else if (world2[y][x] == playerLaser
		                && world2[y-1][x] != enemyLaser) {
		                    world2[y-1][x] = playerLaser;
							world2[y][x] = ' ';
		                }
		            }
		        }
			}
			#pragma omp critical
			{
		         /*refresh arah gerak musuh*/
		        /* fungsi dibawah adalah untuk refresh/gerak kesamping pada sumbu x*/
		        for (y = 0; y < sizey; y ++) {
		            if (world2[y][0] == enemy) {
		                direction = 'r';
		                drop = 1;
		                break;
		            }
		            if (world2[y][sizex-1] == enemy){
		                direction = 'l';
		                drop = 1;
		                break;
		            }
		        }
			}
			#pragma omp critical
			{
		        /*update board*/
		        /* fungsi dibawah adalah untuk refresh/gerak kebawah jika board sudah mencapai batasan x sehingga tidak menumpuk*/
		        if (i % enemySpeed == 0) {
		            if (direction == 'l') {
		                for (x = 0; x < sizex - 1; x ++) {
		                    for (y = 0; y < sizey; y ++) {
		                        if (drop && (world2[y-1][x+1] == enemy
		                            || world2[y-1][x+1] == enemyShielded)){
		                            world2[y][x] = world2[y-1][x+1];
		                            world2[y-1][x+1] = ' ';
		                        }
		                        else if (!drop && (world2[y][x+1] == enemy
		                            || world2[y][x+1] == enemyShielded)) {
		                            world2[y][x] = world2[y][x+1];
		                            world2[y][x+1] = ' ';
		                        }
		                    }
		                }
		            }
		            else {
		                for (x = sizex; x > 0; x --) {
		                    for (y = 0; y < sizey; y ++) {
		                        if (drop && (world2[y-1][x-1] == enemy
		                            || world2[y-1][x-1] == enemyShielded)) {
		                            world2[y][x] = world2[y-1][x-1];
		                            world2[y-1][x-1] = ' ';
		                        }
		                        else if (!drop && (world2[y][x-1] == enemy
		                            || world2[y][x-1] == enemyShielded)) {
		                            world2[y][x] = world2[y][x-1];
		                            world2[y][x-1] = ' ';
		                        }
		                    }
		                }
		            }
		            for (x = 0; x < sizex; x ++) {
		                if (world2[sizey - 1][x] == enemy) {
		                    victory2 = 0;
		                }
		            }
		        }
			}
		    }
		
		
	#pragma omp taskwait
	
		    	
			//print area for multiplayer
	system("cls");
    printf("\t\tSCORE:    %d\t\t\t\t SCORE :    %d", score, score2);
    printf("\n");
    for (y = 0; y < sizey; y ++) {
        printf("|");
        for (x = 0; x < sizex; x ++) {
            printf("%c",world[y][x]);
        }
        printf("|");
            for (x = 0; x < sizex; x ++){
            	printf("%c",world2[y][x]);
			}
        printf("| \n");
    }
			Sleep(refresh);
		} while (victory && victory2);
		}
	
	}
	/*
	if(victory && !victory2)printf("Player 2 WIN");
	else if (victory2 && !victory) printf("Player 1 WIN");
	*/
			
			
	
	
	
	t2 = omp_get_wtime();
	printf("\n\nTime = %f", t2-t1);
	return 0;
}
