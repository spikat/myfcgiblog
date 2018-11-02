#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define BUFSZ   512


int dump_content(const char* a)
{
    FILE*   f;
    char    buf[BUFSZ + 1];
    buf[BUFSZ] = '\0';
    f = fopen(a, "r");
    if (!f) return (ENOENT);
    while (fgets(buf, BUFSZ, f)) {
        puts(buf);
    }
    fclose(f);
    return (0);
}

int main(int ac, char** av)
{
    char* articles;
    char* article;

    puts("Content-type: text/html\n\n");
    dump_content(TOSTRING(CSS));
    articles = strdup(TOSTRING(ARTICLES));
    for (article = strtok(articles, " ");
         article;
         article = strtok(NULL, " ")) {
        dump_content(article);
    }
    free(articles);
    return (0);
}
