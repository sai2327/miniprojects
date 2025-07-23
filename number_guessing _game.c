#include<stdio.h>      // For input and output functions
#include<stdlib.h>     // For rand(), srand()
#include<time.h>       // For time() used in seeding srand()

int main(){
    int SecretNumber;  // Variable to store the randomly generated secret number
    int guess;         // Variable to store user's guess
    int count=0;       // Counter to track number of guesses

    srand(time(NULL)); // Seed the random number generator with current time

    SecretNumber=(rand()%1000)+1; // Generate a random number between 1 and 1000

    printf("Welcome to the Number Guessing Game!\n");
    printf("I have picked a number between 1 and 1000. Can you guess it?\n");

    while(1){ // Infinite loop until the correct guess is made
        count++; // Increment guess count

        printf("Enter your guess:");
        scanf("%d",&guess); // Read user's guess

        if(guess<SecretNumber) 
            printf("Your guess is less than the SecretNumber Try again.\n");
        else if(guess>SecretNumber) 
            printf("Your guess is greater than the SecretNumber Try again.\n");
        else {
            // If guess is correct
            printf("You Won! \n You have guessed the number %d in %d operations.",SecretNumber,count);
            break; // Exit the loop
        }
    }

    return 0; // End of program
}
