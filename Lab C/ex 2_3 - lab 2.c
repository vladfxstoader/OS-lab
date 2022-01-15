#include <stdio.h>
#include <string.h>
#include <stdlib.h>
using namespace std;
int count_occurrences(const char* text, const char* word)
{
    int nr=0;
    for (int i=0;text[i]!='\0';i++)
		if (strstr(&text[i],word)==&text[i]) //daca prima aparatie a cuvantului este
        {                                    //la pozitia in care ne aflam acum, crestem contorul
			nr++;
			i=i+strlen(word)-1; //index-ul se muta la finalul cuvantului pentru a putea
		}                       //relua cautarea mai departe
    return nr;
}
char* replace_word (const char* text, const char* old_word, const char* new_word)
{
    int nr=count_occurrences(text,old_word);
    char* new_text;
    int neww_l=strlen(new_word);
    int oldw_l=strlen(old_word);
    int new_size=strlen(text)+nr*(neww_l-oldw_l)+1; //dimensiunea noului text
    new_text=(char*)malloc(sizeof(char)*new_size);
    int i=0;
    while (*text)
    {
        if (strstr(text,old_word)==text) //daca gasim cuvantul care trebuie inlocuit
        {
            strcpy(&new_text[i],new_word); //punem in sirul nou cuvantul cu care trebuie inlocuit
            i=i+neww_l;                    //si inaintam
            text=text+oldw_l;
        }
        else
            new_text[i++]=*text++;      //daca gasim altceva inafara de cuvantul ce trebuie inlocuit
    }                                   //il punem in noul sir
    new_text[i]='\0';
    return new_text;
}
int main()
{
    printf("Introduceti textul:\n");
    char *text=(char*)malloc(sizeof(char)*100);
    fgets(text,100,stdin);
    printf("Introduceti cuvantul ce trebuie inlocuit:\n");
    char *oldw=(char*)malloc(sizeof(char)*20);
    scanf("%s",oldw);
    printf("Introduceti cuvantul cu care trebuie inlocuit:\n");
    char* neww=(char*)malloc(sizeof(char)*20);
    scanf("%s",neww);
    printf("REZULTAT:\n");
    printf(replace_word(text,oldw,neww));
    free(text);
    free(oldw)
    free(neww)
    return 0;
}
