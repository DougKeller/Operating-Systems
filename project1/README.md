Douglas Keller

dwk24@zips.uakron.edu

Operating Systems

Project 1

15 February 2016


What was done:

	Created a basic kernel in C
	
		- Print strings terminated by NULL
		- Read strings up to 80 characters long from the user (including 0x0)
			* Clear the last character if BACKSPACE is typed
			* Do not clear past the first character typed
			* Append 0x0 and return if ENTER is typed
		- Clear the screen and set the background/foreground colors
			* Do nothing if bx or cx are invalid numbers
		- Modify the provided writeInt function
			* Make sure '0' is printed when called with writeInt(0)
		- Read positive integers from the user (including 0)
			* Does not allow the user to enter non-numerical characters
			* Does not protect from out-of-range values (e.g. 70,000)
			* Clear the last character if BACKSPACE is typed
			* Do not clear past the first character typed
			* Return if ENTER is typed
		- Create a handler for interrupt 21 to unify the above functions

How to verify:

	- Compile the OS by running compileOS.sh
	- Launch floppya.img in an a virtual machine (Bochs, VirtualBox, etc)
