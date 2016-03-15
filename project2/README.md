Douglas Keller

dwk24@zips.uakron.edu

Operating Systems

Project 2

15 March 2016


What was done:

	Basic file reading & execution
	
		- Interrupt 2: Read data from a sector into a char array buffer
		- Interrupt 3: Search for a given filename and read it into a buffer
			* Print 'File not found' if the file does not exist
			* Supports files that take up multiple sectors of storage
		- Interrupt 4: Load a program into memory and execute it
		- Interrupt 5: Create a stop function for halting program execution
		- Interrupt 15: Create an error function for displaying error messages
			* File Not Found error
			* General error

How to verify:

	- Compile the OS by running compileOS.sh
	- Launch floppya.img in an a virtual machine (Bochs, VirtualBox, etc)
	- Each step demonstrates one of the tasks listed above
