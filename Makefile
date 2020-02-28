include config.mk

NAME		=	myblog.cgi
SRC			=	myblog.c
OBJ			=	$(SRC:.c=.o)

ARTICLES_HTML		=	$(ARTICLES_MD:.md=.html)

CFLAGS	=	-g
#LDFLAGS =	-lfcgi
LDFLAGS =	/usr/lib/x86_64-linux-gnu/libfcgi.a
RM		=	rm -f
CC		=	gcc

%.o		:	%.c
			$(CC) -o $@ -c $< $(CFLAGS) \
			-D_ARTICLES="$(ARTICLES_HTML)" \
			-D_GALLERIES="$(GALLERIES_DIR)" \
			-D_CSS=$(CSS)

%.html	:	%.md
			python -m markdown -x codehilite $< > $@

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
			cp -f $(NAME) $(ARTICLES_HTML) $(CSS) $(INSTALL_DIR)/
			cp -fr $(GALLERIES_DIR) $(INSTALL_DIR)/
