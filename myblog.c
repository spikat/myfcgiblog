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

int filename_to_title(char* filename)
{
    int i;

    for (int i = 0; filename[i]; i++) {
        if (i == 0 && filename[i] >= 'a' && filename[i] <= 'z') {
            filename[i] = filename[i] - 'a' + 'A';
        }
        if (filename[i] == '_') filename[i] = ' ';
        if (!strcmp(filename + i, ".html")) {
            filename[i] = '\0';
            break;
        }
    }
    return (0);
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
    int i;

    puts("<aside><ul>");
    articles = strdup(TOSTRING(_ARTICLES));
    article = strtok(articles, " ");
    for (i = 0/* , article = strtok(articles, " ") */;
         article;
         i++, article = strtok(NULL, " ")) {
        filename_to_title(article);
        printf("<li><a href=\"#_%i\"><font color=\"white\">%s</font></a></li>\n", i, article);
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

    puts("<body>");
    dump_header();
    puts("<section>");
    dump_aside();

    articles = strdup(TOSTRING(_ARTICLES));
    for (i = 0, article = strtok(articles, " ");
         article;
         i++, article = strtok(NULL, " ")) {
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
