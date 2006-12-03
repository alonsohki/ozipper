#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Tabla de reemplazos */
static char * tab[256];
/* Fichero de entities */
const char *ent_filename = "entities-latin1.dtd";

/*
 * Agrega un reemplazo asociado a un caracter
 */
static void addConv(const unsigned char code, const char *to)
{
  if (tab[(int)code] != 0)
  {
    free(tab[(int)code]);
  }
  tab[(int)code] = strdup(to);
}


/*
 * Inicializa la tabla de conversión
 */
static void initConv()
{
  memset(tab, 0, sizeof(char *) * 256);
}

/*
 * Libera la memoria de la tabla de conversión
 */
static void endConv()
{
  int i;
  
  for (i = 255; i >= 0; i--)
  {
    if (tab[i] != 0)
    {
      free(tab[i]);
    }
  }
}

/*
 * Parser del DTD standard de latin-1
 */
static int parseDTD(const char *ent_filename)
{
  char line[100];
  char name[16];
  char value[16];
  char to[16];

  FILE *fd = fopen(ent_filename, "r");
  
  if (fd == 0)
  {
    return 0;
  }
 
  do
  {
    fgets(line, sizeof(line) - 1, fd);
    if (!strncasecmp(line, "<!ENTITY ", 9))
    {
      sscanf(line, "%*s %s %*s \"&#%[^;]\"", name, value);
      sprintf(to, "&%s;", name);
      
      addConv(atoi(value), to);
    }
  }
  while (!feof(fd));
 
  fclose(fd);
  
  return 1; 
}


int main(int argc, char *argv[])
{
  int i;
  const char *filename;

  if (argc < 2)
  {
    filename = ent_filename;
  }
  else
  {
    filename = argv[1];
  }
  
  initConv();

  if (!parseDTD(filename))
  {
    fprintf(stderr, "Error al abrir el archivo DTD: %s\n", filename);
    exit(-1);
  }

  /* Agregamos conversiones especiales */
  addConv('\n', "<br />");


  puts("/*");
  puts(" * Archivo generado automáticamente por 'htmlspecialchars'");
  puts(" * Programado por Alberto Alonso, webmaster@ozipper.net");
  puts(" */");
  puts("");
  puts("htmlspecialchars = function()");
  puts("{");
  puts("  var htmlspecialchars_tab = new Array(");
  printf("    ");
  for (i = 0; i < 256; i++)
  {
    if (tab[i] == 0)
    {
      printf("'\\%.03o', ", i);
    }
    else
    {
      printf("'%s', ", tab[i]);
    }

    if (!((i + 1) % 10))
    {
      printf("\n    ");
    }
  }
  puts("'\\000'");
  puts("  );");
  puts("");
  puts("  this.Transform = function(code)");
  puts("  {");
  puts("    return htmlspecialchars_tab[code.charCodeAt(0)];");
  puts("  }");
  puts("}");


  endConv();
  
  return 0;
}

