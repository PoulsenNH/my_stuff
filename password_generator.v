module main

import os
import time
import rand

fn get_password_data() /* The variables that we would like to return */ (int, string, string) {
    // The length
    println('How long do you want your password to be?')
    lg_input := os.input('') 
    lg := lg_input.int()
    println('Password length set to: $lg')

    // Special Characters
    println('Do you want your password to have any special characters? (y/n)')
    mut sc := os.input('')
    sc = sc.to_lower()
    
    // Variable updates
    if sc.contains('y') {
        sc = 'True'
    } else {
        sc = 'False'
    }

    time.sleep(1000 * time.millisecond)

    // Upper-case letters
    println('Do you want your password to have any upper-case letters? (y/n)')
    mut ul := os.input('')
    ul = ul.to_lower()

    if ul.contains('y') {
        ul = 'True'
    } else {
        ul = 'False'
    }

    // Tells the user what's happening
    println('--- Settings Confirmed ---')
    println('Length: $lg')
    println('Special chars: $sc')
    println('Upper-case: $ul')

    // Returns all of the variables
    return lg, sc, ul
}

fn generate_password(lg int, sc string, ul string) {
	mut ps := "" // The Password
	mut rn := 0 // Random Number
	ab := "abcdefghijklmnopqrstuvwxyz" // The Alphabet
	syb := "!@#$%^&*()_+" // The Symbols
	for _ in 0 .. lg {
		if ps.len > 25 { rn = rand.int_in_range(0, 26) or { 0 } } else {
			rn = rand.int_in_range(0, ps.len) or { 0 }
		} // To prevent crashes
		ps = ps + ab[rn].ascii_str() // Gets a random letter of the alphabet, and assigns it to the password variable
	}
	if ul  == "True" {
		for _ in 0.. lg / 10 {
			rn = rand.int_in_range(0, ps.len) or { 0 }
			upper_char := ps[rn].ascii_str().to_upper() // Converts the selected letter to upper-case
			ps = ps[0..rn] + upper_char + ps[rn + 1..] // Re-constructs the entire string around the new upper-case letter
		} // Converts 10% of the letters to upper-case
	}
	if sc == "True" {
		for _ in 0.. lg / 5 {
			rn = rand.int_in_range(0, ps.len) or { 0 }
			syb_li := rand.int_in_range(0, syb.len) or { 0 } 
			special_char := syb[syb_li].ascii_str()
			ps = ps[0..rn] + special_char + ps[rn + 1..]
		}	
	}
	println(ps)
}	

fn main() {
    println('Welcome to the password generator!!')
    time.sleep(1000 * time.millisecond)
    
    lg, sc, ul := get_password_data() // Capture the returned value into the variable
    generate_password(lg, sc, ul)
}
