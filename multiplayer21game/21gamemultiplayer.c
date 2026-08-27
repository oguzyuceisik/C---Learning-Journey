#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef struct{
    char name[50];
    int bet;
    int deck[50];
    int index;
    int total;
    int status;
}Player;

void shuffle(int deck[]);
void playerDrawCard(int deck[], int playerCards[], int *playerIndex, int *univIndex);
void semiRevealCards(int playerCards[], int playerIndex, char playerName[]);
void fullyRevealCards(int playerCards[], int playerIndex, char playerName[]);
void totalPoints(int playerCards[], int playerIndex, int *total);
void checkStatus(int *total, int playerDeck[], int playerIndex, int *status);
int checkAce(int *total, int playerDeck[], int playerIndex);
void checkWinner(int playertotal, int pctotal, char playername[], int playerindex, int pcindex, int bet);

int main(){
    srand(time(NULL));
    FILE *pFile = fopen("deck.txt", "r");
    if (pFile == NULL){
        printf("The file could not be opened!");
        return 1;
    }

    int howMany = 0;
    int deck[52];
    int univIndex = 0;
    int pcCards[50];
    int pcIndex = 0;
    int pcTotal = 0;
    int pcStatus = 0;
    int allStatus = 0;

    for(int i = 0; i<52; i++){
        fscanf(pFile, "%d", &deck[i]);
    }
    fclose(pFile);

    
    printf("___WELCOME TO THE 21 CARD GAME___\n");
    printf("How many players are there?: ");
    scanf("%d", &howMany);


    Player *list = malloc(howMany * sizeof(Player));

    if(list==NULL){
        printf("Memory allocation failed!\n");
        return 1;
    }
    shuffle(deck);

    for(int i=0; i<howMany; i++){
        printf("Player #%d, please enter your name: ", i+1);
        scanf("%s", list[i].name);
        printf("%s, please place your bet: ", list[i].name);
        scanf("%d", &list[i].bet);
        list[i].index = 0;
        list[i].status = 0;
        list[i].total = 0;
        printf("_________________\n");
    }
    printf("___GAME BEGINS___\n");
    playerDrawCard(deck, pcCards, &pcIndex, &univIndex);
    playerDrawCard(deck, pcCards, &pcIndex, &univIndex);
    for(int j=0; j<howMany; j++){
        playerDrawCard(deck, list[j].deck , &list[j].index, &univIndex);
        playerDrawCard(deck, list[j].deck , &list[j].index, &univIndex);
    }

    while(1){
        printf("_________________\n");
        semiRevealCards(pcCards, pcIndex, "Computer");
        for(int i =0; i<howMany; i++){
            checkStatus(&list[i].total, list[i].deck, list[i].index, &list[i].status);
            fullyRevealCards(list[i].deck, list[i].index, list[i].name);
            totalPoints(list[i].deck, list[i].index, &list[i].total);
        }
        for(int i = 0; i<howMany; i++){
            int userChoice;
            if(list[i].status==0){
                printf("_________________\n");
                printf("%s, choose a move,\n", list[i].name);
                printf("PASS: 0\nHIT: 1\n");
                printf("%s's move: ", list[i].name);
                scanf("%d", &userChoice);
                if(userChoice ==1){
                    playerDrawCard(deck, list[i].deck , &list[i].index, &univIndex);
                    printf("%s drawed %d\n", list[i].name, list[i].deck[list[i].index-1]);
                }else if(userChoice == 0){
                    printf("%s passed!\n", list[i].name);
                    list[i].status = 1;
                    continue;
                }else{
                    printf("Invalid move! %s's turn is over!\n", list[i].name);
                    list[i].status = 1;
                    continue;
                }
            }else{
                continue;
            }
        }
        totalPoints(pcCards, pcIndex, &pcTotal);
        checkStatus(&pcTotal, pcCards, pcIndex, &pcStatus);
        if(pcStatus == 0){
            if(pcTotal<17){
                playerDrawCard(deck, pcCards, &pcIndex, &univIndex);
                printf("_________________\n");
                printf("Computer drawed %d\n", pcCards[pcIndex - 1]);
            }else{
                printf("_________________\n");
                printf("Computer Passed!\n");
                pcStatus = 1;
            }
        }else{
            printf("_________________\n");
            printf("Computer Passed!\n");
        }
        allStatus = 0;
        if(pcStatus == 1) allStatus++;
        for(int i=0; i<howMany; i++){
            allStatus += list[i].status;
        }
        if(allStatus==howMany+1){
            break;
        }
    }
    printf("_________________\n");
    printf("___GAME ENDED!___\n");
    fullyRevealCards(pcCards, pcIndex, "Computer");
    for(int i=0; i<howMany;i++){
        fullyRevealCards(list[i].deck, list[i].index, list[i].name);
    }
    printf("_________________\n");
    for(int i = 0; i<howMany; i++){
        checkWinner(list[i].total, pcTotal, list[i].name, list[i].index, pcIndex, list[i].bet);
    }

    free(list);
    return 0;
}

void shuffle(int deck[]){
    for(int i = 0; i<52; i++){
        int j = (rand() % 52);
        int temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

void playerDrawCard(int deck[], int playerCards[], int *playerIndex, int *univIndex){
    playerCards[*playerIndex] = deck[*univIndex];
    (*playerIndex)++;
    (*univIndex)++;
}

void semiRevealCards(int playerCards[], int playerIndex, char playerName[]){
    printf("%s: ",playerName);
    printf(" ? ");
    for(int i=1; i<playerIndex;i++){
        printf("%d ", playerCards[i]);
    }
    printf("\n");
}

void fullyRevealCards(int playerCards[], int playerIndex, char playerName[]){
    printf("%s: ",playerName);
    for(int i=0; i<playerIndex;i++){
        printf("%d ", playerCards[i]);
    }
    printf("\n");
}

void totalPoints(int playerCards[], int playerIndex, int *total){
    (*total) = 0;
    for(int i=0; i<playerIndex; i++){
        (*total) += playerCards[i];
    }
}

void checkStatus(int *total, int playerDeck[], int playerIndex, int *status){
    if(*total > 21){
        if(checkAce(total, playerDeck, playerIndex)==0){
            *status = 1;
        }
    }else if(*total == 21 && playerIndex ==2){
        *status = 1;
    }
}

int checkAce(int *total, int playerDeck[], int playerIndex){
    for(int i = 0; i<playerIndex; i++){
        if(playerDeck[i]==11){
            playerDeck[i] = 1;
            (*total) -=10;
            return 1;
        }
    }
    return 0;
}

void checkWinner(int playertotal, int pctotal, char playername[], int playerindex, int pcindex, int bet){
    if(pcindex==2 && pctotal==21){
        printf("Computer has BLACKJACK, %s lost %d.\n", playername, bet);
        return;
    }
    
    if(playertotal==21 && playerindex ==2){
        if(pctotal != 21 && pcindex !=2){
            printf("BLACKJACK! %s win %d!\n", playername, bet*3/2);
        }else if(pctotal ==21 && pcindex==2){
            printf("%s and the computer TIED, %d returned.\n",playername , bet);
        }else if(pctotal ==21 && pcindex!=2){
            printf("BLACKJACK! %s win %d!\n", playername, bet*3/2);
        }
        return;    
    }
    
    
    if(playertotal < 22 && pctotal < 22 && playerindex !=2){
        if(playertotal>pctotal){
            printf("%s won %d.\n", playername, bet);
        }else if(pctotal>playertotal){
            printf("Computer won against %s and %s lost %d.\n", playername,playername,bet);
        }else if(pctotal == playertotal){
            printf("%s and the computer TIED, %d returned.\n", playername, bet);
        }
    }else if(playertotal>21 && pctotal<=21){
        printf("%s BUSTED and lost %d!\n", playername, bet);
    }else if(pctotal>21 && playertotal<=21){
        printf("Computer BUSTED, %s won %d.\n", playername, bet);
    }else if(playertotal>21 && pctotal>21){
        printf("%s BUSTED, and lost %d.\n", playername, bet);
    }

}