#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <getopt.h>
#define SIZE 26

// flags for commandline
static int alpha;
static int len;
static int longest;
static char include;

// source: man getopt
static struct option longOptions[] =
	{
		{"alpha", no_argument, &alpha, 1},
		{"len", no_argument, &len, 1},
		{"longest", no_argument, &longest, 1},
		{"include", required_argument, &include, 1},
		{NULL, 0, NULL, 0}};

/**
 * METHOD USED
 * make word frequency vectors of 'given word' and 'dictionary word' in the form of array[26]
 * so 	"abd" 	-> [1,1,0,1,0,0,0......0]
 * and 	"abe" 	-> [1,1,0,0,1,0,0......0]
 * and 	"aabe" 	-> [2,1,0,0,1,0,0......0]
 * then compare the arrays. 
 * If for every letter, the frequency in 'given word' isn't the same or more than in 'dictionary word'
 * then the 'given word' can't form the 'dictionary word'
 * 
 * How make word vector array?
 * 1. initialize an array[26] of 0s
 * 2. iterate through the word (access every letter)
 * 3. increment the [(ASCII value of word) % 97]th index of the array.
 * 4. e.g. if letter is 'b' increment (98 % 97) = 2nd index of array by 1
 * (it is safe to assume only alphabets will be input to the function)
 */
int can_form_word(char word[], char element[], char *includedChar)
{

	// wordVector array
	int wordVector[26] = {0};
	for (int a = 0; word[a] != '\0'; a++)
	{
		wordVector[(int)tolower((int)word[a]) % 97]++;
	}

	// fromVector array
	int elementVector[26] = {0};
	for (int b = 0; element[b] != '\0'; b++)
	{
		elementVector[(int)tolower((int)element[b]) % 97]++;
	}

	// check include
	if (include && *includedChar != NULL)
	{
		if (!(elementVector[(int)tolower((int)*includedChar) % 97] > 0))
		{
			return 0;
		}
	}

	for (int i = 0; i < SIZE; i++)
	{
		if (wordVector[i] < elementVector[i])
		{
			return 0;
		}
	}
	return 1;
}

void incorrect_usage()
{
	fprintf(stderr, "Usage: unjumble [-alpha|-len|-longest] [-include letter] letters [dictionary]");
	exit(1);
}

int main(int argc, char *argv[])
{
	/**
	 * Commandline Arguements
	 * -alpha| -len| -longest
	 * -include a
	 * letters
	 * dictionary
	 */
	char *letters = "";
	char *path = "/usr/share/dict/words";
	// char *path = "dict.txt";
	char *includedChar = NULL;

	// source: man getopt
	int argCount = 0;
	int c;
	while (optind < argc)
	{
		int optionIndex = 0;
		if ((c = getopt_long_only(argc, argv, "", longOptions, &optionIndex)) != -1)
		{
			switch (c)
			{
				// handle exceptions in input
			case 0:
				// handle option arguements
				// automatically handled above this is
				// just to ensure the default doesn't get triggered
				if (longOptions[optionIndex].name == "include")
				{
					if (strlen(optarg) > 1)
					{
						incorrect_usage();
					}

					if (optarg)
					{
						includedChar = optarg;
					}
				}
				break;

			case 1:
				// handle regular arguements
				// automatically handled above this is
				// just to ensure the default doesn't get triggered
				break;

			case '?':
				incorrect_usage();
				break;

			case ':':
				incorrect_usage();
				break;

			default:
				incorrect_usage();
			}
		}

		else
		{
			if (argCount == 0)
			{
				if (argv[optind] == NULL)
				{
					incorrect_usage();
				}
				letters = argv[optind];
			}

			if (argCount == 1)
			{
				if (argv[optind] == NULL)
				{
					incorrect_usage();
				}
				path = argv[optind];
			}

			argCount++;
			optind++;
		}
	}

	// access given file
	char readElement[SIZE];
	FILE *filepath = fopen(path, "r");

	// exit code 2 check
	// check whether filepath is valid
	if (filepath == NULL)
	{
		fprintf(stderr, "unjumble: file %s can not be opened\n", path);
		exit(2);
	}

	// exit code 3 check
	// check whether less than 3 letters
	if (strlen(letters) < 3)
	{
		fprintf(stderr, "unjumble: must supply at least three letters\n");
		exit(3);
	}

	// exit code 4 check
	// check whether less than 3 letters
	for (int i = 0; i < strlen(letters); i++)
	{
		if (isalpha(letters[i]) == 0)
		{
			fprintf(stderr, "unjumble: can only unjumble alphabetic characters\n");
			exit(4);
		}
	}

	int canFormWord = 0;
	while (fgets(readElement, SIZE, filepath) != NULL)
	{

		// element words with length less than 3 letters
		if (strlen(readElement) < 3)
		{
			continue;
		}

		// remove '\n' from end of word if present
		if ((int)readElement[strlen(readElement) - 1] == '\n')
		{
			readElement[strlen(readElement) - 1] = '\0';
		}

		// find if 'word' can form the 'current' dict element
		canFormWord = can_form_word(letters, readElement, includedChar);

		if (canFormWord == 1)
		{
			printf("%s\n", readElement);
		}
	}
	fclose(filepath);
	return 0;
}