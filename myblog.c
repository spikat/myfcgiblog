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

#define H1 "# "
#define H2 "## "
#define H3 "### "
#define QUOTE "> "
#define BULLET "* "
int dump_article_form_md(const char* a)
{
    FILE*   f;
    char    buf[BUFSZ + 1];
    char*   s;

    buf[BUFSZ] = '\0';
    f = fopen(a, "r");
    if (!f) return (ENOENT);
    while (fgets(buf, BUFSZ, f)) {
        if (s = strstr(buf, H3))
            printf("<h3>%s</h3>", s + strlen(H3));
        else if (s = strstr(buf, H2))
            printf("<h2>%s</h2>", s + strlen(H2));
        else if (s = strstr(buf, H1))
            printf("<h1>%s</h1>", s + strlen(H1));
        else if (s = strstr(buf, QUOTE))
            printf("<blockquote><p>%s</p></blockquote>", s + strlen(QUOTE));
        else if (s = strstr(buf, BULLET))
            printf("<ul><li>%s</li></ul>", s + strlen(BULLET));
        else
            printf("<p>%s</p>", buf);
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
        dump_article_form_md(article);
    }
    free(articles);
    return (0);
}
