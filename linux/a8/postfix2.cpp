#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <thread.h>

using namespace std;

int fds[100][2];
int numPipes = 0;
int vals [100][2];
char ops[100];

void child(int index) {
	dup2(fds[inputIndex1][0], 0);
	dup2(fds[inputIndex2][0], 3);
	dup2(fds[outputIndex][1], 1);

	for(int i=0; i<numPipes; i++) {
		close(fds[i][0]);
		close(fds[i][1]);
	}
	
	switch (ops[index]) {
		case '+':
			read()
			execl("./add", "add", NULL);
			break;
		case '-':
			execl("./subtract", "subtract", NULL);
			break;
		case '*':
			execl("./multiply", "multiply", NULL);
			break;
		case '/':
			execl("./divide", "divide", NULL);
			break;
		default:
			fprintf(stderr, "This is spooky.\n");
			exit(1);
	}

	fprintf(stderr, "This is spooky too.\n");
	exit(2);
}

int main() {
	dup(0); //just to make fd index 3 busy & to avoid complication with the pipes/children
	string line; 
	char token;
	//full documentation @ https://cplusplus.com/reference/stack/stack/
	stack<int> indexStack; 
	vector<int> inputIndices;
	pthread_t threads[100];

	getline(cin, line);
	//count the tokens --> that is # of pipes!
	istringstream istr2(line);
	while (istr2 >> token)
		numPipes++;

	for(int i=0; i<numPipes; i++)
		pipe(fds[i]);

	int index = 0;
	istringstream istr(line);
	while (istr >> token) {
		//token is a, b, c, d, e, ...
		if (isalpha(token)) {
			indexStack.push(index);
			inputIndices.push_back(index);
		}
		else { //token is + - * /
			//pop 2 index values and create child
			vals[index][1] = indexStack.top();
			indexStack.pop();
			vals[index][0] = indexStack.top();
			indexStack.pop();
			ops[index] = token;
			pthread_create(&(threads[index], NULL, child, (void *) index);
			//push current index 
			indexStack.push(index);
		}
		index++;
	}

	//read the data from user and pump to pipes!
	while (1) {
		int value;
		for(int i=0 ; i < inputIndices.size() ; i++)
			if (cin >> value) { //quit if user input fails
				int pipeIndex = inputIndices[i];
				write(fds[pipeIndex][1], &value, sizeof(int));
			} else
				exit(0);
		int result = 0;
		read(fds[numPipes-1][0], &result, sizeof(int));
		cout << result << endl;
		for(int i=0; i<index; i++){
			void *ptr;
			pthread_join(threads[i], &ptr);
		}
	}
}
