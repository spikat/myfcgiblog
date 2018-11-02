BLOG_TITLE	=	"My cool blog which is cool"
BLOG_AUTHOR	=	"Spi"
BLOG_GIT	=	"https:\/\/github.com/spikat/myfcgiblog"

INSTALL_DIR	=	~/public_html/fastcgi/

NAME		=	myblog.cgi
SRC			=	myblog.c
OBJ			=	$(SRC:.c=.o)

CSS					=	my.css
ARTICLES_MD			=	$(sort $(wildcard  *.md))
ARTICLES_HTML		=	$(ARTICLES_MD:.md=.html)

CFLAGS	=
#LDFLAGS =	-lfcgi
LDFLAGS =	/usr/lib/x86_64-linux-gnu/libfcgi.a
RM		=	rm -f
CC		=	gcc

%.o		:	%.c
			$(CC) -o $@ -c $< $(CFLAGS) \
			-DARTICLES="$(ARTICLES_HTML)" \
			-DCSS=$(CSS) \
			-DBLOG_TITLE=$(BLOG_TITLE) \
			-DBLOG_AUTHOR=$(BLOG_AUTHOR) \
			-DBLOG_GIT=$(BLOG_GIT)

%.html	:	%.md
			markdown $< > $@

all 	:	htmls cgi

cgi		:	$(OBJ)
			$(CC) -o $(NAME) $(OBJ) $(CFLAGS) $(LDFLAGS)

htmls	:	$(ARTICLES_HTML)

clean 	:
			$(RM) $(OBJ) *~

fclean 	:	clean
			$(RM) $(NAME) $(ARTICLES_HTML)

re		:	fclean all

install	:	all
			mkdir -p $(INSTALL_DIR)
			cp -f $(NAME) $(ARTICLES_HTML) $(CSS) $(INSTALL_DIR)
