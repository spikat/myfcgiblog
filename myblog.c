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
        printf("%s", buf);
    }
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
         "<title>" TOSTRING(BLOG_TITLE) "</title>");
    puts("<style type=\"text/css\">");
    dump_content(TOSTRING(CSS));
    puts("</style></head>");
    return (0);
}

int dump_header()
{
    puts("<header>");
    puts("<h1>" TOSTRING(BLOG_TITLE) "</h1>");
    puts("<p>Author: " TOSTRING(BLOG_AUTHOR) "");
    puts("<p>Blog source: <a href=" TOSTRING(BLOG_GIT) ">" TOSTRING(BLOG_GIT) "</a>");
    dump_nav();
    puts("</header>");
}

int dump_footer()
{
    puts("<footer>footer</footer>");
    return (0);
}

int dump_aside()
{
    char* articles;
    char* article;

    puts("<aside><ul>");
        articles = strdup(TOSTRING(ARTICLES));
    for (article = strtok(articles, " ");
         article;
         article = strtok(NULL, " ")) {
        filename_to_title(article);
        printf("<li>%s</li>\n", article);
    }
    free(articles);
    puts("</ul></aside>");
    return (0);
}

int dump_nav()
{
    puts("<nav>");
    puts("TODO: nav");
    puts("</nav>");
    return (0);
}

int dump_body()
{
    char* articles;
    char* article;

    puts("<body>");
    dump_header();
    puts("<section>");
    dump_aside();

    articles = strdup(TOSTRING(ARTICLES));
    for (article = strtok(articles, " ");
         article;
         article = strtok(NULL, " ")) {
        puts("<article>");
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
