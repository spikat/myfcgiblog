#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "config.h"

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
    while (fgets(buf, BUFSZ, f))
        printf("%s", buf);
    fclose(f);
    return (0);
}

char* filename_to_title(char* filename)
{
    int i;
    char* title;

    title = malloc(strlen(filename));
    strcpy(title, filename);
    for (int i = 0; filename[i]; i++) {
        if (i == 0 && filename[i] >= 'a' && filename[i] <= 'z') {
            title[i] = filename[i] - 'a' + 'A';
        }
        if (filename[i] == '_') title[i] = ' ';
        if (!strcmp(filename + i, ".html")) {
            title[i] = '\0';
            break;
        }
    }
    return (title);
}

int dump_head()
{
    puts("<head>"
         "<meta charset=\"UTF-8\">"
         "<title>" BLOG_TITLE "</title>");
    puts("<style type=\"text/css\">");
    dump_content(TOSTRING(_CSS));
    puts("</style></head>");
    return (0);
}

int dump_nav()
{
    puts("<nav>");
    puts("</nav>");
    return (0);
}

int dump_header()
{
    puts("<header>");
    puts("<h1>" BLOG_TITLE "</h1>");
    /* printf("<h1><a href=\"%s\">%s</a></h1>\n", getenv("SCRIPT_NAME"), BLOG_TITLE); */
    puts("<p>Author: " BLOG_AUTHOR "");
    dump_nav();
    puts("</header>");
}

int dump_footer()
{
    puts("<footer>");
#ifdef BLOG_AUTHOR_MAIL
    if (strlen(BLOG_AUTHOR_MAIL))
        puts("<p>Contact: <a href=\"mailto:"BLOG_AUTHOR_MAIL"\">"BLOG_AUTHOR_MAIL"</a>");
#endif /* BLOG_AUTHOR_MAIL */
    puts("<p>Blog source: <a href="BLOG_GIT">"BLOG_GIT"</a>");
    puts("</footer>");
    return (0);
}

int dump_aside()
{
    char* articles;
    char* article;
    char* script;
    char* title;
    int i;

    script = getenv("SCRIPT_NAME");
    puts("<aside><ul>");
    articles = strdup(TOSTRING(_ARTICLES));
    article = strtok(articles, " ");
    printf("<li><a href=\"%s\">%s</a></li>\n", getenv("SCRIPT_NAME"), "Home page");
    for (i = 0/* , article = strtok(articles, " ") */;
         article;
         i++, article = strtok(NULL, " ")) {
        title = filename_to_title(article);
        printf("<li><a href=\"%s/%s\"><font color=\"white\">%s</font></a></li>\n",
               script, article, title);
        free(title);
    }
    free(articles);
    puts("</ul></aside>");
    return (0);
}

int dump_body()
{
    char* articles;
    char* article;
    int i;
    char* request;
    char* script;
    char* toprint = NULL;

    puts("<body>");
    dump_header();
    puts("<section>");
    dump_aside();

    articles = strdup(TOSTRING(_ARTICLES));
    script = getenv("SCRIPT_NAME");
    request = getenv("REQUEST_URI");
    /* -1 is for the '/' case on "myblog.cgi/" */
    if (strlen(script) < strlen(request) - 1)
        toprint = request + strlen(script) + 1;

    for (i = 0, article = strtok(articles, " ");
         article;
         i++, article = strtok(NULL, " ")) {
        if (toprint && strcmp(toprint, article))
            continue;
        puts("<article>");
        printf("<a name=\"_%i\"></a>", i);
        dump_content(article);
        puts("</article>");
    }
    free(articles);

    puts("</section>");
    dump_footer();
    puts("</body>");
    return (0);
}

int main(int ac, char** av)
{
    puts("<!DOCTYPE HTML><html>");
    dump_head();
    dump_body();
    puts("</html>");
    return (0);
}
