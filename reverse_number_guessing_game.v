module main // The start of the execution

import os
import rand
import time

struct AIConfiguration {
mut: // Allows the fields below it to be changed if the struct instance in mutable
	minimum_guess int
	maximum_guess int
	aggressiveness int
	difficulty int
}

struct GameConfiguration {
mut:
	lowest_number int
	highest_number int
}
 // 12'th - 13'th of January
fn recieve_input(question string) int {
	println(question)
	input := os.input('')
	res := input.int()
	println('Settings Updated To: $res')
	return res
}

fn change_ai_configuration_settings(mut cfg AIConfiguration, mut gfg GameConfiguration, wait_time int) {
	println("It is now time to customize your opponent, choose wisely.")
	time.sleep(wait_time * time.second)
	
	cfg.minimum_guess = recieve_input('What is the minimum guess that the AI should be able to make?')
	time.sleep(wait_time * time.second)
	cfg.maximum_guess = recieve_input('What is the maximum guess that the AI should be allowed to make?')
	time.sleep(wait_time * time.second)
	cfg.aggressiveness = recieve_input('How aggressive should the AI be? 1/2/3')
	time.sleep(wait_time *  time.second)
	cfg.difficulty = recieve_input('What should the difficulty level be on the AI? 1/2/3')
	time.sleep(wait_time * time.second)
	println('It is now time to customize your game settings.')
	time.sleep(wait_time * time.second)
	gfg.lowest_number = recieve_input('What should the lowest possible number be?')
	time.sleep(wait_time * time.second)
	gfg.highest_number = recieve_input('What should the highest possible number be?')
	time.sleep(wait_time * time.second)

	println('Enter "Start" when you are ready to start the game.')
	input := os.input('')
	if input.to_lower() == 'start' {
		gameplay_loop(cfg.difficulty, cfg.minimum_guess, cfg.maximum_guess, wait_time) // Starts the game
	}
}

fn gameplay_loop (difficulty int, minimum_guess int, maximum_guess int, wait_time int) {
	random_number := rand.int_in_range(1, 100) or { 0 } // Note Change this later

	// Dynamic Variables
	mut already_checked_numbers := []int{}
	mut lowest_guess := 0
	mut heighest_guess := 0

	mut guess := 0

	mut player_response := 'nothing'
	
	println('Computer: Lets do this')
	time.sleep(wait_time * time.second)
	
	for {
		match difficulty {
		1 {
				guess = rand.int_in_range(minimum_guess, maximum_guess) or { 0 }
				println('I guess that the number is $guess')
				
				println('higher/lower/equal')
				player_response = os.input('')

				match player_response.to_lower() {
					'higher' {
						guess = guess + rand.int_in_range(minimum_guess, maximum_guess) or { 0 }
					}
					'lower' {
						guess = guess - rand.int_in_range(minimum_guess, maximum_guess) or { 0 }
					}
					'equal' {
						println('Hurrah!! I win again!!')
						break
					}
					else {
						println('AAAAAAAAAA')
						break // Shut up compiler
					}
				} 
			}
			else {
				println('This feature has not been added into the game yet.')
				break // Shut up compiler
			}
		} 
	} // Infinite Loop
}

fn main() {

	wait_time := 2

	mut configuration := AIConfiguration {
		minimum_guess: 0
		maximum_guess: 0
		aggressiveness: 0
		difficulty: 0
	}

	mut game_configuration := GameConfiguration {
		lowest_number: 0
		highest_number: 0
	}
		
	println("Welcome to our reverse number guessing game!")
	time.sleep(wait_time * time.second)
	println("You may have played a normal guessing game before..")
	time.sleep(wait_time * time.second)
	println("But have you ever played a reverse guessing game before?")
	time.sleep(wait_time * time.second)
	println("How this works, is you are going to tell the AI higher or lower, and the AI is going to attempt to guess a number of your choosing")
	time.sleep(wait_time * time.second)
	println("This game is very customizable, and so we hope that you enjoiy it!!")
	time.sleep(wait_time * time.second)
	change_ai_configuration_settings(mut configuration, mut game_configuration, wait_time)
}
