#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char name[50];
    int  total_number_of_guesses;
} Player;

char magic_number[5]; 
int  total_number_of_guesses = 0; 
Player players[200];
int num_players = 0;

// Function to generate a random magic number
void generate_magic_number() {
    srand(time(NULL)); // Seed the random number generator with the current time
    for (int i = 0; i < 4; i++) {
        magic_number[i] = '0' + rand() % 10; 
    }
    magic_number[4] = '\0'; 
}

// Function to add a player with their name and total guesses to the players array
void add_player(const char *name) {
    if (num_players < 200) {
        strcpy(players[num_players].name, name);
        players[num_players]. total_number_of_guesses =  total_number_of_guesses;

        //Sort the array of players based on the total number of guesses
        int i = num_players - 1;
        while (i >= 0 && players[i]. total_number_of_guesses > players[i+1]. total_number_of_guesses) {
            Player temp = players[i];
            players[i] = players[i+1];
            players[i+1] = temp;
            i--;
        }

        num_players++;
    }
}

// Function to save player data to a file
void save_player_data() {
    FILE *file = fopen("ratio_player_data.txt", "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Sort players by total guesses
    for (int i = 0; i < num_players - 1; i++) {
        for (int j = 0; j < num_players - i - 1; j++) {
            if (players[j]. total_number_of_guesses > players[j + 1]. total_number_of_guesses) {
                Player temp = players[j];
                players[j] = players[j + 1];
                players[j + 1] = temp;
            }
        }
    }

    fprintf(file, "%d\n", num_players);
    for (int i = 0; i < num_players; i++) {
        fprintf(file, "%s %d\n", players[i].name, players[i]. total_number_of_guesses);
    }

    fclose(file);
}

// Function to load player data from a file
void load_player_data() {
    FILE *file = fopen("ratio_player_data.txt", "r");
    if (file == NULL) {

        printf("No existing data found.\n");
        return;
    }

    fscanf(file, "%d", &num_players);
    for (int i = 0; i < num_players; i++) {

        fscanf(file, "%s %d", players[i].name, &players[i]. total_number_of_guesses);
    }

    fclose(file);
}

// Function to print game statistics
void print_history() {
    float lucky_ratio = 1/(float) total_number_of_guesses; // Calculate the lucky ratio
    printf("Your lucky ratio: %f %% with %d total guesses\n", lucky_ratio*100,  total_number_of_guesses);
    printf("Ranking of top 5 about lucky ratio:\n");
    for (int i = 0; i < 5; i++) {
        float ratio = 1/(float)players[i]. total_number_of_guesses; // Calculate player's lucky ratio
        printf("%d. %s: %0.2f %% with %d guesses.\n", i + 1, players[i].name, ratio*100, players[i]. total_number_of_guesses);
    }
}

// Function to check a guess against the magic number
void check_guess(const char *guess) {
     total_number_of_guesses++;
    for (int i = 0; i < 4; i++) {
        if (guess[i] == magic_number[i]) {
            printf("%c", guess[i]); // Print the digit if it matches
        } else {
            printf("-"); // Print a dash if it doesn't match
        }
    }
    printf("\n");
}

// Main function
int main() {
    printf("|\t MAGIC NUMBER GAME\t|\n");
    generate_magic_number(); // Generate the magic number

    load_player_data(); // Load existing player data from file

    char name[50];
    char play_again;
    char showhstr;
    do {
        printf("Input your name: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = '\0';

        char guess[5];
        printf("Enter the number you guessed (4 digits): ");
        scanf("%s", guess); 

        while (strcmp(guess, "exit") != 0) {
            check_guess(guess); 
            if (strcmp(guess, magic_number) == 0) {
                printf("Well! You've guessed the magic number!\n");
                add_player(name); 
                goto guessed;
            }
            printf("Enter the number you guessed (4 digits): ");
            scanf("%s", guess); 
        }

        guessed:
            print_history(); 
       
         total_number_of_guesses = 0; 
        printf("Do you want to play again? (Y/N): ");
        scanf(" %c", &play_again); 
        getchar(); 

    } while (play_again == 'Y' || play_again == 'y');

    save_player_data(); // Save player data to file

    return 0;
}