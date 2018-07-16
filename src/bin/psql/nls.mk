# $Header: /project/eecs/db/cvsroot/postgres/src/bin/psql/nls.mk,v 1.2 2004/03/24 08:11:26 chungwu Exp $
CATALOG_NAME	:= psql
AVAIL_LANGUAGES	:= cs de fr hu ru sv zh_CN zh_TW
GETTEXT_FILES	:= command.c common.c copy.c help.c input.c large_obj.c \
                   mainloop.c print.c startup.c describe.c
GETTEXT_TRIGGERS:= _ psql_error simple_prompt
