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
    puts("<aside>");
    puts("<p>aside");
    puts("<p>aside");
    puts("<p>aside");
    puts("<p>aside");
    puts("<p>aside");
    puts("<p>aside");
    puts("<p>aside");
    puts("<p>aside");
    puts("<p>aside");
    puts("<p>aside");
    puts("<p>aside");
    puts("<p>aside");
    puts("<p>aside");
    puts("</aside>");
    return (0);
}

int dump_nav()
{
    puts("<nav>");
    puts("<ul>"
         "<li>nav</li>"
         "<li>nav</li>"
         "</ul>");
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
    puts("<!DOCTYPE html><html>");
    dump_head();
    dump_body();
    puts("</html>");
    return (0);
}
