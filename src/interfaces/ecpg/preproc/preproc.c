/* A Bison parser, made by GNU Bison 1.875.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     SQL_ALLOCATE = 258,
     SQL_AUTOCOMMIT = 259,
     SQL_BOOL = 260,
     SQL_BREAK = 261,
     SQL_CALL = 262,
     SQL_CARDINALITY = 263,
     SQL_CONNECT = 264,
     SQL_CONNECTION = 265,
     SQL_CONTINUE = 266,
     SQL_COUNT = 267,
     SQL_CURRENT = 268,
     SQL_DATA = 269,
     SQL_DATETIME_INTERVAL_CODE = 270,
     SQL_DATETIME_INTERVAL_PRECISION = 271,
     SQL_DESCRIPTOR = 272,
     SQL_DISCONNECT = 273,
     SQL_ENUM = 274,
     SQL_FOUND = 275,
     SQL_FREE = 276,
     SQL_GO = 277,
     SQL_GOTO = 278,
     SQL_IDENTIFIED = 279,
     SQL_INDICATOR = 280,
     SQL_KEY_MEMBER = 281,
     SQL_LENGTH = 282,
     SQL_LONG = 283,
     SQL_NAME = 284,
     SQL_NULLABLE = 285,
     SQL_OCTET_LENGTH = 286,
     SQL_OPEN = 287,
     SQL_RELEASE = 288,
     SQL_REFERENCE = 289,
     SQL_RETURNED_LENGTH = 290,
     SQL_RETURNED_OCTET_LENGTH = 291,
     SQL_SCALE = 292,
     SQL_SECTION = 293,
     SQL_SHORT = 294,
     SQL_SIGNED = 295,
     SQL_SQL = 296,
     SQL_SQLERROR = 297,
     SQL_SQLPRINT = 298,
     SQL_SQLWARNING = 299,
     SQL_START = 300,
     SQL_STOP = 301,
     SQL_STRUCT = 302,
     SQL_UNSIGNED = 303,
     SQL_VALUE = 304,
     SQL_VAR = 305,
     SQL_WHENEVER = 306,
     S_ADD = 307,
     S_AND = 308,
     S_ANYTHING = 309,
     S_AUTO = 310,
     S_CONST = 311,
     S_DEC = 312,
     S_DIV = 313,
     S_DOTPOINT = 314,
     S_EQUAL = 315,
     S_EXTERN = 316,
     S_INC = 317,
     S_LSHIFT = 318,
     S_MEMPOINT = 319,
     S_MEMBER = 320,
     S_MOD = 321,
     S_MUL = 322,
     S_NEQUAL = 323,
     S_OR = 324,
     S_REGISTER = 325,
     S_RSHIFT = 326,
     S_STATIC = 327,
     S_SUB = 328,
     S_VOLATILE = 329,
     S_TYPEDEF = 330,
     TYPECAST = 331,
     ABORT_TRANS = 332,
     ABSOLUTE = 333,
     ACCESS = 334,
     ACTION = 335,
     ADD = 336,
     AFTER = 337,
     AGGREGATE = 338,
     ALL = 339,
     ALTER = 340,
     ANALYSE = 341,
     ANALYZE = 342,
     AND = 343,
     ANY = 344,
     AS = 345,
     ASC = 346,
     ASSERTION = 347,
     ASSIGNMENT = 348,
     AT = 349,
     AUTHORIZATION = 350,
     BACKWARD = 351,
     BEFORE = 352,
     BEGIN_TRANS = 353,
     BETWEEN = 354,
     BIGINT = 355,
     BINARY = 356,
     BIT = 357,
     BOTH = 358,
     BOOLEAN = 359,
     BY = 360,
     CACHE = 361,
     CALLED = 362,
     CASCADE = 363,
     CASE = 364,
     CAST = 365,
     CHAIN = 366,
     CHAR_P = 367,
     CHARACTER = 368,
     CHARACTERISTICS = 369,
     CHECK = 370,
     CHECKPOINT = 371,
     CLASS = 372,
     CLOSE = 373,
     CLUSTER = 374,
     COALESCE = 375,
     COLLATE = 376,
     COLUMN = 377,
     COMMENT = 378,
     COMMIT = 379,
     COMMITTED = 380,
     CONSTRAINT = 381,
     CONSTRAINTS = 382,
     CONVERSION_P = 383,
     CONVERT = 384,
     COPY = 385,
     CREATE = 386,
     CREATEDB = 387,
     CREATEUSER = 388,
     CROSS = 389,
     CURRENT_DATE = 390,
     CURRENT_TIME = 391,
     CURRENT_TIMESTAMP = 392,
     CURRENT_USER = 393,
     CURSOR = 394,
     CYCLE = 395,
     DATABASE = 396,
     DAY_P = 397,
     DEALLOCATE = 398,
     DEC = 399,
     DECIMAL = 400,
     DECLARE = 401,
     DEFAULT = 402,
     DEFERRABLE = 403,
     DEFERRED = 404,
     DEFINER = 405,
     DELETE_P = 406,
     DELIMITER = 407,
     DELIMITERS = 408,
     DESC = 409,
     DISTINCT = 410,
     DO = 411,
     DOMAIN_P = 412,
     DOUBLE = 413,
     DROP = 414,
     EACH = 415,
     ELSE = 416,
     ENCODING = 417,
     ENCRYPTED = 418,
     END_TRANS = 419,
     ESCAPE = 420,
     EXCEPT = 421,
     EXCLUSIVE = 422,
     EXECUTE = 423,
     EXISTS = 424,
     EXPLAIN = 425,
     EXTERNAL = 426,
     EXTRACT = 427,
     FALSE_P = 428,
     FETCH = 429,
     FLOAT_P = 430,
     FOR = 431,
     FORCE = 432,
     FOREIGN = 433,
     FORWARD = 434,
     FREEZE = 435,
     FROM = 436,
     FULL = 437,
     FUNCTION = 438,
     GET = 439,
     GLOBAL = 440,
     GRANT = 441,
     GROUP_P = 442,
     HANDLER = 443,
     HAVING = 444,
     HOUR_P = 445,
     ILIKE = 446,
     IMMEDIATE = 447,
     IMMUTABLE = 448,
     IMPLICIT_P = 449,
     IN_P = 450,
     INCREMENT = 451,
     INDEX = 452,
     INHERITS = 453,
     INITIALLY = 454,
     INNER_P = 455,
     INOUT = 456,
     INPUT = 457,
     INSENSITIVE = 458,
     INSERT = 459,
     INSTEAD = 460,
     INT = 461,
     INTEGER = 462,
     INTERSECT = 463,
     INTERVAL = 464,
     INTO = 465,
     INVOKER = 466,
     IS = 467,
     ISNULL = 468,
     ISOLATION = 469,
     JOIN = 470,
     KEY = 471,
     LANCOMPILER = 472,
     LANGUAGE = 473,
     LEADING = 474,
     LEFT = 475,
     LEVEL = 476,
     LIKE = 477,
     LIMIT = 478,
     LISTEN = 479,
     LOAD = 480,
     LOCAL = 481,
     LOCATION = 482,
     LOCK_P = 483,
     MATCH = 484,
     MAXVALUE = 485,
     MINUTE_P = 486,
     MINVALUE = 487,
     MODE = 488,
     MONTH_P = 489,
     MOVE = 490,
     NAMES = 491,
     NATIONAL = 492,
     NATURAL = 493,
     NCHAR = 494,
     NEW = 495,
     NEXT = 496,
     NO = 497,
     NOCREATEDB = 498,
     NOCREATEUSER = 499,
     NONE = 500,
     NOT = 501,
     NOTHING = 502,
     NOTIFY = 503,
     NOTNULL = 504,
     NULL_P = 505,
     NULLIF = 506,
     NUMERIC = 507,
     OF = 508,
     OFF = 509,
     OFFSET = 510,
     OIDS = 511,
     OLD = 512,
     ON = 513,
     ONLY = 514,
     OPERATOR = 515,
     OPTION = 516,
     OR = 517,
     ORDER = 518,
     OUT_P = 519,
     OUTER_P = 520,
     OVERLAPS = 521,
     OVERLAY = 522,
     OWNER = 523,
     PARTIAL = 524,
     PASSWORD = 525,
     PATH_P = 526,
     PENDANT = 527,
     PLACING = 528,
     POSITION = 529,
     PRECISION = 530,
     PREPARE = 531,
     PRIMARY = 532,
     PRIOR = 533,
     PRIVILEGES = 534,
     PROCEDURE = 535,
     PROCEDURAL = 536,
     READ = 537,
     REAL = 538,
     RECHECK = 539,
     REFERENCES = 540,
     REINDEX = 541,
     RELATIVE = 542,
     RENAME = 543,
     REPLACE = 544,
     RESET = 545,
     RESTRICT = 546,
     RETURNS = 547,
     REVOKE = 548,
     RIGHT = 549,
     ROLLBACK = 550,
     ROW = 551,
     RULE = 552,
     SCHEMA = 553,
     SCROLL = 554,
     SECOND_P = 555,
     SECURITY = 556,
     SELECT = 557,
     SEQUENCE = 558,
     SERIALIZABLE = 559,
     SESSION = 560,
     SESSION_USER = 561,
     SET = 562,
     SETOF = 563,
     SHARE = 564,
     SHOW = 565,
     SIMILAR = 566,
     SIMPLE = 567,
     SMALLINT = 568,
     SOME = 569,
     STABLE = 570,
     START = 571,
     STATEMENT = 572,
     STATISTICS = 573,
     STDIN = 574,
     STDOUT = 575,
     STORAGE = 576,
     STRICT = 577,
     SUBSTRING = 578,
     SYSID = 579,
     TABLE = 580,
     TEMP = 581,
     TEMPLATE = 582,
     TEMPORARY = 583,
     THEN = 584,
     TIME = 585,
     TIMESTAMP = 586,
     TO = 587,
     TOAST = 588,
     TRAILING = 589,
     TRANSACTION = 590,
     TREAT = 591,
     TRIGGER = 592,
     TRIM = 593,
     TRUE_P = 594,
     TRUNCATE = 595,
     TRUSTED = 596,
     TYPE_P = 597,
     UNENCRYPTED = 598,
     UNION = 599,
     UNIQUE = 600,
     UNKNOWN = 601,
     UNLISTEN = 602,
     UNTIL = 603,
     UPDATE = 604,
     USAGE = 605,
     USER = 606,
     USING = 607,
     VACUUM = 608,
     VALID = 609,
     VALUES = 610,
     VARCHAR = 611,
     VARYING = 612,
     VERBOSE = 613,
     VERSION = 614,
     VIEW = 615,
     VOLATILE = 616,
     WHEN = 617,
     WHERE = 618,
     WITH = 619,
     WITHOUT = 620,
     WORK = 621,
     WRITE = 622,
     YEAR_P = 623,
     ZONE = 624,
     UNIONJOIN = 625,
     IDENT = 626,
     SCONST = 627,
     Op = 628,
     CSTRING = 629,
     CVARIABLE = 630,
     CPP_LINE = 631,
     IP = 632,
     BCONST = 633,
     XCONST = 634,
     ICONST = 635,
     PARAM = 636,
     FCONST = 637,
     POSTFIXOP = 638,
     UMINUS = 639
   };
#endif
#define SQL_ALLOCATE 258
#define SQL_AUTOCOMMIT 259
#define SQL_BOOL 260
#define SQL_BREAK 261
#define SQL_CALL 262
#define SQL_CARDINALITY 263
#define SQL_CONNECT 264
#define SQL_CONNECTION 265
#define SQL_CONTINUE 266
#define SQL_COUNT 267
#define SQL_CURRENT 268
#define SQL_DATA 269
#define SQL_DATETIME_INTERVAL_CODE 270
#define SQL_DATETIME_INTERVAL_PRECISION 271
#define SQL_DESCRIPTOR 272
#define SQL_DISCONNECT 273
#define SQL_ENUM 274
#define SQL_FOUND 275
#define SQL_FREE 276
#define SQL_GO 277
#define SQL_GOTO 278
#define SQL_IDENTIFIED 279
#define SQL_INDICATOR 280
#define SQL_KEY_MEMBER 281
#define SQL_LENGTH 282
#define SQL_LONG 283
#define SQL_NAME 284
#define SQL_NULLABLE 285
#define SQL_OCTET_LENGTH 286
#define SQL_OPEN 287
#define SQL_RELEASE 288
#define SQL_REFERENCE 289
#define SQL_RETURNED_LENGTH 290
#define SQL_RETURNED_OCTET_LENGTH 291
#define SQL_SCALE 292
#define SQL_SECTION 293
#define SQL_SHORT 294
#define SQL_SIGNED 295
#define SQL_SQL 296
#define SQL_SQLERROR 297
#define SQL_SQLPRINT 298
#define SQL_SQLWARNING 299
#define SQL_START 300
#define SQL_STOP 301
#define SQL_STRUCT 302
#define SQL_UNSIGNED 303
#define SQL_VALUE 304
#define SQL_VAR 305
#define SQL_WHENEVER 306
#define S_ADD 307
#define S_AND 308
#define S_ANYTHING 309
#define S_AUTO 310
#define S_CONST 311
#define S_DEC 312
#define S_DIV 313
#define S_DOTPOINT 314
#define S_EQUAL 315
#define S_EXTERN 316
#define S_INC 317
#define S_LSHIFT 318
#define S_MEMPOINT 319
#define S_MEMBER 320
#define S_MOD 321
#define S_MUL 322
#define S_NEQUAL 323
#define S_OR 324
#define S_REGISTER 325
#define S_RSHIFT 326
#define S_STATIC 327
#define S_SUB 328
#define S_VOLATILE 329
#define S_TYPEDEF 330
#define TYPECAST 331
#define ABORT_TRANS 332
#define ABSOLUTE 333
#define ACCESS 334
#define ACTION 335
#define ADD 336
#define AFTER 337
#define AGGREGATE 338
#define ALL 339
#define ALTER 340
#define ANALYSE 341
#define ANALYZE 342
#define AND 343
#define ANY 344
#define AS 345
#define ASC 346
#define ASSERTION 347
#define ASSIGNMENT 348
#define AT 349
#define AUTHORIZATION 350
#define BACKWARD 351
#define BEFORE 352
#define BEGIN_TRANS 353
#define BETWEEN 354
#define BIGINT 355
#define BINARY 356
#define BIT 357
#define BOTH 358
#define BOOLEAN 359
#define BY 360
#define CACHE 361
#define CALLED 362
#define CASCADE 363
#define CASE 364
#define CAST 365
#define CHAIN 366
#define CHAR_P 367
#define CHARACTER 368
#define CHARACTERISTICS 369
#define CHECK 370
#define CHECKPOINT 371
#define CLASS 372
#define CLOSE 373
#define CLUSTER 374
#define COALESCE 375
#define COLLATE 376
#define COLUMN 377
#define COMMENT 378
#define COMMIT 379
#define COMMITTED 380
#define CONSTRAINT 381
#define CONSTRAINTS 382
#define CONVERSION_P 383
#define CONVERT 384
#define COPY 385
#define CREATE 386
#define CREATEDB 387
#define CREATEUSER 388
#define CROSS 389
#define CURRENT_DATE 390
#define CURRENT_TIME 391
#define CURRENT_TIMESTAMP 392
#define CURRENT_USER 393
#define CURSOR 394
#define CYCLE 395
#define DATABASE 396
#define DAY_P 397
#define DEALLOCATE 398
#define DEC 399
#define DECIMAL 400
#define DECLARE 401
#define DEFAULT 402
#define DEFERRABLE 403
#define DEFERRED 404
#define DEFINER 405
#define DELETE_P 406
#define DELIMITER 407
#define DELIMITERS 408
#define DESC 409
#define DISTINCT 410
#define DO 411
#define DOMAIN_P 412
#define DOUBLE 413
#define DROP 414
#define EACH 415
#define ELSE 416
#define ENCODING 417
#define ENCRYPTED 418
#define END_TRANS 419
#define ESCAPE 420
#define EXCEPT 421
#define EXCLUSIVE 422
#define EXECUTE 423
#define EXISTS 424
#define EXPLAIN 425
#define EXTERNAL 426
#define EXTRACT 427
#define FALSE_P 428
#define FETCH 429
#define FLOAT_P 430
#define FOR 431
#define FORCE 432
#define FOREIGN 433
#define FORWARD 434
#define FREEZE 435
#define FROM 436
#define FULL 437
#define FUNCTION 438
#define GET 439
#define GLOBAL 440
#define GRANT 441
#define GROUP_P 442
#define HANDLER 443
#define HAVING 444
#define HOUR_P 445
#define ILIKE 446
#define IMMEDIATE 447
#define IMMUTABLE 448
#define IMPLICIT_P 449
#define IN_P 450
#define INCREMENT 451
#define INDEX 452
#define INHERITS 453
#define INITIALLY 454
#define INNER_P 455
#define INOUT 456
#define INPUT 457
#define INSENSITIVE 458
#define INSERT 459
#define INSTEAD 460
#define INT 461
#define INTEGER 462
#define INTERSECT 463
#define INTERVAL 464
#define INTO 465
#define INVOKER 466
#define IS 467
#define ISNULL 468
#define ISOLATION 469
#define JOIN 470
#define KEY 471
#define LANCOMPILER 472
#define LANGUAGE 473
#define LEADING 474
#define LEFT 475
#define LEVEL 476
#define LIKE 477
#define LIMIT 478
#define LISTEN 479
#define LOAD 480
#define LOCAL 481
#define LOCATION 482
#define LOCK_P 483
#define MATCH 484
#define MAXVALUE 485
#define MINUTE_P 486
#define MINVALUE 487
#define MODE 488
#define MONTH_P 489
#define MOVE 490
#define NAMES 491
#define NATIONAL 492
#define NATURAL 493
#define NCHAR 494
#define NEW 495
#define NEXT 496
#define NO 497
#define NOCREATEDB 498
#define NOCREATEUSER 499
#define NONE 500
#define NOT 501
#define NOTHING 502
#define NOTIFY 503
#define NOTNULL 504
#define NULL_P 505
#define NULLIF 506
#define NUMERIC 507
#define OF 508
#define OFF 509
#define OFFSET 510
#define OIDS 511
#define OLD 512
#define ON 513
#define ONLY 514
#define OPERATOR 515
#define OPTION 516
#define OR 517
#define ORDER 518
#define OUT_P 519
#define OUTER_P 520
#define OVERLAPS 521
#define OVERLAY 522
#define OWNER 523
#define PARTIAL 524
#define PASSWORD 525
#define PATH_P 526
#define PENDANT 527
#define PLACING 528
#define POSITION 529
#define PRECISION 530
#define PREPARE 531
#define PRIMARY 532
#define PRIOR 533
#define PRIVILEGES 534
#define PROCEDURE 535
#define PROCEDURAL 536
#define READ 537
#define REAL 538
#define RECHECK 539
#define REFERENCES 540
#define REINDEX 541
#define RELATIVE 542
#define RENAME 543
#define REPLACE 544
#define RESET 545
#define RESTRICT 546
#define RETURNS 547
#define REVOKE 548
#define RIGHT 549
#define ROLLBACK 550
#define ROW 551
#define RULE 552
#define SCHEMA 553
#define SCROLL 554
#define SECOND_P 555
#define SECURITY 556
#define SELECT 557
#define SEQUENCE 558
#define SERIALIZABLE 559
#define SESSION 560
#define SESSION_USER 561
#define SET 562
#define SETOF 563
#define SHARE 564
#define SHOW 565
#define SIMILAR 566
#define SIMPLE 567
#define SMALLINT 568
#define SOME 569
#define STABLE 570
#define START 571
#define STATEMENT 572
#define STATISTICS 573
#define STDIN 574
#define STDOUT 575
#define STORAGE 576
#define STRICT 577
#define SUBSTRING 578
#define SYSID 579
#define TABLE 580
#define TEMP 581
#define TEMPLATE 582
#define TEMPORARY 583
#define THEN 584
#define TIME 585
#define TIMESTAMP 586
#define TO 587
#define TOAST 588
#define TRAILING 589
#define TRANSACTION 590
#define TREAT 591
#define TRIGGER 592
#define TRIM 593
#define TRUE_P 594
#define TRUNCATE 595
#define TRUSTED 596
#define TYPE_P 597
#define UNENCRYPTED 598
#define UNION 599
#define UNIQUE 600
#define UNKNOWN 601
#define UNLISTEN 602
#define UNTIL 603
#define UPDATE 604
#define USAGE 605
#define USER 606
#define USING 607
#define VACUUM 608
#define VALID 609
#define VALUES 610
#define VARCHAR 611
#define VARYING 612
#define VERBOSE 613
#define VERSION 614
#define VIEW 615
#define VOLATILE 616
#define WHEN 617
#define WHERE 618
#define WITH 619
#define WITHOUT 620
#define WORK 621
#define WRITE 622
#define YEAR_P 623
#define ZONE 624
#define UNIONJOIN 625
#define IDENT 626
#define SCONST 627
#define Op 628
#define CSTRING 629
#define CVARIABLE 630
#define CPP_LINE 631
#define IP 632
#define BCONST 633
#define XCONST 634
#define ICONST 635
#define PARAM 636
#define FCONST 637
#define POSTFIXOP 638
#define UMINUS 639




/* Copy the first part of user declarations.  */
#line 3 "preproc.y"

#include "postgres_fe.h"

#include "extern.h"

/*
 * Variables containing simple states.
 */
int struct_level = 0;
int braces_open; /* brace level counter */
char	errortext[128];
char	*connection = NULL;
char	*input_filename = NULL;

static int	QueryIsRule = 0, FoundInto = 0;
static int	initializer = 0;
static struct this_type actual_type[STRUCT_DEPTH];
static char *actual_storage[STRUCT_DEPTH];
static char *actual_startline[STRUCT_DEPTH];

/* temporarily store struct members while creating the data structure */
struct ECPGstruct_member *struct_member_list[STRUCT_DEPTH] = { NULL };

/* also store struct type so we can do a sizeof() later */
static char *ECPGstruct_sizeof = NULL;

struct ECPGtype ecpg_no_indicator = {ECPGt_NO_INDICATOR, 0L, NULL, {NULL}};
struct variable no_indicator = {"no_indicator", &ecpg_no_indicator, 0, NULL};

struct ECPGtype ecpg_query = {ECPGt_char_variable, 0L, NULL, {NULL}};

/*
 * Handle parsing errors and warnings
 */
void
mmerror(int error_code, enum errortype type, char * error)
{
	switch(type)
	{
		case ET_WARNING:
			fprintf(stderr, "%s:%d: WARNING: %s\n", input_filename, yylineno, error);
			break;
		case ET_ERROR:
			fprintf(stderr, "%s:%d: ERROR: %s\n", input_filename, yylineno, error);
			ret_value = error_code;
			break;
		case ET_FATAL:
			fprintf(stderr, "%s:%d: ERROR: %s\n", input_filename, yylineno, error);
			exit(error_code);
	}
}

/*
 * string concatenation
 */

static char *
cat2_str(char *str1, char *str2)
{
	char * res_str	= (char *)mm_alloc(strlen(str1) + strlen(str2) + 2);

	strcpy(res_str, str1);
	strcat(res_str, " ");
	strcat(res_str, str2);
	free(str1);
	free(str2);
	return(res_str);
}

static char *
cat_str(int count, ...)
{
	va_list		args;
	int			i;
	char		*res_str;

	va_start(args, count);

	res_str = va_arg(args, char *);

	/* now add all other strings */
	for (i = 1; i < count; i++)
		res_str = cat2_str(res_str, va_arg(args, char *));

	va_end(args);

	return(res_str);
}

char *
make_str(const char *str)
{
	char * res_str = (char *)mm_alloc(strlen(str) + 1);

	strcpy(res_str, str);
	return res_str;
}

static char *
make2_str(char *str1, char *str2)
{
	char * res_str	= (char *)mm_alloc(strlen(str1) + strlen(str2) + 1);

	strcpy(res_str, str1);
	strcat(res_str, str2);
	free(str1);
	free(str2);
	return(res_str);
}

static char *
make3_str(char *str1, char *str2, char *str3)
{
	char * res_str	= (char *)mm_alloc(strlen(str1) + strlen(str2) +strlen(str3) + 1);

	strcpy(res_str, str1);
	strcat(res_str, str2);
	strcat(res_str, str3);
	free(str1);
	free(str2);
	free(str3);
	return(res_str);
}

static char *
make_name(void)
{
	char * name = (char *)mm_alloc(yyleng + 1);

	strncpy(name, yytext, yyleng);
	name[yyleng] = '\0';
	return(name);
}



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 139 "preproc.y"
typedef union YYSTYPE {
	double	dval;
	int		ival;
	char	*str;
	struct	when		action;
	struct	index		index;
	int		tagname;
	struct	this_type	type;
	enum	ECPGttype	type_enum;
	enum	ECPGdtype	dtype_enum;
	struct	fetch_desc	descriptor;
} YYSTYPE;
/* Line 191 of yacc.c.  */
#line 992 "y.tab.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 1004 "y.tab.c"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   38764

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  405
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  466
/* YYNRULES -- Number of rules. */
#define YYNRULES  1724
/* YYNRULES -- Number of states. */
#define YYNSTATES  2806

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   640

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned short yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   391,     2,     2,
     396,   397,   389,   387,   402,   388,   398,   390,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   404,   399,
     384,   383,   385,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   394,     2,   395,   392,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   400,     2,   401,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   386,   393,
     403
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short yyprhs[] =
{
       0,     0,     3,     5,     6,     9,    14,    18,    20,    22,
      24,    26,    28,    31,    33,    35,    37,    39,    41,    43,
      45,    47,    49,    51,    53,    55,    57,    59,    61,    63,
      65,    67,    69,    71,    73,    75,    77,    79,    81,    83,
      85,    87,    89,    91,    93,    95,    97,    99,   101,   103,
     105,   107,   109,   111,   113,   115,   117,   119,   121,   123,
     125,   127,   129,   131,   133,   135,   137,   139,   141,   143,
     145,   147,   149,   151,   153,   155,   157,   159,   161,   163,
     165,   167,   169,   171,   173,   175,   177,   179,   181,   183,
     185,   187,   189,   191,   193,   199,   201,   202,   207,   213,
     219,   224,   228,   231,   232,   235,   238,   240,   242,   244,
     246,   250,   254,   258,   260,   265,   271,   274,   275,   278,
     281,   288,   295,   299,   307,   312,   314,   315,   318,   319,
     321,   323,   325,   328,   332,   336,   340,   344,   348,   354,
     362,   365,   369,   373,   375,   377,   379,   383,   386,   388,
     391,   394,   395,   397,   399,   401,   403,   405,   407,   409,
     411,   413,   417,   424,   426,   428,   430,   432,   433,   435,
     437,   440,   444,   449,   453,   456,   459,   463,   468,   472,
     475,   480,   482,   484,   486,   488,   490,   497,   505,   515,
     525,   535,   545,   553,   559,   567,   574,   581,   585,   588,
     590,   592,   593,   596,   598,   599,   609,   611,   613,   615,
     617,   619,   622,   623,   625,   627,   631,   635,   637,   638,
     641,   642,   646,   647,   649,   650,   660,   671,   673,   675,
     678,   681,   684,   687,   688,   690,   691,   693,   697,   699,
     701,   703,   708,   711,   712,   716,   718,   720,   723,   725,
     727,   730,   735,   738,   744,   746,   749,   752,   755,   758,
     762,   764,   769,   774,   780,   792,   796,   797,   801,   803,
     805,   808,   811,   812,   814,   816,   819,   822,   823,   827,
     831,   834,   836,   838,   841,   844,   849,   850,   853,   856,
     857,   858,   867,   871,   872,   876,   878,   880,   886,   889,
     890,   893,   895,   898,   901,   904,   907,   916,   918,   919,
     921,   923,   926,   927,   933,   935,   936,   951,   971,   973,
     975,   977,   981,   987,   989,   991,   993,   997,   999,  1000,
    1002,  1004,  1006,  1010,  1011,  1013,  1015,  1016,  1019,  1021,
    1024,  1026,  1029,  1032,  1034,  1037,  1040,  1047,  1056,  1060,
    1065,  1070,  1075,  1081,  1090,  1094,  1098,  1100,  1104,  1108,
    1110,  1112,  1114,  1116,  1129,  1131,  1135,  1140,  1148,  1153,
    1156,  1158,  1159,  1161,  1162,  1170,  1175,  1177,  1179,  1181,
    1183,  1185,  1187,  1189,  1191,  1193,  1197,  1199,  1201,  1205,
    1212,  1218,  1224,  1229,  1233,  1239,  1244,  1249,  1253,  1256,
    1258,  1260,  1262,  1264,  1266,  1268,  1270,  1272,  1274,  1276,
    1283,  1293,  1301,  1311,  1320,  1329,  1336,  1338,  1340,  1342,
    1344,  1346,  1348,  1350,  1352,  1354,  1356,  1358,  1366,  1374,
    1377,  1379,  1381,  1383,  1387,  1389,  1391,  1393,  1395,  1397,
    1399,  1401,  1403,  1405,  1407,  1409,  1411,  1413,  1416,  1419,
    1422,  1425,  1428,  1430,  1434,  1436,  1439,  1443,  1444,  1448,
    1449,  1451,  1455,  1458,  1470,  1472,  1473,  1476,  1477,  1479,
    1481,  1485,  1487,  1493,  1496,  1498,  1501,  1502,  1512,  1515,
    1516,  1520,  1523,  1525,  1529,  1532,  1534,  1536,  1538,  1540,
    1542,  1546,  1548,  1550,  1555,  1557,  1560,  1563,  1566,  1568,
    1570,  1572,  1577,  1583,  1585,  1589,  1593,  1596,  1599,  1602,
    1603,  1609,  1617,  1619,  1621,  1629,  1631,  1635,  1639,  1643,
    1645,  1649,  1661,  1672,  1675,  1676,  1685,  1690,  1695,  1697,
    1699,  1701,  1702,  1711,  1720,  1722,  1723,  1725,  1726,  1727,
    1742,  1744,  1746,  1750,  1754,  1756,  1758,  1760,  1762,  1764,
    1766,  1768,  1769,  1771,  1773,  1775,  1777,  1779,  1780,  1787,
    1790,  1793,  1796,  1799,  1802,  1805,  1809,  1812,  1815,  1818,
    1822,  1824,  1826,  1827,  1835,  1838,  1844,  1848,  1850,  1853,
    1857,  1861,  1865,  1869,  1873,  1877,  1881,  1885,  1887,  1888,
    1894,  1899,  1903,  1911,  1913,  1914,  1925,  1930,  1935,  1941,
    1947,  1950,  1955,  1957,  1959,  1961,  1962,  1964,  1965,  1967,
    1968,  1972,  1973,  1977,  1982,  1984,  1986,  1988,  1990,  1992,
    1997,  2002,  2005,  2007,  2015,  2020,  2024,  2026,  2029,  2034,
    2039,  2043,  2044,  2047,  2050,  2053,  2057,  2059,  2063,  2065,
    2068,  2075,  2082,  2084,  2086,  2088,  2091,  2092,  2094,  2096,
    2100,  2104,  2106,  2109,  2114,  2119,  2121,  2123,  2132,  2137,
    2142,  2147,  2150,  2152,  2153,  2157,  2161,  2166,  2171,  2176,
    2181,  2184,  2186,  2188,  2189,  2191,  2192,  2194,  2200,  2202,
    2203,  2205,  2206,  2210,  2212,  2216,  2219,  2222,  2224,  2226,
    2227,  2232,  2237,  2240,  2243,  2248,  2250,  2251,  2253,  2255,
    2257,  2259,  2261,  2265,  2266,  2269,  2270,  2274,  2278,  2280,
    2281,  2284,  2285,  2288,  2289,  2293,  2295,  2297,  2300,  2302,
    2305,  2311,  2318,  2324,  2326,  2329,  2331,  2336,  2340,  2345,
    2349,  2355,  2360,  2366,  2371,  2377,  2380,  2385,  2387,  2390,
    2393,  2396,  2398,  2400,  2401,  2406,  2409,  2411,  2414,  2417,
    2422,  2426,  2431,  2434,  2435,  2437,  2441,  2444,  2447,  2450,
    2454,  2459,  2460,  2462,  2466,  2470,  2474,  2478,  2482,  2486,
    2488,  2490,  2492,  2494,  2496,  2499,  2505,  2508,  2510,  2512,
    2514,  2516,  2518,  2520,  2522,  2524,  2526,  2528,  2530,  2533,
    2536,  2539,  2542,  2545,  2547,  2551,  2552,  2558,  2562,  2563,
    2569,  2573,  2574,  2576,  2578,  2580,  2582,  2588,  2591,  2593,
    2595,  2597,  2599,  2605,  2608,  2611,  2614,  2616,  2620,  2624,
    2627,  2629,  2630,  2634,  2635,  2638,  2639,  2645,  2648,  2654,
    2657,  2659,  2663,  2667,  2668,  2670,  2672,  2674,  2676,  2678,
    2680,  2684,  2688,  2692,  2696,  2700,  2704,  2708,  2709,  2713,
    2718,  2723,  2727,  2731,  2735,  2740,  2744,  2750,  2755,  2760,
    2764,  2768,  2772,  2776,  2778,  2780,  2782,  2784,  2786,  2788,
    2790,  2792,  2794,  2796,  2798,  2800,  2802,  2804,  2806,  2808,
    2813,  2815,  2820,  2822,  2826,  2832,  2835,  2838,  2841,  2844,
    2847,  2850,  2854,  2858,  2862,  2866,  2870,  2874,  2878,  2882,
    2886,  2890,  2893,  2896,  2900,  2904,  2907,  2911,  2917,  2922,
    2929,  2933,  2939,  2944,  2951,  2956,  2963,  2969,  2977,  2980,
    2984,  2987,  2992,  2996,  3001,  3005,  3010,  3014,  3019,  3025,
    3032,  3040,  3046,  3053,  3057,  3062,  3067,  3070,  3072,  3074,
    3078,  3081,  3084,  3087,  3090,  3093,  3097,  3101,  3105,  3109,
    3113,  3117,  3121,  3125,  3129,  3133,  3136,  3139,  3145,  3152,
    3160,  3162,  3164,  3168,  3172,  3178,  3180,  3184,  3189,  3195,
    3201,  3206,  3208,  3211,  3216,  3219,  3224,  3227,  3230,  3233,
    3240,  3245,  3250,  3255,  3260,  3267,  3273,  3279,  3285,  3290,
    3295,  3297,  3300,  3305,  3312,  3313,  3315,  3319,  3323,  3327,
    3328,  3332,  3334,  3336,  3338,  3340,  3342,  3344,  3346,  3348,
    3350,  3355,  3359,  3362,  3366,  3367,  3371,  3375,  3378,  3381,
    3383,  3384,  3387,  3390,  3394,  3397,  3399,  3403,  3405,  3406,
    3408,  3412,  3414,  3418,  3420,  3424,  3430,  3437,  3442,  3445,
    3447,  3452,  3455,  3456,  3458,  3459,  3462,  3465,  3468,  3471,
    3474,  3478,  3481,  3482,  3486,  3488,  3492,  3494,  3496,  3500,
    3502,  3504,  3509,  3513,  3515,  3517,  3519,  3521,  3523,  3525,
    3529,  3531,  3533,  3535,  3539,  3541,  3543,  3545,  3547,  3549,
    3551,  3553,  3555,  3557,  3560,  3564,  3571,  3574,  3576,  3578,
    3580,  3582,  3584,  3586,  3588,  3590,  3592,  3594,  3596,  3598,
    3601,  3603,  3605,  3607,  3609,  3611,  3613,  3615,  3618,  3621,
    3623,  3625,  3627,  3629,  3631,  3633,  3635,  3637,  3639,  3641,
    3643,  3645,  3651,  3655,  3658,  3662,  3670,  3672,  3675,  3678,
    3680,  3681,  3683,  3687,  3689,  3692,  3693,  3696,  3697,  3700,
    3701,  3703,  3707,  3712,  3716,  3718,  3720,  3722,  3725,  3726,
    3733,  3737,  3740,  3741,  3746,  3752,  3758,  3759,  3761,  3763,
    3765,  3768,  3771,  3773,  3776,  3777,  3785,  3786,  3787,  3795,
    3797,  3799,  3801,  3803,  3804,  3806,  3808,  3809,  3811,  3813,
    3815,  3817,  3819,  3823,  3826,  3830,  3835,  3840,  3843,  3846,
    3848,  3851,  3854,  3858,  3860,  3863,  3866,  3870,  3874,  3879,
    3882,  3884,  3887,  3889,  3891,  3894,  3897,  3901,  3903,  3905,
    3907,  3908,  3910,  3914,  3919,  3920,  3923,  3924,  3926,  3929,
    3933,  3936,  3938,  3940,  3942,  3943,  3945,  3947,  3951,  3952,
    3958,  3960,  3962,  3965,  3969,  3970,  3973,  3974,  3976,  3979,
    3984,  3985,  3987,  3989,  3991,  3993,  3997,  4002,  4006,  4010,
    4014,  4016,  4020,  4022,  4024,  4026,  4028,  4030,  4032,  4034,
    4036,  4038,  4040,  4042,  4044,  4046,  4048,  4050,  4052,  4056,
    4058,  4062,  4067,  4074,  4081,  4084,  4089,  4094,  4096,  4098,
    4103,  4108,  4109,  4117,  4121,  4125,  4130,  4135,  4136,  4138,
    4139,  4140,  4148,  4152,  4157,  4161,  4163,  4165,  4167,  4170,
    4174,  4180,  4183,  4189,  4191,  4193,  4195,  4197,  4199,  4201,
    4203,  4205,  4207,  4209,  4211,  4213,  4215,  4217,  4219,  4221,
    4223,  4225,  4227,  4229,  4231,  4233,  4235,  4237,  4239,  4241,
    4243,  4245,  4247,  4249,  4251,  4253,  4255,  4257,  4259,  4261,
    4263,  4265,  4267,  4268,  4270,  4272,  4274,  4276,  4278,  4280,
    4282,  4284,  4286,  4288,  4290,  4292,  4294,  4296,  4298,  4300,
    4302,  4304,  4306,  4308,  4310,  4312,  4314,  4316,  4318,  4320,
    4322,  4324,  4326,  4328,  4330,  4332,  4334,  4336,  4338,  4340,
    4342,  4344,  4346,  4348,  4350,  4352,  4354,  4356,  4358,  4360,
    4362,  4364,  4366,  4368,  4370,  4372,  4374,  4376,  4378,  4380,
    4382,  4384,  4386,  4388,  4390,  4392,  4394,  4396,  4398,  4400,
    4402,  4404,  4406,  4408,  4410,  4412,  4414,  4416,  4418,  4420,
    4422,  4424,  4426,  4428,  4430,  4432,  4434,  4436,  4438,  4440,
    4442,  4444,  4446,  4448,  4450,  4452,  4454,  4456,  4458,  4460,
    4462,  4464,  4466,  4468,  4470,  4472,  4474,  4476,  4478,  4480,
    4482,  4484,  4486,  4488,  4490,  4492,  4494,  4496,  4498,  4500,
    4502,  4504,  4506,  4508,  4510,  4512,  4514,  4516,  4518,  4520,
    4522,  4524,  4526,  4528,  4530,  4532,  4534,  4536,  4538,  4540,
    4542,  4544,  4546,  4548,  4550,  4552,  4554,  4556,  4558,  4560,
    4562,  4564,  4566,  4568,  4570,  4572,  4574,  4576,  4578,  4580,
    4582,  4584,  4586,  4588,  4590,  4592,  4594,  4596,  4598,  4600,
    4602,  4604,  4606,  4608,  4610,  4612,  4614,  4616,  4618,  4620,
    4622,  4624,  4626,  4628,  4630,  4632,  4634,  4636,  4638,  4640,
    4642,  4644,  4646,  4648,  4650,  4652,  4654,  4656,  4658,  4660,
    4662,  4664,  4666,  4668,  4670,  4672,  4674,  4676,  4678,  4680,
    4682,  4684,  4686,  4688,  4690,  4692,  4694,  4696,  4698,  4700,
    4702,  4704,  4706,  4708,  4710,  4712,  4714,  4716,  4718,  4720,
    4722,  4724,  4726,  4728,  4730,  4732,  4734,  4736,  4738,  4740,
    4742,  4744,  4746,  4748,  4750,  4752,  4754,  4756,  4758,  4760,
    4762,  4764,  4766,  4768,  4770,  4772,  4774,  4776,  4778,  4780,
    4782,  4784,  4786,  4788,  4790,  4792,  4794,  4796,  4798,  4800,
    4802,  4804,  4806,  4808,  4810,  4812,  4814,  4816,  4818,  4820,
    4822,  4824,  4826,  4828,  4830,  4832,  4834,  4836,  4838,  4840,
    4842,  4844,  4846,  4848,  4850,  4852,  4854,  4856,  4858,  4860,
    4862,  4864,  4866,  4868,  4870,  4872,  4874,  4878,  4880,  4881,
    4883,  4886,  4888,  4891,  4893,  4895,  4898,  4901,  4903,  4905,
    4907,  4909,  4911,  4914,  4918,  4920,  4923,  4925,  4929,  4931,
    4935,  4937,  4939,  4941,  4943,  4945,  4947,  4949,  4951,  4953,
    4955,  4957,  4959,  4961,  4963,  4965,  4967,  4969,  4971,  4973,
    4975,  4977,  4979,  4981,  4983,  4985,  4987,  4989,  4991,  4993,
    4995,  4997,  4999,  5001,  5003,  5005,  5007,  5009,  5011,  5013,
    5015,  5017,  5019,  5021,  5023,  5025,  5027,  5029,  5031,  5033,
    5035,  5037,  5039,  5041,  5043
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short yyrhs[] =
{
     406,     0,    -1,   407,    -1,    -1,   407,   408,    -1,   857,
     409,   410,   399,    -1,   857,   410,   399,    -1,   777,    -1,
     869,    -1,   376,    -1,   400,    -1,   401,    -1,    94,   763,
      -1,   604,    -1,   422,    -1,   445,    -1,   413,    -1,   414,
      -1,   448,    -1,   533,    -1,   450,    -1,   459,    -1,   482,
      -1,   574,    -1,   606,    -1,   556,    -1,   424,    -1,   419,
      -1,   487,    -1,   490,    -1,   510,    -1,   518,    -1,   496,
      -1,   411,    -1,   609,    -1,   512,    -1,   524,    -1,   528,
      -1,   576,    -1,   423,    -1,   523,    -1,   494,    -1,   511,
      -1,   509,    -1,   591,    -1,   415,    -1,   617,    -1,   529,
      -1,   536,    -1,   548,    -1,   593,    -1,   594,    -1,   624,
      -1,   592,    -1,   577,    -1,   569,    -1,   571,    -1,   568,
      -1,   580,    -1,   537,    -1,   618,    -1,   583,    -1,   595,
      -1,   598,    -1,   599,    -1,   600,    -1,   605,    -1,   610,
      -1,   611,    -1,   428,    -1,   439,    -1,   440,    -1,   441,
      -1,   444,    -1,   608,    -1,   822,    -1,   762,    -1,   775,
      -1,   776,    -1,   821,    -1,   806,    -1,   807,    -1,   810,
      -1,   813,    -1,   830,    -1,   829,    -1,   814,    -1,   820,
      -1,   831,    -1,   832,    -1,   834,    -1,   835,    -1,   839,
      -1,   841,    -1,   131,   351,   760,   412,   416,    -1,   364,
      -1,    -1,    85,   351,   760,   416,    -1,    85,   351,   760,
     364,   416,    -1,    85,   351,   760,   307,   429,    -1,    85,
     351,   760,   440,    -1,   159,   351,   418,    -1,   416,   417,
      -1,    -1,   270,   752,    -1,   324,   753,    -1,   132,    -1,
     243,    -1,   133,    -1,   244,    -1,   195,   187,   418,    -1,
     354,   348,   752,    -1,   418,   402,   760,    -1,   760,    -1,
     131,   187,   760,   420,    -1,   131,   187,   760,   364,   420,
      -1,   420,   421,    -1,    -1,   351,   418,    -1,   324,   753,
      -1,    85,   187,   760,    81,   351,   418,    -1,    85,   187,
     760,   159,   351,   418,    -1,   159,   187,   760,    -1,   131,
     298,   760,   425,    95,   760,   426,    -1,   131,   298,   847,
     426,    -1,   847,    -1,    -1,   426,   427,    -1,    -1,   459,
      -1,   536,    -1,   598,    -1,   307,   429,    -1,   307,   226,
     429,    -1,   307,   305,   429,    -1,   847,   332,   430,    -1,
     847,   403,   430,    -1,   330,   369,   436,    -1,   335,   214,
     221,   432,   433,    -1,   305,   114,    90,   335,   214,   221,
     432,    -1,   236,   437,    -1,   305,    95,   438,    -1,   305,
      95,   147,    -1,   431,    -1,   147,    -1,   434,    -1,   431,
     402,   434,    -1,   282,   125,    -1,   304,    -1,   282,   367,
      -1,   282,   259,    -1,    -1,   435,    -1,   758,    -1,   847,
      -1,   339,    -1,   173,    -1,   258,    -1,   254,    -1,   758,
      -1,   863,    -1,   690,   755,   692,    -1,   690,   396,   753,
     397,   755,   692,    -1,   147,    -1,   226,    -1,   755,    -1,
     147,    -1,    -1,   847,    -1,   755,    -1,   310,   847,    -1,
     310,   330,   369,    -1,   310,   335,   214,   221,    -1,   310,
     305,    95,    -1,   310,    84,    -1,   290,   847,    -1,   290,
     330,   369,    -1,   290,   335,   214,   221,    -1,   290,   305,
      95,    -1,   290,    84,    -1,   307,   127,   442,   443,    -1,
      84,    -1,   739,    -1,   149,    -1,   192,    -1,   116,    -1,
      85,   325,   662,    81,   582,   464,    -1,    85,   325,   662,
      85,   582,   847,   446,    -1,    85,   325,   662,    85,   582,
     847,   159,   246,   250,    -1,    85,   325,   662,    85,   582,
     847,   307,   246,   250,    -1,    85,   325,   662,    85,   582,
     847,   307,   318,   753,    -1,    85,   325,   662,    85,   582,
     847,   307,   321,   847,    -1,    85,   325,   662,   159,   582,
     847,   447,    -1,    85,   325,   662,    81,   470,    -1,    85,
     325,   662,   159,   126,   740,   447,    -1,    85,   325,   738,
     131,   333,   325,    -1,    85,   325,   738,   268,   332,   760,
      -1,   307,   147,   702,    -1,   159,   147,    -1,   108,    -1,
     291,    -1,    -1,   118,   449,    -1,   847,    -1,    -1,   130,
     455,   738,   456,   451,   452,   457,   412,   453,    -1,   332,
      -1,   181,    -1,   755,    -1,   319,    -1,   320,    -1,   453,
     454,    -1,    -1,   101,    -1,   256,    -1,   152,   607,   755,
      -1,   250,   607,   755,    -1,   101,    -1,    -1,   364,   256,
      -1,    -1,   458,   153,   755,    -1,    -1,   352,    -1,    -1,
     131,   460,   325,   738,   396,   461,   397,   480,   481,    -1,
     131,   460,   325,   738,   253,   738,   396,   461,   397,   481,
      -1,   328,    -1,   326,    -1,   226,   328,    -1,   226,   326,
      -1,   185,   328,    -1,   185,   326,    -1,    -1,   462,    -1,
      -1,   463,    -1,   462,   402,   463,    -1,   464,    -1,   469,
      -1,   470,    -1,   847,   667,   465,   688,    -1,   465,   466,
      -1,    -1,   126,   740,   467,    -1,   467,    -1,   468,    -1,
     246,   250,    -1,   250,    -1,   345,    -1,   277,   216,    -1,
     115,   396,   702,   397,    -1,   147,   703,    -1,   285,   738,
     472,   475,   476,    -1,   148,    -1,   246,   148,    -1,   199,
     149,    -1,   199,   192,    -1,   222,   527,    -1,   126,   740,
     471,    -1,   471,    -1,   115,   396,   702,   397,    -1,   345,
     396,   473,   397,    -1,   277,   216,   396,   473,   397,    -1,
     178,   216,   396,   473,   397,   285,   738,   472,   475,   476,
     506,    -1,   396,   473,   397,    -1,    -1,   473,   402,   474,
      -1,   474,    -1,   847,    -1,   229,   182,    -1,   229,   269,
      -1,    -1,   477,    -1,   478,    -1,   477,   478,    -1,   478,
     477,    -1,    -1,   258,   151,   479,    -1,   258,   349,   479,
      -1,   242,    80,    -1,   291,    -1,   108,    -1,   307,   147,
      -1,   307,   250,    -1,   198,   396,   737,   397,    -1,    -1,
     364,   256,    -1,   365,   256,    -1,    -1,    -1,   131,   460,
     325,   738,   484,    90,   483,   630,    -1,   396,   485,   397,
      -1,    -1,   485,   402,   486,    -1,   486,    -1,   847,    -1,
     131,   460,   303,   738,   488,    -1,   488,   489,    -1,    -1,
     106,   757,    -1,   140,    -1,   196,   757,    -1,   230,   757,
      -1,   232,   757,    -1,   316,   757,    -1,   131,   491,   495,
     218,   438,   188,   492,   493,    -1,   341,    -1,    -1,   740,
      -1,   727,    -1,   217,   755,    -1,    -1,   159,   495,   218,
     755,   447,    -1,   281,    -1,    -1,   131,   337,   740,   497,
     498,   258,   738,   500,   168,   280,   740,   396,   503,   397,
      -1,   131,   126,   337,   740,    82,   498,   258,   738,   505,
     506,   176,   160,   296,   168,   280,   746,   396,   503,   397,
      -1,    97,    -1,    82,    -1,   499,    -1,   499,   262,   499,
      -1,   499,   262,   499,   262,   499,    -1,   204,    -1,   151,
      -1,   349,    -1,   176,   501,   502,    -1,   160,    -1,    -1,
     296,    -1,   317,    -1,   504,    -1,   503,   402,   504,    -1,
      -1,   759,    -1,   847,    -1,    -1,   181,   738,    -1,   507,
      -1,   507,   508,    -1,   508,    -1,   508,   507,    -1,   246,
     148,    -1,   148,    -1,   199,   192,    -1,   199,   149,    -1,
     159,   337,   740,   258,   738,   447,    -1,   131,    92,   740,
     115,   396,   702,   397,   506,    -1,   159,    92,   740,    -1,
     131,    83,   746,   514,    -1,   131,   260,   698,   514,    -1,
     131,   342,   527,   514,    -1,   131,   342,   527,    90,   513,
      -1,   131,   113,   307,   607,   527,   184,   514,   688,    -1,
     396,   665,   397,    -1,   396,   515,   397,    -1,   516,    -1,
     515,   402,   516,    -1,   850,   383,   517,    -1,   850,    -1,
     563,    -1,   701,    -1,   758,    -1,   131,   260,   117,   527,
     521,   176,   342,   667,   352,   742,    90,   519,    -1,   520,
      -1,   519,   402,   520,    -1,   260,   753,   573,   522,    -1,
     260,   753,   573,   396,   572,   397,   522,    -1,   183,   753,
     746,   558,    -1,   321,   667,    -1,   147,    -1,    -1,   284,
      -1,    -1,   159,   260,   117,   527,   352,   742,   447,    -1,
     159,   525,   526,   447,    -1,   325,    -1,   303,    -1,   360,
      -1,   197,    -1,   342,    -1,   157,    -1,   128,    -1,   298,
      -1,   527,    -1,   526,   402,   527,    -1,   847,    -1,   727,
      -1,   340,   637,   738,    -1,   174,   530,   531,   532,   740,
     817,    -1,   174,   531,   532,   740,   817,    -1,   174,   530,
     532,   740,   817,    -1,   174,   532,   740,   817,    -1,   174,
     740,   817,    -1,   235,   530,   531,   532,   740,    -1,   235,
     531,   532,   740,    -1,   235,   530,   532,   740,    -1,   235,
     532,   740,    -1,   235,   740,    -1,   179,    -1,    96,    -1,
     287,    -1,    78,    -1,   754,    -1,    84,    -1,   241,    -1,
     278,    -1,   195,    -1,   181,    -1,   123,   258,   534,   740,
     212,   535,    -1,   123,   258,    83,   746,   396,   570,   397,
     212,   535,    -1,   123,   258,   183,   746,   558,   212,   535,
      -1,   123,   258,   260,   698,   396,   572,   397,   212,   535,
      -1,   123,   258,   337,   740,   258,   527,   212,   535,    -1,
     123,   258,   297,   740,   258,   527,   212,   535,    -1,   123,
     258,   297,   740,   212,   535,    -1,   122,    -1,   141,    -1,
     298,    -1,   197,    -1,   303,    -1,   325,    -1,   157,    -1,
     342,    -1,   360,    -1,   755,    -1,   250,    -1,   186,   538,
     258,   541,   332,   542,   544,    -1,   293,   545,   538,   258,
     541,   181,   542,    -1,    84,   279,    -1,    84,    -1,   539,
      -1,   540,    -1,   539,   402,   540,    -1,   302,    -1,   204,
      -1,   349,    -1,   151,    -1,   297,    -1,   285,    -1,   337,
      -1,   168,    -1,   350,    -1,   131,    -1,   328,    -1,   326,
      -1,   737,    -1,   325,   737,    -1,   183,   546,    -1,   141,
     739,    -1,   218,   739,    -1,   298,   739,    -1,   543,    -1,
     542,   402,   543,    -1,   847,    -1,   187,   847,    -1,   364,
     186,   261,    -1,    -1,   186,   261,   176,    -1,    -1,   547,
      -1,   546,   402,   547,    -1,   746,   558,    -1,   131,   549,
     197,   744,   258,   738,   550,   396,   551,   397,   664,    -1,
     345,    -1,    -1,   352,   742,    -1,    -1,   552,    -1,   553,
      -1,   552,   402,   554,    -1,   554,    -1,   746,   396,   739,
     397,   555,    -1,   743,   555,    -1,   527,    -1,   352,   527,
      -1,    -1,   131,   557,   183,   746,   558,   292,   563,   565,
     567,    -1,   262,   289,    -1,    -1,   396,   559,   397,    -1,
     396,   397,    -1,   560,    -1,   559,   402,   560,    -1,   561,
     564,    -1,   564,    -1,   195,    -1,   264,    -1,   201,    -1,
     755,    -1,   755,   402,   755,    -1,   564,    -1,   667,    -1,
     848,   728,   391,   342,    -1,   566,    -1,   565,   566,    -1,
      90,   562,    -1,   218,   438,    -1,   193,    -1,   315,    -1,
     361,    -1,   107,   258,   250,   202,    -1,   292,   250,   258,
     250,   202,    -1,   322,    -1,   171,   301,   150,    -1,   171,
     301,   211,    -1,   301,   150,    -1,   301,   211,    -1,   364,
     514,    -1,    -1,   159,   183,   746,   558,   447,    -1,   159,
      83,   746,   396,   570,   397,   447,    -1,   667,    -1,   389,
      -1,   159,   260,   698,   396,   572,   397,   447,    -1,   667,
      -1,   667,   402,   667,    -1,   245,   402,   667,    -1,   667,
     402,   245,    -1,   698,    -1,   847,   398,   573,    -1,   131,
     110,   396,   671,    90,   671,   397,   364,   183,   547,   575,
      -1,   131,   110,   396,   671,    90,   671,   397,   365,   183,
     575,    -1,    90,    93,    -1,    -1,   159,   110,   396,   671,
      90,   671,   397,   447,    -1,   286,   578,   738,   579,    -1,
     286,   141,   740,   579,    -1,   197,    -1,   325,    -1,   177,
      -1,    -1,    85,   325,   662,   288,   582,   581,   332,   740,
      -1,    85,   337,   740,   258,   662,   288,   332,   740,    -1,
     740,    -1,    -1,   122,    -1,    -1,    -1,   131,   557,   297,
     740,    90,   584,   258,   589,   332,   738,   664,   156,   590,
     585,    -1,   247,    -1,   587,    -1,   396,   586,   397,    -1,
     586,   399,   588,    -1,   588,    -1,   630,    -1,   619,    -1,
     627,    -1,   623,    -1,   592,    -1,   587,    -1,    -1,   302,
      -1,   349,    -1,   151,    -1,   204,    -1,   205,    -1,    -1,
     159,   297,   740,   258,   738,   447,    -1,   248,   738,    -1,
     224,   738,    -1,   347,   738,    -1,   347,   389,    -1,    77,
     597,    -1,    98,   597,    -1,   316,   335,   596,    -1,   124,
     597,    -1,   164,   597,    -1,   295,   597,    -1,   214,   221,
     432,    -1,   366,    -1,   335,    -1,    -1,   131,   557,   360,
     738,   472,    90,   630,    -1,   225,   745,    -1,   131,   141,
     741,   364,   601,    -1,   131,   141,   741,    -1,   602,    -1,
     601,   602,    -1,   227,   603,   755,    -1,   227,   603,   147,
      -1,   327,   603,   740,    -1,   327,   603,   147,    -1,   162,
     603,   756,    -1,   162,   603,   147,    -1,   268,   603,   740,
      -1,   268,   603,   147,    -1,   383,    -1,    -1,    85,   141,
     741,   307,   429,    -1,    85,   141,   741,   440,    -1,   159,
     141,   741,    -1,   131,   157,   527,   607,   667,   465,   688,
      -1,    90,    -1,    -1,   131,   521,   128,   527,   176,   755,
     332,   755,   181,   527,    -1,   119,   744,   258,   738,    -1,
     353,   614,   615,   613,    -1,   353,   614,   615,   613,   738,
      -1,   353,   614,   615,   613,   611,    -1,   612,   613,    -1,
     612,   613,   738,   616,    -1,    87,    -1,    86,    -1,   358,
      -1,    -1,   182,    -1,    -1,   180,    -1,    -1,   396,   739,
     397,    -1,    -1,   170,   613,   618,    -1,   170,   612,   613,
     618,    -1,   630,    -1,   628,    -1,   627,    -1,   619,    -1,
     623,    -1,   204,   210,   738,   620,    -1,   355,   396,   734,
     397,    -1,   147,   355,    -1,   630,    -1,   396,   621,   397,
     355,   396,   734,   397,    -1,   396,   621,   397,   630,    -1,
     621,   402,   622,    -1,   622,    -1,   847,   705,    -1,   151,
     181,   662,   664,    -1,   228,   637,   737,   625,    -1,   195,
     626,   233,    -1,    -1,    79,   309,    -1,   296,   309,    -1,
     296,   167,    -1,   309,   349,   167,    -1,   309,    -1,   309,
     296,   167,    -1,   167,    -1,    79,   167,    -1,   349,   662,
     307,   732,   654,   664,    -1,   146,   740,   629,   139,   176,
     630,    -1,   101,    -1,   203,    -1,   299,    -1,   203,   299,
      -1,    -1,   632,    -1,   631,    -1,   396,   632,   397,    -1,
     396,   631,   397,    -1,   634,    -1,   633,   641,    -1,   633,
     640,   651,   646,    -1,   633,   640,   645,   652,    -1,   634,
      -1,   631,    -1,   302,   639,   730,   635,   654,   664,   649,
     650,    -1,   633,   344,   638,   633,    -1,   633,   208,   638,
     633,    -1,   633,   166,   638,   633,    -1,   210,   636,    -1,
     817,    -1,    -1,   328,   637,   738,    -1,   326,   637,   738,
      -1,   226,   328,   637,   738,    -1,   226,   326,   637,   738,
      -1,   185,   328,   637,   738,    -1,   185,   326,   637,   738,
      -1,   325,   738,    -1,   738,    -1,   325,    -1,    -1,    84,
      -1,    -1,   155,    -1,   155,   258,   396,   706,   397,    -1,
      84,    -1,    -1,   641,    -1,    -1,   263,   105,   642,    -1,
     643,    -1,   642,   402,   643,    -1,   702,   644,    -1,   352,
     698,    -1,    91,    -1,   154,    -1,    -1,   223,   647,   255,
     648,    -1,   255,   648,   223,   647,    -1,   223,   647,    -1,
     255,   648,    -1,   223,   647,   402,   648,    -1,   645,    -1,
      -1,   753,    -1,    84,    -1,   381,    -1,   753,    -1,   381,
      -1,   187,   105,   706,    -1,    -1,   189,   702,    -1,    -1,
     176,   349,   653,    -1,   176,   282,   259,    -1,   651,    -1,
      -1,   253,   739,    -1,    -1,   181,   655,    -1,    -1,   655,
     402,   656,    -1,   656,    -1,   662,    -1,   662,   658,    -1,
     663,    -1,   663,   658,    -1,   663,    90,   396,   665,   397,
      -1,   663,    90,   847,   396,   665,   397,    -1,   663,   847,
     396,   665,   397,    -1,   631,    -1,   631,   658,    -1,   657,
      -1,   396,   657,   397,   658,    -1,   396,   657,   397,    -1,
     656,   134,   215,   656,    -1,   656,   370,   656,    -1,   656,
     659,   215,   656,   661,    -1,   656,   215,   656,   661,    -1,
     656,   238,   659,   215,   656,    -1,   656,   238,   215,   656,
      -1,    90,   847,   396,   739,   397,    -1,    90,   847,    -1,
     847,   396,   739,   397,    -1,   847,    -1,   182,   660,    -1,
     220,   660,    -1,   294,   660,    -1,   200,    -1,   265,    -1,
      -1,   352,   396,   739,   397,    -1,   258,   702,    -1,   738,
      -1,   738,   389,    -1,   259,   738,    -1,   259,   396,   738,
     397,    -1,   746,   396,   397,    -1,   746,   396,   706,   397,
      -1,   363,   702,    -1,    -1,   666,    -1,   665,   402,   666,
      -1,   847,   667,    -1,   670,   668,    -1,   308,   670,    -1,
     394,   395,   668,    -1,   394,   669,   395,   668,    -1,    -1,
     753,    -1,   396,   669,   397,    -1,   669,   387,   669,    -1,
     669,   388,   669,    -1,   669,   389,   669,    -1,   669,   390,
     669,    -1,   669,   391,   669,    -1,   672,    -1,   689,    -1,
     673,    -1,   677,    -1,   681,    -1,   690,   692,    -1,   690,
     396,   753,   397,   692,    -1,   848,   728,    -1,   672,    -1,
     689,    -1,   673,    -1,   678,    -1,   682,    -1,   848,    -1,
     206,    -1,   207,    -1,   313,    -1,   100,    -1,   283,    -1,
     175,   674,    -1,   158,   275,    -1,   145,   676,    -1,   144,
     676,    -1,   252,   675,    -1,   104,    -1,   396,   753,   397,
      -1,    -1,   396,   753,   402,   753,   397,    -1,   396,   753,
     397,    -1,    -1,   396,   753,   402,   753,   397,    -1,   396,
     753,   397,    -1,    -1,   679,    -1,   680,    -1,   679,    -1,
     680,    -1,   102,   686,   396,   753,   397,    -1,   102,   686,
      -1,   683,    -1,   684,    -1,   683,    -1,   684,    -1,   685,
     396,   753,   397,   687,    -1,   685,   687,    -1,   113,   686,
      -1,   112,   686,    -1,   356,    -1,   237,   113,   686,    -1,
     237,   112,   686,    -1,   239,   686,    -1,   357,    -1,    -1,
     113,   307,   847,    -1,    -1,   121,   847,    -1,    -1,   331,
     396,   753,   397,   691,    -1,   331,   691,    -1,   330,   396,
     753,   397,   691,    -1,   330,   691,    -1,   209,    -1,   364,
     330,   369,    -1,   365,   330,   369,    -1,    -1,   368,    -1,
     234,    -1,   142,    -1,   190,    -1,   231,    -1,   300,    -1,
     368,   332,   234,    -1,   142,   332,   190,    -1,   142,   332,
     231,    -1,   142,   332,   300,    -1,   190,   332,   231,    -1,
     231,   332,   300,    -1,   190,   332,   300,    -1,    -1,   694,
     195,   631,    -1,   694,   246,   195,   631,    -1,   694,   701,
     697,   631,    -1,   694,   701,   631,    -1,   694,   701,   694,
      -1,   694,   212,   250,    -1,   694,   212,   246,   250,    -1,
     694,   266,   694,    -1,   694,   212,   155,   181,   694,    -1,
     296,   396,   695,   397,    -1,   296,   396,   702,   397,    -1,
     296,   396,   397,    -1,   396,   695,   397,    -1,   696,   402,
     702,    -1,   696,   402,   702,    -1,   702,    -1,    89,    -1,
     314,    -1,    84,    -1,   373,    -1,   699,    -1,   387,    -1,
     388,    -1,   389,    -1,   391,    -1,   392,    -1,   390,    -1,
     384,    -1,   385,    -1,   383,    -1,   373,    -1,   260,   396,
     573,   397,    -1,   698,    -1,   260,   396,   573,   397,    -1,
     704,    -1,   702,    76,   667,    -1,   702,    94,   330,   369,
     704,    -1,   387,   702,    -1,   388,   702,    -1,   391,   702,
      -1,   392,   702,    -1,   702,   391,    -1,   702,   392,    -1,
     702,   387,   702,    -1,   702,   388,   702,    -1,   702,   389,
     702,    -1,   702,   390,   702,    -1,   702,   391,   702,    -1,
     702,   392,   702,    -1,   702,   384,   702,    -1,   702,   385,
     702,    -1,   702,   383,   702,    -1,   702,   700,   702,    -1,
     700,   702,    -1,   702,   700,    -1,   702,    88,   702,    -1,
     702,   262,   702,    -1,   246,   702,    -1,   702,   222,   702,
      -1,   702,   222,   702,   165,   702,    -1,   702,   246,   222,
     702,    -1,   702,   246,   222,   702,   165,   702,    -1,   702,
     191,   702,    -1,   702,   191,   702,   165,   702,    -1,   702,
     246,   191,   702,    -1,   702,   246,   191,   702,   165,   702,
      -1,   702,   311,   332,   702,    -1,   702,   311,   332,   702,
     165,   702,    -1,   702,   246,   311,   332,   702,    -1,   702,
     246,   311,   332,   702,   165,   702,    -1,   702,   213,    -1,
     702,   212,   250,    -1,   702,   249,    -1,   702,   212,   246,
     250,    -1,   702,   212,   339,    -1,   702,   212,   246,   339,
      -1,   702,   212,   173,    -1,   702,   212,   246,   173,    -1,
     702,   212,   346,    -1,   702,   212,   246,   346,    -1,   702,
     212,   155,   181,   702,    -1,   702,   212,   253,   396,   708,
     397,    -1,   702,   212,   246,   253,   396,   708,   397,    -1,
     702,    99,   703,    88,   703,    -1,   702,   246,    99,   703,
      88,   703,    -1,   702,   195,   719,    -1,   702,   246,   195,
     719,    -1,   702,   701,   697,   631,    -1,   345,   631,    -1,
     693,    -1,   704,    -1,   703,    76,   667,    -1,   388,   703,
      -1,   391,   703,    -1,   392,   703,    -1,   703,   391,    -1,
     703,   392,    -1,   703,   387,   703,    -1,   703,   388,   703,
      -1,   703,   389,   703,    -1,   703,   390,   703,    -1,   703,
     391,   703,    -1,   703,   392,   703,    -1,   703,   384,   703,
      -1,   703,   385,   703,    -1,   703,   383,   703,    -1,   703,
     373,   703,    -1,   700,   703,    -1,   703,   700,    -1,   703,
     212,   155,   181,   703,    -1,   703,   212,   253,   396,   703,
     397,    -1,   703,   212,   246,   253,   396,   703,   397,    -1,
     726,    -1,   747,    -1,   381,   728,   705,    -1,   396,   702,
     397,    -1,   396,   702,   397,   728,   705,    -1,   721,    -1,
     746,   396,   397,    -1,   746,   396,   706,   397,    -1,   746,
     396,    84,   706,   397,    -1,   746,   396,   155,   706,   397,
      -1,   746,   396,   389,   397,    -1,   135,    -1,   136,   729,
      -1,   136,   396,   753,   397,    -1,   137,   729,    -1,   137,
     396,   753,   397,    -1,   138,   729,    -1,   306,   729,    -1,
     351,   729,    -1,   110,   396,   702,    90,   667,   397,    -1,
     172,   396,   707,   397,    -1,   267,   396,   710,   397,    -1,
     274,   396,   712,   397,    -1,   323,   396,   713,   397,    -1,
     336,   396,   702,    90,   667,   397,    -1,   338,   396,   103,
     716,   397,    -1,   338,   396,   219,   716,   397,    -1,   338,
     396,   334,   716,   397,    -1,   338,   396,   716,   397,    -1,
     129,   396,   717,   397,    -1,   631,    -1,   169,   631,    -1,
     394,   702,   395,   705,    -1,   394,   702,   404,   702,   395,
     705,    -1,    -1,   702,    -1,   706,   402,   702,    -1,   706,
     352,   702,    -1,   709,   181,   702,    -1,    -1,   708,   402,
     667,    -1,   667,    -1,   863,    -1,   368,    -1,   234,    -1,
     142,    -1,   190,    -1,   231,    -1,   300,    -1,   755,    -1,
     702,   711,   714,   715,    -1,   702,   711,   714,    -1,   273,
     702,    -1,   703,   195,   703,    -1,    -1,   702,   714,   715,
      -1,   702,   715,   714,    -1,   702,   714,    -1,   702,   715,
      -1,   706,    -1,    -1,   181,   702,    -1,   176,   702,    -1,
     702,   181,   706,    -1,   181,   706,    -1,   706,    -1,   702,
     352,   527,    -1,   718,    -1,    -1,   702,    -1,   718,   402,
     702,    -1,   631,    -1,   396,   720,   397,    -1,   702,    -1,
     720,   402,   702,    -1,   109,   725,   722,   724,   164,    -1,
     251,   396,   702,   402,   702,   397,    -1,   120,   396,   706,
     397,    -1,   722,   723,    -1,   723,    -1,   362,   702,   329,
     702,    -1,   161,   702,    -1,    -1,   702,    -1,    -1,   736,
     705,    -1,   727,   705,    -1,   736,   728,    -1,   398,   743,
      -1,   398,   389,    -1,   398,   743,   728,    -1,   396,   397,
      -1,    -1,   730,   402,   731,    -1,   731,    -1,   702,    90,
     850,    -1,   702,    -1,   389,    -1,   732,   402,   733,    -1,
     733,    -1,   389,    -1,   847,   705,   383,   702,    -1,   734,
     402,   735,    -1,   735,    -1,   731,    -1,   147,    -1,   761,
      -1,   847,    -1,   738,    -1,   737,   402,   738,    -1,   736,
      -1,   727,    -1,   740,    -1,   739,   402,   740,    -1,   847,
      -1,   847,    -1,   847,    -1,   847,    -1,   847,    -1,   755,
      -1,   849,    -1,   727,    -1,   759,    -1,   671,   755,    -1,
     690,   755,   692,    -1,   690,   396,   753,   397,   755,   692,
      -1,   381,   705,    -1,   339,    -1,   173,    -1,   250,    -1,
     860,    -1,   380,    -1,   382,    -1,   378,    -1,   379,    -1,
     372,    -1,   748,    -1,   861,    -1,   753,    -1,   388,   753,
      -1,   752,    -1,   861,    -1,   748,    -1,   752,    -1,   861,
      -1,   749,    -1,   748,    -1,   388,   749,    -1,   388,   748,
      -1,   861,    -1,   752,    -1,   757,    -1,   752,    -1,   749,
      -1,   748,    -1,   750,    -1,   751,    -1,   861,    -1,   847,
      -1,   257,    -1,   240,    -1,     9,   332,   763,   769,   770,
      -1,     9,   332,   147,    -1,     9,   771,    -1,   741,   766,
     768,    -1,   764,   404,   765,   768,   390,   741,   774,    -1,
     755,    -1,   863,   375,    -1,   373,   767,    -1,   765,    -1,
      -1,   847,    -1,   847,   398,   767,    -1,   377,    -1,   404,
     753,    -1,    -1,    90,   763,    -1,    -1,   351,   771,    -1,
      -1,   772,    -1,   772,   390,   772,    -1,   772,    24,   105,
     772,    -1,   772,   352,   772,    -1,   760,    -1,   755,    -1,
     375,    -1,   373,   847,    -1,    -1,   146,   740,   629,   139,
     176,   863,    -1,   143,   276,   863,    -1,   143,   863,    -1,
      -1,   779,   778,   781,   780,    -1,   857,    98,   146,    38,
     399,    -1,   857,   164,   146,    38,   399,    -1,    -1,   782,
      -1,   786,    -1,   784,    -1,   782,   786,    -1,   782,   784,
      -1,   786,    -1,   783,   786,    -1,    -1,    75,   785,   791,
     805,   851,   837,   399,    -1,    -1,    -1,   789,   790,   787,
     791,   788,   802,   399,    -1,    61,    -1,    72,    -1,    70,
      -1,    55,    -1,    -1,    56,    -1,    74,    -1,    -1,   798,
      -1,   794,    -1,   795,    -1,   792,    -1,   851,    -1,    19,
     845,   793,    -1,    19,   846,    -1,   400,   867,   401,    -1,
     796,   400,   783,   401,    -1,   797,   400,   783,   401,    -1,
      47,   845,    -1,   344,   845,    -1,   799,    -1,   801,   800,
      -1,    48,    39,    -1,    48,    39,   206,    -1,    48,    -1,
      48,   206,    -1,    48,    28,    -1,    48,    28,   206,    -1,
      48,    28,    28,    -1,    48,    28,    28,   206,    -1,    48,
     112,    -1,    39,    -1,    39,   206,    -1,   206,    -1,    28,
      -1,    28,   206,    -1,    28,    28,    -1,    28,    28,   206,
      -1,     5,    -1,   112,    -1,    40,    -1,    -1,   803,    -1,
     802,   402,   803,    -1,   805,   851,   668,   804,    -1,    -1,
     383,   868,    -1,    -1,   389,    -1,   389,   389,    -1,   146,
     317,   863,    -1,    18,   808,    -1,   809,    -1,    13,    -1,
      84,    -1,    -1,   763,    -1,   147,    -1,   168,   192,   812,
      -1,    -1,   168,   740,   811,   815,   818,    -1,   773,    -1,
     374,    -1,    21,   740,    -1,    32,   740,   815,    -1,    -1,
     352,   819,    -1,    -1,    41,    -1,   210,   856,    -1,   210,
     816,    17,   864,    -1,    -1,   817,    -1,   860,    -1,   861,
      -1,   803,    -1,   803,   402,   819,    -1,   276,   740,   181,
     812,    -1,   143,    17,   864,    -1,     3,    17,   864,    -1,
     375,   383,   824,    -1,    12,    -1,   375,   383,   826,    -1,
       8,    -1,    14,    -1,    15,    -1,    16,    -1,    25,    -1,
      26,    -1,    27,    -1,    29,    -1,    30,    -1,    31,    -1,
     275,    -1,    35,    -1,    36,    -1,    37,    -1,   342,    -1,
     823,    -1,   827,   402,   823,    -1,   825,    -1,   828,   402,
     825,    -1,   184,    17,   864,   827,    -1,   184,    17,   864,
      49,   375,   828,    -1,   184,    17,   864,    49,   748,   828,
      -1,   595,    33,    -1,   307,     4,   383,   833,    -1,   307,
       4,   332,   833,    -1,   258,    -1,   254,    -1,   307,    10,
     332,   809,    -1,   307,    10,   383,   809,    -1,    -1,   342,
     836,   850,   212,   791,   837,   838,    -1,   394,   395,   837,
      -1,   396,   397,   837,    -1,   394,   669,   395,   837,    -1,
     396,   669,   397,   837,    -1,    -1,    34,    -1,    -1,    -1,
      50,   840,   850,   212,   791,   837,   838,    -1,    51,    42,
     842,    -1,    51,   246,    20,   842,    -1,    51,    44,   842,
      -1,    11,    -1,    43,    -1,    46,    -1,    23,   740,    -1,
      22,   332,   740,    -1,   156,   740,   396,   858,   397,    -1,
     156,     6,    -1,     7,   740,   396,   858,   397,    -1,     6,
      -1,     7,    -1,     8,    -1,     9,    -1,    11,    -1,    12,
      -1,    14,    -1,    15,    -1,    16,    -1,    18,    -1,    20,
      -1,    22,    -1,    23,    -1,    24,    -1,    25,    -1,    26,
      -1,    27,    -1,    29,    -1,    30,    -1,    31,    -1,    32,
      -1,    33,    -1,    35,    -1,    36,    -1,    37,    -1,    38,
      -1,    42,    -1,    43,    -1,    44,    -1,    46,    -1,    50,
      -1,    51,    -1,     5,    -1,    28,    -1,    39,    -1,    47,
      -1,    40,    -1,    48,    -1,   846,    -1,    -1,   850,    -1,
     863,    -1,   852,    -1,   853,    -1,   843,    -1,   112,    -1,
     863,    -1,   852,    -1,   843,    -1,   844,    -1,   863,    -1,
     852,    -1,   854,    -1,   843,    -1,   851,    -1,   844,    -1,
     112,    -1,   206,    -1,   344,    -1,   863,    -1,   852,    -1,
     853,    -1,   854,    -1,   855,    -1,   843,    -1,    77,    -1,
      78,    -1,    79,    -1,    80,    -1,    81,    -1,    82,    -1,
      83,    -1,    85,    -1,    92,    -1,    93,    -1,    94,    -1,
      96,    -1,    97,    -1,    98,    -1,   105,    -1,   106,    -1,
     108,    -1,   111,    -1,   114,    -1,   116,    -1,   117,    -1,
     118,    -1,   119,    -1,   123,    -1,   124,    -1,   125,    -1,
     127,    -1,   128,    -1,   130,    -1,   132,    -1,   133,    -1,
     139,    -1,   140,    -1,   141,    -1,   142,    -1,   143,    -1,
     146,    -1,   149,    -1,   151,    -1,   152,    -1,   153,    -1,
     157,    -1,   158,    -1,   159,    -1,   160,    -1,   162,    -1,
     163,    -1,   165,    -1,   167,    -1,   168,    -1,   170,    -1,
     174,    -1,   177,    -1,   179,    -1,   183,    -1,   185,    -1,
     188,    -1,   190,    -1,   192,    -1,   193,    -1,   194,    -1,
     196,    -1,   197,    -1,   198,    -1,   201,    -1,   203,    -1,
     204,    -1,   205,    -1,   214,    -1,   216,    -1,   218,    -1,
     217,    -1,   221,    -1,   224,    -1,   225,    -1,   226,    -1,
     227,    -1,   228,    -1,   229,    -1,   230,    -1,   231,    -1,
     232,    -1,   233,    -1,   234,    -1,   235,    -1,   236,    -1,
     237,    -1,   241,    -1,   242,    -1,   243,    -1,   244,    -1,
     247,    -1,   248,    -1,   253,    -1,   256,    -1,   260,    -1,
     261,    -1,   264,    -1,   268,    -1,   269,    -1,   270,    -1,
     271,    -1,   272,    -1,   275,    -1,   276,    -1,   278,    -1,
     279,    -1,   281,    -1,   280,    -1,   282,    -1,   284,    -1,
     286,    -1,   287,    -1,   288,    -1,   289,    -1,   290,    -1,
     291,    -1,   292,    -1,   293,    -1,   295,    -1,   297,    -1,
     298,    -1,   299,    -1,   300,    -1,   305,    -1,   303,    -1,
     304,    -1,   307,    -1,   309,    -1,   310,    -1,   312,    -1,
     315,    -1,   316,    -1,   317,    -1,   318,    -1,   319,    -1,
     320,    -1,   321,    -1,   324,    -1,   326,    -1,   327,    -1,
     328,    -1,   333,    -1,   335,    -1,   337,    -1,   340,    -1,
     341,    -1,   342,    -1,   343,    -1,   346,    -1,   347,    -1,
     348,    -1,   349,    -1,   350,    -1,   353,    -1,   354,    -1,
     355,    -1,   357,    -1,   359,    -1,   360,    -1,   364,    -1,
     365,    -1,   367,    -1,   366,    -1,   368,    -1,   369,    -1,
     100,    -1,   102,    -1,   113,    -1,   120,    -1,   144,    -1,
     145,    -1,   169,    -1,   172,    -1,   175,    -1,   207,    -1,
     209,    -1,   239,    -1,   245,    -1,   251,    -1,   252,    -1,
     274,    -1,   283,    -1,   296,    -1,   308,    -1,   313,    -1,
     323,    -1,   330,    -1,   331,    -1,   336,    -1,   338,    -1,
     356,    -1,    95,    -1,    99,    -1,   101,    -1,   134,    -1,
     180,    -1,   182,    -1,   191,    -1,   195,    -1,   200,    -1,
     212,    -1,   213,    -1,   215,    -1,   220,    -1,   222,    -1,
     238,    -1,   249,    -1,   265,    -1,   266,    -1,   294,    -1,
     311,    -1,   358,    -1,    84,    -1,    86,    -1,    87,    -1,
      88,    -1,    89,    -1,    90,    -1,    91,    -1,   103,    -1,
     109,    -1,   110,    -1,   115,    -1,   121,    -1,   122,    -1,
     126,    -1,   131,    -1,   135,    -1,   136,    -1,   137,    -1,
     138,    -1,   147,    -1,   148,    -1,   154,    -1,   155,    -1,
     156,    -1,   161,    -1,   164,    -1,   166,    -1,   173,    -1,
     176,    -1,   178,    -1,   181,    -1,   186,    -1,   187,    -1,
     189,    -1,   199,    -1,   208,    -1,   210,    -1,   219,    -1,
     223,    -1,   240,    -1,   246,    -1,   250,    -1,   254,    -1,
     255,    -1,   257,    -1,   258,    -1,   259,    -1,   262,    -1,
     263,    -1,   277,    -1,   285,    -1,   302,    -1,   306,    -1,
     314,    -1,   325,    -1,   329,    -1,   332,    -1,   334,    -1,
     339,    -1,   345,    -1,   351,    -1,   352,    -1,   362,    -1,
     363,    -1,   859,    -1,   856,   402,   859,    -1,    45,    -1,
      -1,   867,    -1,   375,   862,    -1,   375,    -1,   375,   862,
      -1,   375,    -1,   375,    -1,    25,   375,    -1,    25,   740,
      -1,   371,    -1,   374,    -1,   740,    -1,   773,    -1,   870,
      -1,   396,   397,    -1,   396,   866,   397,    -1,   865,    -1,
     866,   865,    -1,   868,    -1,   867,   402,   868,    -1,   866,
      -1,   400,   867,   401,    -1,   870,    -1,   396,    -1,   397,
      -1,   402,    -1,   399,    -1,   371,    -1,   374,    -1,   753,
      -1,   749,    -1,   752,    -1,   389,    -1,   387,    -1,   388,
      -1,   390,    -1,   391,    -1,   250,    -1,    52,    -1,    53,
      -1,    54,    -1,    55,    -1,    56,    -1,    57,    -1,    58,
      -1,    59,    -1,    60,    -1,    61,    -1,    62,    -1,    63,
      -1,    65,    -1,    64,    -1,    66,    -1,    67,    -1,    68,
      -1,    69,    -1,    70,    -1,    71,    -1,    72,    -1,    73,
      -1,    75,    -1,     5,    -1,    19,    -1,   206,    -1,    28,
      -1,    39,    -1,    40,    -1,    47,    -1,    48,    -1,   112,
      -1,   158,    -1,   175,    -1,   344,    -1,   356,    -1,   394,
      -1,   395,    -1,   383,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   409,   409,   411,   412,   415,   416,   417,   418,   419,
     420,   421,   424,   436,   437,   438,   439,   440,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   459,   460,   461,   462,   463,
     464,   465,   466,   467,   468,   469,   470,   471,   472,   473,
     474,   475,   476,   477,   479,   480,   481,   482,   483,   484,
     485,   492,   493,   499,   500,   501,   502,   503,   504,   505,
     506,   507,   508,   509,   510,   511,   517,   527,   531,   541,
     547,   551,   560,   564,   571,   578,   584,   610,   619,   620,
     626,   635,   644,   651,   672,   676,   677,   688,   690,   694,
     696,   706,   713,   714,   717,   719,   721,   723,   725,   727,
     729,   731,   735,   737,   747,   749,   756,   757,   760,   762,
     773,   775,   785,   796,   798,   802,   803,   806,   807,   814,
     815,   816,   829,   831,   833,   837,   839,   841,   843,   845,
     847,   849,   851,   855,   857,   861,   863,   867,   868,   871,
     872,   874,   877,   878,   879,   882,   883,   884,   885,   895,
     896,   897,   899,   901,   903,   907,   908,   909,   912,   913,
     916,   918,   920,   922,   924,   928,   930,   932,   934,   936,
     940,   944,   946,   950,   951,   957,   969,   972,   975,   978,
     981,   984,   987,   990,   993,   996,   999,  1004,  1005,  1008,
    1009,  1010,  1020,  1023,  1024,  1035,  1040,  1041,  1049,  1050,
    1051,  1054,  1055,  1058,  1059,  1060,  1062,  1066,  1067,  1070,
    1071,  1078,  1081,  1084,  1085,  1095,  1098,  1108,  1109,  1110,
    1111,  1112,  1117,  1122,  1126,  1129,  1131,  1133,  1137,  1138,
    1139,  1142,  1153,  1154,  1157,  1159,  1160,  1174,  1176,  1178,
    1180,  1182,  1184,  1186,  1201,  1202,  1203,  1204,  1207,  1218,
    1220,  1224,  1226,  1228,  1230,  1235,  1236,  1239,  1241,  1245,
    1248,  1250,  1256,  1259,  1260,  1261,  1262,  1263,  1266,  1270,
    1274,  1275,  1276,  1277,  1278,  1281,  1284,  1287,  1288,  1289,
    1299,  1298,  1309,  1312,  1315,  1317,  1321,  1331,  1335,  1336,
    1339,  1341,  1343,  1345,  1347,  1349,  1361,  1366,  1367,  1374,
    1376,  1380,  1383,  1386,  1390,  1391,  1402,  1407,  1415,  1416,
    1419,  1421,  1423,  1427,  1428,  1429,  1432,  1436,  1437,  1440,
    1441,  1444,  1446,  1449,  1452,  1453,  1456,  1457,  1460,  1461,
    1468,  1469,  1478,  1480,  1484,  1486,  1490,  1501,  1509,  1524,
    1526,  1528,  1530,  1532,  1536,  1540,  1544,  1545,  1548,  1549,
    1553,  1554,  1555,  1558,  1565,  1566,  1569,  1571,  1573,  1575,
    1579,  1580,  1583,  1584,  1587,  1599,  1603,  1604,  1605,  1606,
    1607,  1608,  1609,  1610,  1613,  1615,  1619,  1621,  1630,  1643,
    1650,  1652,  1654,  1656,  1658,  1660,  1662,  1664,  1666,  1670,
    1671,  1672,  1673,  1680,  1681,  1682,  1683,  1686,  1687,  1702,
    1704,  1706,  1708,  1710,  1712,  1714,  1718,  1719,  1720,  1721,
    1722,  1723,  1724,  1725,  1726,  1729,  1730,  1740,  1744,  1748,
    1749,  1750,  1753,  1755,  1759,  1760,  1761,  1762,  1763,  1764,
    1765,  1766,  1767,  1768,  1769,  1770,  1773,  1775,  1777,  1779,
    1781,  1783,  1787,  1789,  1793,  1794,  1797,  1802,  1805,  1810,
    1813,  1815,  1819,  1830,  1835,  1836,  1839,  1842,  1845,  1846,
    1849,  1851,  1855,  1859,  1863,  1864,  1865,  1868,  1873,  1874,
    1877,  1879,  1883,  1885,  1889,  1897,  1900,  1901,  1907,  1915,
    1917,  1921,  1931,  1933,  1938,  1940,  1944,  1946,  1948,  1950,
    1952,  1954,  1956,  1958,  1960,  1962,  1964,  1966,  1970,  1971,
    1984,  1988,  1992,  1993,  1997,  2001,  2003,  2005,  2007,  2012,
    2014,  2018,  2021,  2026,  2027,  2031,  2042,  2044,  2048,  2049,
    2052,  2053,  2064,  2066,  2070,  2071,  2074,  2075,  2086,  2085,
    2095,  2096,  2097,  2101,  2103,  2107,  2108,  2109,  2110,  2111,
    2114,  2115,  2119,  2120,  2121,  2122,  2125,  2126,  2129,  2141,
    2145,  2149,  2151,  2164,  2165,  2166,  2167,  2168,  2169,  2172,
    2175,  2176,  2177,  2187,  2199,  2211,  2213,  2217,  2219,  2223,
    2225,  2227,  2229,  2231,  2233,  2235,  2237,  2241,  2242,  2253,
    2255,  2266,  2277,  2283,  2284,  2288,  2300,  2313,  2315,  2317,
    2321,  2323,  2327,  2328,  2331,  2332,  2335,  2336,  2339,  2340,
    2343,  2346,  2357,  2359,  2407,  2408,  2409,  2410,  2411,  2422,
    2426,  2428,  2430,  2432,  2434,  2438,  2440,  2444,  2456,  2460,
    2464,  2467,  2470,  2471,  2472,  2473,  2474,  2475,  2476,  2477,
    2487,  2501,  2532,  2533,  2534,  2535,  2536,  2546,  2548,  2552,
    2554,  2558,  2560,  2562,  2564,  2568,  2569,  2572,  2576,  2578,
    2580,  2584,  2589,  2590,  2600,  2602,  2604,  2606,  2608,  2613,
    2618,  2620,  2624,  2625,  2628,  2629,  2632,  2634,  2636,  2639,
    2642,  2643,  2646,  2650,  2651,  2654,  2658,  2659,  2660,  2661,
    2664,  2666,  2668,  2670,  2672,  2676,  2677,  2680,  2686,  2687,
    2690,  2696,  2706,  2709,  2712,  2715,  2718,  2720,  2724,  2725,
    2728,  2729,  2740,  2741,  2744,  2745,  2755,  2757,  2759,  2761,
    2763,  2765,  2767,  2769,  2771,  2773,  2775,  2796,  2798,  2800,
    2802,  2804,  2806,  2808,  2812,  2814,  2816,  2818,  2822,  2823,
    2824,  2825,  2829,  2830,  2840,  2842,  2846,  2848,  2850,  2852,
    2856,  2858,  2862,  2863,  2866,  2868,  2872,  2885,  2887,  2892,
    2898,  2908,  2915,  2916,  2917,  2918,  2919,  2920,  2921,  2924,
    2925,  2926,  2927,  2928,  2929,  2931,  2933,  2937,  2938,  2939,
    2940,  2941,  2944,  2952,  2954,  2956,  2958,  2960,  2962,  2964,
    2966,  2968,  2970,  2972,  2976,  2979,  2982,  2984,  2987,  2990,
    2992,  2995,  3003,  3004,  3007,  3008,  3011,  3015,  3024,  3025,
    3028,  3029,  3032,  3036,  3040,  3042,  3044,  3046,  3048,  3050,
    3054,  3057,  3060,  3063,  3066,  3069,  3072,  3074,  3076,  3078,
    3082,  3086,  3088,  3091,  3094,  3095,  3096,  3097,  3098,  3099,
    3100,  3101,  3102,  3103,  3104,  3105,  3106,  3107,  3121,  3123,
    3125,  3127,  3129,  3131,  3133,  3135,  3137,  3141,  3143,  3145,
    3147,  3151,  3155,  3157,  3161,  3162,  3163,  3166,  3167,  3170,
    3171,  3172,  3173,  3174,  3175,  3176,  3177,  3178,  3181,  3182,
    3185,  3186,  3205,  3207,  3209,  3220,  3222,  3224,  3226,  3228,
    3230,  3232,  3234,  3236,  3238,  3240,  3242,  3244,  3246,  3248,
    3250,  3252,  3254,  3256,  3258,  3260,  3262,  3264,  3266,  3268,
    3270,  3272,  3274,  3276,  3278,  3280,  3282,  3284,  3286,  3288,
    3290,  3292,  3303,  3305,  3307,  3309,  3311,  3313,  3315,  3317,
    3319,  3321,  3323,  3325,  3327,  3329,  3331,  3333,  3345,  3347,
    3349,  3351,  3353,  3355,  3357,  3359,  3361,  3363,  3365,  3367,
    3369,  3371,  3373,  3375,  3377,  3379,  3381,  3383,  3385,  3387,
    3399,  3401,  3403,  3405,  3407,  3409,  3411,  3413,  3415,  3417,
    3419,  3421,  3423,  3425,  3427,  3429,  3431,  3433,  3435,  3437,
    3439,  3441,  3443,  3445,  3447,  3450,  3452,  3454,  3456,  3458,
    3460,  3462,  3469,  3471,  3474,  3477,  3479,  3481,  3485,  3488,
    3491,  3493,  3501,  3502,  3503,  3504,  3505,  3506,  3507,  3508,
    3512,  3514,  3519,  3523,  3526,  3529,  3531,  3533,  3535,  3537,
    3540,  3543,  3547,  3551,  3553,  3555,  3560,  3562,  3565,  3568,
    3569,  3572,  3574,  3578,  3580,  3599,  3601,  3603,  3607,  3609,
    3613,  3617,  3620,  3623,  3624,  3627,  3629,  3633,  3637,  3639,
    3641,  3645,  3646,  3657,  3659,  3664,  3666,  3668,  3673,  3675,
    3677,  3681,  3685,  3687,  3691,  3692,  3702,  3703,  3706,  3708,
    3712,  3714,  3718,  3720,  3725,  3726,  3727,  3728,  3729,  3731,
    3741,  3743,  3751,  3753,  3755,  3757,  3759,  3761,  3763,  3765,
    3767,  3771,  3772,  3773,  3774,  3775,  3786,  3787,  3790,  3791,
    3794,  3795,  3798,  3799,  3800,  3803,  3804,  3805,  3806,  3807,
    3810,  3811,  3814,  3815,  3816,  3817,  3818,  3819,  3822,  3824,
    3831,  3847,  3849,  3852,  3856,  3868,  3893,  3916,  3934,  3946,
    3947,  3950,  3951,  3952,  3955,  3956,  3959,  3960,  3963,  3964,
    3967,  3969,  3971,  3973,  3977,  3984,  4007,  4033,  4046,  4053,
    4096,  4098,  4106,  4105,  4115,  4117,  4119,  4120,  4123,  4124,
    4125,  4126,  4129,  4130,  4134,  4133,  4196,  4201,  4195,  4221,
    4222,  4223,  4224,  4225,  4228,  4229,  4230,  4233,  4241,  4249,
    4257,  4265,  4310,  4312,  4316,  4319,  4327,  4335,  4345,  4355,
    4356,  4359,  4360,  4361,  4362,  4363,  4364,  4365,  4373,  4381,
    4384,  4385,  4386,  4387,  4388,  4389,  4397,  4405,  4406,  4409,
    4410,  4413,  4415,  4419,  4498,  4499,  4506,  4507,  4508,  4515,
    4524,  4527,  4528,  4529,  4530,  4533,  4534,  4540,  4554,  4553,
    4571,  4573,  4581,  4586,  4588,  4589,  4596,  4596,  4598,  4602,
    4609,  4610,  4613,  4613,  4615,  4615,  4621,  4633,  4643,  4653,
    4657,  4660,  4662,  4663,  4664,  4665,  4666,  4667,  4668,  4669,
    4670,  4671,  4672,  4673,  4674,  4675,  4676,  4679,  4680,  4683,
    4684,  4687,  4692,  4694,  4703,  4721,  4722,  4725,  4726,  4733,
    4734,  4741,  4740,  4801,  4807,  4813,  4822,  4832,  4839,  4840,
    4847,  4846,  4915,  4921,  4927,  4935,  4941,  4947,  4953,  4959,
    4965,  4971,  4977,  4988,  4989,  4990,  4991,  4992,  4993,  4994,
    4995,  4996,  4997,  4998,  4999,  5000,  5001,  5002,  5003,  5004,
    5005,  5006,  5007,  5008,  5009,  5010,  5011,  5012,  5013,  5014,
    5015,  5016,  5017,  5018,  5019,  5023,  5024,  5025,  5026,  5027,
    5028,  5031,  5032,  5035,  5051,  5052,  5053,  5054,  5055,  5060,
    5061,  5062,  5063,  5068,  5069,  5070,  5071,  5077,  5078,  5079,
    5080,  5081,  5084,  5085,  5086,  5087,  5088,  5089,  5105,  5106,
    5107,  5108,  5109,  5110,  5111,  5112,  5113,  5114,  5115,  5116,
    5117,  5118,  5119,  5120,  5121,  5122,  5123,  5124,  5125,  5126,
    5127,  5128,  5129,  5130,  5131,  5132,  5133,  5134,  5135,  5136,
    5137,  5138,  5139,  5140,  5141,  5142,  5143,  5144,  5145,  5146,
    5147,  5148,  5149,  5150,  5151,  5152,  5153,  5154,  5155,  5156,
    5157,  5158,  5159,  5160,  5161,  5162,  5163,  5164,  5165,  5166,
    5167,  5168,  5169,  5170,  5171,  5172,  5173,  5174,  5175,  5176,
    5177,  5178,  5179,  5180,  5181,  5182,  5183,  5184,  5185,  5186,
    5187,  5188,  5189,  5190,  5191,  5192,  5193,  5194,  5195,  5196,
    5197,  5198,  5199,  5200,  5201,  5202,  5203,  5204,  5205,  5206,
    5207,  5208,  5209,  5210,  5211,  5212,  5213,  5214,  5215,  5216,
    5217,  5218,  5219,  5220,  5221,  5222,  5223,  5224,  5225,  5226,
    5227,  5228,  5229,  5230,  5231,  5232,  5233,  5234,  5235,  5236,
    5237,  5238,  5239,  5240,  5241,  5242,  5243,  5244,  5245,  5246,
    5247,  5248,  5249,  5250,  5251,  5252,  5253,  5254,  5255,  5256,
    5257,  5258,  5259,  5260,  5261,  5262,  5263,  5264,  5265,  5266,
    5267,  5268,  5269,  5270,  5284,  5285,  5289,  5290,  5291,  5292,
    5293,  5294,  5295,  5299,  5300,  5301,  5302,  5303,  5304,  5305,
    5306,  5307,  5308,  5309,  5310,  5311,  5312,  5313,  5314,  5315,
    5329,  5330,  5331,  5332,  5333,  5334,  5335,  5336,  5337,  5338,
    5339,  5340,  5341,  5342,  5343,  5344,  5345,  5346,  5347,  5348,
    5349,  5359,  5360,  5361,  5362,  5363,  5364,  5365,  5366,  5367,
    5368,  5369,  5370,  5371,  5372,  5373,  5374,  5375,  5376,  5377,
    5378,  5379,  5380,  5381,  5382,  5383,  5384,  5385,  5386,  5387,
    5388,  5389,  5390,  5391,  5392,  5393,  5394,  5395,  5396,  5397,
    5398,  5399,  5400,  5401,  5402,  5403,  5404,  5405,  5406,  5407,
    5408,  5409,  5410,  5411,  5412,  5413,  5414,  5415,  5416,  5417,
    5421,  5422,  5423,  5424,  5425,  5429,  5429,  5432,  5435,  5436,
    5439,  5441,  5446,  5455,  5462,  5463,  5464,  5467,  5468,  5471,
    5473,  5481,  5482,  5483,  5487,  5488,  5492,  5493,  5496,  5497,
    5500,  5501,  5502,  5503,  5504,  5507,  5508,  5509,  5510,  5511,
    5512,  5513,  5514,  5515,  5516,  5517,  5518,  5519,  5520,  5521,
    5522,  5523,  5524,  5525,  5526,  5527,  5528,  5529,  5530,  5531,
    5532,  5533,  5534,  5535,  5536,  5537,  5538,  5539,  5540,  5541,
    5542,  5543,  5544,  5545,  5546,  5547,  5548,  5549,  5550,  5551,
    5552,  5553,  5554,  5555,  5556
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "SQL_ALLOCATE", "SQL_AUTOCOMMIT", 
  "SQL_BOOL", "SQL_BREAK", "SQL_CALL", "SQL_CARDINALITY", "SQL_CONNECT", 
  "SQL_CONNECTION", "SQL_CONTINUE", "SQL_COUNT", "SQL_CURRENT", 
  "SQL_DATA", "SQL_DATETIME_INTERVAL_CODE", 
  "SQL_DATETIME_INTERVAL_PRECISION", "SQL_DESCRIPTOR", "SQL_DISCONNECT", 
  "SQL_ENUM", "SQL_FOUND", "SQL_FREE", "SQL_GO", "SQL_GOTO", 
  "SQL_IDENTIFIED", "SQL_INDICATOR", "SQL_KEY_MEMBER", "SQL_LENGTH", 
  "SQL_LONG", "SQL_NAME", "SQL_NULLABLE", "SQL_OCTET_LENGTH", "SQL_OPEN", 
  "SQL_RELEASE", "SQL_REFERENCE", "SQL_RETURNED_LENGTH", 
  "SQL_RETURNED_OCTET_LENGTH", "SQL_SCALE", "SQL_SECTION", "SQL_SHORT", 
  "SQL_SIGNED", "SQL_SQL", "SQL_SQLERROR", "SQL_SQLPRINT", 
  "SQL_SQLWARNING", "SQL_START", "SQL_STOP", "SQL_STRUCT", "SQL_UNSIGNED", 
  "SQL_VALUE", "SQL_VAR", "SQL_WHENEVER", "S_ADD", "S_AND", "S_ANYTHING", 
  "S_AUTO", "S_CONST", "S_DEC", "S_DIV", "S_DOTPOINT", "S_EQUAL", 
  "S_EXTERN", "S_INC", "S_LSHIFT", "S_MEMPOINT", "S_MEMBER", "S_MOD", 
  "S_MUL", "S_NEQUAL", "S_OR", "S_REGISTER", "S_RSHIFT", "S_STATIC", 
  "S_SUB", "S_VOLATILE", "S_TYPEDEF", "TYPECAST", "ABORT_TRANS", 
  "ABSOLUTE", "ACCESS", "ACTION", "ADD", "AFTER", "AGGREGATE", "ALL", 
  "ALTER", "ANALYSE", "ANALYZE", "AND", "ANY", "AS", "ASC", "ASSERTION", 
  "ASSIGNMENT", "AT", "AUTHORIZATION", "BACKWARD", "BEFORE", 
  "BEGIN_TRANS", "BETWEEN", "BIGINT", "BINARY", "BIT", "BOTH", "BOOLEAN", 
  "BY", "CACHE", "CALLED", "CASCADE", "CASE", "CAST", "CHAIN", "CHAR_P", 
  "CHARACTER", "CHARACTERISTICS", "CHECK", "CHECKPOINT", "CLASS", "CLOSE", 
  "CLUSTER", "COALESCE", "COLLATE", "COLUMN", "COMMENT", "COMMIT", 
  "COMMITTED", "CONSTRAINT", "CONSTRAINTS", "CONVERSION_P", "CONVERT", 
  "COPY", "CREATE", "CREATEDB", "CREATEUSER", "CROSS", "CURRENT_DATE", 
  "CURRENT_TIME", "CURRENT_TIMESTAMP", "CURRENT_USER", "CURSOR", "CYCLE", 
  "DATABASE", "DAY_P", "DEALLOCATE", "DEC", "DECIMAL", "DECLARE", 
  "DEFAULT", "DEFERRABLE", "DEFERRED", "DEFINER", "DELETE_P", "DELIMITER", 
  "DELIMITERS", "DESC", "DISTINCT", "DO", "DOMAIN_P", "DOUBLE", "DROP", 
  "EACH", "ELSE", "ENCODING", "ENCRYPTED", "END_TRANS", "ESCAPE", 
  "EXCEPT", "EXCLUSIVE", "EXECUTE", "EXISTS", "EXPLAIN", "EXTERNAL", 
  "EXTRACT", "FALSE_P", "FETCH", "FLOAT_P", "FOR", "FORCE", "FOREIGN", 
  "FORWARD", "FREEZE", "FROM", "FULL", "FUNCTION", "GET", "GLOBAL", 
  "GRANT", "GROUP_P", "HANDLER", "HAVING", "HOUR_P", "ILIKE", "IMMEDIATE", 
  "IMMUTABLE", "IMPLICIT_P", "IN_P", "INCREMENT", "INDEX", "INHERITS", 
  "INITIALLY", "INNER_P", "INOUT", "INPUT", "INSENSITIVE", "INSERT", 
  "INSTEAD", "INT", "INTEGER", "INTERSECT", "INTERVAL", "INTO", "INVOKER", 
  "IS", "ISNULL", "ISOLATION", "JOIN", "KEY", "LANCOMPILER", "LANGUAGE", 
  "LEADING", "LEFT", "LEVEL", "LIKE", "LIMIT", "LISTEN", "LOAD", "LOCAL", 
  "LOCATION", "LOCK_P", "MATCH", "MAXVALUE", "MINUTE_P", "MINVALUE", 
  "MODE", "MONTH_P", "MOVE", "NAMES", "NATIONAL", "NATURAL", "NCHAR", 
  "NEW", "NEXT", "NO", "NOCREATEDB", "NOCREATEUSER", "NONE", "NOT", 
  "NOTHING", "NOTIFY", "NOTNULL", "NULL_P", "NULLIF", "NUMERIC", "OF", 
  "OFF", "OFFSET", "OIDS", "OLD", "ON", "ONLY", "OPERATOR", "OPTION", 
  "OR", "ORDER", "OUT_P", "OUTER_P", "OVERLAPS", "OVERLAY", "OWNER", 
  "PARTIAL", "PASSWORD", "PATH_P", "PENDANT", "PLACING", "POSITION", 
  "PRECISION", "PREPARE", "PRIMARY", "PRIOR", "PRIVILEGES", "PROCEDURE", 
  "PROCEDURAL", "READ", "REAL", "RECHECK", "REFERENCES", "REINDEX", 
  "RELATIVE", "RENAME", "REPLACE", "RESET", "RESTRICT", "RETURNS", 
  "REVOKE", "RIGHT", "ROLLBACK", "ROW", "RULE", "SCHEMA", "SCROLL", 
  "SECOND_P", "SECURITY", "SELECT", "SEQUENCE", "SERIALIZABLE", "SESSION", 
  "SESSION_USER", "SET", "SETOF", "SHARE", "SHOW", "SIMILAR", "SIMPLE", 
  "SMALLINT", "SOME", "STABLE", "START", "STATEMENT", "STATISTICS", 
  "STDIN", "STDOUT", "STORAGE", "STRICT", "SUBSTRING", "SYSID", "TABLE", 
  "TEMP", "TEMPLATE", "TEMPORARY", "THEN", "TIME", "TIMESTAMP", "TO", 
  "TOAST", "TRAILING", "TRANSACTION", "TREAT", "TRIGGER", "TRIM", 
  "TRUE_P", "TRUNCATE", "TRUSTED", "TYPE_P", "UNENCRYPTED", "UNION", 
  "UNIQUE", "UNKNOWN", "UNLISTEN", "UNTIL", "UPDATE", "USAGE", "USER", 
  "USING", "VACUUM", "VALID", "VALUES", "VARCHAR", "VARYING", "VERBOSE", 
  "VERSION", "VIEW", "VOLATILE", "WHEN", "WHERE", "WITH", "WITHOUT", 
  "WORK", "WRITE", "YEAR_P", "ZONE", "UNIONJOIN", "IDENT", "SCONST", "Op", 
  "CSTRING", "CVARIABLE", "CPP_LINE", "IP", "BCONST", "XCONST", "ICONST", 
  "PARAM", "FCONST", "'='", "'<'", "'>'", "POSTFIXOP", "'+'", "'-'", 
  "'*'", "'/'", "'%'", "'^'", "UMINUS", "'['", "']'", "'('", "')'", "'.'", 
  "';'", "'{'", "'}'", "','", "\"=\"", "':'", "$accept", "prog", 
  "statements", "statement", "opt_at", "stmt", "CreateUserStmt", 
  "opt_with", "AlterUserStmt", "AlterUserSetStmt", "DropUserStmt", 
  "OptUserList", "OptUserElem", "user_list", "CreateGroupStmt", 
  "OptGroupList", "OptGroupElem", "AlterGroupStmt", "DropGroupStmt", 
  "CreateSchemaStmt", "OptSchemaName", "OptSchemaEltList", "schema_stmt", 
  "VariableSetStmt", "set_rest", "var_list_or_default", "var_list", 
  "iso_level", "opt_mode", "var_value", "opt_boolean", "zone_value", 
  "opt_encoding", "ColId_or_Sconst", "VariableShowStmt", 
  "VariableResetStmt", "ConstraintsSetStmt", "constraints_set_list", 
  "constraints_set_mode", "CheckPointStmt", "AlterTableStmt", 
  "alter_column_default", "opt_drop_behavior", "ClosePortalStmt", 
  "opt_id", "CopyStmt", "copy_from", "copy_file_name", "copy_opt_list", 
  "copy_opt_item", "opt_binary", "opt_oids", "copy_delimiter", 
  "opt_using", "CreateStmt", "OptTemp", "OptTableElementList", 
  "TableElementList", "TableElement", "columnDef", "ColQualList", 
  "ColConstraint", "ColConstraintElem", "ConstraintAttr", 
  "TableLikeClause", "TableConstraint", "ConstraintElem", 
  "opt_column_list", "columnList", "columnElem", "key_match", 
  "key_actions", "key_delete", "key_update", "key_action", "OptInherit", 
  "OptWithOids", "CreateAsStmt", "@1", "OptCreateAs", "CreateAsList", 
  "CreateAsElement", "CreateSeqStmt", "OptSeqList", "OptSeqElem", 
  "CreatePLangStmt", "opt_Trusted", "handler_name", "opt_lancompiler", 
  "DropPLangStmt", "opt_procedural", "CreateTrigStmt", 
  "TriggerActionTime", "TriggerEvents", "TriggerOneEvent", 
  "TriggerForSpec", "TriggerForOpt", "TriggerForType", "TriggerFuncArgs", 
  "TriggerFuncArg", "OptConstrFromTable", "ConstraintAttributeSpec", 
  "ConstraintDeferrabilitySpec", "ConstraintTimeSpec", "DropTrigStmt", 
  "CreateAssertStmt", "DropAssertStmt", "DefineStmt", "rowdefinition", 
  "definition", "def_list", "def_elem", "def_arg", "CreateOpClassStmt", 
  "opclass_item_list", "opclass_item", "opt_default", "opt_recheck", 
  "DropOpClassStmt", "DropStmt", "drop_type", "any_name_list", "any_name", 
  "TruncateStmt", "FetchStmt", "direction", "fetch_how_many", "from_in", 
  "CommentStmt", "comment_type", "comment_text", "GrantStmt", 
  "RevokeStmt", "privileges", "privilege_list", "privilege", 
  "privilege_target", "grantee_list", "grantee", "opt_grant_grant_option", 
  "opt_revoke_grant_option", "function_with_argtypes_list", 
  "function_with_argtypes", "IndexStmt", "index_opt_unique", 
  "access_method_clause", "index_params", "index_list", "func_index", 
  "index_elem", "opt_class", "CreateFunctionStmt", "opt_or_replace", 
  "func_args", "func_args_list", "func_arg", "opt_arg", "func_as", 
  "func_return", "func_type", "createfunc_opt_list", 
  "createfunc_opt_item", "opt_definition", "RemoveFuncStmt", 
  "RemoveAggrStmt", "aggr_argtype", "RemoveOperStmt", "oper_argtypes", 
  "any_operator", "CreateCastStmt", "cast_context", "DropCastStmt", 
  "ReindexStmt", "reindex_type", "opt_force", "RenameStmt", "opt_name", 
  "opt_column", "RuleStmt", "@2", "RuleActionList", "RuleActionMulti", 
  "RuleActionStmt", "RuleActionStmtOrEmpty", "event", "opt_instead", 
  "DropRuleStmt", "NotifyStmt", "ListenStmt", "UnlistenStmt", 
  "TransactionStmt", "trans_options", "opt_trans", "ViewStmt", "LoadStmt", 
  "CreatedbStmt", "createdb_opt_list", "createdb_opt_item", "opt_equal", 
  "AlterDatabaseSetStmt", "DropdbStmt", "CreateDomainStmt", "opt_as", 
  "CreateConversionStmt", "ClusterStmt", "VacuumStmt", "AnalyzeStmt", 
  "analyze_keyword", "opt_verbose", "opt_full", "opt_freeze", 
  "opt_name_list", "ExplainStmt", "OptimizableStmt", "InsertStmt", 
  "insert_rest", "insert_column_list", "insert_column_item", "DeleteStmt", 
  "LockStmt", "opt_lock", "lock_type", "UpdateStmt", "CursorStmt", 
  "opt_cursor", "SelectStmt", "select_with_parens", "select_no_parens", 
  "select_clause", "simple_select", "into_clause", "OptTempTableName", 
  "opt_table", "opt_all", "opt_distinct", "opt_sort_clause", 
  "sort_clause", "sortby_list", "sortby", "OptUseOp", "select_limit", 
  "opt_select_limit", "select_limit_value", "select_offset_value", 
  "group_clause", "having_clause", "for_update_clause", 
  "opt_for_update_clause", "update_list", "from_clause", "from_list", 
  "table_ref", "joined_table", "alias_clause", "join_type", "join_outer", 
  "join_qual", "relation_expr", "func_table", "where_clause", 
  "TableFuncElementList", "TableFuncElement", "Typename", 
  "opt_array_bounds", "Iresult", "SimpleTypename", "ConstTypename", 
  "GenericType", "Numeric", "opt_float", "opt_numeric", "opt_decimal", 
  "Bit", "ConstBit", "BitWithLength", "BitWithoutLength", "Character", 
  "ConstCharacter", "CharacterWithLength", "CharacterWithoutLength", 
  "character", "opt_varying", "opt_charset", "opt_collate", 
  "ConstDatetime", "ConstInterval", "opt_timezone", "opt_interval", 
  "r_expr", "row", "row_descriptor", "row_list", "sub_type", "all_Op", 
  "MathOp", "qual_Op", "qual_all_Op", "a_expr", "b_expr", "c_expr", 
  "opt_indirection", "expr_list", "extract_list", "type_list", 
  "extract_arg", "overlay_list", "overlay_placing", "position_list", 
  "substr_list", "substr_from", "substr_for", "trim_list", "convert_list", 
  "convert_args", "in_expr", "in_expr_nodes", "case_expr", 
  "when_clause_list", "when_clause", "case_default", "case_arg", 
  "columnref", "dotted_name", "attrs", "opt_empty_parentheses", 
  "target_list", "target_el", "update_target_list", "update_target_el", 
  "insert_target_list", "insert_target_el", "relation_name", 
  "qualified_name_list", "qualified_name", "name_list", "name", 
  "database_name", "access_method", "attr_name", "index_name", 
  "file_name", "func_name", "AexprConst", "Iconst", "Fconst", "Bconst", 
  "Xconst", "Sconst", "PosIntConst", "IntConst", "StringConst", 
  "PosIntStringConst", "NumConst", "AllConst", "PosAllConst", "UserId", 
  "SpecialRuleRelation", "ECPGConnect", "connection_target", "db_prefix", 
  "server", "opt_server", "server_name", "opt_port", 
  "opt_connection_name", "opt_user", "ora_user", "user_name", 
  "char_variable", "opt_options", "ECPGCursorStmt", "ECPGDeallocate", 
  "ECPGDeclaration", "@3", "sql_startdeclare", "sql_enddeclare", 
  "var_type_declarations", "vt_declarations", "variable_declarations", 
  "type_declaration", "@4", "var_declaration", "@5", "@6", 
  "storage_clause", "storage_modifier", "type", "enum_type", 
  "enum_definition", "struct_type", "union_type", "s_struct", "s_union", 
  "simple_type", "unsigned_type", "signed_type", "opt_signed", 
  "variable_list", "variable", "opt_initializer", "opt_pointer", 
  "ECPGDeclare", "ECPGDisconnect", "dis_name", "connection_object", 
  "ECPGExecute", "@7", "execstring", "ECPGFree", "ECPGOpen", 
  "opt_ecpg_using", "opt_sql", "ecpg_into", "opt_ecpg_into", 
  "variablelist", "ECPGPrepare", "ECPGDeallocateDescr", 
  "ECPGAllocateDescr", "ECPGGetDescHeaderItem", "desc_header_item", 
  "ECPGGetDescItem", "descriptor_item", "ECPGGetDescHeaderItems", 
  "ECPGGetDescItems", "ECPGGetDescriptorHeader", "ECPGGetDescriptor", 
  "ECPGRelease", "ECPGSetAutocommit", "on_off", "ECPGSetConnection", 
  "ECPGTypedef", "@8", "opt_type_array_bounds", "opt_reference", 
  "ECPGVar", "@9", "ECPGWhenever", "action", "ECPGKeywords", 
  "ECPGTypeName", "opt_symbol", "symbol", "ColId", "type_name", 
  "function_name", "ColLabel", "ECPGColLabel", "unreserved_keyword", 
  "col_name_keyword", "func_name_keyword", "reserved_keyword", 
  "into_list", "ecpgstart", "c_args", "coutputvariable", "civarind", 
  "civar", "indicator", "ident", "quoted_ident_stringvar", "c_stuff_item", 
  "c_stuff", "c_list", "c_term", "c_thing", "c_anything", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   468,   469,   470,   471,   472,   473,   474,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,   496,   497,   498,   499,   500,   501,   502,   503,   504,
     505,   506,   507,   508,   509,   510,   511,   512,   513,   514,
     515,   516,   517,   518,   519,   520,   521,   522,   523,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,   534,
     535,   536,   537,   538,   539,   540,   541,   542,   543,   544,
     545,   546,   547,   548,   549,   550,   551,   552,   553,   554,
     555,   556,   557,   558,   559,   560,   561,   562,   563,   564,
     565,   566,   567,   568,   569,   570,   571,   572,   573,   574,
     575,   576,   577,   578,   579,   580,   581,   582,   583,   584,
     585,   586,   587,   588,   589,   590,   591,   592,   593,   594,
     595,   596,   597,   598,   599,   600,   601,   602,   603,   604,
     605,   606,   607,   608,   609,   610,   611,   612,   613,   614,
     615,   616,   617,   618,   619,   620,   621,   622,   623,   624,
     625,   626,   627,   628,   629,   630,   631,   632,   633,   634,
     635,   636,   637,    61,    60,    62,   638,    43,    45,    42,
      47,    37,    94,   639,    91,    93,    40,    41,    46,    59,
     123,   125,    44,   640,    58
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned short yyr1[] =
{
       0,   405,   406,   407,   407,   408,   408,   408,   408,   408,
     408,   408,   409,   410,   410,   410,   410,   410,   410,   410,
     410,   410,   410,   410,   410,   410,   410,   410,   410,   410,
     410,   410,   410,   410,   410,   410,   410,   410,   410,   410,
     410,   410,   410,   410,   410,   410,   410,   410,   410,   410,
     410,   410,   410,   410,   410,   410,   410,   410,   410,   410,
     410,   410,   410,   410,   410,   410,   410,   410,   410,   410,
     410,   410,   410,   410,   410,   410,   410,   410,   410,   410,
     410,   410,   410,   410,   410,   410,   410,   410,   410,   410,
     410,   410,   410,   410,   411,   412,   412,   413,   413,   414,
     414,   415,   416,   416,   417,   417,   417,   417,   417,   417,
     417,   417,   418,   418,   419,   419,   420,   420,   421,   421,
     422,   422,   423,   424,   424,   425,   425,   426,   426,   427,
     427,   427,   428,   428,   428,   429,   429,   429,   429,   429,
     429,   429,   429,   430,   430,   431,   431,   432,   432,   433,
     433,   433,   434,   434,   434,   435,   435,   435,   435,   436,
     436,   436,   436,   436,   436,   437,   437,   437,   438,   438,
     439,   439,   439,   439,   439,   440,   440,   440,   440,   440,
     441,   442,   442,   443,   443,   444,   445,   445,   445,   445,
     445,   445,   445,   445,   445,   445,   445,   446,   446,   447,
     447,   447,   448,   449,   449,   450,   451,   451,   452,   452,
     452,   453,   453,   454,   454,   454,   454,   455,   455,   456,
     456,   457,   457,   458,   458,   459,   459,   460,   460,   460,
     460,   460,   460,   460,   461,   461,   462,   462,   463,   463,
     463,   464,   465,   465,   466,   466,   466,   467,   467,   467,
     467,   467,   467,   467,   468,   468,   468,   468,   469,   470,
     470,   471,   471,   471,   471,   472,   472,   473,   473,   474,
     475,   475,   475,   476,   476,   476,   476,   476,   477,   478,
     479,   479,   479,   479,   479,   480,   480,   481,   481,   481,
     483,   482,   484,   484,   485,   485,   486,   487,   488,   488,
     489,   489,   489,   489,   489,   489,   490,   491,   491,   492,
     492,   493,   493,   494,   495,   495,   496,   496,   497,   497,
     498,   498,   498,   499,   499,   499,   500,   501,   501,   502,
     502,   503,   503,   503,   504,   504,   505,   505,   506,   506,
     506,   506,   507,   507,   508,   508,   509,   510,   511,   512,
     512,   512,   512,   512,   513,   514,   515,   515,   516,   516,
     517,   517,   517,   518,   519,   519,   520,   520,   520,   520,
     521,   521,   522,   522,   523,   524,   525,   525,   525,   525,
     525,   525,   525,   525,   526,   526,   527,   527,   528,   529,
     529,   529,   529,   529,   529,   529,   529,   529,   529,   530,
     530,   530,   530,   531,   531,   531,   531,   532,   532,   533,
     533,   533,   533,   533,   533,   533,   534,   534,   534,   534,
     534,   534,   534,   534,   534,   535,   535,   536,   537,   538,
     538,   538,   539,   539,   540,   540,   540,   540,   540,   540,
     540,   540,   540,   540,   540,   540,   541,   541,   541,   541,
     541,   541,   542,   542,   543,   543,   544,   544,   545,   545,
     546,   546,   547,   548,   549,   549,   550,   550,   551,   551,
     552,   552,   553,   554,   555,   555,   555,   556,   557,   557,
     558,   558,   559,   559,   560,   560,   561,   561,   561,   562,
     562,   563,   564,   564,   565,   565,   566,   566,   566,   566,
     566,   566,   566,   566,   566,   566,   566,   566,   567,   567,
     568,   569,   570,   570,   571,   572,   572,   572,   572,   573,
     573,   574,   574,   575,   575,   576,   577,   577,   578,   578,
     579,   579,   580,   580,   581,   581,   582,   582,   584,   583,
     585,   585,   585,   586,   586,   587,   587,   587,   587,   587,
     588,   588,   589,   589,   589,   589,   590,   590,   591,   592,
     593,   594,   594,   595,   595,   595,   595,   595,   595,   596,
     597,   597,   597,   598,   599,   600,   600,   601,   601,   602,
     602,   602,   602,   602,   602,   602,   602,   603,   603,   604,
     604,   605,   606,   607,   607,   608,   609,   610,   610,   610,
     611,   611,   612,   612,   613,   613,   614,   614,   615,   615,
     616,   616,   617,   617,   618,   618,   618,   618,   618,   619,
     620,   620,   620,   620,   620,   621,   621,   622,   623,   624,
     625,   625,   626,   626,   626,   626,   626,   626,   626,   626,
     627,   628,   629,   629,   629,   629,   629,   630,   630,   631,
     631,   632,   632,   632,   632,   633,   633,   634,   634,   634,
     634,   635,   635,   635,   636,   636,   636,   636,   636,   636,
     636,   636,   637,   637,   638,   638,   639,   639,   639,   639,
     640,   640,   641,   642,   642,   643,   644,   644,   644,   644,
     645,   645,   645,   645,   645,   646,   646,   647,   647,   647,
     648,   648,   649,   649,   650,   650,   651,   651,   652,   652,
     653,   653,   654,   654,   655,   655,   656,   656,   656,   656,
     656,   656,   656,   656,   656,   656,   656,   657,   657,   657,
     657,   657,   657,   657,   658,   658,   658,   658,   659,   659,
     659,   659,   660,   660,   661,   661,   662,   662,   662,   662,
     663,   663,   664,   664,   665,   665,   666,   667,   667,   668,
     668,   668,   669,   669,   669,   669,   669,   669,   669,   670,
     670,   670,   670,   670,   670,   670,   670,   671,   671,   671,
     671,   671,   672,   673,   673,   673,   673,   673,   673,   673,
     673,   673,   673,   673,   674,   674,   675,   675,   675,   676,
     676,   676,   677,   677,   678,   678,   679,   680,   681,   681,
     682,   682,   683,   684,   685,   685,   685,   685,   685,   685,
     686,   686,   687,   687,   688,   688,   689,   689,   689,   689,
     690,   691,   691,   691,   692,   692,   692,   692,   692,   692,
     692,   692,   692,   692,   692,   692,   692,   692,   693,   693,
     693,   693,   693,   693,   693,   693,   693,   694,   694,   694,
     694,   695,   696,   696,   697,   697,   697,   698,   698,   699,
     699,   699,   699,   699,   699,   699,   699,   699,   700,   700,
     701,   701,   702,   702,   702,   702,   702,   702,   702,   702,
     702,   702,   702,   702,   702,   702,   702,   702,   702,   702,
     702,   702,   702,   702,   702,   702,   702,   702,   702,   702,
     702,   702,   702,   702,   702,   702,   702,   702,   702,   702,
     702,   702,   702,   702,   702,   702,   702,   702,   702,   702,
     702,   702,   702,   702,   702,   702,   702,   702,   703,   703,
     703,   703,   703,   703,   703,   703,   703,   703,   703,   703,
     703,   703,   703,   703,   703,   703,   703,   703,   703,   703,
     704,   704,   704,   704,   704,   704,   704,   704,   704,   704,
     704,   704,   704,   704,   704,   704,   704,   704,   704,   704,
     704,   704,   704,   704,   704,   704,   704,   704,   704,   704,
     704,   704,   705,   705,   705,   706,   706,   706,   707,   707,
     708,   708,   709,   709,   709,   709,   709,   709,   709,   709,
     710,   710,   711,   712,   712,   713,   713,   713,   713,   713,
     713,   714,   715,   716,   716,   716,   717,   717,   717,   718,
     718,   719,   719,   720,   720,   721,   721,   721,   722,   722,
     723,   724,   724,   725,   725,   726,   726,   727,   728,   728,
     728,   729,   729,   730,   730,   731,   731,   731,   732,   732,
     732,   733,   734,   734,   735,   735,   736,   736,   737,   737,
     738,   738,   739,   739,   740,   741,   742,   743,   744,   745,
     746,   746,   747,   747,   747,   747,   747,   747,   747,   747,
     747,   748,   749,   750,   751,   752,   753,   753,   754,   754,
     755,   755,   756,   756,   756,   757,   757,   757,   757,   757,
     758,   758,   759,   759,   759,   759,   759,   759,   760,   761,
     761,   762,   762,   762,   763,   763,   763,   764,   765,   766,
     766,   767,   767,   767,   768,   768,   769,   769,   770,   770,
     771,   771,   771,   771,   772,   772,   773,   774,   774,   775,
     776,   776,   778,   777,   779,   780,   781,   781,   782,   782,
     782,   782,   783,   783,   785,   784,   787,   788,   786,   789,
     789,   789,   789,   789,   790,   790,   790,   791,   791,   791,
     791,   791,   792,   792,   793,   794,   795,   796,   797,   798,
     798,   799,   799,   799,   799,   799,   799,   799,   799,   799,
     800,   800,   800,   800,   800,   800,   800,   800,   800,   801,
     801,   802,   802,   803,   804,   804,   805,   805,   805,   806,
     807,   808,   808,   808,   808,   809,   809,   810,   811,   810,
     812,   812,   813,   814,   815,   815,   816,   816,   817,   817,
     818,   818,   803,   803,   819,   819,   820,   821,   822,   823,
     824,   825,   826,   826,   826,   826,   826,   826,   826,   826,
     826,   826,   826,   826,   826,   826,   826,   827,   827,   828,
     828,   829,   830,   830,   831,   832,   832,   833,   833,   834,
     834,   836,   835,   837,   837,   837,   837,   837,   838,   838,
     840,   839,   841,   841,   841,   842,   842,   842,   842,   842,
     842,   842,   842,   843,   843,   843,   843,   843,   843,   843,
     843,   843,   843,   843,   843,   843,   843,   843,   843,   843,
     843,   843,   843,   843,   843,   843,   843,   843,   843,   843,
     843,   843,   843,   843,   843,   844,   844,   844,   844,   844,
     844,   845,   845,   846,   847,   847,   847,   847,   847,   848,
     848,   848,   848,   849,   849,   849,   849,   850,   850,   850,
     850,   850,   851,   851,   851,   851,   851,   851,   852,   852,
     852,   852,   852,   852,   852,   852,   852,   852,   852,   852,
     852,   852,   852,   852,   852,   852,   852,   852,   852,   852,
     852,   852,   852,   852,   852,   852,   852,   852,   852,   852,
     852,   852,   852,   852,   852,   852,   852,   852,   852,   852,
     852,   852,   852,   852,   852,   852,   852,   852,   852,   852,
     852,   852,   852,   852,   852,   852,   852,   852,   852,   852,
     852,   852,   852,   852,   852,   852,   852,   852,   852,   852,
     852,   852,   852,   852,   852,   852,   852,   852,   852,   852,
     852,   852,   852,   852,   852,   852,   852,   852,   852,   852,
     852,   852,   852,   852,   852,   852,   852,   852,   852,   852,
     852,   852,   852,   852,   852,   852,   852,   852,   852,   852,
     852,   852,   852,   852,   852,   852,   852,   852,   852,   852,
     852,   852,   852,   852,   852,   852,   852,   852,   852,   852,
     852,   852,   852,   852,   852,   852,   852,   852,   852,   852,
     852,   852,   852,   852,   852,   852,   852,   852,   852,   852,
     852,   852,   852,   852,   852,   852,   852,   852,   852,   852,
     852,   852,   852,   852,   853,   853,   853,   853,   853,   853,
     853,   853,   853,   853,   853,   853,   853,   853,   853,   853,
     853,   853,   853,   853,   853,   853,   853,   853,   853,   853,
     854,   854,   854,   854,   854,   854,   854,   854,   854,   854,
     854,   854,   854,   854,   854,   854,   854,   854,   854,   854,
     854,   855,   855,   855,   855,   855,   855,   855,   855,   855,
     855,   855,   855,   855,   855,   855,   855,   855,   855,   855,
     855,   855,   855,   855,   855,   855,   855,   855,   855,   855,
     855,   855,   855,   855,   855,   855,   855,   855,   855,   855,
     855,   855,   855,   855,   855,   855,   855,   855,   855,   855,
     855,   855,   855,   855,   855,   855,   855,   855,   855,   855,
     855,   855,   855,   855,   855,   856,   856,   857,   858,   858,
     859,   859,   860,   861,   862,   862,   862,   863,   863,   864,
     864,   865,   865,   865,   866,   866,   867,   867,   868,   868,
     869,   869,   869,   869,   869,   870,   870,   870,   870,   870,
     870,   870,   870,   870,   870,   870,   870,   870,   870,   870,
     870,   870,   870,   870,   870,   870,   870,   870,   870,   870,
     870,   870,   870,   870,   870,   870,   870,   870,   870,   870,
     870,   870,   870,   870,   870,   870,   870,   870,   870,   870,
     870,   870,   870,   870,   870
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     0,     2,     4,     3,     1,     1,     1,
       1,     1,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     5,     1,     0,     4,     5,     5,
       4,     3,     2,     0,     2,     2,     1,     1,     1,     1,
       3,     3,     3,     1,     4,     5,     2,     0,     2,     2,
       6,     6,     3,     7,     4,     1,     0,     2,     0,     1,
       1,     1,     2,     3,     3,     3,     3,     3,     5,     7,
       2,     3,     3,     1,     1,     1,     3,     2,     1,     2,
       2,     0,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     6,     1,     1,     1,     1,     0,     1,     1,
       2,     3,     4,     3,     2,     2,     3,     4,     3,     2,
       4,     1,     1,     1,     1,     1,     6,     7,     9,     9,
       9,     9,     7,     5,     7,     6,     6,     3,     2,     1,
       1,     0,     2,     1,     0,     9,     1,     1,     1,     1,
       1,     2,     0,     1,     1,     3,     3,     1,     0,     2,
       0,     3,     0,     1,     0,     9,    10,     1,     1,     2,
       2,     2,     2,     0,     1,     0,     1,     3,     1,     1,
       1,     4,     2,     0,     3,     1,     1,     2,     1,     1,
       2,     4,     2,     5,     1,     2,     2,     2,     2,     3,
       1,     4,     4,     5,    11,     3,     0,     3,     1,     1,
       2,     2,     0,     1,     1,     2,     2,     0,     3,     3,
       2,     1,     1,     2,     2,     4,     0,     2,     2,     0,
       0,     8,     3,     0,     3,     1,     1,     5,     2,     0,
       2,     1,     2,     2,     2,     2,     8,     1,     0,     1,
       1,     2,     0,     5,     1,     0,    14,    19,     1,     1,
       1,     3,     5,     1,     1,     1,     3,     1,     0,     1,
       1,     1,     3,     0,     1,     1,     0,     2,     1,     2,
       1,     2,     2,     1,     2,     2,     6,     8,     3,     4,
       4,     4,     5,     8,     3,     3,     1,     3,     3,     1,
       1,     1,     1,    12,     1,     3,     4,     7,     4,     2,
       1,     0,     1,     0,     7,     4,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     1,     1,     3,     6,
       5,     5,     4,     3,     5,     4,     4,     3,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     6,
       9,     7,     9,     8,     8,     6,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     7,     7,     2,
       1,     1,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     2,
       2,     2,     1,     3,     1,     2,     3,     0,     3,     0,
       1,     3,     2,    11,     1,     0,     2,     0,     1,     1,
       3,     1,     5,     2,     1,     2,     0,     9,     2,     0,
       3,     2,     1,     3,     2,     1,     1,     1,     1,     1,
       3,     1,     1,     4,     1,     2,     2,     2,     1,     1,
       1,     4,     5,     1,     3,     3,     2,     2,     2,     0,
       5,     7,     1,     1,     7,     1,     3,     3,     3,     1,
       3,    11,    10,     2,     0,     8,     4,     4,     1,     1,
       1,     0,     8,     8,     1,     0,     1,     0,     0,    14,
       1,     1,     3,     3,     1,     1,     1,     1,     1,     1,
       1,     0,     1,     1,     1,     1,     1,     0,     6,     2,
       2,     2,     2,     2,     2,     3,     2,     2,     2,     3,
       1,     1,     0,     7,     2,     5,     3,     1,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     0,     5,
       4,     3,     7,     1,     0,    10,     4,     4,     5,     5,
       2,     4,     1,     1,     1,     0,     1,     0,     1,     0,
       3,     0,     3,     4,     1,     1,     1,     1,     1,     4,
       4,     2,     1,     7,     4,     3,     1,     2,     4,     4,
       3,     0,     2,     2,     2,     3,     1,     3,     1,     2,
       6,     6,     1,     1,     1,     2,     0,     1,     1,     3,
       3,     1,     2,     4,     4,     1,     1,     8,     4,     4,
       4,     2,     1,     0,     3,     3,     4,     4,     4,     4,
       2,     1,     1,     0,     1,     0,     1,     5,     1,     0,
       1,     0,     3,     1,     3,     2,     2,     1,     1,     0,
       4,     4,     2,     2,     4,     1,     0,     1,     1,     1,
       1,     1,     3,     0,     2,     0,     3,     3,     1,     0,
       2,     0,     2,     0,     3,     1,     1,     2,     1,     2,
       5,     6,     5,     1,     2,     1,     4,     3,     4,     3,
       5,     4,     5,     4,     5,     2,     4,     1,     2,     2,
       2,     1,     1,     0,     4,     2,     1,     2,     2,     4,
       3,     4,     2,     0,     1,     3,     2,     2,     2,     3,
       4,     0,     1,     3,     3,     3,     3,     3,     3,     1,
       1,     1,     1,     1,     2,     5,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       2,     2,     2,     1,     3,     0,     5,     3,     0,     5,
       3,     0,     1,     1,     1,     1,     5,     2,     1,     1,
       1,     1,     5,     2,     2,     2,     1,     3,     3,     2,
       1,     0,     3,     0,     2,     0,     5,     2,     5,     2,
       1,     3,     3,     0,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     3,     3,     3,     3,     0,     3,     4,
       4,     3,     3,     3,     4,     3,     5,     4,     4,     3,
       3,     3,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     4,
       1,     4,     1,     3,     5,     2,     2,     2,     2,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     3,     3,     2,     3,     5,     4,     6,
       3,     5,     4,     6,     4,     6,     5,     7,     2,     3,
       2,     4,     3,     4,     3,     4,     3,     4,     5,     6,
       7,     5,     6,     3,     4,     4,     2,     1,     1,     3,
       2,     2,     2,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     5,     6,     7,
       1,     1,     3,     3,     5,     1,     3,     4,     5,     5,
       4,     1,     2,     4,     2,     4,     2,     2,     2,     6,
       4,     4,     4,     4,     6,     5,     5,     5,     4,     4,
       1,     2,     4,     6,     0,     1,     3,     3,     3,     0,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     3,     2,     3,     0,     3,     3,     2,     2,     1,
       0,     2,     2,     3,     2,     1,     3,     1,     0,     1,
       3,     1,     3,     1,     3,     5,     6,     4,     2,     1,
       4,     2,     0,     1,     0,     2,     2,     2,     2,     2,
       3,     2,     0,     3,     1,     3,     1,     1,     3,     1,
       1,     4,     3,     1,     1,     1,     1,     1,     1,     3,
       1,     1,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     3,     6,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     5,     3,     2,     3,     7,     1,     2,     2,     1,
       0,     1,     3,     1,     2,     0,     2,     0,     2,     0,
       1,     3,     4,     3,     1,     1,     1,     2,     0,     6,
       3,     2,     0,     4,     5,     5,     0,     1,     1,     1,
       2,     2,     1,     2,     0,     7,     0,     0,     7,     1,
       1,     1,     1,     0,     1,     1,     0,     1,     1,     1,
       1,     1,     3,     2,     3,     4,     4,     2,     2,     1,
       2,     2,     3,     1,     2,     2,     3,     3,     4,     2,
       1,     2,     1,     1,     2,     2,     3,     1,     1,     1,
       0,     1,     3,     4,     0,     2,     0,     1,     2,     3,
       2,     1,     1,     1,     0,     1,     1,     3,     0,     5,
       1,     1,     2,     3,     0,     2,     0,     1,     2,     4,
       0,     1,     1,     1,     1,     3,     4,     3,     3,     3,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     1,
       3,     4,     6,     6,     2,     4,     4,     1,     1,     4,
       4,     0,     7,     3,     3,     4,     4,     0,     1,     0,
       0,     7,     3,     4,     3,     1,     1,     1,     2,     3,
       5,     2,     5,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     1,     0,     1,
       2,     1,     2,     1,     1,     2,     2,     1,     1,     1,
       1,     1,     2,     3,     1,     2,     1,     3,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned short yydefact[] =
{
       3,     0,     2,     1,  1709,  1710,  1712,  1713,  1714,  1647,
    1715,  1716,  1686,  1687,  1688,  1689,  1690,  1691,  1692,  1693,
    1694,  1695,  1696,  1697,  1699,  1698,  1700,  1701,  1702,  1703,
    1704,  1705,  1706,  1707,  1708,  1717,  1718,  1719,  1711,  1685,
    1720,  1721,  1675,  1095,  1676,  1653,     9,  1091,  1092,  1724,
    1681,  1682,  1680,  1683,  1684,  1722,  1723,  1671,  1672,  1674,
      10,    11,  1673,     4,  1096,  1678,  1679,  1677,     7,  1152,
       0,  1097,     8,  1670,  1173,     0,     0,  1224,     0,     0,
    1290,     0,   572,     0,   603,   602,     0,   572,   185,   204,
       0,     0,   572,   218,   479,     0,     0,     0,   315,   572,
       0,   605,     0,     0,     0,     0,     0,     0,   673,     0,
       0,     0,     0,     0,   459,   572,   679,     0,     0,     0,
     673,  1281,     0,     0,   607,     0,     0,     0,    33,    16,
      17,    45,    27,    14,    39,    26,    69,    70,    71,    72,
      73,    15,    18,    20,    21,    22,    28,    29,    41,    32,
      43,    30,    42,    35,    31,    40,    36,    37,    47,    19,
      48,    59,    49,    25,    57,    55,    56,    23,    38,    54,
      58,    61,    44,    53,    50,    51,    62,    63,    64,    65,
      13,    66,    24,    74,    34,    67,    68,   605,    46,    60,
     617,   618,    52,   616,   615,   614,   656,   647,   681,   655,
      76,    77,    78,    80,    81,    82,    83,    86,    87,    79,
      75,    85,    84,    88,    89,    90,    91,    92,    93,  1172,
    1169,  1171,  1170,  1164,     0,  1173,  1159,  1158,  1176,     0,
    1303,  1304,  1305,  1306,  1307,  1308,  1309,  1310,  1311,  1312,
    1313,  1314,  1315,  1316,  1317,  1318,  1319,  1320,  1321,  1322,
    1323,  1324,  1325,  1326,  1327,  1328,  1329,  1330,  1331,  1332,
    1333,  1334,  1368,  1369,  1370,  1371,  1372,  1373,  1374,  1375,
    1376,  1377,  1378,  1379,  1380,  1381,  1534,  1535,  1382,  1383,
    1384,  1385,  1348,  1536,  1386,  1387,  1388,  1389,  1390,  1537,
    1391,  1392,  1393,  1394,  1395,  1396,  1397,  1398,  1399,  1400,
    1401,  1402,  1403,  1538,  1539,  1404,  1405,  1406,  1407,  1408,
    1409,  1410,  1411,  1412,  1413,  1414,  1415,  1416,  1417,  1540,
    1418,  1541,  1419,  1542,  1420,  1421,  1422,  1423,  1424,  1425,
    1426,  1427,  1428,  1429,  1430,  1431,  1432,  1433,  1434,  1435,
    1543,  1544,  1436,  1437,  1439,  1438,  1440,  1441,  1442,  1443,
    1444,  1445,  1446,  1447,  1448,  1449,  1450,  1451,  1452,  1453,
    1454,  1545,  1455,  1456,  1457,  1458,  1546,  1459,  1460,  1547,
    1548,  1461,  1462,  1463,  1464,  1465,  1466,  1467,  1468,  1469,
    1470,  1549,  1471,  1472,  1473,  1474,  1476,  1475,  1477,  1550,
    1478,  1479,  1480,  1481,  1482,  1483,  1484,  1485,  1486,  1487,
    1551,  1488,  1489,  1490,  1491,  1493,  1494,  1492,  1495,  1552,
    1496,  1497,  1498,  1553,  1499,  1500,  1501,  1502,  1503,  1504,
    1505,  1554,  1506,  1507,  1508,  1509,  1555,  1556,     0,  1510,
    1511,  1557,  1512,  1558,  1513,  1514,  1515,  1516,  1517,  1518,
    1519,  1520,  1521,  1522,  1523,  1524,  1559,  1525,  1526,  1527,
    1528,  1529,  1531,  1530,  1532,  1533,  1657,  1658,  1100,  1145,
    1144,  1123,  1140,  1347,  1118,  1345,  1346,  1101,  1344,  1222,
    1223,  1226,  1130,  1126,  1225,     0,  1220,  1221,  1075,  1344,
    1232,  1074,  1234,     0,     0,     0,     0,   571,   570,   563,
       0,     0,     0,     0,     0,    12,     0,   564,   202,   203,
       0,  1078,     0,   566,   217,     0,     0,     0,     0,     0,
       0,     0,   370,     0,     0,     0,     0,     0,     0,     0,
     228,   227,     0,   307,     0,   464,     0,     0,   315,     0,
       0,     0,     0,     0,  1151,  1501,   646,     0,     0,     0,
       0,   382,     0,   381,     0,     0,   379,     0,   314,     0,
     383,   377,   376,     0,   380,     0,   378,     0,     0,   567,
    1426,  1228,   604,   605,     0,   402,   404,   400,   399,   408,
     407,   405,   406,   401,     0,     0,     0,     0,     0,  1098,
     403,     0,   430,   443,   437,   441,   435,   439,   438,   434,
     445,   444,   440,   436,   442,     0,   431,   432,     0,  1120,
    1119,  1071,  1070,   560,  1066,  1067,   574,  1079,   672,     0,
       0,     0,     0,   398,   559,     0,     0,   528,   529,     0,
     179,  1492,  1555,  1511,   175,     0,     0,   568,   678,   676,
       0,     0,     0,  1394,  1443,  1453,  1492,  1555,  1511,   132,
       0,   174,  1492,  1555,  1511,   170,     0,     0,     0,   562,
     561,     0,     0,   746,   606,   609,   656,     0,   572,     0,
       6,  1274,   600,   675,   675,     0,   675,     0,   652,  1210,
    1153,     0,  1161,  1160,  1174,  1175,  1166,  1146,  1659,  1660,
    1248,  1122,  1137,     0,     0,     0,     0,  1129,  1135,     0,
    1127,  1216,  1233,  1335,  1336,  1337,  1339,  1338,  1340,  1581,
    1582,  1583,  1584,  1585,  1586,  1587,  1560,  1561,  1562,  1588,
    1589,  1590,  1359,  1591,  1592,  1593,  1594,  1595,  1563,  1596,
    1597,  1598,  1599,  1600,  1601,  1602,  1603,  1604,  1605,  1606,
    1607,  1608,  1609,  1610,  1564,  1611,  1565,  1612,  1613,  1614,
    1566,  1567,  1615,  1568,  1360,  1616,  1617,  1569,  1570,  1571,
    1618,  1572,  1573,  1619,  1574,  1620,  1621,  1575,  1622,  1623,
    1624,  1625,  1626,  1627,  1628,  1629,  1576,  1577,  1630,  1631,
    1578,  1632,  1633,  1579,  1634,  1635,  1636,  1637,  1638,  1639,
    1361,  1640,  1641,  1642,  1580,  1643,  1644,  1367,  1358,     0,
    1357,  1363,  1364,  1365,  1366,  1362,     0,  1295,     0,     0,
    1296,  1297,     0,  1292,  1294,     0,     0,     0,     0,   746,
       0,   103,     0,     0,     0,   416,   417,   422,     0,   419,
       0,     0,   418,   420,   421,     0,   423,   424,     0,   220,
    1081,     0,     0,  1347,  1080,  1345,  1355,  1344,     0,     0,
     594,     0,   576,   594,   387,   386,   232,   231,   117,   230,
     229,     0,   867,   877,   875,   876,   869,   870,   871,   874,
     872,   873,     0,   868,   478,   126,  1118,     0,     0,    96,
       0,     0,     0,     0,     0,     0,     0,     0,  1247,  1150,
    1219,   642,   643,   644,     0,   753,     0,   348,     0,   591,
       0,   122,     0,     0,     0,     0,   101,   113,     0,   201,
     384,  1231,  1230,  1227,  1234,     0,     0,   612,  1099,   405,
     406,     0,     0,     0,     0,  1236,   393,     0,   429,     0,
       0,     0,     0,  1047,   631,  1068,     0,     0,     0,   397,
       0,   531,   531,   178,   176,     0,     0,     0,     0,  1534,
    1535,   793,  1044,     0,  1348,  1536,  1537,     0,   971,  1052,
    1052,  1052,  1538,  1539,  1410,  1540,  1541,  1088,  1542,   783,
    1543,  1544,  1454,  1545,     0,  1089,  1547,  1548,  1463,     0,
    1549,  1550,  1551,  1052,  1553,  1554,  1555,  1556,  1557,  1558,
    1087,     0,  1052,  1559,   878,  1653,  1093,  1094,   994,     0,
       0,  1057,     0,     0,     0,   990,     0,   777,   779,   780,
     804,   805,   781,   810,   811,   823,   778,     0,   937,     0,
       0,  1056,   882,   965,   960,   994,   663,  1054,   994,     0,
     961,  1114,  1113,  1115,  1116,  1112,  1082,  1347,  1352,   782,
    1345,  1090,  1117,  1344,     0,     0,     0,     0,   181,     0,
     182,  1072,  1492,   133,   166,   140,   165,     0,  1386,   134,
       0,     0,     0,     0,   173,   171,     0,     0,   565,   388,
       0,     0,   748,     0,   747,   608,   605,   650,   649,     5,
     611,   674,     0,     0,     0,     0,     0,     0,     0,   709,
     696,  1342,  1209,  1342,  1193,  1342,  1216,  1180,  1178,  1179,
       0,     0,  1177,  1189,     0,  1181,     0,  1210,     0,  1139,
       0,  1143,  1141,  1133,  1128,  1131,     0,  1124,  1135,  1217,
    1244,     0,  1235,  1242,  1243,  1210,     0,     0,  1298,  1301,
       0,  1293,     0,   590,     0,     0,   537,   537,   537,   537,
       0,     0,     0,     0,   103,    97,   100,  1154,   596,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   349,     0,
     786,   821,   821,   821,   801,   801,   795,   784,   821,   798,
     787,   785,   833,   833,   816,     0,  1351,  1350,  1349,   593,
       0,     0,     0,     0,   117,   114,   371,   350,     0,   125,
     124,   319,   318,     0,     0,   351,    95,   103,   299,   293,
       0,     0,     0,     0,     0,   266,   645,     0,     0,   628,
       0,     0,     0,   201,     0,     0,     0,     0,     0,   201,
     199,   200,     0,   375,  1240,   613,   646,     0,     0,     0,
     392,  1237,  1651,     0,  1238,  1645,     0,     0,  1267,  1271,
    1401,  1422,  1438,  1489,     0,     0,   446,   433,     0,     0,
       0,   619,   622,  1049,  1048,  1077,     0,     0,   629,     0,
     396,   395,  1246,   530,   527,   526,   177,   458,     0,     0,
     820,   807,  1043,     0,     0,   815,   814,     0,  1028,     0,
     972,     0,   974,     0,   976,     0,   791,   790,   789,   991,
     999,     0,   788,   821,   821,   819,   905,     0,     0,   792,
       0,     0,  1014,     0,   977,  1020,     0,     0,     0,   829,
       0,   827,     0,     0,   936,   978,     0,  1654,  1652,     0,
    1086,   994,   885,   886,   887,   888,   990,     0,     0,   863,
    1083,     0,     0,   813,     0,   847,     0,     0,     0,     0,
       0,   880,     0,   901,     0,     0,     0,     0,     0,     0,
       0,     0,   918,     0,     0,   920,     0,     0,     0,   878,
     877,   875,   876,   869,   870,   871,   874,   889,   890,   902,
       0,  1046,  1236,     0,   713,   662,  1045,     0,  1278,  1277,
    1276,  1275,  1279,  1280,   183,   184,   180,     0,     0,   142,
     141,   169,   168,     0,   163,   830,   164,     0,   137,     0,
    1106,  1105,  1110,  1111,   159,  1109,   160,     0,   144,   156,
     158,   157,   155,   135,   143,   145,   152,   153,   154,   136,
     172,     0,  1210,     0,  1060,   713,  1059,   994,   597,     0,
     601,   656,   660,   655,   659,   682,   683,   689,   658,     0,
     711,   698,   699,   692,   697,   701,   693,   700,   708,   654,
     695,   653,     0,  1183,  1343,  1187,  1341,  1195,  1191,  1199,
    1194,  1188,     0,  1173,  1173,  1207,  1203,  1200,  1208,  1202,
    1190,     0,  1167,  1136,     0,  1121,  1142,     0,  1134,     0,
    1218,  1216,   761,  1287,  1648,  1299,  1648,   589,     0,     0,
       0,   536,     0,     0,     0,     0,   193,   260,     0,     0,
       0,     0,   535,     0,     0,     0,    99,    98,   106,   108,
       0,   107,   109,     0,     0,     0,   102,     0,     0,     0,
       0,     0,     0,     0,   219,   207,   206,     0,     0,   356,
     359,     0,     0,     0,     0,   588,   588,   588,   588,   575,
     577,     0,   243,   761,   769,   771,   772,   802,   803,   773,
     808,   809,   770,   847,   782,   115,     0,     0,   116,     0,
       0,   233,   127,   129,   130,   131,   324,   323,   325,     0,
     320,     0,   352,    94,   297,     0,   235,     0,     0,     0,
       0,     0,   538,     0,     0,     0,   752,   513,     0,   512,
       0,   486,   488,   487,   481,     0,   482,     0,   485,   492,
     782,   510,     0,     0,     0,   515,   201,   201,   112,   313,
     385,  1241,  1229,     0,     0,   391,   390,  1650,     0,     0,
       0,     0,     0,     0,   449,   448,   460,     0,   450,   451,
     447,     0,   621,     0,     0,   626,   994,  1050,     0,   638,
       0,   636,     0,  1069,   394,     0,   995,     0,     0,     0,
    1042,  1039,     0,     0,  1029,     0,  1027,  1051,     0,     0,
       0,  1005,  1006,  1007,  1004,  1008,  1003,     0,     0,  1009,
    1002,     0,   818,   817,     0,     0,     0,   519,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   938,     0,   859,
       0,   863,   995,  1019,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   995,  1025,     0,  1655,  1656,     0,
     962,   860,     0,   963,     0,     0,     0,   836,   837,   838,
     835,   839,   834,  1084,   848,     0,     0,   853,     0,     0,
       0,     0,   855,   866,   864,   865,     0,   851,   852,     0,
     883,   903,  1055,     0,     0,   910,     0,  1031,   933,     0,
     924,     0,   919,     0,   922,   926,   906,     0,     0,     0,
       0,     0,     0,   904,     0,   899,   897,   898,   891,   892,
     893,   894,   895,   896,   900,     0,  1423,  1443,     0,   673,
     673,   661,   671,  1053,     0,   753,     0,     0,     0,   966,
       0,  1073,     0,  1108,  1107,     0,   847,     0,   148,   151,
       0,   569,  1287,   749,     0,   753,     0,   599,   598,     0,
       0,   687,   688,     0,   685,   707,     0,   706,     0,     0,
       0,     0,  1182,  1197,  1196,  1192,  1287,  1173,  1162,  1173,
    1205,  1204,  1201,     0,  1216,  1138,  1132,     0,  1245,     0,
    1214,     0,     0,  1289,     0,     0,     0,  1664,  1668,  1649,
    1666,  1661,     0,   120,   121,     0,     0,     0,     0,     0,
     186,     0,     0,   201,   201,     0,   534,   195,   196,     0,
       0,   104,   105,     0,     0,     0,     0,   426,   415,   425,
       0,     0,   409,   209,   210,   222,   208,   355,     0,     0,
       0,     0,     0,     0,   587,     0,     0,     0,     0,   578,
     758,   825,   757,     0,   774,   776,   119,   118,     0,   128,
       0,     0,     0,     0,     0,   754,     0,     0,   301,     0,
       0,     0,     0,   298,     0,     0,     0,   234,   236,   238,
     239,   240,     0,   295,   296,   290,     0,     0,   467,     0,
       0,     0,   268,   269,     0,   641,  1149,   201,     0,   480,
       0,   484,   776,   201,  1076,     0,   201,     0,   558,   346,
       0,   389,  1239,  1646,     0,  1269,  1272,  1273,  1250,  1249,
    1268,     0,   462,     0,   457,   452,   454,  1065,  1064,     0,
    1063,     0,     0,   627,   639,   632,   634,   633,     0,     0,
     630,     0,     0,   677,     0,     0,     0,     0,  1038,     0,
       0,  1037,     0,   989,     0,   973,   975,   800,     0,   980,
       0,   794,     0,   797,     0,   879,     0,     0,     0,   981,
     940,   941,   942,     0,   955,     0,     0,     0,     0,   878,
       0,     0,     0,     0,     0,     0,     0,   943,   944,   956,
     982,   857,   858,     0,     0,  1017,  1018,   983,   831,   832,
     833,   833,     0,     0,  1024,     0,     0,     0,   988,   994,
       0,   861,   994,   822,   823,     0,     0,     0,     0,     0,
       0,   854,   849,     0,   863,   850,     0,     0,     0,  1033,
       0,     0,   925,   921,     0,   923,   927,     0,     0,     0,
     912,   934,   908,     0,     0,   914,   935,   673,   673,   673,
     673,   670,     0,     0,     0,   723,   712,   715,   725,   716,
     718,  1071,     0,   703,     0,     0,   970,   967,     0,     0,
     161,   147,     0,   138,   146,  1289,  1058,   640,     0,   610,
     684,   686,   710,   690,   694,   691,     0,  1198,     0,  1185,
    1163,  1186,  1206,  1155,     0,  1211,  1148,   761,     0,     0,
     762,     0,  1213,  1287,     0,  1287,     0,  1288,  1291,  1662,
       0,     0,  1302,  1665,     0,  1300,     0,   259,     0,     0,
       0,   243,     0,     0,   187,   194,   192,     0,     0,   110,
     111,     0,   411,     0,     0,     0,   223,    96,     0,   357,
    1463,   870,   358,   360,   491,   361,   362,     0,     0,   825,
       0,   584,  1102,  1103,   583,  1104,   580,   579,   586,   585,
     582,   581,     0,     0,     0,     0,   254,     0,     0,   248,
       0,     0,   249,   242,   245,   246,   592,     0,     0,   123,
       0,     0,   321,   354,     0,   756,   300,   302,   303,   304,
     305,   235,   258,   286,     0,   292,     0,     0,   312,   310,
     309,  1074,     0,     0,     0,     0,     0,   265,     0,   573,
     511,   201,   483,     0,   374,   517,   514,   518,   516,     0,
       0,     0,   461,   455,     0,     0,   427,   620,     0,     0,
     624,   625,   637,   635,   428,   997,   996,   806,     0,  1041,
    1035,     0,  1026,  1030,     0,   998,     0,     0,   520,  1012,
    1011,   939,  1013,     0,     0,     0,   954,   953,   951,   952,
     945,   946,   947,   948,   949,   950,  1022,  1021,  1015,  1016,
     828,   826,     0,   985,   986,   987,  1023,   992,     0,   964,
     812,   847,   841,   842,   843,   844,   846,   845,   840,   856,
     881,   884,   931,   911,  1032,     0,   928,     0,  1001,     0,
     907,     0,     0,     0,   916,   879,     0,     0,     0,     0,
       0,   665,   664,   723,     0,   725,     0,   724,   737,     0,
       0,   743,   741,     0,   743,     0,   743,     0,     0,   717,
       0,   719,   737,     0,     0,   705,   968,   969,     0,     0,
     150,   149,  1282,  1061,  1184,  1165,  1168,  1216,     0,  1125,
     759,     0,     0,     0,     0,     0,     0,   761,  1215,  1283,
    1287,  1284,  1287,  1663,  1669,  1667,   261,     0,     0,   262,
     825,   198,     0,     0,     0,     0,     0,   532,   533,     0,
       0,   414,   413,   212,     0,   343,     0,     0,   347,   338,
     340,     0,     0,   353,   336,     0,   824,     0,   252,   256,
     257,   255,   247,   250,   266,   847,     0,     0,   328,     0,
       0,   755,     0,     0,   289,   237,   294,   296,   291,     0,
     306,     0,   466,     0,     0,     0,     0,   498,     0,     0,
       0,   499,   503,   500,   509,   494,   554,   555,   552,   553,
       0,   267,   525,   493,  1252,  1253,  1254,  1255,  1256,  1257,
    1258,  1259,  1260,  1261,  1263,  1264,  1265,  1262,  1266,  1251,
    1270,     0,   453,  1062,     0,  1040,   979,   799,  1036,   796,
    1010,     0,     0,     0,   984,   994,  1085,  1034,     0,   929,
       0,   932,   913,   909,     0,   915,   669,   668,   667,   666,
     727,   735,     0,   714,     0,   742,   738,     0,   739,     0,
       0,   740,   729,     0,     0,   735,     0,   750,     0,     0,
       0,   657,   139,   847,  1212,  1147,   763,   764,   765,   766,
     767,   768,   760,  1285,  1286,     0,   263,   241,   188,   197,
     189,   190,   191,   410,   412,   205,   221,   345,   344,   342,
     339,   341,     0,   524,     0,     0,     0,     0,   244,   272,
     775,     0,   235,   327,     0,     0,   322,   289,     0,     0,
       0,   225,   311,     0,     0,   468,   469,   471,   476,     0,
    1077,   496,   489,     0,     0,   497,     0,   506,   507,     0,
     495,   477,     0,   456,     0,   957,     0,     0,   993,   930,
    1000,   917,   726,     0,     0,   728,     0,     0,   731,   733,
       0,     0,     0,     0,     0,  1074,   751,   702,   704,   162,
       0,   213,   594,   594,   214,   211,   524,     0,   522,   337,
       0,   251,     0,   277,     0,   329,   330,   326,     0,   226,
       0,   287,   288,   595,   753,     0,     0,   474,   473,     0,
       0,     0,   504,   505,     0,   508,   753,   623,     0,   958,
       0,   736,   745,     0,   732,   730,   720,     0,   722,   266,
       0,     0,   521,   523,     0,   270,   271,     0,   253,   273,
     274,     0,     0,   285,   463,   470,   475,     0,   490,   501,
       0,     0,   959,   734,     0,   721,   272,   215,   216,     0,
       0,     0,     0,   275,     0,   276,     0,     0,     0,   363,
     364,   333,   476,   502,   557,   744,   277,     0,   282,     0,
     281,     0,   278,   279,     0,     0,   369,     0,     0,   331,
     334,   335,   472,   556,     0,     0,     0,   280,   283,   284,
       0,   373,   365,   316,     0,   540,   551,   539,   541,   549,
     546,   548,   547,   545,   264,     0,   368,   372,     0,   366,
     332,     0,   550,   544,   656,   647,     0,     0,   542,   551,
     333,   373,   543,     0,   367,   317
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short yydefgoto[] =
{
      -1,     1,     2,    63,   126,   127,   128,  1187,   129,   130,
     131,  1135,  1506,   896,   132,  1175,  1548,   133,   134,   135,
    1178,  1180,  1552,   136,   639,  1403,  1404,  1789,  2113,  1405,
    1406,  1388,  1045,  1380,   137,   138,   139,  1039,  1376,   140,
     141,  2164,  1213,   142,   498,   143,  1517,  1875,  2575,  2655,
     505,  1146,  2177,  2178,   144,   527,  1916,  1917,  1918,  1919,
    1891,  2213,  2214,  2215,  1920,  1921,  1487,  1574,  1931,  1932,
    2663,  2708,  2709,  2710,  2752,  2454,  2601,   145,  2237,  1567,
    1922,  1923,   146,  1564,  1913,   147,   528,  2238,  2460,   148,
     557,   149,  1183,  1559,  1560,  2449,  2594,  2667,  2758,  2759,
    2585,  2428,  2429,  2430,   150,   151,   152,   153,  1562,  1148,
    1518,  1519,  2182,   154,  2739,  2740,   529,  2789,   155,   156,
     558,   899,  2677,   157,   158,   575,   576,   577,   159,   828,
    1868,   160,   161,   595,   596,   597,  1235,  1964,  1965,  2266,
     626,  1615,  1616,   162,   530,  2244,  2604,  2605,  2606,  2607,
    2678,   163,   531,  1203,  1585,  1586,  1587,  2611,  2183,  1588,
    2474,  2475,  2621,   164,   165,  1578,   166,  1594,  1666,   167,
    2658,   168,   169,   619,  1254,   170,  1855,  1488,   171,  1930,
    2777,  2791,  2792,  2793,  2480,  2764,   172,  2779,   174,   175,
     176,  1058,   497,   177,   178,   179,  1529,  1530,  1885,   180,
     181,   182,  1170,   183,   184,   185,   186,   187,   564,   655,
    1066,  1420,   188,   189,   190,  1241,  1624,  1625,   191,   192,
    1248,  1632,   193,   194,   884,   195,   995,   197,   198,   199,
    1364,  1771,   609,  1072,   630,   667,   668,  1425,  1426,  1804,
    1079,  1441,  1433,  1436,  2375,  2551,  1080,  1439,  1807,  1775,
    2096,  2097,  2098,  2357,  2368,  2536,  2638,  2099,  2100,  1199,
    1904,  1905,  1589,  1830,  2139,  1533,   996,   997,   998,  1282,
    1289,  1276,  1536,   999,  1000,  1001,  1539,  1002,  1003,  1004,
    1005,  1261,  1323,  2216,  1006,  1007,  1299,  1713,  1008,  1009,
    1317,  1318,  1729,  1331,   863,  1010,  1360,  1636,  1676,  1012,
    1310,  1695,  1657,  2339,  1658,  1670,  2008,  1678,  1684,  2035,
    2036,  1696,  1645,  1646,  1738,  2070,  1013,  1640,  1641,  1989,
    1263,  1014,  1015,   923,  1270,  1016,  1968,  1415,  1416,  1969,
    1970,  1018,  1236,   653,  2634,  1041,   472,  1943,  2608,   500,
     606,  1019,  1020,  1021,  1022,  1023,  1024,  1025,    67,   580,
    1869,  2194,  1393,  1407,  1026,   460,   604,   200,   474,   475,
     687,   688,  1104,  1107,  1099,  1465,   461,   462,   679,  2389,
     201,   202,    68,    74,    69,   670,   224,   225,  1817,   226,
     669,  1818,  1097,  1824,   228,   676,  1086,  1087,  1812,  1088,
    1089,  1090,  1091,  1092,  1093,  1460,  1094,  2134,  1110,  2142,
    1111,   203,   204,   476,   477,   205,   904,   903,   206,   207,
     692,  1223,   916,  1602,  1112,   208,   209,   210,  1228,  1959,
    1955,  2499,  1229,  1956,   211,   212,   213,   214,  1370,   215,
     216,   648,  1833,  2148,   217,   483,   218,   803,   463,  1028,
    1442,  1446,   605,  1029,   834,  1444,   790,   465,   466,   836,
     794,  1224,    70,  1836,  1225,  1031,  1032,  1308,   468,   680,
    1837,  1838,  1839,  1840,    72,  1841
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -2495
static const int yypact[] =
{
   -2495,   283,  6951, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
    8307, -2495, -2495, -2495,  1434,   471, 25462, 24722, 36914, 36914,
   -2495,   142,   -75,   422, -2495, -2495, 26942,   116, -2495, 36914,
   36914,    16,   -75,   443,  2794,    82, 37283,   431,  3655,   -75,
   37652,    56, 22859,   634,   889,   508, 33224,   733,   461, 22859,
   33224, 36914,   378, 33593,   521,   -75,   157, 23983, 33962,   486,
     461, -2495, 20993, 32117,   671,   -21,  9463,   447, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495,   843, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495,   544, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495,   -18, -2495,   487,    -6,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495,   867,  1686, -2495, -2495,   205, 27312,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, 25832, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495,    78, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495,   616, -2495, -2495,   632, -2495, -2495, -2495,   572,
   -2495, -2495,   691, 24353,  1207,  1207,  1053, -2495, -2495, -2495,
   36914, 36914, 32117, 36914, 36914, -2495,  1146, -2495, -2495, -2495,
     858, -2495,  1083, -2495, -2495, 33224, 31010, 36914,   730,   914,
     800, 36914, -2495, 33224,   257, 36914,   885,  1005,   907, 36914,
   -2495, -2495, 36914, -2495, 33224, -2495, 36914,   261,   996,  1155,
    1106,   391, 27312,   871, -2495,   871,   386, 32117, 31010, 36914,
     918, -2495, 36914, -2495, 31010, 36914, -2495,  1212, -2495, 36914,
   -2495, -2495, -2495, 36914, -2495, 36914, -2495,  1092, 33224, -2495,
     655, -2495, -2495,   544,    85,    -5, -2495,    20,    68, -2495,
   -2495,    90,   117,   124,   664,   400,   706, 36914,  1165, -2495,
   -2495, 27312,  1140, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495,  1113,  1026, -2495, 33224, -2495,
   -2495, -2495,  1109, -2495, -2495, -2495, -2495, -2495, -2495, 33224,
     400,   706, 36914, -2495, -2495,  1256, 36914, -2495, -2495, 33224,
   -2495,  1418,  1147,  1311, -2495,  1278,   889, -2495, -2495,  1287,
   12592,   -73,   238, 34331, 38021,   143, 34700,  1187,  1352, -2495,
     222, -2495,  1464,  1201,  1362, -2495,  1365, 33224, 24353, -2495,
   -2495, 18723,  1280,  1220, -2495,  1435,  1229,  1238,   -75,  1243,
   -2495, -2495, 33224,  1560,  1560,  1541,  1560,   600,   662, 30272,
   -2495,  1484, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495,  1571,  1554, 26942, 26942, 23611, -2495,  1268,   616,
   -2495,   575, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,  1465,
   -2495, -2495, -2495, -2495, -2495, -2495, 36914, -2495,  1350, 36914,
   -2495, -2495, 38390, -2495, -2495,  1207,   430,   574,    76,   -15,
    1425,   357,  1285, 33224, 31010, -2495, -2495, -2495, 31010, -2495,
    1731, 36914, -2495, -2495, -2495, 36914, -2495, -2495, 36914,  1321,
   -2495,  1109,  1291,  1294, -2495,  1298, -2495,  1306,  1586, 29903,
    1613, 36914,  1346,  1613, -2495,  1314, -2495, -2495,  1349, -2495,
   -2495, 33224, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495,  1291, -2495, -2495, 36914,    34,   667,    14,  1351,
   33224, 33224,  1491, 33224, 36914, 31010, 36914, 33224, -2495, -2495,
   -2495, -2495,  1415, -2495,  1579,  1356,  1328, -2495, 29903, -2495,
    1341, -2495, 33224,  1343,  1488,  1490,  1340, -2495,   733,     2,
   -2495, -2495, -2495, -2495,   691,    85, 36914, -2495, -2495, -2495,
   -2495,   706, 36914, 36914,  1165,    60, -2495,    49, -2495, 32486,
    2156,    81, 21363, -2495,   -48, -2495,   706, 36914, 36914, -2495,
     655,  1574,  1574, -2495, -2495,  1532,  1578,  1497,  1361,   909,
     591, -2495, 13768,  1363,   171,   171,  1372,  1373, -2495,  1379,
    1384,  1385,   651,   651,  1510,  1390,  1391, -2495,   727, -2495,
     946,   779,  1158,   171, 13768, -2495,  1397,   781,  1400,  1403,
    1407,  1007,  1409,  1385,  1038,  1414,   944,   980,  1416,  1419,
   -2495,  1390,  1385,    27, -2495,    57, -2495, -2495,  -123, 13768,
   13768, -2495, 13768, 13768, 12984, -2495,   733, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495,    21, -2495,   847, -2495,  2643,
   13768,  6531, -2495, -2495, -2495,   943,   -57, -2495,  -123,  1420,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495,   859, -2495, -2495,
     869, -2495, -2495,   917,   695,   695, 26202, 26202, -2495,   718,
    1396, -2495,   173, -2495, -2495, -2495, -2495, 26572,  1727, -2495,
    1402,  1597, 22103, 22103, -2495, -2495,  1599,  1604, -2495, -2495,
    1619, 33224, -2495, 21733, -2495, -2495,   544, -2495, -2495, -2495,
    1438, -2495,   -21,   -21, 13768,   -21,   -42,   133,   788,  1659,
      42, 24353, -2495, 24353,   180, 24353,  1448, -2495, -2495, -2495,
    1439,  1441, -2495, -2495,   199, -2495,  1692, 30272, 26942,  1492,
   26942, -2495, -2495, -2495, -2495,  1450,   664, -2495,  1268,  1460,
    1451, 30641, -2495, -2495, -2495, 30272,  1454, 36914, -2495,  1456,
    1458, -2495, 38021, -2495,  1506,  1507,    47,  1734,  1054,  1734,
    1528,  1534, 32117, 38021, -2495,   502, -2495, -2495, -2495,  1473,
    1341,  1476,   379,  1615,  1667,  1624,    -9, 24353, -2495,  1485,
   -2495,  1525,  1525,  1525,  1489,  1489,  1502, -2495,  1525,  1503,
   -2495, -2495,   497,   603, -2495,  1796, -2495, -2495, -2495, -2495,
   33224,  1805,     5, 29163, -2495,   -55,  1754, -2495,  1807, -2495,
     686, -2495, -2495,   129,  1508, -2495, -2495, -2495, -2495,   -24,
   26942,  1730,  1653,  1341,  1822,  1518, -2495,  1740, 13768, -2495,
   20623,  1827, 10993,    44,  1567, 28053, 33224, 33224, 36914,    44,
   -2495, -2495, 33224, -2495,  1165, -2495,   386, 36914,  1165,  1165,
   -2495, -2495,    57,  1903,  1522, -2495,   775,  1542, -2495,  1526,
   36914, 31010, 36914, 36914, 33224,  1595,  1529, -2495,  1575,  1538,
   19098, -2495, -2495, -2495,  1109, -2495,   469, 33224, -2495, 36914,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, 32486, 13768,
   -2495,  1539,  8830,  1580, 13768, -2495, -2495, 13768, 13768,   355,
   -2495,   355, -2495,  1540, -2495,   664, -2495, -2495, -2495, -2495,
    1070,   664, -2495,  1525,  1525, -2495,  5026, 13768,   664, -2495,
   20235, 13768, 14160, 10207, -2495, 13768,  1606,  1611,   664, -2495,
     664, -2495, 13768, 11808, -2495, -2495, 27682, -2495, -2495, 13768,
   -2495,  1550,  1873,  1873,    45,   200,  1596,  1553,  1549,   107,
   -2495,  1645,   664, -2495,   664,   674,  1390,   510,  1767,  1576,
     -81, -2495,    48,   209, 29163, 13768, 24353,  1633, 14160, 13768,
    1577,   654, -2495, 13768,    55, -2495,  1581, 13768,  1635,   113,
   13768, 13768, 13768, 13768, 13768, 13768, 13768, 15715, 16094, 14552,
     132, -2495, 25092, 12592,  1790, -2495, -2495,  9814, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, 36914,  1727, -2495,
   -2495, -2495, -2495,  1641, -2495, -2495, -2495,   911, -2495,   953,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495,   289, -2495, -2495,
   -2495, -2495, -2495, -2495,  1582, -2495, -2495, -2495, -2495, -2495,
   -2495,   289, 30272,  1583, -2495,   -37, -2495,  1550, 31748, 36914,
   -2495, -2495,  1771, -2495, -2495,  1585, -2495,  6118,  1771,  1722,
    1732, -2495, -2495,   274, -2495, -2495,  1769, -2495, -2495, -2495,
   -2495, -2495,  1598,  1600, -2495, -2495, -2495,   183,  1789, -2495,
   -2495, -2495, 30641,  1039,  1039, -2495,   195,  1797, -2495, -2495,
   -2495,  1966, -2495, -2495, 26942, -2495, -2495, 23611, -2495,  1618,
   -2495,   575,  1622,  1064,  9058, -2495,  9058, -2495, 36914, 36914,
    1629, -2495, 36914,  1793,  1802,  1630, -2495, -2495, 36914, 36914,
   36914, 36914, 36914,  1702, 36914,  1741, -2495,   502, -2495, -2495,
    1844, -2495, -2495,  1660,   664,  1687, -2495, 20623,  1825, 28053,
     382, 33224, 33224,   382, -2495, -2495, -2495,   563,   682, -2495,
    1655, 13768, 29903,  1855,   129,  1662,  1662,  1662,  1662,     5,
   -2495, 29533, -2495,  1622, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495,   620,  1109,   -55,   664, 36914, -2495,  1870,
   36914,   812, -2495, -2495, -2495, -2495, -2495, -2495, -2495,  1791,
    1785, 36914, -2495,   502,   628, 33224, 31379,  1958,  1862,   733,
   33224,  1763, -2495, 36914,  1967,   520,  8830, -2495,  1663, -2495,
   29903, -2495,   630,   854, -2495,   722, -2495, 29163, -2495, -2495,
    1109, -2495, 36914,  1657,  1664,  1661,    44,    44, -2495, -2495,
   -2495, -2495, -2495,  1923,  1165, -2495, -2495, -2495, 27312,  1689,
    1693,  1693,  2055,  1694,  1396,  1669, -2495,  1341,  1396,  1396,
    1529, 35069, -2495, 12200,   823, -2495,  1550, -2495,    29, -2495,
     358,   370,  1839, -2495, -2495,  1894,  8830,   304,   664, 13768,
       9, -2495,  6856,   543,  7375,  1679,  1675, -2495,  1681,  1682,
     830, -2495, -2495, -2495, -2495, -2495, -2495,  1683,  1901, -2495,
   -2495,  1688, -2495, -2495,  3109,   897,  1691, -2495,  1685,  7445,
    1697, 14160, 14160, 14160, 12984, 14160,  1338, -2495,  1698, -2495,
    1699,  3426,  6272,   298,  1700,  1715,  1717,  1701,  1703,  7477,
   13376, 13768, 13376, 13376,  7764,   298,  1704, -2495, -2495,  2542,
   -2495, -2495, 13768,  1109, 36914,  1706,  1708,  1757,  1758,  1775,
   -2495, -2495,  1781, -2495, -2495,  1918,  1874, -2495,  1390, 20235,
    1409, 13768, -2495, -2495, -2495, -2495, 12984, -2495, -2495,  1390,
   -2495,  5026, -2495,  1756,   168,  1063, 12984, -2495, -2495,  1945,
   -2495,   490, -2495,  1733, -2495, -2495,  1308, 14160, 13768,  1577,
   13768,  1795, 20235,  4843, 13768,  2167,   148,   148,    80,    80,
      45,    45,    45,   200,   209,  1390,  1072,  1166, 33224,  2919,
    3407, -2495, -2495, -2495, 18348,  1356, 13768, 13768,  1735, -2495,
     582, -2495,  1914, -2495, -2495,   664,   674,  2005, -2495,  1852,
   22481, -2495,  1064, -2495, 36914,  1356,  1759, -2495, -2495,   898,
   13768, -2495, -2495,  1731, -2495, -2495, 36914, -2495,   788,   788,
     133,  9058, -2495,  1931, -2495, -2495,  1064,    59, -2495,    63,
    1935, -2495, -2495,  1744,   575, -2495, -2495, 36914, -2495,   951,
    1761,  1088,  1060,  2112, 11346,  9058,  1750, -2495, 17582,  1749,
   -2495, -2495,  1760,  1340,  1340, 13768,   130,  1762,  1766, 36914,
   -2495, 29163,   368,    44,    44,  1823, -2495, -2495, -2495,  1824,
   36914, -2495, -2495,  1660,  1768,   382,  1770, -2495, -2495, -2495,
    1952,  1956, -2495, -2495, -2495,   105, -2495, -2495, 24353, 19848,
    4352,  1772,  1291,  1913, -2495,   212,   -20, 35438, 35807, -2495,
   -2495,  1676, -2495,   664, -2495, -2495, -2495,  1340,  1833, -2495,
    1851,  1818, 33224,   129,   930, -2495, 29163,  1042, -2495,  1042,
    1042,  1042,  1042, -2495,  1784, 33224,  1786,  1779, -2495, -2495,
   -2495, -2495,   936, -2495, 29163, -2495, 33224,  1850,  1832, 29163,
    1927,  1004, -2495, -2495,   -21, -2495, -2495,    44,  1792, -2495,
   28423, -2495,  1803,    44, -2495, 29163,    44, 28793, -2495, -2495,
    2011, -2495, -2495, -2495,  1812, -2495,  1794,  1794, -2495, -2495,
   -2495, 31010, -2495, 36914,  -101, -2495, -2495, -2495, -2495,  1062,
   -2495,   248, 36914, -2495, -2495, -2495, -2495, -2495,  2021,  2030,
   -2495, 35069, 13768, -2495, 13768,  1801,  8773, 13768, -2495,  2035,
   29163, -2495, 33224, -2495, 13768, -2495, -2495, -2495,   664, -2495,
   13768, -2495, 13768, -2495,   664, -2495, 20235, 13768,  2022, -2495,
    2126,    50,  2126,   107,   337, 29163, 14160,   433,  1400, 15323,
   14160, 14160, 14160, 14160, 14160, 14160, 14160, 16473, 16852, -2495,
   -2495, -2495, -2495, 13768, 13768,  2028,  2022, -2495, -2495, -2495,
    1082,  1082, 29163,  1808,   298,  1810,  1811, 13768, -2495,  1550,
   13768,  3161,  1550, -2495,  2097,   733,    17,     6,  1911,  1979,
     -81, -2495, -2495,  1826,  8830, -2495, 14944, 14160, 13768,  8830,
    1068, 13768, -2495, -2495,  1828, -2495, -2495, 29163, 13768,  1266,
    3039, -2495,  3492, 13768,  1838,  4786, -2495,   461,   461,   461,
     461, -2495, 33224, 33224, 17973, 36176,  1817,   877, -2495, 36176,
   36545,  1840,  1841,  2033,   704,   710, -2495, -2495,  2001,  1845,
   -2495, -2495,   -49, -2495, -2495,  2112, -2495, -2495, 13768, -2495,
   -2495, -2495,  1396, -2495, -2495, -2495,  1075, -2495,  1847, -2495,
   -2495, -2495, -2495, -2495,  1099, -2495,  1865,  1622,   595,  1345,
   -2495,  9058, -2495,  1064,  1569,  1064,  1234, -2495, -2495, -2495,
   11415,  1134, -2495, -2495,  9058, -2495,  5397, -2495, 36914, 36914,
    1069, -2495,    79,   660, -2495, -2495, -2495, 36914, 36914,  1340,
   -2495,  2029, -2495,  2032,   382,   382, -2495,  1351,  2094, -2495,
    1576,   911, -2495, -2495, -2495, -2495, -2495,   631,  1226,  2127,
   33224, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495,  1853, 36914, 36914, 14160, -2495,   773,   140, -2495,
    2034, 33224, -2495, -2495, -2495, -2495, -2495,  1856, 29163,   686,
   33224,  2078,  1994, -2495, 36914, -2495, -2495, -2495, -2495, -2495,
   -2495, 31379, -2495,  2059, 31379, -2495, 36914,   -21,  2041, -2495,
   -2495,  1314,   733, 36914,  1866,  1899,   590, -2495, 36914, -2495,
   -2495,    44, -2495,  1921, -2495, -2495, -2495, -2495, -2495,   -21,
     163,  1693, -2495, -2495,  2079, 35069, -2495, -2495, 12200,  1868,
   -2495, -2495, -2495, -2495,  1867,  8830,  8830, -2495, 13768,  8830,
   -2495,  1871, -2495,  8830,  1875,  8830,  5465,  1876, -2495,  8830,
    2028, -2495,  1842,  2089,  2023,  1879,   337,  1842,  1505,  1505,
     111,   111,    50,    50,    50,  2126,  8830,  8830, -2495, -2495,
   -2495, -2495,  1881, -2495, -2495, -2495,   298, -2495,  5737, -2495,
   -2495,   674, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495,   977,  2419, -2495, 13768,   322, 29163, -2495,  1078,
    2419, 14160, 13768, 13768,  5114,   165, 13768, 33224, 33224, 33224,
   33224, -2495, -2495, 17229,   877,  1882, 36914, -2495,  1884, 18348,
    2067,  2018, -2495, 18348,  2018,   698,  2018, 18348,  2069, -2495,
   19473, -2495,  1889, 10600,  2181,  2099, -2495, -2495,   289,   733,
   -2495, -2495, -2495,  8830, -2495, -2495, -2495,   575, 36914, -2495,
   -2495,  1276,   595,   595,   595,   595,   595,  1622, -2495, -2495,
    1064, -2495,  1064, -2495, -2495, -2495, -2495,  1089,  1091, -2495,
    1676, -2495,  2039, 13768,  2040,   664, 36914, -2495, -2495,   382,
     382, -2495, -2495, -2495,   733, -2495,   793,  2143, -2495,  2093,
      91,  2110,  2111, -2495,  2114, 13768, -2495,   492,  1842, -2495,
   -2495, -2495, -2495, -2495,  1518,   674,  1946,   309,  2137,  2131,
     129, -2495,  1905,  1908,  1305, -2495, -2495, -2495, -2495,   733,
   -2495,  2125, -2495, 31010,   733,  2050,  2009, -2495, 26942,  2063,
     102, -2495, -2495, -2495,  1848, -2495, -2495, -2495, -2495, -2495,
    1982, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495,  2054, -2495, -2495, 12200,  8830, -2495, -2495, -2495, -2495,
   -2495, 14160,  1922, 14160, -2495,  1550, -2495,  8830,  1103, -2495,
   29163,   977,  2419,  2419, 13768,  2419, -2495, -2495, -2495, -2495,
   36176,  1924, 36914,   877, 18348, -2495, -2495,  1115, -2495, 18348,
    2102, -2495, -2495, 18348, 36914,  1925, 36914, -2495,   728, 13768,
   13768, -2495, -2495,   674, -2495, -2495, -2495,   617,   617, -2495,
   -2495, -2495, -2495, -2495, -2495,  2047, -2495, -2495, -2495,  8830,
   -2495, -2495, -2495, -2495, -2495,   505, -2495, -2495, -2495, -2495,
   -2495, -2495, 31010,  2229, 33224,   631,  5680,  2075, -2495,  2106,
   -2495, 36914, 31379, -2495,   681,  2056, -2495,  1305, 33224,  2081,
    2082, -2495, -2495, 33224,  1937,  1940, -2495, -2495, 32855,  1947,
    1314, -2495,  1943,  2096,   463, -2495,  2095, -2495, -2495,  1291,
   -2495, -2495, 33224, -2495,  1117,   757, 14160,   744, -2495, -2495,
   -2495,  2419, -2495, 36914,  1125, -2495, 13768,  1951, -2495, -2495,
   18348,  1115,  1126, 36914,  1132, 29163, -2495,   298,  8830, -2495,
   33224, -2495,  1613,  1613, -2495, -2495,  2229,  2259, -2495, -2495,
    2178, -2495,    40,  2098,  2265, -2495, -2495, -2495, 36914, -2495,
    1141, -2495, -2495, -2495,  1356, 36914, 33224, -2495, -2495, 36914,
     733,  2155, -2495, -2495,  2113, -2495,  1356, -2495,   810, -2495,
    1144, -2495,  8830, 36914, -2495, -2495, -2495,  1145, -2495,  1518,
     733,   733, -2495, -2495,  2201, -2495, -2495,    -1, -2495,  2108,
    2115,    26,  1968, -2495, -2495, -2495, -2495,  1161, -2495, -2495,
    2166,  2213, -2495, -2495,  1172, -2495,  2106, -2495, -2495,  2074,
     583,   583,  2025, -2495,  2220, -2495,   664,   664, 29163,  1970,
   -2495, 23234, 32855, -2495,  2170, -2495,  2098,  2208, -2495,  2298,
   -2495,    71, -2495, -2495, 31010, 20235, -2495,    26,  1181, -2495,
   -2495, -2495, -2495, -2495,   566,   631,  2104, -2495, -2495, -2495,
    1341,   -70, -2495, -2495, 23234, -2495,   641, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, 31010, -2495, -2495, 28053, -2495,
   -2495,  1163, -2495, -2495,  1209,  1238,  1990,  1998, -2495,   641,
   23234,  2116, -2495,  1192, -2495, -2495
};

/* YYPGOTO[NTERM-NUM].  */
static const short yypgoto[] =
{
   -2495, -2495, -2495, -2495, -2495,  2272, -2495,   224, -2495, -2495,
   -2495,  -403, -2495, -1384, -2495,  1225, -2495, -2495, -2495, -2495,
   -2495,   506, -2495, -2495,  -543,  1353, -2495, -1375, -2495,   614,
   -2495, -2495, -2495, -1162, -2495,   799, -2495, -2495, -2495, -2495,
   -2495, -2495, -1131, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -1156,   856,   177, -2495,   175,   922,
     250, -2495,   -25, -2495, -2495,  1288,   569, -2314, -1685,   172,
    -307,  -325,  -287,  -285,  -306, -2495,  -164, -2495, -2495, -2495,
   -2495,   198, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,
    1907, -2495, -2495,   912, -1820, -2495, -2495, -2495,  -363,  -336,
   -2495, -2425,    13,    10, -2495, -2495, -2495, -2495, -2495,  -849,
   -2495,   567, -2495, -2495, -2495,  -313,  1270,  -353, -2495, -2495,
   -2495, -2495,  -510, -2495, -2495,  2340,   -28,   -45, -2495, -2495,
   -1459, -1150, -2495,  1829, -2495,  1530,  1194,   473,   191, -2495,
   -2495, -2495, -1890, -2495, -2495, -2495, -2495, -2495, -2495,  -216,
    -282, -2495,   910, -1132, -2495,   523, -2495, -2495,   536, -1484,
   -2495,    -7, -2495, -2495, -2495,   961, -2495, -1486, -1690, -2495,
    -187, -2495, -2495, -2495,  1543, -2495, -2495,   485, -2495, -2495,
   -2495, -2495,  -292,  -322, -2495, -2495, -2495,   202, -2495, -2495,
   -2495, -2495,   972, -1147, -2495, -2495, -2495,   950,   106, -2495,
   -2495, -2495,  -839, -2495, -2495, -2495,  1065,  2385,  -152, -2495,
   -2495, -2495, -2495,  -458, -2494, -2495, -2495,   515, -1612, -2495,
   -2495, -2495, -1591, -2495,  1272,  -876,   -58,  -119,   -54,   284,
   -2495, -2495,   -77,   954, -2495, -2495, -2495, -2495,   689, -2495,
    1410, -2495,   684,  -133, -2495, -2495,  1412, -2495, -2495,  1081,
   -2495, -2017,   403, -2010,   134,  -737,  -143,   -83, -2495, -1715,
   -1885,   276, -1158, -1494, -1723,   971,  -769,  1139,  1319, -2495,
   -2495,  -787, -2495, -2495,  2105,  2777, -2495, -2495,  2849,  2932,
   -2495,  -822,   449, -2053,  3436,   933,  -950, -1487, -2495, -1277,
    1211, -2495,  1148,  -516, -2495,  6986,  -968,  4177, -1095, -1223,
    -997, -1208, -2495,   170, -2495, -2495, -2495, -2495, -2495, -1741,
   -1887,  -429, -2495, -2495,   760, -2495, -2495, -2495,   874, -2495,
   -2495, -2495,  3653,  -922,   208, -2495,  -578, -2495,   723,    12,
     255,  4189,  -607,   290,  -628,   155,  -479, -2151,  1603,  1652,
   -2495,  -389, -2495,    36,   636, -2495, -2495,   742,   -65, -2495,
     764, -2495,  -478, -1001, -2180,  -469, -2495, -2495,   -52, -2495,
    1846, -2495,  1066,  1421, -2495, -2495,  1067,  -588,  -475, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495,  1076,  2309,
   -2495,   -67, -2495, -2495, -2495, -2495,  -969, -2495, -2495, -2495,
   -2495, -2495, -2495, -2495, -2495, -2495, -2495, -2495, -1745, -2495,
    1453, -2495, -2495, -2495,   643, -2495, -2495,  1607, -2495, -2495,
    1632, -2495,  -841, -2495,  1071, -2495, -2495, -2495,   928, -2495,
     288, -2495, -2495,   949, -2495, -2495, -2495, -2495,  1509, -2495,
   -2495, -2495, -1704,   446, -2495, -2495, -2495,  -374,  5535,  -467,
     558,  1481,  1475,  3694, -2495,  -463,  -604,  6831,  -466,  -457,
   -2495, -2495,  2339,  1090,   955,  -681,    -2,  1347,  2566,  -523,
   -1687,   731,  -976, -1289, -2495,  2565
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1510
static const short yytable[] =
{
      71,   862,   924,   843,  1173,  1040,   657,   227,  1508,   878,
    1113,   806,   196,  1177,   868,  1532,   788,   792,  1361,  1185,
     789,  1366,   807,  1866,  1553,   811,   793,  1301,  1568,  2063,
    1554,   893,   842,  1555,   495,   662,  1791,   579,    64,  1892,
     652,  1332,  1579,   647,   579,  1242,   848,  1595,   900,  1394,
     865,  1637,  1017,  1722,  1872,  1728,  1894,   869,   917,  1643,
    2103,  1571,  2084,   889,   612,  1095,  1311,   656,   196,  1677,
    1165,  2262,  1591,  1220,   467,   467,   891,  2354,  1599,  2135,
    2117,   611,  1306,  2222,   467,   902,   897,  1683,  2115,  2369,
    2371,  1043,  2462,  1049,  1843,  1844,  1101,  1102,  1226,   532,
      71,  1221,   683,  1941,  1184,   467,   907,    71,  2144,  2146,
    1210,   804,  2128,  -648,   219,  1677,  1130,   832,   219,  1201,
     220,  1334,  1265,  1266,   220,  -651,  2015,  2196,  1462,   221,
    2589,   222,  1723,   221,  1321,   222,  2433,  1724,    64,  1337,
    -816,  1285,    84,    85,  1774,    64,  1473,  1246,  2308,   886,
    2730,  2153,  1210,  1362,  1747,   890,  1334,  1126,   673,  1780,
    2660,  1127,  1480,  1897,  2160,  -128,  1277,  1525,  -648,  1481,
    1987,  2484,  1515,  1482,  1337,  1365,  1730,  2485,  2486,  2487,
    -651,   788,   792,  1334,   484,  1060,   485,  2015,  2488,  2489,
    2490,   793,  2491,  2492,  2493,  1335,  1974,  -867,  2494,  2495,
    2496,  1337,  -867,   792,  1455, -1369,  1338,  2322,  1447,  2736,
    2380,  1813,   793,  1301,  2787,  1720,  1723,  1431,  2768,  1448,
    -128,  1724,  2705,  1820,  1334,  1483,  2411,  1456,  1238,  1565,
   -1379,   906,  1526,   480,   482,  1128,    97,  2325,  1457,  2425,
    1429,   628,  1337,  1734,  2015,  1480,  1748,  1338,  2323,  -881,
    1749,   536,  2617,  1131,  -881,   561,  2067,   578,  -224,  1034,
     487,   674,   496,  2264,   613,  1077,   615,  2290,  1047,  1546,
    2780,  1309,   173,  1527,   502,   922,  1334,  1750, -1421,   675,
    1556,   116,  2780,     3,  -821,  1334,  2737,  1378,  2441,   105,
    1044,   488,  1449,  1211,  1337,  2309,  1547,  1078,  1339,  2110,
   -1455,  2265,  1340,  1337,  1141,  2780,  2326,  1430,  1483,  2706,
    1035,  1458,   629,  2618,  1700,  1721,  2288,  2324,  2381,  1341,
    1342,  2769,  1627,  1516,  1484,  2412,  2788, -1473,   173,  1343,
    1265,  1266,  1528,  1557, -1480,  1211,  1285,  2427,  1975,  1339,
    2784,  1176,  2533,  1340,  1720,  1363,  2537,  2738,  2731,  1579,
    2542,  1595,    43,  1344,  1247,    45,  1345,  2567,   533,  2191,
    1341,  1342,  1725,  1191,  1129,  1794,  1751,  1346,  1277,  1347,
    1343,  1639,  1566,  1601,  1064,   125,   682,  1605,  1606,  -648,
    2017,  -648,  1204,   116,   678,  2726,  1450,   116,   486,  1814,
    2442,  -651,  1485,  -651, -1369,  2184,   603,  1345,  1334,  -816,
     614,  1821,  -816,  2510,  1212,  1459,  2172,  1484,  1346,   808,
    1147,   905,   650,  2015,   562,  2391,  1337,  1322,  1348, -1379,
    1796,  1341,  1342,  -816,  1227,  1139,   467,  -867,  2018,  1140,
     684,  1121,  1307,  -128,   123,  1222,  1239,  1358,  2497,  2399,
    2664,  2401,  2028,  1792,  1726,  2184,  1725,  1215,  1677,  1677,
    1677,   487,  1677,   456,   885,   902,   457,  2176,  1345,  1348,
    2129,  1662,  1663,  2153,  2131,  1948,  1949, -1421,   685,  1355,
    1356,  1357,  1358,  2407,  2408,  1485,  2169,  1240,  1558,  -881,
    1349,   125,   488,  2044,   566,  1962,  1193,   881,   229, -1455,
    1350,  1351,  1352,  1095,  1353,  1354,  1355,  1356,  1357,  1358,
    2025,  2026,  2027,  2028,  1703,  2498,   196,  1472,    45,   908,
     579,  1095,  1466,    47,  1432,    43, -1473,  2635,    45,   616,
    2632,  1349,  2639, -1480,  1677,  1976,  2641,  2162,  1260,  1808,
     912,   913, -1510, -1510, -1510,  1353,  1354,  1355,  1356,  1357,
    1358,  2019,  -167,  -821,   504,   579,  -821,   911,  1628,  2017,
     116,  2020,  2021,  2022,  1052,  2023,  2024,  2025,  2026,  2027,
    2028,  2760,  1565,   490,   870,   927,   928,  -821,  2104,  2105,
    1036,  1787,    71,    71,   875,   617,  2010,  2011,  2012,  1477,
    2014,   569,   926,   846,    43,   847,   871,    45,  2293,   882,
    1496,  1510,    47,  1788,  2760,   570,  1353,  1354,  1355,  1356,
    1357,  1358,  1614,  2269,  1618,  1619,  2651,  2202,    71,   491,
      64,    64,   537,  2682,   788,   792,   788,   792,   788,   792,
    2760,  1037,  1895,  2694,   793,  1053,   793,  1620,   793,  1973,
    2596,   792,  1867,   467,  1498,  1499,  1629,  1511,    65,  2205,
     793,   909,  2554,  2390,   125,   792,    64,   113,   810,   792,
    1982,   581,  2079,   663,   793,  1124,  1982,  2652,   793,  2642,
    1523,  2644,   838,  2072,  1133,  1715,  1978,  1977,  1942,  2557,
    2558,  2559,  2560,  2561,  2683,  2163,  1809,   867,   910,  2294,
     788,   792,   467,   467,  1520,   883,  2295,   678,   876,  1114,
     793,  2748,  2656,  2161,   887,   664,  2563,  1500,  2564,  1935,
    1984,  1983,  1600,   618,   894,  2592,  1984,   625,   895,  1353,
    1354,  1355,  1356,  1357,  1358,  2421,  2422,    97,   598,  1979,
     113,  1134,  2165,  2166,  2023,  2024,  2025,  2026,  2027,  2028,
      45,  1497,   914,  1125,  1907,    47,   678,  1122,  2587,  1598,
    2073,  2476,  2209,  2074,    66,  1501,  1502,   492,  2225,  1181,
     665,   877,  1647,  1881,    43,  2653,  1716,    45,  2697,   493,
    1717,  2654,  1707,  1951,  1182,  1630,  2161,   929,  1908,  2210,
     105,   931,  1503,   494,  1667,    45,  1076,  2211,  1631,  2425,
      47,  2052,   809,  2329,  1563,  1773,   608,  2255,   574,  2258,
    1113,  1799,    97,  1677,  2477,   829,  1677,  1677,  1677,  1677,
    1677,  1677,  1677,  1677,  1677,  1677,  2250,  2413,  1095,  1739,
    1708,  1938,  2254,  2775,   110,  2256,  1707,  1551,   458,   458,
    2015,   646,   116,  1077,  1909,  2749,  1504,  1740,   458,  2075,
    2426,   666,  2281,  2015,  2516,  2126,  2076,  2212,  -680,  2316,
     459,   473,  1617,  2331,  1677,   105,   660,   196,  1816,   458,
     473,  1709,  2398,   654,  1710,  1078,  1505,  2291,  1910,  2151,
    1911,  1296,  1297,   196,  1708,  2405,  1217,  1374,   116,   788,
     792,   607,   104,  1732,  2750,   657,   661,  2427,  2186,   793,
    2361,  1249,  1873,  1874,  2312,  -680,  2015,   569,   921,   110,
    2751,   456,  2478,  1298,   457,  1982,   467,  1279,  2362,   925,
    1741,   570,   562,  2562,  1742,  1709,  2414,  1743,  1710,   932,
    1375,  2185,     9,  2539,  1418,   123,   125,  -680,  2364,  2338,
    1711,  2292,  2439,  1304,  2296,  2297,  2298,  2299,  2300,  2301,
    2302,  2303,  2304,  2305,  1982,    43,  1316,  1059,    45,  2479,
    1991,  1062,  2577,   116,  1912,  1984,   792,   690,  1260,  1368,
     985,  1116,  1070,  1369,  1118,   793,  2017,  1120,  2590,  2714,
    2573,  2574,  2776,  -821,  1109,  2440,  -821,  1296,  1297, -1510,
      45,  2721,  2332,   582,  1711,    47,  1142,  2665,  2415,  2107,
    1143,  2416,  1677,  1144,  1984,  2578,   792,  -821,  1712,   686,
     123,  2138,  2366,  1744,   467,   793,  1171,   514,  2666,  1300,
    1745,  1870,  1871,  2552,  2018,   467,  2394,  2395,  2396,   897,
     897,  2360,  1434,  1437,  1421,  1421,  1893,  1421,  1422,  1424,
     583,  1428,  2017,  -801, -1432,  1858,  -801, -1432, -1432,   901,
     677,  1194, -1432,  2189,   467,   467,   689,   125,   516,    45,
     584,  1468,  1712,   691,    47,   467,  1463,  1275,  1395,  1495,
    1395,  1395,  2317,  2015,   489,  2319,  1982,   585,  2249,  2361,
    2446,  1216,  1982,  1553,   503,  2771,  2649,  1218,  1219,  1554,
    2018,   559,  1555,   805,   518,    71,    71,  2362,   897,  1877,
    1982,  1899,  1250,  1251,  1878,  1952,  1390,   627,  1390,  1390,
    2310,  2311,  2363,   586,   219,  2270,   467,  2364,   467,  -795,
     220,  2376,  -795,  1138,    71,    43,  1984,  2377,    45,   221,
    2438,   222,  1984,    64,    64,  2365,   813,  2019,  1677,  1939,
    2482,   657,   851,  1281,  1940,  2646,   839,  2020,  2021,  2022,
    1984,  2023,  2024,  2025,  2026,  2027,  2028,   841,   520,  1334,
     521,  2689,    64,  1113,  2023,  2024,  2025,  2026,  2027,  2028,
    1610,  -830,  2781,  -798,  -830,    47,  -798,  1337,  1272,  1274,
    1188,  1189,  1338,    45,  2781,  2548,   814,  1195,    47,  1435,
     458,  2366,  -479,  2782,   587,  -830,  1481,  1288,  2122,  2338,
    1490,  1294,   656,  2019,   812,  2782,   588,  2781,   467,  2017,
    1305,   589,   473,  2020,  2021,  2022,   864,  2023,  2024,  2025,
    2026,  2027,  2028,  1667,  1648,   815,  1649,  2722,  2782,   925,
    1650,   849,  1651,   850,   796,   590,  1661,   591,   797,    43,
    1971,   840,    45,  1665,   816,  1972,   592,  1997,  2068,   798,
     799, -1351,  1998,  1687, -1351,  1688,  1667,  2018,   593,   594,
     817, -1350,   456,  1324, -1350,   457,  2521,  2367, -1465,  2360,
     800, -1465, -1465,   801, -1510, -1356, -1465,  1705,  1340,  1706,
    1652,  2043,  1611,  2045,  2046, -1354,   818,    71,  1714,    71,
    1283,  1284,  1475,    71,  1727,  1341,  1342,   548,   467,    71,
     819,  -786,  1737,   873,  -786, -1510,    71,  2121,  1677, -1349,
    1677,    47, -1349,    48,  2003,  2119,    71,  2361,    71,  2004,
    1377,  1653,  2797,   874,  1654,    64,  2615,    64,  1296,  1297,
     898,    64,  1345, -1353,   888,  2362,  -833,    64,  -784,  -833,
      71,  -784,    71,  1346,    64,    43,    45,  2223,    45,   892,
    2363,    47,  2224,  2235,    64,  2364,    64,  1309,  2236, -1081,
    1298,  2647,  2015,   820,  1296,  1297,  2137,  2138,  2136,  1785,
    2019,  1413,  -833,  2365,  2341,  -833,  1423,  1423,    64,  1423,
      64,  2458,  2630,   802,  2023,  2024,  2025,  2026,  2027,  2028,
    1655,   919,  1604,  2636, -1510,   915,  1300,   458,   852,  -787,
     821,   822,  -787,  1935,  1334,  2102,   823,   467,   853,   854,
     855,   897,   856,   857,   858,   859,   860,   861,  2087,  1046,
    2088,  2247,  1337,  1677,  1634,  2232,  2248,  1338,   824,  2366,
    -785,   788,   792,  -785,  2015,  1520,  2625,    45,  2627,   918,
     825,   793,    47,  1783,    48,   826,   458,   458,   920,  2226,
    1387,  2227,  2228,  2229,  2230,    45,  1349,   930,  1656,  1862,
      47,   456,    43,   827,   457,    45,  1296,  1297,   459,   459,
    1353,  1354,  1355,  1356,  1357,  1358,  2138,  2145,  1831,  2267,
    1832,  1698,   467,    45,  2268,  2334,  2409,  2637,    47,  1114,
    2335,  2248,    71,  2078,    71,  2519,  2384,  2154,  2017, -1156,
    2520,  1896,  2282,  2143,  2138,  2367,  2565,  2225,  2566,   219,
    1667,  2248,  2089,  2248,  2090,   220,  1596,  1597,  2386, -1510,
    2629,  2387,    71,  1340,   221,  2520,   222,   922,   467,   223,
      64,   467,    64,   933,  2687,   467,   934,   196,  2628,  2268,
    1341,  1342,  2691,  2696,   925,   935,  2018,  1377,  2224,  2698,
   -1510,  2688,  1781,  2016,  2224,  2404,  2154,  1633,  2713,   936,
      64,  2723,  2725,  1247,    71,   938,  1377,  2224,   925,  1384,
    2017,   464,   478,   481,   481,   657,  1050,  1345,  2742,  1054,
    2798,   478,  2799,  1377,   499,   501,  1051,   467,  1346,  2745,
    1055,   481,  1617,  1985,  1377,   481,  1056,   481,  2773,  1057,
    2756,  2015,    64,  2774,   481,   852,   481,  1063,   624,  2805,
    2431,  2432,   640,   645,  2774,   853,   854,   855,  2018,   856,
     857,   858,   859,   860,   861,  1123,  1067,   657,  -648,  1064,
    1136,  1385,  1489,  1491,  1492,  1065,  1316,   657,  1073, -1510,
    1075,  2392,  2393,  2394,  2395,  2396,  1067,  2538,  1386,  2541,
    1595,  2402,  1886,  1887,  1888,  1068,    71,  1846,  2786,  2019,
     458,  1445,  1069,  1451,  1071,  1853,  1074,  1856,  1096,  2020,
    2021,  2022,  1772,  2023,  2024,  2025,  2026,  2027,  2028,  1100,
    2062,  1098,  1209,  2392,  2393,  2394,  2395,  2396,  1316,  2599,
    2600,  2065,  1106,  2556,    64,  2123,  2124,  1115,  1316,  1372,
    1373,  1349,  1117,  1132,  1137,  1145,  1391,  1147,  1391,  1391,
   -1356,  1737,  2092,  2093, -1354,  1353,  1354,  1355,  1356,  1357,
    1358,  1149, -1353,  1169,   481,  2102,  1113,  2086,  1798,  1190,
    1172,  2019, -1067,  1174,  1196,  1186,  2095,  2017,  1197,  1198,
    2109,  2020,  2021,  2022,  1200,  2023,  2024,  2025,  2026,  2027,
    2028, -1157,  2392,  2393,  2394,  2395,  2396,  1202,   458,  1205,
    2397,   219,  1208,  1437,  1437,  1434,  1206,   220,  1207,   458,
    2130,  1253,  2130,  1256,  1257,  1258,   221,  1259,   222,  1264,
    1320,   223,  -656,   678,  2140,  2018,  2140,  2140,  1267,  1268,
    2685,  1325,  -656,   456,    43,  1269,   457,    45,   458,   458,
    1271,  1273,    47,    71,    48,  1278,   125,  1280,  1395,   458,
    1387,  2202,  1392,  1287,  1392,  1392,  1290,  2203,  1377,  1291,
     473,   473,  2204,  1292,  -656,  1293,    71,    71,    71,    71,
    1295,  1381,  1302,  2700,  2701,  1303,  1367,  1383,  1397,  -656,
    1410,    64,  1114,  2205,  2206,  1411,  1390,    71,  2217,    71,
      71,  1412,    71,    71,  1419,  1076,    71,  1109,  1461,  1453,
     458,  1454,   458,  1464,    64,    64,    64,    64,  1467,  1470,
    1474,  -656, -1303,  1471,  1476,  1914,  1481,  1478,  1479,  -656,
    1928,  1493,   473,   467,   459,    64,  1494,    64,    64,  1507,
      64,    64,  1509,  1512,    64,  2207,   196,  1395,  2019,  1513,
    1514,  1521,  1260,  2195,   467,  1275,  1522,  1524,  2783, -1510,
   -1510,    71,  2023,  2024,  2025,  2026,  2027,  2028,  1281,  1288,
    2783,   512,  1550,   478,  1561,  1395,  1569,  1395,  1395,  1395,
    1395,  1570,  1572,   196,  1573,  1390,  1575,  1580,  2015,  1592,
    1608,  2192,  2208,  2783,  1609,  1612,  2209,  1621,  1613,    64,
    1622,  1247,   458,  2284,  1623,  1638,  1685,  1647,  2464,  2287,
    -656,  1686,  1639,  1390,  1309,  1390,  1390,  1390,  1390,  1334,
    1701,  1702,  1704,  2210,  1381,  2465,  2392,  2393,  2394,  2395,
    2396,  2211,  1718,  1733,  2400,   478,   464,  1754,   481,   464,
    2102,  1774,  1719,  1736,  2102,   657,  1782,  1752,  2102,   664,
    1793,  1805,   481,  1389,  1790,  1806,   478,  1800,   845,  2464,
     464,  2670,  1810,  1067,   866,  1815,    71,   481,  1811,   845,
   -1341,   464,    71,  1822,  1823,  2690,  2465,   481,  1827,  1847,
    2347,  2348,  2349,  2350,   481,  2690,  1829,   478,  1848,  2466,
     464,  2212,   458,  1784,   481,  1845,  1849,  1857,   481,  1859,
     464,  1860,    43,   845,    64,  1863,  2353,  1865,  1879,  1882,
      64,  2467,  2199,  2201,  1659,  1884,  1898,  1903,  1925,  1902,
    1926,  2717,   481,   467,  2017,  1929,   481,  1934,  2091,  1945,
    1937,  1946,  1950,  1947,  1222,  2724,  2468,  1958,  1954,  1227,
    2466,  1961,  1980,  2140,  2609,  1981,  1993,  1994,  1995,  1996,
    1999,  2240,  2000,  2006,  2038,  2001,  2039,   481,  2005,  2056,
    2057,   481,  2467,  2673,  2009,  2030,  2031,  2037,  2040,  2060,
    2041,  2048,  2018,  2054,   852,  2055,  1543,  2058,   481,   640,
      65,   640,    65,  2059,   853,   854,   855,  2468,   856,   857,
     858,   859,   860,   861,  2061,  2066,  2071,  2083,  2108,  2077,
    2111,   458,  2106,  1543,  2112,  1543,    71,  2127,  1543,    71,
    2469,  2132,  2118,  2133,  2141,  2102,  2147,  2152,    71,  2470,
    2102,  2154,    71,  1786,  2102,  2167,  2168,  2155,  2158,   464,
     464,  1105,  2159,  2471,  2174,  2171,  2716,  2173,  2175,  2188,
    2472,  2190,   467,   467,    64,  2218,  2220,    64,   877,   196,
    2231,  2234,  2242,  2233,  2243,  2246,    64,  2259,  2272,  2251,
      64,  2469,  2221,  1617,  2253,  2260,  2261,  2273,  2277,  2280,
    2470,   196,  2015,  2034,  2033,  2313,   458,  2314,  2315,  2473,
    1321,  2327,  2619,  2328,  2471,  2019,    66,  1783,    66,  2359,
    2374,  2472,  2378,  2330,  2337,  2020,  2021,  2022,   459,  2023,
    2024,  2025,  2026,  2027,  2028,  2345, -1081,  2373,  2388,  1667,
     467,  2419,  2379,  1334,  2420,  1861,  2385,  2424,  2203,  2435,
    2443,  2102,   458,  2445,  2448,   458,  2450,  2453,  2459,   458,
    2473,  1337,  2463,  2483,  2504,  2501,  1338,  1543,  2506,  2265,
    2511,   481,  2507,  2509,   481,  2513,  2512,   481,  2514,  2530,
    2532,  1876,  2534,  2535,  2543,  2546,  2549,   583,  2550,  2568,
    2570,  2579,  2426,  2582,  2583,  2584,   481,  2593,  2591,  2595,
     481,  2095,  2597,   481,  2598,  2095,  2603,   584,  2613,  2095,
    2614,   458,  1534,  2616,  2622,  2623,   481,  2640,  2626,  2657,
    2633,  2643,  2417,  2418,   585,  2442,   845,  2140,  2140,  2140,
    2140,  2140,  2650,  1927,  2674,  2662,  2668,  2671,  2672,  1534,
    1179,  1534,  2675,  2679,  1534,  2680,  2681,  2693,   845,   501,
    2571,   481,  2703,  2684,  2704,  2711,  2707,  2719,  1339,  2437,
     586,  2729,  1340,  2720,  2741,  2770,  2732,   845,  2743,  2744,
    2747,  2730,  2757,  2734,  2731,  2763,  2766,   467,  2767,  1341,
    1342,   481,  2351,  2352,  2785,  1114,  2800,   481,   481,  1343,
      71,    71,    71,    71,    71,  2801,  2796,  1245,   659,  1545,
    2787,  2423,   481,   481,  2114,  2219,  1409,  1900,  2452,  2455,
    1850,  2410,  2588,    71,  1486,  2157,  1345,   467,   467,  2746,
    2481,  2765,   467,  2735,  2733,  2753,  1391,  1346,    64,    64,
      64,    64,    64,  2669,  2456,   872,  1883,  2803,  2790,  2580,
    1543,   587,  1543,  2581,  2772,  2179,  1549,    65,  2804,   610,
    1237,    64,  1635,   588,  2274,   937,  2502,   467,   589,  2715,
    2762,  1901,   467,  2252,  1543,  2245,   467,  2620,  1864,  2702,
      65,    65,  2778,  1534,    65,  1255,  2095,  2802,  1348,  1889,
    2434,  2095,   590,  1797,   591,  2095,   563,  2271,  1603,  2120,
    1440,  1438,  1535,   592,  2125,  1334,  1795,  2355,  2695,  2540,
    2451,  2444,  1890,  2320,  1680,   593,   594,  2518,  1765,  2081,
    2447,   478,   478,  1337,  1988,  1391,  2624,  2116,  1338,  1535,
    1543,  1535,  1382,  2503,  1535,  1244,  1192,  1408,  1408,  1469,
    1819,  1825,  1392,  1826,   672,  1108,  1214,  1252,  1417,  1452,
    1349,  1960,  1828,  1391,  1371,  1391,  1391,  1391,  1391,  2500,
    1350,  1351,  1352,    66,  1353,  1354,  1355,  1356,  1357,  1358,
    1957,  2382,  1443,   671,  1953,  2150,  1842,    73,     0,  1607,
       0,     0,     0,   478,     0,   464,    66,    66,     0,     0,
      66,     0,  2095,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   481,     0,     0,     0,     0,   640,     0,     0,
       0,     0,     0,     0,     0,  2170,     0,   458,   640,     0,
       0,     0,     0,     0,  1340,     0,     0,     0,  1334,     0,
       0,  1392,     0,     0,     0,     0,     0,  2193,   458,     0,
    1335,  1341,  1342,     0,     0,     0,  1337,  2526,  2527,  2528,
    2529,  1338,     0,   479,     0,   845,  1534,     0,  1534,     0,
    2197,     0,   479,  1535,     0,     0,     0,  2795,     0,     0,
       0,   534,     0,     0,     0,  1382,     0,     0,  1345,     0,
    1534,  2754,  2755,     0,     0,     0,     0,     0,   467,  1346,
       0,     0,     0,   464,     0,     0,     0,   845,     0,     0,
       0,     0,   481,     0,     0,     0,     0,     0,   467,   467,
       0,     0,     0,     0,     0,   481,   196,   481,   481,     0,
       0,     0,     0,     0,     0,  1626,     0,     0,  2794,     0,
       0,     0,     0,     0,   481,     0,  1534,     0,     0,     0,
       0,     0,     0,  1339,    71,    71,     0,  1340,     0,     0,
       0,   196,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1341,  1342,     0,     0,     0,     0,
       0,     0,     0,     0,  1343,  1668,     0,     0,     0,     0,
       0,     0,    64,    64,     0,     0,     0,    65,     0,     0,
       0,   481,     0,     0,  1543,     0,    65,     0,  1344,     0,
      65,  1345,  1349,     0,     0,     0,     0,   458,     0,     0,
       0,     0,  1346,     0,  1347,     0,  1353,  1354,  1355,  1356,
    1357,  1358,  1543,     0,     0,     0,     0,  1784,     0,  2321,
       0,     0,     0,  2712,     0,     0,  1535,     0,  1535,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1326,  1543,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1535,     0,   481,  1348,     0,  1327,     0,  1543,     0,     0,
       0,     0,  1543,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1543,  2659,     0,     0,   506,  1543,     0,
    1543,     0,     0,    66,     0,     0,   507,     0,   925,  1328,
       0,     0,    66,     0,   481,     0,    66,     0,     0,     0,
       0,     0,     0,  1329,   508,     0,  1535,   509,     0,  1330,
       0,     0,  2686,     0,     0,  1349,   458,   458,     0,     0,
     510,     0,  -371,  1543,     0,  1350,  1351,  1352,     0,  1353,
    1354,  1355,  1356,  1357,  1358,   511,     0,  2049,     0,   464,
    2699,   512,  1105,     0,     0,     0,  2050,     0,  1543,     0,
       0,   513,     0,   464,   464,     0,     0,   481,     0,     0,
       0,     0,     0,  1851,  1852,   481,  1854,   481,     0,   464,
       0,     0,     0,     0,     0,  1543,     0,     0,     0,   514,
       0,   515,     0,     0,   458,     0,   845,   845,     0,     0,
    1534,  -465,     0,     0,   479,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  2461,     0,     0,     0,
    1543,     0,  -308,     0,     0,     0,   852,     0,  1534,     0,
     516,     0,   464,     0,     0,   464,   853,   854,   855,     0,
     856,   857,   858,   859,   860,   861,  1906,     0,     0,     0,
       0,  1924,     0,     0,     0,  1534,     0,     0,  1933,   795,
   -1507,     0,     0,     0,   517,     0,   518,     0,     0,     0,
       0,     0,     0,  1534,     0,     0,     0,  1944,  1534,     0,
       0,     0,   837,     0,     0,  -308,     0,     0,     0,  1534,
       0,     0,     0,   481,  1534, -1507,  1534,     0,     0,     0,
       0,     0,   519,     0,     0, -1507,  1966,  -233,     0,   879,
   -1507,   880,     0,     0,   837, -1507, -1507,     0, -1507,     0,
     837,     0,     0,     0,     0,  1334,     0,     0,     0,  -233,
     520,   458,   521,     0,     0,     0,     0, -1507,     0,  1534,
       0,   522,     0,  1337,     0,   523,   524,     0,  1338,   525,
       0,     0, -1507,  2553,     0,   526,     0,     0,     0,     0,
       0,  1543,     0,     0,  1534,     0,     0,     0,     0,     0,
       0,   458,   458,     0,     0,     0,   458,     0,     0,     0,
    1535,     0,     0,     0, -1507,     0,     0,     0,     0,  2053,
       0,  1534, -1507,     0,     0,  1334,     0,     0,  2576,     0,
       0,     0,     0,     0,  1668,     0,  1033,  1335,  1535,     0,
       0,   458,     0,  1337,  2342,     0,   458,     0,  1338,     0,
     458,     0,     0,     0,   795,     0,  1534,     0,     0,     0,
       0,     0,     0,  2602,     0,  1535,     0,  1668,  2612,     0,
   -1510,     0,  1381,     0,  1340,   795,     0,  1334,     0,     0,
       0,     0,     0,  1535,   608,     0,     0,     0,  1535,  1335,
       0,  1341,  1342,     0,     0,  1337,     0,     0,     0,  1535,
    1338, -1510,     0, -1507,  1535,  1408,  1535,     0,     0,  1417,
    1543,     0,     0,     0,     0,     0,     0,     0,  1537,     0,
       0,   481, -1507,     0,     0,     0,     0,     0,  1345,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1346,
    1339,     0,   478,     0,  1340,  1537,     0,  1537,     0,  1535,
    1537,     0,     0,     0,     0,     0, -1507, -1507, -1507,     0,
       0,  1341,  1342,     0,  1933,     0,     0,     0,     0,     0,
       0,  1343,     0,     0,  1535,   464,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   -1510,     0,  1339,     0,     0,  1344,  1340,  1534,  1345,     0,
       0,  1535,   481,   481,     0,     0,     0,     0,     0,  1346,
       0,  1347,     0,  1341,  1342,     0,     0,     0,     0,     0,
     837,     0,     0,  1343,   837,     0,     0,     0,     0,     0,
     845,     0,     0,     0,     0,     0,  1535,     0,     0,     0,
       0,  2241,     0,     0,     0,  1168,     0,  1344,     0,     0,
    1345,     0,  1349,     0,     0,     0,     0,     0,     0,     0,
    1348,  1346,   458,  1347,     0,     0,  1353,  1354,  1355,  1356,
    1357,  1358,     0,     0,     0,     0,     0,     0,  2263,  1537,
       0,   837,   458,   458,  2718,     0,     0,  1626,     0,     0,
       0,     0,     0,  1543,  1168,     0,  1966,     0,     0,     0,
       0,     0,     0,     0,  2727,  2728,     0,   845,     0,     0,
       0,     0,  1348,     0,     0,     0,  1534,     0,     0,     0,
       0,  1668,  1349,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1350,  1351,  1352,     0,  1353,  1354,  1355,  1356,
    1357,  1358,  1334,     0,     0,     0,     0,     0,  1033,     0,
       0,  2002,     0,     0,  1335,     0,     0,     0,     0,     0,
    1337,     0,     0,     0,     0,  1338,     0,     0,     0,     0,
    1033,     0,     0,     0,  1349,     0,     0,  1535, -1509,     0,
       0,     0,     0,     0,  1350,  1351,  1352,     0,  1353,  1354,
    1355,  1356,  1357,  1358,     0,  1033,  1033,     0,  1033,  1033,
    1033,     0,     0,  -862,     0,     0,     0,     0,  1334,     0,
    2358,     0,     0, -1509,  2358,  2372,  1033,     0,  1543,     0,
       0,     0,     0, -1509,     0,     0,  1337,     0, -1509,     0,
       0,  1338,     0, -1509, -1509,     0, -1509,     0,     0,     0,
       0,     0,   479,   479,     0,     0,     0,     0,     0,     0,
       0,     0,  1537,     0,  1537, -1509,  1396,  1339,     0,     0,
       0,  1340,     0,     0,     0,     0,     0,     0,     0,     0,
   -1509,     0,     0,  1933,  1933,     0,  1537,     0,  1341,  1342,
    1033,     0,   481,   481,     0,     0,     0,   795,  1343,   795,
       0,   795,     0,     0,     0,     0,  1535,  2343,     0,  1534,
       0,     0, -1509,   795,   479,     0,     0,     0,     0,     0,
   -1509,  1543,  1344,     0,     0,  1345,     0,   795,  2436,   481,
       0,   795,     0, -1510,     0,     0,  1346,  1340,  1347,     0,
       0,     0,  1537,     0,     0,     0,     0,     0,     0,  1906,
       0,     0,     0,     0,  1341,  1342,  1851,     0,     0,  1851,
       0,  2457,     0,   795, -1510,     0,     0,     0,  1944,     0,
       0,  1543,     0,  1933,     0,     0,     0,     0,     0,     0,
       0,     0,   608,     0,     0,     0,     0,  1348,   538,  1168,
    1966,  1345,     0,     0,     0,     0,     0,   539,     0,     0,
       0, -1509,  1346,     0,     0,     0,     0,     0,     0,   601,
       0,     0,     0,   601,  1033,   540,  1168,     0,  1168,     0,
   -1509,  1168,     0,     0,     0,   601,   601,     0,     0,     0,
       0,     0,     0,   541,  1534,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   542,   837,     0,  1349,
       0,     0,     0, -1510, -1509, -1509, -1509,     0,     0,  1350,
    1351,  1352,   543,  1353,  1354,  1355,  1356,  1357,  1358,     0,
       0,     0,     0,  2032,     0,  1033,     0,     0,  2358,     0,
    1033,  2531,     0,  1033,  1033,     0,     0,     0,   544,  1535,
       0,     0,   545,     0,     0,  2545,  1660,     0,     0,     0,
       0,     0,   546,  1033,     0,     0,     0,  1033,  1033,  1033,
       0,  1033,     0,  2555,     0,  1349,     0,     0,  1033,  1033,
       0,     0,     0,     0,     0,  1033,     0,  1534,     0,  1353,
    1354,  1355,  1356,  1357,  1358,     0,     0,     0,     0,     0,
       0,  2572,     0,     0,     0,     0,     0,     0,     0,     0,
    1168,  1033,   795,     0,  1033,  1033,     0,     0,     0,  1033,
       0,     0,     0,  1033,     0,   547,  1033,  1033,  1033,  1033,
    1033,  1033,  1033,  1033,  1033,  1033,     0,  1534,     0,  1033,
       0,     0,     0,  1033,     0,     0,   548,     0,  2610,     0,
       0,     0,     0,  1382,     0,     0,     0,     0,     0,     0,
    1538,     0,   549,   550,     0,     0,  1537,     0,   551,     0,
       0,     0,     0,     0,  1535,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1538,   795,  1538,
     552,     0,  1538,     0,  1537,     0,     0,     0,     0,     0,
       0,     0,   553,     0,     0,     0,     0,   554,     0,     0,
       0,     0,     0,     0,     0,  2358,   555,   481,     0,     0,
       0,  1537,     0,     0,     0,   556,     0,     0,   795,  1906,
       0,  2645,  1540,     0,     0,     0,     0,     0,     0,  1537,
       0,     0,     0,     0,  1537,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1537,     0,     0,     0,  1540,
    1537,  1540,  1537,     0,  1540,     0,     0,  1535,     0,     0,
       0,     0,     0,     0,     0,     0,  1944,  1851,     0,     0,
       0,     0,     0,  1168,     0,  1168,     0,     0,   845,     0,
       0,     0,     0,   845,     0,     0,     0,  1033,  1168,     0,
       0,     0,     0,     0,     0,  1537,     0,  1168,     0,     0,
       0,     0,     0,     0,     0,  1541,     0,  1535,   481,     0,
       0,  1538,     0,     0,     0,     0,     0,     0,  2645,     0,
    1537,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1541,     0,  1541,     0,     0,  1541,     0,     0,
       0,  1936,     0,   481,     0,   601,  1168,  1537,     0,     0,
    1245,   845,     0,  1168,   481,     0,     0,     0,   601,   830,
       0,     0,     0,     0,     0,     0,   844,     0,   481,     0,
       0,     0,     0,     0,     0,     0,     0,   844,     0,     0,
       0,     0,  1537,  1540,     0,     0,     0,     0,     0,  1033,
     601,   830,     0,     0,     0,     0,     0,   830,     0,     0,
       0,     0,     0,     0,     0,  1033,     0,     0,     0,     0,
       0,   844,     0,     0,     0,     0,  2761,   845,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1668,     0,     0,     0,     0,     0,     0,  1033,  1033,  1033,
    1033,  1033,     0,     0,     0,     0,     0,     0,     0,  2761,
       0,   601,     0,     0,     0,     0,  1033,  1033,  1033,  1033,
       0,     0,   601,     0,     0,     0,  1541,     0,  1033,     0,
       0,     0,   601,     0,     0,  2761,     0,     0,     0,     0,
       0,     0,     0,     0,  1538,     0,  1538,  1033,     0,     0,
       0,     0,  1033,     0,     0,   602,     0,     0,     0,   602,
     601,     0,  1033,     0,   601,     0,     0,     0,  1538,     0,
       0,   602,   602,  1033,  1033,   601,  1033,     0,     0,     0,
    1033,     0,     0,  1537,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     837,     0,  1033,  1033,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1540,     0,  1540,     0,
       0,     0,     0,     0,  1538,     0,  1033,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1540,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1033,     0,     0,     0,     0,     0,  1168,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1334,     0,
       0,     0,     0,     0,     0,     0,  1540,     0,     0,  1541,
    1335,  1541,  1537,     0,   795,  1168,  1337,     0,     0,     0,
       0,  1338,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1541,     0,     0,   601,   830,     0,     0,
       0,   830,  1168,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1168,     0,     0,     0,     0,  1168,     0,     0,     0,     0,
       0,     0,     0,     0,   844,     0,  1168,     0,     0,     0,
       0,  1168,     0,  1168,     0,     0,     0,     0,     0,  1541,
       0,     0,     0,   601,   601,     0,   844,   837,   830,     0,
     601,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1339,     0,   844,     0,  1340,  1033,     0,
    1033,     0,     0,  1033,     0,     0,  1168,     0,     0,     0,
    1033,     0,     0,     0,  1341,  1342,  1033,     0,  1033,     0,
       0,     0,   601,  1033,  1343,     0,     0,     0,     0,     0,
       0,  1168,  1033,     0,     0,  1033,  1033,  1033,  1033,  1033,
    1033,  1033,  1033,  1033,  1033,     0,     0,     0,  1344,  1033,
    1033,  1345,     0,     0,     0,     0,     0,     0,  1168,  1542,
       0,     0,  1346,  1033,  1347,     0,  1033,     0,     0,     0,
       0,     0,     0,     0,     0,  1537,     0,     0,  1538,     0,
       0,     0,  1033,  1033,  1033,     0,  1542,  1033,  1542,     0,
       0,  1542,     0,  1168,  1033,     0,     0,     0,     0,  1033,
       0,     0,     0,     0,     0,     0,  1538,     0,     0,     0,
     837,     0,     0,  1348,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   602,     0,  1538,  1033,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   602,   831,     0,     0,     0,     0,
    1540,  1538,   831,     0,     0,     0,  1538,     0,     0,     0,
       0,     0,     0,   831,   601,     0,     0,  1538,     0,     0,
       0,     0,  1538,     0,  1538,  1349,   602,   831,  1540,     0,
       0,     0,     0,   831,     0,  1350,  1351,  1352,     0,  1353,
    1354,  1355,  1356,  1357,  1358,     0,     0,   831,     0,  2187,
    1537,     0,     0,     0,     0,  1540,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1538,     0,     0,
    1542,  1033,     0,  1540,     0,     0,     0,     0,  1540,     0,
       0,     0,     0,  1541,  1168,   601,     0,   602,     0,  1540,
       0,     0,  1538,     0,  1540,     0,  1540,     0,   602,     0,
       0,     0,     0,     0,     0,     0,     0,  1011,   602,     0,
       0,  1541,     0,     0,     0,     0,     0,     0,     0,  1538,
       0,     0,     0,   844,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1033,     0,   602,     0,  1541,  1540,
     602,     0,     0,  1537,  1033,     0,     0,     0,     0,     0,
       0,   602,     0,     0,  1538,     0,  1541,     0,     0,   601,
     601,  1541,  1334,     0,  1540,   844,     0,  1544,     0,     0,
       0,     0,  1541,     0,     0,     0,     0,  1541,     0,  1541,
    1337,     0,     0,     0,   830,  1338,     0,   601,     0,     0,
       0,  1540,     0,  1537,  1544,     0,  1590,     0,     0,  1544,
     601,  1033,     0,  1168,     0,     0,     0,  1033,  1033,  1033,
       0,   601,  1033,     0,     0,     0,     0,     0,     0,  1334,
       0,     0,  1541,     0,     0,   837,  1540,     0,     0,   837,
       0,  1335,     0,   837,     0,     0,     0,  1337,     0,  1033,
       0,     0,  1338,  1542,     0,  1542,     0,  1541,     0,     0,
       0,  2346,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1542,     0,     0,
       0,     0,     0,     0,  1541,     0,     0, -1510,     0,  1033,
       0,  1340,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1538,     0,     0,  1341,  1342,
       0,  1033,   602,   831,     0,     0,     0,   831, -1510,  1541,
       0,     0,     0,     0,     0,   601,     0,     0,     0,     0,
       0,     0,     0,  1542,     0,     0,     0,     0,  1544,   837,
       0,     0,     0,     0,  1339,  1345,     0,     0,  1340,     0,
     831,     0,     0,     0,     0,     0,  1346,     0,     0,     0,
       0,     0,     0,     0,     0,  1341,  1342,     0,     0,   602,
     602,     0,   831,     0,   831,  1343,   602,  1540,     0,     0,
    1033,   601,     0,     0,     0,     0,     0,  1033,     0,  1033,
       0,   831,     0,     0,     0,     0,  1168,     0,     0,  1344,
    1033,     0,  1345,     0,     0,     0,     0, -1510,     0,     0,
     837,     0,  1334,  1346,     0,   837,     0,     0,   602,   837,
       0,     0,     0,     0,  1538,  1033,  1033,     0,     0,  1262,
    1337,     0,     0,     0,     0,  1338,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1286,     0,     0,     0,     0,     0,     0,   837,     0,
    1541,     0,     0,     0,  1348,     0,     0,     0,     0,  1349,
       0,     0,     0,     0,   844,   844,  1312,  1313,     0,  1314,
    1315,  1319,     0,  1353,  1354,  1355,  1356,  1357,  1358,     0,
       0,     0,     0,     0,     0,     0,  1540,  1333,     0,     0,
    1334,     0,  1033,     0,     0,     0,     0,     0,     0,     0,
       0,  1544,  1033,  1544,     0,     0,   837,     0,  1337,     0,
       0,  1168,     0,  1338,     0,     0,  1349,  1339,   601,     0,
       0,  1340,     0,   601,     0,  1544,  1350,  1351,  1352,     0,
    1353,  1354,  1355,  1356,  1357,  1358,     0,     0,  1341,  1342,
       0,     0,     0,     0,     0,     0,     0,     0,  1343,     0,
     602,  1427,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1541,
       0,     0,  1344,     0,     0,  1345,     0,     0,     0,  2524,
       0,  1590,     0,     0,     0,     0,  1346,  1542,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1538,     0,     0,
       0,     0,     0,     0,  1168, -1510,     0,     0,     0,  1340,
       0,     0,     0,     0,     0,  1542,     0,     0,     0,     0,
     837,   602,     0,     0,     0,     0,  1341,  1342,     0,     0,
       0,     0,     0,     0,     0,     0, -1510,  1348,     0,     0,
       0,     0,  1542,     0,     0,     0,     0,     0,     0,     0,
       0,   837,     0,     0,  1168,     0,     0,     0,     0,   831,
    1542,     0,     0,  1345,     0,  1542,     0,     0,     0,  1540,
       0,     0,     0,     0,  1346,  1576,  1542,     0,     0,     0,
       0,  1542,     0,  1542,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   602,   602,     0,     0,  1349,
       0,   831,     0,     0,     0,     0,     0,     0,     0,  1350,
    1351,  1352,     0,  1353,  1354,  1355,  1356,  1357,  1358,     0,
     831,   601,  1538,   602,     0, -1510,  1542,  2101,     0,     0,
       0,     0,     0,     0,     0,     0,   602,     0,     0,     0,
       0,  1642,     0,     0,     0,  1644,     0,   602,     0,     0,
       0,  1542,  1541,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1664,     0,     0,     0,  1669,     0,
    1681,     0,  1682,  1334,     0,     0,     0,     0,  1542,  1689,
    1694,     0,     0,     0,     0,  1335,  1699,  1349,     0,     0,
       0,  1337,     0,     0,  1540,     0,  1338,     0,     0,     0,
       0,  1353,  1354,  1355,  1356,  1357,  1358,     0,     0,     0,
       0,     0,  1731,  1542,     0,  1538,  1735,     0,     0,     0,
    1746,     0,     0,     0,  1753,     0,     0,  1755,  1756,  1757,
    1758,  1759,  1760,  1761,  1762,  1763,  1764,     0,     0,     0,
    1011,  1334,     0,     0,     0,  1544,     0,     0,     0,     0,
       0,   602,     0,  1335,     0,   601,     0,     0,     0,  1337,
       0,     0,     0,     0,  1338,  1538,     0,     0,   844,     0,
       0,     0,     0,  1590,     0,     0,     0,  1541,     0,  2239,
       0,     0,     0,     0,     0,     0,     0,  1540,  1339,     0,
       0,     0,  1340,     0,     0,     0,     0,     0,     0,     0,
    1544,     0,     0,     0,     0,     0,     0,   602,     0,  1341,
    1342,     0,     0,     0,   830,     0,     0,     0,  1544,  1343,
       0,     0,     0,  1590,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1590,     0,     0,  1540,     0,  1544,
       0,  1544,     0,  1344,     0,   844,  1345,     0,     0,     0,
       0,     0,     0,     0,  1542,     0,  1339,  1346,     0,  1347,
    1340,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1541,     0,     0,     0,     0,     0,     0,  1341,  1342,     0,
       0,     0,     0,     0,  1544,     0,     0,  1343,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1880,     0,
     831,   831,     0,     0,     0,     0,     0,     0,  1348,  1544,
       0,  1344,     0,     0,  1345,     0,     0,     0,     0,     0,
    1541,     0,     0,     0,     0,  1346,     0,  1347,     0,     0,
       0,     0,     0,     0,     0,     0,  1544,     0,     0,     0,
       0,     0,     0,     0,     0,   601,   601,  2101,     0,     0,
       0,     0,     0,     0,   602,     0,  1334,     0,     0,   602,
       0,     0,     0,     0,     0,     0,     0,     0,  1335,     0,
    1349,  1544,     0,  1542,  1337,     0,  1348,     0,     0,  1338,
    1350,  1351,  1352,     0,  1353,  1354,  1355,  1356,  1357,  1358,
       0,     0,     0,     0,  2406,     0,     0,     0,     0,     0,
    1011,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1334,     0,     0,  1986,     0,     0,     0,
       0,     0,     0,     0,     0,  1335,     0,     0,     0,     0,
       0,  1337,     0,     0,     0,     0,  1338,     0,  1349,     0,
       0,     0,     0,   601,     0,     0,     0,     0,  1350,  1351,
    1352,  2013,  1353,  1354,  1355,  1356,  1357,  1358,     0,     0,
       0,     0,  2508,     0,   601,     0,     0,  1694,     0,  1694,
    1694,  1339,     0,   601,     0,  1340,     0,     0,     0,  2051,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1341,  1342,     0,     0,     0,     0,  2064,     0,
       0,     0,  1343,  2064,     0,     0,     0,     0,     0,     0,
       0,     0,  1544,  2069,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  2080,  1344,  2082,  1339,  1345,
       0,  2085,  1340,     0,     0,     0,     0,     0,     0,     0,
    1346,     0,  1347,     0,     0,     0,     0,     0,     0,  1341,
    1342,     0,     0,     0,     0,     0,  1542,   602,     0,  1343,
       0,     0,     0,   602,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1427,     0,     0,
       0,     0,     0,  1344,     0,     0,  1345,     0,     0,     0,
       0,  1348,     0,     0,     0,     0,     0,  1346,     0,  1347,
     601,   601,   601,   601,     0,     0,     0,     0,     0,     0,
       0,     0,  2101,     0,     0,     0,  2101,     0,   787,     0,
    2101,     0,  2156,     0,     0,     0,     0,     0,     0,     0,
       0,  1544,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   833,     0,     0,     0,     0,     0,     0,  1348,     0,
       0,     0,     0,  1349,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1350,  1351,  1352,     0,  1353,  1354,  1355,
    1356,  1357,  1358,   833,     0,     0,     0,  2661,     0,   833,
       0,  1542,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   602,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   831,     0,     0,     0,     0,     0,
    1349,     0,     0,     0,     0,   831,   830,     0,     0,     0,
    1350,  1351,  1352,     0,  1353,  1354,  1355,  1356,  1357,  1358,
       0,     0,  2515,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     831,     0,     0,     0,     0,     0,     0,     0,     0,  2275,
       0,  2276,     0,     0,  2279,  1027,     0,     0,     0,     0,
       0,  2283,     0,     0,  1542,     0,     0,  2285,     0,  2286,
       0,   831,     0,   787,  2289,     0,     0,  2101,     0,     0,
       0,     0,  2101,     0,  1334,     0,  2101,     0,     0,     0,
       0,     0,     0,     0,   787,     0,  1335,     0,     0,  1801,
    2306,  2307,  1337,     0,  1544,     0,     0,  1338,     0,     0,
       0,     0,     0,     0,  1542,     0,     0,  2318,     0,     0,
       0,     0,     0,     0,     0,   830,     0,   601,     0,     0,
       0,     0,     0,     0,     0,  2333,     0,     0,  2336,     0,
       0,   601,     0,     0,     0,  2340,   844,     0,     0,     0,
    2344,   844,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1802,     0,     0,   601,     0,     0,     0,     0,
       0,   602,   602,   602,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  2101,     0,  2383,     0,     0,     0,     0,
       0,     0,     0,   601,     0,     0,     0,     0,     0,  1339,
       0,     0,     0,  1340,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   844,
    1341,  1342,     0,     0,     0,     0,     0,     0,     0,  1544,
    1343,     0,     0,     0,     0,     0,     0,     0,  1334,   833,
       0,     0,     0,   833,     0,     0,     0,     0,     0,     0,
    1335,     0,     0,     0,  1344,     0,  1337,  1345,     0,     0,
       0,  1338,     0,     0,  1166,     0,     0,     0,  1346,   602,
    1347,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   844,     0,     0,     0,     0,
     602,     0,     0,     0,     0,     0,     0,   830,     0,   602,
     833,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1166,     0,     0,     0,     0,     0,  1348,
       0,     0,  1544,     0,     0,     0,     0,     0,   830,     0,
       0,     0,     0,     0,     0,  1011,     0,     0,  2033,     0,
       0,     0,     0,  2034,     0,  2505,     0,     0,     0,     0,
       0,     0,     0,  1339,     0,     0,     0,  1340,     0,     0,
    1803,     0,     0,     0,     0,     0,     0,  1027,     0,     0,
       0,     0,  1544,     0,  1341,  1342,     0,     0,     0,     0,
       0,  1349,     0,     0,  1343,     0,     0,     0,     0,  1027,
       0,  1350,  1351,  1352,     0,  1353,  1354,  1355,  1356,  1357,
    1358,     0,  2517,     0,     0,     0,     0,     0,  1344,  2522,
    2523,  1345,     0,  2525,  1027,  1027,     0,  1027,  1027,  1027,
       0,     0,  1346,     0,  1347,     0,   602,   602,   602,   602,
       0,     0,     0,     0,     0,  1027,     0,     0,   602,     0,
       0,     0,   602,     0,     0,     0,   602,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1348,     0,     0,     0,     0,     0,     0,
    2569,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1334,     0,  1027,
       0,     0,  2586,     0,     0,     0,   787,     0,   787,  1335,
     787,  1336,     0,     0,     0,  1337,     0,     0,     0,     0,
    1338,     0,   787,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1349,   787,     0,     0,     0,
     787,     0,   831,     0,     0,  1350,  1351,  1352,     0,  1353,
    1354,  1355,  1356,  1357,  1358,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1011,   787,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  2631,     0,     0,     0,     0,     0,     0,  1166,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1339,   602,     0,     0,  1340,  2648,   602,     0,
       0,     0,   602,  1027,     0,  1166,     0,  1166,     0,     0,
    1166,     0,     0,  1341,  1342,     0,     0,     0,     0,     0,
       0,     0,     0,  1343,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   833,     0,     0,     0,
       0,   831,     0,   602,     0,     0,     0,  1344,     0,     0,
    1345,     0,     0,     0,     0,     0,     0,   602,     0,     0,
       0,  1346,   831,  1347,  1027,     0,     0,   831,     0,  1027,
       0,     0,  1027,  1027,     0,     0,     0,     0,     0,     0,
       0,   602,     0,  2692,     0,     0,     0,     0,     0,     0,
       0,     0,  1027,     0,     0,     0,  1027,  1027,  1027,   602,
    1027,     0,     0,     0,     0,     0,     0,  1027,  1027,   602,
       0,     0,  1348,     0,  1027,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   831,     0,     0,     0,  1166,
    1027,   787,     0,  1027,  1027,     0,     0,     0,  1027,     0,
       0,     0,  1027,     0,     0,  1027,  1027,  1027,  1027,  1027,
    1027,  1027,  1027,  1027,  1027,     0,     0,     0,  1027,     0,
       0,     0,  1027,     0,  1349,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1350,  1351,  1352,     0,  1353,  1354,
    1355,  1356,  1357,  1358,     0,     0,     0,     0,     0,     0,
       0,   831,  1334,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   831,  1335,     0,  1990,   787,     0,     0,
    1337,     0,     0,     0,     0,  1338,     4,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       5,     0,     0,     0,   831,     0,     0,     0,     0,     6,
       0,     0,     0,     0,     0,     0,     0,   787,     0,     0,
       7,     8,     0,     0,     0,     0,     9,     0,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,     0,    34,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1166,     0,  1166,     0,     0,  1339,     0,     0,
       0,  1340,     0,     0,     0,     0,  1027,  1166,     0,     0,
       0,     0,     0,    35,     0,     0,  1166,     0,  1341,  1342,
       0,     0,     0,     0,     0,     0,     0,     0,  1343,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1344,     0,     0,  1345,     0,     0,     0,    36,
       0,     0,     0,     0,     0,  1166,  1346,     0,  1347,     0,
       0,     0,  1166,     0,     0,     0,    37,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    38,  1027,     0,
       0,     0,     0,     0,     0,     0,     0,  1348,     0,     0,
       0,     0,     0,     0,  1027,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    39,     0,     0,     0,     0,  1027,  1027,  1027,  1027,
    1027,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1027,  1027,  1027,  1027,  1349,
       0,     0,     0,     0,     0,     0,     0,  1027,     0,  1350,
    1351,  1352,     0,  1353,  1354,  1355,  1356,  1357,  1358,     0,
       0,     0,     0,     0,     0,     0,  1027,     0,     0,     0,
       0,  1027,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1027,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1027,  1027,     0,  1027,     0,     0,     0,  1027,
       0,     0,     0,     0,     0,    40,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    41,     0,   833,
       0,  1027,  1027,     0,   791,     0,     0,     0,     0,     0,
       0,     0,    42,    43,     0,    44,    45,    46,     0,     0,
       0,    47,     0,    48,    49,  1027,     0,   835,    50,    51,
      52,    53,    54,     0,     0,    55,    56,    57,    58,     0,
      59,    60,    61,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   835,
       0,     0,     0,     0,     0,   835,     0,     0,     0,     0,
    1027,     0,     0,     0,     0,     0,  1166,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   787,  1166,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1166,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1334,     0,     0,     0,     0,     0,     0,     0,  1166,
       0,  1030,     0,  1335,  1166,     0,     0,     0,     0,  1337,
       0,     0,     0,     0,  1338,  1166,     0,     0,     0,   791,
    1166,     0,  1166,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   833,     0,     0,     0,
     791,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1027,     0,  1027,
       0,  1334,  1027,     0,     0,  1166,     0,     0,     0,  1027,
       0,     0,     0,  1335,     0,  1027,     0,  1027,     0,  1337,
       0,     0,  1027,     0,  1338,     0,     0,     0,     0,     0,
    1166,  1027,     0,  1334,  1027,  1027,  1027,  1027,  1027,  1027,
    1027,  1027,  1027,  1027,     0,  1335,  1339,  2042,  1027,  1027,
    1340,  1337,     0,     0,     0,     0,  1338,  1166,     0,     0,
       0,     0,  1027,     0,     0,  1027,     0,  1341,  1342,     0,
       0,     0,     0,     0,     0,     0,     0,  1343,     0,     0,
       0,  1027,  1027,  1027,     0,     0,  1027,     0,     0,     0,
       0,     0,  1166,  1027,     0,     0,     0,     0,  1027,     0,
       0,  1344,     0,     0,  1345,     0,     0,     0,     0,   833,
       0,     0,     0,     0,     0,  1346,  1339,  1347,     0,     0,
    1340,     0,     0,     0,     0,   835,     0,     0,     0,   835,
       0,     0,     0,  1027,     0,     0,     0,  1341,  1342,     0,
       0,     0,     0,     0,     0,     0,     0,  1343,  1339,     0,
    1167,     0,  1340,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1348,     0,     0,  1341,
    1342,  1344,     0,     0,  1345,     0,     0,     0,     0,  1343,
       0,     0,     0,     0,     0,  1346,   835,  1347,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  2007,  1167,
       0,     0,     0,  1344,     0,     0,  1345,  1992,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1346,     0,  1347,
    1027,     0,     0,     0,     0,     0,     0,     0,  1349,     0,
       0,     0,     0,  1166,     0,     0,  1348,     0,  1350,  1351,
    1352,     0,  1353,  1354,  1355,  1356,  1357,  1358,     0,     0,
       0,     0,     0,  1030,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1348,     0,
       0,     0,     0,     0,     0,  1030,     0,     0,     0,     0,
       0,     0,     0,  1027,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1027,     0,     0,     0,     0,  1349,     0,
    1030,  1030,     0,  1030,  1030,  1030,     0,     0,  1350,  1351,
    1352,     0,  1353,  1354,  1355,  1356,  1357,  1358,     0,     0,
    1334,  1030,     0,     0,     0,     0,     0,     0,     0,     0,
    1349,     0,  1335,     0,     0,     0,     0,     0,  1337,     0,
    1350,  1351,  1352,  1338,  1353,  1354,  1355,  1356,  1357,  1358,
    1027,     0,  1166,     0,     0,     0,  1027,  1027,  1027,     0,
       0,  1027,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   833,     0,     0,     0,   833,     0,
       0,     0,   833,     0,     0,  1030,     0,     0,  1027,     0,
       0,     0,   791,     0,   791,     0,   791,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   791,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   791,     0,     0,  2047,   791,     0,  1027,     0,
       0,     0,     0,     0,     0,  1339,     0,     0,     0,  1340,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1027,     0,     0,     0,     0,     0,  1341,  1342,   791,     0,
       0,     0,     0,     0,     0,     0,  1343,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1359,   833,     0,
       0,     0,     0,     0,  1167,     0,     0,     0,     0,     0,
    1344,     0,     0,  1345,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1346,     0,  1347,     0,     0,  1030,
       0,  1167,     0,  1167,     0,     0,  1167,     0,     0,  1027,
       0,     0,     0,     0,     0,     0,  1027,     0,  1027,     0,
       0,     0,     0,     0,     0,  1166,     0,     0,     0,  1027,
       0,     0,   835,     0,     0,     0,     0,     0,     0,   833,
       0,     0,     0,     0,   833,  1348,     0,     0,   833,     0,
       0,     0,     0,     0,  1027,  1027,     0,     0,     0,     0,
    1030,     0,     0,     0,     0,  1030,     0,     0,  1030,  1030,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   833,  1030,     0,
       0,     0,  1030,  1030,  1030,     0,  1030,     0,     0,     0,
       0,     0,     0,  1030,  1030,     0,     0,  1349,     0,     0,
    1030,     0,     0,     0,     0,     0,     0,  1350,  1351,  1352,
       0,  1353,  1354,  1355,  1356,  1357,  1358,     0,     0,     0,
       0,  1027,     0,     0,     0,  1167,  1030,   791,     0,  1030,
    1030,  1027,     0,     0,  1030,   833,     0,     0,  1030,     0,
    1166,  1030,  1030,  1030,  1030,  1030,  1030,  1030,  1030,  1030,
    1030,     0,     0,     0,  1030,     0,     0,     0,  1030,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   791,     0,     0,     0,     0,  1359,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1359,  1166,     0,     0,     0,     0,  1675,     0,
       0,     0,     0,   791,     0,     0,     0,     0,     0,   833,
       0,     0,     0,     0,     0,     0,     0,     0,  1359,  1359,
    1359,  1359,     0,     0,     0,  1359,     0,     0,     0,     0,
      75,     0,     0,     0,     0,     0,    76,     0,     0,  1359,
     833,     0,     0,  1166,  1675,    77,     0,     0,    78,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1167,    79,
    1167,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1030,  1167,     0,     0,     0,    80,    81,     0,
       0,     0,  1167,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,    83,    84,    85,     0,     0,     0,     0,     0,
       0,    86,     0,     0,     0,    87,     0,     0,     0,     0,
       0,  1167,     0,  1359,     0,     0,     0,     0,  1167,     0,
       0,     0,     0,    88,     0,    89,    90,     0,     0,     0,
      91,    92,     0,     0,     0,     0,     0,    93,    94,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,  1030,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,     0,
    1030,    99,     0,     0,     0,   100,     0,   101,     0,     0,
       0,   102,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   103,     0,   104,     0,     0,     0,     0,     0,     0,
       0,     0,  1030,  1030,  1030,  1030,  1030,     0,     0,     0,
       0,   105,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1030,  1030,  1030,  1030,     0,     0,     0,     0,     0,
       0,   106,   107,  1030,     0,   108,     0,     0,     0,     0,
       0,     0,   109,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1030,     0,     0,   110,     0,  1030,     0,     0,
       0,     0,  1359,     0,     0,     0,     0,  1030,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1030,  1030,
       0,  1030,     0,   111,     0,  1030,     0,     0,     0,     0,
       0,     0,     0,   112,     0,     0,     0,   113,     0,     0,
     114,     0,   115,     0,     0,   835,     0,  1030,  1030,   116,
       0,     0,     0,     0,   117,     0,     0,   118,     0,     0,
       0,     0,  1359,   119,     0,     0,     0,     0,  1359,     0,
    1359,  1030,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   120,     0,   121,
    1359,     0,     0,     0,   122,  1359,   123,  1675,  1675,  1675,
     124,  1675,  2029,     0,     0,     0,     0,  1359,  1359,     0,
       0,     0,     0,     0,     0,  1359,  1030,     0,     0,     0,
    1359,     0,  1167,     0,     0,  1359,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   125,     0,     0,     0,     0,     0,   791,
    1167,     0,     0,     0,     0,     0,     0,  1359,     0,     0,
    2029,  1359,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1359,  1675,     0,     0,     0,  1167,     0,  1359,
       0,  1359,  1359,  1359,  1359,  1359,  1359,  1359,  1359,  1359,
    1359,     0,     0,     0,     0,  1167,     0,     0,     0,     0,
    1167,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1167,     0,     0,     0,     0,  1167,     0,  1167,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   835,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1030,     0,  1030,     0,     0,  1030,     0,
       0,  1167,     0,     0,     0,  1030,     0,     0,     0,     0,
       0,  1030,     0,  1030,     0,     0,     0,     0,  1030,     0,
       0,     0,     0,     0,     0,     0,  1167,  1030,     0,  1334,
    1030,  1030,  1030,  1030,  1030,  1030,  1030,  1030,  1030,  1030,
       0,  1335,     0,     0,  1030,  1030,  1359,  1337,     0,     0,
       0,     0,  1338,  1167,     0,     0,     0,     0,  1030,     0,
       0,  1030,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1030,  1030,  1030,
       0,     0,  1030,     0,     0,     0,  1334,     0,  1167,  1030,
       0,     0,     0,     0,  1030,     0,     0,     0,  1335,     0,
       0,     0,     0,     0,  1337,   835,     0,     0,     0,  1338,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1030,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1339,     0,     0,     0,  1340,     0,
       0,     0,  1359,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1341,  1342,     0,     0,     0,
       0,     0,     0,     0,     0,  1343,  2029,  2029,  2029,  1359,
    2029,     0,  1675,     0,     0,  1675,  1675,  1675,  1675,  1675,
    1675,  1675,  1675,  1675,  1675,     0,     0,     0,     0,  1344,
       0,  1339,  1345,     0,     0,  1340,     0,     0,     0,     0,
       0,     0,     0,  1346,     0,  1347,  1030,  1359,     0,     0,
       0,     0,  1341,  1342,     0,     0,     0,     0,     0,  1167,
    1359,     0,  1343,  1675,     0,  1359,     0,     0,     0,     0,
       0,     0,     0,     4,     0,  2029,  1359,     0,  1359,     0,
       0,  1359,     0,     0,     0,     0,  1344,     5,     0,  1345,
       0,     0,     0,     0,  1348,     0,     6,     0,     0,     0,
    1346,     0,  1347,     0,     0,     0,     0,     7,     8,  1030,
       0,     0,  2278,     0,     0,    10,    11,     0,     0,  1030,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,     0,    34,     0,     0,     0,     0,     0,     0,
       0,  1348,  1359,     0,     0,     0,  1349,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1350,  1351,  1352,     0,
    1353,  1354,  1355,  1356,  1357,  1358,  1030,     0,  1167,     0,
      35,     0,  1030,  1030,  1030,     0,     0,  1030,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     835,  1675,     0,     0,   835,     0,     0,     0,   835,     0,
       0,     0,     0,  1349,  1030,     0,     0,     0,     0,     0,
       0,     0,     0,  1350,  1351,  1352,    36,  1353,  1354,  1355,
    1356,  1357,  1358,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    37,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1030,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1359,  1359,     0,    38,  1359,  1030,     0,     0,  1359,
       0,  1359,  1359,     0,     0,  1359,     0,     0,  2029,     0,
       0,     0,  2029,  2029,  2029,  2029,  2029,  2029,  2029,  2029,
    2029,  2029,  1359,  1359,   835,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1359,     0,     0,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  2029,  1359,
       0,     0,  1359,     0,     0,     0,  1359,  1675,     0,     0,
    1359,     0,     0,     0,     0,  1030,     0,     0,     0,     0,
       0,     0,  1030,     0,  1030,     0,     0,     0,     0,     0,
       0,  1167,     0,     0,     0,  1030,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   835,     0,     0,     0,  1359,
     835,     0,     0,     0,   835,     0,     0,     0,     0,     0,
    1030,  1030,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   835,    41,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  2029,     0,     0,     0,     0,    42,
      43,     0,    44,    45,     0,     0,     0,     0,    47,     0,
      48,    49,     0,     0,     0,    50,    51,    52,    53,    54,
       0,     0,    55,    56,  1834,     0,     0,  1030,  1835,     0,
       0,     0,     0,     0,     0,     0,    75,  1030,     0,     0,
       0,   835,    76,     0,     0,     0,  1167,     0,     0,     0,
       0,    77,     0,     0,    78,     0,     0,     0,     0,     0,
       0,  1359,     0,     0,     0,    79,     0,  1675,     0,  1675,
       0,     0,     0,  1359,     0,     0,     0,  2029,  1359,  1359,
       0,  1359,     0,    80,    81,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,     0,     0,     0,     0,  1359,     0,     0,     0,     0,
       0,   658,     0,     0,     0,     0,     0,     0,     0,  1167,
       0,     0,  1359,     0,     0,     0,     0,     0,     0,    88,
       0,    89,    90,     0,     0,   835,    91,    92,     0,     0,
       0,     0,     0,    93,    94,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    95,     0,     0,    96,
       0,  2029,  1675,  2029,    97,     0,   835,  1359,     0,  1167,
       0,     0,    98,     0,     0,     0,     0,    99,     0,     0,
       0,   100,     0,   101,  1359,     0,     0,   102,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   103,     0,   104,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   105,     0,     0,
       0,     0,     0,     0,  2029,     0,     0,     0,  1359,     0,
       0,     0,     0,     0,     0,     0,     0,   106,   107,     0,
       0,   108,     0,     0,     0,     0,     0,     0,   109,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   110,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   111,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   112,
       0,     0,     0,   113,     0,     0,   114,     0,   115,     0,
       0,     0,     0,     0,     0,   116,     0,     0,     0,     0,
     117,     0,     0,   118,     0,     0,     0,     0,     0,   119,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   120,     0,   121,     0,     0,     0,     0,
     122,     0,   123,     0,     0,     0,   124,     0,     0,   693,
     230,   231,   232,   233,     0,   234,   235,     0,   236,   237,
     238,     0,   239,     0,   240,     0,   241,   242,   243,   244,
     245,   246,   694,   247,   248,   249,   250,   251,     0,   252,
     253,   254,   255,   695,   696,     0,   256,   257,   258,   125,
     259,   697,   698,     0,   260,   261,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   262,   263,   264,   265,   266,   267,   268,  1776,   269,
       0,     0,     0,     0,     0,     0,   270,   271,   272,   706,
     273,   274,   275,   707,   939,   708,   940,     0,   941,   278,
     279,     0,   280,   942,   943,   281,   944,   945,   284,     0,
     285,   286,   287,   288,   946,     0,     0,   290,   291,   292,
       0,   293,   294,   947,   295,     0,   296,   297,   718,   948,
     949,   950,   951,   298,   299,   300,   301,   302,   952,   953,
     305,     0,     0,   306,     0,   307,   308,   309,     0,  1777,
       0,   310,   954,   312,   313,     0,   314,   315,     0,   316,
       0,   317,   318,   955,   320,     0,   956,   957,   322,   958,
       0,   324,     0,   325,   734,     0,   736,   326,     0,   327,
       0,     0,   328,     0,   329,   740,   330,   331,   332,   741,
     333,   334,   335,     0,   743,   336,     0,   337,   338,   339,
     959,   960,     0,   961,     0,     0,   747,   748,   342,   749,
     343,   344,   345,     0,   751,   346,   752,     0,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   962,   754,   963,   599,   362,   363,   364,   365,   366,
     964,   367,   368,   757,   965,   966,   967,   371,     0,     0,
     372,   600,     0,     0,   968,   374,     0,     0,   375,   766,
     767,   969,   376,   377,   378,   379,   380,     0,   970,   382,
     383,     0,   384,   385,   386,   387,   388,   971,   390,     0,
     391,   392,   393,   394,   395,   396,   397,   398,   770,   399,
     972,   401,   402,   403,   404,     0,     0,   405,   406,   407,
     973,   408,   409,   410,   411,   773,   412,   974,     0,   414,
     415,   416,   417,   418,   419,   420,     0,   975,   422,     0,
     423,   424,   425,     0,   976,   977,     0,   429,     0,   430,
     978,   432,   979,   980,   434,   435,   436,   437,     0,   981,
     438,   439,   440,   441,   442,   982,     0,   443,   444,   445,
     983,   447,   784,   448,   449,     0,     0,     0,   450,   451,
     452,   453,   454,   455,     0,   456,    43,   984,   457,   985,
       0,     0,   986,   987,    47,   988,    48,     0,     0,     0,
       0,   989,   990,  1778,     0,   992,   993,     0,     0,     0,
     994,  1779,   693,   230,   231,   232,   233,     0,   234,   235,
       0,   236,   237,   238,     0,   239,     0,   240,     0,   241,
     242,   243,   244,   245,   246,   694,   247,   248,   249,   250,
     251,     0,   252,   253,   254,   255,   695,   696,     0,   256,
     257,   258,     0,   259,   697,   698,     0,   260,   261,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   262,   263,   264,   265,   266,   267,
     268,     0,   269,     0,     0,     0,     0,     0,     0,   270,
     271,   272,   706,   273,   274,   275,   707,   939,   708,   940,
       0,   941,   278,   279,     0,   280,   942,   943,   281,   944,
     945,   284,     0,   285,   286,   287,   288,   946,     0,     0,
     290,   291,   292,     0,   293,   294,   947,   295,     0,   296,
     297,   718,   948,   949,   950,   951,   298,   299,   300,   301,
     302,   952,   953,   305,     0,     0,   306,     0,   307,   308,
     309,     0,     0,     0,   310,   954,   312,   313,     0,   314,
     315,     0,   316,     0,   317,   318,   955,   320,     0,   956,
     957,   322,   958,     0,   324,     0,   325,   734,     0,   736,
     326,     0,   327,     0,     0,   328,     0,   329,   740,   330,
     331,   332,   741,   333,   334,   335,     0,   743,   336,     0,
     337,   338,   339,   959,   960,     0,   961,     0,     0,   747,
     748,   342,   749,   343,   344,   345,     0,   751,   346,   752,
       0,   347,   348,   349,   350,   351,   352,   353,   354,   355,
     356,   357,   358,   359,   962,   754,   963,   599,   362,   363,
     364,   365,   366,   964,   367,   368,   757,   965,   966,   967,
     371,     0,     0,   372,   600,     0,     0,   968,   374,     0,
       0,   375,   766,   767,   969,   376,   377,   378,   379,   380,
       0,   970,   382,   383,     0,   384,   385,   386,   387,   388,
     971,   390,     0,   391,   392,   393,   394,   395,   396,   397,
     398,   770,   399,   972,   401,   402,   403,   404,     0,     0,
     405,   406,   407,   973,   408,   409,   410,   411,   773,   412,
     974,     0,   414,   415,   416,   417,   418,   419,   420,     0,
     975,   422,     0,   423,   424,   425,     0,   976,   977,     0,
     429,     0,   430,   978,   432,   979,   980,   434,   435,   436,
     437,     0,   981,   438,   439,   440,   441,   442,   982,     0,
     443,   444,   445,   983,   447,   784,   448,   449,     0,     0,
       0,   450,   451,   452,   453,   454,   455,     0,   456,    43,
     984,   457,   985,     0,     0,   986,   987,    47,   988,    48,
       0,     0,     0,     0,   989,   990,     0,     0,   992,   993,
       0,     0,     0,   994,  1679,   693,   230,   231,   232,   233,
       0,   234,   235,     0,   236,   237,   238,     0,   239,     0,
     240,     0,   241,   242,   243,   244,   245,   246,   694,   247,
     248,   249,   250,   251,     0,   252,   253,   254,   255,   695,
     696,     0,   256,   257,   258,     0,   259,   697,   698,     0,
     260,   261,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   262,   263,   264,
     265,   266,   267,   268,     0,   269,     0,     0,     0,     0,
       0,     0,   270,   271,   272,   706,   273,   274,   275,   707,
     939,   708,   940,     0,   941,   278,   279,     0,   280,   942,
     943,   281,   944,   945,   284,     0,   285,   286,   287,   288,
     946,     0,     0,   290,   291,   292,     0,   293,   294,   947,
     295,     0,   296,   297,   718,   948,   949,   950,   951,   298,
     299,   300,   301,   302,   952,   953,   305,     0,     0,   306,
       0,   307,   308,   309,     0,     0,     0,   310,   954,   312,
     313,     0,   314,   315,     0,   316,     0,   317,   318,   955,
     320,     0,   956,   957,   322,   958,     0,   324,     0,   325,
     734,     0,   736,   326,     0,   327,     0,     0,   328,     0,
     329,   740,   330,   331,   332,   741,   333,   334,   335,     0,
     743,   336,     0,   337,   338,   339,   959,   960,     0,   961,
       0,     0,   747,   748,   342,   749,   343,   344,   345,     0,
     751,   346,   752,     0,   347,   348,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,   962,   754,   963,
     599,   362,   363,   364,   365,   366,   964,   367,   368,   757,
     965,   966,   967,   371,     0,     0,   372,   600,     0,     0,
     968,   374,     0,     0,   375,   766,   767,   969,   376,   377,
     378,   379,   380,     0,   970,   382,   383,     0,   384,   385,
     386,   387,   388,   971,   390,     0,   391,   392,   393,   394,
     395,   396,   397,   398,   770,   399,   972,   401,   402,   403,
     404,     0,     0,   405,   406,   407,   973,   408,   409,   410,
     411,   773,   412,   974,     0,   414,   415,   416,   417,   418,
     419,   420,     0,   975,   422,     0,   423,   424,   425,     0,
     976,   977,     0,   429,     0,   430,   978,   432,   979,   980,
     434,   435,   436,   437,     0,   981,   438,   439,   440,   441,
     442,   982,     0,   443,   444,   445,   983,   447,   784,   448,
     449,     0,     0,     0,   450,   451,   452,   453,   454,   455,
       0,   456,    43,   984,   457,   985,     0,     0,   986,   987,
      47,   988,    48,     0,     0,     0,     0,   989,   990,     0,
       0,   992,   993,     0,     0,     0,   994,  2547,   693,   230,
     231,   232,   233,     0,   234,   235,     0,   236,   237,   238,
       0,   239,     0,   240,     0,   241,   242,   243,   244,   245,
     246,   694,   247,   248,   249,   250,   251,     0,   252,   253,
     254,   255,   695,   696,     0,   256,   257,   258,     0,   259,
     697,   698,     0,   260,   261,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     262,   263,   264,   265,   266,   267,   268,     0,   269,     0,
       0,     0,     0,     0,     0,   270,   271,   272,     0,   273,
     274,   275,     0,  1150,     0,  1151,     0,   941,   278,   279,
       0,   280,     0,     0,   281,  1152,  1153,   284,     0,   285,
     286,   287,   288,     0,     0,     0,   290,   291,   292,     0,
     293,   294,     0,   295,     0,   296,   297,     0,     0,     0,
       0,     0,   298,   299,   300,   301,   302,  1154,  1155,   305,
       0,     0,   306,     0,   307,   308,   309,     0,     0,     0,
     310,   954,   312,   313,     0,   314,   315,     0,   316,     0,
     317,   318,     0,   320,     0,     0,     0,   322,  1156,     0,
     324,     0,   325,     0,     0,     0,   326,     0,   327,     0,
       0,   328,     0,   329,     0,   330,   331,   332,  1581,   333,
     334,   335,     0,     0,  1582,     0,   337,   338,   339,   959,
    1157,     0,  1385,     0,     0,     0,     0,   342,     0,   343,
     344,   345,     0,     0,   346,     0,     0,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     962,     0,  1158,     0,   362,   363,   364,   365,     0,     0,
     367,   368,     0,     0,     0,  1159,   371,     0,     0,   372,
       0,     0,     0,   373,   374,     0,     0,  1583,     0,     0,
       0,   376,   377,   378,   379,   380,     0,     0,   382,   383,
       0,   384,   385,   386,   387,   388,  1160,   390,     0,   391,
     392,   393,   394,   395,   396,   397,   398,     0,   399,     0,
     401,   402,   403,   404,     0,     0,   405,   406,   407,     0,
     408,  1531,   410,   411,     0,   412,  1161,     0,   414,   415,
     416,   417,   418,   419,   420,     0,     0,   422,     0,   423,
     424,   425,     0,  1162,  1163,     0,   429,     0,   430,     0,
     432,     0,     0,   434,   435,   436,   437,     0,     0,   438,
     439,   440,   441,   442,     0,     0,   443,   444,   445,  1164,
     447,     4,   448,   449,     0,     0,     0,   450,   451,   452,
     453,   454,   455,     0,   456,     5,     0,   457,     0,     0,
       0,     0,     0,     0,     6,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     0,     0,     0,
    1584,     0,     0,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
       4,    34,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     5,     0,     0,     0,     0,     0,
       0,     0,     0,     6,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     7,     8,     0,     0,    35,     0,
       0,     0,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,     0,
      34,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    36,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    37,     0,     0,     0,     0,     0,    35,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    38,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    36,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      37,     0,     0,     0,     0,     0,    39,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    38,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    39,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    41,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    42,    43,     0,
      44,    45,     0,     0,     0,     0,    47,     0,    48,    49,
       0,     0,     0,    50,    51,    52,    53,    54,     0,     0,
      55,    56,  1834,  2149,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    41,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    42,    43,     0,    44,
      45,     0,     0,     0,     0,    47,     0,    48,    49,     0,
       0,     0,    50,    51,    52,    53,    54,     0,     0,    55,
      56,  1834,  2403,   693,   230,   231,   232,   233,     0,   234,
     235,     0,   236,   237,   238,     0,   239,     0,   240,     0,
     241,   242,   243,   244,   245,   246,   694,   247,   248,   249,
     250,   251,     0,   252,   253,   254,   255,   695,   696,     0,
     256,   257,   258,     0,   259,   697,   698,     0,   260,   261,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   262,   263,   264,   265,   266,
     267,   268,     0,   269,     0,     0,     0,     0,     0,     0,
     270,   271,   272,   706,   273,   274,   275,   707,   939,   708,
     940,  1690,   941,   278,   279,     0,   280,   942,   943,   281,
     944,   945,   284,     0,   285,   286,   287,   288,   946,     0,
       0,   290,   291,   292,     0,   293,   294,   947,   295,     0,
     296,   297,   718,   948,   949,   950,   951,   298,   299,   300,
     301,   302,   952,   953,   305,     0,     0,   306,     0,   307,
     308,   309,     0,     0,     0,   310,   954,   312,   313,     0,
     314,   315,     0,   316,     0,   317,   318,   955,   320,     0,
     956,   957,   322,   958,     0,   324,     0,   325,   734,  1691,
     736,   326,     0,   327,     0,     0,   328,     0,   329,   740,
     330,   331,   332,   741,   333,   334,   335,     0,   743,   336,
       0,   337,   338,   339,   959,   960,     0,   961,     0,     0,
     747,   748,   342,   749,   343,   344,   345,  1692,   751,   346,
     752,     0,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   962,   754,   963,   599,   362,
     363,   364,   365,   366,   964,   367,   368,   757,   965,   966,
     967,   371,     0,     0,   372,   600,     0,     0,   968,   374,
       0,     0,   375,   766,   767,   969,   376,   377,   378,   379,
     380,     0,   970,   382,   383,     0,   384,   385,   386,   387,
     388,   971,   390,     0,   391,   392,   393,   394,   395,   396,
     397,   398,   770,   399,   972,   401,   402,   403,   404,     0,
       0,   405,   406,   407,   973,   408,   409,   410,   411,   773,
     412,   974,     0,   414,   415,   416,   417,   418,   419,   420,
       0,   975,   422,     0,   423,   424,   425,     0,   976,   977,
       0,   429,  1693,   430,   978,   432,   979,   980,   434,   435,
     436,   437,     0,   981,   438,   439,   440,   441,   442,   982,
       0,   443,   444,   445,   983,   447,   784,   448,   449,     0,
       0,     0,   450,   451,   452,   453,   454,   455,     0,   456,
      43,   984,   457,   985,     0,     0,   986,   987,    47,   988,
      48,     0,     0,     0,     0,   989,   990,     0,     0,   992,
     993,     0,     0,     0,   994,   693,   230,   231,   232,   233,
       0,   234,   235,     0,   236,   237,   238,     0,   239,     0,
     240,     0,   241,   242,   243,   244,   245,   246,   694,   247,
     248,   249,   250,   251,     0,   252,   253,   254,   255,   695,
     696,     0,   256,   257,   258,     0,   259,   697,   698,     0,
     260,   261,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   262,   263,   264,
     265,   266,   267,   268,     0,   269,     0,     0,     0,     0,
       0,     0,   270,   271,   272,   706,   273,   274,   275,   707,
     939,   708,   940,     0,   941,   278,   279,     0,   280,   942,
     943,   281,   944,   945,   284,     0,   285,   286,   287,   288,
     946,     0,     0,   290,   291,   292,     0,   293,   294,   947,
     295,     0,   296,   297,   718,   948,   949,   950,   951,   298,
     299,   300,   301,   302,   952,   953,   305,  1967,     0,   306,
       0,   307,   308,   309,     0,     0,     0,   310,   954,   312,
     313,     0,   314,   315,     0,   316,     0,   317,   318,   955,
     320,     0,   956,   957,   322,   958,     0,   324,     0,   325,
     734,     0,   736,   326,     0,   327,     0,     0,   328,     0,
     329,   740,   330,   331,   332,   741,   333,   334,   335,     0,
     743,   336,     0,   337,   338,   339,   959,   960,     0,   961,
       0,     0,   747,   748,   342,   749,   343,   344,   345,     0,
     751,   346,   752,     0,   347,   348,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,   962,   754,   963,
     599,   362,   363,   364,   365,   366,   964,   367,   368,   757,
     965,   966,   967,   371,     0,     0,   372,   600,     0,     0,
     968,   374,     0,     0,   375,   766,   767,   969,   376,   377,
     378,   379,   380,     0,   970,   382,   383,     0,   384,   385,
     386,   387,   388,   971,   390,     0,   391,   392,   393,   394,
     395,   396,   397,   398,   770,   399,   972,   401,   402,   403,
     404,     0,     0,   405,   406,   407,   973,   408,   409,   410,
     411,   773,   412,   974,     0,   414,   415,   416,   417,   418,
     419,   420,     0,   975,   422,     0,   423,   424,   425,     0,
     976,   977,     0,   429,     0,   430,   978,   432,   979,   980,
     434,   435,   436,   437,     0,   981,   438,   439,   440,   441,
     442,   982,     0,   443,   444,   445,   983,   447,   784,   448,
     449,     0,     0,     0,   450,   451,   452,   453,   454,   455,
       0,   456,    43,   984,   457,   985,     0,     0,   986,   987,
      47,   988,    48,     0,     0,     0,     0,   989,   990,   991,
       0,   992,   993,     0,     0,     0,   994,   693,   230,   231,
     232,   233,     0,   234,   235,     0,   236,   237,   238,     0,
     239,     0,   240,     0,   241,   242,   243,   244,   245,   246,
     694,   247,   248,   249,   250,   251,     0,   252,   253,   254,
     255,   695,   696,     0,   256,   257,   258,     0,   259,   697,
     698,     0,   260,   261,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   262,
     263,   264,   265,   266,   267,   268,     0,   269,     0,     0,
       0,     0,     0,     0,   270,   271,   272,   706,   273,   274,
     275,   707,   939,   708,   940,     0,   941,   278,   279,     0,
     280,   942,   943,   281,   944,   945,   284,     0,   285,   286,
     287,   288,   946,     0,     0,   290,   291,   292,     0,   293,
     294,   947,   295,     0,   296,   297,   718,   948,   949,   950,
     951,   298,   299,   300,   301,   302,   952,   953,   305,     0,
       0,   306,     0,   307,   308,   309,     0,     0,     0,   310,
     954,   312,   313,     0,   314,   315,     0,   316,     0,   317,
     318,   955,   320,     0,   956,   957,   322,   958,     0,   324,
       0,   325,   734,     0,   736,   326,     0,   327,     0,     0,
     328,     0,   329,   740,   330,   331,   332,   741,   333,   334,
     335,     0,   743,   336,     0,   337,   338,   339,   959,   960,
       0,   961,     0,     0,   747,   748,   342,   749,   343,   344,
     345,     0,   751,   346,   752,     0,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   962,
     754,   963,   599,   362,   363,   364,   365,   366,   964,   367,
     368,   757,   965,   966,   967,   371,     0,     0,   372,   600,
       0,     0,   968,   374,     0,     0,   375,   766,   767,   969,
     376,   377,   378,   379,   380,     0,   970,   382,   383,     0,
     384,   385,   386,   387,   388,   971,   390,     0,   391,   392,
     393,   394,   395,   396,   397,   398,   770,   399,   972,   401,
     402,   403,   404,     0,     0,   405,   406,   407,   973,   408,
     409,   410,   411,   773,   412,   974,     0,   414,   415,   416,
     417,   418,   419,   420,     0,   975,   422,     0,   423,   424,
     425,     0,   976,   977,     0,   429,     0,   430,   978,   432,
     979,   980,   434,   435,   436,   437,     0,   981,   438,   439,
     440,   441,   442,   982,     0,   443,   444,   445,   983,   447,
     784,   448,   449,     0,     0,     0,   450,   451,   452,   453,
     454,   455,     0,   456,    43,   984,   457,   985,     0,     0,
     986,   987,    47,   988,    48,     0,     0,     0,     0,   989,
     990,   991,     0,   992,   993,     0,     0,     0,   994,   693,
     230,   231,   232,   233,     0,   234,   235,     0,   236,   237,
     238,     0,   239,     0,   240,     0,   241,   242,   243,   244,
     245,   246,   694,   247,   248,   249,   250,   251,     0,   252,
     253,   254,   255,   695,   696,     0,   256,   257,   258,     0,
     259,   697,   698,     0,   260,   261,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   262,   263,   264,   265,   266,   267,   268,     0,   269,
       0,     0,     0,     0,     0,     0,   270,   271,   272,   706,
     273,   274,   275,   707,   939,   708,   940,     0,   941,   278,
     279,     0,   280,   942,   943,   281,   944,   945,   284,     0,
     285,   286,   287,   288,   946,     0,     0,   290,   291,   292,
       0,   293,   294,   947,   295,     0,   296,   297,   718,   948,
     949,   950,   951,   298,   299,   300,   301,   302,   952,   953,
     305,     0,     0,   306,     0,   307,   308,   309,     0,     0,
       0,   310,   954,   312,   313,     0,   314,   315,     0,   316,
       0,   317,   318,   955,   320,     0,   956,   957,   322,   958,
       0,   324,     0,   325,   734,     0,   736,   326,     0,   327,
       0,     0,   328,     0,   329,   740,   330,   331,   332,   741,
     333,   334,   335,     0,   743,   336,     0,   337,   338,   339,
     959,   960,     0,   961,     0,     0,   747,   748,   342,   749,
     343,   344,   345,     0,   751,   346,   752,     0,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   962,   754,   963,   599,   362,   363,   364,   365,   366,
     964,   367,   368,   757,   965,   966,   967,   371,     0,     0,
     372,   600,     0,     0,   968,   374,     0,     0,   375,   766,
     767,   969,   376,   377,   378,   379,   380,     0,   970,   382,
     383,     0,   384,   385,   386,   387,   388,   971,   390,     0,
     391,   392,   393,   394,   395,   396,   397,   398,   770,   399,
     972,   401,   402,   403,   404,     0,   116,   405,   406,   407,
     973,   408,   409,   410,   411,   773,   412,   974,     0,   414,
     415,   416,   417,   418,   419,   420,     0,   975,   422,     0,
     423,   424,   425,     0,   976,   977,     0,   429,     0,   430,
     978,   432,   979,   980,   434,   435,   436,   437,     0,   981,
     438,   439,   440,   441,   442,   982,     0,   443,   444,   445,
     983,   447,   784,   448,   449,     0,     0,     0,   450,   451,
     452,   453,   454,   455,     0,   456,    43,   984,   457,   985,
       0,     0,   986,   987,    47,   988,    48,     0,     0,     0,
       0,   989,   990,     0,     0,   992,   993,     0,     0,     0,
     994,   693,   230,   231,   232,   233,     0,   234,   235,     0,
     236,   237,   238,     0,   239,     0,   240,     0,   241,   242,
     243,   244,   245,   246,   694,   247,   248,   249,   250,   251,
       0,   252,   253,   254,   255,   695,   696,     0,   256,   257,
     258,     0,   259,   697,   698,     0,   260,   261,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   262,   263,   264,   265,   266,   267,   268,
       0,   269,     0,     0,     0,     0,     0,     0,   270,   271,
     272,   706,   273,   274,   275,   707,   939,   708,   940,     0,
     941,   278,   279,     0,   280,   942,   943,   281,   944,   945,
     284,     0,   285,   286,   287,   288,   946,     0,     0,   290,
     291,   292,     0,   293,   294,   947,   295,     0,   296,   297,
     718,   948,   949,   950,   951,   298,   299,   300,   301,   302,
     952,   953,   305,     0,     0,   306,     0,   307,   308,   309,
       0,     0,     0,   310,   954,   312,   313,     0,   314,   315,
       0,   316,     0,   317,   318,   955,   320,     0,   956,   957,
     322,   958,     0,   324,     0,   325,   734,  1691,   736,   326,
       0,   327,     0,     0,   328,     0,   329,   740,   330,   331,
     332,   741,   333,   334,   335,     0,   743,   336,     0,   337,
     338,   339,   959,   960,     0,   961,     0,     0,   747,   748,
     342,   749,   343,   344,   345,     0,   751,   346,   752,     0,
     347,   348,   349,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,   962,   754,   963,   599,   362,   363,   364,
     365,   366,   964,   367,   368,   757,   965,   966,   967,   371,
       0,     0,   372,   600,     0,     0,   968,   374,     0,     0,
     375,   766,   767,   969,   376,   377,   378,   379,   380,     0,
     970,   382,   383,     0,   384,   385,   386,   387,   388,   971,
     390,     0,   391,   392,   393,   394,   395,   396,   397,   398,
     770,   399,   972,   401,   402,   403,   404,     0,     0,   405,
     406,   407,   973,   408,   409,   410,   411,   773,   412,   974,
       0,   414,   415,   416,   417,   418,   419,   420,     0,   975,
     422,     0,   423,   424,   425,     0,   976,   977,     0,   429,
       0,   430,   978,   432,   979,   980,   434,   435,   436,   437,
       0,   981,   438,   439,   440,   441,   442,   982,     0,   443,
     444,   445,   983,   447,   784,   448,   449,     0,     0,     0,
     450,   451,   452,   453,   454,   455,     0,   456,    43,   984,
     457,   985,     0,     0,   986,   987,    47,   988,    48,     0,
       0,     0,     0,   989,   990,     0,     0,   992,   993,     0,
       0,     0,   994,   693,   230,   231,   232,   233,     0,   234,
     235,     0,   236,   237,   238,     0,   239,     0,   240,     0,
     241,   242,   243,   244,   245,   246,   694,   247,   248,   249,
     250,   251,     0,   252,   253,   254,   255,   695,   696,     0,
     256,   257,   258,     0,   259,   697,   698,     0,   260,   261,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   262,   263,   264,   265,   266,
     267,   268,     0,   269,     0,     0,     0,     0,     0,     0,
     270,   271,   272,   706,   273,   274,   275,   707,   939,   708,
     940,     0,   941,   278,   279,     0,   280,   942,   943,   281,
     944,   945,   284,     0,   285,   286,   287,   288,   946,     0,
       0,   290,   291,   292,     0,   293,   294,   947,   295,     0,
     296,   297,   718,   948,   949,   950,   951,   298,   299,   300,
     301,   302,   952,   953,   305,     0,     0,   306,     0,   307,
     308,   309,     0,     0,     0,   310,   954,   312,   313,     0,
     314,   315,     0,   316,     0,   317,   318,   955,   320,     0,
     956,   957,   322,   958,     0,   324,     0,   325,   734,     0,
     736,   326,     0,   327,     0,     0,   328,     0,   329,   740,
     330,   331,   332,   741,   333,   334,   335,     0,   743,   336,
       0,   337,   338,   339,   959,   960,     0,   961,     0,     0,
     747,   748,   342,   749,   343,   344,   345,     0,   751,   346,
     752,     0,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   962,   754,   963,   599,   362,
     363,   364,   365,   366,   964,   367,   368,   757,   965,   966,
     967,   371,     0,     0,   372,   600,     0,     0,   968,   374,
       0,     0,   375,   766,   767,   969,   376,   377,   378,   379,
     380,     0,   970,   382,   383,     0,   384,   385,   386,   387,
     388,   971,   390,     0,   391,   392,   393,   394,   395,   396,
     397,   398,   770,   399,   972,   401,   402,   403,   404,     0,
       0,   405,   406,   407,   973,   408,   409,   410,   411,   773,
     412,   974,     0,   414,   415,   416,   417,   418,   419,   420,
       0,   975,   422,     0,   423,   424,   425,     0,   976,   977,
       0,   429,     0,   430,   978,   432,   979,   980,   434,   435,
     436,   437,     0,   981,   438,   439,   440,   441,   442,   982,
       0,   443,   444,   445,   983,   447,   784,   448,   449,     0,
       0,     0,   450,   451,   452,   453,   454,   455,     0,   456,
      43,   984,   457,   985,     0,     0,   986,   987,    47,   988,
      48,     0,     0,     0,     0,   989,   990,     0,     0,   992,
     993,     0,     0,     0,   994,   693,   230,   231,   232,   233,
       0,   234,   235,     0,   236,   237,   238,     0,   239,     0,
     240,     0,   241,   242,   243,   244,   245,   246,   694,   247,
     248,   249,   250,   251,     0,   252,   253,   254,   255,   695,
     696,     0,   256,   257,   258,     0,   259,   697,   698,     0,
     260,   261,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   262,   263,   264,
     265,   266,   267,   268,     0,   269,     0,     0,     0,     0,
       0,     0,   270,   271,   272,   706,   273,   274,   275,   707,
     939,   708,   940,     0,   941,   278,   279,     0,   280,   942,
     943,   281,   944,   945,   284,     0,   285,   286,   287,   288,
     946,     0,     0,   290,   291,   292,     0,   293,   294,   947,
     295,     0,   296,   297,   718,   948,   949,   950,   951,   298,
     299,   300,   301,   302,   952,   953,   305,     0,     0,   306,
       0,   307,   308,   309,     0,     0,     0,   310,   954,   312,
     313,     0,   314,   315,     0,   316,     0,   317,   318,   955,
     320,     0,   956,   957,   322,   958,     0,   324,     0,   325,
     734,     0,   736,   326,     0,   327,     0,     0,   328,     0,
     329,   740,   330,   331,   332,   741,   333,   334,   335,     0,
     743,   336,     0,   337,   338,   339,   959,   960,     0,   961,
       0,     0,   747,   748,   342,   749,   343,   344,   345,     0,
     751,   346,   752,     0,   347,   348,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,   962,   754,   963,
     599,   362,   363,   364,   365,   366,     0,   367,   368,   757,
     965,   966,   967,   371,     0,     0,   372,   600,     0,     0,
     968,   374,     0,     0,   375,   766,   767,   969,   376,   377,
     378,   379,   380,     0,   970,   382,   383,     0,   384,   385,
     386,   387,   388,   971,   390,     0,   391,   392,   393,   394,
     395,   396,   397,   398,   770,   399,   400,   401,   402,   403,
     404,     0,     0,   405,   406,   407,   973,   408,   409,   410,
     411,   773,   412,   974,     0,   414,   415,   416,   417,   418,
     419,   420,     0,   975,   422,     0,   423,   424,   425,     0,
     976,   977,     0,   429,     0,   430,   978,   432,   979,   980,
     434,   435,   436,   437,     0,     0,   438,   439,   440,   441,
     442,   982,     0,   443,   444,   445,   983,   447,   784,   448,
     449,     0,     0,     0,   450,   451,   452,   453,   454,   455,
       0,   456,    43,   984,   457,   985,     0,     0,   986,   987,
      47,   988,    48,     0,     0,     0,     0,     0,  1671,     0,
       0,  1672,  1673,     0,     0,     0,  1674,   693,   230,   231,
     232,   233,     0,   234,   235,     0,   236,   237,   238,     0,
     239,     0,   240,     0,   241,   242,   243,   244,   245,   246,
     694,   247,   248,   249,   250,   251,     0,   252,   253,   254,
     255,   695,   696,     0,   256,   257,   258,     0,   259,   697,
     698,     0,   260,   261,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   262,
     263,   264,   265,   266,   267,   268,     0,   269,     0,     0,
       0,     0,     0,     0,   270,   271,   272,   706,   273,   274,
     275,     0,   939,   708,   940,     0,   941,   278,   279,     0,
     280,   942,   943,   281,   944,   945,   284,     0,   285,   286,
     287,   288,   946,     0,     0,   290,   291,   292,     0,   293,
     294,   947,   295,     0,   296,   297,   718,   948,   949,   950,
     951,   298,   299,   300,   301,   302,   952,   953,   305,     0,
       0,   306,     0,   307,   308,   309,     0,     0,     0,   310,
     954,   312,   313,     0,   314,   315,     0,     0,     0,   317,
     318,   955,   320,     0,   956,   957,   322,   958,     0,   324,
       0,   325,   734,     0,   736,   326,     0,   327,     0,     0,
     328,     0,   329,     0,   330,   331,   332,     0,   333,   334,
     335,     0,   743,   336,     0,   337,   338,   339,   959,   960,
       0,   961,     0,     0,   747,   748,   342,   749,   343,   344,
     345,     0,   751,   346,     0,     0,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   962,
     754,   963,   599,   362,   363,   364,   365,   366,     0,   367,
     368,   757,   965,   966,   967,   371,     0,     0,   372,   600,
       0,     0,   968,   374,     0,     0,   375,   766,   767,   969,
     376,   377,   378,   379,   380,     0,   970,   382,   383,     0,
     384,   385,   386,   387,   388,   971,   390,     0,   391,   392,
     393,   394,   395,   396,   397,   398,   770,   399,   972,   401,
     402,   403,   404,     0,     0,   405,   406,   407,   973,   408,
     409,   410,   411,     0,   412,   974,     0,   414,   415,   416,
     417,   418,   419,   420,     0,   975,   422,     0,   423,   424,
     425,     0,   976,   977,     0,   429,     0,   430,   978,   432,
     979,   980,   434,   435,   436,   437,     0,   981,   438,   439,
     440,   441,   442,   982,     0,   443,   444,   445,   983,   447,
     784,   448,   449,     0,     0,     0,   450,   451,   452,   453,
     454,   455,     0,   456,    43,   984,   457,   985,     0,     0,
     986,   987,    47,   988,    48,     0,     0,     0,     0,   989,
     990,     0,     0,   992,   993,     0,     0,     0,   994,   693,
     230,   231,   232,   233,     0,   234,   235,     0,   236,   237,
     238,     0,   239,     0,   240,     0,   241,   242,   243,   244,
     245,   246,   694,   247,   248,   249,   250,   251,     0,   252,
     253,   254,   255,   695,   696,     0,   256,   257,   258,     0,
     259,   697,   698,     0,   260,   261,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   262,   263,   264,   265,   266,   267,   268,     0,   269,
       0,     0,     0,     0,     0,     0,   270,   271,   272,   706,
     273,   274,   275,   707,   939,   708,   940,     0,   941,   278,
     279,     0,   280,   942,   943,   281,   944,   945,   284,     0,
     285,   286,   287,   288,   946,     0,     0,   290,   291,   292,
       0,   293,   294,   947,   295,     0,   296,   297,   718,   948,
     949,   950,   951,   298,   299,   300,   301,   302,   952,   953,
     305,     0,     0,   306,     0,   307,   308,   309,     0,     0,
       0,   310,   954,   312,   313,     0,   314,   315,     0,   316,
       0,   317,   318,   955,   320,     0,   956,   957,   322,   958,
       0,   324,     0,   325,   734,     0,   736,   326,     0,   327,
       0,     0,   328,     0,   329,   740,   330,   331,   332,   741,
     333,   334,   335,     0,   743,   336,     0,   337,   338,   339,
     959,   960,     0,   961,     0,     0,   747,   748,   342,   749,
     343,   344,   345,     0,   751,   346,   752,     0,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   962,   754,   963,   599,   362,   363,   364,   365,   366,
       0,   367,   368,   757,   965,   966,   967,   371,     0,     0,
     372,   600,     0,     0,   373,   374,     0,     0,   375,   766,
     767,   969,   376,   377,   378,   379,   380,     0,   970,   382,
     383,     0,   384,   385,   386,   387,   388,   971,   390,     0,
     391,   392,   393,   394,   395,   396,   397,   398,   770,   399,
     400,   401,   402,   403,   404,     0,     0,   405,   406,   407,
     973,   408,   409,   410,   411,   773,   412,   974,     0,   414,
     415,   416,   417,   418,   419,   420,     0,   975,   422,     0,
     423,   424,   425,     0,   976,   977,     0,   429,     0,   430,
     978,   432,   979,   980,   434,   435,   436,   437,     0,     0,
     438,   439,   440,   441,   442,   982,     0,   443,   444,   445,
     983,   447,   784,   448,   449,     0,     0,     0,   450,   451,
     452,   453,   454,   455,     0,   456,    43,     0,   457,   985,
       0,     0,   986,   987,    47,   988,    48,     0,   693,   230,
     231,   232,   233,     0,   234,   235,     0,   236,   237,   238,
    1674,   239,     0,   240,     0,   241,   242,   243,   244,   245,
     246,   694,   247,   248,   249,   250,   251,     0,   252,   253,
     254,   255,   695,   696,     0,   256,   257,   258,     0,   259,
     697,   698,     0,   260,   261,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     262,   263,   264,   265,   266,   267,   268,     0,   269,     0,
       0,     0,     0,     0,     0,   270,   271,   272,   706,   273,
     274,   275,     0,   939,   708,   940,     0,   941,   278,   279,
       0,   280,   942,   943,   281,   944,   945,   284,     0,   285,
     286,   287,   288,   946,     0,     0,   290,   291,   292,     0,
     293,   294,   947,   295,     0,   296,   297,   718,   948,   949,
     950,   951,   298,   299,   300,   301,   302,   952,   953,   305,
       0,     0,   306,     0,   307,   308,   309,     0,     0,     0,
     310,   954,   312,   313,     0,   314,   315,     0,     0,     0,
     317,   318,   955,   320,     0,   956,   957,   322,   958,     0,
     324,     0,   325,   734,     0,   736,   326,     0,   327,     0,
       0,   328,     0,   329,     0,   330,   331,   332,     0,   333,
     334,   335,     0,   743,   336,     0,   337,   338,   339,   959,
     960,     0,   961,     0,     0,   747,   748,   342,   749,   343,
     344,   345,     0,   751,   346,     0,     0,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     962,   754,   963,   599,   362,   363,   364,   365,   366,     0,
     367,   368,   757,   965,   966,   967,   371,     0,     0,   372,
     600,     0,     0,     0,   374,     0,     0,   375,   766,   767,
     969,   376,   377,   378,   379,   380,     0,   970,   382,   383,
       0,   384,   385,   386,   387,   388,   971,   390,     0,   391,
     392,   393,   394,   395,   396,   397,   398,   770,   399,   400,
     401,   402,   403,   404,     0,     0,   405,   406,   407,   973,
     408,   409,   410,   411,     0,   412,   974,     0,   414,   415,
     416,   417,   418,   419,   420,     0,   975,   422,     0,   423,
     424,   425,     0,   976,   977,     0,   429,     0,   430,   978,
     432,   979,   980,   434,   435,   436,   437,     0,     0,   438,
     439,   440,   441,   442,   982,     0,   443,   444,   445,   983,
     447,   784,   448,   449,     0,     0,     0,   450,   451,   452,
     453,   454,   455,     0,   456,    43,     0,   457,   985,     0,
       0,   986,   987,    47,   988,    48,     0,     0,     0,     0,
       0,  1671,     0,     0,  1672,  1673,     0,     0,     0,  1674,
     693,   230,   231,   232,   233,     0,   234,   235,     0,   236,
     237,   238,     0,   239,     0,   240,     0,   241,   242,   243,
     244,   245,   246,   694,   247,   248,   249,   250,   251,     0,
     252,   253,   254,   255,   695,   696,     0,   256,   257,   258,
       0,   259,   697,   698,     0,   260,   261,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   262,   263,   264,   265,   266,   267,   268,  -872,
     269,     0,     0,     0,  -872,     0,     0,   270,   271,   272,
     706,   273,   274,   275,     0,   939,   708,   940,     0,   941,
     278,   279,     0,   280,   942,   943,   281,   944,   945,   284,
       0,   285,   286,   287,   288,   946,     0,     0,   290,   291,
     292,     0,   293,   294,   947,   295,     0,   296,   297,   718,
     948,   949,   950,   951,   298,   299,   300,   301,   302,   952,
     953,   305,     0,     0,   306,     0,   307,   308,   309,     0,
       0,     0,   310,   954,   312,   313,     0,   314,   315,     0,
       0,     0,   317,   318,   955,   320,     0,   956,   957,   322,
     958,     0,   324,     0,   325,   734,     0,   736,   326,     0,
     327,     0,     0,   328,     0,   329,     0,   330,   331,   332,
       0,   333,   334,   335,     0,   743,   336,     0,   337,   338,
     339,   959,   960,     0,   961,     0,     0,     0,     0,   342,
     749,   343,   344,   345,     0,   751,   346,     0,     0,   347,
     348,   349,   350,   351,   352,   353,   354,   355,   356,   357,
     358,   359,   962,   754,   963,   599,   362,   363,   364,   365,
     366,     0,   367,   368,     0,   965,   966,   967,   371,     0,
       0,   372,   600,     0,     0,     0,   374,     0,     0,   375,
     766,   767,   969,   376,   377,   378,   379,   380,     0,   970,
     382,   383,     0,   384,   385,   386,   387,   388,   971,   390,
       0,   391,   392,   393,   394,   395,   396,   397,   398,   770,
     399,   972,   401,   402,   403,   404,     0,     0,   405,   406,
     407,   973,   408,   409,   410,   411,     0,   412,   974,  -872,
     414,   415,   416,   417,   418,   419,   420,     0,   975,   422,
       0,   423,   424,   425,     0,   976,   977,     0,   429,     0,
     430,   978,   432,   979,   980,   434,   435,   436,   437,     0,
     981,   438,   439,   440,   441,   442,   982,     0,   443,   444,
     445,   983,   447,   784,   448,   449,     0,     0,     0,   450,
     451,   452,   453,   454,   455,     0,   456,    43,     0,   457,
     985,     0,     0,   986,   987,    47,   988,    48,     0,   693,
     230,   231,   232,   233,     0,   234,   235,   993,   236,   237,
     238,   994,   239,     0,   240,     0,   241,   242,   243,   244,
     245,   246,   694,   247,   248,   249,   250,   251,     0,   252,
     253,   254,   255,   695,   696,     0,   256,   257,   258,     0,
     259,   697,   698,     0,   260,   261,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   262,   263,   264,   265,   266,   267,   268,  -873,   269,
       0,     0,     0,  -873,     0,     0,   270,   271,   272,   706,
     273,   274,   275,     0,   939,   708,   940,     0,   941,   278,
     279,     0,   280,   942,   943,   281,   944,   945,   284,     0,
     285,   286,   287,   288,   946,     0,     0,   290,   291,   292,
       0,   293,   294,   947,   295,     0,   296,   297,   718,   948,
     949,   950,   951,   298,   299,   300,   301,   302,   952,   953,
     305,     0,     0,   306,     0,   307,   308,   309,     0,     0,
       0,   310,   954,   312,   313,     0,   314,   315,     0,     0,
       0,   317,   318,   955,   320,     0,   956,   957,   322,   958,
       0,   324,     0,   325,   734,     0,   736,   326,     0,   327,
       0,     0,   328,     0,   329,     0,   330,   331,   332,     0,
     333,   334,   335,     0,   743,   336,     0,   337,   338,   339,
     959,   960,     0,   961,     0,     0,     0,     0,   342,   749,
     343,   344,   345,     0,   751,   346,     0,     0,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   962,   754,   963,   599,   362,   363,   364,   365,   366,
       0,   367,   368,     0,   965,   966,   967,   371,     0,     0,
     372,   600,     0,     0,     0,   374,     0,     0,   375,   766,
     767,   969,   376,   377,   378,   379,   380,     0,   970,   382,
     383,     0,   384,   385,   386,   387,   388,   971,   390,     0,
     391,   392,   393,   394,   395,   396,   397,   398,   770,   399,
     972,   401,   402,   403,   404,     0,     0,   405,   406,   407,
     973,   408,   409,   410,   411,     0,   412,   974,  -873,   414,
     415,   416,   417,   418,   419,   420,     0,   975,   422,     0,
     423,   424,   425,     0,   976,   977,     0,   429,     0,   430,
     978,   432,   979,   980,   434,   435,   436,   437,     0,   981,
     438,   439,   440,   441,   442,   982,     0,   443,   444,   445,
     983,   447,   784,   448,   449,     0,     0,     0,   450,   451,
     452,   453,   454,   455,     0,   456,    43,     0,   457,   985,
       0,     0,   986,   987,    47,   988,    48,     0,   693,   230,
     231,   232,   233,     0,   234,   235,     0,   236,   237,   238,
     994,   239,     0,   240,     0,   241,   242,   243,   244,   245,
     246,   694,   247,   248,   249,   250,   251,     0,   252,   253,
     254,   255,   695,   696,     0,   256,   257,   258,     0,   259,
     697,   698,     0,   260,   261,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     262,   263,   264,   265,   266,   267,   268,     0,   269,     0,
       0,     0,     0,     0,     0,   270,   271,   272,   706,   273,
     274,   275,     0,   939,   708,   940,     0,   941,   278,   279,
       0,   280,   942,   943,   281,   944,   945,   284,     0,   285,
     286,   287,   288,   946,     0,     0,   290,   291,   292,     0,
     293,   294,   947,   295,     0,   296,   297,   718,   948,   949,
     950,   951,   298,   299,   300,   301,   302,   952,   953,   305,
       0,     0,   306,     0,   307,   308,   309,     0,     0,     0,
     310,   954,   312,   313,     0,   314,   315,     0,     0,     0,
     317,   318,   955,   320,     0,   956,   957,   322,   958,     0,
     324,     0,   325,   734,     0,   736,   326,     0,   327,     0,
       0,   328,     0,   329,     0,   330,   331,   332,     0,   333,
     334,   335,     0,   743,   336,     0,   337,   338,   339,   959,
     960,     0,   961,     0,     0,     0,     0,   342,   749,   343,
     344,   345,     0,   751,   346,     0,     0,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     962,   754,   963,   599,   362,   363,   364,   365,   366,     0,
     367,   368,     0,     0,   966,   967,   371,     0,     0,   372,
     600,     0,     0,     0,   374,     0,     0,   375,   766,   767,
     969,   376,   377,   378,   379,   380,     0,   970,   382,   383,
       0,   384,   385,   386,   387,   388,   971,   390,     0,   391,
     392,   393,   394,   395,   396,   397,   398,   770,   399,   400,
     401,   402,   403,   404,     0,     0,   405,   406,   407,   973,
     408,   409,   410,   411,     0,   412,   974,     0,   414,   415,
     416,   417,   418,   419,   420,     0,   975,   422,     0,   423,
     424,   425,     0,   976,   977,     0,   429,     0,   430,   978,
     432,   979,   980,   434,   435,   436,   437,     0,     0,   438,
     439,   440,   441,   442,   982,     0,   443,   444,   445,   983,
     447,   784,   448,   449,     0,     0,     0,   450,   451,   452,
     453,   454,   455,     0,   456,    43,     0,   457,   985,     0,
       0,   986,   987,    47,   988,    48,     0,   693,   230,   231,
     232,   233,     0,   234,   235,  1673,   236,   237,   238,  1674,
     239,     0,   240,     0,   241,   242,   243,   244,   245,   246,
     694,   247,   248,   249,   250,   251,     0,   252,   253,   254,
     255,   695,   696,     0,   256,   257,   258,     0,   259,   697,
     698,     0,   260,   261,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   262,
     263,   264,   265,   266,   267,   268,     0,   269,     0,     0,
       0,     0,     0,     0,   270,   271,   272,   706,   273,   274,
     275,     0,   939,   708,   940,     0,   941,   278,   279,     0,
     280,   942,   943,   281,   944,   945,   284,     0,   285,   286,
     287,   288,   946,     0,     0,   290,   291,   292,     0,   293,
     294,   947,   295,     0,   296,   297,   718,   948,   949,   950,
     951,   298,   299,   300,   301,   302,   952,   953,   305,     0,
       0,   306,     0,   307,   308,   309,     0,     0,     0,   310,
     954,   312,   313,     0,   314,   315,     0,     0,     0,   317,
     318,   955,   320,     0,   956,   957,   322,   958,     0,   324,
       0,   325,   734,     0,   736,   326,     0,   327,     0,     0,
     328,     0,   329,     0,   330,   331,   332,     0,   333,   334,
     335,     0,   743,   336,     0,   337,   338,   339,   959,   960,
       0,   961,     0,     0,     0,     0,   342,   749,   343,   344,
     345,     0,   751,   346,     0,     0,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   962,
     754,   963,   599,   362,   363,   364,   365,   366,     0,   367,
     368,     0,     0,   966,   967,   371,     0,     0,   372,   600,
       0,     0,     0,   374,     0,     0,   375,   766,   767,   969,
     376,   377,   378,   379,   380,     0,   970,   382,   383,     0,
     384,   385,   386,   387,   388,   971,   390,     0,   391,   392,
     393,   394,   395,   396,   397,   398,   770,   399,   400,   401,
     402,   403,   404,     0,     0,   405,   406,   407,   973,   408,
     409,   410,   411,     0,   412,   974,     0,   414,   415,   416,
     417,   418,   419,   420,     0,   975,   422,     0,   423,   424,
     425,     0,   976,   977,     0,   429,     0,   430,   978,   432,
     979,   980,   434,   435,   436,   437,     0,     0,   438,   439,
     440,   441,   442,   982,     0,   443,   444,   445,   983,   447,
     784,   448,   449,     0,     0,     0,   450,   451,   452,   453,
     454,   455,     0,   456,    43,     0,   457,   985,     0,     0,
     986,   987,    47,   988,    48,   230,   231,   232,   233,     0,
     234,   235,     0,   236,   237,   238,     0,   239,  1674,   240,
       0,   241,   242,   243,   244,   245,   246,     0,   247,   248,
     249,   250,   251,     0,   252,   253,   254,   255,     0,     0,
       0,   256,   257,   258,     0,   259,     0,     0,     0,   260,
     261,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   262,   263,   264,   265,
     266,   267,   268,     0,   269,     0,     0,     0,     0,  2356,
       0,   270,   271,   272,     0,   273,   274,   275,     0,   276,
       0,   277,     0,     0,   278,   279,     0,   280,     0,     0,
     281,   282,   283,   284,     0,   285,   286,   287,   288,   289,
       0,     0,   290,   291,   292,     0,   293,   294,     0,   295,
       0,   296,   297,     0,     0,     0,     0,     0,   298,   299,
     300,   301,   302,   303,   304,   305,     0,     0,   306,     0,
     307,   308,   309,     0,     0,     0,   310,   311,   312,   313,
       0,   314,   315,     0,   316,  -656,   317,   318,   319,   320,
       0,   321,     0,   322,   323,  -656,   324,     0,   325,     0,
       0,     0,   326,     0,   327,     0,     0,   328,     0,   329,
       0,   330,   331,   332,     0,   333,   334,   335,     0,     0,
     336,     0,   337,   338,   339,     0,   340,  -656,   341,     0,
       0,     0,     0,   342,     0,   343,   344,   345,     0,     0,
     346,     0,  -656,   347,   348,   349,   350,   351,   352,   353,
     354,   355,   356,   357,   358,   359,   360,     0,   361,     0,
     362,   363,   364,   365,   366,     0,   367,   368,     0,     0,
     369,   370,   371,     0,  -656,   372,     0,     0,     0,   373,
     374,     0,  -656,   375,     0,     0,     0,   376,   377,   378,
     379,   380,     0,   381,   382,   383,     0,   384,   385,   386,
     387,   388,   389,   390,     0,   391,   392,   393,   394,   395,
     396,   397,   398,     0,   399,   400,   401,   402,   403,   404,
       0,     0,   405,   406,   407,     0,   408,   409,   410,   411,
       0,   412,   413,     0,   414,   415,   416,   417,   418,   419,
     420,     0,   421,   422,     0,   423,   424,   425,     0,   426,
     427,     0,   429,     0,   430,   431,   432,   433,     0,   434,
     435,   436,   437,  -656,     0,   438,   439,   440,   441,   442,
       0,     0,   443,   444,   445,   446,   447,     4,   448,   449,
       0,     0,     0,   450,   451,   452,   453,   454,   455,     0,
     456,     5,     0,   457,     0,     0,     0,     0,     0,     0,
       6,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,  1067,     0,     0,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,     0,    34,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    35,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    37,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    38,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    39,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    41,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    42,    43,     0,    44,    45,     0,     0,
       0,     0,    47,     0,    48,    49,     0,     0,     0,    50,
      51,    52,    53,    54,     0,     0,    55,    56,  1834,   230,
     231,   232,   233,     0,   234,   235,     0,   236,   237,   238,
       0,   239,     0,   240,     0,   241,   242,   243,   244,   245,
     246,     0,   247,   248,   249,   250,   251,     0,   252,   253,
     254,   255,     0,     0,     0,   256,   257,   258,     0,   259,
       0,     0,     0,   260,   261,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     262,   263,   264,   265,   266,   267,   268,     0,   269,     0,
       0,     0,     0,     0,     0,   270,   271,   272,   706,   273,
     274,   275,   707,   276,   708,   277,     0,     0,   278,   279,
       0,   280,     0,     0,   281,   282,   283,   284,     0,   285,
     286,   287,   288,   289,     0,     0,   290,   291,   292,     0,
     293,   294,     0,   295,     0,   296,   297,   718,     0,     0,
       0,     0,   298,   299,   300,   301,   302,   303,   304,   305,
       0,     0,   306,     0,   307,   308,   309,     0,     0,     0,
     310,   311,   312,   313,     0,   314,   315,     0,   316,     0,
     317,   318,   319,   320,     0,   321,     0,   322,   323,     0,
     324,     0,   325,   734,     0,   736,   326,     0,   327,     0,
       0,   328,     0,   329,   740,   330,   331,   332,   741,   333,
     334,   335,     0,   743,   336,     0,   337,   338,   339,     0,
     340,     0,   341,     0,     0,   747,   748,   342,   749,   343,
     344,   345,     0,   751,   346,   752,     0,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,   754,   361,   599,   362,   363,   364,   365,   366,     0,
     367,   368,   757,     0,   369,   370,   371,     0,     0,   372,
     600,     0,   651,   373,   374,     0,     0,   375,   766,   767,
       0,   376,   377,   378,   379,   380,     0,   381,   382,   383,
       0,   384,   385,   386,   387,   388,   389,   390,     0,   391,
     392,   393,   394,   395,   396,   397,   398,   770,   399,   400,
     401,   402,   403,   404,     0,   116,   405,   406,   407,     0,
     408,   409,   410,   411,   773,   412,   413,     0,   414,   415,
     416,   417,   418,   419,   420,     0,   421,   422,     0,   423,
     424,   425,     0,   426,   427,     0,   429,     0,   430,   431,
     432,   433,     0,   434,   435,   436,   437,     0,     0,   438,
     439,   440,   441,   442,     0,     0,   443,   444,   445,   446,
     447,   784,   448,   449,     0,     0,     0,   450,   451,   452,
     453,   454,   455,     0,   456,     0,     0,   457,     0,     0,
       0,     0,     0,     0,   230,   231,   232,   233,     0,   234,
     235,     0,   236,   237,   238,     0,   239,     0,   240,  2094,
     241,   242,   243,   244,   245,   246,     0,   247,   248,   249,
     250,   251,     0,   252,   253,   254,   255,     0,     0,     0,
     256,   257,   258,     0,   259,     0,     0,     0,   260,   261,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   262,   263,   264,   265,   266,
     267,   268,     0,   269,     0,     0,     0,     0,     0,     0,
     270,   271,   272,   706,   273,   274,   275,   707,   276,   708,
     277,     0,     0,   278,   279,     0,   280,     0,     0,   281,
     282,   283,   284,     0,   285,   286,   287,   288,   289,     0,
       0,   290,   291,   292,     0,   293,   294,     0,   295,     0,
     296,   297,   718,     0,     0,     0,     0,   298,   299,   300,
     301,   302,   303,   304,   305,     0,     0,   306,     0,   307,
     308,   309,     0,     0,     0,   310,   311,   312,   313,     0,
     314,   315,     0,   316,     0,   317,   318,   319,   320,     0,
     321,     0,   322,   323,     0,   324,     0,   325,   734,     0,
     736,   326,     0,   327,     0,     0,   328,     0,   329,   740,
     330,   331,   332,   741,   333,   334,   335,     0,   743,   336,
       0,   337,   338,   339,     0,   340,     0,   341,     0,     0,
     747,   748,   342,   749,   343,   344,   345,     0,   751,   346,
     752,     0,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   754,   361,   599,   362,
     363,   364,   365,   366,     0,   367,   368,   757,     0,   369,
     370,   371,     0,     0,   372,   600,     0,   651,   373,   374,
       0,     0,   375,   766,   767,     0,   376,   377,   378,   379,
     380,     0,   381,   382,   383,     0,   384,   385,   386,   387,
     388,   389,   390,     0,   391,   392,   393,   394,   395,   396,
     397,   398,   770,   399,   400,   401,   402,   403,   404,     0,
       0,   405,   406,   407,     0,   408,   409,   410,   411,   773,
     412,   413,     0,   414,   415,   416,   417,   418,   419,   420,
       0,   421,   422,     0,   423,   424,   425,     0,   426,   427,
       0,   429,     0,   430,   431,   432,   433,     0,   434,   435,
     436,   437,     0,     0,   438,   439,   440,   441,   442,     0,
       0,   443,   444,   445,   446,   447,   784,   448,   449,     0,
       0,     0,   450,   451,   452,   453,   454,   455,     0,   456,
       0,     0,   457,     0,     0,     0,     0,     0,     0,   230,
     231,   232,   233,     0,   234,   235,     0,   236,   237,   238,
       0,   239,     0,   240,  2094,   241,   242,   243,   244,   245,
     246,     0,   247,   248,   249,   250,   251,     0,   252,   253,
     254,   255,     0,     0,     0,   256,   257,   258,     0,   259,
       0,     0,     0,   260,   261,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     262,   263,   264,   265,   266,   267,   268,     0,   269,     0,
       0,     0,     0,     0,     0,   270,   271,   272,     0,   273,
     274,   275,     0,   276,     0,   277,     0,     0,   278,   279,
       0,   280,     0,     0,   281,   282,   283,   284,     0,   285,
     286,   287,   288,   289,     0,     0,   290,   291,   292,     0,
     293,   294,     0,   295,     0,   296,   297,     0,     0,     0,
       0,     0,   298,   299,   300,   301,   302,   303,   304,   305,
       0,     0,   306,     0,   307,   308,   309,     0,     0,     0,
     310,   311,   312,   313,     0,   314,   315,     0,   316,     0,
     317,   318,   319,   320,     0,   321,     0,   322,   323,     0,
     324,     0,   325,     0,     0,     0,   326,     0,   327,     0,
       0,   328,     0,   329,     0,   330,   331,   332,     0,   333,
     334,   335,     0,     0,   336,     0,   337,   338,   339,     0,
     340,     0,   341,     0,     0,     0,     0,   342,     0,   343,
     344,   345,     0,     0,   346,     0,     0,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,     0,   361,   599,   362,   363,   364,   365,   366,     0,
     367,   368,     0,     0,   369,   370,   371,     0,     0,   372,
     600,     0,     0,   373,   374,     0,     0,   375,     0,     0,
       0,   376,   377,   378,   379,   380,     0,   381,   382,   383,
       0,   384,   385,   386,   387,   388,   389,   390,     0,   391,
     392,   393,   394,   395,   396,   397,   398,     0,   399,   400,
     401,   402,   403,   404,     0,     0,   405,   406,   407,     0,
     408,   409,   410,   411,     0,   412,   413,     0,   414,   415,
     416,   417,   418,   419,   420,     0,   421,   422,     0,   423,
     424,   425,     0,   426,   427,     0,   429,     0,   430,   431,
     432,   433,     0,   434,   435,   436,   437,     0,     0,   438,
     439,   440,   441,   442,     0,     0,   443,   444,   445,   446,
     447,     0,   448,   449,     0,     0,     0,   450,   451,   452,
     453,   454,   455,     0,   456,     0,     0,   457,     0,     0,
       0,     0,     0,     0,   230,   231,   232,   233,     0,   234,
     235,     0,   236,   237,   238,     0,   239,     0,   240,  1061,
     241,   242,   243,   244,   245,   246,     0,   247,   248,   249,
     250,   251,     0,   252,   253,   254,   255,     0,     0,     0,
     256,   257,   258,     0,   259,     0,     0,     0,   260,   261,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   262,   263,   264,   265,   266,
     267,   268,     0,   269,     0,     0,     0,     0,     0,     0,
     270,   271,   272,     0,   273,   274,   275,     0,   276,     0,
     277,     0,     0,   278,   279,     0,   280,     0,     0,   281,
     282,   283,   284,     0,   285,   286,   287,   288,   289,     0,
       0,   290,   291,   292,     0,   293,   294,     0,   295,     0,
     296,   297,     0,     0,     0,     0,     0,   298,   299,   300,
     301,   302,   303,   304,   305,     0,     0,   306,     0,   307,
     308,   309,     0,     0,     0,   310,   311,   312,   313,     0,
     314,   315,     0,   316,     0,   317,   318,   319,   320,     0,
     321,     0,   322,   323,     0,   324,     0,   325,     0,     0,
       0,   326,     0,   327,     0,     0,   328,     0,   329,     0,
     330,   331,   332,     0,   333,   334,   335,     0,     0,   336,
       0,   337,   338,   339,     0,   340,     0,   341,     0,     0,
       0,     0,   342,     0,   343,   344,   345,     0,     0,   346,
       0,     0,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,     0,   361,     0,   362,
     363,   364,   365,   366,     0,   367,   368,     0,     0,   369,
     370,   371,     0,     0,   372,     0,     0,     0,   373,   374,
       0,     0,   375,     0,     0,     0,   376,   377,   378,   379,
     380,     0,   381,   382,   383,     0,   384,   385,   386,   387,
     388,   389,   390,     0,   391,   392,   393,   394,   395,   396,
     397,   398,     0,   399,   400,   401,   402,   403,   404,     0,
     116,   405,   406,   407,     0,   408,   409,   410,   411,     0,
     412,   413,     0,   414,   415,   416,   417,   418,   419,   420,
       0,   421,   422,     0,   423,   424,   425,     0,   426,   427,
       0,   429,     0,   430,   431,   432,   433,     0,   434,   435,
     436,   437,     0,     0,   438,   439,   440,   441,   442,     0,
       0,   443,   444,   445,   446,   447,     0,   448,   449,     0,
       0,     0,   450,   451,   452,   453,   454,   455,     0,   456,
       0,     0,   457,     0,     0,     0,     0,     0,     0,   230,
     231,   232,   233,     0,   234,   235,     0,   236,   237,   238,
       0,   239,     0,   240,   125,   241,   242,   243,   244,   245,
     246,     0,   247,   248,   249,   250,   251,     0,   252,   253,
     254,   255,     0,     0,     0,   256,   257,   258,     0,   259,
       0,     0,     0,   260,   261,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     262,   263,   264,   265,   266,   267,   268,     0,   269,     0,
       0,     0,     0,     0,     0,   270,   271,   272,     0,   273,
     274,   275,     0,   276,     0,   277,     0,     0,   278,   279,
       0,   280,     0,     0,   281,   282,   283,   284,     0,   285,
     286,   287,   288,   289,     0,     0,   290,   291,   292,     0,
     293,   294,     0,   295,     0,   296,   297,     0,     0,     0,
       0,     0,   298,   299,   300,   301,   302,   303,   304,   305,
       0,     0,   306,     0,   307,   308,   309,     0,     0,     0,
     310,   311,   312,   313,     0,   314,   315,     0,   316,     0,
     317,   318,   319,   320,     0,   321,     0,   322,   323,     0,
     324,     0,   325,     0,     0,     0,   326,     0,   327,     0,
       0,   328,     0,   329,     0,   330,   331,   332,     0,   333,
     334,   335,     0,     0,   336,     0,   337,   338,   339,     0,
     340,     0,   341,     0,     0,     0,     0,   342,     0,   343,
     344,   345,     0,     0,   346,     0,     0,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,     0,   361,     0,   362,   363,   364,   365,   366,     0,
     367,   368,     0,     0,   369,   370,   371,     0,     0,   372,
       0,     0,     0,   373,   374,     0,     0,   375,     0,     0,
       0,   376,   377,   378,   379,   380,     0,   381,   382,   383,
       0,   384,   385,   386,   387,   388,   389,   390,     0,   391,
     392,   393,   394,   395,   396,   397,   398,     0,   399,   400,
     401,   402,   403,   404,     0,     0,   405,   406,   407,     0,
     408,   409,   410,   411,     0,   412,   413,     0,   414,   415,
     416,   417,   418,   419,   420,     0,   421,   422,     0,   423,
     424,   425,     0,   426,   427,     0,   429,     0,   430,   431,
     432,   433,     0,   434,   435,   436,   437,     0,     0,   438,
     439,   440,   441,   442,     0,     0,   443,   444,   445,   446,
     447,     0,   448,   449,     0,     0,     0,   450,   451,   452,
     453,   454,   455,     0,   456,     0,     0,   457,     0,     0,
       0,     0,     0,   693,   230,   231,   232,   233,     0,   234,
     235,     0,   236,   237,   238,     0,   239,     0,   240,  2544,
     241,   242,   243,   244,   245,   246,   694,   247,   248,   249,
     250,   251,     0,   252,   253,   254,   255,   695,   696,     0,
     256,   257,   258,     0,   259,   697,   698,     0,   260,   261,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   262,   263,   264,   265,   266,
     267,   268,     0,   269,     0,     0,     0,     0,     0,     0,
     270,   271,   272,     0,   273,   274,   275,     0,  1150,     0,
    1151,     0,   941,   278,   279,     0,   280,     0,     0,   281,
    1152,  1153,   284,     0,   285,   286,   287,   288,     0,     0,
       0,   290,   291,   292,     0,   293,   294,     0,   295,     0,
     296,   297,     0,     0,     0,     0,     0,   298,   299,   300,
     301,   302,  1154,  1155,   305,     0,     0,   306,     0,   307,
     308,   309,     0,     0,     0,   310,   954,   312,   313,     0,
     314,   315,     0,   316,     0,   317,   318,     0,   320,     0,
       0,     0,   322,  1156,     0,   324,     0,   325,     0,     0,
       0,   326,     0,   327,     0,     0,   328,     0,   329,     0,
     330,   331,   332,     0,   333,   334,   335,     0,     0,   336,
       0,   337,   338,   339,   959,  1157,     0,  1385,     0,     0,
       0,     0,   342,     0,   343,   344,   345,     0,     0,   346,
       0,     0,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   962,     0,  1158,     0,   362,
     363,   364,   365,     0,     0,   367,   368,     0,     0,     0,
    1159,   371,     0,     0,   372,     0,     0,     0,  2180,   374,
       0,     0,   375,     0,     0,     0,   376,   377,   378,   379,
     380,     0,     0,   382,   383,     0,   384,   385,   386,   387,
     388,  1160,   390,     0,   391,   392,   393,   394,   395,   396,
     397,   398,     0,   399,     0,   401,   402,   403,   404,     0,
       0,   405,   406,   407,     0,   408,  1531,   410,   411,     0,
     412,  1161,     0,   414,   415,   416,   417,   418,   419,   420,
       0,     0,   422,     0,   423,   424,   425,     0,  1162,  1163,
       0,   429,     0,   430,     0,   432,     0,     0,   434,   435,
     436,   437,     0,     0,   438,   439,   440,   441,   442,     0,
       0,   443,   444,   445,  1164,   447,     0,   448,   449,     0,
       0,     0,   450,   451,   452,   453,   454,   455,     0,   456,
      43,   852,   457,    45,     0,     0,     0,     0,    47,     0,
      48,   853,   854,   855,     0,   856,  2181,   858,   859,   860,
     861,   230,   231,   232,   233,     0,   234,   235,     0,   236,
     237,   238,     0,   239,     0,   240,     0,   241,   242,   243,
     244,   245,   246,     0,   247,   248,   249,   250,   251,     0,
     252,   253,   254,   255,     0,     0,     0,   256,   257,   258,
       0,   259,     0,     0,     0,   260,   261,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   262,   263,   264,   265,   266,   267,   268,     0,
     269,     0,     0,     0,     0,     0,     0,   270,   271,   272,
       0,   273,   274,   275,     0,   276,     0,   277,     0,     0,
     278,   279,     0,   280,     0,     0,   281,   282,   283,   284,
       0,   285,   286,   287,   288,   289,     0,     0,   290,   291,
     292,     0,   293,   294,     0,   295,     0,   296,   297,     0,
       0,     0,     0,     0,   298,   299,   300,   301,   302,   303,
     304,   305,     0,     0,   306,     0,   307,   308,   309,     0,
       0,     0,   310,   311,   312,   313,     0,   314,   315,     0,
     316,     0,   317,   318,   319,   320,     0,   321,     0,   322,
     323,     0,   324,     0,   325,     0,     0,     0,   326,     0,
     327,     0,     0,   328,     0,   329,     0,   330,   331,   332,
       0,   333,   334,   335,     0,     0,   336,     0,   337,   338,
     339,     0,   340,     0,   341,     0,     0,     0,     0,   342,
       0,   343,   344,   345,     0,     0,   346,     0,     0,   347,
     348,   349,   350,   351,   352,   353,   354,   355,   356,   357,
     358,   359,   360,     0,   361,     0,   362,   363,   364,   365,
     366,     0,   367,   368,     0,     0,   369,   370,   371,     0,
       0,   372,     0,     0,     0,   373,   374,     0,     0,   375,
       0,     0,     0,   376,   377,   378,   379,   380,     0,   381,
     382,   383,     0,   384,   385,   386,   387,   388,   389,   390,
       0,   391,   392,   393,   394,   395,   396,   397,   398,     0,
     399,   400,   401,   402,   403,   404,     0,     0,   405,   406,
     407,     0,   408,   409,   410,   411,     0,   412,   413,     0,
     414,   415,   416,   417,   418,   419,   420,     0,   421,   422,
       0,   423,   424,   425,     0,   426,   427,     0,   429,     0,
     430,   431,   432,   433,     0,   434,   435,   436,   437,     0,
       0,   438,   439,   440,   441,   442,     0,     0,   443,   444,
     445,   446,   447,     0,   448,   449,     0,     0,     0,   450,
     451,   452,   453,   454,   455,     0,   456,     0,   852,   457,
       0,     0,     0,     0,     0,     0,     0,     0,   853,   854,
     855,     0,   856,   857,   858,   859,   860,   861,   693,   230,
     231,   232,   233,     0,   234,   235,     0,   236,   237,   238,
       0,   239,     0,   240,     0,   241,   242,   243,   244,   245,
     246,   694,   247,   248,   249,   250,   251,     0,   252,   253,
     254,   255,   695,   696,     0,   256,   257,   258,     0,   259,
     697,   698,     0,   260,   261,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     262,   263,   264,   265,   266,   267,   268,     0,   269,     0,
       0,     0,     0,     0,     0,   270,   271,   272,     0,   273,
     274,   275,     0,  1150,     0,  1151,     0,   941,   278,   279,
       0,   280,     0,     0,   281,  1152,  1153,   284,     0,   285,
     286,   287,   288,     0,     0,     0,   290,   291,   292,     0,
     293,   294,     0,   295,     0,   296,   297,     0,     0,     0,
       0,     0,   298,   299,   300,   301,   302,  1154,  1155,   305,
       0,     0,   306,     0,   307,   308,   309,     0,     0,     0,
     310,   954,   312,   313,     0,   314,   315,     0,   316,     0,
     317,   318,     0,   320,     0,     0,     0,   322,  1156,     0,
     324,     0,   325,     0,     0,     0,   326,     0,   327,     0,
       0,   328,     0,   329,     0,   330,   331,   332,     0,   333,
     334,   335,     0,     0,   336,     0,   337,   338,   339,   959,
    1157,     0,  1385,     0,     0,     0,     0,   342,     0,   343,
     344,   345,     0,     0,   346,     0,     0,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     962,     0,  1158,     0,   362,   363,   364,   365,     0,     0,
     367,   368,     0,     0,     0,  1159,   371,     0,     0,   372,
       0,     0,     0,   373,   374,     0,     0,   375,     0,     0,
       0,   376,   377,   378,   379,   380,     0,     0,   382,   383,
       0,   384,   385,   386,   387,   388,  1160,   390,     0,   391,
     392,   393,   394,   395,   396,   397,   398,     0,   399,     0,
     401,   402,   403,   404,     0,     0,   405,   406,   407,     0,
     408,  1531,   410,   411,     0,   412,  1161,     0,   414,   415,
     416,   417,   418,   419,   420,     0,     0,   422,     0,   423,
     424,   425,     0,  1162,  1163,     0,   429,     0,   430,     0,
     432,     0,     0,   434,   435,   436,   437,     0,     0,   438,
     439,   440,   441,   442,     0,     0,   443,   444,   445,  1164,
     447,     0,   448,   449,     0,     0,     0,   450,   451,   452,
     453,   454,   455,     0,   456,     0,     0,   457,     0,   230,
     231,   232,   233,     0,   234,   235,     0,   236,   237,   238,
       0,   239,  1577,   240,     0,   241,   242,   243,   244,   245,
     246,     0,   247,   248,   249,   250,   251,     0,   252,   253,
     254,   255,     0,     0,     0,   256,   257,   258,     0,   259,
       0,     0,     0,   260,   261,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     262,   263,   264,   265,   266,   267,   268,     0,   269,     0,
       0,     0,     0,     0,     0,   270,   271,   272,     0,   273,
     274,   275,     0,   276,     0,   277,     0,     0,   278,   279,
       0,   280,     0,     0,   281,   282,   283,   284,     0,   285,
     286,   287,   288,   289,     0,     0,   290,   291,   292,     0,
     293,   294,     0,   295,     0,   296,   297,     0,     0,     0,
       0,     0,   298,   299,   300,   301,   302,   303,   304,   305,
       0,     0,   306,     0,   307,   308,   309,     0,     0,     0,
     310,   311,   312,   313,     0,   314,   315,     0,   316,     0,
     317,   318,   319,   320,     0,   321,     0,   322,   323,     0,
     324,     0,   325,     0,     0,     0,   326,     0,   327,     0,
       0,   328,     0,   329,     0,   330,   331,   332,     0,   333,
     334,   335,     0,     0,   336,     0,   337,   338,   339,     0,
     340,     0,   341,     0,     0,     0,     0,   342,     0,   343,
     344,   345,     0,     0,   346,     0,     0,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,     0,   361,   599,   362,   363,   364,   365,   366,     0,
     367,   368,     0,     0,   369,   370,   371,     0,     0,   372,
     600,     0,     0,   373,   374,     0,     0,   375,     0,     0,
       0,   376,   377,   378,   379,   380,     0,   381,   382,   383,
       0,   384,   385,   386,   387,   388,   389,   390,     0,   391,
     392,   393,   394,   395,   396,   397,   398,     0,   399,   400,
     401,   402,   403,   404,     0,     0,   405,   406,   407,     0,
     408,   409,   410,   411,     0,   412,   413,     0,   414,   415,
     416,   417,   418,   419,   420,     0,   421,   422,     0,   423,
     424,   425,     0,   426,   427,     0,   429,     0,   430,   431,
     432,   433,     0,   434,   435,   436,   437,     0,     0,   438,
     439,   440,   441,   442,     0,     0,   443,   444,   445,   446,
     447,     0,   448,   449,     0,     0,     0,   450,   451,   452,
     453,   454,   455,     0,   456,     0,     0,   457,     0,   230,
     231,   232,   233,     0,   234,   235,     0,   236,   237,   238,
       0,   239,   649,   240,     0,   241,   242,   243,   244,   245,
     246,     0,   247,   248,   249,   250,   251,     0,   252,   253,
     254,   255,     0,     0,     0,   256,   257,   258,     0,   259,
       0,     0,     0,   260,   261,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     262,   263,   264,   265,   266,   267,   268,     0,   269,     0,
       0,     0,     0,     0,     0,   270,   271,   272,     0,   273,
     274,   275,     0,   276,     0,   277,     0,     0,   278,   279,
       0,   280,     0,     0,   281,   282,   283,   284,     0,   285,
     286,   287,   288,   289,     0,     0,   290,   291,   292,     0,
     293,   294,     0,   295,     0,   296,   297,     0,     0,     0,
       0,     0,   298,   299,   300,   301,   302,   303,   304,   305,
       0,     0,   306,     0,   307,   308,   309,     0,     0,     0,
     310,   311,   312,   313,     0,   314,   315,     0,   316,     0,
     317,   318,   319,   320,     0,   321,     0,   322,   323,     0,
     324,     0,   325,     0,     0,     0,   326,     0,   327,     0,
       0,   328,     0,   329,     0,   330,   331,   332,     0,   333,
     334,   335,     0,     0,   336,     0,   337,   338,   339,     0,
     340,     0,   341,     0,     0,     0,     0,   342,     0,   343,
     344,   345,     0,     0,   346,     0,     0,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,     0,   361,     0,   362,   363,   364,   365,   366,     0,
     367,   368,     0,     0,   369,   370,   371,     0,     0,   372,
       0,     0,     0,   373,   374,     0,     0,   375,     0,     0,
       0,   376,   377,   378,   379,   380,     0,   381,   382,   383,
       0,   384,   385,   386,   387,   388,   389,   390,     0,   391,
     392,   393,   394,   395,   396,   397,   398,     0,   399,   400,
     401,   402,   403,   404,     0,     0,   405,   406,   407,     0,
     408,   409,   410,   411,     0,   412,   413,     0,   414,   415,
     416,   417,   418,   419,   420,     0,   421,   422,     0,   423,
     424,   425,     0,   426,   427,     0,   429,     0,   430,   431,
     432,   433,     0,   434,   435,   436,   437,     0,     0,   438,
     439,   440,   441,   442,     0,     0,   443,   444,   445,   446,
     447,     0,   448,   449,     0,     0,     0,   450,   451,   452,
     453,   454,   455,     0,   456,     0,     0,   457,     0,   230,
     231,   232,   233,     0,   234,   235,     0,   236,   237,   238,
       0,   239,  1243,   240,     0,   241,   242,   243,   244,   245,
     246,     0,   247,   248,   249,   250,   251,     0,   252,   253,
     254,   255,     0,     0,     0,   256,   257,   258,     0,   259,
       0,     0,     0,   260,   261,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     262,   263,   264,   265,   266,   267,   268,     0,   269,     0,
       0,     0,     0,     0,     0,   270,   271,   272,     0,   273,
     274,   275,     0,   276,     0,   277,     0,     0,   278,   279,
       0,   280,     0,     0,   281,   282,   283,   284,     0,   285,
     286,   287,   288,   289,     0,     0,   290,   291,   292,     0,
     293,   294,     0,   295,     0,   296,   297,     0,     0,     0,
       0,     0,   298,   299,   300,   301,   302,   303,   304,   305,
       0,     0,   306,     0,   307,   308,   309,     0,     0,     0,
     310,   311,   312,   313,     0,   314,   315,     0,   316,     0,
     317,   318,   319,   320,     0,   321,     0,   322,   323,     0,
     324,     0,   325,     0,     0,     0,   326,     0,   327,     0,
       0,   328,     0,   329,     0,   330,   331,   332,     0,   333,
     334,   335,     0,     0,   336,     0,   337,   338,   339,     0,
     340,     0,   341,     0,     0,     0,     0,   342,     0,   343,
     344,   345,     0,     0,   346,     0,     0,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,     0,   361,     0,   362,   363,   364,   365,   366,     0,
     367,   368,     0,     0,   369,   370,   371,     0,     0,   372,
       0,     0,     0,   373,   374,     0,     0,   375,     0,     0,
       0,   376,   377,   378,   379,   380,     0,   381,   382,   383,
       0,   384,   385,   386,   387,   388,   389,   390,     0,   391,
     392,   393,   394,   395,   396,   397,   398,     0,   399,   400,
     401,   402,   403,   404,     0,     0,   405,   406,   407,     0,
     408,   409,   410,   411,     0,   412,   413,     0,   414,   415,
     416,   417,   418,   419,   420,     0,   421,   422,     0,   423,
     424,   425,     0,   426,   427,     0,   429,     0,   430,   431,
     432,   433,     0,   434,   435,   436,   437,     0,     0,   438,
     439,   440,   441,   442,     0,     0,   443,   444,   445,   446,
     447,     0,   448,   449,     0,     0,     0,   450,   451,   452,
     453,   454,   455,     0,   456,     0,     0,   457,     0,   230,
     231,   232,   233,     0,   234,   235,     0,   236,   237,   238,
       0,   239,  1414,   240,     0,   241,   242,   243,   244,   245,
     246,     0,   247,   248,   249,   250,   251,     0,   252,   253,
     254,   255,     0,     0,     0,   256,   257,   258,     0,   259,
       0,     0,     0,   260,   261,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     262,   263,   264,   265,   266,   267,   268,     0,   269,     0,
       0,     0,     0,     0,     0,   270,   271,   272,     0,   273,
     274,   275,     0,   276,     0,   277,     0,     0,   278,   279,
       0,   280,     0,     0,   281,   282,   283,   284,     0,   285,
     286,   287,   288,   289,     0,     0,   290,   291,   292,     0,
     293,   294,     0,   295,     0,   296,   297,     0,     0,     0,
       0,     0,   298,   299,   300,   301,   302,   303,   304,   305,
    1398,     0,   306,     0,   307,   308,   309,     0,     0,     0,
     310,   311,   312,   313,     0,   314,   315,     0,   316,     0,
     317,   318,   319,   320,     0,   321,  1399,   322,   323,     0,
     324,     0,   325,     0,     0,     0,   326,     0,   327,     0,
       0,   328,     0,   329,     0,   330,   331,   332,     0,   333,
     334,   335,     0,     0,   336,     0,   337,   338,   339,     0,
     340,     0,   341,     0,     0,     0,     0,   342,     0,   343,
     344,   345,     0,     0,   346,     0,     0,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,     0,   361,     0,   362,   363,   364,   365,   366,     0,
     367,   368,     0,     0,   369,   370,   371,  1400,     0,   372,
       0,  1401,     0,   373,   374,     0,     0,   375,     0,     0,
       0,   376,   377,   378,   379,   380,     0,   381,   382,   383,
       0,   384,   385,   386,   387,   388,   389,   390,     0,   391,
     392,   393,   394,   395,   396,   397,   398,     0,   399,   400,
     401,   402,   403,   404,     0,     0,   405,   406,   407,     0,
     408,   409,   410,   411,     0,   412,   413,     0,   414,   415,
     416,   417,   418,   419,   420,     0,   421,   422,     0,   423,
     424,   425,     0,   426,   427,     0,   429,     0,   430,   431,
     432,   433,  1402,   434,   435,   436,   437,     0,     0,   438,
     439,   440,   441,   442,     0,     0,   443,   444,   445,   446,
     447,     0,   448,   449,     0,     0,     0,   450,   451,   452,
     453,   454,   455,     0,   456,    43,     0,   457,    45,     0,
       0,     0,     0,    47,     0,    48,     0,   230,   231,   232,
     233,  1387,   234,   235,     0,   236,   237,   238,     0,   239,
       0,   240,     0,   241,   242,   243,   244,   245,   246,     0,
     247,   248,   249,   250,   251,     0,   252,   253,   254,   255,
       0,     0,     0,   256,   257,   258,     0,   259,     0,     0,
       0,   260,   261,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   262,   263,
     264,   265,   266,   267,   268,     0,   269,     0,     0,     0,
       0,     0,     0,   270,   271,   272,     0,   273,   274,   275,
       0,   276,     0,   277,     0,     0,   278,   279,     0,   280,
       0,     0,   281,   282,   283,   284,     0,   285,   286,   287,
     288,   289,     0,     0,   290,   291,   292,     0,   293,   294,
       0,   295,     0,   296,   297,     0,     0,     0,     0,     0,
     298,   299,   300,   301,   302,   303,   304,   305,     0,     0,
     306,     0,   307,   308,   309,     0,     0,     0,   310,   311,
     312,   313,     0,   314,   315,     0,   316,     0,   317,   318,
     319,   320,     0,   321,  1399,   322,   323,     0,   324,     0,
     325,     0,     0,     0,   326,     0,   327,     0,     0,   328,
       0,   329,     0,   330,   331,   332,     0,   333,   334,   335,
       0,     0,   336,     0,   337,   338,   339,     0,   340,     0,
     341,     0,     0,     0,     0,   342,     0,   343,   344,   345,
       0,     0,   346,     0,     0,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,     0,
     361,     0,   362,   363,   364,   365,   366,     0,   367,   368,
       0,     0,   369,   370,   371,  1400,     0,   372,     0,  1401,
       0,   373,   374,     0,     0,   375,     0,     0,     0,   376,
     377,   378,   379,   380,     0,   381,   382,   383,     0,   384,
     385,   386,   387,   388,   389,   390,     0,   391,   392,   393,
     394,   395,   396,   397,   398,     0,   399,   400,   401,   402,
     403,   404,     0,     0,   405,   406,   407,     0,   408,   409,
     410,   411,     0,   412,   413,     0,   414,   415,   416,   417,
     418,   419,   420,     0,   421,   422,     0,   423,   424,   425,
       0,   426,   427,     0,   429,     0,   430,   431,   432,   433,
    1402,   434,   435,   436,   437,     0,     0,   438,   439,   440,
     441,   442,     0,     0,   443,   444,   445,   446,   447,     0,
     448,   449,     0,     0,     0,   450,   451,   452,   453,   454,
     455,     0,   456,    43,     0,   457,    45,     0,     0,     0,
       0,    47,     0,    48,     0,   230,   231,   232,   233,  1387,
     234,   235,     0,   236,   237,   238,     0,   239,     0,   240,
       0,   241,   242,   243,   244,   245,   246,     0,   247,   248,
     249,   250,   251,     0,   252,   253,   254,   255,     0,     0,
       0,   256,   257,   258,     0,   259,     0,     0,     0,   260,
     261,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   262,   565,   264,   265,
     266,   267,   268,   566,   269,     0,     0,     0,     0,     0,
       0,   270,   271,   272,     0,   567,   274,   275,     0,   276,
       0,   277,     0,     0,   278,   279,     0,   280,     0,     0,
     281,   282,   283,   284,     0,   285,   286,   287,   288,   289,
       0,     0,   290,   291,   292,     0,   293,   294,     0,   295,
       0,   296,   297,     0,     0,     0,     0,     0,   298,   299,
     300,   301,   302,   303,   304,   305,     0,     0,   306,     0,
     307,   308,   309,     0,     0,     0,   310,   311,   312,   313,
       0,   314,   315,     0,   316,     0,   317,   318,   319,   320,
       0,   321,     0,   322,   323,     0,   324,     0,   568,     0,
     569,     0,   326,     0,   327,     0,     0,   328,     0,   329,
       0,   330,   331,   332,   570,   333,   334,   335,     0,     0,
     336,     0,   337,   338,   339,     0,   340,     0,   341,     0,
       0,     0,     0,   342,     0,   343,   344,   345,     0,     0,
     346,     0,     0,   347,   348,   349,   350,   351,   352,   353,
     354,   355,   356,   357,   358,   359,   360,     0,   361,     0,
     571,   363,   364,   365,   366,     0,   367,   368,     0,     0,
     369,   370,   371,     0,     0,   372,     0,     0,     0,   373,
     374,     0,     0,   375,     0,     0,     0,   376,   377,   378,
     379,   380,     0,   381,   382,   383,     0,   572,   385,   386,
     387,   388,   389,   390,     0,   391,   573,   393,   394,   395,
     396,   397,   398,     0,   399,   400,   401,   402,   403,   404,
       0,     0,   405,   406,   407,     0,   408,   409,   410,   411,
       0,   412,   413,     0,   414,   415,   416,   417,   418,   419,
     420,     0,   421,   422,     0,   423,   424,   425,     0,   426,
     427,     0,   429,     0,   430,   431,   432,   433,     0,   434,
     435,   436,   437,     0,     0,   438,   439,   440,   441,   442,
       0,     0,   443,   444,   445,   446,   447,     0,   448,   449,
       0,     0,     0,   450,   451,   452,   453,   454,   455,     0,
     456,     0,     0,   457,    45,     0,     0,     0,     0,    47,
     230,   231,   232,   233,     0,   234,   235,   574,   236,   237,
     238,     0,   239,     0,   240,     0,   241,   242,   243,   244,
     245,   246,     0,   247,   248,   249,   250,   251,     0,   252,
     253,   254,   255,     0,     0,     0,   256,   257,   258,     0,
     259,     0,     0,     0,   260,   261,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   262,   263,   264,   265,   266,   267,   268,     0,   269,
       0,     0,     0,     0,     0,     0,   270,   271,   272,     0,
     273,   274,   275,     0,   276,     0,   277,     0,     0,   278,
     279,     0,   280,     0,     0,   281,   282,   283,   284,     0,
     285,   286,   287,   288,   289,     0,     0,   290,   291,   292,
       0,   293,   294,     0,   295,     0,   296,   297,     0,     0,
       0,     0,     0,   298,   299,   300,   301,   302,   303,   304,
     305,     0,     0,   306,     0,   307,   308,   309,     0,     0,
       0,   310,   311,   312,   313,     0,   314,   315,     0,   316,
       0,   317,   318,   319,   320,     0,   321,     0,   322,   323,
       0,   324,     0,   325,     0,     0,     0,   326,     0,   327,
       0,     0,   328,     0,   329,     0,   330,   331,   332,     0,
     333,   334,   335,     0,     0,   336,     0,   337,   338,   339,
       0,   340,     0,   341,     0,     0,     0,     0,   342,     0,
     343,   344,   345,     0,     0,   346,     0,     0,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,     0,   361,     0,   362,   363,   364,   365,   366,
       0,   367,   368,     0,     0,   369,   370,   371,     0,     0,
     372,     0,     0,     0,   373,   374,     0,     0,   375,     0,
       0,     0,   376,   377,   378,   379,   380,     0,   381,   382,
     383,     0,   384,   385,   386,   387,   388,   389,   390,     0,
     391,   392,   393,   394,   395,   396,   397,   398,     0,   399,
     400,   401,   402,   403,   404,     0,     0,   405,   406,   407,
       0,   408,   409,   410,   411,     0,   412,   413,     0,   414,
     415,   416,   417,   418,   419,   420,     0,   421,   422,     0,
     423,   424,   425,     0,   426,   427,     0,   429,     0,   430,
     431,   432,   433,     0,   434,   435,   436,   437,     0,     0,
     438,   439,   440,   441,   442,     0,     0,   443,   444,   445,
     446,   447,     0,   448,   449,     0,     0,     0,   450,   451,
     452,   453,   454,   455,     0,   456,    43,     0,   457,    45,
       0,     0,   986,   987,    47,     0,    48,   230,   231,   232,
     233,     0,   234,   235,     0,   236,   237,   238,     0,   239,
       0,   240,     0,   241,   242,   243,   244,   245,   246,     0,
     247,   248,   249,   250,   251,     0,   252,   253,   254,   255,
       0,     0,     0,   256,   257,   258,     0,   259,     0,     0,
       0,   260,   261,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   262,   263,
     264,   265,   266,   267,   268,     0,   269,     0,     0,     0,
       0,     0,     0,   270,   271,   272,     0,   273,   274,   275,
       0,   276,     0,   277,     0,     0,   278,   279,     0,   280,
       0,     0,   281,   282,   283,   284,     0,   285,   286,   287,
     288,   289,     0,     0,   290,   291,   292,     0,   293,   294,
       0,   295,     0,   296,   297,     0,     0,     0,     0,     0,
     298,   299,   300,   301,   302,   303,   304,   305,     0,     0,
     306,     0,   307,   308,   309,     0,     0,     0,   310,   311,
     312,   313,     0,   314,   315,     0,   316,     0,   317,   318,
     319,   320,     0,   321,     0,   322,   323,     0,   324,     0,
     325,     0,     0,     0,   326,     0,   327,     0,     0,   328,
       0,   329,     0,   330,   331,   332,     0,   333,   334,   335,
       0,     0,   336,     0,   337,   338,   339,     0,   340,     0,
     341,     0,     0,     0,     0,   342,     0,   343,   344,   345,
       0,     0,   346,     0,     0,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,     0,
     361,     0,   362,   363,   364,   365,   366,     0,   367,   368,
       0,     0,   369,   370,   371,     0,     0,   372,     0,     0,
       0,   373,   374,     0,     0,   375,     0,     0,     0,   376,
     377,   378,   379,   380,     0,   381,   382,   383,     0,   384,
     385,   386,   387,   388,   389,   390,     0,   391,   392,   393,
     394,   395,   396,   397,   398,     0,   399,   400,   401,   402,
     403,   404,     0,     0,   405,   406,   407,     0,   408,   409,
     410,   411,     0,   412,   413,     0,   414,   415,   416,   417,
     418,   419,   420,     0,   421,   422,     0,   423,   424,   425,
       0,   426,   427,     0,   429,     0,   430,   431,   432,   433,
       0,   434,   435,   436,   437,     0,     0,   438,   439,   440,
     441,   442,     0,     0,   443,   444,   445,   446,   447,     0,
     448,   449,     0,     0,     0,   450,   451,   452,   453,   454,
     455,     0,   456,     0,     0,   457,     0,   631,  1103,   230,
     231,   232,   233,   632,   234,   235,     0,   236,   237,   238,
       0,   239,     0,   240,     0,   241,   242,   243,   244,   245,
     246,     0,   247,   248,   249,   250,   251,     0,   252,   253,
     254,   255,     0,     0,     0,   256,   257,   258,     0,   259,
       0,     0,     0,   260,   261,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     262,   263,   264,   265,   266,   267,   268,     0,   269,     0,
       0,     0,     0,     0,     0,   270,   271,   272,     0,   273,
     274,   275,     0,   276,     0,   277,     0,     0,   278,   279,
       0,   280,     0,     0,   281,   282,   283,   284,     0,   285,
     286,   287,   288,   289,     0,     0,   290,   291,   292,     0,
     633,   294,     0,   295,     0,   296,   297,     0,     0,     0,
       0,     0,   298,   299,   300,   301,   302,   303,   304,   305,
       0,     0,   306,     0,   307,   308,   309,     0,     0,     0,
     310,   311,   312,   313,     0,   314,   315,     0,   316,     0,
     317,   318,   319,   320,     0,   321,     0,   322,   323,     0,
     324,     0,   325,     0,     0,     0,   326,     0,   327,     0,
       0,   328,     0,   329,     0,   330,   331,   332,     0,   333,
     334,   335,     0,     0,   336,     0,   337,   338,   339,     0,
     340,     0,   341,     0,     0,     0,     0,   342,     0,   343,
     344,   345,     0,     0,   346,     0,     0,   347,   348,   634,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   635,
     360,     0,   361,     0,   362,   363,   364,   365,   366,     0,
     367,   368,     0,     0,   369,   370,   371,     0,     0,   372,
       0,     0,     0,   373,   374,     0,     0,   375,     0,     0,
       0,   376,   377,   378,   379,   380,     0,   381,   382,   383,
       0,   384,   385,   386,   387,   388,   389,   390,     0,   391,
     392,   393,   394,   395,   396,   397,   398,     0,   399,   400,
     401,   402,   403,   404,     0,     0,   405,   406,   636,     0,
     408,   409,   410,   411,     0,   412,   413,     0,   414,   415,
     416,   417,   418,   419,   420,     0,   421,   422,     0,   423,
     424,   425,     0,   637,   427,     0,   429,     0,   638,   431,
     432,   433,     0,   434,   435,   436,   437,     0,     0,   438,
     439,   440,   441,   442,     0,     0,   443,   444,   445,   446,
     447,     0,   448,   449,     0,     0,     0,   450,   451,   452,
     453,   454,   455,     0,   456,     0,     0,   457,   693,   230,
     231,   232,   233,     0,   234,   235,     0,   236,   237,   238,
       0,   239,     0,   240,     0,   241,   242,   243,   244,   245,
     246,   694,   247,   248,   249,   250,   251,     0,   252,   253,
     254,   255,   695,   696,     0,   256,   257,   258,     0,   259,
     697,   698,     0,   260,   261,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     262,   263,   264,   265,   266,   267,   268,   699,   269,   700,
     701,   702,   703,   704,   705,   270,   271,   272,   706,   273,
     274,   275,   707,   276,   708,   277,   709,     0,   278,   279,
       0,   280,   710,   711,   281,   712,   283,   284,   713,   285,
     286,   287,   288,   289,   714,   715,   290,   291,   292,   716,
     293,   294,     0,   295,   717,   296,   297,   718,   719,   720,
     721,   722,   298,   299,   300,   301,   302,   303,   304,   305,
     723,   724,   306,     0,   307,   308,   309,   725,   726,   727,
     310,   311,   312,   313,   728,   314,   315,   729,   316,   730,
     317,   318,   319,   320,     0,   321,   731,   322,   323,   732,
     324,   733,   325,   734,   735,   736,   326,     0,   327,   737,
     738,   328,   739,   329,   740,   330,   331,   332,   741,   333,
     334,   335,   742,   743,   336,     0,   337,   338,   339,   744,
     340,   745,   341,   746,     0,   747,   748,   342,   749,   343,
     344,   345,   750,   751,   346,   752,   753,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,   754,   361,   755,   362,   363,   364,   365,   366,   756,
     367,   368,   757,   758,   369,   370,   371,   759,   760,   372,
     761,   762,   763,   373,   374,   764,   765,   375,   766,   767,
       0,   376,   377,   378,   379,   380,     0,   381,   382,   383,
     768,   384,   385,   386,   387,   388,   389,   390,   769,   391,
     392,   393,   394,   395,   396,   397,   398,   770,   399,   400,
     401,   402,   403,   404,     0,   771,   405,   406,   407,   772,
     408,   409,   410,   411,   773,   412,   413,   774,   414,   415,
     416,   417,   418,   419,   420,     0,   421,   422,   775,   423,
     424,   425,   776,   426,   427,   777,   429,   778,   430,   431,
     432,   433,   779,   434,   435,   436,   437,   780,   781,   438,
     439,   440,   441,   442,   782,   783,   443,   444,   445,   446,
     447,   784,   448,   449,     0,   785,   786,   450,   451,   452,
     453,   454,   455,     0,   456,     0,     0,   457,   230,   231,
     232,   233,     0,   234,   235,   469,   236,   237,   238,     0,
     239,     0,   240,     0,   241,   242,   243,   244,   245,   246,
       0,   247,   248,   249,   250,   251,     0,   252,   253,   254,
     255,     0,     0,     0,   256,   257,   258,     0,   259,     0,
       0,     0,   260,   261,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   262,
     263,   264,   265,   266,   267,   268,   470,   269,     0,     0,
       0,     0,     0,     0,   270,   271,   272,     0,   273,   274,
     275,     0,   276,     0,   277,     0,     0,   278,   279,     0,
     280,     0,     0,   281,   282,   283,   284,     0,   285,   286,
     287,   288,   289,     0,     0,   290,   291,   292,     0,   293,
     294,     0,   295,     0,   296,   297,     0,     0,     0,     0,
       0,   298,   299,   300,   301,   302,   303,   304,   305,   471,
       0,   306,     0,   307,   308,   309,     0,     0,     0,   310,
     311,   312,   313,     0,   314,   315,     0,   316,     0,   317,
     318,   319,   320,     0,   321,     0,   322,   323,     0,   324,
       0,   325,     0,     0,     0,   326,     0,   327,     0,     0,
     328,     0,   329,     0,   330,   331,   332,     0,   333,   334,
     335,     0,     0,   336,     0,   337,   338,   339,     0,   340,
       0,   341,     0,     0,     0,     0,   342,     0,   343,   344,
     345,     0,     0,   346,     0,     0,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
       0,   361,     0,   362,   363,   364,   365,   366,     0,   367,
     368,     0,     0,   369,   370,   371,     0,     0,   372,     0,
       0,     0,   373,   374,     0,     0,   375,     0,     0,     0,
     376,   377,   378,   379,   380,     0,   381,   382,   383,     0,
     384,   385,   386,   387,   388,   389,   390,     0,   391,   392,
     393,   394,   395,   396,   397,   398,     0,   399,   400,   401,
     402,   403,   404,     0,     0,   405,   406,   407,     0,   408,
     409,   410,   411,     0,   412,   413,     0,   414,   415,   416,
     417,   418,   419,   420,     0,   421,   422,     0,   423,   424,
     425,     0,   426,   427,     0,   429,     0,   430,   431,   432,
     433,     0,   434,   435,   436,   437,     0,     0,   438,   439,
     440,   441,   442,     0,     0,   443,   444,   445,   446,   447,
       0,   448,   449,     0,     0,     0,   450,   451,   452,   453,
     454,   455,     0,   456,    43,     0,   457,    45,   230,   231,
     232,   233,     0,   234,   235,     0,   236,   237,   238,     0,
     239,     0,   240,     0,   241,   242,   243,   244,   245,   246,
       0,   247,   248,   249,   250,   251,     0,   252,   253,   254,
     255,     0,     0,  1221,   256,   257,   258,     0,   259,     0,
       0,     0,   260,   261,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   262,
     263,   264,   265,   266,   267,   268,     0,   269,     0,     0,
       0,     0,     0,     0,   270,   271,   272,     0,   273,   274,
     275,     0,   276,     0,   277,     0,     0,   278,   279,     0,
     280,     0,     0,   281,   282,   283,   284,     0,   285,   286,
     287,   288,   289,     0,     0,   290,   291,   292,     0,   293,
     294,     0,   295,     0,   296,   297,     0,     0,     0,     0,
       0,   298,   299,   300,   301,   302,   303,   304,   305,     0,
       0,   306,     0,   307,   308,   309,     0,     0,     0,   310,
     311,   312,   313,     0,   314,   315,     0,   316,     0,   317,
     318,   319,   320,     0,   321,     0,   322,   323,     0,   324,
       0,   325,     0,     0,     0,   326,     0,  1766,     0,     0,
     328,     0,   329,     0,   330,   331,   332,     0,   333,   334,
     335,     0,     0,   336,     0,   337,   338,   339,     0,   340,
       0,   341,     0,     0,     0,     0,   342,     0,   343,   344,
     345,     0,     0,   346,     0,     0,   347,   348,  1767,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
       0,   361,   599,   362,   363,   364,   365,   366,     0,   367,
     368,     0,     0,   369,   370,   371,     0,     0,   372,   600,
       0,     0,   373,   374,     0,     0,   375,     0,     0,     0,
     376,   377,   378,   379,   380,     0,   381,   382,   383,     0,
     384,   385,   386,   387,   388,   389,   390,     0,   391,   392,
     393,   394,   395,   396,   397,   398,     0,   399,   400,   401,
     402,   403,   404,     0,     0,   405,   406,   407,     0,   408,
     409,   410,   411,     0,   412,   413,     0,   414,   415,   416,
     417,   418,   419,   420,     0,   421,   422,  1768,  1769,   424,
    1770,     0,   426,   427,     0,   429,     0,   430,   431,   432,
     433,     0,   434,   435,   436,   437,     0,     0,   438,   439,
     440,   441,   442,     0,     0,   443,   444,   445,   446,   447,
       0,   448,   449,     0,     0,     0,   450,   451,   452,   453,
     454,   455,     0,   456,     0,     0,   457,  1222,   230,   231,
     232,   233,     0,   234,   235,     0,   236,   237,   238,     0,
     239,     0,   240,     0,   241,   242,   243,   244,   245,   246,
       0,   247,   248,   249,   250,   251,     0,   252,   253,   254,
     255,     0,     0,     0,   256,   257,   258,     0,   259,     0,
       0,     0,   260,   261,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   262,
     263,   264,   265,   266,   267,   268,     0,   269,     0,     0,
       0,     0,     0,     0,   270,   271,   272,     0,   273,   274,
     275,     0,   276,     0,   277,     0,     0,   278,   279,     0,
     280,     0,     0,   281,   282,   283,   284,     0,   285,   286,
     287,   288,   289,     0,     0,   290,   291,   292,     0,   293,
     294,     0,   295,     0,   296,   297,     0,     0,     0,     0,
       0,   298,   299,   300,   301,   302,   303,   304,   305,     0,
       0,   306,     0,   307,   308,   309,     0,     0,     0,   310,
     311,   312,   313,     0,   314,   315,     0,   316,     0,   317,
     318,   319,   320,     0,   321,     0,   322,   323,     0,   324,
       0,   325,     0,     0,     0,   326,     0,   327,     0,     0,
     328,     0,   329,     0,   330,   331,   332,     0,   333,   334,
     335,     0,     0,   336,     0,   337,   338,   339,     0,   340,
       0,   341,     0,     0,     0,     0,   342,     0,   343,   344,
     345,     0,     0,   346,     0,     0,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
       0,   361,     0,   362,   363,   364,   365,   366,     0,   367,
     368,     0,     0,   369,   370,   371,     0,     0,   372,     0,
       0,     0,   373,   374,     0,     0,   375,     0,     0,     0,
     376,   377,   378,   379,   380,     0,   381,   382,   383,     0,
     384,   385,   386,   387,   388,   389,   390,     0,   391,   392,
     393,   394,   395,   396,   397,   398,     0,   399,   400,   401,
     402,   403,   404,     0,     0,   405,   406,   407,     0,   408,
     409,   410,   411,     0,   412,   413,     0,   414,   415,   416,
     417,   418,   419,   420,     0,   421,   422,     0,   423,   424,
     425,     0,   426,   427,   428,   429,     0,   430,   431,   432,
     433,     0,   434,   435,   436,   437,     0,     0,   438,   439,
     440,   441,   442,     0,     0,   443,   444,   445,   446,   447,
       0,   448,   449,     0,     0,     0,   450,   451,   452,   453,
     454,   455,     0,   456,    43,     0,   457,    45,   230,   231,
     232,   233,     0,   234,   235,     0,   236,   237,   238,     0,
     239,     0,   240,     0,   241,   242,   243,   244,   245,   246,
       0,   247,   248,   249,   250,   251,     0,   252,   253,   254,
     255,     0,     0,     0,   256,   257,   258,     0,   259,     0,
       0,     0,   260,   261,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   262,
     263,   264,   265,   266,   267,   268,     0,   269,     0,     0,
       0,     0,     0,     0,   270,   271,   272,     0,   273,   274,
     275,     0,   276,     0,   277,     0,     0,   278,   279,     0,
     280,     0,     0,   281,   282,   283,   284,     0,   285,   286,
     287,   288,   289,     0,     0,   290,   291,   292,     0,   293,
     294,     0,   295,     0,   296,   297,     0,     0,     0,     0,
       0,   298,   299,   300,   301,   302,   303,   304,   305,   681,
       0,   306,     0,   307,   308,   309,     0,     0,     0,   310,
     311,   312,   313,     0,   314,   315,     0,   316,     0,   317,
     318,   319,   320,     0,   321,     0,   322,   323,     0,   324,
       0,   325,     0,     0,     0,   326,     0,   327,     0,     0,
     328,     0,   329,     0,   330,   331,   332,     0,   333,   334,
     335,     0,     0,   336,     0,   337,   338,   339,     0,   340,
       0,   341,     0,     0,     0,     0,   342,     0,   343,   344,
     345,     0,     0,   346,     0,     0,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
       0,   361,     0,   362,   363,   364,   365,   366,     0,   367,
     368,     0,     0,   369,   370,   371,     0,     0,   372,     0,
       0,     0,   373,   374,     0,     0,   375,     0,     0,     0,
     376,   377,   378,   379,   380,     0,   381,   382,   383,     0,
     384,   385,   386,   387,   388,   389,   390,     0,   391,   392,
     393,   394,   395,   396,   397,   398,     0,   399,   400,   401,
     402,   403,   404,     0,     0,   405,   406,   407,     0,   408,
     409,   410,   411,     0,   412,   413,     0,   414,   415,   416,
     417,   418,   419,   420,     0,   421,   422,     0,   423,   424,
     425,     0,   426,   427,     0,   429,     0,   430,   431,   432,
     433,     0,   434,   435,   436,   437,     0,     0,   438,   439,
     440,   441,   442,     0,     0,   443,   444,   445,   446,   447,
       0,   448,   449,     0,     0,     0,   450,   451,   452,   453,
     454,   455,     0,   456,    43,     0,   457,    45,   230,   231,
     232,   233,     0,   234,   235,     0,   236,   237,   238,     0,
     239,     0,   240,     0,   241,   242,   243,   244,   245,   246,
       0,   247,   248,   249,   250,   251,     0,   252,   253,   254,
     255,     0,     0,     0,   256,   257,   258,     0,   259,     0,
       0,     0,   260,   261,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   262,
     263,   264,   265,   266,   267,   268,     0,   269,     0,     0,
       0,     0,     0,     0,   270,   271,   272,     0,   273,   274,
     275,     0,   276,     0,   277,     0,     0,   278,   279,     0,
     280,     0,     0,   281,   282,   283,   284,     0,   285,   286,
     287,   288,   289,     0,     0,   290,   291,   292,     0,   293,
     294,     0,   295,     0,   296,   297,     0,     0,     0,     0,
       0,   298,   299,   300,   301,   302,   303,   304,   305,   471,
       0,   306,     0,   307,   308,   309,     0,     0,     0,   310,
     311,   312,   313,     0,   314,   315,     0,   316,     0,   317,
     318,   319,   320,     0,   321,     0,   322,   323,     0,   324,
       0,   325,     0,     0,     0,   326,     0,   327,     0,     0,
     328,     0,   329,     0,   330,   331,   332,     0,   333,   334,
     335,     0,     0,   336,     0,   337,   338,   339,     0,   340,
       0,   341,     0,     0,     0,     0,   342,     0,   343,   344,
     345,     0,     0,   346,     0,     0,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
       0,   361,     0,   362,   363,   364,   365,   366,     0,   367,
     368,     0,     0,   369,   370,   371,     0,     0,   372,     0,
       0,     0,   373,   374,     0,     0,   375,     0,     0,     0,
     376,   377,   378,   379,   380,     0,   381,   382,   383,     0,
     384,   385,   386,   387,   388,   389,   390,     0,   391,   392,
     393,   394,   395,   396,   397,   398,     0,   399,   400,   401,
     402,   403,   404,     0,     0,   405,   406,   407,     0,   408,
     409,   410,   411,     0,   412,   413,     0,   414,   415,   416,
     417,   418,   419,   420,     0,   421,   422,     0,   423,   424,
     425,     0,   426,   427,     0,   429,     0,   430,   431,   432,
     433,     0,   434,   435,   436,   437,     0,     0,   438,   439,
     440,   441,   442,     0,     0,   443,   444,   445,   446,   447,
       0,   448,   449,     0,     0,     0,   450,   451,   452,   453,
     454,   455,     0,   456,    43,     0,   457,    45,   230,   231,
     232,   233,     0,   234,   235,     0,   236,   237,   238,     0,
     239,     0,   240,     0,   241,   242,   243,   244,   245,   246,
       0,   247,   248,   249,   250,   251,     0,   252,   253,   254,
     255,     0,     0,     0,   256,   257,   258,     0,   259,     0,
       0,     0,   260,   261,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   262,
     263,   264,   265,   266,   267,   268,     0,   269,     0,     0,
       0,     0,     0,     0,   270,   271,   272,     0,   273,   274,
     275,     0,   276,     0,   277,     0,     0,   278,   279,     0,
     280,     0,     0,   281,   282,   283,   284,     0,   285,   286,
     287,   288,   289,     0,     0,   290,   291,   292,     0,   293,
     294,     0,   295,     0,   296,   297,     0,     0,     0,     0,
       0,   298,   299,   300,   301,   302,   303,   304,   305,  1379,
       0,   306,     0,   307,   308,   309,     0,     0,     0,   310,
     311,   312,   313,     0,   314,   315,     0,   316,     0,   317,
     318,   319,   320,     0,   321,     0,   322,   323,     0,   324,
       0,   325,     0,     0,     0,   326,     0,   327,     0,     0,
     328,     0,   329,     0,   330,   331,   332,     0,   333,   334,
     335,     0,     0,   336,     0,   337,   338,   339,     0,   340,
       0,   341,     0,     0,     0,     0,   342,     0,   343,   344,
     345,     0,     0,   346,     0,     0,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
       0,   361,     0,   362,   363,   364,   365,   366,     0,   367,
     368,     0,     0,   369,   370,   371,     0,     0,   372,     0,
       0,     0,   373,   374,     0,     0,   375,     0,     0,     0,
     376,   377,   378,   379,   380,     0,   381,   382,   383,     0,
     384,   385,   386,   387,   388,   389,   390,     0,   391,   392,
     393,   394,   395,   396,   397,   398,     0,   399,   400,   401,
     402,   403,   404,     0,     0,   405,   406,   407,     0,   408,
     409,   410,   411,     0,   412,   413,     0,   414,   415,   416,
     417,   418,   419,   420,     0,   421,   422,     0,   423,   424,
     425,     0,   426,   427,     0,   429,     0,   430,   431,   432,
     433,     0,   434,   435,   436,   437,     0,     0,   438,   439,
     440,   441,   442,     0,     0,   443,   444,   445,   446,   447,
       0,   448,   449,     0,     0,     0,   450,   451,   452,   453,
     454,   455,     0,   456,    43,     0,   457,    45,   230,   231,
     232,   233,     0,   234,   235,     0,   236,   237,   238,     0,
     239,     0,   240,     0,   241,   242,   243,   244,   245,   246,
       0,   247,   248,   249,   250,   251,     0,   252,   253,   254,
     255,     0,     0,     0,   256,   257,   258,     0,   259,     0,
       0,     0,   260,   261,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   262,
     263,   264,   265,   266,   267,   268,     0,   269,     0,     0,
       0,     0,     0,     0,   270,   271,   272,     0,   273,   274,
     275,     0,   276,     0,   277,     0,     0,   278,   279,     0,
     280,     0,     0,   281,   282,   283,   284,     0,   285,   286,
     287,   288,   289,     0,     0,   290,   291,   292,     0,   293,
     294,     0,   295,     0,   296,   297,     0,     0,     0,     0,
       0,   298,   299,   300,   301,   302,   303,   304,   305,     0,
       0,   306,     0,   307,   308,   309,     0,     0,     0,   310,
     311,   312,   313,     0,   314,   315,     0,   316,     0,   317,
     318,   319,   320,     0,   321,     0,   322,   323,     0,   324,
       0,   325,     0,     0,     0,   326,     0,   327,     0,     0,
     328,     0,   329,     0,   330,   331,   332,     0,   333,   334,
     335,     0,     0,   336,     0,   337,   338,   339,     0,   340,
       0,   341,     0,     0,     0,     0,   342,     0,   343,   344,
     345,     0,     0,   346,     0,     0,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
       0,   361,     0,   362,   363,   364,   365,   366,     0,   367,
     368,     0,     0,   369,   370,   371,     0,     0,   372,     0,
       0,     0,   373,   374,     0,     0,   375,     0,     0,     0,
     376,   377,   378,   379,   380,     0,   381,   382,   383,     0,
     384,   385,   386,   387,   388,   389,   390,     0,   391,   392,
     393,   394,   395,   396,   397,   398,     0,   399,   400,   401,
     402,   403,   404,     0,     0,   405,   406,   407,     0,   408,
     409,   410,   411,     0,   412,   413,     0,   414,   415,   416,
     417,   418,   419,   420,     0,   421,   422,     0,   423,   424,
     425,     0,   426,   427,     0,   429,     0,   430,   431,   432,
     433,     0,   434,   435,   436,   437,     0,     0,   438,   439,
     440,   441,   442,     0,     0,   443,   444,   445,   446,   447,
       0,   448,   449,     0,     0,     0,   450,   451,   452,   453,
     454,   455,     0,   456,    43,     0,   457,    45,   230,   231,
     232,   233,     0,   234,   235,     0,   236,   237,   238,     0,
     239,     0,   240,     0,   241,   242,   243,   244,   245,   246,
       0,   247,   248,   249,   250,   251,     0,   252,   253,   254,
     255,     0,     0,     0,   256,   257,   258,     0,   259,     0,
       0,     0,   260,   261,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   262,
     263,   264,   265,   266,   267,   268,     0,   269,     0,     0,
       0,     0,     0,     0,   270,   271,   272,     0,   273,   274,
     275,     0,   276,     0,   277,     0,     0,   278,   279,     0,
     280,     0,     0,   281,   282,   283,   284,     0,   285,   286,
     287,   288,   289,     0,     0,   290,   291,   292,     0,   293,
     294,     0,   295,     0,   296,   297,     0,     0,     0,     0,
       0,   298,   299,   300,   301,   302,   303,   304,   305,     0,
       0,   306,     0,   307,   308,   309,     0,     0,     0,   310,
     311,   312,   313,     0,   314,   315,     0,   316,     0,   317,
     318,   319,   320,     0,   321,     0,   322,   323,     0,   324,
       0,   325,     0,     0,     0,   326,     0,   327,     0,     0,
     328,     0,   329,     0,   330,   331,   332,     0,   333,   334,
     335,     0,     0,   336,     0,   337,   338,   339,     0,   340,
       0,   341,     0,     0,     0,     0,   342,     0,   343,   344,
     345,     0,     0,   346,     0,     0,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
       0,   361,     0,   362,   363,   364,   365,   366,     0,   367,
     368,     0,     0,   369,   370,   371,     0,     0,   372,     0,
       0,     0,   373,   374,     0,     0,   375,     0,     0,     0,
     376,   377,   378,   379,   380,     0,   381,   382,   383,     0,
     384,   385,   386,   387,   388,   389,   390,     0,   391,   392,
     393,   394,   395,   396,   397,   398,     0,   399,   400,   401,
     402,   403,   404,     0,     0,   405,   406,   407,     0,   408,
     409,   410,   411,     0,   412,   413,     0,   414,   415,   416,
     417,   418,   419,   420,     0,   421,   422,     0,   423,   424,
     425,     0,   426,   427,     0,   429,     0,   430,   431,   432,
     433,     0,   434,   435,   436,   437,     0,     0,   438,   439,
     440,   441,   442,     0,     0,   443,   444,   445,   446,   447,
       0,   448,   449,     0,     0,     0,   450,   451,   452,   453,
     454,   455,     0,   456,     0,     0,   457,   677,   230,   231,
     232,   233,     0,   234,   235,     0,   236,   237,   238,     0,
     239,     0,   240,     0,   241,   242,   243,   244,   245,   246,
       0,   247,   248,   249,   250,   251,     0,   252,   253,   254,
     255,     0,     0,     0,   256,   257,   258,     0,   259,     0,
       0,     0,   260,   261,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   262,
     263,   264,   265,   266,   267,   268,     0,   269,     0,     0,
       0,     0,     0,     0,   270,   271,   272,     0,   273,   274,
     275,     0,   276,     0,   277,     0,     0,   278,   279,     0,
     280,     0,     0,   281,   282,   283,   284,     0,   285,   286,
     287,   288,   289,     0,     0,   290,   291,   292,     0,   293,
     294,     0,   295,     0,   296,   297,     0,     0,     0,     0,
       0,   298,   299,   300,   301,   302,   303,   304,   305,     0,
       0,   306,     0,   307,   308,   309,     0,     0,     0,   310,
     311,   312,   313,     0,   314,   315,     0,   316,     0,   317,
     318,   319,   320,     0,   321,     0,   322,   323,     0,   324,
       0,   325,     0,     0,     0,   326,     0,   327,     0,     0,
     328,     0,   329,     0,   330,   331,   332,     0,   333,   334,
     335,     0,     0,   336,     0,   337,   338,   339,     0,   340,
       0,   341,     0,     0,     0,     0,   342,     0,   343,   344,
     345,     0,     0,   346,     0,     0,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
       0,   361,     0,   362,   363,   364,   365,   366,     0,   367,
     368,     0,     0,   369,   370,   371,     0,     0,   372,     0,
       0,     0,   373,   374,     0,     0,   375,     0,     0,     0,
     376,   377,   378,   379,   380,     0,   381,   382,   383,     0,
     384,   385,   386,   387,   388,   389,   390,     0,   391,   392,
     393,   394,   395,   396,   397,   398,     0,   399,   400,   401,
     402,   403,   404,     0,     0,   405,   406,   407,     0,   408,
     409,   410,   411,     0,   412,   413,     0,   414,   415,   416,
     417,   418,   419,   420,     0,   421,   422,     0,   423,   424,
     425,     0,   426,   427,     0,   429,     0,   430,   431,   432,
     433,     0,   434,   435,   436,   437,     0,     0,   438,   439,
     440,   441,   442,     0,     0,   443,   444,   445,   446,   447,
       0,   448,   449,     0,     0,     0,   450,   451,   452,   453,
     454,   455,     0,   456,     0,     0,   457,  1697,   693,   230,
     231,   232,   233,     0,   234,   235,     0,   236,   237,   238,
       0,   239,     0,   240,     0,   241,   242,   243,   244,   245,
     246,   694,   247,   248,   249,   250,   251,     0,   252,   253,
     254,   255,   695,   696,     0,   256,   257,   258,     0,   259,
     697,   698,     0,   260,   261,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     262,   263,   264,   265,   266,   267,   268,     0,   269,     0,
       0,     0,     0,     0,     0,   270,   271,   272,     0,   273,
     274,   275,     0,  1150,     0,  1151,     0,   941,   278,   279,
       0,   280,     0,     0,   281,  1152,  1153,   284,     0,   285,
     286,   287,   288,     0,     0,     0,   290,   291,   292,     0,
     293,   294,     0,   295,     0,   296,   297,     0,     0,     0,
       0,     0,   298,   299,   300,   301,   302,  1154,  1155,   305,
       0,     0,   306,     0,   307,   308,   309,     0,     0,     0,
     310,   954,   312,   313,     0,   314,   315,     0,   316,     0,
     317,   318,     0,   320,     0,     0,     0,   322,  1156,     0,
     324,     0,   325,     0,     0,     0,   326,     0,   327,     0,
       0,   328,     0,   329,     0,   330,   331,   332,     0,   333,
     334,   335,     0,     0,   336,     0,   337,   338,   339,   959,
    1157,     0,  1385,     0,     0,     0,     0,   342,     0,   343,
     344,   345,     0,     0,   346,     0,     0,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     962,     0,  1158,     0,   362,   363,   364,   365,  1593,     0,
     367,   368,     0,     0,     0,  1159,   371,     0,     0,   372,
       0,     0,     0,   373,   374,     0,     0,   375,     0,     0,
       0,   376,   377,   378,   379,   380,     0,     0,   382,   383,
       0,   384,   385,   386,   387,   388,  1160,   390,     0,   391,
     392,   393,   394,   395,   396,   397,   398,     0,   399,     0,
     401,   402,   403,   404,     0,     0,   405,   406,   407,     0,
     408,  1531,   410,   411,     0,   412,  1161,     0,   414,   415,
     416,   417,   418,   419,   420,     0,     0,   422,     0,   423,
     424,   425,     0,  1162,  1163,     0,   429,     0,   430,     0,
     432,     0,     0,   434,   435,   436,   437,     0,     0,   438,
     439,   440,   441,   442,     0,     0,   443,   444,   445,  1164,
     447,     0,   448,   449,     0,     0,     0,   450,   451,   452,
     453,   454,   455,     0,   456,     0,     0,   457,   693,   230,
     231,   232,   233,     0,   234,   235,     0,   236,   237,   238,
       0,   239,     0,   240,     0,   241,   242,   243,   244,   245,
     246,   694,   247,   248,   249,   250,   251,     0,   252,   253,
     254,   255,   695,   696,     0,   256,   257,   258,     0,   259,
     697,   698,     0,   260,   261,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     262,   263,   264,   265,   266,   267,   268,     0,   269,     0,
       0,     0,     0,     0,     0,   270,   271,   272,     0,   273,
     274,   275,     0,  1150,     0,  1151,     0,   941,   278,   279,
       0,   280,     0,     0,   281,  1152,  1153,   284,     0,   285,
     286,   287,   288,     0,     0,     0,   290,   291,   292,     0,
     293,   294,     0,   295,     0,   296,   297,     0,     0,     0,
       0,     0,   298,   299,   300,   301,   302,  1154,  1155,   305,
       0,     0,   306,     0,   307,   308,   309,     0,     0,     0,
     310,   954,   312,   313,     0,   314,   315,     0,   316,     0,
     317,   318,     0,   320,     0,     0,     0,   322,  1156,     0,
     324,     0,   325,     0,     0,     0,   326,     0,   327,     0,
       0,   328,     0,   329,     0,   330,   331,   332,  1581,   333,
     334,   335,     0,     0,  1582,     0,   337,   338,   339,   959,
    1157,     0,  1385,     0,     0,     0,     0,   342,     0,   343,
     344,   345,     0,     0,   346,     0,     0,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     962,     0,  1158,     0,   362,   363,   364,   365,     0,     0,
     367,   368,     0,     0,     0,  1159,   371,     0,     0,   372,
       0,     0,     0,   373,   374,     0,     0,  1583,     0,     0,
       0,   376,   377,   378,   379,   380,     0,     0,   382,   383,
       0,   384,   385,   386,   387,   388,  1160,   390,     0,   391,
     392,   393,   394,   395,   396,   397,   398,     0,   399,     0,
     401,   402,   403,   404,     0,     0,   405,   406,   407,     0,
     408,  1531,   410,   411,     0,   412,  1161,     0,   414,   415,
     416,   417,   418,   419,   420,     0,     0,   422,     0,   423,
     424,   425,     0,  1162,  1163,     0,   429,     0,   430,     0,
     432,     0,     0,   434,   435,   436,   437,     0,     0,   438,
     439,   440,   441,   442,     0,     0,   443,   444,   445,  1164,
     447,     0,   448,   449,     0,     0,     0,   450,   451,   452,
     453,   454,   455,     0,   456,     0,     0,   457,   693,   230,
     231,   232,   233,     0,   234,   235,     0,   236,   237,   238,
       0,   239,     0,   240,     0,   241,   242,   243,   244,   245,
     246,   694,   247,   248,   249,   250,   251,     0,   252,   253,
     254,   255,   695,   696,     0,   256,   257,   258,     0,   259,
     697,   698,     0,   260,   261,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     262,   263,   264,   265,   266,   267,   268,     0,   269,     0,
       0,     0,     0,     0,     0,   270,   271,   272,     0,   273,
     274,   275,     0,  1150,     0,  1151,     0,   941,   278,   279,
       0,   280,     0,     0,   281,  1152,  1153,   284,     0,   285,
     286,   287,   288,     0,     0,     0,   290,   291,   292,     0,
     293,   294,     0,   295,     0,   296,   297,     0,     0,     0,
       0,     0,   298,   299,   300,   301,   302,  1154,  1155,   305,
       0,     0,   306,     0,   307,   308,   309,     0,     0,     0,
     310,   954,   312,   313,     0,   314,   315,     0,   316,     0,
     317,   318,     0,   320,     0,     0,     0,   322,  1156,     0,
     324,     0,   325,     0,     0,     0,   326,     0,   327,     0,
       0,   328,     0,   329,     0,   330,   331,   332,     0,   333,
     334,   335,     0,     0,   336,     0,   337,   338,   339,   959,
    1157,     0,  1385,     0,     0,     0,     0,   342,     0,   343,
     344,   345,     0,     0,   346,     0,     0,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     962,     0,  1158,     0,   362,   363,   364,   365,  2257,     0,
     367,   368,     0,     0,     0,  1159,   371,     0,     0,   372,
       0,     0,     0,   373,   374,     0,     0,   375,     0,     0,
       0,   376,   377,   378,   379,   380,     0,     0,   382,   383,
       0,   384,   385,   386,   387,   388,  1160,   390,     0,   391,
     392,   393,   394,   395,   396,   397,   398,     0,   399,     0,
     401,   402,   403,   404,     0,     0,   405,   406,   407,     0,
     408,  1531,   410,   411,     0,   412,  1161,     0,   414,   415,
     416,   417,   418,   419,   420,     0,     0,   422,     0,   423,
     424,   425,     0,  1162,  1163,     0,   429,     0,   430,     0,
     432,     0,     0,   434,   435,   436,   437,     0,     0,   438,
     439,   440,   441,   442,     0,     0,   443,   444,   445,  1164,
     447,     0,   448,   449,     0,     0,     0,   450,   451,   452,
     453,   454,   455,     0,   456,     0,     0,   457,   693,   230,
     231,   232,   233,     0,   234,   235,     0,   236,   237,   238,
       0,   239,     0,   240,     0,   241,   242,   243,   244,   245,
     246,   694,   247,   248,   249,   250,   251,     0,   252,   253,
     254,   255,   695,   696,     0,   256,   257,   258,     0,   259,
     697,   698,     0,   260,   261,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     262,   263,   264,   265,   266,   267,   268,     0,   269,     0,
       0,     0,     0,     0,     0,   270,   271,   272,     0,   273,
     274,   275,     0,  1150,     0,  1151,     0,   941,   278,   279,
       0,   280,     0,     0,   281,  1152,  1153,   284,     0,   285,
     286,   287,   288,     0,     0,     0,   290,   291,   292,     0,
     293,   294,     0,   295,     0,   296,   297,     0,     0,     0,
       0,     0,   298,   299,   300,   301,   302,  1154,  1155,   305,
       0,     0,   306,     0,   307,   308,   309,     0,     0,     0,
     310,   954,   312,   313,     0,   314,   315,     0,   316,     0,
     317,   318,     0,   320,     0,     0,     0,   322,  1156,     0,
     324,     0,   325,     0,     0,     0,   326,     0,   327,     0,
       0,   328,     0,   329,     0,   330,   331,   332,     0,   333,
     334,   335,     0,     0,   336,     0,   337,   338,   339,   959,
    1157,     0,  1385,     0,     0,     0,     0,   342,     0,   343,
     344,   345,     0,     0,   346,     0,     0,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     962,     0,  1158,     0,   362,   363,   364,   365,     0,     0,
     367,   368,     0,     0,     0,  1159,   371,     0,     0,   372,
       0,     0,     0,   373,   374,     0,     0,   375,     0,     0,
       0,   376,   377,   378,   379,   380,     0,     0,   382,   383,
       0,   384,   385,   386,   387,   388,  1160,   390,     0,   391,
     392,   393,   394,   395,   396,   397,   398,     0,   399,     0,
     401,   402,   403,   404,     0,     0,   405,   406,   407,     0,
     408,  1531,   410,   411,     0,   412,  1161,     0,   414,   415,
     416,   417,   418,   419,   420,     0,     0,   422,     0,   423,
     424,   425,     0,  1162,  1163,     0,   429,     0,   430,     0,
     432,     0,     0,   434,   435,   436,   437,     0,     0,   438,
     439,   440,   441,   442,     0,     0,   443,   444,   445,  1164,
     447,     0,   448,   449,     0,     0,     0,   450,   451,   452,
     453,   454,   455,     0,   456,     0,     0,   457,   693,   230,
     231,   232,   233,     0,   234,   235,     0,   236,   237,   238,
       0,   239,     0,   240,     0,   241,   242,   243,   244,   245,
     246,   694,   247,   248,   249,   250,   251,     0,   252,   253,
     254,   255,   695,   696,     0,   256,   257,   258,     0,   259,
     697,   698,     0,   260,   261,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     262,   263,   264,   265,   266,   267,   268,     0,   269,     0,
       0,     0,     0,     0,     0,   270,   271,   272,     0,   273,
     274,   275,     0,  1150,     0,  1151,     0,   941,   278,   279,
       0,   280,     0,     0,   281,  1152,  1153,   284,     0,   285,
     286,   287,   288,     0,     0,     0,   290,   291,   292,     0,
     293,   294,     0,   295,     0,   296,   297,     0,     0,     0,
       0,     0,   298,   299,   300,   301,   302,  1154,  1155,   305,
       0,     0,   306,     0,   307,   308,   309,     0,     0,     0,
     310,   954,   312,   313,     0,   314,   315,     0,   316,     0,
     317,   318,     0,   320,     0,     0,     0,   322,  1156,     0,
     324,     0,   325,     0,     0,     0,   326,     0,   327,     0,
       0,   328,     0,   329,     0,   330,   331,   332,     0,   333,
     334,   335,     0,     0,   336,     0,   337,   338,   339,   959,
    1157,     0,  1385,     0,     0,     0,     0,   342,     0,   343,
     344,   345,     0,     0,   346,     0,     0,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     962,     0,  1158,     0,   362,   363,   364,   365,     0,     0,
     367,   368,     0,     0,     0,  1159,   371,     0,     0,   372,
       0,     0,     0,   373,   374,     0,     0,   375,     0,     0,
       0,   376,   377,   378,   379,   380,     0,     0,   382,   383,
       0,   384,   385,   386,   387,   388,  1160,   390,     0,   391,
     392,   393,   394,   395,   396,   397,   398,     0,   399,     0,
     401,   402,   403,   404,     0,     0,   405,   406,   407,     0,
     408,     0,   410,   411,     0,   412,  1161,     0,   414,   415,
     416,   417,   418,   419,   420,     0,     0,   422,     0,   423,
     424,   425,     0,  1162,  1163,     0,   429,     0,   430,     0,
     432,     0,     0,   434,   435,   436,   437,     0,     0,   438,
     439,   440,   441,   442,     0,     0,   443,   444,   445,  1164,
     447,     0,   448,   449,     0,     0,     0,   450,   451,   452,
     453,   454,   455,     0,   456,     0,     0,   457,   693,   230,
     231,   232,   233,     0,   234,   235,     0,   236,   237,   238,
       0,   239,     0,   240,     0,   241,   242,   243,   244,   245,
     246,   694,   247,   248,   249,   250,   251,     0,   252,   253,
     254,   255,   695,   696,     0,   256,   257,   258,     0,   259,
     697,   698,     0,   260,   261,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     262,   263,   264,   265,   266,   267,   268,     0,   269,     0,
       0,     0,     0,     0,     0,   270,   271,   272,     0,   273,
     274,   275,     0,  1150,     0,  1151,     0,   941,   278,   279,
       0,   280,     0,     0,   281,  1152,  1153,   284,     0,   285,
     286,   287,   288,     0,     0,     0,   290,   291,   292,     0,
     293,   294,     0,   295,     0,   296,   297,     0,     0,     0,
       0,     0,   298,   299,   300,   301,   302,  1154,  1155,   305,
       0,     0,   306,     0,   307,   308,   309,     0,     0,     0,
     310,   954,   312,   313,     0,   314,   315,     0,   316,     0,
     317,   318,     0,   320,     0,     0,     0,   322,  1156,     0,
     324,     0,   325,     0,     0,     0,   326,     0,   327,     0,
       0,   328,     0,   329,     0,   330,   331,   332,     0,   333,
     334,   335,     0,     0,   336,     0,   337,   338,   339,   959,
    1157,     0,     0,     0,     0,     0,     0,   342,     0,   343,
     344,   345,     0,     0,   346,     0,     0,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     962,     0,  1158,     0,   362,   363,   364,   365,     0,     0,
     367,   368,     0,     0,     0,  1159,   371,     0,     0,   372,
       0,     0,     0,   373,   374,     0,     0,   375,     0,     0,
       0,   376,   377,   378,   379,   380,     0,     0,   382,   383,
       0,   384,   385,   386,   387,   388,  1160,   390,     0,   391,
     392,   393,   394,   395,   396,   397,   398,     0,   399,     0,
     401,   402,   403,   404,     0,     0,   405,   406,   407,     0,
     408,     0,   410,   411,     0,   412,  1161,     0,   414,   415,
     416,   417,   418,   419,   420,     0,     0,   422,     0,   423,
     424,   425,     0,  1162,  1163,     0,   429,     0,   430,     0,
     432,     0,     0,   434,   435,   436,   437,     0,     0,   438,
     439,   440,   441,   442,     0,     0,   443,   444,   445,  1164,
     447,     0,   448,   449,     0,     0,     0,   450,   451,   452,
     453,   454,   455,     0,   456,     0,     0,   457,   230,   231,
     232,   233,     0,   234,   235,     0,   236,   237,   238,     0,
     239,  1081,   240,     0,   241,   242,   243,   244,   245,   246,
       0,   247,   248,   249,   250,   251,     0,   252,   253,   254,
     255,     0,  1082,     0,   256,   257,   258,     0,   259,  1083,
    1084,     0,   260,   261,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   262,
     263,   264,   265,   266,   267,   268,   699,   269,   700,   701,
     702,   703,   704,   705,   270,   271,   272,   706,   273,   274,
     275,   707,   276,   708,   277,   709,     0,   278,   279,     0,
     280,   710,   711,   281,     0,   283,   284,   713,   285,   286,
     287,   288,   289,   714,   715,   290,   291,   292,   716,   293,
     294,     0,   295,   717,   296,   297,   718,   719,   720,   721,
     722,   298,   299,   300,   301,   302,   303,   304,   305,   723,
     724,   306,     0,   307,   308,   309,   725,   726,   727,   310,
     311,   312,   313,   728,   314,   315,   729,   316,   730,   317,
     318,   319,   320,     0,   321,   731,   322,   323,   732,   324,
     733,   325,   734,   735,   736,   326,     0,   327,   737,   738,
     328,   739,   329,   740,   330,   331,   332,   741,   333,   334,
     335,   742,   743,   336,     0,   337,   338,   339,     0,   340,
     745,   341,   746,     0,   747,   748,   342,   749,   343,   344,
     345,   750,   751,   346,   752,   753,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
     754,   361,   755,   362,   363,   364,   365,   366,   756,   367,
     368,   757,   758,   369,   370,   371,   759,   760,   372,   761,
     762,   763,   373,   374,   764,   765,   375,   766,   767,     0,
     376,   377,   378,   379,   380,     0,   381,   382,   383,   768,
     384,   385,   386,   387,   388,   389,   390,   769,   391,   392,
     393,   394,   395,   396,   397,   398,   770,   399,   400,   401,
     402,   403,   404,     0,   771,   405,   406,   407,   772,   408,
     409,   410,   411,   773,   412,   413,   774,   414,   415,   416,
     417,   418,   419,   420,     0,   421,   422,   775,   423,   424,
     425,   776,   426,   427,   777,   429,   778,   430,   431,   432,
     433,   779,   434,   435,   436,   437,  1085,   781,   438,   439,
     440,   441,   442,   782,   783,   443,   444,   445,   446,   447,
     784,   448,   449,     0,   785,   786,   450,   451,   452,   453,
     454,   455,     0,   456,     0,     0,   457,   230,   231,   232,
     233,     0,   234,   235,     0,   236,   237,   238,     0,   239,
       0,   240,     0,   241,   242,   243,   244,   245,   246,     0,
     247,   248,   249,   250,   251,     0,   252,   253,   254,   255,
       0,     0,     0,   256,   257,   258,     0,   259,     0,     0,
       0,   260,   261,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   262,   263,
     264,   265,   266,   267,   268,   699,   269,   700,   701,   702,
     703,   704,   705,   270,   271,   272,   706,   273,   274,   275,
     707,   276,   708,   277,   709,     0,   278,   279,     0,   280,
     710,   711,   281,     0,   283,   284,   713,   285,   286,   287,
     288,   289,   714,   715,   290,   291,   292,   716,   293,   294,
       0,   295,   717,   296,   297,   718,   719,   720,   721,   722,
     298,   299,   300,   301,   302,   303,   304,   305,   723,   724,
     306,     0,   307,   308,   309,   725,   726,   727,   310,   311,
     312,   313,   728,   314,   315,   729,   316,   730,   317,   318,
     319,   320,     0,   321,   731,   322,   323,   732,   324,   733,
     325,   734,   735,   736,   326,     0,   327,   737,   738,   328,
     739,   329,   740,   330,   331,   332,   741,   333,   334,   335,
     742,   743,   336,     0,   337,   338,   339,     0,   340,   745,
     341,   746,     0,   747,   748,   342,   749,   343,   344,   345,
     750,   751,   346,   752,   753,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   754,
     361,   755,   362,   363,   364,   365,   366,   756,   367,   368,
     757,   758,   369,   370,   371,   759,   760,   372,   761,   762,
     763,   373,   374,   764,   765,   375,   766,   767,     0,   376,
     377,   378,   379,   380,     0,   381,   382,   383,   768,   384,
     385,   386,   387,   388,   389,   390,   769,   391,   392,   393,
     394,   395,   396,   397,   398,   770,   399,   400,   401,   402,
     403,   404,     0,   771,   405,   406,   407,   772,   408,   409,
     410,   411,   773,   412,   413,   774,   414,   415,   416,   417,
     418,   419,   420,     0,   421,   422,   775,   423,   424,   425,
     776,   426,   427,   777,   429,   778,   430,   431,   432,   433,
     779,   434,   435,   436,   437,     0,   781,   438,   439,   440,
     441,   442,   782,   783,   443,   444,   445,   446,   447,   784,
     448,   449,     0,   785,   786,   450,   451,   452,   453,   454,
     455,     0,   456,     0,     0,   457,   230,   231,   232,   233,
       0,   234,   235,     0,   236,   237,   238,     0,   239,     0,
     240,     0,   241,   242,   243,   244,   245,   246,     0,   247,
     248,   249,   250,   251,     0,   252,   253,   254,   255,     0,
       0,     0,   256,   257,   258,     0,   259,     0,     0,     0,
     260,   261,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   262,   263,   264,
     265,   266,   267,   268,     0,   269,     0,     0,     0,     0,
       0,     0,   270,   271,   272,   706,   273,   274,   275,   707,
     276,   708,   277,     0,     0,   278,   279,     0,   280,     0,
       0,   281,   282,   283,   284,     0,   285,   286,   287,   288,
     289,     0,     0,   290,   291,   292,     0,   293,   294,     0,
     295,     0,   296,   297,   718,     0,     0,     0,     0,   298,
     299,   300,   301,   302,   303,   304,   305,     0,     0,   306,
       0,   307,   308,   309,     0,     0,     0,   310,   311,   312,
     313,     0,   314,   315,     0,   316,     0,   317,   318,   319,
     320,     0,   321,     0,   322,   323,     0,   324,     0,   325,
     734,     0,   736,   326,     0,   327,     0,     0,   328,     0,
     329,   740,   330,   331,   332,   741,   333,   334,   335,     0,
     743,   336,     0,   337,   338,   339,     0,   340,     0,   341,
       0,     0,   747,   748,   342,   749,   343,   344,   345,     0,
     751,   346,   752,     0,   347,   348,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,   360,   754,   361,
     599,   362,   363,   364,   365,   366,     0,   367,   368,   757,
       0,   369,   370,   371,     0,     0,   372,   600,     0,     0,
     373,   374,     0,     0,   375,   766,   767,     0,   376,   377,
     378,   379,   380,     0,   381,   382,   383,     0,   384,   385,
     386,   387,   388,   389,   390,     0,   391,   392,   393,   394,
     395,   396,   397,   398,   770,   399,   400,   401,   402,   403,
     404,     0,     0,   405,   406,   407,     0,   408,   409,   410,
     411,   773,   412,   413,     0,   414,   415,   416,   417,   418,
     419,   420,     0,   421,   422,     0,   423,   424,   425,     0,
     426,   427,     0,   429,     0,   430,   431,   432,   433,     0,
     434,   435,   436,   437,     0,     0,   438,   439,   440,   441,
     442,     0,     0,   443,   444,   445,   446,   447,   784,   448,
     449,     0,     0,     0,   450,   451,   452,   453,   454,   455,
       0,   456,     0,     0,   457,   230,   231,   232,   233,     0,
     234,   235,     0,   236,   237,   238,     0,   239,     0,   240,
       0,   241,   242,   243,   244,   245,   246,     0,   247,   248,
     249,   250,   251,     0,   252,   253,   254,   255,     0,     0,
       0,   256,   257,   258,     0,   259,     0,     0,     0,   260,
     261,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   262,   263,   264,   265,
     266,   267,   268,     0,   269,     0,     0,     0,     0,     0,
       0,   270,   271,   272,     0,   273,   274,   275,     0,   276,
       0,   277,     0,     0,   278,   279,     0,   280,     0,     0,
     281,   282,   283,   284,  1480,   285,   286,   287,   288,   289,
       0,     0,   290,   291,   292,  1482,   293,   294,     0,   295,
       0,   296,   297,     0,     0,     0,     0,     0,   298,   299,
     300,   301,   302,   303,   304,   305,     0,     0,   306,     0,
     307,   308,   309,     0,     0,     0,   310,   311,   312,   313,
       0,   314,   315,     0,   316,     0,   317,   318,   319,   320,
       0,   321,     0,   322,   323,     0,   324,  1483,   325,     0,
       0,     0,   326,     0,   327,     0,     0,   328,     0,   329,
       0,   330,   331,   332,     0,   333,   334,   335,     0,     0,
     336,     0,   337,   338,   339,     0,   340,     0,   341,     0,
       0,     0,     0,   342,     0,   343,   344,   345,     0,     0,
     346,  1915,     0,   347,   348,   349,   350,   351,   352,   353,
     354,   355,   356,   357,   358,   359,   360,     0,   361,     0,
     362,   363,   364,   365,   366,     0,   367,   368,     0,     0,
     369,   370,   371,     0,     0,   372,     0,     0,     0,   373,
     374,     0,     0,   375,     0,     0,     0,   376,   377,   378,
     379,   380,     0,   381,   382,   383,  1484,   384,   385,   386,
     387,   388,   389,   390,     0,   391,   392,   393,   394,   395,
     396,   397,   398,     0,   399,   400,   401,   402,   403,   404,
       0,     0,   405,   406,   407,     0,   408,   409,   410,   411,
       0,   412,   413,     0,   414,   415,   416,   417,   418,   419,
     420,     0,   421,   422,     0,   423,   424,   425,     0,   426,
     427,     0,   429,     0,   430,   431,   432,   433,     0,   434,
     435,   436,   437,     0,  1485,   438,   439,   440,   441,   442,
       0,     0,   443,   444,   445,   446,   447,     0,   448,   449,
       0,     0,     0,   450,   451,   452,   453,   454,   455,     0,
     456,     0,     0,   457,   230,   231,   232,   233,     0,   234,
     235,     0,   236,   237,   238,     0,   239,     0,   240,     0,
     241,   242,   243,   244,   245,   246,     0,   247,   248,   249,
     250,   251,     0,   252,   253,   254,   255,     0,     0,     0,
     256,   257,   258,     0,   259,     0,     0,     0,   260,   261,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   262,   263,   264,   265,   266,
     267,   268,     0,   269,    84,    85,     0,     0,     0,     0,
     270,   271,   272,     0,   273,   274,   275,     0,   276,     0,
     277,     0,     0,   278,   279,     0,   280,     0,     0,   281,
     282,   283,   284,     0,   285,   286,   287,   288,   289,     0,
       0,   290,   291,   292,     0,   293,   294,     0,   295,     0,
     296,   297,     0,     0,     0,     0,     0,   298,   299,   300,
     301,   302,   303,   304,   305,     0,     0,   306,     0,   307,
     308,   309,     0,     0,     0,   310,   311,   312,   313,     0,
     314,   315,     0,   316,     0,   317,   318,   319,   320,     0,
     321,     0,   322,   323,     0,   324,     0,   325,     0,     0,
       0,   326,     0,   327,     0,     0,   328,     0,   329,     0,
     330,   331,   332,     0,   333,   334,   335,     0,     0,   336,
       0,   337,   338,   339,     0,   340,     0,   341,     0,     0,
       0,     0,   342,     0,   343,   344,   345,     0,     0,   346,
       0,     0,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,     0,   361,   599,   362,
     363,   364,   365,   366,     0,   367,   368,     0,     0,   369,
     370,   371,     0,     0,   372,   600,     0,     0,   373,   374,
       0,     0,   375,     0,     0,     0,   376,   377,   378,   379,
     380,     0,   381,   382,   383,     0,   384,   385,   386,   387,
     388,   389,   390,     0,   391,   392,   393,   394,   395,   396,
     397,   398,     0,   399,   400,   401,   402,   403,   404,     0,
       0,   405,   406,   407,     0,   408,   409,   410,   411,     0,
     412,   413,     0,   414,   415,   416,   417,   418,   419,   420,
       0,   421,   422,     0,   423,   424,   425,     0,   426,   427,
       0,   429,     0,   430,   431,   432,   433,     0,   434,   435,
     436,   437,     0,     0,   438,   439,   440,   441,   442,     0,
       0,   443,   444,   445,   446,   447,     0,   448,   449,     0,
       0,     0,   450,   451,   452,   453,   454,   455,     0,   456,
       0,     0,   457,   230,   231,   232,   233,     0,   234,   235,
       0,   236,   237,   238,     0,   239,     0,   240,     0,   241,
     242,   243,   244,   245,   246,     0,   247,   248,   249,   250,
     251,     0,   252,   253,   254,   255,     0,     0,     0,   256,
     257,   258,     0,   259,     0,     0,     0,   260,   261,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   262,   263,   264,   265,   266,   267,
     268,     0,   269,     0,     0,     0,     0,     0,     0,   270,
     271,   272,     0,   273,   274,   275,     0,   276,     0,   277,
       0,     0,   278,   279,     0,   280,     0,     0,   281,   282,
     283,   284,     0,   285,   286,   287,   288,   289,     0,     0,
     290,   291,   292,     0,   293,   294,     0,   295,     0,   296,
     297,     0,     0,     0,     0,     0,   298,   299,   300,   301,
     302,   303,   304,   305,     0,     0,   306,     0,   307,   308,
     309,     0,     0,     0,   310,   311,   312,   313,     0,   314,
     315,     0,   316,     0,   317,   318,   319,   320,     0,   321,
       0,   322,   323,     0,   324,     0,   325,     0,     0,     0,
     326,     0,   327,     0,     0,   328,     0,   329,     0,   330,
     331,   332,     0,   333,   334,   335,     0,     0,   336,     0,
     337,   338,   339,     0,   340,     0,   341,     0,     0,     0,
       0,   342,     0,   343,   344,   345,     0,     0,   346,     0,
       0,   347,   348,   349,   350,   351,   352,   353,   354,   355,
     356,   357,   358,   359,   360,     0,   361,   599,   362,   363,
     364,   365,   366,     0,   367,   368,     0,     0,   369,   370,
     371,     0,     0,   372,   600,     0,   651,   373,   374,     0,
       0,   375,     0,     0,     0,   376,   377,   378,   379,   380,
       0,   381,   382,   383,     0,   384,   385,   386,   387,   388,
     389,   390,     0,   391,   392,   393,   394,   395,   396,   397,
     398,     0,   399,   400,   401,   402,   403,   404,     0,     0,
     405,   406,   407,     0,   408,   409,   410,   411,     0,   412,
     413,     0,   414,   415,   416,   417,   418,   419,   420,     0,
     421,   422,     0,   423,   424,   425,     0,   426,   427,     0,
     429,     0,   430,   431,   432,   433,     0,   434,   435,   436,
     437,     0,     0,   438,   439,   440,   441,   442,     0,     0,
     443,   444,   445,   446,   447,     0,   448,   449,     0,     0,
       0,   450,   451,   452,   453,   454,   455,     0,   456,     0,
       0,   457,   230,   231,   232,   233,     0,   234,   235,     0,
     236,   237,   238,     0,   239,     0,   240,     0,   241,   242,
     243,   244,   245,   246,     0,   247,   248,   249,   250,   251,
       0,   252,   253,   254,   255,     0,     0,     0,   256,   257,
     258,     0,   259,     0,     0,     0,   260,   261,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   262,   263,   264,   265,   266,   267,   268,
       0,   269,     0,     0,     0,     0,     0,     0,   270,   271,
     272,     0,   273,   274,   275,     0,   276,     0,   277,     0,
       0,   278,   279,     0,   280,     0,     0,   281,   282,   283,
     284,     0,   285,   286,   287,   288,   289,     0,     0,   290,
     291,   292,     0,   293,   294,     0,   295,     0,   296,   297,
       0,     0,     0,     0,     0,   298,   299,  1230,   301,   302,
     303,   304,   305,     0,     0,   306,     0,   307,   308,   309,
       0,     0,     0,   310,   311,   312,   313,     0,   314,   315,
       0,   316,     0,   317,   318,   319,   320,     0,   321,     0,
     322,   323,     0,   324,     0,   325,     0,     0,     0,  1231,
       0,   327,     0,     0,   328,     0,   329,     0,   330,   331,
     332,     0,   333,   334,   335,     0,     0,   336,     0,   337,
     338,   339,     0,   340,     0,   341,     0,     0,     0,     0,
     342,     0,   343,   344,  1232,     0,     0,   346,     0,     0,
     347,   348,   349,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,   360,     0,   361,   599,   362,   363,   364,
     365,   366,     0,   367,   368,     0,     0,   369,   370,   371,
       0,     0,   372,   600,     0,     0,   373,   374,     0,     0,
     375,     0,     0,     0,   376,   377,   378,   379,   380,     0,
     381,   382,   383,     0,   384,   385,   386,   387,   388,   389,
     390,     0,   391,   392,   393,   394,   395,   396,   397,   398,
       0,   399,   400,   401,  1233,   403,   404,     0,     0,   405,
     406,   407,     0,   408,   409,   410,   411,     0,   412,   413,
       0,   414,   415,   416,   417,   418,   419,   420,     0,   421,
     422,  1234,   423,   424,   425,     0,   426,   427,     0,   429,
       0,   430,   431,   432,   433,     0,   434,   435,   436,   437,
       0,     0,   438,   439,   440,   441,   442,     0,     0,   443,
     444,   445,   446,   447,     0,   448,   449,     0,     0,     0,
     450,   451,   452,   453,   454,   455,     0,   456,     0,     0,
     457,   230,   231,   232,   233,     0,   234,   235,     0,   236,
     237,   238,     0,   239,     0,   240,     0,   241,   242,   243,
     244,   245,   246,     0,   247,   248,   249,   250,   251,     0,
     252,   253,   254,   255,     0,     0,     0,   256,   257,   258,
       0,   259,     0,     0,     0,   260,   261,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   262,   263,   264,   265,   266,   267,   268,     0,
     269,     0,     0,     0,     0,     0,     0,   270,   271,   272,
       0,   273,   274,   275,     0,   276,     0,   277,     0,     0,
     278,   279,     0,   280,     0,     0,   281,   282,   283,   284,
       0,   285,   286,   287,   288,   289,     0,     0,   290,   291,
     292,     0,   293,   294,     0,   295,     0,   296,   297,     0,
       0,     0,     0,     0,   298,   299,   300,   301,   302,   303,
     304,   305,     0,     0,   306,     0,   307,   308,   309,     0,
       0,     0,   310,   311,   312,   313,     0,   314,   315,     0,
     316,     0,   317,   318,   319,   320,     0,   321,     0,   322,
     323,     0,   324,     0,   325,     0,     0,     0,   326,     0,
     327,     0,     0,   328,     0,   329,     0,   330,   331,   332,
       0,   333,   334,   335,     0,     0,   336,     0,   337,   338,
     339,     0,   340,     0,   341,     0,     0,     0,     0,   342,
       0,   343,   344,   345,     0,     0,   346,     0,     0,   347,
     348,   349,   350,   351,   352,   353,   354,   355,   356,   357,
     358,   359,   360,     0,   361,   599,   362,   363,   364,   365,
     366,     0,   367,   368,     0,     0,   369,   370,   371,     0,
       0,   372,   600,     0,     0,   373,   374,     0,     0,   375,
       0,     0,     0,   376,   377,   378,   379,   380,     0,   381,
     382,   383,     0,   384,   385,   386,   387,   388,   389,   390,
       0,   391,   392,   393,   394,   395,   396,   397,   398,     0,
     399,   400,   401,   402,   403,   404,     0,     0,   405,   406,
     407,     0,   408,   409,   410,   411,     0,   412,   413,     0,
     414,   415,   416,   417,   418,   419,   420,     0,   421,   422,
       0,   423,   424,   425,     0,   426,   427,     0,   429,     0,
     430,   431,   432,   433,     0,   434,   435,   436,   437,     0,
       0,   438,   439,   440,   441,   442,     0,  2676,   443,   444,
     445,   446,   447,     0,   448,   449,     0,     0,     0,   450,
     451,   452,   453,   454,   455,     0,   456,     0,     0,   457,
     230,   231,   232,   233,     0,   234,   235,     0,   236,   237,
     238,     0,   239,     0,   240,     0,   241,   242,   243,   244,
     245,   246,     0,   247,   248,   249,   250,   251,     0,   252,
     253,   254,   255,     0,     0,     0,   256,   257,   258,     0,
     259,     0,     0,     0,   260,   261,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   262,   263,   264,   265,   266,   267,   268,     0,   269,
       0,     0,     0,     0,     0,     0,   270,   271,   272,     0,
     273,   274,   275,     0,   276,     0,   277,     0,     0,   278,
     279,     0,   280,     0,     0,   281,   282,   283,   284,     0,
     285,   286,   287,   288,   289,     0,     0,   290,   291,   292,
       0,   293,   294,     0,   295,     0,   296,   297,     0,     0,
       0,     0,     0,   298,   299,   300,   301,   302,   303,   304,
     305,     0,     0,   306,     0,   307,   308,   309,     0,     0,
       0,   310,   311,   312,   313,     0,   314,   315,     0,   316,
       0,   317,   318,   319,   320,     0,   321,     0,   322,   323,
       0,   324,     0,   325,     0,     0,     0,   326,     0,   327,
       0,     0,   328,     0,   329,     0,   330,   331,   332,     0,
     333,   334,   335,     0,     0,   336,     0,   337,   338,   339,
       0,   340,     0,   341,     0,     0,     0,     0,   342,     0,
     343,   344,   345,     0,     0,   346,     0,     0,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,     0,   361,   599,   362,   363,   364,   365,   366,
       0,   367,   368,     0,     0,   369,   370,   371,     0,     0,
     372,   600,     0,     0,   373,   374,     0,     0,   375,     0,
       0,     0,   376,   377,   378,   379,   380,     0,   381,   382,
     383,     0,   384,   385,   386,   387,   388,   389,   390,     0,
     391,   392,   393,   394,   395,   396,   397,   398,     0,   399,
     400,   401,   402,   403,   404,     0,     0,   405,   406,   407,
       0,   408,   409,   410,   411,     0,   412,   413,     0,   414,
     415,   416,   417,   418,   419,   420,     0,   421,   422,     0,
     423,   424,   425,     0,   426,   427,     0,   429,     0,   430,
     431,   432,   433,     0,   434,   435,   436,   437,     0,     0,
     438,   439,   440,   441,   442,     0,     0,   443,   444,   445,
     446,   447,     0,   448,   449,     0,     0,     0,   450,   451,
     452,   453,   454,   455,     0,   456,     0,     0,   457,   230,
     231,   232,   233,     0,   234,   235,     0,   236,   237,   238,
       0,   239,     0,   240,     0,   241,   242,   243,   244,   245,
     246,     0,   247,   248,   249,   250,   251,     0,   252,   253,
     254,   255,     0,     0,     0,   256,   257,   258,     0,   259,
       0,     0,     0,   260,   261,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     262,   263,   264,   265,   266,   267,   268,   620,   269,     0,
       0,     0,     0,     0,     0,   270,   271,   272,     0,   273,
     274,   275,     0,   276,     0,   277,     0,     0,   278,   279,
       0,   280,     0,     0,   281,   282,   283,   284,     0,   285,
     286,   287,   288,   289,     0,     0,   290,   291,   292,     0,
     293,   294,     0,   295,     0,   296,   297,     0,     0,     0,
       0,     0,   298,   299,   300,   301,   302,   303,   304,   305,
       0,     0,   306,     0,   307,   308,   309,     0,     0,     0,
     310,   311,   312,   313,     0,   314,   315,     0,   316,     0,
     317,   318,   319,   320,     0,   321,     0,   322,   323,     0,
     324,     0,   325,     0,     0,     0,   326,     0,   327,     0,
       0,   328,     0,   329,     0,   330,   331,   332,     0,   333,
     334,   335,     0,     0,   336,     0,   337,   338,   339,     0,
     340,     0,   341,     0,     0,     0,     0,   342,     0,   343,
     344,   345,     0,     0,   346,     0,     0,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,     0,   361,     0,   362,   363,   364,   365,   366,     0,
     367,   368,     0,     0,   369,   370,   371,     0,     0,   372,
       0,     0,     0,   373,   374,     0,     0,   375,     0,     0,
       0,   376,   377,   378,   379,   380,     0,   381,   382,   383,
       0,   384,   385,   386,   387,   388,   389,   390,     0,   391,
     392,   393,   394,   395,   396,   397,   398,     0,   399,   400,
     401,   402,   403,   404,     0,     0,   405,   406,   621,     0,
     408,   409,   410,   411,     0,   412,   413,     0,   414,   415,
     416,   417,   418,   419,   420,     0,   421,   422,     0,   423,
     424,   425,     0,   622,   427,     0,   429,     0,   623,   431,
     432,   433,     0,   434,   435,   436,   437,     0,     0,   438,
     439,   440,   441,   442,     0,     0,   443,   444,   445,   446,
     447,     0,   448,   449,     0,     0,     0,   450,   451,   452,
     453,   454,   455,     0,   456,     0,     0,   457,   230,   231,
     232,   233,     0,   234,   235,     0,   236,   237,   238,     0,
     239,     0,   240,     0,   241,   242,   243,   244,   245,   246,
       0,   247,   248,   249,   250,   251,     0,   252,   253,   254,
     255,     0,     0,     0,   256,   257,   258,     0,   259,     0,
       0,     0,   260,   261,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   262,
     263,   264,   265,   266,   267,   268,   641,   269,     0,     0,
       0,     0,     0,     0,   270,   271,   272,     0,   273,   274,
     275,     0,   276,     0,   277,     0,     0,   278,   279,     0,
     280,     0,     0,   281,   282,   283,   284,     0,   285,   286,
     287,   288,   289,     0,     0,   290,   291,   292,     0,   293,
     294,     0,   295,     0,   296,   297,     0,     0,     0,     0,
       0,   298,   299,   300,   301,   302,   303,   304,   305,     0,
       0,   306,     0,   307,   308,   309,     0,     0,     0,   310,
     311,   312,   313,     0,   314,   315,     0,   316,     0,   317,
     318,   319,   320,     0,   321,     0,   322,   323,     0,   324,
       0,   325,     0,     0,     0,   326,     0,   327,     0,     0,
     328,     0,   329,     0,   330,   331,   332,     0,   333,   334,
     335,     0,     0,   336,     0,   337,   338,   339,     0,   340,
       0,   341,     0,     0,     0,     0,   342,     0,   343,   344,
     345,     0,     0,   346,     0,     0,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
       0,   361,     0,   362,   363,   364,   365,   366,     0,   367,
     368,     0,     0,   369,   370,   371,     0,     0,   372,     0,
       0,     0,   373,   374,     0,     0,   375,     0,     0,     0,
     376,   377,   378,   379,   380,     0,   381,   382,   383,     0,
     384,   385,   386,   387,   388,   389,   390,     0,   391,   392,
     393,   394,   395,   396,   397,   398,     0,   399,   400,   401,
     402,   403,   404,     0,     0,   405,   406,   642,     0,   408,
     409,   410,   411,     0,   412,   413,     0,   414,   415,   416,
     417,   418,   419,   420,     0,   421,   422,     0,   423,   424,
     425,     0,   643,   427,     0,   429,     0,   644,   431,   432,
     433,     0,   434,   435,   436,   437,     0,     0,   438,   439,
     440,   441,   442,     0,     0,   443,   444,   445,   446,   447,
       0,   448,   449,     0,     0,     0,   450,   451,   452,   453,
     454,   455,     0,   456,     0,     0,   457,   230,   231,   232,
     233,     0,   234,   235,     0,   236,   237,   238,     0,   239,
       0,   240,     0,   241,   242,   243,   244,   245,   246,     0,
     247,   248,   249,   250,   251,     0,   252,   253,   254,   255,
       0,     0,     0,   256,   257,   258,     0,   259,     0,     0,
       0,   260,   261,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   262,   263,
     264,   265,   266,   267,   268,  1038,   269,     0,     0,     0,
       0,     0,     0,   270,   271,   272,     0,   273,   274,   275,
       0,   276,     0,   277,     0,     0,   278,   279,     0,   280,
       0,     0,   281,   282,   283,   284,     0,   285,   286,   287,
     288,   289,     0,     0,   290,   291,   292,     0,   293,   294,
       0,   295,     0,   296,   297,     0,     0,     0,     0,     0,
     298,   299,   300,   301,   302,   303,   304,   305,     0,     0,
     306,     0,   307,   308,   309,     0,     0,     0,   310,   311,
     312,   313,     0,   314,   315,     0,   316,     0,   317,   318,
     319,   320,     0,   321,     0,   322,   323,     0,   324,     0,
     325,     0,     0,     0,   326,     0,   327,     0,     0,   328,
       0,   329,     0,   330,   331,   332,     0,   333,   334,   335,
       0,     0,   336,     0,   337,   338,   339,     0,   340,     0,
     341,     0,     0,     0,     0,   342,     0,   343,   344,   345,
       0,     0,   346,     0,     0,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,     0,
     361,     0,   362,   363,   364,   365,   366,     0,   367,   368,
       0,     0,   369,   370,   371,     0,     0,   372,     0,     0,
       0,   373,   374,     0,     0,   375,     0,     0,     0,   376,
     377,   378,   379,   380,     0,   381,   382,   383,     0,   384,
     385,   386,   387,   388,   389,   390,     0,   391,   392,   393,
     394,   395,   396,   397,   398,     0,   399,   400,   401,   402,
     403,   404,     0,     0,   405,   406,   407,     0,   408,   409,
     410,   411,     0,   412,   413,     0,   414,   415,   416,   417,
     418,   419,   420,     0,   421,   422,     0,   423,   424,   425,
       0,   426,   427,     0,   429,     0,   430,   431,   432,   433,
       0,   434,   435,   436,   437,     0,     0,   438,   439,   440,
     441,   442,     0,     0,   443,   444,   445,   446,   447,     0,
     448,   449,     0,     0,     0,   450,   451,   452,   453,   454,
     455,     0,   456,     0,     0,   457,   230,   231,   232,   233,
       0,   234,   235,     0,   236,   237,   238,     0,   239,     0,
     240,     0,   241,   242,   243,   244,   245,   246,     0,   247,
     248,   249,   250,   251,     0,   252,   253,   254,   255,     0,
       0,     0,   256,   257,   258,     0,   259,     0,     0,     0,
     260,   261,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   262,   263,   264,
     265,   266,   267,   268,     0,   269,     0,     0,     0,     0,
       0,     0,   270,   271,   272,  1047,   273,   274,   275,     0,
     276,     0,   277,     0,     0,   278,   279,     0,   280,     0,
       0,   281,   282,   283,  1048,     0,   285,   286,   287,   288,
     289,     0,     0,   290,   291,   292,     0,   293,   294,     0,
     295,     0,   296,   297,     0,     0,     0,     0,     0,   298,
     299,   300,   301,   302,   303,   304,   305,     0,     0,   306,
       0,   307,   308,   309,     0,     0,     0,   310,   311,   312,
     313,     0,   314,   315,     0,   316,     0,   317,   318,   319,
     320,     0,   321,     0,   322,   323,     0,   324,     0,   325,
       0,     0,     0,   326,     0,   327,     0,     0,   328,     0,
     329,     0,   330,   331,   332,     0,   333,   334,   335,     0,
       0,   336,     0,   337,   338,   339,     0,   340,     0,   341,
       0,     0,     0,     0,   342,     0,   343,   344,   345,     0,
       0,   346,     0,     0,   347,   348,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   635,   360,     0,   361,
       0,   362,   363,   364,   365,   366,     0,   367,   368,     0,
       0,   369,   370,   371,     0,     0,   372,     0,     0,     0,
     373,   374,     0,     0,   375,     0,     0,     0,   376,   377,
     378,   379,   380,     0,   381,   382,   383,     0,   384,   385,
     386,   387,   388,   389,   390,     0,   391,   392,   393,   394,
     395,   396,   397,   398,     0,   399,   400,   401,   402,   403,
     404,     0,     0,   405,   406,  1042,     0,   408,   409,   410,
     411,     0,   412,   413,     0,   414,   415,   416,   417,   418,
     419,   420,     0,   421,   422,     0,   423,   424,   425,     0,
     637,   427,     0,   429,     0,   638,   431,   432,   433,     0,
     434,   435,   436,   437,     0,     0,   438,   439,   440,   441,
     442,     0,     0,   443,   444,   445,   446,   447,     0,   448,
     449,     0,     0,     0,   450,   451,   452,   453,   454,   455,
       0,   456,     0,     0,   457,   230,   231,   232,   233,     0,
     234,   235,     0,   236,   237,   238,     0,   239,     0,   240,
       0,   241,   242,   243,   244,   245,   246,     0,   247,   248,
     249,   250,   251,     0,   252,   253,   254,   255,     0,     0,
       0,   256,   257,   258,     0,   259,     0,     0,     0,   260,
     261,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   262,   263,   264,   265,
     266,   267,   268,     0,   269,     0,     0,     0,     0,     0,
       0,   270,   271,   272,     0,   273,   274,   275,     0,   276,
       0,   277,     0,     0,   278,   279,     0,   280,     0,     0,
     281,   282,   283,   284,     0,   285,   286,   287,   288,   289,
       0,     0,   290,   291,   292,     0,   293,   294,     0,   295,
       0,   296,   297,     0,     0,     0,     0,     0,   298,   299,
     300,   301,   302,   303,   304,   305,     0,     0,   306,     0,
     307,   308,   309,     0,     0,     0,   310,   311,   312,   313,
       0,   314,   315,     0,   316,     0,   317,   318,   319,   320,
       0,   321,     0,   322,   323,     0,   324,     0,   325,     0,
       0,     0,   326,     0,   327,     0,  1963,   328,     0,   329,
       0,   330,   331,   332,     0,   333,   334,   335,     0,     0,
     336,     0,   337,   338,   339,     0,   340,     0,   341,     0,
       0,     0,     0,   342,     0,   343,   344,   345,     0,     0,
     346,     0,     0,   347,   348,   349,   350,   351,   352,   353,
     354,   355,   356,   357,   358,   359,   360,     0,   361,     0,
     362,   363,   364,   365,   366,     0,   367,   368,     0,     0,
     369,   370,   371,     0,     0,   372,     0,     0,     0,   373,
     374,     0,     0,   375,     0,     0,     0,   376,   377,   378,
     379,   380,     0,   381,   382,   383,     0,   384,   385,   386,
     387,   388,   389,   390,     0,   391,   392,   393,   394,   395,
     396,   397,   398,     0,   399,   400,   401,   402,   403,   404,
       0,     0,   405,   406,   407,     0,   408,   409,   410,   411,
       0,   412,   413,     0,   414,   415,   416,   417,   418,   419,
     420,     0,   421,   422,     0,   423,   424,   425,     0,   426,
     427,     0,   429,     0,   430,   431,   432,   433,     0,   434,
     435,   436,   437,     0,     0,   438,   439,   440,   441,   442,
       0,     0,   443,   444,   445,   446,   447,     0,   448,   449,
       0,     0,     0,   450,   451,   452,   453,   454,   455,     0,
     456,     0,     0,   457,   230,   231,   232,   233,     0,   234,
     235,     0,   236,   237,   238,     0,   239,     0,   240,     0,
     241,   242,   243,   244,   245,   246,     0,   247,   248,   249,
     250,   251,     0,   252,   253,   254,   255,     0,     0,     0,
     256,   257,   258,     0,   259,     0,     0,     0,   260,   261,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   262,   263,   264,   265,   266,
     267,   268,     0,   269,     0,     0,     0,     0,     0,     0,
     270,   271,   272,     0,   273,   274,   275,     0,   276,     0,
     277,     0,     0,   278,   279,     0,   280,     0,     0,   281,
     282,   283,   284,     0,   285,   286,   287,   288,   289,     0,
       0,   290,   291,   292,     0,   293,   294,     0,   295,     0,
     296,   297,     0,     0,     0,     0,     0,   298,   299,   300,
     301,   302,   303,   304,   305,  2198,     0,   306,     0,   307,
     308,   309,     0,     0,     0,   310,   311,   312,   313,     0,
     314,   315,     0,   316,     0,   317,   318,   319,   320,     0,
     321,     0,   322,   323,     0,   324,     0,   325,     0,     0,
       0,   326,     0,   327,     0,     0,   328,     0,   329,     0,
     330,   331,   332,     0,   333,   334,   335,     0,     0,   336,
       0,   337,   338,   339,     0,   340,     0,   341,     0,     0,
       0,     0,   342,     0,   343,   344,   345,     0,     0,   346,
       0,     0,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,     0,   361,     0,   362,
     363,   364,   365,   366,     0,   367,   368,     0,     0,   369,
     370,   371,     0,     0,   372,     0,     0,     0,   373,   374,
       0,     0,   375,     0,     0,     0,   376,   377,   378,   379,
     380,     0,   381,   382,   383,     0,   384,   385,   386,   387,
     388,   389,   390,     0,   391,   392,   393,   394,   395,   396,
     397,   398,     0,   399,   400,   401,   402,   403,   404,     0,
       0,   405,   406,   407,     0,   408,   409,   410,   411,     0,
     412,   413,     0,   414,   415,   416,   417,   418,   419,   420,
       0,   421,   422,     0,   423,   424,   425,     0,   426,   427,
       0,   429,     0,   430,   431,   432,   433,     0,   434,   435,
     436,   437,     0,     0,   438,   439,   440,   441,   442,     0,
       0,   443,   444,   445,   446,   447,     0,   448,   449,     0,
       0,     0,   450,   451,   452,   453,   454,   455,     0,   456,
       0,     0,   457,   230,   231,   232,   233,     0,   234,   235,
       0,   236,   237,   238,     0,   239,     0,   240,     0,   241,
     242,   243,   244,   245,   246,     0,   247,   248,   249,   250,
     251,     0,   252,   253,   254,   255,     0,     0,     0,   256,
     257,   258,     0,   259,     0,     0,     0,   260,   261,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   262,   263,   264,   265,   266,   267,
     268,     0,   269,     0,     0,     0,     0,     0,     0,   270,
     271,   272,     0,   273,   274,   275,     0,   276,     0,   277,
       0,     0,   278,   279,     0,   280,     0,     0,   281,   282,
     283,   284,     0,   285,   286,   287,   288,   289,     0,     0,
     290,   291,   292,     0,   293,   294,     0,   295,     0,   296,
     297,     0,     0,     0,     0,     0,   298,   299,   300,   301,
     302,   303,   304,   305,  2200,     0,   306,     0,   307,   308,
     309,     0,     0,     0,   310,   311,   312,   313,     0,   314,
     315,     0,   316,     0,   317,   318,   319,   320,     0,   321,
       0,   322,   323,     0,   324,     0,   325,     0,     0,     0,
     326,     0,   327,     0,     0,   328,     0,   329,     0,   330,
     331,   332,     0,   333,   334,   335,     0,     0,   336,     0,
     337,   338,   339,     0,   340,     0,   341,     0,     0,     0,
       0,   342,     0,   343,   344,   345,     0,     0,   346,     0,
       0,   347,   348,   349,   350,   351,   352,   353,   354,   355,
     356,   357,   358,   359,   360,     0,   361,     0,   362,   363,
     364,   365,   366,     0,   367,   368,     0,     0,   369,   370,
     371,     0,     0,   372,     0,     0,     0,   373,   374,     0,
       0,   375,     0,     0,     0,   376,   377,   378,   379,   380,
       0,   381,   382,   383,     0,   384,   385,   386,   387,   388,
     389,   390,     0,   391,   392,   393,   394,   395,   396,   397,
     398,     0,   399,   400,   401,   402,   403,   404,     0,     0,
     405,   406,   407,     0,   408,   409,   410,   411,     0,   412,
     413,     0,   414,   415,   416,   417,   418,   419,   420,     0,
     421,   422,     0,   423,   424,   425,     0,   426,   427,     0,
     429,     0,   430,   431,   432,   433,     0,   434,   435,   436,
     437,     0,     0,   438,   439,   440,   441,   442,     0,     0,
     443,   444,   445,   446,   447,     0,   448,   449,     0,     0,
       0,   450,   451,   452,   453,   454,   455,     0,   456,     0,
       0,   457,   230,   231,   232,   233,     0,   234,   235,     0,
     236,   237,   238,     0,   239,     0,   240,     0,   241,   242,
     243,   244,   245,   246,     0,   247,   248,   249,   250,   251,
       0,   252,   253,   254,   255,     0,     0,     0,   256,   257,
     258,     0,   259,     0,     0,     0,   260,   261,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   262,   263,   264,   265,   266,   267,   268,
       0,   269,     0,     0,     0,     0,  2356,     0,   270,   271,
     272,     0,   273,   274,   275,     0,   276,     0,   277,     0,
       0,   278,   279,     0,   280,     0,     0,   281,   282,   283,
     284,     0,   285,   286,   287,   288,   289,     0,     0,   290,
     291,   292,     0,   293,   294,     0,   295,     0,   296,   297,
       0,     0,     0,     0,     0,   298,   299,   300,   301,   302,
     303,   304,   305,     0,     0,   306,     0,   307,   308,   309,
       0,     0,     0,   310,   311,   312,   313,     0,   314,   315,
       0,   316,     0,   317,   318,   319,   320,     0,   321,     0,
     322,   323,     0,   324,     0,   325,     0,     0,     0,   326,
       0,   327,     0,     0,   328,     0,   329,     0,   330,   331,
     332,     0,   333,   334,   335,     0,     0,   336,     0,   337,
     338,   339,     0,   340,     0,   341,     0,     0,     0,     0,
     342,     0,   343,   344,   345,     0,     0,   346,     0,     0,
     347,   348,   349,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,   360,     0,   361,     0,   362,   363,   364,
     365,   366,     0,   367,   368,     0,     0,   369,   370,   371,
       0,     0,   372,     0,     0,     0,   373,   374,     0,     0,
     375,     0,     0,     0,   376,   377,   378,   379,   380,     0,
     381,   382,   383,     0,   384,   385,   386,   387,   388,   389,
     390,     0,   391,   392,   393,   394,   395,   396,   397,   398,
       0,   399,   400,   401,   402,   403,   404,     0,     0,   405,
     406,   407,     0,   408,   409,   410,   411,     0,   412,   413,
       0,   414,   415,   416,   417,   418,   419,   420,     0,   421,
     422,     0,   423,   424,   425,     0,   426,   427,     0,   429,
       0,   430,   431,   432,   433,     0,   434,   435,   436,   437,
       0,     0,   438,   439,   440,   441,   442,     0,     0,   443,
     444,   445,   446,   447,     0,   448,   449,     0,     0,     0,
     450,   451,   452,   453,   454,   455,     0,   456,     0,     0,
     457,   230,   231,   232,   233,     0,   234,   235,     0,   236,
     237,   238,     0,   239,     0,   240,     0,   241,   242,   243,
     244,   245,   246,     0,   247,   248,   249,   250,   251,     0,
     252,   253,   254,   255,     0,     0,     0,   256,   257,   258,
       0,   259,     0,     0,     0,   260,   261,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   262,   263,   264,   265,   266,   267,   268,     0,
     269,     0,     0,     0,     0,  2370,     0,   270,   271,   272,
       0,   273,   274,   275,     0,   276,     0,   277,     0,     0,
     278,   279,     0,   280,     0,     0,   281,   282,   283,   284,
       0,   285,   286,   287,   288,   289,     0,     0,   290,   291,
     292,     0,   293,   294,     0,   295,     0,   296,   297,     0,
       0,     0,     0,     0,   298,   299,   300,   301,   302,   303,
     304,   305,     0,     0,   306,     0,   307,   308,   309,     0,
       0,     0,   310,   311,   312,   313,     0,   314,   315,     0,
     316,     0,   317,   318,   319,   320,     0,   321,     0,   322,
     323,     0,   324,     0,   325,     0,     0,     0,   326,     0,
     327,     0,     0,   328,     0,   329,     0,   330,   331,   332,
       0,   333,   334,   335,     0,     0,   336,     0,   337,   338,
     339,     0,   340,     0,   341,     0,     0,     0,     0,   342,
       0,   343,   344,   345,     0,     0,   346,     0,     0,   347,
     348,   349,   350,   351,   352,   353,   354,   355,   356,   357,
     358,   359,   360,     0,   361,     0,   362,   363,   364,   365,
     366,     0,   367,   368,     0,     0,   369,   370,   371,     0,
       0,   372,     0,     0,     0,   373,   374,     0,     0,   375,
       0,     0,     0,   376,   377,   378,   379,   380,     0,   381,
     382,   383,     0,   384,   385,   386,   387,   388,   389,   390,
       0,   391,   392,   393,   394,   395,   396,   397,   398,     0,
     399,   400,   401,   402,   403,   404,     0,     0,   405,   406,
     407,     0,   408,   409,   410,   411,     0,   412,   413,     0,
     414,   415,   416,   417,   418,   419,   420,     0,   421,   422,
       0,   423,   424,   425,     0,   426,   427,     0,   429,     0,
     430,   431,   432,   433,     0,   434,   435,   436,   437,     0,
       0,   438,   439,   440,   441,   442,     0,     0,   443,   444,
     445,   446,   447,     0,   448,   449,     0,     0,     0,   450,
     451,   452,   453,   454,   455,     0,   456,     0,     0,   457,
     230,   231,   232,   233,     0,   234,   235,     0,   236,   237,
     238,     0,   239,     0,   240,     0,   241,   242,   243,   244,
     245,   246,     0,   247,   248,   249,   250,   251,     0,   252,
     253,   254,   255,     0,     0,     0,   256,   257,   258,     0,
     259,     0,     0,     0,   260,   261,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   262,   263,   264,   265,   266,   267,   268,     0,   269,
       0,     0,     0,     0,     0,     0,   270,   271,   272,     0,
     273,   274,   275,     0,   276,     0,   277,     0,     0,   278,
     279,     0,   280,     0,     0,   281,   282,   283,   284,     0,
     285,   286,   287,   288,   289,     0,     0,   290,   291,   292,
       0,   293,   294,     0,   295,     0,   296,   297,     0,     0,
       0,     0,     0,   298,   299,   300,   301,   302,   303,   304,
     305,     0,     0,   306,     0,   307,   308,   309,     0,     0,
       0,   310,   311,   312,   313,     0,   314,   315,     0,   316,
       0,   317,   318,   319,   320,     0,   321,     0,   322,   323,
       0,   324,     0,   325,     0,     0,     0,   326,     0,   327,
       0,     0,   328,     0,   329,     0,   330,   331,   332,     0,
     333,   334,   335,     0,     0,   336,     0,   337,   338,   339,
       0,   340,     0,   341,     0,     0,     0,     0,   342,     0,
     343,   344,   345,     0,     0,   346,     0,     0,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,     0,   361,     0,   362,   363,   364,   365,   366,
       0,   367,   368,     0,     0,   369,   370,   371,     0,     0,
     372,     0,     0,     0,   373,   374,     0,     0,   375,     0,
       0,     0,   376,   377,   378,   379,   380,     0,   381,   382,
     383,     0,   384,   385,   386,   387,   388,   389,   390,     0,
     391,   392,   393,   394,   395,   396,   397,   398,     0,   399,
     400,   401,   402,   403,   404,     0,     0,   405,   406,   407,
       0,   408,   409,   410,   411,     0,   412,   413,     0,   414,
     415,   416,   417,   418,   419,   420,     0,   421,   422,     0,
     423,   424,   425,     0,   426,   427,     0,   429,     0,   430,
     431,   432,   433,     0,   434,   435,   436,   437,     0,     0,
     438,   439,   440,   441,   442,     0,     0,   443,   444,   445,
     446,   447,     0,   448,   449,     0,     0,     0,   450,   451,
     452,   453,   454,   455,     0,   456,     0,     0,   457,   230,
     231,   232,   233,     0,   234,   235,     0,   236,   237,   238,
       0,   239,     0,   240,     0,   241,   242,   243,   244,   245,
     246,     0,   247,   248,   249,   250,   251,     0,   252,   253,
     254,   255,     0,     0,     0,   256,   257,   258,     0,   259,
       0,     0,     0,   260,   261,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     262,   263,   264,   265,   266,   267,   268,     0,   269,     0,
       0,     0,     0,     0,     0,   270,   271,   272,     0,   273,
     274,   275,     0,   276,     0,   277,     0,     0,   278,   279,
       0,   280,     0,     0,   281,   282,   283,   284,     0,   285,
     286,   287,   288,   289,     0,     0,   290,   291,   292,     0,
     293,   294,     0,   295,     0,   296,   297,     0,     0,     0,
       0,     0,   298,   299,   300,   301,   302,   303,   304,   305,
       0,     0,   306,     0,   307,   308,   309,     0,     0,     0,
     310,   311,   312,   313,     0,   314,   315,     0,   316,     0,
     317,   318,   319,   320,     0,   321,     0,   322,   323,     0,
     324,     0,   325,     0,     0,     0,   326,     0,   327,     0,
       0,   328,     0,   329,     0,   330,   331,   332,     0,   333,
     334,   335,     0,     0,   336,     0,   337,   338,   339,     0,
     340,     0,   341,     0,     0,     0,     0,   342,     0,   343,
     344,   345,     0,     0,   346,     0,     0,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,     0,   361,     0,   362,   363,   364,   365,   366,     0,
     367,   368,     0,     0,   369,   370,   371,     0,     0,   372,
       0,     0,     0,   373,   374,     0,     0,   375,     0,     0,
       0,   376,   377,   378,   379,   380,     0,   381,   382,   383,
       0,   384,   385,   386,   387,   388,   389,   390,     0,   391,
     392,   393,   394,   395,   396,   397,   398,     0,   399,   400,
     401,   402,   403,   404,     0,     0,   405,   406,   407,     0,
     408,   409,   410,   411,     0,   412,   413,     0,   414,   415,
     535,   417,   418,   419,   420,     0,   421,   422,     0,   423,
     424,   425,     0,   426,   427,     0,   429,     0,   430,   431,
     432,   433,     0,   434,   435,   436,   437,     0,     0,   438,
     439,   440,   441,   442,     0,     0,   443,   444,   445,   446,
     447,     0,   448,   449,     0,     0,     0,   450,   451,   452,
     453,   454,   455,     0,   456,     0,     0,   457,   230,   231,
     232,   233,     0,   234,   235,     0,   236,   237,   238,     0,
     239,     0,   240,     0,   241,   242,   243,   244,   245,   246,
       0,   247,   248,   249,   250,   251,     0,   252,   253,   254,
     255,     0,     0,     0,   256,   257,   258,     0,   259,     0,
       0,     0,   260,   261,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   262,
     263,   264,   265,   266,   267,   268,     0,   269,     0,     0,
       0,     0,     0,     0,   270,   271,   272,     0,   273,   274,
     275,     0,   276,     0,   277,     0,     0,   278,   279,     0,
     280,     0,     0,   281,   282,   283,   284,     0,   285,   286,
     287,   288,   289,     0,     0,   290,   291,   292,     0,   293,
     294,     0,   295,     0,   296,   297,     0,     0,     0,     0,
       0,   298,   299,   300,   301,   302,   303,   304,   305,     0,
       0,   306,     0,   307,   308,   309,     0,     0,     0,   310,
     311,   312,   313,     0,   314,   315,     0,   316,     0,   317,
     318,   319,   320,     0,   321,     0,   322,   323,     0,   324,
       0,   325,     0,     0,     0,   326,     0,   327,     0,     0,
     328,     0,   329,     0,   560,   331,   332,     0,   333,   334,
     335,     0,     0,   336,     0,   337,   338,   339,     0,   340,
       0,   341,     0,     0,     0,     0,   342,     0,   343,   344,
     345,     0,     0,   346,     0,     0,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
       0,   361,     0,   362,   363,   364,   365,   366,     0,   367,
     368,     0,     0,   369,   370,   371,     0,     0,   372,     0,
       0,     0,   373,   374,     0,     0,   375,     0,     0,     0,
     376,   377,   378,   379,   380,     0,   381,   382,   383,     0,
     384,   385,   386,   387,   388,   389,   390,     0,   391,   392,
     393,   394,   395,   396,   397,   398,     0,   399,   400,   401,
     402,   403,   404,     0,     0,   405,   406,   407,     0,   408,
     409,   410,   411,     0,   412,   413,     0,   414,   415,   416,
     417,   418,   419,   420,     0,   421,   422,     0,   423,   424,
     425,     0,   426,   427,     0,   429,     0,   430,   431,   432,
     433,     0,   434,   435,   436,   437,     0,     0,   438,   439,
     440,   441,   442,     0,     0,   443,   444,   445,   446,   447,
       0,   448,   449,     0,     0,     0,   450,   451,   452,   453,
     454,   455,     0,   456,     0,     0,   457,   230,   231,   232,
     233,     0,   234,   235,     0,   236,   237,   238,     0,   239,
       0,   240,     0,   241,   242,   243,   244,   245,   246,     0,
     247,   248,   249,   250,   251,     0,   252,   253,   254,   255,
       0,     0,     0,   256,   257,   258,     0,   259,     0,     0,
       0,   260,   261,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   262,   263,
     264,   265,   266,   267,   268,     0,   269,     0,     0,     0,
       0,     0,     0,   270,   271,   272,     0,   273,   274,   275,
       0,   276,     0,   277,     0,     0,   278,   279,     0,   280,
       0,     0,   281,   282,   283,   284,     0,   285,   286,   287,
     288,   289,     0,     0,   290,   291,   292,     0,   293,   294,
       0,   295,     0,   296,   297,     0,     0,     0,     0,     0,
     298,   299,   300,   301,   302,   303,   304,   305,     0,     0,
     306,     0,   307,   308,   309,     0,     0,     0,   310,   311,
     312,   313,     0,   314,   315,     0,   316,     0,   317,   318,
     319,   320,     0,   321,     0,   322,   323,     0,   324,     0,
     325,     0,     0,     0,   326,     0,   327,     0,     0,   328,
       0,   329,     0,   330,   331,   332,     0,   333,   334,   335,
       0,     0,   336,     0,   337,   338,   339,     0,   340,     0,
     341,     0,     0,     0,     0,   342,     0,   343,   344,   345,
       0,     0,   346,     0,     0,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   635,   360,     0,
     361,     0,   362,   363,   364,   365,   366,     0,   367,   368,
       0,     0,   369,   370,   371,     0,     0,   372,     0,     0,
       0,   373,   374,     0,     0,   375,     0,     0,     0,   376,
     377,   378,   379,   380,     0,   381,   382,   383,     0,   384,
     385,   386,   387,   388,   389,   390,     0,   391,   392,   393,
     394,   395,   396,   397,   398,     0,   399,   400,   401,   402,
     403,   404,     0,     0,   405,   406,  1042,     0,   408,   409,
     410,   411,     0,   412,   413,     0,   414,   415,   416,   417,
     418,   419,   420,     0,   421,   422,     0,   423,   424,   425,
       0,   637,   427,     0,   429,     0,   638,   431,   432,   433,
       0,   434,   435,   436,   437,     0,     0,   438,   439,   440,
     441,   442,     0,     0,   443,   444,   445,   446,   447,     0,
     448,   449,     0,     0,     0,   450,   451,   452,   453,   454,
     455,     0,   456,     0,     0,   457,  1119,   231,   232,   233,
       0,   234,   235,     0,   236,   237,   238,     0,   239,     0,
     240,     0,   241,   242,   243,   244,   245,   246,     0,   247,
     248,   249,   250,   251,     0,   252,   253,   254,   255,     0,
       0,     0,   256,   257,   258,     0,   259,     0,     0,     0,
     260,   261,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   262,   263,   264,
     265,   266,   267,   268,     0,   269,     0,     0,     0,     0,
       0,     0,   270,   271,   272,     0,   273,   274,   275,     0,
     276,     0,   277,     0,     0,   278,   279,     0,   280,     0,
       0,   281,   282,   283,   284,     0,   285,   286,   287,   288,
     289,     0,     0,   290,   291,   292,     0,   293,   294,     0,
     295,     0,   296,   297,     0,     0,     0,     0,     0,   298,
     299,   300,   301,   302,   303,   304,   305,     0,     0,   306,
       0,   307,   308,   309,     0,     0,     0,   310,   311,   312,
     313,     0,   314,   315,     0,   316,     0,   317,   318,   319,
     320,     0,   321,     0,   322,   323,     0,   324,     0,   325,
       0,     0,     0,   326,     0,   327,     0,     0,   328,     0,
     329,     0,   330,   331,   332,     0,   333,   334,   335,     0,
       0,   336,     0,   337,   338,   339,     0,   340,     0,   341,
       0,     0,     0,     0,   342,     0,   343,   344,   345,     0,
       0,   346,     0,     0,   347,   348,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,   360,     0,   361,
       0,   362,   363,   364,   365,   366,     0,   367,   368,     0,
       0,   369,   370,   371,     0,     0,   372,     0,     0,     0,
     373,   374,     0,     0,   375,     0,     0,     0,   376,   377,
     378,   379,   380,     0,   381,   382,   383,     0,   384,   385,
     386,   387,   388,   389,   390,     0,   391,   392,   393,   394,
     395,   396,   397,   398,     0,   399,   400,   401,   402,   403,
     404,     0,     0,   405,   406,   407,     0,   408,   409,   410,
     411,     0,   412,   413,     0,   414,   415,   416,   417,   418,
     419,   420,     0,   421,   422,     0,   423,   424,   425,     0,
     426,   427,     0,   429,     0,   430,   431,   432,   433,     0,
     434,   435,   436,   437,     0,     0,   438,   439,   440,   441,
     442,     0,     0,   443,   444,   445,   446,   447,     0,   448,
     449,     0,     0,     0,   450,   451,   452,   453,   454,   455,
       0,   456,     0,     0,   457
};

static const short yycheck[] =
{
       2,   517,   609,   513,   843,   633,   125,    74,  1140,   532,
     691,   490,    70,   862,   524,  1173,   483,   483,  1015,   868,
     483,  1018,   491,  1509,  1180,   494,   483,   977,  1190,  1719,
    1180,   547,   511,  1180,    86,   187,  1411,   102,     2,  1533,
     123,  1009,  1200,   120,   109,   921,   515,  1205,   558,  1050,
     519,  1259,   630,  1330,  1513,  1332,  1543,   526,   581,  1267,
    1775,  1193,  1752,   542,   109,   669,   988,   125,   126,  1292,
     839,  1961,  1203,   914,    76,    77,   545,  2094,  1209,  1824,
    1795,   109,    25,  1903,    86,   560,   555,  1295,  1792,  2099,
    2100,   634,  2243,   636,  1478,  1479,   684,   685,    49,    17,
     102,    41,    24,  1587,    90,   107,   564,   109,  1831,  1832,
     108,   485,  1816,   131,    55,  1338,   131,   506,    55,   888,
      61,    76,   944,   945,    61,   131,    76,   147,  1097,    70,
    2444,    72,    84,    70,   113,    72,  2189,    89,   102,    94,
     113,   963,    86,    87,   181,   109,  1115,   195,  2035,   538,
     151,  1838,   108,   210,    99,   544,    76,    81,   225,  1367,
    2585,    85,   115,  1547,  1849,   131,   953,   162,   186,   122,
     161,     8,   181,   126,    94,  1016,  1334,    14,    15,    16,
     186,   648,   648,    76,    42,   648,    44,    76,    25,    26,
      27,   648,    29,    30,    31,    88,   167,    84,    35,    36,
      37,    94,    89,   669,     5,   210,    99,   190,    28,   183,
     259,    28,   669,  1163,   284,   296,    84,    84,   147,    39,
     186,    89,   182,    28,    76,   178,   147,    28,   147,   253,
     210,   146,   227,    78,    79,   159,   151,   231,    39,   148,
     282,    84,    94,  1338,    76,   115,   191,    99,   231,    84,
     195,    96,   150,   268,    89,   100,    88,   102,   153,   332,
     335,    56,   146,   364,   109,   223,   111,  2008,    95,   324,
    2764,   394,    70,   268,   258,   398,    76,   222,   210,    74,
     151,   302,  2776,     0,   113,    76,   260,   114,   148,   204,
     147,   366,   112,   291,    94,  2036,   351,   255,   191,  1786,
     210,   402,   195,    94,   820,  2799,   300,   349,   178,   269,
     383,   112,   155,   211,  1311,   396,  2006,   300,   367,   212,
     213,   250,  1244,   332,   277,   246,   396,   210,   126,   222,
    1152,  1153,   327,   204,   210,   291,  1158,   246,   309,   191,
    2765,   851,  2359,   195,   296,   402,  2363,   321,   349,  1507,
    2367,  1509,   372,   246,   402,   375,   249,  2410,   276,   147,
     212,   213,   314,   873,   288,   402,   311,   260,  1155,   262,
     222,   362,   396,  1214,   389,   396,   428,  1218,  1219,   397,
     212,   399,   892,   302,   229,  2699,   206,   302,   246,   206,
     250,   397,   345,   399,   399,  1879,   106,   249,    76,   372,
     110,   206,   375,  2290,   402,   206,  1865,   277,   260,   492,
     396,   563,   122,    76,   358,  2138,    94,   396,   311,   399,
    1417,   212,   213,   396,   375,   814,   428,   314,   260,   818,
     352,   805,   375,   399,   349,   375,   355,   392,   275,  2143,
    2591,  2145,   392,  1412,   396,  1929,   314,   905,  1671,  1672,
    1673,   335,  1675,   371,   537,   930,   374,   352,   249,   311,
     401,  1283,  1284,  2150,   401,  1596,  1597,   399,   390,   389,
     390,   391,   392,  2158,  2159,   345,  1860,   396,   349,   314,
     373,   396,   366,  1691,    84,  1617,   875,   101,    17,   399,
     383,   384,   385,  1097,   387,   388,   389,   390,   391,   392,
     389,   390,   391,   392,   397,   342,   564,  1111,   375,   574,
     575,  1115,  1100,   380,   381,   372,   399,  2534,   375,   141,
    2530,   373,  2539,   399,  1747,   167,  2543,   159,   357,   255,
     575,   576,   384,   385,   212,   387,   388,   389,   390,   391,
     392,   373,   399,   372,   101,   610,   375,   575,    79,   212,
     302,   383,   384,   385,   332,   387,   388,   389,   390,   391,
     392,  2741,   253,   141,   303,   610,   611,   396,  1776,  1777,
     332,   282,   574,   575,   183,   197,  1671,  1672,  1673,  1122,
    1675,   181,   610,   326,   372,   328,   325,   375,   155,   203,
    1133,   212,   380,   304,  2774,   195,   387,   388,   389,   390,
     391,   392,  1230,   355,  1232,  1233,   101,   115,   610,   187,
     574,   575,   181,   150,  1081,  1081,  1083,  1083,  1085,  1085,
    2800,   383,  1544,  2640,  1081,   403,  1083,  1234,  1085,  1626,
    2450,  1097,   250,   635,   132,   133,   167,   258,     2,   147,
    1097,   241,  2387,  2137,   396,  1111,   610,   290,   493,  1115,
     352,    17,  1747,   166,  1111,    81,   352,   152,  1115,  2544,
    1170,  2546,   507,   173,   307,   155,   296,   309,  1590,  2392,
    2393,  2394,  2395,  2396,   211,   307,   402,   522,   278,   246,
    1147,  1147,   684,   685,  1147,   299,   253,   532,   297,   691,
    1147,   108,  2582,  1851,   539,   208,  2400,   195,  2402,  1575,
     402,   397,  1212,   325,   549,   396,   402,   186,   553,   387,
     388,   389,   390,   391,   392,  2174,  2175,   151,   210,   349,
     290,   364,  1853,  1854,   387,   388,   389,   390,   391,   392,
     375,  1134,   577,   159,   106,   380,   581,   307,   246,  1208,
     250,   151,   250,   253,     2,   243,   244,   325,  1906,    82,
     263,   360,   397,  1522,   372,   250,   246,   375,  2643,   337,
     250,   256,   142,  1604,    97,   296,  1924,   612,   140,   277,
     204,   616,   270,   351,  1290,   375,   176,   285,   309,   148,
     380,  1703,   492,  2060,  1187,  1363,   325,  1945,   388,  1947,
    1471,  1419,   151,  2016,   204,   505,  2019,  2020,  2021,  2022,
    2023,  2024,  2025,  2026,  2027,  2028,  1937,   147,  1412,   155,
     190,  1580,  1943,   247,   248,  1946,   142,   131,    76,    77,
      76,   335,   302,   223,   196,   242,   324,   173,    86,   339,
     199,   344,  1990,    76,  2321,  1811,   346,   345,   176,  2047,
      76,    77,  1231,  2066,  2067,   204,   399,   905,  1452,   107,
      86,   231,  2141,   182,   234,   255,   354,  2015,   230,  1835,
     232,   364,   365,   921,   190,  2154,   911,   149,   302,  1336,
    1336,   107,   186,  1336,   291,   994,    33,   246,  1879,  1336,
     182,   926,   319,   320,  2042,   223,    76,   181,   598,   248,
     307,   371,   302,   396,   374,   352,   898,   955,   200,   609,
     246,   195,   358,  2397,   250,   231,   246,   253,   234,   619,
     192,  1879,    45,   215,  1066,   349,   396,   255,   220,  2077,
     300,  2016,   149,   981,  2019,  2020,  2021,  2022,  2023,  2024,
    2025,  2026,  2027,  2028,   352,   372,   994,   647,   375,   349,
     397,   651,   149,   302,   316,   402,  1412,   375,   357,   254,
     375,   796,   662,   258,   799,  1412,   212,   802,  2445,  2674,
    2419,  2420,   396,   372,   389,   192,   375,   364,   365,   212,
     375,  2686,  2067,    84,   300,   380,   821,   296,   318,   397,
     825,   321,  2205,   828,   402,   192,  1452,   396,   368,   373,
     349,   396,   294,   339,   996,  1452,   841,   185,   317,   396,
     346,  1511,  1512,  2378,   260,  1007,   389,   390,   391,  1478,
    1479,   134,  1077,  1078,  1072,  1073,   396,  1075,  1072,  1073,
     131,  1075,   212,   372,   394,  1494,   375,   397,   398,   374,
     375,   876,   402,  1882,  1036,  1037,   404,   396,   226,   375,
     151,  1106,   368,   352,   380,  1047,  1098,   396,  1050,  1132,
    1052,  1053,  2049,    76,    82,  2052,   352,   168,  1934,   182,
    2218,   906,   352,  2219,    92,  2755,  2553,   912,   913,  2219,
     260,    99,  2219,    20,   262,  1077,  1078,   200,  1547,   397,
     352,  1550,   927,   928,   402,  1608,  1050,   115,  1052,  1053,
    2040,  2041,   215,   204,    55,  1971,  1098,   220,  1100,   372,
      61,   397,   375,   813,  1106,   372,   402,   397,   375,    70,
    2205,    72,   402,  1077,  1078,   238,   258,   373,  2341,   397,
    2251,  1240,   117,   396,   402,   397,   396,   383,   384,   385,
     402,   387,   388,   389,   390,   391,   392,   337,   326,    76,
     328,   397,  1106,  1824,   387,   388,   389,   390,   391,   392,
     375,   372,  2764,   372,   375,   380,   375,    94,   950,   951,
     870,   871,    99,   375,  2776,  2373,    83,   877,   380,   381,
     428,   294,   360,  2764,   285,   396,   122,   396,  1806,  2337,
     126,   973,  1240,   373,    38,  2776,   297,  2799,  1190,   212,
     982,   302,   428,   383,   384,   385,   289,   387,   388,   389,
     390,   391,   392,  1719,  1269,   122,  1271,   397,  2799,   919,
    1275,   326,   142,   328,     7,   326,  1281,   328,    11,   372,
     397,   307,   375,  1288,   141,   402,   337,   397,   165,    22,
      23,   372,   402,  1298,   375,  1300,  1752,   260,   349,   350,
     157,   372,   371,   396,   375,   374,  2341,   370,   394,   134,
      43,   397,   398,    46,   191,   396,   402,  1322,   195,  1324,
     190,  1690,  1226,  1692,  1693,   396,   183,  1269,  1326,  1271,
     112,   113,  1117,  1275,  1332,   212,   213,   281,  1280,  1281,
     197,   372,  1340,   128,   375,   222,  1288,  1803,  2511,   372,
    2513,   380,   375,   382,   397,   397,  1298,   182,  1300,   402,
     402,   231,  2788,   197,   234,  1269,  2468,  1271,   364,   365,
     218,  1275,   249,   396,   396,   200,   372,  1281,   372,   375,
    1322,   375,  1324,   260,  1288,   372,   375,   397,   375,   117,
     215,   380,   402,   397,  1298,   220,  1300,   394,   402,   396,
     396,  2549,    76,   260,   364,   365,   395,   396,  1827,   396,
     373,  1061,   372,   238,    88,   375,  1072,  1073,  1322,  1075,
    1324,  2237,  2520,   156,   387,   388,   389,   390,   391,   392,
     300,   258,  1217,   258,   311,   210,   396,   635,   373,   372,
     297,   298,   375,  2259,    76,  1774,   303,  1389,   383,   384,
     385,  1860,   387,   388,   389,   390,   391,   392,   326,   635,
     328,   397,    94,  2626,  1249,  1915,   402,    99,   325,   294,
     372,  1878,  1878,   375,    76,  1878,  2511,   375,  2513,   279,
     337,  1878,   380,  1387,   382,   342,   684,   685,   402,  1907,
     388,  1909,  1910,  1911,  1912,   375,   373,   181,   368,  1504,
     380,   371,   372,   360,   374,   375,   364,   365,   684,   685,
     387,   388,   389,   390,   391,   392,   396,   397,   394,   397,
     396,  1306,  1464,   375,   402,   397,   397,   352,   380,  1471,
     402,   402,  1474,   165,  1476,   397,   401,   402,   212,    45,
     402,  1546,  1992,   395,   396,   370,   397,  2645,   397,    55,
    2006,   402,   326,   402,   328,    61,  1206,  1207,   399,   191,
     397,   402,  1504,   195,    70,   402,    72,   398,  1510,    75,
    1474,  1513,  1476,    95,   397,  1517,   369,  1575,  2515,   402,
     212,   213,   397,   397,  1234,   214,   260,   402,   402,   397,
     222,  2626,  1377,   195,   402,   401,   402,  1247,   397,   261,
    1504,   397,   397,   402,  1546,   258,   402,   402,  1258,   147,
     212,    76,    77,    78,    79,  1674,   369,   249,   397,    95,
     397,    86,   399,   402,    89,    90,   214,  1569,   260,   397,
     369,    96,  1961,  1638,   402,   100,   214,   102,   397,   214,
    2738,    76,  1546,   402,   109,   373,   111,   307,   113,   397,
     364,   365,   117,   118,   402,   383,   384,   385,   260,   387,
     388,   389,   390,   391,   392,   806,   397,  1726,   399,   389,
     811,   209,  1127,  1128,  1129,   180,  1674,  1736,   664,   311,
     666,   387,   388,   389,   390,   391,   397,  2364,   226,  2366,
    2788,   397,  1526,  1527,  1528,   397,  1638,  1482,  2770,   373,
     898,  1083,   399,  1085,    84,  1490,   105,  1492,   164,   383,
     384,   385,  1362,   387,   388,   389,   390,   391,   392,   105,
    1718,    90,   898,   387,   388,   389,   390,   391,  1726,   364,
     365,  1729,   404,   397,  1638,  1808,  1809,   212,  1736,  1036,
    1037,   373,   332,   258,   399,   364,  1050,   396,  1052,  1053,
     396,  1749,  1769,  1770,   396,   387,   388,   389,   390,   391,
     392,   115,   396,    90,   229,  2094,  2387,  1765,  1418,   218,
     364,   373,   398,   364,   299,   364,  1774,   212,   139,   363,
    1785,   383,   384,   385,   396,   387,   388,   389,   390,   391,
     392,    45,   387,   388,   389,   390,   391,   396,   996,   396,
     395,    55,   402,  1808,  1809,  1810,   258,    61,   258,  1007,
    1817,   177,  1819,   221,   176,   258,    70,   396,    72,   396,
     996,    75,   166,  1608,  1829,   260,  1831,  1832,   396,   396,
    2619,  1007,   176,   371,   372,   396,   374,   375,  1036,  1037,
     396,   396,   380,  1785,   382,   275,   396,   396,  1790,  1047,
     388,   115,  1050,   396,  1052,  1053,   396,   121,   402,   396,
    1036,  1037,   126,   396,   208,   396,  1808,  1809,  1810,  1811,
     396,  1047,   396,  2652,  2653,   396,   396,    90,   221,   223,
     221,  1785,  1824,   147,   148,   221,  1790,  1829,  1893,  1831,
    1832,   212,  1834,  1835,   396,   176,  1838,   389,   146,   400,
    1098,   400,  1100,   351,  1808,  1809,  1810,  1811,   398,   389,
     396,   255,   396,   402,   396,  1565,   122,   351,   351,   263,
    1570,   333,  1098,  1865,  1100,  1829,   332,  1831,  1832,   396,
    1834,  1835,   396,   258,  1838,   199,  1934,  1879,   373,   212,
     256,   396,   357,  1885,  1886,   396,    90,    82,  2764,   384,
     385,  1893,   387,   388,   389,   390,   391,   392,   396,   396,
    2776,   147,    95,   428,   396,  1907,   176,  1909,  1910,  1911,
    1912,   258,    90,  1971,   396,  1879,   176,    90,    76,   352,
      17,  1885,   246,  2799,   402,   383,   250,   332,   402,  1893,
     355,   402,  1190,  1998,   396,   396,   330,   397,    90,  2004,
     344,   330,   362,  1907,   394,  1909,  1910,  1911,  1912,    76,
     397,   402,   307,   277,  1190,   107,   387,   388,   389,   390,
     391,   285,   195,   330,   395,   490,   491,   332,   493,   494,
    2359,   181,   396,   396,  2363,  2094,   335,   396,  2367,   208,
     397,   259,   507,  1050,   402,   253,   511,   402,   513,    90,
     515,  2598,   223,   397,   519,   206,  1998,   522,   400,   524,
     400,   526,  2004,   206,    38,  2633,   107,   532,   390,   216,
    2087,  2088,  2089,  2090,   539,  2643,   394,   542,   216,   171,
     545,   345,  1280,  1387,   549,   396,   396,   325,   553,   288,
     555,   187,   372,   558,  1998,   348,  2094,   212,   383,   184,
    2004,   193,  1887,  1888,  1280,   383,   176,   262,    90,   258,
     188,  2679,   577,  2055,   212,   292,   581,    90,  1768,   402,
     397,   397,   139,   402,   375,  2693,   218,    12,   375,   375,
     171,   402,   233,  2138,  2463,   181,   397,   402,   397,   397,
     397,  1926,   181,   398,   369,   397,   369,   612,   397,   332,
     332,   616,   193,  2603,   397,   397,   397,   397,   397,   181,
     397,   397,   260,   397,   373,   397,  1173,   332,   633,   634,
    1474,   636,  1476,   332,   383,   384,   385,   218,   387,   388,
     389,   390,   391,   392,   250,   369,   181,   332,   214,   396,
     125,  1389,   397,  1200,   282,  1202,  2138,   206,  1205,  2141,
     292,   206,   383,   399,   383,  2534,    34,   397,  2150,   301,
    2539,   402,  2154,  1389,  2543,   332,   332,   397,   396,   684,
     685,   686,   396,   315,   212,   397,  2676,   397,   212,   397,
     322,   258,  2174,  2175,  2138,   342,   325,  2141,   360,  2237,
     396,   402,   332,   397,   352,   258,  2150,   176,   167,   397,
    2154,   292,  1902,  2582,   391,   383,   402,   167,   397,   164,
     301,  2259,    76,   181,   176,   397,  1464,   397,   397,   361,
     113,   300,   364,   234,   315,   373,  1474,  2181,  1476,   402,
     187,   322,   221,   397,   396,   383,   384,   385,  1464,   387,
     388,   389,   390,   391,   392,   397,   396,   396,   373,  2755,
    2242,   212,   397,    76,   212,  1503,   399,   153,   121,   396,
     216,  2640,  1510,   397,   176,  1513,   262,   198,   217,  1517,
     361,    94,   396,   342,   396,   186,    99,  1334,   397,   402,
     181,   796,   397,   397,   799,   396,   253,   802,   397,   397,
     396,  1517,   215,   265,   215,   396,   105,   131,   189,   250,
     250,   148,   199,   183,   183,   181,   821,   160,   352,   168,
     825,  2359,   397,   828,   396,  2363,   181,   151,   258,  2367,
     301,  1569,  1173,   250,   332,   261,   841,   215,   396,    90,
     396,   396,  2167,  2168,   168,   250,   851,  2392,  2393,  2394,
    2395,  2396,   285,  1569,   397,   229,   280,   256,   256,  1200,
     865,  1202,   402,   396,  1205,   402,   250,   396,   873,   874,
    2415,   876,    93,   258,   176,    90,   258,   202,   191,  2204,
     204,   160,   195,   250,   396,  2754,   258,   892,   202,   156,
     296,   151,   402,   258,   349,   205,   168,  2379,    80,   212,
     213,   906,  2092,  2093,   280,  2387,   396,   912,   913,   222,
    2392,  2393,  2394,  2395,  2396,   397,  2785,   922,   126,  1174,
     284,  2177,   927,   928,  1790,  1899,  1053,  1551,  2231,  2234,
    1488,  2161,  2437,  2415,  1126,  1846,   249,  2419,  2420,  2726,
    2248,  2746,  2424,  2710,  2709,  2731,  1790,   260,  2392,  2393,
    2394,  2395,  2396,  2597,  2236,   528,  1524,  2800,  2774,  2429,
    1507,   285,  1509,  2430,  2757,  1878,  1176,  1811,  2801,   109,
     920,  2415,  1258,   297,  1981,   626,  2265,  2459,   302,  2675,
    2742,  1551,  2464,  1940,  1531,  1929,  2468,  2474,  1507,  2656,
    1834,  1835,  2764,  1334,  1838,   932,  2534,  2799,   311,  1529,
    2190,  2539,   326,  1418,   328,  2543,   101,  1972,  1216,  1800,
    1080,  1079,  1173,   337,  1810,    76,  1415,  2094,  2641,  2365,
    2224,  2211,  1531,  2054,  1293,   349,   350,  2337,  1360,  1749,
    2220,  1036,  1037,    94,  1640,  1879,  2504,  1794,    99,  1200,
    1587,  1202,  1047,  2268,  1205,   922,   874,  1052,  1053,  1108,
    1454,  1464,  1790,  1467,   225,   689,   904,   930,  1063,  1086,
     373,  1613,  1471,  1907,  1035,  1909,  1910,  1911,  1912,  2261,
     383,   384,   385,  1811,   387,   388,   389,   390,   391,   392,
    1611,  2115,  1081,   224,  1609,  1834,  1476,     2,    -1,  1222,
      -1,    -1,    -1,  1098,    -1,  1100,  1834,  1835,    -1,    -1,
    1838,    -1,  2640,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1117,    -1,    -1,    -1,    -1,  1122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1863,    -1,  1865,  1133,    -1,
      -1,    -1,    -1,    -1,   195,    -1,    -1,    -1,    76,    -1,
      -1,  1879,    -1,    -1,    -1,    -1,    -1,  1885,  1886,    -1,
      88,   212,   213,    -1,    -1,    -1,    94,  2347,  2348,  2349,
    2350,    99,    -1,    77,    -1,  1170,  1507,    -1,  1509,    -1,
    1886,    -1,    86,  1334,    -1,    -1,    -1,  2776,    -1,    -1,
      -1,    95,    -1,    -1,    -1,  1190,    -1,    -1,   249,    -1,
    1531,  2736,  2737,    -1,    -1,    -1,    -1,    -1,  2680,   260,
      -1,    -1,    -1,  1208,    -1,    -1,    -1,  1212,    -1,    -1,
      -1,    -1,  1217,    -1,    -1,    -1,    -1,    -1,  2700,  2701,
      -1,    -1,    -1,    -1,    -1,  1230,  2764,  1232,  1233,    -1,
      -1,    -1,    -1,    -1,    -1,  1240,    -1,    -1,  2776,    -1,
      -1,    -1,    -1,    -1,  1249,    -1,  1587,    -1,    -1,    -1,
      -1,    -1,    -1,   191,  2736,  2737,    -1,   195,    -1,    -1,
      -1,  2799,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   212,   213,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   222,  1290,    -1,    -1,    -1,    -1,
      -1,    -1,  2736,  2737,    -1,    -1,    -1,  2141,    -1,    -1,
      -1,  1306,    -1,    -1,  1851,    -1,  2150,    -1,   246,    -1,
    2154,   249,   373,    -1,    -1,    -1,    -1,  2055,    -1,    -1,
      -1,    -1,   260,    -1,   262,    -1,   387,   388,   389,   390,
     391,   392,  1879,    -1,    -1,    -1,    -1,  2181,    -1,  2055,
      -1,    -1,    -1,  2668,    -1,    -1,  1507,    -1,  1509,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,  1906,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1531,    -1,  1377,   311,    -1,   212,    -1,  1924,    -1,    -1,
      -1,    -1,  1929,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1940,  2584,    -1,    -1,    83,  1945,    -1,
    1947,    -1,    -1,  2141,    -1,    -1,    92,    -1,  2598,   246,
      -1,    -1,  2150,    -1,  1419,    -1,  2154,    -1,    -1,    -1,
      -1,    -1,    -1,   260,   110,    -1,  1587,   113,    -1,   266,
      -1,    -1,  2622,    -1,    -1,   373,  2174,  2175,    -1,    -1,
     126,    -1,   128,  1990,    -1,   383,   384,   385,    -1,   387,
     388,   389,   390,   391,   392,   141,    -1,   395,    -1,  1464,
    2650,   147,  1467,    -1,    -1,    -1,   404,    -1,  2015,    -1,
      -1,   157,    -1,  1478,  1479,    -1,    -1,  1482,    -1,    -1,
      -1,    -1,    -1,  1488,  1489,  1490,  1491,  1492,    -1,  1494,
      -1,    -1,    -1,    -1,    -1,  2042,    -1,    -1,    -1,   185,
      -1,   187,    -1,    -1,  2242,    -1,  1511,  1512,    -1,    -1,
    1851,   197,    -1,    -1,   428,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  2242,    -1,    -1,    -1,
    2077,    -1,   218,    -1,    -1,    -1,   373,    -1,  1879,    -1,
     226,    -1,  1547,    -1,    -1,  1550,   383,   384,   385,    -1,
     387,   388,   389,   390,   391,   392,  1561,    -1,    -1,    -1,
      -1,  1566,    -1,    -1,    -1,  1906,    -1,    -1,  1573,   483,
     131,    -1,    -1,    -1,   260,    -1,   262,    -1,    -1,    -1,
      -1,    -1,    -1,  1924,    -1,    -1,    -1,  1592,  1929,    -1,
      -1,    -1,   506,    -1,    -1,   281,    -1,    -1,    -1,  1940,
      -1,    -1,    -1,  1608,  1945,   166,  1947,    -1,    -1,    -1,
      -1,    -1,   298,    -1,    -1,   176,  1621,   303,    -1,   533,
     181,   535,    -1,    -1,   538,   186,   187,    -1,   189,    -1,
     544,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,   325,
     326,  2379,   328,    -1,    -1,    -1,    -1,   208,    -1,  1990,
      -1,   337,    -1,    94,    -1,   341,   342,    -1,    99,   345,
      -1,    -1,   223,  2379,    -1,   351,    -1,    -1,    -1,    -1,
      -1,  2218,    -1,    -1,  2015,    -1,    -1,    -1,    -1,    -1,
      -1,  2419,  2420,    -1,    -1,    -1,  2424,    -1,    -1,    -1,
    1851,    -1,    -1,    -1,   255,    -1,    -1,    -1,    -1,  1704,
      -1,  2042,   263,    -1,    -1,    76,    -1,    -1,  2424,    -1,
      -1,    -1,    -1,    -1,  1719,    -1,   630,    88,  1879,    -1,
      -1,  2459,    -1,    94,   165,    -1,  2464,    -1,    99,    -1,
    2468,    -1,    -1,    -1,   648,    -1,  2077,    -1,    -1,    -1,
      -1,    -1,    -1,  2459,    -1,  1906,    -1,  1752,  2464,    -1,
     191,    -1,  2468,    -1,   195,   669,    -1,    76,    -1,    -1,
      -1,    -1,    -1,  1924,   325,    -1,    -1,    -1,  1929,    88,
      -1,   212,   213,    -1,    -1,    94,    -1,    -1,    -1,  1940,
      99,   222,    -1,   344,  1945,  1790,  1947,    -1,    -1,  1794,
    2337,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1173,    -1,
      -1,  1806,   363,    -1,    -1,    -1,    -1,    -1,   249,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   260,
     191,    -1,  1827,    -1,   195,  1200,    -1,  1202,    -1,  1990,
    1205,    -1,    -1,    -1,    -1,    -1,   397,   398,   399,    -1,
      -1,   212,   213,    -1,  1849,    -1,    -1,    -1,    -1,    -1,
      -1,   222,    -1,    -1,  2015,  1860,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     311,    -1,   191,    -1,    -1,   246,   195,  2218,   249,    -1,
      -1,  2042,  1887,  1888,    -1,    -1,    -1,    -1,    -1,   260,
      -1,   262,    -1,   212,   213,    -1,    -1,    -1,    -1,    -1,
     814,    -1,    -1,   222,   818,    -1,    -1,    -1,    -1,    -1,
    1915,    -1,    -1,    -1,    -1,    -1,  2077,    -1,    -1,    -1,
      -1,  1926,    -1,    -1,    -1,   839,    -1,   246,    -1,    -1,
     249,    -1,   373,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     311,   260,  2680,   262,    -1,    -1,   387,   388,   389,   390,
     391,   392,    -1,    -1,    -1,    -1,    -1,    -1,  1963,  1334,
      -1,   875,  2700,  2701,  2680,    -1,    -1,  1972,    -1,    -1,
      -1,    -1,    -1,  2520,   888,    -1,  1981,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  2700,  2701,    -1,  1992,    -1,    -1,
      -1,    -1,   311,    -1,    -1,    -1,  2337,    -1,    -1,    -1,
      -1,  2006,   373,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   383,   384,   385,    -1,   387,   388,   389,   390,
     391,   392,    76,    -1,    -1,    -1,    -1,    -1,   942,    -1,
      -1,   402,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      94,    -1,    -1,    -1,    -1,    99,    -1,    -1,    -1,    -1,
     964,    -1,    -1,    -1,   373,    -1,    -1,  2218,   131,    -1,
      -1,    -1,    -1,    -1,   383,   384,   385,    -1,   387,   388,
     389,   390,   391,   392,    -1,   989,   990,    -1,   992,   993,
     994,    -1,    -1,   402,    -1,    -1,    -1,    -1,    76,    -1,
    2095,    -1,    -1,   166,  2099,  2100,  1010,    -1,  2645,    -1,
      -1,    -1,    -1,   176,    -1,    -1,    94,    -1,   181,    -1,
      -1,    99,    -1,   186,   187,    -1,   189,    -1,    -1,    -1,
      -1,    -1,  1036,  1037,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1507,    -1,  1509,   208,  1050,   191,    -1,    -1,
      -1,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     223,    -1,    -1,  2158,  2159,    -1,  1531,    -1,   212,   213,
    1074,    -1,  2167,  2168,    -1,    -1,    -1,  1081,   222,  1083,
      -1,  1085,    -1,    -1,    -1,    -1,  2337,   165,    -1,  2520,
      -1,    -1,   255,  1097,  1098,    -1,    -1,    -1,    -1,    -1,
     263,  2738,   246,    -1,    -1,   249,    -1,  1111,  2203,  2204,
      -1,  1115,    -1,   191,    -1,    -1,   260,   195,   262,    -1,
      -1,    -1,  1587,    -1,    -1,    -1,    -1,    -1,    -1,  2224,
      -1,    -1,    -1,    -1,   212,   213,  2231,    -1,    -1,  2234,
      -1,  2236,    -1,  1147,   222,    -1,    -1,    -1,  2243,    -1,
      -1,  2788,    -1,  2248,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   325,    -1,    -1,    -1,    -1,   311,    83,  1173,
    2265,   249,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,
      -1,   344,   260,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      -1,    -1,    -1,   110,  1198,   110,  1200,    -1,  1202,    -1,
     363,  1205,    -1,    -1,    -1,   122,   123,    -1,    -1,    -1,
      -1,    -1,    -1,   128,  2645,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,  1231,    -1,   373,
      -1,    -1,    -1,   311,   397,   398,   399,    -1,    -1,   383,
     384,   385,   157,   387,   388,   389,   390,   391,   392,    -1,
      -1,    -1,    -1,   397,    -1,  1259,    -1,    -1,  2353,    -1,
    1264,  2356,    -1,  1267,  1268,    -1,    -1,    -1,   183,  2520,
      -1,    -1,   187,    -1,    -1,  2370,  1280,    -1,    -1,    -1,
      -1,    -1,   197,  1287,    -1,    -1,    -1,  1291,  1292,  1293,
      -1,  1295,    -1,  2388,    -1,   373,    -1,    -1,  1302,  1303,
      -1,    -1,    -1,    -1,    -1,  1309,    -1,  2738,    -1,   387,
     388,   389,   390,   391,   392,    -1,    -1,    -1,    -1,    -1,
      -1,  2416,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1334,  1335,  1336,    -1,  1338,  1339,    -1,    -1,    -1,  1343,
      -1,    -1,    -1,  1347,    -1,   260,  1350,  1351,  1352,  1353,
    1354,  1355,  1356,  1357,  1358,  1359,    -1,  2788,    -1,  1363,
      -1,    -1,    -1,  1367,    -1,    -1,   281,    -1,  2463,    -1,
      -1,    -1,    -1,  2468,    -1,    -1,    -1,    -1,    -1,    -1,
    1173,    -1,   297,   298,    -1,    -1,  1851,    -1,   303,    -1,
      -1,    -1,    -1,    -1,  2645,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1200,  1412,  1202,
     325,    -1,  1205,    -1,  1879,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   337,    -1,    -1,    -1,    -1,   342,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  2530,   351,  2532,    -1,    -1,
      -1,  1906,    -1,    -1,    -1,   360,    -1,    -1,  1452,  2544,
      -1,  2546,  1173,    -1,    -1,    -1,    -1,    -1,    -1,  1924,
      -1,    -1,    -1,    -1,  1929,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1940,    -1,    -1,    -1,  1200,
    1945,  1202,  1947,    -1,  1205,    -1,    -1,  2738,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  2591,  2592,    -1,    -1,
      -1,    -1,    -1,  1507,    -1,  1509,    -1,    -1,  2603,    -1,
      -1,    -1,    -1,  2608,    -1,    -1,    -1,  1521,  1522,    -1,
      -1,    -1,    -1,    -1,    -1,  1990,    -1,  1531,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1173,    -1,  2788,  2633,    -1,
      -1,  1334,    -1,    -1,    -1,    -1,    -1,    -1,  2643,    -1,
    2015,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1200,    -1,  1202,    -1,    -1,  1205,    -1,    -1,
      -1,  1575,    -1,  2668,    -1,   492,  1580,  2042,    -1,    -1,
    2675,  2676,    -1,  1587,  2679,    -1,    -1,    -1,   505,   506,
      -1,    -1,    -1,    -1,    -1,    -1,   513,    -1,  2693,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   524,    -1,    -1,
      -1,    -1,  2077,  1334,    -1,    -1,    -1,    -1,    -1,  1623,
     537,   538,    -1,    -1,    -1,    -1,    -1,   544,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1639,    -1,    -1,    -1,    -1,
      -1,   558,    -1,    -1,    -1,    -1,  2741,  2742,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    2755,    -1,    -1,    -1,    -1,    -1,    -1,  1671,  1672,  1673,
    1674,  1675,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  2774,
      -1,   598,    -1,    -1,    -1,    -1,  1690,  1691,  1692,  1693,
      -1,    -1,   609,    -1,    -1,    -1,  1334,    -1,  1702,    -1,
      -1,    -1,   619,    -1,    -1,  2800,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1507,    -1,  1509,  1721,    -1,    -1,
      -1,    -1,  1726,    -1,    -1,   106,    -1,    -1,    -1,   110,
     647,    -1,  1736,    -1,   651,    -1,    -1,    -1,  1531,    -1,
      -1,   122,   123,  1747,  1748,   662,  1750,    -1,    -1,    -1,
    1754,    -1,    -1,  2218,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1774,    -1,  1776,  1777,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1507,    -1,  1509,    -1,
      -1,    -1,    -1,    -1,  1587,    -1,  1800,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1531,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1845,    -1,    -1,    -1,    -1,    -1,  1851,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1587,    -1,    -1,  1507,
      88,  1509,  2337,    -1,  1878,  1879,    94,    -1,    -1,    -1,
      -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1531,    -1,    -1,   813,   814,    -1,    -1,
      -1,   818,  1906,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1924,    -1,    -1,    -1,    -1,  1929,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   851,    -1,  1940,    -1,    -1,    -1,
      -1,  1945,    -1,  1947,    -1,    -1,    -1,    -1,    -1,  1587,
      -1,    -1,    -1,   870,   871,    -1,   873,  1961,   875,    -1,
     877,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,    -1,   892,    -1,   195,  1982,    -1,
    1984,    -1,    -1,  1987,    -1,    -1,  1990,    -1,    -1,    -1,
    1994,    -1,    -1,    -1,   212,   213,  2000,    -1,  2002,    -1,
      -1,    -1,   919,  2007,   222,    -1,    -1,    -1,    -1,    -1,
      -1,  2015,  2016,    -1,    -1,  2019,  2020,  2021,  2022,  2023,
    2024,  2025,  2026,  2027,  2028,    -1,    -1,    -1,   246,  2033,
    2034,   249,    -1,    -1,    -1,    -1,    -1,    -1,  2042,  1173,
      -1,    -1,   260,  2047,   262,    -1,  2050,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  2520,    -1,    -1,  1851,    -1,
      -1,    -1,  2066,  2067,  2068,    -1,  1200,  2071,  1202,    -1,
      -1,  1205,    -1,  2077,  2078,    -1,    -1,    -1,    -1,  2083,
      -1,    -1,    -1,    -1,    -1,    -1,  1879,    -1,    -1,    -1,
    2094,    -1,    -1,   311,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   492,    -1,  1906,  2118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   505,   506,    -1,    -1,    -1,    -1,
    1851,  1924,   513,    -1,    -1,    -1,  1929,    -1,    -1,    -1,
      -1,    -1,    -1,   524,  1061,    -1,    -1,  1940,    -1,    -1,
      -1,    -1,  1945,    -1,  1947,   373,   537,   538,  1879,    -1,
      -1,    -1,    -1,   544,    -1,   383,   384,   385,    -1,   387,
     388,   389,   390,   391,   392,    -1,    -1,   558,    -1,   397,
    2645,    -1,    -1,    -1,    -1,  1906,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1990,    -1,    -1,
    1334,  2205,    -1,  1924,    -1,    -1,    -1,    -1,  1929,    -1,
      -1,    -1,    -1,  1851,  2218,  1132,    -1,   598,    -1,  1940,
      -1,    -1,  2015,    -1,  1945,    -1,  1947,    -1,   609,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   630,   619,    -1,
      -1,  1879,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  2042,
      -1,    -1,    -1,  1170,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  2268,    -1,   647,    -1,  1906,  1990,
     651,    -1,    -1,  2738,  2278,    -1,    -1,    -1,    -1,    -1,
      -1,   662,    -1,    -1,  2077,    -1,  1924,    -1,    -1,  1206,
    1207,  1929,    76,    -1,  2015,  1212,    -1,  1173,    -1,    -1,
      -1,    -1,  1940,    -1,    -1,    -1,    -1,  1945,    -1,  1947,
      94,    -1,    -1,    -1,  1231,    99,    -1,  1234,    -1,    -1,
      -1,  2042,    -1,  2788,  1200,    -1,  1202,    -1,    -1,  1205,
    1247,  2335,    -1,  2337,    -1,    -1,    -1,  2341,  2342,  2343,
      -1,  1258,  2346,    -1,    -1,    -1,    -1,    -1,    -1,    76,
      -1,    -1,  1990,    -1,    -1,  2359,  2077,    -1,    -1,  2363,
      -1,    88,    -1,  2367,    -1,    -1,    -1,    94,    -1,  2373,
      -1,    -1,    99,  1507,    -1,  1509,    -1,  2015,    -1,    -1,
      -1,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1531,    -1,    -1,
      -1,    -1,    -1,    -1,  2042,    -1,    -1,   191,    -1,  2413,
      -1,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  2218,    -1,    -1,   212,   213,
      -1,  2435,   813,   814,    -1,    -1,    -1,   818,   222,  2077,
      -1,    -1,    -1,    -1,    -1,  1362,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1587,    -1,    -1,    -1,    -1,  1334,  2463,
      -1,    -1,    -1,    -1,   191,   249,    -1,    -1,   195,    -1,
     851,    -1,    -1,    -1,    -1,    -1,   260,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   212,   213,    -1,    -1,   870,
     871,    -1,   873,    -1,   875,   222,   877,  2218,    -1,    -1,
    2504,  1418,    -1,    -1,    -1,    -1,    -1,  2511,    -1,  2513,
      -1,   892,    -1,    -1,    -1,    -1,  2520,    -1,    -1,   246,
    2524,    -1,   249,    -1,    -1,    -1,    -1,   311,    -1,    -1,
    2534,    -1,    76,   260,    -1,  2539,    -1,    -1,   919,  2543,
      -1,    -1,    -1,    -1,  2337,  2549,  2550,    -1,    -1,   942,
      94,    -1,    -1,    -1,    -1,    99,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   964,    -1,    -1,    -1,    -1,    -1,    -1,  2582,    -1,
    2218,    -1,    -1,    -1,   311,    -1,    -1,    -1,    -1,   373,
      -1,    -1,    -1,    -1,  1511,  1512,   989,   990,    -1,   992,
     993,   994,    -1,   387,   388,   389,   390,   391,   392,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  2337,  1010,    -1,    -1,
      76,    -1,  2626,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1507,  2636,  1509,    -1,    -1,  2640,    -1,    94,    -1,
      -1,  2645,    -1,    99,    -1,    -1,   373,   191,  1565,    -1,
      -1,   195,    -1,  1570,    -1,  1531,   383,   384,   385,    -1,
     387,   388,   389,   390,   391,   392,    -1,    -1,   212,   213,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   222,    -1,
    1061,  1074,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  2337,
      -1,    -1,   246,    -1,    -1,   249,    -1,    -1,    -1,   165,
      -1,  1587,    -1,    -1,    -1,    -1,   260,  1851,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  2520,    -1,    -1,
      -1,    -1,    -1,    -1,  2738,   191,    -1,    -1,    -1,   195,
      -1,    -1,    -1,    -1,    -1,  1879,    -1,    -1,    -1,    -1,
    2754,  1132,    -1,    -1,    -1,    -1,   212,   213,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   222,   311,    -1,    -1,
      -1,    -1,  1906,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  2785,    -1,    -1,  2788,    -1,    -1,    -1,    -1,  1170,
    1924,    -1,    -1,   249,    -1,  1929,    -1,    -1,    -1,  2520,
      -1,    -1,    -1,    -1,   260,  1198,  1940,    -1,    -1,    -1,
      -1,  1945,    -1,  1947,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1206,  1207,    -1,    -1,   373,
      -1,  1212,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   383,
     384,   385,    -1,   387,   388,   389,   390,   391,   392,    -1,
    1231,  1768,  2645,  1234,    -1,   311,  1990,  1774,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1247,    -1,    -1,    -1,
      -1,  1264,    -1,    -1,    -1,  1268,    -1,  1258,    -1,    -1,
      -1,  2015,  2520,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1287,    -1,    -1,    -1,  1291,    -1,
    1293,    -1,  1295,    76,    -1,    -1,    -1,    -1,  2042,  1302,
    1303,    -1,    -1,    -1,    -1,    88,  1309,   373,    -1,    -1,
      -1,    94,    -1,    -1,  2645,    -1,    99,    -1,    -1,    -1,
      -1,   387,   388,   389,   390,   391,   392,    -1,    -1,    -1,
      -1,    -1,  1335,  2077,    -1,  2738,  1339,    -1,    -1,    -1,
    1343,    -1,    -1,    -1,  1347,    -1,    -1,  1350,  1351,  1352,
    1353,  1354,  1355,  1356,  1357,  1358,  1359,    -1,    -1,    -1,
    1363,    76,    -1,    -1,    -1,  1851,    -1,    -1,    -1,    -1,
      -1,  1362,    -1,    88,    -1,  1902,    -1,    -1,    -1,    94,
      -1,    -1,    -1,    -1,    99,  2788,    -1,    -1,  1915,    -1,
      -1,    -1,    -1,  1879,    -1,    -1,    -1,  2645,    -1,  1926,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  2738,   191,    -1,
      -1,    -1,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1906,    -1,    -1,    -1,    -1,    -1,    -1,  1418,    -1,   212,
     213,    -1,    -1,    -1,  1961,    -1,    -1,    -1,  1924,   222,
      -1,    -1,    -1,  1929,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1940,    -1,    -1,  2788,    -1,  1945,
      -1,  1947,    -1,   246,    -1,  1992,   249,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  2218,    -1,   191,   260,    -1,   262,
     195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    2738,    -1,    -1,    -1,    -1,    -1,    -1,   212,   213,    -1,
      -1,    -1,    -1,    -1,  1990,    -1,    -1,   222,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1521,    -1,
    1511,  1512,    -1,    -1,    -1,    -1,    -1,    -1,   311,  2015,
      -1,   246,    -1,    -1,   249,    -1,    -1,    -1,    -1,    -1,
    2788,    -1,    -1,    -1,    -1,   260,    -1,   262,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  2042,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  2092,  2093,  2094,    -1,    -1,
      -1,    -1,    -1,    -1,  1565,    -1,    76,    -1,    -1,  1570,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,
     373,  2077,    -1,  2337,    94,    -1,   311,    -1,    -1,    99,
     383,   384,   385,    -1,   387,   388,   389,   390,   391,   392,
      -1,    -1,    -1,    -1,   397,    -1,    -1,    -1,    -1,    -1,
    1623,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    -1,    -1,  1639,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    99,    -1,   373,    -1,
      -1,    -1,    -1,  2190,    -1,    -1,    -1,    -1,   383,   384,
     385,  1674,   387,   388,   389,   390,   391,   392,    -1,    -1,
      -1,    -1,   397,    -1,  2211,    -1,    -1,  1690,    -1,  1692,
    1693,   191,    -1,  2220,    -1,   195,    -1,    -1,    -1,  1702,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   212,   213,    -1,    -1,    -1,    -1,  1721,    -1,
      -1,    -1,   222,  1726,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  2218,  1736,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1748,   246,  1750,   191,   249,
      -1,  1754,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     260,    -1,   262,    -1,    -1,    -1,    -1,    -1,    -1,   212,
     213,    -1,    -1,    -1,    -1,    -1,  2520,  1768,    -1,   222,
      -1,    -1,    -1,  1774,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1800,    -1,    -1,
      -1,    -1,    -1,   246,    -1,    -1,   249,    -1,    -1,    -1,
      -1,   311,    -1,    -1,    -1,    -1,    -1,   260,    -1,   262,
    2347,  2348,  2349,  2350,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  2359,    -1,    -1,    -1,  2363,    -1,   483,    -1,
    2367,    -1,  1845,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  2337,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   506,    -1,    -1,    -1,    -1,    -1,    -1,   311,    -1,
      -1,    -1,    -1,   373,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   383,   384,   385,    -1,   387,   388,   389,
     390,   391,   392,   538,    -1,    -1,    -1,   397,    -1,   544,
      -1,  2645,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1902,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1915,    -1,    -1,    -1,    -1,    -1,
     373,    -1,    -1,    -1,    -1,  1926,  2463,    -1,    -1,    -1,
     383,   384,   385,    -1,   387,   388,   389,   390,   391,   392,
      -1,    -1,   395,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1961,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1982,
      -1,  1984,    -1,    -1,  1987,   630,    -1,    -1,    -1,    -1,
      -1,  1994,    -1,    -1,  2738,    -1,    -1,  2000,    -1,  2002,
      -1,  1992,    -1,   648,  2007,    -1,    -1,  2534,    -1,    -1,
      -1,    -1,  2539,    -1,    76,    -1,  2543,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   669,    -1,    88,    -1,    -1,    91,
    2033,  2034,    94,    -1,  2520,    -1,    -1,    99,    -1,    -1,
      -1,    -1,    -1,    -1,  2788,    -1,    -1,  2050,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  2582,    -1,  2584,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  2068,    -1,    -1,  2071,    -1,
      -1,  2598,    -1,    -1,    -1,  2078,  2603,    -1,    -1,    -1,
    2083,  2608,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   154,    -1,    -1,  2622,    -1,    -1,    -1,    -1,
      -1,  2092,  2093,  2094,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  2640,    -1,  2118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  2650,    -1,    -1,    -1,    -1,    -1,   191,
      -1,    -1,    -1,   195,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  2676,
     212,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  2645,
     222,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,   814,
      -1,    -1,    -1,   818,    -1,    -1,    -1,    -1,    -1,    -1,
      88,    -1,    -1,    -1,   246,    -1,    94,   249,    -1,    -1,
      -1,    99,    -1,    -1,   839,    -1,    -1,    -1,   260,  2190,
     262,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  2742,    -1,    -1,    -1,    -1,
    2211,    -1,    -1,    -1,    -1,    -1,    -1,  2754,    -1,  2220,
     875,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   888,    -1,    -1,    -1,    -1,    -1,   311,
      -1,    -1,  2738,    -1,    -1,    -1,    -1,    -1,  2785,    -1,
      -1,    -1,    -1,    -1,    -1,  2268,    -1,    -1,   176,    -1,
      -1,    -1,    -1,   181,    -1,  2278,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,    -1,    -1,    -1,   195,    -1,    -1,
     352,    -1,    -1,    -1,    -1,    -1,    -1,   942,    -1,    -1,
      -1,    -1,  2788,    -1,   212,   213,    -1,    -1,    -1,    -1,
      -1,   373,    -1,    -1,   222,    -1,    -1,    -1,    -1,   964,
      -1,   383,   384,   385,    -1,   387,   388,   389,   390,   391,
     392,    -1,  2335,    -1,    -1,    -1,    -1,    -1,   246,  2342,
    2343,   249,    -1,  2346,   989,   990,    -1,   992,   993,   994,
      -1,    -1,   260,    -1,   262,    -1,  2347,  2348,  2349,  2350,
      -1,    -1,    -1,    -1,    -1,  1010,    -1,    -1,  2359,    -1,
      -1,    -1,  2363,    -1,    -1,    -1,  2367,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   311,    -1,    -1,    -1,    -1,    -1,    -1,
    2413,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,  1074,
      -1,    -1,  2435,    -1,    -1,    -1,  1081,    -1,  1083,    88,
    1085,    90,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      99,    -1,  1097,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   373,  1111,    -1,    -1,    -1,
    1115,    -1,  2463,    -1,    -1,   383,   384,   385,    -1,   387,
     388,   389,   390,   391,   392,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  2504,  1147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  2524,    -1,    -1,    -1,    -1,    -1,    -1,  1173,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   191,  2534,    -1,    -1,   195,  2550,  2539,    -1,
      -1,    -1,  2543,  1198,    -1,  1200,    -1,  1202,    -1,    -1,
    1205,    -1,    -1,   212,   213,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   222,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1231,    -1,    -1,    -1,
      -1,  2582,    -1,  2584,    -1,    -1,    -1,   246,    -1,    -1,
     249,    -1,    -1,    -1,    -1,    -1,    -1,  2598,    -1,    -1,
      -1,   260,  2603,   262,  1259,    -1,    -1,  2608,    -1,  1264,
      -1,    -1,  1267,  1268,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  2622,    -1,  2636,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1287,    -1,    -1,    -1,  1291,  1292,  1293,  2640,
    1295,    -1,    -1,    -1,    -1,    -1,    -1,  1302,  1303,  2650,
      -1,    -1,   311,    -1,  1309,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  2676,    -1,    -1,    -1,  1334,
    1335,  1336,    -1,  1338,  1339,    -1,    -1,    -1,  1343,    -1,
      -1,    -1,  1347,    -1,    -1,  1350,  1351,  1352,  1353,  1354,
    1355,  1356,  1357,  1358,  1359,    -1,    -1,    -1,  1363,    -1,
      -1,    -1,  1367,    -1,   373,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   383,   384,   385,    -1,   387,   388,
     389,   390,   391,   392,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  2742,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  2754,    88,    -1,    90,  1412,    -1,    -1,
      94,    -1,    -1,    -1,    -1,    99,     5,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,  2785,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1452,    -1,    -1,
      39,    40,    -1,    -1,    -1,    -1,    45,    -1,    47,    48,
      -1,    -1,    -1,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    -1,    75,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1507,    -1,  1509,    -1,    -1,   191,    -1,    -1,
      -1,   195,    -1,    -1,    -1,    -1,  1521,  1522,    -1,    -1,
      -1,    -1,    -1,   112,    -1,    -1,  1531,    -1,   212,   213,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   222,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   246,    -1,    -1,   249,    -1,    -1,    -1,   158,
      -1,    -1,    -1,    -1,    -1,  1580,   260,    -1,   262,    -1,
      -1,    -1,  1587,    -1,    -1,    -1,   175,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,  1623,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   311,    -1,    -1,
      -1,    -1,    -1,    -1,  1639,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   250,    -1,    -1,    -1,    -1,  1671,  1672,  1673,  1674,
    1675,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1690,  1691,  1692,  1693,   373,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1702,    -1,   383,
     384,   385,    -1,   387,   388,   389,   390,   391,   392,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1721,    -1,    -1,    -1,
      -1,  1726,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1736,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1747,  1748,    -1,  1750,    -1,    -1,    -1,  1754,
      -1,    -1,    -1,    -1,    -1,   344,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   356,    -1,  1774,
      -1,  1776,  1777,    -1,   483,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   371,   372,    -1,   374,   375,   376,    -1,    -1,
      -1,   380,    -1,   382,   383,  1800,    -1,   506,   387,   388,
     389,   390,   391,    -1,    -1,   394,   395,   396,   397,    -1,
     399,   400,   401,   402,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   538,
      -1,    -1,    -1,    -1,    -1,   544,    -1,    -1,    -1,    -1,
    1845,    -1,    -1,    -1,    -1,    -1,  1851,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1878,  1879,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1906,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1924,
      -1,   630,    -1,    88,  1929,    -1,    -1,    -1,    -1,    94,
      -1,    -1,    -1,    -1,    99,  1940,    -1,    -1,    -1,   648,
    1945,    -1,  1947,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1961,    -1,    -1,    -1,
     669,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1982,    -1,  1984,
      -1,    76,  1987,    -1,    -1,  1990,    -1,    -1,    -1,  1994,
      -1,    -1,    -1,    88,    -1,  2000,    -1,  2002,    -1,    94,
      -1,    -1,  2007,    -1,    99,    -1,    -1,    -1,    -1,    -1,
    2015,  2016,    -1,    76,  2019,  2020,  2021,  2022,  2023,  2024,
    2025,  2026,  2027,  2028,    -1,    88,   191,    90,  2033,  2034,
     195,    94,    -1,    -1,    -1,    -1,    99,  2042,    -1,    -1,
      -1,    -1,  2047,    -1,    -1,  2050,    -1,   212,   213,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   222,    -1,    -1,
      -1,  2066,  2067,  2068,    -1,    -1,  2071,    -1,    -1,    -1,
      -1,    -1,  2077,  2078,    -1,    -1,    -1,    -1,  2083,    -1,
      -1,   246,    -1,    -1,   249,    -1,    -1,    -1,    -1,  2094,
      -1,    -1,    -1,    -1,    -1,   260,   191,   262,    -1,    -1,
     195,    -1,    -1,    -1,    -1,   814,    -1,    -1,    -1,   818,
      -1,    -1,    -1,  2118,    -1,    -1,    -1,   212,   213,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   222,   191,    -1,
     839,    -1,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   311,    -1,    -1,   212,
     213,   246,    -1,    -1,   249,    -1,    -1,    -1,    -1,   222,
      -1,    -1,    -1,    -1,    -1,   260,   875,   262,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   273,   888,
      -1,    -1,    -1,   246,    -1,    -1,   249,   352,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   260,    -1,   262,
    2205,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   373,    -1,
      -1,    -1,    -1,  2218,    -1,    -1,   311,    -1,   383,   384,
     385,    -1,   387,   388,   389,   390,   391,   392,    -1,    -1,
      -1,    -1,    -1,   942,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   311,    -1,
      -1,    -1,    -1,    -1,    -1,   964,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  2268,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  2278,    -1,    -1,    -1,    -1,   373,    -1,
     989,   990,    -1,   992,   993,   994,    -1,    -1,   383,   384,
     385,    -1,   387,   388,   389,   390,   391,   392,    -1,    -1,
      76,  1010,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     373,    -1,    88,    -1,    -1,    -1,    -1,    -1,    94,    -1,
     383,   384,   385,    99,   387,   388,   389,   390,   391,   392,
    2335,    -1,  2337,    -1,    -1,    -1,  2341,  2342,  2343,    -1,
      -1,  2346,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  2359,    -1,    -1,    -1,  2363,    -1,
      -1,    -1,  2367,    -1,    -1,  1074,    -1,    -1,  2373,    -1,
      -1,    -1,  1081,    -1,  1083,    -1,  1085,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1097,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1111,    -1,    -1,   181,  1115,    -1,  2413,    -1,
      -1,    -1,    -1,    -1,    -1,   191,    -1,    -1,    -1,   195,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    2435,    -1,    -1,    -1,    -1,    -1,   212,   213,  1147,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   222,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1011,  2463,    -1,
      -1,    -1,    -1,    -1,  1173,    -1,    -1,    -1,    -1,    -1,
     246,    -1,    -1,   249,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   260,    -1,   262,    -1,    -1,  1198,
      -1,  1200,    -1,  1202,    -1,    -1,  1205,    -1,    -1,  2504,
      -1,    -1,    -1,    -1,    -1,    -1,  2511,    -1,  2513,    -1,
      -1,    -1,    -1,    -1,    -1,  2520,    -1,    -1,    -1,  2524,
      -1,    -1,  1231,    -1,    -1,    -1,    -1,    -1,    -1,  2534,
      -1,    -1,    -1,    -1,  2539,   311,    -1,    -1,  2543,    -1,
      -1,    -1,    -1,    -1,  2549,  2550,    -1,    -1,    -1,    -1,
    1259,    -1,    -1,    -1,    -1,  1264,    -1,    -1,  1267,  1268,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  2582,  1287,    -1,
      -1,    -1,  1291,  1292,  1293,    -1,  1295,    -1,    -1,    -1,
      -1,    -1,    -1,  1302,  1303,    -1,    -1,   373,    -1,    -1,
    1309,    -1,    -1,    -1,    -1,    -1,    -1,   383,   384,   385,
      -1,   387,   388,   389,   390,   391,   392,    -1,    -1,    -1,
      -1,  2626,    -1,    -1,    -1,  1334,  1335,  1336,    -1,  1338,
    1339,  2636,    -1,    -1,  1343,  2640,    -1,    -1,  1347,    -1,
    2645,  1350,  1351,  1352,  1353,  1354,  1355,  1356,  1357,  1358,
    1359,    -1,    -1,    -1,  1363,    -1,    -1,    -1,  1367,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1412,    -1,    -1,    -1,    -1,  1262,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1286,  2738,    -1,    -1,    -1,    -1,  1292,    -1,
      -1,    -1,    -1,  1452,    -1,    -1,    -1,    -1,    -1,  2754,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1312,  1313,
    1314,  1315,    -1,    -1,    -1,  1319,    -1,    -1,    -1,    -1,
       3,    -1,    -1,    -1,    -1,    -1,     9,    -1,    -1,  1333,
    2785,    -1,    -1,  2788,  1338,    18,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1507,    32,
    1509,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1521,  1522,    -1,    -1,    -1,    50,    51,    -1,
      -1,    -1,  1531,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    85,    86,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    -1,    -1,    -1,    98,    -1,    -1,    -1,    -1,
      -1,  1580,    -1,  1427,    -1,    -1,    -1,    -1,  1587,    -1,
      -1,    -1,    -1,   116,    -1,   118,   119,    -1,    -1,    -1,
     123,   124,    -1,    -1,    -1,    -1,    -1,   130,   131,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,    -1,   146,  1623,    -1,    -1,    -1,   151,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,    -1,
    1639,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,    -1,
      -1,   174,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   184,    -1,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1671,  1672,  1673,  1674,  1675,    -1,    -1,    -1,
      -1,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1690,  1691,  1692,  1693,    -1,    -1,    -1,    -1,    -1,
      -1,   224,   225,  1702,    -1,   228,    -1,    -1,    -1,    -1,
      -1,    -1,   235,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1721,    -1,    -1,   248,    -1,  1726,    -1,    -1,
      -1,    -1,  1576,    -1,    -1,    -1,    -1,  1736,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1747,  1748,
      -1,  1750,    -1,   276,    -1,  1754,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   286,    -1,    -1,    -1,   290,    -1,    -1,
     293,    -1,   295,    -1,    -1,  1774,    -1,  1776,  1777,   302,
      -1,    -1,    -1,    -1,   307,    -1,    -1,   310,    -1,    -1,
      -1,    -1,  1636,   316,    -1,    -1,    -1,    -1,  1642,    -1,
    1644,  1800,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   340,    -1,   342,
    1664,    -1,    -1,    -1,   347,  1669,   349,  1671,  1672,  1673,
     353,  1675,  1676,    -1,    -1,    -1,    -1,  1681,  1682,    -1,
      -1,    -1,    -1,    -1,    -1,  1689,  1845,    -1,    -1,    -1,
    1694,    -1,  1851,    -1,    -1,  1699,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   396,    -1,    -1,    -1,    -1,    -1,  1878,
    1879,    -1,    -1,    -1,    -1,    -1,    -1,  1731,    -1,    -1,
    1734,  1735,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1746,  1747,    -1,    -1,    -1,  1906,    -1,  1753,
      -1,  1755,  1756,  1757,  1758,  1759,  1760,  1761,  1762,  1763,
    1764,    -1,    -1,    -1,    -1,  1924,    -1,    -1,    -1,    -1,
    1929,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1940,    -1,    -1,    -1,    -1,  1945,    -1,  1947,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1961,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1982,    -1,  1984,    -1,    -1,  1987,    -1,
      -1,  1990,    -1,    -1,    -1,  1994,    -1,    -1,    -1,    -1,
      -1,  2000,    -1,  2002,    -1,    -1,    -1,    -1,  2007,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  2015,  2016,    -1,    76,
    2019,  2020,  2021,  2022,  2023,  2024,  2025,  2026,  2027,  2028,
      -1,    88,    -1,    -1,  2033,  2034,  1880,    94,    -1,    -1,
      -1,    -1,    99,  2042,    -1,    -1,    -1,    -1,  2047,    -1,
      -1,  2050,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  2066,  2067,  2068,
      -1,    -1,  2071,    -1,    -1,    -1,    76,    -1,  2077,  2078,
      -1,    -1,    -1,    -1,  2083,    -1,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    94,  2094,    -1,    -1,    -1,    99,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  2118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   191,    -1,    -1,    -1,   195,    -1,
      -1,    -1,  1986,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   222,  2010,  2011,  2012,  2013,
    2014,    -1,  2016,    -1,    -1,  2019,  2020,  2021,  2022,  2023,
    2024,  2025,  2026,  2027,  2028,    -1,    -1,    -1,    -1,   246,
      -1,   191,   249,    -1,    -1,   195,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   260,    -1,   262,  2205,  2051,    -1,    -1,
      -1,    -1,   212,   213,    -1,    -1,    -1,    -1,    -1,  2218,
    2064,    -1,   222,  2067,    -1,  2069,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     5,    -1,  2079,  2080,    -1,  2082,    -1,
      -1,  2085,    -1,    -1,    -1,    -1,   246,    19,    -1,   249,
      -1,    -1,    -1,    -1,   311,    -1,    28,    -1,    -1,    -1,
     260,    -1,   262,    -1,    -1,    -1,    -1,    39,    40,  2268,
      -1,    -1,   329,    -1,    -1,    47,    48,    -1,    -1,  2278,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   311,  2156,    -1,    -1,    -1,   373,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   383,   384,   385,    -1,
     387,   388,   389,   390,   391,   392,  2335,    -1,  2337,    -1,
     112,    -1,  2341,  2342,  2343,    -1,    -1,  2346,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    2359,  2205,    -1,    -1,  2363,    -1,    -1,    -1,  2367,    -1,
      -1,    -1,    -1,   373,  2373,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   383,   384,   385,   158,   387,   388,   389,
     390,   391,   392,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  2413,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  2275,  2276,    -1,   206,  2279,  2435,    -1,    -1,  2283,
      -1,  2285,  2286,    -1,    -1,  2289,    -1,    -1,  2292,    -1,
      -1,    -1,  2296,  2297,  2298,  2299,  2300,  2301,  2302,  2303,
    2304,  2305,  2306,  2307,  2463,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  2318,    -1,    -1,    -1,   250,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  2332,  2333,
      -1,    -1,  2336,    -1,    -1,    -1,  2340,  2341,    -1,    -1,
    2344,    -1,    -1,    -1,    -1,  2504,    -1,    -1,    -1,    -1,
      -1,    -1,  2511,    -1,  2513,    -1,    -1,    -1,    -1,    -1,
      -1,  2520,    -1,    -1,    -1,  2524,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  2534,    -1,    -1,    -1,  2383,
    2539,    -1,    -1,    -1,  2543,    -1,    -1,    -1,    -1,    -1,
    2549,  2550,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   344,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  2582,   356,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  2438,    -1,    -1,    -1,    -1,   371,
     372,    -1,   374,   375,    -1,    -1,    -1,    -1,   380,    -1,
     382,   383,    -1,    -1,    -1,   387,   388,   389,   390,   391,
      -1,    -1,   394,   395,   396,    -1,    -1,  2626,   400,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,  2636,    -1,    -1,
      -1,  2640,     9,    -1,    -1,    -1,  2645,    -1,    -1,    -1,
      -1,    18,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,
      -1,  2505,    -1,    -1,    -1,    32,    -1,  2511,    -1,  2513,
      -1,    -1,    -1,  2517,    -1,    -1,    -1,  2521,  2522,  2523,
      -1,  2525,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,
      87,    -1,    -1,    -1,    -1,  2569,    -1,    -1,    -1,    -1,
      -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  2738,
      -1,    -1,  2586,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,   118,   119,    -1,    -1,  2754,   123,   124,    -1,    -1,
      -1,    -1,    -1,   130,   131,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,   146,
      -1,  2625,  2626,  2627,   151,    -1,  2785,  2631,    -1,  2788,
      -1,    -1,   159,    -1,    -1,    -1,    -1,   164,    -1,    -1,
      -1,   168,    -1,   170,  2648,    -1,    -1,   174,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   204,    -1,    -1,
      -1,    -1,    -1,    -1,  2688,    -1,    -1,    -1,  2692,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,   225,    -1,
      -1,   228,    -1,    -1,    -1,    -1,    -1,    -1,   235,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   248,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   276,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   286,
      -1,    -1,    -1,   290,    -1,    -1,   293,    -1,   295,    -1,
      -1,    -1,    -1,    -1,    -1,   302,    -1,    -1,    -1,    -1,
     307,    -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,   316,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   340,    -1,   342,    -1,    -1,    -1,    -1,
     347,    -1,   349,    -1,    -1,    -1,   353,    -1,    -1,     5,
       6,     7,     8,     9,    -1,    11,    12,    -1,    14,    15,
      16,    -1,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    -1,    35,
      36,    37,    38,    39,    40,    -1,    42,    43,    44,   396,
      46,    47,    48,    -1,    50,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,    -1,   104,   105,
     106,    -1,   108,   109,   110,   111,   112,   113,   114,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
      -1,   127,   128,   129,   130,    -1,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,    -1,    -1,   149,    -1,   151,   152,   153,    -1,   155,
      -1,   157,   158,   159,   160,    -1,   162,   163,    -1,   165,
      -1,   167,   168,   169,   170,    -1,   172,   173,   174,   175,
      -1,   177,    -1,   179,   180,    -1,   182,   183,    -1,   185,
      -1,    -1,   188,    -1,   190,   191,   192,   193,   194,   195,
     196,   197,   198,    -1,   200,   201,    -1,   203,   204,   205,
     206,   207,    -1,   209,    -1,    -1,   212,   213,   214,   215,
     216,   217,   218,    -1,   220,   221,   222,    -1,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,    -1,    -1,
     256,   257,    -1,    -1,   260,   261,    -1,    -1,   264,   265,
     266,   267,   268,   269,   270,   271,   272,    -1,   274,   275,
     276,    -1,   278,   279,   280,   281,   282,   283,   284,    -1,
     286,   287,   288,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   298,   299,   300,    -1,    -1,   303,   304,   305,
     306,   307,   308,   309,   310,   311,   312,   313,    -1,   315,
     316,   317,   318,   319,   320,   321,    -1,   323,   324,    -1,
     326,   327,   328,    -1,   330,   331,    -1,   333,    -1,   335,
     336,   337,   338,   339,   340,   341,   342,   343,    -1,   345,
     346,   347,   348,   349,   350,   351,    -1,   353,   354,   355,
     356,   357,   358,   359,   360,    -1,    -1,    -1,   364,   365,
     366,   367,   368,   369,    -1,   371,   372,   373,   374,   375,
      -1,    -1,   378,   379,   380,   381,   382,    -1,    -1,    -1,
      -1,   387,   388,   389,    -1,   391,   392,    -1,    -1,    -1,
     396,   397,     5,     6,     7,     8,     9,    -1,    11,    12,
      -1,    14,    15,    16,    -1,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    -1,    35,    36,    37,    38,    39,    40,    -1,    42,
      43,    44,    -1,    46,    47,    48,    -1,    50,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    78,    79,    80,    81,    82,
      83,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
      -1,   104,   105,   106,    -1,   108,   109,   110,   111,   112,
     113,   114,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,    -1,   127,   128,   129,   130,    -1,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    -1,    -1,   149,    -1,   151,   152,
     153,    -1,    -1,    -1,   157,   158,   159,   160,    -1,   162,
     163,    -1,   165,    -1,   167,   168,   169,   170,    -1,   172,
     173,   174,   175,    -1,   177,    -1,   179,   180,    -1,   182,
     183,    -1,   185,    -1,    -1,   188,    -1,   190,   191,   192,
     193,   194,   195,   196,   197,   198,    -1,   200,   201,    -1,
     203,   204,   205,   206,   207,    -1,   209,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,    -1,   220,   221,   222,
      -1,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,    -1,    -1,   256,   257,    -1,    -1,   260,   261,    -1,
      -1,   264,   265,   266,   267,   268,   269,   270,   271,   272,
      -1,   274,   275,   276,    -1,   278,   279,   280,   281,   282,
     283,   284,    -1,   286,   287,   288,   289,   290,   291,   292,
     293,   294,   295,   296,   297,   298,   299,   300,    -1,    -1,
     303,   304,   305,   306,   307,   308,   309,   310,   311,   312,
     313,    -1,   315,   316,   317,   318,   319,   320,   321,    -1,
     323,   324,    -1,   326,   327,   328,    -1,   330,   331,    -1,
     333,    -1,   335,   336,   337,   338,   339,   340,   341,   342,
     343,    -1,   345,   346,   347,   348,   349,   350,   351,    -1,
     353,   354,   355,   356,   357,   358,   359,   360,    -1,    -1,
      -1,   364,   365,   366,   367,   368,   369,    -1,   371,   372,
     373,   374,   375,    -1,    -1,   378,   379,   380,   381,   382,
      -1,    -1,    -1,    -1,   387,   388,    -1,    -1,   391,   392,
      -1,    -1,    -1,   396,   397,     5,     6,     7,     8,     9,
      -1,    11,    12,    -1,    14,    15,    16,    -1,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    -1,    35,    36,    37,    38,    39,
      40,    -1,    42,    43,    44,    -1,    46,    47,    48,    -1,
      50,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    78,    79,
      80,    81,    82,    83,    -1,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,    -1,   104,   105,   106,    -1,   108,   109,
     110,   111,   112,   113,   114,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,    -1,   127,   128,   129,
     130,    -1,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,    -1,   149,
      -1,   151,   152,   153,    -1,    -1,    -1,   157,   158,   159,
     160,    -1,   162,   163,    -1,   165,    -1,   167,   168,   169,
     170,    -1,   172,   173,   174,   175,    -1,   177,    -1,   179,
     180,    -1,   182,   183,    -1,   185,    -1,    -1,   188,    -1,
     190,   191,   192,   193,   194,   195,   196,   197,   198,    -1,
     200,   201,    -1,   203,   204,   205,   206,   207,    -1,   209,
      -1,    -1,   212,   213,   214,   215,   216,   217,   218,    -1,
     220,   221,   222,    -1,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,    -1,    -1,   256,   257,    -1,    -1,
     260,   261,    -1,    -1,   264,   265,   266,   267,   268,   269,
     270,   271,   272,    -1,   274,   275,   276,    -1,   278,   279,
     280,   281,   282,   283,   284,    -1,   286,   287,   288,   289,
     290,   291,   292,   293,   294,   295,   296,   297,   298,   299,
     300,    -1,    -1,   303,   304,   305,   306,   307,   308,   309,
     310,   311,   312,   313,    -1,   315,   316,   317,   318,   319,
     320,   321,    -1,   323,   324,    -1,   326,   327,   328,    -1,
     330,   331,    -1,   333,    -1,   335,   336,   337,   338,   339,
     340,   341,   342,   343,    -1,   345,   346,   347,   348,   349,
     350,   351,    -1,   353,   354,   355,   356,   357,   358,   359,
     360,    -1,    -1,    -1,   364,   365,   366,   367,   368,   369,
      -1,   371,   372,   373,   374,   375,    -1,    -1,   378,   379,
     380,   381,   382,    -1,    -1,    -1,    -1,   387,   388,    -1,
      -1,   391,   392,    -1,    -1,    -1,   396,   397,     5,     6,
       7,     8,     9,    -1,    11,    12,    -1,    14,    15,    16,
      -1,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    -1,    35,    36,
      37,    38,    39,    40,    -1,    42,    43,    44,    -1,    46,
      47,    48,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    94,    -1,    96,
      97,    98,    -1,   100,    -1,   102,    -1,   104,   105,   106,
      -1,   108,    -1,    -1,   111,   112,   113,   114,    -1,   116,
     117,   118,   119,    -1,    -1,    -1,   123,   124,   125,    -1,
     127,   128,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,   149,    -1,   151,   152,   153,    -1,    -1,    -1,
     157,   158,   159,   160,    -1,   162,   163,    -1,   165,    -1,
     167,   168,    -1,   170,    -1,    -1,    -1,   174,   175,    -1,
     177,    -1,   179,    -1,    -1,    -1,   183,    -1,   185,    -1,
      -1,   188,    -1,   190,    -1,   192,   193,   194,   195,   196,
     197,   198,    -1,    -1,   201,    -1,   203,   204,   205,   206,
     207,    -1,   209,    -1,    -1,    -1,    -1,   214,    -1,   216,
     217,   218,    -1,    -1,   221,    -1,    -1,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,    -1,   239,    -1,   241,   242,   243,   244,    -1,    -1,
     247,   248,    -1,    -1,    -1,   252,   253,    -1,    -1,   256,
      -1,    -1,    -1,   260,   261,    -1,    -1,   264,    -1,    -1,
      -1,   268,   269,   270,   271,   272,    -1,    -1,   275,   276,
      -1,   278,   279,   280,   281,   282,   283,   284,    -1,   286,
     287,   288,   289,   290,   291,   292,   293,    -1,   295,    -1,
     297,   298,   299,   300,    -1,    -1,   303,   304,   305,    -1,
     307,   308,   309,   310,    -1,   312,   313,    -1,   315,   316,
     317,   318,   319,   320,   321,    -1,    -1,   324,    -1,   326,
     327,   328,    -1,   330,   331,    -1,   333,    -1,   335,    -1,
     337,    -1,    -1,   340,   341,   342,   343,    -1,    -1,   346,
     347,   348,   349,   350,    -1,    -1,   353,   354,   355,   356,
     357,     5,   359,   360,    -1,    -1,    -1,   364,   365,   366,
     367,   368,   369,    -1,   371,    19,    -1,   374,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    40,    -1,    -1,    -1,
     397,    -1,    -1,    47,    48,    -1,    -1,    -1,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
       5,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    39,    40,    -1,    -1,   112,    -1,
      -1,    -1,    47,    48,    -1,    -1,    -1,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    -1,
      75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   175,    -1,    -1,    -1,    -1,    -1,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     175,    -1,    -1,    -1,    -1,    -1,   250,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   250,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     344,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   356,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   371,   372,    -1,
     374,   375,    -1,    -1,    -1,    -1,   380,    -1,   382,   383,
      -1,    -1,    -1,   387,   388,   389,   390,   391,    -1,    -1,
     394,   395,   396,   397,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   344,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   356,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   371,   372,    -1,   374,
     375,    -1,    -1,    -1,    -1,   380,    -1,   382,   383,    -1,
      -1,    -1,   387,   388,   389,   390,   391,    -1,    -1,   394,
     395,   396,   397,     5,     6,     7,     8,     9,    -1,    11,
      12,    -1,    14,    15,    16,    -1,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    -1,    35,    36,    37,    38,    39,    40,    -1,
      42,    43,    44,    -1,    46,    47,    48,    -1,    50,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    78,    79,    80,    81,
      82,    83,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,    -1,   108,   109,   110,   111,
     112,   113,   114,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,    -1,   127,   128,   129,   130,    -1,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,    -1,    -1,   149,    -1,   151,
     152,   153,    -1,    -1,    -1,   157,   158,   159,   160,    -1,
     162,   163,    -1,   165,    -1,   167,   168,   169,   170,    -1,
     172,   173,   174,   175,    -1,   177,    -1,   179,   180,   181,
     182,   183,    -1,   185,    -1,    -1,   188,    -1,   190,   191,
     192,   193,   194,   195,   196,   197,   198,    -1,   200,   201,
      -1,   203,   204,   205,   206,   207,    -1,   209,    -1,    -1,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,    -1,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,    -1,    -1,   256,   257,    -1,    -1,   260,   261,
      -1,    -1,   264,   265,   266,   267,   268,   269,   270,   271,
     272,    -1,   274,   275,   276,    -1,   278,   279,   280,   281,
     282,   283,   284,    -1,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   298,   299,   300,    -1,
      -1,   303,   304,   305,   306,   307,   308,   309,   310,   311,
     312,   313,    -1,   315,   316,   317,   318,   319,   320,   321,
      -1,   323,   324,    -1,   326,   327,   328,    -1,   330,   331,
      -1,   333,   334,   335,   336,   337,   338,   339,   340,   341,
     342,   343,    -1,   345,   346,   347,   348,   349,   350,   351,
      -1,   353,   354,   355,   356,   357,   358,   359,   360,    -1,
      -1,    -1,   364,   365,   366,   367,   368,   369,    -1,   371,
     372,   373,   374,   375,    -1,    -1,   378,   379,   380,   381,
     382,    -1,    -1,    -1,    -1,   387,   388,    -1,    -1,   391,
     392,    -1,    -1,    -1,   396,     5,     6,     7,     8,     9,
      -1,    11,    12,    -1,    14,    15,    16,    -1,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    -1,    35,    36,    37,    38,    39,
      40,    -1,    42,    43,    44,    -1,    46,    47,    48,    -1,
      50,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    78,    79,
      80,    81,    82,    83,    -1,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,    -1,   104,   105,   106,    -1,   108,   109,
     110,   111,   112,   113,   114,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,    -1,   127,   128,   129,
     130,    -1,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,    -1,   149,
      -1,   151,   152,   153,    -1,    -1,    -1,   157,   158,   159,
     160,    -1,   162,   163,    -1,   165,    -1,   167,   168,   169,
     170,    -1,   172,   173,   174,   175,    -1,   177,    -1,   179,
     180,    -1,   182,   183,    -1,   185,    -1,    -1,   188,    -1,
     190,   191,   192,   193,   194,   195,   196,   197,   198,    -1,
     200,   201,    -1,   203,   204,   205,   206,   207,    -1,   209,
      -1,    -1,   212,   213,   214,   215,   216,   217,   218,    -1,
     220,   221,   222,    -1,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,    -1,    -1,   256,   257,    -1,    -1,
     260,   261,    -1,    -1,   264,   265,   266,   267,   268,   269,
     270,   271,   272,    -1,   274,   275,   276,    -1,   278,   279,
     280,   281,   282,   283,   284,    -1,   286,   287,   288,   289,
     290,   291,   292,   293,   294,   295,   296,   297,   298,   299,
     300,    -1,    -1,   303,   304,   305,   306,   307,   308,   309,
     310,   311,   312,   313,    -1,   315,   316,   317,   318,   319,
     320,   321,    -1,   323,   324,    -1,   326,   327,   328,    -1,
     330,   331,    -1,   333,    -1,   335,   336,   337,   338,   339,
     340,   341,   342,   343,    -1,   345,   346,   347,   348,   349,
     350,   351,    -1,   353,   354,   355,   356,   357,   358,   359,
     360,    -1,    -1,    -1,   364,   365,   366,   367,   368,   369,
      -1,   371,   372,   373,   374,   375,    -1,    -1,   378,   379,
     380,   381,   382,    -1,    -1,    -1,    -1,   387,   388,   389,
      -1,   391,   392,    -1,    -1,    -1,   396,     5,     6,     7,
       8,     9,    -1,    11,    12,    -1,    14,    15,    16,    -1,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    -1,    35,    36,    37,
      38,    39,    40,    -1,    42,    43,    44,    -1,    46,    47,
      48,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      78,    79,    80,    81,    82,    83,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,    -1,   104,   105,   106,    -1,
     108,   109,   110,   111,   112,   113,   114,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,    -1,   127,
     128,   129,   130,    -1,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,    -1,
      -1,   149,    -1,   151,   152,   153,    -1,    -1,    -1,   157,
     158,   159,   160,    -1,   162,   163,    -1,   165,    -1,   167,
     168,   169,   170,    -1,   172,   173,   174,   175,    -1,   177,
      -1,   179,   180,    -1,   182,   183,    -1,   185,    -1,    -1,
     188,    -1,   190,   191,   192,   193,   194,   195,   196,   197,
     198,    -1,   200,   201,    -1,   203,   204,   205,   206,   207,
      -1,   209,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,    -1,   220,   221,   222,    -1,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,    -1,    -1,   256,   257,
      -1,    -1,   260,   261,    -1,    -1,   264,   265,   266,   267,
     268,   269,   270,   271,   272,    -1,   274,   275,   276,    -1,
     278,   279,   280,   281,   282,   283,   284,    -1,   286,   287,
     288,   289,   290,   291,   292,   293,   294,   295,   296,   297,
     298,   299,   300,    -1,    -1,   303,   304,   305,   306,   307,
     308,   309,   310,   311,   312,   313,    -1,   315,   316,   317,
     318,   319,   320,   321,    -1,   323,   324,    -1,   326,   327,
     328,    -1,   330,   331,    -1,   333,    -1,   335,   336,   337,
     338,   339,   340,   341,   342,   343,    -1,   345,   346,   347,
     348,   349,   350,   351,    -1,   353,   354,   355,   356,   357,
     358,   359,   360,    -1,    -1,    -1,   364,   365,   366,   367,
     368,   369,    -1,   371,   372,   373,   374,   375,    -1,    -1,
     378,   379,   380,   381,   382,    -1,    -1,    -1,    -1,   387,
     388,   389,    -1,   391,   392,    -1,    -1,    -1,   396,     5,
       6,     7,     8,     9,    -1,    11,    12,    -1,    14,    15,
      16,    -1,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    -1,    35,
      36,    37,    38,    39,    40,    -1,    42,    43,    44,    -1,
      46,    47,    48,    -1,    50,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    78,    79,    80,    81,    82,    83,    -1,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,    -1,   104,   105,
     106,    -1,   108,   109,   110,   111,   112,   113,   114,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
      -1,   127,   128,   129,   130,    -1,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,    -1,    -1,   149,    -1,   151,   152,   153,    -1,    -1,
      -1,   157,   158,   159,   160,    -1,   162,   163,    -1,   165,
      -1,   167,   168,   169,   170,    -1,   172,   173,   174,   175,
      -1,   177,    -1,   179,   180,    -1,   182,   183,    -1,   185,
      -1,    -1,   188,    -1,   190,   191,   192,   193,   194,   195,
     196,   197,   198,    -1,   200,   201,    -1,   203,   204,   205,
     206,   207,    -1,   209,    -1,    -1,   212,   213,   214,   215,
     216,   217,   218,    -1,   220,   221,   222,    -1,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,    -1,    -1,
     256,   257,    -1,    -1,   260,   261,    -1,    -1,   264,   265,
     266,   267,   268,   269,   270,   271,   272,    -1,   274,   275,
     276,    -1,   278,   279,   280,   281,   282,   283,   284,    -1,
     286,   287,   288,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   298,   299,   300,    -1,   302,   303,   304,   305,
     306,   307,   308,   309,   310,   311,   312,   313,    -1,   315,
     316,   317,   318,   319,   320,   321,    -1,   323,   324,    -1,
     326,   327,   328,    -1,   330,   331,    -1,   333,    -1,   335,
     336,   337,   338,   339,   340,   341,   342,   343,    -1,   345,
     346,   347,   348,   349,   350,   351,    -1,   353,   354,   355,
     356,   357,   358,   359,   360,    -1,    -1,    -1,   364,   365,
     366,   367,   368,   369,    -1,   371,   372,   373,   374,   375,
      -1,    -1,   378,   379,   380,   381,   382,    -1,    -1,    -1,
      -1,   387,   388,    -1,    -1,   391,   392,    -1,    -1,    -1,
     396,     5,     6,     7,     8,     9,    -1,    11,    12,    -1,
      14,    15,    16,    -1,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      -1,    35,    36,    37,    38,    39,    40,    -1,    42,    43,
      44,    -1,    46,    47,    48,    -1,    50,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    78,    79,    80,    81,    82,    83,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,    -1,
     104,   105,   106,    -1,   108,   109,   110,   111,   112,   113,
     114,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,    -1,   127,   128,   129,   130,    -1,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,    -1,    -1,   149,    -1,   151,   152,   153,
      -1,    -1,    -1,   157,   158,   159,   160,    -1,   162,   163,
      -1,   165,    -1,   167,   168,   169,   170,    -1,   172,   173,
     174,   175,    -1,   177,    -1,   179,   180,   181,   182,   183,
      -1,   185,    -1,    -1,   188,    -1,   190,   191,   192,   193,
     194,   195,   196,   197,   198,    -1,   200,   201,    -1,   203,
     204,   205,   206,   207,    -1,   209,    -1,    -1,   212,   213,
     214,   215,   216,   217,   218,    -1,   220,   221,   222,    -1,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
      -1,    -1,   256,   257,    -1,    -1,   260,   261,    -1,    -1,
     264,   265,   266,   267,   268,   269,   270,   271,   272,    -1,
     274,   275,   276,    -1,   278,   279,   280,   281,   282,   283,
     284,    -1,   286,   287,   288,   289,   290,   291,   292,   293,
     294,   295,   296,   297,   298,   299,   300,    -1,    -1,   303,
     304,   305,   306,   307,   308,   309,   310,   311,   312,   313,
      -1,   315,   316,   317,   318,   319,   320,   321,    -1,   323,
     324,    -1,   326,   327,   328,    -1,   330,   331,    -1,   333,
      -1,   335,   336,   337,   338,   339,   340,   341,   342,   343,
      -1,   345,   346,   347,   348,   349,   350,   351,    -1,   353,
     354,   355,   356,   357,   358,   359,   360,    -1,    -1,    -1,
     364,   365,   366,   367,   368,   369,    -1,   371,   372,   373,
     374,   375,    -1,    -1,   378,   379,   380,   381,   382,    -1,
      -1,    -1,    -1,   387,   388,    -1,    -1,   391,   392,    -1,
      -1,    -1,   396,     5,     6,     7,     8,     9,    -1,    11,
      12,    -1,    14,    15,    16,    -1,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    -1,    35,    36,    37,    38,    39,    40,    -1,
      42,    43,    44,    -1,    46,    47,    48,    -1,    50,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    78,    79,    80,    81,
      82,    83,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,    -1,   104,   105,   106,    -1,   108,   109,   110,   111,
     112,   113,   114,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,    -1,   127,   128,   129,   130,    -1,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,    -1,    -1,   149,    -1,   151,
     152,   153,    -1,    -1,    -1,   157,   158,   159,   160,    -1,
     162,   163,    -1,   165,    -1,   167,   168,   169,   170,    -1,
     172,   173,   174,   175,    -1,   177,    -1,   179,   180,    -1,
     182,   183,    -1,   185,    -1,    -1,   188,    -1,   190,   191,
     192,   193,   194,   195,   196,   197,   198,    -1,   200,   201,
      -1,   203,   204,   205,   206,   207,    -1,   209,    -1,    -1,
     212,   213,   214,   215,   216,   217,   218,    -1,   220,   221,
     222,    -1,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,    -1,    -1,   256,   257,    -1,    -1,   260,   261,
      -1,    -1,   264,   265,   266,   267,   268,   269,   270,   271,
     272,    -1,   274,   275,   276,    -1,   278,   279,   280,   281,
     282,   283,   284,    -1,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   298,   299,   300,    -1,
      -1,   303,   304,   305,   306,   307,   308,   309,   310,   311,
     312,   313,    -1,   315,   316,   317,   318,   319,   320,   321,
      -1,   323,   324,    -1,   326,   327,   328,    -1,   330,   331,
      -1,   333,    -1,   335,   336,   337,   338,   339,   340,   341,
     342,   343,    -1,   345,   346,   347,   348,   349,   350,   351,
      -1,   353,   354,   355,   356,   357,   358,   359,   360,    -1,
      -1,    -1,   364,   365,   366,   367,   368,   369,    -1,   371,
     372,   373,   374,   375,    -1,    -1,   378,   379,   380,   381,
     382,    -1,    -1,    -1,    -1,   387,   388,    -1,    -1,   391,
     392,    -1,    -1,    -1,   396,     5,     6,     7,     8,     9,
      -1,    11,    12,    -1,    14,    15,    16,    -1,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    -1,    35,    36,    37,    38,    39,
      40,    -1,    42,    43,    44,    -1,    46,    47,    48,    -1,
      50,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    78,    79,
      80,    81,    82,    83,    -1,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,    -1,   104,   105,   106,    -1,   108,   109,
     110,   111,   112,   113,   114,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,    -1,   127,   128,   129,
     130,    -1,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,    -1,   149,
      -1,   151,   152,   153,    -1,    -1,    -1,   157,   158,   159,
     160,    -1,   162,   163,    -1,   165,    -1,   167,   168,   169,
     170,    -1,   172,   173,   174,   175,    -1,   177,    -1,   179,
     180,    -1,   182,   183,    -1,   185,    -1,    -1,   188,    -1,
     190,   191,   192,   193,   194,   195,   196,   197,   198,    -1,
     200,   201,    -1,   203,   204,   205,   206,   207,    -1,   209,
      -1,    -1,   212,   213,   214,   215,   216,   217,   218,    -1,
     220,   221,   222,    -1,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,    -1,   247,   248,   249,
     250,   251,   252,   253,    -1,    -1,   256,   257,    -1,    -1,
     260,   261,    -1,    -1,   264,   265,   266,   267,   268,   269,
     270,   271,   272,    -1,   274,   275,   276,    -1,   278,   279,
     280,   281,   282,   283,   284,    -1,   286,   287,   288,   289,
     290,   291,   292,   293,   294,   295,   296,   297,   298,   299,
     300,    -1,    -1,   303,   304,   305,   306,   307,   308,   309,
     310,   311,   312,   313,    -1,   315,   316,   317,   318,   319,
     320,   321,    -1,   323,   324,    -1,   326,   327,   328,    -1,
     330,   331,    -1,   333,    -1,   335,   336,   337,   338,   339,
     340,   341,   342,   343,    -1,    -1,   346,   347,   348,   349,
     350,   351,    -1,   353,   354,   355,   356,   357,   358,   359,
     360,    -1,    -1,    -1,   364,   365,   366,   367,   368,   369,
      -1,   371,   372,   373,   374,   375,    -1,    -1,   378,   379,
     380,   381,   382,    -1,    -1,    -1,    -1,    -1,   388,    -1,
      -1,   391,   392,    -1,    -1,    -1,   396,     5,     6,     7,
       8,     9,    -1,    11,    12,    -1,    14,    15,    16,    -1,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    -1,    35,    36,    37,
      38,    39,    40,    -1,    42,    43,    44,    -1,    46,    47,
      48,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      78,    79,    80,    81,    82,    83,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    93,    94,    95,    96,    97,
      98,    -1,   100,   101,   102,    -1,   104,   105,   106,    -1,
     108,   109,   110,   111,   112,   113,   114,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,    -1,   127,
     128,   129,   130,    -1,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,    -1,
      -1,   149,    -1,   151,   152,   153,    -1,    -1,    -1,   157,
     158,   159,   160,    -1,   162,   163,    -1,    -1,    -1,   167,
     168,   169,   170,    -1,   172,   173,   174,   175,    -1,   177,
      -1,   179,   180,    -1,   182,   183,    -1,   185,    -1,    -1,
     188,    -1,   190,    -1,   192,   193,   194,    -1,   196,   197,
     198,    -1,   200,   201,    -1,   203,   204,   205,   206,   207,
      -1,   209,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,    -1,   220,   221,    -1,    -1,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,    -1,   247,
     248,   249,   250,   251,   252,   253,    -1,    -1,   256,   257,
      -1,    -1,   260,   261,    -1,    -1,   264,   265,   266,   267,
     268,   269,   270,   271,   272,    -1,   274,   275,   276,    -1,
     278,   279,   280,   281,   282,   283,   284,    -1,   286,   287,
     288,   289,   290,   291,   292,   293,   294,   295,   296,   297,
     298,   299,   300,    -1,    -1,   303,   304,   305,   306,   307,
     308,   309,   310,    -1,   312,   313,    -1,   315,   316,   317,
     318,   319,   320,   321,    -1,   323,   324,    -1,   326,   327,
     328,    -1,   330,   331,    -1,   333,    -1,   335,   336,   337,
     338,   339,   340,   341,   342,   343,    -1,   345,   346,   347,
     348,   349,   350,   351,    -1,   353,   354,   355,   356,   357,
     358,   359,   360,    -1,    -1,    -1,   364,   365,   366,   367,
     368,   369,    -1,   371,   372,   373,   374,   375,    -1,    -1,
     378,   379,   380,   381,   382,    -1,    -1,    -1,    -1,   387,
     388,    -1,    -1,   391,   392,    -1,    -1,    -1,   396,     5,
       6,     7,     8,     9,    -1,    11,    12,    -1,    14,    15,
      16,    -1,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    -1,    35,
      36,    37,    38,    39,    40,    -1,    42,    43,    44,    -1,
      46,    47,    48,    -1,    50,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    78,    79,    80,    81,    82,    83,    -1,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,    -1,   104,   105,
     106,    -1,   108,   109,   110,   111,   112,   113,   114,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
      -1,   127,   128,   129,   130,    -1,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,    -1,    -1,   149,    -1,   151,   152,   153,    -1,    -1,
      -1,   157,   158,   159,   160,    -1,   162,   163,    -1,   165,
      -1,   167,   168,   169,   170,    -1,   172,   173,   174,   175,
      -1,   177,    -1,   179,   180,    -1,   182,   183,    -1,   185,
      -1,    -1,   188,    -1,   190,   191,   192,   193,   194,   195,
     196,   197,   198,    -1,   200,   201,    -1,   203,   204,   205,
     206,   207,    -1,   209,    -1,    -1,   212,   213,   214,   215,
     216,   217,   218,    -1,   220,   221,   222,    -1,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
      -1,   247,   248,   249,   250,   251,   252,   253,    -1,    -1,
     256,   257,    -1,    -1,   260,   261,    -1,    -1,   264,   265,
     266,   267,   268,   269,   270,   271,   272,    -1,   274,   275,
     276,    -1,   278,   279,   280,   281,   282,   283,   284,    -1,
     286,   287,   288,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   298,   299,   300,    -1,    -1,   303,   304,   305,
     306,   307,   308,   309,   310,   311,   312,   313,    -1,   315,
     316,   317,   318,   319,   320,   321,    -1,   323,   324,    -1,
     326,   327,   328,    -1,   330,   331,    -1,   333,    -1,   335,
     336,   337,   338,   339,   340,   341,   342,   343,    -1,    -1,
     346,   347,   348,   349,   350,   351,    -1,   353,   354,   355,
     356,   357,   358,   359,   360,    -1,    -1,    -1,   364,   365,
     366,   367,   368,   369,    -1,   371,   372,    -1,   374,   375,
      -1,    -1,   378,   379,   380,   381,   382,    -1,     5,     6,
       7,     8,     9,    -1,    11,    12,    -1,    14,    15,    16,
     396,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    -1,    35,    36,
      37,    38,    39,    40,    -1,    42,    43,    44,    -1,    46,
      47,    48,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    94,    95,    96,
      97,    98,    -1,   100,   101,   102,    -1,   104,   105,   106,
      -1,   108,   109,   110,   111,   112,   113,   114,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,    -1,
     127,   128,   129,   130,    -1,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,   149,    -1,   151,   152,   153,    -1,    -1,    -1,
     157,   158,   159,   160,    -1,   162,   163,    -1,    -1,    -1,
     167,   168,   169,   170,    -1,   172,   173,   174,   175,    -1,
     177,    -1,   179,   180,    -1,   182,   183,    -1,   185,    -1,
      -1,   188,    -1,   190,    -1,   192,   193,   194,    -1,   196,
     197,   198,    -1,   200,   201,    -1,   203,   204,   205,   206,
     207,    -1,   209,    -1,    -1,   212,   213,   214,   215,   216,
     217,   218,    -1,   220,   221,    -1,    -1,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,    -1,
     247,   248,   249,   250,   251,   252,   253,    -1,    -1,   256,
     257,    -1,    -1,    -1,   261,    -1,    -1,   264,   265,   266,
     267,   268,   269,   270,   271,   272,    -1,   274,   275,   276,
      -1,   278,   279,   280,   281,   282,   283,   284,    -1,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,    -1,    -1,   303,   304,   305,   306,
     307,   308,   309,   310,    -1,   312,   313,    -1,   315,   316,
     317,   318,   319,   320,   321,    -1,   323,   324,    -1,   326,
     327,   328,    -1,   330,   331,    -1,   333,    -1,   335,   336,
     337,   338,   339,   340,   341,   342,   343,    -1,    -1,   346,
     347,   348,   349,   350,   351,    -1,   353,   354,   355,   356,
     357,   358,   359,   360,    -1,    -1,    -1,   364,   365,   366,
     367,   368,   369,    -1,   371,   372,    -1,   374,   375,    -1,
      -1,   378,   379,   380,   381,   382,    -1,    -1,    -1,    -1,
      -1,   388,    -1,    -1,   391,   392,    -1,    -1,    -1,   396,
       5,     6,     7,     8,     9,    -1,    11,    12,    -1,    14,
      15,    16,    -1,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    -1,
      35,    36,    37,    38,    39,    40,    -1,    42,    43,    44,
      -1,    46,    47,    48,    -1,    50,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    -1,    -1,    -1,    89,    -1,    -1,    92,    93,    94,
      95,    96,    97,    98,    -1,   100,   101,   102,    -1,   104,
     105,   106,    -1,   108,   109,   110,   111,   112,   113,   114,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,    -1,   127,   128,   129,   130,    -1,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,    -1,    -1,   149,    -1,   151,   152,   153,    -1,
      -1,    -1,   157,   158,   159,   160,    -1,   162,   163,    -1,
      -1,    -1,   167,   168,   169,   170,    -1,   172,   173,   174,
     175,    -1,   177,    -1,   179,   180,    -1,   182,   183,    -1,
     185,    -1,    -1,   188,    -1,   190,    -1,   192,   193,   194,
      -1,   196,   197,   198,    -1,   200,   201,    -1,   203,   204,
     205,   206,   207,    -1,   209,    -1,    -1,    -1,    -1,   214,
     215,   216,   217,   218,    -1,   220,   221,    -1,    -1,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,    -1,   247,   248,    -1,   250,   251,   252,   253,    -1,
      -1,   256,   257,    -1,    -1,    -1,   261,    -1,    -1,   264,
     265,   266,   267,   268,   269,   270,   271,   272,    -1,   274,
     275,   276,    -1,   278,   279,   280,   281,   282,   283,   284,
      -1,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,    -1,    -1,   303,   304,
     305,   306,   307,   308,   309,   310,    -1,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,    -1,   323,   324,
      -1,   326,   327,   328,    -1,   330,   331,    -1,   333,    -1,
     335,   336,   337,   338,   339,   340,   341,   342,   343,    -1,
     345,   346,   347,   348,   349,   350,   351,    -1,   353,   354,
     355,   356,   357,   358,   359,   360,    -1,    -1,    -1,   364,
     365,   366,   367,   368,   369,    -1,   371,   372,    -1,   374,
     375,    -1,    -1,   378,   379,   380,   381,   382,    -1,     5,
       6,     7,     8,     9,    -1,    11,    12,   392,    14,    15,
      16,   396,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    -1,    35,
      36,    37,    38,    39,    40,    -1,    42,    43,    44,    -1,
      46,    47,    48,    -1,    50,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      -1,    -1,    -1,    89,    -1,    -1,    92,    93,    94,    95,
      96,    97,    98,    -1,   100,   101,   102,    -1,   104,   105,
     106,    -1,   108,   109,   110,   111,   112,   113,   114,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
      -1,   127,   128,   129,   130,    -1,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,    -1,    -1,   149,    -1,   151,   152,   153,    -1,    -1,
      -1,   157,   158,   159,   160,    -1,   162,   163,    -1,    -1,
      -1,   167,   168,   169,   170,    -1,   172,   173,   174,   175,
      -1,   177,    -1,   179,   180,    -1,   182,   183,    -1,   185,
      -1,    -1,   188,    -1,   190,    -1,   192,   193,   194,    -1,
     196,   197,   198,    -1,   200,   201,    -1,   203,   204,   205,
     206,   207,    -1,   209,    -1,    -1,    -1,    -1,   214,   215,
     216,   217,   218,    -1,   220,   221,    -1,    -1,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
      -1,   247,   248,    -1,   250,   251,   252,   253,    -1,    -1,
     256,   257,    -1,    -1,    -1,   261,    -1,    -1,   264,   265,
     266,   267,   268,   269,   270,   271,   272,    -1,   274,   275,
     276,    -1,   278,   279,   280,   281,   282,   283,   284,    -1,
     286,   287,   288,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   298,   299,   300,    -1,    -1,   303,   304,   305,
     306,   307,   308,   309,   310,    -1,   312,   313,   314,   315,
     316,   317,   318,   319,   320,   321,    -1,   323,   324,    -1,
     326,   327,   328,    -1,   330,   331,    -1,   333,    -1,   335,
     336,   337,   338,   339,   340,   341,   342,   343,    -1,   345,
     346,   347,   348,   349,   350,   351,    -1,   353,   354,   355,
     356,   357,   358,   359,   360,    -1,    -1,    -1,   364,   365,
     366,   367,   368,   369,    -1,   371,   372,    -1,   374,   375,
      -1,    -1,   378,   379,   380,   381,   382,    -1,     5,     6,
       7,     8,     9,    -1,    11,    12,    -1,    14,    15,    16,
     396,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    -1,    35,    36,
      37,    38,    39,    40,    -1,    42,    43,    44,    -1,    46,
      47,    48,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    94,    95,    96,
      97,    98,    -1,   100,   101,   102,    -1,   104,   105,   106,
      -1,   108,   109,   110,   111,   112,   113,   114,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,    -1,
     127,   128,   129,   130,    -1,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,   149,    -1,   151,   152,   153,    -1,    -1,    -1,
     157,   158,   159,   160,    -1,   162,   163,    -1,    -1,    -1,
     167,   168,   169,   170,    -1,   172,   173,   174,   175,    -1,
     177,    -1,   179,   180,    -1,   182,   183,    -1,   185,    -1,
      -1,   188,    -1,   190,    -1,   192,   193,   194,    -1,   196,
     197,   198,    -1,   200,   201,    -1,   203,   204,   205,   206,
     207,    -1,   209,    -1,    -1,    -1,    -1,   214,   215,   216,
     217,   218,    -1,   220,   221,    -1,    -1,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,    -1,
     247,   248,    -1,    -1,   251,   252,   253,    -1,    -1,   256,
     257,    -1,    -1,    -1,   261,    -1,    -1,   264,   265,   266,
     267,   268,   269,   270,   271,   272,    -1,   274,   275,   276,
      -1,   278,   279,   280,   281,   282,   283,   284,    -1,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,    -1,    -1,   303,   304,   305,   306,
     307,   308,   309,   310,    -1,   312,   313,    -1,   315,   316,
     317,   318,   319,   320,   321,    -1,   323,   324,    -1,   326,
     327,   328,    -1,   330,   331,    -1,   333,    -1,   335,   336,
     337,   338,   339,   340,   341,   342,   343,    -1,    -1,   346,
     347,   348,   349,   350,   351,    -1,   353,   354,   355,   356,
     357,   358,   359,   360,    -1,    -1,    -1,   364,   365,   366,
     367,   368,   369,    -1,   371,   372,    -1,   374,   375,    -1,
      -1,   378,   379,   380,   381,   382,    -1,     5,     6,     7,
       8,     9,    -1,    11,    12,   392,    14,    15,    16,   396,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    -1,    35,    36,    37,
      38,    39,    40,    -1,    42,    43,    44,    -1,    46,    47,
      48,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      78,    79,    80,    81,    82,    83,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    93,    94,    95,    96,    97,
      98,    -1,   100,   101,   102,    -1,   104,   105,   106,    -1,
     108,   109,   110,   111,   112,   113,   114,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,    -1,   127,
     128,   129,   130,    -1,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,    -1,
      -1,   149,    -1,   151,   152,   153,    -1,    -1,    -1,   157,
     158,   159,   160,    -1,   162,   163,    -1,    -1,    -1,   167,
     168,   169,   170,    -1,   172,   173,   174,   175,    -1,   177,
      -1,   179,   180,    -1,   182,   183,    -1,   185,    -1,    -1,
     188,    -1,   190,    -1,   192,   193,   194,    -1,   196,   197,
     198,    -1,   200,   201,    -1,   203,   204,   205,   206,   207,
      -1,   209,    -1,    -1,    -1,    -1,   214,   215,   216,   217,
     218,    -1,   220,   221,    -1,    -1,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,    -1,   247,
     248,    -1,    -1,   251,   252,   253,    -1,    -1,   256,   257,
      -1,    -1,    -1,   261,    -1,    -1,   264,   265,   266,   267,
     268,   269,   270,   271,   272,    -1,   274,   275,   276,    -1,
     278,   279,   280,   281,   282,   283,   284,    -1,   286,   287,
     288,   289,   290,   291,   292,   293,   294,   295,   296,   297,
     298,   299,   300,    -1,    -1,   303,   304,   305,   306,   307,
     308,   309,   310,    -1,   312,   313,    -1,   315,   316,   317,
     318,   319,   320,   321,    -1,   323,   324,    -1,   326,   327,
     328,    -1,   330,   331,    -1,   333,    -1,   335,   336,   337,
     338,   339,   340,   341,   342,   343,    -1,    -1,   346,   347,
     348,   349,   350,   351,    -1,   353,   354,   355,   356,   357,
     358,   359,   360,    -1,    -1,    -1,   364,   365,   366,   367,
     368,   369,    -1,   371,   372,    -1,   374,   375,    -1,    -1,
     378,   379,   380,   381,   382,     6,     7,     8,     9,    -1,
      11,    12,    -1,    14,    15,    16,    -1,    18,   396,    20,
      -1,    22,    23,    24,    25,    26,    27,    -1,    29,    30,
      31,    32,    33,    -1,    35,    36,    37,    38,    -1,    -1,
      -1,    42,    43,    44,    -1,    46,    -1,    -1,    -1,    50,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    78,    79,    80,
      81,    82,    83,    -1,    85,    -1,    -1,    -1,    -1,    90,
      -1,    92,    93,    94,    -1,    96,    97,    98,    -1,   100,
      -1,   102,    -1,    -1,   105,   106,    -1,   108,    -1,    -1,
     111,   112,   113,   114,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,    -1,   127,   128,    -1,   130,
      -1,   132,   133,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,   144,   145,   146,    -1,    -1,   149,    -1,
     151,   152,   153,    -1,    -1,    -1,   157,   158,   159,   160,
      -1,   162,   163,    -1,   165,   166,   167,   168,   169,   170,
      -1,   172,    -1,   174,   175,   176,   177,    -1,   179,    -1,
      -1,    -1,   183,    -1,   185,    -1,    -1,   188,    -1,   190,
      -1,   192,   193,   194,    -1,   196,   197,   198,    -1,    -1,
     201,    -1,   203,   204,   205,    -1,   207,   208,   209,    -1,
      -1,    -1,    -1,   214,    -1,   216,   217,   218,    -1,    -1,
     221,    -1,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,    -1,   239,    -1,
     241,   242,   243,   244,   245,    -1,   247,   248,    -1,    -1,
     251,   252,   253,    -1,   255,   256,    -1,    -1,    -1,   260,
     261,    -1,   263,   264,    -1,    -1,    -1,   268,   269,   270,
     271,   272,    -1,   274,   275,   276,    -1,   278,   279,   280,
     281,   282,   283,   284,    -1,   286,   287,   288,   289,   290,
     291,   292,   293,    -1,   295,   296,   297,   298,   299,   300,
      -1,    -1,   303,   304,   305,    -1,   307,   308,   309,   310,
      -1,   312,   313,    -1,   315,   316,   317,   318,   319,   320,
     321,    -1,   323,   324,    -1,   326,   327,   328,    -1,   330,
     331,    -1,   333,    -1,   335,   336,   337,   338,    -1,   340,
     341,   342,   343,   344,    -1,   346,   347,   348,   349,   350,
      -1,    -1,   353,   354,   355,   356,   357,     5,   359,   360,
      -1,    -1,    -1,   364,   365,   366,   367,   368,   369,    -1,
     371,    19,    -1,   374,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    40,    -1,    -1,    -1,   397,    -1,    -1,    47,
      48,    -1,    -1,    -1,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    -1,    75,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   250,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   344,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   356,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   371,   372,    -1,   374,   375,    -1,    -1,
      -1,    -1,   380,    -1,   382,   383,    -1,    -1,    -1,   387,
     388,   389,   390,   391,    -1,    -1,   394,   395,   396,     6,
       7,     8,     9,    -1,    11,    12,    -1,    14,    15,    16,
      -1,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    -1,    29,    30,    31,    32,    33,    -1,    35,    36,
      37,    38,    -1,    -1,    -1,    42,    43,    44,    -1,    46,
      -1,    -1,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,    -1,    -1,   105,   106,
      -1,   108,    -1,    -1,   111,   112,   113,   114,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,    -1,
     127,   128,    -1,   130,    -1,   132,   133,   134,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,   149,    -1,   151,   152,   153,    -1,    -1,    -1,
     157,   158,   159,   160,    -1,   162,   163,    -1,   165,    -1,
     167,   168,   169,   170,    -1,   172,    -1,   174,   175,    -1,
     177,    -1,   179,   180,    -1,   182,   183,    -1,   185,    -1,
      -1,   188,    -1,   190,   191,   192,   193,   194,   195,   196,
     197,   198,    -1,   200,   201,    -1,   203,   204,   205,    -1,
     207,    -1,   209,    -1,    -1,   212,   213,   214,   215,   216,
     217,   218,    -1,   220,   221,   222,    -1,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,    -1,
     247,   248,   249,    -1,   251,   252,   253,    -1,    -1,   256,
     257,    -1,   259,   260,   261,    -1,    -1,   264,   265,   266,
      -1,   268,   269,   270,   271,   272,    -1,   274,   275,   276,
      -1,   278,   279,   280,   281,   282,   283,   284,    -1,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,    -1,   302,   303,   304,   305,    -1,
     307,   308,   309,   310,   311,   312,   313,    -1,   315,   316,
     317,   318,   319,   320,   321,    -1,   323,   324,    -1,   326,
     327,   328,    -1,   330,   331,    -1,   333,    -1,   335,   336,
     337,   338,    -1,   340,   341,   342,   343,    -1,    -1,   346,
     347,   348,   349,   350,    -1,    -1,   353,   354,   355,   356,
     357,   358,   359,   360,    -1,    -1,    -1,   364,   365,   366,
     367,   368,   369,    -1,   371,    -1,    -1,   374,    -1,    -1,
      -1,    -1,    -1,    -1,     6,     7,     8,     9,    -1,    11,
      12,    -1,    14,    15,    16,    -1,    18,    -1,    20,   396,
      22,    23,    24,    25,    26,    27,    -1,    29,    30,    31,
      32,    33,    -1,    35,    36,    37,    38,    -1,    -1,    -1,
      42,    43,    44,    -1,    46,    -1,    -1,    -1,    50,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    78,    79,    80,    81,
      82,    83,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,    -1,    -1,   105,   106,    -1,   108,    -1,    -1,   111,
     112,   113,   114,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,    -1,   127,   128,    -1,   130,    -1,
     132,   133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,   144,   145,   146,    -1,    -1,   149,    -1,   151,
     152,   153,    -1,    -1,    -1,   157,   158,   159,   160,    -1,
     162,   163,    -1,   165,    -1,   167,   168,   169,   170,    -1,
     172,    -1,   174,   175,    -1,   177,    -1,   179,   180,    -1,
     182,   183,    -1,   185,    -1,    -1,   188,    -1,   190,   191,
     192,   193,   194,   195,   196,   197,   198,    -1,   200,   201,
      -1,   203,   204,   205,    -1,   207,    -1,   209,    -1,    -1,
     212,   213,   214,   215,   216,   217,   218,    -1,   220,   221,
     222,    -1,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,    -1,   247,   248,   249,    -1,   251,
     252,   253,    -1,    -1,   256,   257,    -1,   259,   260,   261,
      -1,    -1,   264,   265,   266,    -1,   268,   269,   270,   271,
     272,    -1,   274,   275,   276,    -1,   278,   279,   280,   281,
     282,   283,   284,    -1,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   298,   299,   300,    -1,
      -1,   303,   304,   305,    -1,   307,   308,   309,   310,   311,
     312,   313,    -1,   315,   316,   317,   318,   319,   320,   321,
      -1,   323,   324,    -1,   326,   327,   328,    -1,   330,   331,
      -1,   333,    -1,   335,   336,   337,   338,    -1,   340,   341,
     342,   343,    -1,    -1,   346,   347,   348,   349,   350,    -1,
      -1,   353,   354,   355,   356,   357,   358,   359,   360,    -1,
      -1,    -1,   364,   365,   366,   367,   368,   369,    -1,   371,
      -1,    -1,   374,    -1,    -1,    -1,    -1,    -1,    -1,     6,
       7,     8,     9,    -1,    11,    12,    -1,    14,    15,    16,
      -1,    18,    -1,    20,   396,    22,    23,    24,    25,    26,
      27,    -1,    29,    30,    31,    32,    33,    -1,    35,    36,
      37,    38,    -1,    -1,    -1,    42,    43,    44,    -1,    46,
      -1,    -1,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    94,    -1,    96,
      97,    98,    -1,   100,    -1,   102,    -1,    -1,   105,   106,
      -1,   108,    -1,    -1,   111,   112,   113,   114,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,    -1,
     127,   128,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,   149,    -1,   151,   152,   153,    -1,    -1,    -1,
     157,   158,   159,   160,    -1,   162,   163,    -1,   165,    -1,
     167,   168,   169,   170,    -1,   172,    -1,   174,   175,    -1,
     177,    -1,   179,    -1,    -1,    -1,   183,    -1,   185,    -1,
      -1,   188,    -1,   190,    -1,   192,   193,   194,    -1,   196,
     197,   198,    -1,    -1,   201,    -1,   203,   204,   205,    -1,
     207,    -1,   209,    -1,    -1,    -1,    -1,   214,    -1,   216,
     217,   218,    -1,    -1,   221,    -1,    -1,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,    -1,   239,   240,   241,   242,   243,   244,   245,    -1,
     247,   248,    -1,    -1,   251,   252,   253,    -1,    -1,   256,
     257,    -1,    -1,   260,   261,    -1,    -1,   264,    -1,    -1,
      -1,   268,   269,   270,   271,   272,    -1,   274,   275,   276,
      -1,   278,   279,   280,   281,   282,   283,   284,    -1,   286,
     287,   288,   289,   290,   291,   292,   293,    -1,   295,   296,
     297,   298,   299,   300,    -1,    -1,   303,   304,   305,    -1,
     307,   308,   309,   310,    -1,   312,   313,    -1,   315,   316,
     317,   318,   319,   320,   321,    -1,   323,   324,    -1,   326,
     327,   328,    -1,   330,   331,    -1,   333,    -1,   335,   336,
     337,   338,    -1,   340,   341,   342,   343,    -1,    -1,   346,
     347,   348,   349,   350,    -1,    -1,   353,   354,   355,   356,
     357,    -1,   359,   360,    -1,    -1,    -1,   364,   365,   366,
     367,   368,   369,    -1,   371,    -1,    -1,   374,    -1,    -1,
      -1,    -1,    -1,    -1,     6,     7,     8,     9,    -1,    11,
      12,    -1,    14,    15,    16,    -1,    18,    -1,    20,   396,
      22,    23,    24,    25,    26,    27,    -1,    29,    30,    31,
      32,    33,    -1,    35,    36,    37,    38,    -1,    -1,    -1,
      42,    43,    44,    -1,    46,    -1,    -1,    -1,    50,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    78,    79,    80,    81,
      82,    83,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    93,    94,    -1,    96,    97,    98,    -1,   100,    -1,
     102,    -1,    -1,   105,   106,    -1,   108,    -1,    -1,   111,
     112,   113,   114,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,    -1,   127,   128,    -1,   130,    -1,
     132,   133,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,   144,   145,   146,    -1,    -1,   149,    -1,   151,
     152,   153,    -1,    -1,    -1,   157,   158,   159,   160,    -1,
     162,   163,    -1,   165,    -1,   167,   168,   169,   170,    -1,
     172,    -1,   174,   175,    -1,   177,    -1,   179,    -1,    -1,
      -1,   183,    -1,   185,    -1,    -1,   188,    -1,   190,    -1,
     192,   193,   194,    -1,   196,   197,   198,    -1,    -1,   201,
      -1,   203,   204,   205,    -1,   207,    -1,   209,    -1,    -1,
      -1,    -1,   214,    -1,   216,   217,   218,    -1,    -1,   221,
      -1,    -1,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,    -1,   239,    -1,   241,
     242,   243,   244,   245,    -1,   247,   248,    -1,    -1,   251,
     252,   253,    -1,    -1,   256,    -1,    -1,    -1,   260,   261,
      -1,    -1,   264,    -1,    -1,    -1,   268,   269,   270,   271,
     272,    -1,   274,   275,   276,    -1,   278,   279,   280,   281,
     282,   283,   284,    -1,   286,   287,   288,   289,   290,   291,
     292,   293,    -1,   295,   296,   297,   298,   299,   300,    -1,
     302,   303,   304,   305,    -1,   307,   308,   309,   310,    -1,
     312,   313,    -1,   315,   316,   317,   318,   319,   320,   321,
      -1,   323,   324,    -1,   326,   327,   328,    -1,   330,   331,
      -1,   333,    -1,   335,   336,   337,   338,    -1,   340,   341,
     342,   343,    -1,    -1,   346,   347,   348,   349,   350,    -1,
      -1,   353,   354,   355,   356,   357,    -1,   359,   360,    -1,
      -1,    -1,   364,   365,   366,   367,   368,   369,    -1,   371,
      -1,    -1,   374,    -1,    -1,    -1,    -1,    -1,    -1,     6,
       7,     8,     9,    -1,    11,    12,    -1,    14,    15,    16,
      -1,    18,    -1,    20,   396,    22,    23,    24,    25,    26,
      27,    -1,    29,    30,    31,    32,    33,    -1,    35,    36,
      37,    38,    -1,    -1,    -1,    42,    43,    44,    -1,    46,
      -1,    -1,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    94,    -1,    96,
      97,    98,    -1,   100,    -1,   102,    -1,    -1,   105,   106,
      -1,   108,    -1,    -1,   111,   112,   113,   114,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,    -1,
     127,   128,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,   149,    -1,   151,   152,   153,    -1,    -1,    -1,
     157,   158,   159,   160,    -1,   162,   163,    -1,   165,    -1,
     167,   168,   169,   170,    -1,   172,    -1,   174,   175,    -1,
     177,    -1,   179,    -1,    -1,    -1,   183,    -1,   185,    -1,
      -1,   188,    -1,   190,    -1,   192,   193,   194,    -1,   196,
     197,   198,    -1,    -1,   201,    -1,   203,   204,   205,    -1,
     207,    -1,   209,    -1,    -1,    -1,    -1,   214,    -1,   216,
     217,   218,    -1,    -1,   221,    -1,    -1,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,    -1,   239,    -1,   241,   242,   243,   244,   245,    -1,
     247,   248,    -1,    -1,   251,   252,   253,    -1,    -1,   256,
      -1,    -1,    -1,   260,   261,    -1,    -1,   264,    -1,    -1,
      -1,   268,   269,   270,   271,   272,    -1,   274,   275,   276,
      -1,   278,   279,   280,   281,   282,   283,   284,    -1,   286,
     287,   288,   289,   290,   291,   292,   293,    -1,   295,   296,
     297,   298,   299,   300,    -1,    -1,   303,   304,   305,    -1,
     307,   308,   309,   310,    -1,   312,   313,    -1,   315,   316,
     317,   318,   319,   320,   321,    -1,   323,   324,    -1,   326,
     327,   328,    -1,   330,   331,    -1,   333,    -1,   335,   336,
     337,   338,    -1,   340,   341,   342,   343,    -1,    -1,   346,
     347,   348,   349,   350,    -1,    -1,   353,   354,   355,   356,
     357,    -1,   359,   360,    -1,    -1,    -1,   364,   365,   366,
     367,   368,   369,    -1,   371,    -1,    -1,   374,    -1,    -1,
      -1,    -1,    -1,     5,     6,     7,     8,     9,    -1,    11,
      12,    -1,    14,    15,    16,    -1,    18,    -1,    20,   396,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    -1,    35,    36,    37,    38,    39,    40,    -1,
      42,    43,    44,    -1,    46,    47,    48,    -1,    50,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    78,    79,    80,    81,
      82,    83,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    93,    94,    -1,    96,    97,    98,    -1,   100,    -1,
     102,    -1,   104,   105,   106,    -1,   108,    -1,    -1,   111,
     112,   113,   114,    -1,   116,   117,   118,   119,    -1,    -1,
      -1,   123,   124,   125,    -1,   127,   128,    -1,   130,    -1,
     132,   133,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,   144,   145,   146,    -1,    -1,   149,    -1,   151,
     152,   153,    -1,    -1,    -1,   157,   158,   159,   160,    -1,
     162,   163,    -1,   165,    -1,   167,   168,    -1,   170,    -1,
      -1,    -1,   174,   175,    -1,   177,    -1,   179,    -1,    -1,
      -1,   183,    -1,   185,    -1,    -1,   188,    -1,   190,    -1,
     192,   193,   194,    -1,   196,   197,   198,    -1,    -1,   201,
      -1,   203,   204,   205,   206,   207,    -1,   209,    -1,    -1,
      -1,    -1,   214,    -1,   216,   217,   218,    -1,    -1,   221,
      -1,    -1,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,    -1,   239,    -1,   241,
     242,   243,   244,    -1,    -1,   247,   248,    -1,    -1,    -1,
     252,   253,    -1,    -1,   256,    -1,    -1,    -1,   260,   261,
      -1,    -1,   264,    -1,    -1,    -1,   268,   269,   270,   271,
     272,    -1,    -1,   275,   276,    -1,   278,   279,   280,   281,
     282,   283,   284,    -1,   286,   287,   288,   289,   290,   291,
     292,   293,    -1,   295,    -1,   297,   298,   299,   300,    -1,
      -1,   303,   304,   305,    -1,   307,   308,   309,   310,    -1,
     312,   313,    -1,   315,   316,   317,   318,   319,   320,   321,
      -1,    -1,   324,    -1,   326,   327,   328,    -1,   330,   331,
      -1,   333,    -1,   335,    -1,   337,    -1,    -1,   340,   341,
     342,   343,    -1,    -1,   346,   347,   348,   349,   350,    -1,
      -1,   353,   354,   355,   356,   357,    -1,   359,   360,    -1,
      -1,    -1,   364,   365,   366,   367,   368,   369,    -1,   371,
     372,   373,   374,   375,    -1,    -1,    -1,    -1,   380,    -1,
     382,   383,   384,   385,    -1,   387,   388,   389,   390,   391,
     392,     6,     7,     8,     9,    -1,    11,    12,    -1,    14,
      15,    16,    -1,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    -1,    29,    30,    31,    32,    33,    -1,
      35,    36,    37,    38,    -1,    -1,    -1,    42,    43,    44,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    78,    79,    80,    81,    82,    83,    -1,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,
      -1,    96,    97,    98,    -1,   100,    -1,   102,    -1,    -1,
     105,   106,    -1,   108,    -1,    -1,   111,   112,   113,   114,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,    -1,   127,   128,    -1,   130,    -1,   132,   133,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,   144,
     145,   146,    -1,    -1,   149,    -1,   151,   152,   153,    -1,
      -1,    -1,   157,   158,   159,   160,    -1,   162,   163,    -1,
     165,    -1,   167,   168,   169,   170,    -1,   172,    -1,   174,
     175,    -1,   177,    -1,   179,    -1,    -1,    -1,   183,    -1,
     185,    -1,    -1,   188,    -1,   190,    -1,   192,   193,   194,
      -1,   196,   197,   198,    -1,    -1,   201,    -1,   203,   204,
     205,    -1,   207,    -1,   209,    -1,    -1,    -1,    -1,   214,
      -1,   216,   217,   218,    -1,    -1,   221,    -1,    -1,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,    -1,   239,    -1,   241,   242,   243,   244,
     245,    -1,   247,   248,    -1,    -1,   251,   252,   253,    -1,
      -1,   256,    -1,    -1,    -1,   260,   261,    -1,    -1,   264,
      -1,    -1,    -1,   268,   269,   270,   271,   272,    -1,   274,
     275,   276,    -1,   278,   279,   280,   281,   282,   283,   284,
      -1,   286,   287,   288,   289,   290,   291,   292,   293,    -1,
     295,   296,   297,   298,   299,   300,    -1,    -1,   303,   304,
     305,    -1,   307,   308,   309,   310,    -1,   312,   313,    -1,
     315,   316,   317,   318,   319,   320,   321,    -1,   323,   324,
      -1,   326,   327,   328,    -1,   330,   331,    -1,   333,    -1,
     335,   336,   337,   338,    -1,   340,   341,   342,   343,    -1,
      -1,   346,   347,   348,   349,   350,    -1,    -1,   353,   354,
     355,   356,   357,    -1,   359,   360,    -1,    -1,    -1,   364,
     365,   366,   367,   368,   369,    -1,   371,    -1,   373,   374,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   383,   384,
     385,    -1,   387,   388,   389,   390,   391,   392,     5,     6,
       7,     8,     9,    -1,    11,    12,    -1,    14,    15,    16,
      -1,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    -1,    35,    36,
      37,    38,    39,    40,    -1,    42,    43,    44,    -1,    46,
      47,    48,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    94,    -1,    96,
      97,    98,    -1,   100,    -1,   102,    -1,   104,   105,   106,
      -1,   108,    -1,    -1,   111,   112,   113,   114,    -1,   116,
     117,   118,   119,    -1,    -1,    -1,   123,   124,   125,    -1,
     127,   128,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,   149,    -1,   151,   152,   153,    -1,    -1,    -1,
     157,   158,   159,   160,    -1,   162,   163,    -1,   165,    -1,
     167,   168,    -1,   170,    -1,    -1,    -1,   174,   175,    -1,
     177,    -1,   179,    -1,    -1,    -1,   183,    -1,   185,    -1,
      -1,   188,    -1,   190,    -1,   192,   193,   194,    -1,   196,
     197,   198,    -1,    -1,   201,    -1,   203,   204,   205,   206,
     207,    -1,   209,    -1,    -1,    -1,    -1,   214,    -1,   216,
     217,   218,    -1,    -1,   221,    -1,    -1,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,    -1,   239,    -1,   241,   242,   243,   244,    -1,    -1,
     247,   248,    -1,    -1,    -1,   252,   253,    -1,    -1,   256,
      -1,    -1,    -1,   260,   261,    -1,    -1,   264,    -1,    -1,
      -1,   268,   269,   270,   271,   272,    -1,    -1,   275,   276,
      -1,   278,   279,   280,   281,   282,   283,   284,    -1,   286,
     287,   288,   289,   290,   291,   292,   293,    -1,   295,    -1,
     297,   298,   299,   300,    -1,    -1,   303,   304,   305,    -1,
     307,   308,   309,   310,    -1,   312,   313,    -1,   315,   316,
     317,   318,   319,   320,   321,    -1,    -1,   324,    -1,   326,
     327,   328,    -1,   330,   331,    -1,   333,    -1,   335,    -1,
     337,    -1,    -1,   340,   341,   342,   343,    -1,    -1,   346,
     347,   348,   349,   350,    -1,    -1,   353,   354,   355,   356,
     357,    -1,   359,   360,    -1,    -1,    -1,   364,   365,   366,
     367,   368,   369,    -1,   371,    -1,    -1,   374,    -1,     6,
       7,     8,     9,    -1,    11,    12,    -1,    14,    15,    16,
      -1,    18,   389,    20,    -1,    22,    23,    24,    25,    26,
      27,    -1,    29,    30,    31,    32,    33,    -1,    35,    36,
      37,    38,    -1,    -1,    -1,    42,    43,    44,    -1,    46,
      -1,    -1,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    94,    -1,    96,
      97,    98,    -1,   100,    -1,   102,    -1,    -1,   105,   106,
      -1,   108,    -1,    -1,   111,   112,   113,   114,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,    -1,
     127,   128,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,   149,    -1,   151,   152,   153,    -1,    -1,    -1,
     157,   158,   159,   160,    -1,   162,   163,    -1,   165,    -1,
     167,   168,   169,   170,    -1,   172,    -1,   174,   175,    -1,
     177,    -1,   179,    -1,    -1,    -1,   183,    -1,   185,    -1,
      -1,   188,    -1,   190,    -1,   192,   193,   194,    -1,   196,
     197,   198,    -1,    -1,   201,    -1,   203,   204,   205,    -1,
     207,    -1,   209,    -1,    -1,    -1,    -1,   214,    -1,   216,
     217,   218,    -1,    -1,   221,    -1,    -1,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,    -1,   239,   240,   241,   242,   243,   244,   245,    -1,
     247,   248,    -1,    -1,   251,   252,   253,    -1,    -1,   256,
     257,    -1,    -1,   260,   261,    -1,    -1,   264,    -1,    -1,
      -1,   268,   269,   270,   271,   272,    -1,   274,   275,   276,
      -1,   278,   279,   280,   281,   282,   283,   284,    -1,   286,
     287,   288,   289,   290,   291,   292,   293,    -1,   295,   296,
     297,   298,   299,   300,    -1,    -1,   303,   304,   305,    -1,
     307,   308,   309,   310,    -1,   312,   313,    -1,   315,   316,
     317,   318,   319,   320,   321,    -1,   323,   324,    -1,   326,
     327,   328,    -1,   330,   331,    -1,   333,    -1,   335,   336,
     337,   338,    -1,   340,   341,   342,   343,    -1,    -1,   346,
     347,   348,   349,   350,    -1,    -1,   353,   354,   355,   356,
     357,    -1,   359,   360,    -1,    -1,    -1,   364,   365,   366,
     367,   368,   369,    -1,   371,    -1,    -1,   374,    -1,     6,
       7,     8,     9,    -1,    11,    12,    -1,    14,    15,    16,
      -1,    18,   389,    20,    -1,    22,    23,    24,    25,    26,
      27,    -1,    29,    30,    31,    32,    33,    -1,    35,    36,
      37,    38,    -1,    -1,    -1,    42,    43,    44,    -1,    46,
      -1,    -1,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    94,    -1,    96,
      97,    98,    -1,   100,    -1,   102,    -1,    -1,   105,   106,
      -1,   108,    -1,    -1,   111,   112,   113,   114,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,    -1,
     127,   128,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,   149,    -1,   151,   152,   153,    -1,    -1,    -1,
     157,   158,   159,   160,    -1,   162,   163,    -1,   165,    -1,
     167,   168,   169,   170,    -1,   172,    -1,   174,   175,    -1,
     177,    -1,   179,    -1,    -1,    -1,   183,    -1,   185,    -1,
      -1,   188,    -1,   190,    -1,   192,   193,   194,    -1,   196,
     197,   198,    -1,    -1,   201,    -1,   203,   204,   205,    -1,
     207,    -1,   209,    -1,    -1,    -1,    -1,   214,    -1,   216,
     217,   218,    -1,    -1,   221,    -1,    -1,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,    -1,   239,    -1,   241,   242,   243,   244,   245,    -1,
     247,   248,    -1,    -1,   251,   252,   253,    -1,    -1,   256,
      -1,    -1,    -1,   260,   261,    -1,    -1,   264,    -1,    -1,
      -1,   268,   269,   270,   271,   272,    -1,   274,   275,   276,
      -1,   278,   279,   280,   281,   282,   283,   284,    -1,   286,
     287,   288,   289,   290,   291,   292,   293,    -1,   295,   296,
     297,   298,   299,   300,    -1,    -1,   303,   304,   305,    -1,
     307,   308,   309,   310,    -1,   312,   313,    -1,   315,   316,
     317,   318,   319,   320,   321,    -1,   323,   324,    -1,   326,
     327,   328,    -1,   330,   331,    -1,   333,    -1,   335,   336,
     337,   338,    -1,   340,   341,   342,   343,    -1,    -1,   346,
     347,   348,   349,   350,    -1,    -1,   353,   354,   355,   356,
     357,    -1,   359,   360,    -1,    -1,    -1,   364,   365,   366,
     367,   368,   369,    -1,   371,    -1,    -1,   374,    -1,     6,
       7,     8,     9,    -1,    11,    12,    -1,    14,    15,    16,
      -1,    18,   389,    20,    -1,    22,    23,    24,    25,    26,
      27,    -1,    29,    30,    31,    32,    33,    -1,    35,    36,
      37,    38,    -1,    -1,    -1,    42,    43,    44,    -1,    46,
      -1,    -1,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    94,    -1,    96,
      97,    98,    -1,   100,    -1,   102,    -1,    -1,   105,   106,
      -1,   108,    -1,    -1,   111,   112,   113,   114,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,    -1,
     127,   128,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,   149,    -1,   151,   152,   153,    -1,    -1,    -1,
     157,   158,   159,   160,    -1,   162,   163,    -1,   165,    -1,
     167,   168,   169,   170,    -1,   172,    -1,   174,   175,    -1,
     177,    -1,   179,    -1,    -1,    -1,   183,    -1,   185,    -1,
      -1,   188,    -1,   190,    -1,   192,   193,   194,    -1,   196,
     197,   198,    -1,    -1,   201,    -1,   203,   204,   205,    -1,
     207,    -1,   209,    -1,    -1,    -1,    -1,   214,    -1,   216,
     217,   218,    -1,    -1,   221,    -1,    -1,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,    -1,   239,    -1,   241,   242,   243,   244,   245,    -1,
     247,   248,    -1,    -1,   251,   252,   253,    -1,    -1,   256,
      -1,    -1,    -1,   260,   261,    -1,    -1,   264,    -1,    -1,
      -1,   268,   269,   270,   271,   272,    -1,   274,   275,   276,
      -1,   278,   279,   280,   281,   282,   283,   284,    -1,   286,
     287,   288,   289,   290,   291,   292,   293,    -1,   295,   296,
     297,   298,   299,   300,    -1,    -1,   303,   304,   305,    -1,
     307,   308,   309,   310,    -1,   312,   313,    -1,   315,   316,
     317,   318,   319,   320,   321,    -1,   323,   324,    -1,   326,
     327,   328,    -1,   330,   331,    -1,   333,    -1,   335,   336,
     337,   338,    -1,   340,   341,   342,   343,    -1,    -1,   346,
     347,   348,   349,   350,    -1,    -1,   353,   354,   355,   356,
     357,    -1,   359,   360,    -1,    -1,    -1,   364,   365,   366,
     367,   368,   369,    -1,   371,    -1,    -1,   374,    -1,     6,
       7,     8,     9,    -1,    11,    12,    -1,    14,    15,    16,
      -1,    18,   389,    20,    -1,    22,    23,    24,    25,    26,
      27,    -1,    29,    30,    31,    32,    33,    -1,    35,    36,
      37,    38,    -1,    -1,    -1,    42,    43,    44,    -1,    46,
      -1,    -1,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    94,    -1,    96,
      97,    98,    -1,   100,    -1,   102,    -1,    -1,   105,   106,
      -1,   108,    -1,    -1,   111,   112,   113,   114,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,    -1,
     127,   128,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,   144,   145,   146,
     147,    -1,   149,    -1,   151,   152,   153,    -1,    -1,    -1,
     157,   158,   159,   160,    -1,   162,   163,    -1,   165,    -1,
     167,   168,   169,   170,    -1,   172,   173,   174,   175,    -1,
     177,    -1,   179,    -1,    -1,    -1,   183,    -1,   185,    -1,
      -1,   188,    -1,   190,    -1,   192,   193,   194,    -1,   196,
     197,   198,    -1,    -1,   201,    -1,   203,   204,   205,    -1,
     207,    -1,   209,    -1,    -1,    -1,    -1,   214,    -1,   216,
     217,   218,    -1,    -1,   221,    -1,    -1,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,    -1,   239,    -1,   241,   242,   243,   244,   245,    -1,
     247,   248,    -1,    -1,   251,   252,   253,   254,    -1,   256,
      -1,   258,    -1,   260,   261,    -1,    -1,   264,    -1,    -1,
      -1,   268,   269,   270,   271,   272,    -1,   274,   275,   276,
      -1,   278,   279,   280,   281,   282,   283,   284,    -1,   286,
     287,   288,   289,   290,   291,   292,   293,    -1,   295,   296,
     297,   298,   299,   300,    -1,    -1,   303,   304,   305,    -1,
     307,   308,   309,   310,    -1,   312,   313,    -1,   315,   316,
     317,   318,   319,   320,   321,    -1,   323,   324,    -1,   326,
     327,   328,    -1,   330,   331,    -1,   333,    -1,   335,   336,
     337,   338,   339,   340,   341,   342,   343,    -1,    -1,   346,
     347,   348,   349,   350,    -1,    -1,   353,   354,   355,   356,
     357,    -1,   359,   360,    -1,    -1,    -1,   364,   365,   366,
     367,   368,   369,    -1,   371,   372,    -1,   374,   375,    -1,
      -1,    -1,    -1,   380,    -1,   382,    -1,     6,     7,     8,
       9,   388,    11,    12,    -1,    14,    15,    16,    -1,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    -1,
      29,    30,    31,    32,    33,    -1,    35,    36,    37,    38,
      -1,    -1,    -1,    42,    43,    44,    -1,    46,    -1,    -1,
      -1,    50,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    78,
      79,    80,    81,    82,    83,    -1,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    93,    94,    -1,    96,    97,    98,
      -1,   100,    -1,   102,    -1,    -1,   105,   106,    -1,   108,
      -1,    -1,   111,   112,   113,   114,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,    -1,   127,   128,
      -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
     149,    -1,   151,   152,   153,    -1,    -1,    -1,   157,   158,
     159,   160,    -1,   162,   163,    -1,   165,    -1,   167,   168,
     169,   170,    -1,   172,   173,   174,   175,    -1,   177,    -1,
     179,    -1,    -1,    -1,   183,    -1,   185,    -1,    -1,   188,
      -1,   190,    -1,   192,   193,   194,    -1,   196,   197,   198,
      -1,    -1,   201,    -1,   203,   204,   205,    -1,   207,    -1,
     209,    -1,    -1,    -1,    -1,   214,    -1,   216,   217,   218,
      -1,    -1,   221,    -1,    -1,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,    -1,
     239,    -1,   241,   242,   243,   244,   245,    -1,   247,   248,
      -1,    -1,   251,   252,   253,   254,    -1,   256,    -1,   258,
      -1,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,   268,
     269,   270,   271,   272,    -1,   274,   275,   276,    -1,   278,
     279,   280,   281,   282,   283,   284,    -1,   286,   287,   288,
     289,   290,   291,   292,   293,    -1,   295,   296,   297,   298,
     299,   300,    -1,    -1,   303,   304,   305,    -1,   307,   308,
     309,   310,    -1,   312,   313,    -1,   315,   316,   317,   318,
     319,   320,   321,    -1,   323,   324,    -1,   326,   327,   328,
      -1,   330,   331,    -1,   333,    -1,   335,   336,   337,   338,
     339,   340,   341,   342,   343,    -1,    -1,   346,   347,   348,
     349,   350,    -1,    -1,   353,   354,   355,   356,   357,    -1,
     359,   360,    -1,    -1,    -1,   364,   365,   366,   367,   368,
     369,    -1,   371,   372,    -1,   374,   375,    -1,    -1,    -1,
      -1,   380,    -1,   382,    -1,     6,     7,     8,     9,   388,
      11,    12,    -1,    14,    15,    16,    -1,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    -1,    29,    30,
      31,    32,    33,    -1,    35,    36,    37,    38,    -1,    -1,
      -1,    42,    43,    44,    -1,    46,    -1,    -1,    -1,    50,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    94,    -1,    96,    97,    98,    -1,   100,
      -1,   102,    -1,    -1,   105,   106,    -1,   108,    -1,    -1,
     111,   112,   113,   114,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,    -1,   127,   128,    -1,   130,
      -1,   132,   133,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,   144,   145,   146,    -1,    -1,   149,    -1,
     151,   152,   153,    -1,    -1,    -1,   157,   158,   159,   160,
      -1,   162,   163,    -1,   165,    -1,   167,   168,   169,   170,
      -1,   172,    -1,   174,   175,    -1,   177,    -1,   179,    -1,
     181,    -1,   183,    -1,   185,    -1,    -1,   188,    -1,   190,
      -1,   192,   193,   194,   195,   196,   197,   198,    -1,    -1,
     201,    -1,   203,   204,   205,    -1,   207,    -1,   209,    -1,
      -1,    -1,    -1,   214,    -1,   216,   217,   218,    -1,    -1,
     221,    -1,    -1,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,    -1,   239,    -1,
     241,   242,   243,   244,   245,    -1,   247,   248,    -1,    -1,
     251,   252,   253,    -1,    -1,   256,    -1,    -1,    -1,   260,
     261,    -1,    -1,   264,    -1,    -1,    -1,   268,   269,   270,
     271,   272,    -1,   274,   275,   276,    -1,   278,   279,   280,
     281,   282,   283,   284,    -1,   286,   287,   288,   289,   290,
     291,   292,   293,    -1,   295,   296,   297,   298,   299,   300,
      -1,    -1,   303,   304,   305,    -1,   307,   308,   309,   310,
      -1,   312,   313,    -1,   315,   316,   317,   318,   319,   320,
     321,    -1,   323,   324,    -1,   326,   327,   328,    -1,   330,
     331,    -1,   333,    -1,   335,   336,   337,   338,    -1,   340,
     341,   342,   343,    -1,    -1,   346,   347,   348,   349,   350,
      -1,    -1,   353,   354,   355,   356,   357,    -1,   359,   360,
      -1,    -1,    -1,   364,   365,   366,   367,   368,   369,    -1,
     371,    -1,    -1,   374,   375,    -1,    -1,    -1,    -1,   380,
       6,     7,     8,     9,    -1,    11,    12,   388,    14,    15,
      16,    -1,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    -1,    29,    30,    31,    32,    33,    -1,    35,
      36,    37,    38,    -1,    -1,    -1,    42,    43,    44,    -1,
      46,    -1,    -1,    -1,    50,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    78,    79,    80,    81,    82,    83,    -1,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,    -1,
      96,    97,    98,    -1,   100,    -1,   102,    -1,    -1,   105,
     106,    -1,   108,    -1,    -1,   111,   112,   113,   114,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
      -1,   127,   128,    -1,   130,    -1,   132,   133,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,   144,   145,
     146,    -1,    -1,   149,    -1,   151,   152,   153,    -1,    -1,
      -1,   157,   158,   159,   160,    -1,   162,   163,    -1,   165,
      -1,   167,   168,   169,   170,    -1,   172,    -1,   174,   175,
      -1,   177,    -1,   179,    -1,    -1,    -1,   183,    -1,   185,
      -1,    -1,   188,    -1,   190,    -1,   192,   193,   194,    -1,
     196,   197,   198,    -1,    -1,   201,    -1,   203,   204,   205,
      -1,   207,    -1,   209,    -1,    -1,    -1,    -1,   214,    -1,
     216,   217,   218,    -1,    -1,   221,    -1,    -1,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,    -1,   239,    -1,   241,   242,   243,   244,   245,
      -1,   247,   248,    -1,    -1,   251,   252,   253,    -1,    -1,
     256,    -1,    -1,    -1,   260,   261,    -1,    -1,   264,    -1,
      -1,    -1,   268,   269,   270,   271,   272,    -1,   274,   275,
     276,    -1,   278,   279,   280,   281,   282,   283,   284,    -1,
     286,   287,   288,   289,   290,   291,   292,   293,    -1,   295,
     296,   297,   298,   299,   300,    -1,    -1,   303,   304,   305,
      -1,   307,   308,   309,   310,    -1,   312,   313,    -1,   315,
     316,   317,   318,   319,   320,   321,    -1,   323,   324,    -1,
     326,   327,   328,    -1,   330,   331,    -1,   333,    -1,   335,
     336,   337,   338,    -1,   340,   341,   342,   343,    -1,    -1,
     346,   347,   348,   349,   350,    -1,    -1,   353,   354,   355,
     356,   357,    -1,   359,   360,    -1,    -1,    -1,   364,   365,
     366,   367,   368,   369,    -1,   371,   372,    -1,   374,   375,
      -1,    -1,   378,   379,   380,    -1,   382,     6,     7,     8,
       9,    -1,    11,    12,    -1,    14,    15,    16,    -1,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    -1,
      29,    30,    31,    32,    33,    -1,    35,    36,    37,    38,
      -1,    -1,    -1,    42,    43,    44,    -1,    46,    -1,    -1,
      -1,    50,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    78,
      79,    80,    81,    82,    83,    -1,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    93,    94,    -1,    96,    97,    98,
      -1,   100,    -1,   102,    -1,    -1,   105,   106,    -1,   108,
      -1,    -1,   111,   112,   113,   114,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,    -1,   127,   128,
      -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
     149,    -1,   151,   152,   153,    -1,    -1,    -1,   157,   158,
     159,   160,    -1,   162,   163,    -1,   165,    -1,   167,   168,
     169,   170,    -1,   172,    -1,   174,   175,    -1,   177,    -1,
     179,    -1,    -1,    -1,   183,    -1,   185,    -1,    -1,   188,
      -1,   190,    -1,   192,   193,   194,    -1,   196,   197,   198,
      -1,    -1,   201,    -1,   203,   204,   205,    -1,   207,    -1,
     209,    -1,    -1,    -1,    -1,   214,    -1,   216,   217,   218,
      -1,    -1,   221,    -1,    -1,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,    -1,
     239,    -1,   241,   242,   243,   244,   245,    -1,   247,   248,
      -1,    -1,   251,   252,   253,    -1,    -1,   256,    -1,    -1,
      -1,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,   268,
     269,   270,   271,   272,    -1,   274,   275,   276,    -1,   278,
     279,   280,   281,   282,   283,   284,    -1,   286,   287,   288,
     289,   290,   291,   292,   293,    -1,   295,   296,   297,   298,
     299,   300,    -1,    -1,   303,   304,   305,    -1,   307,   308,
     309,   310,    -1,   312,   313,    -1,   315,   316,   317,   318,
     319,   320,   321,    -1,   323,   324,    -1,   326,   327,   328,
      -1,   330,   331,    -1,   333,    -1,   335,   336,   337,   338,
      -1,   340,   341,   342,   343,    -1,    -1,   346,   347,   348,
     349,   350,    -1,    -1,   353,   354,   355,   356,   357,    -1,
     359,   360,    -1,    -1,    -1,   364,   365,   366,   367,   368,
     369,    -1,   371,    -1,    -1,   374,    -1,     4,   377,     6,
       7,     8,     9,    10,    11,    12,    -1,    14,    15,    16,
      -1,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    -1,    29,    30,    31,    32,    33,    -1,    35,    36,
      37,    38,    -1,    -1,    -1,    42,    43,    44,    -1,    46,
      -1,    -1,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    94,    -1,    96,
      97,    98,    -1,   100,    -1,   102,    -1,    -1,   105,   106,
      -1,   108,    -1,    -1,   111,   112,   113,   114,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,    -1,
     127,   128,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,   149,    -1,   151,   152,   153,    -1,    -1,    -1,
     157,   158,   159,   160,    -1,   162,   163,    -1,   165,    -1,
     167,   168,   169,   170,    -1,   172,    -1,   174,   175,    -1,
     177,    -1,   179,    -1,    -1,    -1,   183,    -1,   185,    -1,
      -1,   188,    -1,   190,    -1,   192,   193,   194,    -1,   196,
     197,   198,    -1,    -1,   201,    -1,   203,   204,   205,    -1,
     207,    -1,   209,    -1,    -1,    -1,    -1,   214,    -1,   216,
     217,   218,    -1,    -1,   221,    -1,    -1,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,    -1,   239,    -1,   241,   242,   243,   244,   245,    -1,
     247,   248,    -1,    -1,   251,   252,   253,    -1,    -1,   256,
      -1,    -1,    -1,   260,   261,    -1,    -1,   264,    -1,    -1,
      -1,   268,   269,   270,   271,   272,    -1,   274,   275,   276,
      -1,   278,   279,   280,   281,   282,   283,   284,    -1,   286,
     287,   288,   289,   290,   291,   292,   293,    -1,   295,   296,
     297,   298,   299,   300,    -1,    -1,   303,   304,   305,    -1,
     307,   308,   309,   310,    -1,   312,   313,    -1,   315,   316,
     317,   318,   319,   320,   321,    -1,   323,   324,    -1,   326,
     327,   328,    -1,   330,   331,    -1,   333,    -1,   335,   336,
     337,   338,    -1,   340,   341,   342,   343,    -1,    -1,   346,
     347,   348,   349,   350,    -1,    -1,   353,   354,   355,   356,
     357,    -1,   359,   360,    -1,    -1,    -1,   364,   365,   366,
     367,   368,   369,    -1,   371,    -1,    -1,   374,     5,     6,
       7,     8,     9,    -1,    11,    12,    -1,    14,    15,    16,
      -1,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    -1,    35,    36,
      37,    38,    39,    40,    -1,    42,    43,    44,    -1,    46,
      47,    48,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,    -1,   105,   106,
      -1,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,    -1,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,    -1,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,    -1,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,    -1,   203,   204,   205,   206,
     207,   208,   209,   210,    -1,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
      -1,   268,   269,   270,   271,   272,    -1,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,    -1,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,    -1,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,   360,    -1,   362,   363,   364,   365,   366,
     367,   368,   369,    -1,   371,    -1,    -1,   374,     6,     7,
       8,     9,    -1,    11,    12,    13,    14,    15,    16,    -1,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      -1,    29,    30,    31,    32,    33,    -1,    35,    36,    37,
      38,    -1,    -1,    -1,    42,    43,    44,    -1,    46,    -1,
      -1,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    93,    94,    -1,    96,    97,
      98,    -1,   100,    -1,   102,    -1,    -1,   105,   106,    -1,
     108,    -1,    -1,   111,   112,   113,   114,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,    -1,   127,
     128,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,   144,   145,   146,   147,
      -1,   149,    -1,   151,   152,   153,    -1,    -1,    -1,   157,
     158,   159,   160,    -1,   162,   163,    -1,   165,    -1,   167,
     168,   169,   170,    -1,   172,    -1,   174,   175,    -1,   177,
      -1,   179,    -1,    -1,    -1,   183,    -1,   185,    -1,    -1,
     188,    -1,   190,    -1,   192,   193,   194,    -1,   196,   197,
     198,    -1,    -1,   201,    -1,   203,   204,   205,    -1,   207,
      -1,   209,    -1,    -1,    -1,    -1,   214,    -1,   216,   217,
     218,    -1,    -1,   221,    -1,    -1,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
      -1,   239,    -1,   241,   242,   243,   244,   245,    -1,   247,
     248,    -1,    -1,   251,   252,   253,    -1,    -1,   256,    -1,
      -1,    -1,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
     268,   269,   270,   271,   272,    -1,   274,   275,   276,    -1,
     278,   279,   280,   281,   282,   283,   284,    -1,   286,   287,
     288,   289,   290,   291,   292,   293,    -1,   295,   296,   297,
     298,   299,   300,    -1,    -1,   303,   304,   305,    -1,   307,
     308,   309,   310,    -1,   312,   313,    -1,   315,   316,   317,
     318,   319,   320,   321,    -1,   323,   324,    -1,   326,   327,
     328,    -1,   330,   331,    -1,   333,    -1,   335,   336,   337,
     338,    -1,   340,   341,   342,   343,    -1,    -1,   346,   347,
     348,   349,   350,    -1,    -1,   353,   354,   355,   356,   357,
      -1,   359,   360,    -1,    -1,    -1,   364,   365,   366,   367,
     368,   369,    -1,   371,   372,    -1,   374,   375,     6,     7,
       8,     9,    -1,    11,    12,    -1,    14,    15,    16,    -1,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      -1,    29,    30,    31,    32,    33,    -1,    35,    36,    37,
      38,    -1,    -1,    41,    42,    43,    44,    -1,    46,    -1,
      -1,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      78,    79,    80,    81,    82,    83,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    93,    94,    -1,    96,    97,
      98,    -1,   100,    -1,   102,    -1,    -1,   105,   106,    -1,
     108,    -1,    -1,   111,   112,   113,   114,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,    -1,   127,
     128,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,   144,   145,   146,    -1,
      -1,   149,    -1,   151,   152,   153,    -1,    -1,    -1,   157,
     158,   159,   160,    -1,   162,   163,    -1,   165,    -1,   167,
     168,   169,   170,    -1,   172,    -1,   174,   175,    -1,   177,
      -1,   179,    -1,    -1,    -1,   183,    -1,   185,    -1,    -1,
     188,    -1,   190,    -1,   192,   193,   194,    -1,   196,   197,
     198,    -1,    -1,   201,    -1,   203,   204,   205,    -1,   207,
      -1,   209,    -1,    -1,    -1,    -1,   214,    -1,   216,   217,
     218,    -1,    -1,   221,    -1,    -1,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
      -1,   239,   240,   241,   242,   243,   244,   245,    -1,   247,
     248,    -1,    -1,   251,   252,   253,    -1,    -1,   256,   257,
      -1,    -1,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
     268,   269,   270,   271,   272,    -1,   274,   275,   276,    -1,
     278,   279,   280,   281,   282,   283,   284,    -1,   286,   287,
     288,   289,   290,   291,   292,   293,    -1,   295,   296,   297,
     298,   299,   300,    -1,    -1,   303,   304,   305,    -1,   307,
     308,   309,   310,    -1,   312,   313,    -1,   315,   316,   317,
     318,   319,   320,   321,    -1,   323,   324,   325,   326,   327,
     328,    -1,   330,   331,    -1,   333,    -1,   335,   336,   337,
     338,    -1,   340,   341,   342,   343,    -1,    -1,   346,   347,
     348,   349,   350,    -1,    -1,   353,   354,   355,   356,   357,
      -1,   359,   360,    -1,    -1,    -1,   364,   365,   366,   367,
     368,   369,    -1,   371,    -1,    -1,   374,   375,     6,     7,
       8,     9,    -1,    11,    12,    -1,    14,    15,    16,    -1,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      -1,    29,    30,    31,    32,    33,    -1,    35,    36,    37,
      38,    -1,    -1,    -1,    42,    43,    44,    -1,    46,    -1,
      -1,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      78,    79,    80,    81,    82,    83,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    93,    94,    -1,    96,    97,
      98,    -1,   100,    -1,   102,    -1,    -1,   105,   106,    -1,
     108,    -1,    -1,   111,   112,   113,   114,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,    -1,   127,
     128,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,   144,   145,   146,    -1,
      -1,   149,    -1,   151,   152,   153,    -1,    -1,    -1,   157,
     158,   159,   160,    -1,   162,   163,    -1,   165,    -1,   167,
     168,   169,   170,    -1,   172,    -1,   174,   175,    -1,   177,
      -1,   179,    -1,    -1,    -1,   183,    -1,   185,    -1,    -1,
     188,    -1,   190,    -1,   192,   193,   194,    -1,   196,   197,
     198,    -1,    -1,   201,    -1,   203,   204,   205,    -1,   207,
      -1,   209,    -1,    -1,    -1,    -1,   214,    -1,   216,   217,
     218,    -1,    -1,   221,    -1,    -1,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
      -1,   239,    -1,   241,   242,   243,   244,   245,    -1,   247,
     248,    -1,    -1,   251,   252,   253,    -1,    -1,   256,    -1,
      -1,    -1,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
     268,   269,   270,   271,   272,    -1,   274,   275,   276,    -1,
     278,   279,   280,   281,   282,   283,   284,    -1,   286,   287,
     288,   289,   290,   291,   292,   293,    -1,   295,   296,   297,
     298,   299,   300,    -1,    -1,   303,   304,   305,    -1,   307,
     308,   309,   310,    -1,   312,   313,    -1,   315,   316,   317,
     318,   319,   320,   321,    -1,   323,   324,    -1,   326,   327,
     328,    -1,   330,   331,   332,   333,    -1,   335,   336,   337,
     338,    -1,   340,   341,   342,   343,    -1,    -1,   346,   347,
     348,   349,   350,    -1,    -1,   353,   354,   355,   356,   357,
      -1,   359,   360,    -1,    -1,    -1,   364,   365,   366,   367,
     368,   369,    -1,   371,   372,    -1,   374,   375,     6,     7,
       8,     9,    -1,    11,    12,    -1,    14,    15,    16,    -1,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      -1,    29,    30,    31,    32,    33,    -1,    35,    36,    37,
      38,    -1,    -1,    -1,    42,    43,    44,    -1,    46,    -1,
      -1,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      78,    79,    80,    81,    82,    83,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    93,    94,    -1,    96,    97,
      98,    -1,   100,    -1,   102,    -1,    -1,   105,   106,    -1,
     108,    -1,    -1,   111,   112,   113,   114,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,    -1,   127,
     128,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,   144,   145,   146,   147,
      -1,   149,    -1,   151,   152,   153,    -1,    -1,    -1,   157,
     158,   159,   160,    -1,   162,   163,    -1,   165,    -1,   167,
     168,   169,   170,    -1,   172,    -1,   174,   175,    -1,   177,
      -1,   179,    -1,    -1,    -1,   183,    -1,   185,    -1,    -1,
     188,    -1,   190,    -1,   192,   193,   194,    -1,   196,   197,
     198,    -1,    -1,   201,    -1,   203,   204,   205,    -1,   207,
      -1,   209,    -1,    -1,    -1,    -1,   214,    -1,   216,   217,
     218,    -1,    -1,   221,    -1,    -1,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
      -1,   239,    -1,   241,   242,   243,   244,   245,    -1,   247,
     248,    -1,    -1,   251,   252,   253,    -1,    -1,   256,    -1,
      -1,    -1,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
     268,   269,   270,   271,   272,    -1,   274,   275,   276,    -1,
     278,   279,   280,   281,   282,   283,   284,    -1,   286,   287,
     288,   289,   290,   291,   292,   293,    -1,   295,   296,   297,
     298,   299,   300,    -1,    -1,   303,   304,   305,    -1,   307,
     308,   309,   310,    -1,   312,   313,    -1,   315,   316,   317,
     318,   319,   320,   321,    -1,   323,   324,    -1,   326,   327,
     328,    -1,   330,   331,    -1,   333,    -1,   335,   336,   337,
     338,    -1,   340,   341,   342,   343,    -1,    -1,   346,   347,
     348,   349,   350,    -1,    -1,   353,   354,   355,   356,   357,
      -1,   359,   360,    -1,    -1,    -1,   364,   365,   366,   367,
     368,   369,    -1,   371,   372,    -1,   374,   375,     6,     7,
       8,     9,    -1,    11,    12,    -1,    14,    15,    16,    -1,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      -1,    29,    30,    31,    32,    33,    -1,    35,    36,    37,
      38,    -1,    -1,    -1,    42,    43,    44,    -1,    46,    -1,
      -1,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      78,    79,    80,    81,    82,    83,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    93,    94,    -1,    96,    97,
      98,    -1,   100,    -1,   102,    -1,    -1,   105,   106,    -1,
     108,    -1,    -1,   111,   112,   113,   114,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,    -1,   127,
     128,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,   144,   145,   146,   147,
      -1,   149,    -1,   151,   152,   153,    -1,    -1,    -1,   157,
     158,   159,   160,    -1,   162,   163,    -1,   165,    -1,   167,
     168,   169,   170,    -1,   172,    -1,   174,   175,    -1,   177,
      -1,   179,    -1,    -1,    -1,   183,    -1,   185,    -1,    -1,
     188,    -1,   190,    -1,   192,   193,   194,    -1,   196,   197,
     198,    -1,    -1,   201,    -1,   203,   204,   205,    -1,   207,
      -1,   209,    -1,    -1,    -1,    -1,   214,    -1,   216,   217,
     218,    -1,    -1,   221,    -1,    -1,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
      -1,   239,    -1,   241,   242,   243,   244,   245,    -1,   247,
     248,    -1,    -1,   251,   252,   253,    -1,    -1,   256,    -1,
      -1,    -1,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
     268,   269,   270,   271,   272,    -1,   274,   275,   276,    -1,
     278,   279,   280,   281,   282,   283,   284,    -1,   286,   287,
     288,   289,   290,   291,   292,   293,    -1,   295,   296,   297,
     298,   299,   300,    -1,    -1,   303,   304,   305,    -1,   307,
     308,   309,   310,    -1,   312,   313,    -1,   315,   316,   317,
     318,   319,   320,   321,    -1,   323,   324,    -1,   326,   327,
     328,    -1,   330,   331,    -1,   333,    -1,   335,   336,   337,
     338,    -1,   340,   341,   342,   343,    -1,    -1,   346,   347,
     348,   349,   350,    -1,    -1,   353,   354,   355,   356,   357,
      -1,   359,   360,    -1,    -1,    -1,   364,   365,   366,   367,
     368,   369,    -1,   371,   372,    -1,   374,   375,     6,     7,
       8,     9,    -1,    11,    12,    -1,    14,    15,    16,    -1,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      -1,    29,    30,    31,    32,    33,    -1,    35,    36,    37,
      38,    -1,    -1,    -1,    42,    43,    44,    -1,    46,    -1,
      -1,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      78,    79,    80,    81,    82,    83,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    93,    94,    -1,    96,    97,
      98,    -1,   100,    -1,   102,    -1,    -1,   105,   106,    -1,
     108,    -1,    -1,   111,   112,   113,   114,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,    -1,   127,
     128,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,   144,   145,   146,   147,
      -1,   149,    -1,   151,   152,   153,    -1,    -1,    -1,   157,
     158,   159,   160,    -1,   162,   163,    -1,   165,    -1,   167,
     168,   169,   170,    -1,   172,    -1,   174,   175,    -1,   177,
      -1,   179,    -1,    -1,    -1,   183,    -1,   185,    -1,    -1,
     188,    -1,   190,    -1,   192,   193,   194,    -1,   196,   197,
     198,    -1,    -1,   201,    -1,   203,   204,   205,    -1,   207,
      -1,   209,    -1,    -1,    -1,    -1,   214,    -1,   216,   217,
     218,    -1,    -1,   221,    -1,    -1,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
      -1,   239,    -1,   241,   242,   243,   244,   245,    -1,   247,
     248,    -1,    -1,   251,   252,   253,    -1,    -1,   256,    -1,
      -1,    -1,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
     268,   269,   270,   271,   272,    -1,   274,   275,   276,    -1,
     278,   279,   280,   281,   282,   283,   284,    -1,   286,   287,
     288,   289,   290,   291,   292,   293,    -1,   295,   296,   297,
     298,   299,   300,    -1,    -1,   303,   304,   305,    -1,   307,
     308,   309,   310,    -1,   312,   313,    -1,   315,   316,   317,
     318,   319,   320,   321,    -1,   323,   324,    -1,   326,   327,
     328,    -1,   330,   331,    -1,   333,    -1,   335,   336,   337,
     338,    -1,   340,   341,   342,   343,    -1,    -1,   346,   347,
     348,   349,   350,    -1,    -1,   353,   354,   355,   356,   357,
      -1,   359,   360,    -1,    -1,    -1,   364,   365,   366,   367,
     368,   369,    -1,   371,   372,    -1,   374,   375,     6,     7,
       8,     9,    -1,    11,    12,    -1,    14,    15,    16,    -1,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      -1,    29,    30,    31,    32,    33,    -1,    35,    36,    37,
      38,    -1,    -1,    -1,    42,    43,    44,    -1,    46,    -1,
      -1,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      78,    79,    80,    81,    82,    83,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    93,    94,    -1,    96,    97,
      98,    -1,   100,    -1,   102,    -1,    -1,   105,   106,    -1,
     108,    -1,    -1,   111,   112,   113,   114,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,    -1,   127,
     128,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,   144,   145,   146,    -1,
      -1,   149,    -1,   151,   152,   153,    -1,    -1,    -1,   157,
     158,   159,   160,    -1,   162,   163,    -1,   165,    -1,   167,
     168,   169,   170,    -1,   172,    -1,   174,   175,    -1,   177,
      -1,   179,    -1,    -1,    -1,   183,    -1,   185,    -1,    -1,
     188,    -1,   190,    -1,   192,   193,   194,    -1,   196,   197,
     198,    -1,    -1,   201,    -1,   203,   204,   205,    -1,   207,
      -1,   209,    -1,    -1,    -1,    -1,   214,    -1,   216,   217,
     218,    -1,    -1,   221,    -1,    -1,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
      -1,   239,    -1,   241,   242,   243,   244,   245,    -1,   247,
     248,    -1,    -1,   251,   252,   253,    -1,    -1,   256,    -1,
      -1,    -1,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
     268,   269,   270,   271,   272,    -1,   274,   275,   276,    -1,
     278,   279,   280,   281,   282,   283,   284,    -1,   286,   287,
     288,   289,   290,   291,   292,   293,    -1,   295,   296,   297,
     298,   299,   300,    -1,    -1,   303,   304,   305,    -1,   307,
     308,   309,   310,    -1,   312,   313,    -1,   315,   316,   317,
     318,   319,   320,   321,    -1,   323,   324,    -1,   326,   327,
     328,    -1,   330,   331,    -1,   333,    -1,   335,   336,   337,
     338,    -1,   340,   341,   342,   343,    -1,    -1,   346,   347,
     348,   349,   350,    -1,    -1,   353,   354,   355,   356,   357,
      -1,   359,   360,    -1,    -1,    -1,   364,   365,   366,   367,
     368,   369,    -1,   371,   372,    -1,   374,   375,     6,     7,
       8,     9,    -1,    11,    12,    -1,    14,    15,    16,    -1,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      -1,    29,    30,    31,    32,    33,    -1,    35,    36,    37,
      38,    -1,    -1,    -1,    42,    43,    44,    -1,    46,    -1,
      -1,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      78,    79,    80,    81,    82,    83,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    93,    94,    -1,    96,    97,
      98,    -1,   100,    -1,   102,    -1,    -1,   105,   106,    -1,
     108,    -1,    -1,   111,   112,   113,   114,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,    -1,   127,
     128,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,   144,   145,   146,    -1,
      -1,   149,    -1,   151,   152,   153,    -1,    -1,    -1,   157,
     158,   159,   160,    -1,   162,   163,    -1,   165,    -1,   167,
     168,   169,   170,    -1,   172,    -1,   174,   175,    -1,   177,
      -1,   179,    -1,    -1,    -1,   183,    -1,   185,    -1,    -1,
     188,    -1,   190,    -1,   192,   193,   194,    -1,   196,   197,
     198,    -1,    -1,   201,    -1,   203,   204,   205,    -1,   207,
      -1,   209,    -1,    -1,    -1,    -1,   214,    -1,   216,   217,
     218,    -1,    -1,   221,    -1,    -1,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
      -1,   239,    -1,   241,   242,   243,   244,   245,    -1,   247,
     248,    -1,    -1,   251,   252,   253,    -1,    -1,   256,    -1,
      -1,    -1,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
     268,   269,   270,   271,   272,    -1,   274,   275,   276,    -1,
     278,   279,   280,   281,   282,   283,   284,    -1,   286,   287,
     288,   289,   290,   291,   292,   293,    -1,   295,   296,   297,
     298,   299,   300,    -1,    -1,   303,   304,   305,    -1,   307,
     308,   309,   310,    -1,   312,   313,    -1,   315,   316,   317,
     318,   319,   320,   321,    -1,   323,   324,    -1,   326,   327,
     328,    -1,   330,   331,    -1,   333,    -1,   335,   336,   337,
     338,    -1,   340,   341,   342,   343,    -1,    -1,   346,   347,
     348,   349,   350,    -1,    -1,   353,   354,   355,   356,   357,
      -1,   359,   360,    -1,    -1,    -1,   364,   365,   366,   367,
     368,   369,    -1,   371,    -1,    -1,   374,   375,     6,     7,
       8,     9,    -1,    11,    12,    -1,    14,    15,    16,    -1,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      -1,    29,    30,    31,    32,    33,    -1,    35,    36,    37,
      38,    -1,    -1,    -1,    42,    43,    44,    -1,    46,    -1,
      -1,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      78,    79,    80,    81,    82,    83,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    93,    94,    -1,    96,    97,
      98,    -1,   100,    -1,   102,    -1,    -1,   105,   106,    -1,
     108,    -1,    -1,   111,   112,   113,   114,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,    -1,   127,
     128,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,   144,   145,   146,    -1,
      -1,   149,    -1,   151,   152,   153,    -1,    -1,    -1,   157,
     158,   159,   160,    -1,   162,   163,    -1,   165,    -1,   167,
     168,   169,   170,    -1,   172,    -1,   174,   175,    -1,   177,
      -1,   179,    -1,    -1,    -1,   183,    -1,   185,    -1,    -1,
     188,    -1,   190,    -1,   192,   193,   194,    -1,   196,   197,
     198,    -1,    -1,   201,    -1,   203,   204,   205,    -1,   207,
      -1,   209,    -1,    -1,    -1,    -1,   214,    -1,   216,   217,
     218,    -1,    -1,   221,    -1,    -1,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
      -1,   239,    -1,   241,   242,   243,   244,   245,    -1,   247,
     248,    -1,    -1,   251,   252,   253,    -1,    -1,   256,    -1,
      -1,    -1,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
     268,   269,   270,   271,   272,    -1,   274,   275,   276,    -1,
     278,   279,   280,   281,   282,   283,   284,    -1,   286,   287,
     288,   289,   290,   291,   292,   293,    -1,   295,   296,   297,
     298,   299,   300,    -1,    -1,   303,   304,   305,    -1,   307,
     308,   309,   310,    -1,   312,   313,    -1,   315,   316,   317,
     318,   319,   320,   321,    -1,   323,   324,    -1,   326,   327,
     328,    -1,   330,   331,    -1,   333,    -1,   335,   336,   337,
     338,    -1,   340,   341,   342,   343,    -1,    -1,   346,   347,
     348,   349,   350,    -1,    -1,   353,   354,   355,   356,   357,
      -1,   359,   360,    -1,    -1,    -1,   364,   365,   366,   367,
     368,   369,    -1,   371,    -1,    -1,   374,   375,     5,     6,
       7,     8,     9,    -1,    11,    12,    -1,    14,    15,    16,
      -1,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    -1,    35,    36,
      37,    38,    39,    40,    -1,    42,    43,    44,    -1,    46,
      47,    48,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    94,    -1,    96,
      97,    98,    -1,   100,    -1,   102,    -1,   104,   105,   106,
      -1,   108,    -1,    -1,   111,   112,   113,   114,    -1,   116,
     117,   118,   119,    -1,    -1,    -1,   123,   124,   125,    -1,
     127,   128,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,   149,    -1,   151,   152,   153,    -1,    -1,    -1,
     157,   158,   159,   160,    -1,   162,   163,    -1,   165,    -1,
     167,   168,    -1,   170,    -1,    -1,    -1,   174,   175,    -1,
     177,    -1,   179,    -1,    -1,    -1,   183,    -1,   185,    -1,
      -1,   188,    -1,   190,    -1,   192,   193,   194,    -1,   196,
     197,   198,    -1,    -1,   201,    -1,   203,   204,   205,   206,
     207,    -1,   209,    -1,    -1,    -1,    -1,   214,    -1,   216,
     217,   218,    -1,    -1,   221,    -1,    -1,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,    -1,   239,    -1,   241,   242,   243,   244,   245,    -1,
     247,   248,    -1,    -1,    -1,   252,   253,    -1,    -1,   256,
      -1,    -1,    -1,   260,   261,    -1,    -1,   264,    -1,    -1,
      -1,   268,   269,   270,   271,   272,    -1,    -1,   275,   276,
      -1,   278,   279,   280,   281,   282,   283,   284,    -1,   286,
     287,   288,   289,   290,   291,   292,   293,    -1,   295,    -1,
     297,   298,   299,   300,    -1,    -1,   303,   304,   305,    -1,
     307,   308,   309,   310,    -1,   312,   313,    -1,   315,   316,
     317,   318,   319,   320,   321,    -1,    -1,   324,    -1,   326,
     327,   328,    -1,   330,   331,    -1,   333,    -1,   335,    -1,
     337,    -1,    -1,   340,   341,   342,   343,    -1,    -1,   346,
     347,   348,   349,   350,    -1,    -1,   353,   354,   355,   356,
     357,    -1,   359,   360,    -1,    -1,    -1,   364,   365,   366,
     367,   368,   369,    -1,   371,    -1,    -1,   374,     5,     6,
       7,     8,     9,    -1,    11,    12,    -1,    14,    15,    16,
      -1,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    -1,    35,    36,
      37,    38,    39,    40,    -1,    42,    43,    44,    -1,    46,
      47,    48,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    94,    -1,    96,
      97,    98,    -1,   100,    -1,   102,    -1,   104,   105,   106,
      -1,   108,    -1,    -1,   111,   112,   113,   114,    -1,   116,
     117,   118,   119,    -1,    -1,    -1,   123,   124,   125,    -1,
     127,   128,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,   149,    -1,   151,   152,   153,    -1,    -1,    -1,
     157,   158,   159,   160,    -1,   162,   163,    -1,   165,    -1,
     167,   168,    -1,   170,    -1,    -1,    -1,   174,   175,    -1,
     177,    -1,   179,    -1,    -1,    -1,   183,    -1,   185,    -1,
      -1,   188,    -1,   190,    -1,   192,   193,   194,   195,   196,
     197,   198,    -1,    -1,   201,    -1,   203,   204,   205,   206,
     207,    -1,   209,    -1,    -1,    -1,    -1,   214,    -1,   216,
     217,   218,    -1,    -1,   221,    -1,    -1,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,    -1,   239,    -1,   241,   242,   243,   244,    -1,    -1,
     247,   248,    -1,    -1,    -1,   252,   253,    -1,    -1,   256,
      -1,    -1,    -1,   260,   261,    -1,    -1,   264,    -1,    -1,
      -1,   268,   269,   270,   271,   272,    -1,    -1,   275,   276,
      -1,   278,   279,   280,   281,   282,   283,   284,    -1,   286,
     287,   288,   289,   290,   291,   292,   293,    -1,   295,    -1,
     297,   298,   299,   300,    -1,    -1,   303,   304,   305,    -1,
     307,   308,   309,   310,    -1,   312,   313,    -1,   315,   316,
     317,   318,   319,   320,   321,    -1,    -1,   324,    -1,   326,
     327,   328,    -1,   330,   331,    -1,   333,    -1,   335,    -1,
     337,    -1,    -1,   340,   341,   342,   343,    -1,    -1,   346,
     347,   348,   349,   350,    -1,    -1,   353,   354,   355,   356,
     357,    -1,   359,   360,    -1,    -1,    -1,   364,   365,   366,
     367,   368,   369,    -1,   371,    -1,    -1,   374,     5,     6,
       7,     8,     9,    -1,    11,    12,    -1,    14,    15,    16,
      -1,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    -1,    35,    36,
      37,    38,    39,    40,    -1,    42,    43,    44,    -1,    46,
      47,    48,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    94,    -1,    96,
      97,    98,    -1,   100,    -1,   102,    -1,   104,   105,   106,
      -1,   108,    -1,    -1,   111,   112,   113,   114,    -1,   116,
     117,   118,   119,    -1,    -1,    -1,   123,   124,   125,    -1,
     127,   128,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,   149,    -1,   151,   152,   153,    -1,    -1,    -1,
     157,   158,   159,   160,    -1,   162,   163,    -1,   165,    -1,
     167,   168,    -1,   170,    -1,    -1,    -1,   174,   175,    -1,
     177,    -1,   179,    -1,    -1,    -1,   183,    -1,   185,    -1,
      -1,   188,    -1,   190,    -1,   192,   193,   194,    -1,   196,
     197,   198,    -1,    -1,   201,    -1,   203,   204,   205,   206,
     207,    -1,   209,    -1,    -1,    -1,    -1,   214,    -1,   216,
     217,   218,    -1,    -1,   221,    -1,    -1,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,    -1,   239,    -1,   241,   242,   243,   244,   245,    -1,
     247,   248,    -1,    -1,    -1,   252,   253,    -1,    -1,   256,
      -1,    -1,    -1,   260,   261,    -1,    -1,   264,    -1,    -1,
      -1,   268,   269,   270,   271,   272,    -1,    -1,   275,   276,
      -1,   278,   279,   280,   281,   282,   283,   284,    -1,   286,
     287,   288,   289,   290,   291,   292,   293,    -1,   295,    -1,
     297,   298,   299,   300,    -1,    -1,   303,   304,   305,    -1,
     307,   308,   309,   310,    -1,   312,   313,    -1,   315,   316,
     317,   318,   319,   320,   321,    -1,    -1,   324,    -1,   326,
     327,   328,    -1,   330,   331,    -1,   333,    -1,   335,    -1,
     337,    -1,    -1,   340,   341,   342,   343,    -1,    -1,   346,
     347,   348,   349,   350,    -1,    -1,   353,   354,   355,   356,
     357,    -1,   359,   360,    -1,    -1,    -1,   364,   365,   366,
     367,   368,   369,    -1,   371,    -1,    -1,   374,     5,     6,
       7,     8,     9,    -1,    11,    12,    -1,    14,    15,    16,
      -1,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    -1,    35,    36,
      37,    38,    39,    40,    -1,    42,    43,    44,    -1,    46,
      47,    48,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    94,    -1,    96,
      97,    98,    -1,   100,    -1,   102,    -1,   104,   105,   106,
      -1,   108,    -1,    -1,   111,   112,   113,   114,    -1,   116,
     117,   118,   119,    -1,    -1,    -1,   123,   124,   125,    -1,
     127,   128,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,   149,    -1,   151,   152,   153,    -1,    -1,    -1,
     157,   158,   159,   160,    -1,   162,   163,    -1,   165,    -1,
     167,   168,    -1,   170,    -1,    -1,    -1,   174,   175,    -1,
     177,    -1,   179,    -1,    -1,    -1,   183,    -1,   185,    -1,
      -1,   188,    -1,   190,    -1,   192,   193,   194,    -1,   196,
     197,   198,    -1,    -1,   201,    -1,   203,   204,   205,   206,
     207,    -1,   209,    -1,    -1,    -1,    -1,   214,    -1,   216,
     217,   218,    -1,    -1,   221,    -1,    -1,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,    -1,   239,    -1,   241,   242,   243,   244,    -1,    -1,
     247,   248,    -1,    -1,    -1,   252,   253,    -1,    -1,   256,
      -1,    -1,    -1,   260,   261,    -1,    -1,   264,    -1,    -1,
      -1,   268,   269,   270,   271,   272,    -1,    -1,   275,   276,
      -1,   278,   279,   280,   281,   282,   283,   284,    -1,   286,
     287,   288,   289,   290,   291,   292,   293,    -1,   295,    -1,
     297,   298,   299,   300,    -1,    -1,   303,   304,   305,    -1,
     307,   308,   309,   310,    -1,   312,   313,    -1,   315,   316,
     317,   318,   319,   320,   321,    -1,    -1,   324,    -1,   326,
     327,   328,    -1,   330,   331,    -1,   333,    -1,   335,    -1,
     337,    -1,    -1,   340,   341,   342,   343,    -1,    -1,   346,
     347,   348,   349,   350,    -1,    -1,   353,   354,   355,   356,
     357,    -1,   359,   360,    -1,    -1,    -1,   364,   365,   366,
     367,   368,   369,    -1,   371,    -1,    -1,   374,     5,     6,
       7,     8,     9,    -1,    11,    12,    -1,    14,    15,    16,
      -1,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    -1,    35,    36,
      37,    38,    39,    40,    -1,    42,    43,    44,    -1,    46,
      47,    48,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    94,    -1,    96,
      97,    98,    -1,   100,    -1,   102,    -1,   104,   105,   106,
      -1,   108,    -1,    -1,   111,   112,   113,   114,    -1,   116,
     117,   118,   119,    -1,    -1,    -1,   123,   124,   125,    -1,
     127,   128,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,   149,    -1,   151,   152,   153,    -1,    -1,    -1,
     157,   158,   159,   160,    -1,   162,   163,    -1,   165,    -1,
     167,   168,    -1,   170,    -1,    -1,    -1,   174,   175,    -1,
     177,    -1,   179,    -1,    -1,    -1,   183,    -1,   185,    -1,
      -1,   188,    -1,   190,    -1,   192,   193,   194,    -1,   196,
     197,   198,    -1,    -1,   201,    -1,   203,   204,   205,   206,
     207,    -1,   209,    -1,    -1,    -1,    -1,   214,    -1,   216,
     217,   218,    -1,    -1,   221,    -1,    -1,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,    -1,   239,    -1,   241,   242,   243,   244,    -1,    -1,
     247,   248,    -1,    -1,    -1,   252,   253,    -1,    -1,   256,
      -1,    -1,    -1,   260,   261,    -1,    -1,   264,    -1,    -1,
      -1,   268,   269,   270,   271,   272,    -1,    -1,   275,   276,
      -1,   278,   279,   280,   281,   282,   283,   284,    -1,   286,
     287,   288,   289,   290,   291,   292,   293,    -1,   295,    -1,
     297,   298,   299,   300,    -1,    -1,   303,   304,   305,    -1,
     307,    -1,   309,   310,    -1,   312,   313,    -1,   315,   316,
     317,   318,   319,   320,   321,    -1,    -1,   324,    -1,   326,
     327,   328,    -1,   330,   331,    -1,   333,    -1,   335,    -1,
     337,    -1,    -1,   340,   341,   342,   343,    -1,    -1,   346,
     347,   348,   349,   350,    -1,    -1,   353,   354,   355,   356,
     357,    -1,   359,   360,    -1,    -1,    -1,   364,   365,   366,
     367,   368,   369,    -1,   371,    -1,    -1,   374,     5,     6,
       7,     8,     9,    -1,    11,    12,    -1,    14,    15,    16,
      -1,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    -1,    35,    36,
      37,    38,    39,    40,    -1,    42,    43,    44,    -1,    46,
      47,    48,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    94,    -1,    96,
      97,    98,    -1,   100,    -1,   102,    -1,   104,   105,   106,
      -1,   108,    -1,    -1,   111,   112,   113,   114,    -1,   116,
     117,   118,   119,    -1,    -1,    -1,   123,   124,   125,    -1,
     127,   128,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,   149,    -1,   151,   152,   153,    -1,    -1,    -1,
     157,   158,   159,   160,    -1,   162,   163,    -1,   165,    -1,
     167,   168,    -1,   170,    -1,    -1,    -1,   174,   175,    -1,
     177,    -1,   179,    -1,    -1,    -1,   183,    -1,   185,    -1,
      -1,   188,    -1,   190,    -1,   192,   193,   194,    -1,   196,
     197,   198,    -1,    -1,   201,    -1,   203,   204,   205,   206,
     207,    -1,    -1,    -1,    -1,    -1,    -1,   214,    -1,   216,
     217,   218,    -1,    -1,   221,    -1,    -1,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,    -1,   239,    -1,   241,   242,   243,   244,    -1,    -1,
     247,   248,    -1,    -1,    -1,   252,   253,    -1,    -1,   256,
      -1,    -1,    -1,   260,   261,    -1,    -1,   264,    -1,    -1,
      -1,   268,   269,   270,   271,   272,    -1,    -1,   275,   276,
      -1,   278,   279,   280,   281,   282,   283,   284,    -1,   286,
     287,   288,   289,   290,   291,   292,   293,    -1,   295,    -1,
     297,   298,   299,   300,    -1,    -1,   303,   304,   305,    -1,
     307,    -1,   309,   310,    -1,   312,   313,    -1,   315,   316,
     317,   318,   319,   320,   321,    -1,    -1,   324,    -1,   326,
     327,   328,    -1,   330,   331,    -1,   333,    -1,   335,    -1,
     337,    -1,    -1,   340,   341,   342,   343,    -1,    -1,   346,
     347,   348,   349,   350,    -1,    -1,   353,   354,   355,   356,
     357,    -1,   359,   360,    -1,    -1,    -1,   364,   365,   366,
     367,   368,   369,    -1,   371,    -1,    -1,   374,     6,     7,
       8,     9,    -1,    11,    12,    -1,    14,    15,    16,    -1,
      18,    19,    20,    -1,    22,    23,    24,    25,    26,    27,
      -1,    29,    30,    31,    32,    33,    -1,    35,    36,    37,
      38,    -1,    40,    -1,    42,    43,    44,    -1,    46,    47,
      48,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,    -1,   105,   106,    -1,
     108,   109,   110,   111,    -1,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,    -1,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,    -1,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    -1,   203,   204,   205,    -1,   207,
     208,   209,   210,    -1,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   265,   266,    -1,
     268,   269,   270,   271,   272,    -1,   274,   275,   276,   277,
     278,   279,   280,   281,   282,   283,   284,   285,   286,   287,
     288,   289,   290,   291,   292,   293,   294,   295,   296,   297,
     298,   299,   300,    -1,   302,   303,   304,   305,   306,   307,
     308,   309,   310,   311,   312,   313,   314,   315,   316,   317,
     318,   319,   320,   321,    -1,   323,   324,   325,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,   356,   357,
     358,   359,   360,    -1,   362,   363,   364,   365,   366,   367,
     368,   369,    -1,   371,    -1,    -1,   374,     6,     7,     8,
       9,    -1,    11,    12,    -1,    14,    15,    16,    -1,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    -1,
      29,    30,    31,    32,    33,    -1,    35,    36,    37,    38,
      -1,    -1,    -1,    42,    43,    44,    -1,    46,    -1,    -1,
      -1,    50,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,    -1,   105,   106,    -1,   108,
     109,   110,   111,    -1,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
      -1,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,    -1,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,    -1,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,    -1,   203,   204,   205,    -1,   207,   208,
     209,   210,    -1,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,    -1,   268,
     269,   270,   271,   272,    -1,   274,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   291,   292,   293,   294,   295,   296,   297,   298,
     299,   300,    -1,   302,   303,   304,   305,   306,   307,   308,
     309,   310,   311,   312,   313,   314,   315,   316,   317,   318,
     319,   320,   321,    -1,   323,   324,   325,   326,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,   343,    -1,   345,   346,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,    -1,   362,   363,   364,   365,   366,   367,   368,
     369,    -1,   371,    -1,    -1,   374,     6,     7,     8,     9,
      -1,    11,    12,    -1,    14,    15,    16,    -1,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    -1,    29,
      30,    31,    32,    33,    -1,    35,    36,    37,    38,    -1,
      -1,    -1,    42,    43,    44,    -1,    46,    -1,    -1,    -1,
      50,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    78,    79,
      80,    81,    82,    83,    -1,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,    -1,    -1,   105,   106,    -1,   108,    -1,
      -1,   111,   112,   113,   114,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,    -1,   127,   128,    -1,
     130,    -1,   132,   133,   134,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,    -1,   149,
      -1,   151,   152,   153,    -1,    -1,    -1,   157,   158,   159,
     160,    -1,   162,   163,    -1,   165,    -1,   167,   168,   169,
     170,    -1,   172,    -1,   174,   175,    -1,   177,    -1,   179,
     180,    -1,   182,   183,    -1,   185,    -1,    -1,   188,    -1,
     190,   191,   192,   193,   194,   195,   196,   197,   198,    -1,
     200,   201,    -1,   203,   204,   205,    -1,   207,    -1,   209,
      -1,    -1,   212,   213,   214,   215,   216,   217,   218,    -1,
     220,   221,   222,    -1,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,    -1,   247,   248,   249,
      -1,   251,   252,   253,    -1,    -1,   256,   257,    -1,    -1,
     260,   261,    -1,    -1,   264,   265,   266,    -1,   268,   269,
     270,   271,   272,    -1,   274,   275,   276,    -1,   278,   279,
     280,   281,   282,   283,   284,    -1,   286,   287,   288,   289,
     290,   291,   292,   293,   294,   295,   296,   297,   298,   299,
     300,    -1,    -1,   303,   304,   305,    -1,   307,   308,   309,
     310,   311,   312,   313,    -1,   315,   316,   317,   318,   319,
     320,   321,    -1,   323,   324,    -1,   326,   327,   328,    -1,
     330,   331,    -1,   333,    -1,   335,   336,   337,   338,    -1,
     340,   341,   342,   343,    -1,    -1,   346,   347,   348,   349,
     350,    -1,    -1,   353,   354,   355,   356,   357,   358,   359,
     360,    -1,    -1,    -1,   364,   365,   366,   367,   368,   369,
      -1,   371,    -1,    -1,   374,     6,     7,     8,     9,    -1,
      11,    12,    -1,    14,    15,    16,    -1,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    -1,    29,    30,
      31,    32,    33,    -1,    35,    36,    37,    38,    -1,    -1,
      -1,    42,    43,    44,    -1,    46,    -1,    -1,    -1,    50,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    78,    79,    80,
      81,    82,    83,    -1,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    94,    -1,    96,    97,    98,    -1,   100,
      -1,   102,    -1,    -1,   105,   106,    -1,   108,    -1,    -1,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,   127,   128,    -1,   130,
      -1,   132,   133,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,   144,   145,   146,    -1,    -1,   149,    -1,
     151,   152,   153,    -1,    -1,    -1,   157,   158,   159,   160,
      -1,   162,   163,    -1,   165,    -1,   167,   168,   169,   170,
      -1,   172,    -1,   174,   175,    -1,   177,   178,   179,    -1,
      -1,    -1,   183,    -1,   185,    -1,    -1,   188,    -1,   190,
      -1,   192,   193,   194,    -1,   196,   197,   198,    -1,    -1,
     201,    -1,   203,   204,   205,    -1,   207,    -1,   209,    -1,
      -1,    -1,    -1,   214,    -1,   216,   217,   218,    -1,    -1,
     221,   222,    -1,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,    -1,   239,    -1,
     241,   242,   243,   244,   245,    -1,   247,   248,    -1,    -1,
     251,   252,   253,    -1,    -1,   256,    -1,    -1,    -1,   260,
     261,    -1,    -1,   264,    -1,    -1,    -1,   268,   269,   270,
     271,   272,    -1,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,    -1,   286,   287,   288,   289,   290,
     291,   292,   293,    -1,   295,   296,   297,   298,   299,   300,
      -1,    -1,   303,   304,   305,    -1,   307,   308,   309,   310,
      -1,   312,   313,    -1,   315,   316,   317,   318,   319,   320,
     321,    -1,   323,   324,    -1,   326,   327,   328,    -1,   330,
     331,    -1,   333,    -1,   335,   336,   337,   338,    -1,   340,
     341,   342,   343,    -1,   345,   346,   347,   348,   349,   350,
      -1,    -1,   353,   354,   355,   356,   357,    -1,   359,   360,
      -1,    -1,    -1,   364,   365,   366,   367,   368,   369,    -1,
     371,    -1,    -1,   374,     6,     7,     8,     9,    -1,    11,
      12,    -1,    14,    15,    16,    -1,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    -1,    29,    30,    31,
      32,    33,    -1,    35,    36,    37,    38,    -1,    -1,    -1,
      42,    43,    44,    -1,    46,    -1,    -1,    -1,    50,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    78,    79,    80,    81,
      82,    83,    -1,    85,    86,    87,    -1,    -1,    -1,    -1,
      92,    93,    94,    -1,    96,    97,    98,    -1,   100,    -1,
     102,    -1,    -1,   105,   106,    -1,   108,    -1,    -1,   111,
     112,   113,   114,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,    -1,   127,   128,    -1,   130,    -1,
     132,   133,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,   144,   145,   146,    -1,    -1,   149,    -1,   151,
     152,   153,    -1,    -1,    -1,   157,   158,   159,   160,    -1,
     162,   163,    -1,   165,    -1,   167,   168,   169,   170,    -1,
     172,    -1,   174,   175,    -1,   177,    -1,   179,    -1,    -1,
      -1,   183,    -1,   185,    -1,    -1,   188,    -1,   190,    -1,
     192,   193,   194,    -1,   196,   197,   198,    -1,    -1,   201,
      -1,   203,   204,   205,    -1,   207,    -1,   209,    -1,    -1,
      -1,    -1,   214,    -1,   216,   217,   218,    -1,    -1,   221,
      -1,    -1,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,    -1,   239,   240,   241,
     242,   243,   244,   245,    -1,   247,   248,    -1,    -1,   251,
     252,   253,    -1,    -1,   256,   257,    -1,    -1,   260,   261,
      -1,    -1,   264,    -1,    -1,    -1,   268,   269,   270,   271,
     272,    -1,   274,   275,   276,    -1,   278,   279,   280,   281,
     282,   283,   284,    -1,   286,   287,   288,   289,   290,   291,
     292,   293,    -1,   295,   296,   297,   298,   299,   300,    -1,
      -1,   303,   304,   305,    -1,   307,   308,   309,   310,    -1,
     312,   313,    -1,   315,   316,   317,   318,   319,   320,   321,
      -1,   323,   324,    -1,   326,   327,   328,    -1,   330,   331,
      -1,   333,    -1,   335,   336,   337,   338,    -1,   340,   341,
     342,   343,    -1,    -1,   346,   347,   348,   349,   350,    -1,
      -1,   353,   354,   355,   356,   357,    -1,   359,   360,    -1,
      -1,    -1,   364,   365,   366,   367,   368,   369,    -1,   371,
      -1,    -1,   374,     6,     7,     8,     9,    -1,    11,    12,
      -1,    14,    15,    16,    -1,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    -1,    29,    30,    31,    32,
      33,    -1,    35,    36,    37,    38,    -1,    -1,    -1,    42,
      43,    44,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    78,    79,    80,    81,    82,
      83,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    -1,    96,    97,    98,    -1,   100,    -1,   102,
      -1,    -1,   105,   106,    -1,   108,    -1,    -1,   111,   112,
     113,   114,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,    -1,   127,   128,    -1,   130,    -1,   132,
     133,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,   144,   145,   146,    -1,    -1,   149,    -1,   151,   152,
     153,    -1,    -1,    -1,   157,   158,   159,   160,    -1,   162,
     163,    -1,   165,    -1,   167,   168,   169,   170,    -1,   172,
      -1,   174,   175,    -1,   177,    -1,   179,    -1,    -1,    -1,
     183,    -1,   185,    -1,    -1,   188,    -1,   190,    -1,   192,
     193,   194,    -1,   196,   197,   198,    -1,    -1,   201,    -1,
     203,   204,   205,    -1,   207,    -1,   209,    -1,    -1,    -1,
      -1,   214,    -1,   216,   217,   218,    -1,    -1,   221,    -1,
      -1,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,    -1,   239,   240,   241,   242,
     243,   244,   245,    -1,   247,   248,    -1,    -1,   251,   252,
     253,    -1,    -1,   256,   257,    -1,   259,   260,   261,    -1,
      -1,   264,    -1,    -1,    -1,   268,   269,   270,   271,   272,
      -1,   274,   275,   276,    -1,   278,   279,   280,   281,   282,
     283,   284,    -1,   286,   287,   288,   289,   290,   291,   292,
     293,    -1,   295,   296,   297,   298,   299,   300,    -1,    -1,
     303,   304,   305,    -1,   307,   308,   309,   310,    -1,   312,
     313,    -1,   315,   316,   317,   318,   319,   320,   321,    -1,
     323,   324,    -1,   326,   327,   328,    -1,   330,   331,    -1,
     333,    -1,   335,   336,   337,   338,    -1,   340,   341,   342,
     343,    -1,    -1,   346,   347,   348,   349,   350,    -1,    -1,
     353,   354,   355,   356,   357,    -1,   359,   360,    -1,    -1,
      -1,   364,   365,   366,   367,   368,   369,    -1,   371,    -1,
      -1,   374,     6,     7,     8,     9,    -1,    11,    12,    -1,
      14,    15,    16,    -1,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    -1,    29,    30,    31,    32,    33,
      -1,    35,    36,    37,    38,    -1,    -1,    -1,    42,    43,
      44,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    78,    79,    80,    81,    82,    83,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,
      94,    -1,    96,    97,    98,    -1,   100,    -1,   102,    -1,
      -1,   105,   106,    -1,   108,    -1,    -1,   111,   112,   113,
     114,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,    -1,   127,   128,    -1,   130,    -1,   132,   133,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
     144,   145,   146,    -1,    -1,   149,    -1,   151,   152,   153,
      -1,    -1,    -1,   157,   158,   159,   160,    -1,   162,   163,
      -1,   165,    -1,   167,   168,   169,   170,    -1,   172,    -1,
     174,   175,    -1,   177,    -1,   179,    -1,    -1,    -1,   183,
      -1,   185,    -1,    -1,   188,    -1,   190,    -1,   192,   193,
     194,    -1,   196,   197,   198,    -1,    -1,   201,    -1,   203,
     204,   205,    -1,   207,    -1,   209,    -1,    -1,    -1,    -1,
     214,    -1,   216,   217,   218,    -1,    -1,   221,    -1,    -1,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,    -1,   239,   240,   241,   242,   243,
     244,   245,    -1,   247,   248,    -1,    -1,   251,   252,   253,
      -1,    -1,   256,   257,    -1,    -1,   260,   261,    -1,    -1,
     264,    -1,    -1,    -1,   268,   269,   270,   271,   272,    -1,
     274,   275,   276,    -1,   278,   279,   280,   281,   282,   283,
     284,    -1,   286,   287,   288,   289,   290,   291,   292,   293,
      -1,   295,   296,   297,   298,   299,   300,    -1,    -1,   303,
     304,   305,    -1,   307,   308,   309,   310,    -1,   312,   313,
      -1,   315,   316,   317,   318,   319,   320,   321,    -1,   323,
     324,   325,   326,   327,   328,    -1,   330,   331,    -1,   333,
      -1,   335,   336,   337,   338,    -1,   340,   341,   342,   343,
      -1,    -1,   346,   347,   348,   349,   350,    -1,    -1,   353,
     354,   355,   356,   357,    -1,   359,   360,    -1,    -1,    -1,
     364,   365,   366,   367,   368,   369,    -1,   371,    -1,    -1,
     374,     6,     7,     8,     9,    -1,    11,    12,    -1,    14,
      15,    16,    -1,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    -1,    29,    30,    31,    32,    33,    -1,
      35,    36,    37,    38,    -1,    -1,    -1,    42,    43,    44,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    78,    79,    80,    81,    82,    83,    -1,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,
      -1,    96,    97,    98,    -1,   100,    -1,   102,    -1,    -1,
     105,   106,    -1,   108,    -1,    -1,   111,   112,   113,   114,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,    -1,   127,   128,    -1,   130,    -1,   132,   133,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,   144,
     145,   146,    -1,    -1,   149,    -1,   151,   152,   153,    -1,
      -1,    -1,   157,   158,   159,   160,    -1,   162,   163,    -1,
     165,    -1,   167,   168,   169,   170,    -1,   172,    -1,   174,
     175,    -1,   177,    -1,   179,    -1,    -1,    -1,   183,    -1,
     185,    -1,    -1,   188,    -1,   190,    -1,   192,   193,   194,
      -1,   196,   197,   198,    -1,    -1,   201,    -1,   203,   204,
     205,    -1,   207,    -1,   209,    -1,    -1,    -1,    -1,   214,
      -1,   216,   217,   218,    -1,    -1,   221,    -1,    -1,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,    -1,   239,   240,   241,   242,   243,   244,
     245,    -1,   247,   248,    -1,    -1,   251,   252,   253,    -1,
      -1,   256,   257,    -1,    -1,   260,   261,    -1,    -1,   264,
      -1,    -1,    -1,   268,   269,   270,   271,   272,    -1,   274,
     275,   276,    -1,   278,   279,   280,   281,   282,   283,   284,
      -1,   286,   287,   288,   289,   290,   291,   292,   293,    -1,
     295,   296,   297,   298,   299,   300,    -1,    -1,   303,   304,
     305,    -1,   307,   308,   309,   310,    -1,   312,   313,    -1,
     315,   316,   317,   318,   319,   320,   321,    -1,   323,   324,
      -1,   326,   327,   328,    -1,   330,   331,    -1,   333,    -1,
     335,   336,   337,   338,    -1,   340,   341,   342,   343,    -1,
      -1,   346,   347,   348,   349,   350,    -1,   352,   353,   354,
     355,   356,   357,    -1,   359,   360,    -1,    -1,    -1,   364,
     365,   366,   367,   368,   369,    -1,   371,    -1,    -1,   374,
       6,     7,     8,     9,    -1,    11,    12,    -1,    14,    15,
      16,    -1,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    -1,    29,    30,    31,    32,    33,    -1,    35,
      36,    37,    38,    -1,    -1,    -1,    42,    43,    44,    -1,
      46,    -1,    -1,    -1,    50,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    78,    79,    80,    81,    82,    83,    -1,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,    -1,
      96,    97,    98,    -1,   100,    -1,   102,    -1,    -1,   105,
     106,    -1,   108,    -1,    -1,   111,   112,   113,   114,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
      -1,   127,   128,    -1,   130,    -1,   132,   133,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,   144,   145,
     146,    -1,    -1,   149,    -1,   151,   152,   153,    -1,    -1,
      -1,   157,   158,   159,   160,    -1,   162,   163,    -1,   165,
      -1,   167,   168,   169,   170,    -1,   172,    -1,   174,   175,
      -1,   177,    -1,   179,    -1,    -1,    -1,   183,    -1,   185,
      -1,    -1,   188,    -1,   190,    -1,   192,   193,   194,    -1,
     196,   197,   198,    -1,    -1,   201,    -1,   203,   204,   205,
      -1,   207,    -1,   209,    -1,    -1,    -1,    -1,   214,    -1,
     216,   217,   218,    -1,    -1,   221,    -1,    -1,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,    -1,   239,   240,   241,   242,   243,   244,   245,
      -1,   247,   248,    -1,    -1,   251,   252,   253,    -1,    -1,
     256,   257,    -1,    -1,   260,   261,    -1,    -1,   264,    -1,
      -1,    -1,   268,   269,   270,   271,   272,    -1,   274,   275,
     276,    -1,   278,   279,   280,   281,   282,   283,   284,    -1,
     286,   287,   288,   289,   290,   291,   292,   293,    -1,   295,
     296,   297,   298,   299,   300,    -1,    -1,   303,   304,   305,
      -1,   307,   308,   309,   310,    -1,   312,   313,    -1,   315,
     316,   317,   318,   319,   320,   321,    -1,   323,   324,    -1,
     326,   327,   328,    -1,   330,   331,    -1,   333,    -1,   335,
     336,   337,   338,    -1,   340,   341,   342,   343,    -1,    -1,
     346,   347,   348,   349,   350,    -1,    -1,   353,   354,   355,
     356,   357,    -1,   359,   360,    -1,    -1,    -1,   364,   365,
     366,   367,   368,   369,    -1,   371,    -1,    -1,   374,     6,
       7,     8,     9,    -1,    11,    12,    -1,    14,    15,    16,
      -1,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    -1,    29,    30,    31,    32,    33,    -1,    35,    36,
      37,    38,    -1,    -1,    -1,    42,    43,    44,    -1,    46,
      -1,    -1,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    94,    -1,    96,
      97,    98,    -1,   100,    -1,   102,    -1,    -1,   105,   106,
      -1,   108,    -1,    -1,   111,   112,   113,   114,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,    -1,
     127,   128,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,   149,    -1,   151,   152,   153,    -1,    -1,    -1,
     157,   158,   159,   160,    -1,   162,   163,    -1,   165,    -1,
     167,   168,   169,   170,    -1,   172,    -1,   174,   175,    -1,
     177,    -1,   179,    -1,    -1,    -1,   183,    -1,   185,    -1,
      -1,   188,    -1,   190,    -1,   192,   193,   194,    -1,   196,
     197,   198,    -1,    -1,   201,    -1,   203,   204,   205,    -1,
     207,    -1,   209,    -1,    -1,    -1,    -1,   214,    -1,   216,
     217,   218,    -1,    -1,   221,    -1,    -1,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,    -1,   239,    -1,   241,   242,   243,   244,   245,    -1,
     247,   248,    -1,    -1,   251,   252,   253,    -1,    -1,   256,
      -1,    -1,    -1,   260,   261,    -1,    -1,   264,    -1,    -1,
      -1,   268,   269,   270,   271,   272,    -1,   274,   275,   276,
      -1,   278,   279,   280,   281,   282,   283,   284,    -1,   286,
     287,   288,   289,   290,   291,   292,   293,    -1,   295,   296,
     297,   298,   299,   300,    -1,    -1,   303,   304,   305,    -1,
     307,   308,   309,   310,    -1,   312,   313,    -1,   315,   316,
     317,   318,   319,   320,   321,    -1,   323,   324,    -1,   326,
     327,   328,    -1,   330,   331,    -1,   333,    -1,   335,   336,
     337,   338,    -1,   340,   341,   342,   343,    -1,    -1,   346,
     347,   348,   349,   350,    -1,    -1,   353,   354,   355,   356,
     357,    -1,   359,   360,    -1,    -1,    -1,   364,   365,   366,
     367,   368,   369,    -1,   371,    -1,    -1,   374,     6,     7,
       8,     9,    -1,    11,    12,    -1,    14,    15,    16,    -1,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      -1,    29,    30,    31,    32,    33,    -1,    35,    36,    37,
      38,    -1,    -1,    -1,    42,    43,    44,    -1,    46,    -1,
      -1,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    93,    94,    -1,    96,    97,
      98,    -1,   100,    -1,   102,    -1,    -1,   105,   106,    -1,
     108,    -1,    -1,   111,   112,   113,   114,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,    -1,   127,
     128,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,   144,   145,   146,    -1,
      -1,   149,    -1,   151,   152,   153,    -1,    -1,    -1,   157,
     158,   159,   160,    -1,   162,   163,    -1,   165,    -1,   167,
     168,   169,   170,    -1,   172,    -1,   174,   175,    -1,   177,
      -1,   179,    -1,    -1,    -1,   183,    -1,   185,    -1,    -1,
     188,    -1,   190,    -1,   192,   193,   194,    -1,   196,   197,
     198,    -1,    -1,   201,    -1,   203,   204,   205,    -1,   207,
      -1,   209,    -1,    -1,    -1,    -1,   214,    -1,   216,   217,
     218,    -1,    -1,   221,    -1,    -1,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
      -1,   239,    -1,   241,   242,   243,   244,   245,    -1,   247,
     248,    -1,    -1,   251,   252,   253,    -1,    -1,   256,    -1,
      -1,    -1,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
     268,   269,   270,   271,   272,    -1,   274,   275,   276,    -1,
     278,   279,   280,   281,   282,   283,   284,    -1,   286,   287,
     288,   289,   290,   291,   292,   293,    -1,   295,   296,   297,
     298,   299,   300,    -1,    -1,   303,   304,   305,    -1,   307,
     308,   309,   310,    -1,   312,   313,    -1,   315,   316,   317,
     318,   319,   320,   321,    -1,   323,   324,    -1,   326,   327,
     328,    -1,   330,   331,    -1,   333,    -1,   335,   336,   337,
     338,    -1,   340,   341,   342,   343,    -1,    -1,   346,   347,
     348,   349,   350,    -1,    -1,   353,   354,   355,   356,   357,
      -1,   359,   360,    -1,    -1,    -1,   364,   365,   366,   367,
     368,   369,    -1,   371,    -1,    -1,   374,     6,     7,     8,
       9,    -1,    11,    12,    -1,    14,    15,    16,    -1,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    -1,
      29,    30,    31,    32,    33,    -1,    35,    36,    37,    38,
      -1,    -1,    -1,    42,    43,    44,    -1,    46,    -1,    -1,
      -1,    50,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    93,    94,    -1,    96,    97,    98,
      -1,   100,    -1,   102,    -1,    -1,   105,   106,    -1,   108,
      -1,    -1,   111,   112,   113,   114,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,    -1,   127,   128,
      -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
     149,    -1,   151,   152,   153,    -1,    -1,    -1,   157,   158,
     159,   160,    -1,   162,   163,    -1,   165,    -1,   167,   168,
     169,   170,    -1,   172,    -1,   174,   175,    -1,   177,    -1,
     179,    -1,    -1,    -1,   183,    -1,   185,    -1,    -1,   188,
      -1,   190,    -1,   192,   193,   194,    -1,   196,   197,   198,
      -1,    -1,   201,    -1,   203,   204,   205,    -1,   207,    -1,
     209,    -1,    -1,    -1,    -1,   214,    -1,   216,   217,   218,
      -1,    -1,   221,    -1,    -1,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,    -1,
     239,    -1,   241,   242,   243,   244,   245,    -1,   247,   248,
      -1,    -1,   251,   252,   253,    -1,    -1,   256,    -1,    -1,
      -1,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,   268,
     269,   270,   271,   272,    -1,   274,   275,   276,    -1,   278,
     279,   280,   281,   282,   283,   284,    -1,   286,   287,   288,
     289,   290,   291,   292,   293,    -1,   295,   296,   297,   298,
     299,   300,    -1,    -1,   303,   304,   305,    -1,   307,   308,
     309,   310,    -1,   312,   313,    -1,   315,   316,   317,   318,
     319,   320,   321,    -1,   323,   324,    -1,   326,   327,   328,
      -1,   330,   331,    -1,   333,    -1,   335,   336,   337,   338,
      -1,   340,   341,   342,   343,    -1,    -1,   346,   347,   348,
     349,   350,    -1,    -1,   353,   354,   355,   356,   357,    -1,
     359,   360,    -1,    -1,    -1,   364,   365,   366,   367,   368,
     369,    -1,   371,    -1,    -1,   374,     6,     7,     8,     9,
      -1,    11,    12,    -1,    14,    15,    16,    -1,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    -1,    29,
      30,    31,    32,    33,    -1,    35,    36,    37,    38,    -1,
      -1,    -1,    42,    43,    44,    -1,    46,    -1,    -1,    -1,
      50,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    78,    79,
      80,    81,    82,    83,    -1,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    92,    93,    94,    95,    96,    97,    98,    -1,
     100,    -1,   102,    -1,    -1,   105,   106,    -1,   108,    -1,
      -1,   111,   112,   113,   114,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,    -1,   127,   128,    -1,
     130,    -1,   132,   133,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,    -1,   149,
      -1,   151,   152,   153,    -1,    -1,    -1,   157,   158,   159,
     160,    -1,   162,   163,    -1,   165,    -1,   167,   168,   169,
     170,    -1,   172,    -1,   174,   175,    -1,   177,    -1,   179,
      -1,    -1,    -1,   183,    -1,   185,    -1,    -1,   188,    -1,
     190,    -1,   192,   193,   194,    -1,   196,   197,   198,    -1,
      -1,   201,    -1,   203,   204,   205,    -1,   207,    -1,   209,
      -1,    -1,    -1,    -1,   214,    -1,   216,   217,   218,    -1,
      -1,   221,    -1,    -1,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,    -1,   239,
      -1,   241,   242,   243,   244,   245,    -1,   247,   248,    -1,
      -1,   251,   252,   253,    -1,    -1,   256,    -1,    -1,    -1,
     260,   261,    -1,    -1,   264,    -1,    -1,    -1,   268,   269,
     270,   271,   272,    -1,   274,   275,   276,    -1,   278,   279,
     280,   281,   282,   283,   284,    -1,   286,   287,   288,   289,
     290,   291,   292,   293,    -1,   295,   296,   297,   298,   299,
     300,    -1,    -1,   303,   304,   305,    -1,   307,   308,   309,
     310,    -1,   312,   313,    -1,   315,   316,   317,   318,   319,
     320,   321,    -1,   323,   324,    -1,   326,   327,   328,    -1,
     330,   331,    -1,   333,    -1,   335,   336,   337,   338,    -1,
     340,   341,   342,   343,    -1,    -1,   346,   347,   348,   349,
     350,    -1,    -1,   353,   354,   355,   356,   357,    -1,   359,
     360,    -1,    -1,    -1,   364,   365,   366,   367,   368,   369,
      -1,   371,    -1,    -1,   374,     6,     7,     8,     9,    -1,
      11,    12,    -1,    14,    15,    16,    -1,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    -1,    29,    30,
      31,    32,    33,    -1,    35,    36,    37,    38,    -1,    -1,
      -1,    42,    43,    44,    -1,    46,    -1,    -1,    -1,    50,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    78,    79,    80,
      81,    82,    83,    -1,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    94,    -1,    96,    97,    98,    -1,   100,
      -1,   102,    -1,    -1,   105,   106,    -1,   108,    -1,    -1,
     111,   112,   113,   114,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,    -1,   127,   128,    -1,   130,
      -1,   132,   133,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,   144,   145,   146,    -1,    -1,   149,    -1,
     151,   152,   153,    -1,    -1,    -1,   157,   158,   159,   160,
      -1,   162,   163,    -1,   165,    -1,   167,   168,   169,   170,
      -1,   172,    -1,   174,   175,    -1,   177,    -1,   179,    -1,
      -1,    -1,   183,    -1,   185,    -1,   187,   188,    -1,   190,
      -1,   192,   193,   194,    -1,   196,   197,   198,    -1,    -1,
     201,    -1,   203,   204,   205,    -1,   207,    -1,   209,    -1,
      -1,    -1,    -1,   214,    -1,   216,   217,   218,    -1,    -1,
     221,    -1,    -1,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,    -1,   239,    -1,
     241,   242,   243,   244,   245,    -1,   247,   248,    -1,    -1,
     251,   252,   253,    -1,    -1,   256,    -1,    -1,    -1,   260,
     261,    -1,    -1,   264,    -1,    -1,    -1,   268,   269,   270,
     271,   272,    -1,   274,   275,   276,    -1,   278,   279,   280,
     281,   282,   283,   284,    -1,   286,   287,   288,   289,   290,
     291,   292,   293,    -1,   295,   296,   297,   298,   299,   300,
      -1,    -1,   303,   304,   305,    -1,   307,   308,   309,   310,
      -1,   312,   313,    -1,   315,   316,   317,   318,   319,   320,
     321,    -1,   323,   324,    -1,   326,   327,   328,    -1,   330,
     331,    -1,   333,    -1,   335,   336,   337,   338,    -1,   340,
     341,   342,   343,    -1,    -1,   346,   347,   348,   349,   350,
      -1,    -1,   353,   354,   355,   356,   357,    -1,   359,   360,
      -1,    -1,    -1,   364,   365,   366,   367,   368,   369,    -1,
     371,    -1,    -1,   374,     6,     7,     8,     9,    -1,    11,
      12,    -1,    14,    15,    16,    -1,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    -1,    29,    30,    31,
      32,    33,    -1,    35,    36,    37,    38,    -1,    -1,    -1,
      42,    43,    44,    -1,    46,    -1,    -1,    -1,    50,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    78,    79,    80,    81,
      82,    83,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    93,    94,    -1,    96,    97,    98,    -1,   100,    -1,
     102,    -1,    -1,   105,   106,    -1,   108,    -1,    -1,   111,
     112,   113,   114,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,    -1,   127,   128,    -1,   130,    -1,
     132,   133,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,   144,   145,   146,   147,    -1,   149,    -1,   151,
     152,   153,    -1,    -1,    -1,   157,   158,   159,   160,    -1,
     162,   163,    -1,   165,    -1,   167,   168,   169,   170,    -1,
     172,    -1,   174,   175,    -1,   177,    -1,   179,    -1,    -1,
      -1,   183,    -1,   185,    -1,    -1,   188,    -1,   190,    -1,
     192,   193,   194,    -1,   196,   197,   198,    -1,    -1,   201,
      -1,   203,   204,   205,    -1,   207,    -1,   209,    -1,    -1,
      -1,    -1,   214,    -1,   216,   217,   218,    -1,    -1,   221,
      -1,    -1,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,    -1,   239,    -1,   241,
     242,   243,   244,   245,    -1,   247,   248,    -1,    -1,   251,
     252,   253,    -1,    -1,   256,    -1,    -1,    -1,   260,   261,
      -1,    -1,   264,    -1,    -1,    -1,   268,   269,   270,   271,
     272,    -1,   274,   275,   276,    -1,   278,   279,   280,   281,
     282,   283,   284,    -1,   286,   287,   288,   289,   290,   291,
     292,   293,    -1,   295,   296,   297,   298,   299,   300,    -1,
      -1,   303,   304,   305,    -1,   307,   308,   309,   310,    -1,
     312,   313,    -1,   315,   316,   317,   318,   319,   320,   321,
      -1,   323,   324,    -1,   326,   327,   328,    -1,   330,   331,
      -1,   333,    -1,   335,   336,   337,   338,    -1,   340,   341,
     342,   343,    -1,    -1,   346,   347,   348,   349,   350,    -1,
      -1,   353,   354,   355,   356,   357,    -1,   359,   360,    -1,
      -1,    -1,   364,   365,   366,   367,   368,   369,    -1,   371,
      -1,    -1,   374,     6,     7,     8,     9,    -1,    11,    12,
      -1,    14,    15,    16,    -1,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    -1,    29,    30,    31,    32,
      33,    -1,    35,    36,    37,    38,    -1,    -1,    -1,    42,
      43,    44,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    78,    79,    80,    81,    82,
      83,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    -1,    96,    97,    98,    -1,   100,    -1,   102,
      -1,    -1,   105,   106,    -1,   108,    -1,    -1,   111,   112,
     113,   114,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,    -1,   127,   128,    -1,   130,    -1,   132,
     133,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,   144,   145,   146,   147,    -1,   149,    -1,   151,   152,
     153,    -1,    -1,    -1,   157,   158,   159,   160,    -1,   162,
     163,    -1,   165,    -1,   167,   168,   169,   170,    -1,   172,
      -1,   174,   175,    -1,   177,    -1,   179,    -1,    -1,    -1,
     183,    -1,   185,    -1,    -1,   188,    -1,   190,    -1,   192,
     193,   194,    -1,   196,   197,   198,    -1,    -1,   201,    -1,
     203,   204,   205,    -1,   207,    -1,   209,    -1,    -1,    -1,
      -1,   214,    -1,   216,   217,   218,    -1,    -1,   221,    -1,
      -1,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,    -1,   239,    -1,   241,   242,
     243,   244,   245,    -1,   247,   248,    -1,    -1,   251,   252,
     253,    -1,    -1,   256,    -1,    -1,    -1,   260,   261,    -1,
      -1,   264,    -1,    -1,    -1,   268,   269,   270,   271,   272,
      -1,   274,   275,   276,    -1,   278,   279,   280,   281,   282,
     283,   284,    -1,   286,   287,   288,   289,   290,   291,   292,
     293,    -1,   295,   296,   297,   298,   299,   300,    -1,    -1,
     303,   304,   305,    -1,   307,   308,   309,   310,    -1,   312,
     313,    -1,   315,   316,   317,   318,   319,   320,   321,    -1,
     323,   324,    -1,   326,   327,   328,    -1,   330,   331,    -1,
     333,    -1,   335,   336,   337,   338,    -1,   340,   341,   342,
     343,    -1,    -1,   346,   347,   348,   349,   350,    -1,    -1,
     353,   354,   355,   356,   357,    -1,   359,   360,    -1,    -1,
      -1,   364,   365,   366,   367,   368,   369,    -1,   371,    -1,
      -1,   374,     6,     7,     8,     9,    -1,    11,    12,    -1,
      14,    15,    16,    -1,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    -1,    29,    30,    31,    32,    33,
      -1,    35,    36,    37,    38,    -1,    -1,    -1,    42,    43,
      44,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    78,    79,    80,    81,    82,    83,
      -1,    85,    -1,    -1,    -1,    -1,    90,    -1,    92,    93,
      94,    -1,    96,    97,    98,    -1,   100,    -1,   102,    -1,
      -1,   105,   106,    -1,   108,    -1,    -1,   111,   112,   113,
     114,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,    -1,   127,   128,    -1,   130,    -1,   132,   133,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
     144,   145,   146,    -1,    -1,   149,    -1,   151,   152,   153,
      -1,    -1,    -1,   157,   158,   159,   160,    -1,   162,   163,
      -1,   165,    -1,   167,   168,   169,   170,    -1,   172,    -1,
     174,   175,    -1,   177,    -1,   179,    -1,    -1,    -1,   183,
      -1,   185,    -1,    -1,   188,    -1,   190,    -1,   192,   193,
     194,    -1,   196,   197,   198,    -1,    -1,   201,    -1,   203,
     204,   205,    -1,   207,    -1,   209,    -1,    -1,    -1,    -1,
     214,    -1,   216,   217,   218,    -1,    -1,   221,    -1,    -1,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,    -1,   239,    -1,   241,   242,   243,
     244,   245,    -1,   247,   248,    -1,    -1,   251,   252,   253,
      -1,    -1,   256,    -1,    -1,    -1,   260,   261,    -1,    -1,
     264,    -1,    -1,    -1,   268,   269,   270,   271,   272,    -1,
     274,   275,   276,    -1,   278,   279,   280,   281,   282,   283,
     284,    -1,   286,   287,   288,   289,   290,   291,   292,   293,
      -1,   295,   296,   297,   298,   299,   300,    -1,    -1,   303,
     304,   305,    -1,   307,   308,   309,   310,    -1,   312,   313,
      -1,   315,   316,   317,   318,   319,   320,   321,    -1,   323,
     324,    -1,   326,   327,   328,    -1,   330,   331,    -1,   333,
      -1,   335,   336,   337,   338,    -1,   340,   341,   342,   343,
      -1,    -1,   346,   347,   348,   349,   350,    -1,    -1,   353,
     354,   355,   356,   357,    -1,   359,   360,    -1,    -1,    -1,
     364,   365,   366,   367,   368,   369,    -1,   371,    -1,    -1,
     374,     6,     7,     8,     9,    -1,    11,    12,    -1,    14,
      15,    16,    -1,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    -1,    29,    30,    31,    32,    33,    -1,
      35,    36,    37,    38,    -1,    -1,    -1,    42,    43,    44,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    78,    79,    80,    81,    82,    83,    -1,
      85,    -1,    -1,    -1,    -1,    90,    -1,    92,    93,    94,
      -1,    96,    97,    98,    -1,   100,    -1,   102,    -1,    -1,
     105,   106,    -1,   108,    -1,    -1,   111,   112,   113,   114,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,    -1,   127,   128,    -1,   130,    -1,   132,   133,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,   144,
     145,   146,    -1,    -1,   149,    -1,   151,   152,   153,    -1,
      -1,    -1,   157,   158,   159,   160,    -1,   162,   163,    -1,
     165,    -1,   167,   168,   169,   170,    -1,   172,    -1,   174,
     175,    -1,   177,    -1,   179,    -1,    -1,    -1,   183,    -1,
     185,    -1,    -1,   188,    -1,   190,    -1,   192,   193,   194,
      -1,   196,   197,   198,    -1,    -1,   201,    -1,   203,   204,
     205,    -1,   207,    -1,   209,    -1,    -1,    -1,    -1,   214,
      -1,   216,   217,   218,    -1,    -1,   221,    -1,    -1,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,    -1,   239,    -1,   241,   242,   243,   244,
     245,    -1,   247,   248,    -1,    -1,   251,   252,   253,    -1,
      -1,   256,    -1,    -1,    -1,   260,   261,    -1,    -1,   264,
      -1,    -1,    -1,   268,   269,   270,   271,   272,    -1,   274,
     275,   276,    -1,   278,   279,   280,   281,   282,   283,   284,
      -1,   286,   287,   288,   289,   290,   291,   292,   293,    -1,
     295,   296,   297,   298,   299,   300,    -1,    -1,   303,   304,
     305,    -1,   307,   308,   309,   310,    -1,   312,   313,    -1,
     315,   316,   317,   318,   319,   320,   321,    -1,   323,   324,
      -1,   326,   327,   328,    -1,   330,   331,    -1,   333,    -1,
     335,   336,   337,   338,    -1,   340,   341,   342,   343,    -1,
      -1,   346,   347,   348,   349,   350,    -1,    -1,   353,   354,
     355,   356,   357,    -1,   359,   360,    -1,    -1,    -1,   364,
     365,   366,   367,   368,   369,    -1,   371,    -1,    -1,   374,
       6,     7,     8,     9,    -1,    11,    12,    -1,    14,    15,
      16,    -1,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    -1,    29,    30,    31,    32,    33,    -1,    35,
      36,    37,    38,    -1,    -1,    -1,    42,    43,    44,    -1,
      46,    -1,    -1,    -1,    50,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    78,    79,    80,    81,    82,    83,    -1,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,    -1,
      96,    97,    98,    -1,   100,    -1,   102,    -1,    -1,   105,
     106,    -1,   108,    -1,    -1,   111,   112,   113,   114,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
      -1,   127,   128,    -1,   130,    -1,   132,   133,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,   144,   145,
     146,    -1,    -1,   149,    -1,   151,   152,   153,    -1,    -1,
      -1,   157,   158,   159,   160,    -1,   162,   163,    -1,   165,
      -1,   167,   168,   169,   170,    -1,   172,    -1,   174,   175,
      -1,   177,    -1,   179,    -1,    -1,    -1,   183,    -1,   185,
      -1,    -1,   188,    -1,   190,    -1,   192,   193,   194,    -1,
     196,   197,   198,    -1,    -1,   201,    -1,   203,   204,   205,
      -1,   207,    -1,   209,    -1,    -1,    -1,    -1,   214,    -1,
     216,   217,   218,    -1,    -1,   221,    -1,    -1,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,    -1,   239,    -1,   241,   242,   243,   244,   245,
      -1,   247,   248,    -1,    -1,   251,   252,   253,    -1,    -1,
     256,    -1,    -1,    -1,   260,   261,    -1,    -1,   264,    -1,
      -1,    -1,   268,   269,   270,   271,   272,    -1,   274,   275,
     276,    -1,   278,   279,   280,   281,   282,   283,   284,    -1,
     286,   287,   288,   289,   290,   291,   292,   293,    -1,   295,
     296,   297,   298,   299,   300,    -1,    -1,   303,   304,   305,
      -1,   307,   308,   309,   310,    -1,   312,   313,    -1,   315,
     316,   317,   318,   319,   320,   321,    -1,   323,   324,    -1,
     326,   327,   328,    -1,   330,   331,    -1,   333,    -1,   335,
     336,   337,   338,    -1,   340,   341,   342,   343,    -1,    -1,
     346,   347,   348,   349,   350,    -1,    -1,   353,   354,   355,
     356,   357,    -1,   359,   360,    -1,    -1,    -1,   364,   365,
     366,   367,   368,   369,    -1,   371,    -1,    -1,   374,     6,
       7,     8,     9,    -1,    11,    12,    -1,    14,    15,    16,
      -1,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    -1,    29,    30,    31,    32,    33,    -1,    35,    36,
      37,    38,    -1,    -1,    -1,    42,    43,    44,    -1,    46,
      -1,    -1,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    94,    -1,    96,
      97,    98,    -1,   100,    -1,   102,    -1,    -1,   105,   106,
      -1,   108,    -1,    -1,   111,   112,   113,   114,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,    -1,
     127,   128,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,   149,    -1,   151,   152,   153,    -1,    -1,    -1,
     157,   158,   159,   160,    -1,   162,   163,    -1,   165,    -1,
     167,   168,   169,   170,    -1,   172,    -1,   174,   175,    -1,
     177,    -1,   179,    -1,    -1,    -1,   183,    -1,   185,    -1,
      -1,   188,    -1,   190,    -1,   192,   193,   194,    -1,   196,
     197,   198,    -1,    -1,   201,    -1,   203,   204,   205,    -1,
     207,    -1,   209,    -1,    -1,    -1,    -1,   214,    -1,   216,
     217,   218,    -1,    -1,   221,    -1,    -1,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,    -1,   239,    -1,   241,   242,   243,   244,   245,    -1,
     247,   248,    -1,    -1,   251,   252,   253,    -1,    -1,   256,
      -1,    -1,    -1,   260,   261,    -1,    -1,   264,    -1,    -1,
      -1,   268,   269,   270,   271,   272,    -1,   274,   275,   276,
      -1,   278,   279,   280,   281,   282,   283,   284,    -1,   286,
     287,   288,   289,   290,   291,   292,   293,    -1,   295,   296,
     297,   298,   299,   300,    -1,    -1,   303,   304,   305,    -1,
     307,   308,   309,   310,    -1,   312,   313,    -1,   315,   316,
     317,   318,   319,   320,   321,    -1,   323,   324,    -1,   326,
     327,   328,    -1,   330,   331,    -1,   333,    -1,   335,   336,
     337,   338,    -1,   340,   341,   342,   343,    -1,    -1,   346,
     347,   348,   349,   350,    -1,    -1,   353,   354,   355,   356,
     357,    -1,   359,   360,    -1,    -1,    -1,   364,   365,   366,
     367,   368,   369,    -1,   371,    -1,    -1,   374,     6,     7,
       8,     9,    -1,    11,    12,    -1,    14,    15,    16,    -1,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      -1,    29,    30,    31,    32,    33,    -1,    35,    36,    37,
      38,    -1,    -1,    -1,    42,    43,    44,    -1,    46,    -1,
      -1,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      78,    79,    80,    81,    82,    83,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    93,    94,    -1,    96,    97,
      98,    -1,   100,    -1,   102,    -1,    -1,   105,   106,    -1,
     108,    -1,    -1,   111,   112,   113,   114,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,    -1,   127,
     128,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,   144,   145,   146,    -1,
      -1,   149,    -1,   151,   152,   153,    -1,    -1,    -1,   157,
     158,   159,   160,    -1,   162,   163,    -1,   165,    -1,   167,
     168,   169,   170,    -1,   172,    -1,   174,   175,    -1,   177,
      -1,   179,    -1,    -1,    -1,   183,    -1,   185,    -1,    -1,
     188,    -1,   190,    -1,   192,   193,   194,    -1,   196,   197,
     198,    -1,    -1,   201,    -1,   203,   204,   205,    -1,   207,
      -1,   209,    -1,    -1,    -1,    -1,   214,    -1,   216,   217,
     218,    -1,    -1,   221,    -1,    -1,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
      -1,   239,    -1,   241,   242,   243,   244,   245,    -1,   247,
     248,    -1,    -1,   251,   252,   253,    -1,    -1,   256,    -1,
      -1,    -1,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
     268,   269,   270,   271,   272,    -1,   274,   275,   276,    -1,
     278,   279,   280,   281,   282,   283,   284,    -1,   286,   287,
     288,   289,   290,   291,   292,   293,    -1,   295,   296,   297,
     298,   299,   300,    -1,    -1,   303,   304,   305,    -1,   307,
     308,   309,   310,    -1,   312,   313,    -1,   315,   316,   317,
     318,   319,   320,   321,    -1,   323,   324,    -1,   326,   327,
     328,    -1,   330,   331,    -1,   333,    -1,   335,   336,   337,
     338,    -1,   340,   341,   342,   343,    -1,    -1,   346,   347,
     348,   349,   350,    -1,    -1,   353,   354,   355,   356,   357,
      -1,   359,   360,    -1,    -1,    -1,   364,   365,   366,   367,
     368,   369,    -1,   371,    -1,    -1,   374,     6,     7,     8,
       9,    -1,    11,    12,    -1,    14,    15,    16,    -1,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    -1,
      29,    30,    31,    32,    33,    -1,    35,    36,    37,    38,
      -1,    -1,    -1,    42,    43,    44,    -1,    46,    -1,    -1,
      -1,    50,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    78,
      79,    80,    81,    82,    83,    -1,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    93,    94,    -1,    96,    97,    98,
      -1,   100,    -1,   102,    -1,    -1,   105,   106,    -1,   108,
      -1,    -1,   111,   112,   113,   114,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,    -1,   127,   128,
      -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
     149,    -1,   151,   152,   153,    -1,    -1,    -1,   157,   158,
     159,   160,    -1,   162,   163,    -1,   165,    -1,   167,   168,
     169,   170,    -1,   172,    -1,   174,   175,    -1,   177,    -1,
     179,    -1,    -1,    -1,   183,    -1,   185,    -1,    -1,   188,
      -1,   190,    -1,   192,   193,   194,    -1,   196,   197,   198,
      -1,    -1,   201,    -1,   203,   204,   205,    -1,   207,    -1,
     209,    -1,    -1,    -1,    -1,   214,    -1,   216,   217,   218,
      -1,    -1,   221,    -1,    -1,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,    -1,
     239,    -1,   241,   242,   243,   244,   245,    -1,   247,   248,
      -1,    -1,   251,   252,   253,    -1,    -1,   256,    -1,    -1,
      -1,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,   268,
     269,   270,   271,   272,    -1,   274,   275,   276,    -1,   278,
     279,   280,   281,   282,   283,   284,    -1,   286,   287,   288,
     289,   290,   291,   292,   293,    -1,   295,   296,   297,   298,
     299,   300,    -1,    -1,   303,   304,   305,    -1,   307,   308,
     309,   310,    -1,   312,   313,    -1,   315,   316,   317,   318,
     319,   320,   321,    -1,   323,   324,    -1,   326,   327,   328,
      -1,   330,   331,    -1,   333,    -1,   335,   336,   337,   338,
      -1,   340,   341,   342,   343,    -1,    -1,   346,   347,   348,
     349,   350,    -1,    -1,   353,   354,   355,   356,   357,    -1,
     359,   360,    -1,    -1,    -1,   364,   365,   366,   367,   368,
     369,    -1,   371,    -1,    -1,   374,     6,     7,     8,     9,
      -1,    11,    12,    -1,    14,    15,    16,    -1,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    -1,    29,
      30,    31,    32,    33,    -1,    35,    36,    37,    38,    -1,
      -1,    -1,    42,    43,    44,    -1,    46,    -1,    -1,    -1,
      50,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    78,    79,
      80,    81,    82,    83,    -1,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    92,    93,    94,    -1,    96,    97,    98,    -1,
     100,    -1,   102,    -1,    -1,   105,   106,    -1,   108,    -1,
      -1,   111,   112,   113,   114,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,    -1,   127,   128,    -1,
     130,    -1,   132,   133,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,    -1,   149,
      -1,   151,   152,   153,    -1,    -1,    -1,   157,   158,   159,
     160,    -1,   162,   163,    -1,   165,    -1,   167,   168,   169,
     170,    -1,   172,    -1,   174,   175,    -1,   177,    -1,   179,
      -1,    -1,    -1,   183,    -1,   185,    -1,    -1,   188,    -1,
     190,    -1,   192,   193,   194,    -1,   196,   197,   198,    -1,
      -1,   201,    -1,   203,   204,   205,    -1,   207,    -1,   209,
      -1,    -1,    -1,    -1,   214,    -1,   216,   217,   218,    -1,
      -1,   221,    -1,    -1,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,    -1,   239,
      -1,   241,   242,   243,   244,   245,    -1,   247,   248,    -1,
      -1,   251,   252,   253,    -1,    -1,   256,    -1,    -1,    -1,
     260,   261,    -1,    -1,   264,    -1,    -1,    -1,   268,   269,
     270,   271,   272,    -1,   274,   275,   276,    -1,   278,   279,
     280,   281,   282,   283,   284,    -1,   286,   287,   288,   289,
     290,   291,   292,   293,    -1,   295,   296,   297,   298,   299,
     300,    -1,    -1,   303,   304,   305,    -1,   307,   308,   309,
     310,    -1,   312,   313,    -1,   315,   316,   317,   318,   319,
     320,   321,    -1,   323,   324,    -1,   326,   327,   328,    -1,
     330,   331,    -1,   333,    -1,   335,   336,   337,   338,    -1,
     340,   341,   342,   343,    -1,    -1,   346,   347,   348,   349,
     350,    -1,    -1,   353,   354,   355,   356,   357,    -1,   359,
     360,    -1,    -1,    -1,   364,   365,   366,   367,   368,   369,
      -1,   371,    -1,    -1,   374
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned short yystos[] =
{
       0,   406,   407,     0,     5,    19,    28,    39,    40,    45,
      47,    48,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    75,   112,   158,   175,   206,   250,
     344,   356,   371,   372,   374,   375,   376,   380,   382,   383,
     387,   388,   389,   390,   391,   394,   395,   396,   397,   399,
     400,   401,   402,   408,   748,   749,   752,   753,   777,   779,
     857,   861,   869,   870,   778,     3,     9,    18,    21,    32,
      50,    51,    77,    85,    86,    87,    94,    98,   116,   118,
     119,   123,   124,   130,   131,   143,   146,   151,   159,   164,
     168,   170,   174,   184,   186,   204,   224,   225,   228,   235,
     248,   276,   286,   290,   293,   295,   302,   307,   310,   316,
     340,   342,   347,   349,   353,   396,   409,   410,   411,   413,
     414,   415,   419,   422,   423,   424,   428,   439,   440,   441,
     444,   445,   448,   450,   459,   482,   487,   490,   494,   496,
     509,   510,   511,   512,   518,   523,   524,   528,   529,   533,
     536,   537,   548,   556,   568,   569,   571,   574,   576,   577,
     580,   583,   591,   592,   593,   594,   595,   598,   599,   600,
     604,   605,   606,   608,   609,   610,   611,   612,   617,   618,
     619,   623,   624,   627,   628,   630,   631,   632,   633,   634,
     762,   775,   776,   806,   807,   810,   813,   814,   820,   821,
     822,   829,   830,   831,   832,   834,   835,   839,   841,    55,
      61,    70,    72,    75,   781,   782,   784,   786,   789,    17,
       6,     7,     8,     9,    11,    12,    14,    15,    16,    18,
      20,    22,    23,    24,    25,    26,    27,    29,    30,    31,
      32,    33,    35,    36,    37,    38,    42,    43,    44,    46,
      50,    51,    77,    78,    79,    80,    81,    82,    83,    85,
      92,    93,    94,    96,    97,    98,   100,   102,   105,   106,
     108,   111,   112,   113,   114,   116,   117,   118,   119,   120,
     123,   124,   125,   127,   128,   130,   132,   133,   139,   140,
     141,   142,   143,   144,   145,   146,   149,   151,   152,   153,
     157,   158,   159,   160,   162,   163,   165,   167,   168,   169,
     170,   172,   174,   175,   177,   179,   183,   185,   188,   190,
     192,   193,   194,   196,   197,   198,   201,   203,   204,   205,
     207,   209,   214,   216,   217,   218,   221,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   239,   241,   242,   243,   244,   245,   247,   248,   251,
     252,   253,   256,   260,   261,   264,   268,   269,   270,   271,
     272,   274,   275,   276,   278,   279,   280,   281,   282,   283,
     284,   286,   287,   288,   289,   290,   291,   292,   293,   295,
     296,   297,   298,   299,   300,   303,   304,   305,   307,   308,
     309,   310,   312,   313,   315,   316,   317,   318,   319,   320,
     321,   323,   324,   326,   327,   328,   330,   331,   332,   333,
     335,   336,   337,   338,   340,   341,   342,   343,   346,   347,
     348,   349,   350,   353,   354,   355,   356,   357,   359,   360,
     364,   365,   366,   367,   368,   369,   371,   374,   752,   755,
     760,   771,   772,   843,   847,   852,   853,   861,   863,    13,
      84,   147,   741,   755,   763,   764,   808,   809,   847,   863,
     740,   847,   740,   840,    42,    44,   246,   335,   366,   597,
     141,   187,   325,   337,   351,   763,   146,   597,   449,   847,
     744,   847,   258,   597,   101,   455,    83,    92,   110,   113,
     126,   141,   147,   157,   185,   187,   226,   260,   262,   298,
     326,   328,   337,   341,   342,   345,   351,   460,   491,   521,
     549,   557,    17,   276,   863,   317,   740,   181,    83,    92,
     110,   128,   141,   157,   183,   187,   197,   260,   281,   297,
     298,   303,   325,   337,   342,   351,   360,   495,   525,   597,
     192,   740,   358,   612,   613,    78,    84,    96,   179,   181,
     195,   241,   278,   287,   388,   530,   531,   532,   740,   753,
     754,    17,    84,   131,   151,   168,   204,   285,   297,   302,
     326,   328,   337,   349,   350,   538,   539,   540,   210,   240,
     257,   727,   736,   738,   761,   847,   745,   755,   325,   637,
     530,   531,   532,   740,   738,   740,   141,   197,   325,   578,
      84,   305,   330,   335,   847,   186,   545,   597,    84,   155,
     639,     4,    10,   127,   226,   236,   305,   330,   335,   429,
     847,    84,   305,   330,   335,   847,   335,   637,   836,   389,
     738,   259,   662,   738,   182,   614,   631,   632,    98,   410,
     399,    33,   613,   166,   208,   263,   344,   640,   641,   785,
     780,   857,   784,   786,    56,    74,   790,   375,   740,   773,
     864,   147,   763,    24,   352,   390,   373,   765,   766,   404,
     375,   352,   815,     5,    28,    39,    40,    47,    48,    84,
      86,    87,    88,    89,    90,    91,    95,    99,   101,   103,
     109,   110,   112,   115,   121,   122,   126,   131,   134,   135,
     136,   137,   138,   147,   148,   154,   155,   156,   161,   164,
     166,   173,   176,   178,   180,   181,   182,   186,   187,   189,
     191,   195,   199,   200,   206,   208,   210,   212,   213,   215,
     219,   220,   222,   223,   238,   240,   246,   249,   250,   254,
     255,   257,   258,   259,   262,   263,   265,   266,   277,   285,
     294,   302,   306,   311,   314,   325,   329,   332,   334,   339,
     344,   345,   351,   352,   358,   362,   363,   843,   844,   850,
     851,   852,   853,   854,   855,   863,     7,    11,    22,    23,
      43,    46,   156,   842,   842,    20,   741,   760,   662,   738,
     740,   760,    38,   258,    83,   122,   141,   157,   183,   197,
     260,   297,   298,   303,   325,   337,   342,   360,   534,   738,
     727,   736,   746,   843,   849,   852,   854,   863,   740,   396,
     307,   337,   741,   527,   727,   847,   326,   328,   760,   326,
     328,   117,   373,   383,   384,   385,   387,   388,   389,   390,
     391,   392,   698,   699,   289,   760,   847,   740,   527,   760,
     303,   325,   495,   128,   197,   183,   297,   360,   864,   863,
     863,   101,   203,   299,   629,   662,   746,   740,   396,   741,
     746,   760,   117,   698,   740,   740,   418,   760,   218,   526,
     527,   374,   773,   812,   811,   613,   146,   618,   753,   241,
     278,   531,   532,   532,   740,   210,   817,   864,   279,   258,
     402,   738,   398,   728,   737,   738,   531,   532,   532,   740,
     181,   740,   738,    95,   369,   214,   261,   538,   258,   100,
     102,   104,   109,   110,   112,   113,   120,   129,   135,   136,
     137,   138,   144,   145,   158,   169,   172,   173,   175,   206,
     207,   209,   237,   239,   246,   250,   251,   252,   260,   267,
     274,   283,   296,   306,   313,   323,   330,   331,   336,   338,
     339,   345,   351,   356,   373,   375,   378,   379,   381,   387,
     388,   389,   391,   392,   396,   631,   671,   672,   673,   678,
     679,   680,   682,   683,   684,   685,   689,   690,   693,   694,
     700,   702,   704,   721,   726,   727,   730,   731,   736,   746,
     747,   748,   749,   750,   751,   752,   759,   843,   844,   848,
     852,   860,   861,   863,   332,   383,   332,   383,    84,   442,
     739,   740,   305,   429,   147,   437,   755,    95,   114,   429,
     369,   214,   332,   403,    95,   369,   214,   214,   596,   738,
     850,   396,   738,   307,   389,   180,   615,   397,   397,   399,
     738,    84,   638,   638,   105,   638,   176,   223,   255,   645,
     651,    19,    40,    47,    48,   344,   791,   792,   794,   795,
     796,   797,   798,   799,   801,   851,   164,   787,    90,   769,
     105,   772,   772,   377,   767,   847,   404,   768,   765,   389,
     803,   805,   819,   860,   861,   212,   740,   332,   740,     6,
     740,   842,   307,   440,    81,   159,    81,    85,   159,   288,
     131,   268,   258,   307,   364,   416,   440,   399,   738,   746,
     746,   698,   740,   740,   740,   364,   456,   396,   514,   115,
     100,   102,   112,   113,   144,   145,   175,   207,   239,   252,
     283,   313,   330,   331,   356,   671,   843,   852,   863,    90,
     607,   740,   364,   607,   364,   420,   527,   514,   425,   847,
     426,    82,    97,   497,    90,   514,   364,   412,   738,   738,
     218,   527,   744,   746,   740,   738,   299,   139,   363,   664,
     396,   671,   396,   558,   527,   396,   258,   258,   402,   755,
     108,   291,   402,   447,   815,   618,   740,   532,   740,   740,
     817,    41,   375,   816,   856,   859,    49,   375,   823,   827,
     141,   183,   218,   298,   325,   541,   737,   540,   147,   355,
     396,   620,   630,   389,   743,   847,   195,   402,   625,   532,
     740,   740,   812,   177,   579,   579,   221,   176,   258,   396,
     357,   686,   702,   725,   396,   686,   686,   396,   396,   396,
     729,   396,   729,   396,   729,   396,   676,   676,   275,   631,
     396,   396,   674,   112,   113,   686,   702,   396,   396,   675,
     396,   396,   396,   396,   729,   396,   364,   365,   396,   691,
     396,   691,   396,   396,   631,   729,    25,   375,   862,   394,
     705,   728,   702,   702,   702,   702,   631,   695,   696,   702,
     755,   113,   396,   687,   396,   755,   195,   212,   246,   260,
     266,   698,   701,   702,    76,    88,    90,    94,    99,   191,
     195,   212,   213,   222,   246,   249,   260,   262,   311,   373,
     383,   384,   385,   387,   388,   389,   390,   391,   392,   700,
     701,   705,   210,   402,   635,   817,   705,   396,   254,   258,
     833,   833,   809,   809,   149,   192,   443,   402,   114,   147,
     438,   755,   847,    90,   147,   209,   226,   388,   436,   690,
     748,   749,   752,   757,   758,   861,   863,   221,   147,   173,
     254,   258,   339,   430,   431,   434,   435,   758,   847,   430,
     221,   221,   212,   738,   389,   732,   733,   847,   613,   396,
     616,   631,   633,   634,   633,   642,   643,   702,   633,   282,
     349,    84,   381,   647,   753,   381,   648,   753,   651,   652,
     645,   646,   845,   846,   850,   845,   846,    28,    39,   112,
     206,   845,   805,   400,   400,     5,    28,    39,   112,   206,
     800,   146,   791,   763,   351,   770,   772,   398,   753,   768,
     389,   402,   851,   791,   396,   740,   396,   429,   351,   351,
     115,   122,   126,   178,   277,   345,   470,   471,   582,   582,
     126,   582,   582,   333,   332,   662,   429,   416,   132,   133,
     195,   243,   244,   270,   324,   354,   417,   396,   558,   396,
     212,   258,   258,   212,   256,   181,   332,   451,   515,   516,
     850,   396,    90,   527,    82,   162,   227,   268,   327,   601,
     602,   308,   667,   670,   672,   673,   677,   679,   680,   681,
     683,   684,   689,   690,   848,   420,   324,   351,   421,   521,
      95,   131,   427,   459,   536,   598,   151,   204,   349,   498,
     499,   396,   513,   416,   488,   253,   396,   484,   438,   176,
     258,   558,    90,   396,   472,   176,   702,   389,   570,   667,
      90,   195,   201,   264,   397,   559,   560,   561,   564,   667,
     848,   447,   352,   245,   572,   667,   738,   738,   760,   447,
     527,   817,   818,   629,   740,   817,   817,   862,    17,   402,
     375,   748,   383,   402,   739,   546,   547,   746,   739,   739,
     737,   332,   355,   396,   621,   622,   847,   728,    79,   167,
     296,   309,   626,   738,   740,   541,   702,   706,   396,   362,
     722,   723,   702,   706,   702,   717,   718,   397,   753,   753,
     753,   142,   190,   231,   234,   300,   368,   707,   709,   755,
     863,   753,   686,   686,   702,   753,   573,   698,   847,   702,
     710,   388,   391,   392,   396,   700,   703,   704,   712,   397,
     695,   702,   702,   706,   713,   330,   330,   753,   753,   702,
     103,   181,   219,   334,   702,   706,   716,   375,   740,   702,
     705,   397,   402,   397,   307,   753,   753,   142,   190,   231,
     234,   300,   368,   692,   631,   155,   246,   250,   195,   396,
     296,   396,   694,    84,    89,   314,   396,   631,   694,   697,
     667,   702,   850,   330,   703,   702,   396,   631,   719,   155,
     173,   246,   250,   253,   339,   346,   702,    99,   191,   195,
     222,   311,   396,   702,   332,   702,   702,   702,   702,   702,
     702,   702,   702,   702,   702,   697,   185,   226,   325,   326,
     328,   636,   738,   731,   181,   654,    84,   155,   389,   397,
     706,   740,   335,   748,   749,   396,   755,   282,   304,   432,
     402,   432,   791,   397,   402,   654,   705,   611,   738,   739,
     402,    91,   154,   352,   644,   259,   253,   653,   255,   402,
     223,   400,   793,    28,   206,   206,   851,   783,   786,   783,
      28,   206,   206,    38,   788,   771,   767,   390,   819,   394,
     668,   394,   396,   837,   396,   400,   858,   865,   866,   867,
     868,   870,   858,   418,   418,   396,   740,   216,   216,   396,
     464,   847,   847,   740,   847,   581,   740,   325,   760,   288,
     187,   752,   753,   348,   570,   212,   572,   250,   535,   755,
     527,   527,   535,   319,   320,   452,   755,   397,   402,   383,
     702,   671,   184,   498,   383,   603,   603,   603,   603,   602,
     670,   465,   668,   396,   692,   728,   753,   418,   176,   760,
     460,   557,   258,   262,   665,   666,   847,   106,   140,   196,
     230,   232,   316,   489,   738,   222,   461,   462,   463,   464,
     469,   470,   485,   486,   847,    90,   188,   755,   738,   292,
     584,   473,   474,   847,    90,   630,   863,   397,   671,   397,
     402,   564,   728,   742,   847,   402,   397,   402,   447,   447,
     139,   817,   864,   859,   375,   825,   828,   828,    12,   824,
     823,   402,   558,   187,   542,   543,   847,   147,   731,   734,
     735,   397,   402,   705,   167,   309,   167,   309,   296,   349,
     233,   181,   352,   397,   402,   753,   702,   161,   723,   724,
      90,   397,   352,   397,   402,   397,   397,   397,   402,   397,
     181,   397,   402,   397,   402,   397,   398,   273,   711,   397,
     703,   703,   703,   702,   703,    76,   195,   212,   260,   373,
     383,   384,   385,   387,   388,   389,   390,   391,   392,   700,
     397,   397,   397,   176,   181,   714,   715,   397,   369,   369,
     397,   397,    90,   716,   706,   716,   716,   181,   397,   395,
     404,   702,   728,   847,   397,   397,   332,   332,   332,   332,
     181,   250,   631,   573,   702,   631,   369,    88,   165,   702,
     720,   181,   173,   250,   253,   339,   346,   396,   165,   703,
     702,   719,   702,   332,   573,   702,   631,   326,   328,   326,
     328,   738,   637,   637,   396,   631,   655,   656,   657,   662,
     663,   727,   746,   664,   706,   706,   397,   397,   214,   753,
     692,   125,   282,   433,   434,   837,   733,   664,   383,   397,
     643,   698,   739,   648,   648,   647,   867,   206,   837,   401,
     786,   401,   206,   399,   802,   803,   741,   395,   396,   669,
     753,   383,   804,   395,   669,   397,   669,    34,   838,   397,
     866,   867,   397,   865,   402,   397,   702,   471,   396,   396,
     473,   667,   159,   307,   446,   447,   447,   332,   332,   418,
     752,   397,   535,   397,   212,   212,   352,   457,   458,   516,
     260,   388,   517,   563,   564,   701,   758,   397,   397,   514,
     258,   147,   748,   752,   756,   861,   147,   755,   147,   740,
     147,   740,   115,   121,   126,   147,   148,   199,   246,   250,
     277,   285,   345,   466,   467,   468,   688,   753,   342,   426,
     325,   738,   499,   397,   402,   667,   757,   757,   757,   757,
     757,   396,   527,   397,   402,   397,   402,   483,   492,   727,
     740,   847,   332,   352,   550,   563,   258,   397,   402,   630,
     447,   397,   560,   391,   447,   667,   447,   245,   667,   176,
     383,   402,   547,   847,   364,   402,   544,   397,   402,   355,
     630,   622,   167,   167,   542,   702,   702,   397,   329,   702,
     164,   667,   527,   702,   753,   702,   702,   753,   573,   702,
     714,   667,   703,   155,   246,   253,   703,   703,   703,   703,
     703,   703,   703,   703,   703,   703,   702,   702,   715,   714,
     691,   691,   667,   397,   397,   397,   706,   705,   702,   705,
     687,   755,   190,   231,   300,   231,   300,   300,   234,   694,
     397,   704,   703,   702,   397,   402,   702,   396,   667,   708,
     702,    88,   165,   165,   702,   397,   165,   637,   637,   637,
     637,   738,   738,   631,   656,   657,    90,   658,   847,   402,
     134,   182,   200,   215,   220,   238,   294,   370,   659,   658,
      90,   658,   847,   396,   187,   649,   397,   397,   221,   397,
     259,   367,   838,   702,   401,   399,   399,   402,   373,   774,
     668,   669,   387,   388,   389,   390,   391,   395,   868,   837,
     395,   837,   397,   397,   401,   868,   397,   473,   473,   397,
     465,   147,   246,   147,   246,   318,   321,   740,   740,   212,
     212,   535,   535,   412,   153,   148,   199,   246,   506,   507,
     508,   364,   365,   688,   738,   396,   847,   740,   703,   149,
     192,   148,   250,   216,   738,   397,   667,   738,   176,   500,
     262,   666,   461,   198,   480,   463,   486,   847,   630,   217,
     493,   755,   742,   396,    90,   107,   171,   193,   218,   292,
     301,   315,   322,   361,   565,   566,   151,   204,   302,   349,
     589,   474,   447,   342,     8,    14,    15,    16,    25,    26,
      27,    29,    30,    31,    35,    36,    37,   275,   342,   826,
     825,   186,   543,   735,   396,   702,   397,   397,   397,   397,
     715,   181,   253,   396,   397,   395,   692,   702,   708,   397,
     402,   703,   702,   702,   165,   702,   738,   738,   738,   738,
     397,   847,   396,   656,   215,   265,   660,   656,   660,   215,
     659,   660,   656,   215,   396,   847,   396,   397,   706,   105,
     189,   650,   432,   755,   803,   847,   397,   669,   669,   669,
     669,   669,   668,   837,   837,   397,   397,   688,   250,   702,
     250,   753,   847,   535,   535,   453,   755,   149,   192,   148,
     508,   507,   183,   183,   181,   505,   702,   246,   467,   472,
     692,   352,   396,   160,   501,   168,   499,   397,   396,   364,
     365,   481,   755,   181,   551,   552,   553,   554,   743,   746,
     847,   562,   755,   258,   301,   438,   250,   150,   211,   364,
     566,   567,   332,   261,   734,   703,   396,   703,   705,   397,
     667,   702,   658,   396,   739,   656,   258,   352,   661,   656,
     215,   656,   665,   396,   665,   847,   397,   706,   702,   692,
     285,   101,   152,   250,   256,   454,   547,    90,   575,   738,
     506,   397,   229,   475,   742,   296,   317,   502,   280,   481,
     737,   256,   256,   527,   397,   402,   352,   527,   555,   396,
     402,   250,   150,   211,   258,   514,   738,   397,   703,   397,
     739,   397,   702,   396,   656,   661,   397,   665,   397,   738,
     607,   607,   575,    93,   176,   182,   269,   258,   476,   477,
     478,    90,   740,   397,   664,   554,   527,   739,   755,   202,
     250,   664,   397,   397,   739,   397,   472,   755,   755,   160,
     151,   349,   258,   478,   258,   477,   183,   260,   321,   519,
     520,   396,   397,   202,   156,   397,   475,   296,   108,   242,
     291,   307,   479,   479,   753,   753,   667,   402,   503,   504,
     759,   847,   555,   205,   590,   476,   168,    80,   147,   250,
     746,   573,   520,   397,   402,   247,   396,   585,   587,   592,
     619,   623,   627,   630,   506,   280,   558,   284,   396,   522,
     504,   586,   587,   588,   631,   632,   746,   572,   397,   399,
     396,   397,   588,   503,   522,   397
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrlab1


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)         \
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (cinluded).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short *bottom, short *top)
#else
static void
yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylineno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylineno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yytype, yyvaluep)
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 5:
#line 415 "preproc.y"
    { connection = NULL; }
    break;

  case 8:
#line 418 "preproc.y"
    { fprintf(yyout, "%s", yyvsp[0].str); free(yyvsp[0].str); }
    break;

  case 9:
#line 419 "preproc.y"
    { fprintf(yyout, "%s", yyvsp[0].str); free(yyvsp[0].str); }
    break;

  case 10:
#line 420 "preproc.y"
    { braces_open++; fputs("{", yyout); }
    break;

  case 11:
#line 421 "preproc.y"
    { remove_variables(braces_open--); fputs("}", yyout); }
    break;

  case 12:
#line 425 "preproc.y"
    {
			connection = yyvsp[0].str;
			/*
			 *	if we have a variable as connection
			 *	target, remove it from the variable
			 *	list or else it will be used twice
			 */
			if (argsinsert != NULL)
				argsinsert = NULL;
		}
    break;

  case 13:
#line 436 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 14:
#line 437 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 15:
#line 438 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 16:
#line 439 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 17:
#line 440 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 18:
#line 441 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 19:
#line 442 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 20:
#line 443 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 21:
#line 444 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 22:
#line 445 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 23:
#line 446 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 24:
#line 447 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 25:
#line 448 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 26:
#line 449 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 27:
#line 450 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 28:
#line 451 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 29:
#line 452 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 30:
#line 453 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 31:
#line 454 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 32:
#line 455 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 33:
#line 456 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 34:
#line 457 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 35:
#line 459 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 36:
#line 460 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 37:
#line 461 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 38:
#line 462 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 39:
#line 463 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 40:
#line 464 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 41:
#line 465 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 42:
#line 466 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 43:
#line 467 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 44:
#line 468 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 45:
#line 469 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 46:
#line 470 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 47:
#line 471 "preproc.y"
    { output_statement(yyvsp[0].str, 1, connection); }
    break;

  case 48:
#line 472 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 49:
#line 473 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 50:
#line 474 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 51:
#line 475 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 52:
#line 476 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 53:
#line 477 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 54:
#line 479 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 55:
#line 480 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 56:
#line 481 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 57:
#line 482 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 58:
#line 483 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 59:
#line 484 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 60:
#line 486 "preproc.y"
    {
			if (strncmp(yyvsp[0].str, "/* " , sizeof("/* ")-1) == 0)
				output_simple_statement(yyvsp[0].str);
			else
				output_statement(yyvsp[0].str, 1, connection);
		}
    break;

  case 61:
#line 492 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 62:
#line 494 "preproc.y"
    {
			fprintf(yyout, "{ ECPGtrans(__LINE__, %s, \"%s\");", connection ? connection : "NULL", yyvsp[0].str);
			whenever_action(2);
			free(yyvsp[0].str);
		}
    break;

  case 63:
#line 499 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 64:
#line 500 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 65:
#line 501 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 66:
#line 502 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 67:
#line 503 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 68:
#line 504 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 69:
#line 505 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 70:
#line 506 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 71:
#line 507 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 72:
#line 508 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 73:
#line 509 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 74:
#line 510 "preproc.y"
    { output_statement(yyvsp[0].str, 0, connection); }
    break;

  case 75:
#line 512 "preproc.y"
    {
			fprintf(yyout,"ECPGallocate_desc(__LINE__, %s);",yyvsp[0].str);
			whenever_action(0);
			free(yyvsp[0].str);
		}
    break;

  case 76:
#line 518 "preproc.y"
    {
			if (connection)
				mmerror(PARSE_ERROR, ET_ERROR, "no at option for connect statement.\n");

			fprintf(yyout, "{ ECPGconnect(__LINE__, %s, %d); ", yyvsp[0].str, autocommit);
			reset_variables();
			whenever_action(2);
			free(yyvsp[0].str);
		}
    break;

  case 77:
#line 528 "preproc.y"
    {
			output_simple_statement(yyvsp[0].str);
		}
    break;

  case 78:
#line 532 "preproc.y"
    {
			if (connection)
				mmerror(PARSE_ERROR, ET_ERROR, "no at option for connect statement.\n");

			fputc('{', yyout);
			fputs(yyvsp[0].str, yyout);
			whenever_action(2);
			free(yyvsp[0].str);
		}
    break;

  case 79:
#line 542 "preproc.y"
    {
			fprintf(yyout,"ECPGdeallocate_desc(__LINE__, %s);",yyvsp[0].str);
			whenever_action(0);
			free(yyvsp[0].str);
		}
    break;

  case 80:
#line 548 "preproc.y"
    {
			output_simple_statement(yyvsp[0].str);
		}
    break;

  case 81:
#line 552 "preproc.y"
    {
			if (connection)
				mmerror(PARSE_ERROR, ET_ERROR, "no at option for disconnect statement.\n");

			fprintf(yyout, "{ ECPGdisconnect(__LINE__, %s);", yyvsp[0].str);
			whenever_action(2);
			free(yyvsp[0].str);
		}
    break;

  case 82:
#line 561 "preproc.y"
    {
			output_statement(yyvsp[0].str, 0, connection);
		}
    break;

  case 83:
#line 565 "preproc.y"
    {
			fprintf(yyout, "{ ECPGdeallocate(__LINE__, \"%s\");", yyvsp[0].str);

			whenever_action(2);
			free(yyvsp[0].str);
		}
    break;

  case 84:
#line 572 "preproc.y"
    {
			lookup_descriptor(yyvsp[0].descriptor.name, connection);
			output_get_descr(yyvsp[0].descriptor.name, yyvsp[0].descriptor.str);
			free(yyvsp[0].descriptor.name);
			free(yyvsp[0].descriptor.str);
		}
    break;

  case 85:
#line 579 "preproc.y"
    {
			lookup_descriptor(yyvsp[0].str, connection);
			output_get_descr_header(yyvsp[0].str);
			free(yyvsp[0].str);
		}
    break;

  case 86:
#line 585 "preproc.y"
    {
			struct cursor *ptr;
			struct arguments *p;

			for (ptr = cur; ptr != NULL; ptr=ptr->next)
			{
				if (strcmp(ptr->name, yyvsp[0].str) == 0)
					break;
			}

			if (ptr == NULL)
			{
				snprintf(errortext, sizeof(errortext), "trying to open undeclared cursor %s\n", yyvsp[0].str);
				mmerror(PARSE_ERROR, ET_ERROR, errortext);
			}

			/* merge variables given in prepare statement with those given here */
			for (p = ptr->argsinsert; p; p = p->next)
				append_variable(&argsinsert, p->variable, p->indicator);

			for (p = ptr->argsresult; p; p = p->next)
				add_variable(&argsresult, p->variable, p->indicator);

			output_statement(mm_strdup(ptr->command), 0, ptr->connection ? mm_strdup(ptr->connection) : NULL);
		}
    break;

  case 87:
#line 611 "preproc.y"
    {
			if (connection)
				mmerror(PARSE_ERROR, ET_ERROR, "no at option for set connection statement.\n");

			fprintf(yyout, "{ ECPGprepare(__LINE__, %s);", yyvsp[0].str);
			whenever_action(2);
			free(yyvsp[0].str);
		}
    break;

  case 88:
#line 619 "preproc.y"
    { /* output already done */ }
    break;

  case 89:
#line 621 "preproc.y"
    {
			fprintf(yyout, "{ ECPGsetcommit(__LINE__, \"%s\", %s);", yyvsp[0].str, connection ? connection : "NULL");
			whenever_action(2);
			free(yyvsp[0].str);
		}
    break;

  case 90:
#line 627 "preproc.y"
    {
			if (connection)
				mmerror(PARSE_ERROR, ET_ERROR, "no at option for set connection statement.\n");

			fprintf(yyout, "{ ECPGsetconn(__LINE__, %s);", yyvsp[0].str);
			whenever_action(2);
			free(yyvsp[0].str);
		}
    break;

  case 91:
#line 636 "preproc.y"
    {
			if (connection)
				mmerror(PARSE_ERROR, ET_ERROR, "no at option for typedef statement.\n");

			fprintf(yyout, "%s", yyvsp[0].str);
			free(yyvsp[0].str);
			output_line_number();
		}
    break;

  case 92:
#line 645 "preproc.y"
    {
			if (connection)
				mmerror(PARSE_ERROR, ET_ERROR, "no at option for var statement.\n");

			output_simple_statement(yyvsp[0].str);
		}
    break;

  case 93:
#line 652 "preproc.y"
    {
			if (connection)
				mmerror(PARSE_ERROR, ET_ERROR, "no at option for whenever statement.\n");

			output_simple_statement(yyvsp[0].str);
		}
    break;

  case 94:
#line 673 "preproc.y"
    { yyval.str = cat_str(4, make_str("create user"), yyvsp[-2].str, make_str("with"), yyvsp[0].str); }
    break;

  case 95:
#line 676 "preproc.y"
    { yyval.str = make_str("with"); }
    break;

  case 96:
#line 677 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 97:
#line 689 "preproc.y"
    { yyval.str = cat_str(3, make_str("alter user"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 98:
#line 691 "preproc.y"
    { yyval.str = cat_str(4, make_str("alter user"), yyvsp[-2].str, make_str("with"), yyvsp[0].str); }
    break;

  case 99:
#line 695 "preproc.y"
    { yyval.str = cat_str(4, make_str("alter user"), yyvsp[-2].str, make_str("set"), yyvsp[0].str); }
    break;

  case 100:
#line 697 "preproc.y"
    { yyval.str = cat_str(3, make_str("alter user"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 101:
#line 707 "preproc.y"
    { yyval.str = cat2_str(make_str("drop user"), yyvsp[0].str);}
    break;

  case 102:
#line 713 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 103:
#line 714 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 104:
#line 718 "preproc.y"
    { yyval.str = cat2_str(make_str("password"), yyvsp[0].str); }
    break;

  case 105:
#line 720 "preproc.y"
    { yyval.str = cat2_str(make_str("sysid"), yyvsp[0].str); }
    break;

  case 106:
#line 722 "preproc.y"
    { yyval.str = make_str("createdb"); }
    break;

  case 107:
#line 724 "preproc.y"
    { yyval.str = make_str("nocreatedb"); }
    break;

  case 108:
#line 726 "preproc.y"
    { yyval.str = make_str("createuser"); }
    break;

  case 109:
#line 728 "preproc.y"
    { yyval.str = make_str("nocreateuser"); }
    break;

  case 110:
#line 730 "preproc.y"
    { yyval.str = cat2_str(make_str("in group"), yyvsp[0].str); }
    break;

  case 111:
#line 732 "preproc.y"
    { yyval.str = cat2_str(make_str("valid until"), yyvsp[0].str); }
    break;

  case 112:
#line 736 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str(","), yyvsp[0].str); }
    break;

  case 113:
#line 738 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 114:
#line 748 "preproc.y"
    { yyval.str = cat_str(3, make_str("create group"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 115:
#line 750 "preproc.y"
    { yyval.str = cat_str(4, make_str("create group"), yyvsp[-2].str, make_str("with"), yyvsp[0].str); }
    break;

  case 116:
#line 756 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 117:
#line 757 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 118:
#line 761 "preproc.y"
    { yyval.str = cat2_str(make_str("user"), yyvsp[0].str); }
    break;

  case 119:
#line 763 "preproc.y"
    { yyval.str = cat2_str(make_str("sysid"), yyvsp[0].str); }
    break;

  case 120:
#line 774 "preproc.y"
    { yyval.str = cat_str(4, make_str("alter group"), yyvsp[-3].str, make_str("add user"), yyvsp[0].str); }
    break;

  case 121:
#line 776 "preproc.y"
    { yyval.str = cat_str(4, make_str("alter group"), yyvsp[-3].str, make_str("drop user"), yyvsp[0].str); }
    break;

  case 122:
#line 786 "preproc.y"
    { yyval.str = cat2_str(make_str("drop group"), yyvsp[0].str); }
    break;

  case 123:
#line 797 "preproc.y"
    { yyval.str = cat_str(6, make_str("create scheme"), yyvsp[-4].str, yyvsp[-3].str, make_str("authorization"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 124:
#line 799 "preproc.y"
    { yyval.str = cat_str(3, make_str("create scheme"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 125:
#line 802 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 126:
#line 803 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 127:
#line 806 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 128:
#line 807 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 129:
#line 814 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 130:
#line 815 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 131:
#line 816 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 132:
#line 830 "preproc.y"
    { yyval.str = cat2_str(make_str("set"), yyvsp[0].str ); }
    break;

  case 133:
#line 832 "preproc.y"
    { yyval.str = cat2_str(make_str("set local"), yyvsp[0].str ); }
    break;

  case 134:
#line 834 "preproc.y"
    { yyval.str = cat2_str(make_str("set session"), yyvsp[0].str ); }
    break;

  case 135:
#line 838 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("to"), yyvsp[0].str); }
    break;

  case 136:
#line 840 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("="), yyvsp[0].str); }
    break;

  case 137:
#line 842 "preproc.y"
    { yyval.str = cat2_str(make_str("time zone"), yyvsp[0].str); }
    break;

  case 138:
#line 844 "preproc.y"
    { yyval.str = cat_str(3, make_str("transaction isolation level"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 139:
#line 846 "preproc.y"
    { yyval.str = cat2_str(make_str("session characteristics as transaction isolation level"), yyvsp[0].str); }
    break;

  case 140:
#line 848 "preproc.y"
    { yyval.str = cat2_str(make_str("names"), yyvsp[0].str); }
    break;

  case 141:
#line 850 "preproc.y"
    { yyval.str = cat2_str(make_str("session authorization"), yyvsp[0].str); }
    break;

  case 142:
#line 852 "preproc.y"
    { yyval.str = make_str("session authorization default"); }
    break;

  case 143:
#line 856 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 144:
#line 858 "preproc.y"
    { yyval.str = make_str("default"); }
    break;

  case 145:
#line 862 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 146:
#line 864 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str(","), yyvsp[0].str); }
    break;

  case 147:
#line 867 "preproc.y"
    { yyval.str = make_str("read committed"); }
    break;

  case 148:
#line 868 "preproc.y"
    { yyval.str = make_str("serializable"); }
    break;

  case 149:
#line 871 "preproc.y"
    { yyval.str = make_str("read write"); }
    break;

  case 150:
#line 872 "preproc.y"
    { mmerror(PARSE_ERROR, ET_ERROR, "SET TRANSACTION/READ ONLY is not yet supported");
			  yyval.str = make_str("read only"); }
    break;

  case 151:
#line 874 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 152:
#line 877 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 153:
#line 878 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 154:
#line 879 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 155:
#line 882 "preproc.y"
    { yyval.str = make_str("true"); }
    break;

  case 156:
#line 883 "preproc.y"
    { yyval.str = make_str("false"); }
    break;

  case 157:
#line 884 "preproc.y"
    { yyval.str = make_str("on"); }
    break;

  case 158:
#line 885 "preproc.y"
    { yyval.str = make_str("off"); }
    break;

  case 159:
#line 895 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 160:
#line 896 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 161:
#line 898 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 162:
#line 900 "preproc.y"
    { yyval.str = cat_str(6, yyvsp[-5].str, make_str("("), yyvsp[-3].str, make_str(")"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 163:
#line 902 "preproc.y"
    { yyval.str = make_str("default"); }
    break;

  case 164:
#line 904 "preproc.y"
    { yyval.str = make_str("local"); }
    break;

  case 165:
#line 907 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 166:
#line 908 "preproc.y"
    { yyval.str = make_str("default"); }
    break;

  case 167:
#line 909 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 168:
#line 912 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 169:
#line 913 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 170:
#line 917 "preproc.y"
    { yyval.str = cat2_str(make_str("show"), yyvsp[0].str); }
    break;

  case 171:
#line 919 "preproc.y"
    { yyval.str = make_str("show time zone"); }
    break;

  case 172:
#line 921 "preproc.y"
    { yyval.str = make_str("show transaction isolation level"); }
    break;

  case 173:
#line 923 "preproc.y"
    { yyval.str = make_str("show session authorization"); }
    break;

  case 174:
#line 925 "preproc.y"
    { yyval.str = make_str("show all"); }
    break;

  case 175:
#line 929 "preproc.y"
    { yyval.str = cat2_str(make_str("reset"), yyvsp[0].str); }
    break;

  case 176:
#line 931 "preproc.y"
    { yyval.str = make_str("reset time zone"); }
    break;

  case 177:
#line 933 "preproc.y"
    { yyval.str = make_str("reset transaction isolation level"); }
    break;

  case 178:
#line 935 "preproc.y"
    { yyval.str = make_str("reset session authorization"); }
    break;

  case 179:
#line 937 "preproc.y"
    { yyval.str = make_str("reset all"); }
    break;

  case 180:
#line 941 "preproc.y"
    { yyval.str = cat_str(3, make_str("set constraints"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 181:
#line 945 "preproc.y"
    { yyval.str = make_str("all"); }
    break;

  case 182:
#line 947 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 183:
#line 950 "preproc.y"
    { yyval.str = make_str("deferred"); }
    break;

  case 184:
#line 951 "preproc.y"
    { yyval.str = make_str("immediate"); }
    break;

  case 185:
#line 957 "preproc.y"
    { yyval.str= make_str("checkpoint"); }
    break;

  case 186:
#line 970 "preproc.y"
    { yyval.str = cat_str(5, make_str("alter table"), yyvsp[-3].str, make_str("add"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 187:
#line 973 "preproc.y"
    { yyval.str = cat_str(6, make_str("alter table"), yyvsp[-4].str, make_str("alter"), yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 188:
#line 976 "preproc.y"
    { yyval.str = cat_str(6, make_str("alter table"), yyvsp[-6].str, make_str("alter"), yyvsp[-4].str, yyvsp[-3].str, make_str("drop not null")); }
    break;

  case 189:
#line 979 "preproc.y"
    { yyval.str = cat_str(6, make_str("alter table"), yyvsp[-6].str, make_str("alter"), yyvsp[-4].str, yyvsp[-3].str, make_str("set not null")); }
    break;

  case 190:
#line 982 "preproc.y"
    { yyval.str = cat_str(7, make_str("alter table"), yyvsp[-6].str, make_str("alter"), yyvsp[-4].str, yyvsp[-3].str, make_str("set statistics"), yyvsp[0].str); }
    break;

  case 191:
#line 985 "preproc.y"
    { yyval.str = cat_str(7, make_str("alter table"), yyvsp[-6].str, make_str("alter"), yyvsp[-4].str, yyvsp[-3].str, make_str("set storage"), yyvsp[0].str); }
    break;

  case 192:
#line 988 "preproc.y"
    { yyval.str = cat_str(6, make_str("alter table"), yyvsp[-4].str, make_str("drop"), yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 193:
#line 991 "preproc.y"
    { yyval.str = cat_str(4, make_str("alter table"), yyvsp[-2].str, make_str("add"), yyvsp[0].str); }
    break;

  case 194:
#line 994 "preproc.y"
    { yyval.str = cat_str(5, make_str("alter table"), yyvsp[-4].str, make_str("drop constraint"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 195:
#line 997 "preproc.y"
    { yyval.str = cat_str(3, make_str("alter table"), yyvsp[-3].str, make_str("create toast table")); }
    break;

  case 196:
#line 1000 "preproc.y"
    { yyval.str = cat_str(4, make_str("alter table"), yyvsp[-3].str, make_str("owner to"), yyvsp[0].str); }
    break;

  case 197:
#line 1004 "preproc.y"
    { yyval.str = cat2_str(make_str("set default"), yyvsp[0].str); }
    break;

  case 198:
#line 1005 "preproc.y"
    { yyval.str = make_str("drop default"); }
    break;

  case 199:
#line 1008 "preproc.y"
    { yyval.str = make_str("cascade"); }
    break;

  case 200:
#line 1009 "preproc.y"
    { yyval.str = make_str("restrict"); }
    break;

  case 201:
#line 1010 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 202:
#line 1020 "preproc.y"
    { yyval.str = cat2_str(make_str("close"), yyvsp[0].str); }
    break;

  case 203:
#line 1023 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 204:
#line 1024 "preproc.y"
    { yyval.str = NULL; }
    break;

  case 205:
#line 1037 "preproc.y"
    { yyval.str = cat_str(9, make_str("copy"), yyvsp[-7].str, yyvsp[-6].str, yyvsp[-5].str, yyvsp[-4].str, yyvsp[-3].str, yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 206:
#line 1040 "preproc.y"
    { yyval.str = make_str("to"); }
    break;

  case 207:
#line 1041 "preproc.y"
    { yyval.str = make_str("from"); }
    break;

  case 208:
#line 1049 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 209:
#line 1050 "preproc.y"
    { yyval.str = make_str("stdin"); }
    break;

  case 210:
#line 1051 "preproc.y"
    { yyval.str = make_str("stdout"); }
    break;

  case 211:
#line 1054 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 212:
#line 1055 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 213:
#line 1058 "preproc.y"
    { yyval.str = make_str("binary"); }
    break;

  case 214:
#line 1059 "preproc.y"
    { yyval.str = make_str("oids"); }
    break;

  case 215:
#line 1061 "preproc.y"
    { yyval.str = cat_str(3, make_str("delimiter"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 216:
#line 1063 "preproc.y"
    { yyval.str = cat_str(3, make_str("null"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 217:
#line 1066 "preproc.y"
    { yyval.str = make_str("binary"); }
    break;

  case 218:
#line 1067 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 219:
#line 1070 "preproc.y"
    { yyval.str = make_str("with oids"); }
    break;

  case 220:
#line 1071 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 221:
#line 1079 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("delimiters"), yyvsp[0].str); }
    break;

  case 222:
#line 1081 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 223:
#line 1084 "preproc.y"
    { yyval.str = make_str("using"); }
    break;

  case 224:
#line 1085 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 225:
#line 1097 "preproc.y"
    { yyval.str = cat_str(9, make_str("create"), yyvsp[-7].str, make_str("table"), yyvsp[-5].str, make_str("("), yyvsp[-3].str, make_str(")"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 226:
#line 1100 "preproc.y"
    { yyval.str = cat_str(10, make_str("create"), yyvsp[-8].str, make_str("table"), yyvsp[-6].str, make_str("of"), yyvsp[-4].str, make_str("("), yyvsp[-2].str, make_str(")"), yyvsp[0].str); }
    break;

  case 227:
#line 1108 "preproc.y"
    { yyval.str = make_str("temporary"); }
    break;

  case 228:
#line 1109 "preproc.y"
    { yyval.str = make_str("temp"); }
    break;

  case 229:
#line 1110 "preproc.y"
    { yyval.str = make_str("local temporary"); }
    break;

  case 230:
#line 1111 "preproc.y"
    { yyval.str = make_str("local temp"); }
    break;

  case 231:
#line 1113 "preproc.y"
    {
			mmerror(PARSE_ERROR, ET_WARNING, "Currently unsupported CREATE TABLE / GLOBAL TEMPORARY will be passed to backend");
			yyval.str = make_str("global temporary");
		}
    break;

  case 232:
#line 1118 "preproc.y"
    {
			mmerror(PARSE_ERROR, ET_WARNING, "Currently unsupported CREATE TABLE / GLOBAL TEMP will be passed to backend");
			yyval.str = make_str("global temp");
		}
    break;

  case 233:
#line 1122 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 234:
#line 1127 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 235:
#line 1129 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 236:
#line 1132 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 237:
#line 1134 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str(","), yyvsp[0].str); }
    break;

  case 238:
#line 1137 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 239:
#line 1138 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 240:
#line 1139 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 241:
#line 1143 "preproc.y"
    {
			if (strlen(yyvsp[0].str) > 0)
			{
				snprintf(errortext, sizeof(errortext), "Currently unsupported CREATE TABLE / COLLATE %s will be passed to backend", yyvsp[0].str);
				mmerror(PARSE_ERROR, ET_WARNING, errortext);
			}
			yyval.str = cat_str(4, yyvsp[-3].str, yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str);
		}
    break;

  case 242:
#line 1153 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str,yyvsp[0].str); }
    break;

  case 243:
#line 1154 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 244:
#line 1158 "preproc.y"
    { yyval.str = cat_str(3, make_str("constraint"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 245:
#line 1159 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 246:
#line 1160 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 247:
#line 1175 "preproc.y"
    { yyval.str = make_str("not null"); }
    break;

  case 248:
#line 1177 "preproc.y"
    { yyval.str = make_str("null"); }
    break;

  case 249:
#line 1179 "preproc.y"
    { yyval.str = make_str("unique"); }
    break;

  case 250:
#line 1181 "preproc.y"
    { yyval.str = make_str("primary key"); }
    break;

  case 251:
#line 1183 "preproc.y"
    { yyval.str = cat_str(3, make_str("check ("), yyvsp[-1].str, make_str(")")); }
    break;

  case 252:
#line 1185 "preproc.y"
    { yyval.str = cat2_str(make_str("default"), yyvsp[0].str); }
    break;

  case 253:
#line 1187 "preproc.y"
    { yyval.str = cat_str(5, make_str("references"), yyvsp[-3].str, yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 254:
#line 1201 "preproc.y"
    { yyval.str = make_str("deferrable"); }
    break;

  case 255:
#line 1202 "preproc.y"
    { yyval.str = make_str("not deferrable"); }
    break;

  case 256:
#line 1203 "preproc.y"
    { yyval.str = make_str("initially deferred"); }
    break;

  case 257:
#line 1204 "preproc.y"
    { yyval.str = make_str("initially immediate"); }
    break;

  case 258:
#line 1208 "preproc.y"
    { 
				mmerror(PARSE_ERROR, ET_ERROR, "LIKE in table definitions not yet supported");	
				yyval.str = cat2_str(make_str("like"), yyvsp[0].str);
			}
    break;

  case 259:
#line 1219 "preproc.y"
    { yyval.str = cat_str(3, make_str("constraint"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 260:
#line 1221 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 261:
#line 1225 "preproc.y"
    { yyval.str = cat_str(3, make_str("check("), yyvsp[-1].str, make_str(")")); }
    break;

  case 262:
#line 1227 "preproc.y"
    { yyval.str = cat_str(3, make_str("unique("), yyvsp[-1].str, make_str(")")); }
    break;

  case 263:
#line 1229 "preproc.y"
    { yyval.str = cat_str(3, make_str("primary key("), yyvsp[-1].str, make_str(")")); }
    break;

  case 264:
#line 1232 "preproc.y"
    { yyval.str = cat_str(8, make_str("foreign key("), yyvsp[-7].str, make_str(") references"), yyvsp[-4].str, yyvsp[-3].str, yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 265:
#line 1235 "preproc.y"
    { yyval.str = cat_str(3, make_str("("), yyvsp[-1].str, make_str(")")); }
    break;

  case 266:
#line 1236 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 267:
#line 1240 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str(","), yyvsp[0].str); }
    break;

  case 268:
#line 1242 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 269:
#line 1245 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 270:
#line 1249 "preproc.y"
    { yyval.str = make_str("match full"); }
    break;

  case 271:
#line 1251 "preproc.y"
    {
			mmerror(PARSE_ERROR, ET_WARNING, "Currently unsupported FOREIGN KEY/MATCH PARTIAL will be passed to backend");
			yyval.str = make_str("match partial");
		}
    break;

  case 272:
#line 1256 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 273:
#line 1259 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 274:
#line 1260 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 275:
#line 1261 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 276:
#line 1262 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 277:
#line 1263 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 278:
#line 1267 "preproc.y"
    { yyval.str = cat2_str(make_str("on delete"), yyvsp[0].str); }
    break;

  case 279:
#line 1271 "preproc.y"
    { yyval.str = cat2_str(make_str("on update"), yyvsp[0].str); }
    break;

  case 280:
#line 1274 "preproc.y"
    { yyval.str = make_str("no action"); }
    break;

  case 281:
#line 1275 "preproc.y"
    { yyval.str = make_str("restrict"); }
    break;

  case 282:
#line 1276 "preproc.y"
    { yyval.str = make_str("cascade"); }
    break;

  case 283:
#line 1277 "preproc.y"
    { yyval.str = make_str("set default"); }
    break;

  case 284:
#line 1278 "preproc.y"
    { yyval.str = make_str("set null"); }
    break;

  case 285:
#line 1282 "preproc.y"
    { yyval.str = cat_str(3, make_str("inherits ("), yyvsp[-1].str, make_str(")")); }
    break;

  case 286:
#line 1284 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 287:
#line 1287 "preproc.y"
    { yyval.str = make_str("with oids"); }
    break;

  case 288:
#line 1288 "preproc.y"
    { yyval.str = make_str("without oids"); }
    break;

  case 289:
#line 1289 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 290:
#line 1299 "preproc.y"
    { FoundInto = 0; }
    break;

  case 291:
#line 1301 "preproc.y"
    {
			if (FoundInto == 1)
				mmerror(PARSE_ERROR, ET_ERROR, "CREATE TABLE / AS SELECT may not specify INTO");

			yyval.str = cat_str(7, make_str("create"), yyvsp[-6].str, make_str("table"), yyvsp[-4].str, yyvsp[-3].str, make_str("as"), yyvsp[0].str);
		}
    break;

  case 292:
#line 1310 "preproc.y"
    { yyval.str = cat_str(3, make_str("("), yyvsp[-1].str, make_str(")")); }
    break;

  case 293:
#line 1312 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 294:
#line 1316 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str(","), yyvsp[0].str); }
    break;

  case 295:
#line 1318 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 296:
#line 1321 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 297:
#line 1332 "preproc.y"
    { yyval.str = cat_str(4, make_str("create sequence"), yyvsp[-3].str, yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 298:
#line 1335 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 299:
#line 1336 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 300:
#line 1340 "preproc.y"
    { yyval.str = cat2_str(make_str("cache"), yyvsp[0].str); }
    break;

  case 301:
#line 1342 "preproc.y"
    { yyval.str = make_str("cycle"); }
    break;

  case 302:
#line 1344 "preproc.y"
    { yyval.str = cat2_str(make_str("increment"), yyvsp[0].str); }
    break;

  case 303:
#line 1346 "preproc.y"
    { yyval.str = cat2_str(make_str("maxvalue"), yyvsp[0].str); }
    break;

  case 304:
#line 1348 "preproc.y"
    { yyval.str = cat2_str(make_str("minvalue"), yyvsp[0].str); }
    break;

  case 305:
#line 1350 "preproc.y"
    { yyval.str = cat2_str(make_str("start"), yyvsp[0].str); }
    break;

  case 306:
#line 1363 "preproc.y"
    { yyval.str = cat_str(8, make_str("create"), yyvsp[-6].str, yyvsp[-5].str, make_str("language"), yyvsp[-3].str, make_str("handler"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 307:
#line 1366 "preproc.y"
    { yyval.str = make_str("trusted"); }
    break;

  case 308:
#line 1367 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 309:
#line 1375 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 310:
#line 1377 "preproc.y"
    { yyval.str = yyvsp[0].str; /* XXX changing soon */ }
    break;

  case 311:
#line 1381 "preproc.y"
    { yyval.str = cat2_str(make_str("lancompiler"), yyvsp[0].str); }
    break;

  case 312:
#line 1383 "preproc.y"
    { yyval.str = ""; }
    break;

  case 313:
#line 1387 "preproc.y"
    { yyval.str = cat_str(5, make_str("drop"), yyvsp[-3].str, make_str("language"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 314:
#line 1390 "preproc.y"
    { yyval.str = make_str("prcedural"); }
    break;

  case 315:
#line 1391 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 316:
#line 1406 "preproc.y"
    { yyval.str = cat_str(12, make_str("create trigger"), yyvsp[-11].str, yyvsp[-10].str, yyvsp[-9].str, make_str("on"), yyvsp[-7].str, yyvsp[-6].str, make_str("execute procedure"), yyvsp[-3].str, make_str("("), yyvsp[-1].str, make_str(")")); }
    break;

  case 317:
#line 1412 "preproc.y"
    { yyval.str = cat_str(13, make_str("create constraint trigger"), yyvsp[-15].str, make_str("after"), yyvsp[-13].str, make_str("on"), yyvsp[-11].str, yyvsp[-10].str, yyvsp[-9].str, make_str("for each row execute procedure"), yyvsp[-3].str, make_str("("), yyvsp[-1].str, make_str(")")); }
    break;

  case 318:
#line 1415 "preproc.y"
    { yyval.str = make_str("before"); }
    break;

  case 319:
#line 1416 "preproc.y"
    { yyval.str = make_str("after"); }
    break;

  case 320:
#line 1420 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 321:
#line 1422 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("or"), yyvsp[0].str); }
    break;

  case 322:
#line 1424 "preproc.y"
    { yyval.str = cat_str(5, yyvsp[-4].str, make_str("or"), yyvsp[-2].str, make_str("or"), yyvsp[0].str); }
    break;

  case 323:
#line 1427 "preproc.y"
    { yyval.str = make_str("insert"); }
    break;

  case 324:
#line 1428 "preproc.y"
    { yyval.str = make_str("delete"); }
    break;

  case 325:
#line 1429 "preproc.y"
    { yyval.str = make_str("update"); }
    break;

  case 326:
#line 1433 "preproc.y"
    { yyval.str = cat_str(3, make_str("for"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 327:
#line 1436 "preproc.y"
    { yyval.str = make_str("each"); }
    break;

  case 328:
#line 1437 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 329:
#line 1440 "preproc.y"
    { yyval.str = make_str("row"); }
    break;

  case 330:
#line 1441 "preproc.y"
    { yyval.str = make_str("statement"); }
    break;

  case 331:
#line 1445 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 332:
#line 1447 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str(","), yyvsp[0].str); }
    break;

  case 333:
#line 1449 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 334:
#line 1452 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 335:
#line 1453 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 336:
#line 1456 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 337:
#line 1457 "preproc.y"
    { yyval.str = cat2_str(make_str("from"), yyvsp[0].str); }
    break;

  case 338:
#line 1460 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 339:
#line 1462 "preproc.y"
    {
			if (strcmp(yyvsp[-1].str, "deferrable") != 0 && strcmp(yyvsp[0].str, "initially deferrable") == 0 )
				mmerror(PARSE_ERROR, ET_ERROR, "INITIALLY DEFERRED constraint must be DEFERRABLE");

			yyval.str = cat2_str(yyvsp[-1].str, yyvsp[0].str);
		}
    break;

  case 340:
#line 1468 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 341:
#line 1470 "preproc.y"
    {
			if (strcmp(yyvsp[0].str, "deferrable") != 0 && strcmp(yyvsp[-1].str, "initially deferrable") == 0 )
				mmerror(PARSE_ERROR, ET_ERROR, "INITIALLY DEFERRED constraint must be DEFERRABLE");

			yyval.str = cat2_str(yyvsp[-1].str, yyvsp[0].str);
		}
    break;

  case 342:
#line 1479 "preproc.y"
    { yyval.str = make_str("not deferrable"); }
    break;

  case 343:
#line 1481 "preproc.y"
    { yyval.str = make_str("deferrable"); }
    break;

  case 344:
#line 1485 "preproc.y"
    { yyval.str = make_str("initially immediate"); }
    break;

  case 345:
#line 1487 "preproc.y"
    { yyval.str = make_str("initially deferred"); }
    break;

  case 346:
#line 1491 "preproc.y"
    { yyval.str = cat_str(5, make_str("drop trigger"), yyvsp[-3].str, make_str("on"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 347:
#line 1503 "preproc.y"
    {
				mmerror(PARSE_ERROR, ET_ERROR, "CREATE ASSERTION is not yet supported");
		       		yyval.str = cat_str(6, make_str("create assertion"), yyvsp[-5].str, make_str("check ("), yyvsp[-2].str, make_str(")"), yyvsp[0].str);
			}
    break;

  case 348:
#line 1510 "preproc.y"
    {
		mmerror(PARSE_ERROR, ET_ERROR, "DROP ASSERTION is not yet supported");
		yyval.str = cat2_str(make_str("drop assertion"), yyvsp[0].str);
	}
    break;

  case 349:
#line 1525 "preproc.y"
    { yyval.str = cat_str(3, make_str("create aggregate"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 350:
#line 1527 "preproc.y"
    { yyval.str = cat_str(3, make_str("create operator"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 351:
#line 1529 "preproc.y"
    { yyval.str = cat_str(3, make_str("create type"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 352:
#line 1531 "preproc.y"
    { yyval.str = cat_str(4, make_str("create type"), yyvsp[-2].str, make_str("as"), yyvsp[0].str); }
    break;

  case 353:
#line 1533 "preproc.y"
    { yyval.str = cat_str(6, make_str("create character set"), yyvsp[-4].str, yyvsp[-3].str, make_str("get"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 354:
#line 1537 "preproc.y"
    { yyval.str = cat_str(3, make_str("("), yyvsp[-1].str, make_str(")"));}
    break;

  case 355:
#line 1541 "preproc.y"
    { yyval.str = cat_str(3, make_str("("), yyvsp[-1].str, make_str(")")); }
    break;

  case 356:
#line 1544 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 357:
#line 1545 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str(","), yyvsp[0].str); }
    break;

  case 358:
#line 1548 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("="), yyvsp[0].str); }
    break;

  case 359:
#line 1549 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 360:
#line 1553 "preproc.y"
    {  yyval.str = yyvsp[0].str; }
    break;

  case 361:
#line 1554 "preproc.y"
    {  yyval.str = yyvsp[0].str; }
    break;

  case 362:
#line 1555 "preproc.y"
    {  yyval.str = yyvsp[0].str; }
    break;

  case 363:
#line 1560 "preproc.y"
    {
					yyval.str = cat_str(9, make_str("create operator class"), yyvsp[-8].str, yyvsp[-7].str, make_str("for type"), yyvsp[-4].str, make_str("using"), yyvsp[-2].str, make_str("as"), yyvsp[0].str);
				}
    break;

  case 364:
#line 1565 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 365:
#line 1566 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str(","), yyvsp[0].str); }
    break;

  case 366:
#line 1570 "preproc.y"
    { yyval.str = cat_str(4, make_str("operator"), yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 367:
#line 1572 "preproc.y"
    { yyval.str =  cat_str(7, make_str("operator"), yyvsp[-5].str, yyvsp[-4].str, make_str("("), yyvsp[-2].str, make_str(")"), yyvsp[0].str); }
    break;

  case 368:
#line 1574 "preproc.y"
    { yyval.str = cat_str(4, make_str("function"), yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 369:
#line 1576 "preproc.y"
    { yyval.str = cat2_str(make_str("storage"), yyvsp[0].str); }
    break;

  case 370:
#line 1579 "preproc.y"
    { yyval.str = make_str("default"); }
    break;

  case 371:
#line 1580 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 372:
#line 1583 "preproc.y"
    { yyval.str = make_str("recheck"); }
    break;

  case 373:
#line 1584 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 374:
#line 1588 "preproc.y"
    { yyval.str = cat_str(5,make_str("drop operator class"), yyvsp[-3].str, make_str("using"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 375:
#line 1600 "preproc.y"
    { yyval.str = cat_str(4, make_str("drop"), yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 376:
#line 1603 "preproc.y"
    { yyval.str = make_str("table"); }
    break;

  case 377:
#line 1604 "preproc.y"
    { yyval.str = make_str("sequence"); }
    break;

  case 378:
#line 1605 "preproc.y"
    { yyval.str = make_str("view"); }
    break;

  case 379:
#line 1606 "preproc.y"
    { yyval.str = make_str("index"); }
    break;

  case 380:
#line 1607 "preproc.y"
    { yyval.str = make_str("type"); }
    break;

  case 381:
#line 1608 "preproc.y"
    { yyval.str = make_str("domain"); }
    break;

  case 382:
#line 1609 "preproc.y"
    { yyval.str = make_str("conversion"); }
    break;

  case 383:
#line 1610 "preproc.y"
    { yyval.str = make_str("schema"); }
    break;

  case 384:
#line 1614 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 385:
#line 1616 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str(","), yyvsp[0].str); }
    break;

  case 386:
#line 1620 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 387:
#line 1622 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 388:
#line 1631 "preproc.y"
    { yyval.str = cat_str(3, make_str("truncate table"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 389:
#line 1644 "preproc.y"
    {
			if (strcmp(yyvsp[-4].str, "relative") == 0 && atol(yyvsp[-3].str) == 0L)
				mmerror(PARSE_ERROR, ET_ERROR, "FETCH/RELATIVE at current position is not supported");

			yyval.str = cat_str(5, make_str("fetch"), yyvsp[-4].str, yyvsp[-3].str, yyvsp[-2].str, yyvsp[-1].str);
		}
    break;

  case 390:
#line 1651 "preproc.y"
    { yyval.str = cat_str(4, make_str("fetch"), yyvsp[-3].str, yyvsp[-2].str, yyvsp[-1].str); }
    break;

  case 391:
#line 1653 "preproc.y"
    { yyval.str = cat_str(4, make_str("fetch"), yyvsp[-3].str, yyvsp[-2].str, yyvsp[-1].str); }
    break;

  case 392:
#line 1655 "preproc.y"
    { yyval.str = cat_str(3, make_str("fetch"), yyvsp[-2].str, yyvsp[-1].str); }
    break;

  case 393:
#line 1657 "preproc.y"
    { yyval.str = cat2_str(make_str("fetch"), yyvsp[-1].str); }
    break;

  case 394:
#line 1659 "preproc.y"
    { yyval.str = cat_str(5, make_str("move"), yyvsp[-3].str, yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 395:
#line 1661 "preproc.y"
    { yyval.str = cat_str(4, make_str("move"), yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 396:
#line 1663 "preproc.y"
    { yyval.str = cat_str(4, make_str("move"), yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 397:
#line 1665 "preproc.y"
    { yyval.str = cat_str(3, make_str("move"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 398:
#line 1667 "preproc.y"
    { yyval.str = cat2_str(make_str("move"), yyvsp[0].str); }
    break;

  case 399:
#line 1670 "preproc.y"
    { yyval.str = make_str("forward"); }
    break;

  case 400:
#line 1671 "preproc.y"
    { yyval.str = make_str("backward"); }
    break;

  case 401:
#line 1672 "preproc.y"
    { yyval.str = make_str("relative"); }
    break;

  case 402:
#line 1674 "preproc.y"
    {
			mmerror(PARSE_ERROR, ET_WARNING, "Currently unsupported FETCH/ABSOLUTE will be passed to backend, backend will use RELATIVE");
			yyval.str = make_str("absolute");
		}
    break;

  case 403:
#line 1680 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 404:
#line 1681 "preproc.y"
    { yyval.str = make_str("all"); }
    break;

  case 405:
#line 1682 "preproc.y"
    { yyval.str = make_str("next"); }
    break;

  case 406:
#line 1683 "preproc.y"
    { yyval.str = make_str("prior"); }
    break;

  case 407:
#line 1686 "preproc.y"
    { yyval.str = make_str("in"); }
    break;

  case 408:
#line 1687 "preproc.y"
    { yyval.str = make_str("from"); }
    break;

  case 409:
#line 1703 "preproc.y"
    { yyval.str = cat_str(5, make_str("comment on"), yyvsp[-3].str, yyvsp[-2].str, make_str("is"), yyvsp[0].str); }
    break;

  case 410:
#line 1705 "preproc.y"
    { yyval.str = cat_str(6, make_str("comment on aggregate"), yyvsp[-5].str, make_str("("), yyvsp[-3].str, make_str(") is"), yyvsp[0].str); }
    break;

  case 411:
#line 1707 "preproc.y"
    { yyval.str = cat_str(5, make_str("comment on function"), yyvsp[-3].str, yyvsp[-2].str, make_str("is"), yyvsp[0].str); }
    break;

  case 412:
#line 1709 "preproc.y"
    { yyval.str = cat_str(6, make_str("comment on operator"), yyvsp[-5].str, make_str("("), yyvsp[-3].str, make_str(") is"), yyvsp[0].str); }
    break;

  case 413:
#line 1711 "preproc.y"
    { yyval.str = cat_str(6, make_str("comment on trigger"), yyvsp[-4].str, make_str("on"), yyvsp[-2].str, make_str("is"), yyvsp[0].str); }
    break;

  case 414:
#line 1713 "preproc.y"
    { yyval.str = cat_str(6, make_str("comment on rule"), yyvsp[-4].str, make_str("on"), yyvsp[-2].str, make_str("is"), yyvsp[0].str); }
    break;

  case 415:
#line 1715 "preproc.y"
    { yyval.str = cat_str(4, make_str("comment on rule"), yyvsp[-2].str, make_str("is"), yyvsp[0].str); }
    break;

  case 416:
#line 1718 "preproc.y"
    { yyval.str = make_str("column"); }
    break;

  case 417:
#line 1719 "preproc.y"
    { yyval.str = make_str("database"); }
    break;

  case 418:
#line 1720 "preproc.y"
    { yyval.str = make_str("schema"); }
    break;

  case 419:
#line 1721 "preproc.y"
    { yyval.str = make_str("idnex"); }
    break;

  case 420:
#line 1722 "preproc.y"
    { yyval.str = make_str("sequence"); }
    break;

  case 421:
#line 1723 "preproc.y"
    { yyval.str = make_str("table"); }
    break;

  case 422:
#line 1724 "preproc.y"
    { yyval.str = make_str("domain"); }
    break;

  case 423:
#line 1725 "preproc.y"
    { yyval.str = make_str("type"); }
    break;

  case 424:
#line 1726 "preproc.y"
    { yyval.str = make_str("view"); }
    break;

  case 425:
#line 1729 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 426:
#line 1730 "preproc.y"
    { yyval.str = make_str("null"); }
    break;

  case 427:
#line 1741 "preproc.y"
    { yyval.str = cat_str(7, make_str("grant"), yyvsp[-5].str, make_str("on"), yyvsp[-3].str, make_str("to"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 428:
#line 1745 "preproc.y"
    { yyval.str = cat_str(8, make_str("revoke"), yyvsp[-5].str, yyvsp[-4].str, make_str("on"), yyvsp[-2].str, make_str("from"), yyvsp[0].str); }
    break;

  case 429:
#line 1748 "preproc.y"
    { yyval.str = make_str("all privileges"); }
    break;

  case 430:
#line 1749 "preproc.y"
    { yyval.str = make_str("all"); }
    break;

  case 431:
#line 1750 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 432:
#line 1754 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 433:
#line 1756 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str(","), yyvsp[0].str); }
    break;

  case 434:
#line 1759 "preproc.y"
    { yyval.str = make_str("select"); }
    break;

  case 435:
#line 1760 "preproc.y"
    { yyval.str = make_str("insert"); }
    break;

  case 436:
#line 1761 "preproc.y"
    { yyval.str = make_str("update"); }
    break;

  case 437:
#line 1762 "preproc.y"
    { yyval.str = make_str("delete"); }
    break;

  case 438:
#line 1763 "preproc.y"
    { yyval.str = make_str("rule"); }
    break;

  case 439:
#line 1764 "preproc.y"
    { yyval.str = make_str("references"); }
    break;

  case 440:
#line 1765 "preproc.y"
    { yyval.str = make_str("trigger"); }
    break;

  case 441:
#line 1766 "preproc.y"
    { yyval.str = make_str("execute"); }
    break;

  case 442:
#line 1767 "preproc.y"
    { yyval.str = make_str("usage"); }
    break;

  case 443:
#line 1768 "preproc.y"
    { yyval.str = make_str("create"); }
    break;

  case 444:
#line 1769 "preproc.y"
    { yyval.str = make_str("temporary"); }
    break;

  case 445:
#line 1770 "preproc.y"
    { yyval.str = make_str("temp"); }
    break;

  case 446:
#line 1774 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 447:
#line 1776 "preproc.y"
    { yyval.str = cat2_str(make_str("table"), yyvsp[0].str); }
    break;

  case 448:
#line 1778 "preproc.y"
    { yyval.str = cat2_str(make_str("function"), yyvsp[0].str); }
    break;

  case 449:
#line 1780 "preproc.y"
    { yyval.str = cat2_str(make_str("database"), yyvsp[0].str); }
    break;

  case 450:
#line 1782 "preproc.y"
    { yyval.str = cat2_str(make_str("language") , yyvsp[0].str); }
    break;

  case 451:
#line 1784 "preproc.y"
    { yyval.str = cat2_str(make_str("schema") , yyvsp[0].str); }
    break;

  case 452:
#line 1788 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 453:
#line 1790 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str(","), yyvsp[0].str); }
    break;

  case 454:
#line 1793 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 455:
#line 1794 "preproc.y"
    { yyval.str = cat2_str(make_str("group"), yyvsp[0].str); }
    break;

  case 456:
#line 1798 "preproc.y"
    {
			mmerror(PARSE_ERROR, ET_WARNING, "Currently unsupported GRANT/WITH GRANT OPTION will be passed to backend");
			yyval.str = make_str("with grant option");
		}
    break;

  case 457:
#line 1802 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 458:
#line 1806 "preproc.y"
    {
			mmerror(PARSE_ERROR, ET_WARNING, "Currently unsupported REVOKE/GRANT OPTION FOR will be passed to backend");
			yyval.str = make_str("with grant option");
		}
    break;

  case 459:
#line 1810 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 460:
#line 1814 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 461:
#line 1816 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str(","), yyvsp[0].str); }
    break;

  case 462:
#line 1819 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 463:
#line 1832 "preproc.y"
    { yyval.str = cat_str(11, make_str("create"), yyvsp[-9].str, make_str("index"), yyvsp[-7].str, make_str("on"), yyvsp[-5].str, yyvsp[-4].str, make_str("("), yyvsp[-2].str, make_str(")"), yyvsp[0].str); }
    break;

  case 464:
#line 1835 "preproc.y"
    { yyval.str = make_str("unique"); }
    break;

  case 465:
#line 1836 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 466:
#line 1840 "preproc.y"
    { yyval.str = cat2_str(make_str("using"), yyvsp[0].str); }
    break;

  case 467:
#line 1842 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 468:
#line 1845 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 469:
#line 1846 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 470:
#line 1850 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str(","), yyvsp[0].str); }
    break;

  case 471:
#line 1852 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 472:
#line 1856 "preproc.y"
    { yyval.str = cat_str(5, yyvsp[-4].str, make_str("("), yyvsp[-2].str, ")", yyvsp[0].str); }
    break;

  case 473:
#line 1860 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 474:
#line 1863 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 475:
#line 1864 "preproc.y"
    { yyval.str = cat2_str(make_str("using"), yyvsp[0].str); }
    break;

  case 476:
#line 1865 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 477:
#line 1870 "preproc.y"
    { yyval.str = cat_str(8, make_str("create"), yyvsp[-7].str, make_str("function"), yyvsp[-5].str, yyvsp[-4].str, make_str("returns"), yyvsp[-2].str, yyvsp[-1].str); }
    break;

  case 478:
#line 1873 "preproc.y"
    { yyval.str = make_str("or replace"); }
    break;

  case 479:
#line 1874 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 480:
#line 1878 "preproc.y"
    { yyval.str = cat_str(3, make_str("("), yyvsp[-1].str, make_str(")")); }
    break;

  case 481:
#line 1880 "preproc.y"
    { yyval.str = make_str("()"); }
    break;

  case 482:
#line 1884 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 483:
#line 1886 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str(","), yyvsp[0].str); }
    break;

  case 484:
#line 1890 "preproc.y"
    {
			/* We can catch over-specified arguments here if we want to,
			 * but for now better to silently swallow typmod, etc.
			 * - thomas 2000-03-22
			 */
			yyval.str = cat2_str(yyvsp[-1].str, yyvsp[0].str);
		}
    break;

  case 485:
#line 1897 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 486:
#line 1900 "preproc.y"
    { yyval.str = make_str("in"); }
    break;

  case 487:
#line 1902 "preproc.y"
    {
			mmerror(PARSE_ERROR, ET_WARNING, "Currently unsupported CREATE FUNCTION/OUT will be passed to backend");

			yyval.str = make_str("out");
		}
    break;

  case 488:
#line 1908 "preproc.y"
    {
			mmerror(PARSE_ERROR, ET_WARNING, "Currently unsupported CREATE FUNCTION/INOUT will be passed to backend");

			yyval.str = make_str("inout");
		}
    break;

  case 489:
#line 1916 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 490:
#line 1918 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str(","), yyvsp[0].str); }
    break;

  case 491:
#line 1922 "preproc.y"
    {
			/* We can catch over-specified arguments here if we want to,
			 * but for now better to silently swallow typmod, etc.
			 * - thomas 2000-03-22
			 */
			yyval.str = yyvsp[0].str;
		}
    break;

  case 492:
#line 1932 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 493:
#line 1934 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-3].str, yyvsp[-2].str, make_str("% type")); }
    break;

  case 494:
#line 1939 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 495:
#line 1941 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 496:
#line 1945 "preproc.y"
    { yyval.str = cat2_str(make_str("as"), yyvsp[0].str); }
    break;

  case 497:
#line 1947 "preproc.y"
    { yyval.str = cat2_str(make_str("language"), yyvsp[0].str); }
    break;

  case 498:
#line 1949 "preproc.y"
    { yyval.str = make_str("immutable"); }
    break;

  case 499:
#line 1951 "preproc.y"
    { yyval.str = make_str("stable"); }
    break;

  case 500:
#line 1953 "preproc.y"
    { yyval.str = make_str("volatile"); }
    break;

  case 501:
#line 1955 "preproc.y"
    { yyval.str = make_str("called on null input"); }
    break;

  case 502:
#line 1957 "preproc.y"
    { yyval.str = make_str("returns null on null input"); }
    break;

  case 503:
#line 1959 "preproc.y"
    { yyval.str = make_str("strict"); }
    break;

  case 504:
#line 1961 "preproc.y"
    { yyval.str = make_str("external security definer"); }
    break;

  case 505:
#line 1963 "preproc.y"
    { yyval.str = make_str("external security invoker"); }
    break;

  case 506:
#line 1965 "preproc.y"
    { yyval.str = make_str("security definer"); }
    break;

  case 507:
#line 1967 "preproc.y"
    { yyval.str = make_str("security invoker"); }
    break;

  case 508:
#line 1970 "preproc.y"
    { yyval.str = cat2_str(make_str("with"), yyvsp[0].str); }
    break;

  case 509:
#line 1971 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 510:
#line 1985 "preproc.y"
    { yyval.str = cat_str(4, make_str("drop function"), yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 511:
#line 1989 "preproc.y"
    { yyval.str = cat_str(6, make_str("drop aggregate"), yyvsp[-4].str, make_str("("), yyvsp[-2].str, make_str(")"), yyvsp[0].str); }
    break;

  case 512:
#line 1992 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 513:
#line 1993 "preproc.y"
    { yyval.str = make_str("*"); }
    break;

  case 514:
#line 1998 "preproc.y"
    { yyval.str = cat_str(6, make_str("drop operator"), yyvsp[-4].str, make_str("("), yyvsp[-2].str, make_str(")"), yyvsp[0].str); }
    break;

  case 515:
#line 2002 "preproc.y"
    { mmerror(PARSE_ERROR, ET_ERROR, "parser: argument type missing (use NONE for unary operators)"); }
    break;

  case 516:
#line 2004 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str(","), yyvsp[0].str); }
    break;

  case 517:
#line 2006 "preproc.y"
    { yyval.str = cat2_str(make_str("none,"), yyvsp[0].str); }
    break;

  case 518:
#line 2008 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-2].str, make_str(", none")); }
    break;

  case 519:
#line 2013 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 520:
#line 2015 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("."), yyvsp[0].str); }
    break;

  case 521:
#line 2020 "preproc.y"
    { yyval.str = cat_str(6, make_str("create cast ("), yyvsp[-7].str, make_str("as"), yyvsp[-5].str, make_str(") with function"), yyvsp[-1].str); }
    break;

  case 522:
#line 2023 "preproc.y"
    { yyval.str = cat_str(6, make_str("create cast ("), yyvsp[-6].str, make_str("as"), yyvsp[-4].str, make_str(") without function"), yyvsp[0].str); }
    break;

  case 523:
#line 2026 "preproc.y"
    { yyval.str = make_str("as assignment"); }
    break;

  case 524:
#line 2027 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 525:
#line 2032 "preproc.y"
    { yyval.str = cat_str(6, make_str("drop cast ("), yyvsp[-4].str, make_str("as"), yyvsp[-2].str, make_str(")"), yyvsp[0].str); }
    break;

  case 526:
#line 2043 "preproc.y"
    { yyval.str = cat_str(4, make_str("reindex"), yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 527:
#line 2045 "preproc.y"
    { yyval.str = cat_str(3, make_str("reindex database"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 528:
#line 2048 "preproc.y"
    { yyval.str = make_str("index"); }
    break;

  case 529:
#line 2049 "preproc.y"
    { yyval.str = make_str("table"); }
    break;

  case 530:
#line 2052 "preproc.y"
    { yyval.str = make_str("force"); }
    break;

  case 531:
#line 2053 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 532:
#line 2065 "preproc.y"
    { yyval.str = cat_str(7, make_str("alter table"), yyvsp[-5].str, make_str("rename"), yyvsp[-3].str, yyvsp[-2].str, make_str("to"), yyvsp[0].str); }
    break;

  case 533:
#line 2067 "preproc.y"
    { yyval.str = cat_str(6, make_str("alter trigger"), yyvsp[-5].str, make_str("on"), yyvsp[-3].str, make_str("rename to"), yyvsp[0].str); }
    break;

  case 534:
#line 2070 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 535:
#line 2071 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 536:
#line 2074 "preproc.y"
    { yyval.str = make_str("column"); }
    break;

  case 537:
#line 2075 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 538:
#line 2086 "preproc.y"
    { QueryIsRule=1; }
    break;

  case 539:
#line 2089 "preproc.y"
    {
			QueryIsRule=0;
			yyval.str = cat_str(12, make_str("create"), yyvsp[-12].str, make_str("rule"), yyvsp[-10].str, make_str("as on"), yyvsp[-6].str, make_str("to"), yyvsp[-4].str, yyvsp[-3].str, make_str("do"), yyvsp[-1].str, yyvsp[0].str);
		}
    break;

  case 540:
#line 2095 "preproc.y"
    { yyval.str = make_str("nothing"); }
    break;

  case 541:
#line 2096 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 542:
#line 2097 "preproc.y"
    { yyval.str = cat_str(3, make_str("("), yyvsp[-1].str, make_str(")")); }
    break;

  case 543:
#line 2102 "preproc.y"
    {  yyval.str = cat_str(3, yyvsp[-2].str, make_str(";"), yyvsp[0].str); }
    break;

  case 544:
#line 2104 "preproc.y"
    { yyval.str = cat2_str(yyvsp[0].str, make_str(";")); }
    break;

  case 550:
#line 2114 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 551:
#line 2115 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 552:
#line 2119 "preproc.y"
    { yyval.str = make_str("select"); }
    break;

  case 553:
#line 2120 "preproc.y"
    { yyval.str = make_str("update"); }
    break;

  case 554:
#line 2121 "preproc.y"
    { yyval.str = make_str("delete"); }
    break;

  case 555:
#line 2122 "preproc.y"
    { yyval.str = make_str("insert"); }
    break;

  case 556:
#line 2125 "preproc.y"
    { yyval.str = make_str("instead"); }
    break;

  case 557:
#line 2126 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 558:
#line 2130 "preproc.y"
    { yyval.str = cat_str(5, make_str("drop rule"), yyvsp[-3].str, make_str("on"), yyvsp[-1].str, yyvsp[0].str);}
    break;

  case 559:
#line 2142 "preproc.y"
    { yyval.str = cat2_str(make_str("notify"), yyvsp[0].str); }
    break;

  case 560:
#line 2146 "preproc.y"
    { yyval.str = cat2_str(make_str("listen"), yyvsp[0].str); }
    break;

  case 561:
#line 2150 "preproc.y"
    { yyval.str = cat2_str(make_str("unlisten"), yyvsp[0].str); }
    break;

  case 562:
#line 2152 "preproc.y"
    { yyval.str = make_str("unlisten *"); }
    break;

  case 563:
#line 2164 "preproc.y"
    { yyval.str = make_str("rollback"); }
    break;

  case 564:
#line 2165 "preproc.y"
    { yyval.str = make_str("begin transaction"); }
    break;

  case 565:
#line 2166 "preproc.y"
    { yyval.str = cat2_str(make_str("start transaction"), yyvsp[0].str); }
    break;

  case 566:
#line 2167 "preproc.y"
    { yyval.str = make_str("commit"); }
    break;

  case 567:
#line 2168 "preproc.y"
    { yyval.str = make_str("commit"); }
    break;

  case 568:
#line 2169 "preproc.y"
    { yyval.str = make_str("rollback"); }
    break;

  case 569:
#line 2172 "preproc.y"
    { yyval.str = cat2_str(make_str("isolation level"), yyvsp[0].str); }
    break;

  case 570:
#line 2175 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 571:
#line 2176 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 572:
#line 2177 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 573:
#line 2188 "preproc.y"
    { yyval.str = cat_str(7, make_str("create"), yyvsp[-5].str, make_str("view"), yyvsp[-3].str, yyvsp[-2].str, make_str("as"), yyvsp[0].str); }
    break;

  case 574:
#line 2200 "preproc.y"
    { yyval.str = cat2_str(make_str("load"), yyvsp[0].str); }
    break;

  case 575:
#line 2212 "preproc.y"
    { yyval.str = cat_str(4, make_str("create database"), yyvsp[-2].str, make_str("with"), yyvsp[0].str); }
    break;

  case 576:
#line 2214 "preproc.y"
    { yyval.str = cat2_str(make_str("create database"), yyvsp[0].str); }
    break;

  case 577:
#line 2218 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 578:
#line 2220 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 579:
#line 2224 "preproc.y"
    { yyval.str = cat_str(3,make_str("location"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 580:
#line 2226 "preproc.y"
    { yyval.str = cat_str(3, make_str("location"), yyvsp[-1].str, make_str("default")); }
    break;

  case 581:
#line 2228 "preproc.y"
    { yyval.str = cat_str(3, make_str("template"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 582:
#line 2230 "preproc.y"
    { yyval.str = cat_str(3, make_str("template"), yyvsp[-1].str, make_str("default")); }
    break;

  case 583:
#line 2232 "preproc.y"
    { yyval.str = cat_str(3, make_str("encoding"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 584:
#line 2234 "preproc.y"
    { yyval.str = cat_str(3, make_str("encoding"), yyvsp[-1].str, make_str("default")); }
    break;

  case 585:
#line 2236 "preproc.y"
    { yyval.str = cat_str(3, make_str("owner"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 586:
#line 2238 "preproc.y"
    { yyval.str = cat_str(3, make_str("owner"), yyvsp[-1].str, make_str("default")); }
    break;

  case 587:
#line 2241 "preproc.y"
    { yyval.str = make_str("="); }
    break;

  case 588:
#line 2242 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 589:
#line 2254 "preproc.y"
    { yyval.str = cat_str(4, make_str("alter database"), yyvsp[-2].str, make_str("set"), yyvsp[0].str); }
    break;

  case 590:
#line 2256 "preproc.y"
    { yyval.str = cat_str(3, make_str("alter database"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 591:
#line 2267 "preproc.y"
    { yyval.str = cat2_str(make_str("drop database"), yyvsp[0].str); }
    break;

  case 592:
#line 2278 "preproc.y"
    {
				yyval.str = cat_str(6, make_str("create domain"), yyvsp[-4].str, yyvsp[-3].str, yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str);
 			}
    break;

  case 593:
#line 2283 "preproc.y"
    {yyval.str = make_str("as"); }
    break;

  case 594:
#line 2284 "preproc.y"
    {yyval.str = EMPTY; }
    break;

  case 595:
#line 2290 "preproc.y"
    { yyval.str = cat_str(10, make_str("create"), yyvsp[-8].str, make_str("conversion"), yyvsp[-6].str, make_str("for"), yyvsp[-4].str, make_str("to"), yyvsp[-2].str, make_str("from"), yyvsp[0].str); }
    break;

  case 596:
#line 2301 "preproc.y"
    { yyval.str = cat_str(4, make_str("cluster"), yyvsp[-2].str, make_str("on"), yyvsp[0].str); }
    break;

  case 597:
#line 2314 "preproc.y"
    { yyval.str = cat_str(4, make_str("vacuum"), yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 598:
#line 2316 "preproc.y"
    { yyval.str = cat_str(5, make_str("vacuum"), yyvsp[-3].str, yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 599:
#line 2318 "preproc.y"
    { yyval.str = cat_str(5, make_str("vacuum"), yyvsp[-3].str, yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 600:
#line 2322 "preproc.y"
    { yyval.str = cat_str(2, yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 601:
#line 2324 "preproc.y"
    { yyval.str = cat_str(4, yyvsp[-3].str, yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 602:
#line 2327 "preproc.y"
    { yyval.str = make_str("analyze"); }
    break;

  case 603:
#line 2328 "preproc.y"
    { yyval.str = make_str("analyse"); }
    break;

  case 604:
#line 2331 "preproc.y"
    { yyval.str = make_str("verbose"); }
    break;

  case 605:
#line 2332 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 606:
#line 2335 "preproc.y"
    { yyval.str = make_str("full"); }
    break;

  case 607:
#line 2336 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 608:
#line 2339 "preproc.y"
    { yyval.str = make_str("freeze"); }
    break;

  case 609:
#line 2340 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 610:
#line 2344 "preproc.y"
    { yyval.str = cat_str(3, make_str("("), yyvsp[-1].str, make_str(")")); }
    break;

  case 611:
#line 2346 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 612:
#line 2358 "preproc.y"
    { yyval.str = cat_str(3, make_str("explain"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 613:
#line 2360 "preproc.y"
    { yyval.str = cat_str(4, make_str("explain"), yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 619:
#line 2423 "preproc.y"
    { yyval.str = cat_str(3, make_str("insert into"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 620:
#line 2427 "preproc.y"
    { yyval.str = cat_str(3, make_str("values("), yyvsp[-1].str, make_str(")")); }
    break;

  case 621:
#line 2429 "preproc.y"
    { yyval.str = make_str("default values"); }
    break;

  case 622:
#line 2431 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 623:
#line 2433 "preproc.y"
    { yyval.str = cat_str(5, make_str("("), yyvsp[-5].str, make_str(") values ("), yyvsp[-1].str, make_str(")")); }
    break;

  case 624:
#line 2435 "preproc.y"
    { yyval.str = cat_str(4, make_str("("), yyvsp[-2].str, make_str(")"), yyvsp[0].str); }
    break;

  case 625:
#line 2439 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str(","), yyvsp[0].str); }
    break;

  case 626:
#line 2441 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 627:
#line 2445 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 628:
#line 2457 "preproc.y"
    { yyval.str = cat_str(3, make_str("delete from"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 629:
#line 2461 "preproc.y"
    { yyval.str = cat_str(4, make_str("lock"), yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 630:
#line 2465 "preproc.y"
    { yyval.str = cat_str(3, make_str("in"), yyvsp[-1].str, make_str("mode")); }
    break;

  case 631:
#line 2467 "preproc.y"
    { yyval.str = EMPTY;}
    break;

  case 632:
#line 2470 "preproc.y"
    { yyval.str = make_str("access share"); }
    break;

  case 633:
#line 2471 "preproc.y"
    { yyval.str = make_str("access share"); }
    break;

  case 634:
#line 2472 "preproc.y"
    { yyval.str = make_str("row exclusive"); }
    break;

  case 635:
#line 2473 "preproc.y"
    { yyval.str = make_str("share update exclusive"); }
    break;

  case 636:
#line 2474 "preproc.y"
    { yyval.str = make_str("share"); }
    break;

  case 637:
#line 2475 "preproc.y"
    { yyval.str = make_str("share row exclusive"); }
    break;

  case 638:
#line 2476 "preproc.y"
    { yyval.str = make_str("exclusive"); }
    break;

  case 639:
#line 2477 "preproc.y"
    { yyval.str = make_str("access exclusive"); }
    break;

  case 640:
#line 2491 "preproc.y"
    {yyval.str = cat_str(6, make_str("update"), yyvsp[-4].str, make_str("set"), yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 641:
#line 2502 "preproc.y"
    {
			struct cursor *ptr, *this;

			for (ptr = cur; ptr != NULL; ptr = ptr->next)
			{
				if (strcmp(yyvsp[-4].str, ptr->name) == 0)
				{
						/* re-definition is a bug */
					snprintf(errortext, sizeof(errortext), "cursor %s already defined", yyvsp[-4].str);
					mmerror(PARSE_ERROR, ET_ERROR, errortext);
				}
			}

			this = (struct cursor *) mm_alloc(sizeof(struct cursor));

			/* initial definition */
			this->next = cur;
			this->name = yyvsp[-4].str;
			this->connection = connection;
			this->command =  cat_str(5, make_str("declare"), mm_strdup(yyvsp[-4].str), yyvsp[-3].str, make_str("cursor for"), yyvsp[0].str);
			this->argsinsert = argsinsert;
			this->argsresult = argsresult;
			argsinsert = argsresult = NULL;

			cur = this;

			yyval.str = cat_str(3, make_str("/*"), mm_strdup(this->command), make_str("*/"));
		}
    break;

  case 642:
#line 2532 "preproc.y"
    { yyval.str = make_str("binary"); }
    break;

  case 643:
#line 2533 "preproc.y"
    { yyval.str = make_str("insensitive"); }
    break;

  case 644:
#line 2534 "preproc.y"
    { yyval.str = make_str("scroll"); }
    break;

  case 645:
#line 2535 "preproc.y"
    { yyval.str = make_str("insensitive scroll"); }
    break;

  case 646:
#line 2536 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 647:
#line 2547 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 648:
#line 2549 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 649:
#line 2553 "preproc.y"
    { yyval.str = cat_str(3, make_str("("), yyvsp[-1].str, make_str(")")); }
    break;

  case 650:
#line 2555 "preproc.y"
    { yyval.str = cat_str(3, make_str("("), yyvsp[-1].str, make_str(")")); }
    break;

  case 651:
#line 2559 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 652:
#line 2561 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 653:
#line 2563 "preproc.y"
    { yyval.str = cat_str(4, yyvsp[-3].str, yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 654:
#line 2565 "preproc.y"
    { yyval.str = cat_str(4, yyvsp[-3].str, yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 655:
#line 2568 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 656:
#line 2569 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 657:
#line 2575 "preproc.y"
    { yyval.str = cat_str(8, make_str("select"), yyvsp[-6].str, yyvsp[-5].str, yyvsp[-4].str, yyvsp[-3].str, yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 658:
#line 2577 "preproc.y"
    { yyval.str = cat_str(4, yyvsp[-3].str, make_str("union"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 659:
#line 2579 "preproc.y"
    { yyval.str = cat_str(4, yyvsp[-3].str, make_str("intersect"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 660:
#line 2581 "preproc.y"
    { yyval.str = cat_str(4, yyvsp[-3].str, make_str("except"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 661:
#line 2585 "preproc.y"
    {
			FoundInto = 1;
			yyval.str= cat2_str(make_str("into"), yyvsp[0].str);
		}
    break;

  case 662:
#line 2589 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 663:
#line 2590 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 664:
#line 2601 "preproc.y"
    { yyval.str = cat_str(3, make_str("temporary"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 665:
#line 2603 "preproc.y"
    { yyval.str = cat_str(3, make_str("temp"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 666:
#line 2605 "preproc.y"
    { yyval.str = cat_str(3, make_str("local temporary"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 667:
#line 2607 "preproc.y"
    { yyval.str = cat_str(3, make_str("local temp"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 668:
#line 2609 "preproc.y"
    {
			mmerror(PARSE_ERROR, ET_WARNING, "Currently unsupported CREATE TABLE / GLOBAL TEMPORARY will be passed to backend");
			yyval.str = cat_str(3, make_str("global temporary"), yyvsp[-1].str, yyvsp[0].str);
		}
    break;

  case 669:
#line 2614 "preproc.y"
    {
			mmerror(PARSE_ERROR, ET_WARNING, "Currently unsupported CREATE TABLE / GLOBAL TEMP will be passed to backend");
			yyval.str = cat_str(3, make_str("global temp"), yyvsp[-1].str, yyvsp[0].str);
		}
    break;

  case 670:
#line 2619 "preproc.y"
    { yyval.str = cat2_str(make_str("table"), yyvsp[0].str); }
    break;

  case 671:
#line 2621 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 672:
#line 2624 "preproc.y"
    { yyval.str = make_str("table"); }
    break;

  case 673:
#line 2625 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 674:
#line 2628 "preproc.y"
    { yyval.str = make_str("all"); }
    break;

  case 675:
#line 2629 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 676:
#line 2633 "preproc.y"
    { yyval.str = make_str("distinct"); }
    break;

  case 677:
#line 2635 "preproc.y"
    { yyval.str = cat_str(3, make_str("distinct on ("), yyvsp[-1].str, make_str(")")); }
    break;

  case 678:
#line 2637 "preproc.y"
    { yyval.str = make_str("all"); }
    break;

  case 679:
#line 2639 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 680:
#line 2642 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 681:
#line 2643 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 682:
#line 2647 "preproc.y"
    { yyval.str = cat2_str(make_str("order by"), yyvsp[0].str); }
    break;

  case 683:
#line 2650 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 684:
#line 2651 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str(","), yyvsp[0].str); }
    break;

  case 685:
#line 2655 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 686:
#line 2658 "preproc.y"
    { yyval.str = cat2_str(make_str("using"), yyvsp[0].str); }
    break;

  case 687:
#line 2659 "preproc.y"
    { yyval.str = make_str("asc"); }
    break;

  case 688:
#line 2660 "preproc.y"
    { yyval.str = make_str("desc"); }
    break;

  case 689:
#line 2661 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 690:
#line 2665 "preproc.y"
    { yyval.str = cat_str(4, make_str("limit"), yyvsp[-2].str, make_str("offset"), yyvsp[0].str); }
    break;

  case 691:
#line 2667 "preproc.y"
    { yyval.str = cat_str(4, make_str("offset"), yyvsp[-2].str, make_str("limit"), yyvsp[0].str); }
    break;

  case 692:
#line 2669 "preproc.y"
    { yyval.str = cat2_str(make_str("limit"), yyvsp[0].str); }
    break;

  case 693:
#line 2671 "preproc.y"
    { yyval.str = cat2_str(make_str("offset"), yyvsp[0].str); }
    break;

  case 694:
#line 2673 "preproc.y"
    { mmerror(PARSE_ERROR, ET_WARNING, "No longer supported LIMIT #,# syntax passed to backend."); }
    break;

  case 695:
#line 2676 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 696:
#line 2677 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 697:
#line 2681 "preproc.y"
    {
			if (atoi(yyvsp[0].str) < 0)
				mmerror(PARSE_ERROR, ET_ERROR, "LIMIT must not be negative");
			yyval.str = yyvsp[0].str;
		}
    break;

  case 698:
#line 2686 "preproc.y"
    { yyval.str = make_str("all"); }
    break;

  case 699:
#line 2687 "preproc.y"
    { yyval.str = make_name(); }
    break;

  case 700:
#line 2691 "preproc.y"
    {
			if (atoi(yyvsp[0].str) < 0)
				mmerror(PARSE_ERROR, ET_ERROR, "OFFSET must not be negative");
			yyval.str = yyvsp[0].str;
		}
    break;

  case 701:
#line 2696 "preproc.y"
    { yyval.str = make_name(); }
    break;

  case 702:
#line 2707 "preproc.y"
    { yyval.str = cat2_str(make_str("group by"), yyvsp[0].str); }
    break;

  case 703:
#line 2709 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 704:
#line 2713 "preproc.y"
    { yyval.str = cat2_str(make_str("having"), yyvsp[0].str); }
    break;

  case 705:
#line 2715 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 706:
#line 2719 "preproc.y"
    { yyval.str = make_str("for update"); }
    break;

  case 707:
#line 2721 "preproc.y"
    { yyval.str = make_str("for read only"); }
    break;

  case 708:
#line 2724 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 709:
#line 2725 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 710:
#line 2728 "preproc.y"
    { yyval.str = cat2_str(make_str("of"), yyvsp[0].str); }
    break;

  case 711:
#line 2729 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 712:
#line 2740 "preproc.y"
    { yyval.str = cat2_str(make_str("from"), yyvsp[0].str); }
    break;

  case 713:
#line 2741 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 714:
#line 2744 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str(","), yyvsp[0].str); }
    break;

  case 715:
#line 2745 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 716:
#line 2756 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 717:
#line 2758 "preproc.y"
    { yyval.str= cat2_str(yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 718:
#line 2760 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 719:
#line 2762 "preproc.y"
    { yyval.str= cat2_str(yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 720:
#line 2764 "preproc.y"
    { yyval.str=cat_str(4, yyvsp[-4].str, make_str("as ("), yyvsp[-1].str, make_str(")")); }
    break;

  case 721:
#line 2766 "preproc.y"
    { yyval.str=cat_str(6, yyvsp[-5].str, make_str("as"), yyvsp[-3].str, make_str("("), yyvsp[-1].str, make_str(")")); }
    break;

  case 722:
#line 2768 "preproc.y"
    { yyval.str=cat_str(5, yyvsp[-4].str, yyvsp[-3].str, make_str("("), yyvsp[-1].str, make_str(")")); }
    break;

  case 723:
#line 2770 "preproc.y"
    {mmerror(PARSE_ERROR, ET_ERROR, "sub-SELECT in FROM must have an alias");}
    break;

  case 724:
#line 2772 "preproc.y"
    { yyval.str=cat2_str(yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 725:
#line 2774 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 726:
#line 2776 "preproc.y"
    { yyval.str=cat_str(4, make_str("("), yyvsp[-2].str, make_str(")"), yyvsp[0].str); }
    break;

  case 727:
#line 2797 "preproc.y"
    { yyval.str = cat_str(3, make_str("("), yyvsp[-1].str, make_str(")")); }
    break;

  case 728:
#line 2799 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-3].str, make_str("cross join"), yyvsp[0].str); }
    break;

  case 729:
#line 2801 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("unionjoin"), yyvsp[0].str); }
    break;

  case 730:
#line 2803 "preproc.y"
    { yyval.str = cat_str(5, yyvsp[-4].str, yyvsp[-3].str, make_str("join"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 731:
#line 2805 "preproc.y"
    { yyval.str = cat_str(4, yyvsp[-3].str, make_str("join"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 732:
#line 2807 "preproc.y"
    { yyval.str = cat_str(5, yyvsp[-4].str, make_str("natural"), yyvsp[-2].str, make_str("join"), yyvsp[0].str); }
    break;

  case 733:
#line 2809 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-3].str, make_str("natural join"), yyvsp[0].str); }
    break;

  case 734:
#line 2813 "preproc.y"
    { yyval.str = cat_str(5, make_str("as"), yyvsp[-3].str, make_str("("), yyvsp[-1].str, make_str(")")); }
    break;

  case 735:
#line 2815 "preproc.y"
    { yyval.str = cat2_str(make_str("as"), yyvsp[0].str); }
    break;

  case 736:
#line 2817 "preproc.y"
    { yyval.str = cat_str(4, yyvsp[-3].str, make_str("("), yyvsp[-1].str, make_str(")")); }
    break;

  case 737:
#line 2819 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 738:
#line 2822 "preproc.y"
    { yyval.str = cat2_str(make_str("full"), yyvsp[0].str); }
    break;

  case 739:
#line 2823 "preproc.y"
    { yyval.str = cat2_str(make_str("left"), yyvsp[0].str); }
    break;

  case 740:
#line 2824 "preproc.y"
    { yyval.str = cat2_str(make_str("right"), yyvsp[0].str); }
    break;

  case 741:
#line 2825 "preproc.y"
    { yyval.str = make_str("inner"); }
    break;

  case 742:
#line 2829 "preproc.y"
    { yyval.str = make_str("outer"); }
    break;

  case 743:
#line 2830 "preproc.y"
    { yyval.str = EMPTY;  /* no qualifiers */ }
    break;

  case 744:
#line 2841 "preproc.y"
    { yyval.str = cat_str(3, make_str("using ("), yyvsp[-1].str, make_str(")")); }
    break;

  case 745:
#line 2843 "preproc.y"
    { yyval.str = cat2_str(make_str("on"), yyvsp[0].str); }
    break;

  case 746:
#line 2847 "preproc.y"
    { /* normal relations */ yyval.str = yyvsp[0].str; }
    break;

  case 747:
#line 2849 "preproc.y"
    { /* inheritance query */ yyval.str = cat2_str(yyvsp[-1].str, make_str("*")); }
    break;

  case 748:
#line 2851 "preproc.y"
    { /* inheritance query */ yyval.str = cat2_str(make_str("only "), yyvsp[0].str); }
    break;

  case 749:
#line 2853 "preproc.y"
    { /* inheritance query */ yyval.str = cat_str(3, make_str("only ("), yyvsp[-1].str, make_str(")")); }
    break;

  case 750:
#line 2857 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-2].str, make_str("()")); }
    break;

  case 751:
#line 2859 "preproc.y"
    { yyval.str = cat_str(4, yyvsp[-3].str, make_str("("), yyvsp[-1].str, make_str(")")); }
    break;

  case 752:
#line 2862 "preproc.y"
    { yyval.str = cat2_str(make_str("where"), yyvsp[0].str); }
    break;

  case 753:
#line 2863 "preproc.y"
    { yyval.str = EMPTY;  /* no qualifiers */ }
    break;

  case 754:
#line 2867 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 755:
#line 2869 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, ',', yyvsp[0].str); }
    break;

  case 756:
#line 2872 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 757:
#line 2886 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, yyvsp[0].index.str); }
    break;

  case 758:
#line 2888 "preproc.y"
    { yyval.str = cat2_str(make_str("setof"), yyvsp[0].str); }
    break;

  case 759:
#line 2893 "preproc.y"
    {
			yyval.index.index1 = 0;
			yyval.index.index2 = yyvsp[0].index.index1;
			yyval.index.str = cat2_str(make_str("[]"), yyvsp[0].index.str);
		}
    break;

  case 760:
#line 2899 "preproc.y"
    {
			char *txt = mm_alloc(20L);

			sprintf (txt, "%d", yyvsp[-2].ival);
			yyval.index.index1 = yyvsp[-2].ival;
			yyval.index.index2 = yyvsp[0].index.index1;
			yyval.index.str = cat_str(4, make_str("["), txt, make_str("]"), yyvsp[0].index.str);
		}
    break;

  case 761:
#line 2908 "preproc.y"
    {
			yyval.index.index1 = -1;
			yyval.index.index2 = -1;
			yyval.index.str= EMPTY;
		}
    break;

  case 762:
#line 2915 "preproc.y"
    { yyval.ival = atol(yyvsp[0].str); }
    break;

  case 763:
#line 2916 "preproc.y"
    { yyval.ival = yyvsp[-1].ival; }
    break;

  case 764:
#line 2917 "preproc.y"
    { yyval.ival = yyvsp[-2].ival + yyvsp[0].ival; }
    break;

  case 765:
#line 2918 "preproc.y"
    { yyval.ival = yyvsp[-2].ival - yyvsp[0].ival; }
    break;

  case 766:
#line 2919 "preproc.y"
    { yyval.ival = yyvsp[-2].ival * yyvsp[0].ival; }
    break;

  case 767:
#line 2920 "preproc.y"
    { yyval.ival = yyvsp[-2].ival / yyvsp[0].ival; }
    break;

  case 768:
#line 2921 "preproc.y"
    { yyval.ival = yyvsp[-2].ival % yyvsp[0].ival; }
    break;

  case 769:
#line 2924 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 770:
#line 2925 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 771:
#line 2926 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 772:
#line 2927 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 773:
#line 2928 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 774:
#line 2930 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 775:
#line 2932 "preproc.y"
    { yyval.str = cat_str(5, yyvsp[-4].str, make_str("("), yyvsp[-2].str, make_str(")"), yyvsp[0].str); }
    break;

  case 776:
#line 2934 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, yyvsp[0].str);}
    break;

  case 777:
#line 2937 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 778:
#line 2938 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 779:
#line 2939 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 780:
#line 2940 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 781:
#line 2941 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 782:
#line 2944 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 783:
#line 2953 "preproc.y"
    { yyval.str = make_str("int"); }
    break;

  case 784:
#line 2955 "preproc.y"
    { yyval.str = make_str("integer"); }
    break;

  case 785:
#line 2957 "preproc.y"
    { yyval.str = make_str("smallint"); }
    break;

  case 786:
#line 2959 "preproc.y"
    { yyval.str = make_str("bigint"); }
    break;

  case 787:
#line 2961 "preproc.y"
    { yyval.str = make_str("real"); }
    break;

  case 788:
#line 2963 "preproc.y"
    { yyval.str = cat2_str(make_str("float"), yyvsp[0].str); }
    break;

  case 789:
#line 2965 "preproc.y"
    { yyval.str = make_str("double precision"); }
    break;

  case 790:
#line 2967 "preproc.y"
    { yyval.str = cat2_str(make_str("decimal"), yyvsp[0].str); }
    break;

  case 791:
#line 2969 "preproc.y"
    { yyval.str = cat2_str(make_str("dec"), yyvsp[0].str); }
    break;

  case 792:
#line 2971 "preproc.y"
    { yyval.str = cat2_str(make_str("numeric"), yyvsp[0].str); }
    break;

  case 793:
#line 2973 "preproc.y"
    { yyval.str = make_str("boolean"); }
    break;

  case 794:
#line 2977 "preproc.y"
    { yyval.str = cat_str(3, make_str("("), yyvsp[-1].str, make_str(")")); }
    break;

  case 795:
#line 2979 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 796:
#line 2983 "preproc.y"
    { yyval.str = cat_str(5, make_str("("), yyvsp[-3].str, make_str(","), yyvsp[-1].str, make_str(")")); }
    break;

  case 797:
#line 2985 "preproc.y"
    { yyval.str = cat_str(3, make_str("("), yyvsp[-1].str, make_str(")")); }
    break;

  case 798:
#line 2987 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 799:
#line 2991 "preproc.y"
    { yyval.str = cat_str(5, make_str("("), yyvsp[-3].str, make_str(","), yyvsp[-1].str, make_str(")")); }
    break;

  case 800:
#line 2993 "preproc.y"
    { yyval.str = cat_str(3, make_str("("), yyvsp[-1].str, make_str(")")); }
    break;

  case 801:
#line 2995 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 802:
#line 3003 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 803:
#line 3004 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 804:
#line 3007 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 805:
#line 3008 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 806:
#line 3012 "preproc.y"
    { yyval.str = cat_str(5, make_str("bit"), yyvsp[-3].str, make_str("("), yyvsp[-1].str, make_str(")")); }
    break;

  case 807:
#line 3016 "preproc.y"
    { yyval.str = cat2_str(make_str("bit"), yyvsp[0].str); }
    break;

  case 808:
#line 3024 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 809:
#line 3025 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 810:
#line 3028 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 811:
#line 3029 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 812:
#line 3033 "preproc.y"
    { yyval.str = cat_str(5, yyvsp[-4].str, make_str("("), yyvsp[-2].str, make_str(")"), yyvsp[0].str); }
    break;

  case 813:
#line 3037 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 814:
#line 3041 "preproc.y"
    { yyval.str = cat2_str(make_str("character"), yyvsp[0].str); }
    break;

  case 815:
#line 3043 "preproc.y"
    { yyval.str = cat2_str(make_str("char"), yyvsp[0].str); }
    break;

  case 816:
#line 3045 "preproc.y"
    { yyval.str = make_str("varchar"); }
    break;

  case 817:
#line 3047 "preproc.y"
    { yyval.str = cat2_str(make_str("national character"), yyvsp[0].str); }
    break;

  case 818:
#line 3049 "preproc.y"
    { yyval.str = cat2_str(make_str("national char"), yyvsp[0].str); }
    break;

  case 819:
#line 3051 "preproc.y"
    { yyval.str = cat2_str(make_str("nchar"), yyvsp[0].str); }
    break;

  case 820:
#line 3055 "preproc.y"
    { yyval.str = make_str("varying"); }
    break;

  case 821:
#line 3057 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 822:
#line 3061 "preproc.y"
    { yyval.str = cat2_str(make_str("character set"), yyvsp[0].str); }
    break;

  case 823:
#line 3063 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 824:
#line 3067 "preproc.y"
    { yyval.str = cat2_str(make_str("collate"), yyvsp[0].str); }
    break;

  case 825:
#line 3069 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 826:
#line 3073 "preproc.y"
    { yyval.str = cat_str(4, make_str("timestamp("), yyvsp[-2].str, make_str(")"), yyvsp[0].str); }
    break;

  case 827:
#line 3075 "preproc.y"
    { yyval.str = cat2_str(make_str("timestamp"), yyvsp[0].str); }
    break;

  case 828:
#line 3077 "preproc.y"
    { yyval.str = cat_str(4, make_str("time("), yyvsp[-2].str, make_str(")"), yyvsp[0].str); }
    break;

  case 829:
#line 3079 "preproc.y"
    { yyval.str = cat2_str(make_str("time"), yyvsp[0].str); }
    break;

  case 830:
#line 3083 "preproc.y"
    { yyval.str = make_str("interval"); }
    break;

  case 831:
#line 3087 "preproc.y"
    { yyval.str = make_str("with time zone"); }
    break;

  case 832:
#line 3089 "preproc.y"
    { yyval.str = make_str("without time zone"); }
    break;

  case 833:
#line 3091 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 834:
#line 3094 "preproc.y"
    { yyval.str = make_str("year"); }
    break;

  case 835:
#line 3095 "preproc.y"
    { yyval.str = make_str("month"); }
    break;

  case 836:
#line 3096 "preproc.y"
    { yyval.str = make_str("day"); }
    break;

  case 837:
#line 3097 "preproc.y"
    { yyval.str = make_str("hour"); }
    break;

  case 838:
#line 3098 "preproc.y"
    { yyval.str = make_str("minute"); }
    break;

  case 839:
#line 3099 "preproc.y"
    { yyval.str = make_str("second"); }
    break;

  case 840:
#line 3100 "preproc.y"
    { yyval.str = make_str("year to month"); }
    break;

  case 841:
#line 3101 "preproc.y"
    { yyval.str = make_str("day to hour"); }
    break;

  case 842:
#line 3102 "preproc.y"
    { yyval.str = make_str("day to minute"); }
    break;

  case 843:
#line 3103 "preproc.y"
    { yyval.str = make_str("day to second"); }
    break;

  case 844:
#line 3104 "preproc.y"
    { yyval.str = make_str("hour to minute"); }
    break;

  case 845:
#line 3105 "preproc.y"
    { yyval.str = make_str("minute to second"); }
    break;

  case 846:
#line 3106 "preproc.y"
    { yyval.str = make_str("hour to second"); }
    break;

  case 847:
#line 3107 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 848:
#line 3122 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("in"), yyvsp[0].str); }
    break;

  case 849:
#line 3124 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-3].str, make_str("not in"), yyvsp[0].str); }
    break;

  case 850:
#line 3126 "preproc.y"
    { yyval.str = cat_str(4, yyvsp[-3].str, yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 851:
#line 3128 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 852:
#line 3130 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 853:
#line 3132 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-2].str, make_str("is null")); }
    break;

  case 854:
#line 3134 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-3].str, make_str("is not null")); }
    break;

  case 855:
#line 3136 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("overlaps"), yyvsp[0].str); }
    break;

  case 856:
#line 3138 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-4].str, make_str("is distinct from"), yyvsp[0].str); }
    break;

  case 857:
#line 3142 "preproc.y"
    { yyval.str = cat_str(3, make_str("row ("), yyvsp[-1].str, make_str(")")); }
    break;

  case 858:
#line 3144 "preproc.y"
    { yyval.str = cat_str(3, make_str("row ("), yyvsp[-1].str, make_str(")")); }
    break;

  case 859:
#line 3146 "preproc.y"
    { yyval.str = make_str("row()"); }
    break;

  case 860:
#line 3148 "preproc.y"
    { yyval.str = cat_str(3, make_str("("), yyvsp[-1].str, make_str(")")); }
    break;

  case 861:
#line 3152 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str(","), yyvsp[0].str); }
    break;

  case 862:
#line 3156 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str(","), yyvsp[0].str); }
    break;

  case 863:
#line 3158 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 864:
#line 3161 "preproc.y"
    { yyval.str = make_str("ANY"); }
    break;

  case 865:
#line 3162 "preproc.y"
    { yyval.str = make_str("SOME"); }
    break;

  case 866:
#line 3163 "preproc.y"
    { yyval.str = make_str("ALL"); }
    break;

  case 867:
#line 3166 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 868:
#line 3167 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 869:
#line 3170 "preproc.y"
    { yyval.str = make_str("+"); }
    break;

  case 870:
#line 3171 "preproc.y"
    { yyval.str = make_str("-"); }
    break;

  case 871:
#line 3172 "preproc.y"
    { yyval.str = make_str("*"); }
    break;

  case 872:
#line 3173 "preproc.y"
    { yyval.str = make_str("%"); }
    break;

  case 873:
#line 3174 "preproc.y"
    { yyval.str = make_str("^"); }
    break;

  case 874:
#line 3175 "preproc.y"
    { yyval.str = make_str("/"); }
    break;

  case 875:
#line 3176 "preproc.y"
    { yyval.str = make_str("<"); }
    break;

  case 876:
#line 3177 "preproc.y"
    { yyval.str = make_str(">"); }
    break;

  case 877:
#line 3178 "preproc.y"
    { yyval.str = make_str("="); }
    break;

  case 878:
#line 3181 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 879:
#line 3182 "preproc.y"
    { yyval.str = cat_str(3, make_str("operator ("), yyvsp[-1].str, make_str(")")); }
    break;

  case 880:
#line 3185 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 881:
#line 3186 "preproc.y"
    { yyval.str = cat_str(3, make_str("operator ("), yyvsp[-1].str, make_str(")")); }
    break;

  case 882:
#line 3206 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 883:
#line 3208 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("::"), yyvsp[0].str); }
    break;

  case 884:
#line 3210 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-4].str, make_str("at time zone"), yyvsp[0].str); }
    break;

  case 885:
#line 3221 "preproc.y"
    { yyval.str = cat2_str(make_str("+"), yyvsp[0].str); }
    break;

  case 886:
#line 3223 "preproc.y"
    { yyval.str = cat2_str(make_str("-"), yyvsp[0].str); }
    break;

  case 887:
#line 3225 "preproc.y"
    { yyval.str = cat2_str(make_str("%"), yyvsp[0].str); }
    break;

  case 888:
#line 3227 "preproc.y"
    { yyval.str = cat2_str(make_str("^"), yyvsp[0].str); }
    break;

  case 889:
#line 3229 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, make_str("%")); }
    break;

  case 890:
#line 3231 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, make_str("^")); }
    break;

  case 891:
#line 3233 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("+"), yyvsp[0].str); }
    break;

  case 892:
#line 3235 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("-"), yyvsp[0].str); }
    break;

  case 893:
#line 3237 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("*"), yyvsp[0].str); }
    break;

  case 894:
#line 3239 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("/"), yyvsp[0].str); }
    break;

  case 895:
#line 3241 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("%"), yyvsp[0].str); }
    break;

  case 896:
#line 3243 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("^"), yyvsp[0].str); }
    break;

  case 897:
#line 3245 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("<"), yyvsp[0].str); }
    break;

  case 898:
#line 3247 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str(">"), yyvsp[0].str); }
    break;

  case 899:
#line 3249 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("="), yyvsp[0].str); }
    break;

  case 900:
#line 3251 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 901:
#line 3253 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 902:
#line 3255 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 903:
#line 3257 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("and"), yyvsp[0].str); }
    break;

  case 904:
#line 3259 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("or"), yyvsp[0].str); }
    break;

  case 905:
#line 3261 "preproc.y"
    { yyval.str = cat2_str(make_str("not"), yyvsp[0].str); }
    break;

  case 906:
#line 3263 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("like"), yyvsp[0].str); }
    break;

  case 907:
#line 3265 "preproc.y"
    { yyval.str = cat_str(5, yyvsp[-4].str, make_str("like"), yyvsp[-2].str, make_str("escape"), yyvsp[0].str); }
    break;

  case 908:
#line 3267 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-3].str, make_str("not like"), yyvsp[0].str); }
    break;

  case 909:
#line 3269 "preproc.y"
    { yyval.str = cat_str(5, yyvsp[-5].str, make_str("not like"), yyvsp[-2].str, make_str("escape"), yyvsp[0].str); }
    break;

  case 910:
#line 3271 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("ilike"), yyvsp[0].str); }
    break;

  case 911:
#line 3273 "preproc.y"
    { yyval.str = cat_str(5, yyvsp[-4].str, make_str("ilike"), yyvsp[-2].str, make_str("escape"), yyvsp[0].str); }
    break;

  case 912:
#line 3275 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-3].str, make_str("not ilike"), yyvsp[0].str); }
    break;

  case 913:
#line 3277 "preproc.y"
    { yyval.str = cat_str(5, yyvsp[-5].str, make_str("not ilike"), yyvsp[-2].str, make_str("escape"), yyvsp[0].str); }
    break;

  case 914:
#line 3279 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-3].str, make_str("similar to"), yyvsp[0].str); }
    break;

  case 915:
#line 3281 "preproc.y"
    { yyval.str = cat_str(5, yyvsp[-5].str, make_str("similar to"), yyvsp[-2].str, make_str("escape"), yyvsp[0].str); }
    break;

  case 916:
#line 3283 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-4].str, make_str("not similar to"), yyvsp[0].str); }
    break;

  case 917:
#line 3285 "preproc.y"
    { yyval.str = cat_str(5, yyvsp[-6].str, make_str("not similar to"), yyvsp[-2].str, make_str("escape"), yyvsp[0].str); }
    break;

  case 918:
#line 3287 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, make_str("isnull")); }
    break;

  case 919:
#line 3289 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-2].str, make_str("is null")); }
    break;

  case 920:
#line 3291 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, make_str("notnull")); }
    break;

  case 921:
#line 3293 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-3].str, make_str("is not null")); }
    break;

  case 922:
#line 3304 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-2].str, make_str("is true")); }
    break;

  case 923:
#line 3306 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-3].str, make_str("is not true")); }
    break;

  case 924:
#line 3308 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-2].str, make_str("is false")); }
    break;

  case 925:
#line 3310 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-3].str, make_str("is not false")); }
    break;

  case 926:
#line 3312 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-2].str, make_str("is unknown")); }
    break;

  case 927:
#line 3314 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-3].str, make_str("is not unknown")); }
    break;

  case 928:
#line 3316 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-4].str, make_str("is distinct from"), yyvsp[0].str); }
    break;

  case 929:
#line 3318 "preproc.y"
    { yyval.str = cat_str(4, yyvsp[-5].str, make_str("is of ("), yyvsp[-1].str, make_str(")")); }
    break;

  case 930:
#line 3320 "preproc.y"
    { yyval.str = cat_str(4, yyvsp[-6].str, make_str("is not of ("), yyvsp[-1].str, make_str(")")); }
    break;

  case 931:
#line 3322 "preproc.y"
    { yyval.str = cat_str(5, yyvsp[-4].str, make_str("between"), yyvsp[-2].str, make_str("and"), yyvsp[0].str); }
    break;

  case 932:
#line 3324 "preproc.y"
    { yyval.str = cat_str(5, yyvsp[-5].str, make_str("not between"), yyvsp[-2].str, make_str("and"), yyvsp[0].str); }
    break;

  case 933:
#line 3326 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("in"), yyvsp[0].str); }
    break;

  case 934:
#line 3328 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-3].str, make_str("not in"), yyvsp[0].str); }
    break;

  case 935:
#line 3330 "preproc.y"
    { yyval.str = cat_str(4, yyvsp[-3].str, yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 936:
#line 3332 "preproc.y"
    { yyval.str = cat2_str(make_str("unique"), yyvsp[0].str); }
    break;

  case 937:
#line 3334 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 938:
#line 3346 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 939:
#line 3348 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("::"), yyvsp[0].str); }
    break;

  case 940:
#line 3350 "preproc.y"
    { yyval.str = cat2_str(make_str("-"), yyvsp[0].str); }
    break;

  case 941:
#line 3352 "preproc.y"
    { yyval.str = cat2_str(make_str("%"), yyvsp[0].str); }
    break;

  case 942:
#line 3354 "preproc.y"
    { yyval.str = cat2_str(make_str("^"), yyvsp[0].str); }
    break;

  case 943:
#line 3356 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, make_str("%")); }
    break;

  case 944:
#line 3358 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, make_str("^")); }
    break;

  case 945:
#line 3360 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("+"), yyvsp[0].str); }
    break;

  case 946:
#line 3362 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("-"), yyvsp[0].str); }
    break;

  case 947:
#line 3364 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("*"), yyvsp[0].str); }
    break;

  case 948:
#line 3366 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("/"), yyvsp[0].str); }
    break;

  case 949:
#line 3368 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("%"), yyvsp[0].str); }
    break;

  case 950:
#line 3370 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("^"), yyvsp[0].str); }
    break;

  case 951:
#line 3372 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("<"), yyvsp[0].str); }
    break;

  case 952:
#line 3374 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str(">"), yyvsp[0].str); }
    break;

  case 953:
#line 3376 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("="), yyvsp[0].str); }
    break;

  case 954:
#line 3378 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 955:
#line 3380 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 956:
#line 3382 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 957:
#line 3384 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-4].str, make_str("is distinct from"), yyvsp[0].str); }
    break;

  case 958:
#line 3386 "preproc.y"
    { yyval.str = cat_str(4, yyvsp[-5].str, make_str("is of ("), yyvsp[-1].str, make_str(")")); }
    break;

  case 959:
#line 3388 "preproc.y"
    { yyval.str = cat_str(4, yyvsp[-6].str, make_str("is not of ("), yyvsp[-1].str, make_str(")")); }
    break;

  case 960:
#line 3400 "preproc.y"
    { yyval.str = yyvsp[0].str;	}
    break;

  case 961:
#line 3402 "preproc.y"
    { yyval.str = yyvsp[0].str;	}
    break;

  case 962:
#line 3404 "preproc.y"
    { yyval.str = cat_str(3, make_str("param"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 963:
#line 3406 "preproc.y"
    { yyval.str = cat_str(3, make_str("("), yyvsp[-1].str, make_str(")")); }
    break;

  case 964:
#line 3408 "preproc.y"
    { yyval.str = cat_str(5, make_str("("), yyvsp[-3].str, make_str(")"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 965:
#line 3410 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 966:
#line 3412 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-2].str, make_str("()"));	}
    break;

  case 967:
#line 3414 "preproc.y"
    { yyval.str = cat_str(4, yyvsp[-3].str, make_str("("), yyvsp[-1].str, make_str(")"));	}
    break;

  case 968:
#line 3416 "preproc.y"
    { yyval.str = cat_str(4, yyvsp[-4].str, make_str("( all"), yyvsp[-1].str, make_str(")"));	}
    break;

  case 969:
#line 3418 "preproc.y"
    { yyval.str = cat_str(4, yyvsp[-4].str, make_str("( distinct"), yyvsp[-1].str, make_str(")"));  }
    break;

  case 970:
#line 3420 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-3].str, make_str("(*)")); }
    break;

  case 971:
#line 3422 "preproc.y"
    { yyval.str = make_str("current_date"); }
    break;

  case 972:
#line 3424 "preproc.y"
    { yyval.str = cat2_str(make_str("current_time"), yyvsp[0].str); }
    break;

  case 973:
#line 3426 "preproc.y"
    { yyval.str = make_str("current_time"); }
    break;

  case 974:
#line 3428 "preproc.y"
    { yyval.str = cat2_str(make_str("current_timestamp"), yyvsp[0].str); }
    break;

  case 975:
#line 3430 "preproc.y"
    { yyval.str = make_str("current_timestamp"); }
    break;

  case 976:
#line 3432 "preproc.y"
    { yyval.str = cat2_str(make_str("current_user"), yyvsp[0].str); }
    break;

  case 977:
#line 3434 "preproc.y"
    { yyval.str = cat2_str(make_str("session_user"), yyvsp[0].str); }
    break;

  case 978:
#line 3436 "preproc.y"
    { yyval.str = cat2_str(make_str("user"), yyvsp[0].str); }
    break;

  case 979:
#line 3438 "preproc.y"
    { yyval.str = cat_str(5, make_str("cast("), yyvsp[-3].str, make_str("as"), yyvsp[-1].str, make_str(")")); }
    break;

  case 980:
#line 3440 "preproc.y"
    { yyval.str = cat_str(3, make_str("extract("), yyvsp[-1].str, make_str(")")); }
    break;

  case 981:
#line 3442 "preproc.y"
    { yyval.str = cat_str(3, make_str("overlay("), yyvsp[-1].str, make_str(")")); }
    break;

  case 982:
#line 3444 "preproc.y"
    { yyval.str = cat_str(3, make_str("position("), yyvsp[-1].str, make_str(")")); }
    break;

  case 983:
#line 3446 "preproc.y"
    { yyval.str = cat_str(3, make_str("substring("), yyvsp[-1].str, make_str(")")); }
    break;

  case 984:
#line 3448 "preproc.y"
    { yyval.str = cat_str(5, make_str("treat("), yyvsp[-3].str, make_str("as"), yyvsp[-1].str, make_str(")")); }
    break;

  case 985:
#line 3451 "preproc.y"
    { yyval.str = cat_str(3, make_str("trim(both"), yyvsp[-1].str, make_str(")")); }
    break;

  case 986:
#line 3453 "preproc.y"
    { yyval.str = cat_str(3, make_str("trim(leading"), yyvsp[-1].str, make_str(")")); }
    break;

  case 987:
#line 3455 "preproc.y"
    { yyval.str = cat_str(3, make_str("trim(trailing"), yyvsp[-1].str, make_str(")")); }
    break;

  case 988:
#line 3457 "preproc.y"
    { yyval.str = cat_str(3, make_str("trim("), yyvsp[-1].str, make_str(")")); }
    break;

  case 989:
#line 3459 "preproc.y"
    { yyval.str = cat_str(3, make_str("convert("), yyvsp[-1].str, make_str(")")); }
    break;

  case 990:
#line 3461 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 991:
#line 3463 "preproc.y"
    { yyval.str = cat2_str(make_str("exists"), yyvsp[0].str); }
    break;

  case 992:
#line 3470 "preproc.y"
    { yyval.str = cat_str(4, make_str("["), yyvsp[-2].str, make_str("]"), yyvsp[0].str); }
    break;

  case 993:
#line 3472 "preproc.y"
    { yyval.str = cat_str(6, make_str("["), yyvsp[-4].str, make_str(":"), yyvsp[-2].str, make_str("]"), yyvsp[0].str); }
    break;

  case 994:
#line 3474 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 995:
#line 3478 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 996:
#line 3480 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str(","), yyvsp[0].str); }
    break;

  case 997:
#line 3482 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("using"), yyvsp[0].str); }
    break;

  case 998:
#line 3486 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("from"), yyvsp[0].str); }
    break;

  case 999:
#line 3488 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 1000:
#line 3492 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, ',', yyvsp[0].str); }
    break;

  case 1001:
#line 3494 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1002:
#line 3501 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1003:
#line 3502 "preproc.y"
    { yyval.str = make_str("year"); }
    break;

  case 1004:
#line 3503 "preproc.y"
    { yyval.str = make_str("month"); }
    break;

  case 1005:
#line 3504 "preproc.y"
    { yyval.str = make_str("day"); }
    break;

  case 1006:
#line 3505 "preproc.y"
    { yyval.str = make_str("hour"); }
    break;

  case 1007:
#line 3506 "preproc.y"
    { yyval.str = make_str("minute"); }
    break;

  case 1008:
#line 3507 "preproc.y"
    { yyval.str = make_str("second"); }
    break;

  case 1009:
#line 3508 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1010:
#line 3513 "preproc.y"
    { yyval.str = cat_str(4, yyvsp[-3].str, 42, yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 1011:
#line 3515 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 1012:
#line 3519 "preproc.y"
    { yyval.str = cat2_str(make_str("placing"), yyvsp[0].str); }
    break;

  case 1013:
#line 3524 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("in"), yyvsp[0].str); }
    break;

  case 1014:
#line 3526 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 1015:
#line 3530 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 1016:
#line 3532 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 1017:
#line 3534 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 1018:
#line 3536 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 1019:
#line 3538 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1020:
#line 3540 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 1021:
#line 3544 "preproc.y"
    { yyval.str = cat2_str(make_str("from"), yyvsp[0].str); }
    break;

  case 1022:
#line 3548 "preproc.y"
    { yyval.str = cat2_str(make_str("for"), yyvsp[0].str); }
    break;

  case 1023:
#line 3552 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("from"), yyvsp[0].str); }
    break;

  case 1024:
#line 3554 "preproc.y"
    { yyval.str = cat2_str(make_str("from"), yyvsp[0].str); }
    break;

  case 1025:
#line 3556 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1026:
#line 3561 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("using"), yyvsp[0].str); }
    break;

  case 1027:
#line 3563 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1028:
#line 3565 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 1029:
#line 3568 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1030:
#line 3569 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, ',', yyvsp[0].str); }
    break;

  case 1031:
#line 3573 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1032:
#line 3575 "preproc.y"
    { yyval.str = cat_str(3, make_str("("), yyvsp[-1].str, make_str(")")); }
    break;

  case 1033:
#line 3579 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1034:
#line 3581 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str(","), yyvsp[0].str);}
    break;

  case 1035:
#line 3600 "preproc.y"
    { yyval.str = cat_str(5, make_str("case"), yyvsp[-3].str, yyvsp[-2].str, yyvsp[-1].str, make_str("end")); }
    break;

  case 1036:
#line 3602 "preproc.y"
    { yyval.str = cat_str(5, make_str("nullif("), yyvsp[-3].str, make_str(","), yyvsp[-1].str, make_str(")")); }
    break;

  case 1037:
#line 3604 "preproc.y"
    { yyval.str = cat_str(3, make_str("coalesce("), yyvsp[-1].str, make_str(")")); }
    break;

  case 1038:
#line 3608 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 1039:
#line 3610 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1040:
#line 3614 "preproc.y"
    { yyval.str = cat_str(4, make_str("when"), yyvsp[-2].str, make_str("then"), yyvsp[0].str); }
    break;

  case 1041:
#line 3618 "preproc.y"
    { yyval.str = cat2_str(make_str("else"), yyvsp[0].str); }
    break;

  case 1042:
#line 3620 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 1043:
#line 3623 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1044:
#line 3624 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 1045:
#line 3628 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 1046:
#line 3630 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 1047:
#line 3634 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 1048:
#line 3638 "preproc.y"
    { yyval.str = cat2_str(make_str("."), yyvsp[0].str); }
    break;

  case 1049:
#line 3640 "preproc.y"
    { yyval.str = make_str(".*"); }
    break;

  case 1050:
#line 3642 "preproc.y"
    { yyval.str = cat_str(3, make_str("."), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 1051:
#line 3645 "preproc.y"
    { yyval.str = make_str("()"); }
    break;

  case 1052:
#line 3646 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 1053:
#line 3658 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str(","), yyvsp[0].str);  }
    break;

  case 1054:
#line 3660 "preproc.y"
    { yyval.str = yyvsp[0].str;	}
    break;

  case 1055:
#line 3665 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str("as"), yyvsp[0].str); }
    break;

  case 1056:
#line 3667 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1057:
#line 3669 "preproc.y"
    { yyval.str = make_str("*"); }
    break;

  case 1058:
#line 3674 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str(","),yyvsp[0].str);	}
    break;

  case 1059:
#line 3676 "preproc.y"
    { yyval.str = yyvsp[0].str;	}
    break;

  case 1060:
#line 3678 "preproc.y"
    { yyval.str = make_str("*"); }
    break;

  case 1061:
#line 3682 "preproc.y"
    { yyval.str = cat_str(4, yyvsp[-3].str, yyvsp[-2].str, make_str("="), yyvsp[0].str); }
    break;

  case 1062:
#line 3686 "preproc.y"
    {	yyval.str = cat_str(3, yyvsp[-2].str, make_str(","), yyvsp[0].str);  }
    break;

  case 1063:
#line 3688 "preproc.y"
    {	yyval.str = yyvsp[0].str;  }
    break;

  case 1064:
#line 3691 "preproc.y"
    {	yyval.str = yyvsp[0].str;  }
    break;

  case 1065:
#line 3692 "preproc.y"
    {	yyval.str = make_str("default"); }
    break;

  case 1066:
#line 3702 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1067:
#line 3703 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1068:
#line 3707 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1069:
#line 3709 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str(","), yyvsp[0].str); }
    break;

  case 1070:
#line 3713 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1071:
#line 3715 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1072:
#line 3719 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1073:
#line 3721 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str(","), yyvsp[0].str); }
    break;

  case 1074:
#line 3725 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1075:
#line 3726 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1076:
#line 3727 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1077:
#line 3728 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1078:
#line 3729 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1079:
#line 3731 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1080:
#line 3742 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1081:
#line 3744 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1082:
#line 3752 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1083:
#line 3754 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 1084:
#line 3756 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 1085:
#line 3758 "preproc.y"
    { yyval.str = cat_str(6, yyvsp[-5].str, make_str("("), yyvsp[-3].str, make_str(")"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 1086:
#line 3760 "preproc.y"
    { yyval.str = cat2_str(make_str("param"), yyvsp[0].str); }
    break;

  case 1087:
#line 3762 "preproc.y"
    { yyval.str = make_str("true"); }
    break;

  case 1088:
#line 3764 "preproc.y"
    { yyval.str = make_str("false"); }
    break;

  case 1089:
#line 3766 "preproc.y"
    { yyval.str = make_str("null"); }
    break;

  case 1090:
#line 3768 "preproc.y"
    { yyval.str = make_str("?"); }
    break;

  case 1091:
#line 3771 "preproc.y"
    { yyval.str = make_name();}
    break;

  case 1092:
#line 3772 "preproc.y"
    { yyval.str = make_name();}
    break;

  case 1093:
#line 3773 "preproc.y"
    { yyval.str = make_name();}
    break;

  case 1094:
#line 3774 "preproc.y"
    { yyval.str = make_name();}
    break;

  case 1095:
#line 3776 "preproc.y"
    {
			yyval.str = (char *)mm_alloc(strlen(yyvsp[0].str) + 3);
			yyval.str[0]='\'';
					strcpy(yyval.str+1, yyvsp[0].str);
			yyval.str[strlen(yyvsp[0].str)+2]='\0';
			yyval.str[strlen(yyvsp[0].str)+1]='\'';
			free(yyvsp[0].str);
		}
    break;

  case 1096:
#line 3786 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1097:
#line 3787 "preproc.y"
    { yyval.str = make_str("?"); }
    break;

  case 1098:
#line 3790 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1099:
#line 3791 "preproc.y"
    { yyval.str = cat2_str(make_str("-"), yyvsp[0].str); }
    break;

  case 1100:
#line 3794 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1101:
#line 3795 "preproc.y"
    { yyval.str = make_str("?"); }
    break;

  case 1102:
#line 3798 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1103:
#line 3799 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1104:
#line 3800 "preproc.y"
    { yyval.str = make_str("?"); }
    break;

  case 1105:
#line 3803 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1106:
#line 3804 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1107:
#line 3805 "preproc.y"
    { yyval.str = cat2_str(make_str("-"), yyvsp[0].str); }
    break;

  case 1108:
#line 3806 "preproc.y"
    { yyval.str = cat2_str(make_str("-"), yyvsp[0].str); }
    break;

  case 1109:
#line 3807 "preproc.y"
    { yyval.str = make_str("?"); }
    break;

  case 1110:
#line 3810 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1111:
#line 3811 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1112:
#line 3814 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1113:
#line 3815 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1114:
#line 3816 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1115:
#line 3817 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1116:
#line 3818 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1117:
#line 3819 "preproc.y"
    { yyval.str = make_str("?"); }
    break;

  case 1118:
#line 3822 "preproc.y"
    { yyval.str = yyvsp[0].str;}
    break;

  case 1119:
#line 3825 "preproc.y"
    {
			if (!QueryIsRule)
				mmerror(PARSE_ERROR, ET_ERROR, "OLD used in non-rule query");

			yyval.str = make_str("old");
		}
    break;

  case 1120:
#line 3832 "preproc.y"
    {
			if (!QueryIsRule)
				mmerror(PARSE_ERROR, ET_ERROR, "NEW used in non-rule query");

			yyval.str = make_str("new");
		}
    break;

  case 1121:
#line 3848 "preproc.y"
    { yyval.str = cat_str(5, yyvsp[-2].str, make_str(","), yyvsp[0].str, make_str(","), yyvsp[-1].str); }
    break;

  case 1122:
#line 3850 "preproc.y"
    { yyval.str = make_str("NULL,NULL,NULL,\"DEFAULT\""); }
    break;

  case 1123:
#line 3853 "preproc.y"
    { yyval.str = cat_str(3, make_str("NULL,"), yyvsp[0].str, make_str(",NULL")); }
    break;

  case 1124:
#line 3857 "preproc.y"
    {
			/* old style: dbname[@server][:port] */
			if (strlen(yyvsp[-1].str) > 0 && *(yyvsp[-1].str) != '@')
			{
				snprintf(errortext, sizeof(errortext),
						 "Expected '@', found '%s'", yyvsp[-1].str);
				mmerror(PARSE_ERROR, ET_ERROR, errortext);
			}

			yyval.str = make3_str(make_str("\""), make3_str(yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str), make_str("\""));
		}
    break;

  case 1125:
#line 3869 "preproc.y"
    {
			/* new style: <tcp|unix>:postgresql://server[:port][/dbname] */
			if (strncmp(yyvsp[-6].str, "unix:postgresql", strlen("unix:postgresql")) != 0 && strncmp(yyvsp[-6].str, "tcp:postgresql", strlen("tcp:postgresql")) != 0)
			{
				snprintf(errortext, sizeof(errortext), "only protocols 'tcp' and 'unix' and database type 'postgresql' are supported");
				mmerror(PARSE_ERROR, ET_ERROR, errortext);
			}

			if (strncmp(yyvsp[-4].str, "//", strlen("//")) != 0)
			{
				snprintf(errortext, sizeof(errortext), "Expected '://', found '%s'", yyvsp[-4].str);
				mmerror(PARSE_ERROR, ET_ERROR, errortext);
			}

			if (strncmp(yyvsp[-6].str, "unix", strlen("unix")) == 0 &&
				strncmp(yyvsp[-4].str + strlen("//"), "localhost", strlen("localhost")) != 0 &&
				strncmp(yyvsp[-4].str + strlen("//"), "127.0.0.1", strlen("127.0.0.1")) != 0)
			{
				snprintf(errortext, sizeof(errortext), "unix domain sockets only work on 'localhost' but not on '%9.9s'", yyvsp[-4].str + strlen("//"));
				mmerror(PARSE_ERROR, ET_ERROR, errortext);
			}

			yyval.str = make3_str(make3_str(make_str("\""), yyvsp[-6].str, make_str(":")), yyvsp[-4].str, make3_str(make3_str(yyvsp[-3].str, make_str("/"), yyvsp[-1].str),	yyvsp[0].str, make_str("\"")));
		}
    break;

  case 1126:
#line 3894 "preproc.y"
    {
			if (yyvsp[0].str[0] == '\"')
				yyval.str = yyvsp[0].str;
			else if (strcmp(yyvsp[0].str, "?") == 0) /* variable */
			{
				enum ECPGttype type = argsinsert->variable->type->type;

				/* if array see what's inside */
				if (type == ECPGt_array)
					type = argsinsert->variable->type->u.element->type;

				/* handle varchars */
				if (type == ECPGt_varchar)
					yyval.str = make2_str(mm_strdup(argsinsert->variable->name), make_str(".arr"));
				else
					yyval.str = mm_strdup(argsinsert->variable->name);
			}
			else
				yyval.str = make3_str(make_str("\""), yyvsp[0].str, make_str("\""));
		}
    break;

  case 1127:
#line 3917 "preproc.y"
    {
			if (strcmp(yyvsp[0].str, "postgresql") != 0 && strcmp(yyvsp[0].str, "postgres") != 0)
			{
				snprintf(errortext, sizeof(errortext), "Expected 'postgresql', found '%s'", yyvsp[0].str);
				mmerror(PARSE_ERROR, ET_ERROR, errortext);
			}

			if (strcmp(yyvsp[-1].str, "tcp") != 0 && strcmp(yyvsp[-1].str, "unix") != 0)
			{
				snprintf(errortext, sizeof(errortext), "Illegal connection type %s", yyvsp[-1].str);
				mmerror(PARSE_ERROR, ET_ERROR, errortext);
			}

			yyval.str = make3_str(yyvsp[-1].str, make_str(":"), yyvsp[0].str);
		}
    break;

  case 1128:
#line 3935 "preproc.y"
    {
			if (strcmp(yyvsp[-1].str, "@") != 0 && strcmp(yyvsp[-1].str, "//") != 0)
			{
				snprintf(errortext, sizeof(errortext), "Expected '@' or '://', found '%s'", yyvsp[-1].str);
				mmerror(PARSE_ERROR, ET_ERROR, errortext);
			}

			yyval.str = make2_str(yyvsp[-1].str, yyvsp[0].str);
		}
    break;

  case 1129:
#line 3946 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1130:
#line 3947 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 1131:
#line 3950 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1132:
#line 3951 "preproc.y"
    { yyval.str = make3_str(yyvsp[-2].str, make_str("."), yyvsp[0].str); }
    break;

  case 1133:
#line 3952 "preproc.y"
    { yyval.str = make_name(); }
    break;

  case 1134:
#line 3955 "preproc.y"
    { yyval.str = make2_str(make_str(":"), yyvsp[0].str); }
    break;

  case 1135:
#line 3956 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 1136:
#line 3959 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1137:
#line 3960 "preproc.y"
    { yyval.str = make_str("NULL"); }
    break;

  case 1138:
#line 3963 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1139:
#line 3964 "preproc.y"
    { yyval.str = make_str("NULL,NULL"); }
    break;

  case 1140:
#line 3968 "preproc.y"
    { yyval.str = cat2_str(yyvsp[0].str, make_str(", NULL")); }
    break;

  case 1141:
#line 3970 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str(","), yyvsp[0].str); }
    break;

  case 1142:
#line 3972 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-3].str, make_str(","), yyvsp[0].str); }
    break;

  case 1143:
#line 3974 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str(","), yyvsp[0].str); }
    break;

  case 1144:
#line 3978 "preproc.y"
    {
			if (yyvsp[0].str[0] == '\"')
				yyval.str = yyvsp[0].str;
			else
				yyval.str = make3_str(make_str("\""), yyvsp[0].str, make_str("\""));
		}
    break;

  case 1145:
#line 3985 "preproc.y"
    {
			if (yyvsp[0].str[0] == '\"')
				yyval.str = yyvsp[0].str;
			else if (strcmp(yyvsp[0].str, "?") == 0) /* variable */
			{
				enum ECPGttype type = argsinsert->variable->type->type;

				/* if array see what's inside */
				if (type == ECPGt_array)
					type = argsinsert->variable->type->u.element->type;

				/* handle varchars */
				if (type == ECPGt_varchar)
					yyval.str = make2_str(mm_strdup(argsinsert->variable->name), make_str(".arr"));
				else
					yyval.str = mm_strdup(argsinsert->variable->name);
			}
			else
				yyval.str = make3_str(make_str("\""), yyvsp[0].str, make_str("\""));
		}
    break;

  case 1146:
#line 4008 "preproc.y"
    {
			/* check if we have a char variable */
			struct variable *p = find_variable(yyvsp[0].str);
			enum ECPGttype type = p->type->type;

			/* if array see what's inside */
			if (type == ECPGt_array)
				type = p->type->u.element->type;

			switch (type)
			{
				case ECPGt_char:
				case ECPGt_unsigned_char:
					yyval.str = yyvsp[0].str;
					break;
				case ECPGt_varchar:
					yyval.str = make2_str(yyvsp[0].str, make_str(".arr"));
					break;
				default:
					mmerror(PARSE_ERROR, ET_ERROR, "invalid datatype");
					break;
			}
		}
    break;

  case 1147:
#line 4034 "preproc.y"
    {
			if (strlen(yyvsp[-1].str) == 0)
				mmerror(PARSE_ERROR, ET_ERROR, "incomplete statement");

			if (strcmp(yyvsp[-1].str, "?") != 0)
			{
				snprintf(errortext, sizeof(errortext), "unrecognised token '%s'", yyvsp[-1].str);
				mmerror(PARSE_ERROR, ET_ERROR, errortext);
			}

			yyval.str = make2_str(make_str("?"), yyvsp[0].str);
		}
    break;

  case 1148:
#line 4046 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 1149:
#line 4054 "preproc.y"
    {
			struct cursor *ptr, *this;
			struct variable *thisquery = (struct variable *)mm_alloc(sizeof(struct variable));

			for (ptr = cur; ptr != NULL; ptr = ptr->next)
			{
				if (strcmp(yyvsp[-4].str, ptr->name) == 0)
				{
						/* re-definition is a bug */
					snprintf(errortext, sizeof(errortext), "cursor %s already defined", yyvsp[-4].str);
					mmerror(PARSE_ERROR, ET_ERROR, errortext);
				}
			}

			this = (struct cursor *) mm_alloc(sizeof(struct cursor));

			/* initial definition */
			this->next = cur;
			this->name = yyvsp[-4].str;
			this->connection = connection;
			this->command =  cat_str(4, make_str("declare"), mm_strdup(yyvsp[-4].str), yyvsp[-3].str, make_str("cursor for ?"));
			this->argsresult = NULL;

			thisquery->type = &ecpg_query;
			thisquery->brace_level = 0;
			thisquery->next = NULL;
			thisquery->name = (char *) mm_alloc(sizeof("ECPGprepared_statement(\"\")") + strlen(yyvsp[0].str));
			sprintf(thisquery->name, "ECPGprepared_statement(\"%s\")", yyvsp[0].str);

			this->argsinsert = NULL;
			add_variable(&(this->argsinsert), thisquery, &no_indicator);

			cur = this;

			yyval.str = cat_str(3, make_str("/*"), mm_strdup(this->command), make_str("*/"));
		}
    break;

  case 1150:
#line 4097 "preproc.y"
    { yyval.str = cat_str(3, make_str("ECPGdeallocate(__LINE__, \""), yyvsp[0].str, make_str("\");")); }
    break;

  case 1151:
#line 4099 "preproc.y"
    { yyval.str = cat_str(2, make_str("ECPGdeallocate(__LINE__, \""), yyvsp[0].str, make_str("\");")); }
    break;

  case 1152:
#line 4106 "preproc.y"
    { fputs("/* exec sql begin declare section */", yyout); }
    break;

  case 1153:
#line 4108 "preproc.y"
    {
			fprintf(yyout, "%s/* exec sql end declare section */", yyvsp[-1].str);
			free(yyvsp[-1].str);
			output_line_number();
		}
    break;

  case 1154:
#line 4115 "preproc.y"
    {}
    break;

  case 1155:
#line 4117 "preproc.y"
    {}
    break;

  case 1156:
#line 4119 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 1157:
#line 4120 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1158:
#line 4123 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1159:
#line 4124 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1160:
#line 4125 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 1161:
#line 4126 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 1162:
#line 4129 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1163:
#line 4130 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 1164:
#line 4134 "preproc.y"
    {
		/* reset this variable so we see if there was */
		/* an initializer specified */
		initializer = 0;
	}
    break;

  case 1165:
#line 4140 "preproc.y"
    {
		/* add entry to list */
		struct typedefs *ptr, *this;
		int dimension = yyvsp[-1].index.index1;
		int length = yyvsp[-1].index.index2;

		if ((yyvsp[-4].type.type_enum == ECPGt_struct ||
		     yyvsp[-4].type.type_enum == ECPGt_union) &&
		    initializer == 1)
		{
			mmerror(PARSE_ERROR, ET_ERROR, "Initializer not allowed in typedef command");

		}
		else
		{
			for (ptr = types; ptr != NULL; ptr = ptr->next)
			{
				if (strcmp(yyvsp[-2].str, ptr->name) == 0)
				{
			        	/* re-definition is a bug */
					snprintf(errortext, sizeof(errortext), "Type %s already defined", yyvsp[-2].str);
					mmerror(PARSE_ERROR, ET_ERROR, errortext);
				}
			}

			adjust_array(yyvsp[-4].type.type_enum, &dimension, &length, yyvsp[-4].type.type_dimension, yyvsp[-4].type.type_index, *yyvsp[-3].str?1:0);

			this = (struct typedefs *) mm_alloc(sizeof(struct typedefs));

	        	/* initial definition */
		        this->next = types;
	        	this->name = yyvsp[-2].str;
			this->type = (struct this_type *) mm_alloc(sizeof(struct this_type));
			this->type->type_enum = yyvsp[-4].type.type_enum;
			this->type->type_str = mm_strdup(yyvsp[-2].str);
			this->type->type_dimension = dimension; /* dimension of array */
			this->type->type_index = length;    /* lenght of string */
			this->type->type_sizeof = ECPGstruct_sizeof;
			this->struct_member_list = (yyvsp[-4].type.type_enum == ECPGt_struct || yyvsp[-4].type.type_enum == ECPGt_union) ?
				struct_member_list[struct_level] : NULL;

			if (yyvsp[-4].type.type_enum != ECPGt_varchar &&
			    yyvsp[-4].type.type_enum != ECPGt_char &&
	        	    yyvsp[-4].type.type_enum != ECPGt_unsigned_char &&
			    this->type->type_index >= 0)
				mmerror(PARSE_ERROR, ET_ERROR, "No multi-dimensional array support for simple data types");

			types = this;
		}

		fprintf(yyout, "typedef %s %s %s %s;\n", yyvsp[-4].type.type_str, *yyvsp[-3].str?"*":"", yyvsp[-2].str, yyvsp[-1].index.str);
		output_line_number();
		yyval.str = make_str("");
	}
    break;

  case 1166:
#line 4196 "preproc.y"
    {
			actual_storage[struct_level] = cat2_str(mm_strdup(yyvsp[-1].str), mm_strdup(yyvsp[0].str));
			actual_startline[struct_level] = hashline_number();
		}
    break;

  case 1167:
#line 4201 "preproc.y"
    {
			actual_type[struct_level].type_enum = yyvsp[0].type.type_enum;
			actual_type[struct_level].type_dimension = yyvsp[0].type.type_dimension;
			actual_type[struct_level].type_index = yyvsp[0].type.type_index;
			actual_type[struct_level].type_sizeof = yyvsp[0].type.type_sizeof;

			/* we do not need the string "varchar" for output */
			/* so replace it with an empty string */
			if (yyvsp[0].type.type_enum == ECPGt_varchar)
			{
				free(yyvsp[0].type.type_str);
				yyvsp[0].type.type_str=EMPTY;
			}
		}
    break;

  case 1168:
#line 4216 "preproc.y"
    {
			yyval.str = cat_str(6, actual_startline[struct_level], yyvsp[-6].str, yyvsp[-5].str, yyvsp[-3].type.type_str, yyvsp[-1].str, make_str(";\n"));
		}
    break;

  case 1169:
#line 4221 "preproc.y"
    { yyval.str = make_str("extern"); }
    break;

  case 1170:
#line 4222 "preproc.y"
    { yyval.str = make_str("static"); }
    break;

  case 1171:
#line 4223 "preproc.y"
    { yyval.str = make_str("register"); }
    break;

  case 1172:
#line 4224 "preproc.y"
    { yyval.str = make_str("auto"); }
    break;

  case 1173:
#line 4225 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 1174:
#line 4228 "preproc.y"
    { yyval.str = make_str("const"); }
    break;

  case 1175:
#line 4229 "preproc.y"
    { yyval.str = make_str("volatile"); }
    break;

  case 1176:
#line 4230 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 1177:
#line 4234 "preproc.y"
    {
			yyval.type.type_enum = yyvsp[0].type_enum;
			yyval.type.type_str = mm_strdup(ECPGtype_name(yyvsp[0].type_enum));
			yyval.type.type_dimension = -1;
			yyval.type.type_index = -1;
			yyval.type.type_sizeof = NULL;
		}
    break;

  case 1178:
#line 4242 "preproc.y"
    {
			yyval.type.type_enum = ECPGt_struct;
			yyval.type.type_str = yyvsp[0].str;
			yyval.type.type_dimension = -1;
			yyval.type.type_index = -1;
			yyval.type.type_sizeof = ECPGstruct_sizeof;
		}
    break;

  case 1179:
#line 4250 "preproc.y"
    {
			yyval.type.type_enum = ECPGt_union;
			yyval.type.type_str = yyvsp[0].str;
			yyval.type.type_dimension = -1;
			yyval.type.type_index = -1;
			yyval.type.type_sizeof = NULL;
		}
    break;

  case 1180:
#line 4258 "preproc.y"
    {
			yyval.type.type_str = yyvsp[0].str;
			yyval.type.type_enum = ECPGt_int;
			yyval.type.type_dimension = -1;
			yyval.type.type_index = -1;
			yyval.type.type_sizeof = NULL;
		}
    break;

  case 1181:
#line 4266 "preproc.y"
    {
			/*
			 * Check for type names that the SQL grammar treats as
			 * unreserved keywords
			 */
			if (strcmp(yyvsp[0].str, "varchar") == 0)
			{
				yyval.type.type_enum = ECPGt_varchar;
				yyval.type.type_str = make_str("varchar");
				yyval.type.type_dimension = -1;
				yyval.type.type_index = -1;
				yyval.type.type_sizeof = NULL;
			}
			else if (strcmp(yyvsp[0].str, "float") == 0)
			{
				yyval.type.type_enum = ECPGt_float;
				yyval.type.type_str = make_str("float");
				yyval.type.type_dimension = -1;
				yyval.type.type_index = -1;
				yyval.type.type_sizeof = NULL;
			}
			else if (strcmp(yyvsp[0].str, "double") == 0)
			{
				yyval.type.type_enum = ECPGt_double;
				yyval.type.type_str = make_str("double");
				yyval.type.type_dimension = -1;
				yyval.type.type_index = -1;
				yyval.type.type_sizeof = NULL;
			}
			else
			{
				/* this is for typedef'ed types */
				struct typedefs *this = get_typedef(yyvsp[0].str);

				yyval.type.type_str = (this->type->type_enum == ECPGt_varchar) ? EMPTY : mm_strdup(this->name);
				yyval.type.type_enum = this->type->type_enum;
				yyval.type.type_dimension = this->type->type_dimension;
				yyval.type.type_index = this->type->type_index;
				yyval.type.type_sizeof = this->type->type_sizeof;
				struct_member_list[struct_level] = ECPGstruct_member_dup(this->struct_member_list);
			}
		}
    break;

  case 1182:
#line 4311 "preproc.y"
    { yyval.str = cat_str(3, make_str("enum"), yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 1183:
#line 4313 "preproc.y"
    { yyval.str = cat2_str(make_str("enum"), yyvsp[0].str); }
    break;

  case 1184:
#line 4317 "preproc.y"
    { yyval.str = cat_str(3, make_str("{"), yyvsp[-1].str, make_str("}")); }
    break;

  case 1185:
#line 4320 "preproc.y"
    {
			ECPGfree_struct_member(struct_member_list[struct_level]);
			free(actual_storage[struct_level--]);
			yyval.str = cat_str(4, yyvsp[-3].str, make_str("{"), yyvsp[-1].str, make_str("}"));
		}
    break;

  case 1186:
#line 4328 "preproc.y"
    {
			ECPGfree_struct_member(struct_member_list[struct_level]);
			free(actual_storage[struct_level--]);
			yyval.str = cat_str(4, yyvsp[-3].str, make_str("{"), yyvsp[-1].str, make_str("}"));
		}
    break;

  case 1187:
#line 4336 "preproc.y"
    {
			struct_member_list[struct_level++] = NULL;
			yyval.str = cat2_str(make_str("struct"), yyvsp[0].str);
			ECPGstruct_sizeof = cat_str(3, make_str("sizeof("), strdup(yyval.str), make_str(")"));
			if (struct_level >= STRUCT_DEPTH)
				 mmerror(PARSE_ERROR, ET_ERROR, "Too many levels in nested structure definition");
		}
    break;

  case 1188:
#line 4346 "preproc.y"
    {
			struct_member_list[struct_level++] = NULL;
			if (struct_level >= STRUCT_DEPTH)
				 mmerror(PARSE_ERROR, ET_ERROR, "Too many levels in nested structure definition");

			yyval.str = cat2_str(make_str("union"), yyvsp[0].str);
		}
    break;

  case 1189:
#line 4355 "preproc.y"
    { yyval.type_enum=yyvsp[0].type_enum; }
    break;

  case 1190:
#line 4356 "preproc.y"
    { yyval.type_enum=yyvsp[0].type_enum; }
    break;

  case 1191:
#line 4359 "preproc.y"
    { yyval.type_enum = ECPGt_unsigned_short; }
    break;

  case 1192:
#line 4360 "preproc.y"
    { yyval.type_enum = ECPGt_unsigned_short; }
    break;

  case 1193:
#line 4361 "preproc.y"
    { yyval.type_enum = ECPGt_unsigned_int; }
    break;

  case 1194:
#line 4362 "preproc.y"
    { yyval.type_enum = ECPGt_unsigned_int; }
    break;

  case 1195:
#line 4363 "preproc.y"
    { yyval.type_enum = ECPGt_unsigned_long; }
    break;

  case 1196:
#line 4364 "preproc.y"
    { yyval.type_enum = ECPGt_unsigned_long; }
    break;

  case 1197:
#line 4366 "preproc.y"
    {
#ifdef HAVE_LONG_LONG_INT_64
			yyval.type_enum = ECPGt_unsigned_long_long;
#else
			yyval.type_enum = ECPGt_unsigned_long;
#endif
		}
    break;

  case 1198:
#line 4374 "preproc.y"
    {
#ifdef HAVE_LONG_LONG_INT_64
			yyval.type_enum = ECPGt_unsigned_long_long;
#else
			yyval.type_enum = ECPGt_unsigned_long;
#endif
		}
    break;

  case 1199:
#line 4381 "preproc.y"
    { yyval.type_enum = ECPGt_unsigned_char; }
    break;

  case 1200:
#line 4384 "preproc.y"
    { yyval.type_enum = ECPGt_short; }
    break;

  case 1201:
#line 4385 "preproc.y"
    { yyval.type_enum = ECPGt_short; }
    break;

  case 1202:
#line 4386 "preproc.y"
    { yyval.type_enum = ECPGt_int; }
    break;

  case 1203:
#line 4387 "preproc.y"
    { yyval.type_enum = ECPGt_long; }
    break;

  case 1204:
#line 4388 "preproc.y"
    { yyval.type_enum = ECPGt_long; }
    break;

  case 1205:
#line 4390 "preproc.y"
    {
#ifdef HAVE_LONG_LONG_INT_64
			yyval.type_enum = ECPGt_long_long;
#else
			yyval.type_enum = ECPGt_long;
#endif
		}
    break;

  case 1206:
#line 4398 "preproc.y"
    {
#ifdef HAVE_LONG_LONG_INT_64
			yyval.type_enum = ECPGt_long_long;
#else
			yyval.type_enum = ECPGt_long;
#endif
		}
    break;

  case 1207:
#line 4405 "preproc.y"
    { yyval.type_enum = ECPGt_bool; }
    break;

  case 1208:
#line 4406 "preproc.y"
    { yyval.type_enum = ECPGt_char; }
    break;

  case 1211:
#line 4414 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1212:
#line 4416 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str(","), yyvsp[0].str); }
    break;

  case 1213:
#line 4420 "preproc.y"
    {
			struct ECPGtype * type;
			int dimension = yyvsp[-1].index.index1; /* dimension of array */
			int length = yyvsp[-1].index.index2;    /* lenght of string */
			char dim[14L], ascii_len[12];

			adjust_array(actual_type[struct_level].type_enum, &dimension, &length, actual_type[struct_level].type_dimension, actual_type[struct_level].type_index, strlen(yyvsp[-3].str));

			switch (actual_type[struct_level].type_enum)
			{
				case ECPGt_struct:
				case ECPGt_union:
					if (dimension < 0)
						type = ECPGmake_struct_type(struct_member_list[struct_level], actual_type[struct_level].type_enum, actual_type[struct_level].type_sizeof);
					else
						type = ECPGmake_array_type(ECPGmake_struct_type(struct_member_list[struct_level], actual_type[struct_level].type_enum, actual_type[struct_level].type_sizeof), dimension);

					yyval.str = cat_str(4, yyvsp[-3].str, mm_strdup(yyvsp[-2].str), yyvsp[-1].index.str, yyvsp[0].str);
					break;

				case ECPGt_varchar:
					if (dimension < 0)
						type = ECPGmake_simple_type(actual_type[struct_level].type_enum, length);
					else
						type = ECPGmake_array_type(ECPGmake_simple_type(actual_type[struct_level].type_enum, length), dimension);

					switch(dimension)
					{
						case 0:
						case -1:
						case 1:
							*dim = '\0';
							break;
						default:
							sprintf(dim, "[%d]", dimension);
							break;
					}
					sprintf(ascii_len, "%d", length);

					if (length == 0)
						mmerror(PARSE_ERROR, ET_ERROR, "pointer to varchar are not implemented");

					if (dimension == 0)
						yyval.str = cat_str(7, mm_strdup(actual_storage[struct_level]), make2_str(make_str(" struct varchar_"), mm_strdup(yyvsp[-2].str)), make_str(" { int len; char arr["), mm_strdup(ascii_len), make_str("]; } *"), mm_strdup(yyvsp[-2].str), yyvsp[0].str);
					else
					   yyval.str = cat_str(8, mm_strdup(actual_storage[struct_level]), make2_str(make_str(" struct varchar_"), mm_strdup(yyvsp[-2].str)), make_str(" { int len; char arr["), mm_strdup(ascii_len), make_str("]; } "), mm_strdup(yyvsp[-2].str), mm_strdup(dim), yyvsp[0].str);
					break;

				case ECPGt_char:
				case ECPGt_unsigned_char:
					if (dimension == -1)
						type = ECPGmake_simple_type(actual_type[struct_level].type_enum, length);
					else
						type = ECPGmake_array_type(ECPGmake_simple_type(actual_type[struct_level].type_enum, length), dimension);

					yyval.str = cat_str(4, yyvsp[-3].str, mm_strdup(yyvsp[-2].str), yyvsp[-1].index.str, yyvsp[0].str);
					break;

				default:
					if (dimension < 0)
						type = ECPGmake_simple_type(actual_type[struct_level].type_enum, 1);
					else
						type = ECPGmake_array_type(ECPGmake_simple_type(actual_type[struct_level].type_enum, 1), dimension);

					yyval.str = cat_str(4, yyvsp[-3].str, mm_strdup(yyvsp[-2].str), yyvsp[-1].index.str, yyvsp[0].str);
					break;
			}

			if (struct_level == 0)
				new_variable(yyvsp[-2].str, type, braces_open);
			else
				ECPGmake_struct_member(yyvsp[-2].str, type, &(struct_member_list[struct_level - 1]));

			free(yyvsp[-2].str);
		}
    break;

  case 1214:
#line 4498 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 1215:
#line 4500 "preproc.y"
    {
			initializer = 1;
			yyval.str = cat2_str(make_str("="), yyvsp[0].str);
		}
    break;

  case 1216:
#line 4506 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 1217:
#line 4507 "preproc.y"
    { yyval.str = make_str("*"); }
    break;

  case 1218:
#line 4508 "preproc.y"
    { yyval.str = make_str("**"); }
    break;

  case 1219:
#line 4516 "preproc.y"
    {
			/* this is only supported for compatibility */
			yyval.str = cat_str(3, make_str("/* declare statement"), yyvsp[0].str, make_str("*/"));
		}
    break;

  case 1220:
#line 4524 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1221:
#line 4527 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1222:
#line 4528 "preproc.y"
    { yyval.str = make_str("\"CURRENT\""); }
    break;

  case 1223:
#line 4529 "preproc.y"
    { yyval.str = make_str("\"ALL\""); }
    break;

  case 1224:
#line 4530 "preproc.y"
    { yyval.str = make_str("\"CURRENT\""); }
    break;

  case 1225:
#line 4533 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1226:
#line 4534 "preproc.y"
    { yyval.str = make_str("\"DEFAULT\""); }
    break;

  case 1227:
#line 4541 "preproc.y"
    {
			struct variable *thisquery = (struct variable *)mm_alloc(sizeof(struct variable));

			thisquery->type = &ecpg_query;
			thisquery->brace_level = 0;
			thisquery->next = NULL;
			thisquery->name = yyvsp[0].str;

			add_variable(&argsinsert, thisquery, &no_indicator);

			yyval.str = make_str("?");
		}
    break;

  case 1228:
#line 4554 "preproc.y"
    {
			struct variable *thisquery = (struct variable *)mm_alloc(sizeof(struct variable));

			thisquery->type = &ecpg_query;
			thisquery->brace_level = 0;
			thisquery->next = NULL;
			thisquery->name = (char *) mm_alloc(sizeof("ECPGprepared_statement(\"\")") + strlen(yyvsp[0].str));
			sprintf(thisquery->name, "ECPGprepared_statement(\"%s\")", yyvsp[0].str);

			add_variable(&argsinsert, thisquery, &no_indicator);
		}
    break;

  case 1229:
#line 4566 "preproc.y"
    {
			yyval.str = make_str("?");
		}
    break;

  case 1230:
#line 4572 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1231:
#line 4574 "preproc.y"
    { yyval.str = make3_str(make_str("\""), yyvsp[0].str, make_str("\"")); }
    break;

  case 1232:
#line 4581 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1233:
#line 4586 "preproc.y"
    { yyval.str = yyvsp[-1].str; }
    break;

  case 1234:
#line 4588 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 1235:
#line 4590 "preproc.y"
    {
			/* mmerror ("open cursor with variables not implemented yet"); */
			yyval.str = EMPTY;
		}
    break;

  case 1238:
#line 4599 "preproc.y"
    {
			yyval.str = EMPTY;
		}
    break;

  case 1239:
#line 4603 "preproc.y"
    {
			add_variable(&argsresult, descriptor_variable(yyvsp[0].str,0), &no_indicator);
			yyval.str = EMPTY;
		}
    break;

  case 1240:
#line 4609 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 1241:
#line 4610 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1246:
#line 4622 "preproc.y"
    { yyval.str = cat2_str(make3_str(make_str("\""), yyvsp[-2].str, make_str("\",")), yyvsp[0].str); }
    break;

  case 1247:
#line 4634 "preproc.y"
    {
			drop_descriptor(yyvsp[0].str,connection);
			yyval.str = yyvsp[0].str;
		}
    break;

  case 1248:
#line 4644 "preproc.y"
    {
			add_descriptor(yyvsp[0].str,connection);
			yyval.str = yyvsp[0].str;
		}
    break;

  case 1249:
#line 4654 "preproc.y"
    { push_assignment(yyvsp[-2].str, yyvsp[0].dtype_enum); }
    break;

  case 1250:
#line 4657 "preproc.y"
    { yyval.dtype_enum = ECPGd_count; }
    break;

  case 1251:
#line 4660 "preproc.y"
    { push_assignment(yyvsp[-2].str, yyvsp[0].dtype_enum); }
    break;

  case 1252:
#line 4662 "preproc.y"
    { yyval.dtype_enum = ECPGd_cardinality; }
    break;

  case 1253:
#line 4663 "preproc.y"
    { yyval.dtype_enum = ECPGd_data; }
    break;

  case 1254:
#line 4664 "preproc.y"
    { yyval.dtype_enum = ECPGd_di_code; }
    break;

  case 1255:
#line 4665 "preproc.y"
    { yyval.dtype_enum = ECPGd_di_precision; }
    break;

  case 1256:
#line 4666 "preproc.y"
    { yyval.dtype_enum = ECPGd_indicator; }
    break;

  case 1257:
#line 4667 "preproc.y"
    { yyval.dtype_enum = ECPGd_key_member; }
    break;

  case 1258:
#line 4668 "preproc.y"
    { yyval.dtype_enum = ECPGd_length; }
    break;

  case 1259:
#line 4669 "preproc.y"
    { yyval.dtype_enum = ECPGd_name; }
    break;

  case 1260:
#line 4670 "preproc.y"
    { yyval.dtype_enum = ECPGd_nullable; }
    break;

  case 1261:
#line 4671 "preproc.y"
    { yyval.dtype_enum = ECPGd_octet; }
    break;

  case 1262:
#line 4672 "preproc.y"
    { yyval.dtype_enum = ECPGd_precision; }
    break;

  case 1263:
#line 4673 "preproc.y"
    { yyval.dtype_enum = ECPGd_length; }
    break;

  case 1264:
#line 4674 "preproc.y"
    { yyval.dtype_enum = ECPGd_ret_octet; }
    break;

  case 1265:
#line 4675 "preproc.y"
    { yyval.dtype_enum = ECPGd_scale; }
    break;

  case 1266:
#line 4676 "preproc.y"
    { yyval.dtype_enum = ECPGd_type; }
    break;

  case 1271:
#line 4689 "preproc.y"
    {  yyval.str = yyvsp[-1].str; }
    break;

  case 1272:
#line 4693 "preproc.y"
    {  yyval.descriptor.str = yyvsp[-1].str; yyval.descriptor.name = yyvsp[-3].str; }
    break;

  case 1273:
#line 4695 "preproc.y"
    {  yyval.descriptor.str = yyvsp[-1].str; yyval.descriptor.name = yyvsp[-3].str; }
    break;

  case 1274:
#line 4704 "preproc.y"
    {
			if (strcmp(yyvsp[-1].str, "begin") == 0)
							mmerror(PARSE_ERROR, ET_ERROR, "RELEASE does not make sense when beginning a transaction");

			fprintf(yyout, "ECPGtrans(__LINE__, %s, \"%s\");",
					connection ? connection : "NULL", yyvsp[-1].str);
			whenever_action(0);
			fprintf(yyout, "ECPGdisconnect(__LINE__, \"\");");
			whenever_action(0);
			free(yyvsp[-1].str);
		}
    break;

  case 1275:
#line 4721 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1276:
#line 4722 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1277:
#line 4725 "preproc.y"
    { yyval.str = make_str("on"); }
    break;

  case 1278:
#line 4726 "preproc.y"
    { yyval.str = make_str("off"); }
    break;

  case 1279:
#line 4733 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1280:
#line 4734 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1281:
#line 4741 "preproc.y"
    {
			/* reset this variable so we see if there was */
			/* an initializer specified */
			initializer = 0;
		}
    break;

  case 1282:
#line 4747 "preproc.y"
    {
			/* add entry to list */
			struct typedefs *ptr, *this;
			int dimension = yyvsp[-1].index.index1;
			int length = yyvsp[-1].index.index2;

			if ((yyvsp[-2].type.type_enum == ECPGt_struct ||
				 yyvsp[-2].type.type_enum == ECPGt_union) &&
				initializer == 1)
				mmerror(PARSE_ERROR, ET_ERROR, "Initializer not allowed in EXEC SQL TYPE command");
			else
			{
				for (ptr = types; ptr != NULL; ptr = ptr->next)
				{
					if (strcmp(yyvsp[-4].str, ptr->name) == 0)
					{
						/* re-definition is a bug */
						snprintf(errortext, sizeof(errortext), "Type %s already defined", yyvsp[-4].str);
						mmerror(PARSE_ERROR, ET_ERROR, errortext);
					}
				}

				adjust_array(yyvsp[-2].type.type_enum, &dimension, &length, yyvsp[-2].type.type_dimension, yyvsp[-2].type.type_index, *yyvsp[0].str?1:0);

				this = (struct typedefs *) mm_alloc(sizeof(struct typedefs));

				/* initial definition */
				this->next = types;
				this->name = yyvsp[-4].str;
				this->type = (struct this_type *) mm_alloc(sizeof(struct this_type));
				this->type->type_enum = yyvsp[-2].type.type_enum;
				this->type->type_str = mm_strdup(yyvsp[-4].str);
				this->type->type_dimension = dimension; /* dimension of array */
				this->type->type_index = length;	/* lenght of string */
				this->type->type_sizeof = ECPGstruct_sizeof;
				this->struct_member_list = (yyvsp[-2].type.type_enum == ECPGt_struct || yyvsp[-2].type.type_enum == ECPGt_union) ?
					struct_member_list[struct_level] : NULL;

				if (yyvsp[-2].type.type_enum != ECPGt_varchar &&
					yyvsp[-2].type.type_enum != ECPGt_char &&
					yyvsp[-2].type.type_enum != ECPGt_unsigned_char &&
					this->type->type_index >= 0)
					mmerror(PARSE_ERROR, ET_ERROR, "No multi-dimensional array support for simple data types");

				types = this;
			}

			if (auto_create_c == false)
				yyval.str = cat_str(7, make_str("/* exec sql type"), mm_strdup(yyvsp[-4].str), make_str("is"), mm_strdup(yyvsp[-2].type.type_str), mm_strdup(yyvsp[-1].index.str), yyvsp[0].str, make_str("*/"));
			else
				yyval.str = cat_str(6, make_str("typedef "), mm_strdup(yyvsp[-2].type.type_str), *yyvsp[0].str?make_str("*"):make_str(""), mm_strdup(yyvsp[-1].index.str), mm_strdup(yyvsp[-4].str), make_str(";"));
		}
    break;

  case 1283:
#line 4802 "preproc.y"
    {
			yyval.index.index1 = 0;
			yyval.index.index2 = yyvsp[0].index.index1;
			yyval.index.str = cat2_str(make_str("[]"), yyvsp[0].index.str);
		}
    break;

  case 1284:
#line 4808 "preproc.y"
    {
			yyval.index.index1 = 0;
			yyval.index.index2 = yyvsp[0].index.index1;
			yyval.index.str = cat2_str(make_str("[]"), yyvsp[0].index.str);
		}
    break;

  case 1285:
#line 4814 "preproc.y"
    {
			char *txt = mm_alloc(20L);

			sprintf (txt, "%d", yyvsp[-2].ival);
			yyval.index.index1 = yyvsp[-2].ival;
			yyval.index.index2 = yyvsp[0].index.index1;
			yyval.index.str = cat_str(4, make_str("["), txt, make_str("]"), yyvsp[0].index.str);
		}
    break;

  case 1286:
#line 4823 "preproc.y"
    {
			char *txt = mm_alloc(20L);

			sprintf (txt, "%d", yyvsp[-2].ival);
			yyval.index.index1 = yyvsp[-2].ival;
			yyval.index.index2 = yyvsp[0].index.index1;
			yyval.index.str = cat_str(4, make_str("["), txt, make_str("]"), yyvsp[0].index.str);
		}
    break;

  case 1287:
#line 4832 "preproc.y"
    {
			yyval.index.index1 = -1;
			yyval.index.index2 = -1;
			yyval.index.str= EMPTY;
		}
    break;

  case 1288:
#line 4839 "preproc.y"
    { yyval.str = make_str("reference"); }
    break;

  case 1289:
#line 4840 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 1290:
#line 4847 "preproc.y"
    {
			/* reset this variable so we see if there was */
			/* an initializer specified */
			initializer = 0;
		}
    break;

  case 1291:
#line 4853 "preproc.y"
    {
			struct variable *p = find_variable(yyvsp[-4].str);
			int dimension = yyvsp[-1].index.index1;
			int length = yyvsp[-1].index.index2;
			struct ECPGtype * type;

			if ((yyvsp[-2].type.type_enum == ECPGt_struct ||
				 yyvsp[-2].type.type_enum == ECPGt_union) &&
				initializer == 1)
				mmerror(PARSE_ERROR, ET_ERROR, "Initializer not allowed in EXEC SQL VAR command");
			else
			{
				adjust_array(yyvsp[-2].type.type_enum, &dimension, &length, yyvsp[-2].type.type_dimension, yyvsp[-2].type.type_index, *yyvsp[0].str?1:0);

				switch (yyvsp[-2].type.type_enum)
				{
					case ECPGt_struct:
					case ECPGt_union:
						if (dimension < 0)
							type = ECPGmake_struct_type(struct_member_list[struct_level], yyvsp[-2].type.type_enum, yyvsp[-2].type.type_sizeof);
						else
							type = ECPGmake_array_type(ECPGmake_struct_type(struct_member_list[struct_level], yyvsp[-2].type.type_enum,yyvsp[-2].type.type_sizeof), dimension);
						break;

					case ECPGt_varchar:
						if (dimension == -1)
							type = ECPGmake_simple_type(yyvsp[-2].type.type_enum, length);
						else
							type = ECPGmake_array_type(ECPGmake_simple_type(yyvsp[-2].type.type_enum, length), dimension);
						break;

					case ECPGt_char:
					case ECPGt_unsigned_char:
						if (dimension == -1)
							type = ECPGmake_simple_type(yyvsp[-2].type.type_enum, length);
						else
							type = ECPGmake_array_type(ECPGmake_simple_type(yyvsp[-2].type.type_enum, length), dimension);
						break;

					default:
						if (length >= 0)
							mmerror(PARSE_ERROR, ET_ERROR, "No multi-dimensional array support for simple data types");

						if (dimension < 0)
							type = ECPGmake_simple_type(yyvsp[-2].type.type_enum, 1);
						else
							type = ECPGmake_array_type(ECPGmake_simple_type(yyvsp[-2].type.type_enum, 1), dimension);
						break;
				}

				ECPGfree_type(p->type);
				p->type = type;
			}

			yyval.str = cat_str(7, make_str("/* exec sql var"), mm_strdup(yyvsp[-4].str), make_str("is"), mm_strdup(yyvsp[-2].type.type_str), mm_strdup(yyvsp[-1].index.str), yyvsp[0].str, make_str("*/"));
		}
    break;

  case 1292:
#line 4916 "preproc.y"
    {
			when_error.code = yyvsp[0].action.code;
			when_error.command = yyvsp[0].action.command;
			yyval.str = cat_str(3, make_str("/* exec sql whenever sqlerror "), yyvsp[0].action.str, make_str("; */\n"));
		}
    break;

  case 1293:
#line 4922 "preproc.y"
    {
			when_nf.code = yyvsp[0].action.code;
			when_nf.command = yyvsp[0].action.command;
			yyval.str = cat_str(3, make_str("/* exec sql whenever not found "), yyvsp[0].action.str, make_str("; */\n"));
		}
    break;

  case 1294:
#line 4928 "preproc.y"
    {
			when_warn.code = yyvsp[0].action.code;
			when_warn.command = yyvsp[0].action.command;
			yyval.str = cat_str(3, make_str("/* exec sql whenever sql_warning "), yyvsp[0].action.str, make_str("; */\n"));
		}
    break;

  case 1295:
#line 4936 "preproc.y"
    {
			yyval.action.code = W_NOTHING;
			yyval.action.command = NULL;
			yyval.action.str = make_str("continue");
		}
    break;

  case 1296:
#line 4942 "preproc.y"
    {
			yyval.action.code = W_SQLPRINT;
			yyval.action.command = NULL;
			yyval.action.str = make_str("sqlprint");
		}
    break;

  case 1297:
#line 4948 "preproc.y"
    {
			yyval.action.code = W_STOP;
			yyval.action.command = NULL;
			yyval.action.str = make_str("stop");
		}
    break;

  case 1298:
#line 4954 "preproc.y"
    {
			yyval.action.code = W_GOTO;
			yyval.action.command = strdup(yyvsp[0].str);
			yyval.action.str = cat2_str(make_str("goto "), yyvsp[0].str);
		}
    break;

  case 1299:
#line 4960 "preproc.y"
    {
			yyval.action.code = W_GOTO;
			yyval.action.command = strdup(yyvsp[0].str);
			yyval.action.str = cat2_str(make_str("goto "), yyvsp[0].str);
		}
    break;

  case 1300:
#line 4966 "preproc.y"
    {
			yyval.action.code = W_DO;
			yyval.action.command = cat_str(4, yyvsp[-3].str, make_str("("), yyvsp[-1].str, make_str(")"));
			yyval.action.str = cat2_str(make_str("do"), mm_strdup(yyval.action.command));
		}
    break;

  case 1301:
#line 4972 "preproc.y"
    {
			yyval.action.code = W_BREAK;
			yyval.action.command = NULL;
			yyval.action.str = make_str("break");
		}
    break;

  case 1302:
#line 4978 "preproc.y"
    {
			yyval.action.code = W_DO;
			yyval.action.command = cat_str(4, yyvsp[-3].str, make_str("("), yyvsp[-1].str, make_str(")"));
			yyval.action.str = cat2_str(make_str("call"), mm_strdup(yyval.action.command));
		}
    break;

  case 1303:
#line 4988 "preproc.y"
    { yyval.str = make_str("break"); }
    break;

  case 1304:
#line 4989 "preproc.y"
    { yyval.str = make_str("call"); }
    break;

  case 1305:
#line 4990 "preproc.y"
    { yyval.str = make_str("cardinality"); }
    break;

  case 1306:
#line 4991 "preproc.y"
    { yyval.str = make_str("connect"); }
    break;

  case 1307:
#line 4992 "preproc.y"
    { yyval.str = make_str("continue"); }
    break;

  case 1308:
#line 4993 "preproc.y"
    { yyval.str = make_str("count"); }
    break;

  case 1309:
#line 4994 "preproc.y"
    { yyval.str = make_str("data"); }
    break;

  case 1310:
#line 4995 "preproc.y"
    { yyval.str = make_str("datetime_interval_code"); }
    break;

  case 1311:
#line 4996 "preproc.y"
    { yyval.str = make_str("datetime_interval_precision"); }
    break;

  case 1312:
#line 4997 "preproc.y"
    { yyval.str = make_str("disconnect"); }
    break;

  case 1313:
#line 4998 "preproc.y"
    { yyval.str = make_str("found"); }
    break;

  case 1314:
#line 4999 "preproc.y"
    { yyval.str = make_str("go"); }
    break;

  case 1315:
#line 5000 "preproc.y"
    { yyval.str = make_str("goto"); }
    break;

  case 1316:
#line 5001 "preproc.y"
    { yyval.str = make_str("identified"); }
    break;

  case 1317:
#line 5002 "preproc.y"
    { yyval.str = make_str("indicator"); }
    break;

  case 1318:
#line 5003 "preproc.y"
    { yyval.str = make_str("key_member"); }
    break;

  case 1319:
#line 5004 "preproc.y"
    { yyval.str = make_str("length"); }
    break;

  case 1320:
#line 5005 "preproc.y"
    { yyval.str = make_str("name"); }
    break;

  case 1321:
#line 5006 "preproc.y"
    { yyval.str = make_str("nullable"); }
    break;

  case 1322:
#line 5007 "preproc.y"
    { yyval.str = make_str("octet_length"); }
    break;

  case 1323:
#line 5008 "preproc.y"
    { yyval.str = make_str("open"); }
    break;

  case 1324:
#line 5009 "preproc.y"
    { yyval.str = make_str("release"); }
    break;

  case 1325:
#line 5010 "preproc.y"
    { yyval.str = make_str("returned_length"); }
    break;

  case 1326:
#line 5011 "preproc.y"
    { yyval.str = make_str("returned_octet_length"); }
    break;

  case 1327:
#line 5012 "preproc.y"
    { yyval.str = make_str("scale"); }
    break;

  case 1328:
#line 5013 "preproc.y"
    { yyval.str = make_str("section"); }
    break;

  case 1329:
#line 5014 "preproc.y"
    { yyval.str = make_str("sqlerror"); }
    break;

  case 1330:
#line 5015 "preproc.y"
    { yyval.str = make_str("sqlprint"); }
    break;

  case 1331:
#line 5016 "preproc.y"
    { yyval.str = make_str("sqlwarning"); }
    break;

  case 1332:
#line 5017 "preproc.y"
    { yyval.str = make_str("stop"); }
    break;

  case 1333:
#line 5018 "preproc.y"
    { yyval.str = make_str("var"); }
    break;

  case 1334:
#line 5019 "preproc.y"
    { yyval.str = make_str("whenever"); }
    break;

  case 1335:
#line 5023 "preproc.y"
    { yyval.str = make_str("bool"); }
    break;

  case 1336:
#line 5024 "preproc.y"
    { yyval.str = make_str("long"); }
    break;

  case 1337:
#line 5025 "preproc.y"
    { yyval.str = make_str("short"); }
    break;

  case 1338:
#line 5026 "preproc.y"
    { yyval.str = make_str("struct"); }
    break;

  case 1339:
#line 5027 "preproc.y"
    { yyval.str = make_str("signed"); }
    break;

  case 1340:
#line 5028 "preproc.y"
    { yyval.str = make_str("unsigned"); }
    break;

  case 1341:
#line 5031 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1342:
#line 5032 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 1343:
#line 5035 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1344:
#line 5051 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1345:
#line 5052 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1346:
#line 5053 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1347:
#line 5054 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1348:
#line 5055 "preproc.y"
    { yyval.str = make_str("char"); }
    break;

  case 1349:
#line 5060 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1350:
#line 5061 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1351:
#line 5062 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1352:
#line 5063 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1353:
#line 5068 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1354:
#line 5069 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1355:
#line 5070 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1356:
#line 5071 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1357:
#line 5077 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1358:
#line 5078 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1359:
#line 5079 "preproc.y"
    { yyval.str = make_str("char"); }
    break;

  case 1360:
#line 5080 "preproc.y"
    { yyval.str = make_str("int"); }
    break;

  case 1361:
#line 5081 "preproc.y"
    { yyval.str = make_str("union"); }
    break;

  case 1362:
#line 5084 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1363:
#line 5085 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1364:
#line 5086 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1365:
#line 5087 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1366:
#line 5088 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1367:
#line 5089 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1368:
#line 5105 "preproc.y"
    { yyval.str = make_str("abort"); }
    break;

  case 1369:
#line 5106 "preproc.y"
    { yyval.str = make_str("absolute"); }
    break;

  case 1370:
#line 5107 "preproc.y"
    { yyval.str = make_str("access"); }
    break;

  case 1371:
#line 5108 "preproc.y"
    { yyval.str = make_str("action"); }
    break;

  case 1372:
#line 5109 "preproc.y"
    { yyval.str = make_str("add"); }
    break;

  case 1373:
#line 5110 "preproc.y"
    { yyval.str = make_str("after"); }
    break;

  case 1374:
#line 5111 "preproc.y"
    { yyval.str = make_str("aggregate"); }
    break;

  case 1375:
#line 5112 "preproc.y"
    { yyval.str = make_str("alter"); }
    break;

  case 1376:
#line 5113 "preproc.y"
    { yyval.str = make_str("assertion"); }
    break;

  case 1377:
#line 5114 "preproc.y"
    { yyval.str = make_str("assignment"); }
    break;

  case 1378:
#line 5115 "preproc.y"
    { yyval.str = make_str("at"); }
    break;

  case 1379:
#line 5116 "preproc.y"
    { yyval.str = make_str("backward"); }
    break;

  case 1380:
#line 5117 "preproc.y"
    { yyval.str = make_str("before"); }
    break;

  case 1381:
#line 5118 "preproc.y"
    { yyval.str = make_str("begin"); }
    break;

  case 1382:
#line 5119 "preproc.y"
    { yyval.str = make_str("by"); }
    break;

  case 1383:
#line 5120 "preproc.y"
    { yyval.str = make_str("cache"); }
    break;

  case 1384:
#line 5121 "preproc.y"
    { yyval.str = make_str("cascade"); }
    break;

  case 1385:
#line 5122 "preproc.y"
    { yyval.str = make_str("chain"); }
    break;

  case 1386:
#line 5123 "preproc.y"
    { yyval.str = make_str("characteristics"); }
    break;

  case 1387:
#line 5124 "preproc.y"
    { yyval.str = make_str("checkpoint"); }
    break;

  case 1388:
#line 5125 "preproc.y"
    { yyval.str = make_str("class"); }
    break;

  case 1389:
#line 5126 "preproc.y"
    { yyval.str = make_str("close"); }
    break;

  case 1390:
#line 5127 "preproc.y"
    { yyval.str = make_str("cluster"); }
    break;

  case 1391:
#line 5128 "preproc.y"
    { yyval.str = make_str("comment"); }
    break;

  case 1392:
#line 5129 "preproc.y"
    { yyval.str = make_str("commit"); }
    break;

  case 1393:
#line 5130 "preproc.y"
    { yyval.str = make_str("committed"); }
    break;

  case 1394:
#line 5131 "preproc.y"
    { yyval.str = make_str("constraints"); }
    break;

  case 1395:
#line 5132 "preproc.y"
    { yyval.str = make_str("conversion"); }
    break;

  case 1396:
#line 5133 "preproc.y"
    { yyval.str = make_str("copy"); }
    break;

  case 1397:
#line 5134 "preproc.y"
    { yyval.str = make_str("createdb"); }
    break;

  case 1398:
#line 5135 "preproc.y"
    { yyval.str = make_str("createuser"); }
    break;

  case 1399:
#line 5136 "preproc.y"
    { yyval.str = make_str("cursor"); }
    break;

  case 1400:
#line 5137 "preproc.y"
    { yyval.str = make_str("cycle"); }
    break;

  case 1401:
#line 5138 "preproc.y"
    { yyval.str = make_str("database"); }
    break;

  case 1402:
#line 5139 "preproc.y"
    { yyval.str = make_str("day"); }
    break;

  case 1403:
#line 5140 "preproc.y"
    { yyval.str = make_str("deallocate"); }
    break;

  case 1404:
#line 5141 "preproc.y"
    { yyval.str = make_str("declare"); }
    break;

  case 1405:
#line 5142 "preproc.y"
    { yyval.str = make_str("deferred"); }
    break;

  case 1406:
#line 5143 "preproc.y"
    { yyval.str = make_str("delete"); }
    break;

  case 1407:
#line 5144 "preproc.y"
    { yyval.str = make_str("delimiter"); }
    break;

  case 1408:
#line 5145 "preproc.y"
    { yyval.str = make_str("delimiters"); }
    break;

  case 1409:
#line 5146 "preproc.y"
    { yyval.str = make_str("domain"); }
    break;

  case 1410:
#line 5147 "preproc.y"
    { yyval.str = make_str("double"); }
    break;

  case 1411:
#line 5148 "preproc.y"
    { yyval.str = make_str("drop"); }
    break;

  case 1412:
#line 5149 "preproc.y"
    { yyval.str = make_str("each"); }
    break;

  case 1413:
#line 5150 "preproc.y"
    { yyval.str = make_str("encoding"); }
    break;

  case 1414:
#line 5151 "preproc.y"
    { yyval.str = make_str("encrypted"); }
    break;

  case 1415:
#line 5152 "preproc.y"
    { yyval.str = make_str("escape"); }
    break;

  case 1416:
#line 5153 "preproc.y"
    { yyval.str = make_str("exclusive"); }
    break;

  case 1417:
#line 5154 "preproc.y"
    { yyval.str = make_str("execute"); }
    break;

  case 1418:
#line 5155 "preproc.y"
    { yyval.str = make_str("explain"); }
    break;

  case 1419:
#line 5156 "preproc.y"
    { yyval.str = make_str("fetch"); }
    break;

  case 1420:
#line 5157 "preproc.y"
    { yyval.str = make_str("force"); }
    break;

  case 1421:
#line 5158 "preproc.y"
    { yyval.str = make_str("forward"); }
    break;

  case 1422:
#line 5159 "preproc.y"
    { yyval.str = make_str("function"); }
    break;

  case 1423:
#line 5160 "preproc.y"
    { yyval.str = make_str("global"); }
    break;

  case 1424:
#line 5161 "preproc.y"
    { yyval.str = make_str("handler"); }
    break;

  case 1425:
#line 5162 "preproc.y"
    { yyval.str = make_str("hour"); }
    break;

  case 1426:
#line 5163 "preproc.y"
    { yyval.str = make_str("immediate"); }
    break;

  case 1427:
#line 5164 "preproc.y"
    { yyval.str = make_str("immutable"); }
    break;

  case 1428:
#line 5165 "preproc.y"
    { yyval.str = make_str("implicit"); }
    break;

  case 1429:
#line 5166 "preproc.y"
    { yyval.str = make_str("increment"); }
    break;

  case 1430:
#line 5167 "preproc.y"
    { yyval.str = make_str("index"); }
    break;

  case 1431:
#line 5168 "preproc.y"
    { yyval.str = make_str("inherits"); }
    break;

  case 1432:
#line 5169 "preproc.y"
    { yyval.str = make_str("inout"); }
    break;

  case 1433:
#line 5170 "preproc.y"
    { yyval.str = make_str("insensitive"); }
    break;

  case 1434:
#line 5171 "preproc.y"
    { yyval.str = make_str("insert"); }
    break;

  case 1435:
#line 5172 "preproc.y"
    { yyval.str = make_str("instead"); }
    break;

  case 1436:
#line 5173 "preproc.y"
    { yyval.str = make_str("isolation"); }
    break;

  case 1437:
#line 5174 "preproc.y"
    { yyval.str = make_str("key"); }
    break;

  case 1438:
#line 5175 "preproc.y"
    { yyval.str = make_str("language"); }
    break;

  case 1439:
#line 5176 "preproc.y"
    { yyval.str = make_str("lancompiler"); }
    break;

  case 1440:
#line 5177 "preproc.y"
    { yyval.str = make_str("level"); }
    break;

  case 1441:
#line 5178 "preproc.y"
    { yyval.str = make_str("listen"); }
    break;

  case 1442:
#line 5179 "preproc.y"
    { yyval.str = make_str("load"); }
    break;

  case 1443:
#line 5180 "preproc.y"
    { yyval.str = make_str("local"); }
    break;

  case 1444:
#line 5181 "preproc.y"
    { yyval.str = make_str("location"); }
    break;

  case 1445:
#line 5182 "preproc.y"
    { yyval.str = make_str("lock"); }
    break;

  case 1446:
#line 5183 "preproc.y"
    { yyval.str = make_str("match"); }
    break;

  case 1447:
#line 5184 "preproc.y"
    { yyval.str = make_str("maxvalue"); }
    break;

  case 1448:
#line 5185 "preproc.y"
    { yyval.str = make_str("minute"); }
    break;

  case 1449:
#line 5186 "preproc.y"
    { yyval.str = make_str("minvalue"); }
    break;

  case 1450:
#line 5187 "preproc.y"
    { yyval.str = make_str("mode"); }
    break;

  case 1451:
#line 5188 "preproc.y"
    { yyval.str = make_str("month"); }
    break;

  case 1452:
#line 5189 "preproc.y"
    { yyval.str = make_str("move"); }
    break;

  case 1453:
#line 5190 "preproc.y"
    { yyval.str = make_str("names"); }
    break;

  case 1454:
#line 5191 "preproc.y"
    { yyval.str = make_str("national"); }
    break;

  case 1455:
#line 5192 "preproc.y"
    { yyval.str = make_str("next"); }
    break;

  case 1456:
#line 5193 "preproc.y"
    { yyval.str = make_str("no"); }
    break;

  case 1457:
#line 5194 "preproc.y"
    { yyval.str = make_str("nocreatedb"); }
    break;

  case 1458:
#line 5195 "preproc.y"
    { yyval.str = make_str("nocreateuser"); }
    break;

  case 1459:
#line 5196 "preproc.y"
    { yyval.str = make_str("nothing"); }
    break;

  case 1460:
#line 5197 "preproc.y"
    { yyval.str = make_str("notify"); }
    break;

  case 1461:
#line 5198 "preproc.y"
    { yyval.str = make_str("of"); }
    break;

  case 1462:
#line 5199 "preproc.y"
    { yyval.str = make_str("oids"); }
    break;

  case 1463:
#line 5200 "preproc.y"
    { yyval.str = make_str("operator"); }
    break;

  case 1464:
#line 5201 "preproc.y"
    { yyval.str = make_str("option"); }
    break;

  case 1465:
#line 5202 "preproc.y"
    { yyval.str = make_str("out"); }
    break;

  case 1466:
#line 5203 "preproc.y"
    { yyval.str = make_str("owner"); }
    break;

  case 1467:
#line 5204 "preproc.y"
    { yyval.str = make_str("partial"); }
    break;

  case 1468:
#line 5205 "preproc.y"
    { yyval.str = make_str("password"); }
    break;

  case 1469:
#line 5206 "preproc.y"
    { yyval.str = make_str("path"); }
    break;

  case 1470:
#line 5207 "preproc.y"
    { yyval.str = make_str("pendant"); }
    break;

  case 1471:
#line 5208 "preproc.y"
    { yyval.str = make_str("precision"); }
    break;

  case 1472:
#line 5209 "preproc.y"
    { yyval.str = make_str("prepare"); }
    break;

  case 1473:
#line 5210 "preproc.y"
    { yyval.str = make_str("prior"); }
    break;

  case 1474:
#line 5211 "preproc.y"
    { yyval.str = make_str("privileges"); }
    break;

  case 1475:
#line 5212 "preproc.y"
    { yyval.str = make_str("procedural"); }
    break;

  case 1476:
#line 5213 "preproc.y"
    { yyval.str = make_str("procedure"); }
    break;

  case 1477:
#line 5214 "preproc.y"
    { yyval.str = make_str("read"); }
    break;

  case 1478:
#line 5215 "preproc.y"
    { yyval.str = make_str("recheck"); }
    break;

  case 1479:
#line 5216 "preproc.y"
    { yyval.str = make_str("reindex"); }
    break;

  case 1480:
#line 5217 "preproc.y"
    { yyval.str = make_str("relative"); }
    break;

  case 1481:
#line 5218 "preproc.y"
    { yyval.str = make_str("rename"); }
    break;

  case 1482:
#line 5219 "preproc.y"
    { yyval.str = make_str("replace"); }
    break;

  case 1483:
#line 5220 "preproc.y"
    { yyval.str = make_str("reset"); }
    break;

  case 1484:
#line 5221 "preproc.y"
    { yyval.str = make_str("restrict"); }
    break;

  case 1485:
#line 5222 "preproc.y"
    { yyval.str = make_str("returns"); }
    break;

  case 1486:
#line 5223 "preproc.y"
    { yyval.str = make_str("revoke"); }
    break;

  case 1487:
#line 5224 "preproc.y"
    { yyval.str = make_str("rollback"); }
    break;

  case 1488:
#line 5225 "preproc.y"
    { yyval.str = make_str("rule"); }
    break;

  case 1489:
#line 5226 "preproc.y"
    { yyval.str = make_str("schema"); }
    break;

  case 1490:
#line 5227 "preproc.y"
    { yyval.str = make_str("scroll"); }
    break;

  case 1491:
#line 5228 "preproc.y"
    { yyval.str = make_str("second"); }
    break;

  case 1492:
#line 5229 "preproc.y"
    { yyval.str = make_str("session"); }
    break;

  case 1493:
#line 5230 "preproc.y"
    { yyval.str = make_str("sequence"); }
    break;

  case 1494:
#line 5231 "preproc.y"
    { yyval.str = make_str("serializable"); }
    break;

  case 1495:
#line 5232 "preproc.y"
    { yyval.str = make_str("set"); }
    break;

  case 1496:
#line 5233 "preproc.y"
    { yyval.str = make_str("share"); }
    break;

  case 1497:
#line 5234 "preproc.y"
    { yyval.str = make_str("show"); }
    break;

  case 1498:
#line 5235 "preproc.y"
    { yyval.str = make_str("simple"); }
    break;

  case 1499:
#line 5236 "preproc.y"
    { yyval.str = make_str("stable"); }
    break;

  case 1500:
#line 5237 "preproc.y"
    { yyval.str = make_str("start"); }
    break;

  case 1501:
#line 5238 "preproc.y"
    { yyval.str = make_str("statement"); }
    break;

  case 1502:
#line 5239 "preproc.y"
    { yyval.str = make_str("statistics"); }
    break;

  case 1503:
#line 5240 "preproc.y"
    { yyval.str = make_str("stdin"); }
    break;

  case 1504:
#line 5241 "preproc.y"
    { yyval.str = make_str("stdout"); }
    break;

  case 1505:
#line 5242 "preproc.y"
    { yyval.str = make_str("storage"); }
    break;

  case 1506:
#line 5243 "preproc.y"
    { yyval.str = make_str("sysid"); }
    break;

  case 1507:
#line 5244 "preproc.y"
    { yyval.str = make_str("temp"); }
    break;

  case 1508:
#line 5245 "preproc.y"
    { yyval.str = make_str("template"); }
    break;

  case 1509:
#line 5246 "preproc.y"
    { yyval.str = make_str("temporary"); }
    break;

  case 1510:
#line 5247 "preproc.y"
    { yyval.str = make_str("toast"); }
    break;

  case 1511:
#line 5248 "preproc.y"
    { yyval.str = make_str("transaction"); }
    break;

  case 1512:
#line 5249 "preproc.y"
    { yyval.str = make_str("trigger"); }
    break;

  case 1513:
#line 5250 "preproc.y"
    { yyval.str = make_str("truncate"); }
    break;

  case 1514:
#line 5251 "preproc.y"
    { yyval.str = make_str("trusted"); }
    break;

  case 1515:
#line 5252 "preproc.y"
    { yyval.str = make_str("type"); }
    break;

  case 1516:
#line 5253 "preproc.y"
    { yyval.str = make_str("unencrypted"); }
    break;

  case 1517:
#line 5254 "preproc.y"
    { yyval.str = make_str("unknown"); }
    break;

  case 1518:
#line 5255 "preproc.y"
    { yyval.str = make_str("unlisten"); }
    break;

  case 1519:
#line 5256 "preproc.y"
    { yyval.str = make_str("until"); }
    break;

  case 1520:
#line 5257 "preproc.y"
    { yyval.str = make_str("update"); }
    break;

  case 1521:
#line 5258 "preproc.y"
    { yyval.str = make_str("usage"); }
    break;

  case 1522:
#line 5259 "preproc.y"
    { yyval.str = make_str("vacuum"); }
    break;

  case 1523:
#line 5260 "preproc.y"
    { yyval.str = make_str("valid"); }
    break;

  case 1524:
#line 5261 "preproc.y"
    { yyval.str = make_str("values"); }
    break;

  case 1525:
#line 5262 "preproc.y"
    { yyval.str = make_str("varying"); }
    break;

  case 1526:
#line 5263 "preproc.y"
    { yyval.str = make_str("version"); }
    break;

  case 1527:
#line 5264 "preproc.y"
    { yyval.str = make_str("view"); }
    break;

  case 1528:
#line 5265 "preproc.y"
    { yyval.str = make_str("with"); }
    break;

  case 1529:
#line 5266 "preproc.y"
    { yyval.str = make_str("without"); }
    break;

  case 1530:
#line 5267 "preproc.y"
    { yyval.str = make_str("write"); }
    break;

  case 1531:
#line 5268 "preproc.y"
    { yyval.str = make_str("work"); }
    break;

  case 1532:
#line 5269 "preproc.y"
    { yyval.str = make_str("year"); }
    break;

  case 1533:
#line 5270 "preproc.y"
    { yyval.str = make_str("zone"); }
    break;

  case 1534:
#line 5284 "preproc.y"
    { yyval.str = make_str("bigint");}
    break;

  case 1535:
#line 5285 "preproc.y"
    { yyval.str = make_str("bit"); }
    break;

  case 1536:
#line 5289 "preproc.y"
    { yyval.str = make_str("character"); }
    break;

  case 1537:
#line 5290 "preproc.y"
    { yyval.str = make_str("coalesce"); }
    break;

  case 1538:
#line 5291 "preproc.y"
    { yyval.str = make_str("dec"); }
    break;

  case 1539:
#line 5292 "preproc.y"
    { yyval.str = make_str("decimal"); }
    break;

  case 1540:
#line 5293 "preproc.y"
    { yyval.str = make_str("exists"); }
    break;

  case 1541:
#line 5294 "preproc.y"
    { yyval.str = make_str("extract"); }
    break;

  case 1542:
#line 5295 "preproc.y"
    { yyval.str = make_str("float"); }
    break;

  case 1543:
#line 5299 "preproc.y"
    { yyval.str = make_str("integer"); }
    break;

  case 1544:
#line 5300 "preproc.y"
    { yyval.str = make_str("interval"); }
    break;

  case 1545:
#line 5301 "preproc.y"
    { yyval.str = make_str("nchar"); }
    break;

  case 1546:
#line 5302 "preproc.y"
    { yyval.str = make_str("none"); }
    break;

  case 1547:
#line 5303 "preproc.y"
    { yyval.str = make_str("nullif"); }
    break;

  case 1548:
#line 5304 "preproc.y"
    { yyval.str = make_str("numeric"); }
    break;

  case 1549:
#line 5305 "preproc.y"
    { yyval.str = make_str("position"); }
    break;

  case 1550:
#line 5306 "preproc.y"
    { yyval.str = make_str("real"); }
    break;

  case 1551:
#line 5307 "preproc.y"
    { yyval.str = make_str("row"); }
    break;

  case 1552:
#line 5308 "preproc.y"
    { yyval.str = make_str("setof"); }
    break;

  case 1553:
#line 5309 "preproc.y"
    { yyval.str = make_str("smallint"); }
    break;

  case 1554:
#line 5310 "preproc.y"
    { yyval.str = make_str("substring"); }
    break;

  case 1555:
#line 5311 "preproc.y"
    { yyval.str = make_str("time"); }
    break;

  case 1556:
#line 5312 "preproc.y"
    { yyval.str = make_str("timestamp"); }
    break;

  case 1557:
#line 5313 "preproc.y"
    { yyval.str = make_str("treat"); }
    break;

  case 1558:
#line 5314 "preproc.y"
    { yyval.str = make_str("trim"); }
    break;

  case 1559:
#line 5315 "preproc.y"
    { yyval.str = make_str("varchar"); }
    break;

  case 1560:
#line 5329 "preproc.y"
    { yyval.str = make_str("authorization"); }
    break;

  case 1561:
#line 5330 "preproc.y"
    { yyval.str = make_str("between"); }
    break;

  case 1562:
#line 5331 "preproc.y"
    { yyval.str = make_str("binary"); }
    break;

  case 1563:
#line 5332 "preproc.y"
    { yyval.str = make_str("cross"); }
    break;

  case 1564:
#line 5333 "preproc.y"
    { yyval.str = make_str("freeze"); }
    break;

  case 1565:
#line 5334 "preproc.y"
    { yyval.str = make_str("full"); }
    break;

  case 1566:
#line 5335 "preproc.y"
    { yyval.str = make_str("ilike"); }
    break;

  case 1567:
#line 5336 "preproc.y"
    { yyval.str = make_str("in"); }
    break;

  case 1568:
#line 5337 "preproc.y"
    { yyval.str = make_str("inner"); }
    break;

  case 1569:
#line 5338 "preproc.y"
    { yyval.str = make_str("is"); }
    break;

  case 1570:
#line 5339 "preproc.y"
    { yyval.str = make_str("isnull"); }
    break;

  case 1571:
#line 5340 "preproc.y"
    { yyval.str = make_str("join"); }
    break;

  case 1572:
#line 5341 "preproc.y"
    { yyval.str = make_str("left"); }
    break;

  case 1573:
#line 5342 "preproc.y"
    { yyval.str = make_str("like"); }
    break;

  case 1574:
#line 5343 "preproc.y"
    { yyval.str = make_str("natural"); }
    break;

  case 1575:
#line 5344 "preproc.y"
    { yyval.str = make_str("notnull"); }
    break;

  case 1576:
#line 5345 "preproc.y"
    { yyval.str = make_str("outer"); }
    break;

  case 1577:
#line 5346 "preproc.y"
    { yyval.str = make_str("overlaps"); }
    break;

  case 1578:
#line 5347 "preproc.y"
    { yyval.str = make_str("right"); }
    break;

  case 1579:
#line 5348 "preproc.y"
    { yyval.str = make_str("similar"); }
    break;

  case 1580:
#line 5349 "preproc.y"
    { yyval.str = make_str("verbose"); }
    break;

  case 1581:
#line 5359 "preproc.y"
    { yyval.str = make_str("all"); }
    break;

  case 1582:
#line 5360 "preproc.y"
    { yyval.str = make_str("analyse"); }
    break;

  case 1583:
#line 5361 "preproc.y"
    { yyval.str = make_str("analyze"); }
    break;

  case 1584:
#line 5362 "preproc.y"
    { yyval.str = make_str("and"); }
    break;

  case 1585:
#line 5363 "preproc.y"
    { yyval.str = make_str("any"); }
    break;

  case 1586:
#line 5364 "preproc.y"
    { yyval.str = make_str("as"); }
    break;

  case 1587:
#line 5365 "preproc.y"
    { yyval.str = make_str("asc"); }
    break;

  case 1588:
#line 5366 "preproc.y"
    { yyval.str = make_str("both"); }
    break;

  case 1589:
#line 5367 "preproc.y"
    { yyval.str = make_str("case"); }
    break;

  case 1590:
#line 5368 "preproc.y"
    { yyval.str = make_str("cast"); }
    break;

  case 1591:
#line 5369 "preproc.y"
    { yyval.str = make_str("check"); }
    break;

  case 1592:
#line 5370 "preproc.y"
    { yyval.str = make_str("collate"); }
    break;

  case 1593:
#line 5371 "preproc.y"
    { yyval.str = make_str("column"); }
    break;

  case 1594:
#line 5372 "preproc.y"
    { yyval.str = make_str("constraint"); }
    break;

  case 1595:
#line 5373 "preproc.y"
    { yyval.str = make_str("create"); }
    break;

  case 1596:
#line 5374 "preproc.y"
    { yyval.str = make_str("current_date"); }
    break;

  case 1597:
#line 5375 "preproc.y"
    { yyval.str = make_str("current_time"); }
    break;

  case 1598:
#line 5376 "preproc.y"
    { yyval.str = make_str("current_timestamp"); }
    break;

  case 1599:
#line 5377 "preproc.y"
    { yyval.str = make_str("current_user"); }
    break;

  case 1600:
#line 5378 "preproc.y"
    { yyval.str = make_str("default"); }
    break;

  case 1601:
#line 5379 "preproc.y"
    { yyval.str = make_str("deferrable"); }
    break;

  case 1602:
#line 5380 "preproc.y"
    { yyval.str = make_str("desc"); }
    break;

  case 1603:
#line 5381 "preproc.y"
    { yyval.str = make_str("distinct"); }
    break;

  case 1604:
#line 5382 "preproc.y"
    { yyval.str = make_str("do"); }
    break;

  case 1605:
#line 5383 "preproc.y"
    { yyval.str = make_str("else"); }
    break;

  case 1606:
#line 5384 "preproc.y"
    { yyval.str = make_str("end"); }
    break;

  case 1607:
#line 5385 "preproc.y"
    { yyval.str = make_str("except"); }
    break;

  case 1608:
#line 5386 "preproc.y"
    { yyval.str = make_str("false"); }
    break;

  case 1609:
#line 5387 "preproc.y"
    { yyval.str = make_str("for"); }
    break;

  case 1610:
#line 5388 "preproc.y"
    { yyval.str = make_str("foreign"); }
    break;

  case 1611:
#line 5389 "preproc.y"
    { yyval.str = make_str("from"); }
    break;

  case 1612:
#line 5390 "preproc.y"
    { yyval.str = make_str("grant"); }
    break;

  case 1613:
#line 5391 "preproc.y"
    { yyval.str = make_str("group"); }
    break;

  case 1614:
#line 5392 "preproc.y"
    { yyval.str = make_str("having"); }
    break;

  case 1615:
#line 5393 "preproc.y"
    { yyval.str = make_str("initially"); }
    break;

  case 1616:
#line 5394 "preproc.y"
    { yyval.str = make_str("intersect"); }
    break;

  case 1617:
#line 5395 "preproc.y"
    { yyval.str = make_str("into"); }
    break;

  case 1618:
#line 5396 "preproc.y"
    { yyval.str = make_str("leading"); }
    break;

  case 1619:
#line 5397 "preproc.y"
    { yyval.str = make_str("limit"); }
    break;

  case 1620:
#line 5398 "preproc.y"
    { yyval.str = make_str("new"); }
    break;

  case 1621:
#line 5399 "preproc.y"
    { yyval.str = make_str("not"); }
    break;

  case 1622:
#line 5400 "preproc.y"
    { yyval.str = make_str("null"); }
    break;

  case 1623:
#line 5401 "preproc.y"
    { yyval.str = make_str("off"); }
    break;

  case 1624:
#line 5402 "preproc.y"
    { yyval.str = make_str("offset"); }
    break;

  case 1625:
#line 5403 "preproc.y"
    { yyval.str = make_str("old"); }
    break;

  case 1626:
#line 5404 "preproc.y"
    { yyval.str = make_str("on"); }
    break;

  case 1627:
#line 5405 "preproc.y"
    { yyval.str = make_str("only"); }
    break;

  case 1628:
#line 5406 "preproc.y"
    { yyval.str = make_str("or"); }
    break;

  case 1629:
#line 5407 "preproc.y"
    { yyval.str = make_str("order"); }
    break;

  case 1630:
#line 5408 "preproc.y"
    { yyval.str = make_str("primary"); }
    break;

  case 1631:
#line 5409 "preproc.y"
    { yyval.str = make_str("references"); }
    break;

  case 1632:
#line 5410 "preproc.y"
    { yyval.str = make_str("select"); }
    break;

  case 1633:
#line 5411 "preproc.y"
    { yyval.str = make_str("session_user"); }
    break;

  case 1634:
#line 5412 "preproc.y"
    { yyval.str = make_str("some"); }
    break;

  case 1635:
#line 5413 "preproc.y"
    { yyval.str = make_str("table"); }
    break;

  case 1636:
#line 5414 "preproc.y"
    { yyval.str = make_str("then"); }
    break;

  case 1637:
#line 5415 "preproc.y"
    { yyval.str = make_str("to"); }
    break;

  case 1638:
#line 5416 "preproc.y"
    { yyval.str = make_str("trailing"); }
    break;

  case 1639:
#line 5417 "preproc.y"
    { yyval.str = make_str("true"); }
    break;

  case 1640:
#line 5421 "preproc.y"
    { yyval.str = make_str("unique"); }
    break;

  case 1641:
#line 5422 "preproc.y"
    { yyval.str = make_str("user"); }
    break;

  case 1642:
#line 5423 "preproc.y"
    { yyval.str = make_str("using"); }
    break;

  case 1643:
#line 5424 "preproc.y"
    { yyval.str = make_str("when"); }
    break;

  case 1644:
#line 5425 "preproc.y"
    { yyval.str = make_str("where"); }
    break;

  case 1647:
#line 5432 "preproc.y"
    { reset_variables(); }
    break;

  case 1648:
#line 5435 "preproc.y"
    { yyval.str = EMPTY; }
    break;

  case 1649:
#line 5436 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1650:
#line 5440 "preproc.y"
    { add_variable(&argsresult, find_variable(yyvsp[-1].str), find_variable(yyvsp[0].str)); }
    break;

  case 1651:
#line 5442 "preproc.y"
    { add_variable(&argsresult, find_variable(yyvsp[0].str), &no_indicator); }
    break;

  case 1652:
#line 5447 "preproc.y"
    {
			if (yyvsp[0].str != NULL && (find_variable(yyvsp[0].str))->type->type == ECPGt_array)
				mmerror(PARSE_ERROR, ET_ERROR, "arrays of indicators are not allowed on input");

			add_variable(&argsinsert, find_variable(yyvsp[-1].str), (yyvsp[0].str == NULL) ? &no_indicator : find_variable(yyvsp[0].str));
		}
    break;

  case 1653:
#line 5456 "preproc.y"
    {
			add_variable(&argsinsert, find_variable(yyvsp[0].str), &no_indicator);
			yyval.str = yyvsp[0].str;
		}
    break;

  case 1654:
#line 5462 "preproc.y"
    { check_indicator((find_variable(yyvsp[0].str))->type); yyval.str = yyvsp[0].str; }
    break;

  case 1655:
#line 5463 "preproc.y"
    { check_indicator((find_variable(yyvsp[0].str))->type); yyval.str = yyvsp[0].str; }
    break;

  case 1656:
#line 5464 "preproc.y"
    { check_indicator((find_variable(yyvsp[0].str))->type); yyval.str = yyvsp[0].str; }
    break;

  case 1657:
#line 5467 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1658:
#line 5468 "preproc.y"
    { yyval.str = make3_str(make_str("\""), yyvsp[0].str, make_str("\"")); }
    break;

  case 1659:
#line 5472 "preproc.y"
    { yyval.str = make3_str(make_str("\""), yyvsp[0].str, make_str("\"")); }
    break;

  case 1660:
#line 5474 "preproc.y"
    { yyval.str = make3_str(make_str("("), yyvsp[0].str, make_str(")")); }
    break;

  case 1661:
#line 5481 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1662:
#line 5482 "preproc.y"
    { yyval.str = make_str("()"); }
    break;

  case 1663:
#line 5484 "preproc.y"
    { yyval.str = cat_str(3, make_str("("), yyvsp[-1].str, make_str(")")); }
    break;

  case 1664:
#line 5487 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1665:
#line 5489 "preproc.y"
    { yyval.str = cat2_str(yyvsp[-1].str, yyvsp[0].str); }
    break;

  case 1666:
#line 5492 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1667:
#line 5493 "preproc.y"
    { yyval.str = cat_str(3, yyvsp[-2].str, make_str(","), yyvsp[0].str); }
    break;

  case 1668:
#line 5496 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1669:
#line 5497 "preproc.y"
    { yyval.str = cat_str(3, make_str("{"), yyvsp[-1].str, make_str("}")); }
    break;

  case 1670:
#line 5500 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1671:
#line 5501 "preproc.y"
    { yyval.str = make_str("("); }
    break;

  case 1672:
#line 5502 "preproc.y"
    { yyval.str = make_str(")"); }
    break;

  case 1673:
#line 5503 "preproc.y"
    { yyval.str = make_str(","); }
    break;

  case 1674:
#line 5504 "preproc.y"
    { yyval.str = make_str(";"); }
    break;

  case 1675:
#line 5507 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1676:
#line 5508 "preproc.y"
    { yyval.str = make3_str(make_str("\""), yyvsp[0].str, make_str("\"")); }
    break;

  case 1677:
#line 5509 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1678:
#line 5510 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1679:
#line 5511 "preproc.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1680:
#line 5512 "preproc.y"
    { yyval.str = make_str("*"); }
    break;

  case 1681:
#line 5513 "preproc.y"
    { yyval.str = make_str("+"); }
    break;

  case 1682:
#line 5514 "preproc.y"
    { yyval.str = make_str("-"); }
    break;

  case 1683:
#line 5515 "preproc.y"
    { yyval.str = make_str("/"); }
    break;

  case 1684:
#line 5516 "preproc.y"
    { yyval.str = make_str("%"); }
    break;

  case 1685:
#line 5517 "preproc.y"
    { yyval.str = make_str("NULL"); }
    break;

  case 1686:
#line 5518 "preproc.y"
    { yyval.str = make_str("+="); }
    break;

  case 1687:
#line 5519 "preproc.y"
    { yyval.str = make_str("&&"); }
    break;

  case 1688:
#line 5520 "preproc.y"
    { yyval.str = make_name(); }
    break;

  case 1689:
#line 5521 "preproc.y"
    { yyval.str = make_str("auto"); }
    break;

  case 1690:
#line 5522 "preproc.y"
    { yyval.str = make_str("const"); }
    break;

  case 1691:
#line 5523 "preproc.y"
    { yyval.str = make_str("--"); }
    break;

  case 1692:
#line 5524 "preproc.y"
    { yyval.str = make_str("/="); }
    break;

  case 1693:
#line 5525 "preproc.y"
    { yyval.str = make_str(".*"); }
    break;

  case 1694:
#line 5526 "preproc.y"
    { yyval.str = make_str("=="); }
    break;

  case 1695:
#line 5527 "preproc.y"
    { yyval.str = make_str("extern"); }
    break;

  case 1696:
#line 5528 "preproc.y"
    { yyval.str = make_str("++"); }
    break;

  case 1697:
#line 5529 "preproc.y"
    { yyval.str = make_str("<<"); }
    break;

  case 1698:
#line 5530 "preproc.y"
    { yyval.str = make_str("->"); }
    break;

  case 1699:
#line 5531 "preproc.y"
    { yyval.str = make_str("->*"); }
    break;

  case 1700:
#line 5532 "preproc.y"
    { yyval.str = make_str("%="); }
    break;

  case 1701:
#line 5533 "preproc.y"
    { yyval.str = make_str("*="); }
    break;

  case 1702:
#line 5534 "preproc.y"
    { yyval.str = make_str("!="); }
    break;

  case 1703:
#line 5535 "preproc.y"
    { yyval.str = make_str("||"); }
    break;

  case 1704:
#line 5536 "preproc.y"
    { yyval.str = make_str("register"); }
    break;

  case 1705:
#line 5537 "preproc.y"
    { yyval.str = make_str(">>"); }
    break;

  case 1706:
#line 5538 "preproc.y"
    { yyval.str = make_str("static"); }
    break;

  case 1707:
#line 5539 "preproc.y"
    { yyval.str = make_str("-="); }
    break;

  case 1708:
#line 5540 "preproc.y"
    { yyval.str = make_str("typedef"); }
    break;

  case 1709:
#line 5541 "preproc.y"
    { yyval.str = make_str("bool"); }
    break;

  case 1710:
#line 5542 "preproc.y"
    { yyval.str = make_str("enum"); }
    break;

  case 1711:
#line 5543 "preproc.y"
    { yyval.str = make_str("int"); }
    break;

  case 1712:
#line 5544 "preproc.y"
    { yyval.str = make_str("long"); }
    break;

  case 1713:
#line 5545 "preproc.y"
    { yyval.str = make_str("short"); }
    break;

  case 1714:
#line 5546 "preproc.y"
    { yyval.str = make_str("signed"); }
    break;

  case 1715:
#line 5547 "preproc.y"
    { yyval.str = make_str("struct"); }
    break;

  case 1716:
#line 5548 "preproc.y"
    { yyval.str = make_str("unsigned"); }
    break;

  case 1717:
#line 5549 "preproc.y"
    { yyval.str = make_str("char"); }
    break;

  case 1718:
#line 5550 "preproc.y"
    { yyval.str = make_str("double"); }
    break;

  case 1719:
#line 5551 "preproc.y"
    { yyval.str = make_str("float"); }
    break;

  case 1720:
#line 5552 "preproc.y"
    { yyval.str = make_str("union"); }
    break;

  case 1721:
#line 5553 "preproc.y"
    { yyval.str = make_str("varchar"); }
    break;

  case 1722:
#line 5554 "preproc.y"
    { yyval.str = make_str("["); }
    break;

  case 1723:
#line 5555 "preproc.y"
    { yyval.str = make_str("]"); }
    break;

  case 1724:
#line 5556 "preproc.y"
    { yyval.str = make_str("="); }
    break;


    }

/* Line 999 of yacc.c.  */
#line 21427 "y.tab.c"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("syntax error, unexpected ") + 1;
	  yysize += yystrlen (yytname[yytype]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* Return failure if at end of input.  */
      if (yychar == YYEOF)
        {
	  /* Pop the error token.  */
          YYPOPSTACK;
	  /* Pop the rest of the stack.  */
	  while (yyss < yyssp)
	    {
	      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
	      yydestruct (yystos[*yyssp], yyvsp);
	      YYPOPSTACK;
	    }
	  YYABORT;
        }

      YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
      yydestruct (yytoken, &yylval);
      yychar = YYEMPTY;

    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*----------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp);
      yyvsp--;
      yystate = *--yyssp;

      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 5559 "preproc.y"


void yyerror( char * error)
{
	char buf[1024];

	snprintf(buf,sizeof buf,"%s at or near \"%s\"", error, token_start ? token_start : yytext);
	buf[sizeof(buf)-1]=0;
	mmerror(PARSE_ERROR, ET_ERROR, buf);
}

#include "pgc.c"

