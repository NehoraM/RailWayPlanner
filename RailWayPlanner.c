#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/**
 * @author nehora moshe
 * this program find the cheapest price to make rail-way.
 */

#define ADD_TO_ARRAY 10
#define ARGUMENT_AMOUNT 2
#define FALSE 1
#define TRUE 0
#define MAX_LINE 1024
#define ASCII 256

/**
 * struct for the part of the rail way
 */
typedef struct Part
{
    int startIndex, endIndex;
    char start[1], end[1];
    int length, price;
} Part;
/**
 * the data in the file
 */
typedef struct DATA
{
    char *typeArr;
    Part *partArr;
    int railLength;
    int typeSum;
    int partSum;
    int partArrLength;
} DATA;

/**
 *
 * @param splitLine the splited line with ','
 * @return 0 if its legal , 1 otherwise
 */
int line4validator(char *splitLine[]);

/**
 * function that check if string if some number
 * @param c the string to check
 * @return 0 if its number, o otherwise
 */
int isNumber(char *c);

/**
 * free the data
 * @param data pointer to the data to free
 * @return 0 if its success , 1 otherwise
 */
int freeData(DATA *data);

/**
 * creat table that calculate the minimal price and return the minimal price
 * @param data the data of the file
 * @return the minimal price
 */
int creatTable(DATA *data);

/**
 * check if the part valid
 * @param part the part to check
 * @return 0 if its valid , 1 otherwise
 */
int partValidator(Part *part);

/**
 * print to the file
 * @param c string to print
 * @param d int to print- if d==MAX_INT there if no int
 * @param Point if add point in the end
 * @return 0 if its success , 1 otherwise
 */
int printToFile(char *c, int d, int Point);

/**
 *
 * @param line the line to read
 * @param part the part to put the data in
 * @param currentPartIndex the line index of this line
 * @param data the data
 * @return 0 if its success , 1 otherwise
 */
int readLine(char line[], Part *part, int currentPartIndex, DATA *data);

/**
 * going throw the line of the file and handel with the data
 * @param path the path of the file to read
 * @param data
 * @return 0 if its success , 1 otherwise
 */
int fileHandler(char *path, DATA *data);

/**
 * check if the line legal and save the data
 * @param line the first line
 * @return 0 if its success , 1 otherwise
 */
int line1Handler(char *line);

/**
 * check if the line legal and save the data
 * @param line the second line
 * @return 0 if its success , 1 otherwise
 */
int line2Handler(char *line);

/**
 * check if the line legal and save the data
 * @param line the third line
 * @param data
 * @return 0 if its success , 1 otherwise
 */
int line3Handler(char *line, DATA *data);

/**
 * heck if the line legal and save the data
 * @param line the line to check
 * @param data the data
 * @param lineCounter the line index
 * @return 0 if its success , 1 otherwise
 */
int line4Handler(char *line, DATA *data, int lineCounter);

/**
 * print to file invalid line
 * @param line the number of the line
 * @return 0 if its success , 1 otherwise
 */
int printInvalidLine(int line);

/**
 * gets the cheapest price for cell in the table
 * @param table the table of the prices
 * @param i the row
 * @param j the column
 * @param data the data of the program
 * @return 0
 */
int getCheapestPrice(int **table, int i, int j, DATA *data);

int main(int argc, char *argv[])
{
    if (argc != ARGUMENT_AMOUNT)
    {
        printToFile("Usage: RailwayPlanner <InputFile>", INT_MAX, FALSE);
        exit(EXIT_FAILURE);
    }
    char *path = argv[1];
    char *pathP = path;
    Part *partArr = malloc(ADD_TO_ARRAY * sizeof(Part));
    if (partArr == NULL)
    {
        exit(EXIT_FAILURE);
    }
    int railLength = -1;
    int typeSum = -1;
    int partSum = 0;
    DATA data;
    data.partSum = partSum;
    data.partArr = partArr;
    data.railLength = railLength;
    data.typeSum = typeSum;
    DATA *pointer = &data;
    data.partArrLength = ADD_TO_ARRAY;
    fileHandler(pathP, pointer);
    int min = creatTable(pointer);
    fflush(stdout);
    printToFile("The minimal price is: ", min, FALSE);
    freeData(pointer);
    exit(EXIT_SUCCESS);
}

int fileHandler(char *path, DATA *data)
{
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        printToFile("File doesn't exists", INT_MAX, TRUE);
        exit(EXIT_FAILURE);
    }
    int lineCounter = 0;
    char line[MAX_LINE];
    data->railLength = 0;
    while (fgets(line, MAX_LINE, file) != NULL)
    {
        char *pointer = line;
        if (lineCounter == 0)
        {
            data->railLength = line1Handler(pointer);
            if (data->railLength < 0)
            {
                printInvalidLine(1);
                exit(EXIT_FAILURE);
            }
        }
        else if (lineCounter == 1)
        {
            data->typeSum = line2Handler(pointer);
            if (data->typeSum < 0)
            {
                printInvalidLine(2);
                exit(EXIT_FAILURE);
            }
        }
        else if (lineCounter == 2)
        {
            data->typeArr = malloc(data->typeSum);
            if (data->typeArr == NULL)
            {
                exit(EXIT_FAILURE);
            }
            line3Handler(pointer, data);
        }
        else
        {
            data->partSum = line4Handler(pointer, data, lineCounter);
        }
        lineCounter++;
    }
    if (lineCounter == 0)
    {
        printToFile("File is empty", INT_MAX, TRUE);
        exit(EXIT_FAILURE);
    }
    return 0;
}

int line1Handler(char *line)
{
    if (isNumber(line) == FALSE)
    {
        printInvalidLine(1);
        exit(EXIT_FAILURE);
    }
    char *nothing;
    int railLength = strtol(line, &nothing, 10);
    return railLength;
}

int line2Handler(char *line)
{
    if (isNumber(line) == FALSE)
    {
        printInvalidLine(2);
        exit(EXIT_FAILURE);
    }
    char *nothing;
    int typeSum = strtol(line, &nothing, 10);
    return typeSum;
}

int line3Handler(char *line, DATA *data)
{
    char arr[ASCII];
    for (int i = 0; i < ASCII; i++)
    {
        arr[i] = '0';
    }
    for (int i = 0; i < MAX_LINE; i++)
    {
        if (line[i] == '\0' || line[i] == '\r' || line[i] == '\n')
        {
            break;
        }
        else if (line[i] != ',')
        {
            int index = line[i];
            arr[index] = '1';
        }
        if (i % 2 == 1)
        {
            if (line[i] != ',')
            {
                freeData(data);
                printInvalidLine(3);
                exit(EXIT_FAILURE);
            }
        }
    }
    int k = 0;
    for (int i = 0; i < ASCII; i++)
    {
        if (arr[i] == '1')
        {
            data->typeArr[k] = (char) i;
            k++;
        }
    }
    return 1;
}

int line4Handler(char *line, DATA *data, int lineCounter)
{
    int currentPartIndex = lineCounter - 3;
    if (currentPartIndex == data->partArrLength)
    {
        data->partArr = realloc(data->partArr, currentPartIndex * sizeof(Part) + ADD_TO_ARRAY * sizeof(Part));
        data->partArrLength = data->partArrLength + ADD_TO_ARRAY;
        if (data->partArr == NULL)
        {
            exit(EXIT_FAILURE);
        }
    }
    Part tempPart;
    Part *pointer = &tempPart;
    DATA *dataPointer = data;
    readLine(line, pointer, currentPartIndex, dataPointer);
    data->partArr[currentPartIndex] = tempPart;
    currentPartIndex++;
    return currentPartIndex;
}

int readLine(char line[], Part *part, int currentPartIndex, DATA *data)
{
    if (strchr(line, ',') == NULL)
    {
        printInvalidLine(currentPartIndex + 4);
        freeData(data);
        exit(EXIT_FAILURE);
    }
    char *splitLine[4];
    char *field = strtok(line, ",");
    int i = 0;
    while (field != NULL)
    {
        splitLine[i] = field;
        i++;
        field = strtok(NULL, ",");
    }
    char *nothing;
    DATA *dataPointer = data;
    int temp = line4validator(splitLine);
    if (temp == FALSE)
    {
        printInvalidLine(currentPartIndex + 4);
        freeData(dataPointer);
        exit(EXIT_FAILURE);
    }
    strcpy(part->start, splitLine[0]);
    strcpy(part->end, splitLine[1]);
    part->length = strtol(splitLine[2], &nothing, 10);
    part->price = strtol(splitLine[3], &nothing, 10);
    Part *pointer = part;
    if (partValidator(pointer) == FALSE)
    {
        printInvalidLine(currentPartIndex + 4);
        freeData(dataPointer);
        exit(EXIT_FAILURE);
    }
    int startTemp = 0;
    int endTemp = 0;
    for (int k = 0; k < data->typeSum; k++)
    {
        if (part->start[0] == data->typeArr[k])
        {
            part->startIndex = k;
            startTemp = 1;
        }
        if (part->end[0] == data->typeArr[k])
        {
            part->endIndex = k;
            endTemp = 1;
        }
    }
    if (startTemp == 0 || endTemp == 0)
    {
        printInvalidLine(currentPartIndex + 4);
        freeData(dataPointer);
        exit(EXIT_FAILURE);
    }
    return TRUE;
}

int creatTable(DATA *data)
{
    int **table = (int **) malloc((data->railLength + 1) * sizeof(int *));
    if (table == NULL)
    {
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < data->railLength + 1; i++)
    {
        table[i] = (int *) malloc(data->typeSum * sizeof(int));
        if (table[i] == NULL)
        {
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < data->railLength + 1; i++)
    {
        for (int j = 0; j < data->typeSum; j++)
        {
            if (i == 0)
            {
                table[i][j] = 0;
            }
            else
            {
                table[i][j] = INT_MAX;
            }
        }
    }

    for (int i = 1; i <= data->railLength; i++)
    {
        for (int j = 0; j < data->partSum; j++)
        {
            getCheapestPrice(table, i, j, data);
        }
    }
    int min = INT_MAX;
    for (int j = 0; j < data->typeSum; j++)
    {
        if (table[data->railLength][j] < min)
        {
            min = table[data->railLength][j];
        }
    }
    if (min == INT_MAX)
    {
        min = -1;
    }
    for (int i = 0; i < data->railLength + 1; i++)
    {
        free(table[i]);
    }
    free(table);
    return min;
}

int partValidator(Part *part)
{
    if (part->price <= 0 || part->length <= 0)
    {
        return FALSE;
    }
    return TRUE;
}

int printInvalidLine(int line)
{
    char *c = "Invalid input in line: ";
    return printToFile(c, line, TRUE);
}

int printToFile(char *c, int d, int point)
{
    FILE *fPtr;
    fPtr = fopen("railway_planner_output.txt", "w");
    if (fPtr == NULL)
    {
        return FALSE;
    }
    if (c != NULL)
    {
        fputs(c, fPtr);
    }
    if (d != INT_MAX)
    {
        fprintf(fPtr, "%d", d);
    }
    if (point == TRUE)
    {
        fputs(".", fPtr);
    }
    fclose(fPtr);
    return TRUE;
}

int freeData(DATA *data)
{
    free(data->typeArr);
    data->typeArr = NULL;
    free(data->partArr);
    data->partArr = NULL;
    return 1;
}

int isNumber(char *c)
{
    if (c == NULL)
    {
        return FALSE;
    }
    for (int i = 0; c[i] != '\0' && c[i] != '\r' && c[i] != '\n'; i++)
    {
        if (c[i] > 57 || c[i] < 48)
        {
            return FALSE;
        }
    }
    return TRUE;
}

int line4validator(char *splitLine[])
{
    if (isNumber(splitLine[2]) == FALSE || isNumber(splitLine[3]) == FALSE)
    {
        return FALSE;
    }
    if (strlen(splitLine[0]) != 1 || strlen(splitLine[1]) != 1)
    {
        return FALSE;
    }
    return TRUE;
}

int getCheapestPrice(int **table, int i, int j, DATA *data)
{
    if (data->partArr[j].length <= i)
    {
        int prev = table[i - data->partArr[j].length][data->partArr[j].startIndex];
        if (prev < INT_MAX && prev + data->partArr[j].price < table[i][data->partArr[j].endIndex])
        {
            if (prev + data->partArr[j].price < prev || prev + data->partArr[j].price < data->partArr[j].price)
            {
                table[i][data->partArr[j].endIndex] = INT_MAX;
            }
            else
            {
                table[i][data->partArr[j].endIndex] = prev + data->partArr[j].price;
            }
        }
    }
    return TRUE;
}
