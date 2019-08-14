#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// function prototypes
bool validStart(char start[]);
bool isExist(char a, char temp[], int length_of_temp);
void hangmanArt(int level, int mistake);

int main()
{
	// int variable
	int i;
	int highScore, score;
	int level, stage;
	int choice;
	int generator;
	int wordCount;
	int mistake, trueCounter;
	int dupeCounter;
	
	// char variable
	char start[10];
	char word[1002];
	char wordBank[1002][20];
	char guessWord[20];
	char guessLine[40];
	char mistakeChar[10];
	char duplicate[20];
	char input;
	char *wordSearch;
	
	// boolean flag
	bool gameOverFlag = false;
	bool wordFound = true;
	
	// file pointers
	FILE *data = NULL;
	FILE *scoreData = NULL;
	
	do
	{
		// display title screen
		hangmanArt(0, 0);
		printf("H A N G M A N\n");
		gameOverFlag = false;
		score = 0;
		level = 0;		
		
		// access and read word data and store it on word bank array
		data = fopen("data.owen", "rb");
		/*for (i = 0; i < 1002; i++)
		{
			scanf("%s", &word);
			fwrite(word, sizeof(char), sizeof(word), data);
		}*/
		if (data == NULL)
		{
			printf("E R R O R\n");
			printf("File 'data.owen' not found\n");
			return 0;
		}
		for (i = 0; i < 1002; i++)
		{
			fread(word, sizeof(char), sizeof(word), data);
			strcpy(wordBank[i], word);
		}
		fclose(data);
		
		// access and read score data
		scoreData = fopen("score.owen", "rb");
		//fwrite (&highScore, sizeof(highScore), 1, scoreData);
		fread(&highScore, sizeof(highScore), 1, scoreData);
		if (scoreData == NULL)
		{
			highScore = 0;
			printf("Data 'score.owen' not found and will be created after game over\n\n");
		}
		printf("Best Score: %d\n", highScore);
		
		// display menu
		printf("1. Play!\n");
		printf("2. Instruction\n");
		printf("3. Exit\n");
		
		// prompt user choice input
		do
		{
			choice = 0;
			printf("Choice: ");
			scanf("%d", &choice);
			fflush(stdin);
		}
		while ((choice != 1 && choice != 2 && choice != 3));
		
		if (choice == 1 && gameOverFlag == false)
		{
			puts("");
			// prompt user to start game
			do
			{
				strcpy(start, "\n");
				printf("Please type 'Start Game' to begin: ");
				scanf("%[^\n]s", start);
				fflush(stdin);
			}
			while(validStart(start) == false);
			
			do
			{
				system("cls");
				
				level++;
				mistake = 0;
				trueCounter = 0;
				
				// generate random guess word from word bank
				srand(time(NULL));
				generator = rand() % 1002;
				strcpy(guessWord, wordBank[generator]);
				wordCount = strlen(guessWord);
				
				// uppercase guess word
				for (i = 0; i < wordCount; i++)
				{
					guessWord[i] = toupper(guessWord[i]);
				}
				for (i = 0; i < 20; i++)
				{
					guessLine[i] = ' ';
				}
				
				if (level > 0)
				{
					dupeCounter = 0;
					
					// change stage when player reached certain level
					if (level <= 20)
						stage = 1;
					else if (level > 20 && level <= 30)
						stage = 2;
					else if (level > 30)
						stage = 3;
					
					do
					{
						system("cls");
						//printf("%s", guessWord);
						
						// display game interface
						hangmanArt(stage, mistake);
						printf("Score : %5d      | Best Score : %5d\n", score, highScore);
						printf("Level : %5d      | Mistake(s) : ", level);
						
						//display incorrect characters
						for (i = 0; i < mistake; i++)
						{
							printf("%c ", mistakeChar[i]);
						}
						
						// display initial guess word lines
						if (trueCounter == 0)
						{
							for (i = 0; i < wordCount; i++)
							{
								guessLine[i * 2] = '_';
								guessLine[(i * 2) + 1] = ' ';
							}
						}
						printf("\n\n%s", guessLine);
						
						// proceed to next level when word is fully guessed
						if (trueCounter == strlen(guessWord))
						{
							score++;
							
							printf("\n\n");
							printf("C O R R E C T !\n");
							printf("Press 'Enter' to proceed to the next level");
							
							getchar();
							break;
						}
						
						// display special message when player reached certain level
						if (level == 21)
						{
							printf("\n\nYou have reached level 21!");
							printf("\nMistake tolerance have decreased from 6 to 5");
							printf("\n\"You're pretty good\" -Snake");
						}	
						else if (level == 31)
						{
							printf("\n\nYou have reached level 31!");
							printf("\nMistake tolerance have decreased once again from 5 to 4");
							printf("\n\"IMPOSSIBRU!\" -Ayumu Kato");
						}
							
						// prompt user input
						printf("\n\nInput Character : ");
						scanf(" %c", &input);
						fflush(stdin);	
						
						// check for previous input duplicates
						input = toupper(input);					
						if (isExist(input, duplicate, dupeCounter) == true)
						{
							continue;
						}
						else
						{
							duplicate[dupeCounter] = input;
							dupeCounter++;
						}
						
						// check if user character input exist on guess word
						wordFound = false;
						wordSearch = strchr(guessWord, input);
						while (wordSearch != NULL)
						{
							guessLine[(wordSearch - guessWord) * 2] = input;
	    					wordSearch = strchr(wordSearch + 1, input);
	    					trueCounter++;
	    					wordFound = true;
						}
						
						// increment mistake counter if word not found
						if (wordFound == false)
						{
							mistake++;
							mistakeChar[mistake - 1] = input;
						}
						
						// break loop and display game over screen
						if (mistake == 6 || (stage == 2 && mistake == 5) || (stage == 3 && mistake == 4))
							break;
					}
					while (score != -1);
					
					// display game over screen
					if (mistake == 6 || (stage == 2 && mistake == 5) || (stage == 3 && mistake == 4))
					{
						system("cls");
						hangmanArt(stage, mistake);
						gameOverFlag = true;
						
						printf("ANSWER = %s\n", guessWord);
						printf("G A M E  O V E R");
						printf("\n=============================");
						printf("\nFinal Score : %d", score);
						printf("\nPrevious Best Score : %d", highScore);
						
						// access and overwrite new high score on score data
						if (score > highScore)
						{
							printf("\n\nCONGRATULATION!!");
							printf("\nNEW PERSONAL RECORD");
							highScore = score;
							
							scoreData = fopen("score.owen", "wb");
							fwrite(&highScore, sizeof (highScore), 1, scoreData);
							fclose(scoreData);
						}
						else if (score == 0 && scoreData == NULL) // if score is 0 and no score data exist
						{
							scoreData = fopen("score.owen", "wb");
							fwrite(&score, sizeof (score), 1, scoreData);
							fclose(scoreData);
						}
							
						printf("\nPress 'Enter' to continue");
						
						getchar();
						system("cls");
					}
				}
				
				// break loop and return to title screen
				if (gameOverFlag == true)
					break;
			}
			while (score != -1);
			
			system("cls");
		}
		else if (choice == 2)
		{
			system("cls");
			hangmanArt(0, 0);
			
			// display game instructions
			printf("Hangman Instructions");
			printf("\n1. Open Hangman.exe");
			printf("\n2. Choose Menu Option (input number 1/2/3 to proceed)");
			printf("\n	a) Play Option");
			printf("\n		- Type the word \"Start Game\" (including spaces)");
			printf("\n		- Guess the word by inputing a character");
			printf("\n		- For level 1-20, mistake tolerance is set to 6");
			printf("\n		  For level 21-31, mistake tolerance is set to 5");
			printf("\n		  For level 31 keatas, mistake tolerance is set to 4");
			printf("\n		- The game is over when the player reached the mistake tolerance limit");
			printf("\n	b) Instruction Option");
			printf("\n		Shows game instructions");
			printf("\n	c) Exit Option");
			printf("\n		Exit the program");
			printf("\n\nPress 'Enter' to continue");
			
			getchar();
			system("cls");
		}
	}
	while (choice != 3);
	
	// display exit message
	puts("");
	printf("Thank you for playing (^-^)/\n");
	printf("Made by: jowenzero\n");
	
	return 0;
}

// start word validation function
bool validStart(char start[])
{
	if (strcmp(start, "Start Game") == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// word duplicate check function
bool isExist(char a, char temp[], int lengthOfTemp)
{
	if (!isalnum(a) || isdigit(a))
	{
		return true;
	}
	else if (lengthOfTemp == 0)
	{
		return false;
	}
	else
	{
		int i;
		for (i = 0; i < lengthOfTemp; i++)
		{
			if (a == temp[i])
			{
				return true;
			}
		}
	}
	
	return false;
}

// hangman art display function
void hangmanArt(int stage, int mistake)
{
	if (mistake == 0 && stage != 0)
	{
		printf("\n");
		printf("       _____________   \n");
		printf("      |            |   \n");
		printf("                   |   \n");
		printf("                   |   \n");
		printf("                   |   \n");
		printf("                   |   \n");	
		printf("                   |   \n");
		printf("                   |   \n");
		printf("                   |   \n");
		printf("      ================ \n");
		printf("\n");
	}
	else if (mistake == 1)
	{
		printf("\n");
		printf("       _____________   \n");
		printf("      |            |   \n");
		printf("      /\\           |   \n");
		printf("      \\/           |   \n");
		printf("                   |   \n");
		printf("                   |   \n");	
		printf("                   |   \n");
		printf("                   |   \n");
		printf("                   |   \n");
		printf("      ================ \n");
		printf("\n");
	}
	else if (mistake == 2)
	{
		printf("\n");
		printf("       _____________   \n");
		printf("      |            |   \n");
		printf("      /\\           |   \n");
		printf("      \\/           |   \n");
		printf("      ||           |   \n");
		printf("      ||           |   \n");		
		printf("                   |   \n");
		printf("                   |   \n");
		printf("                   |   \n");
		printf("      ================ \n");
		printf("\n");
	}
	else if (stage == 1 && mistake == 3)
	{
		printf("\n");
		printf("       _____________   \n");
		printf("      |            |   \n");
		printf("      /\\           |   \n");
		printf("      \\/           |   \n");
		printf("     /||           |   \n");
		printf("    / ||           |   \n");		
		printf("                   |   \n");
		printf("                   |   \n");
		printf("                   |   \n");
		printf("      ================ \n");
		printf("\n");
	}
	else if ((stage == 1 && mistake == 4) || ((stage == 2 || stage == 3) && mistake == 3))
	{
		printf("\n");
		printf("       _____________   \n");
		printf("      |            |   \n");
		printf("      /\\           |   \n");
		printf("      \\/           |   \n");
		printf("     /||\\          |   \n");
		printf("    / || \\         |   \n");			
		printf("                   |   \n");
		printf("                   |   \n");
		printf("                   |   \n");
		printf("      ================ \n");
		printf("\n");
	}
	else if ((stage == 1 && mistake == 5) || (stage == 2 && mistake == 4))
	{
		printf("\n");
		printf("       _____________   \n");
		printf("      |            |   \n");
		printf("      /\\           |   \n");
		printf("      \\/           |   \n");
		printf("     /||\\          |   \n");
		printf("    / || \\         |   \n");			
		printf("      /            |   \n");
		printf("     /             |   \n");
		printf("                   |   \n");
		printf("      ================ \n");
		printf("\n");
	}
	else if (stage == 0 || (stage == 1 && mistake == 6) || (stage == 2 && mistake == 5) || (stage == 3 && mistake == 4))
	{
		printf("\n");
		printf("       _____________   \n");
		printf("      |            |   \n");
		printf("      /\\           |   \n");
		printf("      \\/           |   \n");
		printf("     /||\\          |   \n");
		printf("    / || \\         |   \n");	
		printf("      /\\           |   \n");
		printf("     /  \\          |   \n");
		printf("                   |   \n");
		printf("      ================ \n");
		printf("\n");
	}
}
