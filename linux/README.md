# linux
Each directory has a single C file that I was assigned as homework. Other files may be present for reference, data, etc.
Description of each assignment are as follows:
- a8: calculator.c - Takes in an arithmetic formula (e.g. a + b / c), creates child processes (fork) that each execute an operation, pipes operands to each process in order, which return the final result to parent. 
- a9: mysh.c - A shell that supports any number of commands connected by pipe ( | ) like bash. Can also use '=' between two commands that pipe to each other back and forth.
- a10: gserver.c - Chooses a random word from a given list, and utilizes named pipes to create a hangman game playable by the user.
- a11: htree.c - Given a data file and specified number of threads, create a binary tree of threads, where each leaf thread hashes a single block from the file and returns the value to parent. Each internal node first hashes its block, then concatenates that value with its children's values, and hashes that, and returns that to parent.
- a12: sserver.c - Forks a processes for each client connected, and uses sockets to play a number guessing game with each client.
- a13: mserver.c - Multithreaded messaging server that creates a new thread for each client connected. With sockets, clients can send messages to each other, and view all active users on the server
