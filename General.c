#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "General.h"

char* getStrExactLength(const char* msg)
{
	char* str;
	char temp[MAX_STR_LEN];
	getsStrFixSize(temp, MAX_STR_LEN, msg);

	str = getDynStr(temp);
	return str;
}

char* getDynStr(char* str)
{
	char* theStr;

	theStr = strdup(str);

	if (!theStr)
		return NULL;

	return theStr;
}

char* getsStrFixSize(char* buffer, int size, const char* msg)
{
	puts(msg);
	return myGets(buffer, size);
}

char* myGets(char* buffer, int size)
{
	char* ok;
	if (buffer != NULL && size > 0)
	{
		do
		{
			ok = fgets(buffer, size, stdin);
		} while (ok && (strlen(buffer) <= 1) && (iscntrl(buffer[0])));

		if (ok)
		{
			char* back = buffer + strlen(buffer);
			while ((buffer < back) && (iscntrl(*--back)));
			*(back + 1) = '\0';
			return buffer;
		}
		buffer[0] = '\0';
	}
	return NULL;
}

char** splitCharsToWords(char* str, int* pCount, const char* del, size_t* pTotalLength)
{
	char* word;
	int count = 0;
	char* temp = strdup(str);
	*pTotalLength = 0;
	char** wordsArray = NULL;

	word = strtok(temp, del);
	while (word != NULL)
	{
		wordsArray = (char**)safeRealloc(wordsArray, (count + 1) * sizeof(char*));

		if(!wordsArray)
			return 0;

		wordsArray[count] = getDynStr(word);
		count++;
		*pTotalLength += (int)strlen(word);
		word = strtok(NULL, del);
	}
	*pCount = count;
	free(temp);
	return wordsArray;
}

float getPositiveFloat(const char* msg)
{
	float val;

	do {
		puts(msg);
		scanf("%f", &val);
	} while (val < 0);

	return val;
}

int	getPositiveInt(const char* msg)
{
	int val;

	do {
		puts(msg);
		scanf("%d", &val);
	} while (val < 0);

	return val;
}

int	countCharInString(const char* str, char tav)
{
	int count = 0;

	while (*str)
	{
		if (*str == tav)
			count++;
		str++;
	}

	return count;
}

int	checkEmptyString(char* str)
{
	while (*str)
	{
		if (!isspace(*str))
			return 0;
		str++;
	}

	return 1;
}

void* safeRealloc(void* ptr, size_t size)
{
	void* newPtr = realloc(ptr, size);
	if (!newPtr)
	{
		free(ptr);
		return NULL;
	}
	return newPtr;
}

char* myStrDup(const char* str)
{
	size_t len = strlen(str);
	char* newStr = (char*)malloc((len + 1) * sizeof(char));
	if(newStr)
		strcpy(newStr, str);
	return newStr;	
}

int checkAlphaSpaceStr(const char* str)
{
	while (*str)
	{
		if (*str != ' ') //We allow space!
		{
			if (!isalpha(*str))
				return 0;
		}
		str++;
	}
	return 1;
}

void toLowerStr(char* str)
{
	while (*str)
	{
		*str = tolower(*str);
		str++;
	}
}
