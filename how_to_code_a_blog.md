# How to code a blog

## Why ?

.. why not? :)

## Requirements

* http server (personally, I use lighttpd).
* gcc (any version)
* libfcgi
* emacs (other crappy editors may also works).

## HOW TO

### Install and setup your http server

For lighttpd, look here: https://redmine.lighttpd.net/projects/1/wiki/docs_modfastcgi
OR here: http//google.com :)

## Wrote your articles

Today, only basic markdown parsing is available. So, just create your own_article.md.

So:

> $> emacs own_article.md

Wrote, wrote, wrote, save and exit.

### Customize your CSS

If you want a custom CSS, replace the my.css file (or modify/sed the Makefile).

## make your blog :)

> $> make

## That's it.

Nothing more. <\\//
