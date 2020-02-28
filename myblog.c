#define _GNU_SOURCE

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

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

int dump_aside_articles(const char* script)
{
    char* title;
    char* articles;
    char* article;

    articles = strdup(TOSTRING(_ARTICLES));
    article = strtok(articles, " ");

    /* list articles */
    puts("<p>Articles:</p>");
    for (; article; article = strtok(NULL, " ")) {
        title = filename_to_title(article);
        printf("<li><a href=\"%s/%s\"><font color=\"white\">%s</font></a></li>\n",
               script, article, title);
        free(title);
    }
    free(articles);
    return (0);
}

int dump_aside_galleries(const char* script)
{
    char* title;
    char* galleries;
    char* gallery;

    galleries = strdup(TOSTRING(_GALLERIES));
    gallery = strtok(galleries, " ");

    /* list galleries */
    puts("<p>Galleries:</p>");
    for (; gallery; gallery = strtok(NULL, " ")) {
        title = filename_to_title(gallery);
        printf("<li><a href=\"%s/%s/\"><font color=\"white\">%s</font></a></li>\n",
               script, gallery, title);
        free(title);
    }
    free(galleries);
    return (0);
}

int dump_aside()
{
    char* script;

    script = getenv("SCRIPT_NAME");
    puts("<aside><ul>");
    printf("<li><a href=\"%s\">%s</a></li>\n", getenv("SCRIPT_NAME"), "Home page");

    dump_aside_articles(script);
    dump_aside_galleries(script);
    
    puts("</ul></aside>");
    return (0);
}

int is_url_html(const char* url)
{
    char* s;

    s = strcasestr(url, ".html");
    if (s && strlen(s) == strlen(".html"))
        return (1);
    return (0);
}

int is_url_dir(const char* url)
{
    if (url[strlen(url) - 1] == '/')
        return (1);
    return (0);
}

int dump_body_gallery(const char* script, const char* url)
{
    DIR *d;
    struct dirent *dir;
    char* url_noslash;

    puts("<body>");
    dump_header();
    puts("<section>");
    dump_aside();

    /* remove last '/' from url */
    url_noslash = strdup(url);
    url_noslash[strlen(url_noslash) - 1] = '\0';

    d = opendir(url_noslash);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_REG)
                printf("<a href=\"%s/%s/%s\"><img src=\"%s/%s/%s\" width=\"210\" height=\"150\">\n",
                       script, url_noslash, dir->d_name, 
                       script, url_noslash, dir->d_name
                    );
        }
        closedir(d);
    } else
        printf("<p>ERROR: %s</p>\n", strerror(errno));
    free(url_noslash);

    puts("</section>");
    dump_footer();
    puts("</body>");
    return (0);
}

int dump_body_article(const char* url)
{
    char* articles;
    char* article;

    puts("<body>");
    dump_header();
    puts("<section>");
    dump_aside();

    articles = strdup(TOSTRING(_ARTICLES));
    for (article = strtok(articles, " ");
         article;
         article = strtok(NULL, " ")) {
        if (strcmp(url, article))
            continue;
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

int dump_body(void)
{
    char* articles;
    char* article;

    puts("<body>");
    dump_header();
    puts("<section>");
    dump_aside();

    articles = strdup(TOSTRING(_ARTICLES));
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
    char* request;
    char* script;
    char* url = NULL;

    script = getenv("SCRIPT_NAME");
    request = getenv("REQUEST_URI");
    /* -1 is for the '/' case on "myblog.cgi/" */
    if (strlen(script) < strlen(request) - 1)
        url = request + strlen(script) + 1;


    if (url && is_url_html(url)) { /* dump a single article */
        puts("<!DOCTYPE HTML><html>");
        dump_head();
        dump_body_article(url);
        puts("</html>");
    } else if (url && is_url_dir(url)) { /* dump a single gallery */
        puts("<!DOCTYPE HTML><html>");
        dump_head();
        dump_body_gallery(script, url);
        puts("</html>");
    } else if (!url) {
        puts("<!DOCTYPE HTML><html>");
        dump_head();
        dump_body(); /* dump all articles and galleries */
        puts("</html>");
    } /* else { */
    /*     puts("HTTP/1.1 200 OK"); */
    /*     puts("Accept-Ranges: bytes"); */
    /*     puts("Content-Length: 129298"); */
    /*     puts("Content-Type: image/jpeg"); */
    /*     puts("Connection: close"); */
    /*     puts(""); */

    /*     /\* char* path; *\/ */
    /*     /\* asnprintf(&path, "%s%s", url *\/ */

    /*     /\* puts("<!DOCTYPE HTML><html>"); *\/ */
    /*     /\* puts(url); *\/ */
    /*     /\* puts("</html>"); *\/ */
    /*     dump_content(url); */

    /* } */
    return (0);
}
