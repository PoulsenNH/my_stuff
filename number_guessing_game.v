module main // The entry point to the program

import os
import rand // For the random number generator
import time // To temporarily stop the programs execution

fn compare_guess_to_answer(user_guess int, random_number int) bool // To declare that the function will return either true or false
{
    if user_guess > random_number
    {
        println('Lower')
    } // If the users' guess is greater than the random number
    else if user_guess < random_number
    {
        println('Higher')
    } // If the users' guess is lower than the random number
    else if user_guess == random_number
    {
        println('You Have Won The Game! The Correct Number Was $random_number')
        return true // To tell the main function when to stop the game-loop
    } // if the users' guess is equal to the random number
    else
    {
         println('An unexpected error has occured enter your guess again')
    }          
    return false // If the game has not been won yet
} // Compares the users' guess to the correct number and returns a value

fn guess(random_number int) bool // To declare that the function will return a boolean
{
    println('Your Guess: ')
    input_text := os.get_line() // Fixed typo and added :
    user_guess := input_text.int() // Turns the users' input into an intiger value

    if user_guess == 0 && input_text != '0'
    {
        println('Your guess must be an intiger value.')
        return false // Changed continue to return as it's not in a loop block here
    } // For if the users' guess is not an intiger value

    return compare_guess_to_answer(user_guess, random_number)
}        


fn main()
{
    println('Welcome to our number guessing game.')

    time.sleep(2 * time.second) // .second meaning that we don't have to use nano-seconds

    println('What is the minimum value that you would like to be the number?')

    min_input := os.get_line() // The minimum that the number can be
    min_rand := min_input.int() // Converts the users' insertion to an intiger value

    println('What is the maximum value that you would like to be the number?')

    max_input := os.get_line() // The maximum that the number can be
    max_rand := max_input.int() // Converts the users' insertion to an intiger value

    time.sleep(2 * time.second) // Waits 2 seconds

    println('You have chosen $min_rand and $max_rand for your values.')

    time.sleep(2 * time.second) // Waits 2 seconds

    // Fixed: Added or { 0 } because rand.int_in_range returns a Result
    random_number := rand.int_in_range(min_rand, max_rand + 1) or { 0 } 

    println('A random number has been chosen')

    time.sleep(2 * time.second) // Waits 2 seconds

    println('It is now time for you to make your guesses, choose wisely')    

    for
    {
        // Fixed: Passed random_number and added if check to break on win
        if guess(random_number)
        {
            break
        }
    } // Runs forever
}
