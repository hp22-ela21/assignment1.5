/**********************************************************************************************
* I f�ljande program deklareras en array som tilldelas flyttal tv� g�nger fr�n separata
* funktioner. F�rst tilldelas randomiserade flyttal, som sorteras i stigande ordning och
* skrivs ut b�de i konsolen samt textfilen data.txt. D�refter tilldelas flyttal 0 - 19, som
* sorteras i fallande ordning och bifogas i textfilen. Filskrivningarna kontrolleras genom
* att inneh�llet l�ses in rad f�r rad och skrivs ut i konsolen.
**********************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 20 
#define MAX 100.0

typedef enum { SORT_DIRECTION_ASCENDING, SORT_DIRECTION_DESCENDING } sort_direction_t;

/**********************************************************************************************
* Returnerar randomiserat flyttal mellan 0 - MAX.
**********************************************************************************************/
static double get_random(void) { return (double)rand() / RAND_MAX * MAX; }

/**********************************************************************************************
* Returnerar flyttal, som inkrementeras med ett efter varje anrop.
**********************************************************************************************/
static double get_number(void)
{
   static double number = 0.0;
   return number++;
}

/**********************************************************************************************
* Tilldelar data till en array fr�n en given funktion, som pekas p� av ing�ende argument
* get_double, vilket �r en funktionspekare till funktionen som anv�nds f�r tilldelningen.
**********************************************************************************************/
static void assign(double* data, const size_t size, double (*get_double)(void))
{
   register double* i;
   for (i = data; i < data + size; i++)
      *i = get_double();
   return;
}

/**********************************************************************************************
* Sorterar en array i stigande eller fallande ordning, vilket specificeras via ing�ende
* argument sort_direction.
**********************************************************************************************/
static void sort(double* data, const size_t size, const sort_direction_t sort_direction)
{
   register double* i, * j;

   for (i = data; i < data + size - 1; i++)
   {
      for (j = i + 1; j < data + size; j++)
      {
         if ((sort_direction == SORT_DIRECTION_ASCENDING && *i > *j) ||
            (sort_direction == SORT_DIRECTION_DESCENDING && *i < *j))
         {
            const double temp = *i;
            *i = *j;
            *j = temp;
         }
      }
   }
   return;
}
/**********************************************************************************************
* Returnerar datum och tid p� formen "Dag YYYY-MM-DD HH:MM:SS".
**********************************************************************************************/
static void get_datetime(char* s, const size_t size)
{
   time_t current_time = time(0);
   struct tm* tptr = localtime(&current_time);
   strftime(s, size, "%A %Y-%m-%d %H:%M:%S", tptr);
   return;
}

/**********************************************************************************************
* Skriver ut inneh�ll ur en array till en specifik utenhet, d�r standardutenheten stdout 
* anv�nds som default f�r skrivning ut i konsolen. Vid skrivning till fil (om varken en
* nullpekare eller stdout har passerats) s� dokumenteras ocks� tid och datum f�r skrivningen.
**********************************************************************************************/
static void print(FILE* stream, const double* data, const size_t size)
{
   if (!stream) stream = stdout;

   if (stream != stdout)
   {
      char datetime[100];
      datetime[0] = '\0';
      get_datetime(datetime, sizeof(datetime));
      fprintf(stream, "Data added at %s!\n", datetime);
   }

   register const double* i;
   fprintf(stream, "-------------------------------------------------------------------------\n");
   for (i = data; i < data + size; i++)
      fprintf(stream, "%g\n", *i);
   fprintf(stream, "-------------------------------------------------------------------------\n\n");
   return;
}

/**********************************************************************************************
* L�ser inneh�llet fr�n en fil rad f�r rad och skriver ut i konsolen.
**********************************************************************************************/
static void file_read(const char* filepath)
{
   FILE* fstream = fopen(filepath, "r");
   char s[100];
   if (!fstream) return;
   s[0] = '\0';
   while (fgets(s, sizeof(s), fstream))
      fprintf(stdout, "%s", s);
   fclose(fstream);
   return;
}

/**********************************************************************************************
* Fyller en statisk array med 20 flyttal mellan 0 - 100 och skriver ut dessa i konsolen.
* Flyttalen sorteras i stigande ordning och skrivs till filen data.txt, d�r eventuellt
* tidigare inneh�ll raderas. Sedan tilldelas flyttal 0 - 19 till arrayen, som sorteras i
* fallande ordning och bifogas sedan l�ngst ned i filen data.txt. Innan programmet avslutas
* verifieras filskrivningarna genom att inneh�llet l�ses in och skrivs ut i konsolen.
**********************************************************************************************/
int main(void)
{
   double data[SIZE];
   FILE* fstream;

   assign(data, SIZE, &get_random);
   print(stdout, data, SIZE);
   sort(data, SIZE, SORT_DIRECTION_ASCENDING);
   fstream = fopen("data.txt", "w");
   print(fstream, data, SIZE);

   assign(data, SIZE, &get_number);
   sort(data, SIZE, SORT_DIRECTION_DESCENDING);
   fstream = fopen("data.txt", "a");
   print(fstream, data, SIZE);

   fclose(fstream);
   file_read("data.txt");
   return 0;
}

