#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>	// Used for alarm

char inputChar()
{
    // TODO: rewrite this function
	// Create a random character from space (Starting at ASCII 32, ' ') to punctuation (ASCII 126, ~).
	// Excludes 'command' characters like carriage return or delete.
    return  (rand() % (127 - 33 + 1)) + 32;
}

char *inputString()
{
    // TODO: rewrite this function
	// Testing strings of length 6
	char *string = (char*)malloc(6);
	// Assign a random string of lowercase letters
	int i;
	for (i = 0; i < 5; i++) {
		string[i] = (rand() % (122 - 97 + 1)) + 97;
	}
	string[5] = '\0';	// Ensure we're working with a null-terminated string.
    return string;
}

void testme()
{
	alarm(300);	// Set an alarm to close the program if 5 minutes have passed
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;
  while (1)
  {
    tcCount++;
    c = inputChar();
    s = inputString();
    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
    if (c == ' '&& state == 3) state = 4;
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;
    if (s[0] == 'r' && s[1] == 'e'
       && s[2] == 's' && s[3] == 'e'
       && s[4] == 't' && s[5] == '\0'
       && state == 9)
    {
      printf("error ");
	  free(s);
      exit(200);
    }
	free(s);
  }
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    testme();
    return 0;
}
