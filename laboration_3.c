#include <stdio.h>
#include <stdbool.h>
#include <string.h>


#define FILE_NAME "/Users/leo/Desktop/"
#define ARR_SIZE 1000
#define string_SIZE 3//all names shortened to 3 characters
#define result_SIZE 5//two digits will always represent the scores, 05-03
#define date_SIZE 8

    struct match {
        char date[date_SIZE + 1];
        char home_team[string_SIZE+1];//+1 room for '\0'
        char away_team[string_SIZE+1];
        char result[result_SIZE+1];
    } arr_matches[ARR_SIZE];


//--- FUNCTION DECLARATIONS ---
    int main_menu(struct match arr_matches[], int *p_match_counter, char filename[]);

int register_match(struct match arr_matches[], int *p_match_counter);
void write_all_matches(struct match arr_matches[], int match_counter);
void sort_matches(struct match arr_matches[], int match_counter);
int search_matches(struct match arr_matches[], int match_counter, int arr_indexes[]);
void unregister_match(struct match arr_matches[], int *p_match_counter, int arr_indexes[]);
int present_stats(struct match arr_matches[], int match_counter);
int end_main_menu(struct match arr_matches[], int *p_match_counter, char filename[]);

void date_sort(struct match arr_matches[], int match_counter);
void home_team_sort(struct match arr_match[], int match_counter);


bool stra_morethan_strb(char a[], char b[]);




int main(void)
{
    int menu_value;
    int match_counter = 0; //should be decided by file
    int *p_match_counter = &match_counter;

    char file_name[40] = FILE_NAME;
    char user_file[20] = {'\0'};

    printf("What file do you wish to read from?\n");
    scanf("%s", user_file);
    strcat(user_file, ".txt");
    strcat(file_name, user_file);

    printf("Path to file: %s\n", file_name);
    // --- FILLING THE ARRAY ---
    {
        FILE *fp = fopen(file_name, "r");
        if(fp != NULL)
        {
            fscanf(fp, "%d", &match_counter);
            if(match_counter < ARR_SIZE)
            {
                for(int i = 0; i < match_counter; i++)
                {
                    fscanf(fp, "%s%s", arr_matches[i].date, arr_matches[i].home_team);
                    fscanf(fp, "%s%s", arr_matches[i].away_team, arr_matches[i].result);
                }
            }
            else
            {
                printf("No room left in database...\nFile was not loaded...\n");
            }
            fclose(fp);
        }
        else
            printf("File could not be opened...\n");
            fclose(fp);

    }

    // --- CALL MAIN MENU ---
    do
    {
        menu_value = main_menu(arr_matches, p_match_counter, file_name);
    } while(menu_value != 0);

    return 0;
}

//--- MAIN MENU func ---
int main_menu(struct match arr_matches[], int *p_match_counter, char file_name[])
{
    int choice;
    int arr_indexes[ARR_SIZE];//--- FOR SEARCH MATCHES FUNCTION!!

    printf("_____________________________________________\n");
    printf("\n(1)Register new matches\n(2)Write out all matches\n(3)Sort matches\n(4)Search matches\n(5)Unregister a match\n(6)Present statistics\n(7)End\n");
    scanf("%d", &choice);

    switch(choice)
    {
    case 1:
        while(register_match(arr_matches, p_match_counter) == 1)
        {
            *p_match_counter = *p_match_counter + 1;//Increments match_counter only when register_match() fucntion returns 1
        }
        break;
    case 2:
        write_all_matches(arr_matches, *p_match_counter);
        break;
    case 3:
        sort_matches(arr_matches,*p_match_counter);//derefrenced to "read-not-write"
        break;
    case 4:
        search_matches(arr_matches, *p_match_counter, arr_indexes);//derefrenced to "read-not-write"
        break;
    case 5:
        unregister_match(arr_matches, p_match_counter, arr_indexes);
        break;
    case 6:
        present_stats(arr_matches, *p_match_counter);
        break;
    default:
        end_main_menu(arr_matches, p_match_counter, file_name);
        return 0;
    }

    return 1;
}


//--- MENU OPTION FUNCTIONS ---
int register_match(struct match arr_matches[], int *p_match_counter)
{
    int i = *p_match_counter;
    char tmpdate;

    if(i >= ARR_SIZE-1)
    {
        printf("Database is full! Cannot perform operation.\n");
        return 0;
    }


    printf("What date was the match? (YYYYMMDD)\n");
    getchar();//remove old newline
    tmpdate = getchar();//save possible new newline
    if(tmpdate == '\n')//test character
        return 0;
    else//finish the string
    {
        arr_matches[i].date[0] = tmpdate; //first character in the string is assigned the non- newline character
        for(int j = 1; j < date_SIZE; j++)//the rest of the characters fill the "string"
            arr_matches[i].date[j] = getchar();//The '\0' character is added, to end the "string"
        arr_matches[i].date[date_SIZE] = '\0';
    }
    printf("Name of Home Team (only upper case): (XXX)\n");
    scanf("%s", arr_matches[i].home_team);
    arr_matches[i].home_team[string_SIZE] = '\0';//To ensure length of string

    printf("\nName of Away Team (only upper case): (XXX)\n");
    scanf("%s", arr_matches[i].away_team);
    arr_matches[i].away_team[string_SIZE] = '\0';//To ensure length of string

    printf("\nResult of match: (x-x)\n");
    scanf("%s", arr_matches[i].result);

    return 1;//function can be interrupted above and return 0;
}

void write_all_matches(struct match arr_matches[], int match_counter)
{
    printf("Date   Home team   Away team   Result\n");
    printf("_____________________________________\n");
    for(int i = 0; i < match_counter; i++)
    {
        printf("%s  ", arr_matches[i].date);
        printf("%s  ", arr_matches[i].home_team);
        printf("    %s  ", arr_matches[i].away_team);
        printf("        %s\n", arr_matches[i].result);
    }
    return;
}


// --- FUNCTIONS ---

void sort_matches(struct match arr_matches[], int match_counter)
{
    int user_input;

    printf("Sort by date(1) or by home team(2)?\n");
    scanf("%d", &user_input);
    if(user_input == 1)
        date_sort(arr_matches, match_counter);
    else if(user_input == 2)
        home_team_sort(arr_matches, match_counter);
    else
        printf("Invalid input...\n");

    return;
}

int search_matches(struct match arr_matches[], int match_counter, int arr_indexes[])
{
    char str_s[40];//Can be big, is only temporary
    int counter = 0;

    printf("Enter search: ");
    scanf("%s", str_s);
    if(str_s[39] != '\0')
    {
        printf("Search is too long...\n");
        return counter;
    }
    printf("\n");

    for(int i = 0; i < match_counter; i++)
    {
        if(strstr(arr_matches[i].date, str_s) != NULL)
        {
            arr_indexes[counter] = i;
            counter++;
        }
        else if(strstr(arr_matches[i].home_team, str_s) != NULL)
        {
            arr_indexes[counter] = i;
            counter++;
        }
        else if(strstr(arr_matches[i].away_team, str_s) != NULL)
        {
            arr_indexes[counter] = i;
            counter++;
        }
    }

    if(counter > 0)// Only if search had any results
    {
        printf("Nr      Date   Home team   Away team   Result\n");
        printf("_____________________________________________\n");
        for(int i = 0; i < counter; i++)
            {
                if((arr_indexes[i] + 1) < 10)//looks nicer
                    printf(" %d  ", arr_indexes[i]+1);
                else
                    printf("%d  ", arr_indexes[i]+1);

                printf("    %s  ", arr_matches[arr_indexes[i]].date);
                printf("%s  ", arr_matches[arr_indexes[i]].home_team);
                printf("    %s  ", arr_matches[arr_indexes[i]].away_team);
                printf("        %s\n", arr_matches[arr_indexes[i]].result);
            }
    }
    else
        printf("No results found for %s\n", str_s);


    return counter;
}

void unregister_match(struct match arr_matches[], int *p_match_counter, int arr_indexes[])
{
    int index;
    printf("You have chosen to unregister a match\n");

    search_matches(arr_matches, *p_match_counter, arr_indexes);
    printf("What match do you wish to unregister? ");
    scanf("%d", &index);
    index--; //Nr in table is index + 1, so no zero appears

    *p_match_counter = *p_match_counter - 1;//removing one of the matches, means less total matches
    for(int i = index; i < *p_match_counter; i++)
        arr_matches[i] = arr_matches[i+1];

    return;
}

int present_stats(struct match arr_match[], int match_counter)
{
    char arr_teams[match_counter][string_SIZE + 1];
    int counter = 0;
    strcpy(arr_teams[0], "\0");
    bool not_duplicate;

    for(int i = 0; i < match_counter; i++)
    {
        for(int j = 0; j < counter; j++)
        {
            if(strcmp(arr_teams[j], arr_matches[i].home_team) != 0)
            {
                not_duplicate = true;
            }
            else
            {
                not_duplicate = false;
                printf("now is break\n");
                break;
            }
        }
        if(not_duplicate)
        {
            strcpy(arr_teams[counter], arr_matches[i].home_team);
            counter++;
        }

    }

    for(int i = 0; i < match_counter; i++)
    {
        for(int j = 0; j < counter; j++)
        {
            if(strcmp(arr_teams[j], arr_matches[i].away_team) != 0)
            {
                not_duplicate = true;
            }
            else
            {
                not_duplicate = false;
                break;
            }
        }
        if(not_duplicate)
        {
            strcpy(arr_teams[counter], arr_matches[i].away_team);
            counter++;
        }

    }
    for(int i = 0; i < counter; i++)
        printf("%s\n", arr_teams[i]);

    return 0;
}

int end_main_menu(struct match arr_match[], int *p_match_counter, char file_name[])
{
    FILE *fp = fopen(file_name, "w");
    fprintf(fp, "%d\n", *p_match_counter);
    for(int i = 0; i < *p_match_counter; i++)
    {
        fprintf(fp, "%s ", arr_matches[i].date);
        fprintf(fp, "%s ", arr_matches[i].home_team);
        fprintf(fp, "%s ", arr_matches[i].away_team);
        fprintf(fp, "%s\n", arr_matches[i].result);
    }
    fclose(fp);
    return 0;
}





void date_sort(struct match arr_match[], int match_counter)
{
    struct match tmp_match;

    // - BUBBLE SORT -
    for(int i = 0; i < match_counter - 1; i++)
    {
        for(int j = 0; j < match_counter - 1 - i; j++)
        {
            if(stra_morethan_strb(arr_match[j].date, arr_match[j+1].date))
            {
                tmp_match = arr_match[j];
                arr_match[j] = arr_match[j+1];
                arr_matches[j+1] = tmp_match;
            }
        }
    }

    return;
}

void home_team_sort(struct match arr_match[], int match_counter)
{

    struct match tmp_match;

    // - BUBBLE SORT - with only the first letter, grouping first letters
    for(int i = 0; i < match_counter - 1; i++)
    {
        for(int j = 0; j < match_counter - 1 - i; j++)
        {
            if(stra_morethan_strb(arr_match[j].home_team, arr_match[j+1].home_team))
            {
                tmp_match = arr_match[j];
                arr_match[j] = arr_match[j+1];
                arr_matches[j+1] = tmp_match;
            }
        }
    }

    return;
}



bool stra_morethan_strb(char a[], char b[])//tests one string from arr_match struct array
{
    bool b_value;
    int i = 0;

    while(a[i] != '\0' && b[i] != '\0')
    {
        if(a[i] > b[i])
        {
            b_value = true;
            break;
        }
        else if(a[i] < b[i])
        {
            b_value = false;
            break;
        }
        {
            b_value = false;
            i++;
        }
    }

    return b_value;
}
