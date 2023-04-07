#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/file.h>

#define NAME_LEN 33
#define CGI_NAME "points.cgi"
#define CSS_PATH "style.css"
#define MAX_NEG_PTS -100

struct points_record
{
   int points; /* Total points that the owner has */
   char name[NAME_LEN]; /* Name of the points owner        */
   struct points_record *p_next;
};

typedef struct points_record PTS_REC;

void print_headers();

PTS_REC *create_pts_rec(char *p_name, int points);

void insert_pts_rec(PTS_REC **p_pts_db, PTS_REC *p_pts_rec);

void print_pts_rec(PTS_REC *p_pts_db);

void save_pts_rec(PTS_REC *p_pts_db);

void load_pts_rec(PTS_REC **p_pts_db);

void update_pts_rec(PTS_REC *p_pts_db, char *p_name, int increment);

void destroy_pts_rec(PTS_REC **p_pts_db);

void cleanup_negatives(PTS_REC **p_pts_db);

void delete_pts_rec(PTS_REC **p_pts_db, char *p_name);

int test();

int main(int argc, char *argv[])
{
   PTS_REC *p_db = NULL;
   char *p_query, cmd, temp_name[NAME_LEN];
   int param_count = 0,
       increment = 0;
   
   print_headers();
   printf("<!DOCTYPE html><html><meta charset='utf-8'><head>\n");
   printf("<meta name='viewport' content='width=device-width, initial-scale=1'/>");
   printf("<link rel='stylesheet' href='%s'>", CSS_PATH);
   printf("</head><body>");
   /* Load the points records database from the file */
   if ((p_query = getenv("QUERY_STRING")) == NULL)
   {
      cmd = 'p';
   }
   load_pts_rec(&p_db);
   param_count = sscanf(p_query, "c=%c&n=%[A-Za-z]&i=%d", &cmd, temp_name, &increment);
   switch (cmd)
   {
      default:
      case 'p':
         break;
      case 'a':
         if (param_count == 2)
         {
            insert_pts_rec(&p_db, create_pts_rec(temp_name, 0));
            save_pts_rec(p_db);
         }
         break;
      case 'd':
         if (param_count == 2)
         {
            delete_pts_rec(&p_db, temp_name);
            save_pts_rec(p_db);
         }
         break;
      case 'u':
         if (param_count == 3)
         {
            update_pts_rec(p_db, temp_name, increment);
            cleanup_negatives(&p_db);
            save_pts_rec(p_db);
         }
         break;

   }
   print_pts_rec(p_db);
   destroy_pts_rec(&p_db);
      
   printf("<form action='%s'>\n", CGI_NAME);
   printf("<input type='hidden' name='c' value='a'>\n");
   printf("<input name='n'>");
   printf("<input type='submit' value='Join'>\n");
   printf("</form>\n");
   printf("</body></html>");
   return 0;
   
}

int test()
{
   PTS_REC *p_db = NULL;
   char menu_choice, temp_name[NAME_LEN];
   int temp_incr;
   
   print_headers();
   
   do
   {
      scanf(" %c", &menu_choice);
      switch (menu_choice)
      {
         case 'w':
            printf("Saving points records to file\n");
            save_pts_rec(p_db);
            break;
         case 'l':
            printf("Loading points records from file\n");
            destroy_pts_rec(&p_db);
            load_pts_rec(&p_db);
            break;
         case 'a':
            printf("Adding new record to DB\n");
            scanf(" %s", temp_name);
            insert_pts_rec(&p_db, create_pts_rec(temp_name, 0));
            break;
         case 'u':
            scanf(" %s", temp_name);
            scanf(" %d", &temp_incr);
            update_pts_rec(p_db, temp_name, temp_incr);
            break;
         case 'p':
            print_pts_rec(p_db);
            break;
         case 'q':
            printf("Quitting...\n");
            break;
         default:
            break;
      }
   }
   while (tolower(menu_choice) != 'q');
   return 0;
}

/* Print necessary HTTP headers for CGI */
void print_headers()
{
   printf("Content-Type: text/html;charset=utf-8\r\n\r\n");
   return;
}

/* Create a points record */
PTS_REC *create_pts_rec(char *p_name, int points)
{
   PTS_REC *p_new_pts_rec; /* Newly allocated points record */
   if ((p_new_pts_rec = malloc(sizeof(PTS_REC))) == NULL)
   {
      printf("There was an error allocating enough memory for a new points record!\n");
      exit(1);
   }
   
   strcpy(p_new_pts_rec->name, p_name);
   p_new_pts_rec->points = points;
   p_new_pts_rec->p_next = NULL;
   
   return p_new_pts_rec;
}

/* Insert a points record into the ascending sorted points DB */
void insert_pts_rec(PTS_REC **p_pts_db, PTS_REC *p_pts_rec)
{
   PTS_REC *p_prev_pts_rec = NULL,
           *p_curr_pts_rec = *p_pts_db;
           
   if (*p_pts_db)
   {
      while (p_curr_pts_rec &&
             p_pts_rec->points < p_curr_pts_rec->points)
      {
         p_prev_pts_rec = p_curr_pts_rec;
         p_curr_pts_rec = p_curr_pts_rec->p_next;
      }
      
      if (p_curr_pts_rec)
      {
         if (p_prev_pts_rec)
         {
            /* Insert in the middle of the list */
            p_pts_rec->p_next = p_curr_pts_rec;
            p_prev_pts_rec->p_next = p_pts_rec;
         }
         else
         {
            /* Insert at the head of the list */
            *p_pts_db = p_pts_rec;
            p_pts_rec->p_next = p_curr_pts_rec;
         }
      }
      else
         p_prev_pts_rec->p_next = p_pts_rec;
   }
   else
      *p_pts_db = p_pts_rec;
   return;
}

/* Print the points record database with a special message if empty */
void print_pts_rec(PTS_REC *p_pts_db)
{
   printf("<table>");
   if (p_pts_db)
      while (p_pts_db)
      {
         printf("<tr><td>%s</td><td>%d</td>\n", p_pts_db->name, p_pts_db->points);
         printf("<td><a href='%1$s?c=u&n=%2$s&i=-4'>--</a></td><td><a href='%1$s?c=u&n=%2$s&i=4'>++</a></td></tr>", CGI_NAME, p_pts_db->name);
         p_pts_db = p_pts_db->p_next;
      }
   else
      printf("<tr>Currently the points database is empty</tr>\n");
   printf("</table>");
   return;
}

/* Save a points record database to a file */
void save_pts_rec(PTS_REC *p_pts_db)
{
   FILE *p_file;
   if ((p_file = fopen("points.db", "wb")) == NULL)
   {
      printf("Something bad happened while trying to open the points DB for writing.\n");
      exit(1);
   }

   while (p_pts_db)
   {
      fwrite(p_pts_db, sizeof(PTS_REC), 1, p_file);
      p_pts_db = p_pts_db->p_next;
   }
   fclose(p_file);
   return;
}

/* Load a points record database from a file */
void load_pts_rec(PTS_REC **p_pts_db)
{
   FILE *p_file;
   PTS_REC p_temp_pts_rec;
   
   if ((p_file = fopen("points.db", "rb")) == NULL)
   {
      printf("Something bad happened while trying to open the points DB for reading.\n");
      exit(1);
   }

   do
   {
      fread(&p_temp_pts_rec, sizeof(PTS_REC), 1, p_file);
      if (feof(p_file))
      {
         p_temp_pts_rec.p_next = NULL;
         break;
      }
      insert_pts_rec(p_pts_db, create_pts_rec(p_temp_pts_rec.name, p_temp_pts_rec.points));
   }
   while (p_temp_pts_rec.p_next);
   
   fclose(p_file);
   return;
}

/*  */
void update_pts_rec(PTS_REC *p_pts_db, char *p_name, int increment)
{
   while (p_pts_db)
   {
      if (!strcmp(p_pts_db->name, p_name))
      {
         p_pts_db->points += increment;
      }
      p_pts_db = p_pts_db->p_next;
   }
   return;
}

void cleanup_negatives(PTS_REC **p_pts_db)
{
   PTS_REC *p_prev_pts_rec = NULL,
           *p_curr_pts_rec = *p_pts_db;
           
   while (p_curr_pts_rec)
   {
      if (p_curr_pts_rec->points < MAX_NEG_PTS)
      {
         if (p_prev_pts_rec)
         {
            /* delete a record in the middle or end of the database */
            p_prev_pts_rec->p_next = p_curr_pts_rec->p_next;
            free(p_curr_pts_rec);
            p_curr_pts_rec = p_prev_pts_rec;
         }
         else
         {
            /* delete a record at the beginning of the databse */
            *p_pts_db = p_curr_pts_rec->p_next;
            free(p_curr_pts_rec);
            p_curr_pts_rec = NULL;
         }
      }
      p_prev_pts_rec = p_curr_pts_rec;
      p_curr_pts_rec = p_curr_pts_rec->p_next;
   }
   return;
}

void delete_pts_rec(PTS_REC **p_pts_db, char *p_name)
{
   PTS_REC *p_prev_pts_rec = NULL,
           *p_curr_pts_rec = *p_pts_db;
           
   while (p_curr_pts_rec)
   {
      if (!strcmp(p_curr_pts_rec->name, p_name))
      {
         if (p_prev_pts_rec)
         {
            /* delete a record in the middle or end of the database */
            p_prev_pts_rec->p_next = p_curr_pts_rec->p_next;
            free(p_curr_pts_rec);
            p_curr_pts_rec = p_prev_pts_rec;
         }
         else
         {
            /* delete a record at the beginning of the databse */
            *p_pts_db = p_curr_pts_rec->p_next;
            free(p_curr_pts_rec);
            p_curr_pts_rec = NULL;
         }
      }
      p_prev_pts_rec = p_curr_pts_rec;
      p_curr_pts_rec = p_curr_pts_rec->p_next;
   }
}

void destroy_pts_rec(PTS_REC **p_pts_db)
{
   PTS_REC *p_temp_pts_rec;
   while (*p_pts_db)
   {
      p_temp_pts_rec = *p_pts_db;
      *p_pts_db = (*p_pts_db)->p_next;
      free(p_temp_pts_rec);
   }
   *p_pts_db = NULL;
   return;
}