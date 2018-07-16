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
     ABORT_TRANS = 258,
     ABSOLUTE = 259,
     ACCESS = 260,
     ACTION = 261,
     ADD = 262,
     AFTER = 263,
     AGGREGATE = 264,
     ALL = 265,
     ALTER = 266,
     ANALYSE = 267,
     ANALYZE = 268,
     AND = 269,
     ANY = 270,
     AS = 271,
     ASC = 272,
     ASSERTION = 273,
     ASSIGNMENT = 274,
     AT = 275,
     AUTHORIZATION = 276,
     BACKWARD = 277,
     BEFORE = 278,
     BEGIN_TRANS = 279,
     BETWEEN = 280,
     BIGINT = 281,
     BINARY = 282,
     BIT = 283,
     BOOLEAN = 284,
     BOTH = 285,
     BY = 286,
     CACHE = 287,
     CALLED = 288,
     CASCADE = 289,
     CASE = 290,
     CAST = 291,
     CHAIN = 292,
     CHAR_P = 293,
     CHARACTER = 294,
     CHARACTERISTICS = 295,
     CHECK = 296,
     CHECKPOINT = 297,
     CLASS = 298,
     CLOSE = 299,
     CLUSTER = 300,
     COALESCE = 301,
     COLLATE = 302,
     COLUMN = 303,
     COMMENT = 304,
     COMMIT = 305,
     COMMITTED = 306,
     CONSTRAINT = 307,
     CONSTRAINTS = 308,
     CONVERSION_P = 309,
     CONVERT = 310,
     COPY = 311,
     CQEDDY = 312,
     CREATE = 313,
     CREATEDB = 314,
     CREATEUSER = 315,
     CROSS = 316,
     CURRENT_DATE = 317,
     CURRENT_TIME = 318,
     CURRENT_TIMESTAMP = 319,
     CURRENT_USER = 320,
     CURSOR = 321,
     CYCLE = 322,
     DATABASE = 323,
     DAY_P = 324,
     DEALLOCATE = 325,
     DEC = 326,
     DECIMAL = 327,
     DECLARE = 328,
     DEFAULT = 329,
     DEFERRABLE = 330,
     DEFERRED = 331,
     DEFINER = 332,
     DELETE_P = 333,
     DELIMITER = 334,
     DELIMITERS = 335,
     DESC = 336,
     DISTINCT = 337,
     DO = 338,
     DOMAIN_P = 339,
     DOUBLE = 340,
     DROP = 341,
     EACH = 342,
     ELSE = 343,
     ENCODING = 344,
     ENCRYPTED = 345,
     END_TRANS = 346,
     ESCAPE = 347,
     EXCEPT = 348,
     EXCLUSIVE = 349,
     EXECUTE = 350,
     EXISTS = 351,
     EXPLAIN = 352,
     EXTERNAL = 353,
     EXTRACT = 354,
     FALSE_P = 355,
     FETCH = 356,
     FLOAT_P = 357,
     FOR = 358,
     FORCE = 359,
     FOREIGN = 360,
     FORWARD = 361,
     FREEZE = 362,
     FROM = 363,
     FULL = 364,
     FUNCTION = 365,
     GET = 366,
     GLOBAL = 367,
     GRANT = 368,
     GROUP_P = 369,
     HANDLER = 370,
     HAVING = 371,
     HOUR_P = 372,
     ILIKE = 373,
     IMMEDIATE = 374,
     IMMUTABLE = 375,
     IMPLICIT_P = 376,
     IN_P = 377,
     INCREMENT = 378,
     INDEX = 379,
     INHERITS = 380,
     INITIALLY = 381,
     INNER_P = 382,
     INOUT = 383,
     INPUT = 384,
     INSENSITIVE = 385,
     INSERT = 386,
     INSTEAD = 387,
     INT = 388,
     INTEGER = 389,
     INTERSECT = 390,
     INTERVAL = 391,
     INTO = 392,
     INVOKER = 393,
     IS = 394,
     ISNULL = 395,
     ISOLATION = 396,
     JOIN = 397,
     KEY = 398,
     LANCOMPILER = 399,
     LANGUAGE = 400,
     LEADING = 401,
     LEFT = 402,
     LEVEL = 403,
     LIKE = 404,
     LIMIT = 405,
     LISTEN = 406,
     LOAD = 407,
     LOCAL = 408,
     LOCALTIME = 409,
     LOCALTIMESTAMP = 410,
     LOCATION = 411,
     LOCK_P = 412,
     MATCH = 413,
     MAXVALUE = 414,
     MINUTE_P = 415,
     MINVALUE = 416,
     MODE = 417,
     MONTH_P = 418,
     MOVE = 419,
     NAMES = 420,
     NATIONAL = 421,
     NATURAL = 422,
     NCHAR = 423,
     NEW = 424,
     NEXT = 425,
     NO = 426,
     NOCREATEDB = 427,
     NOCREATEUSER = 428,
     NONE = 429,
     NOT = 430,
     NOTHING = 431,
     NOTIFY = 432,
     NOTNULL = 433,
     NULL_P = 434,
     NULLIF = 435,
     NUMERIC = 436,
     OF = 437,
     OFF = 438,
     OFFSET = 439,
     OIDS = 440,
     OLD = 441,
     ON = 442,
     ONLY = 443,
     OPERATOR = 444,
     OPTION = 445,
     OR = 446,
     ORDER = 447,
     OUT_P = 448,
     OUTER_P = 449,
     OVERLAPS = 450,
     OVERLAY = 451,
     OWNER = 452,
     PARTIAL = 453,
     PASSWORD = 454,
     PATH_P = 455,
     PENDANT = 456,
     PLACING = 457,
     POSITION = 458,
     PRECISION = 459,
     PREPARE = 460,
     PRIMARY = 461,
     PRIOR = 462,
     PRIVILEGES = 463,
     PROCEDURAL = 464,
     PROCEDURE = 465,
     READ = 466,
     REAL = 467,
     RECHECK = 468,
     REFERENCES = 469,
     REINDEX = 470,
     RELATIVE = 471,
     RENAME = 472,
     REPLACE = 473,
     RESET = 474,
     RESTRICT = 475,
     RETURNS = 476,
     REVOKE = 477,
     RIGHT = 478,
     ROLLBACK = 479,
     ROW = 480,
     RULE = 481,
     SCHEMA = 482,
     SCROLL = 483,
     SECOND_P = 484,
     SECURITY = 485,
     SELECT = 486,
     SEQUENCE = 487,
     SERIALIZABLE = 488,
     SESSION = 489,
     SESSION_USER = 490,
     SET = 491,
     SETOF = 492,
     SHARE = 493,
     SHOW = 494,
     SIMILAR = 495,
     SIMPLE = 496,
     SMALLINT = 497,
     SOME = 498,
     STABLE = 499,
     START = 500,
     STATEMENT = 501,
     STATISTICS = 502,
     STDIN = 503,
     STDOUT = 504,
     STORAGE = 505,
     STRICT = 506,
     SUBSTRING = 507,
     SYSID = 508,
     TABLE = 509,
     TEMP = 510,
     TEMPLATE = 511,
     TEMPORARY = 512,
     THEN = 513,
     TIME = 514,
     TIMESTAMP = 515,
     TO = 516,
     TOAST = 517,
     TRAILING = 518,
     TRANSACTION = 519,
     TREAT = 520,
     TRIGGER = 521,
     TRIM = 522,
     TRUE_P = 523,
     TRUNCATE = 524,
     TRUSTED = 525,
     TYPE_P = 526,
     UNENCRYPTED = 527,
     UNION = 528,
     UNIQUE = 529,
     UNKNOWN = 530,
     UNLISTEN = 531,
     UNTIL = 532,
     UPDATE = 533,
     USAGE = 534,
     USER = 535,
     USING = 536,
     VACUUM = 537,
     VALID = 538,
     VALIDATOR = 539,
     VALUES = 540,
     VARCHAR = 541,
     VARYING = 542,
     VERBOSE = 543,
     VERSION = 544,
     VIEW = 545,
     VOLATILE = 546,
     WHEN = 547,
     WHERE = 548,
     WITH = 549,
     WITHOUT = 550,
     WORK = 551,
     WRITE = 552,
     YEAR_P = 553,
     ZONE = 554,
     SLIDE = 555,
     SLIDETIMEOUT = 556,
     RANGE = 557,
     WINDOW = 558,
     ARCHIVED = 559,
     UNARCHIVED = 560,
     UNARCHIVED_DROP = 561,
     UNARCHIVED_SPILL = 562,
     TIMESTAMPCOLUMN = 563,
     STREAM = 564,
     WRAPPER = 565,
     WRAPPERINFO = 566,
     DECLUST = 567,
     BINDINGS = 568,
     RECURSIVE_RESULT = 569,
     RECURSIVE_NEXT = 570,
     ISTREAM = 571,
     RSTREAM = 572,
     DSTREAM = 573,
     OPTIONAL = 574,
     OVERLOAD = 575,
     KEEP = 576,
     COUNTS = 577,
     MYHIST = 578,
     BLOCK = 579,
     REGHIST = 580,
     WAVELET = 581,
     SAMPLE = 582,
     EVENT = 583,
     PARTITION = 584,
     ANYONE = 585,
     FOLLOWS = 586,
     NOTFOLLOWS = 587,
     PRECEDES = 588,
     NOTPRECEDES = 589,
     SLICES = 590,
     UNIONJOIN = 591,
     IDENT = 592,
     FCONST = 593,
     SCONST = 594,
     NCONST = 595,
     BCONST = 596,
     XCONST = 597,
     Op = 598,
     ICONST = 599,
     PARAM = 600,
     OP = 601,
     POSTFIXOP = 602,
     UMINUS = 603,
     TYPECAST = 604
   };
#endif
#define ABORT_TRANS 258
#define ABSOLUTE 259
#define ACCESS 260
#define ACTION 261
#define ADD 262
#define AFTER 263
#define AGGREGATE 264
#define ALL 265
#define ALTER 266
#define ANALYSE 267
#define ANALYZE 268
#define AND 269
#define ANY 270
#define AS 271
#define ASC 272
#define ASSERTION 273
#define ASSIGNMENT 274
#define AT 275
#define AUTHORIZATION 276
#define BACKWARD 277
#define BEFORE 278
#define BEGIN_TRANS 279
#define BETWEEN 280
#define BIGINT 281
#define BINARY 282
#define BIT 283
#define BOOLEAN 284
#define BOTH 285
#define BY 286
#define CACHE 287
#define CALLED 288
#define CASCADE 289
#define CASE 290
#define CAST 291
#define CHAIN 292
#define CHAR_P 293
#define CHARACTER 294
#define CHARACTERISTICS 295
#define CHECK 296
#define CHECKPOINT 297
#define CLASS 298
#define CLOSE 299
#define CLUSTER 300
#define COALESCE 301
#define COLLATE 302
#define COLUMN 303
#define COMMENT 304
#define COMMIT 305
#define COMMITTED 306
#define CONSTRAINT 307
#define CONSTRAINTS 308
#define CONVERSION_P 309
#define CONVERT 310
#define COPY 311
#define CQEDDY 312
#define CREATE 313
#define CREATEDB 314
#define CREATEUSER 315
#define CROSS 316
#define CURRENT_DATE 317
#define CURRENT_TIME 318
#define CURRENT_TIMESTAMP 319
#define CURRENT_USER 320
#define CURSOR 321
#define CYCLE 322
#define DATABASE 323
#define DAY_P 324
#define DEALLOCATE 325
#define DEC 326
#define DECIMAL 327
#define DECLARE 328
#define DEFAULT 329
#define DEFERRABLE 330
#define DEFERRED 331
#define DEFINER 332
#define DELETE_P 333
#define DELIMITER 334
#define DELIMITERS 335
#define DESC 336
#define DISTINCT 337
#define DO 338
#define DOMAIN_P 339
#define DOUBLE 340
#define DROP 341
#define EACH 342
#define ELSE 343
#define ENCODING 344
#define ENCRYPTED 345
#define END_TRANS 346
#define ESCAPE 347
#define EXCEPT 348
#define EXCLUSIVE 349
#define EXECUTE 350
#define EXISTS 351
#define EXPLAIN 352
#define EXTERNAL 353
#define EXTRACT 354
#define FALSE_P 355
#define FETCH 356
#define FLOAT_P 357
#define FOR 358
#define FORCE 359
#define FOREIGN 360
#define FORWARD 361
#define FREEZE 362
#define FROM 363
#define FULL 364
#define FUNCTION 365
#define GET 366
#define GLOBAL 367
#define GRANT 368
#define GROUP_P 369
#define HANDLER 370
#define HAVING 371
#define HOUR_P 372
#define ILIKE 373
#define IMMEDIATE 374
#define IMMUTABLE 375
#define IMPLICIT_P 376
#define IN_P 377
#define INCREMENT 378
#define INDEX 379
#define INHERITS 380
#define INITIALLY 381
#define INNER_P 382
#define INOUT 383
#define INPUT 384
#define INSENSITIVE 385
#define INSERT 386
#define INSTEAD 387
#define INT 388
#define INTEGER 389
#define INTERSECT 390
#define INTERVAL 391
#define INTO 392
#define INVOKER 393
#define IS 394
#define ISNULL 395
#define ISOLATION 396
#define JOIN 397
#define KEY 398
#define LANCOMPILER 399
#define LANGUAGE 400
#define LEADING 401
#define LEFT 402
#define LEVEL 403
#define LIKE 404
#define LIMIT 405
#define LISTEN 406
#define LOAD 407
#define LOCAL 408
#define LOCALTIME 409
#define LOCALTIMESTAMP 410
#define LOCATION 411
#define LOCK_P 412
#define MATCH 413
#define MAXVALUE 414
#define MINUTE_P 415
#define MINVALUE 416
#define MODE 417
#define MONTH_P 418
#define MOVE 419
#define NAMES 420
#define NATIONAL 421
#define NATURAL 422
#define NCHAR 423
#define NEW 424
#define NEXT 425
#define NO 426
#define NOCREATEDB 427
#define NOCREATEUSER 428
#define NONE 429
#define NOT 430
#define NOTHING 431
#define NOTIFY 432
#define NOTNULL 433
#define NULL_P 434
#define NULLIF 435
#define NUMERIC 436
#define OF 437
#define OFF 438
#define OFFSET 439
#define OIDS 440
#define OLD 441
#define ON 442
#define ONLY 443
#define OPERATOR 444
#define OPTION 445
#define OR 446
#define ORDER 447
#define OUT_P 448
#define OUTER_P 449
#define OVERLAPS 450
#define OVERLAY 451
#define OWNER 452
#define PARTIAL 453
#define PASSWORD 454
#define PATH_P 455
#define PENDANT 456
#define PLACING 457
#define POSITION 458
#define PRECISION 459
#define PREPARE 460
#define PRIMARY 461
#define PRIOR 462
#define PRIVILEGES 463
#define PROCEDURAL 464
#define PROCEDURE 465
#define READ 466
#define REAL 467
#define RECHECK 468
#define REFERENCES 469
#define REINDEX 470
#define RELATIVE 471
#define RENAME 472
#define REPLACE 473
#define RESET 474
#define RESTRICT 475
#define RETURNS 476
#define REVOKE 477
#define RIGHT 478
#define ROLLBACK 479
#define ROW 480
#define RULE 481
#define SCHEMA 482
#define SCROLL 483
#define SECOND_P 484
#define SECURITY 485
#define SELECT 486
#define SEQUENCE 487
#define SERIALIZABLE 488
#define SESSION 489
#define SESSION_USER 490
#define SET 491
#define SETOF 492
#define SHARE 493
#define SHOW 494
#define SIMILAR 495
#define SIMPLE 496
#define SMALLINT 497
#define SOME 498
#define STABLE 499
#define START 500
#define STATEMENT 501
#define STATISTICS 502
#define STDIN 503
#define STDOUT 504
#define STORAGE 505
#define STRICT 506
#define SUBSTRING 507
#define SYSID 508
#define TABLE 509
#define TEMP 510
#define TEMPLATE 511
#define TEMPORARY 512
#define THEN 513
#define TIME 514
#define TIMESTAMP 515
#define TO 516
#define TOAST 517
#define TRAILING 518
#define TRANSACTION 519
#define TREAT 520
#define TRIGGER 521
#define TRIM 522
#define TRUE_P 523
#define TRUNCATE 524
#define TRUSTED 525
#define TYPE_P 526
#define UNENCRYPTED 527
#define UNION 528
#define UNIQUE 529
#define UNKNOWN 530
#define UNLISTEN 531
#define UNTIL 532
#define UPDATE 533
#define USAGE 534
#define USER 535
#define USING 536
#define VACUUM 537
#define VALID 538
#define VALIDATOR 539
#define VALUES 540
#define VARCHAR 541
#define VARYING 542
#define VERBOSE 543
#define VERSION 544
#define VIEW 545
#define VOLATILE 546
#define WHEN 547
#define WHERE 548
#define WITH 549
#define WITHOUT 550
#define WORK 551
#define WRITE 552
#define YEAR_P 553
#define ZONE 554
#define SLIDE 555
#define SLIDETIMEOUT 556
#define RANGE 557
#define WINDOW 558
#define ARCHIVED 559
#define UNARCHIVED 560
#define UNARCHIVED_DROP 561
#define UNARCHIVED_SPILL 562
#define TIMESTAMPCOLUMN 563
#define STREAM 564
#define WRAPPER 565
#define WRAPPERINFO 566
#define DECLUST 567
#define BINDINGS 568
#define RECURSIVE_RESULT 569
#define RECURSIVE_NEXT 570
#define ISTREAM 571
#define RSTREAM 572
#define DSTREAM 573
#define OPTIONAL 574
#define OVERLOAD 575
#define KEEP 576
#define COUNTS 577
#define MYHIST 578
#define BLOCK 579
#define REGHIST 580
#define WAVELET 581
#define SAMPLE 582
#define EVENT 583
#define PARTITION 584
#define ANYONE 585
#define FOLLOWS 586
#define NOTFOLLOWS 587
#define PRECEDES 588
#define NOTPRECEDES 589
#define SLICES 590
#define UNIONJOIN 591
#define IDENT 592
#define FCONST 593
#define SCONST 594
#define NCONST 595
#define BCONST 596
#define XCONST 597
#define Op 598
#define ICONST 599
#define PARAM 600
#define OP 601
#define POSTFIXOP 602
#define UMINUS 603
#define TYPECAST 604




/* Copy the first part of user declarations.  */
#line 1 "gram.y"


/*#define YYDEBUG 1*/
/*-------------------------------------------------------------------------
 *
 * gram.y
 *	  POSTGRES SQL YACC rules/actions
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 * Portions Copyright (c) 1996-2002, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/parser/gram.y,v 1.33 2006/02/02 01:46:44 phred Exp $
 *
 * HISTORY
 *	  AUTHOR			DATE			MAJOR EVENT
 *	  Andrew Yu			Sept, 1994		POSTQUEL to SQL conversion
 *	  Andrew Yu			Oct, 1994		lispy code conversion
 *
 * NOTES
 *	  CAPITALS are used to represent terminal symbols.
 *	  non-capitals are used to represent non-terminals.
 *	  SQL92-specific syntax is separated from plain SQL/Postgres syntax
 *	  to help isolate the non-extensible portions of the parser.
 *
 *	  In general, nothing in this file should initiate database accesses
 *	  nor depend on changeable state (such as SET variables).  If you do
 *	  database accesses, your code will fail when we have aborted the
 *	  current transaction and are just parsing commands to find the next
 *	  ROLLBACK or COMMIT.  If you make use of SET variables, then you
 *	  will do the wrong thing in multi-query strings like this:
 *			SET SQL_inheritance TO off; SELECT * FROM foo;
 *	  because the entire string is parsed by gram.y before the SET gets
 *	  executed.  Anything that depends on the database or changeable state
 *	  should be handled inside parse_analyze() so that it happens at the
 *	  right time not the wrong time.  The handling of SQL_inheritance is
 *	  a good example.
 *
 * WARNINGS
 *	  If you use a list, make sure the datum is a node so that the printing
 *	  routines work.
 *
 *	  Sometimes we assign constants to makeStrings. Make sure we don't free
 *	  those.
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include <ctype.h>

#include "access/htup.h"
#include "catalog/index.h"
#include "catalog/namespace.h"
#include "catalog/pg_type.h"
#include "nodes/makefuncs.h"
#include "nodes/params.h"
#include "nodes/parsenodes.h"
#include "parser/gramparse.h"
#include "parser/parse_node.h"
#include "parser/parse_coerce.h"
#include "storage/lmgr.h"
#include "utils/numeric.h"
#include "utils/datetime.h"
#include "utils/date.h"
#include "miscadmin.h"
#include "utils/window_agg.h"
#include "telegraphcq/fluxplans.h" /* @dfluxMS */
#include "telegraphcq/shedding.h"
#include "telegraphcq/events.h" /* - TCQ SR */

extern List *parsetree;			/* final parse result is delivered here */

static bool QueryIsRule = FALSE;

/*
 * If you need access to certain yacc-generated variables and find that
 * they're static by default, uncomment the next line.  (this is not a
 * problem, yet.)
 */
/*#define __YYSCLASS*/

static Node *makeTypeCast(Node *arg, TypeName *typename);
static Node *makeStringConst(char *str, TypeName *typename);
static Node *makeIntConst(int val);
static Node *makeFloatConst(char *str);
static Node *makeAConst(Value *v);
static Node *makeRowExpr(List *opr, List *largs, List *rargs);
static Node *makeDistinctExpr(List *largs, List *rargs);
static Node *makeRowNullTest(NullTestType test, List *args);
static DefElem *makeDefElem(char *name, Node *arg);
static A_Const *makeBoolConst(bool state);
static FuncCall *makeOverlaps(List *largs, List *rargs);
static SelectStmt *findLeftmostSelect(SelectStmt *node);
static void insertSelectOptions(SelectStmt *stmt,
								List *sortClause, List *forUpdate,
								Node *limitOffset, Node *limitCount);
static Node *makeSetOp(SetOperation op, bool all, Node *larg, Node *rarg);
static Node *doNegate(Node *n);
static void doNegateFloat(Value *v);



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
#line 108 "gram.y"
typedef union YYSTYPE {
	int					ival;
	char				chr;
	char				*str;
	const char			*keyword;
	bool				boolean;
	JoinType			jtype;
	DropBehavior		dbehavior;
	List				*list;
	Node				*node;
	Value				*value;
	ColumnRef			*columnref;

	TypeName			*typnam;
	DefElem				*defelt;
	SortGroupBy			*sortgroupby;
	JoinExpr			*jexpr;
	IndexElem			*ielem;
	Alias				*alias;
	RangeVar			*range;
	A_Indices			*aind;
	ResTarget			*target;
	PrivTarget			*privtarget;
    WindowExpr          *window;
	AggWindowSpec       *agg_window;
  
	InsertStmt			*istmt;
	VariableSetStmt		*vsetstmt;

  CursorType cursortype;
  OverloadParams* overload_params;
} YYSTYPE;
/* Line 191 of yacc.c.  */
#line 911 "y.tab.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 923 "y.tab.c"

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
#define YYFINAL  510
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   30696

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  367
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  391
/* YYNRULES -- Number of rules. */
#define YYNRULES  1490
/* YYNRULES -- Number of states. */
#define YYNSTATES  2568

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   604

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned short yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   355,     2,     2,
     360,   361,   353,   351,   365,   352,   363,   354,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   366,   364,
     348,   347,   349,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   358,     2,   359,   356,     2,     2,     2,     2,     2,
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
     345,   346,   350,   357,   362
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short yyprhs[] =
{
       0,     0,     3,     5,     9,    11,    13,    15,    17,    19,
      21,    23,    25,    27,    29,    31,    33,    35,    37,    39,
      41,    43,    45,    47,    49,    51,    53,    55,    57,    59,
      61,    63,    65,    67,    69,    71,    73,    75,    77,    79,
      81,    83,    85,    87,    89,    91,    93,    95,    97,    99,
     101,   103,   105,   107,   109,   111,   113,   115,   117,   119,
     121,   123,   125,   127,   129,   131,   133,   135,   137,   139,
     141,   143,   145,   147,   148,   154,   156,   157,   163,   169,
     174,   178,   181,   182,   185,   189,   193,   196,   198,   200,
     202,   204,   208,   212,   216,   218,   224,   227,   228,   231,
     234,   241,   243,   245,   249,   256,   261,   263,   264,   267,
     268,   270,   272,   274,   277,   281,   285,   289,   293,   297,
     303,   311,   314,   318,   322,   324,   326,   328,   332,   334,
     336,   338,   341,   343,   346,   349,   350,   352,   354,   356,
     358,   360,   362,   366,   373,   375,   377,   379,   381,   383,
     384,   386,   388,   391,   395,   400,   404,   407,   410,   414,
     419,   423,   426,   431,   433,   435,   437,   439,   441,   450,
     457,   463,   464,   468,   470,   472,   475,   480,   487,   495,
     505,   515,   525,   535,   543,   549,   557,   564,   571,   575,
     578,   580,   582,   583,   586,   588,   589,   600,   602,   604,
     606,   608,   610,   613,   614,   616,   618,   622,   626,   628,
     629,   632,   633,   637,   638,   640,   641,   651,   662,   672,
     677,   681,   685,   686,   688,   690,   692,   697,   699,   701,
     703,   705,   707,   711,   712,   715,   716,   722,   724,   726,
     729,   732,   735,   738,   739,   741,   742,   744,   748,   750,
     752,   754,   759,   762,   763,   767,   769,   771,   774,   776,
     778,   780,   783,   788,   791,   797,   799,   802,   805,   808,
     811,   815,   817,   822,   827,   833,   845,   849,   850,   852,
     856,   858,   861,   864,   867,   868,   870,   872,   875,   878,
     879,   883,   887,   890,   892,   894,   897,   900,   905,   906,
     909,   912,   913,   921,   925,   926,   928,   932,   934,   940,
     943,   944,   947,   949,   952,   955,   958,   961,   963,   965,
     967,   970,   972,   975,   985,   987,   988,   990,   992,   995,
     996,   999,  1000,  1006,  1008,  1009,  1024,  1044,  1046,  1048,
    1050,  1054,  1060,  1062,  1064,  1066,  1070,  1072,  1073,  1075,
    1077,  1079,  1083,  1084,  1086,  1088,  1090,  1092,  1094,  1096,
    1099,  1100,  1102,  1105,  1107,  1110,  1111,  1114,  1116,  1119,
    1122,  1129,  1138,  1143,  1148,  1153,  1158,  1166,  1175,  1179,
    1181,  1185,  1189,  1193,  1197,  1199,  1203,  1207,  1209,  1211,
    1213,  1215,  1217,  1230,  1232,  1236,  1241,  1249,  1254,  1257,
    1259,  1260,  1262,  1263,  1271,  1276,  1278,  1280,  1282,  1284,
    1286,  1288,  1290,  1292,  1294,  1296,  1298,  1302,  1304,  1306,
    1310,  1317,  1327,  1335,  1345,  1354,  1363,  1370,  1379,  1381,
    1383,  1385,  1387,  1389,  1391,  1393,  1395,  1397,  1399,  1401,
    1407,  1412,  1417,  1422,  1426,  1429,  1435,  1440,  1445,  1449,
    1452,  1461,  1463,  1465,  1467,  1469,  1471,  1474,  1476,  1478,
    1480,  1482,  1484,  1492,  1500,  1502,  1504,  1507,  1509,  1513,
    1515,  1517,  1519,  1521,  1523,  1525,  1527,  1529,  1531,  1533,
    1535,  1537,  1539,  1542,  1545,  1548,  1551,  1554,  1556,  1560,
    1562,  1565,  1569,  1570,  1574,  1575,  1577,  1581,  1584,  1596,
    1598,  1599,  1602,  1603,  1605,  1607,  1609,  1613,  1619,  1622,
    1624,  1627,  1628,  1638,  1641,  1642,  1646,  1649,  1651,  1655,
    1658,  1660,  1662,  1664,  1666,  1668,  1670,  1675,  1677,  1680,
    1683,  1686,  1688,  1690,  1692,  1697,  1703,  1705,  1709,  1713,
    1716,  1719,  1721,  1725,  1728,  1729,  1735,  1743,  1745,  1747,
    1755,  1757,  1761,  1765,  1769,  1771,  1775,  1787,  1798,  1801,
    1804,  1805,  1814,  1819,  1824,  1826,  1828,  1830,  1831,  1840,
    1849,  1851,  1852,  1854,  1855,  1856,  1871,  1873,  1875,  1879,
    1883,  1885,  1887,  1889,  1891,  1893,  1895,  1897,  1898,  1900,
    1902,  1904,  1906,  1908,  1909,  1916,  1919,  1922,  1925,  1928,
    1931,  1934,  1938,  1941,  1944,  1947,  1951,  1952,  1954,  1956,
    1957,  1965,  1968,  1974,  1977,  1978,  1982,  1986,  1990,  1994,
    1998,  2002,  2006,  2010,  2014,  2016,  2017,  2023,  2028,  2032,
    2040,  2042,  2043,  2054,  2059,  2064,  2070,  2076,  2079,  2084,
    2086,  2088,  2090,  2092,  2093,  2095,  2096,  2098,  2099,  2103,
    2104,  2108,  2113,  2116,  2122,  2126,  2127,  2129,  2133,  2138,
    2142,  2143,  2146,  2150,  2152,  2154,  2156,  2158,  2160,  2165,
    2170,  2173,  2175,  2183,  2188,  2190,  2194,  2197,  2202,  2207,
    2211,  2212,  2215,  2218,  2221,  2225,  2227,  2231,  2233,  2236,
    2243,  2250,  2259,  2261,  2263,  2265,  2267,  2269,  2271,  2274,
    2275,  2277,  2279,  2281,  2285,  2289,  2291,  2294,  2299,  2304,
    2306,  2308,  2322,  2327,  2332,  2337,  2341,  2345,  2348,  2350,
    2354,  2357,  2358,  2362,  2366,  2371,  2376,  2381,  2386,  2389,
    2391,  2393,  2394,  2396,  2398,  2399,  2401,  2407,  2409,  2410,
    2412,  2413,  2417,  2419,  2423,  2426,  2429,  2431,  2433,  2434,
    2439,  2444,  2447,  2450,  2455,  2457,  2458,  2460,  2462,  2464,
    2466,  2468,  2472,  2473,  2476,  2477,  2481,  2482,  2491,  2498,
    2503,  2504,  2512,  2513,  2519,  2520,  2523,  2525,  2528,  2530,
    2534,  2538,  2539,  2542,  2543,  2547,  2548,  2552,  2553,  2557,
    2561,  2563,  2564,  2567,  2568,  2571,  2572,  2574,  2578,  2582,
    2583,  2586,  2590,  2592,  2595,  2601,  2608,  2614,  2616,  2619,
    2621,  2626,  2630,  2635,  2639,  2645,  2650,  2656,  2661,  2667,
    2670,  2675,  2677,  2680,  2683,  2686,  2688,  2690,  2691,  2696,
    2699,  2701,  2704,  2707,  2712,  2716,  2721,  2724,  2725,  2728,
    2729,  2731,  2738,  2747,  2756,  2765,  2774,  2783,  2785,  2789,
    2792,  2795,  2798,  2802,  2807,  2808,  2810,  2812,  2814,  2816,
    2818,  2821,  2827,  2830,  2832,  2834,  2836,  2838,  2840,  2842,
    2844,  2846,  2848,  2850,  2852,  2855,  2858,  2861,  2864,  2867,
    2869,  2873,  2874,  2880,  2884,  2885,  2891,  2895,  2896,  2898,
    2900,  2902,  2904,  2910,  2913,  2915,  2917,  2919,  2921,  2927,
    2930,  2933,  2936,  2938,  2942,  2946,  2949,  2951,  2952,  2956,
    2957,  2960,  2961,  2967,  2970,  2976,  2979,  2981,  2985,  2989,
    2990,  2992,  2994,  2996,  2998,  3000,  3002,  3006,  3010,  3014,
    3018,  3022,  3026,  3030,  3031,  3035,  3040,  3045,  3049,  3053,
    3057,  3062,  3066,  3072,  3077,  3082,  3086,  3090,  3094,  3096,
    3098,  3100,  3102,  3104,  3106,  3108,  3110,  3112,  3114,  3116,
    3118,  3120,  3122,  3124,  3129,  3131,  3136,  3138,  3142,  3146,
    3152,  3155,  3158,  3161,  3164,  3167,  3170,  3174,  3178,  3182,
    3186,  3190,  3194,  3198,  3202,  3206,  3210,  3213,  3216,  3220,
    3224,  3227,  3231,  3237,  3242,  3249,  3253,  3259,  3264,  3271,
    3276,  3283,  3289,  3297,  3300,  3304,  3307,  3312,  3316,  3321,
    3325,  3330,  3334,  3339,  3345,  3352,  3360,  3366,  3373,  3377,
    3382,  3387,  3390,  3392,  3394,  3398,  3401,  3404,  3407,  3410,
    3413,  3416,  3420,  3424,  3428,  3432,  3436,  3440,  3444,  3448,
    3452,  3456,  3459,  3462,  3468,  3475,  3483,  3485,  3487,  3491,
    3495,  3501,  3503,  3507,  3512,  3518,  3524,  3529,  3531,  3533,
    3538,  3540,  3545,  3547,  3552,  3554,  3559,  3561,  3563,  3565,
    3572,  3577,  3582,  3587,  3592,  3599,  3605,  3611,  3617,  3622,
    3629,  3634,  3636,  3639,  3644,  3651,  3652,  3654,  3658,  3660,
    3664,  3668,  3669,  3673,  3675,  3677,  3679,  3681,  3683,  3685,
    3687,  3689,  3691,  3696,  3700,  3703,  3707,  3708,  3712,  3716,
    3719,  3722,  3724,  3725,  3728,  3731,  3735,  3738,  3740,  3742,
    3746,  3752,  3759,  3764,  3766,  3769,  3774,  3777,  3778,  3780,
    3781,  3784,  3787,  3790,  3793,  3796,  3800,  3802,  3806,  3810,
    3812,  3814,  3816,  3820,  3825,  3827,  3831,  3833,  3835,  3837,
    3839,  3841,  3845,  3847,  3849,  3851,  3855,  3857,  3859,  3861,
    3863,  3865,  3867,  3869,  3871,  3873,  3875,  3877,  3879,  3881,
    3883,  3886,  3890,  3897,  3900,  3902,  3904,  3906,  3908,  3910,
    3912,  3914,  3916,  3918,  3920,  3922,  3924,  3926,  3928,  3930,
    3932,  3934,  3936,  3938,  3940,  3942,  3944,  3946,  3948,  3950,
    3952,  3954,  3956,  3958,  3960,  3962,  3964,  3966,  3968,  3970,
    3972,  3974,  3976,  3978,  3980,  3982,  3984,  3986,  3988,  3990,
    3992,  3994,  3996,  3998,  4000,  4002,  4004,  4006,  4008,  4010,
    4012,  4014,  4016,  4018,  4020,  4022,  4024,  4026,  4028,  4030,
    4032,  4034,  4036,  4038,  4040,  4042,  4044,  4046,  4048,  4050,
    4052,  4054,  4056,  4058,  4060,  4062,  4064,  4066,  4068,  4070,
    4072,  4074,  4076,  4078,  4080,  4082,  4084,  4086,  4088,  4090,
    4092,  4094,  4096,  4098,  4100,  4102,  4104,  4106,  4108,  4110,
    4112,  4114,  4116,  4118,  4120,  4122,  4124,  4126,  4128,  4130,
    4132,  4134,  4136,  4138,  4140,  4142,  4144,  4146,  4148,  4150,
    4152,  4154,  4156,  4158,  4160,  4162,  4164,  4166,  4168,  4170,
    4172,  4174,  4176,  4178,  4180,  4182,  4184,  4186,  4188,  4190,
    4192,  4194,  4196,  4198,  4200,  4202,  4204,  4206,  4208,  4210,
    4212,  4214,  4216,  4218,  4220,  4222,  4224,  4226,  4228,  4230,
    4232,  4234,  4236,  4238,  4240,  4242,  4244,  4246,  4248,  4250,
    4252,  4254,  4256,  4258,  4260,  4262,  4264,  4266,  4268,  4270,
    4272,  4274,  4276,  4278,  4280,  4282,  4284,  4286,  4288,  4290,
    4292,  4294,  4296,  4298,  4300,  4302,  4304,  4306,  4308,  4310,
    4312,  4314,  4316,  4318,  4320,  4322,  4324,  4326,  4328,  4330,
    4332,  4334,  4336,  4338,  4340,  4342,  4344,  4346,  4348,  4350,
    4352,  4354,  4356,  4358,  4360,  4362,  4364,  4366,  4368,  4370,
    4372,  4374,  4376,  4378,  4380,  4382,  4384,  4386,  4388,  4390,
    4392,  4394,  4396,  4398,  4400,  4402,  4404,  4406,  4408,  4410,
    4412,  4414,  4416,  4418,  4420,  4422,  4424,  4426,  4428,  4430,
    4432,  4434,  4436,  4438,  4440,  4442,  4444,  4446,  4448,  4450,
    4452,  4454,  4456,  4458,  4460,  4462,  4464,  4466,  4468,  4470,
    4472,  4474,  4476,  4478,  4480,  4482,  4484,  4486,  4488,  4490,
    4492,  4494,  4496,  4498,  4500,  4502,  4504,  4506,  4508,  4510,
    4512,  4514,  4516,  4518,  4520,  4522,  4524,  4526,  4528,  4530,
    4532
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short yyrhs[] =
{
     368,     0,    -1,   369,    -1,   369,   364,   370,    -1,   370,
      -1,   581,    -1,   382,    -1,   406,    -1,   410,    -1,   373,
      -1,   374,    -1,   413,    -1,   415,    -1,   424,    -1,   454,
      -1,   551,    -1,   583,    -1,   533,    -1,   385,    -1,   379,
      -1,   458,    -1,   495,    -1,   464,    -1,   485,    -1,   425,
      -1,   471,    -1,   371,    -1,   431,    -1,   586,    -1,   601,
      -1,   487,    -1,   501,    -1,   505,    -1,   506,    -1,   553,
      -1,   384,    -1,   500,    -1,   469,    -1,   486,    -1,   484,
      -1,   568,    -1,   375,    -1,   599,    -1,   595,    -1,   509,
      -1,   513,    -1,   525,    -1,   570,    -1,   571,    -1,   608,
      -1,   569,    -1,   596,    -1,   554,    -1,   546,    -1,   548,
      -1,   545,    -1,   557,    -1,   514,    -1,   602,    -1,   560,
      -1,   572,    -1,   575,    -1,   576,    -1,   577,    -1,   582,
      -1,   587,    -1,   588,    -1,   389,    -1,   400,    -1,   401,
      -1,   402,    -1,   405,    -1,   585,    -1,    -1,    58,   280,
     748,   372,   376,    -1,   294,    -1,    -1,    11,   280,   748,
     372,   376,    -1,    11,   280,   748,   236,   390,    -1,    11,
     280,   748,   401,    -1,    86,   280,   378,    -1,   376,   377,
      -1,    -1,   199,   747,    -1,    90,   199,   747,    -1,   272,
     199,   747,    -1,   253,   746,    -1,    59,    -1,   172,    -1,
      60,    -1,   173,    -1,   122,   114,   378,    -1,   283,   277,
     747,    -1,   378,   365,   748,    -1,   748,    -1,    58,   114,
     748,   372,   380,    -1,   380,   381,    -1,    -1,   280,   378,
      -1,   253,   746,    -1,    11,   114,   748,   383,   280,   378,
      -1,     7,    -1,    86,    -1,    86,   114,   748,    -1,    58,
     227,   386,    21,   748,   387,    -1,    58,   227,   749,   387,
      -1,   749,    -1,    -1,   387,   388,    -1,    -1,   424,    -1,
     513,    -1,   575,    -1,   236,   390,    -1,   236,   153,   390,
      -1,   236,   234,   390,    -1,   749,   261,   391,    -1,   749,
     347,   391,    -1,   259,   299,   397,    -1,   264,   141,   148,
     394,   395,    -1,   234,    40,    16,   264,   141,   148,   394,
      -1,   165,   398,    -1,   234,    21,   399,    -1,   234,    21,
      74,    -1,   392,    -1,    74,    -1,   393,    -1,   392,   365,
     393,    -1,   396,    -1,   399,    -1,   461,    -1,   211,    51,
      -1,   233,    -1,   211,   297,    -1,   211,   188,    -1,    -1,
     268,    -1,   100,    -1,   187,    -1,   183,    -1,   747,    -1,
     337,    -1,   691,   747,   693,    -1,   691,   360,   746,   361,
     747,   693,    -1,   461,    -1,    74,    -1,   153,    -1,   747,
      -1,    74,    -1,    -1,   749,    -1,   339,    -1,   239,   749,
      -1,   239,   259,   299,    -1,   239,   264,   141,   148,    -1,
     239,   234,    21,    -1,   239,    10,    -1,   219,   749,    -1,
     219,   259,   299,    -1,   219,   264,   141,   148,    -1,   219,
     234,    21,    -1,   219,    10,    -1,   236,    53,   403,   404,
      -1,    10,    -1,   736,    -1,    76,    -1,   119,    -1,    42,
      -1,    11,   309,   662,     7,   310,   744,   407,   430,    -1,
      11,   309,   662,    86,   310,   744,    -1,   294,   313,   360,
     408,   361,    -1,    -1,   408,   365,   409,    -1,   409,    -1,
     446,    -1,   446,   319,    -1,   446,   319,    74,   747,    -1,
      11,   254,   662,     7,   559,   436,    -1,    11,   254,   662,
      11,   559,   749,   411,    -1,    11,   254,   662,    11,   559,
     749,    86,   175,   179,    -1,    11,   254,   662,    11,   559,
     749,   236,   175,   179,    -1,    11,   254,   662,    11,   559,
     749,   236,   247,   463,    -1,    11,   254,   662,    11,   559,
     749,   236,   250,   749,    -1,    11,   254,   662,    86,   559,
     749,   412,    -1,    11,   254,   662,     7,   442,    -1,    11,
     254,   662,    86,    52,   737,   412,    -1,    11,   254,   735,
      58,   262,   254,    -1,    11,   254,   735,   197,   261,   748,
      -1,   236,    74,   702,    -1,    86,    74,    -1,    34,    -1,
     220,    -1,    -1,    44,   414,    -1,   749,    -1,    -1,    56,
     420,   735,   444,   421,   416,   417,   422,   372,   418,    -1,
     108,    -1,   261,    -1,   747,    -1,   248,    -1,   249,    -1,
     418,   419,    -1,    -1,    27,    -1,   185,    -1,    79,   584,
     747,    -1,   179,   584,   747,    -1,    27,    -1,    -1,   294,
     185,    -1,    -1,   423,    80,   747,    -1,    -1,   281,    -1,
      -1,    58,   432,   254,   735,   360,   433,   361,   452,   453,
      -1,    58,   432,   254,   735,   182,   735,   360,   433,   361,
     453,    -1,    58,   309,   735,   360,   433,   361,   271,   428,
     426,    -1,   187,   320,   321,   427,    -1,   187,   320,    86,
      -1,   187,   320,   324,    -1,    -1,   322,    -1,   325,    -1,
     323,    -1,   326,   360,   747,   361,    -1,   327,    -1,   304,
      -1,   305,    -1,   306,    -1,   307,    -1,   294,   311,   747,
      -1,    -1,   311,   747,    -1,    -1,    58,   310,   744,   488,
     429,    -1,   257,    -1,   255,    -1,   153,   257,    -1,   153,
     255,    -1,   112,   257,    -1,   112,   255,    -1,    -1,   434,
      -1,    -1,   435,    -1,   434,   365,   435,    -1,   436,    -1,
     441,    -1,   442,    -1,   749,   669,   437,   689,    -1,   437,
     438,    -1,    -1,    52,   737,   439,    -1,   439,    -1,   440,
      -1,   175,   179,    -1,   179,    -1,   274,    -1,   308,    -1,
     206,   143,    -1,    41,   360,   702,   361,    -1,    74,   703,
      -1,   214,   735,   444,   447,   448,    -1,    75,    -1,   175,
      75,    -1,   126,    76,    -1,   126,   119,    -1,   149,   504,
      -1,    52,   737,   443,    -1,   443,    -1,    41,   360,   702,
     361,    -1,   274,   360,   445,   361,    -1,   206,   143,   360,
     445,   361,    -1,   105,   143,   360,   445,   361,   214,   735,
     444,   447,   448,   481,    -1,   360,   445,   361,    -1,    -1,
     446,    -1,   445,   365,   446,    -1,   749,    -1,   158,   109,
      -1,   158,   198,    -1,   158,   241,    -1,    -1,   449,    -1,
     450,    -1,   449,   450,    -1,   450,   449,    -1,    -1,   187,
     278,   451,    -1,   187,    78,   451,    -1,   171,     6,    -1,
     220,    -1,    34,    -1,   236,   179,    -1,   236,    74,    -1,
     125,   360,   734,   361,    -1,    -1,   294,   185,    -1,   295,
     185,    -1,    -1,    58,   432,   254,   735,   455,    16,   615,
      -1,   360,   456,   361,    -1,    -1,   457,    -1,   456,   365,
     457,    -1,   749,    -1,    58,   432,   232,   735,   459,    -1,
     459,   460,    -1,    -1,    32,   461,    -1,    67,    -1,   123,
     461,    -1,   159,   461,    -1,   161,   461,    -1,   245,   461,
      -1,   462,    -1,   463,    -1,   338,    -1,   352,   338,    -1,
     746,    -1,   352,   746,    -1,    58,   465,   470,   145,   399,
     115,   466,   468,   467,    -1,   270,    -1,    -1,   737,    -1,
     725,    -1,   144,   747,    -1,    -1,   284,   466,    -1,    -1,
      86,   470,   145,   399,   412,    -1,   209,    -1,    -1,    58,
     266,   737,   472,   473,   187,   735,   475,    95,   210,   743,
     360,   478,   361,    -1,    58,    52,   266,   737,     8,   473,
     187,   735,   480,   481,   103,    87,   225,    95,   210,   743,
     360,   478,   361,    -1,    23,    -1,     8,    -1,   474,    -1,
     474,   191,   474,    -1,   474,   191,   474,   191,   474,    -1,
     131,    -1,    78,    -1,   278,    -1,   103,   476,   477,    -1,
      87,    -1,    -1,   225,    -1,   246,    -1,   479,    -1,   478,
     365,   479,    -1,    -1,   344,    -1,   338,    -1,   747,    -1,
     341,    -1,   342,    -1,   749,    -1,   108,   735,    -1,    -1,
     482,    -1,   482,   483,    -1,   483,    -1,   483,   482,    -1,
      -1,   175,    75,    -1,    75,    -1,   126,   119,    -1,   126,
      76,    -1,    86,   266,   737,   187,   735,   412,    -1,    58,
      18,   737,    41,   360,   702,   361,   481,    -1,    86,    18,
     737,   412,    -1,    58,     9,   743,   491,    -1,    58,   189,
     550,   491,    -1,    58,   271,   504,   491,    -1,    58,   271,
     504,    16,   360,   667,   361,    -1,    58,    39,   236,   584,
     504,   111,   491,   689,    -1,   360,   489,   361,    -1,   490,
      -1,   489,   365,   490,    -1,   752,   347,   725,    -1,   752,
     347,   750,    -1,   360,   492,   361,    -1,   493,    -1,   492,
     365,   493,    -1,   752,   347,   494,    -1,   752,    -1,   539,
      -1,   701,    -1,   461,    -1,   747,    -1,    58,   189,    43,
     504,   498,   103,   271,   669,   281,   739,    16,   496,    -1,
     497,    -1,   496,   365,   497,    -1,   189,   746,   550,   499,
      -1,   189,   746,   550,   360,   549,   361,   499,    -1,   110,
     746,   743,   535,    -1,   250,   669,    -1,    74,    -1,    -1,
     213,    -1,    -1,    86,   189,    43,   504,   281,   739,   412,
      -1,    86,   502,   503,   412,    -1,   254,    -1,   232,    -1,
     290,    -1,   124,    -1,   271,    -1,    84,    -1,    54,    -1,
     227,    -1,   309,    -1,   310,    -1,   504,    -1,   503,   365,
     504,    -1,   749,    -1,   725,    -1,   269,   625,   735,    -1,
      49,   187,   507,   504,   139,   508,    -1,    49,   187,     9,
     743,   360,   547,   361,   139,   508,    -1,    49,   187,   110,
     743,   535,   139,   508,    -1,    49,   187,   189,   550,   360,
     549,   361,   139,   508,    -1,    49,   187,    52,   737,   187,
     504,   139,   508,    -1,    49,   187,   226,   737,   187,   504,
     139,   508,    -1,    49,   187,   226,   737,   139,   508,    -1,
      49,   187,   266,   737,   187,   504,   139,   508,    -1,    48,
      -1,    68,    -1,   227,    -1,   124,    -1,   232,    -1,   254,
      -1,    84,    -1,   271,    -1,   290,    -1,   747,    -1,   179,
      -1,   101,   510,   511,   512,   737,    -1,   101,   303,   512,
     737,    -1,   101,   511,   512,   737,    -1,   101,   510,   512,
     737,    -1,   101,   512,   737,    -1,   101,   737,    -1,   164,
     510,   511,   512,   737,    -1,   164,   511,   512,   737,    -1,
     164,   510,   512,   737,    -1,   164,   512,   737,    -1,   164,
     737,    -1,   164,   510,   737,   259,    31,   358,   747,   359,
      -1,   106,    -1,    22,    -1,   216,    -1,     4,    -1,   746,
      -1,   352,   746,    -1,    10,    -1,   170,    -1,   207,    -1,
     122,    -1,   108,    -1,   113,   515,   187,   518,   261,   519,
     521,    -1,   222,   522,   515,   187,   518,   108,   519,    -1,
     516,    -1,    10,    -1,    10,   208,    -1,   517,    -1,   516,
     365,   517,    -1,   231,    -1,   131,    -1,   278,    -1,    78,
      -1,   226,    -1,   214,    -1,   266,    -1,    95,    -1,   279,
      -1,    58,    -1,   257,    -1,   255,    -1,   734,    -1,   254,
     734,    -1,   110,   523,    -1,    68,   736,    -1,   145,   736,
      -1,   227,   736,    -1,   520,    -1,   519,   365,   520,    -1,
     749,    -1,   114,   749,    -1,   294,   113,   190,    -1,    -1,
     113,   190,   103,    -1,    -1,   524,    -1,   523,   365,   524,
      -1,   743,   535,    -1,    58,   526,   124,   741,   187,   735,
     527,   360,   528,   361,   664,    -1,   274,    -1,    -1,   281,
     739,    -1,    -1,   529,    -1,   530,    -1,   531,    -1,   529,
     365,   531,    -1,   743,   360,   736,   361,   532,    -1,   740,
     532,    -1,   504,    -1,   281,   504,    -1,    -1,    58,   534,
     110,   743,   535,   221,   539,   541,   544,    -1,   191,   218,
      -1,    -1,   360,   536,   361,    -1,   360,   361,    -1,   537,
      -1,   536,   365,   537,    -1,   538,   540,    -1,   540,    -1,
     122,    -1,   193,    -1,   128,    -1,   540,    -1,   669,    -1,
     750,   726,   355,   271,    -1,   542,    -1,   541,   542,    -1,
      16,   543,    -1,   145,   399,    -1,   120,    -1,   244,    -1,
     291,    -1,    33,   187,   179,   129,    -1,   221,   179,   187,
     179,   129,    -1,   251,    -1,    98,   230,    77,    -1,    98,
     230,   138,    -1,   230,    77,    -1,   230,   138,    -1,   747,
      -1,   747,   365,   747,    -1,   294,   491,    -1,    -1,    86,
     110,   743,   535,   412,    -1,    86,     9,   743,   360,   547,
     361,   412,    -1,   669,    -1,   353,    -1,    86,   189,   550,
     360,   549,   361,   412,    -1,   669,    -1,   669,   365,   669,
      -1,   174,   365,   669,    -1,   669,   365,   174,    -1,   698,
      -1,   749,   363,   550,    -1,    58,    36,   360,   672,    16,
     672,   361,   294,   110,   524,   552,    -1,    58,    36,   360,
     672,    16,   672,   361,   295,   110,   552,    -1,    16,   121,
      -1,    16,    19,    -1,    -1,    86,    36,   360,   672,    16,
     672,   361,   412,    -1,   215,   555,   735,   556,    -1,   215,
      68,   737,   556,    -1,   124,    -1,   254,    -1,   104,    -1,
      -1,    11,   254,   662,   217,   559,   558,   261,   737,    -1,
      11,   266,   737,   187,   662,   217,   261,   737,    -1,   737,
      -1,    -1,    48,    -1,    -1,    -1,    58,   534,   226,   737,
      16,   561,   187,   566,   261,   735,   664,    83,   567,   562,
      -1,   176,    -1,   564,    -1,   360,   563,   361,    -1,   563,
     364,   565,    -1,   565,    -1,   615,    -1,   603,    -1,   611,
      -1,   607,    -1,   569,    -1,   564,    -1,    -1,   231,    -1,
     278,    -1,    78,    -1,   131,    -1,   132,    -1,    -1,    86,
     226,   737,   187,   735,   412,    -1,   177,   735,    -1,   151,
     735,    -1,   276,   735,    -1,   276,   353,    -1,     3,   574,
      -1,    24,   574,    -1,   245,   264,   573,    -1,    50,   574,
      -1,    91,   574,    -1,   224,   574,    -1,   141,   148,   394,
      -1,    -1,   296,    -1,   264,    -1,    -1,    58,   534,   290,
     735,   444,    16,   615,    -1,   152,   742,    -1,    58,    68,
     738,   372,   578,    -1,   578,   579,    -1,    -1,   156,   580,
     747,    -1,   156,   580,    74,    -1,   256,   580,   737,    -1,
     256,   580,    74,    -1,    89,   580,   747,    -1,    89,   580,
     746,    -1,    89,   580,    74,    -1,   197,   580,   737,    -1,
     197,   580,    74,    -1,   347,    -1,    -1,    11,    68,   738,
     236,   390,    -1,    11,    68,   738,   401,    -1,    86,    68,
     738,    -1,    58,    84,   504,   584,   669,   437,   689,    -1,
      16,    -1,    -1,    58,   498,    54,   504,   103,   747,   261,
     747,   108,   504,    -1,    45,   741,   187,   735,    -1,   282,
     592,   593,   591,    -1,   282,   592,   593,   591,   735,    -1,
     282,   592,   593,   591,   588,    -1,   589,   591,    -1,   589,
     591,   735,   594,    -1,    13,    -1,    12,    -1,    57,    -1,
     288,    -1,    -1,   109,    -1,    -1,   107,    -1,    -1,   360,
     736,   361,    -1,    -1,    97,   591,   602,    -1,    97,   589,
     591,   602,    -1,    97,   590,    -1,   205,   737,   597,    16,
     602,    -1,   360,   598,   361,    -1,    -1,   669,    -1,   598,
     365,   669,    -1,    95,   737,   600,   623,    -1,   360,   706,
     361,    -1,    -1,    70,   737,    -1,    70,   205,   737,    -1,
     615,    -1,   612,    -1,   611,    -1,   603,    -1,   607,    -1,
     131,   137,   735,   604,    -1,   285,   360,   731,   361,    -1,
      74,   285,    -1,   615,    -1,   360,   605,   361,   285,   360,
     731,   361,    -1,   360,   605,   361,   615,    -1,   606,    -1,
     605,   365,   606,    -1,   749,   705,    -1,    78,   108,   662,
     664,    -1,   157,   625,   734,   609,    -1,   122,   610,   162,
      -1,    -1,     5,   238,    -1,   225,   238,    -1,   225,    94,
      -1,   238,   278,    94,    -1,   238,    -1,   238,   225,    94,
      -1,    94,    -1,     5,    94,    -1,   278,   662,   236,   729,
     653,   664,    -1,    73,   737,   614,    66,   103,   615,    -1,
      73,   737,   614,    16,   613,    66,   103,   615,    -1,   316,
      -1,   317,    -1,   318,    -1,    27,    -1,   130,    -1,   228,
      -1,   130,   228,    -1,    -1,   617,    -1,   616,    -1,   620,
      -1,   360,   617,   361,    -1,   360,   616,   361,    -1,   619,
      -1,   618,   629,    -1,   618,   628,   650,   634,    -1,   618,
     628,   633,   651,    -1,   619,    -1,   616,    -1,   231,   627,
     727,   623,   653,   639,   665,   664,   637,   638,   641,   640,
     655,    -1,   618,   273,   626,   618,    -1,   618,   135,   626,
     618,    -1,   618,    93,   626,   618,    -1,   618,   315,   618,
      -1,   294,   621,   616,    -1,   621,   622,    -1,   622,    -1,
     735,    16,   616,    -1,   137,   624,    -1,    -1,   257,   625,
     735,    -1,   255,   625,   735,    -1,   153,   257,   625,   735,
      -1,   153,   255,   625,   735,    -1,   112,   257,   625,   735,
      -1,   112,   255,   625,   735,    -1,   254,   735,    -1,   735,
      -1,   254,    -1,    -1,    10,    -1,    82,    -1,    -1,    82,
      -1,    82,   187,   360,   706,   361,    -1,    10,    -1,    -1,
     629,    -1,    -1,   192,    31,   630,    -1,   631,    -1,   630,
     365,   631,    -1,   702,   632,    -1,   281,   701,    -1,    17,
      -1,    81,    -1,    -1,   150,   635,   184,   636,    -1,   184,
     636,   150,   635,    -1,   150,   635,    -1,   184,   636,    -1,
     150,   635,   365,   636,    -1,   633,    -1,    -1,   746,    -1,
      10,    -1,   345,    -1,   746,    -1,   345,    -1,   114,    31,
     706,    -1,    -1,   116,   702,    -1,    -1,   329,    31,   707,
      -1,    -1,   312,   358,   746,   365,   747,   365,   747,   359,
      -1,   312,   358,   746,   365,   747,   359,    -1,   312,   358,
     746,   359,    -1,    -1,   281,   303,   358,   746,   365,   746,
     359,    -1,    -1,   358,   643,   647,   649,   359,    -1,    -1,
     646,   648,    -1,   644,    -1,   335,   645,    -1,   747,    -1,
     645,   365,   747,    -1,   302,    31,   747,    -1,    -1,   301,
     746,    -1,    -1,   300,    31,   747,    -1,    -1,   245,    20,
     747,    -1,    -1,   103,   278,   652,    -1,   103,   211,   188,
      -1,   650,    -1,    -1,   182,   736,    -1,    -1,   108,   654,
      -1,    -1,   656,    -1,   654,   365,   656,    -1,   314,   139,
     656,    -1,    -1,   662,   642,    -1,   662,   658,   642,    -1,
     663,    -1,   663,   658,    -1,   663,    16,   360,   667,   361,
      -1,   663,    16,   749,   360,   667,   361,    -1,   663,   749,
     360,   667,   361,    -1,   616,    -1,   616,   658,    -1,   657,
      -1,   360,   657,   361,   658,    -1,   360,   657,   361,    -1,
     656,    61,   142,   656,    -1,   656,   336,   656,    -1,   656,
     659,   142,   656,   661,    -1,   656,   142,   656,   661,    -1,
     656,   167,   659,   142,   656,    -1,   656,   167,   142,   656,
      -1,    16,   749,   360,   736,   361,    -1,    16,   749,    -1,
     749,   360,   736,   361,    -1,   749,    -1,   109,   660,    -1,
     147,   660,    -1,   223,   660,    -1,   127,    -1,   194,    -1,
      -1,   281,   360,   736,   361,    -1,   187,   702,    -1,   735,
      -1,   735,   353,    -1,   188,   735,    -1,   188,   360,   735,
     361,    -1,   743,   360,   361,    -1,   743,   360,   706,   361,
      -1,   293,   702,    -1,    -1,   328,   666,    -1,    -1,   735,
      -1,   330,   360,   666,   365,   666,   361,    -1,    30,   360,
     666,   365,   666,   365,   339,   361,    -1,   331,   360,   666,
     365,   666,   365,   339,   361,    -1,   332,   360,   666,   365,
     666,   365,   339,   361,    -1,   333,   360,   666,   365,   666,
     365,   339,   361,    -1,   334,   360,   666,   365,   666,   365,
     339,   361,    -1,   668,    -1,   667,   365,   668,    -1,   749,
     669,    -1,   671,   670,    -1,   237,   671,    -1,   670,   358,
     359,    -1,   670,   358,   746,   359,    -1,    -1,   673,    -1,
     674,    -1,   678,    -1,   682,    -1,   690,    -1,   691,   693,
      -1,   691,   360,   746,   361,   693,    -1,   750,   726,    -1,
     673,    -1,   674,    -1,   679,    -1,   683,    -1,   690,    -1,
     750,    -1,   133,    -1,   134,    -1,   242,    -1,    26,    -1,
     212,    -1,   102,   675,    -1,    85,   204,    -1,    72,   677,
      -1,    71,   677,    -1,   181,   676,    -1,    29,    -1,   360,
     746,   361,    -1,    -1,   360,   746,   365,   746,   361,    -1,
     360,   746,   361,    -1,    -1,   360,   746,   365,   746,   361,
      -1,   360,   746,   361,    -1,    -1,   680,    -1,   681,    -1,
     680,    -1,   681,    -1,    28,   687,   360,   746,   361,    -1,
      28,   687,    -1,   684,    -1,   685,    -1,   684,    -1,   685,
      -1,   686,   360,   746,   361,   688,    -1,   686,   688,    -1,
      39,   687,    -1,    38,   687,    -1,   286,    -1,   166,    39,
     687,    -1,   166,    38,   687,    -1,   168,   687,    -1,   287,
      -1,    -1,    39,   236,   749,    -1,    -1,    47,   749,    -1,
      -1,   260,   360,   746,   361,   692,    -1,   260,   692,    -1,
     259,   360,   746,   361,   692,    -1,   259,   692,    -1,   136,
      -1,   294,   259,   299,    -1,   295,   259,   299,    -1,    -1,
     298,    -1,   163,    -1,    69,    -1,   117,    -1,   160,    -1,
     229,    -1,   298,   261,   163,    -1,    69,   261,   117,    -1,
      69,   261,   160,    -1,    69,   261,   229,    -1,   117,   261,
     160,    -1,   117,   261,   229,    -1,   160,   261,   229,    -1,
      -1,   695,   122,   616,    -1,   695,   175,   122,   616,    -1,
     695,   701,   697,   616,    -1,   695,   701,   616,    -1,   695,
     701,   695,    -1,   695,   139,   179,    -1,   695,   139,   175,
     179,    -1,   695,   195,   695,    -1,   695,   139,    82,   108,
     695,    -1,   225,   360,   696,   361,    -1,   225,   360,   702,
     361,    -1,   225,   360,   361,    -1,   360,   696,   361,    -1,
     706,   365,   702,    -1,    15,    -1,   243,    -1,    10,    -1,
     343,    -1,   699,    -1,   351,    -1,   352,    -1,   353,    -1,
     354,    -1,   355,    -1,   356,    -1,   348,    -1,   349,    -1,
     347,    -1,   343,    -1,   189,   360,   550,   361,    -1,   698,
      -1,   189,   360,   550,   361,    -1,   704,    -1,   702,   362,
     669,    -1,   702,    47,   749,    -1,   702,    20,   259,   299,
     704,    -1,   351,   702,    -1,   352,   702,    -1,   355,   702,
      -1,   356,   702,    -1,   702,   355,    -1,   702,   356,    -1,
     702,   351,   702,    -1,   702,   352,   702,    -1,   702,   353,
     702,    -1,   702,   354,   702,    -1,   702,   355,   702,    -1,
     702,   356,   702,    -1,   702,   348,   702,    -1,   702,   349,
     702,    -1,   702,   347,   702,    -1,   702,   700,   702,    -1,
     700,   702,    -1,   702,   700,    -1,   702,    14,   702,    -1,
     702,   191,   702,    -1,   175,   702,    -1,   702,   149,   702,
      -1,   702,   149,   702,    92,   702,    -1,   702,   175,   149,
     702,    -1,   702,   175,   149,   702,    92,   702,    -1,   702,
     118,   702,    -1,   702,   118,   702,    92,   702,    -1,   702,
     175,   118,   702,    -1,   702,   175,   118,   702,    92,   702,
      -1,   702,   240,   261,   702,    -1,   702,   240,   261,   702,
      92,   702,    -1,   702,   175,   240,   261,   702,    -1,   702,
     175,   240,   261,   702,    92,   702,    -1,   702,   140,    -1,
     702,   139,   179,    -1,   702,   178,    -1,   702,   139,   175,
     179,    -1,   702,   139,   268,    -1,   702,   139,   175,   268,
      -1,   702,   139,   100,    -1,   702,   139,   175,   100,    -1,
     702,   139,   275,    -1,   702,   139,   175,   275,    -1,   702,
     139,    82,   108,   702,    -1,   702,   139,   182,   360,   709,
     361,    -1,   702,   139,   175,   182,   360,   709,   361,    -1,
     702,    25,   703,    14,   703,    -1,   702,   175,    25,   703,
      14,   703,    -1,   702,   122,   718,    -1,   702,   175,   122,
     718,    -1,   702,   701,   697,   616,    -1,   274,   616,    -1,
     694,    -1,   704,    -1,   703,   362,   669,    -1,   351,   703,
      -1,   352,   703,    -1,   355,   703,    -1,   356,   703,    -1,
     703,   355,    -1,   703,   356,    -1,   703,   351,   703,    -1,
     703,   352,   703,    -1,   703,   353,   703,    -1,   703,   354,
     703,    -1,   703,   355,   703,    -1,   703,   356,   703,    -1,
     703,   348,   703,    -1,   703,   349,   703,    -1,   703,   347,
     703,    -1,   703,   700,   703,    -1,   700,   703,    -1,   703,
     700,    -1,   703,   139,    82,   108,   703,    -1,   703,   139,
     182,   360,   709,   361,    -1,   703,   139,   175,   182,   360,
     709,   361,    -1,   724,    -1,   745,    -1,   345,   726,   705,
      -1,   360,   702,   361,    -1,   360,   702,   361,   726,   705,
      -1,   719,    -1,   743,   360,   361,    -1,   743,   360,   706,
     361,    -1,   743,   360,    10,   706,   361,    -1,   743,   360,
      82,   706,   361,    -1,   743,   360,   353,   361,    -1,    62,
      -1,    63,    -1,    63,   360,   746,   361,    -1,    64,    -1,
      64,   360,   746,   361,    -1,   154,    -1,   154,   360,   746,
     361,    -1,   155,    -1,   155,   360,   746,   361,    -1,    65,
      -1,   235,    -1,   280,    -1,    36,   360,   702,    16,   669,
     361,    -1,    99,   360,   708,   361,    -1,   196,   360,   711,
     361,    -1,   203,   360,   713,   361,    -1,   252,   360,   714,
     361,    -1,   265,   360,   702,    16,   669,   361,    -1,   267,
     360,    30,   717,   361,    -1,   267,   360,   146,   717,   361,
      -1,   267,   360,   263,   717,   361,    -1,   267,   360,   717,
     361,    -1,    55,   360,   702,   281,   504,   361,    -1,    55,
     360,   706,   361,    -1,   616,    -1,    96,   616,    -1,   705,
     358,   702,   359,    -1,   705,   358,   702,   366,   702,   359,
      -1,    -1,   702,    -1,   706,   365,   702,    -1,   337,    -1,
     337,   365,   707,    -1,   710,   108,   702,    -1,    -1,   709,
     365,   669,    -1,   669,    -1,   337,    -1,   298,    -1,   163,
      -1,    69,    -1,   117,    -1,   160,    -1,   229,    -1,   339,
      -1,   702,   712,   715,   716,    -1,   702,   712,   715,    -1,
     202,   702,    -1,   703,   122,   703,    -1,    -1,   702,   715,
     716,    -1,   702,   716,   715,    -1,   702,   715,    -1,   702,
     716,    -1,   706,    -1,    -1,   108,   702,    -1,   103,   702,
      -1,   702,   108,   706,    -1,   108,   706,    -1,   706,    -1,
     616,    -1,   360,   706,   361,    -1,    35,   723,   720,   722,
      91,    -1,   180,   360,   702,   365,   702,   361,    -1,    46,
     360,   706,   361,    -1,   721,    -1,   720,   721,    -1,   292,
     702,   258,   702,    -1,    88,   702,    -1,    -1,   702,    -1,
      -1,   733,   705,    -1,   725,   705,    -1,   733,   726,    -1,
     363,   740,    -1,   363,   353,    -1,   363,   740,   726,    -1,
     728,    -1,   727,   365,   728,    -1,   702,    16,   752,    -1,
     702,    -1,   353,    -1,   730,    -1,   729,   365,   730,    -1,
     749,   705,   347,   702,    -1,   732,    -1,   731,   365,   732,
      -1,   728,    -1,    74,    -1,   757,    -1,   749,    -1,   735,
      -1,   734,   365,   735,    -1,   733,    -1,   725,    -1,   737,
      -1,   736,   365,   737,    -1,   749,    -1,   749,    -1,   749,
      -1,   749,    -1,   749,    -1,   747,    -1,   751,    -1,   725,
      -1,   749,    -1,   746,    -1,   338,    -1,   747,    -1,   341,
      -1,   342,    -1,   672,   747,    -1,   691,   747,   693,    -1,
     691,   360,   746,   361,   747,   693,    -1,   345,   705,    -1,
     268,    -1,   100,    -1,   179,    -1,   344,    -1,   339,    -1,
     749,    -1,   337,    -1,   753,    -1,   754,    -1,   337,    -1,
     753,    -1,   337,    -1,   753,    -1,   755,    -1,   337,    -1,
     753,    -1,   754,    -1,   755,    -1,   756,    -1,     3,    -1,
       4,    -1,     5,    -1,     6,    -1,     7,    -1,     8,    -1,
       9,    -1,    11,    -1,    18,    -1,    19,    -1,    20,    -1,
      22,    -1,    23,    -1,    24,    -1,    31,    -1,    32,    -1,
      33,    -1,    34,    -1,    37,    -1,    40,    -1,    42,    -1,
      43,    -1,    44,    -1,    45,    -1,    49,    -1,    50,    -1,
      51,    -1,    53,    -1,    54,    -1,    56,    -1,    59,    -1,
      60,    -1,    66,    -1,    67,    -1,    68,    -1,    69,    -1,
      70,    -1,    73,    -1,    76,    -1,    77,    -1,    78,    -1,
      79,    -1,    80,    -1,    84,    -1,    85,    -1,    86,    -1,
      87,    -1,    89,    -1,    90,    -1,    92,    -1,    94,    -1,
      95,    -1,    97,    -1,    98,    -1,   101,    -1,   104,    -1,
     106,    -1,   110,    -1,   111,    -1,   112,    -1,   115,    -1,
     117,    -1,   119,    -1,   120,    -1,   121,    -1,   123,    -1,
     124,    -1,   125,    -1,   128,    -1,   129,    -1,   130,    -1,
     131,    -1,   132,    -1,   138,    -1,   141,    -1,   143,    -1,
     145,    -1,   144,    -1,   148,    -1,   151,    -1,   152,    -1,
     153,    -1,   156,    -1,   157,    -1,   158,    -1,   159,    -1,
     160,    -1,   161,    -1,   162,    -1,   163,    -1,   164,    -1,
     165,    -1,   166,    -1,   170,    -1,   171,    -1,   172,    -1,
     173,    -1,   176,    -1,   177,    -1,   182,    -1,   185,    -1,
     189,    -1,   190,    -1,   193,    -1,   197,    -1,   198,    -1,
     199,    -1,   200,    -1,   201,    -1,   204,    -1,   205,    -1,
     207,    -1,   208,    -1,   209,    -1,   210,    -1,   211,    -1,
     213,    -1,   215,    -1,   216,    -1,   217,    -1,   218,    -1,
     219,    -1,   220,    -1,   221,    -1,   222,    -1,   224,    -1,
     226,    -1,   227,    -1,   228,    -1,   229,    -1,   230,    -1,
     234,    -1,   232,    -1,   233,    -1,   236,    -1,   238,    -1,
     239,    -1,   241,    -1,   244,    -1,   245,    -1,   246,    -1,
     247,    -1,   248,    -1,   249,    -1,   250,    -1,   251,    -1,
     253,    -1,   255,    -1,   256,    -1,   257,    -1,   262,    -1,
     264,    -1,   266,    -1,   269,    -1,   270,    -1,   271,    -1,
     272,    -1,   275,    -1,   276,    -1,   277,    -1,   278,    -1,
     279,    -1,   282,    -1,   283,    -1,   284,    -1,   285,    -1,
     287,    -1,   289,    -1,   290,    -1,   291,    -1,   294,    -1,
     295,    -1,   297,    -1,   296,    -1,   298,    -1,   299,    -1,
      26,    -1,    28,    -1,    29,    -1,    38,    -1,    39,    -1,
      46,    -1,    55,    -1,    71,    -1,    72,    -1,    96,    -1,
      99,    -1,   102,    -1,   133,    -1,   134,    -1,   136,    -1,
     168,    -1,   174,    -1,   180,    -1,   181,    -1,   196,    -1,
     203,    -1,   212,    -1,   225,    -1,   237,    -1,   242,    -1,
     252,    -1,   259,    -1,   260,    -1,   265,    -1,   267,    -1,
     286,    -1,    21,    -1,    25,    -1,    27,    -1,    61,    -1,
     107,    -1,   109,    -1,   118,    -1,   122,    -1,   127,    -1,
     139,    -1,   140,    -1,   142,    -1,   147,    -1,   149,    -1,
     167,    -1,   178,    -1,   194,    -1,   195,    -1,   223,    -1,
     240,    -1,   288,    -1,    10,    -1,    12,    -1,    13,    -1,
      14,    -1,    15,    -1,    16,    -1,    17,    -1,    30,    -1,
      35,    -1,    36,    -1,    41,    -1,    47,    -1,    48,    -1,
      52,    -1,    58,    -1,    62,    -1,    63,    -1,    64,    -1,
      65,    -1,    74,    -1,    75,    -1,    81,    -1,    82,    -1,
      83,    -1,    88,    -1,    91,    -1,    93,    -1,   100,    -1,
     103,    -1,   105,    -1,   108,    -1,   113,    -1,   114,    -1,
     116,    -1,   126,    -1,   135,    -1,   137,    -1,   146,    -1,
     150,    -1,   154,    -1,   155,    -1,   169,    -1,   175,    -1,
     179,    -1,   183,    -1,   184,    -1,   186,    -1,   187,    -1,
     188,    -1,   191,    -1,   192,    -1,   202,    -1,   206,    -1,
     214,    -1,   231,    -1,   235,    -1,   243,    -1,   254,    -1,
     258,    -1,   261,    -1,   263,    -1,   268,    -1,   273,    -1,
     274,    -1,   280,    -1,   281,    -1,   292,    -1,   293,    -1,
     186,    -1,   169,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   498,   498,   502,   508,   517,   518,   519,   520,   521,
     522,   523,   524,   525,   526,   527,   528,   529,   530,   531,
     532,   533,   534,   535,   536,   537,   538,   539,   540,   541,
     542,   543,   544,   545,   546,   547,   548,   549,   550,   551,
     552,   553,   554,   555,   556,   557,   558,   559,   560,   561,
     562,   563,   564,   565,   566,   567,   568,   569,   570,   571,
     572,   573,   574,   575,   576,   577,   578,   579,   580,   581,
     582,   583,   584,   586,   597,   607,   608,   619,   630,   638,
     659,   671,   672,   676,   680,   684,   688,   692,   696,   700,
     704,   708,   712,   718,   719,   732,   745,   746,   750,   754,
     769,   779,   780,   792,   808,   820,   832,   833,   837,   838,
     846,   847,   848,   862,   868,   874,   882,   889,   896,   904,
     911,   918,   926,   933,   943,   944,   947,   948,   951,   953,
     955,   959,   960,   963,   965,   970,   974,   975,   976,   977,
     989,   993,   997,  1010,  1028,  1029,  1030,  1034,  1035,  1036,
    1040,  1041,  1046,  1052,  1058,  1064,  1070,  1079,  1085,  1091,
    1097,  1103,  1113,  1123,  1124,  1128,  1129,  1137,  1152,  1176,
    1201,  1206,  1212,  1216,  1222,  1230,  1238,  1258,  1267,  1277,
    1286,  1295,  1305,  1316,  1326,  1335,  1345,  1354,  1366,  1374,
    1378,  1379,  1380,  1393,  1401,  1402,  1416,  1440,  1441,  1450,
    1451,  1452,  1458,  1459,  1464,  1468,  1472,  1476,  1485,  1489,
    1493,  1497,  1502,  1506,  1510,  1511,  1522,  1534,  1553,  1570,
    1571,  1572,  1573,  1577,  1578,  1579,  1580,  1584,  1589,  1593,
    1603,  1607,  1613,  1618,  1621,  1626,  1630,  1644,  1645,  1646,
    1647,  1648,  1654,  1660,  1664,  1665,  1669,  1673,  1680,  1681,
    1682,  1685,  1703,  1704,  1708,  1729,  1730,  1749,  1759,  1769,
    1779,  1790,  1800,  1810,  1828,  1856,  1862,  1868,  1874,  1888,
    1901,  1922,  1926,  1935,  1945,  1955,  1973,  1974,  1978,  1979,
    1982,  1988,  1992,  1997,  2002,  2014,  2016,  2018,  2020,  2023,
    2026,  2029,  2033,  2034,  2035,  2036,  2037,  2040,  2041,  2045,
    2046,  2047,  2057,  2077,  2078,  2082,  2083,  2087,  2112,  2122,
    2123,  2126,  2130,  2134,  2138,  2142,  2146,  2153,  2154,  2157,
    2158,  2166,  2170,  2186,  2199,  2200,  2208,  2210,  2214,  2215,
    2219,  2220,  2224,  2234,  2235,  2247,  2270,  2299,  2300,  2304,
    2309,  2314,  2323,  2324,  2325,  2329,  2336,  2337,  2341,  2342,
    2346,  2347,  2348,  2352,  2358,  2359,  2360,  2361,  2362,  2366,
    2367,  2371,  2373,  2380,  2387,  2395,  2399,  2400,  2404,  2405,
    2410,  2431,  2448,  2469,  2477,  2485,  2493,  2526,  2536,  2540,
    2541,  2547,  2551,  2557,  2561,  2562,  2565,  2569,  2576,  2577,
    2578,  2579,  2592,  2606,  2607,  2611,  2621,  2631,  2640,  2649,
    2650,  2653,  2654,  2659,  2678,  2688,  2689,  2690,  2691,  2692,
    2693,  2694,  2695,  2696,  2697,  2701,  2702,  2705,  2706,  2717,
    2739,  2748,  2758,  2767,  2777,  2786,  2795,  2805,  2817,  2818,
    2819,  2820,  2821,  2822,  2823,  2824,  2825,  2829,  2830,  2842,
    2864,  2874,  2892,  2906,  2916,  2926,  2941,  2959,  2969,  2979,
    2989,  3010,  3011,  3012,  3013,  3022,  3023,  3025,  3026,  3027,
    3030,  3031,  3041,  3054,  3069,  3070,  3071,  3075,  3076,  3082,
    3083,  3084,  3085,  3086,  3087,  3088,  3089,  3090,  3091,  3092,
    3093,  3100,  3107,  3114,  3121,  3128,  3135,  3146,  3147,  3150,
    3161,  3176,  3180,  3184,  3188,  3193,  3194,  3199,  3218,  3233,
    3234,  3238,  3240,  3244,  3245,  3248,  3249,  3252,  3262,  3272,
    3303,  3304,  3319,  3334,  3335,  3338,  3339,  3343,  3344,  3347,
    3355,  3358,  3359,  3365,  3374,  3388,  3389,  3401,  3402,  3406,
    3410,  3414,  3418,  3422,  3426,  3430,  3434,  3438,  3442,  3446,
    3450,  3456,  3457,  3464,  3465,  3480,  3491,  3502,  3503,  3507,
    3518,  3522,  3524,  3526,  3531,  3533,  3544,  3554,  3566,  3567,
    3568,  3572,  3593,  3602,  3614,  3615,  3618,  3619,  3631,  3643,
    3654,  3655,  3658,  3659,  3670,  3669,  3688,  3689,  3690,  3695,
    3701,  3710,  3711,  3712,  3713,  3714,  3718,  3719,  3723,  3724,
    3725,  3726,  3730,  3731,  3736,  3756,  3764,  3773,  3779,  3800,
    3807,  3814,  3821,  3828,  3835,  3844,  3846,  3849,  3850,  3851,
    3861,  3881,  3897,  3907,  3908,  3912,  3916,  3920,  3924,  3928,
    3932,  3936,  3940,  3944,  3955,  3956,  3967,  3975,  3993,  4009,
    4023,  4024,  4038,  4059,  4076,  4088,  4100,  4112,  4124,  4139,
    4140,  4144,  4148,  4149,  4152,  4153,  4156,  4157,  4161,  4162,
    4176,  4184,  4192,  4211,  4221,  4222,  4225,  4226,  4237,  4247,
    4248,  4258,  4264,  4284,  4285,  4286,  4287,  4288,  4300,  4308,
    4315,  4322,  4329,  4336,  4346,  4347,  4352,  4370,  4379,  4389,
    4390,  4393,  4394,  4395,  4396,  4397,  4398,  4399,  4400,  4411,
    4432,  4440,  4450,  4451,  4452,  4455,  4456,  4457,  4458,  4459,
    4507,  4508,  4509,  4513,  4514,  4524,  4525,  4531,  4537,  4546,
    4547,  4574,  4611,  4615,  4619,  4624,  4632,  4637,  4641,  4648,
    4656,  4657,  4665,  4670,  4675,  4680,  4685,  4692,  4699,  4704,
    4711,  4712,  4715,  4716,  4717,  4724,  4725,  4726,  4727,  4731,
    4732,  4736,  4740,  4741,  4744,  4752,  4753,  4755,  4758,  4763,
    4765,  4767,  4769,  4771,  4779,  4781,  4785,  4801,  4815,  4827,
    4843,  4864,  4865,  4869,  4870,  4875,  4876,  4884,  4908,  4927,
    4934,  4940,  4950,  4958,  4977,  4982,  4993,  5003,  5006,  5007,
    5010,  5015,  5020,  5025,  5029,  5034,  5039,  5044,  5053,  5054,
    5058,  5059,  5063,  5064,  5076,  5077,  5081,  5082,  5085,  5090,
    5102,  5111,  5121,  5128,  5136,  5143,  5153,  5163,  5180,  5187,
    5191,  5217,  5221,  5233,  5247,  5260,  5274,  5285,  5300,  5306,
    5311,  5317,  5324,  5325,  5326,  5327,  5331,  5332,  5344,  5345,
    5350,  5357,  5364,  5371,  5381,  5390,  5403,  5404,  5411,  5415,
    5419,  5426,  5435,  5444,  5453,  5462,  5471,  5486,  5490,  5496,
    5517,  5522,  5530,  5532,  5535,  5547,  5548,  5549,  5550,  5551,
    5552,  5558,  5567,  5585,  5586,  5587,  5588,  5589,  5593,  5604,
    5608,  5612,  5616,  5620,  5624,  5628,  5632,  5637,  5642,  5647,
    5653,  5667,  5673,  5686,  5696,  5703,  5716,  5726,  5737,  5741,
    5747,  5751,  5759,  5776,  5797,  5801,  5807,  5811,  5824,  5855,
    5878,  5880,  5882,  5884,  5886,  5888,  5893,  5894,  5898,  5899,
    5903,  5904,  5908,  5925,  5944,  5957,  5972,  5976,  5977,  5978,
    5982,  5983,  5984,  5985,  5986,  5987,  5988,  5990,  5992,  5995,
    5998,  6000,  6003,  6005,  6020,  6030,  6040,  6054,  6068,  6073,
    6077,  6081,  6085,  6121,  6122,  6123,  6124,  6127,  6130,  6131,
    6132,  6135,  6136,  6139,  6140,  6141,  6142,  6143,  6144,  6145,
    6146,  6147,  6150,  6152,  6156,  6158,  6177,  6178,  6180,  6189,
    6207,  6209,  6211,  6213,  6215,  6217,  6219,  6221,  6223,  6225,
    6227,  6229,  6231,  6233,  6235,  6238,  6240,  6242,  6245,  6247,
    6249,  6252,  6254,  6263,  6265,  6274,  6276,  6285,  6287,  6297,
    6308,  6317,  6328,  6347,  6354,  6361,  6368,  6375,  6382,  6389,
    6396,  6403,  6410,  6417,  6419,  6423,  6427,  6433,  6439,  6467,
    6495,  6505,  6518,  6531,  6533,  6535,  6537,  6539,  6541,  6543,
    6545,  6547,  6549,  6551,  6553,  6555,  6557,  6559,  6561,  6563,
    6565,  6567,  6569,  6571,  6573,  6577,  6591,  6592,  6593,  6605,
    6606,  6614,  6616,  6625,  6634,  6647,  6656,  6681,  6707,  6730,
    6751,  6774,  6797,  6820,  6841,  6864,  6887,  6896,  6905,  6914,
    6916,  6925,  6939,  6949,  6961,  6977,  6989,  6998,  7007,  7016,
    7030,  7039,  7049,  7066,  7073,  7081,  7085,  7086,  7097,  7098,
    7107,  7114,  7117,  7121,  7132,  7133,  7134,  7135,  7136,  7137,
    7138,  7139,  7148,  7152,  7159,  7166,  7167,  7182,  7186,  7190,
    7194,  7201,  7206,  7210,  7213,  7216,  7217,  7218,  7221,  7227,
    7245,  7253,  7264,  7284,  7285,  7289,  7299,  7300,  7303,  7304,
    7312,  7318,  7327,  7331,  7333,  7335,  7349,  7350,  7354,  7361,
    7368,  7387,  7388,  7392,  7402,  7403,  7407,  7408,  7426,  7427,
    7431,  7432,  7436,  7443,  7468,  7470,  7475,  7478,  7481,  7483,
    7485,  7487,  7489,  7491,  7494,  7500,  7507,  7514,  7521,  7528,
    7540,  7548,  7559,  7573,  7581,  7585,  7589,  7597,  7598,  7599,
    7614,  7615,  7616,  7621,  7622,  7628,  7629,  7630,  7636,  7637,
    7638,  7639,  7640,  7656,  7657,  7658,  7659,  7660,  7661,  7662,
    7663,  7664,  7665,  7666,  7667,  7668,  7669,  7670,  7671,  7672,
    7673,  7674,  7675,  7676,  7677,  7678,  7679,  7680,  7681,  7682,
    7683,  7684,  7685,  7686,  7687,  7688,  7689,  7690,  7691,  7692,
    7693,  7694,  7695,  7696,  7697,  7698,  7699,  7700,  7701,  7702,
    7703,  7704,  7705,  7706,  7707,  7708,  7709,  7710,  7711,  7712,
    7713,  7714,  7715,  7716,  7717,  7718,  7719,  7720,  7721,  7722,
    7723,  7724,  7725,  7726,  7727,  7728,  7729,  7730,  7731,  7732,
    7733,  7734,  7735,  7736,  7737,  7738,  7739,  7740,  7741,  7742,
    7743,  7744,  7745,  7746,  7747,  7748,  7749,  7750,  7751,  7752,
    7753,  7754,  7755,  7756,  7757,  7758,  7759,  7760,  7761,  7762,
    7763,  7764,  7765,  7766,  7767,  7768,  7769,  7770,  7771,  7772,
    7773,  7774,  7775,  7776,  7777,  7778,  7779,  7780,  7781,  7782,
    7783,  7784,  7785,  7786,  7787,  7788,  7789,  7790,  7791,  7792,
    7793,  7794,  7795,  7796,  7797,  7798,  7799,  7800,  7801,  7802,
    7803,  7804,  7805,  7806,  7807,  7808,  7809,  7810,  7811,  7812,
    7813,  7814,  7815,  7816,  7817,  7818,  7819,  7820,  7821,  7822,
    7823,  7824,  7825,  7826,  7827,  7828,  7829,  7830,  7831,  7845,
    7846,  7847,  7848,  7849,  7850,  7851,  7852,  7853,  7854,  7855,
    7856,  7857,  7858,  7859,  7860,  7861,  7862,  7863,  7864,  7865,
    7866,  7867,  7868,  7869,  7870,  7871,  7872,  7873,  7874,  7875,
    7889,  7890,  7891,  7892,  7893,  7894,  7895,  7896,  7897,  7898,
    7899,  7900,  7901,  7902,  7903,  7904,  7905,  7906,  7907,  7908,
    7909,  7919,  7920,  7921,  7922,  7923,  7924,  7925,  7926,  7927,
    7928,  7929,  7930,  7931,  7932,  7933,  7934,  7935,  7936,  7937,
    7938,  7939,  7940,  7941,  7942,  7943,  7944,  7945,  7946,  7947,
    7948,  7949,  7950,  7951,  7952,  7953,  7954,  7955,  7956,  7957,
    7958,  7959,  7960,  7961,  7962,  7963,  7964,  7965,  7966,  7967,
    7968,  7969,  7970,  7971,  7972,  7973,  7974,  7975,  7976,  7977,
    7978,  7979,  7980,  7981,  7982,  7983,  7984,  7985,  7986,  7991,
    7998
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ABORT_TRANS", "ABSOLUTE", "ACCESS", 
  "ACTION", "ADD", "AFTER", "AGGREGATE", "ALL", "ALTER", "ANALYSE", 
  "ANALYZE", "AND", "ANY", "AS", "ASC", "ASSERTION", "ASSIGNMENT", "AT", 
  "AUTHORIZATION", "BACKWARD", "BEFORE", "BEGIN_TRANS", "BETWEEN", 
  "BIGINT", "BINARY", "BIT", "BOOLEAN", "BOTH", "BY", "CACHE", "CALLED", 
  "CASCADE", "CASE", "CAST", "CHAIN", "CHAR_P", "CHARACTER", 
  "CHARACTERISTICS", "CHECK", "CHECKPOINT", "CLASS", "CLOSE", "CLUSTER", 
  "COALESCE", "COLLATE", "COLUMN", "COMMENT", "COMMIT", "COMMITTED", 
  "CONSTRAINT", "CONSTRAINTS", "CONVERSION_P", "CONVERT", "COPY", 
  "CQEDDY", "CREATE", "CREATEDB", "CREATEUSER", "CROSS", "CURRENT_DATE", 
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
  "LOCALTIME", "LOCALTIMESTAMP", "LOCATION", "LOCK_P", "MATCH", 
  "MAXVALUE", "MINUTE_P", "MINVALUE", "MODE", "MONTH_P", "MOVE", "NAMES", 
  "NATIONAL", "NATURAL", "NCHAR", "NEW", "NEXT", "NO", "NOCREATEDB", 
  "NOCREATEUSER", "NONE", "NOT", "NOTHING", "NOTIFY", "NOTNULL", "NULL_P", 
  "NULLIF", "NUMERIC", "OF", "OFF", "OFFSET", "OIDS", "OLD", "ON", "ONLY", 
  "OPERATOR", "OPTION", "OR", "ORDER", "OUT_P", "OUTER_P", "OVERLAPS", 
  "OVERLAY", "OWNER", "PARTIAL", "PASSWORD", "PATH_P", "PENDANT", 
  "PLACING", "POSITION", "PRECISION", "PREPARE", "PRIMARY", "PRIOR", 
  "PRIVILEGES", "PROCEDURAL", "PROCEDURE", "READ", "REAL", "RECHECK", 
  "REFERENCES", "REINDEX", "RELATIVE", "RENAME", "REPLACE", "RESET", 
  "RESTRICT", "RETURNS", "REVOKE", "RIGHT", "ROLLBACK", "ROW", "RULE", 
  "SCHEMA", "SCROLL", "SECOND_P", "SECURITY", "SELECT", "SEQUENCE", 
  "SERIALIZABLE", "SESSION", "SESSION_USER", "SET", "SETOF", "SHARE", 
  "SHOW", "SIMILAR", "SIMPLE", "SMALLINT", "SOME", "STABLE", "START", 
  "STATEMENT", "STATISTICS", "STDIN", "STDOUT", "STORAGE", "STRICT", 
  "SUBSTRING", "SYSID", "TABLE", "TEMP", "TEMPLATE", "TEMPORARY", "THEN", 
  "TIME", "TIMESTAMP", "TO", "TOAST", "TRAILING", "TRANSACTION", "TREAT", 
  "TRIGGER", "TRIM", "TRUE_P", "TRUNCATE", "TRUSTED", "TYPE_P", 
  "UNENCRYPTED", "UNION", "UNIQUE", "UNKNOWN", "UNLISTEN", "UNTIL", 
  "UPDATE", "USAGE", "USER", "USING", "VACUUM", "VALID", "VALIDATOR", 
  "VALUES", "VARCHAR", "VARYING", "VERBOSE", "VERSION", "VIEW", 
  "VOLATILE", "WHEN", "WHERE", "WITH", "WITHOUT", "WORK", "WRITE", 
  "YEAR_P", "ZONE", "SLIDE", "SLIDETIMEOUT", "RANGE", "WINDOW", 
  "ARCHIVED", "UNARCHIVED", "UNARCHIVED_DROP", "UNARCHIVED_SPILL", 
  "TIMESTAMPCOLUMN", "STREAM", "WRAPPER", "WRAPPERINFO", "DECLUST", 
  "BINDINGS", "RECURSIVE_RESULT", "RECURSIVE_NEXT", "ISTREAM", "RSTREAM", 
  "DSTREAM", "OPTIONAL", "OVERLOAD", "KEEP", "COUNTS", "MYHIST", "BLOCK", 
  "REGHIST", "WAVELET", "SAMPLE", "EVENT", "PARTITION", "ANYONE", 
  "FOLLOWS", "NOTFOLLOWS", "PRECEDES", "NOTPRECEDES", "SLICES", 
  "UNIONJOIN", "IDENT", "FCONST", "SCONST", "NCONST", "BCONST", "XCONST", 
  "Op", "ICONST", "PARAM", "OP", "'='", "'<'", "'>'", "POSTFIXOP", "'+'", 
  "'-'", "'*'", "'/'", "'%'", "'^'", "UMINUS", "'['", "']'", "'('", "')'", 
  "TYPECAST", "'.'", "';'", "','", "':'", "$accept", "stmtblock", 
  "stmtmulti", "stmt", "CreateUserStmt", "opt_with", "AlterUserStmt", 
  "AlterUserSetStmt", "DropUserStmt", "OptUserList", "OptUserElem", 
  "user_list", "CreateGroupStmt", "OptGroupList", "OptGroupElem", 
  "AlterGroupStmt", "add_drop", "DropGroupStmt", "CreateSchemaStmt", 
  "OptSchemaName", "OptSchemaEltList", "schema_stmt", "VariableSetStmt", 
  "set_rest", "var_list_or_default", "var_list", "var_value", "iso_level", 
  "opt_mode", "opt_boolean", "zone_value", "opt_encoding", 
  "ColId_or_Sconst", "VariableShowStmt", "VariableResetStmt", 
  "ConstraintsSetStmt", "constraints_set_list", "constraints_set_mode", 
  "CheckPointStmt", "AlterStreamStmt", "optbindings", "binding_list", 
  "binding", "AlterTableStmt", "alter_column_default", 
  "opt_drop_behavior", "ClosePortalStmt", "opt_id", "CopyStmt", 
  "copy_from", "copy_file_name", "copy_opt_list", "copy_opt_item", 
  "opt_binary", "opt_oids", "copy_delimiter", "opt_using", "CreateStmt", 
  "CreateStrmStmt", "OptWithOverload", "OverloadParams", "stream_type", 
  "optwrapperinfo", "optaswrapperinfo", "CreateWrapperStmt", "OptTemp", 
  "OptTableElementList", "TableElementList", "TableElement", "columnDef", 
  "ColQualList", "ColConstraint", "ColConstraintElem", "ConstraintAttr", 
  "TableLikeClause", "TableConstraint", "ConstraintElem", 
  "opt_column_list", "columnList", "columnElem", "key_match", 
  "key_actions", "key_update", "key_delete", "key_action", "OptInherit", 
  "OptWithOids", "CreateAsStmt", "OptCreateAs", "CreateAsList", 
  "CreateAsElement", "CreateSeqStmt", "OptSeqList", "OptSeqElem", 
  "NumericOnly", "FloatOnly", "IntegerOnly", "CreatePLangStmt", 
  "opt_trusted", "handler_name", "opt_lancompiler", "opt_validator", 
  "DropPLangStmt", "opt_procedural", "CreateTrigStmt", 
  "TriggerActionTime", "TriggerEvents", "TriggerOneEvent", 
  "TriggerForSpec", "TriggerForOpt", "TriggerForType", "TriggerFuncArgs", 
  "TriggerFuncArg", "OptConstrFromTable", "ConstraintAttributeSpec", 
  "ConstraintDeferrabilitySpec", "ConstraintTimeSpec", "DropTrigStmt", 
  "CreateAssertStmt", "DropAssertStmt", "DefineStmt", 
  "wrapper_definition", "wrapper_definition_list", "wrapper_def", 
  "definition", "def_list", "def_elem", "def_arg", "CreateOpClassStmt", 
  "opclass_item_list", "opclass_item", "opt_default", "opt_recheck", 
  "DropOpClassStmt", "DropStmt", "drop_type", "any_name_list", "any_name", 
  "TruncateStmt", "CommentStmt", "comment_type", "comment_text", 
  "FetchStmt", "direction", "fetch_how_many", "from_in", "GrantStmt", 
  "RevokeStmt", "privileges", "privilege_list", "privilege", 
  "privilege_target", "grantee_list", "grantee", "opt_grant_grant_option", 
  "opt_revoke_grant_option", "function_with_argtypes_list", 
  "function_with_argtypes", "IndexStmt", "index_opt_unique", 
  "access_method_clause", "index_params", "index_list", "func_index", 
  "index_elem", "opt_class", "CreateFunctionStmt", "opt_or_replace", 
  "func_args", "func_args_list", "func_arg", "opt_arg", "func_return", 
  "func_type", "createfunc_opt_list", "createfunc_opt_item", "func_as", 
  "opt_definition", "RemoveFuncStmt", "RemoveAggrStmt", "aggr_argtype", 
  "RemoveOperStmt", "oper_argtypes", "any_operator", "CreateCastStmt", 
  "cast_context", "DropCastStmt", "ReindexStmt", "reindex_type", 
  "opt_force", "RenameStmt", "opt_name", "opt_column", "RuleStmt", "@1", 
  "RuleActionList", "RuleActionMulti", "RuleActionStmt", 
  "RuleActionStmtOrEmpty", "event", "opt_instead", "DropRuleStmt", 
  "NotifyStmt", "ListenStmt", "UnlistenStmt", "TransactionStmt", 
  "trans_options", "opt_trans", "ViewStmt", "LoadStmt", "CreatedbStmt", 
  "createdb_opt_list", "createdb_opt_item", "opt_equal", 
  "AlterDatabaseSetStmt", "DropdbStmt", "CreateDomainStmt", "opt_as", 
  "CreateConversionStmt", "ClusterStmt", "VacuumStmt", "AnalyzeStmt", 
  "analyze_keyword", "explaincqeddy_keyword", "opt_verbose", "opt_full", 
  "opt_freeze", "opt_name_list", "ExplainStmt", "PrepareStmt", 
  "prep_type_clause", "prep_type_list", "ExecuteStmt", 
  "execute_param_clause", "DeallocateStmt", "OptimizableStmt", 
  "InsertStmt", "insert_rest", "insert_column_list", "insert_column_item", 
  "DeleteStmt", "LockStmt", "opt_lock", "lock_type", "UpdateStmt", 
  "CursorStmt", "cursor_stream_type", "opt_cursor", "SelectStmt", 
  "select_with_parens", "select_no_parens", "select_clause", 
  "simple_select", "with_select", "with_def_list", "with_def_item", 
  "into_clause", "OptTempTableName", "opt_table", "opt_all", 
  "opt_distinct", "opt_sort_clause", "sort_clause", "sortby_list", 
  "sortby", "OptUseOp", "select_limit", "opt_select_limit", 
  "select_limit_value", "select_offset_value", "group_clause", 
  "having_clause", "partition_clause", "declust_clause", 
  "having_window_clause", "window_expr", "window_clause", "optslices", 
  "slices_list", "optrangeby", "optslidetimeout", "optslideby", 
  "optstartat", "for_update_clause", "opt_for_update_clause", 
  "update_list", "from_clause", "from_list", "result_clause", "table_ref", 
  "joined_table", "alias_clause", "join_type", "join_outer", "join_qual", 
  "relation_expr", "func_table", "where_clause", "event_clause", 
  "event_expr", "TableFuncElementList", "TableFuncElement", "Typename", 
  "opt_array_bounds", "SimpleTypename", "ConstTypename", "GenericType", 
  "Numeric", "opt_float", "opt_numeric", "opt_decimal", "Bit", "ConstBit", 
  "BitWithLength", "BitWithoutLength", "Character", "ConstCharacter", 
  "CharacterWithLength", "CharacterWithoutLength", "character", 
  "opt_varying", "opt_charset", "opt_collate", "ConstDatetime", 
  "ConstInterval", "opt_timezone", "opt_interval", "r_expr", "row", 
  "row_descriptor", "sub_type", "all_Op", "MathOp", "qual_Op", 
  "qual_all_Op", "a_expr", "b_expr", "c_expr", "opt_indirection", 
  "expr_list", "col_list", "extract_list", "type_list", "extract_arg", 
  "overlay_list", "overlay_placing", "position_list", "substr_list", 
  "substr_from", "substr_for", "trim_list", "in_expr", "case_expr", 
  "when_clause_list", "when_clause", "case_default", "case_arg", 
  "columnref", "dotted_name", "attrs", "target_list", "target_el", 
  "update_target_list", "update_target_el", "insert_target_list", 
  "insert_target_el", "relation_name", "qualified_name_list", 
  "qualified_name", "name_list", "name", "database_name", "access_method", 
  "attr_name", "index_name", "file_name", "func_name", "wrapper_name", 
  "AexprConst", "Iconst", "Sconst", "UserId", "ColId", "type_name", 
  "function_name", "ColLabel", "unreserved_keyword", "col_name_keyword", 
  "func_name_keyword", "reserved_keyword", "SpecialRuleRelation", 0
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
     595,   596,   597,   598,   599,   600,   601,    61,    60,    62,
     602,    43,    45,    42,    47,    37,    94,   603,    91,    93,
      40,    41,   604,    46,    59,    44,    58
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned short yyr1[] =
{
       0,   367,   368,   369,   369,   370,   370,   370,   370,   370,
     370,   370,   370,   370,   370,   370,   370,   370,   370,   370,
     370,   370,   370,   370,   370,   370,   370,   370,   370,   370,
     370,   370,   370,   370,   370,   370,   370,   370,   370,   370,
     370,   370,   370,   370,   370,   370,   370,   370,   370,   370,
     370,   370,   370,   370,   370,   370,   370,   370,   370,   370,
     370,   370,   370,   370,   370,   370,   370,   370,   370,   370,
     370,   370,   370,   370,   371,   372,   372,   373,   374,   374,
     375,   376,   376,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   378,   378,   379,   380,   380,   381,   381,
     382,   383,   383,   384,   385,   385,   386,   386,   387,   387,
     388,   388,   388,   389,   389,   389,   390,   390,   390,   390,
     390,   390,   390,   390,   391,   391,   392,   392,   393,   393,
     393,   394,   394,   395,   395,   395,   396,   396,   396,   396,
     397,   397,   397,   397,   397,   397,   397,   398,   398,   398,
     399,   399,   400,   400,   400,   400,   400,   401,   401,   401,
     401,   401,   402,   403,   403,   404,   404,   405,   406,   406,
     407,   407,   408,   408,   409,   409,   409,   410,   410,   410,
     410,   410,   410,   410,   410,   410,   410,   410,   411,   411,
     412,   412,   412,   413,   414,   414,   415,   416,   416,   417,
     417,   417,   418,   418,   419,   419,   419,   419,   420,   420,
     421,   421,   422,   422,   423,   423,   424,   424,   425,   426,
     426,   426,   426,   427,   427,   427,   427,   427,   428,   428,
     428,   428,   429,   429,   430,   430,   431,   432,   432,   432,
     432,   432,   432,   432,   433,   433,   434,   434,   435,   435,
     435,   436,   437,   437,   438,   438,   438,   439,   439,   439,
     439,   439,   439,   439,   439,   440,   440,   440,   440,   441,
     442,   442,   443,   443,   443,   443,   444,   444,   445,   445,
     446,   447,   447,   447,   447,   448,   448,   448,   448,   448,
     449,   450,   451,   451,   451,   451,   451,   452,   452,   453,
     453,   453,   454,   455,   455,   456,   456,   457,   458,   459,
     459,   460,   460,   460,   460,   460,   460,   461,   461,   462,
     462,   463,   463,   464,   465,   465,   466,   466,   467,   467,
     468,   468,   469,   470,   470,   471,   471,   472,   472,   473,
     473,   473,   474,   474,   474,   475,   476,   476,   477,   477,
     478,   478,   478,   479,   479,   479,   479,   479,   479,   480,
     480,   481,   481,   481,   481,   481,   482,   482,   483,   483,
     484,   485,   486,   487,   487,   487,   487,   487,   488,   489,
     489,   490,   490,   491,   492,   492,   493,   493,   494,   494,
     494,   494,   495,   496,   496,   497,   497,   497,   497,   498,
     498,   499,   499,   500,   501,   502,   502,   502,   502,   502,
     502,   502,   502,   502,   502,   503,   503,   504,   504,   505,
     506,   506,   506,   506,   506,   506,   506,   506,   507,   507,
     507,   507,   507,   507,   507,   507,   507,   508,   508,   509,
     509,   509,   509,   509,   509,   509,   509,   509,   509,   509,
     509,   510,   510,   510,   510,   511,   511,   511,   511,   511,
     512,   512,   513,   514,   515,   515,   515,   516,   516,   517,
     517,   517,   517,   517,   517,   517,   517,   517,   517,   517,
     517,   518,   518,   518,   518,   518,   518,   519,   519,   520,
     520,   521,   521,   522,   522,   523,   523,   524,   525,   526,
     526,   527,   527,   528,   528,   529,   529,   530,   531,   532,
     532,   532,   533,   534,   534,   535,   535,   536,   536,   537,
     537,   538,   538,   538,   539,   540,   540,   541,   541,   542,
     542,   542,   542,   542,   542,   542,   542,   542,   542,   542,
     542,   543,   543,   544,   544,   545,   546,   547,   547,   548,
     549,   549,   549,   549,   550,   550,   551,   551,   552,   552,
     552,   553,   554,   554,   555,   555,   556,   556,   557,   557,
     558,   558,   559,   559,   561,   560,   562,   562,   562,   563,
     563,   564,   564,   564,   564,   564,   565,   565,   566,   566,
     566,   566,   567,   567,   568,   569,   570,   571,   571,   572,
     572,   572,   572,   572,   572,   573,   573,   574,   574,   574,
     575,   576,   577,   578,   578,   579,   579,   579,   579,   579,
     579,   579,   579,   579,   580,   580,   581,   581,   582,   583,
     584,   584,   585,   586,   587,   587,   587,   588,   588,   589,
     589,   590,   591,   591,   592,   592,   593,   593,   594,   594,
     595,   595,   595,   596,   597,   597,   598,   598,   599,   600,
     600,   601,   601,   602,   602,   602,   602,   602,   603,   604,
     604,   604,   604,   604,   605,   605,   606,   607,   608,   609,
     609,   610,   610,   610,   610,   610,   610,   610,   610,   611,
     612,   612,   613,   613,   613,   614,   614,   614,   614,   614,
     615,   615,   615,   616,   616,   617,   617,   617,   617,   618,
     618,   619,   619,   619,   619,   619,   620,   621,   621,   622,
     623,   623,   624,   624,   624,   624,   624,   624,   624,   624,
     625,   625,   626,   626,   626,   627,   627,   627,   627,   628,
     628,   629,   630,   630,   631,   632,   632,   632,   632,   633,
     633,   633,   633,   633,   634,   634,   635,   635,   635,   636,
     636,   637,   637,   638,   638,   639,   639,   640,   640,   640,
     640,   641,   641,   642,   642,   643,   643,   644,   645,   645,
     646,   646,   647,   647,   648,   648,   649,   649,   650,   650,
     651,   651,   652,   652,   653,   653,   654,   654,   655,   655,
     656,   656,   656,   656,   656,   656,   656,   656,   656,   656,
     656,   657,   657,   657,   657,   657,   657,   657,   658,   658,
     658,   658,   659,   659,   659,   659,   660,   660,   661,   661,
     662,   662,   662,   662,   663,   663,   664,   664,   665,   665,
     666,   666,   666,   666,   666,   666,   666,   667,   667,   668,
     669,   669,   670,   670,   670,   671,   671,   671,   671,   671,
     671,   671,   671,   672,   672,   672,   672,   672,   673,   674,
     674,   674,   674,   674,   674,   674,   674,   674,   674,   674,
     675,   675,   676,   676,   676,   677,   677,   677,   678,   678,
     679,   679,   680,   681,   682,   682,   683,   683,   684,   685,
     686,   686,   686,   686,   686,   686,   687,   687,   688,   688,
     689,   689,   690,   690,   690,   690,   691,   692,   692,   692,
     693,   693,   693,   693,   693,   693,   693,   693,   693,   693,
     693,   693,   693,   693,   694,   694,   694,   694,   694,   694,
     694,   694,   694,   695,   695,   695,   695,   696,   697,   697,
     697,   698,   698,   699,   699,   699,   699,   699,   699,   699,
     699,   699,   700,   700,   701,   701,   702,   702,   702,   702,
     702,   702,   702,   702,   702,   702,   702,   702,   702,   702,
     702,   702,   702,   702,   702,   702,   702,   702,   702,   702,
     702,   702,   702,   702,   702,   702,   702,   702,   702,   702,
     702,   702,   702,   702,   702,   702,   702,   702,   702,   702,
     702,   702,   702,   702,   702,   702,   702,   702,   702,   702,
     702,   702,   702,   703,   703,   703,   703,   703,   703,   703,
     703,   703,   703,   703,   703,   703,   703,   703,   703,   703,
     703,   703,   703,   703,   703,   703,   704,   704,   704,   704,
     704,   704,   704,   704,   704,   704,   704,   704,   704,   704,
     704,   704,   704,   704,   704,   704,   704,   704,   704,   704,
     704,   704,   704,   704,   704,   704,   704,   704,   704,   704,
     704,   704,   704,   705,   705,   705,   706,   706,   707,   707,
     708,   708,   709,   709,   710,   710,   710,   710,   710,   710,
     710,   710,   711,   711,   712,   713,   713,   714,   714,   714,
     714,   714,   714,   715,   716,   717,   717,   717,   718,   718,
     719,   719,   719,   720,   720,   721,   722,   722,   723,   723,
     724,   724,   725,   726,   726,   726,   727,   727,   728,   728,
     728,   729,   729,   730,   731,   731,   732,   732,   733,   733,
     734,   734,   735,   735,   736,   736,   737,   738,   739,   740,
     741,   742,   743,   743,   744,   745,   745,   745,   745,   745,
     745,   745,   745,   745,   745,   745,   745,   746,   747,   748,
     749,   749,   749,   750,   750,   751,   751,   751,   752,   752,
     752,   752,   752,   753,   753,   753,   753,   753,   753,   753,
     753,   753,   753,   753,   753,   753,   753,   753,   753,   753,
     753,   753,   753,   753,   753,   753,   753,   753,   753,   753,
     753,   753,   753,   753,   753,   753,   753,   753,   753,   753,
     753,   753,   753,   753,   753,   753,   753,   753,   753,   753,
     753,   753,   753,   753,   753,   753,   753,   753,   753,   753,
     753,   753,   753,   753,   753,   753,   753,   753,   753,   753,
     753,   753,   753,   753,   753,   753,   753,   753,   753,   753,
     753,   753,   753,   753,   753,   753,   753,   753,   753,   753,
     753,   753,   753,   753,   753,   753,   753,   753,   753,   753,
     753,   753,   753,   753,   753,   753,   753,   753,   753,   753,
     753,   753,   753,   753,   753,   753,   753,   753,   753,   753,
     753,   753,   753,   753,   753,   753,   753,   753,   753,   753,
     753,   753,   753,   753,   753,   753,   753,   753,   753,   753,
     753,   753,   753,   753,   753,   753,   753,   753,   753,   753,
     753,   753,   753,   753,   753,   753,   753,   753,   753,   753,
     753,   753,   753,   753,   753,   753,   753,   753,   753,   753,
     753,   753,   753,   753,   753,   753,   753,   753,   753,   754,
     754,   754,   754,   754,   754,   754,   754,   754,   754,   754,
     754,   754,   754,   754,   754,   754,   754,   754,   754,   754,
     754,   754,   754,   754,   754,   754,   754,   754,   754,   754,
     755,   755,   755,   755,   755,   755,   755,   755,   755,   755,
     755,   755,   755,   755,   755,   755,   755,   755,   755,   755,
     755,   756,   756,   756,   756,   756,   756,   756,   756,   756,
     756,   756,   756,   756,   756,   756,   756,   756,   756,   756,
     756,   756,   756,   756,   756,   756,   756,   756,   756,   756,
     756,   756,   756,   756,   756,   756,   756,   756,   756,   756,
     756,   756,   756,   756,   756,   756,   756,   756,   756,   756,
     756,   756,   756,   756,   756,   756,   756,   756,   756,   756,
     756,   756,   756,   756,   756,   756,   756,   756,   756,   757,
     757
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     5,     1,     0,     5,     5,     4,
       3,     2,     0,     2,     3,     3,     2,     1,     1,     1,
       1,     3,     3,     3,     1,     5,     2,     0,     2,     2,
       6,     1,     1,     3,     6,     4,     1,     0,     2,     0,
       1,     1,     1,     2,     3,     3,     3,     3,     3,     5,
       7,     2,     3,     3,     1,     1,     1,     3,     1,     1,
       1,     2,     1,     2,     2,     0,     1,     1,     1,     1,
       1,     1,     3,     6,     1,     1,     1,     1,     1,     0,
       1,     1,     2,     3,     4,     3,     2,     2,     3,     4,
       3,     2,     4,     1,     1,     1,     1,     1,     8,     6,
       5,     0,     3,     1,     1,     2,     4,     6,     7,     9,
       9,     9,     9,     7,     5,     7,     6,     6,     3,     2,
       1,     1,     0,     2,     1,     0,    10,     1,     1,     1,
       1,     1,     2,     0,     1,     1,     3,     3,     1,     0,
       2,     0,     3,     0,     1,     0,     9,    10,     9,     4,
       3,     3,     0,     1,     1,     1,     4,     1,     1,     1,
       1,     1,     3,     0,     2,     0,     5,     1,     1,     2,
       2,     2,     2,     0,     1,     0,     1,     3,     1,     1,
       1,     4,     2,     0,     3,     1,     1,     2,     1,     1,
       1,     2,     4,     2,     5,     1,     2,     2,     2,     2,
       3,     1,     4,     4,     5,    11,     3,     0,     1,     3,
       1,     2,     2,     2,     0,     1,     1,     2,     2,     0,
       3,     3,     2,     1,     1,     2,     2,     4,     0,     2,
       2,     0,     7,     3,     0,     1,     3,     1,     5,     2,
       0,     2,     1,     2,     2,     2,     2,     1,     1,     1,
       2,     1,     2,     9,     1,     0,     1,     1,     2,     0,
       2,     0,     5,     1,     0,    14,    19,     1,     1,     1,
       3,     5,     1,     1,     1,     3,     1,     0,     1,     1,
       1,     3,     0,     1,     1,     1,     1,     1,     1,     2,
       0,     1,     2,     1,     2,     0,     2,     1,     2,     2,
       6,     8,     4,     4,     4,     4,     7,     8,     3,     1,
       3,     3,     3,     3,     1,     3,     3,     1,     1,     1,
       1,     1,    12,     1,     3,     4,     7,     4,     2,     1,
       0,     1,     0,     7,     4,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     1,     1,     3,
       6,     9,     7,     9,     8,     8,     6,     8,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     5,
       4,     4,     4,     3,     2,     5,     4,     4,     3,     2,
       8,     1,     1,     1,     1,     1,     2,     1,     1,     1,
       1,     1,     7,     7,     1,     1,     2,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     2,     1,     3,     1,
       2,     3,     0,     3,     0,     1,     3,     2,    11,     1,
       0,     2,     0,     1,     1,     1,     3,     5,     2,     1,
       2,     0,     9,     2,     0,     3,     2,     1,     3,     2,
       1,     1,     1,     1,     1,     1,     4,     1,     2,     2,
       2,     1,     1,     1,     4,     5,     1,     3,     3,     2,
       2,     1,     3,     2,     0,     5,     7,     1,     1,     7,
       1,     3,     3,     3,     1,     3,    11,    10,     2,     2,
       0,     8,     4,     4,     1,     1,     1,     0,     8,     8,
       1,     0,     1,     0,     0,    14,     1,     1,     3,     3,
       1,     1,     1,     1,     1,     1,     1,     0,     1,     1,
       1,     1,     1,     0,     6,     2,     2,     2,     2,     2,
       2,     3,     2,     2,     2,     3,     0,     1,     1,     0,
       7,     2,     5,     2,     0,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     1,     0,     5,     4,     3,     7,
       1,     0,    10,     4,     4,     5,     5,     2,     4,     1,
       1,     1,     1,     0,     1,     0,     1,     0,     3,     0,
       3,     4,     2,     5,     3,     0,     1,     3,     4,     3,
       0,     2,     3,     1,     1,     1,     1,     1,     4,     4,
       2,     1,     7,     4,     1,     3,     2,     4,     4,     3,
       0,     2,     2,     2,     3,     1,     3,     1,     2,     6,
       6,     8,     1,     1,     1,     1,     1,     1,     2,     0,
       1,     1,     1,     3,     3,     1,     2,     4,     4,     1,
       1,    13,     4,     4,     4,     3,     3,     2,     1,     3,
       2,     0,     3,     3,     4,     4,     4,     4,     2,     1,
       1,     0,     1,     1,     0,     1,     5,     1,     0,     1,
       0,     3,     1,     3,     2,     2,     1,     1,     0,     4,
       4,     2,     2,     4,     1,     0,     1,     1,     1,     1,
       1,     3,     0,     2,     0,     3,     0,     8,     6,     4,
       0,     7,     0,     5,     0,     2,     1,     2,     1,     3,
       3,     0,     2,     0,     3,     0,     3,     0,     3,     3,
       1,     0,     2,     0,     2,     0,     1,     3,     3,     0,
       2,     3,     1,     2,     5,     6,     5,     1,     2,     1,
       4,     3,     4,     3,     5,     4,     5,     4,     5,     2,
       4,     1,     2,     2,     2,     1,     1,     0,     4,     2,
       1,     2,     2,     4,     3,     4,     2,     0,     2,     0,
       1,     6,     8,     8,     8,     8,     8,     1,     3,     2,
       2,     2,     3,     4,     0,     1,     1,     1,     1,     1,
       2,     5,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     2,     2,     2,     1,
       3,     0,     5,     3,     0,     5,     3,     0,     1,     1,
       1,     1,     5,     2,     1,     1,     1,     1,     5,     2,
       2,     2,     1,     3,     3,     2,     1,     0,     3,     0,
       2,     0,     5,     2,     5,     2,     1,     3,     3,     0,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     3,     0,     3,     4,     4,     3,     3,     3,
       4,     3,     5,     4,     4,     3,     3,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     1,     4,     1,     3,     3,     5,
       2,     2,     2,     2,     2,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     3,     3,
       2,     3,     5,     4,     6,     3,     5,     4,     6,     4,
       6,     5,     7,     2,     3,     2,     4,     3,     4,     3,
       4,     3,     4,     5,     6,     7,     5,     6,     3,     4,
       4,     2,     1,     1,     3,     2,     2,     2,     2,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     5,     6,     7,     1,     1,     3,     3,
       5,     1,     3,     4,     5,     5,     4,     1,     1,     4,
       1,     4,     1,     4,     1,     4,     1,     1,     1,     6,
       4,     4,     4,     4,     6,     5,     5,     5,     4,     6,
       4,     1,     2,     4,     6,     0,     1,     3,     1,     3,
       3,     0,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     3,     2,     3,     0,     3,     3,     2,
       2,     1,     0,     2,     2,     3,     2,     1,     1,     3,
       5,     6,     4,     1,     2,     4,     2,     0,     1,     0,
       2,     2,     2,     2,     2,     3,     1,     3,     3,     1,
       1,     1,     3,     4,     1,     3,     1,     1,     1,     1,
       1,     3,     1,     1,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     3,     6,     2,     1,     1,     1,     1,     1,     1,
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
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned short yydefact[] =
{
      73,   609,     0,   640,   639,   609,   167,   195,     0,     0,
     609,   209,   514,     0,     0,     0,   334,   609,     0,   643,
       0,     0,     0,     0,     0,   731,     0,     0,     0,     0,
       0,   494,   609,   738,     0,     0,     0,   731,     0,     0,
     645,     0,     0,     0,     2,     4,    26,     9,    10,    41,
      19,     6,    35,    18,    67,    68,    69,    70,    71,     7,
       8,    11,    12,    13,    24,    27,    14,    20,    22,    37,
      25,    39,    23,    38,    30,    21,    36,    31,    32,    33,
      44,    45,    57,    46,    17,    55,    53,    54,    15,    34,
      52,    56,    59,    40,    50,    47,    48,    60,    61,    62,
      63,     5,    64,    16,    72,    28,    65,    66,   643,    43,
      51,    42,    29,    58,   666,   667,    49,   665,   664,   663,
     710,   700,   740,   709,   702,   608,   607,   599,     0,     0,
       0,     0,     0,     0,   600,  1193,  1194,  1195,  1196,  1197,
    1198,  1199,  1200,  1201,  1202,  1203,  1204,  1205,  1206,  1369,
    1370,  1371,  1207,  1208,  1209,  1210,  1211,  1372,  1373,  1212,
    1213,  1214,  1215,  1216,  1374,  1217,  1218,  1219,  1220,  1221,
    1375,  1222,  1223,  1224,  1225,  1226,  1227,  1228,  1229,  1376,
    1377,  1230,  1231,  1232,  1233,  1234,  1235,  1236,  1237,  1238,
    1239,  1240,  1241,  1242,  1243,  1244,  1378,  1245,  1246,  1379,
    1247,  1380,  1248,  1249,  1250,  1251,  1252,  1253,  1254,  1255,
    1256,  1257,  1258,  1259,  1260,  1261,  1262,  1263,  1264,  1265,
    1381,  1382,  1383,  1266,  1267,  1268,  1270,  1269,  1271,  1272,
    1273,  1274,  1275,  1276,  1277,  1278,  1279,  1280,  1281,  1282,
    1283,  1284,  1285,  1384,  1286,  1287,  1288,  1289,  1385,  1290,
    1291,  1386,  1387,  1292,  1293,  1294,  1295,  1296,  1388,  1297,
    1298,  1299,  1300,  1301,  1389,  1302,  1303,  1304,  1305,  1306,
    1307,  1308,  1390,  1309,  1310,  1311,  1312,  1313,  1314,  1315,
    1316,  1317,  1318,  1391,  1319,  1320,  1321,  1322,  1323,  1325,
    1326,  1324,  1327,  1392,  1328,  1329,  1330,  1393,  1331,  1332,
    1333,  1334,  1335,  1336,  1337,  1338,  1394,  1339,  1340,  1341,
    1342,  1395,  1396,  1343,  1344,  1397,  1345,  1398,  1346,  1347,
    1348,  1349,  1350,  1351,  1352,  1353,  1354,  1355,  1356,  1357,
    1358,  1399,  1359,  1360,  1361,  1362,  1363,  1364,  1366,  1365,
    1367,  1368,  1180,   193,   194,  1181,  1182,     0,  1160,     0,
     602,   208,     0,     0,     0,     0,     0,     0,     0,   399,
       0,     0,     0,     0,     0,     0,   107,   238,   237,     0,
     324,     0,   499,     0,     0,     0,     0,   334,     0,     0,
       0,  1303,   661,  1156,   699,     0,     0,     0,     0,   411,
       0,   410,     0,     0,   408,     0,   333,     0,   412,   406,
     405,     0,   409,     0,   407,   413,   414,     0,     0,   603,
     660,   641,   642,   643,   652,     0,   454,   457,   452,   451,
     461,   460,  1286,  1304,   453,     0,  1177,     0,     0,     0,
       0,   444,   455,   465,   478,   472,   476,   470,   474,   473,
     469,   480,   479,   475,   471,   477,     0,   464,   467,     0,
    1490,  1489,  1153,  1152,   596,  1149,  1148,  1178,   611,  1161,
     730,     0,     0,     0,     0,   449,   595,   655,     0,   564,
     565,     0,   161,  1324,  1395,  1344,   157,     0,     0,   604,
     737,   735,     0,  1220,  1274,   149,  1324,  1395,  1344,   113,
       0,   156,  1324,  1395,  1344,   152,   606,     0,   598,   597,
       0,     0,   830,   644,   647,     0,   718,     0,   710,     0,
       1,    73,   637,   734,   734,     0,   734,     0,     0,   706,
       0,  1157,     0,  1179,     0,   830,     0,    76,     0,     0,
       0,   428,     0,   429,   434,     0,   431,     0,     0,   430,
     432,   433,     0,   435,   436,     0,   277,  1400,  1401,  1402,
    1403,  1404,  1405,  1406,  1407,  1408,  1409,  1410,  1411,  1412,
    1413,  1414,  1415,  1416,  1417,  1418,  1419,  1420,  1180,  1163,
       0,     0,  1162,  1181,  1187,     0,     0,   631,     0,    76,
     631,   418,   417,   242,   241,    76,   240,   239,  1214,   951,
     961,   959,   960,   953,   954,   955,   956,   957,   958,     0,
     554,   952,     0,   513,     0,   109,     0,     0,    76,     0,
       0,  1164,     0,     0,     0,     0,     0,     0,     0,     0,
     662,   695,   696,   697,     0,   837,     0,   192,     0,   628,
       0,   103,  1214,     0,     0,     0,    80,    94,     0,   192,
     415,     0,   721,     0,   650,     0,   456,   458,   459,     0,
       0,     0,   443,   466,     0,     0,     0,     0,  1132,   680,
    1150,     0,     0,     0,     0,   448,     0,     0,   567,   567,
     160,   158,     0,     0,     0,     0,  1369,  1370,  1371,  1129,
       0,  1372,  1373,  1374,  1375,  1057,  1058,  1060,  1066,  1376,
    1377,  1237,  1378,  1379,  1175,  1380,  1381,  1382,  1383,  1062,
    1064,  1285,  1384,     0,  1176,  1386,  1387,  1294,  1388,  1389,
    1390,  1391,  1067,  1393,  1394,  1395,  1396,  1397,  1398,  1174,
       0,  1068,  1399,  1180,  1166,  1168,  1169,   962,  1085,     0,
       0,  1140,     0,     0,     0,  1081,     0,   863,   864,   865,
     890,   891,   866,   896,   897,   909,   867,     0,  1022,     0,
       0,  1139,   966,  1051,  1046,  1085,   721,  1136,  1085,     0,
    1047,  1165,  1167,   868,  1181,   163,     0,   164,  1154,  1324,
     114,   148,   121,   147,     0,  1212,   115,     0,     0,     0,
       0,   155,   153,     0,     0,   601,   419,     0,   832,     0,
     831,   646,   643,   716,   717,     0,   704,   703,     3,   649,
     732,   733,     0,     0,     0,     0,   710,   715,   709,     0,
       0,     0,   791,   755,     0,   627,   101,   102,     0,   573,
     573,   573,   573,     0,     0,     0,     0,    75,    82,    79,
       0,     0,   633,     0,     0,     0,     0,     0,     0,     0,
       0,   211,     0,   373,     0,   872,   907,   879,   907,   907,
     887,   887,   881,   869,   870,   907,   884,   873,   871,   919,
     919,   902,  1183,     0,  1184,   630,     0,     0,   614,     0,
      97,   400,   374,     0,     0,   105,   338,   337,     0,     0,
     375,    82,   245,     0,   233,   310,   304,     0,     0,     0,
       0,     0,   277,   698,     0,     0,     0,   677,     0,   190,
     191,   372,     0,     0,   192,     0,     0,     0,     0,     0,
     151,   192,   150,     0,   404,  1086,     0,     0,   658,   651,
     440,     0,   442,   441,  1227,  1250,  1269,  1320,     0,     0,
     481,   468,     0,     0,     0,   668,   671,  1134,  1133,  1159,
       0,     0,   678,     0,   447,     0,   446,   916,     0,     0,
     656,   854,   855,   856,   857,   888,   889,   858,   894,   895,
     859,   933,   868,     0,   566,   563,   562,   159,   493,     0,
       0,   906,   893,  1128,     0,     0,   901,   900,     0,     0,
       0,     0,     0,   877,   876,   875,  1082,  1091,     0,   874,
       0,     0,   907,   907,   905,   990,     0,     0,   878,     0,
       0,  1106,     0,  1112,     0,     0,     0,   915,     0,   913,
       0,     0,  1021,  1173,  1085,   970,   971,   972,   973,  1081,
       0,  1086,     0,  1170,     0,     0,   899,     0,   933,     0,
       0,     0,     0,     0,   964,     0,   986,     0,     0,     0,
       0,     0,     0,     0,     0,  1003,     0,     0,  1005,     0,
       0,     0,   962,   961,   959,   960,   953,   954,   955,   956,
     974,   975,     0,   987,     0,  1131,     0,   795,  1130,     0,
     165,   166,   162,     0,     0,   123,   122,     0,   145,   146,
     141,   319,     0,   118,   144,   317,   318,     0,   321,   140,
       0,   125,   137,   139,   138,   136,   116,   124,   126,   128,
     129,   130,   117,   154,     0,     0,   795,  1141,  1085,   634,
     719,     0,   638,   714,   713,   741,   742,   748,   712,     0,
     793,   757,   758,   751,   756,   760,   752,   759,   790,   708,
     754,   707,   626,     0,     0,   572,     0,     0,     0,     0,
     184,   271,     0,     0,     0,     0,   571,     0,     0,     0,
      78,    77,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   278,   280,     0,     0,  1421,  1422,  1423,
    1424,  1425,  1426,  1427,  1428,  1429,  1430,  1431,  1432,  1433,
    1434,  1435,  1436,  1437,  1438,  1439,  1440,  1441,  1442,  1443,
    1444,  1445,  1446,  1447,  1448,  1449,  1450,  1451,  1452,  1453,
    1454,  1455,  1456,  1457,  1458,  1459,  1460,  1461,  1462,  1463,
    1464,  1465,  1466,  1467,  1468,  1469,  1470,  1471,  1472,  1473,
    1474,  1475,  1476,  1477,  1478,  1479,  1480,  1481,  1482,  1483,
    1484,  1485,  1486,  1487,  1488,  1188,     0,   384,   387,  1189,
    1190,  1191,  1192,     0,     0,     0,     0,   612,   253,    95,
       0,   555,   109,   243,   108,   110,   111,   112,   343,   342,
     344,     0,   339,     0,    74,     0,     0,   244,   246,   248,
     249,   250,     0,     0,   379,     0,     0,   236,   308,     0,
     245,     0,     0,     0,     0,     0,   574,     0,   692,   693,
     694,     0,     0,   836,   548,     0,   547,     0,   521,   523,
     522,   516,     0,   517,     0,   520,   525,   868,   545,     0,
       0,     0,   550,   192,   192,    93,   332,   416,   659,     0,
    1252,  1274,     0,   731,   731,   720,   729,   439,   484,   483,
     495,     0,   485,   486,   482,     0,   670,     0,     0,   674,
    1085,  1135,     0,   687,     0,   685,     0,  1151,   445,     0,
     851,   654,     0,   850,   922,   923,   924,   921,   925,   920,
       0,   860,   862,   653,     0,     0,     0,     0,  1127,  1123,
       0,     0,  1086,     0,     0,     0,     0,  1097,  1098,  1099,
    1096,  1100,  1095,  1094,  1101,     0,     0,     0,     0,     0,
     904,   903,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1023,     0,   945,     0,  1086,  1086,
    1111,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1086,  1117,     0,     0,  1048,   946,  1049,     0,     0,
       0,     0,  1171,   934,     0,     0,   939,     0,     0,     0,
       0,   941,   950,   948,   949,     0,   937,   938,     0,   988,
    1138,     0,     0,   968,   995,     0,  1118,  1018,     0,  1009,
       0,  1004,     0,  1007,  1011,   991,     0,     0,     0,     0,
       0,     0,   989,     0,   984,   982,   983,   976,   977,   978,
     979,   980,   981,   967,   985,     0,  1137,     0,   766,     0,
       0,     0,  1052,     0,  1155,     0,   320,   322,     0,   933,
       0,   132,   135,     0,   605,   833,     0,   837,     0,   636,
     635,     0,     0,   746,   747,     0,   744,   789,     0,   788,
       0,     0,     0,   100,     0,     0,     0,     0,     0,   177,
       0,   192,   192,     0,   570,   186,   187,     0,    87,    89,
       0,     0,    88,    90,     0,     0,     0,     0,    81,   171,
     169,     0,     0,     0,     0,   438,   426,   437,     0,     0,
     420,   276,     0,   210,   197,   198,     0,   383,     0,     0,
       0,     0,     0,     0,   625,   625,   625,   625,   613,   911,
       0,     0,    96,     0,   104,     0,     0,     0,     0,     0,
     847,     0,   269,     0,     0,   253,   378,     0,     0,     0,
       0,   312,     0,     0,     0,     0,   309,     0,     0,     0,
     305,   307,     0,     0,     0,   502,     0,     0,     0,     0,
     690,   192,     0,   515,     0,   519,   862,   192,  1158,     0,
     192,     0,   594,   370,  1087,   731,   731,   731,   731,   728,
       0,     0,     0,   497,     0,   492,   487,   489,  1147,  1146,
       0,  1144,     0,     0,   676,   688,   681,   683,   682,     0,
       0,   679,     0,   657,     0,     0,     0,     0,     0,     0,
       0,   736,     0,     0,     0,  1124,     0,     0,  1122,     0,
    1080,  1059,  1061,   886,     0,  1070,     0,   880,  1063,  1065,
       0,   883,     0,   963,     0,     0,  1071,  1025,  1026,  1027,
    1028,     0,  1041,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1029,  1030,     0,  1042,  1072,   943,   944,
       0,     0,  1109,  1110,  1073,   917,   918,   919,   919,     0,
       0,  1116,     0,     0,     0,  1078,     0,  1085,   947,   908,
     909,     0,     0,   940,   935,     0,   936,     0,     0,     0,
       0,     0,  1010,  1006,     0,  1008,  1012,     0,     0,     0,
     997,  1019,   993,     0,     0,   999,  1020,     0,   807,   794,
     796,   809,   774,   802,  1153,     0,     0,   839,     0,     0,
    1056,  1053,     0,     0,   142,   131,     0,   119,   127,  1142,
     689,     0,   648,   743,   745,   792,   749,   753,   750,     0,
     270,     0,     0,     0,     0,     0,   178,   185,   183,     0,
       0,     0,     0,    83,    86,     0,     0,     0,   235,     0,
       0,   422,     0,     0,     0,   279,   200,   201,   213,   199,
     385,  1294,   954,   390,   386,   388,   524,   389,   391,   365,
       0,   911,     0,   624,     0,     0,     0,     0,     0,     0,
       0,     0,   265,     0,     0,   258,     0,     0,   259,   260,
     252,   255,   256,   629,    99,    98,     0,     0,     0,   340,
     376,     0,   849,     0,   247,   911,   380,  1183,   381,   382,
    1184,   232,   311,   313,   314,   315,   316,   245,   298,   303,
       0,   302,   331,   327,   326,  1156,     0,     0,     0,     0,
       0,   610,     0,   546,   192,   518,     0,   403,   552,   549,
     553,   551,     0,     0,     0,     0,   723,   722,   496,   490,
       0,     0,   462,   669,     0,     0,   673,   675,   686,   684,
       0,   852,     0,   927,   928,   929,   930,   931,   932,   926,
     933,   463,   892,     0,  1126,  1120,     0,     0,     0,  1090,
       0,     0,  1104,  1103,  1105,     0,     0,     0,  1039,  1037,
    1038,  1031,  1032,  1033,  1034,  1035,  1036,  1024,  1040,  1114,
    1113,  1107,  1108,   914,   912,     0,  1075,  1076,  1077,  1115,
    1083,     0,  1050,   898,   933,   942,   965,   969,  1016,   996,
    1119,  1013,     0,  1093,     0,   992,     0,     0,     0,  1001,
     963,     0,   710,     0,   809,     0,   808,   821,     0,     0,
     827,   825,     0,   827,     0,   827,     0,     0,   781,   800,
     774,     0,   803,   821,     0,     0,     0,   837,  1054,  1055,
       0,     0,   134,   133,  1143,   272,     0,     0,   273,   189,
       0,     0,     0,     0,     0,   568,   569,    84,    91,    85,
      92,     0,     0,   168,     0,   424,     0,   425,   427,   214,
      76,     0,   367,     0,     0,   371,   361,   363,     0,     0,
     377,   360,   621,   620,   619,   616,   615,   623,   622,   618,
     617,     0,   910,     0,   263,   267,   268,   266,   257,   261,
     277,     0,     0,   347,     0,     0,   848,   228,   229,   230,
     231,   222,   251,     0,     0,   301,   306,   307,     0,   329,
       0,   501,     0,     0,     0,     0,   531,     0,     0,     0,
     532,   536,   533,   544,   527,   590,   591,   588,   589,     0,
     691,   561,   526,   727,   726,   725,   724,     0,   488,  1145,
       0,   450,   853,   861,  1125,  1069,  1079,   885,  1121,   882,
    1102,     0,     0,     0,  1074,     0,  1172,     0,  1014,     0,
    1017,   998,   994,     0,  1000,   811,   819,     0,   797,     0,
     826,   822,     0,   823,     0,     0,   824,   813,     0,     0,
       0,   783,   776,   785,   801,     0,   819,     0,   834,     0,
    1088,   765,     0,     0,     0,     0,     0,     0,   838,   840,
     762,   120,   933,     0,   274,   179,   188,   180,     0,   181,
     182,     0,   234,   421,   423,   203,     0,   369,   368,   366,
     362,   364,     0,   560,     0,   365,     0,     0,   254,   284,
       0,   245,   346,     0,     0,   341,     0,   218,   301,     0,
       0,     0,   216,   330,     0,   323,     0,     0,   503,   504,
     505,   511,     0,  1159,   529,   541,     0,     0,   530,     0,
     539,   540,     0,   528,   512,     0,   491,     0,  1043,     0,
       0,  1084,  1015,  1092,  1002,   810,     0,     0,   812,     0,
       0,   815,   817,     0,     0,     0,   777,   778,     0,   787,
       0,   775,     0,     0,     0,  1156,   835,     0,     0,     0,
       0,     0,     0,     0,     0,   764,   143,     0,     0,   173,
     174,   196,   212,   560,     0,   557,   359,     0,   262,     0,
     289,     0,   348,   349,   345,     0,     0,   217,     0,   299,
     300,   328,   632,   837,     0,     0,   509,   508,     0,     0,
       0,   537,   538,     0,   543,   837,   672,     0,  1044,     0,
     820,   829,     0,   816,   814,   780,     0,   782,     0,     0,
       0,   804,     0,   806,  1089,     0,     0,     0,     0,     0,
       0,     0,     0,   772,   277,   170,     0,   175,   204,   631,
     631,   205,   202,   556,   559,   558,     0,   281,   282,   283,
       0,   264,   285,   286,     0,     0,   220,     0,   221,   297,
     498,   506,   510,     0,   542,   534,     0,     0,  1045,   818,
       0,   779,     0,   773,   784,   805,     0,     0,     0,     0,
       0,     0,   761,   763,     0,   770,   284,   172,     0,     0,
       0,     0,     0,     0,     0,   287,     0,   288,     0,     0,
       0,   392,   393,   352,   223,   225,   224,     0,   227,   219,
     511,   535,   593,   828,   786,     0,     0,     0,     0,     0,
       0,     0,     0,   799,   289,   176,   206,   207,     0,   294,
       0,   293,     0,   291,   290,     0,     0,   398,     0,   354,
     356,   357,   353,     0,   350,   355,   358,     0,   507,   592,
       0,     0,   841,     0,     0,     0,     0,     0,     0,     0,
     711,   365,     0,   292,   296,   295,     0,   402,   394,   335,
       0,     0,   576,   587,   575,   577,   585,   582,   584,   583,
     581,     0,     0,     0,     0,     0,     0,     0,     0,   275,
       0,   397,   401,     0,   395,   351,   226,     0,   586,   580,
     710,   700,   842,   843,   844,   845,   846,     0,   769,     0,
     798,     0,     0,   578,   587,     0,     0,   352,   402,   579,
     771,   768,     0,     0,   396,     0,   336,   767
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short yydefgoto[] =
{
      -1,    43,    44,    45,    46,   828,    47,    48,    49,  1151,
    1548,   636,    50,  1249,  1582,    51,   818,    52,    53,   604,
     875,  1254,    54,   489,  1096,  1097,  1098,  1502,  1787,  1099,
    1083,   772,  1100,    55,    56,    57,   766,  1072,    58,    59,
    1818,  2308,  2309,    60,  1806,   901,    61,   343,    62,  1566,
    1828,  2311,  2382,   352,  1166,  2060,  2061,    63,    64,  2237,
    2449,  2101,  1277,  2053,    65,   376,  1608,  1267,  1268,  1269,
    1579,  1860,  1861,  1862,  1270,  1271,  1141,   841,  1162,  1163,
    2320,  2391,  2392,  2393,  2473,  2105,  2242,    66,  1281,  1609,
    1610,    67,  1278,  1606,  1101,  1085,  1086,    68,   377,  1892,
    2245,  2109,    69,   407,    70,   878,  1261,  1262,  2094,  2233,
    2324,  2483,  2484,  2225,  2065,  2066,  2067,    71,    72,    73,
      74,   884,  1273,  1274,   843,  1236,  1237,  1834,    75,  2441,
    2442,   378,  2534,    76,    77,   408,   639,  2336,    78,    79,
     545,  1556,    80,   428,   429,   430,    81,    82,   446,   447,
     448,   929,  1645,  1646,  1922,   478,  1329,  1330,    83,   379,
    1898,  2247,  2248,  2249,  2250,  2337,    84,   380,   904,  1302,
    1303,  1304,  1835,  1305,  2123,  2124,  2254,  2264,    85,    86,
    1295,    87,  1311,   599,    88,  2315,    89,    90,   471,   965,
      91,  1533,  1142,    92,  1617,  2514,  2537,  2538,  2539,  2129,
    2490,    93,  2516,    95,    96,    97,   785,   127,    98,    99,
     100,  1247,  1578,  1844,   101,   102,   103,   866,   104,   105,
     106,   107,   108,   414,   415,   504,   792,  1112,   109,   110,
     667,   949,   111,   642,   112,   113,   114,   935,  1338,  1339,
     115,   116,   942,  1346,   117,   118,  1291,   624,   119,   735,
     121,   122,   123,   124,   505,   506,   918,  1325,   461,   802,
     482,   518,   519,  1115,  1116,  1516,   812,  1131,  1123,  1126,
    2305,  2373,  1777,  2463,  2425,  2019,  2181,  2182,  2286,  2183,
    2289,  2291,  2359,   813,  1129,  1519,  1488,  1769,  2500,  1770,
    1771,  2006,  2017,  2171,  2281,  1772,  1773,   897,  2027,  2198,
    1589,  1590,  1306,  1353,   951,   736,   737,   738,   989,   998,
     983,   954,   739,   740,   741,   957,   742,   743,   744,   745,
     972,  1026,  1863,   746,   747,  1007,  1361,   748,   749,  1020,
    1448,  1034,   601,   750,  1064,   915,  1403,   752,  1013,  1022,
    2191,  1385,  1994,  1386,  1396,  1695,  1405,  1411,  1722,  1723,
    1423,  1457,   753,  1368,  1369,  1676,   974,   754,   755,   658,
     756,  1649,  1106,  1107,  1650,  1651,   758,   930,   502,  2277,
     768,   520,  1627,  2251,   347,   458,   759,   610,   760,   761,
     762,   637,   455,   763,   572,  1238,   345,   346,   574,  1242,
     456
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -2124
static const short yypact[] =
{
    6185,   -49,   564, -2124, -2124,   -49, -2124, 27393, 27393,   208,
     -49,   647, 30386, 27694, 27393,   429,  3765,   -49, 27393,   110,
   18461,   619,   596, 24082,   228,   486, 18811, 24082, 27393,    73,
   24684,   636,   -49,   153, 27995, 24985,   535,   486, 16866, 22878,
     705, 24082,   277,   789,   482, -2124, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,   533, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,
      74, -2124,   510,    79, -2124, -2124, -2124, -2124, 27393, 27393,
   22878, 27393, 27393, 22878, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124,   654, -2124,   834,
   -2124, -2124, 24082, 21640, 27393,   495,   639,   684, 27393, -2124,
   24082,   574, 27393,   635, 15450,   765, 27393, -2124, -2124, 27393,
   -2124, 24082, -2124, 27393, 24082, 27393,   -11,   795,   962,   896,
      33, 27393, -2124, -2124,    92, 22878, 21640, 27393,   672, -2124,
   27393, -2124, 21640, 27393, -2124, 15804, -2124, 27393, -2124, -2124,
   -2124, 27393, -2124, 27393, -2124, -2124, -2124,   927, 24082, -2124,
     681, -2124, -2124,   533, -2124,    53,    80, -2124,    82,    83,
   -2124, -2124,   613,   716,    84,   731, -2124,   713,    78,   731,
   27393, -2124, -2124,   854, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124,   891,   743, -2124, 24082,
   -2124, -2124, -2124,   784, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124, 24082, 19161,   731, 27393, -2124, -2124,   806, 27393, -2124,
   -2124, 24082, -2124,  1131,   870,  1033, -2124,   996,   619, -2124,
   -2124,  1006,  9116, 25286, 28296,   184, 25587,   908,  1078, -2124,
    -103, -2124,  1204,   929,  1097, -2124,  1110, 24082, -2124, -2124,
   13941,  1020,   905, -2124,  1163, 14243, -2124,  1265,   919,   922,
   -2124,  6185, 24082,   195,   195,  1258,   195,   277,   449,   452,
     332, -2124,   139, -2124,   127,    39,  1104,   294,   200, 24082,
   21640, -2124, 27393, -2124, -2124, 21640, -2124, 16158, 27393, -2124,
   -2124, -2124, 27393, -2124, -2124, 24082,   934, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,   939, -2124,
     784,   941, -2124,   942, -2124,  1263, 30102,  1302, 27393,  1026,
    1302, -2124,   959, -2124, -2124,  1026, -2124, -2124, 24082, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,   941,
   -2124, -2124,   960, -2124,  1304,  1305,   210,    59,  1026,   972,
     973, -2124, 24082, 24082,  1189, 24082, 27393, 21640, 27393, 24082,
   -2124, -2124,  1108, -2124,   179,  1044,   984,   160, 30102, -2124,
     985, -2124, 24082,   987,  1161,  1162,   986, -2124, 21002,    56,
   -2124, 10190,  1213,    53, -2124, 27393, -2124, -2124, -2124,   731,
   27393, 27393, -2124, -2124, 23179,  1165,    24, 17167, -2124,    -8,
   -2124,   731, 27393,  1094, 27393, -2124, 29500,  1339,  1252,  1252,
   -2124, -2124,  1209,  1256,  1173,  1002,  1024,   351,  1027, 10190,
    1005,    64,    64,  1018,  1022, -2124,  1025,  1028, -2124,   430,
     430,  1180,  1030,  1032, -2124,   455,  1047,  1048,   457,  1034,
    1037,   876,    64, 10190, -2124,  1038,   467,  1039,  1040,  1042,
    1069,  1052, -2124,  1074,  1056,   369,   517,  1058,  1059, -2124,
    1030, -2124,    77,   468, -2124, -2124, -2124, -2124,   784, 10190,
   10190, -2124, 10190, 10190,  9474, -2124,   228, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124,    54, -2124,   477, -2124,  2258,
   10190,  3651, -2124, -2124, -2124,  1061,    -1, -2124,   784,  1064,
   -2124, -2124, -2124, -2124,   480, -2124,   456,  1062, -2124,   198,
   -2124, -2124, -2124, -2124, 20690,  1410, -2124,    35,  1280, 17769,
   17769, -2124, -2124,  1281,  1282, -2124, -2124, 24082, -2124, 27393,
   -2124, -2124,   533, -2124, -2124,  1030, -2124, -2124, -2124,  1072,
   -2124, -2124,   277,   277, 10190,   277, -2124,  1298, -2124,   389,
     105,   581,  1332,   507, 28296, -2124, -2124, -2124,  1159,   859,
    1392,   832,  1392,  1179,  1181, 22878, 28296, -2124, -2124, -2124,
    1137,  1139, -2124,  1090,  1266,   985,  1091,   529,  1267,  1313,
   27393,  1164, 21339, -2124,  1096, -2124,  1170, -2124,  1170,  1170,
    1100,  1100,  1101, -2124, -2124,  1170,  1102, -2124, -2124,   265,
     437, -2124, -2124,  1449, -2124, -2124, 24082,  1458, -2124, 29500,
   -2124,  1395, -2124, 16158, 27393,   469, -2124, -2124,    52,  1111,
   -2124, -2124, 22276, 21339,  1176, -2124,    -5, 21002,  1369,  1286,
     985,  1462,   934, -2124,   728,  1376, 10190, -2124, 17468, -2124,
   -2124, -2124,  1464,  8080,   160,  1200, 28597, 24082, 24082, 27393,
   -2124,   160, -2124, 24082, -2124, 19553,   568, 23480, -2124, -2124,
   -2124, 27393, -2124, -2124, 27393, 21640, 27393, 27393, 24082,  1222,
    1119, -2124,  1203,  1132, 14545, -2124, -2124, -2124,   784, -2124,
     252, 24082, -2124, 27393, -2124,  1466, -2124, -2124, 29801,   631,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124,    36,   784,    53, -2124, -2124, -2124, -2124, -2124, 23179,
   10190, -2124,  1135, 19553,  1206, 10190, -2124, -2124, 10190, 10190,
     713,   713,   713, -2124, -2124, -2124, -2124,   562,   713, -2124,
     713,   713,  1170,  1170, -2124,  3729, 10190,   713, -2124, 16158,
   10190, 10548,  7042, 10190,  1241,  1242,   713, -2124,   713, -2124,
   10190,  8400, -2124,  1145, -2124,    48,    48,    61,    65,   520,
    1143,  4770,  1140, -2124,  1272,   713, -2124,   713,   107,  1030,
     387,  1387,  1150,   -60, -2124,    76,   100, 10190, 21339,  1254,
   10548, 27393, 10190,  1156,  1142, -2124, 10190,   664, -2124,  1157,
   10190,  1257,   158, 10190, 10190, 10190, 10190, 10190, 10190, 10190,
   11965, 12308, 29500, 10906,   253,  1145,  9116,  1411,  1145,  6683,
   -2124, -2124, -2124, 27393,  1410, -2124, -2124,  1259, -2124, -2124,
   -2124, -2124,   399, -2124, -2124, -2124, -2124,   552, -2124, -2124,
     528, -2124, -2124, -2124, -2124, -2124, -2124,  1166, -2124, -2124,
   -2124, -2124, -2124, -2124,   528,  1160,    -2, -2124, -2124, 22577,
   -2124, 27393, -2124,  1298, -2124,  1169, -2124,  4209,  1298,  1334,
    1344, -2124, -2124,   -39, -2124, -2124,  1385, -2124, -2124, -2124,
   -2124, -2124, -2124, 27393,  1177, -2124, 27393,  1393,  1396,  1182,
   -2124, -2124, 27393, 27393, 27393, 27393, 27393,  1289, 27393,  1328,
   -2124,  1063, 27393, 27393, 17468, 24082,  1407, 28597,   -23, 24082,
   24082,   -23,   632, -2124, -2124,  1362,    42, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124,   633, -2124,  1201, -2124,
   -2124, -2124, -2124, 10190, 30102,  1438,    52,    55, -2124,   433,
    1448, -2124, -2124,   651, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124,  1365,  1363, 27393,  1063, 24082,  1192,  1190, -2124, -2124,
   -2124, -2124, 29500,   638, -2124,  1210,  1249, -2124,   137, 24082,
   22276,  1545,  1447,   228, 24082,  1342, -2124,  1549, -2124, -2124,
   -2124,  1504,   267, 19553, -2124,  1211, -2124, 30102, -2124,   626,
     689, -2124,   644, -2124, 29500, -2124, -2124,   784, -2124, 27393,
    1208,  1215,  1214,   160,   160, -2124, -2124, -2124, -2124, 10190,
     841,   856, 24082,  2859,  3396, -2124, -2124, -2124,  1062,  1217,
   -2124,   985,  1062,  1062,  1119, 25888, -2124,  8758,   661, -2124,
   -2124, -2124,    67, -2124,    87,   258,  1409, -2124, -2124,  1216,
   -2124, -2124, 29500,  1225,  1324,  1325,  1326, -2124, -2124,  1327,
     713, -2124, -2124, -2124,  1481,   694,   713, 10190,    45, -2124,
    5222,   702,  5263,   703,  1231,  1244,   709, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124,  1245,  1485,  1247,  1250,  1255,
   -2124, -2124,  2292,   710,  1260,  5126,  1261, 10548, 10548, 10548,
   10548,  9474, 10548,  1021, -2124,  1269, -2124,  1270,  5573,  3281,
    1238,  1271,  1311,  1314,  1276,  1277,  5653,  9832, 10190,  9832,
    9832,  6144,  1238,  1278, 10190,  1145, -2124,   784, 10190, 27393,
    1284,  1285, -2124, -2124,  1509,  1440, -2124,  1030, 16158,  1052,
   10190, -2124, -2124, -2124, -2124,  9474, -2124, -2124,  1030,  3729,
   -2124,  1321,   242, -2124,  5894,  9474, -2124, -2124,  1525, -2124,
     397, -2124,  1283, -2124, -2124, 19892, 10548, 10190,  1156, 10190,
    1380, 16158, 19878, 10190,  2551, 19594, 19594,   132,   132,    61,
      61,    61,    65, -2124,   100,  1030, -2124, 13639,  1318, 10190,
   10190,  1287, -2124,   712, -2124,  1501, -2124, -2124,   713,   107,
    1599, -2124,  1442, 18111, -2124, -2124, 27393,  1044,   563, -2124,
   -2124,   719, 10190, -2124, -2124,  1837, -2124, -2124, 27393, -2124,
     581,   581,   105,   986, 10190,    69,  1291,  1294, 27393, -2124,
     104,   160,   160,  1394, -2124, -2124, -2124,  1398, -2124, -2124,
    1457,  1543, -2124, -2124,   228,   713,  1463,  1384, -2124,  1370,
   -2124,  1306,  1524,   -23,  1307, -2124, -2124, -2124,  1531,  1532,
   -2124, -2124, 27393, -2124, -2124, -2124,   291, -2124, 21339, 16512,
    6293,  1312,   941,  1487,  1331,  1331,  1331,  1331, -2124,  1232,
     713, 27393, -2124,  1401,   469,  1425,  1390, 24082,    52,   720,
   -2124, 29500, -2124,  1412, 22276, -2124, -2124, 21339, 24383,   228,
     400, -2124,   400,   400,   400,   400, -2124,  1330,  1320,   732,
   -2124, 29500,   267, 24082,  1423,  1404, 29500,  1499,   267,  1588,
   -2124,   160,  1333, -2124, 28898, -2124,  1337,   160, -2124, 29500,
     160, 29199, -2124, -2124, 19553,   486,   486,   486,   486, -2124,
   24082, 24082, 21640, -2124, 27393,  -114, -2124, -2124, -2124, -2124,
     738, -2124,   -65, 27393,  1145, -2124, -2124, -2124, -2124,  1603,
    1604, -2124,   228, -2124,   233,   358,   313,  1473,  1546,  1354,
   25888, -2124,  1355, 19447, 10190, -2124,  1622, 29500, -2124, 24082,
   -2124, -2124, -2124, -2124,   713, -2124, 10190, -2124, -2124, -2124,
   10190, -2124,   713, -2124, 10190,  1609, -2124,  1356,  1356,   531,
    1356,  4770,   764, 10548,   525,  1039, 10548, 10548, 10548, 10548,
   10548, 10548, 10548, 12651, 12994, 29500, 11607, -2124, -2124, -2124,
   10190, 10190,  1616,  1609, -2124, -2124, -2124,   671,   671, 29500,
    1360,  1238,  1361,  1364, 10190, -2124,  1389, -2124,  2814, -2124,
    1684,   228,   -60, -2124, -2124,  1367, -2124, 11264, 10548, 10190,
     745, 10190, -2124, -2124,  1366, -2124, -2124, 29500, 10190,   659,
   19906, -2124, 19947, 10190,  1368, 19972, -2124, 13337, 26189,  1359,
     145, -2124, 15149, 26490,  1061,  1371,  1699,  1405,   760,   770,
   -2124, -2124,  1586,  1386, -2124, -2124,    20, -2124, -2124, -2124,
   -2124, 10190, -2124, -2124, -2124,  1062, -2124, -2124, -2124, 19467,
   -2124, 27393, 27393,   785,    96,   451, -2124, -2124, -2124, 27393,
   27393,   228, 27393, -2124, -2124,   228,   228,  1433,  1439,  1610,
     -23, -2124,  1613,   -23,   -23, -2124, -2124, -2124,    60, -2124,
   -2124,  1150,   399, -2124, -2124, -2124, -2124, -2124, -2124,   540,
     678,  1706, 24082, -2124,   124,    37, 26791, 27092,  1397, 27393,
   27393, 10548, -2124,   489,   263, -2124,  1611, 24082, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124,   986, 29500, 24082,  1655,  1569,
   -2124, 27393, -2124,  1035, -2124,  1232, -2124,  1400, -2124, -2124,
    1402, -2124, -2124, -2124, -2124, -2124, -2124, 22276,  1636, -2124,
   27393, -2124,  1482, -2124, -2124,   959,   228, 27393,  1413,   628,
      71, -2124,   267, -2124,   160, -2124,  1500, -2124, -2124, -2124,
   -2124, -2124, 24082, 24082, 24082, 24082, -2124, -2124, -2124, -2124,
    1659, 25888, -2124, -2124,  8758,  1414, -2124, -2124, -2124, -2124,
    1416, -2124,  1418, -2124, -2124, -2124, -2124, -2124, -2124, -2124,
     107,  1421, -2124, 10190, 19553, -2124,  1417,  1422,  1430, 19553,
   19490,  1443, 19553,  1616,  1352,  1679,  1617,  1451,  1352,  1789,
    1789,   606,   606,   531,   531,   531,  1356, -2124,   764, 19553,
   19553, -2124, -2124, -2124, -2124,  1452, -2124, -2124, -2124,  1238,
   -2124, 10190,  1145, -2124,   107, -2124, -2124, -2124,   912,  1640,
   -2124,   194, 29500, -2124,   818,  1640, 10548, 10190, 10190, 19988,
     273, 10190,  7760,   145,  1453, 27393, -2124,  1455, 13639,  1661,
    1615, -2124, 13639,  1615,   804,  1615, 13639,  1670,   -85, -2124,
    1459, 14847, -2124,  1456,  7401,  1483, 21941,  1044, -2124, -2124,
     528,   228, -2124, -2124, 19553, -2124,   823,   830, -2124, -2124,
    1644, 10190,  1645,   572, 27393, -2124, -2124, -2124,   986, -2124,
   -2124,  1467,   228, -2124,   -23, -2124,   -23, -2124, -2124, -2124,
    1026,  1752, -2124,   608,  1758, -2124,  1708,   327,  1726,  1727,
   -2124,  1739, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124, 10190, -2124,   682,  1352, -2124, -2124, -2124, -2124, -2124,
     934,  1568,    99,  1763,  1756,    52, -2124, -2124, -2124, -2124,
   -2124,  1668, -2124,  1497,  1502,   792, -2124, -2124, 24082,  1717,
    1755, -2124, 21640,   228,  1677,  1635, -2124, 21002,  1687,   108,
   -2124, -2124, -2124,   938, -2124, -2124, -2124, -2124, -2124,  1607,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124,  1681, -2124, -2124,
    8758, -2124, -2124, -2124, 19553, -2124, -2124, -2124, -2124, -2124,
   -2124, 10548,  1512, 29500, -2124, 19510, -2124,   847, -2124, 29500,
     912,  1640,  1640, 10190,  1640, 26189,  1515, 27393,   145, 13639,
   -2124, -2124,   967, -2124, 13639,  1731, -2124, -2124, 13639,  1838,
     228,  1575, -2124,  1579, -2124, 27393,  1527, 27393, -2124,   848,
    1523, -2124,  1529,  1530,  1534,  1535,  1536,  1538, -2124, -2124,
    1777, -2124,   107,  1686, -2124, -2124, 19553, -2124,   713, -2124,
   -2124, 27393, -2124, -2124, -2124, -2124,   228, -2124, -2124, -2124,
   -2124, -2124, 21640,  1885, 24082,   540, 19533,  1724, -2124,  1746,
   27393, 22276, -2124,   511,  1695, -2124,  1589, -2124,   792, 24082,
    1721,  1723, -2124, -2124,   228, -2124, 24082,  1551,  1548, -2124,
   -2124, 23781,  1550,   959, -2124,  1554,  1735,   524, -2124,  1729,
   -2124, -2124,   941, -2124, -2124, 24082, -2124,   855,   849, 29500,
     857, -2124, -2124, -2124,  1640, -2124, 27393,   865, -2124, 10190,
    1560, -2124, -2124, 13639,   967,   228,  1559, -2124,   713,  1680,
    1895, -2124,   866, 27393,   872, 29500, -2124,  1483, 21941, 21941,
   21941, 21941, 21941, 21941,  1896,  1818, -2124, 24082,   879, -2124,
    1618,   174, -2124,  1885,   140, -2124, -2124,  1836, -2124,   380,
    1753,  1926, -2124, -2124, -2124, 21640,    18, -2124,   880, -2124,
   -2124, -2124, -2124,  1044, 27393, 24082, -2124, -2124, 27393,   228,
    1815, -2124, -2124,  1766, -2124,  1044, -2124,   885, -2124,   888,
   -2124, 19553, 27393, -2124, -2124, -2124,   228, -2124,  1927,  1587,
     228, -2124,   910, -2124, -2124,  1584,  1585,  1590,  1592,  1593,
    1594, 10190, 10190,  1672,   934, -2124, 27393,  1877, -2124,  1302,
    1302, -2124, -2124, -2124, -2124, -2124,  1867, -2124, -2124, -2124,
      57, -2124,  1773,  1775,   102,  1608, -2124,   655, -2124, -2124,
   -2124, -2124, -2124,   917, -2124, -2124,  1834,  1881, -2124, -2124,
     948, -2124,   228, -2124, -2124, -2124, 21941, 21941, 21941, 21941,
   21941, 21941,  1238, 19553,  1664,  1657,  1746, -2124,   228,   228,
     228,  1745,   386,   386,  1894, -2124,  1696, -2124,   713,   713,
   29500,  1612, -2124, 20348, -2124, -2124, -2124,  1620, -2124, -2124,
   23781, -2124,  1841, -2124, -2124,  1621,  1614,  1623,  1624,  1625,
    1632,  1627,  1629,  1662,  1753, -2124, -2124, -2124,  1887, -2124,
    1993, -2124,    90, -2124, -2124, 21640, 16158, -2124,   102, -2124,
   -2124, -2124, -2124,   949, -2124, -2124, -2124,   228, -2124, -2124,
      51,  1665, -2124,  1666,  1667,  1671,  1674,   713,   713,  1861,
   -2124,   540,  1793, -2124, -2124, -2124,   985,   -56, -2124, -2124,
   20348,  1646, -2124,   393, -2124, -2124, -2124, -2124, -2124, -2124,
   -2124,  1650,  1654,  1658,  1660,  1669,  1663,   578, 13639, -2124,
   21640, -2124, -2124, 28597, -2124, -2124, -2124,   423, -2124, -2124,
     669,   922, -2124, -2124, -2124, -2124, -2124,   713, -2124,   228,
     145,  1673,  1675, -2124,   393,  1676,   590, 20348,  1807, -2124,
   -2124, -2124,   228,   954, -2124,  1678, -2124, -2124
};

/* YYPGOTO[NTERM-NUM].  */
static const short yypgoto[] =
{
   -2124, -2124, -2124,  1514, -2124,  -567, -2124, -2124, -2124,  1148,
   -2124, -1066, -2124, -2124, -2124, -2124, -2124, -2124, -2124, -2124,
     771, -2124, -2124,  -385,  1251, -2124,   538, -1077, -2124, -2124,
   -2124, -2124,  -623, -2124,  -297, -2124, -2124, -2124, -2124, -2124,
   -2124, -2124,  -338, -2124, -2124,  -596, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124, -2124, -2124,  -820, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124,   779,  -863, -2124,   448,   902,
     450, -2124,   -36, -2124, -2124,  1233,   532,  -884, -1368, -1512,
    -376,  -410,  -337,  -333,  -375, -2124,  -178, -2124, -2124, -2124,
     171, -2124, -2124, -2124,  -737, -2124,    21, -2124, -2124,   -43,
   -2124, -2124, -2124,  1689, -2124, -2124,   824, -1515, -2124, -2124,
   -2124,  -485,  -437, -2124, -2123,     7,    10, -2124, -2124, -2124,
   -2124, -2124, -2124,   478,  -586, -2124,   513, -2124, -2124, -2124,
    -395,  1218,  -474, -2124, -2124, -2124, -2124,  -308, -2124, -2124,
   -2124, -1114, -2124,  2059,    50,    81,  -817, -2124,  1619, -2124,
    1431,  1122,   418,   172, -2124, -2124, -2124, -1574, -2124, -2124,
   -2124, -2124, -2124, -2124,  -240,  -355, -2124,   843,  -824, -2124,
     474, -2124,   483, -1208, -2124,   -22, -2124, -2124, -2124, -2124,
     946, -2124, -1140,  -389, -2124,  -211, -2124, -2124, -2124,  1434,
   -2124, -2124,   343, -2124, -2124, -2124, -2124,  -383,  -449, -2124,
   -2124, -2124,    72, -2124, -2124, -2124, -2124,   749,  -806, -2124,
   -2124, -2124, -2124,  -247, -2124, -2124, -2124,  -579, -2124, -2124,
   -2124,  1000,  2091, -2124,   -47, -2124, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124, -2124, -2124,  -344, -1874, -2124, -2124,   458,
   -1794, -2124, -2124, -2124, -1766, -2124, -2124, -2124,  -633,     0,
     -40,  -396,  -283, -2124, -2124,  1626,  1358, -2124,   -12,   614,
   -2124, -2124, -2124, -2124,   600, -2124,  1303, -2124,   593,  -384,
   -2124, -2124, -2124, -2124, -2124,    98, -2124, -2124, -2124, -2124,
   -2124, -2124, -2124,  1308, -2124, -2124,  1013, -2124, -2124, -1702,
     354, -1695,   111,  -816,  -162,     9, -2124, -1474, -2124,   101,
   -1996,   255,  -334, -2124,  1186,  -539,  1264,  1426, -2124, -2124,
    -572, -2124, -2124,  1513,  1971, -2124, -2124,  2312,  3240, -2124,
    -527,   390, -1172,  3413,   627,  -688, -1008, -2124,  -971,  1125,
    1065,  -359, -2124,  4406,  -715,  4016,   680,  -112,  -720,  -328,
    -161, -2124, -1879, -2124, -2124, -2124, -2124, -2124, -1492, -1605,
    -472,   667, -2124, -2124,   780, -2124, -2124, -2124,  1436,  -672,
   -2124,  -412, -2124,   641,    12,   225,  2181,  -454,  1015,  -423,
      31,   196, -1805,  1493,  1537, -2124,  -160,     5, -2124,   190,
      15,  -100,    -4,  -377, -2124,  -829,  3217,  -796,  -789, -2124,
   -2124
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1343
static const short yytable[] =
{
     120,   869,   509,   344,   348,   600,   633,   659,  1287,   383,
     383,  1156,   868,   872,   383,   911,   383,  1554,   870,  1266,
    1432,   880,   383,   936,   383,   497,   476,  1504,  1009,   522,
     490,   495,   527,  1790,  1035,  1065,   600,   863,  1068,   459,
    1084,   881,   508,   914,   382,   384,  1240,  1560,   501,   410,
    1825,   431,   580,  1241,  1275,  1255,  1014,   465,  1256,   467,
     767,   512,  1441,   607,  1447,  2003,  1285,  1523,  1918,  1257,
     757,   644,    94,  1869,  -701,   879,   463,  2020,  2022,  -705,
   -1194,  1039, -1204, -1249, -1311,  1039,  1442,  1240,   417,   902,
     899,  1443,  2111,  1024,  1241,  1041,  1625,   823,   932,   770,
     640,   776,  2317,  -907,  2396,  1354,  1487,   464,  1041,  1078,
    1134,  2075,  1041,  2157,   940,  1121,  -902,  1971,   984,   621,
    1039,   807,     3,     4,   521,   523,    14,   383,   523,    15,
    1258,    15,  -701,  1674,   819,  2432,   917,  -705,   820,   524,
    -215,   468,   528,   617,  1574,  1520,   816,  1041,   836,  2125,
    1564,  1076,  1039,  1355,   976,   977,  1555,  2532,   779,  2384,
    1803,  1655,   526,   480,  2504,  1439,    33,   411,  -951,  1600,
    2039,   947,  1009,  -951,  1137,   994,  1354,  1279,   600,  1041,
    1920,  1657,    22,  1259,    22,  2260,   420,  -701,  1079,  2292,
    1804,  2294,  -705,   571,   899,   894,  1356,   469,  2072,  1357,
     421,  2378,  2126,  1953,  1601,   800,  2009,   830,  2032,  1450,
     432,  1575,  2438,   821,  1039,   125,   432,  2179,   876,   774,
    1925,   612,   622,   815,  1355,   817,   626,  2512,    27,    41,
     829,  1972,   630,   877,   808,   481,   824,   839,  1074,  1044,
    1045,  1041,  1240,   613,   780,   895,  2261,   126,   647,  1241,
    2180,  1921,  1576,  2379,  2010,    33,  1748,  1342,   771,   618,
    1602,  2385,   585,  1442,  1282,  1358,  1341,  1356,  1443,  2505,
    1357,  2040,  2011,   608,  2270,  1138,   900,   801,  1048,   984,
     871,  1279,    33,  -965,    33,   648,   831,  2012,  -965,   962,
    1362,  2439,  2013,   631,  1425,    42,  1603,  2362,  1604,   919,
    1440,  1439,  2127,  1565,  2533,  1656,  2168,   888,  1308,   933,
    2172,  1577,  2014,   916,  2177,  1316,   457,  2033,    41,  1444,
     623,   976,   977,   619,   905,  1658,  1521,   470,   994,    39,
    1260,    39,   950, -1343,  1359,  2433,  1358,  1367,  2087,  2397,
    1805,  2059,  2398,  1139,   822,    41,  1343,    41,  2150,  2128,
     383,   971,  2440,  2380,   521,  1280,   582,   941,   523,  2381,
     602,  1836,   605,  1506,  1066,   383,   643,   582,  2015,   523,
     833,   611,  1080,  1081,   457,   835,   457,   383,  2529,   426,
     900,  1704,  1605,   383,   934,   575,   521,  1082,  1508,   523,
    2347,   602,   790,   383,   625,   349,  1360,   383,   412,   523,
     606,  -951,  2062,  -907,   582,  1359,  1113,  1114,  1836,  1118,
    1062,  2513,   620,    42,  1025,   120,  -902,  1061,   627,   842,
    2469,   913,   426,  1062,  -907,  2321,   383,  1062,   634,  1132,
     427,  1705,   635,  2036,  2037,  -701,  1445,  -902,  -701,  1821,
    -705,  1150,  2088,  -705, -1194, -1284, -1204, -1249, -1311,   426,
    1122,  1056,  1057,  1058,  1059,  1060,  1061,   890,   383,  2231,
     383,   652,  1062,   457,   383,  1390,  1391,  2278,   426,  1434,
    2275,    15,  2282,  1936,  1334,  1933,  2284,  1344,   649,   383,
     490,  2016,   490,  1659,  1251,  1058,  1059,  1060,  1061,  2387,
    1345,  1784,   962,   663,  1062,   665,  1444,  1752,    33,   668,
     773,  1328,  2064,  1332,  1333,   793,   645,  1643,    33,   650,
     651,   120,   661,    30,   600,  1865,  -965,   806,  1934,   808,
     808,   962,   808,   457,    22,  2041,  1307,  1253,   383,   962,
     826, -1284,  1070,   602,   383,  1248,  1660,   385,   383,  1826,
    1827,   582,  1937,   662,   664,  1056,  1057,  1058,  1059,  1060,
    1061,    30,   809,  2200,   579,  -739,  1062,  2470,  1245,  1004,
    1005,    41,  1435,   834,  1296,  2085,  1436,   457,   814,   837,
      27,   962,  1312,   838,   383,  1071,  1753,   426,  2388,  1754,
    2235,  2353,    21,    94,   582,   727,   629,  1935,   827,  1706,
    1707,  1708,  1931,  1709,  1710,  1711,  1712,  1713,  1714,   810,
    1119,  2341,  -739,   513,  1715,  1317,  2471,  1955,  2086,   867,
    1394,   582,   348,  -710,   383,  2062,  2517,   646,   432,  1363,
    1654,  2389,  2472,  -710,    33,  1006,  2042,    42,   582,   433,
     457,  1377,   128,   811,   912,  1626,  -739,    42,   971,  2517,
     600,   383,  1365,   120,  2113,   514,   383,   383,  2313,   891,
    1371,  1373,   432,   939,  1486,  -710,   120,   810,   383,  1620,
     383,  2114,  2342,  1004,  1005,  1755,  2063,  1120,  1158,  2070,
    -710,    39,  1756,  1996,   351,  1410,   920,   434,   129,  1378,
    2517,   922,   923,  1422,  2217,   962,  1580,    41,  1511,  1466,
    -907,   811,   986,   944,   509,   946,  2518,   435,  2043,  2310,
    1956,  2044,   515,  2102,  -710,  1571,  2055,  1957,  -919,  2057,
    2058,  -907,  -710,  1581,   436,  2064,  1159,  1632,  1633,  2518,
    1012,  -458,  1379,  1848,  2519,  1380,  2115,  2218,  1483,  1006,
     921,  1004,  1005,   449,  1019,  -458,  2322,  1496,  1081,  1500,
     460,  1493,   943,   426,   426,  1109,  2048,  2519,  2116,   477,
     437,  1023,  1082,    42,   134,  1737,  1851,  2323,  1622,   350,
    2518,  1501,  1028,   361,  1255,  1331,   409,  1256,  1275,  -887,
     912,  1985,  1240,  2117,  1252,   912,   912,   962,  1257,  1241,
     962,   479,  1467,   516,  2553,  1108,  1468,  2554,  2519,   510,
     982,  1381,  1089,  -710,  -881,  1110,  -916,  1008,  1704,   496,
    1794,  1240,   806,   806,   363,   806,  -884, -1183,  1241,  1315,
     490,  1004,  1005,  1469,   503,   988,   457,  -916,   130, -1184,
    1296,   412,   490,  1312,  -459,   517,  2550,   997, -1185,   583,
     131,   584,  1833,   438,  1149,  -710,  1164,  1027,  -459,   420,
   -1186,   529,   365,   530,   132,   439,   511,  1552,  1705,  2118,
     440,  1558,  1559,   421,  1837,   576,  -919,  2227,  2119,  2400,
    1382,  1855,   582,  1882,  2310,  1883,  1884,  1885,  1886,   602,
     523,  2407,  2120,   133,   441,   577,   442,  1008,  1272,  2121,
    1135,   796,   531,   912,  1144,   443,   532,  1714,  1856,  1404,
     586,   457,   587,  1715,   509,   962,  1857,   444,   445,  1383,
    1134,  1384,   533,  1704,  1470,   523,   367,  1135,   368,   582,
    1791,  1136,  1498,  2010,   992,   993,   426,   383,   534,  2122,
     383,  1424,   383,   383,  2208,   426,  1125,  1307,  1404,  1318,
    1340,  2011,  2143,  1319,   508,  1807,  1808,  2548,  1595,   383,
    2213,  -514,  2214,  2549,   535,  1730,  2174,  1732,  1733,  2561,
     578,  2013,  1327,  2201,  2113,  2562,  1858,  1592,   536,  1711,
    1712,  1713,  1714,   120,  1137,  1004,  1005,  1088,  1715,  1088,
    1088,  2114,  2068,  2069,  1348,   962,  2156,  2444,  2445,  1891,
    2446,  2447,  2448,   603, -1261,  1901,  1841, -1261, -1343, -1261,
    1859, -1261,  1351,  1561,  1567,   602,  1352,  1562,  1568,  1596,
    1124,  1127,   727,  1597,   396,  1623,  1706,  1707,  1708,  1624,
    1709,  1710,  1711,  1712,  1713,  1714,   615,  1982,  1663,  1926,
     616,  1715,  1652,   537,  2103,  1903,  1653,  2015,  2009,  1433,
     796,  1907,   628,  -701,  1909,  1446,  2115,  1453,   454,  1973,
    1974,   641,   466,  1456,  1288,  1289,  1290, -1296,  1536,  1745,
   -1296,  1704, -1296,   499, -1296,  1671,   507,   426,  2116,  1319,
     538,   539,   653,  1678,  1680,  1138,   540,  1319,  1319,   383,
    1683,  1691,   638,  1781,  1684,  1692,  2010,  1319,   654,   600,
    1792,  1870,  1764,  2117,  1073,  1871,  2240,  2241,   541,  1422,
    1731,  1422,  1422,  1889,  2011,  1795,  1635,  1890,  1636,  1923,
     542,  1705,  1499,  1924,  1494,   543,  1990,   383,   655,  2012,
    1319,  1637,   600,  1638,  2013,  1709,  1710,  1711,  1712,  1713,
    1714,  2028,  1538,  1539,   544,  1319,  1715,  1750,   803,   523,
     805,  2029,   383,  1139,  2014,  1319,  1796,  1797,  1272,  1530,
     383,  1532,   383,  1703,   523,   525,  2038,   657,   611,   611,
    1562,   582,   670,  1540,  2279,   582,   582,  1549,  1550,  2118,
    1704,  1778,  1779,  1143,  1145,  1146,   666,  1525,  2119,   671,
    1374,  1375,  1376,  1557,   672,  1531,  1557,  1534,  1387,  2158,
    1388,  1389,  2120,  2159,  2203,  1541,   673,  1393,  1562,  2121,
    2015,  2204,  1307,   675,  2306,  1562,  1414,  2173,  1415,  2176,
    1709,  1710,  1711,  1712,  1713,  1714,  2229,   777,  2272,  2296,
    1705,  1715,  2159,  1319,   962,  1430,  2346,  1431,  2348,   778,
    1924,  1879,  2159,   434,  1458,   781,  2350,  2361,   782,  2122,
    1073,  1871,  2262,  2363,   962,  1542,  1543,  1871,   783,  1307,
    2375,  2399,  1459,   435,  2376,   941,  2408,  1307,  2280,  2409,
    2159,   784,   962,  1073,   962,   727,   789,  1872,   790,  1591,
     436,   582,  1544,  1709,  1710,  1711,  1712,  1713,  1714,  2130,
     791,  2415,  1497,  1848,  1715,  1871,  1611,  1595,  2450,  1849,
     796,   795,  1073,   797,  1850,  1404,  1404,  1404,  1404,   804,
    1404,   825,   120,   961,   840,  1908,   437,  1911,  1614, -1185,
     962,   842, -1186,  2016,   844,  1628,  1851,  1852,  2131,  2453,
    2509,  1640,  1641,  1073,  2510,  2566,  1545,  1460,   865,  2510,
     827,  1461, -1149,   873,  1462,   874,  -106,  1775,  1845,  1846,
    1847,  1647,   882,   883,   887,  1546,   893,   896,   962,  2097,
    2098,  2099,  2100,  1946,   898,   903,  1547,   906,   907,   908,
     917,   909,   962,   945,  1404,   963,   964,   967,  1853,   968,
     969,   509,   970,  -872,   727,   975,  -879,   546,  1706,  1707,
    1708,  1947,  1709,  1710,  1711,  1712,  1713,  1714,   978,   438,
     962,  1967,   979,  1715,   985,   980,  -869,  -870,   981,   609,
      42,   439,   987,  2552,   990,  1975,   440,   991,   996,   999,
    1000,  1019,  1001,  1037,  1087,   509,  1979,  1854,  -873,  1039,
    1463,  1855,  1002,  -871,  1040,   509,  1003,  1464,  1010,  1011,
     441, -1163,   442,  1993,  1069,  1739,  1077,  1073,  1090,  1103,
    1104,   443,  1111,   514,   602,   809,  1041,  1744,  1856,  1133,
    1135,  1147,  1148,   444,   445,  1019,  1857,  1152,  1746,  1153,
    1154,  1157,  1161,  1155,  1160,  1019,  1243,   971,  1165,   452,
     982,   988,   997,   452,   656,  1244,  1246,   602,  1456,   359,
    1276,  1263,  1283,  1284,   452,   452,   660,   452,  1286,  1292,
    1297,  1309,  1331,  1335,   941,  1766,   669,  1768,  1336,   962,
    2426,  1704,  1337,  2215,  2258,  1366,   961,  1349,  1367,   912,
    1412,  1413,  1108,  1424,  1426,  1428,  1858,  1042,  1429,  1437,
    1438,  1043,   786,  1451,   383,   788,  1455,  1471,  1473,  1487,
     507,  1505,  1517,  1495,  1164,   961,  1518,   799,  1044,  1045,
     961,  1503,  2091,   961,  1512,  1522,  1526,  1524,  1046,  1527,
    1859,  1705,  1528,  1535,   832,  1537,  1553,  1563,  1569,  1572,
    1669,  1583,  1587,  1593,  1588,  1594,  1672,  1598,  1164,  1813,
    1599,  1612,  1613,  1616,  1047,  1618,   452,  1048,  1557,   452,
    1619,  1661,  1621,  1629,  1662,   961,  1630,   523,  1049,  1631,
    1050,  1829,  1642,  1664,  1838,  1665,  1666,  1667,  1668,  1670,
    1272,  1404,  1681,  1686,  1404,  1404,  1404,  1404,  1404,  1404,
    1404,  1404,  1404,  1319,  1404,  1682,  1685,  1775,  1687,  1895,
    1725,  1688,   120,  1726,  1881,   962,  1689,  1742,   120,  1743,
    1747,  1693,  1696,  1912,  1913,  1914,  1915,   885,   886,  1051,
    1717,  1718,  1724,  1751,   892,  1987,  1404,  1727,  1728,  1735,
    1919,  1763,  1782,  1757,  1894,  1740,  1741,  1776,  1780,  1340,
    1785,  1801,   120,  1786,  1802,  1809,  1811,  1812,  1993,  1810,
    1039,  1816,  1815,  1820,  1817,  1040,  1647,  1819,  1822,   660,
    1823,  1824,  1866,  1840,  1842,   582,  2344,  1930,  1843,  1867,
     619,  1888,  2531,  1873,  1896,  1897,  1900,  1041,  1783,   961,
    1887,  1902,  1906,  1088,  1904,   727,  2189,  1928,  1929,  1706,
    1707,  1708,  1938,  1709,  1710,  1711,  1712,  1713,  1714,  1939,
    1127,  1127,  1124,  1945,  1715,  1940,  1942,  1721,  1715,  1720,
    1452,  1976,  1977,  1024,  2008,  1978,  1992,   509,  1986,  2000,
    2025,  2024,  1052,  2026,  2030,  1814,  1053,  1054,  1055,  1404,
    1056,  1057,  1058,  1059,  1060,  1061,  2051,  2031,  1980,  2054,
    2052,  1062,  2056,  1849,  2089,  1981,  1984,  2081,  2093,  1088,
    2095,  2104,  1043, -1180,  2007, -1181,  2108,  2002,  2007,  2023,
    1864,  2132,  2137,  2112,  2140,  2141,   962,  2142,  2145,  1044,
    1045,   961,   962,  2146,   961,  2328,  1921,  2151,   452,   569,
    1088,  2147,  1088,  1088,  1088,  1088,   581,  1164,  1164,  2152,
    2429,  2430,  1105,  2169,  2149,   383,   383,   581,   523,  2170,
     452,  2153,  2178,  2154,  2165,  2167,  2187,  2018,  1048,  1993,
    2190,   452,   569,  2205,  2207,  2273,  2047,  2211,   569,  1049,
    2049,  2050,  2216,  2219,  2063,  1557,  2222,  2223,  1557,  1557,
    2045,  2046,   383,   383,   581,  2082,   383,  2224,  1775,  2230,
    2232,  2234,  1775,  2349,  1932,  2236,  1775,  2520,  2238,  2074,
    2076,  2244,  2239,  2246,  2256,  2257,  2259,  1591,  2265,  2285,
    2349,  2266,  2269,  2283,  1948,  2276,  2288,  2078,  2080,  2290,
    2520,  2083,  1951,  1272,  1404,   452,  2107,  2293,  2297,  2298,
    2299,  2304,   962,  1628,  2300,  2301,  2302,   452,  2303,   961,
    2307,  2314,   120,  2088,  2319,  2325,  2329,   452,  2330,  2326,
    2338,  2110,  2333,  2334,  2340,  2403,  2343,  1647,   962,  2339,
    2352,  2520,  1313,  1314,  2356,  2358,  2360,  2371,  1704,  2410,
     952,   961,  1326,   452,  2372,  1993,   452,  2377,  2332,  2386,
    2390,   452,  2394,   660,  2405,  2406,  2413,  2412,   452,  2416,
    2417,  2428,  2252,  2424,  2431,  2418,  1347,  2419,  2420,  2421,
    2434,  1872,  2436,  2451,  2452,   452,   569,  2461,  2443,  2462,
    2468,   569,  2432,  2489,  2433,  2492,  2499,  2478,  1705,   961,
    2487,   581,  2502,  1052,   660,  2497,  2491,  2498,  2493,  2494,
    2495,  1056,  1057,  1058,  1059,  1060,  1061,  2496,  2007,  2503,
    2528,  2166,  1062,  2530,  2521,  2522,  2523,  2536,  1768,  1775,
    2524,  2542,  1768,  2525,  1775,  2543,  1768,  2186,  1775,  2544,
    2532,  2545,  1497,  1584,   581,   798,  1032,  2402,  2547,  1264,
    2546,  1102,  1585,  2557,  2073,  2560,  2558,  2567,  2427,  1404,
    2210,  1788,  1874,  2422,  1529,  1875,  2202,  2228,   452,   452,
    2464,   581,  1140,   569,  2501,   452,  2437,  1800,  2474,  2435,
    2327,  2106,  1331,   962,  2209,  2243,   614,  2212,   581,  1557,
    1573,  1557,  2563,  2535,  2221,  1876,  2220,  1697,  1698,  1699,
    1700,  1830,  1702,  2508,  2564,   462,   931,  2507,  1941,  1250,
     452,  1364,   953,  2138,  2401,  2488,  1586,   674,  1905,  1899,
    1551,  2263,  2383,   966,  1895,  2559,  2477,  2515,  2253,  1509,
     413,  1927,  1793,   912,  1067,  1798,  1130,   600,  2184,  1507,
    1128,  2004,  2354,  1775,  1510,  2175,  2096,  1407,  2255,  1485,
    1983,   794,   727,   952,  1350,  1761,  2364, -1343, -1343,  1894,
    1709,  1710,  1711,  1712,  1713,  1714,  1759,  1789,  1675,  2139,
     938,  1715,  2267,   889,     0,     0,   962,     0,     0,     0,
       0,  2007,   952,   383,     0,  2395,     0,   952,     0,  1768,
     952,     0,     0,     0,  1768,     0,     0,     0,  1768,   955,
     589,  1591,     0,  2295,   590,   591,   592,     0,   593,   594,
     595,   596,   597,   598,     0,  2287,   961,     0,     0,  1312,
       0,     0,     0,     0,   453,     0,     0,  1164,   453,     0,
       0,     0,   952,     0,     0,     0,     0,     0,   961,   453,
     453,     0,   453,   452,     0,     0,  1628,  1272,     0,     0,
       0,  2312,     0,  1088,     0,     0,     0,     0,   961,     0,
       0,     0,   582,   961,     0,     0,     0,   582,     0,     0,
       0,   961,     0,     0,     0,     0,   961,     0,   961,  2331,
       0,   452,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   383,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1768,     0,     0,     0,     0,     0,  2295,
       0,     0,     0,     0,  1607,   953,     0,     0,     0,  1615,
    2355,     0,   581,     0,   961,     0,  1037,     0,     0,     0,
       0,   453,  1039,     0,   453,  2506,     0,  1040,     0,     0,
       0,     0,     0,     0,   953,     0,   952,     0,     0,   953,
     939,   582,   953,     0,   383,     0,     0,  1639,     0,  1041,
       0,     0,   961,   452,   452,     0,     0,     0,   383,   581,
       0,     0,     0,   452,  2404,     0,   961,     0,     0,     0,
       0,   569,     0,     0,   452,     0,     0,     0,  1775,     0,
    2551,  2411,  1164,     0,   953,  2414,     0,   452,     0,     0,
    1029,     0,   955,  1954,   961,     0,  1958,  1959,  1960,  1961,
    1962,  1963,  1964,  1965,  1966,     0,  1968,  1030,  1497,  2365,
    2366,  2367,  2368,  2369,  2370,   452,     0,     0,     0,     0,
    1042,   955,     0,     0,  1043,     0,   955,     0,   952,   955,
       0,   952,     0,     0,     0,     0,     0,  2454,  1988,     0,
       0,  1044,  1045,  1031,     0,     0,     0,     0,     0,  2486,
       0,  1046,     0,  2465,  2466,  2467,   582,  1032,     0,     0,
       0,     0,     0,  1033,     0,     0,     0,     0,  2485,     0,
       0,   955,     0,     0,     0,     0,     0,  1047,     0,     0,
    1048,     0,   602,  2541,     0,     0,     0,     0,  2357,     0,
       0,  1049,     0,  1050,     0,     0,     0,     0,   953,     0,
     120,     0,     0,   961,     0,     0,     0,     0,     0,     0,
       0,     0,  2511,     0,     0,     0,  2486,     0,     0,     0,
       0,     0,     0,  2540,     0,     0,     0,  2455,  2456,  2457,
    2458,  2459,  2460,     0,     0,  2485,     0,     0,  1768,     0,
       0,  2084,  1051,   453,   570,     0,   952,     0,     0,     0,
       0,   570,     0,     0,     0,   452,     0,     0,     0,     0,
       0,     0,   570,  2486,   120,   453,     0,     0,     0,     0,
       0,     0,     0,     0,  2556,     0,   453,   570,   952,     0,
       0,  1039,  2485,   570,     0,   955,  1040,  2565,     0,     0,
     953,     0,     0,   953,     0,     0,     0,     0,     0,   570,
       0,   581,     0,     0,     0,   581,   581,     0,  1041,     0,
       0,   589,  1868,     0,     0,   590,   591,   592,     0,   593,
     594,   595,   596,   597,   598,     0,   952,     0,     0,   961,
       0,     0,     0,     0,     0,     0,     0,     0,  2475,  2476,
     453,     0,     0,     0,     0,  1052,     0,   956,     0,  1053,
    1054,  1055,   453,  1056,  1057,  1058,  1059,  1060,  1061,     0,
       0,     0,   453,     0,  1062,  1916,  1917,  1690,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   955,     0,  1042,
     955,     0,     0,  1043,     0,     0,  2160,     0,   453,     0,
       0,   453,     0,     0,     0,     0,   453,  2526,  2527,     0,
    1044,  1045,     0,   453,     0,     0,     0,     0,   953,     0,
    1046,   581,     0,     0,     0,     0,     0,     0,     0,     0,
     453,   570,     0,     0,     0,   452,   570,     0,     0,     0,
     452,     0,     0,     0,     0,     0,   570,     0,     0,  1048,
     953,     0,     0,     0,     0,     0,     0,  2555,     0,     0,
    1049,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   452,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   570,
       0,     0,     0,     0,     0,     0,     0,     0,   953,     0,
     961,     0,     0,     0,     0,   955,   961,     0,     0,     0,
       0,  1051,     0,   453,   453,     0,   570,     0,   570,     0,
     453,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   570,     0,     0,     0,   955,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1037,     0,
       0,  2268,     0,   952,  1039,   453,     0,     0,     0,  1040,
     956,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   952,     0,  2071,     0, -1340,
       0,  1041,     0,     0,     0,   955,     0,     0,     0,   956,
       0,     0,  2090,     0,   956,   952,     0,   956,     0,     0,
     952,     0,  2092,     0,     0,     0,     0,     0,   952,     0,
       0,     0,     0,   952,  1052,   952,   961,     0,  1053,  1054,
    1055,     0,  1056,  1057,  1058,  1059,  1060,  1061,     0,     0,
       0,     0,     0,  1062,     0,     0,     0, -1340,     0,   956,
       0,     0,   961,  1774,     0,     0,     0,  2133,  2134,  2135,
    2136,     0,  1042,     0,     0,     0,  1043,     0,     0,     0,
       0,   952,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0, -1340,  1044,  1045,     0,     0,     0,     0,     0,
       0,     0, -1340,  1046,     0,     0,     0, -1340,   453,     0,
       0,     0, -1340, -1340,     0, -1340,     0,     0,   958,   952,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1047,
       0,     0,  1048,   952, -1340,   953,     0,     0,     0,     0,
       0,     0,     0,  1049,     0,  1050,   453,     0,     0, -1340,
       0,     0,     0,     0,     0,     0,     0,   953,     0,     0,
       0,   952,     0,   452,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   956,  1878,     0,     0,   953,     0,     0,
       0,  2199,   953, -1340,     0,     0,     0,   570,     0,  1893,
     953, -1340,     0,     0,  1051,   953,     0,   953,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   961,     0,     0,
       0,     0,     0,     0,     0,     0,   452,   452,   569,     0,
       0,     0,   955,     0,     0,     0,     0,     0,   453,   453,
       0,     0,     0,     0,   570,     0,     0,     0,   453,     0,
       0,     0,     0,   953,   955,     0,   570,     0,     0,   453,
       0,     0,     0,   460,     0,   581,     0,     0,     0,     0,
       0,     0,   453,     0,   955,   956,     0,     0,   956,   955,
     952,     0, -1340,     0,     0,     0,     0,   955,     0,     0,
   -1340,   953,   955,     0,   955,     0,     0,     0,     0,     0,
     453,     0, -1340,     0,     0,   953,     0,  1052,     0,     0,
     961,  1053,  1054,  1055,     0,  1056,  1057,  1058,  1059,  1060,
    1061, -1340,     0, -1340, -1340,     0,  1062,     0,     0, -1087,
       0,   958,     0,   953,     0,     0,     0, -1340, -1340,     0,
     955,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1774,     0,     0,     0,     0,     0,     0,
     958,     0,     0,     0,     0,   958,     0,     0,   958,     0,
   -1340,     0, -1340, -1340,     0,     0,     0,     0,   955,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  2316,
       0,     0,   955,   956,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   660,     0,   952,     0,     0,     0,
     958,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     955,     0,     0,     0,     0,   956,     0,     0,   452,     0,
    2345,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     453,     0,   953,   452,     0,  1037,     0,     0,     0,     0,
       0,  1039,     0,   452,     0,     0,  1040,     0,     0,     0,
       0,     0,     0,  2199,  2199,  2199,  2199,  2199,  2199,     0,
       0,     0,  2374,   956,     0,     0,     0,     0,  1041,     0,
       0,     0,     0,     0,     0,     0,   570,     0,     0,     0,
     570,   570,     0,     0,     0,     0,     0,     0,   452,   452,
     452,   452,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   958,     0,     0,     0,     0,   955,
       0,     0,     0,     0,  1720,     0,     0,     0,     0,  1721,
       0,     0,     0,     0,     0,     0, -1342,     0,     0,  1042,
       0,     0,     0,  1043,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   952,   953,     0,
    1044,  1045,     0,   952,     0,     0,     0,     0,     0,     0,
    1046,  2199,  2199,  2199,  2199,  2199,  2199,     0,     0,     0,
       0,     0,     0,     0,  1774,     0,   570,     0,  1774,     0,
       0,     0,  1774,     0, -1342,     0,  1047,     0,     0,  1048,
     453,     0,   452,     0,     0,   453,   958,     0,     0,   958,
    1049,     0,  1050,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0, -1342,
       0,     0,     0,     0,     0,     0,     0,     0,     0, -1342,
       0,     0,     0,   453, -1342,   955,     0,     0,     0, -1342,
   -1342,     0, -1342,     0,     0,     0,     0,     0,     0,     0,
       0,  1051,     0,     0,     0,     0,     0,     0,     0,     0,
       0, -1342,     0,   952,     0,     0,     0,     0,     0,     0,
     956,     0,     0,     0,  1893,     0, -1342,     0,   569,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   952,
       0,     0,   956,     0,     0,     0,     0,     0,     0,     0,
     573,     0,     0,     0,     0,     0,     0,     0,     0,   953,
   -1342,     0,   956,     0,   958,   953,     0,   956, -1342,     0,
       0,     0,     0,     0,     0,   956,     0,     0,     0,     0,
     956,     0,   956,   573,     0,  1774,     0,     0,     0,   573,
    1774,     0,     0,     0,  1774,     0,   958,     0,     0,     0,
       0,     0,     0,     0,  1052,     0,     0,     0,  1053,  1054,
    1055,     0,  1056,  1057,  1058,  1059,  1060,  1061,     0,     0,
       0,     0,     0,  1062,     0,     0,     0,     0,   956,     0,
     460,     0,     0,     0,     0,     0,     0,     0,   569,     0,
     452,     0,     0,     0,   958,  1037,   955,  1038,   453, -1342,
       0,  1039,   955,     0,     0,   452,  1040, -1342,     0,     0,
       0,     0,   581,     0,     0,     0,   956,   581,     0, -1342,
       0,     0,     0,     0,     0,   953,     0,     0,  1041,   764,
     956,   452,     0,     0,   952,     0,     0,     0, -1342,     0,
   -1342, -1342,     0,     0,     0,     0,     0,     0,     0,  1774,
       0,   953,     0,     0, -1342, -1342,     0,     0,   956,     0,
       0,     0,     0,     0,   452,   452,   452,   452,   452,   452,
       0,     0,     0,   452,     0,     0,     0,   573,     0,  1039,
       0,     0,   573,     0,  1040,     0,     0, -1342,     0, -1342,
   -1342,   569,     0,     0,     0,     0,     0,     0,   453,  1042,
       0,   581,     0,  1043,   386,     0,  1041,     0,     0,   570,
       0,     0,   955,   387,     0,     0,     0,     0,     0,     0,
    1044,  1045,     0,   864,   570,     0,     0,   952,     0,     0,
    1046,   388,     0,     0,     0,     0,     0,     0,   955,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   389,
       0,   453,   453,   570,     0,     0,  1047,     0,     0,  1048,
       0,     0,     0,   390,   573,     0,     0,   956,     0,     0,
    1049,     0,  1050,     0,     0,   864,     0,  1042,     0,   391,
       0,  1043,   452,   452,   452,   452,   452,   452,   764,     0,
     570,     0,     0,     0,     0,     0,   953,     0,  1044,  1045,
       0,     0,     0,     0,     0,   392,     0,     0,  1046,   393,
       0,   958,     0,   864,     0,     0,   581,     0,     0,   394,
       0,  1051,     0,     0,     0,     0,   764,     0,     0,     0,
       0,     0,     0,   958,  1047,     0,   959,  1048,     0,     0,
       0,   569,     0,     0,     0,     0,     0,     0,  1049,     0,
     764,     0,     0,   958,     0,     0,     0,     0,   958,     0,
       0,     0,     0,     0,     0,     0,   958,     0,     0,     0,
       0,   958,     0,   958,     0,     0,   764,   764,   453,   764,
     764,   764,     0,   955,   395,     0,     0,     0,     0,   953,
       0,     0,     0,   956,  1774,     0,   569,   764,     0,  1051,
       0,     0,     0,     0,   396,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   958,
       0,   397,   398,     0,  1052,     0,     0,   399,  1053,  1054,
    1055,     0,  1056,  1057,  1058,  1059,  1060,  1061,     0,     0,
       0,     0,     0,  1062,     0,     0,     0,     0,     0,   400,
       0,   764,     0,   453,     0,     0,     0,   958,     0,     0,
       0,   401,     0,     0,     0,     0,   402,     0,   453,     0,
       0,   958,     0,     0,     0,   403,   955,     0,   453,     0,
       0,     0,     0,     0,     0,   404,     0,     0,     0,  1239,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   958,
       0,     0,  1052,     0,   405,   406,  1053,  1054,  1055,   960,
    1056,  1057,  1058,  1059,  1060,  1061,   864,     0,     0,     0,
       0,  1062,     0,   453,   453,   453,   453,     0,     0,     0,
    1239,     0,     0,     0,     0,     0,     0,     0,     0,   959,
       0,     0,     0,   764,     0,   864,     0,     0,     0,     0,
     864,     0,     0,   864,   956,     0,     0,     0,     0,     0,
     956,     0,     0,     0,     0,     0,     0,     0,   959,     0,
       0,     0,   573,   959,     0,     0,   959,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   864,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   958,     0,
       0,     0,     0,     0,     0,     0,     0,   764,   959,   453,
       0,     0,   764,   453,     0,   764,   764,   453,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   453,     0,     0,
       0,     0,     0,   764,     0,     0,     0,   764,   764,   764,
     764,     0,     0,  1037,     0,     0,  1513,   764,   764,  1039,
       0,     0,     0,     0,  1040,     0,     0,     0,     0,     0,
     956,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   764,  1239,  1041,   764,     0,   764,
       0,     0,     0,   764,     0,     0,   956,   764,     0,     0,
     764,   764,   764,   764,   764,   764,   764,   764,   764,   864,
     764,     0,   960,   764,     0,     0,   764,     0,     0,   570,
    1514,     0,     0,   570,     0,     0,     0,     0,     0,     0,
       0,     0,   959,     0,   958,     0,     0,     0,     0,     0,
       0,   960,     0,     0,     0,     0,   960,     0,     0,   960,
       0,     0,     0,     0,     0,     0,     0,  1042,     0,     0,
       0,  1043,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1044,  1045,
     453,     0,     0,     0,     0,   453,     0,     0,  1046,   453,
       0,   960,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   864,     0,     0,   864,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1047,     0,     0,  1048,     0,     0,
       0,     0,     0,     0,   959,     0,     0,   959,  1049,     0,
    1050,     0,     0,   570,     0,   453,     0,     0,     0,     0,
       0,   956,     0,     0,     0,     0,     0,     0,     0,     0,
     453,     0,     0,     0,     0,     0,     0,   570,     0,     0,
       0,     0,   570,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   453,     0,     0,  1051,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     764,   864,     0,     0,   453,   958,     0,     0,     0,     0,
       0,   958,     0,     0,     0,   960,     0,     0,     0,   453,
     453,   453,   453,   453,   453,     0,     0,     0,   453,   864,
    1515,     0,     0,     0,     0,     0,     0,     0,   751,     0,
       0,     0,     0,     0,   956,     0,   570,     0,     0,     0,
       0,     0,   959,     0,   864,     0,   570,     0,     0,     0,
       0,   864,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   764,     0,     0,     0,
       0,     0,     0,     0,   959,     0,     0,     0,     0,     0,
       0,     0,  1052,     0,   764,     0,  1053,  1054,  1055,     0,
    1056,  1057,  1058,  1059,  1060,  1061,     0,   960,     0,   864,
     960,  1062,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   958,     0,     0,   764,     0,     0,     0,     0,     0,
       0,     0,   959,     0,     0,     0,     0,   453,   453,   453,
     453,   453,   453,     0,     0,     0,     0,   958,     0,     0,
       0,     0,     0,     0,   764,   764,   764,   764,   764,   764,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   570,     0,     0,   764,   764,   764,   764,     0,     0,
       0,   764,     0,     0,     0,   764,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   570,   764,     0,     0,
       0,     0,   764,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   764,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   764,   764,   960,   764,     0,     0,     0,
     764,     0,     0,     0,     0,   973,     0,     0,     0,     0,
       0,     0,     0,     0,   573,     0,   764,   764,     0,   453,
       0,   570,     0,     0,     0,     0,     0,   960,     0,   995,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   764,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   764,     0,     0,     0,  1015,  1016,     0,  1017,  1018,
    1021,     0,   958,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   960,  1036,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1037,  1239,   864,     0,     0,     0,
    1039,     0,     0,     0,     0,  1040,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   864,   959,
       0,     0,     0,     0,  1239,  1880,     0,  1041,     0,     0,
    1117,     0,     0,     0,     0,     0,     0,     0,   864,     0,
       0,   959,     0,   864,     0,     0,     0,     0,     0,     0,
       0,   864,     0,     0,     0,   958,   864,     0,   864,     0,
       0,   959,     0,     0,     0,     0,   959,     0,     0,   573,
       0,     0,     0,     0,   959,     0,     0,     0,     0,   959,
       0,   959,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1042,     0,
       0,   764,  1043,     0,   864,     0,     0,     0,     0,     0,
       0,     0,     0,   764,     0,     0,     0,   764,     0,  1044,
    1045,   764,  1293,     0,     0,     0,     0,   959,     0,  1046,
     764,     0,     0,   764,   764,   764,   764,   764,   764,   764,
     764,   764,   864,   764,     0,     0,     0,   764,   764,     0,
       0,     0,     0,     0,     0,  1047,   864,     0,  1048,     0,
       0,   764,     0,     0,     0,   959,     0,     0,     0,  1049,
       0,  1050,     0,     0,   764,   764,   764,     0,   764,   959,
       0,     0,     0,     0,   864,   764,     0,     0,     0,     0,
     764,     0,   960,     0,   573,     0,     0,     0,     0,     0,
       0,  1370,     0,     0,     0,  1372,     0,   959,     0,     0,
       0,     0,     0,     0,   960,     0,     0,     0,   764,     0,
    1051,     0,  1392,     0,     0,     0,  1395,     0,  1408,  1409,
       0,     0,     0,     0,   960,     0,  1416,  1421,     0,   960,
       0,     0,     0,     0,     0,     0,     0,   960,     0,     0,
       0,     0,   960,     0,   960,     0,     0,     0,     0,     0,
       0,     0,     0,  1449,     0,     0,     0,     0,  1454,     0,
       0,     0,  1465,     0,     0,     0,  1472,     0,   764,  1474,
    1475,  1476,  1477,  1478,  1479,  1480,  1481,  1482,     0,  1484,
       0,     0,   751,   864,     0,     0,     0,     0,     0,     0,
     960,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   959,     0,     0,     0,
       0,     0,     0,  1052,     0,     0,     0,  1053,  1054,  1055,
       0,  1056,  1057,  1058,  1059,  1060,  1061,     0,   960,     0,
       0,  1427,  1062,     0,     0,     0,     0,     0,     0,     0,
    1037,   764,   960,     0,     0,     0,  1039,     0,     0,     0,
       0,  1040,     0,     0,     0,     0,     0,  1063,     0,     0,
     764,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     960,     0,     0,  1041,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   764,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   864,
       0,     0,     0,   764,   764,   764,     0,     0,   764,     0,
       0,     0,     0,     0,     0,   573,     0,     0,     0,   573,
       0,     0,   959,   573,     0,     0,  1037,     0,  1677,     0,
       0,   764,  1039,     0,  1042,     0,     0,  1040,  1043,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   764,  1570,
       0,     0,     0,     0,     0,  1044,  1045,     0,     0,  1041,
       0,     0,     0,     0,     0,  1046,     0,  1037,     0,   960,
       0,     0,     0,  1039,     0,     0,     0,     0,  1040,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   764,     0,
       0,  1047,     0,     0,  1048,     0,     0,     0,     0,     0,
    1041,     0,     0,     0,     0,  1049,     0,  1050,     0,     0,
       0,  1063,     0,     0,     0,     0,     0,     0,  1694,   573,
       0,     0,     0,     0,     0,  1634,     0,     0,     0,     0,
    1042,     0,     0,     0,  1043,     0,     0,     0,     0,     0,
       0,     0,     0,   751,     0,     0,     0,   764,     0,     0,
       0,  1044,  1045,     0,     0,     0,  1051,     0,   764,     0,
     864,  1046,     0,     0,     0,     0,   864,     0,     0,  1063,
     764,  1042,     0,  1673,     0,  1043,   573,     0,     0,     0,
       0,   573,     0,   959,     0,   573,     0,  1047,     0,   959,
    1048,  1063,  1044,  1045,     0,   960,     0,  1402,     0,     0,
       0,  1049,  1046,  1050,     0,     0,     0,  1701,     0,     0,
       0,  1063,  1063,  1063,  1063,     0,     0,  1063,     0,     0,
       0,     0,     0,  1421,     0,  1421,  1421,     0,  1047,   573,
    1736,  1048,  1063,     0,  1738,     0,  1402,     0,     0,     0,
       0,     0,  1049,     0,  1050,     0,     0,     0,     0,     0,
       0,     0,  1051,     0,     0,     0,     0,     0,     0,  1052,
       0,     0,     0,  1053,  1054,  1055,     0,  1056,  1057,  1058,
    1059,  1060,  1061,  1760,     0,  1762,   864,     0,  1062,  1765,
       0,     0,     0,     0,     0,     0,   764,     0,     0,     0,
     573,     0,     0,  1051,     0,     0,     0,     0,     0,   959,
       0,     0,   864,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1063,     0,     0,     0,     0,  1117,     0,
       0,     0,     0,     0,     0,   959,     0,     0,     0,     0,
    1799,     0,   573,     0,  1679,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1052,   960,     0,     0,  1053,
    1054,  1055,   960,  1056,  1057,  1058,  1059,  1060,  1061,     0,
       0,     0,     0,     0,  1062,     0,     0,  1037,   764,   764,
       0,     0,     0,  1039,     0,     0,     0,     0,  1040,     0,
       0,     0,     0,     0,     0,     0,  1052,     0,     0,     0,
    1053,  1054,  1055,     0,  1056,  1057,  1058,  1059,  1060,  1061,
    1041,     0,     0,     0,     0,  1062,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   864,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1037,     0,  1729,
       0,     0,     0,  1039,     0,     0,     0,     0,  1040,     0,
     959,     0,   960,     0,     0,     0,     0,     0,     0,     0,
    1944,  1042,   573,     0,     0,  1043,     0,     0,     0,  1063,
    1041,     0,  1949,     0,     0,     0,  1950,     0,   960,     0,
    1952,     0,  1044,  1045,     0,     0,     0,     0,     0,     0,
       0,     0,  1046,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1969,  1970,     0,     0,
       0,     0,     0,     0,     0,   573,     0,   573,  1047,     0,
     864,  1048,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1049,     0,  1050,  1989,     0,  1991,     0,     0,
       0,  1042,     0,   959,  1995,  1043,  1063,     0,  1063,  1999,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1044,  1045,     0,     0,     0,     0,  1063,     0,
       0,  1063,  1046,  1402,  1402,  1402,  1402,  2034,  1402,  1716,
       0,     0,     0,  1051,  1063,  1063,     0,     0,     0,     0,
       0,     0,  1063,     0,     0,     0,     0,  1063,  1047,     0,
       0,  1048,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1049,     0,  1050,     0,     0,     0,     0,     0,
       0,     0,     0,   960,     0,  1063,     0,     0,  1716,     0,
    1063,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1063,  1402,     0,     0,     0,     0,     0,  1063,     0,
    1063,  1063,  1063,  1063,  1063,  1063,  1063,  1063,  1063,     0,
    1063,     0,     0,  1051,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1039,     0,  1052,     0,     0,  1040,
    1053,  1054,  1055,     0,  1056,  1057,  1058,  1059,  1060,  1061,
       0,     0,     0,     0,  1719,  1062,     0,     0,     0,     0,
     751,  1041,     0,     0,     0,     0,   960,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  2144,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1063,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1749,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1052,  2155,     0,     0,
    1053,  1054,  1055,     0,  1056,  1057,  1058,  1059,  1060,  1061,
       0,     0, -1343,  2161,  2162,  1062,  1043,  2164,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1044,  1045,     0,     0,     0,     0,     0,
    1063,     0,     0, -1343,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  2206,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1048,     0,     0,     0,     0,     0,     0,  1063,
       0,     0,     0,  1049,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  2226,     0,     0,
       0,     0,     0,  1716,  1716,  1716,  1716,  1063,  1716,  1402,
       0,     0,  1402,  1402,  1402,  1402,  1402,  1402,  1402,  1402,
    1402,     0,  1402,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0, -1343,     0,     0,     0,     0,     0,
       0,     0,  1063,     0,  1063,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1402,     0,   751,     0,  1037,     0,
       0,     0,     0,     0,  1039,  1716,  1063,     0,  1063,  1040,
       0,  1063,     0,     0,     0,     0,     0,     0,     0,  2274,
       0,     0,     0,     0,     0,     0,     0,     0,     1,     0,
       0,  1041,     0,     0,     0,     0,     2,     3,     4,     0,
       0,     0,     0,     0,     0,  1063,     0,     0,     0,     5,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     6,     0,     7,
       8,     0,     0,     0,     9,    10,     0,  1052,     0,     0,
       0,    11,     0,    12,     0,  1056,  1057,  1058,  1059,  1060,
    1061,     0,  1734,     0,     0,    13,  1062,  1402,    14,     0,
       0,     0,  1042,    15,     0,     0,  1043,     0,     0,     0,
       0,    16,     0,     0,     0,     0,    17,     0,     0,     0,
      18,     0,    19,  1044,  1045,     0,    20,     0,     0,     0,
       0,     0,     0,  1046,     0,  2351,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,  1037,     0,     0,
       0,     0,     0,  1039,     0,     0,    22,     0,  1040,  1047,
       0,     0,  1048,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1049,     0,  1050,    23,    24,     0,     0,
    1041,     0,    25,     0,     0,     0,     0,     0,     0,    26,
    1063,     0,     0,     0,     0,  1063,  1063,     0,  1063,     0,
    1716,     0,    27,     0,  1716,  1716,  1716,  1716,  1716,  1716,
    1716,  1716,  1716,     0,  1716,  1063,  1063,     0,     0,     0,
       0,     0,     0,     0,  1051,     0,     0,     0,  2423,     0,
      28,     0,     0,     0,  1716,  1063,     0,  1063,     0,     0,
      29,  1063,  1402,     0,    30,  1063,     0,    31,     0,    32,
       0,  1042,     0,     0,     0,  1043,    33,     0,     0,     0,
       0,    34,     0,     0,    35,     0,     0,     0,     0,     0,
      36,     0,  1044,  1045,     0,     0,     0,     0,     0,     0,
    1063,     0,  1046,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    37,     0,     0,     0,     0,     0,
       0,    38,     0,    39,     0,     0,     0,    40,  1047,     0,
       0,  1048,     0,     0,     0,     0,     0,     0,     0,    41,
       0,     0,  1049,     0,  1050,     0,     0,  1052,     0,     0,
    1716,  1053,  1054,  1055,     0,  1056,  1057,  1058,  1059,  1060,
    1061,     0,     0,     0,     0,     0,  1062,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1051,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    42,     0,     0,     0,     0,
    1063,     0,     0,     0,     0,     0,     0,  1402,     0,     0,
       0,  1063,     0,     0,     0,     0,  1716,  1063,  1063,     0,
    1063,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1063,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1063,     0,     0,     0,  1052,     0,     0,     0,
    1053,  1054,  1055,     0,  1056,  1057,  1058,  1059,  1060,  1061,
       0,     0,     0,     0,  1839,  1062,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1716,     0,     0,     0,     0,     0,
    1063,     0,     0,     0,     0,     0,   135,   136,   137,   138,
     139,   140,   141,  1489,   142,     0,     0,     0,     0,     0,
       0,   143,   144,   145,   547,   146,   147,   148,   548,   676,
     549,   677,   678,     0,   152,   153,   154,   155,   679,   680,
     156,   681,   682,   159,     0,   160,   161,   162,   163,   683,
       0,     0,   165,   166,   167,     0,   168,   169,   684,   171,
       0,     0,   172,   173,   550,   685,   686,   687,   688,   174,
     175,   176,   177,   178,   689,   690,   181,  1063,     0,   182,
     183,   184,   185,   186,     0,  1490,     0,   187,   691,   189,
     190,     0,   191,   192,     0,   193,     0,   194,   195,   692,
     197,   198,   693,   694,   200,   695,     0,   202,     0,   203,
     551,     0,   552,   204,   205,   206,     0,     0,   207,     0,
     208,   553,   209,   210,   211,   554,   212,   213,   214,     0,
     555,   215,   216,   217,   218,   219,   696,   697,     0,   698,
       0,   223,   556,   557,   224,   558,   225,   226,   227,  1063,
     559,   228,   560,     0,   229,   230,   231,   699,   700,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   701,
     561,   702,   450,   244,   245,   246,   247,   248,   703,   249,
     250,   562,   704,   705,   706,   253,     0,     0,   254,   451,
       0,     0,   707,   256,     0,     0,   257,   563,   564,   708,
     259,   260,   261,   262,   263,     0,   709,   265,   266,     0,
     267,   268,   269,   270,   271,   710,   273,     0,   274,   275,
     276,   277,   278,   279,   280,   281,   565,   282,   711,   284,
     285,   286,   287,   288,     0,   289,   290,   291,   712,   292,
     293,   294,   295,   566,   296,   713,     0,   298,   299,   300,
     301,   302,   303,   304,   305,   714,   307,     0,   308,   309,
     310,     0,   715,   716,     0,   313,     0,   314,   717,   316,
     718,   719,   318,   319,   320,   321,     0,   720,   322,   323,
     324,   325,   326,   721,     0,   327,   328,   329,   330,   722,
     332,   567,   333,   334,   335,     0,     0,   336,   337,   338,
     339,   340,   341,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     723,   724,   457,     0,   725,   726,   727,   426,   728,     0,
       0,     0,     0,     0,   729,   730,  1491,     0,   732,   733,
       0,     0,     0,   734,  1492,   135,   136,   137,   138,   139,
     140,   141,     0,   142,     0,     0,     0,     0,     0,     0,
     143,   144,   145,   547,   146,   147,   148,   548,   676,   549,
     677,   678,     0,   152,   153,   154,   155,   679,   680,   156,
     681,   682,   159,     0,   160,   161,   162,   163,   683,     0,
       0,   165,   166,   167,     0,   168,   169,   684,   171,     0,
       0,   172,   173,   550,   685,   686,   687,   688,   174,   175,
     176,   177,   178,   689,   690,   181,     0,     0,   182,   183,
     184,   185,   186,     0,     0,     0,   187,   691,   189,   190,
       0,   191,   192,     0,   193,     0,   194,   195,   692,   197,
     198,   693,   694,   200,   695,     0,   202,     0,   203,   551,
       0,   552,   204,   205,   206,     0,     0,   207,     0,   208,
     553,   209,   210,   211,   554,   212,   213,   214,     0,   555,
     215,   216,   217,   218,   219,   696,   697,     0,   698,     0,
     223,   556,   557,   224,   558,   225,   226,   227,     0,   559,
     228,   560,     0,   229,   230,   231,   699,   700,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   701,   561,
     702,   450,   244,   245,   246,   247,   248,   703,   249,   250,
     562,   704,   705,   706,   253,     0,     0,   254,   451,     0,
       0,   707,   256,     0,     0,   257,   563,   564,   708,   259,
     260,   261,   262,   263,     0,   709,   265,   266,     0,   267,
     268,   269,   270,   271,   710,   273,     0,   274,   275,   276,
     277,   278,   279,   280,   281,   565,   282,   711,   284,   285,
     286,   287,   288,     0,   289,   290,   291,   712,   292,   293,
     294,   295,   566,   296,   713,     0,   298,   299,   300,   301,
     302,   303,   304,   305,   714,   307,     0,   308,   309,   310,
       0,   715,   716,     0,   313,     0,   314,   717,   316,   718,
     719,   318,   319,   320,   321,     0,   720,   322,   323,   324,
     325,   326,   721,     0,   327,   328,   329,   330,   722,   332,
     567,   333,   334,   335,     0,     0,   336,   337,   338,   339,
     340,   341,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   723,
     724,   457,     0,   725,   726,   727,   426,   728,     0,     0,
       0,     0,     0,   729,   730,     0,     0,   732,   733,     0,
       0,     0,   734,  1406,   135,   136,   137,   138,   139,   140,
     141,     0,   142,     0,     0,     0,     0,     0,     0,   143,
     144,   145,   547,   146,   147,   148,   548,   676,   549,   677,
     678,     0,   152,   153,   154,   155,   679,   680,   156,   681,
     682,   159,     0,   160,   161,   162,   163,   683,     0,     0,
     165,   166,   167,     0,   168,   169,   684,   171,     0,     0,
     172,   173,   550,   685,   686,   687,   688,   174,   175,   176,
     177,   178,   689,   690,   181,     0,     0,   182,   183,   184,
     185,   186,     0,     0,     0,   187,   691,   189,   190,     0,
     191,   192,     0,   193,     0,   194,   195,   692,   197,   198,
     693,   694,   200,   695,     0,   202,     0,   203,   551,     0,
     552,   204,   205,   206,     0,     0,   207,     0,   208,   553,
     209,   210,   211,   554,   212,   213,   214,     0,   555,   215,
     216,   217,   218,   219,   696,   697,     0,   698,     0,   223,
     556,   557,   224,   558,   225,   226,   227,     0,   559,   228,
     560,     0,   229,   230,   231,   699,   700,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   701,   561,   702,
     450,   244,   245,   246,   247,   248,   703,   249,   250,   562,
     704,   705,   706,   253,     0,     0,   254,   451,     0,     0,
     707,   256,     0,     0,   257,   563,   564,   708,   259,   260,
     261,   262,   263,     0,   709,   265,   266,     0,   267,   268,
     269,   270,   271,   710,   273,     0,   274,   275,   276,   277,
     278,   279,   280,   281,   565,   282,   711,   284,   285,   286,
     287,   288,     0,   289,   290,   291,   712,   292,   293,   294,
     295,   566,   296,   713,     0,   298,   299,   300,   301,   302,
     303,   304,   305,   714,   307,     0,   308,   309,   310,     0,
     715,   716,     0,   313,     0,   314,   717,   316,   718,   719,
     318,   319,   320,   321,     0,   720,   322,   323,   324,   325,
     326,   721,     0,   327,   328,   329,   330,   722,   332,   567,
     333,   334,   335,     0,     0,   336,   337,   338,   339,   340,
     341,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   723,   724,
     457,     0,   725,   726,   727,   426,   728,     0,     0,     0,
       0,     0,   729,   730,     0,     0,   732,   733,     0,     0,
       0,   734,  2188,   135,   136,   137,   138,   139,   140,   141,
       0,   142,     0,     0,     0,     0,  2005,     0,   143,   144,
     145,     0,   146,   147,   148,     0,   149,     0,   150,   151,
       0,   152,   153,   154,   155,     0,     0,   156,   157,   158,
     159,     0,   160,   161,   162,   163,   164,     0,     0,   165,
     166,   167,     0,   168,   169,   170,   171,     0,     0,   172,
     173,  -807,     0,     0,     0,     0,   174,   175,   176,   177,
     178,   179,   180,   181,     0,     0,   182,   183,   184,   185,
     186,     0,     0,     0,   187,   188,   189,   190,     0,   191,
     192,     0,   193,     0,   194,   195,   196,   197,   198,   199,
       0,   200,   201,     0,   202,     0,   203,     0,     0,  -807,
     204,   205,   206,     0,     0,   207,     0,   208,     0,   209,
     210,   211,     0,   212,   213,   214,     0,  -807,   215,   216,
     217,   218,   219,   220,   221,     0,   222,     0,   223,     0,
       0,   224,  -807,   225,   226,   227,     0,  -807,   228,     0,
       0,   229,   230,   231,     0,     0,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,  -807,   243,     0,
     244,   245,   246,   247,   248,     0,   249,   250,     0,     0,
     251,   252,   253,     0,     0,   254,     0,     0,     0,   255,
     256,     0,     0,   257,     0,     0,   258,   259,   260,   261,
     262,   263,     0,   264,   265,   266,     0,   267,   268,   269,
     270,   271,   272,   273,     0,   274,   275,   276,   277,   278,
     279,   280,   281,  -807,   282,   283,   284,   285,   286,   287,
     288,     0,   289,   290,   291,     0,   292,   293,   294,   295,
       0,   296,   297,     0,   298,   299,   300,   301,   302,   303,
     304,   305,   306,   307,     0,   308,   309,   310,     0,   311,
     312,     0,   313,     0,   314,   315,   316,   317,     0,   318,
     319,   320,   321,     0,     0,   322,   323,   324,   325,   326,
       0,     0,   327,   328,   329,   330,   331,   332,     0,   333,
     334,   335,     0,     0,   336,   337,   338,   339,   340,   341,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   135,   136,   137,   138,   139,   140,   141,
       0,   142,     0,     0,     0,     0,  -807,   342,   143,   144,
     145,     0,   146,   147,   148,     0,   845,     0,   846,   847,
       0,   152,   153,   154,   155,     0,     0,   156,   848,   849,
     159,   796,   160,   161,   162,   163,     0,     0,     0,   165,
     166,   167,     0,   168,   169,     0,   171,     0,     0,   172,
     173,     0,     0,     0,     0,     0,   174,   175,   176,   177,
     178,   850,   851,   181,     0,     0,   182,   183,   184,   185,
     186,     0,     0,     0,   187,   691,   189,   190,     0,   191,
     192,     0,   193,     0,   194,   195,     0,   197,   198,     0,
       0,   200,   852,     0,   202,     0,   203,     0,     0,     0,
     204,   205,   206,     0,     0,   207,     0,   208,     0,   209,
     210,   211,  1298,   212,   213,   214,     0,     0,  1299,   216,
     217,   218,   219,   853,   854,     0,   947,     0,   223,     0,
       0,   224,     0,   225,   226,   227,     0,     0,   228,     0,
       0,   229,   230,   231,     0,     0,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   701,     0,   855,     0,
     244,   245,   246,   247,     0,     0,   249,   250,     0,     0,
       0,   856,   253,     0,     0,   254,     0,     0,     0,   255,
     256,     0,     0,  1300,     0,     0,     0,   259,   260,   261,
     262,   263,     0,     0,   265,   266,     0,   267,   268,   269,
     270,   271,   857,   273,     0,   274,   275,   276,   277,   278,
     279,   280,   281,     0,   282,     0,   284,   285,   286,   287,
     288,     0,   289,   290,   291,     0,   292,   948,   294,   295,
       0,   296,   858,     0,   298,   299,   300,   301,   302,   303,
     304,   305,     0,   307,     0,   308,   309,   310,     0,   859,
     860,     0,   313,     0,   314,     0,   316,     0,     0,   318,
     319,   320,   321,     0,     0,   322,   323,   324,   325,   326,
       0,     0,   327,   328,   329,   330,   861,   332,     0,   333,
     334,   335,     0,     0,   336,   337,   338,   339,   340,   341,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   135,   136,   137,   138,   139,   140,   141,
       0,   142,     0,     0,     0,     0,     0,   862,   143,   144,
     145,   547,   146,   147,   148,   548,   676,   549,   677,   678,
    1417,   152,   153,   154,   155,   679,   680,   156,   681,   682,
     159,  1301,   160,   161,   162,   163,   683,     0,     0,   165,
     166,   167,     0,   168,   169,   684,   171,     0,     0,   172,
     173,   550,   685,   686,   687,   688,   174,   175,   176,   177,
     178,   689,   690,   181,     0,     0,   182,   183,   184,   185,
     186,     0,     0,     0,   187,   691,   189,   190,     0,   191,
     192,     0,   193,     0,   194,   195,   692,   197,   198,   693,
     694,   200,   695,     0,   202,     0,   203,   551,  1418,   552,
     204,   205,   206,     0,     0,   207,     0,   208,   553,   209,
     210,   211,   554,   212,   213,   214,     0,   555,   215,   216,
     217,   218,   219,   696,   697,     0,   698,     0,   223,   556,
     557,   224,   558,   225,   226,   227,  1419,   559,   228,   560,
       0,   229,   230,   231,   699,   700,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   701,   561,   702,   450,
     244,   245,   246,   247,   248,   703,   249,   250,   562,   704,
     705,   706,   253,     0,     0,   254,   451,     0,     0,   707,
     256,     0,     0,   257,   563,   564,   708,   259,   260,   261,
     262,   263,     0,   709,   265,   266,     0,   267,   268,   269,
     270,   271,   710,   273,     0,   274,   275,   276,   277,   278,
     279,   280,   281,   565,   282,   711,   284,   285,   286,   287,
     288,     0,   289,   290,   291,   712,   292,   293,   294,   295,
     566,   296,   713,     0,   298,   299,   300,   301,   302,   303,
     304,   305,   714,   307,     0,   308,   309,   310,     0,   715,
     716,     0,   313,  1420,   314,   717,   316,   718,   719,   318,
     319,   320,   321,     0,   720,   322,   323,   324,   325,   326,
     721,     0,   327,   328,   329,   330,   722,   332,   567,   333,
     334,   335,     0,     0,   336,   337,   338,   339,   340,   341,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   723,   724,   457,
       0,   725,   726,   727,   426,   728,     0,     0,     0,     0,
       0,   729,   730,     0,     0,   732,   733,     0,     0,     0,
     734,   135,   136,   137,   138,   139,   140,   141,     0,   142,
       0,     0,     0,     0,     0,     0,   143,   144,   145,   547,
     146,   147,   148,   548,   676,   549,   677,   678,     0,   152,
     153,   154,   155,   679,   680,   156,   681,   682,   159,     0,
     160,   161,   162,   163,   683,     0,     0,   165,   166,   167,
       0,   168,   169,   684,   171,     0,     0,   172,   173,   550,
     685,   686,   687,   688,   174,   175,   176,   177,   178,   689,
     690,   181,  1648,     0,   182,   183,   184,   185,   186,     0,
       0,     0,   187,   691,   189,   190,     0,   191,   192,     0,
     193,     0,   194,   195,   692,   197,   198,   693,   694,   200,
     695,     0,   202,     0,   203,   551,     0,   552,   204,   205,
     206,     0,     0,   207,     0,   208,   553,   209,   210,   211,
     554,   212,   213,   214,     0,   555,   215,   216,   217,   218,
     219,   696,   697,     0,   698,     0,   223,   556,   557,   224,
     558,   225,   226,   227,     0,   559,   228,   560,     0,   229,
     230,   231,   699,   700,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   701,   561,   702,   450,   244,   245,
     246,   247,   248,   703,   249,   250,   562,   704,   705,   706,
     253,     0,     0,   254,   451,     0,     0,   707,   256,     0,
       0,   257,   563,   564,   708,   259,   260,   261,   262,   263,
       0,   709,   265,   266,     0,   267,   268,   269,   270,   271,
     710,   273,     0,   274,   275,   276,   277,   278,   279,   280,
     281,   565,   282,   711,   284,   285,   286,   287,   288,     0,
     289,   290,   291,   712,   292,   293,   294,   295,   566,   296,
     713,     0,   298,   299,   300,   301,   302,   303,   304,   305,
     714,   307,     0,   308,   309,   310,     0,   715,   716,     0,
     313,     0,   314,   717,   316,   718,   719,   318,   319,   320,
     321,     0,   720,   322,   323,   324,   325,   326,   721,     0,
     327,   328,   329,   330,   722,   332,   567,   333,   334,   335,
       0,     0,   336,   337,   338,   339,   340,   341,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   723,   724,   457,     0,   725,
     726,   727,   426,   728,     0,     0,     0,     0,     0,   729,
     730,   731,     0,   732,   733,     0,     0,     0,   734,   135,
     136,   137,   138,   139,   140,   141,     0,   142,     0,     0,
       0,     0,     0,     0,   143,   144,   145,   547,   146,   147,
     148,   548,   676,   549,   677,   678,     0,   152,   153,   154,
     155,   679,   680,   156,   681,   682,   159,     0,   160,   161,
     162,   163,   683,     0,     0,   165,   166,   167,     0,   168,
     169,   684,   171,     0,     0,   172,   173,   550,   685,   686,
     687,   688,   174,   175,   176,   177,   178,   689,   690,   181,
       0,     0,   182,   183,   184,   185,   186,     0,     0,     0,
     187,   691,   189,   190,     0,   191,   192,     0,   193,     0,
     194,   195,   692,   197,   198,   693,   694,   200,   695,     0,
     202,     0,   203,   551,     0,   552,   204,   205,   206,     0,
       0,   207,     0,   208,   553,   209,   210,   211,   554,   212,
     213,   214,     0,   555,   215,   216,   217,   218,   219,   696,
     697,     0,   698,     0,   223,   556,   557,   224,   558,   225,
     226,   227,     0,   559,   228,   560,     0,   229,   230,   231,
     699,   700,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   701,   561,   702,   450,   244,   245,   246,   247,
     248,   703,   249,   250,   562,   704,   705,   706,   253,     0,
       0,   254,   451,     0,     0,   707,   256,     0,     0,   257,
     563,   564,   708,   259,   260,   261,   262,   263,     0,   709,
     265,   266,     0,   267,   268,   269,   270,   271,   710,   273,
       0,   274,   275,   276,   277,   278,   279,   280,   281,   565,
     282,   711,   284,   285,   286,   287,   288,     0,   289,   290,
     291,   712,   292,   293,   294,   295,   566,   296,   713,     0,
     298,   299,   300,   301,   302,   303,   304,   305,   714,   307,
       0,   308,   309,   310,     0,   715,   716,     0,   313,     0,
     314,   717,   316,   718,   719,   318,   319,   320,   321,     0,
     720,   322,   323,   324,   325,   326,   721,     0,   327,   328,
     329,   330,   722,   332,   567,   333,   334,   335,     0,     0,
     336,   337,   338,   339,   340,   341,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   723,   724,   457,     0,   725,   726,   727,
     426,   728,     0,     0,     0,     0,     0,   729,   730,   731,
       0,   732,   733,     0,     0,     0,   734,   135,   136,   137,
     138,   139,   140,   141,     0,   142,     0,     0,     0,     0,
       0,     0,   143,   144,   145,   547,   146,   147,   148,   548,
     676,   549,   677,   678,     0,   152,   153,   154,   155,   679,
     680,   156,   681,   682,   159,     0,   160,   161,   162,   163,
     683,     0,     0,   165,   166,   167,     0,   168,   169,   684,
     171,     0,     0,   172,   173,   550,   685,   686,   687,   688,
     174,   175,   176,   177,   178,   689,   690,   181,     0,     0,
     182,   183,   184,   185,   186,     0,     0,     0,   187,   691,
     189,   190,     0,   191,   192,     0,   193,     0,   194,   195,
     692,   197,   198,   693,   694,   200,   695,     0,   202,     0,
     203,   551,     0,   552,   204,   205,   206,     0,     0,   207,
       0,   208,   553,   209,   210,   211,   554,   212,   213,   214,
       0,   555,   215,   216,   217,   218,   219,   696,   697,     0,
     698,     0,   223,   556,   557,   224,   558,   225,   226,   227,
       0,   559,   228,   560,     0,   229,   230,   231,   699,   700,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     701,   561,   702,   450,   244,   245,   246,   247,   248,   703,
     249,   250,   562,   704,   705,   706,   253,     0,     0,   254,
     451,     0,     0,   707,   256,     0,     0,   257,   563,   564,
     708,   259,   260,   261,   262,   263,     0,   709,   265,   266,
       0,   267,   268,   269,   270,   271,   710,   273,     0,   274,
     275,   276,   277,   278,   279,   280,   281,   565,   282,   711,
     284,   285,   286,   287,   288,    33,   289,   290,   291,   712,
     292,   293,   294,   295,   566,   296,   713,     0,   298,   299,
     300,   301,   302,   303,   304,   305,   714,   307,     0,   308,
     309,   310,     0,   715,   716,     0,   313,     0,   314,   717,
     316,   718,   719,   318,   319,   320,   321,     0,   720,   322,
     323,   324,   325,   326,   721,     0,   327,   328,   329,   330,
     722,   332,   567,   333,   334,   335,     0,     0,   336,   337,
     338,   339,   340,   341,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   723,   724,   457,     0,   725,   726,   727,   426,   728,
       0,     0,     0,     0,     0,   729,   730,     0,     0,   732,
     733,     0,     0,     0,   734,   135,   136,   137,   138,   139,
     140,   141,     0,   142,     0,     0,     0,     0,     0,     0,
     143,   144,   145,   547,   146,   147,   148,   548,   676,   549,
     677,   678,     0,   152,   153,   154,   155,   679,   680,   156,
     681,   682,   159,     0,   160,   161,   162,   163,   683,     0,
       0,   165,   166,   167,     0,   168,   169,   684,   171,     0,
       0,   172,   173,   550,   685,   686,   687,   688,   174,   175,
     176,   177,   178,   689,   690,   181,     0,     0,   182,   183,
     184,   185,   186,     0,     0,     0,   187,   691,   189,   190,
       0,   191,   192,     0,   193,     0,   194,   195,   692,   197,
     198,   693,   694,   200,   695,     0,   202,     0,   203,   551,
    1418,   552,   204,   205,   206,     0,     0,   207,     0,   208,
     553,   209,   210,   211,   554,   212,   213,   214,     0,   555,
     215,   216,   217,   218,   219,   696,   697,     0,   698,     0,
     223,   556,   557,   224,   558,   225,   226,   227,     0,   559,
     228,   560,     0,   229,   230,   231,   699,   700,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   701,   561,
     702,   450,   244,   245,   246,   247,   248,   703,   249,   250,
     562,   704,   705,   706,   253,     0,     0,   254,   451,     0,
       0,   707,   256,     0,     0,   257,   563,   564,   708,   259,
     260,   261,   262,   263,     0,   709,   265,   266,     0,   267,
     268,   269,   270,   271,   710,   273,     0,   274,   275,   276,
     277,   278,   279,   280,   281,   565,   282,   711,   284,   285,
     286,   287,   288,     0,   289,   290,   291,   712,   292,   293,
     294,   295,   566,   296,   713,     0,   298,   299,   300,   301,
     302,   303,   304,   305,   714,   307,     0,   308,   309,   310,
       0,   715,   716,     0,   313,     0,   314,   717,   316,   718,
     719,   318,   319,   320,   321,     0,   720,   322,   323,   324,
     325,   326,   721,     0,   327,   328,   329,   330,   722,   332,
     567,   333,   334,   335,     0,     0,   336,   337,   338,   339,
     340,   341,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   723,
     724,   457,     0,   725,   726,   727,   426,   728,     0,     0,
       0,     0,     0,   729,   730,     0,     0,   732,   733,     0,
       0,     0,   734,   135,   136,   137,   138,   139,   140,   141,
       0,   142,     0,     0,     0,     0,     0,     0,   143,   144,
     145,   547,   146,   147,   148,   548,   676,   549,   677,   678,
       0,   152,   153,   154,   155,   679,   680,   156,   681,   682,
     159,     0,   160,   161,   162,   163,   683,     0,     0,   165,
     166,   167,     0,   168,   169,   684,   171,     0,     0,   172,
     173,   550,   685,   686,   687,   688,   174,   175,   176,   177,
     178,   689,   690,   181,     0,     0,   182,   183,   184,   185,
     186,     0,     0,     0,   187,   691,   189,   190,     0,   191,
     192,     0,   193,     0,   194,   195,   692,   197,   198,   693,
     694,   200,   695,     0,   202,     0,   203,   551,     0,   552,
     204,   205,   206,     0,     0,   207,     0,   208,   553,   209,
     210,   211,   554,   212,   213,   214,     0,   555,   215,   216,
     217,   218,   219,   696,   697,     0,   698,     0,   223,   556,
     557,   224,   558,   225,   226,   227,     0,   559,   228,   560,
       0,   229,   230,   231,   699,   700,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   701,   561,   702,   450,
     244,   245,   246,   247,   248,   703,   249,   250,   562,   704,
     705,   706,   253,     0,     0,   254,   451,     0,     0,   707,
     256,     0,     0,   257,   563,   564,   708,   259,   260,   261,
     262,   263,     0,   709,   265,   266,     0,   267,   268,   269,
     270,   271,   710,   273,     0,   274,   275,   276,   277,   278,
     279,   280,   281,   565,   282,   711,   284,   285,   286,   287,
     288,     0,   289,   290,   291,   712,   292,   293,   294,   295,
     566,   296,   713,     0,   298,   299,   300,   301,   302,   303,
     304,   305,   714,   307,     0,   308,   309,   310,     0,   715,
     716,     0,   313,     0,   314,   717,   316,   718,   719,   318,
     319,   320,   321,     0,   720,   322,   323,   324,   325,   326,
     721,     0,   327,   328,   329,   330,   722,   332,   567,   333,
     334,   335,     0,     0,   336,   337,   338,   339,   340,   341,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   723,   724,   457,
       0,   725,   726,   727,   426,   728,     0,     0,     0,     0,
       0,   729,   730,     0,     0,   732,   733,     0,     0,     0,
     734,   135,   136,   137,   138,   139,   140,   141,     0,   142,
       0,     0,     0,     0,     0,     0,   143,   144,   145,   547,
     146,   147,   148,   548,   676,   549,   677,   678,     0,   152,
     153,   154,   155,   679,   680,   156,   681,   682,   159,     0,
     160,   161,   162,   163,   683,     0,     0,   165,   166,   167,
       0,   168,   169,   684,   171,     0,     0,   172,   173,   550,
     685,   686,   687,   688,   174,   175,   176,   177,   178,   689,
     690,   181,     0,     0,   182,   183,   184,   185,   186,     0,
       0,     0,   187,   691,   189,   190,     0,   191,   192,     0,
     193,     0,   194,   195,   692,   197,   198,   693,   694,   200,
     695,     0,   202,     0,   203,   551,     0,   552,   204,   205,
     206,     0,     0,   207,     0,   208,   553,   209,   210,   211,
     554,   212,   213,   214,     0,   555,   215,   216,   217,   218,
     219,   696,   697,     0,   698,     0,   223,   556,   557,   224,
     558,   225,   226,   227,     0,   559,   228,   560,     0,   229,
     230,   231,   699,   700,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   701,   561,   702,   450,   244,   245,
     246,   247,   248,     0,   249,   250,   562,   704,   705,   706,
     253,     0,     0,   254,   451,     0,     0,   707,   256,     0,
       0,   257,   563,   564,   708,   259,   260,   261,   262,   263,
       0,   709,   265,   266,     0,   267,   268,   269,   270,   271,
     710,   273,     0,   274,   275,   276,   277,   278,   279,   280,
     281,   565,   282,   283,   284,   285,   286,   287,   288,     0,
     289,   290,   291,   712,   292,   293,   294,   295,   566,   296,
     713,     0,   298,   299,   300,   301,   302,   303,   304,   305,
     714,   307,     0,   308,   309,   310,     0,   715,   716,     0,
     313,     0,   314,   717,   316,   718,   719,   318,   319,   320,
     321,     0,     0,   322,   323,   324,   325,   326,   721,     0,
     327,   328,   329,   330,   722,   332,   567,   333,   334,   335,
       0,     0,   336,   337,   338,   339,   340,   341,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   723,   724,   457,     0,   725,
     726,   727,   426,   728,     0,     0,     0,     0,     0,  1397,
    1398,     0,     0,  1399,  1400,     0,     0,     0,  1401,   135,
     136,   137,   138,   139,   140,   141,     0,   142,     0,     0,
       0,     0,     0,     0,   143,   144,   145,   547,   146,   147,
     148,     0,   676,   549,   677,   678,     0,   152,   153,   154,
     155,   679,   680,   156,   681,   682,   159,     0,   160,   161,
     162,   163,   683,     0,     0,   165,   166,   167,     0,   168,
     169,   684,   171,     0,     0,   172,   173,   550,   685,   686,
     687,   688,   174,   175,   176,   177,   178,   689,   690,   181,
       0,     0,   182,   183,   184,   185,   186,     0,     0,     0,
     187,   691,   189,   190,     0,   191,   192,     0,     0,     0,
     194,   195,   692,   197,   198,   693,   694,   200,   695,     0,
     202,     0,   203,   551,     0,   552,   204,   205,   206,     0,
       0,   207,     0,   208,     0,   209,   210,   211,     0,   212,
     213,   214,     0,   555,   215,   216,   217,   218,   219,   696,
     697,     0,   698,     0,   223,   556,   557,   224,   558,   225,
     226,   227,     0,   559,   228,     0,     0,   229,   230,   231,
     699,   700,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   701,   561,   702,   450,   244,   245,   246,   247,
     248,     0,   249,   250,   562,   704,   705,   706,   253,     0,
       0,   254,   451,     0,     0,   707,   256,     0,     0,   257,
     563,   564,   708,   259,   260,   261,   262,   263,     0,   709,
     265,   266,     0,   267,   268,   269,   270,   271,   710,   273,
       0,   274,   275,   276,   277,   278,   279,   280,   281,   565,
     282,   711,   284,   285,   286,   287,   288,     0,   289,   290,
     291,   712,   292,   293,   294,   295,     0,   296,   713,     0,
     298,   299,   300,   301,   302,   303,   304,   305,   714,   307,
       0,   308,   309,   310,     0,   715,   716,     0,   313,     0,
     314,   717,   316,   718,   719,   318,   319,   320,   321,     0,
     720,   322,   323,   324,   325,   326,   721,     0,   327,   328,
     329,   330,   722,   332,   567,   333,   334,   335,     0,     0,
     336,   337,   338,   339,   340,   341,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   723,   724,   457,     0,   725,   726,   727,
     426,   728,     0,     0,     0,     0,     0,   729,   730,     0,
       0,   732,   733,     0,     0,     0,   734,   135,   136,   137,
     138,   139,   140,   141,     0,   142,     0,     0,     0,     0,
       0,     0,   143,   144,   145,   547,   146,   147,   148,   548,
     676,   549,   677,   678,     0,   152,   153,   154,   155,   679,
     680,   156,   681,   682,   159,     0,   160,   161,   162,   163,
     683,     0,     0,   165,   166,   167,     0,   168,   169,   684,
     171,     0,     0,   172,   173,   550,   685,   686,   687,   688,
     174,   175,   176,   177,   178,   689,   690,   181,     0,     0,
     182,   183,   184,   185,   186,     0,     0,     0,   187,   691,
     189,   190,     0,   191,   192,     0,   193,     0,   194,   195,
     692,   197,   198,   693,   694,   200,   695,     0,   202,     0,
     203,   551,     0,   552,   204,   205,   206,     0,     0,   207,
       0,   208,   553,   209,   210,   211,   554,   212,   213,   214,
       0,   555,   215,   216,   217,   218,   219,   696,   697,     0,
     698,     0,   223,   556,   557,   224,   558,   225,   226,   227,
       0,   559,   228,   560,     0,   229,   230,   231,   699,   700,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     701,   561,   702,   450,   244,   245,   246,   247,   248,     0,
     249,   250,   562,   704,   705,   706,   253,     0,     0,   254,
     451,     0,     0,   255,   256,     0,     0,   257,   563,   564,
     708,   259,   260,   261,   262,   263,     0,   709,   265,   266,
       0,   267,   268,   269,   270,   271,   710,   273,     0,   274,
     275,   276,   277,   278,   279,   280,   281,   565,   282,   283,
     284,   285,   286,   287,   288,     0,   289,   290,   291,   712,
     292,   293,   294,   295,   566,   296,   713,     0,   298,   299,
     300,   301,   302,   303,   304,   305,   714,   307,     0,   308,
     309,   310,     0,   715,   716,     0,   313,     0,   314,   717,
     316,   718,   719,   318,   319,   320,   321,     0,     0,   322,
     323,   324,   325,   326,   721,     0,   327,   328,   329,   330,
     722,   332,   567,   333,   334,   335,     0,     0,   336,   337,
     338,   339,   340,   341,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   723,   724,   457,     0,   725,   726,     0,   426,   728,
     135,   136,   137,   138,   139,   140,   141,     0,   142,     0,
       0,     0,     0,     0,  1401,   143,   144,   145,   547,   146,
     147,   148,     0,   676,   549,   677,   678,     0,   152,   153,
     154,   155,   679,   680,   156,   681,   682,   159,     0,   160,
     161,   162,   163,   683,     0,     0,   165,   166,   167,     0,
     168,   169,   684,   171,     0,     0,   172,   173,   550,   685,
     686,   687,   688,   174,   175,   176,   177,   178,   689,   690,
     181,     0,     0,   182,   183,   184,   185,   186,     0,     0,
       0,   187,   691,   189,   190,     0,   191,   192,     0,     0,
       0,   194,   195,   692,   197,   198,   693,   694,   200,   695,
       0,   202,     0,   203,   551,     0,   552,   204,   205,   206,
       0,     0,   207,     0,   208,     0,   209,   210,   211,     0,
     212,   213,   214,     0,   555,   215,   216,   217,   218,   219,
     696,   697,     0,   698,     0,   223,   556,   557,   224,   558,
     225,   226,   227,     0,   559,   228,     0,     0,   229,   230,
     231,   699,   700,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   701,   561,   702,   450,   244,   245,   246,
     247,   248,     0,   249,   250,   562,   704,   705,   706,   253,
       0,     0,   254,   451,     0,     0,   707,   256,     0,     0,
     257,   563,   564,   708,   259,   260,   261,   262,   263,     0,
     709,   265,   266,     0,   267,   268,   269,   270,   271,   710,
     273,     0,   274,   275,   276,   277,   278,   279,   280,   281,
     565,   282,   283,   284,   285,   286,   287,   288,     0,   289,
     290,   291,   712,   292,   293,   294,   295,     0,   296,   713,
       0,   298,   299,   300,   301,   302,   303,   304,   305,   714,
     307,     0,   308,   309,   310,     0,   715,   716,     0,   313,
       0,   314,   717,   316,   718,   719,   318,   319,   320,   321,
       0,     0,   322,   323,   324,   325,   326,   721,     0,   327,
     328,   329,   330,   722,   332,   567,   333,   334,   335,     0,
       0,   336,   337,   338,   339,   340,   341,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   723,   724,   457,     0,   725,   726,
     727,   426,   728,     0,     0,     0,     0,     0,  1397,  1398,
       0,     0,  1399,  1400,     0,     0,     0,  1401,   135,   136,
     137,   138,   139,   140,   141,  -957,   142,     0,     0,     0,
    -957,     0,     0,   143,   144,   145,   547,   146,   147,   148,
       0,   676,   549,   677,   678,     0,   152,   153,   154,   155,
     679,   680,   156,   681,   682,   159,     0,   160,   161,   162,
     163,   683,     0,     0,   165,   166,   167,     0,   168,   169,
     684,   171,     0,     0,   172,   173,   550,   685,   686,   687,
     688,   174,   175,   176,   177,   178,   689,   690,   181,     0,
       0,   182,   183,   184,   185,   186,     0,     0,     0,   187,
     691,   189,   190,     0,   191,   192,     0,     0,     0,   194,
     195,   692,   197,   198,   693,   694,   200,   695,     0,   202,
       0,   203,   551,     0,   552,   204,   205,   206,     0,     0,
     207,     0,   208,     0,   209,   210,   211,     0,   212,   213,
     214,     0,   555,   215,   216,   217,   218,   219,   696,   697,
       0,   698,     0,   223,     0,     0,   224,   558,   225,   226,
     227,     0,   559,   228,     0,     0,   229,   230,   231,   699,
     700,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   701,   561,   702,   450,   244,   245,   246,   247,   248,
       0,   249,   250,     0,   704,   705,   706,   253,     0,     0,
     254,   451,     0,     0,     0,   256,     0,     0,   257,   563,
     564,   708,   259,   260,   261,   262,   263,     0,   709,   265,
     266,     0,   267,   268,   269,   270,   271,   710,   273,     0,
     274,   275,   276,   277,   278,   279,   280,   281,   565,   282,
     711,   284,   285,   286,   287,   288,     0,   289,   290,   291,
     712,   292,   293,   294,   295,     0,   296,   713,  -957,   298,
     299,   300,   301,   302,   303,   304,   305,   714,   307,     0,
     308,   309,   310,     0,   715,   716,     0,   313,     0,   314,
     717,   316,   718,   719,   318,   319,   320,   321,     0,   720,
     322,   323,   324,   325,   326,   721,     0,   327,   328,   329,
     330,   722,   332,   567,   333,   334,   335,     0,     0,   336,
     337,   338,   339,   340,   341,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   723,   724,   457,     0,   725,   726,     0,   426,
     728,   135,   136,   137,   138,   139,   140,   141,  -958,   142,
       0,   733,     0,  -958,     0,   734,   143,   144,   145,   547,
     146,   147,   148,     0,   676,   549,   677,   678,     0,   152,
     153,   154,   155,   679,   680,   156,   681,   682,   159,     0,
     160,   161,   162,   163,   683,     0,     0,   165,   166,   167,
       0,   168,   169,   684,   171,     0,     0,   172,   173,   550,
     685,   686,   687,   688,   174,   175,   176,   177,   178,   689,
     690,   181,     0,     0,   182,   183,   184,   185,   186,     0,
       0,     0,   187,   691,   189,   190,     0,   191,   192,     0,
       0,     0,   194,   195,   692,   197,   198,   693,   694,   200,
     695,     0,   202,     0,   203,   551,     0,   552,   204,   205,
     206,     0,     0,   207,     0,   208,     0,   209,   210,   211,
       0,   212,   213,   214,     0,   555,   215,   216,   217,   218,
     219,   696,   697,     0,   698,     0,   223,     0,     0,   224,
     558,   225,   226,   227,     0,   559,   228,     0,     0,   229,
     230,   231,   699,   700,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   701,   561,   702,   450,   244,   245,
     246,   247,   248,     0,   249,   250,     0,   704,   705,   706,
     253,     0,     0,   254,   451,     0,     0,     0,   256,     0,
       0,   257,   563,   564,   708,   259,   260,   261,   262,   263,
       0,   709,   265,   266,     0,   267,   268,   269,   270,   271,
     710,   273,     0,   274,   275,   276,   277,   278,   279,   280,
     281,   565,   282,   711,   284,   285,   286,   287,   288,     0,
     289,   290,   291,   712,   292,   293,   294,   295,     0,   296,
     713,  -958,   298,   299,   300,   301,   302,   303,   304,   305,
     714,   307,     0,   308,   309,   310,     0,   715,   716,     0,
     313,     0,   314,   717,   316,   718,   719,   318,   319,   320,
     321,     0,   720,   322,   323,   324,   325,   326,   721,     0,
     327,   328,   329,   330,   722,   332,   567,   333,   334,   335,
       0,     0,   336,   337,   338,   339,   340,   341,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   723,   724,   457,     0,   725,
     726,     0,   426,   728,   135,   136,   137,   138,   139,   140,
     141,     0,   142,     0,     0,     0,     0,     0,   734,   143,
     144,   145,   547,   146,   147,   148,     0,   676,   549,   677,
     678,     0,   152,   153,   154,   155,   679,   680,   156,   681,
     682,   159,     0,   160,   161,   162,   163,   683,     0,     0,
     165,   166,   167,     0,   168,   169,   684,   171,     0,     0,
     172,   173,   550,   685,   686,   687,   688,   174,   175,   176,
     177,   178,   689,   690,   181,     0,     0,   182,   183,   184,
     185,   186,     0,     0,     0,   187,   691,   189,   190,     0,
     191,   192,     0,     0,     0,   194,   195,   692,   197,   198,
     693,   694,   200,   695,     0,   202,     0,   203,   551,     0,
     552,   204,   205,   206,     0,     0,   207,     0,   208,     0,
     209,   210,   211,     0,   212,   213,   214,     0,   555,   215,
     216,   217,   218,   219,   696,   697,     0,   698,     0,   223,
       0,     0,   224,   558,   225,   226,   227,     0,   559,   228,
       0,     0,   229,   230,   231,   699,   700,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   701,   561,   702,
     450,   244,   245,   246,   247,   248,     0,   249,   250,     0,
       0,   705,   706,   253,     0,     0,   254,   451,     0,     0,
       0,   256,     0,     0,   257,   563,   564,   708,   259,   260,
     261,   262,   263,     0,   709,   265,   266,     0,   267,   268,
     269,   270,   271,   710,   273,     0,   274,   275,   276,   277,
     278,   279,   280,   281,   565,   282,   283,   284,   285,   286,
     287,   288,     0,   289,   290,   291,   712,   292,   293,   294,
     295,     0,   296,   713,     0,   298,   299,   300,   301,   302,
     303,   304,   305,   714,   307,     0,   308,   309,   310,     0,
     715,   716,     0,   313,     0,   314,   717,   316,   718,   719,
     318,   319,   320,   321,     0,     0,   322,   323,   324,   325,
     326,   721,     0,   327,   328,   329,   330,   722,   332,   567,
     333,   334,   335,     0,     0,   336,   337,   338,   339,   340,
     341,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   723,   724,
     457,     0,   725,   726,     0,   426,   728,   135,   136,   137,
     138,   139,   140,   141,     0,   142,     0,  1400,     0,     0,
       0,  1401,   143,   144,   145,   547,   146,   147,   148,     0,
     676,   549,   677,   678,     0,   152,   153,   154,   155,   679,
     680,   156,   681,   682,   159,     0,   160,   161,   162,   163,
     683,     0,     0,   165,   166,   167,     0,   168,   169,   684,
     171,     0,     0,   172,   173,   550,   685,   686,   687,   688,
     174,   175,   176,   177,   178,   689,   690,   181,     0,     0,
     182,   183,   184,   185,   186,     0,     0,     0,   187,   691,
     189,   190,     0,   191,   192,     0,     0,     0,   194,   195,
     692,   197,   198,   693,   694,   200,   695,     0,   202,     0,
     203,   551,     0,   552,   204,   205,   206,     0,     0,   207,
       0,   208,     0,   209,   210,   211,     0,   212,   213,   214,
       0,   555,   215,   216,   217,   218,   219,   696,   697,     0,
     698,     0,   223,     0,     0,   224,   558,   225,   226,   227,
       0,   559,   228,     0,     0,   229,   230,   231,   699,   700,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     701,   561,   702,   450,   244,   245,   246,   247,   248,     0,
     249,   250,     0,     0,   705,   706,   253,     0,     0,   254,
     451,     0,     0,     0,   256,     0,     0,   257,   563,   564,
     708,   259,   260,   261,   262,   263,     0,   709,   265,   266,
       0,   267,   268,   269,   270,   271,   710,   273,     0,   274,
     275,   276,   277,   278,   279,   280,   281,   565,   282,   283,
     284,   285,   286,   287,   288,     0,   289,   290,   291,   712,
     292,   293,   294,   295,     0,   296,   713,     0,   298,   299,
     300,   301,   302,   303,   304,   305,   714,   307,     0,   308,
     309,   310,     0,   715,   716,     0,   313,     0,   314,   717,
     316,   718,   719,   318,   319,   320,   321,     0,     0,   322,
     323,   324,   325,   326,   721,     0,   327,   328,   329,   330,
     722,   332,   567,   333,   334,   335,     0,     0,   336,   337,
     338,   339,   340,   341,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   723,   724,   457,     0,   725,   726,     0,   426,   728,
     135,   136,   137,   138,   139,   140,   141,     0,   142,     0,
       0,     0,     0,     0,  1401,   143,   144,   145,   547,   146,
     147,   148,   548,   149,   549,   150,   151,     0,   152,   153,
     154,   155,     0,     0,   156,   157,   158,   159,     0,   160,
     161,   162,   163,   164,     0,     0,   165,   166,   167,     0,
     168,   169,   170,   171,     0,     0,   172,   173,   550,     0,
       0,     0,     0,   174,   175,   176,   177,   178,   179,   180,
     181,     0,     0,   182,   183,   184,   185,   186,     0,     0,
       0,   187,   188,   189,   190,     0,   191,   192,     0,   193,
       0,   194,   195,   196,   197,   198,   199,     0,   200,   201,
       0,   202,     0,   203,   551,     0,   552,   204,   205,   206,
       0,     0,   207,     0,   208,   553,   209,   210,   211,   554,
     212,   213,   214,     0,   555,   215,   216,   217,   218,   219,
     220,   221,     0,   222,     0,   223,   556,   557,   224,   558,
     225,   226,   227,     0,   559,   228,   560,     0,   229,   230,
     231,     0,     0,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   561,   243,   450,   244,   245,   246,
     247,   248,     0,   249,   250,   562,     0,   251,   252,   253,
       0,     0,   254,   451,     0,   500,   255,   256,     0,     0,
     257,   563,   564,   258,   259,   260,   261,   262,   263,     0,
     264,   265,   266,     0,   267,   268,   269,   270,   271,   272,
     273,     0,   274,   275,   276,   277,   278,   279,   280,   281,
     565,   282,   283,   284,   285,   286,   287,   288,    33,   289,
     290,   291,     0,   292,   293,   294,   295,   566,   296,   297,
       0,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,     0,   308,   309,   310,     0,   311,   312,     0,   313,
       0,   314,   315,   316,   317,     0,   318,   319,   320,   321,
       0,     0,   322,   323,   324,   325,   326,     0,     0,   327,
     328,   329,   330,   331,   332,   567,   333,   334,   335,     0,
       0,   336,   337,   338,   339,   340,   341,     0,     0,     0,
       0,     0,   135,   136,   137,   138,   139,   140,   141,     0,
     142,     0,     0,     0,     0,     0,     0,   143,   144,   145,
     547,   146,   147,   148,   548,   149,   549,   150,   151,     0,
     152,   153,   154,   155,   568,     0,   156,   157,   158,   159,
       0,   160,   161,   162,   163,   164,     0,     0,   165,   166,
     167,     0,   168,   169,   170,   171,     0,  1767,   172,   173,
     550,     0,     0,     0,     0,   174,   175,   176,   177,   178,
     179,   180,   181,     0,     0,   182,   183,   184,   185,   186,
       0,     0,     0,   187,   188,   189,   190,     0,   191,   192,
       0,   193,     0,   194,   195,   196,   197,   198,   199,     0,
     200,   201,     0,   202,     0,   203,   551,     0,   552,   204,
     205,   206,     0,     0,   207,     0,   208,   553,   209,   210,
     211,   554,   212,   213,   214,     0,   555,   215,   216,   217,
     218,   219,   220,   221,     0,   222,     0,   223,   556,   557,
     224,   558,   225,   226,   227,     0,   559,   228,   560,     0,
     229,   230,   231,     0,     0,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   561,   243,   450,   244,
     245,   246,   247,   248,     0,   249,   250,   562,     0,   251,
     252,   253,     0,     0,   254,   451,     0,   500,   255,   256,
       0,     0,   257,   563,   564,   258,   259,   260,   261,   262,
     263,     0,   264,   265,   266,     0,   267,   268,   269,   270,
     271,   272,   273,     0,   274,   275,   276,   277,   278,   279,
     280,   281,   565,   282,   283,   284,   285,   286,   287,   288,
       0,   289,   290,   291,     0,   292,   293,   294,   295,   566,
     296,   297,     0,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,     0,   308,   309,   310,     0,   311,   312,
       0,   313,     0,   314,   315,   316,   317,     0,   318,   319,
     320,   321,     0,     0,   322,   323,   324,   325,   326,     0,
       0,   327,   328,   329,   330,   331,   332,   567,   333,   334,
     335,     0,     0,   336,   337,   338,   339,   340,   341,     0,
       0,     0,     0,     0,   135,   136,   137,   138,   139,   140,
     141,     0,   142,     0,     0,     0,     0,     0,     0,   143,
     144,   145,     0,   146,   147,   148,     0,   149,     0,   150,
     151,     0,   152,   153,   154,   155,   568,     0,   156,   157,
     158,   159,     0,   160,   161,   162,   163,   164,     0,     0,
     165,   166,   167,     0,   168,   169,   170,   171,     0,  1767,
     172,   173,     0,     0,     0,     0,     0,   174,   175,   176,
     177,   178,   179,   180,   181,     0,     0,   182,   183,   184,
     185,   186,     0,     0,     0,   187,   188,   189,   190,     0,
     191,   192,     0,   193,     0,   194,   195,   196,   197,   198,
     199,     0,   200,   201,     0,   202,     0,   203,     0,     0,
       0,   204,   205,   206,     0,     0,   207,     0,   208,     0,
     209,   210,   211,     0,   212,   213,   214,     0,     0,   215,
     216,   217,   218,   219,   220,   221,     0,   222,     0,   223,
       0,     0,   224,     0,   225,   226,   227,     0,     0,   228,
       0,     0,   229,   230,   231,     0,     0,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,     0,   243,
     450,   244,   245,   246,   247,   248,     0,   249,   250,     0,
       0,   251,   252,   253,     0,     0,   254,   451,     0,     0,
     255,   256,     0,     0,   257,     0,     0,   258,   259,   260,
     261,   262,   263,     0,   264,   265,   266,     0,   267,   268,
     269,   270,   271,   272,   273,     0,   274,   275,   276,   277,
     278,   279,   280,   281,     0,   282,   283,   284,   285,   286,
     287,   288,     0,   289,   290,   291,     0,   292,   293,   294,
     295,     0,   296,   297,     0,   298,   299,   300,   301,   302,
     303,   304,   305,   306,   307,     0,   308,   309,   310,     0,
     311,   312,     0,   313,     0,   314,   315,   316,   317,     0,
     318,   319,   320,   321,     0,     0,   322,   323,   324,   325,
     326,     0,     0,   327,   328,   329,   330,   331,   332,     0,
     333,   334,   335,     0,     0,   336,   337,   338,   339,   340,
     341,     0,     0,     0,     0,     0,   135,   136,   137,   138,
     139,   140,   141,     0,   142,     0,     0,     0,     0,     0,
       0,   143,   144,   145,     0,   146,   147,   148,     0,   149,
       0,   150,   151,     0,   152,   153,   154,   155,   342,     0,
     156,   157,   158,   159,     0,   160,   161,   162,   163,   164,
       0,     0,   165,   166,   167,     0,   168,   169,   170,   171,
       0,   787,   172,   173,     0,     0,     0,     0,     0,   174,
     175,   176,   177,   178,   179,   180,   181,     0,     0,   182,
     183,   184,   185,   186,     0,     0,     0,   187,   188,   189,
     190,     0,   191,   192,     0,   193,     0,   194,   195,   196,
     197,   198,   199,     0,   200,   201,     0,   202,     0,   203,
       0,     0,     0,   204,   205,   206,     0,     0,   207,     0,
     208,     0,   209,   210,   211,     0,   212,   213,   214,     0,
       0,   215,   216,   217,   218,   219,   220,   221,     0,   222,
       0,   223,     0,     0,   224,     0,   225,   226,   227,     0,
       0,   228,     0,     0,   229,   230,   231,     0,     0,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
       0,   243,   450,   244,   245,   246,   247,   248,     0,   249,
     250,     0,     0,   251,   252,   253,     0,     0,   254,   451,
       0,     0,   255,   256,     0,     0,   257,     0,     0,   258,
     259,   260,   261,   262,   263,     0,   264,   265,   266,     0,
     267,   268,   269,   270,   271,   272,   273,     0,   274,   275,
     276,   277,   278,   279,   280,   281,     0,   282,   283,   284,
     285,   286,   287,   288,     0,   289,   290,   291,     0,   292,
     293,   294,   295,     0,   296,   297,     0,   298,   299,   300,
     301,   302,   303,   304,   305,   306,   307,     0,   308,   309,
     310,     0,   311,   312,     0,   313,     0,   314,   315,   316,
     317,     0,   318,   319,   320,   321,     0,     0,   322,   323,
     324,   325,   326,     0,     0,   327,   328,   329,   330,   331,
     332,     0,   333,   334,   335,     0,     0,   336,   337,   338,
     339,   340,   341,     0,     0,     0,     0,     0,   135,   136,
     137,   138,   139,   140,   141,     0,   142,     0,     0,     0,
       0,     0,     0,   143,   144,   145,     0,   146,   147,   148,
       0,   149,     0,   150,   151,     0,   152,   153,   154,   155,
     342,     0,   156,   157,   158,   159,     0,   160,   161,   162,
     163,   164,     0,     0,   165,   166,   167,     0,   168,   169,
     170,   171,     0,    42,   172,   173,     0,     0,     0,     0,
       0,   174,   175,   176,   177,   178,   179,   180,   181,     0,
       0,   182,   183,   184,   185,   186,     0,     0,     0,   187,
     188,   189,   190,     0,   191,   192,     0,   193,     0,   194,
     195,   196,   197,   198,   199,     0,   200,   201,     0,   202,
       0,   203,     0,     0,     0,   204,   205,   206,     0,     0,
     207,     0,   208,     0,   209,   210,   211,     0,   212,   213,
     214,     0,     0,   215,   216,   217,   218,   219,   220,   221,
       0,   222,     0,   223,     0,     0,   224,     0,   225,   226,
     227,     0,     0,   228,     0,     0,   229,   230,   231,     0,
       0,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,     0,   243,     0,   244,   245,   246,   247,   248,
       0,   249,   250,     0,     0,   251,   252,   253,     0,     0,
     254,     0,     0,     0,   255,   256,     0,     0,   257,     0,
       0,   258,   259,   260,   261,   262,   263,     0,   264,   265,
     266,     0,   267,   268,   269,   270,   271,   272,   273,     0,
     274,   275,   276,   277,   278,   279,   280,   281,     0,   282,
     283,   284,   285,   286,   287,   288,    33,   289,   290,   291,
       0,   292,   293,   294,   295,     0,   296,   297,     0,   298,
     299,   300,   301,   302,   303,   304,   305,   306,   307,     0,
     308,   309,   310,     0,   311,   312,     0,   313,     0,   314,
     315,   316,   317,     0,   318,   319,   320,   321,     0,     0,
     322,   323,   324,   325,   326,     0,     0,   327,   328,   329,
     330,   331,   332,     0,   333,   334,   335,     0,     0,   336,
     337,   338,   339,   340,   341,     0,     0,     0,     0,     0,
     135,   136,   137,   138,   139,   140,   141,     0,   142,     0,
       0,     0,     0,     0,     0,   143,   144,   145,     0,   146,
     147,   148,     0,   149,     0,   150,   151,     0,   152,   153,
     154,   155,   342,     0,   156,   157,   158,   159,     0,   160,
     161,   162,   163,   164,     0,     0,   165,   166,   167,     0,
     168,   169,   170,   171,     0,    42,   172,   173,     0,     0,
       0,     0,     0,   174,   175,   176,   177,   178,   179,   180,
     181,     0,     0,   182,   183,   184,   185,   186,     0,     0,
       0,   187,   188,   189,   190,     0,   191,   192,     0,   193,
       0,   194,   195,   196,   197,   198,   199,     0,   200,   201,
       0,   202,     0,   203,     0,     0,     0,   204,   205,   206,
       0,     0,   207,     0,   208,     0,   209,   210,   211,     0,
     212,   213,   214,     0,     0,   215,   216,   217,   218,   219,
     220,   221,     0,   222,     0,   223,     0,     0,   224,     0,
     225,   226,   227,     0,     0,   228,     0,     0,   229,   230,
     231,     0,     0,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,     0,   243,     0,   244,   245,   246,
     247,   248,     0,   249,   250,     0,     0,   251,   252,   253,
       0,     0,   254,     0,     0,     0,   255,   256,     0,     0,
     257,     0,     0,   258,   259,   260,   261,   262,   263,     0,
     264,   265,   266,     0,   267,   268,   269,   270,   271,   272,
     273,     0,   274,   275,   276,   277,   278,   279,   280,   281,
       0,   282,   283,   284,   285,   286,   287,   288,     0,   289,
     290,   291,     0,   292,   293,   294,   295,     0,   296,   297,
       0,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,     0,   308,   309,   310,     0,   311,   312,     0,   313,
       0,   314,   315,   316,   317,     0,   318,   319,   320,   321,
       0,     0,   322,   323,   324,   325,   326,     0,     0,   327,
     328,   329,   330,   331,   332,     0,   333,   334,   335,     0,
       0,   336,   337,   338,   339,   340,   341,     0,     0,     0,
       0,     0,   135,   136,   137,   138,   139,   140,   141,     0,
     142,     0,     0,     0,     0,  2005,     0,   143,   144,   145,
       0,   146,   147,   148,     0,   149,     0,   150,   151,     0,
     152,   153,   154,   155,   342,     0,   156,   157,   158,   159,
       0,   160,   161,   162,   163,   164,     0,     0,   165,   166,
     167,     0,   168,   169,   170,   171,     0,  2185,   172,   173,
       0,     0,     0,     0,     0,   174,   175,   176,   177,   178,
     179,   180,   181,     0,     0,   182,   183,   184,   185,   186,
       0,     0,     0,   187,   188,   189,   190,     0,   191,   192,
       0,   193,     0,   194,   195,   196,   197,   198,   199,     0,
     200,   201,     0,   202,     0,   203,     0,     0,     0,   204,
     205,   206,     0,     0,   207,     0,   208,     0,   209,   210,
     211,     0,   212,   213,   214,     0,     0,   215,   216,   217,
     218,   219,   220,   221,     0,   222,     0,   223,     0,     0,
     224,     0,   225,   226,   227,     0,     0,   228,     0,     0,
     229,   230,   231,     0,     0,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,     0,   243,     0,   244,
     245,   246,   247,   248,     0,   249,   250,     0,     0,   251,
     252,   253,     0,     0,   254,     0,     0,     0,   255,   256,
       0,     0,   257,     0,     0,   258,   259,   260,   261,   262,
     263,     0,   264,   265,   266,     0,   267,   268,   269,   270,
     271,   272,   273,     0,   274,   275,   276,   277,   278,   279,
     280,   281,     0,   282,   283,   284,   285,   286,   287,   288,
       0,   289,   290,   291,     0,   292,   293,   294,   295,     0,
     296,   297,     0,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,     0,   308,   309,   310,     0,   311,   312,
       0,   313,     0,   314,   315,   316,   317,     0,   318,   319,
     320,   321,     0,     0,   322,   323,   324,   325,   326,     0,
       0,   327,   328,   329,   330,   331,   332,     0,   333,   334,
     335,     0,     0,   336,   337,   338,   339,   340,   341,     0,
       0,     0,     0,   135,   136,   137,   138,   139,   140,   141,
       0,   142,     0,     0,     0,     0,     0,     0,   143,   144,
     145,     0,   146,   147,   148,     0,   149,     0,   150,   151,
       0,   152,   153,   154,   155,     0,   342,   156,   157,   158,
     159,     0,   160,   588,   162,   163,   164,     0,     0,   165,
     166,   167,     0,   168,   169,   170,   171,  2018,     0,   172,
     173,     0,     0,     0,     0,     0,   174,   175,   176,   177,
     178,   179,   180,   181,     0,     0,   182,   183,   184,   185,
     186,     0,     0,     0,   187,   188,   189,   190,     0,   191,
     192,     0,   193,     0,   194,   195,   196,   197,   198,   199,
       0,   200,   201,     0,   202,     0,   203,     0,     0,     0,
     204,   205,   206,     0,     0,   207,     0,   208,     0,   209,
     210,   211,     0,   212,   213,   214,     0,     0,   215,   216,
     217,   218,   219,   220,   221,     0,   222,     0,   223,     0,
       0,   224,     0,   225,   226,   227,     0,     0,   228,     0,
       0,   229,   230,   231,     0,     0,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,     0,   243,     0,
     244,   245,   246,   247,   248,     0,   249,   250,     0,     0,
     251,   252,   253,     0,     0,   254,     0,     0,     0,   255,
     256,     0,     0,   257,     0,     0,   258,   259,   260,   261,
     262,   263,     0,   264,   265,   266,     0,   267,   268,   269,
     270,   271,   272,   273,     0,   274,   275,   276,   277,   278,
     279,   280,   281,     0,   282,   283,   284,   285,   286,   287,
     288,     0,   289,   290,   291,     0,   292,   293,   294,   295,
       0,   296,   297,     0,   298,   299,   300,   301,   302,   303,
     304,   305,   306,   307,     0,   308,   309,   310,     0,   311,
     312,     0,   313,     0,   314,   315,   316,   317,     0,   318,
     319,   320,   321,     0,     0,   322,   323,   324,   325,   326,
       0,     0,   327,   328,   329,   330,   331,   332,     0,   333,
     334,   335,     0,     0,   336,   337,   338,   339,   340,   341,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   342,     0,     0,
       0,     0,     0,   589,     0,     0,     0,   590,   591,   592,
       0,   593,   594,   595,   596,   597,   598,   135,   136,   137,
     138,   139,   140,   141,     0,   142,     0,     0,     0,     0,
       0,     0,   143,   144,   145,     0,   146,   147,   148,     0,
     149,     0,   150,   151,     0,   152,   153,   154,   155,     0,
       0,   156,   157,   158,   159,     0,   160,   632,   162,   163,
     164,     0,     0,   165,   166,   167,     0,   168,   169,   170,
     171,     0,     0,   172,   173,     0,     0,     0,     0,     0,
     174,   175,   176,   177,   178,   179,   180,   181,     0,     0,
     182,   183,   184,   185,   186,     0,     0,     0,   187,   188,
     189,   190,     0,   191,   192,     0,   193,     0,   194,   195,
     196,   197,   198,   199,     0,   200,   201,     0,   202,     0,
     203,     0,     0,     0,   204,   205,   206,     0,     0,   207,
       0,   208,     0,   209,   210,   211,     0,   212,   213,   214,
       0,     0,   215,   216,   217,   218,   219,   220,   221,     0,
     222,     0,   223,     0,     0,   224,     0,   225,   226,   227,
       0,     0,   228,     0,     0,   229,   230,   231,     0,     0,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,     0,   243,     0,   244,   245,   246,   247,   248,     0,
     249,   250,     0,     0,   251,   252,   253,     0,     0,   254,
       0,     0,     0,   255,   256,     0,     0,   257,     0,     0,
     258,   259,   260,   261,   262,   263,     0,   264,   265,   266,
       0,   267,   268,   269,   270,   271,   272,   273,     0,   274,
     275,   276,   277,   278,   279,   280,   281,     0,   282,   283,
     284,   285,   286,   287,   288,     0,   289,   290,   291,     0,
     292,   293,   294,   295,     0,   296,   297,     0,   298,   299,
     300,   301,   302,   303,   304,   305,   306,   307,     0,   308,
     309,   310,     0,   311,   312,     0,   313,     0,   314,   315,
     316,   317,     0,   318,   319,   320,   321,     0,     0,   322,
     323,   324,   325,   326,     0,     0,   327,   328,   329,   330,
     331,   332,     0,   333,   334,   335,     0,     0,   336,   337,
     338,   339,   340,   341,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   342,     0,     0,     0,     0,     0,   589,     0,     0,
       0,   590,   591,   592,     0,   593,   594,   595,   596,   597,
     598,   135,   136,   137,   138,   139,   140,   141,     0,   142,
       0,     0,     0,     0,     0,     0,   143,   144,   145,     0,
     146,   147,   148,     0,   149,     0,   150,   151,     0,   152,
     153,   154,   155,     0,     0,   156,   157,   158,   159,     0,
     160,   161,   162,   163,   164,     0,     0,   165,   166,   167,
       0,   168,   169,   170,   171,     0,     0,   172,   173,     0,
       0,     0,     0,     0,   174,   175,   176,   177,   178,   179,
     180,   181,     0,     0,   182,   183,   184,   185,   186,     0,
       0,     0,   187,   188,   189,   190,     0,   191,   192,     0,
     193,     0,   194,   195,   196,   197,   198,   199,     0,   200,
     201,     0,   202,     0,   203,     0,     0,     0,   204,   205,
     206,     0,     0,   207,     0,   208,     0,   209,   210,   211,
       0,   212,   213,   214,     0,     0,   215,   216,   217,   218,
     219,   220,   221,     0,   222,     0,   223,     0,     0,   224,
       0,   225,   226,   227,     0,     0,   228,     0,     0,   229,
     230,   231,     0,     0,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,     0,   243,     0,   244,   245,
     246,   247,   248,     0,   249,   250,     0,     0,   251,   252,
     253,     0,     0,   254,     0,     0,     0,   255,   256,     0,
       0,   257,     0,     0,   258,   259,   260,   261,   262,   263,
       0,   264,   265,   266,     0,   267,   268,   269,   270,   271,
     272,   273,     0,   274,   275,   276,   277,   278,   279,   280,
     281,     0,   282,   283,   284,   285,   286,   287,   288,     0,
     289,   290,   291,     0,   292,   293,   294,   295,     0,   296,
     297,     0,   298,   299,   300,   301,   302,   303,   304,   305,
     306,   307,     0,   308,   309,   310,     0,   311,   312,     0,
     313,     0,   314,   315,   316,   317,     0,   318,   319,   320,
     321,     0,     0,   322,   323,   324,   325,   326,     0,     0,
     327,   328,   329,   330,   331,   332,     0,   333,   334,   335,
       0,     0,   336,   337,   338,   339,   340,   341,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   342,     0,     0,     0,     0,
       0,   589,     0,     0,     0,   590,   591,   592,     0,   593,
     594,   595,   596,   597,   598,   135,   136,   137,   138,   139,
     140,   141,     0,   142,     0,     0,     0,     0,     0,     0,
     143,   144,   145,     0,   146,   147,   148,     0,   845,     0,
     846,   847,     0,   152,   153,   154,   155,     0,     0,   156,
     848,   849,   159,     0,   160,   161,   162,   163,     0,     0,
       0,   165,   166,   167,     0,   168,   169,     0,   171,     0,
       0,   172,   173,     0,     0,     0,     0,     0,   174,   175,
     176,   177,   178,   850,   851,   181,     0,     0,   182,   183,
     184,   185,   186,     0,     0,     0,   187,   691,   189,   190,
       0,   191,   192,     0,   193,     0,   194,   195,     0,   197,
     198,     0,     0,   200,   852,     0,   202,     0,   203,     0,
       0,     0,   204,   205,   206,     0,     0,   207,     0,   208,
       0,   209,   210,   211,     0,   212,   213,   214,     0,     0,
     215,   216,   217,   218,   219,   853,   854,     0,   947,     0,
     223,     0,     0,   224,     0,   225,   226,   227,     0,     0,
     228,     0,     0,   229,   230,   231,     0,     0,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   701,     0,
     855,     0,   244,   245,   246,   247,     0,     0,   249,   250,
       0,     0,     0,   856,   253,     0,     0,   254,     0,     0,
       0,  1831,   256,     0,     0,   257,     0,     0,     0,   259,
     260,   261,   262,   263,     0,     0,   265,   266,     0,   267,
     268,   269,   270,   271,   857,   273,     0,   274,   275,   276,
     277,   278,   279,   280,   281,     0,   282,     0,   284,   285,
     286,   287,   288,     0,   289,   290,   291,     0,   292,   948,
     294,   295,     0,   296,   858,     0,   298,   299,   300,   301,
     302,   303,   304,   305,     0,   307,     0,   308,   309,   310,
       0,   859,   860,     0,   313,     0,   314,     0,   316,     0,
       0,   318,   319,   320,   321,     0,     0,   322,   323,   324,
     325,   326,     0,     0,   327,   328,   329,   330,   861,   332,
       0,   333,   334,   335,     0,     0,   336,   337,   338,   339,
     340,   341,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   862,
    1081,   457,     0,     0,     0,   589,   426,     0,     0,   590,
     591,   592,     0,   593,  1832,   595,   596,   597,   598,   135,
     136,   137,   138,   139,   140,   141,     0,   142,     0,     0,
       0,     0,     0,     0,   143,   144,   145,     0,   146,   147,
     148,     0,   149,     0,   150,   151,     0,   152,   153,   154,
     155,     0,     0,   156,   157,   158,   159,     0,   160,   161,
     162,   163,   164,     0,     0,   165,   166,   167,     0,   168,
     169,   170,   171,     0,     0,   172,   173,     0,     0,     0,
       0,     0,   174,   175,   176,   177,   178,   179,   180,   181,
       0,     0,   182,   183,   184,   185,   186,     0,     0,     0,
     187,   188,   189,   190,     0,   191,   192,     0,   193,     0,
     194,   195,   196,   197,   198,   199,     0,   200,   201,     0,
     202,     0,   203,     0,     0,     0,   204,   205,   206,     0,
       0,   207,     0,   208,     0,   209,   210,   211,     0,   212,
     213,   214,     0,     0,   215,   216,   217,   218,   219,   220,
     221,     0,   222,     0,   223,     0,     0,   224,     0,   225,
     226,   227,     0,     0,   228,     0,     0,   229,   230,   231,
       0,     0,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,     0,   243,   450,   244,   245,   246,   247,
     248,     0,   249,   250,     0,     0,   251,   252,   253,     0,
       0,   254,   451,     0,     0,   255,   256,     0,     0,   257,
       0,     0,   258,   259,   260,   261,   262,   263,     0,   264,
     265,   266,     0,   267,   268,   269,   270,   271,   272,   273,
       0,   274,   275,   276,   277,   278,   279,   280,   281,     0,
     282,   283,   284,   285,   286,   287,   288,     0,   289,   290,
     291,     0,   292,   293,   294,   295,     0,   296,   297,     0,
     298,   299,   300,   301,   302,   303,   304,   305,   306,   307,
       0,   308,   309,   310,     0,   311,   312,     0,   313,     0,
     314,   315,   316,   317,     0,   318,   319,   320,   321,     0,
       0,   322,   323,   324,   325,   326,     0,     0,   327,   328,
     329,   330,   331,   332,     0,   333,   334,   335,     0,     0,
     336,   337,   338,   339,   340,   341,     0,     0,     0,     0,
     135,   136,   137,   138,   139,   140,   141,     0,   142,     0,
       0,     0,     0,     0,     0,   143,   144,   145,     0,   146,
     147,   148,     0,   149,     0,   150,   151,     0,   152,   153,
     154,   155,     0,   342,   156,   157,   158,   159,     0,   160,
     161,   162,   163,   164,     0,     0,   165,   166,   167,   498,
     168,   169,   170,   171,     0,     0,   172,   173,     0,     0,
       0,     0,     0,   174,   175,   176,   177,   178,   179,   180,
     181,     0,     0,   182,   183,   184,   185,   186,     0,     0,
       0,   187,   188,   189,   190,     0,   191,   192,     0,   193,
       0,   194,   195,   196,   197,   198,   199,     0,   200,   201,
       0,   202,     0,   203,     0,     0,     0,   204,   205,   206,
       0,     0,   207,     0,   208,     0,   209,   210,   211,     0,
     212,   213,   214,     0,     0,   215,   216,   217,   218,   219,
     220,   221,     0,   222,     0,   223,     0,     0,   224,     0,
     225,   226,   227,     0,     0,   228,     0,     0,   229,   230,
     231,     0,     0,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,     0,   243,     0,   244,   245,   246,
     247,   248,     0,   249,   250,     0,     0,   251,   252,   253,
       0,     0,   254,     0,     0,     0,   255,   256,     0,     0,
     257,     0,     0,   258,   259,   260,   261,   262,   263,     0,
     264,   265,   266,     0,   267,   268,   269,   270,   271,   272,
     273,     0,   274,   275,   276,   277,   278,   279,   280,   281,
       0,   282,   283,   284,   285,   286,   287,   288,     0,   289,
     290,   291,     0,   292,   293,   294,   295,     0,   296,   297,
       0,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,     0,   308,   309,   310,     0,   311,   312,     0,   313,
       0,   314,   315,   316,   317,     0,   318,   319,   320,   321,
       0,     0,   322,   323,   324,   325,   326,     0,     0,   327,
     328,   329,   330,   331,   332,     0,   333,   334,   335,     0,
       0,   336,   337,   338,   339,   340,   341,     0,     0,     0,
       0,   135,   136,   137,   138,   139,   140,   141,     0,   142,
       0,     0,     0,     0,     0,     0,   143,   144,   145,     0,
     146,   147,   148,     0,   845,     0,   846,   847,     0,   152,
     153,   154,   155,     0,   342,   156,   848,   849,   159,     0,
     160,   161,   162,   163,     0,     0,     0,   165,   166,   167,
     937,   168,   169,     0,   171,     0,     0,   172,   173,     0,
       0,     0,     0,     0,   174,   175,   176,   177,   178,   850,
     851,   181,     0,     0,   182,   183,   184,   185,   186,     0,
       0,     0,   187,   691,   189,   190,     0,   191,   192,     0,
     193,     0,   194,   195,     0,   197,   198,     0,     0,   200,
     852,     0,   202,     0,   203,     0,     0,     0,   204,   205,
     206,     0,     0,   207,     0,   208,     0,   209,   210,   211,
       0,   212,   213,   214,     0,     0,   215,   216,   217,   218,
     219,   853,   854,     0,   947,     0,   223,     0,     0,   224,
       0,   225,   226,   227,     0,     0,   228,     0,     0,   229,
     230,   231,     0,     0,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   701,     0,   855,     0,   244,   245,
     246,   247,     0,     0,   249,   250,     0,     0,     0,   856,
     253,     0,     0,   254,     0,     0,     0,   255,   256,     0,
       0,   257,     0,     0,     0,   259,   260,   261,   262,   263,
       0,     0,   265,   266,     0,   267,   268,   269,   270,   271,
     857,   273,     0,   274,   275,   276,   277,   278,   279,   280,
     281,     0,   282,     0,   284,   285,   286,   287,   288,     0,
     289,   290,   291,     0,   292,   948,   294,   295,     0,   296,
     858,     0,   298,   299,   300,   301,   302,   303,   304,   305,
       0,   307,     0,   308,   309,   310,     0,   859,   860,     0,
     313,     0,   314,     0,   316,     0,     0,   318,   319,   320,
     321,     0,     0,   322,   323,   324,   325,   326,     0,     0,
     327,   328,   329,   330,   861,   332,     0,   333,   334,   335,
       0,     0,   336,   337,   338,   339,   340,   341,     0,     0,
       0,     0,   135,   136,   137,   138,   139,   140,   141,     0,
     142,     0,     0,     0,     0,     0,     0,   143,   144,   145,
       0,   146,   147,   148,     0,   149,     0,   150,   151,     0,
     152,   153,   154,   155,     0,   862,   156,   157,   158,   159,
       0,   160,   161,   162,   163,   164,     0,     0,   165,   166,
     167,  1294,   168,   169,   170,   171,     0,     0,   172,   173,
       0,     0,     0,     0,     0,   174,   175,   176,   177,   178,
     179,   180,   181,  1091,     0,   182,   183,   184,   185,   186,
       0,     0,     0,   187,   188,   189,   190,     0,   191,   192,
       0,   193,     0,   194,   195,   196,   197,   198,   199,  1092,
     200,   201,     0,   202,     0,   203,     0,     0,     0,   204,
     205,   206,     0,     0,   207,     0,   208,     0,   209,   210,
     211,     0,   212,   213,   214,     0,     0,   215,   216,   217,
     218,   219,   220,   221,     0,   222,     0,   223,     0,     0,
     224,     0,   225,   226,   227,     0,     0,   228,     0,     0,
     229,   230,   231,     0,     0,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,     0,   243,     0,   244,
     245,   246,   247,   248,     0,   249,   250,     0,     0,   251,
     252,   253,  1093,     0,   254,     0,  1094,     0,   255,   256,
       0,     0,   257,     0,     0,   258,   259,   260,   261,   262,
     263,     0,   264,   265,   266,     0,   267,   268,   269,   270,
     271,   272,   273,     0,   274,   275,   276,   277,   278,   279,
     280,   281,     0,   282,   283,   284,   285,   286,   287,   288,
       0,   289,   290,   291,     0,   292,   293,   294,   295,     0,
     296,   297,     0,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,     0,   308,   309,   310,     0,   311,   312,
       0,   313,     0,   314,   315,   316,   317,  1095,   318,   319,
     320,   321,     0,     0,   322,   323,   324,   325,   326,     0,
       0,   327,   328,   329,   330,   331,   332,     0,   333,   334,
     335,     0,     0,   336,   337,   338,   339,   340,   341,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   342,  1081,   910,     0,
       0,     0,     0,   426,   135,   136,   137,   138,   139,   140,
     141,  1082,   142,     0,     0,     0,     0,     0,     0,   143,
     144,   145,     0,   146,   147,   148,     0,   149,     0,   150,
     151,     0,   152,   153,   154,   155,     0,     0,   156,   157,
     158,   159,     0,   160,   161,   162,   163,   164,     0,     0,
     165,   166,   167,     0,   168,   169,   170,   171,     0,     0,
     172,   173,     0,     0,     0,     0,     0,   174,   175,   176,
     177,   178,   179,   180,   181,     0,     0,   182,   183,   184,
     185,   186,     0,     0,     0,   187,   188,   189,   190,     0,
     191,   192,     0,   193,     0,   194,   195,   196,   197,   198,
     199,  1092,   200,   201,     0,   202,     0,   203,     0,     0,
       0,   204,   205,   206,     0,     0,   207,     0,   208,     0,
     209,   210,   211,     0,   212,   213,   214,     0,     0,   215,
     216,   217,   218,   219,   220,   221,     0,   222,     0,   223,
       0,     0,   224,     0,   225,   226,   227,     0,     0,   228,
       0,     0,   229,   230,   231,     0,     0,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,     0,   243,
       0,   244,   245,   246,   247,   248,     0,   249,   250,     0,
       0,   251,   252,   253,  1093,     0,   254,     0,  1094,     0,
     255,   256,     0,     0,   257,     0,     0,   258,   259,   260,
     261,   262,   263,     0,   264,   265,   266,     0,   267,   268,
     269,   270,   271,   272,   273,     0,   274,   275,   276,   277,
     278,   279,   280,   281,     0,   282,   283,   284,   285,   286,
     287,   288,     0,   289,   290,   291,     0,   292,   293,   294,
     295,     0,   296,   297,     0,   298,   299,   300,   301,   302,
     303,   304,   305,   306,   307,     0,   308,   309,   310,     0,
     311,   312,     0,   313,     0,   314,   315,   316,   317,  1095,
     318,   319,   320,   321,     0,     0,   322,   323,   324,   325,
     326,     0,     0,   327,   328,   329,   330,   331,   332,     0,
     333,   334,   335,     0,     0,   336,   337,   338,   339,   340,
     341,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   342,  1081,
     910,     0,     0,     0,     0,   426,     0,     0,     0,     0,
       0,     0,     0,  1082,   135,   416,   137,   138,   139,   140,
     141,   417,   142,     0,     0,     0,     0,     0,     0,   143,
     144,   145,     0,   418,   147,   148,     0,   149,     0,   150,
     151,     0,   152,   153,   154,   155,     0,     0,   156,   157,
     158,   159,     0,   160,   161,   162,   163,   164,     0,     0,
     165,   166,   167,     0,   168,   169,   170,   171,     0,     0,
     172,   173,     0,     0,     0,     0,     0,   174,   175,   176,
     177,   178,   179,   180,   181,     0,     0,   182,   183,   184,
     185,   186,     0,     0,     0,   187,   188,   189,   190,     0,
     191,   192,     0,   193,     0,   194,   195,   196,   197,   198,
     199,     0,   200,   201,     0,   202,     0,   419,     0,   420,
       0,   204,   205,   206,     0,     0,   207,     0,   208,     0,
     209,   210,   211,   421,   212,   213,   214,     0,     0,   215,
     216,   217,   218,   219,   220,   221,     0,   222,     0,   223,
       0,     0,   224,     0,   225,   226,   227,     0,     0,   228,
       0,     0,   229,   230,   231,     0,     0,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,     0,   243,
       0,   422,   245,   246,   247,   248,     0,   249,   250,     0,
       0,   251,   252,   253,     0,     0,   254,     0,     0,     0,
     255,   256,     0,     0,   257,     0,     0,   258,   259,   260,
     261,   262,   263,     0,   264,   265,   266,     0,   423,   268,
     269,   270,   271,   272,   273,     0,   274,   424,   276,   277,
     278,   279,   280,   281,     0,   282,   283,   284,   285,   286,
     287,   288,     0,   289,   290,   291,     0,   292,   293,   294,
     295,     0,   296,   297,     0,   298,   299,   300,   301,   302,
     303,   304,   305,   306,   307,     0,   308,   309,   310,     0,
     311,   312,     0,   313,     0,   314,   315,   316,   317,     0,
     318,   319,   320,   321,     0,     0,   322,   323,   324,   325,
     326,     0,     0,   327,   328,   329,   330,   331,   332,     0,
     333,   334,   335,     0,     0,   336,   337,   338,   339,   340,
     341,     0,     0,     0,   425,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   342,     0,
       0,     0,     0,     0,     0,   426,     0,     0,     0,     0,
       0,     0,     0,   427,   135,   416,   137,   138,   139,   140,
     141,   417,   142,     0,     0,     0,     0,     0,     0,   143,
     144,   145,     0,   418,   147,   148,     0,   149,     0,   150,
     151,     0,   152,   153,   154,   155,     0,     0,   156,   157,
     158,   159,     0,   160,   161,   162,   163,   164,     0,     0,
     165,   166,   167,     0,   168,   169,   170,   171,     0,     0,
     172,   173,     0,     0,     0,     0,     0,   174,   175,   176,
     177,   178,   179,   180,   181,     0,     0,   182,   183,   184,
     185,   186,     0,     0,     0,   187,   188,   189,   190,     0,
     191,   192,     0,   193,     0,   194,   195,   196,   197,   198,
     199,     0,   200,   201,     0,   202,     0,   419,     0,   420,
       0,   204,   205,   206,     0,     0,   207,     0,   208,     0,
     209,   210,   211,   421,   212,   213,   214,     0,     0,   215,
     216,   217,   218,   219,   220,   221,     0,   222,     0,   223,
       0,     0,   224,     0,   225,   226,   227,     0,     0,   228,
       0,     0,   229,   230,   231,     0,     0,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,     0,   243,
       0,   422,   245,   246,   247,   248,     0,   249,   250,     0,
       0,   251,   252,   253,     0,     0,   254,     0,     0,     0,
     255,   256,     0,     0,   257,     0,     0,   258,   259,   260,
     261,   262,   263,     0,   264,   265,   266,     0,   423,   268,
     269,   270,   271,   272,   273,     0,   274,   424,   276,   277,
     278,   279,   280,   281,     0,   282,   283,   284,   285,   286,
     287,   288,     0,   289,   290,   291,     0,   292,   293,   294,
     295,     0,   296,   297,     0,   298,   299,   300,   301,   302,
     303,   304,   305,   306,   307,     0,   308,   309,   310,     0,
     311,   312,     0,   313,     0,   314,   315,   316,   317,     0,
     318,   319,   320,   321,     0,     0,   322,   323,   324,   325,
     326,     0,     0,   327,   328,   329,   330,   331,   332,     0,
     333,   334,   335,     0,     0,   336,   337,   338,   339,   340,
     341,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   342,     0,
       0,     0,     0,     0,     0,   426,     0,     0,     0,     0,
       0,     0,     0,   427,   135,   136,   137,   138,   139,   140,
     141,   417,   142,     0,     0,     0,     0,     0,     0,   143,
     144,   145,     0,   146,   147,   148,     0,   149,     0,   150,
     151,     0,   152,   153,   154,   155,     0,     0,   156,   157,
     158,   159,     0,   160,   161,   162,   163,   164,     0,     0,
     165,   166,   167,     0,   168,   169,   170,   171,     0,     0,
     172,   173,     0,     0,     0,     0,     0,   174,   175,   176,
     177,   178,   179,   180,   181,     0,     0,   182,   183,   184,
     185,   186,     0,     0,     0,   187,   188,   189,   190,     0,
     191,   192,     0,   193,     0,   194,   195,   196,   197,   198,
     199,     0,   200,   201,     0,   202,     0,   203,     0,   420,
       0,   204,   205,   206,     0,     0,   207,     0,   208,     0,
     209,   210,   211,   421,   212,   213,   214,     0,     0,   215,
     216,   217,   218,   219,   220,   221,     0,   222,     0,   223,
       0,     0,   224,     0,   225,   226,   227,     0,     0,   228,
       0,     0,   229,   230,   231,     0,     0,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,     0,   243,
       0,   422,   245,   246,   247,   248,     0,   249,   250,     0,
       0,   251,   252,   253,     0,     0,   254,     0,     0,     0,
     255,   256,     0,     0,   257,     0,     0,   258,   259,   260,
     261,   262,   263,     0,   264,   265,   266,     0,   423,   268,
     269,   270,   271,   272,   273,     0,   274,   275,   276,   277,
     278,   279,   280,   281,     0,   282,   283,   284,   285,   286,
     287,   288,     0,   289,   290,   291,     0,   292,   293,   294,
     295,     0,   296,   297,     0,   298,   299,   300,   301,   302,
     303,   304,   305,   306,   307,     0,   308,   309,   310,     0,
     311,   312,     0,   313,     0,   314,   315,   316,   317,     0,
     318,   319,   320,   321,     0,     0,   322,   323,   324,   325,
     326,     0,     0,   327,   328,   329,   330,   331,   332,     0,
     333,   334,   335,     0,     0,   336,   337,   338,   339,   340,
     341,  1037,     0,     0,     0,     0,     0,  1039,     0,     0,
       0,     0,  1040,     0,     0,     0,     0,     0,     0,     0,
       0,  1037,     0,     0,     0,     0,     0,  1039,     0,     0,
       0,     0,  1040,     0,  1041,     0,     0,     0,   342,     0,
       0,     0,     0,     0,  1037,   426,     0,     0,     0,     0,
    1039,     0,     0,   427,  1041,  1040,     0,     0,     0,     0,
       0,     0,     0,     0,  1037,     0,     0,     0,     0,     0,
    1039,     0,     0,     0,     0,  1040,     0,  1041,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1037,     0,     0,
       0,     0,     0,  1039,     0,     0,     0,  1041,  1040,     0,
       0,     0,     0,     0,     0,  1042,     0,  1037,     0,  1043,
       0,     0,     0,  1039,     0,     0,     0,     0,  1040,     0,
    1041,     0,     0,     0,     0,  1042,  1044,  1045,     0,  1043,
       0,     0,     0,     0,     0,     0,  1046,     0,     0,     0,
    1041,     0,     0,     0,     0,     0,  1044,  1045,  1042,     0,
       0,     0,  1043,     0,  1039,     0,  1046,     0,     0,  1040,
       0,     0,  1047,     0,     0,  1048,     0,     0,  1042,  1044,
    1045,     0,  1043,     0,     0,     0,  1049,     0,  1050,  1046,
       0,  1041,  1047,     0,     0,  1048,     0,     0,     0,  1044,
    1045,  1042,     0,     0,     0,  1043,  1049,     0,  1050,  1046,
       0,     0,     0,     0,     0,  1047,     0,     0,  1048,     0,
       0,  1042,  1044,  1045,     0,  1043,     0,     0,     0,  1049,
       0,  1050,  1046,     0,     0,  1047,     0,  1051,  1048,     0,
       0,     0,  1044,  1045,     0,     0,     0,     0,     0,  1049,
       0,  1050,  1046,     0,     0,  1943,     0,  1051,  1047,     0,
       0,  1048,  1042,     0,     0,     0,  1043,     0,     0,     0,
       0,     0,  1049,     0,  1050,     0,     0,     0,  1047,     0,
    1051,  1048,     0,  1044,  1045,     0,     0,     0,     0,     0,
       0,     0,  1049,  1046,  1050,     0,     0,     0,     0,     0,
    1051,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1048,  1051,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1049,     0,     0,     0,     0,     0,     0,
    1052,     0,     0,  1051,  1053,  1054,  1055,     0,  1056,  1057,
    1058,  1059,  1060,  1061,     0,     0,     0,     0,     0,  1062,
    1052,     0,     0,     0,  1053,  1054,  1055,     0,  1056,  1057,
    1058,  1059,  1060,  1061,     0,     0,     0,     0,  2035,  1062,
       0,     0,     0,  1052,  1051,     0,     0,  1053,  1054,  1055,
       0,  1056,  1057,  1058,  1059,  1060,  1061,     0,     0,     0,
       0,  2148,  1062,  1052,     0,     0,     0,  1053,  1054,  1055,
       0,  1056,  1057,  1058,  1059,  1060,  1061,     0,     0,  2271,
       0,     0,  1062,     0,     0,     0,  1052,     0,     0,     0,
    1053,  1054,  1055,     0,  1056,  1057,  1058,  1059,  1060,  1061,
       0,     0,  1037,     0,  2318,  1062,  1052,     0,  1039,     0,
    1053,  1054,  1055,  1040,  1056,  1057,  1058,  1059,  1060,  1061,
       0,     0,  1039,     0,     0,  1062,     0,  1040,     0,     0,
       0,     0,     0,     0,     0,  1041,  1039,     0,     0,     0,
       0,  1040,     0,     0,     0,     0,     0,  1052,     0,  1041,
       0,     0, -1343, -1343,     0,  1056,  1057,  1058,  1059,  1060,
    1061,     0,     0,  1041,     0,     0,  1062,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1039,     0,     0,
       0,     0,  1040,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1758,     0,     0,     0,     0,     0,
       0,     0,  1039,     0,  1041,     0,  1042,  1040,  1997,     0,
    1043,     0,     0,     0,     0,     0,     0,     0,  1039,     0,
   -1343,     0,     0,  1040,  1043,     0,     0,  1044,  1045,  1041,
       0,     0,     0,     0, -1343,     0,     0,  1046,  1043,     0,
       0,  1044,  1045,     0,     0,  1041,     0,     0,     0,  1998,
       0, -1343,     0,     0,     0,  1044,  1045,     0,     0,     0,
       0,     0,     0,  1047,     0, -1343,  1048,     0,     0,     0,
       0,     0,     0,     0,  2001, -1343,     0,  1049,     0,  1043,
    1048,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    2163,  1049,     0,     0,  1048,     0,  1044,  1045,     0,     0,
   -1343,     0,     0,     0,  1043,  1049, -1343,     0,     0,     0,
       0,     0,     0,     0,     0,     0, -1343,     0,     0,     0,
    1043,  1044,  1045,     0,     0,     0,     0,     0,  1051,     0,
       0, -1343,     0,     0,     0,  1048,     0,  1044,  1045,     0,
       0,     0, -1343,     0,     0,     0,  1049, -1343,     0,     0,
       0,     0,     0,     0,     0,     0, -1343,     0,     0,     0,
    1048,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1049,     0,     0,     0,     0,  1048,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1049,     0,     0,
       0,     0,     0,     0,     0,     0,     0, -1343,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0, -1343,     0,     0,     0,     0,     0,     0,     0,
       0,  1052,     0,     0,     0,  1053,  1054,  1055, -1343,  1056,
    1057,  1058,  1059,  1060,  1061,  1052,     0,     0,     0,     0,
    1062,     0,     0,  1056,  1057,  1058,  1059,  1060,  1061,  1052,
       0,     0,     0,     0,  1062,     0,     0,  1056,  1057,  1058,
    1059,  1060,  1061,     0,     0,     0,     0,     0,  1062,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1052,     0,     0,     0,     0,     0,     0,     0,  1056,  1057,
    1058,  1059,  1060,  1061,     0,     0,     0,     0,     0,  1062,
       0,     0,     0,     0,     0,  1052,     0,     0,     0,     0,
       0,     0,     0,  1056,  1057,  1058,  1059,  1060,  1061,     0,
       0,  1052,     0,     0,  1062,     0,     0,     0,     0,  1056,
    1057,  1058,  1059,  1060,  1061,     0,     0,     0,     0,     0,
    1062,   135,   136,   137,   138,   139,   140,   141,     0,   142,
       0,     0,     0,     0,     0,     0,   143,   144,   145,     0,
     146,   147,   148,     0,   149,     0,   150,   151,     0,   152,
     153,   154,   155,     0,     0,   156,   157,   158,   159,     0,
     160,   161,   162,   163,   164,     0,     0,   165,   166,   167,
       0,   168,   169,   170,   171,     0,     0,   172,   173,     0,
       0,     0,     0,     0,   174,   175,   176,   177,   178,   179,
     180,   181,     0,     0,   182,   183,   184,   185,   186,     0,
       0,     0,   187,   188,   189,   190,     0,   191,   192,     0,
     193,     0,   194,   195,   196,   197,   198,   199,     0,   200,
     201,     0,   202,     0,   203,     0,     0,     0,   204,   205,
     206,     0,     0,   207,     0,   208,     0,   209,   210,   211,
       0,   212,   213,   214,     0,     0,   215,   216,   217,   218,
     219,   220,   221,     0,   222,     0,   223,     0,     0,   224,
       0,   225,   226,   227,     0,     0,   228,     0,     0,   229,
     230,   231,     0,     0,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,     0,   243,     0,   244,   245,
     246,   247,   248,     0,   249,   250,     0,     0,   251,   252,
     253,     0,     0,   254,     0,     0,     0,   255,   256,     0,
       0,   257,     0,     0,   258,   259,   260,   261,   262,   263,
       0,   264,   265,   266,     0,   267,   268,   269,   270,   271,
     272,   273,     0,   274,   275,   276,   277,   278,   279,   280,
     281,     0,   282,   283,   284,   285,   286,   287,   288,     0,
     289,   290,   291,     0,   292,   293,   294,   295,     0,   296,
     297,     0,   298,   299,   300,   301,   302,   303,   304,   305,
     306,   307,     0,   308,   309,   310,     0,   311,   312,     0,
     313,     0,   314,   315,   316,   317,     0,   318,   319,   320,
     321,     0,     0,   322,   323,   324,   325,   326,     0,     0,
     327,   328,   329,   330,   331,   332,     0,   333,   334,   335,
       0,     0,   336,   337,   338,   339,   340,   341,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   342,  2479,   457,     0,  2480,
    2481,     0,  2482,   135,   136,   137,   138,   139,   140,   141,
       0,   142,     0,     0,     0,     0,     0,     0,   143,   144,
     145,     0,   146,   147,   148,     0,   149,     0,   150,   151,
       0,   152,   153,   154,   155,     0,     0,   156,   157,   158,
     159,     0,   160,   161,   162,   163,   164,     0,     0,   165,
     166,   167,     0,   168,   169,   170,   171,     0,     0,   172,
     173,     0,     0,     0,     0,     0,   174,   175,   176,   177,
     178,   179,   180,   181,  1075,     0,   182,   183,   184,   185,
     186,     0,     0,     0,   187,   188,   189,   190,     0,   191,
     192,     0,   193,     0,   194,   195,   196,   197,   198,   199,
       0,   200,   201,     0,   202,     0,   203,     0,     0,     0,
     204,   205,   206,     0,     0,   207,     0,   208,     0,   209,
     210,   211,     0,   212,   213,   214,     0,     0,   215,   216,
     217,   218,   219,   220,   221,     0,   222,     0,   223,     0,
       0,   224,     0,   225,   226,   227,     0,     0,   228,     0,
       0,   229,   230,   231,     0,     0,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,     0,   243,     0,
     244,   245,   246,   247,   248,     0,   249,   250,     0,     0,
     251,   252,   253,     0,     0,   254,     0,     0,     0,   255,
     256,     0,     0,   257,     0,     0,   258,   259,   260,   261,
     262,   263,     0,   264,   265,   266,     0,   267,   268,   269,
     270,   271,   272,   273,     0,   274,   275,   276,   277,   278,
     279,   280,   281,     0,   282,   283,   284,   285,   286,   287,
     288,     0,   289,   290,   291,     0,   292,   293,   294,   295,
       0,   296,   297,     0,   298,   299,   300,   301,   302,   303,
     304,   305,   306,   307,     0,   308,   309,   310,     0,   311,
     312,     0,   313,     0,   314,   315,   316,   317,     0,   318,
     319,   320,   321,     0,     0,   322,   323,   324,   325,   326,
       0,     0,   327,   328,   329,   330,   331,   332,     0,   333,
     334,   335,     0,     0,   336,   337,   338,   339,   340,   341,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   135,   136,   137,   138,   139,
     140,   141,     0,   142,     0,     0,     0,     0,     0,     0,
     143,   144,   145,     0,   146,   147,   148,   342,   149,   910,
     150,   151,     0,   152,   153,   154,   155,     0,     0,   156,
     157,   158,   159,     0,   160,   161,   162,   163,   164,     0,
       0,   165,   166,   167,     0,   168,   169,   170,   171,     0,
       0,   172,   173,     0,     0,     0,     0,     0,   174,   175,
     176,   177,   178,   179,   180,   181,     0,     0,   182,   183,
     184,   185,   186,     0,     0,     0,   187,   188,   189,   190,
       0,   191,   192,     0,   193,     0,   194,   195,   196,   197,
     198,   199,     0,   200,   201,     0,   202,     0,   203,     0,
       0,     0,   204,   205,   206,     0,     0,   207,     0,   208,
       0,   209,   210,   211,     0,   212,   213,   214,     0,     0,
     215,   216,   217,   218,   219,   220,   221,     0,   222,     0,
     223,     0,     0,   224,     0,   225,   226,   227,     0,     0,
     228,     0,     0,   229,   230,   231,     0,     0,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,     0,
     243,     0,   244,   245,   246,   247,   248,     0,   249,   250,
       0,     0,   251,   252,   253,     0,     0,   254,     0,     0,
       0,   255,   256,     0,     0,   257,     0,     0,   258,   259,
     260,   261,   262,   263,     0,   264,   265,   266,     0,   267,
     268,   269,   270,   271,   272,   273,     0,   274,   275,   276,
     277,   278,   279,   280,   281,     0,   282,   283,   284,   285,
     286,   287,   288,     0,   289,   290,   291,     0,   292,   293,
     294,   295,     0,   296,   297,     0,   298,   299,   300,   301,
     302,   303,   304,   305,   306,   307,     0,   308,   309,   310,
       0,   311,   312,     0,   313,     0,   314,   315,   316,   317,
       0,   318,   319,   320,   321,     0,     0,   322,   323,   324,
     325,   326,     0,     0,   327,   328,   329,   330,   331,   332,
       0,   333,   334,   335,     0,     0,   336,   337,   338,   339,
     340,   341,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   342,
       0,   910,   135,   136,   137,   138,   139,   140,   141,  1167,
     142,  1168,  1169,  1170,  1171,  1172,  1173,   143,   144,   145,
     547,   146,   147,   148,   548,   149,   549,   150,   151,  1174,
     152,   153,   154,   155,  1175,  1176,   156,   157,   158,   159,
    1177,   160,   161,   162,   163,   164,  1178,  1179,   165,   166,
     167,  1180,   168,   169,   170,   171,     0,  1181,   172,   173,
     550,  1182,  1183,  1184,  1185,   174,   175,   176,   177,   178,
     179,   180,   181,  1186,  1187,   182,   183,   184,   185,   186,
    1188,  1189,  1190,   187,   188,   189,   190,  1191,   191,   192,
    1192,   193,  1193,   194,   195,   196,   197,   198,   199,  1194,
     200,   201,  1195,   202,  1196,   203,   551,  1197,   552,   204,
     205,   206,  1198,  1199,   207,  1200,   208,   553,   209,   210,
     211,   554,   212,   213,   214,  1201,   555,   215,   216,   217,
     218,   219,   220,   221,  1202,   222,  1203,   223,   556,   557,
     224,   558,   225,   226,   227,  1204,   559,   228,   560,  1205,
     229,   230,   231,  1206,  1207,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   561,   243,  1208,   244,
     245,   246,   247,   248,  1209,   249,   250,   562,  1210,   251,
     252,   253,  1211,  1212,   254,  1213,  1214,  1215,   255,   256,
    1216,  1217,   257,   563,   564,   258,   259,   260,   261,   262,
     263,  1218,   264,   265,   266,  1219,   267,   268,   269,   270,
     271,   272,   273,  1220,   274,   275,   276,   277,   278,   279,
     280,   281,   565,   282,   283,   284,   285,   286,   287,   288,
    1221,   289,   290,   291,  1222,   292,   293,   294,   295,   566,
     296,   297,  1223,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,  1224,   308,   309,   310,  1225,   311,   312,
    1226,   313,  1227,   314,   315,   316,   317,  1228,   318,   319,
     320,   321,  1229,  1230,   322,   323,   324,   325,   326,  1231,
    1232,   327,   328,   329,   330,   331,   332,   567,   333,   334,
     335,  1233,  1234,   336,   337,   338,   339,   340,   341,     0,
       0,     0,     0,   135,   136,   137,   138,   139,   140,   141,
       0,   142,     0,     0,     0,     0,     0,     0,   143,   144,
     145,   547,   146,   147,   148,   548,   149,   549,   150,   151,
       0,   152,   153,   154,   155,     0,  1235,   156,   157,   158,
     159,     0,   160,   161,   162,   163,   164,     0,     0,   165,
     166,   167,     0,   168,   169,   170,   171,     0,     0,   172,
     173,   550,     0,     0,     0,     0,   174,   175,   176,   177,
     178,   179,   180,   181,     0,     0,   182,   183,   184,   185,
     186,     0,     0,     0,   187,   188,   189,   190,     0,   191,
     192,     0,   193,     0,   194,   195,   196,   197,   198,   199,
       0,   200,   201,     0,   202,     0,   203,   551,     0,   552,
     204,   205,   206,     0,     0,   207,     0,   208,   553,   209,
     210,   211,   554,   212,   213,   214,     0,   555,   215,   216,
     217,   218,   219,   220,   221,     0,   222,     0,   223,   556,
     557,   224,   558,   225,   226,   227,     0,   559,   228,   560,
       0,   229,   230,   231,     0,     0,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   561,   243,   450,
     244,   245,   246,   247,   248,     0,   249,   250,   562,     0,
     251,   252,   253,     0,     0,   254,   451,     0,     0,   255,
     256,     0,     0,   257,   563,   564,   258,   259,   260,   261,
     262,   263,     0,   264,   265,   266,     0,   267,   268,   269,
     270,   271,   272,   273,     0,   274,   275,   276,   277,   278,
     279,   280,   281,   565,   282,   283,   284,   285,   286,   287,
     288,     0,   289,   290,   291,     0,   292,   293,   294,   295,
     566,   296,   297,     0,   298,   299,   300,   301,   302,   303,
     304,   305,   306,   307,     0,   308,   309,   310,     0,   311,
     312,     0,   313,     0,   314,   315,   316,   317,     0,   318,
     319,   320,   321,     0,     0,   322,   323,   324,   325,   326,
       0,     0,   327,   328,   329,   330,   331,   332,   567,   333,
     334,   335,     0,     0,   336,   337,   338,   339,   340,   341,
       0,     0,     0,     0,   135,   136,   137,   138,   139,   140,
     141,     0,   142,     0,     0,     0,     0,     0,     0,   143,
     144,   145,     0,   146,   147,   148,     0,   149,     0,   150,
     151,  2192,   152,   153,   154,   155,     0,   568,   156,   157,
     158,   159,     0,   160,   161,   162,   163,   164,     0,     0,
     165,   166,   167,     0,   168,   169,   170,   171,     0,     0,
     172,   173,     0,     0,     0,     0,     0,   174,   175,   176,
     177,   178,   179,   180,   181,     0,     0,   182,   183,   184,
     185,   186,     0,     0,     0,   187,   188,   189,   190,     0,
     191,   192,     0,   193,     0,   194,   195,   196,   197,   198,
     199,     0,   200,   201,     0,   202,     0,   203,     0,     0,
       0,   204,   205,   206,     0,     0,   207,     0,   208,     0,
     209,   210,   211,     0,   212,   213,   214,     0,     0,   215,
     216,   217,   218,   219,   220,   221,     0,   222,     0,   223,
       0,     0,   224,     0,   225,   226,   227,     0,     0,   228,
       0,     0,   229,   230,   231,     0,     0,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,     0,   243,
     450,   244,   245,   246,   247,   248,     0,   249,   250,     0,
       0,   251,   252,   253,     0,     0,   254,   451,     0,     0,
     255,   256,     0,     0,   257,     0,     0,   258,   259,   260,
     261,   262,   263,     0,   264,   265,   266,     0,   267,   268,
     269,   270,   271,   272,   273,     0,   274,   275,   276,   277,
     278,   279,   280,   281,     0,   282,   283,   284,   285,   286,
     287,   288,     0,   289,   290,   291,     0,   292,   293,   294,
     295,     0,   296,   297,     0,   298,   299,   300,   301,   302,
     303,   304,   305,   306,   307,     0,   308,   309,   310,     0,
     311,   312,     0,   313,     0,   314,   315,   316,   317,     0,
     318,   319,   320,   321,     0,     0,   322,   323,   324,   325,
     326,     0,     0,   327,   328,   329,   330,   331,   332,     0,
     333,   334,   335,     0,     0,   336,   337,   338,   339,   340,
     341,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  2193,  2194,  2195,  2196,  2197,     0,     0,   342,   135,
     136,   137,   138,   139,   140,   141,     0,   142,     0,     0,
       0,     0,     0,     0,   143,   144,   145,     0,   146,   147,
     148,     0,   149,     0,   150,   151,     0,   152,   153,   154,
     155,     0,     0,   156,   157,   158,   159,  1134,   160,   161,
     162,   163,   164,     0,     0,   165,   166,   167,  1136,   168,
     169,   170,   171,     0,     0,   172,   173,     0,     0,     0,
       0,     0,   174,   175,   176,   177,   178,   179,   180,   181,
       0,     0,   182,   183,   184,   185,   186,     0,     0,     0,
     187,   188,   189,   190,     0,   191,   192,     0,   193,     0,
     194,   195,   196,   197,   198,   199,     0,   200,   201,     0,
     202,  1137,   203,     0,     0,     0,   204,   205,   206,     0,
       0,   207,     0,   208,     0,   209,   210,   211,     0,   212,
     213,   214,     0,     0,   215,   216,   217,   218,   219,   220,
     221,     0,   222,     0,   223,     0,     0,   224,     0,   225,
     226,   227,     0,     0,   228,  1265,     0,   229,   230,   231,
       0,     0,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,     0,   243,     0,   244,   245,   246,   247,
     248,     0,   249,   250,     0,     0,   251,   252,   253,     0,
       0,   254,     0,     0,     0,   255,   256,     0,     0,   257,
       0,     0,   258,   259,   260,   261,   262,   263,     0,   264,
     265,   266,  1138,   267,   268,   269,   270,   271,   272,   273,
       0,   274,   275,   276,   277,   278,   279,   280,   281,     0,
     282,   283,   284,   285,   286,   287,   288,     0,   289,   290,
     291,     0,   292,   293,   294,   295,     0,   296,   297,     0,
     298,   299,   300,   301,   302,   303,   304,   305,   306,   307,
       0,   308,   309,   310,     0,   311,   312,     0,   313,     0,
     314,   315,   316,   317,     0,   318,   319,   320,   321,     0,
    1139,   322,   323,   324,   325,   326,     0,     0,   327,   328,
     329,   330,   331,   332,     0,   333,   334,   335,     0,     0,
     336,   337,   338,   339,   340,   341,     0,     0,     0,     0,
     135,   136,   137,   138,   139,   140,   141,     0,   142,     3,
       4,     0,     0,     0,     0,   143,   144,   145,     0,   146,
     147,   148,     0,   149,     0,   150,   151,     0,   152,   153,
     154,   155,     0,   342,   156,   157,   158,   159,     0,   160,
     161,   162,   163,   164,     0,     0,   165,   166,   167,     0,
     168,   169,   170,   171,     0,     0,   172,   173,     0,     0,
       0,     0,     0,   174,   175,   176,   177,   178,   179,   180,
     181,     0,     0,   182,   183,   184,   185,   186,     0,     0,
       0,   187,   188,   189,   190,     0,   191,   192,     0,   193,
       0,   194,   195,   196,   197,   198,   199,     0,   200,   201,
       0,   202,     0,   203,     0,     0,     0,   204,   205,   206,
       0,     0,   207,     0,   208,     0,   209,   210,   211,     0,
     212,   213,   214,     0,     0,   215,   216,   217,   218,   219,
     220,   221,     0,   222,     0,   223,     0,     0,   224,     0,
     225,   226,   227,     0,     0,   228,     0,     0,   229,   230,
     231,     0,     0,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,     0,   243,   450,   244,   245,   246,
     247,   248,     0,   249,   250,     0,     0,   251,   252,   253,
       0,     0,   254,   451,     0,     0,   255,   256,     0,     0,
     257,     0,     0,   258,   259,   260,   261,   262,   263,     0,
     264,   265,   266,     0,   267,   268,   269,   270,   271,   272,
     273,     0,   274,   275,   276,   277,   278,   279,   280,   281,
       0,   282,   283,   284,   285,   286,   287,   288,     0,   289,
     290,   291,     0,   292,   293,   294,   295,     0,   296,   297,
       0,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,     0,   308,   309,   310,     0,   311,   312,     0,   313,
       0,   314,   315,   316,   317,     0,   318,   319,   320,   321,
       0,     0,   322,   323,   324,   325,   326,     0,     0,   327,
     328,   329,   330,   331,   332,     0,   333,   334,   335,     0,
       0,   336,   337,   338,   339,   340,   341,     0,     0,     0,
       0,   135,   136,   137,   138,   139,   140,   141,     0,   142,
       0,     0,     0,     0,     0,     0,   143,   144,   145,     0,
     146,   147,   148,     0,   149,     0,   150,   151,     0,   152,
     153,   154,   155,     0,   342,   156,   157,   158,   159,     0,
     160,   161,   162,   163,   164,     0,     0,   165,   166,   167,
       0,   168,   169,   170,   171,     0,     0,   172,   173,     0,
       0,     0,     0,     0,   174,   175,   176,   177,   178,   179,
     180,   181,     0,     0,   182,   183,   184,   185,   186,     0,
       0,     0,   187,   188,   189,   190,     0,   191,   192,     0,
     193,     0,   194,   195,   196,   197,   198,   199,     0,   200,
     201,     0,   202,     0,   203,     0,     0,     0,   204,   205,
     206,     0,     0,   207,     0,   208,     0,   209,   210,   211,
       0,   212,   213,   214,     0,     0,   215,   216,   217,   218,
     219,   220,   221,     0,   222,     0,   223,     0,     0,   224,
       0,   225,   226,   227,     0,     0,   228,     0,     0,   229,
     230,   231,     0,     0,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,     0,   243,   450,   244,   245,
     246,   247,   248,     0,   249,   250,     0,     0,   251,   252,
     253,     0,     0,   254,   451,     0,   500,   255,   256,     0,
       0,   257,     0,     0,   258,   259,   260,   261,   262,   263,
       0,   264,   265,   266,     0,   267,   268,   269,   270,   271,
     272,   273,     0,   274,   275,   276,   277,   278,   279,   280,
     281,     0,   282,   283,   284,   285,   286,   287,   288,     0,
     289,   290,   291,     0,   292,   293,   294,   295,     0,   296,
     297,     0,   298,   299,   300,   301,   302,   303,   304,   305,
     306,   307,     0,   308,   309,   310,     0,   311,   312,     0,
     313,     0,   314,   315,   316,   317,     0,   318,   319,   320,
     321,     0,     0,   322,   323,   324,   325,   326,     0,     0,
     327,   328,   329,   330,   331,   332,     0,   333,   334,   335,
       0,     0,   336,   337,   338,   339,   340,   341,     0,     0,
       0,     0,   135,   136,   137,   138,   139,   140,   141,     0,
     142,     0,     0,     0,     0,     0,     0,   143,   144,   145,
       0,   146,   147,   148,     0,   149,     0,   150,   151,     0,
     152,   153,   154,   155,     0,   342,   156,   157,   158,   159,
       0,   160,   161,   162,   163,   164,     0,     0,   165,   166,
     167,     0,   168,   169,   170,   171,     0,     0,   172,   173,
       0,     0,     0,     0,     0,   174,   175,   924,   177,   178,
     179,   180,   181,     0,     0,   182,   183,   184,   185,   186,
       0,     0,     0,   187,   188,   189,   190,     0,   191,   192,
       0,   193,     0,   194,   195,   196,   197,   198,   199,     0,
     200,   201,     0,   202,     0,   203,     0,     0,     0,   925,
     205,   206,     0,     0,   207,     0,   208,     0,   209,   210,
     211,     0,   212,   213,   214,     0,     0,   215,   216,   217,
     218,   219,   220,   221,     0,   222,     0,   223,     0,     0,
     224,     0,   225,   226,   926,     0,     0,   228,     0,     0,
     229,   230,   231,     0,     0,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,     0,   243,   450,   244,
     245,   246,   247,   248,     0,   249,   250,     0,     0,   251,
     252,   253,     0,     0,   254,   451,     0,     0,   255,   256,
       0,     0,   257,     0,     0,   258,   259,   260,   261,   262,
     263,     0,   264,   265,   266,     0,   267,   268,   269,   270,
     271,   272,   273,     0,   274,   275,   276,   277,   278,   279,
     280,   281,     0,   282,   283,   284,   927,   286,   287,   288,
       0,   289,   290,   291,     0,   292,   293,   294,   295,     0,
     296,   297,     0,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   928,   308,   309,   310,     0,   311,   312,
       0,   313,     0,   314,   315,   316,   317,     0,   318,   319,
     320,   321,     0,     0,   322,   323,   324,   325,   326,     0,
       0,   327,   328,   329,   330,   331,   332,     0,   333,   334,
     335,     0,     0,   336,   337,   338,   339,   340,   341,     0,
       0,     0,     0,   135,   136,   137,   138,   139,   140,   141,
       0,   142,     0,     0,     0,     0,     0,     0,   143,   144,
     145,     0,   146,   147,   148,     0,   149,     0,   150,   151,
       0,   152,   153,   154,   155,     0,   342,   156,   157,   158,
     159,     0,   160,   161,   162,   163,   164,     0,     0,   165,
     166,   167,     0,   168,   169,   170,   171,     0,     0,   172,
     173,     0,     0,     0,     0,     0,   174,   175,   176,   177,
     178,   179,   180,   181,     0,     0,   182,   183,   184,   185,
     186,     0,     0,     0,   187,   188,   189,   190,     0,   191,
     192,     0,   193,     0,   194,   195,   196,   197,   198,   199,
       0,   200,   201,     0,   202,     0,   203,     0,     0,     0,
     204,   205,  1320,     0,     0,   207,     0,   208,     0,   209,
     210,   211,     0,   212,   213,   214,     0,     0,   215,   216,
     217,   218,   219,   220,   221,     0,   222,     0,   223,     0,
       0,   224,     0,   225,   226,   227,     0,     0,   228,     0,
       0,   229,   230,  1321,     0,     0,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,     0,   243,   450,
     244,   245,   246,   247,   248,     0,   249,   250,     0,     0,
     251,   252,   253,     0,     0,   254,   451,     0,     0,   255,
     256,     0,     0,   257,     0,     0,   258,   259,   260,   261,
     262,   263,     0,   264,   265,   266,     0,   267,   268,   269,
     270,   271,   272,   273,     0,   274,   275,   276,   277,   278,
     279,   280,   281,     0,   282,   283,   284,   285,   286,   287,
     288,     0,   289,   290,   291,     0,   292,   293,   294,   295,
       0,   296,   297,     0,   298,   299,   300,   301,   302,   303,
     304,   305,   306,   307,  1322,  1323,   309,  1324,     0,   311,
     312,     0,   313,     0,   314,   315,   316,   317,     0,   318,
     319,   320,   321,     0,     0,   322,   323,   324,   325,   326,
       0,     0,   327,   328,   329,   330,   331,   332,     0,   333,
     334,   335,     0,     0,   336,   337,   338,   339,   340,   341,
       0,     0,     0,     0,   135,   136,   137,   138,   139,   140,
     141,     0,   142,     0,     0,     0,     0,     0,     0,   143,
     144,   145,     0,   146,   147,   148,     0,   149,     0,   150,
     151,     0,   152,   153,   154,   155,     0,   342,   156,   157,
     158,   159,     0,   160,   161,   162,   163,   164,     0,     0,
     165,   166,   167,     0,   168,   169,   170,   171,     0,     0,
     172,   173,     0,     0,     0,     0,     0,   174,   175,   176,
     177,   178,   179,   180,   181,     0,     0,   182,   183,   184,
     185,   186,     0,     0,     0,   187,   188,   189,   190,     0,
     191,   192,     0,   193,     0,   194,   195,   196,   197,   198,
     199,     0,   200,   201,     0,   202,     0,   203,     0,     0,
       0,   204,   205,   206,     0,     0,   207,     0,   208,     0,
     209,   210,   211,     0,   212,   213,   214,     0,     0,   215,
     216,   217,   218,   219,   220,   221,     0,   222,     0,   223,
       0,     0,   224,     0,   225,   226,   227,     0,     0,   228,
       0,     0,   229,   230,   231,     0,     0,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,     0,   243,
     450,   244,   245,   246,   247,   248,     0,   249,   250,     0,
       0,   251,   252,   253,     0,     0,   254,   451,     0,     0,
     255,   256,     0,     0,   257,     0,     0,   258,   259,   260,
     261,   262,   263,     0,   264,   265,   266,     0,   267,   268,
     269,   270,   271,   272,   273,     0,   274,   275,   276,   277,
     278,   279,   280,   281,     0,   282,   283,   284,   285,   286,
     287,   288,     0,   289,   290,   291,     0,   292,   293,   294,
     295,     0,   296,   297,     0,   298,   299,   300,   301,   302,
     303,   304,   305,   306,   307,     0,   308,   309,   310,     0,
     311,   312,     0,   313,     0,   314,   315,   316,   317,     0,
     318,   319,   320,   321,     0,     0,   322,   323,   324,   325,
     326,     0,  2335,   327,   328,   329,   330,   331,   332,     0,
     333,   334,   335,     0,     0,   336,   337,   338,   339,   340,
     341,     0,     0,     0,     0,   135,   136,   137,   138,   139,
     140,   141,     0,   142,     0,     0,     0,     0,     0,     0,
     143,   144,   145,     0,   146,   147,   148,     0,   149,     0,
     150,   151,     0,   152,   153,   154,   155,     0,   342,   156,
     157,   158,   159,     0,   160,   161,   162,   163,   164,     0,
       0,   165,   166,   167,     0,   168,   169,   170,   171,     0,
       0,   172,   173,     0,     0,     0,     0,     0,   174,   175,
     176,   177,   178,   179,   180,   181,     0,     0,   182,   183,
     184,   185,   186,     0,     0,     0,   187,   188,   189,   190,
       0,   191,   192,     0,   193,     0,   194,   195,   196,   197,
     198,   199,     0,   200,   201,     0,   202,     0,   203,     0,
       0,     0,   204,   205,   206,     0,     0,   207,     0,   208,
       0,   209,   210,   211,     0,   212,   213,   214,     0,     0,
     215,   216,   217,   218,   219,   220,   221,     0,   222,     0,
     223,     0,     0,   224,     0,   225,   226,   227,     0,     0,
     228,     0,     0,   229,   230,   231,     0,     0,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,     0,
     243,   450,   244,   245,   246,   247,   248,     0,   249,   250,
       0,     0,   251,   252,   253,     0,     0,   254,   451,     0,
       0,   255,   256,     0,     0,   257,     0,     0,   258,   259,
     260,   261,   262,   263,     0,   264,   265,   266,     0,   267,
     268,   269,   270,   271,   272,   273,     0,   274,   275,   276,
     277,   278,   279,   280,   281,     0,   282,   283,   284,   285,
     286,   287,   288,     0,   289,   290,   291,     0,   292,   293,
     294,   295,     0,   296,   297,     0,   298,   299,   300,   301,
     302,   303,   304,   305,   306,   307,     0,   308,   309,   310,
       0,   311,   312,     0,   313,     0,   314,   315,   316,   317,
       0,   318,   319,   320,   321,     0,     0,   322,   323,   324,
     325,   326,     0,     0,   327,   328,   329,   330,   331,   332,
       0,   333,   334,   335,     0,     0,   336,   337,   338,   339,
     340,   341,     0,     0,     0,     0,   135,   136,   137,   138,
     139,   140,   141,     0,   142,     0,     0,     0,     0,     0,
       0,   143,   144,   145,     0,   146,   147,   148,     0,   149,
       0,   150,   151,     0,   152,   153,   154,   155,     0,   342,
     156,   157,   158,   159,     0,   160,   161,   162,   163,   164,
       0,     0,   165,   166,   167,     0,   168,   169,   170,   171,
       0,     0,   172,   173,     0,     0,     0,     0,     0,   174,
     175,   176,   177,   178,   179,   180,   181,     0,     0,   182,
     183,   184,   185,   186,     0,     0,     0,   187,   188,   189,
     190,     0,   191,   192,     0,   193,     0,   194,   195,   196,
     197,   198,   199,     0,   200,   201,     0,   202,     0,   203,
       0,     0,     0,   204,   205,   206,     0,     0,   207,     0,
     208,     0,   209,   210,   211,     0,   212,   213,   214,     0,
       0,   215,   216,   217,   218,   219,   220,   221,     0,   222,
       0,   223,     0,     0,   224,     0,   225,   226,   227,     0,
       0,   228,     0,     0,   229,   230,   231,     0,     0,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
       0,   243,   450,   244,   245,   246,   247,   248,     0,   249,
     250,     0,     0,   251,   252,   253,     0,     0,   254,   451,
       0,     0,   255,   256,     0,     0,   257,     0,     0,   258,
     259,   260,   261,   262,   263,     0,   264,   265,   266,     0,
     267,   268,   269,   270,   271,   272,   273,     0,   274,   275,
     276,   277,   278,   279,   280,   281,     0,   282,   283,   284,
     285,   286,   287,   288,     0,   289,   290,   291,     0,   292,
     293,   294,   295,     0,   296,   297,     0,   298,   299,   300,
     301,   302,   303,   304,   305,   306,   307,     0,   308,   309,
     310,     0,   311,   312,     0,   313,     0,   314,   315,   316,
     317,     0,   318,   319,   320,   321,     0,     0,   322,   323,
     324,   325,   326,     0,     0,   327,   328,   329,   330,   331,
     332,     0,   333,   334,   335,     0,     0,   336,   337,   338,
     339,   340,   341,     0,     0,     0,     0,   135,   136,   137,
     138,   139,   140,   141,   472,   142,     0,     0,     0,     0,
       0,     0,   143,   144,   145,     0,   146,   147,   148,     0,
     149,     0,   150,   151,     0,   152,   153,   154,   155,     0,
    1877,   156,   157,   158,   159,     0,   160,   161,   162,   163,
     164,     0,     0,   165,   166,   167,     0,   168,   169,   170,
     171,     0,     0,   172,   173,     0,     0,     0,     0,     0,
     174,   175,   176,   177,   178,   179,   180,   181,     0,     0,
     182,   183,   184,   185,   186,     0,     0,     0,   187,   188,
     189,   190,     0,   191,   192,     0,   193,     0,   194,   195,
     196,   197,   198,   199,     0,   200,   201,     0,   202,     0,
     203,     0,     0,     0,   204,   205,   206,     0,     0,   207,
       0,   208,     0,   209,   210,   211,     0,   212,   213,   214,
       0,     0,   215,   216,   217,   218,   219,   220,   221,     0,
     222,     0,   223,     0,     0,   224,     0,   225,   226,   227,
       0,     0,   228,     0,     0,   229,   230,   231,     0,     0,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,     0,   243,     0,   244,   245,   246,   247,   248,     0,
     249,   250,     0,     0,   251,   252,   253,     0,     0,   254,
       0,     0,     0,   255,   256,     0,     0,   257,     0,     0,
     258,   259,   260,   261,   262,   263,     0,   264,   265,   266,
       0,   267,   268,   269,   270,   271,   272,   273,     0,   274,
     275,   276,   277,   278,   279,   280,   281,     0,   282,   283,
     284,   285,   286,   287,   288,     0,   289,   290,   473,     0,
     292,   293,   294,   295,     0,   296,   297,     0,   298,   299,
     300,   301,   302,   303,   304,   305,   306,   307,     0,   308,
     309,   310,     0,   474,   312,     0,   313,     0,   475,   315,
     316,   317,     0,   318,   319,   320,   321,     0,     0,   322,
     323,   324,   325,   326,     0,     0,   327,   328,   329,   330,
     331,   332,     0,   333,   334,   335,     0,     0,   336,   337,
     338,   339,   340,   341,     0,     0,     0,     0,   135,   136,
     137,   138,   139,   140,   141,   491,   142,     0,     0,     0,
       0,     0,     0,   143,   144,   145,     0,   146,   147,   148,
       0,   149,     0,   150,   151,     0,   152,   153,   154,   155,
       0,   342,   156,   157,   158,   159,     0,   160,   161,   162,
     163,   164,     0,     0,   165,   166,   167,     0,   168,   169,
     170,   171,     0,     0,   172,   173,     0,     0,     0,     0,
       0,   174,   175,   176,   177,   178,   179,   180,   181,     0,
       0,   182,   183,   184,   185,   186,     0,     0,     0,   187,
     188,   189,   190,     0,   191,   192,     0,   193,     0,   194,
     195,   196,   197,   198,   199,     0,   200,   201,     0,   202,
       0,   203,     0,     0,     0,   204,   205,   206,     0,     0,
     207,     0,   208,     0,   209,   210,   211,     0,   212,   213,
     214,     0,     0,   215,   216,   217,   218,   219,   220,   221,
       0,   222,     0,   223,     0,     0,   224,     0,   225,   226,
     227,     0,     0,   228,     0,     0,   229,   230,   231,     0,
       0,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,     0,   243,     0,   244,   245,   246,   247,   248,
       0,   249,   250,     0,     0,   251,   252,   253,     0,     0,
     254,     0,     0,     0,   255,   256,     0,     0,   257,     0,
       0,   258,   259,   260,   261,   262,   263,     0,   264,   265,
     266,     0,   267,   268,   269,   270,   271,   272,   273,     0,
     274,   275,   276,   277,   278,   279,   280,   281,     0,   282,
     283,   284,   285,   286,   287,   288,     0,   289,   290,   492,
       0,   292,   293,   294,   295,     0,   296,   297,     0,   298,
     299,   300,   301,   302,   303,   304,   305,   306,   307,     0,
     308,   309,   310,     0,   493,   312,     0,   313,     0,   494,
     315,   316,   317,     0,   318,   319,   320,   321,     0,     0,
     322,   323,   324,   325,   326,     0,     0,   327,   328,   329,
     330,   331,   332,     0,   333,   334,   335,     0,     0,   336,
     337,   338,   339,   340,   341,     0,     0,     0,     0,   135,
     136,   137,   138,   139,   140,   141,   765,   142,     0,     0,
       0,     0,     0,     0,   143,   144,   145,     0,   146,   147,
     148,     0,   149,     0,   150,   151,     0,   152,   153,   154,
     155,     0,   342,   156,   157,   158,   159,     0,   160,   161,
     162,   163,   164,     0,     0,   165,   166,   167,     0,   168,
     169,   170,   171,     0,     0,   172,   173,     0,     0,     0,
       0,     0,   174,   175,   176,   177,   178,   179,   180,   181,
       0,     0,   182,   183,   184,   185,   186,     0,     0,     0,
     187,   188,   189,   190,     0,   191,   192,     0,   193,     0,
     194,   195,   196,   197,   198,   199,     0,   200,   201,     0,
     202,     0,   203,     0,     0,     0,   204,   205,   206,     0,
       0,   207,     0,   208,     0,   209,   210,   211,     0,   212,
     213,   214,     0,     0,   215,   216,   217,   218,   219,   220,
     221,     0,   222,     0,   223,     0,     0,   224,     0,   225,
     226,   227,     0,     0,   228,     0,     0,   229,   230,   231,
       0,     0,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,     0,   243,     0,   244,   245,   246,   247,
     248,     0,   249,   250,     0,     0,   251,   252,   253,     0,
       0,   254,     0,     0,     0,   255,   256,     0,     0,   257,
       0,     0,   258,   259,   260,   261,   262,   263,     0,   264,
     265,   266,     0,   267,   268,   269,   270,   271,   272,   273,
       0,   274,   275,   276,   277,   278,   279,   280,   281,     0,
     282,   283,   284,   285,   286,   287,   288,     0,   289,   290,
     291,     0,   292,   293,   294,   295,     0,   296,   297,     0,
     298,   299,   300,   301,   302,   303,   304,   305,   306,   307,
       0,   308,   309,   310,     0,   311,   312,     0,   313,     0,
     314,   315,   316,   317,     0,   318,   319,   320,   321,     0,
       0,   322,   323,   324,   325,   326,     0,     0,   327,   328,
     329,   330,   331,   332,     0,   333,   334,   335,     0,     0,
     336,   337,   338,   339,   340,   341,     0,     0,     0,     0,
     135,   136,   137,   138,   139,   140,   141,     0,   142,     0,
       0,     0,     0,     0,     0,   143,   144,   145,   774,   146,
     147,   148,     0,   149,     0,   150,   151,     0,   152,   153,
     154,   155,     0,   342,   156,   157,   158,   775,     0,   160,
     161,   162,   163,   164,     0,     0,   165,   166,   167,     0,
     168,   169,   170,   171,     0,     0,   172,   173,     0,     0,
       0,     0,     0,   174,   175,   176,   177,   178,   179,   180,
     181,     0,     0,   182,   183,   184,   185,   186,     0,     0,
       0,   187,   188,   189,   190,     0,   191,   192,     0,   193,
       0,   194,   195,   196,   197,   198,   199,     0,   200,   201,
       0,   202,     0,   203,     0,     0,     0,   204,   205,   206,
       0,     0,   207,     0,   208,     0,   209,   210,   211,     0,
     212,   213,   214,     0,     0,   215,   216,   217,   218,   219,
     220,   221,     0,   222,     0,   223,     0,     0,   224,     0,
     225,   226,   227,     0,     0,   228,     0,     0,   229,   230,
     231,     0,     0,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   485,   242,     0,   243,     0,   244,   245,   246,
     247,   248,     0,   249,   250,     0,     0,   251,   252,   253,
       0,     0,   254,     0,     0,     0,   255,   256,     0,     0,
     257,     0,     0,   258,   259,   260,   261,   262,   263,     0,
     264,   265,   266,     0,   267,   268,   269,   270,   271,   272,
     273,     0,   274,   275,   276,   277,   278,   279,   280,   281,
       0,   282,   283,   284,   285,   286,   287,   288,     0,   289,
     290,   769,     0,   292,   293,   294,   295,     0,   296,   297,
       0,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,     0,   308,   309,   310,     0,   487,   312,     0,   313,
       0,   488,   315,   316,   317,     0,   318,   319,   320,   321,
       0,     0,   322,   323,   324,   325,   326,     0,     0,   327,
     328,   329,   330,   331,   332,     0,   333,   334,   335,     0,
       0,   336,   337,   338,   339,   340,   341,     0,     0,     0,
       0,   135,   136,   137,   138,   139,   140,   141,     0,   142,
       0,     0,     0,     0,     0,     0,   143,   144,   145,     0,
     146,   147,   148,     0,   149,     0,   150,   151,     0,   152,
     153,   154,   155,     0,   342,   156,   157,   158,   159,     0,
     160,   161,   162,   163,   164,     0,     0,   165,   166,   167,
       0,   168,   169,   170,   171,     0,     0,   172,   173,     0,
       0,     0,     0,     0,   174,   175,   176,   177,   178,   179,
     180,   181,     0,     0,   182,   183,   184,   185,   186,     0,
       0,     0,   187,   188,   189,   190,     0,   191,   192,     0,
     193,     0,   194,   195,   196,   197,   198,   199,     0,   200,
     201,     0,   202,     0,   203,     0,     0,     0,   204,   205,
     206,     0,  1644,   207,     0,   208,     0,   209,   210,   211,
       0,   212,   213,   214,     0,     0,   215,   216,   217,   218,
     219,   220,   221,     0,   222,     0,   223,     0,     0,   224,
       0,   225,   226,   227,     0,     0,   228,     0,     0,   229,
     230,   231,     0,     0,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,     0,   243,     0,   244,   245,
     246,   247,   248,     0,   249,   250,     0,     0,   251,   252,
     253,     0,     0,   254,     0,     0,     0,   255,   256,     0,
       0,   257,     0,     0,   258,   259,   260,   261,   262,   263,
       0,   264,   265,   266,     0,   267,   268,   269,   270,   271,
     272,   273,     0,   274,   275,   276,   277,   278,   279,   280,
     281,     0,   282,   283,   284,   285,   286,   287,   288,     0,
     289,   290,   291,     0,   292,   293,   294,   295,     0,   296,
     297,     0,   298,   299,   300,   301,   302,   303,   304,   305,
     306,   307,     0,   308,   309,   310,     0,   311,   312,     0,
     313,     0,   314,   315,   316,   317,     0,   318,   319,   320,
     321,     0,     0,   322,   323,   324,   325,   326,     0,     0,
     327,   328,   329,   330,   331,   332,     0,   333,   334,   335,
       0,     0,   336,   337,   338,   339,   340,   341,     0,     0,
       0,     0,   135,   136,   137,   138,   139,   140,   141,     0,
     142,     0,     0,     0,     0,  2005,     0,   143,   144,   145,
       0,   146,   147,   148,     0,   149,     0,   150,   151,     0,
     152,   153,   154,   155,     0,   342,   156,   157,   158,   159,
       0,   160,   161,   162,   163,   164,     0,     0,   165,   166,
     167,     0,   168,   169,   170,   171,     0,     0,   172,   173,
       0,     0,     0,     0,     0,   174,   175,   176,   177,   178,
     179,   180,   181,     0,     0,   182,   183,   184,   185,   186,
       0,     0,     0,   187,   188,   189,   190,     0,   191,   192,
       0,   193,     0,   194,   195,   196,   197,   198,   199,     0,
     200,   201,     0,   202,     0,   203,     0,     0,     0,   204,
     205,   206,     0,     0,   207,     0,   208,     0,   209,   210,
     211,     0,   212,   213,   214,     0,     0,   215,   216,   217,
     218,   219,   220,   221,     0,   222,     0,   223,     0,     0,
     224,     0,   225,   226,   227,     0,     0,   228,     0,     0,
     229,   230,   231,     0,     0,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,     0,   243,     0,   244,
     245,   246,   247,   248,     0,   249,   250,     0,     0,   251,
     252,   253,     0,     0,   254,     0,     0,     0,   255,   256,
       0,     0,   257,     0,     0,   258,   259,   260,   261,   262,
     263,     0,   264,   265,   266,     0,   267,   268,   269,   270,
     271,   272,   273,     0,   274,   275,   276,   277,   278,   279,
     280,   281,     0,   282,   283,   284,   285,   286,   287,   288,
       0,   289,   290,   291,     0,   292,   293,   294,   295,     0,
     296,   297,     0,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,     0,   308,   309,   310,     0,   311,   312,
       0,   313,     0,   314,   315,   316,   317,     0,   318,   319,
     320,   321,     0,     0,   322,   323,   324,   325,   326,     0,
       0,   327,   328,   329,   330,   331,   332,     0,   333,   334,
     335,     0,     0,   336,   337,   338,   339,   340,   341,     0,
       0,     0,     0,   135,   136,   137,   138,   139,   140,   141,
       0,   142,     0,     0,     0,     0,  2021,     0,   143,   144,
     145,     0,   146,   147,   148,     0,   149,     0,   150,   151,
       0,   152,   153,   154,   155,     0,   342,   156,   157,   158,
     159,     0,   160,   161,   162,   163,   164,     0,     0,   165,
     166,   167,     0,   168,   169,   170,   171,     0,     0,   172,
     173,     0,     0,     0,     0,     0,   174,   175,   176,   177,
     178,   179,   180,   181,     0,     0,   182,   183,   184,   185,
     186,     0,     0,     0,   187,   188,   189,   190,     0,   191,
     192,     0,   193,     0,   194,   195,   196,   197,   198,   199,
       0,   200,   201,     0,   202,     0,   203,     0,     0,     0,
     204,   205,   206,     0,     0,   207,     0,   208,     0,   209,
     210,   211,     0,   212,   213,   214,     0,     0,   215,   216,
     217,   218,   219,   220,   221,     0,   222,     0,   223,     0,
       0,   224,     0,   225,   226,   227,     0,     0,   228,     0,
       0,   229,   230,   231,     0,     0,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,     0,   243,     0,
     244,   245,   246,   247,   248,     0,   249,   250,     0,     0,
     251,   252,   253,     0,     0,   254,     0,     0,     0,   255,
     256,     0,     0,   257,     0,     0,   258,   259,   260,   261,
     262,   263,     0,   264,   265,   266,     0,   267,   268,   269,
     270,   271,   272,   273,     0,   274,   275,   276,   277,   278,
     279,   280,   281,     0,   282,   283,   284,   285,   286,   287,
     288,     0,   289,   290,   291,     0,   292,   293,   294,   295,
       0,   296,   297,     0,   298,   299,   300,   301,   302,   303,
     304,   305,   306,   307,     0,   308,   309,   310,     0,   311,
     312,     0,   313,     0,   314,   315,   316,   317,     0,   318,
     319,   320,   321,     0,     0,   322,   323,   324,   325,   326,
       0,     0,   327,   328,   329,   330,   331,   332,     0,   333,
     334,   335,     0,     0,   336,   337,   338,   339,   340,   341,
       0,     0,     0,     0,   135,   136,   137,   138,   139,   140,
     141,     0,   142,     0,     0,     0,     0,     0,     0,   143,
     144,   145,     0,   146,   147,   148,     0,   149,     0,   150,
     151,     0,   152,   153,   154,   155,     0,   342,   156,   157,
     158,   159,     0,   160,   161,   162,   163,   164,     0,     0,
     165,   166,   167,     0,   168,   169,   170,   171,     0,     0,
     172,   173,     0,     0,     0,     0,     0,   174,   175,   176,
     177,   178,   179,   180,   181,  2077,     0,   182,   183,   184,
     185,   186,     0,     0,     0,   187,   188,   189,   190,     0,
     191,   192,     0,   193,     0,   194,   195,   196,   197,   198,
     199,     0,   200,   201,     0,   202,     0,   203,     0,     0,
       0,   204,   205,   206,     0,     0,   207,     0,   208,     0,
     209,   210,   211,     0,   212,   213,   214,     0,     0,   215,
     216,   217,   218,   219,   220,   221,     0,   222,     0,   223,
       0,     0,   224,     0,   225,   226,   227,     0,     0,   228,
       0,     0,   229,   230,   231,     0,     0,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,     0,   243,
       0,   244,   245,   246,   247,   248,     0,   249,   250,     0,
       0,   251,   252,   253,     0,     0,   254,     0,     0,     0,
     255,   256,     0,     0,   257,     0,     0,   258,   259,   260,
     261,   262,   263,     0,   264,   265,   266,     0,   267,   268,
     269,   270,   271,   272,   273,     0,   274,   275,   276,   277,
     278,   279,   280,   281,     0,   282,   283,   284,   285,   286,
     287,   288,     0,   289,   290,   291,     0,   292,   293,   294,
     295,     0,   296,   297,     0,   298,   299,   300,   301,   302,
     303,   304,   305,   306,   307,     0,   308,   309,   310,     0,
     311,   312,     0,   313,     0,   314,   315,   316,   317,     0,
     318,   319,   320,   321,     0,     0,   322,   323,   324,   325,
     326,     0,     0,   327,   328,   329,   330,   331,   332,     0,
     333,   334,   335,     0,     0,   336,   337,   338,   339,   340,
     341,     0,     0,     0,     0,   135,   136,   137,   138,   139,
     140,   141,     0,   142,     0,     0,     0,     0,     0,     0,
     143,   144,   145,     0,   146,   147,   148,     0,   149,     0,
     150,   151,     0,   152,   153,   154,   155,     0,   342,   156,
     157,   158,   159,     0,   160,   161,   162,   163,   164,     0,
       0,   165,   166,   167,     0,   168,   169,   170,   171,     0,
       0,   172,   173,     0,     0,     0,     0,     0,   174,   175,
     176,   177,   178,   179,   180,   181,  2079,     0,   182,   183,
     184,   185,   186,     0,     0,     0,   187,   188,   189,   190,
       0,   191,   192,     0,   193,     0,   194,   195,   196,   197,
     198,   199,     0,   200,   201,     0,   202,     0,   203,     0,
       0,     0,   204,   205,   206,     0,     0,   207,     0,   208,
       0,   209,   210,   211,     0,   212,   213,   214,     0,     0,
     215,   216,   217,   218,   219,   220,   221,     0,   222,     0,
     223,     0,     0,   224,     0,   225,   226,   227,     0,     0,
     228,     0,     0,   229,   230,   231,     0,     0,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,     0,
     243,     0,   244,   245,   246,   247,   248,     0,   249,   250,
       0,     0,   251,   252,   253,     0,     0,   254,     0,     0,
       0,   255,   256,     0,     0,   257,     0,     0,   258,   259,
     260,   261,   262,   263,     0,   264,   265,   266,     0,   267,
     268,   269,   270,   271,   272,   273,     0,   274,   275,   276,
     277,   278,   279,   280,   281,     0,   282,   283,   284,   285,
     286,   287,   288,     0,   289,   290,   291,     0,   292,   293,
     294,   295,     0,   296,   297,     0,   298,   299,   300,   301,
     302,   303,   304,   305,   306,   307,     0,   308,   309,   310,
       0,   311,   312,     0,   313,     0,   314,   315,   316,   317,
       0,   318,   319,   320,   321,     0,     0,   322,   323,   324,
     325,   326,     0,     0,   327,   328,   329,   330,   331,   332,
       0,   333,   334,   335,     0,     0,   336,   337,   338,   339,
     340,   341,     0,     0,     0,     0,   135,   136,   137,   138,
     139,   140,   141,     0,   142,     0,     0,     0,     0,     0,
       0,   143,   144,   145,     0,   146,   147,   148,     0,   149,
       0,   150,   151,     0,   152,   153,   154,   155,     0,   342,
     156,   157,   158,   159,     0,   160,   161,   162,   163,   164,
       0,     0,   165,   166,   167,     0,   168,   169,   170,   171,
       0,     0,   172,   173,     0,     0,     0,     0,     0,   174,
     175,   176,   177,   178,   179,   180,   181,     0,     0,   182,
     183,   184,   185,   186,     0,     0,     0,   187,   188,   189,
     190,     0,   191,   192,     0,   193,     0,   194,   195,   196,
     197,   198,   199,     0,   200,   201,     0,   202,     0,   203,
       0,     0,     0,   204,   205,   206,     0,     0,   207,     0,
     208,     0,   209,   210,   211,     0,   212,   213,   214,     0,
       0,   215,   216,   217,   218,   219,   220,   221,     0,   222,
       0,   223,     0,     0,   224,     0,   225,   226,   227,     0,
       0,   228,     0,     0,   229,   230,   231,     0,     0,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
       0,   243,     0,   244,   245,   246,   247,   248,     0,   249,
     250,     0,     0,   251,   252,   253,     0,     0,   254,     0,
       0,     0,   255,   256,     0,     0,   257,     0,     0,   258,
     259,   260,   261,   262,   263,     0,   264,   265,   266,     0,
     267,   268,   269,   270,   271,   272,   273,     0,   274,   275,
     276,   277,   278,   279,   280,   281,     0,   282,   283,   284,
     285,   286,   287,   288,     0,   289,   290,   291,     0,   292,
     293,   294,   295,     0,   296,   297,     0,   298,   299,   300,
     301,   302,   303,   304,   305,   306,   307,     0,   308,   309,
     310,     0,   311,   312,     0,   313,     0,   314,   315,   316,
     317,     0,   318,   319,   320,   321,     0,     0,   322,   323,
     324,   325,   326,     0,     0,   327,   328,   329,   330,   331,
     332,     0,   333,   334,   335,     0,     0,   336,   337,   338,
     339,   340,   341,     0,     0,     0,     0,   135,   136,   137,
     138,   139,   140,   141,     0,   142,     0,     0,     0,     0,
       0,     0,   143,   144,   145,     0,   146,   147,   148,     0,
     149,     0,   150,   151,     0,   152,   153,   154,   155,     0,
     342,   156,   157,   158,   159,     0,   160,   161,   162,   163,
     164,     0,     0,   165,   166,   167,     0,   168,   169,   170,
     171,     0,     0,   172,   173,     0,     0,     0,     0,     0,
     174,   175,   176,   177,   178,   179,   180,   181,     0,     0,
     182,   183,   184,   185,   186,     0,     0,     0,   187,   188,
     189,   190,     0,   191,   192,     0,   193,     0,   194,   195,
     196,   197,   198,   199,     0,   200,   201,     0,   202,     0,
     203,     0,     0,     0,   204,   205,   206,     0,     0,   207,
       0,   208,     0,   209,   210,   211,     0,   212,   213,   214,
       0,     0,   215,   216,   217,   218,   219,   220,   221,     0,
     222,     0,   223,     0,     0,   224,     0,   225,   226,   227,
       0,     0,   228,     0,     0,   229,   230,   231,     0,     0,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,     0,   243,     0,   244,   245,   246,   247,   248,     0,
     249,   250,     0,     0,   251,   252,   253,     0,     0,   254,
       0,     0,     0,   255,   256,     0,     0,   257,     0,     0,
     258,   259,   260,   261,   262,   263,     0,   264,   265,   381,
       0,   267,   268,   269,   270,   271,   272,   273,     0,   274,
     275,   276,   277,   278,   279,   280,   281,     0,   282,   283,
     284,   285,   286,   287,   288,     0,   289,   290,   291,     0,
     292,   293,   294,   295,     0,   296,   297,     0,   298,   299,
     300,   301,   302,   303,   304,   305,   306,   307,     0,   308,
     309,   310,     0,   311,   312,     0,   313,     0,   314,   315,
     316,   317,     0,   318,   319,   320,   321,     0,     0,   322,
     323,   324,   325,   326,     0,     0,   327,   328,   329,   330,
     331,   332,     0,   333,   334,   335,     0,     0,   336,   337,
     338,   339,   340,   341,     0,     0,     0,     0,   135,   136,
     137,   138,   139,   140,   141,     0,   142,     0,     0,     0,
       0,     0,     0,   143,   144,   145,     0,   146,   147,   148,
       0,   149,     0,   150,   151,     0,   152,   153,   154,   155,
       0,   342,   156,   157,   158,   159,     0,   160,   161,   162,
     163,   164,     0,     0,   165,   166,   167,     0,   483,   169,
     170,   171,     0,     0,   172,   173,     0,     0,     0,     0,
       0,   174,   175,   176,   177,   178,   179,   180,   181,     0,
       0,   182,   183,   184,   185,   186,     0,     0,     0,   187,
     188,   189,   190,     0,   191,   192,     0,   193,     0,   194,
     195,   196,   197,   198,   199,     0,   200,   201,     0,   202,
       0,   203,     0,     0,     0,   204,   205,   206,     0,     0,
     207,     0,   208,     0,   209,   210,   211,     0,   212,   213,
     214,     0,     0,   215,   216,   217,   218,   219,   220,   221,
       0,   222,     0,   223,     0,     0,   224,     0,   225,   226,
     227,     0,     0,   228,     0,     0,   229,   230,   484,     0,
       0,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     485,   242,     0,   243,     0,   244,   245,   246,   247,   248,
       0,   249,   250,     0,     0,   251,   252,   253,     0,     0,
     254,     0,     0,     0,   255,   256,     0,     0,   257,     0,
       0,   258,   259,   260,   261,   262,   263,     0,   264,   265,
     266,     0,   267,   268,   269,   270,   271,   272,   273,     0,
     274,   275,   276,   277,   278,   279,   280,   281,     0,   282,
     283,   284,   285,   286,   287,   288,     0,   289,   290,   486,
       0,   292,   293,   294,   295,     0,   296,   297,     0,   298,
     299,   300,   301,   302,   303,   304,   305,   306,   307,     0,
     308,   309,   310,     0,   487,   312,     0,   313,     0,   488,
     315,   316,   317,     0,   318,   319,   320,   321,     0,     0,
     322,   323,   324,   325,   326,     0,     0,   327,   328,   329,
     330,   331,   332,     0,   333,   334,   335,     0,     0,   336,
     337,   338,   339,   340,   341,     0,     0,     0,     0,   135,
     136,   137,   138,   139,   140,   141,     0,   142,     0,     0,
       0,     0,     0,     0,   143,   144,   145,     0,   146,   147,
     148,     0,   149,     0,   150,   151,     0,   152,   153,   154,
     155,     0,   342,   156,   157,   158,   159,     0,   160,   161,
     162,   163,   164,     0,     0,   165,   166,   167,     0,   168,
     169,   170,   171,     0,     0,   172,   173,     0,     0,     0,
       0,     0,   174,   175,   176,   177,   178,   179,   180,   181,
       0,     0,   182,   183,   184,   185,   186,     0,     0,     0,
     187,   188,   189,   190,     0,   191,   192,     0,   193,     0,
     194,   195,   196,   197,   198,   199,     0,   200,   201,     0,
     202,     0,   203,     0,     0,     0,   204,   205,   206,     0,
       0,   207,     0,   208,     0,   209,   210,   211,     0,   212,
     213,   214,     0,     0,   215,   216,   217,   218,   219,   220,
     221,     0,   222,     0,   223,     0,     0,   224,     0,   225,
     226,   227,     0,     0,   228,     0,     0,   229,   230,   231,
       0,     0,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   485,   242,     0,   243,     0,   244,   245,   246,   247,
     248,     0,   249,   250,     0,     0,   251,   252,   253,     0,
       0,   254,     0,     0,     0,   255,   256,     0,     0,   257,
       0,     0,   258,   259,   260,   261,   262,   263,     0,   264,
     265,   266,     0,   267,   268,   269,   270,   271,   272,   273,
       0,   274,   275,   276,   277,   278,   279,   280,   281,     0,
     282,   283,   284,   285,   286,   287,   288,     0,   289,   290,
     769,     0,   292,   293,   294,   295,     0,   296,   297,     0,
     298,   299,   300,   301,   302,   303,   304,   305,   306,   307,
       0,   308,   309,   310,     0,   487,   312,     0,   313,     0,
     488,   315,   316,   317,     0,   318,   319,   320,   321,     0,
       0,   322,   323,   324,   325,   326,     0,     0,   327,   328,
     329,   330,   331,   332,     0,   333,   334,   335,     0,     0,
     336,   337,   338,   339,   340,   341,     0,     0,     0,     0,
     135,   136,   137,   138,   139,   140,   141,     0,   142,     0,
       0,     0,     0,     0,     0,   143,   144,   145,     0,   146,
     147,   148,     0,   845,     0,   846,   847,     0,   152,   153,
     154,   155,     0,   342,   156,   848,   849,   159,     0,   160,
     161,   162,   163,     0,     0,     0,   165,   166,   167,     0,
     168,   169,     0,   171,     0,     0,   172,   173,     0,     0,
       0,     0,     0,   174,   175,   176,   177,   178,   850,   851,
     181,     0,     0,   182,   183,   184,   185,   186,     0,     0,
       0,   187,   691,   189,   190,     0,   191,   192,     0,   193,
       0,   194,   195,     0,   197,   198,     0,     0,   200,   852,
       0,   202,     0,   203,     0,     0,     0,   204,   205,   206,
       0,     0,   207,     0,   208,     0,   209,   210,   211,     0,
     212,   213,   214,     0,     0,   215,   216,   217,   218,   219,
     853,   854,     0,   947,     0,   223,     0,     0,   224,     0,
     225,   226,   227,     0,     0,   228,     0,     0,   229,   230,
     231,     0,     0,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   701,     0,   855,     0,   244,   245,   246,
     247,  1310,     0,   249,   250,     0,     0,     0,   856,   253,
       0,     0,   254,     0,     0,     0,   255,   256,     0,     0,
     257,     0,     0,     0,   259,   260,   261,   262,   263,     0,
       0,   265,   266,     0,   267,   268,   269,   270,   271,   857,
     273,     0,   274,   275,   276,   277,   278,   279,   280,   281,
       0,   282,     0,   284,   285,   286,   287,   288,     0,   289,
     290,   291,     0,   292,   948,   294,   295,     0,   296,   858,
       0,   298,   299,   300,   301,   302,   303,   304,   305,     0,
     307,     0,   308,   309,   310,     0,   859,   860,     0,   313,
       0,   314,     0,   316,     0,     0,   318,   319,   320,   321,
       0,     0,   322,   323,   324,   325,   326,     0,     0,   327,
     328,   329,   330,   861,   332,     0,   333,   334,   335,     0,
       0,   336,   337,   338,   339,   340,   341,     0,     0,     0,
       0,   135,   136,   137,   138,   139,   140,   141,     0,   142,
       0,     0,     0,     0,     0,     0,   143,   144,   145,     0,
     146,   147,   148,     0,   845,     0,   846,   847,     0,   152,
     153,   154,   155,     0,   862,   156,   848,   849,   159,     0,
     160,   161,   162,   163,     0,     0,     0,   165,   166,   167,
       0,   168,   169,     0,   171,     0,     0,   172,   173,     0,
       0,     0,     0,     0,   174,   175,   176,   177,   178,   850,
     851,   181,     0,     0,   182,   183,   184,   185,   186,     0,
       0,     0,   187,   691,   189,   190,     0,   191,   192,     0,
     193,     0,   194,   195,     0,   197,   198,     0,     0,   200,
     852,     0,   202,     0,   203,     0,     0,     0,   204,   205,
     206,     0,     0,   207,     0,   208,     0,   209,   210,   211,
    1298,   212,   213,   214,     0,     0,  1299,   216,   217,   218,
     219,   853,   854,     0,   947,     0,   223,     0,     0,   224,
       0,   225,   226,   227,     0,     0,   228,     0,     0,   229,
     230,   231,     0,     0,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   701,     0,   855,     0,   244,   245,
     246,   247,     0,     0,   249,   250,     0,     0,     0,   856,
     253,     0,     0,   254,     0,     0,     0,   255,   256,     0,
       0,  1300,     0,     0,     0,   259,   260,   261,   262,   263,
       0,     0,   265,   266,     0,   267,   268,   269,   270,   271,
     857,   273,     0,   274,   275,   276,   277,   278,   279,   280,
     281,     0,   282,     0,   284,   285,   286,   287,   288,     0,
     289,   290,   291,     0,   292,   948,   294,   295,     0,   296,
     858,     0,   298,   299,   300,   301,   302,   303,   304,   305,
       0,   307,     0,   308,   309,   310,     0,   859,   860,     0,
     313,     0,   314,     0,   316,     0,     0,   318,   319,   320,
     321,     0,     0,   322,   323,   324,   325,   326,     0,     0,
     327,   328,   329,   330,   861,   332,     0,   333,   334,   335,
       0,     0,   336,   337,   338,   339,   340,   341,     0,     0,
       0,     0,   135,   136,   137,   138,   139,   140,   141,     0,
     142,     0,     0,     0,     0,     0,     0,   143,   144,   145,
       0,   146,   147,   148,     0,   845,     0,   846,   847,     0,
     152,   153,   154,   155,     0,   862,   156,   848,   849,   159,
       0,   160,   161,   162,   163,     0,     0,     0,   165,   166,
     167,     0,   168,   169,     0,   171,     0,     0,   172,   173,
       0,     0,     0,     0,     0,   174,   175,   176,   177,   178,
     850,   851,   181,     0,     0,   182,   183,   184,   185,   186,
       0,     0,     0,   187,   691,   189,   190,     0,   191,   192,
       0,   193,     0,   194,   195,     0,   197,   198,     0,     0,
     200,   852,     0,   202,     0,   203,     0,     0,     0,   204,
     205,   206,     0,     0,   207,     0,   208,     0,   209,   210,
     211,     0,   212,   213,   214,     0,     0,   215,   216,   217,
     218,   219,   853,   854,     0,   947,     0,   223,     0,     0,
     224,     0,   225,   226,   227,     0,     0,   228,     0,     0,
     229,   230,   231,     0,     0,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   701,     0,   855,     0,   244,
     245,   246,   247,  1910,     0,   249,   250,     0,     0,     0,
     856,   253,     0,     0,   254,     0,     0,     0,   255,   256,
       0,     0,   257,     0,     0,     0,   259,   260,   261,   262,
     263,     0,     0,   265,   266,     0,   267,   268,   269,   270,
     271,   857,   273,     0,   274,   275,   276,   277,   278,   279,
     280,   281,     0,   282,     0,   284,   285,   286,   287,   288,
       0,   289,   290,   291,     0,   292,   948,   294,   295,     0,
     296,   858,     0,   298,   299,   300,   301,   302,   303,   304,
     305,     0,   307,     0,   308,   309,   310,     0,   859,   860,
       0,   313,     0,   314,     0,   316,     0,     0,   318,   319,
     320,   321,     0,     0,   322,   323,   324,   325,   326,     0,
       0,   327,   328,   329,   330,   861,   332,     0,   333,   334,
     335,     0,     0,   336,   337,   338,   339,   340,   341,     0,
       0,     0,     0,   135,   136,   137,   138,   139,   140,   141,
       0,   142,     0,     0,     0,     0,     0,     0,   143,   144,
     145,     0,   146,   147,   148,     0,   845,     0,   846,   847,
       0,   152,   153,   154,   155,     0,   862,   156,   848,   849,
     159,     0,   160,   161,   162,   163,     0,     0,     0,   165,
     166,   167,     0,   168,   169,     0,   171,     0,     0,   172,
     173,     0,     0,     0,     0,     0,   174,   175,   176,   177,
     178,   850,   851,   181,     0,     0,   182,   183,   184,   185,
     186,     0,     0,     0,   187,   691,   189,   190,     0,   191,
     192,     0,   193,     0,   194,   195,     0,   197,   198,     0,
       0,   200,   852,     0,   202,     0,   203,     0,     0,     0,
     204,   205,   206,     0,     0,   207,     0,   208,     0,   209,
     210,   211,     0,   212,   213,   214,     0,     0,   215,   216,
     217,   218,   219,   853,   854,     0,   947,     0,   223,     0,
       0,   224,     0,   225,   226,   227,     0,     0,   228,     0,
       0,   229,   230,   231,     0,     0,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   701,     0,   855,     0,
     244,   245,   246,   247,     0,     0,   249,   250,     0,     0,
       0,   856,   253,     0,     0,   254,     0,     0,     0,   255,
     256,     0,     0,   257,     0,     0,     0,   259,   260,   261,
     262,   263,     0,     0,   265,   266,     0,   267,   268,   269,
     270,   271,   857,   273,     0,   274,   275,   276,   277,   278,
     279,   280,   281,     0,   282,     0,   284,   285,   286,   287,
     288,     0,   289,   290,   291,     0,   292,   948,   294,   295,
       0,   296,   858,     0,   298,   299,   300,   301,   302,   303,
     304,   305,     0,   307,     0,   308,   309,   310,     0,   859,
     860,     0,   313,     0,   314,     0,   316,     0,     0,   318,
     319,   320,   321,     0,     0,   322,   323,   324,   325,   326,
       0,     0,   327,   328,   329,   330,   861,   332,     0,   333,
     334,   335,     0,     0,   336,   337,   338,   339,   340,   341,
       0,     0,     0,     0,   135,   136,   137,   138,   139,   140,
     141,     0,   142,     0,     0,     0,     0,     0,     0,   143,
     144,   145,     0,   146,   147,   148,     0,   845,     0,   846,
     847,     0,   152,   153,   154,   155,     0,   862,   156,   848,
     849,   159,     0,   160,   161,   162,   163,     0,     0,     0,
     165,   166,   167,     0,   168,   169,     0,   171,     0,     0,
     172,   173,     0,     0,     0,     0,     0,   174,   175,   176,
     177,   178,   850,   851,   181,     0,     0,   182,   183,   184,
     185,   186,     0,     0,     0,   187,   691,   189,   190,     0,
     191,   192,     0,   193,     0,   194,   195,     0,   197,   198,
       0,     0,   200,   852,     0,   202,     0,   203,     0,     0,
       0,   204,   205,   206,     0,     0,   207,     0,   208,     0,
     209,   210,   211,     0,   212,   213,   214,     0,     0,   215,
     216,   217,   218,   219,   853,   854,     0,   947,     0,   223,
       0,     0,   224,     0,   225,   226,   227,     0,     0,   228,
       0,     0,   229,   230,   231,     0,     0,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   701,     0,   855,
       0,   244,   245,   246,   247,     0,     0,   249,   250,     0,
       0,     0,   856,   253,     0,     0,   254,     0,     0,     0,
     255,   256,     0,     0,   257,     0,     0,     0,   259,   260,
     261,   262,   263,     0,     0,   265,   266,     0,   267,   268,
     269,   270,   271,   857,   273,     0,   274,   275,   276,   277,
     278,   279,   280,   281,     0,   282,     0,   284,   285,   286,
     287,   288,     0,   289,   290,   291,     0,   292,     0,   294,
     295,     0,   296,   858,     0,   298,   299,   300,   301,   302,
     303,   304,   305,     0,   307,     0,   308,   309,   310,     0,
     859,   860,     0,   313,     0,   314,     0,   316,     0,     0,
     318,   319,   320,   321,     0,     0,   322,   323,   324,   325,
     326,     0,     0,   327,   328,   329,   330,   861,   332,     0,
     333,   334,   335,     0,     0,   336,   337,   338,   339,   340,
     341,     0,     0,     0,     0,   135,   136,   137,   138,   139,
     140,   141,     0,   142,     0,     0,     0,     0,     0,     0,
     143,   144,   145,     0,   146,   147,   148,     0,   845,     0,
     846,   847,     0,   152,   153,   154,   155,     0,   862,   156,
     848,   849,   159,     0,   160,   161,   162,   163,     0,     0,
       0,   165,   166,   167,     0,   168,   169,     0,   171,     0,
       0,   172,   173,     0,     0,     0,     0,     0,   174,   175,
     176,   177,   178,   850,   851,   181,     0,     0,   182,   183,
     184,   185,   186,     0,     0,     0,   187,   691,   189,   190,
       0,   191,   192,     0,   193,     0,   194,   195,     0,   197,
     198,     0,     0,   200,   852,     0,   202,     0,   203,     0,
       0,     0,   204,   205,   206,     0,     0,   207,     0,   208,
       0,   209,   210,   211,     0,   212,   213,   214,     0,     0,
     215,   216,   217,   218,   219,   853,   854,     0,     0,     0,
     223,     0,     0,   224,     0,   225,   226,   227,     0,     0,
     228,     0,     0,   229,   230,   231,     0,     0,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   701,     0,
     855,     0,   244,   245,   246,   247,     0,     0,   249,   250,
       0,     0,     0,   856,   253,     0,     0,   254,     0,     0,
       0,   255,   256,     0,     0,   257,     0,     0,     0,   259,
     260,   261,   262,   263,     0,     0,   265,   266,     0,   267,
     268,   269,   270,   271,   857,   273,     0,   274,   275,   276,
     277,   278,   279,   280,   281,     0,   282,     0,   284,   285,
     286,   287,   288,     0,   289,   290,   291,     0,   292,     0,
     294,   295,     0,   296,   858,     0,   298,   299,   300,   301,
     302,   303,   304,   305,     0,   307,     0,   308,   309,   310,
       0,   859,   860,     0,   313,     0,   314,     0,   316,     0,
       0,   318,   319,   320,   321,     0,     0,   322,   323,   324,
     325,   326,     0,     0,   327,   328,   329,   330,   861,   332,
       0,   333,   334,   335,     0,   353,   336,   337,   338,   339,
     340,   341,     0,     0,   354,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   355,     0,     0,   356,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   357,   862,
    -400,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   358,     0,     0,     0,     0,     0,
     359,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     360,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   361,     0,
     362,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -500,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -325,     0,     0,     0,     0,     0,     0,     0,   363,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   364,     0,   365,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -325,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   366,     0,     0,     0,     0,  -243,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -243,   367,     0,   368,     0,     0,     0,     0,     0,     0,
       0,     0,   369,     0,     0,     0,   370,   371,     0,     0,
     372,     0,     0,     0,     0,     0,   373,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   374,   375
};

static const short yycheck[] =
{
       0,   580,    42,     7,     8,   364,   395,   461,   892,    13,
      14,   835,   579,   599,    18,   638,    20,  1157,   585,   882,
    1028,   607,    26,   656,    28,    37,    30,  1104,   716,   129,
      34,    35,   132,  1507,   749,   755,   395,   576,   758,    24,
     777,   608,    42,   639,    13,    14,   842,  1161,    39,    18,
    1562,    20,   360,   842,   883,   875,   728,    26,   875,    28,
     483,   108,  1033,   371,  1035,  1767,   890,  1133,  1642,   875,
     482,   415,     0,  1588,     0,    16,    26,  1772,  1773,     0,
       0,    20,     0,     0,     0,    20,    10,   883,    10,   628,
      34,    15,  1897,    39,   883,    47,  1304,    58,    74,   484,
     408,   486,  2225,    39,    86,    69,   108,    26,    47,    74,
      41,    74,    47,  1992,   122,    10,    39,  1722,   690,    27,
      20,   517,    12,    13,   128,   129,    73,   131,   132,    78,
      78,    78,    58,    88,     7,    78,   137,    58,    11,   130,
      80,    68,   133,   110,    89,   184,     7,    47,   537,    78,
     108,   774,    20,   117,   681,   682,   179,   213,   261,    19,
    1528,    94,   131,    10,    74,   225,   231,    57,    10,    32,
      74,   136,   860,    15,   105,   702,    69,   182,   537,    47,
     294,    94,   131,   131,   131,    77,   108,   113,   153,  2185,
      86,  2187,   113,   353,    34,    16,   160,   124,    74,   163,
     122,    27,   131,  1695,    67,    10,    61,     7,   188,  1038,
      20,   156,   110,    86,    20,   264,    26,   302,     8,    21,
     285,   232,   130,   520,   117,    86,   386,   176,   177,   294,
     527,  1723,   392,    23,   517,    82,   197,   545,    40,   139,
     140,    47,  1038,   254,   347,    66,   138,   296,   170,  1038,
     335,   365,   197,    79,   109,   231,    14,     5,    74,   226,
     123,   121,   362,    10,   887,   229,   938,   160,    15,   179,
     163,   175,   127,   373,  2153,   206,   220,    82,   178,   851,
     588,   182,   231,    10,   231,   207,    86,   142,    15,   666,
     962,   189,   147,   393,  1014,   360,   159,  2293,   161,   643,
     360,   225,   231,   261,   360,   238,  2008,   615,   904,   285,
    2012,   256,   167,   641,  2016,   911,   339,   297,   294,   243,
     228,   848,   849,   290,   632,   238,   365,   254,   855,   278,
     278,   278,   666,   139,   298,   278,   229,   292,    75,   321,
     236,   281,   324,   274,   217,   294,    94,   294,  1953,   278,
     354,   287,   250,   179,   358,   360,   360,   365,   362,   185,
     364,  1569,   366,   365,   365,   369,   413,   371,   223,   373,
     530,   375,   337,   338,   339,   535,   339,   381,  2501,   344,
     220,   139,   245,   387,   360,   354,   390,   352,  1108,   393,
    2269,   395,   353,   397,   385,   187,   360,   401,   288,   403,
     369,   243,    75,   339,   408,   298,   802,   803,  1616,   805,
     362,   360,   381,   360,   360,   415,   339,   356,   387,   360,
      34,   365,   344,   362,   360,  2230,   430,   362,   397,   814,
     352,   189,   401,  1801,  1802,   361,   360,   360,   364,  1553,
     361,   826,   179,   364,   364,   261,   364,   364,   364,   344,
     345,   351,   352,   353,   354,   355,   356,   617,   462,   360,
     464,   430,   362,   339,   468,   992,   993,  2169,   344,    82,
    2165,    78,  2174,   160,   928,   117,  2178,   225,   428,   483,
     484,   336,   486,   225,   873,   353,   354,   355,   356,   109,
     238,  1499,   869,   462,   362,   464,   243,   100,   231,   468,
     485,   924,   175,   926,   927,   505,   425,  1331,   231,   428,
     429,   511,   462,   219,   873,  1581,   243,   517,   160,   802,
     803,   898,   805,   339,   131,    74,   903,    58,   532,   906,
     236,   347,    76,   537,   538,   869,   278,   108,   542,   248,
     249,   545,   229,   462,   463,   351,   352,   353,   354,   355,
     356,   219,   103,  2027,   358,   103,   362,   171,   866,   294,
     295,   294,   175,   532,   898,    76,   179,   339,   236,   538,
     177,   948,   906,   542,   578,   119,   179,   344,   198,   182,
    2095,  2283,   113,   511,   588,   343,   390,   229,   294,   347,
     348,   349,   359,   351,   352,   353,   354,   355,   356,   150,
     211,    77,   150,    93,   362,   913,   220,    82,   119,   578,
     999,   615,   616,    93,   618,    75,  2490,   427,   428,   963,
    1340,   241,   236,   103,   231,   360,   175,   360,   632,    10,
     339,    69,    68,   184,   638,  1307,   184,   360,   287,  2513,
     999,   645,   970,   643,    16,   135,   650,   651,  2222,   618,
     978,   979,   462,   657,  1066,   135,   656,   150,   662,  1292,
     664,    33,   138,   294,   295,   268,   126,   278,   139,  1841,
     150,   278,   275,    14,    27,  1003,   645,    58,   114,   117,
    2554,   650,   651,  1011,    76,  1062,   253,   294,  1111,    25,
     339,   184,   692,   662,   734,   664,  2490,    78,   247,  2211,
     175,   250,   192,  1875,   184,  1244,  1820,   182,   339,  1823,
    1824,   360,   192,   280,    95,   175,   187,  1313,  1314,  2513,
     720,   108,   160,    41,  2490,   163,    98,   119,  1062,   360,
     649,   294,   295,   137,   734,   122,   225,   338,   338,   211,
     254,  1069,   661,   344,   344,   792,  1812,  2513,   120,   113,
     131,   736,   352,   360,     5,  1427,    74,   246,  1297,    10,
    2554,   233,   747,   112,  1584,   925,    17,  1584,  1597,   339,
     774,  1742,  1568,   145,   874,   779,   780,  1154,  1584,  1568,
    1157,    32,   118,   273,   361,   789,   122,   364,  2554,     0,
     360,   229,   777,   273,   339,   795,   339,   360,   139,   264,
    1515,  1597,   802,   803,   153,   805,   339,   339,  1597,   909,
     814,   294,   295,   149,   109,   360,   339,   360,   254,   339,
    1154,   288,   826,  1157,   108,   315,  2528,   360,   360,   255,
     266,   257,  1569,   214,   825,   315,   840,   360,   122,   108,
     360,   187,   191,     9,   280,   226,   364,  1155,   189,   221,
     231,  1159,  1160,   122,  1569,   360,   339,   175,   230,  2333,
     298,   179,   866,  1600,  2376,  1602,  1603,  1604,  1605,   873,
     874,  2345,   244,   309,   255,   236,   257,   360,   882,   251,
      48,   361,    48,   887,    52,   266,    52,   356,   206,  1001,
     255,   339,   257,   362,   934,  1272,   214,   278,   279,   337,
      41,   339,    68,   139,   240,   909,   255,    48,   257,   913,
     347,    52,   360,   109,    38,    39,   344,   921,    84,   291,
     924,   358,   926,   927,   352,   344,   345,  1304,  1040,   361,
     934,   127,  1940,   365,   934,  1531,  1532,   359,  1272,   943,
    2054,   290,  2056,   365,   110,  1417,   142,  1419,  1420,   359,
     266,   147,   921,  2030,    16,   365,   274,  1265,   124,   353,
     354,   355,   356,   963,   105,   294,   295,   777,   362,   779,
     780,    33,   294,   295,   943,  1352,  1984,   322,   323,  1612,
     325,   326,   327,   218,   358,  1618,  1572,   361,   139,   363,
     308,   365,   361,   361,   361,   999,   365,   365,   365,   361,
     810,   811,   343,   365,   209,   361,   347,   348,   349,   365,
     351,   352,   353,   354,   355,   356,    54,  1737,  1352,  1652,
     124,   362,   361,   189,  1887,  1621,   365,   223,    61,  1029,
     361,  1627,   360,   364,  1630,  1035,    98,  1041,    23,  1727,
    1728,   360,    27,  1043,   316,   317,   318,   358,  1148,  1438,
     361,   139,   363,    38,   365,   361,    41,   344,   120,   365,
     226,   227,   208,   361,   361,   206,   232,   365,   365,  1073,
     361,   361,   145,   361,   365,   365,   109,   365,   187,  1438,
     361,   361,  1471,   145,   365,   365,   294,   295,   254,  1417,
    1418,  1419,  1420,   361,   127,  1518,   255,   365,   257,   361,
     266,   189,  1087,   365,  1073,   271,   361,  1111,   365,   142,
     365,   255,  1471,   257,   147,   351,   352,   353,   354,   355,
     356,   361,    59,    60,   290,   365,   362,  1455,   514,  1133,
     516,   361,  1136,   274,   167,   365,  1520,  1521,  1142,  1143,
    1144,  1145,  1146,   122,  1148,   130,   361,   363,  1152,  1153,
     365,  1155,    21,    90,   187,  1159,  1160,  1152,  1153,   221,
     139,  1489,  1490,   820,   821,   822,   360,  1136,   230,   299,
     980,   981,   982,  1158,   141,  1144,  1161,  1146,   988,   361,
     990,   991,   244,   365,   361,   122,   190,   997,   365,   251,
     223,   361,  1569,   187,  2202,   365,  1006,  2013,  1008,  2015,
     351,   352,   353,   354,   355,   356,  2090,   299,   361,   361,
     189,   362,   365,   365,  1591,  1025,   361,  1027,   361,   141,
     365,  1598,   365,    58,    82,    21,   361,   361,   299,   291,
     365,   365,   294,   361,  1611,   172,   173,   365,   141,  1616,
     361,   361,   100,    78,   365,   365,   361,  1624,   281,   361,
     365,   141,  1629,   365,  1631,   343,   236,  1591,   353,  1263,
      95,  1265,   199,   351,   352,   353,   354,   355,   356,  1902,
     107,   361,  1082,    41,   362,   365,  1280,  1611,   361,    47,
     361,    16,   365,   361,    52,  1397,  1398,  1399,  1400,    31,
    1402,   187,  1292,   666,   360,  1629,   131,  1631,  1283,   360,
    1677,   360,   360,   336,    41,  1309,    74,    75,  1904,   361,
     361,  1323,  1324,   365,   365,   361,   253,   175,    16,   365,
     294,   179,   363,   363,   182,    21,    21,  1487,  1575,  1576,
    1577,  1335,   360,   360,   145,   272,   228,   293,  1715,   304,
     305,   306,   307,  1677,   360,   360,   283,   360,   187,   187,
     137,   365,  1729,   259,  1466,    16,   104,   148,   126,   103,
     187,  1401,   360,   339,   343,   360,   339,   352,   347,   348,
     349,  1679,   351,   352,   353,   354,   355,   356,   360,   214,
    1757,  1715,   360,   362,   204,   360,   339,   339,   360,   374,
     360,   226,   360,  2533,   360,  1729,   231,   360,   360,   360,
     360,  1401,   360,    14,   777,  1445,  1734,   175,   339,    20,
     268,   179,   360,   339,    25,  1455,   360,   275,   360,   360,
     255,   360,   257,  1757,   360,  1429,    16,   365,   148,   148,
     148,   266,   360,   135,  1438,   103,    47,  1437,   206,   280,
      48,   262,   261,   278,   279,  1445,   214,   310,  1448,   310,
     360,   360,   139,   187,   187,  1455,   360,   287,   294,    23,
     360,   360,   360,    27,   449,    16,     8,  1471,  1468,    74,
     294,   360,   103,   187,    38,    39,   461,    41,    16,   103,
      16,   281,  1642,   261,   365,  1485,   471,  1487,   285,  1866,
    2374,   139,   360,  2060,  2117,   360,   869,    31,   292,  1503,
     259,   259,  1506,   358,   361,   365,   274,   118,   236,   122,
     360,   122,   497,   259,  1518,   500,   360,   360,   261,   108,
     505,   361,   188,   264,  1528,   898,   182,   512,   139,   140,
     903,   365,  1866,   906,   365,   150,   143,   360,   149,   143,
     308,   189,   360,   254,   529,   217,   139,   185,   347,   111,
    1360,   103,   187,   361,   191,   365,  1366,   347,  1562,  1544,
     311,    16,   115,   221,   175,    16,   130,   178,  1553,   133,
      66,   162,   361,   365,   358,   948,   361,  1581,   189,   365,
     191,  1566,   365,   358,  1569,   261,   261,   261,   261,   108,
    1594,  1703,   361,   108,  1706,  1707,  1708,  1709,  1710,  1711,
    1712,  1713,  1714,   365,  1716,   361,   361,  1767,   361,  1613,
     299,   361,  1612,   299,  1599,  1992,   361,   108,  1618,   179,
     299,   361,   361,  1635,  1636,  1637,  1638,   612,   613,   240,
     361,   361,   361,   108,   619,  1747,  1748,   361,   361,   361,
    1644,   261,   141,   360,  1613,   361,   361,   329,   361,  1653,
      51,   360,  1652,   211,   360,   261,   199,   114,  1992,   261,
      20,   277,   199,   139,   294,    25,  1670,   361,   361,   654,
     139,   139,   271,   361,   187,  1679,  2262,  1662,   347,   254,
     290,   361,  2506,   271,   261,   281,   187,    47,  1498,  1062,
     360,   103,   355,  1503,   361,   343,  2024,    94,    94,   347,
     348,   349,   229,   351,   352,   353,   354,   355,   356,   163,
    1520,  1521,  1522,    91,   362,   361,   361,   108,   362,   103,
    1040,   361,   361,    39,   365,   361,   360,  1767,   361,   361,
      31,   360,   343,   328,   148,  1545,   347,   348,   349,  1851,
     351,   352,   353,   354,   355,   356,   313,   361,   359,   139,
     311,   362,   139,    47,   143,   366,  1741,   360,   103,  1569,
     191,   125,   122,   363,  1768,   363,   284,  1767,  1772,  1773,
    1580,   271,   113,   360,   360,   359,  2153,   359,   361,   139,
     140,  1154,  2159,   361,  1157,  2239,   365,   108,   352,   353,
    1600,   361,  1602,  1603,  1604,  1605,   360,  1801,  1802,   182,
    2379,  2380,   787,   142,   361,  1809,  1810,   371,  1812,   194,
     374,   360,   142,   361,   361,   360,   360,   358,   178,  2153,
     337,   385,   386,   179,   179,  2159,  1811,   360,   392,   189,
    1815,  1816,    80,    75,   126,  1820,   110,   110,  1823,  1824,
    1809,  1810,  1846,  1847,   408,  1849,  1850,   108,  2008,   281,
      87,    95,  2012,  2276,  1664,   187,  2016,  2490,   361,  1844,
    1845,   144,   360,   108,   187,   230,   179,  1871,   261,    31,
    2293,   190,   360,   142,  1684,   360,   301,  1846,  1847,   300,
    2513,  1850,  1692,  1887,  1996,   449,  1890,   360,   365,   360,
     360,   114,  2269,  1897,   360,   360,   360,   461,   360,  1272,
     214,    16,  1902,   179,   158,   210,   185,   471,   185,   320,
     360,  1896,   361,   365,   179,  2338,   187,  1921,  2295,   365,
     360,  2554,   907,   908,   365,   245,    31,    31,   139,  2352,
     666,  1304,   917,   497,   116,  2269,   500,   319,  2246,   103,
     187,   505,    16,   928,   129,   179,   359,    20,   512,   365,
     365,    74,  2112,   281,    87,   365,   941,   365,   365,   365,
     187,  2295,   187,   129,    83,   529,   530,   303,   360,   312,
     225,   535,    78,   132,   278,   361,   314,   365,   189,  1352,
     360,   545,    95,   343,   969,   358,   365,   358,   365,   365,
     365,   351,   352,   353,   354,   355,   356,   365,  2002,     6,
     139,  2005,   362,   210,   339,   339,   339,   361,  2008,  2169,
     339,   361,  2012,   339,  2174,   361,  2016,  2021,  2178,   361,
     213,   361,  1832,  1252,   588,   511,   189,  2335,   365,   881,
     361,   780,  1253,   360,  1844,   359,   361,   359,  2376,  2151,
    2044,  1503,  1594,  2371,  1142,  1595,  2031,  2083,   612,   613,
    2426,   615,   819,   617,  2464,   619,  2393,  1525,  2433,  2392,
    2238,  1890,  2222,  2440,  2043,  2108,   377,  2052,   632,  2054,
    1246,  2056,  2557,  2510,  2067,  1597,  2066,  1397,  1398,  1399,
    1400,  1568,  1402,  2478,  2558,    26,   655,  2476,  1670,   871,
     654,   969,   666,  1921,  2334,  2450,  1253,   478,  1624,  1616,
    1154,  2123,  2313,   669,  2108,  2554,  2440,  2490,  2112,  1109,
      19,  1653,  1512,  2117,   756,  1522,   813,  2476,  2020,  1106,
     812,  1767,  2284,  2283,  1109,  2014,  1871,  1002,  2113,  1064,
    1740,   505,   343,   869,   948,  1468,  2297,   348,   349,  2108,
     351,   352,   353,   354,   355,   356,  1466,  1506,  1368,  1924,
     657,   362,  2140,   616,    -1,    -1,  2533,    -1,    -1,    -1,
      -1,  2165,   898,  2167,    -1,  2325,    -1,   903,    -1,  2169,
     906,    -1,    -1,    -1,  2174,    -1,    -1,    -1,  2178,   666,
     343,  2185,    -1,  2187,   347,   348,   349,    -1,   351,   352,
     353,   354,   355,   356,    -1,  2180,  1569,    -1,    -1,  2533,
      -1,    -1,    -1,    -1,    23,    -1,    -1,  2211,    27,    -1,
      -1,    -1,   948,    -1,    -1,    -1,    -1,    -1,  1591,    38,
      39,    -1,    41,   787,    -1,    -1,  2230,  2231,    -1,    -1,
      -1,  2216,    -1,  2043,    -1,    -1,    -1,    -1,  1611,    -1,
      -1,    -1,  2246,  1616,    -1,    -1,    -1,  2251,    -1,    -1,
      -1,  1624,    -1,    -1,    -1,    -1,  1629,    -1,  1631,  2244,
      -1,   825,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  2276,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  2283,    -1,    -1,    -1,    -1,    -1,  2293,
      -1,    -1,    -1,    -1,  1279,   869,    -1,    -1,    -1,  1284,
    2285,    -1,   866,    -1,  1677,    -1,    14,    -1,    -1,    -1,
      -1,   130,    20,    -1,   133,  2475,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,   898,    -1,  1062,    -1,    -1,   903,
    2334,  2335,   906,    -1,  2338,    -1,    -1,  1322,    -1,    47,
      -1,    -1,  1715,   907,   908,    -1,    -1,    -1,  2352,   913,
      -1,    -1,    -1,   917,  2339,    -1,  1729,    -1,    -1,    -1,
      -1,   925,    -1,    -1,   928,    -1,    -1,    -1,  2528,    -1,
    2530,  2356,  2376,    -1,   948,  2360,    -1,   941,    -1,    -1,
     122,    -1,   869,  1703,  1757,    -1,  1706,  1707,  1708,  1709,
    1710,  1711,  1712,  1713,  1714,    -1,  1716,   139,  2208,  2298,
    2299,  2300,  2301,  2302,  2303,   969,    -1,    -1,    -1,    -1,
     118,   898,    -1,    -1,   122,    -1,   903,    -1,  1154,   906,
      -1,  1157,    -1,    -1,    -1,    -1,    -1,  2412,  1748,    -1,
      -1,   139,   140,   175,    -1,    -1,    -1,    -1,    -1,  2443,
      -1,   149,    -1,  2428,  2429,  2430,  2450,   189,    -1,    -1,
      -1,    -1,    -1,   195,    -1,    -1,    -1,    -1,  2443,    -1,
      -1,   948,    -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,
     178,    -1,  2476,  2513,    -1,    -1,    -1,    -1,  2288,    -1,
      -1,   189,    -1,   191,    -1,    -1,    -1,    -1,  1062,    -1,
    2490,    -1,    -1,  1866,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  2487,    -1,    -1,    -1,  2510,    -1,    -1,    -1,
      -1,    -1,    -1,  2513,    -1,    -1,    -1,  2416,  2417,  2418,
    2419,  2420,  2421,    -1,    -1,  2510,    -1,    -1,  2528,    -1,
      -1,  1851,   240,   352,   353,    -1,  1272,    -1,    -1,    -1,
      -1,   360,    -1,    -1,    -1,  1109,    -1,    -1,    -1,    -1,
      -1,    -1,   371,  2557,  2554,   374,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  2549,    -1,   385,   386,  1304,    -1,
      -1,    20,  2557,   392,    -1,  1062,    25,  2562,    -1,    -1,
    1154,    -1,    -1,  1157,    -1,    -1,    -1,    -1,    -1,   408,
      -1,  1155,    -1,    -1,    -1,  1159,  1160,    -1,    47,    -1,
      -1,   343,  1587,    -1,    -1,   347,   348,   349,    -1,   351,
     352,   353,   354,   355,   356,    -1,  1352,    -1,    -1,  1992,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  2438,  2439,
     449,    -1,    -1,    -1,    -1,   343,    -1,   666,    -1,   347,
     348,   349,   461,   351,   352,   353,   354,   355,   356,    -1,
      -1,    -1,   471,    -1,   362,  1640,  1641,   365,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1154,    -1,   118,
    1157,    -1,    -1,   122,    -1,    -1,  1996,    -1,   497,    -1,
      -1,   500,    -1,    -1,    -1,    -1,   505,  2497,  2498,    -1,
     139,   140,    -1,   512,    -1,    -1,    -1,    -1,  1272,    -1,
     149,  1265,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     529,   530,    -1,    -1,    -1,  1279,   535,    -1,    -1,    -1,
    1284,    -1,    -1,    -1,    -1,    -1,   545,    -1,    -1,   178,
    1304,    -1,    -1,    -1,    -1,    -1,    -1,  2547,    -1,    -1,
     189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1322,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   588,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1352,    -1,
    2153,    -1,    -1,    -1,    -1,  1272,  2159,    -1,    -1,    -1,
      -1,   240,    -1,   612,   613,    -1,   615,    -1,   617,    -1,
     619,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   632,    -1,    -1,    -1,  1304,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,    -1,
      -1,  2151,    -1,  1569,    20,   654,    -1,    -1,    -1,    25,
     869,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1591,    -1,  1842,    -1,     0,
      -1,    47,    -1,    -1,    -1,  1352,    -1,    -1,    -1,   898,
      -1,    -1,  1857,    -1,   903,  1611,    -1,   906,    -1,    -1,
    1616,    -1,  1867,    -1,    -1,    -1,    -1,    -1,  1624,    -1,
      -1,    -1,    -1,  1629,   343,  1631,  2269,    -1,   347,   348,
     349,    -1,   351,   352,   353,   354,   355,   356,    -1,    -1,
      -1,    -1,    -1,   362,    -1,    -1,    -1,    58,    -1,   948,
      -1,    -1,  2295,  1487,    -1,    -1,    -1,  1912,  1913,  1914,
    1915,    -1,   118,    -1,    -1,    -1,   122,    -1,    -1,    -1,
      -1,  1677,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,   139,   140,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   103,   149,    -1,    -1,    -1,   108,   787,    -1,
      -1,    -1,   113,   114,    -1,   116,    -1,    -1,   666,  1715,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,
      -1,    -1,   178,  1729,   135,  1569,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   189,    -1,   191,   825,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1591,    -1,    -1,
      -1,  1757,    -1,  1587,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1062,  1598,    -1,    -1,  1611,    -1,    -1,
      -1,  2026,  1616,   184,    -1,    -1,    -1,   866,    -1,  1613,
    1624,   192,    -1,    -1,   240,  1629,    -1,  1631,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  2440,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1640,  1641,  1642,    -1,
      -1,    -1,  1569,    -1,    -1,    -1,    -1,    -1,   907,   908,
      -1,    -1,    -1,    -1,   913,    -1,    -1,    -1,   917,    -1,
      -1,    -1,    -1,  1677,  1591,    -1,   925,    -1,    -1,   928,
      -1,    -1,    -1,   254,    -1,  1679,    -1,    -1,    -1,    -1,
      -1,    -1,   941,    -1,  1611,  1154,    -1,    -1,  1157,  1616,
    1866,    -1,   273,    -1,    -1,    -1,    -1,  1624,    -1,    -1,
     281,  1715,  1629,    -1,  1631,    -1,    -1,    -1,    -1,    -1,
     969,    -1,   293,    -1,    -1,  1729,    -1,   343,    -1,    -1,
    2533,   347,   348,   349,    -1,   351,   352,   353,   354,   355,
     356,   312,    -1,   314,   315,    -1,   362,    -1,    -1,   365,
      -1,   869,    -1,  1757,    -1,    -1,    -1,   328,   329,    -1,
    1677,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1767,    -1,    -1,    -1,    -1,    -1,    -1,
     898,    -1,    -1,    -1,    -1,   903,    -1,    -1,   906,    -1,
     361,    -1,   363,   364,    -1,    -1,    -1,    -1,  1715,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  2224,
      -1,    -1,  1729,  1272,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  2239,    -1,  1992,    -1,    -1,    -1,
     948,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1757,    -1,    -1,    -1,    -1,  1304,    -1,    -1,  1842,    -1,
    2265,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1109,    -1,  1866,  1857,    -1,    14,    -1,    -1,    -1,    -1,
      -1,    20,    -1,  1867,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,  2298,  2299,  2300,  2301,  2302,  2303,    -1,
      -1,    -1,  2307,  1352,    -1,    -1,    -1,    -1,    47,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1155,    -1,    -1,    -1,
    1159,  1160,    -1,    -1,    -1,    -1,    -1,    -1,  1912,  1913,
    1914,  1915,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1062,    -1,    -1,    -1,    -1,  1866,
      -1,    -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,     0,    -1,    -1,   118,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  2153,  1992,    -1,
     139,   140,    -1,  2159,    -1,    -1,    -1,    -1,    -1,    -1,
     149,  2416,  2417,  2418,  2419,  2420,  2421,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  2008,    -1,  1265,    -1,  2012,    -1,
      -1,    -1,  2016,    -1,    58,    -1,   175,    -1,    -1,   178,
    1279,    -1,  2026,    -1,    -1,  1284,  1154,    -1,    -1,  1157,
     189,    -1,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,
      -1,    -1,    -1,  1322,   108,  1992,    -1,    -1,    -1,   113,
     114,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   240,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   135,    -1,  2269,    -1,    -1,    -1,    -1,    -1,    -1,
    1569,    -1,    -1,    -1,  2108,    -1,   150,    -1,  2112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  2295,
      -1,    -1,  1591,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     353,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  2153,
     184,    -1,  1611,    -1,  1272,  2159,    -1,  1616,   192,    -1,
      -1,    -1,    -1,    -1,    -1,  1624,    -1,    -1,    -1,    -1,
    1629,    -1,  1631,   386,    -1,  2169,    -1,    -1,    -1,   392,
    2174,    -1,    -1,    -1,  2178,    -1,  1304,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   343,    -1,    -1,    -1,   347,   348,
     349,    -1,   351,   352,   353,   354,   355,   356,    -1,    -1,
      -1,    -1,    -1,   362,    -1,    -1,    -1,    -1,  1677,    -1,
     254,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  2222,    -1,
    2224,    -1,    -1,    -1,  1352,    14,  2153,    16,  1487,   273,
      -1,    20,  2159,    -1,    -1,  2239,    25,   281,    -1,    -1,
      -1,    -1,  2246,    -1,    -1,    -1,  1715,  2251,    -1,   293,
      -1,    -1,    -1,    -1,    -1,  2269,    -1,    -1,    47,   482,
    1729,  2265,    -1,    -1,  2440,    -1,    -1,    -1,   312,    -1,
     314,   315,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  2283,
      -1,  2295,    -1,    -1,   328,   329,    -1,    -1,  1757,    -1,
      -1,    -1,    -1,    -1,  2298,  2299,  2300,  2301,  2302,  2303,
      -1,    -1,    -1,  2307,    -1,    -1,    -1,   530,    -1,    20,
      -1,    -1,   535,    -1,    25,    -1,    -1,   361,    -1,   363,
     364,  2325,    -1,    -1,    -1,    -1,    -1,    -1,  1587,   118,
      -1,  2335,    -1,   122,     9,    -1,    47,    -1,    -1,  1598,
      -1,    -1,  2269,    18,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   576,  1613,    -1,    -1,  2533,    -1,    -1,
     149,    36,    -1,    -1,    -1,    -1,    -1,    -1,  2295,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,
      -1,  1640,  1641,  1642,    -1,    -1,   175,    -1,    -1,   178,
      -1,    -1,    -1,    68,   617,    -1,    -1,  1866,    -1,    -1,
     189,    -1,   191,    -1,    -1,   628,    -1,   118,    -1,    84,
      -1,   122,  2416,  2417,  2418,  2419,  2420,  2421,   641,    -1,
    1679,    -1,    -1,    -1,    -1,    -1,  2440,    -1,   139,   140,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,   149,   114,
      -1,  1569,    -1,   666,    -1,    -1,  2450,    -1,    -1,   124,
      -1,   240,    -1,    -1,    -1,    -1,   679,    -1,    -1,    -1,
      -1,    -1,    -1,  1591,   175,    -1,   666,   178,    -1,    -1,
      -1,  2475,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,
     703,    -1,    -1,  1611,    -1,    -1,    -1,    -1,  1616,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1624,    -1,    -1,    -1,
      -1,  1629,    -1,  1631,    -1,    -1,   729,   730,  1767,   732,
     733,   734,    -1,  2440,   189,    -1,    -1,    -1,    -1,  2533,
      -1,    -1,    -1,  1992,  2528,    -1,  2530,   750,    -1,   240,
      -1,    -1,    -1,    -1,   209,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1677,
      -1,   226,   227,    -1,   343,    -1,    -1,   232,   347,   348,
     349,    -1,   351,   352,   353,   354,   355,   356,    -1,    -1,
      -1,    -1,    -1,   362,    -1,    -1,    -1,    -1,    -1,   254,
      -1,   804,    -1,  1842,    -1,    -1,    -1,  1715,    -1,    -1,
      -1,   266,    -1,    -1,    -1,    -1,   271,    -1,  1857,    -1,
      -1,  1729,    -1,    -1,    -1,   280,  2533,    -1,  1867,    -1,
      -1,    -1,    -1,    -1,    -1,   290,    -1,    -1,    -1,   842,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1757,
      -1,    -1,   343,    -1,   309,   310,   347,   348,   349,   666,
     351,   352,   353,   354,   355,   356,   869,    -1,    -1,    -1,
      -1,   362,    -1,  1912,  1913,  1914,  1915,    -1,    -1,    -1,
     883,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   869,
      -1,    -1,    -1,   896,    -1,   898,    -1,    -1,    -1,    -1,
     903,    -1,    -1,   906,  2153,    -1,    -1,    -1,    -1,    -1,
    2159,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   898,    -1,
      -1,    -1,   925,   903,    -1,    -1,   906,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   948,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1866,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   970,   948,  2008,
      -1,    -1,   975,  2012,    -1,   978,   979,  2016,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  2026,    -1,    -1,
      -1,    -1,    -1,   996,    -1,    -1,    -1,  1000,  1001,  1002,
    1003,    -1,    -1,    14,    -1,    -1,    17,  1010,  1011,    20,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
    2269,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1037,  1038,    47,  1040,    -1,  1042,
      -1,    -1,    -1,  1046,    -1,    -1,  2295,  1050,    -1,    -1,
    1053,  1054,  1055,  1056,  1057,  1058,  1059,  1060,  1061,  1062,
    1063,    -1,   869,  1066,    -1,    -1,  1069,    -1,    -1,  2108,
      81,    -1,    -1,  2112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1062,    -1,  1992,    -1,    -1,    -1,    -1,    -1,
      -1,   898,    -1,    -1,    -1,    -1,   903,    -1,    -1,   906,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
    2169,    -1,    -1,    -1,    -1,  2174,    -1,    -1,   149,  2178,
      -1,   948,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1154,    -1,    -1,  1157,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,    -1,    -1,   178,    -1,    -1,
      -1,    -1,    -1,    -1,  1154,    -1,    -1,  1157,   189,    -1,
     191,    -1,    -1,  2222,    -1,  2224,    -1,    -1,    -1,    -1,
      -1,  2440,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    2239,    -1,    -1,    -1,    -1,    -1,    -1,  2246,    -1,    -1,
      -1,    -1,  2251,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  2265,    -1,    -1,   240,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1243,  1244,    -1,    -1,  2283,  2153,    -1,    -1,    -1,    -1,
      -1,  2159,    -1,    -1,    -1,  1062,    -1,    -1,    -1,  2298,
    2299,  2300,  2301,  2302,  2303,    -1,    -1,    -1,  2307,  1272,
     281,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   482,    -1,
      -1,    -1,    -1,    -1,  2533,    -1,  2325,    -1,    -1,    -1,
      -1,    -1,  1272,    -1,  1297,    -1,  2335,    -1,    -1,    -1,
      -1,  1304,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1319,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1304,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   343,    -1,  1337,    -1,   347,   348,   349,    -1,
     351,   352,   353,   354,   355,   356,    -1,  1154,    -1,  1352,
    1157,   362,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  2269,    -1,    -1,  1367,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1352,    -1,    -1,    -1,    -1,  2416,  2417,  2418,
    2419,  2420,  2421,    -1,    -1,    -1,    -1,  2295,    -1,    -1,
      -1,    -1,    -1,    -1,  1397,  1398,  1399,  1400,  1401,  1402,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  2450,    -1,    -1,  1417,  1418,  1419,  1420,    -1,    -1,
      -1,  1424,    -1,    -1,    -1,  1428,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  2475,  1440,    -1,    -1,
      -1,    -1,  1445,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1455,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1466,  1467,  1272,  1469,    -1,    -1,    -1,
    1473,    -1,    -1,    -1,    -1,   679,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1487,    -1,  1489,  1490,    -1,  2528,
      -1,  2530,    -1,    -1,    -1,    -1,    -1,  1304,    -1,   703,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1512,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1524,    -1,    -1,    -1,   729,   730,    -1,   732,   733,
     734,    -1,  2440,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1352,   750,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    14,  1568,  1569,    -1,    -1,    -1,
      20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1591,  1569,
      -1,    -1,    -1,    -1,  1597,  1598,    -1,    47,    -1,    -1,
     804,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1611,    -1,
      -1,  1591,    -1,  1616,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1624,    -1,    -1,    -1,  2533,  1629,    -1,  1631,    -1,
      -1,  1611,    -1,    -1,    -1,    -1,  1616,    -1,    -1,  1642,
      -1,    -1,    -1,    -1,  1624,    -1,    -1,    -1,    -1,  1629,
      -1,  1631,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,
      -1,  1674,   122,    -1,  1677,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1686,    -1,    -1,    -1,  1690,    -1,   139,
     140,  1694,   896,    -1,    -1,    -1,    -1,  1677,    -1,   149,
    1703,    -1,    -1,  1706,  1707,  1708,  1709,  1710,  1711,  1712,
    1713,  1714,  1715,  1716,    -1,    -1,    -1,  1720,  1721,    -1,
      -1,    -1,    -1,    -1,    -1,   175,  1729,    -1,   178,    -1,
      -1,  1734,    -1,    -1,    -1,  1715,    -1,    -1,    -1,   189,
      -1,   191,    -1,    -1,  1747,  1748,  1749,    -1,  1751,  1729,
      -1,    -1,    -1,    -1,  1757,  1758,    -1,    -1,    -1,    -1,
    1763,    -1,  1569,    -1,  1767,    -1,    -1,    -1,    -1,    -1,
      -1,   975,    -1,    -1,    -1,   979,    -1,  1757,    -1,    -1,
      -1,    -1,    -1,    -1,  1591,    -1,    -1,    -1,  1791,    -1,
     240,    -1,   996,    -1,    -1,    -1,  1000,    -1,  1002,  1003,
      -1,    -1,    -1,    -1,  1611,    -1,  1010,  1011,    -1,  1616,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1624,    -1,    -1,
      -1,    -1,  1629,    -1,  1631,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1037,    -1,    -1,    -1,    -1,  1042,    -1,
      -1,    -1,  1046,    -1,    -1,    -1,  1050,    -1,  1851,  1053,
    1054,  1055,  1056,  1057,  1058,  1059,  1060,  1061,    -1,  1063,
      -1,    -1,  1066,  1866,    -1,    -1,    -1,    -1,    -1,    -1,
    1677,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1866,    -1,    -1,    -1,
      -1,    -1,    -1,   343,    -1,    -1,    -1,   347,   348,   349,
      -1,   351,   352,   353,   354,   355,   356,    -1,  1715,    -1,
      -1,   361,   362,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      14,  1924,  1729,    -1,    -1,    -1,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,   751,    -1,    -1,
    1943,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1757,    -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1981,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1992,
      -1,    -1,    -1,  1996,  1997,  1998,    -1,    -1,  2001,    -1,
      -1,    -1,    -1,    -1,    -1,  2008,    -1,    -1,    -1,  2012,
      -1,    -1,  1992,  2016,    -1,    -1,    14,    -1,    16,    -1,
      -1,  2024,    20,    -1,   118,    -1,    -1,    25,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  2041,  1243,
      -1,    -1,    -1,    -1,    -1,   139,   140,    -1,    -1,    47,
      -1,    -1,    -1,    -1,    -1,   149,    -1,    14,    -1,  1866,
      -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  2081,    -1,
      -1,   175,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,
      47,    -1,    -1,    -1,    -1,   189,    -1,   191,    -1,    -1,
      -1,   915,    -1,    -1,    -1,    -1,    -1,    -1,   202,  2112,
      -1,    -1,    -1,    -1,    -1,  1319,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1337,    -1,    -1,    -1,  2140,    -1,    -1,
      -1,   139,   140,    -1,    -1,    -1,   240,    -1,  2151,    -1,
    2153,   149,    -1,    -1,    -1,    -1,  2159,    -1,    -1,   973,
    2163,   118,    -1,  1367,    -1,   122,  2169,    -1,    -1,    -1,
      -1,  2174,    -1,  2153,    -1,  2178,    -1,   175,    -1,  2159,
     178,   995,   139,   140,    -1,  1992,    -1,  1001,    -1,    -1,
      -1,   189,   149,   191,    -1,    -1,    -1,  1401,    -1,    -1,
      -1,  1015,  1016,  1017,  1018,    -1,    -1,  1021,    -1,    -1,
      -1,    -1,    -1,  1417,    -1,  1419,  1420,    -1,   175,  2222,
    1424,   178,  1036,    -1,  1428,    -1,  1040,    -1,    -1,    -1,
      -1,    -1,   189,    -1,   191,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   240,    -1,    -1,    -1,    -1,    -1,    -1,   343,
      -1,    -1,    -1,   347,   348,   349,    -1,   351,   352,   353,
     354,   355,   356,  1467,    -1,  1469,  2269,    -1,   362,  1473,
      -1,    -1,    -1,    -1,    -1,    -1,  2279,    -1,    -1,    -1,
    2283,    -1,    -1,   240,    -1,    -1,    -1,    -1,    -1,  2269,
      -1,    -1,  2295,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1117,    -1,    -1,    -1,    -1,  1512,    -1,
      -1,    -1,    -1,    -1,    -1,  2295,    -1,    -1,    -1,    -1,
    1524,    -1,  2325,    -1,   281,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   343,  2153,    -1,    -1,   347,
     348,   349,  2159,   351,   352,   353,   354,   355,   356,    -1,
      -1,    -1,    -1,    -1,   362,    -1,    -1,    14,  2371,  2372,
      -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   343,    -1,    -1,    -1,
     347,   348,   349,    -1,   351,   352,   353,   354,   355,   356,
      47,    -1,    -1,    -1,    -1,   362,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  2440,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,    -1,    16,
      -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,
    2440,    -1,  2269,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1674,   118,  2475,    -1,    -1,   122,    -1,    -1,    -1,  1293,
      47,    -1,  1686,    -1,    -1,    -1,  1690,    -1,  2295,    -1,
    1694,    -1,   139,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1720,  1721,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  2528,    -1,  2530,   175,    -1,
    2533,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   189,    -1,   191,  1749,    -1,  1751,    -1,    -1,
      -1,   118,    -1,  2533,  1758,   122,  1370,    -1,  1372,  1763,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,    -1,    -1,    -1,  1392,    -1,
      -1,  1395,   149,  1397,  1398,  1399,  1400,  1791,  1402,  1403,
      -1,    -1,    -1,   240,  1408,  1409,    -1,    -1,    -1,    -1,
      -1,    -1,  1416,    -1,    -1,    -1,    -1,  1421,   175,    -1,
      -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   189,    -1,   191,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  2440,    -1,  1449,    -1,    -1,  1452,    -1,
    1454,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1465,  1466,    -1,    -1,    -1,    -1,    -1,  1472,    -1,
    1474,  1475,  1476,  1477,  1478,  1479,  1480,  1481,  1482,    -1,
    1484,    -1,    -1,   240,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    20,    -1,   343,    -1,    -1,    25,
     347,   348,   349,    -1,   351,   352,   353,   354,   355,   356,
      -1,    -1,    -1,    -1,   361,   362,    -1,    -1,    -1,    -1,
    1924,    47,    -1,    -1,    -1,    -1,  2533,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1943,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1570,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   343,  1981,    -1,    -1,
     347,   348,   349,    -1,   351,   352,   353,   354,   355,   356,
      -1,    -1,   118,  1997,  1998,   362,   122,  2001,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,    -1,    -1,    -1,    -1,
    1634,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  2041,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,  1673,
      -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  2081,    -1,    -1,
      -1,    -1,    -1,  1697,  1698,  1699,  1700,  1701,  1702,  1703,
      -1,    -1,  1706,  1707,  1708,  1709,  1710,  1711,  1712,  1713,
    1714,    -1,  1716,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   240,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1736,    -1,  1738,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1748,    -1,  2140,    -1,    14,    -1,
      -1,    -1,    -1,    -1,    20,  1759,  1760,    -1,  1762,    25,
      -1,  1765,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  2163,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,
      -1,    47,    -1,    -1,    -1,    -1,    11,    12,    13,    -1,
      -1,    -1,    -1,    -1,    -1,  1799,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,    44,
      45,    -1,    -1,    -1,    49,    50,    -1,   343,    -1,    -1,
      -1,    56,    -1,    58,    -1,   351,   352,   353,   354,   355,
     356,    -1,   108,    -1,    -1,    70,   362,  1851,    73,    -1,
      -1,    -1,   118,    78,    -1,    -1,   122,    -1,    -1,    -1,
      -1,    86,    -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,
      95,    -1,    97,   139,   140,    -1,   101,    -1,    -1,    -1,
      -1,    -1,    -1,   149,    -1,  2279,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,    -1,    -1,
      -1,    -1,    -1,    20,    -1,    -1,   131,    -1,    25,   175,
      -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   189,    -1,   191,   151,   152,    -1,    -1,
      47,    -1,   157,    -1,    -1,    -1,    -1,    -1,    -1,   164,
    1944,    -1,    -1,    -1,    -1,  1949,  1950,    -1,  1952,    -1,
    1954,    -1,   177,    -1,  1958,  1959,  1960,  1961,  1962,  1963,
    1964,  1965,  1966,    -1,  1968,  1969,  1970,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   240,    -1,    -1,    -1,  2372,    -1,
     205,    -1,    -1,    -1,  1988,  1989,    -1,  1991,    -1,    -1,
     215,  1995,  1996,    -1,   219,  1999,    -1,   222,    -1,   224,
      -1,   118,    -1,    -1,    -1,   122,   231,    -1,    -1,    -1,
      -1,   236,    -1,    -1,   239,    -1,    -1,    -1,    -1,    -1,
     245,    -1,   139,   140,    -1,    -1,    -1,    -1,    -1,    -1,
    2034,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   269,    -1,    -1,    -1,    -1,    -1,
      -1,   276,    -1,   278,    -1,    -1,    -1,   282,   175,    -1,
      -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   294,
      -1,    -1,   189,    -1,   191,    -1,    -1,   343,    -1,    -1,
    2084,   347,   348,   349,    -1,   351,   352,   353,   354,   355,
     356,    -1,    -1,    -1,    -1,    -1,   362,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   240,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   360,    -1,    -1,    -1,    -1,
    2144,    -1,    -1,    -1,    -1,    -1,    -1,  2151,    -1,    -1,
      -1,  2155,    -1,    -1,    -1,    -1,  2160,  2161,  2162,    -1,
    2164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  2206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  2226,    -1,    -1,    -1,   343,    -1,    -1,    -1,
     347,   348,   349,    -1,   351,   352,   353,   354,   355,   356,
      -1,    -1,    -1,    -1,   361,   362,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  2268,    -1,    -1,    -1,    -1,    -1,
    2274,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    -1,    -1,    -1,    -1,    -1,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    -1,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    -1,    42,    43,    44,    45,    46,
      -1,    -1,    49,    50,    51,    -1,    53,    54,    55,    56,
      -1,    -1,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,  2351,    -1,    76,
      77,    78,    79,    80,    -1,    82,    -1,    84,    85,    86,
      87,    -1,    89,    90,    -1,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,   102,    -1,   104,    -1,   106,
     107,    -1,   109,   110,   111,   112,    -1,    -1,   115,    -1,
     117,   118,   119,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,   131,   132,   133,   134,    -1,   136,
      -1,   138,   139,   140,   141,   142,   143,   144,   145,  2423,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,    -1,    -1,   185,   186,
      -1,    -1,   189,   190,    -1,    -1,   193,   194,   195,   196,
     197,   198,   199,   200,   201,    -1,   203,   204,   205,    -1,
     207,   208,   209,   210,   211,   212,   213,    -1,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,    -1,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,    -1,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,    -1,   255,   256,
     257,    -1,   259,   260,    -1,   262,    -1,   264,   265,   266,
     267,   268,   269,   270,   271,   272,    -1,   274,   275,   276,
     277,   278,   279,   280,    -1,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,    -1,    -1,   294,   295,   296,
     297,   298,   299,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     337,   338,   339,    -1,   341,   342,   343,   344,   345,    -1,
      -1,    -1,    -1,    -1,   351,   352,   353,    -1,   355,   356,
      -1,    -1,    -1,   360,   361,     3,     4,     5,     6,     7,
       8,     9,    -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    -1,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    -1,    42,    43,    44,    45,    46,    -1,
      -1,    49,    50,    51,    -1,    53,    54,    55,    56,    -1,
      -1,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    -1,    -1,    76,    77,
      78,    79,    80,    -1,    -1,    -1,    84,    85,    86,    87,
      -1,    89,    90,    -1,    92,    -1,    94,    95,    96,    97,
      98,    99,   100,   101,   102,    -1,   104,    -1,   106,   107,
      -1,   109,   110,   111,   112,    -1,    -1,   115,    -1,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,   132,   133,   134,    -1,   136,    -1,
     138,   139,   140,   141,   142,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,    -1,    -1,   185,   186,    -1,
      -1,   189,   190,    -1,    -1,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    -1,   203,   204,   205,    -1,   207,
     208,   209,   210,   211,   212,   213,    -1,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,    -1,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,    -1,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,    -1,   255,   256,   257,
      -1,   259,   260,    -1,   262,    -1,   264,   265,   266,   267,
     268,   269,   270,   271,   272,    -1,   274,   275,   276,   277,
     278,   279,   280,    -1,   282,   283,   284,   285,   286,   287,
     288,   289,   290,   291,    -1,    -1,   294,   295,   296,   297,
     298,   299,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   337,
     338,   339,    -1,   341,   342,   343,   344,   345,    -1,    -1,
      -1,    -1,    -1,   351,   352,    -1,    -1,   355,   356,    -1,
      -1,    -1,   360,   361,     3,     4,     5,     6,     7,     8,
       9,    -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    -1,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    42,    43,    44,    45,    46,    -1,    -1,
      49,    50,    51,    -1,    53,    54,    55,    56,    -1,    -1,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    -1,    -1,    76,    77,    78,
      79,    80,    -1,    -1,    -1,    84,    85,    86,    87,    -1,
      89,    90,    -1,    92,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,   102,    -1,   104,    -1,   106,   107,    -1,
     109,   110,   111,   112,    -1,    -1,   115,    -1,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    -1,   127,   128,
     129,   130,   131,   132,   133,   134,    -1,   136,    -1,   138,
     139,   140,   141,   142,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,    -1,    -1,   185,   186,    -1,    -1,
     189,   190,    -1,    -1,   193,   194,   195,   196,   197,   198,
     199,   200,   201,    -1,   203,   204,   205,    -1,   207,   208,
     209,   210,   211,   212,   213,    -1,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,    -1,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,    -1,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,    -1,   255,   256,   257,    -1,
     259,   260,    -1,   262,    -1,   264,   265,   266,   267,   268,
     269,   270,   271,   272,    -1,   274,   275,   276,   277,   278,
     279,   280,    -1,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   291,    -1,    -1,   294,   295,   296,   297,   298,
     299,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   337,   338,
     339,    -1,   341,   342,   343,   344,   345,    -1,    -1,    -1,
      -1,    -1,   351,   352,    -1,    -1,   355,   356,    -1,    -1,
      -1,   360,   361,     3,     4,     5,     6,     7,     8,     9,
      -1,    11,    -1,    -1,    -1,    -1,    16,    -1,    18,    19,
      20,    -1,    22,    23,    24,    -1,    26,    -1,    28,    29,
      -1,    31,    32,    33,    34,    -1,    -1,    37,    38,    39,
      40,    -1,    42,    43,    44,    45,    46,    -1,    -1,    49,
      50,    51,    -1,    53,    54,    55,    56,    -1,    -1,    59,
      60,    61,    -1,    -1,    -1,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    -1,    -1,    76,    77,    78,    79,
      80,    -1,    -1,    -1,    84,    85,    86,    87,    -1,    89,
      90,    -1,    92,    -1,    94,    95,    96,    97,    98,    99,
      -1,   101,   102,    -1,   104,    -1,   106,    -1,    -1,   109,
     110,   111,   112,    -1,    -1,   115,    -1,   117,    -1,   119,
     120,   121,    -1,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,   132,   133,   134,    -1,   136,    -1,   138,    -1,
      -1,   141,   142,   143,   144,   145,    -1,   147,   148,    -1,
      -1,   151,   152,   153,    -1,    -1,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,    -1,
     170,   171,   172,   173,   174,    -1,   176,   177,    -1,    -1,
     180,   181,   182,    -1,    -1,   185,    -1,    -1,    -1,   189,
     190,    -1,    -1,   193,    -1,    -1,   196,   197,   198,   199,
     200,   201,    -1,   203,   204,   205,    -1,   207,   208,   209,
     210,   211,   212,   213,    -1,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,    -1,   232,   233,   234,    -1,   236,   237,   238,   239,
      -1,   241,   242,    -1,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,    -1,   255,   256,   257,    -1,   259,
     260,    -1,   262,    -1,   264,   265,   266,   267,    -1,   269,
     270,   271,   272,    -1,    -1,   275,   276,   277,   278,   279,
      -1,    -1,   282,   283,   284,   285,   286,   287,    -1,   289,
     290,   291,    -1,    -1,   294,   295,   296,   297,   298,   299,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,     9,
      -1,    11,    -1,    -1,    -1,    -1,   336,   337,    18,    19,
      20,    -1,    22,    23,    24,    -1,    26,    -1,    28,    29,
      -1,    31,    32,    33,    34,    -1,    -1,    37,    38,    39,
      40,   361,    42,    43,    44,    45,    -1,    -1,    -1,    49,
      50,    51,    -1,    53,    54,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    -1,    -1,    76,    77,    78,    79,
      80,    -1,    -1,    -1,    84,    85,    86,    87,    -1,    89,
      90,    -1,    92,    -1,    94,    95,    -1,    97,    98,    -1,
      -1,   101,   102,    -1,   104,    -1,   106,    -1,    -1,    -1,
     110,   111,   112,    -1,    -1,   115,    -1,   117,    -1,   119,
     120,   121,   122,   123,   124,   125,    -1,    -1,   128,   129,
     130,   131,   132,   133,   134,    -1,   136,    -1,   138,    -1,
      -1,   141,    -1,   143,   144,   145,    -1,    -1,   148,    -1,
      -1,   151,   152,   153,    -1,    -1,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,    -1,   168,    -1,
     170,   171,   172,   173,    -1,    -1,   176,   177,    -1,    -1,
      -1,   181,   182,    -1,    -1,   185,    -1,    -1,    -1,   189,
     190,    -1,    -1,   193,    -1,    -1,    -1,   197,   198,   199,
     200,   201,    -1,    -1,   204,   205,    -1,   207,   208,   209,
     210,   211,   212,   213,    -1,   215,   216,   217,   218,   219,
     220,   221,   222,    -1,   224,    -1,   226,   227,   228,   229,
     230,    -1,   232,   233,   234,    -1,   236,   237,   238,   239,
      -1,   241,   242,    -1,   244,   245,   246,   247,   248,   249,
     250,   251,    -1,   253,    -1,   255,   256,   257,    -1,   259,
     260,    -1,   262,    -1,   264,    -1,   266,    -1,    -1,   269,
     270,   271,   272,    -1,    -1,   275,   276,   277,   278,   279,
      -1,    -1,   282,   283,   284,   285,   286,   287,    -1,   289,
     290,   291,    -1,    -1,   294,   295,   296,   297,   298,   299,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,     9,
      -1,    11,    -1,    -1,    -1,    -1,    -1,   337,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,   361,    42,    43,    44,    45,    46,    -1,    -1,    49,
      50,    51,    -1,    53,    54,    55,    56,    -1,    -1,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    -1,    -1,    76,    77,    78,    79,
      80,    -1,    -1,    -1,    84,    85,    86,    87,    -1,    89,
      90,    -1,    92,    -1,    94,    95,    96,    97,    98,    99,
     100,   101,   102,    -1,   104,    -1,   106,   107,   108,   109,
     110,   111,   112,    -1,    -1,   115,    -1,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,   132,   133,   134,    -1,   136,    -1,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,    -1,    -1,   185,   186,    -1,    -1,   189,
     190,    -1,    -1,   193,   194,   195,   196,   197,   198,   199,
     200,   201,    -1,   203,   204,   205,    -1,   207,   208,   209,
     210,   211,   212,   213,    -1,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,    -1,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,    -1,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,    -1,   255,   256,   257,    -1,   259,
     260,    -1,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,    -1,   274,   275,   276,   277,   278,   279,
     280,    -1,   282,   283,   284,   285,   286,   287,   288,   289,
     290,   291,    -1,    -1,   294,   295,   296,   297,   298,   299,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   337,   338,   339,
      -1,   341,   342,   343,   344,   345,    -1,    -1,    -1,    -1,
      -1,   351,   352,    -1,    -1,   355,   356,    -1,    -1,    -1,
     360,     3,     4,     5,     6,     7,     8,     9,    -1,    11,
      -1,    -1,    -1,    -1,    -1,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    -1,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    -1,
      42,    43,    44,    45,    46,    -1,    -1,    49,    50,    51,
      -1,    53,    54,    55,    56,    -1,    -1,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    -1,    76,    77,    78,    79,    80,    -1,
      -1,    -1,    84,    85,    86,    87,    -1,    89,    90,    -1,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
     102,    -1,   104,    -1,   106,   107,    -1,   109,   110,   111,
     112,    -1,    -1,   115,    -1,   117,   118,   119,   120,   121,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
     132,   133,   134,    -1,   136,    -1,   138,   139,   140,   141,
     142,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,    -1,    -1,   185,   186,    -1,    -1,   189,   190,    -1,
      -1,   193,   194,   195,   196,   197,   198,   199,   200,   201,
      -1,   203,   204,   205,    -1,   207,   208,   209,   210,   211,
     212,   213,    -1,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,    -1,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,    -1,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,    -1,   255,   256,   257,    -1,   259,   260,    -1,
     262,    -1,   264,   265,   266,   267,   268,   269,   270,   271,
     272,    -1,   274,   275,   276,   277,   278,   279,   280,    -1,
     282,   283,   284,   285,   286,   287,   288,   289,   290,   291,
      -1,    -1,   294,   295,   296,   297,   298,   299,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   337,   338,   339,    -1,   341,
     342,   343,   344,   345,    -1,    -1,    -1,    -1,    -1,   351,
     352,   353,    -1,   355,   356,    -1,    -1,    -1,   360,     3,
       4,     5,     6,     7,     8,     9,    -1,    11,    -1,    -1,
      -1,    -1,    -1,    -1,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    -1,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    -1,    42,    43,
      44,    45,    46,    -1,    -1,    49,    50,    51,    -1,    53,
      54,    55,    56,    -1,    -1,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      -1,    -1,    76,    77,    78,    79,    80,    -1,    -1,    -1,
      84,    85,    86,    87,    -1,    89,    90,    -1,    92,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,   102,    -1,
     104,    -1,   106,   107,    -1,   109,   110,   111,   112,    -1,
      -1,   115,    -1,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,   132,   133,
     134,    -1,   136,    -1,   138,   139,   140,   141,   142,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,    -1,
      -1,   185,   186,    -1,    -1,   189,   190,    -1,    -1,   193,
     194,   195,   196,   197,   198,   199,   200,   201,    -1,   203,
     204,   205,    -1,   207,   208,   209,   210,   211,   212,   213,
      -1,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,    -1,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,    -1,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
      -1,   255,   256,   257,    -1,   259,   260,    -1,   262,    -1,
     264,   265,   266,   267,   268,   269,   270,   271,   272,    -1,
     274,   275,   276,   277,   278,   279,   280,    -1,   282,   283,
     284,   285,   286,   287,   288,   289,   290,   291,    -1,    -1,
     294,   295,   296,   297,   298,   299,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   337,   338,   339,    -1,   341,   342,   343,
     344,   345,    -1,    -1,    -1,    -1,    -1,   351,   352,   353,
      -1,   355,   356,    -1,    -1,    -1,   360,     3,     4,     5,
       6,     7,     8,     9,    -1,    11,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    -1,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    -1,    42,    43,    44,    45,
      46,    -1,    -1,    49,    50,    51,    -1,    53,    54,    55,
      56,    -1,    -1,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    -1,    -1,
      76,    77,    78,    79,    80,    -1,    -1,    -1,    84,    85,
      86,    87,    -1,    89,    90,    -1,    92,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,   102,    -1,   104,    -1,
     106,   107,    -1,   109,   110,   111,   112,    -1,    -1,   115,
      -1,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,   132,   133,   134,    -1,
     136,    -1,   138,   139,   140,   141,   142,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,    -1,    -1,   185,
     186,    -1,    -1,   189,   190,    -1,    -1,   193,   194,   195,
     196,   197,   198,   199,   200,   201,    -1,   203,   204,   205,
      -1,   207,   208,   209,   210,   211,   212,   213,    -1,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,    -1,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,    -1,   255,
     256,   257,    -1,   259,   260,    -1,   262,    -1,   264,   265,
     266,   267,   268,   269,   270,   271,   272,    -1,   274,   275,
     276,   277,   278,   279,   280,    -1,   282,   283,   284,   285,
     286,   287,   288,   289,   290,   291,    -1,    -1,   294,   295,
     296,   297,   298,   299,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   337,   338,   339,    -1,   341,   342,   343,   344,   345,
      -1,    -1,    -1,    -1,    -1,   351,   352,    -1,    -1,   355,
     356,    -1,    -1,    -1,   360,     3,     4,     5,     6,     7,
       8,     9,    -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    -1,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    -1,    42,    43,    44,    45,    46,    -1,
      -1,    49,    50,    51,    -1,    53,    54,    55,    56,    -1,
      -1,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    -1,    -1,    76,    77,
      78,    79,    80,    -1,    -1,    -1,    84,    85,    86,    87,
      -1,    89,    90,    -1,    92,    -1,    94,    95,    96,    97,
      98,    99,   100,   101,   102,    -1,   104,    -1,   106,   107,
     108,   109,   110,   111,   112,    -1,    -1,   115,    -1,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,   132,   133,   134,    -1,   136,    -1,
     138,   139,   140,   141,   142,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,    -1,    -1,   185,   186,    -1,
      -1,   189,   190,    -1,    -1,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    -1,   203,   204,   205,    -1,   207,
     208,   209,   210,   211,   212,   213,    -1,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,    -1,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,    -1,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,    -1,   255,   256,   257,
      -1,   259,   260,    -1,   262,    -1,   264,   265,   266,   267,
     268,   269,   270,   271,   272,    -1,   274,   275,   276,   277,
     278,   279,   280,    -1,   282,   283,   284,   285,   286,   287,
     288,   289,   290,   291,    -1,    -1,   294,   295,   296,   297,
     298,   299,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   337,
     338,   339,    -1,   341,   342,   343,   344,   345,    -1,    -1,
      -1,    -1,    -1,   351,   352,    -1,    -1,   355,   356,    -1,
      -1,    -1,   360,     3,     4,     5,     6,     7,     8,     9,
      -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      -1,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    -1,    42,    43,    44,    45,    46,    -1,    -1,    49,
      50,    51,    -1,    53,    54,    55,    56,    -1,    -1,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    -1,    -1,    76,    77,    78,    79,
      80,    -1,    -1,    -1,    84,    85,    86,    87,    -1,    89,
      90,    -1,    92,    -1,    94,    95,    96,    97,    98,    99,
     100,   101,   102,    -1,   104,    -1,   106,   107,    -1,   109,
     110,   111,   112,    -1,    -1,   115,    -1,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,   132,   133,   134,    -1,   136,    -1,   138,   139,
     140,   141,   142,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,    -1,    -1,   185,   186,    -1,    -1,   189,
     190,    -1,    -1,   193,   194,   195,   196,   197,   198,   199,
     200,   201,    -1,   203,   204,   205,    -1,   207,   208,   209,
     210,   211,   212,   213,    -1,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,    -1,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,    -1,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,    -1,   255,   256,   257,    -1,   259,
     260,    -1,   262,    -1,   264,   265,   266,   267,   268,   269,
     270,   271,   272,    -1,   274,   275,   276,   277,   278,   279,
     280,    -1,   282,   283,   284,   285,   286,   287,   288,   289,
     290,   291,    -1,    -1,   294,   295,   296,   297,   298,   299,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   337,   338,   339,
      -1,   341,   342,   343,   344,   345,    -1,    -1,    -1,    -1,
      -1,   351,   352,    -1,    -1,   355,   356,    -1,    -1,    -1,
     360,     3,     4,     5,     6,     7,     8,     9,    -1,    11,
      -1,    -1,    -1,    -1,    -1,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    -1,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    -1,
      42,    43,    44,    45,    46,    -1,    -1,    49,    50,    51,
      -1,    53,    54,    55,    56,    -1,    -1,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    -1,    -1,    76,    77,    78,    79,    80,    -1,
      -1,    -1,    84,    85,    86,    87,    -1,    89,    90,    -1,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
     102,    -1,   104,    -1,   106,   107,    -1,   109,   110,   111,
     112,    -1,    -1,   115,    -1,   117,   118,   119,   120,   121,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
     132,   133,   134,    -1,   136,    -1,   138,   139,   140,   141,
     142,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,    -1,   176,   177,   178,   179,   180,   181,
     182,    -1,    -1,   185,   186,    -1,    -1,   189,   190,    -1,
      -1,   193,   194,   195,   196,   197,   198,   199,   200,   201,
      -1,   203,   204,   205,    -1,   207,   208,   209,   210,   211,
     212,   213,    -1,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,    -1,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,    -1,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,    -1,   255,   256,   257,    -1,   259,   260,    -1,
     262,    -1,   264,   265,   266,   267,   268,   269,   270,   271,
     272,    -1,    -1,   275,   276,   277,   278,   279,   280,    -1,
     282,   283,   284,   285,   286,   287,   288,   289,   290,   291,
      -1,    -1,   294,   295,   296,   297,   298,   299,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   337,   338,   339,    -1,   341,
     342,   343,   344,   345,    -1,    -1,    -1,    -1,    -1,   351,
     352,    -1,    -1,   355,   356,    -1,    -1,    -1,   360,     3,
       4,     5,     6,     7,     8,     9,    -1,    11,    -1,    -1,
      -1,    -1,    -1,    -1,    18,    19,    20,    21,    22,    23,
      24,    -1,    26,    27,    28,    29,    -1,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    -1,    42,    43,
      44,    45,    46,    -1,    -1,    49,    50,    51,    -1,    53,
      54,    55,    56,    -1,    -1,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      -1,    -1,    76,    77,    78,    79,    80,    -1,    -1,    -1,
      84,    85,    86,    87,    -1,    89,    90,    -1,    -1,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,   102,    -1,
     104,    -1,   106,   107,    -1,   109,   110,   111,   112,    -1,
      -1,   115,    -1,   117,    -1,   119,   120,   121,    -1,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,   132,   133,
     134,    -1,   136,    -1,   138,   139,   140,   141,   142,   143,
     144,   145,    -1,   147,   148,    -1,    -1,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,    -1,   176,   177,   178,   179,   180,   181,   182,    -1,
      -1,   185,   186,    -1,    -1,   189,   190,    -1,    -1,   193,
     194,   195,   196,   197,   198,   199,   200,   201,    -1,   203,
     204,   205,    -1,   207,   208,   209,   210,   211,   212,   213,
      -1,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,    -1,   232,   233,
     234,   235,   236,   237,   238,   239,    -1,   241,   242,    -1,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
      -1,   255,   256,   257,    -1,   259,   260,    -1,   262,    -1,
     264,   265,   266,   267,   268,   269,   270,   271,   272,    -1,
     274,   275,   276,   277,   278,   279,   280,    -1,   282,   283,
     284,   285,   286,   287,   288,   289,   290,   291,    -1,    -1,
     294,   295,   296,   297,   298,   299,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   337,   338,   339,    -1,   341,   342,   343,
     344,   345,    -1,    -1,    -1,    -1,    -1,   351,   352,    -1,
      -1,   355,   356,    -1,    -1,    -1,   360,     3,     4,     5,
       6,     7,     8,     9,    -1,    11,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    -1,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    -1,    42,    43,    44,    45,
      46,    -1,    -1,    49,    50,    51,    -1,    53,    54,    55,
      56,    -1,    -1,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    -1,    -1,
      76,    77,    78,    79,    80,    -1,    -1,    -1,    84,    85,
      86,    87,    -1,    89,    90,    -1,    92,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,   102,    -1,   104,    -1,
     106,   107,    -1,   109,   110,   111,   112,    -1,    -1,   115,
      -1,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,   132,   133,   134,    -1,
     136,    -1,   138,   139,   140,   141,   142,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,    -1,
     176,   177,   178,   179,   180,   181,   182,    -1,    -1,   185,
     186,    -1,    -1,   189,   190,    -1,    -1,   193,   194,   195,
     196,   197,   198,   199,   200,   201,    -1,   203,   204,   205,
      -1,   207,   208,   209,   210,   211,   212,   213,    -1,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,    -1,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,    -1,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,    -1,   255,
     256,   257,    -1,   259,   260,    -1,   262,    -1,   264,   265,
     266,   267,   268,   269,   270,   271,   272,    -1,    -1,   275,
     276,   277,   278,   279,   280,    -1,   282,   283,   284,   285,
     286,   287,   288,   289,   290,   291,    -1,    -1,   294,   295,
     296,   297,   298,   299,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   337,   338,   339,    -1,   341,   342,    -1,   344,   345,
       3,     4,     5,     6,     7,     8,     9,    -1,    11,    -1,
      -1,    -1,    -1,    -1,   360,    18,    19,    20,    21,    22,
      23,    24,    -1,    26,    27,    28,    29,    -1,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    -1,    42,
      43,    44,    45,    46,    -1,    -1,    49,    50,    51,    -1,
      53,    54,    55,    56,    -1,    -1,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    -1,    -1,    76,    77,    78,    79,    80,    -1,    -1,
      -1,    84,    85,    86,    87,    -1,    89,    90,    -1,    -1,
      -1,    94,    95,    96,    97,    98,    99,   100,   101,   102,
      -1,   104,    -1,   106,   107,    -1,   109,   110,   111,   112,
      -1,    -1,   115,    -1,   117,    -1,   119,   120,   121,    -1,
     123,   124,   125,    -1,   127,   128,   129,   130,   131,   132,
     133,   134,    -1,   136,    -1,   138,   139,   140,   141,   142,
     143,   144,   145,    -1,   147,   148,    -1,    -1,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,    -1,   176,   177,   178,   179,   180,   181,   182,
      -1,    -1,   185,   186,    -1,    -1,   189,   190,    -1,    -1,
     193,   194,   195,   196,   197,   198,   199,   200,   201,    -1,
     203,   204,   205,    -1,   207,   208,   209,   210,   211,   212,
     213,    -1,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,    -1,   232,
     233,   234,   235,   236,   237,   238,   239,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,    -1,   255,   256,   257,    -1,   259,   260,    -1,   262,
      -1,   264,   265,   266,   267,   268,   269,   270,   271,   272,
      -1,    -1,   275,   276,   277,   278,   279,   280,    -1,   282,
     283,   284,   285,   286,   287,   288,   289,   290,   291,    -1,
      -1,   294,   295,   296,   297,   298,   299,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   337,   338,   339,    -1,   341,   342,
     343,   344,   345,    -1,    -1,    -1,    -1,    -1,   351,   352,
      -1,    -1,   355,   356,    -1,    -1,    -1,   360,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    -1,    -1,    -1,
      15,    -1,    -1,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    28,    29,    -1,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    -1,    42,    43,    44,
      45,    46,    -1,    -1,    49,    50,    51,    -1,    53,    54,
      55,    56,    -1,    -1,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    -1,
      -1,    76,    77,    78,    79,    80,    -1,    -1,    -1,    84,
      85,    86,    87,    -1,    89,    90,    -1,    -1,    -1,    94,
      95,    96,    97,    98,    99,   100,   101,   102,    -1,   104,
      -1,   106,   107,    -1,   109,   110,   111,   112,    -1,    -1,
     115,    -1,   117,    -1,   119,   120,   121,    -1,   123,   124,
     125,    -1,   127,   128,   129,   130,   131,   132,   133,   134,
      -1,   136,    -1,   138,    -1,    -1,   141,   142,   143,   144,
     145,    -1,   147,   148,    -1,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
      -1,   176,   177,    -1,   179,   180,   181,   182,    -1,    -1,
     185,   186,    -1,    -1,    -1,   190,    -1,    -1,   193,   194,
     195,   196,   197,   198,   199,   200,   201,    -1,   203,   204,
     205,    -1,   207,   208,   209,   210,   211,   212,   213,    -1,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,    -1,   232,   233,   234,
     235,   236,   237,   238,   239,    -1,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,    -1,
     255,   256,   257,    -1,   259,   260,    -1,   262,    -1,   264,
     265,   266,   267,   268,   269,   270,   271,   272,    -1,   274,
     275,   276,   277,   278,   279,   280,    -1,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,    -1,    -1,   294,
     295,   296,   297,   298,   299,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   337,   338,   339,    -1,   341,   342,    -1,   344,
     345,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      -1,   356,    -1,    15,    -1,   360,    18,    19,    20,    21,
      22,    23,    24,    -1,    26,    27,    28,    29,    -1,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    -1,
      42,    43,    44,    45,    46,    -1,    -1,    49,    50,    51,
      -1,    53,    54,    55,    56,    -1,    -1,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    -1,    -1,    76,    77,    78,    79,    80,    -1,
      -1,    -1,    84,    85,    86,    87,    -1,    89,    90,    -1,
      -1,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
     102,    -1,   104,    -1,   106,   107,    -1,   109,   110,   111,
     112,    -1,    -1,   115,    -1,   117,    -1,   119,   120,   121,
      -1,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
     132,   133,   134,    -1,   136,    -1,   138,    -1,    -1,   141,
     142,   143,   144,   145,    -1,   147,   148,    -1,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,    -1,   176,   177,    -1,   179,   180,   181,
     182,    -1,    -1,   185,   186,    -1,    -1,    -1,   190,    -1,
      -1,   193,   194,   195,   196,   197,   198,   199,   200,   201,
      -1,   203,   204,   205,    -1,   207,   208,   209,   210,   211,
     212,   213,    -1,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,    -1,
     232,   233,   234,   235,   236,   237,   238,   239,    -1,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,    -1,   255,   256,   257,    -1,   259,   260,    -1,
     262,    -1,   264,   265,   266,   267,   268,   269,   270,   271,
     272,    -1,   274,   275,   276,   277,   278,   279,   280,    -1,
     282,   283,   284,   285,   286,   287,   288,   289,   290,   291,
      -1,    -1,   294,   295,   296,   297,   298,   299,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   337,   338,   339,    -1,   341,
     342,    -1,   344,   345,     3,     4,     5,     6,     7,     8,
       9,    -1,    11,    -1,    -1,    -1,    -1,    -1,   360,    18,
      19,    20,    21,    22,    23,    24,    -1,    26,    27,    28,
      29,    -1,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    42,    43,    44,    45,    46,    -1,    -1,
      49,    50,    51,    -1,    53,    54,    55,    56,    -1,    -1,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    -1,    -1,    76,    77,    78,
      79,    80,    -1,    -1,    -1,    84,    85,    86,    87,    -1,
      89,    90,    -1,    -1,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,   102,    -1,   104,    -1,   106,   107,    -1,
     109,   110,   111,   112,    -1,    -1,   115,    -1,   117,    -1,
     119,   120,   121,    -1,   123,   124,   125,    -1,   127,   128,
     129,   130,   131,   132,   133,   134,    -1,   136,    -1,   138,
      -1,    -1,   141,   142,   143,   144,   145,    -1,   147,   148,
      -1,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,    -1,   176,   177,    -1,
      -1,   180,   181,   182,    -1,    -1,   185,   186,    -1,    -1,
      -1,   190,    -1,    -1,   193,   194,   195,   196,   197,   198,
     199,   200,   201,    -1,   203,   204,   205,    -1,   207,   208,
     209,   210,   211,   212,   213,    -1,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,    -1,   232,   233,   234,   235,   236,   237,   238,
     239,    -1,   241,   242,    -1,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,    -1,   255,   256,   257,    -1,
     259,   260,    -1,   262,    -1,   264,   265,   266,   267,   268,
     269,   270,   271,   272,    -1,    -1,   275,   276,   277,   278,
     279,   280,    -1,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   291,    -1,    -1,   294,   295,   296,   297,   298,
     299,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   337,   338,
     339,    -1,   341,   342,    -1,   344,   345,     3,     4,     5,
       6,     7,     8,     9,    -1,    11,    -1,   356,    -1,    -1,
      -1,   360,    18,    19,    20,    21,    22,    23,    24,    -1,
      26,    27,    28,    29,    -1,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    -1,    42,    43,    44,    45,
      46,    -1,    -1,    49,    50,    51,    -1,    53,    54,    55,
      56,    -1,    -1,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    -1,    -1,
      76,    77,    78,    79,    80,    -1,    -1,    -1,    84,    85,
      86,    87,    -1,    89,    90,    -1,    -1,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,   102,    -1,   104,    -1,
     106,   107,    -1,   109,   110,   111,   112,    -1,    -1,   115,
      -1,   117,    -1,   119,   120,   121,    -1,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,   132,   133,   134,    -1,
     136,    -1,   138,    -1,    -1,   141,   142,   143,   144,   145,
      -1,   147,   148,    -1,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,    -1,
     176,   177,    -1,    -1,   180,   181,   182,    -1,    -1,   185,
     186,    -1,    -1,    -1,   190,    -1,    -1,   193,   194,   195,
     196,   197,   198,   199,   200,   201,    -1,   203,   204,   205,
      -1,   207,   208,   209,   210,   211,   212,   213,    -1,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,    -1,   232,   233,   234,   235,
     236,   237,   238,   239,    -1,   241,   242,    -1,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,    -1,   255,
     256,   257,    -1,   259,   260,    -1,   262,    -1,   264,   265,
     266,   267,   268,   269,   270,   271,   272,    -1,    -1,   275,
     276,   277,   278,   279,   280,    -1,   282,   283,   284,   285,
     286,   287,   288,   289,   290,   291,    -1,    -1,   294,   295,
     296,   297,   298,   299,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   337,   338,   339,    -1,   341,   342,    -1,   344,   345,
       3,     4,     5,     6,     7,     8,     9,    -1,    11,    -1,
      -1,    -1,    -1,    -1,   360,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    -1,    31,    32,
      33,    34,    -1,    -1,    37,    38,    39,    40,    -1,    42,
      43,    44,    45,    46,    -1,    -1,    49,    50,    51,    -1,
      53,    54,    55,    56,    -1,    -1,    59,    60,    61,    -1,
      -1,    -1,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    -1,    -1,    76,    77,    78,    79,    80,    -1,    -1,
      -1,    84,    85,    86,    87,    -1,    89,    90,    -1,    92,
      -1,    94,    95,    96,    97,    98,    99,    -1,   101,   102,
      -1,   104,    -1,   106,   107,    -1,   109,   110,   111,   112,
      -1,    -1,   115,    -1,   117,   118,   119,   120,   121,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,   131,   132,
     133,   134,    -1,   136,    -1,   138,   139,   140,   141,   142,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,    -1,    -1,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,    -1,   176,   177,   178,    -1,   180,   181,   182,
      -1,    -1,   185,   186,    -1,   188,   189,   190,    -1,    -1,
     193,   194,   195,   196,   197,   198,   199,   200,   201,    -1,
     203,   204,   205,    -1,   207,   208,   209,   210,   211,   212,
     213,    -1,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,    -1,   236,   237,   238,   239,   240,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,    -1,   255,   256,   257,    -1,   259,   260,    -1,   262,
      -1,   264,   265,   266,   267,    -1,   269,   270,   271,   272,
      -1,    -1,   275,   276,   277,   278,   279,    -1,    -1,   282,
     283,   284,   285,   286,   287,   288,   289,   290,   291,    -1,
      -1,   294,   295,   296,   297,   298,   299,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,     9,    -1,
      11,    -1,    -1,    -1,    -1,    -1,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    -1,
      31,    32,    33,    34,   337,    -1,    37,    38,    39,    40,
      -1,    42,    43,    44,    45,    46,    -1,    -1,    49,    50,
      51,    -1,    53,    54,    55,    56,    -1,   360,    59,    60,
      61,    -1,    -1,    -1,    -1,    66,    67,    68,    69,    70,
      71,    72,    73,    -1,    -1,    76,    77,    78,    79,    80,
      -1,    -1,    -1,    84,    85,    86,    87,    -1,    89,    90,
      -1,    92,    -1,    94,    95,    96,    97,    98,    99,    -1,
     101,   102,    -1,   104,    -1,   106,   107,    -1,   109,   110,
     111,   112,    -1,    -1,   115,    -1,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    -1,   127,   128,   129,   130,
     131,   132,   133,   134,    -1,   136,    -1,   138,   139,   140,
     141,   142,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,    -1,    -1,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,    -1,   176,   177,   178,    -1,   180,
     181,   182,    -1,    -1,   185,   186,    -1,   188,   189,   190,
      -1,    -1,   193,   194,   195,   196,   197,   198,   199,   200,
     201,    -1,   203,   204,   205,    -1,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
      -1,   232,   233,   234,    -1,   236,   237,   238,   239,   240,
     241,   242,    -1,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,    -1,   255,   256,   257,    -1,   259,   260,
      -1,   262,    -1,   264,   265,   266,   267,    -1,   269,   270,
     271,   272,    -1,    -1,   275,   276,   277,   278,   279,    -1,
      -1,   282,   283,   284,   285,   286,   287,   288,   289,   290,
     291,    -1,    -1,   294,   295,   296,   297,   298,   299,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
       9,    -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      19,    20,    -1,    22,    23,    24,    -1,    26,    -1,    28,
      29,    -1,    31,    32,    33,    34,   337,    -1,    37,    38,
      39,    40,    -1,    42,    43,    44,    45,    46,    -1,    -1,
      49,    50,    51,    -1,    53,    54,    55,    56,    -1,   360,
      59,    60,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    -1,    -1,    76,    77,    78,
      79,    80,    -1,    -1,    -1,    84,    85,    86,    87,    -1,
      89,    90,    -1,    92,    -1,    94,    95,    96,    97,    98,
      99,    -1,   101,   102,    -1,   104,    -1,   106,    -1,    -1,
      -1,   110,   111,   112,    -1,    -1,   115,    -1,   117,    -1,
     119,   120,   121,    -1,   123,   124,   125,    -1,    -1,   128,
     129,   130,   131,   132,   133,   134,    -1,   136,    -1,   138,
      -1,    -1,   141,    -1,   143,   144,   145,    -1,    -1,   148,
      -1,    -1,   151,   152,   153,    -1,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,    -1,   168,
     169,   170,   171,   172,   173,   174,    -1,   176,   177,    -1,
      -1,   180,   181,   182,    -1,    -1,   185,   186,    -1,    -1,
     189,   190,    -1,    -1,   193,    -1,    -1,   196,   197,   198,
     199,   200,   201,    -1,   203,   204,   205,    -1,   207,   208,
     209,   210,   211,   212,   213,    -1,   215,   216,   217,   218,
     219,   220,   221,   222,    -1,   224,   225,   226,   227,   228,
     229,   230,    -1,   232,   233,   234,    -1,   236,   237,   238,
     239,    -1,   241,   242,    -1,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,    -1,   255,   256,   257,    -1,
     259,   260,    -1,   262,    -1,   264,   265,   266,   267,    -1,
     269,   270,   271,   272,    -1,    -1,   275,   276,   277,   278,
     279,    -1,    -1,   282,   283,   284,   285,   286,   287,    -1,
     289,   290,   291,    -1,    -1,   294,   295,   296,   297,   298,
     299,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,     9,    -1,    11,    -1,    -1,    -1,    -1,    -1,
      -1,    18,    19,    20,    -1,    22,    23,    24,    -1,    26,
      -1,    28,    29,    -1,    31,    32,    33,    34,   337,    -1,
      37,    38,    39,    40,    -1,    42,    43,    44,    45,    46,
      -1,    -1,    49,    50,    51,    -1,    53,    54,    55,    56,
      -1,   360,    59,    60,    -1,    -1,    -1,    -1,    -1,    66,
      67,    68,    69,    70,    71,    72,    73,    -1,    -1,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    85,    86,
      87,    -1,    89,    90,    -1,    92,    -1,    94,    95,    96,
      97,    98,    99,    -1,   101,   102,    -1,   104,    -1,   106,
      -1,    -1,    -1,   110,   111,   112,    -1,    -1,   115,    -1,
     117,    -1,   119,   120,   121,    -1,   123,   124,   125,    -1,
      -1,   128,   129,   130,   131,   132,   133,   134,    -1,   136,
      -1,   138,    -1,    -1,   141,    -1,   143,   144,   145,    -1,
      -1,   148,    -1,    -1,   151,   152,   153,    -1,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      -1,   168,   169,   170,   171,   172,   173,   174,    -1,   176,
     177,    -1,    -1,   180,   181,   182,    -1,    -1,   185,   186,
      -1,    -1,   189,   190,    -1,    -1,   193,    -1,    -1,   196,
     197,   198,   199,   200,   201,    -1,   203,   204,   205,    -1,
     207,   208,   209,   210,   211,   212,   213,    -1,   215,   216,
     217,   218,   219,   220,   221,   222,    -1,   224,   225,   226,
     227,   228,   229,   230,    -1,   232,   233,   234,    -1,   236,
     237,   238,   239,    -1,   241,   242,    -1,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,    -1,   255,   256,
     257,    -1,   259,   260,    -1,   262,    -1,   264,   265,   266,
     267,    -1,   269,   270,   271,   272,    -1,    -1,   275,   276,
     277,   278,   279,    -1,    -1,   282,   283,   284,   285,   286,
     287,    -1,   289,   290,   291,    -1,    -1,   294,   295,   296,
     297,   298,   299,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,     9,    -1,    11,    -1,    -1,    -1,
      -1,    -1,    -1,    18,    19,    20,    -1,    22,    23,    24,
      -1,    26,    -1,    28,    29,    -1,    31,    32,    33,    34,
     337,    -1,    37,    38,    39,    40,    -1,    42,    43,    44,
      45,    46,    -1,    -1,    49,    50,    51,    -1,    53,    54,
      55,    56,    -1,   360,    59,    60,    -1,    -1,    -1,    -1,
      -1,    66,    67,    68,    69,    70,    71,    72,    73,    -1,
      -1,    76,    77,    78,    79,    80,    -1,    -1,    -1,    84,
      85,    86,    87,    -1,    89,    90,    -1,    92,    -1,    94,
      95,    96,    97,    98,    99,    -1,   101,   102,    -1,   104,
      -1,   106,    -1,    -1,    -1,   110,   111,   112,    -1,    -1,
     115,    -1,   117,    -1,   119,   120,   121,    -1,   123,   124,
     125,    -1,    -1,   128,   129,   130,   131,   132,   133,   134,
      -1,   136,    -1,   138,    -1,    -1,   141,    -1,   143,   144,
     145,    -1,    -1,   148,    -1,    -1,   151,   152,   153,    -1,
      -1,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,    -1,   168,    -1,   170,   171,   172,   173,   174,
      -1,   176,   177,    -1,    -1,   180,   181,   182,    -1,    -1,
     185,    -1,    -1,    -1,   189,   190,    -1,    -1,   193,    -1,
      -1,   196,   197,   198,   199,   200,   201,    -1,   203,   204,
     205,    -1,   207,   208,   209,   210,   211,   212,   213,    -1,
     215,   216,   217,   218,   219,   220,   221,   222,    -1,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
      -1,   236,   237,   238,   239,    -1,   241,   242,    -1,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,    -1,
     255,   256,   257,    -1,   259,   260,    -1,   262,    -1,   264,
     265,   266,   267,    -1,   269,   270,   271,   272,    -1,    -1,
     275,   276,   277,   278,   279,    -1,    -1,   282,   283,   284,
     285,   286,   287,    -1,   289,   290,   291,    -1,    -1,   294,
     295,   296,   297,   298,   299,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,     9,    -1,    11,    -1,
      -1,    -1,    -1,    -1,    -1,    18,    19,    20,    -1,    22,
      23,    24,    -1,    26,    -1,    28,    29,    -1,    31,    32,
      33,    34,   337,    -1,    37,    38,    39,    40,    -1,    42,
      43,    44,    45,    46,    -1,    -1,    49,    50,    51,    -1,
      53,    54,    55,    56,    -1,   360,    59,    60,    -1,    -1,
      -1,    -1,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    -1,    -1,    76,    77,    78,    79,    80,    -1,    -1,
      -1,    84,    85,    86,    87,    -1,    89,    90,    -1,    92,
      -1,    94,    95,    96,    97,    98,    99,    -1,   101,   102,
      -1,   104,    -1,   106,    -1,    -1,    -1,   110,   111,   112,
      -1,    -1,   115,    -1,   117,    -1,   119,   120,   121,    -1,
     123,   124,   125,    -1,    -1,   128,   129,   130,   131,   132,
     133,   134,    -1,   136,    -1,   138,    -1,    -1,   141,    -1,
     143,   144,   145,    -1,    -1,   148,    -1,    -1,   151,   152,
     153,    -1,    -1,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,    -1,   168,    -1,   170,   171,   172,
     173,   174,    -1,   176,   177,    -1,    -1,   180,   181,   182,
      -1,    -1,   185,    -1,    -1,    -1,   189,   190,    -1,    -1,
     193,    -1,    -1,   196,   197,   198,   199,   200,   201,    -1,
     203,   204,   205,    -1,   207,   208,   209,   210,   211,   212,
     213,    -1,   215,   216,   217,   218,   219,   220,   221,   222,
      -1,   224,   225,   226,   227,   228,   229,   230,    -1,   232,
     233,   234,    -1,   236,   237,   238,   239,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,    -1,   255,   256,   257,    -1,   259,   260,    -1,   262,
      -1,   264,   265,   266,   267,    -1,   269,   270,   271,   272,
      -1,    -1,   275,   276,   277,   278,   279,    -1,    -1,   282,
     283,   284,   285,   286,   287,    -1,   289,   290,   291,    -1,
      -1,   294,   295,   296,   297,   298,   299,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,     9,    -1,
      11,    -1,    -1,    -1,    -1,    16,    -1,    18,    19,    20,
      -1,    22,    23,    24,    -1,    26,    -1,    28,    29,    -1,
      31,    32,    33,    34,   337,    -1,    37,    38,    39,    40,
      -1,    42,    43,    44,    45,    46,    -1,    -1,    49,    50,
      51,    -1,    53,    54,    55,    56,    -1,   360,    59,    60,
      -1,    -1,    -1,    -1,    -1,    66,    67,    68,    69,    70,
      71,    72,    73,    -1,    -1,    76,    77,    78,    79,    80,
      -1,    -1,    -1,    84,    85,    86,    87,    -1,    89,    90,
      -1,    92,    -1,    94,    95,    96,    97,    98,    99,    -1,
     101,   102,    -1,   104,    -1,   106,    -1,    -1,    -1,   110,
     111,   112,    -1,    -1,   115,    -1,   117,    -1,   119,   120,
     121,    -1,   123,   124,   125,    -1,    -1,   128,   129,   130,
     131,   132,   133,   134,    -1,   136,    -1,   138,    -1,    -1,
     141,    -1,   143,   144,   145,    -1,    -1,   148,    -1,    -1,
     151,   152,   153,    -1,    -1,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,    -1,   168,    -1,   170,
     171,   172,   173,   174,    -1,   176,   177,    -1,    -1,   180,
     181,   182,    -1,    -1,   185,    -1,    -1,    -1,   189,   190,
      -1,    -1,   193,    -1,    -1,   196,   197,   198,   199,   200,
     201,    -1,   203,   204,   205,    -1,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,   216,   217,   218,   219,   220,
     221,   222,    -1,   224,   225,   226,   227,   228,   229,   230,
      -1,   232,   233,   234,    -1,   236,   237,   238,   239,    -1,
     241,   242,    -1,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,    -1,   255,   256,   257,    -1,   259,   260,
      -1,   262,    -1,   264,   265,   266,   267,    -1,   269,   270,
     271,   272,    -1,    -1,   275,   276,   277,   278,   279,    -1,
      -1,   282,   283,   284,   285,   286,   287,    -1,   289,   290,
     291,    -1,    -1,   294,   295,   296,   297,   298,   299,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,     9,
      -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,    18,    19,
      20,    -1,    22,    23,    24,    -1,    26,    -1,    28,    29,
      -1,    31,    32,    33,    34,    -1,   337,    37,    38,    39,
      40,    -1,    42,    43,    44,    45,    46,    -1,    -1,    49,
      50,    51,    -1,    53,    54,    55,    56,   358,    -1,    59,
      60,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    -1,    -1,    76,    77,    78,    79,
      80,    -1,    -1,    -1,    84,    85,    86,    87,    -1,    89,
      90,    -1,    92,    -1,    94,    95,    96,    97,    98,    99,
      -1,   101,   102,    -1,   104,    -1,   106,    -1,    -1,    -1,
     110,   111,   112,    -1,    -1,   115,    -1,   117,    -1,   119,
     120,   121,    -1,   123,   124,   125,    -1,    -1,   128,   129,
     130,   131,   132,   133,   134,    -1,   136,    -1,   138,    -1,
      -1,   141,    -1,   143,   144,   145,    -1,    -1,   148,    -1,
      -1,   151,   152,   153,    -1,    -1,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,    -1,   168,    -1,
     170,   171,   172,   173,   174,    -1,   176,   177,    -1,    -1,
     180,   181,   182,    -1,    -1,   185,    -1,    -1,    -1,   189,
     190,    -1,    -1,   193,    -1,    -1,   196,   197,   198,   199,
     200,   201,    -1,   203,   204,   205,    -1,   207,   208,   209,
     210,   211,   212,   213,    -1,   215,   216,   217,   218,   219,
     220,   221,   222,    -1,   224,   225,   226,   227,   228,   229,
     230,    -1,   232,   233,   234,    -1,   236,   237,   238,   239,
      -1,   241,   242,    -1,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,    -1,   255,   256,   257,    -1,   259,
     260,    -1,   262,    -1,   264,   265,   266,   267,    -1,   269,
     270,   271,   272,    -1,    -1,   275,   276,   277,   278,   279,
      -1,    -1,   282,   283,   284,   285,   286,   287,    -1,   289,
     290,   291,    -1,    -1,   294,   295,   296,   297,   298,   299,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   337,    -1,    -1,
      -1,    -1,    -1,   343,    -1,    -1,    -1,   347,   348,   349,
      -1,   351,   352,   353,   354,   355,   356,     3,     4,     5,
       6,     7,     8,     9,    -1,    11,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    19,    20,    -1,    22,    23,    24,    -1,
      26,    -1,    28,    29,    -1,    31,    32,    33,    34,    -1,
      -1,    37,    38,    39,    40,    -1,    42,    43,    44,    45,
      46,    -1,    -1,    49,    50,    51,    -1,    53,    54,    55,
      56,    -1,    -1,    59,    60,    -1,    -1,    -1,    -1,    -1,
      66,    67,    68,    69,    70,    71,    72,    73,    -1,    -1,
      76,    77,    78,    79,    80,    -1,    -1,    -1,    84,    85,
      86,    87,    -1,    89,    90,    -1,    92,    -1,    94,    95,
      96,    97,    98,    99,    -1,   101,   102,    -1,   104,    -1,
     106,    -1,    -1,    -1,   110,   111,   112,    -1,    -1,   115,
      -1,   117,    -1,   119,   120,   121,    -1,   123,   124,   125,
      -1,    -1,   128,   129,   130,   131,   132,   133,   134,    -1,
     136,    -1,   138,    -1,    -1,   141,    -1,   143,   144,   145,
      -1,    -1,   148,    -1,    -1,   151,   152,   153,    -1,    -1,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,    -1,   168,    -1,   170,   171,   172,   173,   174,    -1,
     176,   177,    -1,    -1,   180,   181,   182,    -1,    -1,   185,
      -1,    -1,    -1,   189,   190,    -1,    -1,   193,    -1,    -1,
     196,   197,   198,   199,   200,   201,    -1,   203,   204,   205,
      -1,   207,   208,   209,   210,   211,   212,   213,    -1,   215,
     216,   217,   218,   219,   220,   221,   222,    -1,   224,   225,
     226,   227,   228,   229,   230,    -1,   232,   233,   234,    -1,
     236,   237,   238,   239,    -1,   241,   242,    -1,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,    -1,   255,
     256,   257,    -1,   259,   260,    -1,   262,    -1,   264,   265,
     266,   267,    -1,   269,   270,   271,   272,    -1,    -1,   275,
     276,   277,   278,   279,    -1,    -1,   282,   283,   284,   285,
     286,   287,    -1,   289,   290,   291,    -1,    -1,   294,   295,
     296,   297,   298,   299,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   337,    -1,    -1,    -1,    -1,    -1,   343,    -1,    -1,
      -1,   347,   348,   349,    -1,   351,   352,   353,   354,   355,
     356,     3,     4,     5,     6,     7,     8,     9,    -1,    11,
      -1,    -1,    -1,    -1,    -1,    -1,    18,    19,    20,    -1,
      22,    23,    24,    -1,    26,    -1,    28,    29,    -1,    31,
      32,    33,    34,    -1,    -1,    37,    38,    39,    40,    -1,
      42,    43,    44,    45,    46,    -1,    -1,    49,    50,    51,
      -1,    53,    54,    55,    56,    -1,    -1,    59,    60,    -1,
      -1,    -1,    -1,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    -1,    -1,    76,    77,    78,    79,    80,    -1,
      -1,    -1,    84,    85,    86,    87,    -1,    89,    90,    -1,
      92,    -1,    94,    95,    96,    97,    98,    99,    -1,   101,
     102,    -1,   104,    -1,   106,    -1,    -1,    -1,   110,   111,
     112,    -1,    -1,   115,    -1,   117,    -1,   119,   120,   121,
      -1,   123,   124,   125,    -1,    -1,   128,   129,   130,   131,
     132,   133,   134,    -1,   136,    -1,   138,    -1,    -1,   141,
      -1,   143,   144,   145,    -1,    -1,   148,    -1,    -1,   151,
     152,   153,    -1,    -1,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,    -1,   168,    -1,   170,   171,
     172,   173,   174,    -1,   176,   177,    -1,    -1,   180,   181,
     182,    -1,    -1,   185,    -1,    -1,    -1,   189,   190,    -1,
      -1,   193,    -1,    -1,   196,   197,   198,   199,   200,   201,
      -1,   203,   204,   205,    -1,   207,   208,   209,   210,   211,
     212,   213,    -1,   215,   216,   217,   218,   219,   220,   221,
     222,    -1,   224,   225,   226,   227,   228,   229,   230,    -1,
     232,   233,   234,    -1,   236,   237,   238,   239,    -1,   241,
     242,    -1,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,    -1,   255,   256,   257,    -1,   259,   260,    -1,
     262,    -1,   264,   265,   266,   267,    -1,   269,   270,   271,
     272,    -1,    -1,   275,   276,   277,   278,   279,    -1,    -1,
     282,   283,   284,   285,   286,   287,    -1,   289,   290,   291,
      -1,    -1,   294,   295,   296,   297,   298,   299,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   337,    -1,    -1,    -1,    -1,
      -1,   343,    -1,    -1,    -1,   347,   348,   349,    -1,   351,
     352,   353,   354,   355,   356,     3,     4,     5,     6,     7,
       8,     9,    -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,
      18,    19,    20,    -1,    22,    23,    24,    -1,    26,    -1,
      28,    29,    -1,    31,    32,    33,    34,    -1,    -1,    37,
      38,    39,    40,    -1,    42,    43,    44,    45,    -1,    -1,
      -1,    49,    50,    51,    -1,    53,    54,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    -1,    -1,    -1,    66,    67,
      68,    69,    70,    71,    72,    73,    -1,    -1,    76,    77,
      78,    79,    80,    -1,    -1,    -1,    84,    85,    86,    87,
      -1,    89,    90,    -1,    92,    -1,    94,    95,    -1,    97,
      98,    -1,    -1,   101,   102,    -1,   104,    -1,   106,    -1,
      -1,    -1,   110,   111,   112,    -1,    -1,   115,    -1,   117,
      -1,   119,   120,   121,    -1,   123,   124,   125,    -1,    -1,
     128,   129,   130,   131,   132,   133,   134,    -1,   136,    -1,
     138,    -1,    -1,   141,    -1,   143,   144,   145,    -1,    -1,
     148,    -1,    -1,   151,   152,   153,    -1,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,    -1,
     168,    -1,   170,   171,   172,   173,    -1,    -1,   176,   177,
      -1,    -1,    -1,   181,   182,    -1,    -1,   185,    -1,    -1,
      -1,   189,   190,    -1,    -1,   193,    -1,    -1,    -1,   197,
     198,   199,   200,   201,    -1,    -1,   204,   205,    -1,   207,
     208,   209,   210,   211,   212,   213,    -1,   215,   216,   217,
     218,   219,   220,   221,   222,    -1,   224,    -1,   226,   227,
     228,   229,   230,    -1,   232,   233,   234,    -1,   236,   237,
     238,   239,    -1,   241,   242,    -1,   244,   245,   246,   247,
     248,   249,   250,   251,    -1,   253,    -1,   255,   256,   257,
      -1,   259,   260,    -1,   262,    -1,   264,    -1,   266,    -1,
      -1,   269,   270,   271,   272,    -1,    -1,   275,   276,   277,
     278,   279,    -1,    -1,   282,   283,   284,   285,   286,   287,
      -1,   289,   290,   291,    -1,    -1,   294,   295,   296,   297,
     298,   299,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   337,
     338,   339,    -1,    -1,    -1,   343,   344,    -1,    -1,   347,
     348,   349,    -1,   351,   352,   353,   354,   355,   356,     3,
       4,     5,     6,     7,     8,     9,    -1,    11,    -1,    -1,
      -1,    -1,    -1,    -1,    18,    19,    20,    -1,    22,    23,
      24,    -1,    26,    -1,    28,    29,    -1,    31,    32,    33,
      34,    -1,    -1,    37,    38,    39,    40,    -1,    42,    43,
      44,    45,    46,    -1,    -1,    49,    50,    51,    -1,    53,
      54,    55,    56,    -1,    -1,    59,    60,    -1,    -1,    -1,
      -1,    -1,    66,    67,    68,    69,    70,    71,    72,    73,
      -1,    -1,    76,    77,    78,    79,    80,    -1,    -1,    -1,
      84,    85,    86,    87,    -1,    89,    90,    -1,    92,    -1,
      94,    95,    96,    97,    98,    99,    -1,   101,   102,    -1,
     104,    -1,   106,    -1,    -1,    -1,   110,   111,   112,    -1,
      -1,   115,    -1,   117,    -1,   119,   120,   121,    -1,   123,
     124,   125,    -1,    -1,   128,   129,   130,   131,   132,   133,
     134,    -1,   136,    -1,   138,    -1,    -1,   141,    -1,   143,
     144,   145,    -1,    -1,   148,    -1,    -1,   151,   152,   153,
      -1,    -1,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,    -1,   168,   169,   170,   171,   172,   173,
     174,    -1,   176,   177,    -1,    -1,   180,   181,   182,    -1,
      -1,   185,   186,    -1,    -1,   189,   190,    -1,    -1,   193,
      -1,    -1,   196,   197,   198,   199,   200,   201,    -1,   203,
     204,   205,    -1,   207,   208,   209,   210,   211,   212,   213,
      -1,   215,   216,   217,   218,   219,   220,   221,   222,    -1,
     224,   225,   226,   227,   228,   229,   230,    -1,   232,   233,
     234,    -1,   236,   237,   238,   239,    -1,   241,   242,    -1,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
      -1,   255,   256,   257,    -1,   259,   260,    -1,   262,    -1,
     264,   265,   266,   267,    -1,   269,   270,   271,   272,    -1,
      -1,   275,   276,   277,   278,   279,    -1,    -1,   282,   283,
     284,   285,   286,   287,    -1,   289,   290,   291,    -1,    -1,
     294,   295,   296,   297,   298,   299,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,     9,    -1,    11,    -1,
      -1,    -1,    -1,    -1,    -1,    18,    19,    20,    -1,    22,
      23,    24,    -1,    26,    -1,    28,    29,    -1,    31,    32,
      33,    34,    -1,   337,    37,    38,    39,    40,    -1,    42,
      43,    44,    45,    46,    -1,    -1,    49,    50,    51,   353,
      53,    54,    55,    56,    -1,    -1,    59,    60,    -1,    -1,
      -1,    -1,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    -1,    -1,    76,    77,    78,    79,    80,    -1,    -1,
      -1,    84,    85,    86,    87,    -1,    89,    90,    -1,    92,
      -1,    94,    95,    96,    97,    98,    99,    -1,   101,   102,
      -1,   104,    -1,   106,    -1,    -1,    -1,   110,   111,   112,
      -1,    -1,   115,    -1,   117,    -1,   119,   120,   121,    -1,
     123,   124,   125,    -1,    -1,   128,   129,   130,   131,   132,
     133,   134,    -1,   136,    -1,   138,    -1,    -1,   141,    -1,
     143,   144,   145,    -1,    -1,   148,    -1,    -1,   151,   152,
     153,    -1,    -1,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,    -1,   168,    -1,   170,   171,   172,
     173,   174,    -1,   176,   177,    -1,    -1,   180,   181,   182,
      -1,    -1,   185,    -1,    -1,    -1,   189,   190,    -1,    -1,
     193,    -1,    -1,   196,   197,   198,   199,   200,   201,    -1,
     203,   204,   205,    -1,   207,   208,   209,   210,   211,   212,
     213,    -1,   215,   216,   217,   218,   219,   220,   221,   222,
      -1,   224,   225,   226,   227,   228,   229,   230,    -1,   232,
     233,   234,    -1,   236,   237,   238,   239,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,    -1,   255,   256,   257,    -1,   259,   260,    -1,   262,
      -1,   264,   265,   266,   267,    -1,   269,   270,   271,   272,
      -1,    -1,   275,   276,   277,   278,   279,    -1,    -1,   282,
     283,   284,   285,   286,   287,    -1,   289,   290,   291,    -1,
      -1,   294,   295,   296,   297,   298,   299,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,     8,     9,    -1,    11,
      -1,    -1,    -1,    -1,    -1,    -1,    18,    19,    20,    -1,
      22,    23,    24,    -1,    26,    -1,    28,    29,    -1,    31,
      32,    33,    34,    -1,   337,    37,    38,    39,    40,    -1,
      42,    43,    44,    45,    -1,    -1,    -1,    49,    50,    51,
     353,    53,    54,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    -1,    -1,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    -1,    -1,    76,    77,    78,    79,    80,    -1,
      -1,    -1,    84,    85,    86,    87,    -1,    89,    90,    -1,
      92,    -1,    94,    95,    -1,    97,    98,    -1,    -1,   101,
     102,    -1,   104,    -1,   106,    -1,    -1,    -1,   110,   111,
     112,    -1,    -1,   115,    -1,   117,    -1,   119,   120,   121,
      -1,   123,   124,   125,    -1,    -1,   128,   129,   130,   131,
     132,   133,   134,    -1,   136,    -1,   138,    -1,    -1,   141,
      -1,   143,   144,   145,    -1,    -1,   148,    -1,    -1,   151,
     152,   153,    -1,    -1,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,    -1,   168,    -1,   170,   171,
     172,   173,    -1,    -1,   176,   177,    -1,    -1,    -1,   181,
     182,    -1,    -1,   185,    -1,    -1,    -1,   189,   190,    -1,
      -1,   193,    -1,    -1,    -1,   197,   198,   199,   200,   201,
      -1,    -1,   204,   205,    -1,   207,   208,   209,   210,   211,
     212,   213,    -1,   215,   216,   217,   218,   219,   220,   221,
     222,    -1,   224,    -1,   226,   227,   228,   229,   230,    -1,
     232,   233,   234,    -1,   236,   237,   238,   239,    -1,   241,
     242,    -1,   244,   245,   246,   247,   248,   249,   250,   251,
      -1,   253,    -1,   255,   256,   257,    -1,   259,   260,    -1,
     262,    -1,   264,    -1,   266,    -1,    -1,   269,   270,   271,
     272,    -1,    -1,   275,   276,   277,   278,   279,    -1,    -1,
     282,   283,   284,   285,   286,   287,    -1,   289,   290,   291,
      -1,    -1,   294,   295,   296,   297,   298,   299,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,     9,    -1,
      11,    -1,    -1,    -1,    -1,    -1,    -1,    18,    19,    20,
      -1,    22,    23,    24,    -1,    26,    -1,    28,    29,    -1,
      31,    32,    33,    34,    -1,   337,    37,    38,    39,    40,
      -1,    42,    43,    44,    45,    46,    -1,    -1,    49,    50,
      51,   353,    53,    54,    55,    56,    -1,    -1,    59,    60,
      -1,    -1,    -1,    -1,    -1,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    -1,    76,    77,    78,    79,    80,
      -1,    -1,    -1,    84,    85,    86,    87,    -1,    89,    90,
      -1,    92,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,   102,    -1,   104,    -1,   106,    -1,    -1,    -1,   110,
     111,   112,    -1,    -1,   115,    -1,   117,    -1,   119,   120,
     121,    -1,   123,   124,   125,    -1,    -1,   128,   129,   130,
     131,   132,   133,   134,    -1,   136,    -1,   138,    -1,    -1,
     141,    -1,   143,   144,   145,    -1,    -1,   148,    -1,    -1,
     151,   152,   153,    -1,    -1,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,    -1,   168,    -1,   170,
     171,   172,   173,   174,    -1,   176,   177,    -1,    -1,   180,
     181,   182,   183,    -1,   185,    -1,   187,    -1,   189,   190,
      -1,    -1,   193,    -1,    -1,   196,   197,   198,   199,   200,
     201,    -1,   203,   204,   205,    -1,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,   216,   217,   218,   219,   220,
     221,   222,    -1,   224,   225,   226,   227,   228,   229,   230,
      -1,   232,   233,   234,    -1,   236,   237,   238,   239,    -1,
     241,   242,    -1,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,    -1,   255,   256,   257,    -1,   259,   260,
      -1,   262,    -1,   264,   265,   266,   267,   268,   269,   270,
     271,   272,    -1,    -1,   275,   276,   277,   278,   279,    -1,
      -1,   282,   283,   284,   285,   286,   287,    -1,   289,   290,
     291,    -1,    -1,   294,   295,   296,   297,   298,   299,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   337,   338,   339,    -1,
      -1,    -1,    -1,   344,     3,     4,     5,     6,     7,     8,
       9,   352,    11,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      19,    20,    -1,    22,    23,    24,    -1,    26,    -1,    28,
      29,    -1,    31,    32,    33,    34,    -1,    -1,    37,    38,
      39,    40,    -1,    42,    43,    44,    45,    46,    -1,    -1,
      49,    50,    51,    -1,    53,    54,    55,    56,    -1,    -1,
      59,    60,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    -1,    -1,    76,    77,    78,
      79,    80,    -1,    -1,    -1,    84,    85,    86,    87,    -1,
      89,    90,    -1,    92,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,   102,    -1,   104,    -1,   106,    -1,    -1,
      -1,   110,   111,   112,    -1,    -1,   115,    -1,   117,    -1,
     119,   120,   121,    -1,   123,   124,   125,    -1,    -1,   128,
     129,   130,   131,   132,   133,   134,    -1,   136,    -1,   138,
      -1,    -1,   141,    -1,   143,   144,   145,    -1,    -1,   148,
      -1,    -1,   151,   152,   153,    -1,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,    -1,   168,
      -1,   170,   171,   172,   173,   174,    -1,   176,   177,    -1,
      -1,   180,   181,   182,   183,    -1,   185,    -1,   187,    -1,
     189,   190,    -1,    -1,   193,    -1,    -1,   196,   197,   198,
     199,   200,   201,    -1,   203,   204,   205,    -1,   207,   208,
     209,   210,   211,   212,   213,    -1,   215,   216,   217,   218,
     219,   220,   221,   222,    -1,   224,   225,   226,   227,   228,
     229,   230,    -1,   232,   233,   234,    -1,   236,   237,   238,
     239,    -1,   241,   242,    -1,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,    -1,   255,   256,   257,    -1,
     259,   260,    -1,   262,    -1,   264,   265,   266,   267,   268,
     269,   270,   271,   272,    -1,    -1,   275,   276,   277,   278,
     279,    -1,    -1,   282,   283,   284,   285,   286,   287,    -1,
     289,   290,   291,    -1,    -1,   294,   295,   296,   297,   298,
     299,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   337,   338,
     339,    -1,    -1,    -1,    -1,   344,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   352,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      19,    20,    -1,    22,    23,    24,    -1,    26,    -1,    28,
      29,    -1,    31,    32,    33,    34,    -1,    -1,    37,    38,
      39,    40,    -1,    42,    43,    44,    45,    46,    -1,    -1,
      49,    50,    51,    -1,    53,    54,    55,    56,    -1,    -1,
      59,    60,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    -1,    -1,    76,    77,    78,
      79,    80,    -1,    -1,    -1,    84,    85,    86,    87,    -1,
      89,    90,    -1,    92,    -1,    94,    95,    96,    97,    98,
      99,    -1,   101,   102,    -1,   104,    -1,   106,    -1,   108,
      -1,   110,   111,   112,    -1,    -1,   115,    -1,   117,    -1,
     119,   120,   121,   122,   123,   124,   125,    -1,    -1,   128,
     129,   130,   131,   132,   133,   134,    -1,   136,    -1,   138,
      -1,    -1,   141,    -1,   143,   144,   145,    -1,    -1,   148,
      -1,    -1,   151,   152,   153,    -1,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,    -1,   168,
      -1,   170,   171,   172,   173,   174,    -1,   176,   177,    -1,
      -1,   180,   181,   182,    -1,    -1,   185,    -1,    -1,    -1,
     189,   190,    -1,    -1,   193,    -1,    -1,   196,   197,   198,
     199,   200,   201,    -1,   203,   204,   205,    -1,   207,   208,
     209,   210,   211,   212,   213,    -1,   215,   216,   217,   218,
     219,   220,   221,   222,    -1,   224,   225,   226,   227,   228,
     229,   230,    -1,   232,   233,   234,    -1,   236,   237,   238,
     239,    -1,   241,   242,    -1,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,    -1,   255,   256,   257,    -1,
     259,   260,    -1,   262,    -1,   264,   265,   266,   267,    -1,
     269,   270,   271,   272,    -1,    -1,   275,   276,   277,   278,
     279,    -1,    -1,   282,   283,   284,   285,   286,   287,    -1,
     289,   290,   291,    -1,    -1,   294,   295,   296,   297,   298,
     299,    -1,    -1,    -1,   303,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   337,    -1,
      -1,    -1,    -1,    -1,    -1,   344,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   352,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      19,    20,    -1,    22,    23,    24,    -1,    26,    -1,    28,
      29,    -1,    31,    32,    33,    34,    -1,    -1,    37,    38,
      39,    40,    -1,    42,    43,    44,    45,    46,    -1,    -1,
      49,    50,    51,    -1,    53,    54,    55,    56,    -1,    -1,
      59,    60,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    -1,    -1,    76,    77,    78,
      79,    80,    -1,    -1,    -1,    84,    85,    86,    87,    -1,
      89,    90,    -1,    92,    -1,    94,    95,    96,    97,    98,
      99,    -1,   101,   102,    -1,   104,    -1,   106,    -1,   108,
      -1,   110,   111,   112,    -1,    -1,   115,    -1,   117,    -1,
     119,   120,   121,   122,   123,   124,   125,    -1,    -1,   128,
     129,   130,   131,   132,   133,   134,    -1,   136,    -1,   138,
      -1,    -1,   141,    -1,   143,   144,   145,    -1,    -1,   148,
      -1,    -1,   151,   152,   153,    -1,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,    -1,   168,
      -1,   170,   171,   172,   173,   174,    -1,   176,   177,    -1,
      -1,   180,   181,   182,    -1,    -1,   185,    -1,    -1,    -1,
     189,   190,    -1,    -1,   193,    -1,    -1,   196,   197,   198,
     199,   200,   201,    -1,   203,   204,   205,    -1,   207,   208,
     209,   210,   211,   212,   213,    -1,   215,   216,   217,   218,
     219,   220,   221,   222,    -1,   224,   225,   226,   227,   228,
     229,   230,    -1,   232,   233,   234,    -1,   236,   237,   238,
     239,    -1,   241,   242,    -1,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,    -1,   255,   256,   257,    -1,
     259,   260,    -1,   262,    -1,   264,   265,   266,   267,    -1,
     269,   270,   271,   272,    -1,    -1,   275,   276,   277,   278,
     279,    -1,    -1,   282,   283,   284,   285,   286,   287,    -1,
     289,   290,   291,    -1,    -1,   294,   295,   296,   297,   298,
     299,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   337,    -1,
      -1,    -1,    -1,    -1,    -1,   344,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   352,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      19,    20,    -1,    22,    23,    24,    -1,    26,    -1,    28,
      29,    -1,    31,    32,    33,    34,    -1,    -1,    37,    38,
      39,    40,    -1,    42,    43,    44,    45,    46,    -1,    -1,
      49,    50,    51,    -1,    53,    54,    55,    56,    -1,    -1,
      59,    60,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    -1,    -1,    76,    77,    78,
      79,    80,    -1,    -1,    -1,    84,    85,    86,    87,    -1,
      89,    90,    -1,    92,    -1,    94,    95,    96,    97,    98,
      99,    -1,   101,   102,    -1,   104,    -1,   106,    -1,   108,
      -1,   110,   111,   112,    -1,    -1,   115,    -1,   117,    -1,
     119,   120,   121,   122,   123,   124,   125,    -1,    -1,   128,
     129,   130,   131,   132,   133,   134,    -1,   136,    -1,   138,
      -1,    -1,   141,    -1,   143,   144,   145,    -1,    -1,   148,
      -1,    -1,   151,   152,   153,    -1,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,    -1,   168,
      -1,   170,   171,   172,   173,   174,    -1,   176,   177,    -1,
      -1,   180,   181,   182,    -1,    -1,   185,    -1,    -1,    -1,
     189,   190,    -1,    -1,   193,    -1,    -1,   196,   197,   198,
     199,   200,   201,    -1,   203,   204,   205,    -1,   207,   208,
     209,   210,   211,   212,   213,    -1,   215,   216,   217,   218,
     219,   220,   221,   222,    -1,   224,   225,   226,   227,   228,
     229,   230,    -1,   232,   233,   234,    -1,   236,   237,   238,
     239,    -1,   241,   242,    -1,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,    -1,   255,   256,   257,    -1,
     259,   260,    -1,   262,    -1,   264,   265,   266,   267,    -1,
     269,   270,   271,   272,    -1,    -1,   275,   276,   277,   278,
     279,    -1,    -1,   282,   283,   284,   285,   286,   287,    -1,
     289,   290,   291,    -1,    -1,   294,   295,   296,   297,   298,
     299,    14,    -1,    -1,    -1,    -1,    -1,    20,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    14,    -1,    -1,    -1,    -1,    -1,    20,    -1,    -1,
      -1,    -1,    25,    -1,    47,    -1,    -1,    -1,   337,    -1,
      -1,    -1,    -1,    -1,    14,   344,    -1,    -1,    -1,    -1,
      20,    -1,    -1,   352,    47,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      20,    -1,    -1,    -1,    -1,    25,    -1,    47,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,    -1,    -1,
      -1,    -1,    -1,    20,    -1,    -1,    -1,    47,    25,    -1,
      -1,    -1,    -1,    -1,    -1,   118,    -1,    14,    -1,   122,
      -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,
      47,    -1,    -1,    -1,    -1,   118,   139,   140,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      47,    -1,    -1,    -1,    -1,    -1,   139,   140,   118,    -1,
      -1,    -1,   122,    -1,    20,    -1,   149,    -1,    -1,    25,
      -1,    -1,   175,    -1,    -1,   178,    -1,    -1,   118,   139,
     140,    -1,   122,    -1,    -1,    -1,   189,    -1,   191,   149,
      -1,    47,   175,    -1,    -1,   178,    -1,    -1,    -1,   139,
     140,   118,    -1,    -1,    -1,   122,   189,    -1,   191,   149,
      -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,   178,    -1,
      -1,   118,   139,   140,    -1,   122,    -1,    -1,    -1,   189,
      -1,   191,   149,    -1,    -1,   175,    -1,   240,   178,    -1,
      -1,    -1,   139,   140,    -1,    -1,    -1,    -1,    -1,   189,
      -1,   191,   149,    -1,    -1,   258,    -1,   240,   175,    -1,
      -1,   178,   118,    -1,    -1,    -1,   122,    -1,    -1,    -1,
      -1,    -1,   189,    -1,   191,    -1,    -1,    -1,   175,    -1,
     240,   178,    -1,   139,   140,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   189,   149,   191,    -1,    -1,    -1,    -1,    -1,
     240,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   178,   240,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,
     343,    -1,    -1,   240,   347,   348,   349,    -1,   351,   352,
     353,   354,   355,   356,    -1,    -1,    -1,    -1,    -1,   362,
     343,    -1,    -1,    -1,   347,   348,   349,    -1,   351,   352,
     353,   354,   355,   356,    -1,    -1,    -1,    -1,   361,   362,
      -1,    -1,    -1,   343,   240,    -1,    -1,   347,   348,   349,
      -1,   351,   352,   353,   354,   355,   356,    -1,    -1,    -1,
      -1,   361,   362,   343,    -1,    -1,    -1,   347,   348,   349,
      -1,   351,   352,   353,   354,   355,   356,    -1,    -1,   359,
      -1,    -1,   362,    -1,    -1,    -1,   343,    -1,    -1,    -1,
     347,   348,   349,    -1,   351,   352,   353,   354,   355,   356,
      -1,    -1,    14,    -1,   361,   362,   343,    -1,    20,    -1,
     347,   348,   349,    25,   351,   352,   353,   354,   355,   356,
      -1,    -1,    20,    -1,    -1,   362,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    47,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,   343,    -1,    47,
      -1,    -1,   348,   349,    -1,   351,   352,   353,   354,   355,
     356,    -1,    -1,    47,    -1,    -1,   362,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    -1,    47,    -1,   118,    25,    92,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    -1,
     118,    -1,    -1,    25,   122,    -1,    -1,   139,   140,    47,
      -1,    -1,    -1,    -1,   118,    -1,    -1,   149,   122,    -1,
      -1,   139,   140,    -1,    -1,    47,    -1,    -1,    -1,    92,
      -1,   149,    -1,    -1,    -1,   139,   140,    -1,    -1,    -1,
      -1,    -1,    -1,   175,    -1,   149,   178,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    92,   118,    -1,   189,    -1,   122,
     178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,   189,    -1,    -1,   178,    -1,   139,   140,    -1,    -1,
     118,    -1,    -1,    -1,   122,   189,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
     122,   139,   140,    -1,    -1,    -1,    -1,    -1,   240,    -1,
      -1,   149,    -1,    -1,    -1,   178,    -1,   139,   140,    -1,
      -1,    -1,   240,    -1,    -1,    -1,   189,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   240,    -1,    -1,    -1,
     178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   189,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   240,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   240,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   343,    -1,    -1,    -1,   347,   348,   349,   240,   351,
     352,   353,   354,   355,   356,   343,    -1,    -1,    -1,    -1,
     362,    -1,    -1,   351,   352,   353,   354,   355,   356,   343,
      -1,    -1,    -1,    -1,   362,    -1,    -1,   351,   352,   353,
     354,   355,   356,    -1,    -1,    -1,    -1,    -1,   362,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     343,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   351,   352,
     353,   354,   355,   356,    -1,    -1,    -1,    -1,    -1,   362,
      -1,    -1,    -1,    -1,    -1,   343,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   351,   352,   353,   354,   355,   356,    -1,
      -1,   343,    -1,    -1,   362,    -1,    -1,    -1,    -1,   351,
     352,   353,   354,   355,   356,    -1,    -1,    -1,    -1,    -1,
     362,     3,     4,     5,     6,     7,     8,     9,    -1,    11,
      -1,    -1,    -1,    -1,    -1,    -1,    18,    19,    20,    -1,
      22,    23,    24,    -1,    26,    -1,    28,    29,    -1,    31,
      32,    33,    34,    -1,    -1,    37,    38,    39,    40,    -1,
      42,    43,    44,    45,    46,    -1,    -1,    49,    50,    51,
      -1,    53,    54,    55,    56,    -1,    -1,    59,    60,    -1,
      -1,    -1,    -1,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    -1,    -1,    76,    77,    78,    79,    80,    -1,
      -1,    -1,    84,    85,    86,    87,    -1,    89,    90,    -1,
      92,    -1,    94,    95,    96,    97,    98,    99,    -1,   101,
     102,    -1,   104,    -1,   106,    -1,    -1,    -1,   110,   111,
     112,    -1,    -1,   115,    -1,   117,    -1,   119,   120,   121,
      -1,   123,   124,   125,    -1,    -1,   128,   129,   130,   131,
     132,   133,   134,    -1,   136,    -1,   138,    -1,    -1,   141,
      -1,   143,   144,   145,    -1,    -1,   148,    -1,    -1,   151,
     152,   153,    -1,    -1,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,    -1,   168,    -1,   170,   171,
     172,   173,   174,    -1,   176,   177,    -1,    -1,   180,   181,
     182,    -1,    -1,   185,    -1,    -1,    -1,   189,   190,    -1,
      -1,   193,    -1,    -1,   196,   197,   198,   199,   200,   201,
      -1,   203,   204,   205,    -1,   207,   208,   209,   210,   211,
     212,   213,    -1,   215,   216,   217,   218,   219,   220,   221,
     222,    -1,   224,   225,   226,   227,   228,   229,   230,    -1,
     232,   233,   234,    -1,   236,   237,   238,   239,    -1,   241,
     242,    -1,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,    -1,   255,   256,   257,    -1,   259,   260,    -1,
     262,    -1,   264,   265,   266,   267,    -1,   269,   270,   271,
     272,    -1,    -1,   275,   276,   277,   278,   279,    -1,    -1,
     282,   283,   284,   285,   286,   287,    -1,   289,   290,   291,
      -1,    -1,   294,   295,   296,   297,   298,   299,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   337,   338,   339,    -1,   341,
     342,    -1,   344,     3,     4,     5,     6,     7,     8,     9,
      -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,    18,    19,
      20,    -1,    22,    23,    24,    -1,    26,    -1,    28,    29,
      -1,    31,    32,    33,    34,    -1,    -1,    37,    38,    39,
      40,    -1,    42,    43,    44,    45,    46,    -1,    -1,    49,
      50,    51,    -1,    53,    54,    55,    56,    -1,    -1,    59,
      60,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    -1,    76,    77,    78,    79,
      80,    -1,    -1,    -1,    84,    85,    86,    87,    -1,    89,
      90,    -1,    92,    -1,    94,    95,    96,    97,    98,    99,
      -1,   101,   102,    -1,   104,    -1,   106,    -1,    -1,    -1,
     110,   111,   112,    -1,    -1,   115,    -1,   117,    -1,   119,
     120,   121,    -1,   123,   124,   125,    -1,    -1,   128,   129,
     130,   131,   132,   133,   134,    -1,   136,    -1,   138,    -1,
      -1,   141,    -1,   143,   144,   145,    -1,    -1,   148,    -1,
      -1,   151,   152,   153,    -1,    -1,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,    -1,   168,    -1,
     170,   171,   172,   173,   174,    -1,   176,   177,    -1,    -1,
     180,   181,   182,    -1,    -1,   185,    -1,    -1,    -1,   189,
     190,    -1,    -1,   193,    -1,    -1,   196,   197,   198,   199,
     200,   201,    -1,   203,   204,   205,    -1,   207,   208,   209,
     210,   211,   212,   213,    -1,   215,   216,   217,   218,   219,
     220,   221,   222,    -1,   224,   225,   226,   227,   228,   229,
     230,    -1,   232,   233,   234,    -1,   236,   237,   238,   239,
      -1,   241,   242,    -1,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,    -1,   255,   256,   257,    -1,   259,
     260,    -1,   262,    -1,   264,   265,   266,   267,    -1,   269,
     270,   271,   272,    -1,    -1,   275,   276,   277,   278,   279,
      -1,    -1,   282,   283,   284,   285,   286,   287,    -1,   289,
     290,   291,    -1,    -1,   294,   295,   296,   297,   298,   299,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
       8,     9,    -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,
      18,    19,    20,    -1,    22,    23,    24,   337,    26,   339,
      28,    29,    -1,    31,    32,    33,    34,    -1,    -1,    37,
      38,    39,    40,    -1,    42,    43,    44,    45,    46,    -1,
      -1,    49,    50,    51,    -1,    53,    54,    55,    56,    -1,
      -1,    59,    60,    -1,    -1,    -1,    -1,    -1,    66,    67,
      68,    69,    70,    71,    72,    73,    -1,    -1,    76,    77,
      78,    79,    80,    -1,    -1,    -1,    84,    85,    86,    87,
      -1,    89,    90,    -1,    92,    -1,    94,    95,    96,    97,
      98,    99,    -1,   101,   102,    -1,   104,    -1,   106,    -1,
      -1,    -1,   110,   111,   112,    -1,    -1,   115,    -1,   117,
      -1,   119,   120,   121,    -1,   123,   124,   125,    -1,    -1,
     128,   129,   130,   131,   132,   133,   134,    -1,   136,    -1,
     138,    -1,    -1,   141,    -1,   143,   144,   145,    -1,    -1,
     148,    -1,    -1,   151,   152,   153,    -1,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,    -1,
     168,    -1,   170,   171,   172,   173,   174,    -1,   176,   177,
      -1,    -1,   180,   181,   182,    -1,    -1,   185,    -1,    -1,
      -1,   189,   190,    -1,    -1,   193,    -1,    -1,   196,   197,
     198,   199,   200,   201,    -1,   203,   204,   205,    -1,   207,
     208,   209,   210,   211,   212,   213,    -1,   215,   216,   217,
     218,   219,   220,   221,   222,    -1,   224,   225,   226,   227,
     228,   229,   230,    -1,   232,   233,   234,    -1,   236,   237,
     238,   239,    -1,   241,   242,    -1,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,    -1,   255,   256,   257,
      -1,   259,   260,    -1,   262,    -1,   264,   265,   266,   267,
      -1,   269,   270,   271,   272,    -1,    -1,   275,   276,   277,
     278,   279,    -1,    -1,   282,   283,   284,   285,   286,   287,
      -1,   289,   290,   291,    -1,    -1,   294,   295,   296,   297,
     298,   299,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   337,
      -1,   339,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    -1,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   295,   296,   297,   298,   299,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,     9,
      -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      -1,    31,    32,    33,    34,    -1,   337,    37,    38,    39,
      40,    -1,    42,    43,    44,    45,    46,    -1,    -1,    49,
      50,    51,    -1,    53,    54,    55,    56,    -1,    -1,    59,
      60,    61,    -1,    -1,    -1,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    -1,    -1,    76,    77,    78,    79,
      80,    -1,    -1,    -1,    84,    85,    86,    87,    -1,    89,
      90,    -1,    92,    -1,    94,    95,    96,    97,    98,    99,
      -1,   101,   102,    -1,   104,    -1,   106,   107,    -1,   109,
     110,   111,   112,    -1,    -1,   115,    -1,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,   132,   133,   134,    -1,   136,    -1,   138,   139,
     140,   141,   142,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,    -1,    -1,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,    -1,   176,   177,   178,    -1,
     180,   181,   182,    -1,    -1,   185,   186,    -1,    -1,   189,
     190,    -1,    -1,   193,   194,   195,   196,   197,   198,   199,
     200,   201,    -1,   203,   204,   205,    -1,   207,   208,   209,
     210,   211,   212,   213,    -1,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,    -1,   232,   233,   234,    -1,   236,   237,   238,   239,
     240,   241,   242,    -1,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,    -1,   255,   256,   257,    -1,   259,
     260,    -1,   262,    -1,   264,   265,   266,   267,    -1,   269,
     270,   271,   272,    -1,    -1,   275,   276,   277,   278,   279,
      -1,    -1,   282,   283,   284,   285,   286,   287,   288,   289,
     290,   291,    -1,    -1,   294,   295,   296,   297,   298,   299,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
       9,    -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      19,    20,    -1,    22,    23,    24,    -1,    26,    -1,    28,
      29,    30,    31,    32,    33,    34,    -1,   337,    37,    38,
      39,    40,    -1,    42,    43,    44,    45,    46,    -1,    -1,
      49,    50,    51,    -1,    53,    54,    55,    56,    -1,    -1,
      59,    60,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    -1,    -1,    76,    77,    78,
      79,    80,    -1,    -1,    -1,    84,    85,    86,    87,    -1,
      89,    90,    -1,    92,    -1,    94,    95,    96,    97,    98,
      99,    -1,   101,   102,    -1,   104,    -1,   106,    -1,    -1,
      -1,   110,   111,   112,    -1,    -1,   115,    -1,   117,    -1,
     119,   120,   121,    -1,   123,   124,   125,    -1,    -1,   128,
     129,   130,   131,   132,   133,   134,    -1,   136,    -1,   138,
      -1,    -1,   141,    -1,   143,   144,   145,    -1,    -1,   148,
      -1,    -1,   151,   152,   153,    -1,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,    -1,   168,
     169,   170,   171,   172,   173,   174,    -1,   176,   177,    -1,
      -1,   180,   181,   182,    -1,    -1,   185,   186,    -1,    -1,
     189,   190,    -1,    -1,   193,    -1,    -1,   196,   197,   198,
     199,   200,   201,    -1,   203,   204,   205,    -1,   207,   208,
     209,   210,   211,   212,   213,    -1,   215,   216,   217,   218,
     219,   220,   221,   222,    -1,   224,   225,   226,   227,   228,
     229,   230,    -1,   232,   233,   234,    -1,   236,   237,   238,
     239,    -1,   241,   242,    -1,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,    -1,   255,   256,   257,    -1,
     259,   260,    -1,   262,    -1,   264,   265,   266,   267,    -1,
     269,   270,   271,   272,    -1,    -1,   275,   276,   277,   278,
     279,    -1,    -1,   282,   283,   284,   285,   286,   287,    -1,
     289,   290,   291,    -1,    -1,   294,   295,   296,   297,   298,
     299,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   330,   331,   332,   333,   334,    -1,    -1,   337,     3,
       4,     5,     6,     7,     8,     9,    -1,    11,    -1,    -1,
      -1,    -1,    -1,    -1,    18,    19,    20,    -1,    22,    23,
      24,    -1,    26,    -1,    28,    29,    -1,    31,    32,    33,
      34,    -1,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    -1,    49,    50,    51,    52,    53,
      54,    55,    56,    -1,    -1,    59,    60,    -1,    -1,    -1,
      -1,    -1,    66,    67,    68,    69,    70,    71,    72,    73,
      -1,    -1,    76,    77,    78,    79,    80,    -1,    -1,    -1,
      84,    85,    86,    87,    -1,    89,    90,    -1,    92,    -1,
      94,    95,    96,    97,    98,    99,    -1,   101,   102,    -1,
     104,   105,   106,    -1,    -1,    -1,   110,   111,   112,    -1,
      -1,   115,    -1,   117,    -1,   119,   120,   121,    -1,   123,
     124,   125,    -1,    -1,   128,   129,   130,   131,   132,   133,
     134,    -1,   136,    -1,   138,    -1,    -1,   141,    -1,   143,
     144,   145,    -1,    -1,   148,   149,    -1,   151,   152,   153,
      -1,    -1,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,    -1,   168,    -1,   170,   171,   172,   173,
     174,    -1,   176,   177,    -1,    -1,   180,   181,   182,    -1,
      -1,   185,    -1,    -1,    -1,   189,   190,    -1,    -1,   193,
      -1,    -1,   196,   197,   198,   199,   200,   201,    -1,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
      -1,   215,   216,   217,   218,   219,   220,   221,   222,    -1,
     224,   225,   226,   227,   228,   229,   230,    -1,   232,   233,
     234,    -1,   236,   237,   238,   239,    -1,   241,   242,    -1,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
      -1,   255,   256,   257,    -1,   259,   260,    -1,   262,    -1,
     264,   265,   266,   267,    -1,   269,   270,   271,   272,    -1,
     274,   275,   276,   277,   278,   279,    -1,    -1,   282,   283,
     284,   285,   286,   287,    -1,   289,   290,   291,    -1,    -1,
     294,   295,   296,   297,   298,   299,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,     9,    -1,    11,    12,
      13,    -1,    -1,    -1,    -1,    18,    19,    20,    -1,    22,
      23,    24,    -1,    26,    -1,    28,    29,    -1,    31,    32,
      33,    34,    -1,   337,    37,    38,    39,    40,    -1,    42,
      43,    44,    45,    46,    -1,    -1,    49,    50,    51,    -1,
      53,    54,    55,    56,    -1,    -1,    59,    60,    -1,    -1,
      -1,    -1,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    -1,    -1,    76,    77,    78,    79,    80,    -1,    -1,
      -1,    84,    85,    86,    87,    -1,    89,    90,    -1,    92,
      -1,    94,    95,    96,    97,    98,    99,    -1,   101,   102,
      -1,   104,    -1,   106,    -1,    -1,    -1,   110,   111,   112,
      -1,    -1,   115,    -1,   117,    -1,   119,   120,   121,    -1,
     123,   124,   125,    -1,    -1,   128,   129,   130,   131,   132,
     133,   134,    -1,   136,    -1,   138,    -1,    -1,   141,    -1,
     143,   144,   145,    -1,    -1,   148,    -1,    -1,   151,   152,
     153,    -1,    -1,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,    -1,   168,   169,   170,   171,   172,
     173,   174,    -1,   176,   177,    -1,    -1,   180,   181,   182,
      -1,    -1,   185,   186,    -1,    -1,   189,   190,    -1,    -1,
     193,    -1,    -1,   196,   197,   198,   199,   200,   201,    -1,
     203,   204,   205,    -1,   207,   208,   209,   210,   211,   212,
     213,    -1,   215,   216,   217,   218,   219,   220,   221,   222,
      -1,   224,   225,   226,   227,   228,   229,   230,    -1,   232,
     233,   234,    -1,   236,   237,   238,   239,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,    -1,   255,   256,   257,    -1,   259,   260,    -1,   262,
      -1,   264,   265,   266,   267,    -1,   269,   270,   271,   272,
      -1,    -1,   275,   276,   277,   278,   279,    -1,    -1,   282,
     283,   284,   285,   286,   287,    -1,   289,   290,   291,    -1,
      -1,   294,   295,   296,   297,   298,   299,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,     8,     9,    -1,    11,
      -1,    -1,    -1,    -1,    -1,    -1,    18,    19,    20,    -1,
      22,    23,    24,    -1,    26,    -1,    28,    29,    -1,    31,
      32,    33,    34,    -1,   337,    37,    38,    39,    40,    -1,
      42,    43,    44,    45,    46,    -1,    -1,    49,    50,    51,
      -1,    53,    54,    55,    56,    -1,    -1,    59,    60,    -1,
      -1,    -1,    -1,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    -1,    -1,    76,    77,    78,    79,    80,    -1,
      -1,    -1,    84,    85,    86,    87,    -1,    89,    90,    -1,
      92,    -1,    94,    95,    96,    97,    98,    99,    -1,   101,
     102,    -1,   104,    -1,   106,    -1,    -1,    -1,   110,   111,
     112,    -1,    -1,   115,    -1,   117,    -1,   119,   120,   121,
      -1,   123,   124,   125,    -1,    -1,   128,   129,   130,   131,
     132,   133,   134,    -1,   136,    -1,   138,    -1,    -1,   141,
      -1,   143,   144,   145,    -1,    -1,   148,    -1,    -1,   151,
     152,   153,    -1,    -1,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,    -1,   168,   169,   170,   171,
     172,   173,   174,    -1,   176,   177,    -1,    -1,   180,   181,
     182,    -1,    -1,   185,   186,    -1,   188,   189,   190,    -1,
      -1,   193,    -1,    -1,   196,   197,   198,   199,   200,   201,
      -1,   203,   204,   205,    -1,   207,   208,   209,   210,   211,
     212,   213,    -1,   215,   216,   217,   218,   219,   220,   221,
     222,    -1,   224,   225,   226,   227,   228,   229,   230,    -1,
     232,   233,   234,    -1,   236,   237,   238,   239,    -1,   241,
     242,    -1,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,    -1,   255,   256,   257,    -1,   259,   260,    -1,
     262,    -1,   264,   265,   266,   267,    -1,   269,   270,   271,
     272,    -1,    -1,   275,   276,   277,   278,   279,    -1,    -1,
     282,   283,   284,   285,   286,   287,    -1,   289,   290,   291,
      -1,    -1,   294,   295,   296,   297,   298,   299,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,     9,    -1,
      11,    -1,    -1,    -1,    -1,    -1,    -1,    18,    19,    20,
      -1,    22,    23,    24,    -1,    26,    -1,    28,    29,    -1,
      31,    32,    33,    34,    -1,   337,    37,    38,    39,    40,
      -1,    42,    43,    44,    45,    46,    -1,    -1,    49,    50,
      51,    -1,    53,    54,    55,    56,    -1,    -1,    59,    60,
      -1,    -1,    -1,    -1,    -1,    66,    67,    68,    69,    70,
      71,    72,    73,    -1,    -1,    76,    77,    78,    79,    80,
      -1,    -1,    -1,    84,    85,    86,    87,    -1,    89,    90,
      -1,    92,    -1,    94,    95,    96,    97,    98,    99,    -1,
     101,   102,    -1,   104,    -1,   106,    -1,    -1,    -1,   110,
     111,   112,    -1,    -1,   115,    -1,   117,    -1,   119,   120,
     121,    -1,   123,   124,   125,    -1,    -1,   128,   129,   130,
     131,   132,   133,   134,    -1,   136,    -1,   138,    -1,    -1,
     141,    -1,   143,   144,   145,    -1,    -1,   148,    -1,    -1,
     151,   152,   153,    -1,    -1,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,    -1,   168,   169,   170,
     171,   172,   173,   174,    -1,   176,   177,    -1,    -1,   180,
     181,   182,    -1,    -1,   185,   186,    -1,    -1,   189,   190,
      -1,    -1,   193,    -1,    -1,   196,   197,   198,   199,   200,
     201,    -1,   203,   204,   205,    -1,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,   216,   217,   218,   219,   220,
     221,   222,    -1,   224,   225,   226,   227,   228,   229,   230,
      -1,   232,   233,   234,    -1,   236,   237,   238,   239,    -1,
     241,   242,    -1,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,    -1,   259,   260,
      -1,   262,    -1,   264,   265,   266,   267,    -1,   269,   270,
     271,   272,    -1,    -1,   275,   276,   277,   278,   279,    -1,
      -1,   282,   283,   284,   285,   286,   287,    -1,   289,   290,
     291,    -1,    -1,   294,   295,   296,   297,   298,   299,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,     9,
      -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,    18,    19,
      20,    -1,    22,    23,    24,    -1,    26,    -1,    28,    29,
      -1,    31,    32,    33,    34,    -1,   337,    37,    38,    39,
      40,    -1,    42,    43,    44,    45,    46,    -1,    -1,    49,
      50,    51,    -1,    53,    54,    55,    56,    -1,    -1,    59,
      60,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    -1,    -1,    76,    77,    78,    79,
      80,    -1,    -1,    -1,    84,    85,    86,    87,    -1,    89,
      90,    -1,    92,    -1,    94,    95,    96,    97,    98,    99,
      -1,   101,   102,    -1,   104,    -1,   106,    -1,    -1,    -1,
     110,   111,   112,    -1,    -1,   115,    -1,   117,    -1,   119,
     120,   121,    -1,   123,   124,   125,    -1,    -1,   128,   129,
     130,   131,   132,   133,   134,    -1,   136,    -1,   138,    -1,
      -1,   141,    -1,   143,   144,   145,    -1,    -1,   148,    -1,
      -1,   151,   152,   153,    -1,    -1,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,    -1,   168,   169,
     170,   171,   172,   173,   174,    -1,   176,   177,    -1,    -1,
     180,   181,   182,    -1,    -1,   185,   186,    -1,    -1,   189,
     190,    -1,    -1,   193,    -1,    -1,   196,   197,   198,   199,
     200,   201,    -1,   203,   204,   205,    -1,   207,   208,   209,
     210,   211,   212,   213,    -1,   215,   216,   217,   218,   219,
     220,   221,   222,    -1,   224,   225,   226,   227,   228,   229,
     230,    -1,   232,   233,   234,    -1,   236,   237,   238,   239,
      -1,   241,   242,    -1,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   257,    -1,   259,
     260,    -1,   262,    -1,   264,   265,   266,   267,    -1,   269,
     270,   271,   272,    -1,    -1,   275,   276,   277,   278,   279,
      -1,    -1,   282,   283,   284,   285,   286,   287,    -1,   289,
     290,   291,    -1,    -1,   294,   295,   296,   297,   298,   299,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
       9,    -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      19,    20,    -1,    22,    23,    24,    -1,    26,    -1,    28,
      29,    -1,    31,    32,    33,    34,    -1,   337,    37,    38,
      39,    40,    -1,    42,    43,    44,    45,    46,    -1,    -1,
      49,    50,    51,    -1,    53,    54,    55,    56,    -1,    -1,
      59,    60,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    -1,    -1,    76,    77,    78,
      79,    80,    -1,    -1,    -1,    84,    85,    86,    87,    -1,
      89,    90,    -1,    92,    -1,    94,    95,    96,    97,    98,
      99,    -1,   101,   102,    -1,   104,    -1,   106,    -1,    -1,
      -1,   110,   111,   112,    -1,    -1,   115,    -1,   117,    -1,
     119,   120,   121,    -1,   123,   124,   125,    -1,    -1,   128,
     129,   130,   131,   132,   133,   134,    -1,   136,    -1,   138,
      -1,    -1,   141,    -1,   143,   144,   145,    -1,    -1,   148,
      -1,    -1,   151,   152,   153,    -1,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,    -1,   168,
     169,   170,   171,   172,   173,   174,    -1,   176,   177,    -1,
      -1,   180,   181,   182,    -1,    -1,   185,   186,    -1,    -1,
     189,   190,    -1,    -1,   193,    -1,    -1,   196,   197,   198,
     199,   200,   201,    -1,   203,   204,   205,    -1,   207,   208,
     209,   210,   211,   212,   213,    -1,   215,   216,   217,   218,
     219,   220,   221,   222,    -1,   224,   225,   226,   227,   228,
     229,   230,    -1,   232,   233,   234,    -1,   236,   237,   238,
     239,    -1,   241,   242,    -1,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,    -1,   255,   256,   257,    -1,
     259,   260,    -1,   262,    -1,   264,   265,   266,   267,    -1,
     269,   270,   271,   272,    -1,    -1,   275,   276,   277,   278,
     279,    -1,   281,   282,   283,   284,   285,   286,   287,    -1,
     289,   290,   291,    -1,    -1,   294,   295,   296,   297,   298,
     299,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
       8,     9,    -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,
      18,    19,    20,    -1,    22,    23,    24,    -1,    26,    -1,
      28,    29,    -1,    31,    32,    33,    34,    -1,   337,    37,
      38,    39,    40,    -1,    42,    43,    44,    45,    46,    -1,
      -1,    49,    50,    51,    -1,    53,    54,    55,    56,    -1,
      -1,    59,    60,    -1,    -1,    -1,    -1,    -1,    66,    67,
      68,    69,    70,    71,    72,    73,    -1,    -1,    76,    77,
      78,    79,    80,    -1,    -1,    -1,    84,    85,    86,    87,
      -1,    89,    90,    -1,    92,    -1,    94,    95,    96,    97,
      98,    99,    -1,   101,   102,    -1,   104,    -1,   106,    -1,
      -1,    -1,   110,   111,   112,    -1,    -1,   115,    -1,   117,
      -1,   119,   120,   121,    -1,   123,   124,   125,    -1,    -1,
     128,   129,   130,   131,   132,   133,   134,    -1,   136,    -1,
     138,    -1,    -1,   141,    -1,   143,   144,   145,    -1,    -1,
     148,    -1,    -1,   151,   152,   153,    -1,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,    -1,
     168,   169,   170,   171,   172,   173,   174,    -1,   176,   177,
      -1,    -1,   180,   181,   182,    -1,    -1,   185,   186,    -1,
      -1,   189,   190,    -1,    -1,   193,    -1,    -1,   196,   197,
     198,   199,   200,   201,    -1,   203,   204,   205,    -1,   207,
     208,   209,   210,   211,   212,   213,    -1,   215,   216,   217,
     218,   219,   220,   221,   222,    -1,   224,   225,   226,   227,
     228,   229,   230,    -1,   232,   233,   234,    -1,   236,   237,
     238,   239,    -1,   241,   242,    -1,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,    -1,   255,   256,   257,
      -1,   259,   260,    -1,   262,    -1,   264,   265,   266,   267,
      -1,   269,   270,   271,   272,    -1,    -1,   275,   276,   277,
     278,   279,    -1,    -1,   282,   283,   284,   285,   286,   287,
      -1,   289,   290,   291,    -1,    -1,   294,   295,   296,   297,
     298,   299,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,     9,    -1,    11,    -1,    -1,    -1,    -1,    -1,
      -1,    18,    19,    20,    -1,    22,    23,    24,    -1,    26,
      -1,    28,    29,    -1,    31,    32,    33,    34,    -1,   337,
      37,    38,    39,    40,    -1,    42,    43,    44,    45,    46,
      -1,    -1,    49,    50,    51,    -1,    53,    54,    55,    56,
      -1,    -1,    59,    60,    -1,    -1,    -1,    -1,    -1,    66,
      67,    68,    69,    70,    71,    72,    73,    -1,    -1,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    85,    86,
      87,    -1,    89,    90,    -1,    92,    -1,    94,    95,    96,
      97,    98,    99,    -1,   101,   102,    -1,   104,    -1,   106,
      -1,    -1,    -1,   110,   111,   112,    -1,    -1,   115,    -1,
     117,    -1,   119,   120,   121,    -1,   123,   124,   125,    -1,
      -1,   128,   129,   130,   131,   132,   133,   134,    -1,   136,
      -1,   138,    -1,    -1,   141,    -1,   143,   144,   145,    -1,
      -1,   148,    -1,    -1,   151,   152,   153,    -1,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      -1,   168,   169,   170,   171,   172,   173,   174,    -1,   176,
     177,    -1,    -1,   180,   181,   182,    -1,    -1,   185,   186,
      -1,    -1,   189,   190,    -1,    -1,   193,    -1,    -1,   196,
     197,   198,   199,   200,   201,    -1,   203,   204,   205,    -1,
     207,   208,   209,   210,   211,   212,   213,    -1,   215,   216,
     217,   218,   219,   220,   221,   222,    -1,   224,   225,   226,
     227,   228,   229,   230,    -1,   232,   233,   234,    -1,   236,
     237,   238,   239,    -1,   241,   242,    -1,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,    -1,   255,   256,
     257,    -1,   259,   260,    -1,   262,    -1,   264,   265,   266,
     267,    -1,   269,   270,   271,   272,    -1,    -1,   275,   276,
     277,   278,   279,    -1,    -1,   282,   283,   284,   285,   286,
     287,    -1,   289,   290,   291,    -1,    -1,   294,   295,   296,
     297,   298,   299,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    19,    20,    -1,    22,    23,    24,    -1,
      26,    -1,    28,    29,    -1,    31,    32,    33,    34,    -1,
     337,    37,    38,    39,    40,    -1,    42,    43,    44,    45,
      46,    -1,    -1,    49,    50,    51,    -1,    53,    54,    55,
      56,    -1,    -1,    59,    60,    -1,    -1,    -1,    -1,    -1,
      66,    67,    68,    69,    70,    71,    72,    73,    -1,    -1,
      76,    77,    78,    79,    80,    -1,    -1,    -1,    84,    85,
      86,    87,    -1,    89,    90,    -1,    92,    -1,    94,    95,
      96,    97,    98,    99,    -1,   101,   102,    -1,   104,    -1,
     106,    -1,    -1,    -1,   110,   111,   112,    -1,    -1,   115,
      -1,   117,    -1,   119,   120,   121,    -1,   123,   124,   125,
      -1,    -1,   128,   129,   130,   131,   132,   133,   134,    -1,
     136,    -1,   138,    -1,    -1,   141,    -1,   143,   144,   145,
      -1,    -1,   148,    -1,    -1,   151,   152,   153,    -1,    -1,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,    -1,   168,    -1,   170,   171,   172,   173,   174,    -1,
     176,   177,    -1,    -1,   180,   181,   182,    -1,    -1,   185,
      -1,    -1,    -1,   189,   190,    -1,    -1,   193,    -1,    -1,
     196,   197,   198,   199,   200,   201,    -1,   203,   204,   205,
      -1,   207,   208,   209,   210,   211,   212,   213,    -1,   215,
     216,   217,   218,   219,   220,   221,   222,    -1,   224,   225,
     226,   227,   228,   229,   230,    -1,   232,   233,   234,    -1,
     236,   237,   238,   239,    -1,   241,   242,    -1,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,    -1,   255,
     256,   257,    -1,   259,   260,    -1,   262,    -1,   264,   265,
     266,   267,    -1,   269,   270,   271,   272,    -1,    -1,   275,
     276,   277,   278,   279,    -1,    -1,   282,   283,   284,   285,
     286,   287,    -1,   289,   290,   291,    -1,    -1,   294,   295,
     296,   297,   298,   299,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    -1,    -1,    -1,
      -1,    -1,    -1,    18,    19,    20,    -1,    22,    23,    24,
      -1,    26,    -1,    28,    29,    -1,    31,    32,    33,    34,
      -1,   337,    37,    38,    39,    40,    -1,    42,    43,    44,
      45,    46,    -1,    -1,    49,    50,    51,    -1,    53,    54,
      55,    56,    -1,    -1,    59,    60,    -1,    -1,    -1,    -1,
      -1,    66,    67,    68,    69,    70,    71,    72,    73,    -1,
      -1,    76,    77,    78,    79,    80,    -1,    -1,    -1,    84,
      85,    86,    87,    -1,    89,    90,    -1,    92,    -1,    94,
      95,    96,    97,    98,    99,    -1,   101,   102,    -1,   104,
      -1,   106,    -1,    -1,    -1,   110,   111,   112,    -1,    -1,
     115,    -1,   117,    -1,   119,   120,   121,    -1,   123,   124,
     125,    -1,    -1,   128,   129,   130,   131,   132,   133,   134,
      -1,   136,    -1,   138,    -1,    -1,   141,    -1,   143,   144,
     145,    -1,    -1,   148,    -1,    -1,   151,   152,   153,    -1,
      -1,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,    -1,   168,    -1,   170,   171,   172,   173,   174,
      -1,   176,   177,    -1,    -1,   180,   181,   182,    -1,    -1,
     185,    -1,    -1,    -1,   189,   190,    -1,    -1,   193,    -1,
      -1,   196,   197,   198,   199,   200,   201,    -1,   203,   204,
     205,    -1,   207,   208,   209,   210,   211,   212,   213,    -1,
     215,   216,   217,   218,   219,   220,   221,   222,    -1,   224,
     225,   226,   227,   228,   229,   230,    -1,   232,   233,   234,
      -1,   236,   237,   238,   239,    -1,   241,   242,    -1,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,    -1,
     255,   256,   257,    -1,   259,   260,    -1,   262,    -1,   264,
     265,   266,   267,    -1,   269,   270,   271,   272,    -1,    -1,
     275,   276,   277,   278,   279,    -1,    -1,   282,   283,   284,
     285,   286,   287,    -1,   289,   290,   291,    -1,    -1,   294,
     295,   296,   297,   298,   299,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    -1,    -1,
      -1,    -1,    -1,    -1,    18,    19,    20,    -1,    22,    23,
      24,    -1,    26,    -1,    28,    29,    -1,    31,    32,    33,
      34,    -1,   337,    37,    38,    39,    40,    -1,    42,    43,
      44,    45,    46,    -1,    -1,    49,    50,    51,    -1,    53,
      54,    55,    56,    -1,    -1,    59,    60,    -1,    -1,    -1,
      -1,    -1,    66,    67,    68,    69,    70,    71,    72,    73,
      -1,    -1,    76,    77,    78,    79,    80,    -1,    -1,    -1,
      84,    85,    86,    87,    -1,    89,    90,    -1,    92,    -1,
      94,    95,    96,    97,    98,    99,    -1,   101,   102,    -1,
     104,    -1,   106,    -1,    -1,    -1,   110,   111,   112,    -1,
      -1,   115,    -1,   117,    -1,   119,   120,   121,    -1,   123,
     124,   125,    -1,    -1,   128,   129,   130,   131,   132,   133,
     134,    -1,   136,    -1,   138,    -1,    -1,   141,    -1,   143,
     144,   145,    -1,    -1,   148,    -1,    -1,   151,   152,   153,
      -1,    -1,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,    -1,   168,    -1,   170,   171,   172,   173,
     174,    -1,   176,   177,    -1,    -1,   180,   181,   182,    -1,
      -1,   185,    -1,    -1,    -1,   189,   190,    -1,    -1,   193,
      -1,    -1,   196,   197,   198,   199,   200,   201,    -1,   203,
     204,   205,    -1,   207,   208,   209,   210,   211,   212,   213,
      -1,   215,   216,   217,   218,   219,   220,   221,   222,    -1,
     224,   225,   226,   227,   228,   229,   230,    -1,   232,   233,
     234,    -1,   236,   237,   238,   239,    -1,   241,   242,    -1,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
      -1,   255,   256,   257,    -1,   259,   260,    -1,   262,    -1,
     264,   265,   266,   267,    -1,   269,   270,   271,   272,    -1,
      -1,   275,   276,   277,   278,   279,    -1,    -1,   282,   283,
     284,   285,   286,   287,    -1,   289,   290,   291,    -1,    -1,
     294,   295,   296,   297,   298,   299,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,     9,    -1,    11,    -1,
      -1,    -1,    -1,    -1,    -1,    18,    19,    20,    21,    22,
      23,    24,    -1,    26,    -1,    28,    29,    -1,    31,    32,
      33,    34,    -1,   337,    37,    38,    39,    40,    -1,    42,
      43,    44,    45,    46,    -1,    -1,    49,    50,    51,    -1,
      53,    54,    55,    56,    -1,    -1,    59,    60,    -1,    -1,
      -1,    -1,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    -1,    -1,    76,    77,    78,    79,    80,    -1,    -1,
      -1,    84,    85,    86,    87,    -1,    89,    90,    -1,    92,
      -1,    94,    95,    96,    97,    98,    99,    -1,   101,   102,
      -1,   104,    -1,   106,    -1,    -1,    -1,   110,   111,   112,
      -1,    -1,   115,    -1,   117,    -1,   119,   120,   121,    -1,
     123,   124,   125,    -1,    -1,   128,   129,   130,   131,   132,
     133,   134,    -1,   136,    -1,   138,    -1,    -1,   141,    -1,
     143,   144,   145,    -1,    -1,   148,    -1,    -1,   151,   152,
     153,    -1,    -1,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,    -1,   168,    -1,   170,   171,   172,
     173,   174,    -1,   176,   177,    -1,    -1,   180,   181,   182,
      -1,    -1,   185,    -1,    -1,    -1,   189,   190,    -1,    -1,
     193,    -1,    -1,   196,   197,   198,   199,   200,   201,    -1,
     203,   204,   205,    -1,   207,   208,   209,   210,   211,   212,
     213,    -1,   215,   216,   217,   218,   219,   220,   221,   222,
      -1,   224,   225,   226,   227,   228,   229,   230,    -1,   232,
     233,   234,    -1,   236,   237,   238,   239,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,    -1,   255,   256,   257,    -1,   259,   260,    -1,   262,
      -1,   264,   265,   266,   267,    -1,   269,   270,   271,   272,
      -1,    -1,   275,   276,   277,   278,   279,    -1,    -1,   282,
     283,   284,   285,   286,   287,    -1,   289,   290,   291,    -1,
      -1,   294,   295,   296,   297,   298,   299,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,     8,     9,    -1,    11,
      -1,    -1,    -1,    -1,    -1,    -1,    18,    19,    20,    -1,
      22,    23,    24,    -1,    26,    -1,    28,    29,    -1,    31,
      32,    33,    34,    -1,   337,    37,    38,    39,    40,    -1,
      42,    43,    44,    45,    46,    -1,    -1,    49,    50,    51,
      -1,    53,    54,    55,    56,    -1,    -1,    59,    60,    -1,
      -1,    -1,    -1,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    -1,    -1,    76,    77,    78,    79,    80,    -1,
      -1,    -1,    84,    85,    86,    87,    -1,    89,    90,    -1,
      92,    -1,    94,    95,    96,    97,    98,    99,    -1,   101,
     102,    -1,   104,    -1,   106,    -1,    -1,    -1,   110,   111,
     112,    -1,   114,   115,    -1,   117,    -1,   119,   120,   121,
      -1,   123,   124,   125,    -1,    -1,   128,   129,   130,   131,
     132,   133,   134,    -1,   136,    -1,   138,    -1,    -1,   141,
      -1,   143,   144,   145,    -1,    -1,   148,    -1,    -1,   151,
     152,   153,    -1,    -1,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,    -1,   168,    -1,   170,   171,
     172,   173,   174,    -1,   176,   177,    -1,    -1,   180,   181,
     182,    -1,    -1,   185,    -1,    -1,    -1,   189,   190,    -1,
      -1,   193,    -1,    -1,   196,   197,   198,   199,   200,   201,
      -1,   203,   204,   205,    -1,   207,   208,   209,   210,   211,
     212,   213,    -1,   215,   216,   217,   218,   219,   220,   221,
     222,    -1,   224,   225,   226,   227,   228,   229,   230,    -1,
     232,   233,   234,    -1,   236,   237,   238,   239,    -1,   241,
     242,    -1,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,    -1,   255,   256,   257,    -1,   259,   260,    -1,
     262,    -1,   264,   265,   266,   267,    -1,   269,   270,   271,
     272,    -1,    -1,   275,   276,   277,   278,   279,    -1,    -1,
     282,   283,   284,   285,   286,   287,    -1,   289,   290,   291,
      -1,    -1,   294,   295,   296,   297,   298,   299,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,     9,    -1,
      11,    -1,    -1,    -1,    -1,    16,    -1,    18,    19,    20,
      -1,    22,    23,    24,    -1,    26,    -1,    28,    29,    -1,
      31,    32,    33,    34,    -1,   337,    37,    38,    39,    40,
      -1,    42,    43,    44,    45,    46,    -1,    -1,    49,    50,
      51,    -1,    53,    54,    55,    56,    -1,    -1,    59,    60,
      -1,    -1,    -1,    -1,    -1,    66,    67,    68,    69,    70,
      71,    72,    73,    -1,    -1,    76,    77,    78,    79,    80,
      -1,    -1,    -1,    84,    85,    86,    87,    -1,    89,    90,
      -1,    92,    -1,    94,    95,    96,    97,    98,    99,    -1,
     101,   102,    -1,   104,    -1,   106,    -1,    -1,    -1,   110,
     111,   112,    -1,    -1,   115,    -1,   117,    -1,   119,   120,
     121,    -1,   123,   124,   125,    -1,    -1,   128,   129,   130,
     131,   132,   133,   134,    -1,   136,    -1,   138,    -1,    -1,
     141,    -1,   143,   144,   145,    -1,    -1,   148,    -1,    -1,
     151,   152,   153,    -1,    -1,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,    -1,   168,    -1,   170,
     171,   172,   173,   174,    -1,   176,   177,    -1,    -1,   180,
     181,   182,    -1,    -1,   185,    -1,    -1,    -1,   189,   190,
      -1,    -1,   193,    -1,    -1,   196,   197,   198,   199,   200,
     201,    -1,   203,   204,   205,    -1,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,   216,   217,   218,   219,   220,
     221,   222,    -1,   224,   225,   226,   227,   228,   229,   230,
      -1,   232,   233,   234,    -1,   236,   237,   238,   239,    -1,
     241,   242,    -1,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,    -1,   255,   256,   257,    -1,   259,   260,
      -1,   262,    -1,   264,   265,   266,   267,    -1,   269,   270,
     271,   272,    -1,    -1,   275,   276,   277,   278,   279,    -1,
      -1,   282,   283,   284,   285,   286,   287,    -1,   289,   290,
     291,    -1,    -1,   294,   295,   296,   297,   298,   299,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,     9,
      -1,    11,    -1,    -1,    -1,    -1,    16,    -1,    18,    19,
      20,    -1,    22,    23,    24,    -1,    26,    -1,    28,    29,
      -1,    31,    32,    33,    34,    -1,   337,    37,    38,    39,
      40,    -1,    42,    43,    44,    45,    46,    -1,    -1,    49,
      50,    51,    -1,    53,    54,    55,    56,    -1,    -1,    59,
      60,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    -1,    -1,    76,    77,    78,    79,
      80,    -1,    -1,    -1,    84,    85,    86,    87,    -1,    89,
      90,    -1,    92,    -1,    94,    95,    96,    97,    98,    99,
      -1,   101,   102,    -1,   104,    -1,   106,    -1,    -1,    -1,
     110,   111,   112,    -1,    -1,   115,    -1,   117,    -1,   119,
     120,   121,    -1,   123,   124,   125,    -1,    -1,   128,   129,
     130,   131,   132,   133,   134,    -1,   136,    -1,   138,    -1,
      -1,   141,    -1,   143,   144,   145,    -1,    -1,   148,    -1,
      -1,   151,   152,   153,    -1,    -1,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,    -1,   168,    -1,
     170,   171,   172,   173,   174,    -1,   176,   177,    -1,    -1,
     180,   181,   182,    -1,    -1,   185,    -1,    -1,    -1,   189,
     190,    -1,    -1,   193,    -1,    -1,   196,   197,   198,   199,
     200,   201,    -1,   203,   204,   205,    -1,   207,   208,   209,
     210,   211,   212,   213,    -1,   215,   216,   217,   218,   219,
     220,   221,   222,    -1,   224,   225,   226,   227,   228,   229,
     230,    -1,   232,   233,   234,    -1,   236,   237,   238,   239,
      -1,   241,   242,    -1,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,    -1,   255,   256,   257,    -1,   259,
     260,    -1,   262,    -1,   264,   265,   266,   267,    -1,   269,
     270,   271,   272,    -1,    -1,   275,   276,   277,   278,   279,
      -1,    -1,   282,   283,   284,   285,   286,   287,    -1,   289,
     290,   291,    -1,    -1,   294,   295,   296,   297,   298,   299,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
       9,    -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      19,    20,    -1,    22,    23,    24,    -1,    26,    -1,    28,
      29,    -1,    31,    32,    33,    34,    -1,   337,    37,    38,
      39,    40,    -1,    42,    43,    44,    45,    46,    -1,    -1,
      49,    50,    51,    -1,    53,    54,    55,    56,    -1,    -1,
      59,    60,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    -1,    76,    77,    78,
      79,    80,    -1,    -1,    -1,    84,    85,    86,    87,    -1,
      89,    90,    -1,    92,    -1,    94,    95,    96,    97,    98,
      99,    -1,   101,   102,    -1,   104,    -1,   106,    -1,    -1,
      -1,   110,   111,   112,    -1,    -1,   115,    -1,   117,    -1,
     119,   120,   121,    -1,   123,   124,   125,    -1,    -1,   128,
     129,   130,   131,   132,   133,   134,    -1,   136,    -1,   138,
      -1,    -1,   141,    -1,   143,   144,   145,    -1,    -1,   148,
      -1,    -1,   151,   152,   153,    -1,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,    -1,   168,
      -1,   170,   171,   172,   173,   174,    -1,   176,   177,    -1,
      -1,   180,   181,   182,    -1,    -1,   185,    -1,    -1,    -1,
     189,   190,    -1,    -1,   193,    -1,    -1,   196,   197,   198,
     199,   200,   201,    -1,   203,   204,   205,    -1,   207,   208,
     209,   210,   211,   212,   213,    -1,   215,   216,   217,   218,
     219,   220,   221,   222,    -1,   224,   225,   226,   227,   228,
     229,   230,    -1,   232,   233,   234,    -1,   236,   237,   238,
     239,    -1,   241,   242,    -1,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,    -1,   255,   256,   257,    -1,
     259,   260,    -1,   262,    -1,   264,   265,   266,   267,    -1,
     269,   270,   271,   272,    -1,    -1,   275,   276,   277,   278,
     279,    -1,    -1,   282,   283,   284,   285,   286,   287,    -1,
     289,   290,   291,    -1,    -1,   294,   295,   296,   297,   298,
     299,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
       8,     9,    -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,
      18,    19,    20,    -1,    22,    23,    24,    -1,    26,    -1,
      28,    29,    -1,    31,    32,    33,    34,    -1,   337,    37,
      38,    39,    40,    -1,    42,    43,    44,    45,    46,    -1,
      -1,    49,    50,    51,    -1,    53,    54,    55,    56,    -1,
      -1,    59,    60,    -1,    -1,    -1,    -1,    -1,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    76,    77,
      78,    79,    80,    -1,    -1,    -1,    84,    85,    86,    87,
      -1,    89,    90,    -1,    92,    -1,    94,    95,    96,    97,
      98,    99,    -1,   101,   102,    -1,   104,    -1,   106,    -1,
      -1,    -1,   110,   111,   112,    -1,    -1,   115,    -1,   117,
      -1,   119,   120,   121,    -1,   123,   124,   125,    -1,    -1,
     128,   129,   130,   131,   132,   133,   134,    -1,   136,    -1,
     138,    -1,    -1,   141,    -1,   143,   144,   145,    -1,    -1,
     148,    -1,    -1,   151,   152,   153,    -1,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,    -1,
     168,    -1,   170,   171,   172,   173,   174,    -1,   176,   177,
      -1,    -1,   180,   181,   182,    -1,    -1,   185,    -1,    -1,
      -1,   189,   190,    -1,    -1,   193,    -1,    -1,   196,   197,
     198,   199,   200,   201,    -1,   203,   204,   205,    -1,   207,
     208,   209,   210,   211,   212,   213,    -1,   215,   216,   217,
     218,   219,   220,   221,   222,    -1,   224,   225,   226,   227,
     228,   229,   230,    -1,   232,   233,   234,    -1,   236,   237,
     238,   239,    -1,   241,   242,    -1,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,    -1,   255,   256,   257,
      -1,   259,   260,    -1,   262,    -1,   264,   265,   266,   267,
      -1,   269,   270,   271,   272,    -1,    -1,   275,   276,   277,
     278,   279,    -1,    -1,   282,   283,   284,   285,   286,   287,
      -1,   289,   290,   291,    -1,    -1,   294,   295,   296,   297,
     298,   299,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,     9,    -1,    11,    -1,    -1,    -1,    -1,    -1,
      -1,    18,    19,    20,    -1,    22,    23,    24,    -1,    26,
      -1,    28,    29,    -1,    31,    32,    33,    34,    -1,   337,
      37,    38,    39,    40,    -1,    42,    43,    44,    45,    46,
      -1,    -1,    49,    50,    51,    -1,    53,    54,    55,    56,
      -1,    -1,    59,    60,    -1,    -1,    -1,    -1,    -1,    66,
      67,    68,    69,    70,    71,    72,    73,    -1,    -1,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    85,    86,
      87,    -1,    89,    90,    -1,    92,    -1,    94,    95,    96,
      97,    98,    99,    -1,   101,   102,    -1,   104,    -1,   106,
      -1,    -1,    -1,   110,   111,   112,    -1,    -1,   115,    -1,
     117,    -1,   119,   120,   121,    -1,   123,   124,   125,    -1,
      -1,   128,   129,   130,   131,   132,   133,   134,    -1,   136,
      -1,   138,    -1,    -1,   141,    -1,   143,   144,   145,    -1,
      -1,   148,    -1,    -1,   151,   152,   153,    -1,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      -1,   168,    -1,   170,   171,   172,   173,   174,    -1,   176,
     177,    -1,    -1,   180,   181,   182,    -1,    -1,   185,    -1,
      -1,    -1,   189,   190,    -1,    -1,   193,    -1,    -1,   196,
     197,   198,   199,   200,   201,    -1,   203,   204,   205,    -1,
     207,   208,   209,   210,   211,   212,   213,    -1,   215,   216,
     217,   218,   219,   220,   221,   222,    -1,   224,   225,   226,
     227,   228,   229,   230,    -1,   232,   233,   234,    -1,   236,
     237,   238,   239,    -1,   241,   242,    -1,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,    -1,   255,   256,
     257,    -1,   259,   260,    -1,   262,    -1,   264,   265,   266,
     267,    -1,   269,   270,   271,   272,    -1,    -1,   275,   276,
     277,   278,   279,    -1,    -1,   282,   283,   284,   285,   286,
     287,    -1,   289,   290,   291,    -1,    -1,   294,   295,   296,
     297,   298,   299,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,     7,     8,     9,    -1,    11,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    19,    20,    -1,    22,    23,    24,    -1,
      26,    -1,    28,    29,    -1,    31,    32,    33,    34,    -1,
     337,    37,    38,    39,    40,    -1,    42,    43,    44,    45,
      46,    -1,    -1,    49,    50,    51,    -1,    53,    54,    55,
      56,    -1,    -1,    59,    60,    -1,    -1,    -1,    -1,    -1,
      66,    67,    68,    69,    70,    71,    72,    73,    -1,    -1,
      76,    77,    78,    79,    80,    -1,    -1,    -1,    84,    85,
      86,    87,    -1,    89,    90,    -1,    92,    -1,    94,    95,
      96,    97,    98,    99,    -1,   101,   102,    -1,   104,    -1,
     106,    -1,    -1,    -1,   110,   111,   112,    -1,    -1,   115,
      -1,   117,    -1,   119,   120,   121,    -1,   123,   124,   125,
      -1,    -1,   128,   129,   130,   131,   132,   133,   134,    -1,
     136,    -1,   138,    -1,    -1,   141,    -1,   143,   144,   145,
      -1,    -1,   148,    -1,    -1,   151,   152,   153,    -1,    -1,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,    -1,   168,    -1,   170,   171,   172,   173,   174,    -1,
     176,   177,    -1,    -1,   180,   181,   182,    -1,    -1,   185,
      -1,    -1,    -1,   189,   190,    -1,    -1,   193,    -1,    -1,
     196,   197,   198,   199,   200,   201,    -1,   203,   204,   205,
      -1,   207,   208,   209,   210,   211,   212,   213,    -1,   215,
     216,   217,   218,   219,   220,   221,   222,    -1,   224,   225,
     226,   227,   228,   229,   230,    -1,   232,   233,   234,    -1,
     236,   237,   238,   239,    -1,   241,   242,    -1,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,    -1,   255,
     256,   257,    -1,   259,   260,    -1,   262,    -1,   264,   265,
     266,   267,    -1,   269,   270,   271,   272,    -1,    -1,   275,
     276,   277,   278,   279,    -1,    -1,   282,   283,   284,   285,
     286,   287,    -1,   289,   290,   291,    -1,    -1,   294,   295,
     296,   297,   298,   299,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,     9,    -1,    11,    -1,    -1,    -1,
      -1,    -1,    -1,    18,    19,    20,    -1,    22,    23,    24,
      -1,    26,    -1,    28,    29,    -1,    31,    32,    33,    34,
      -1,   337,    37,    38,    39,    40,    -1,    42,    43,    44,
      45,    46,    -1,    -1,    49,    50,    51,    -1,    53,    54,
      55,    56,    -1,    -1,    59,    60,    -1,    -1,    -1,    -1,
      -1,    66,    67,    68,    69,    70,    71,    72,    73,    -1,
      -1,    76,    77,    78,    79,    80,    -1,    -1,    -1,    84,
      85,    86,    87,    -1,    89,    90,    -1,    92,    -1,    94,
      95,    96,    97,    98,    99,    -1,   101,   102,    -1,   104,
      -1,   106,    -1,    -1,    -1,   110,   111,   112,    -1,    -1,
     115,    -1,   117,    -1,   119,   120,   121,    -1,   123,   124,
     125,    -1,    -1,   128,   129,   130,   131,   132,   133,   134,
      -1,   136,    -1,   138,    -1,    -1,   141,    -1,   143,   144,
     145,    -1,    -1,   148,    -1,    -1,   151,   152,   153,    -1,
      -1,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,    -1,   168,    -1,   170,   171,   172,   173,   174,
      -1,   176,   177,    -1,    -1,   180,   181,   182,    -1,    -1,
     185,    -1,    -1,    -1,   189,   190,    -1,    -1,   193,    -1,
      -1,   196,   197,   198,   199,   200,   201,    -1,   203,   204,
     205,    -1,   207,   208,   209,   210,   211,   212,   213,    -1,
     215,   216,   217,   218,   219,   220,   221,   222,    -1,   224,
     225,   226,   227,   228,   229,   230,    -1,   232,   233,   234,
      -1,   236,   237,   238,   239,    -1,   241,   242,    -1,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,    -1,
     255,   256,   257,    -1,   259,   260,    -1,   262,    -1,   264,
     265,   266,   267,    -1,   269,   270,   271,   272,    -1,    -1,
     275,   276,   277,   278,   279,    -1,    -1,   282,   283,   284,
     285,   286,   287,    -1,   289,   290,   291,    -1,    -1,   294,
     295,   296,   297,   298,   299,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,     9,    -1,    11,    -1,    -1,
      -1,    -1,    -1,    -1,    18,    19,    20,    -1,    22,    23,
      24,    -1,    26,    -1,    28,    29,    -1,    31,    32,    33,
      34,    -1,   337,    37,    38,    39,    40,    -1,    42,    43,
      44,    45,    46,    -1,    -1,    49,    50,    51,    -1,    53,
      54,    55,    56,    -1,    -1,    59,    60,    -1,    -1,    -1,
      -1,    -1,    66,    67,    68,    69,    70,    71,    72,    73,
      -1,    -1,    76,    77,    78,    79,    80,    -1,    -1,    -1,
      84,    85,    86,    87,    -1,    89,    90,    -1,    92,    -1,
      94,    95,    96,    97,    98,    99,    -1,   101,   102,    -1,
     104,    -1,   106,    -1,    -1,    -1,   110,   111,   112,    -1,
      -1,   115,    -1,   117,    -1,   119,   120,   121,    -1,   123,
     124,   125,    -1,    -1,   128,   129,   130,   131,   132,   133,
     134,    -1,   136,    -1,   138,    -1,    -1,   141,    -1,   143,
     144,   145,    -1,    -1,   148,    -1,    -1,   151,   152,   153,
      -1,    -1,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,    -1,   168,    -1,   170,   171,   172,   173,
     174,    -1,   176,   177,    -1,    -1,   180,   181,   182,    -1,
      -1,   185,    -1,    -1,    -1,   189,   190,    -1,    -1,   193,
      -1,    -1,   196,   197,   198,   199,   200,   201,    -1,   203,
     204,   205,    -1,   207,   208,   209,   210,   211,   212,   213,
      -1,   215,   216,   217,   218,   219,   220,   221,   222,    -1,
     224,   225,   226,   227,   228,   229,   230,    -1,   232,   233,
     234,    -1,   236,   237,   238,   239,    -1,   241,   242,    -1,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
      -1,   255,   256,   257,    -1,   259,   260,    -1,   262,    -1,
     264,   265,   266,   267,    -1,   269,   270,   271,   272,    -1,
      -1,   275,   276,   277,   278,   279,    -1,    -1,   282,   283,
     284,   285,   286,   287,    -1,   289,   290,   291,    -1,    -1,
     294,   295,   296,   297,   298,   299,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,     9,    -1,    11,    -1,
      -1,    -1,    -1,    -1,    -1,    18,    19,    20,    -1,    22,
      23,    24,    -1,    26,    -1,    28,    29,    -1,    31,    32,
      33,    34,    -1,   337,    37,    38,    39,    40,    -1,    42,
      43,    44,    45,    -1,    -1,    -1,    49,    50,    51,    -1,
      53,    54,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      -1,    -1,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    -1,    -1,    76,    77,    78,    79,    80,    -1,    -1,
      -1,    84,    85,    86,    87,    -1,    89,    90,    -1,    92,
      -1,    94,    95,    -1,    97,    98,    -1,    -1,   101,   102,
      -1,   104,    -1,   106,    -1,    -1,    -1,   110,   111,   112,
      -1,    -1,   115,    -1,   117,    -1,   119,   120,   121,    -1,
     123,   124,   125,    -1,    -1,   128,   129,   130,   131,   132,
     133,   134,    -1,   136,    -1,   138,    -1,    -1,   141,    -1,
     143,   144,   145,    -1,    -1,   148,    -1,    -1,   151,   152,
     153,    -1,    -1,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,    -1,   168,    -1,   170,   171,   172,
     173,   174,    -1,   176,   177,    -1,    -1,    -1,   181,   182,
      -1,    -1,   185,    -1,    -1,    -1,   189,   190,    -1,    -1,
     193,    -1,    -1,    -1,   197,   198,   199,   200,   201,    -1,
      -1,   204,   205,    -1,   207,   208,   209,   210,   211,   212,
     213,    -1,   215,   216,   217,   218,   219,   220,   221,   222,
      -1,   224,    -1,   226,   227,   228,   229,   230,    -1,   232,
     233,   234,    -1,   236,   237,   238,   239,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
     253,    -1,   255,   256,   257,    -1,   259,   260,    -1,   262,
      -1,   264,    -1,   266,    -1,    -1,   269,   270,   271,   272,
      -1,    -1,   275,   276,   277,   278,   279,    -1,    -1,   282,
     283,   284,   285,   286,   287,    -1,   289,   290,   291,    -1,
      -1,   294,   295,   296,   297,   298,   299,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,     8,     9,    -1,    11,
      -1,    -1,    -1,    -1,    -1,    -1,    18,    19,    20,    -1,
      22,    23,    24,    -1,    26,    -1,    28,    29,    -1,    31,
      32,    33,    34,    -1,   337,    37,    38,    39,    40,    -1,
      42,    43,    44,    45,    -1,    -1,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    -1,    -1,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    -1,    -1,    76,    77,    78,    79,    80,    -1,
      -1,    -1,    84,    85,    86,    87,    -1,    89,    90,    -1,
      92,    -1,    94,    95,    -1,    97,    98,    -1,    -1,   101,
     102,    -1,   104,    -1,   106,    -1,    -1,    -1,   110,   111,
     112,    -1,    -1,   115,    -1,   117,    -1,   119,   120,   121,
     122,   123,   124,   125,    -1,    -1,   128,   129,   130,   131,
     132,   133,   134,    -1,   136,    -1,   138,    -1,    -1,   141,
      -1,   143,   144,   145,    -1,    -1,   148,    -1,    -1,   151,
     152,   153,    -1,    -1,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,    -1,   168,    -1,   170,   171,
     172,   173,    -1,    -1,   176,   177,    -1,    -1,    -1,   181,
     182,    -1,    -1,   185,    -1,    -1,    -1,   189,   190,    -1,
      -1,   193,    -1,    -1,    -1,   197,   198,   199,   200,   201,
      -1,    -1,   204,   205,    -1,   207,   208,   209,   210,   211,
     212,   213,    -1,   215,   216,   217,   218,   219,   220,   221,
     222,    -1,   224,    -1,   226,   227,   228,   229,   230,    -1,
     232,   233,   234,    -1,   236,   237,   238,   239,    -1,   241,
     242,    -1,   244,   245,   246,   247,   248,   249,   250,   251,
      -1,   253,    -1,   255,   256,   257,    -1,   259,   260,    -1,
     262,    -1,   264,    -1,   266,    -1,    -1,   269,   270,   271,
     272,    -1,    -1,   275,   276,   277,   278,   279,    -1,    -1,
     282,   283,   284,   285,   286,   287,    -1,   289,   290,   291,
      -1,    -1,   294,   295,   296,   297,   298,   299,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,     9,    -1,
      11,    -1,    -1,    -1,    -1,    -1,    -1,    18,    19,    20,
      -1,    22,    23,    24,    -1,    26,    -1,    28,    29,    -1,
      31,    32,    33,    34,    -1,   337,    37,    38,    39,    40,
      -1,    42,    43,    44,    45,    -1,    -1,    -1,    49,    50,
      51,    -1,    53,    54,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    -1,    -1,    -1,    66,    67,    68,    69,    70,
      71,    72,    73,    -1,    -1,    76,    77,    78,    79,    80,
      -1,    -1,    -1,    84,    85,    86,    87,    -1,    89,    90,
      -1,    92,    -1,    94,    95,    -1,    97,    98,    -1,    -1,
     101,   102,    -1,   104,    -1,   106,    -1,    -1,    -1,   110,
     111,   112,    -1,    -1,   115,    -1,   117,    -1,   119,   120,
     121,    -1,   123,   124,   125,    -1,    -1,   128,   129,   130,
     131,   132,   133,   134,    -1,   136,    -1,   138,    -1,    -1,
     141,    -1,   143,   144,   145,    -1,    -1,   148,    -1,    -1,
     151,   152,   153,    -1,    -1,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,    -1,   168,    -1,   170,
     171,   172,   173,   174,    -1,   176,   177,    -1,    -1,    -1,
     181,   182,    -1,    -1,   185,    -1,    -1,    -1,   189,   190,
      -1,    -1,   193,    -1,    -1,    -1,   197,   198,   199,   200,
     201,    -1,    -1,   204,   205,    -1,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,   216,   217,   218,   219,   220,
     221,   222,    -1,   224,    -1,   226,   227,   228,   229,   230,
      -1,   232,   233,   234,    -1,   236,   237,   238,   239,    -1,
     241,   242,    -1,   244,   245,   246,   247,   248,   249,   250,
     251,    -1,   253,    -1,   255,   256,   257,    -1,   259,   260,
      -1,   262,    -1,   264,    -1,   266,    -1,    -1,   269,   270,
     271,   272,    -1,    -1,   275,   276,   277,   278,   279,    -1,
      -1,   282,   283,   284,   285,   286,   287,    -1,   289,   290,
     291,    -1,    -1,   294,   295,   296,   297,   298,   299,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,     9,
      -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,    18,    19,
      20,    -1,    22,    23,    24,    -1,    26,    -1,    28,    29,
      -1,    31,    32,    33,    34,    -1,   337,    37,    38,    39,
      40,    -1,    42,    43,    44,    45,    -1,    -1,    -1,    49,
      50,    51,    -1,    53,    54,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    -1,    -1,    76,    77,    78,    79,
      80,    -1,    -1,    -1,    84,    85,    86,    87,    -1,    89,
      90,    -1,    92,    -1,    94,    95,    -1,    97,    98,    -1,
      -1,   101,   102,    -1,   104,    -1,   106,    -1,    -1,    -1,
     110,   111,   112,    -1,    -1,   115,    -1,   117,    -1,   119,
     120,   121,    -1,   123,   124,   125,    -1,    -1,   128,   129,
     130,   131,   132,   133,   134,    -1,   136,    -1,   138,    -1,
      -1,   141,    -1,   143,   144,   145,    -1,    -1,   148,    -1,
      -1,   151,   152,   153,    -1,    -1,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,    -1,   168,    -1,
     170,   171,   172,   173,    -1,    -1,   176,   177,    -1,    -1,
      -1,   181,   182,    -1,    -1,   185,    -1,    -1,    -1,   189,
     190,    -1,    -1,   193,    -1,    -1,    -1,   197,   198,   199,
     200,   201,    -1,    -1,   204,   205,    -1,   207,   208,   209,
     210,   211,   212,   213,    -1,   215,   216,   217,   218,   219,
     220,   221,   222,    -1,   224,    -1,   226,   227,   228,   229,
     230,    -1,   232,   233,   234,    -1,   236,   237,   238,   239,
      -1,   241,   242,    -1,   244,   245,   246,   247,   248,   249,
     250,   251,    -1,   253,    -1,   255,   256,   257,    -1,   259,
     260,    -1,   262,    -1,   264,    -1,   266,    -1,    -1,   269,
     270,   271,   272,    -1,    -1,   275,   276,   277,   278,   279,
      -1,    -1,   282,   283,   284,   285,   286,   287,    -1,   289,
     290,   291,    -1,    -1,   294,   295,   296,   297,   298,   299,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
       9,    -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      19,    20,    -1,    22,    23,    24,    -1,    26,    -1,    28,
      29,    -1,    31,    32,    33,    34,    -1,   337,    37,    38,
      39,    40,    -1,    42,    43,    44,    45,    -1,    -1,    -1,
      49,    50,    51,    -1,    53,    54,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    -1,    -1,    76,    77,    78,
      79,    80,    -1,    -1,    -1,    84,    85,    86,    87,    -1,
      89,    90,    -1,    92,    -1,    94,    95,    -1,    97,    98,
      -1,    -1,   101,   102,    -1,   104,    -1,   106,    -1,    -1,
      -1,   110,   111,   112,    -1,    -1,   115,    -1,   117,    -1,
     119,   120,   121,    -1,   123,   124,   125,    -1,    -1,   128,
     129,   130,   131,   132,   133,   134,    -1,   136,    -1,   138,
      -1,    -1,   141,    -1,   143,   144,   145,    -1,    -1,   148,
      -1,    -1,   151,   152,   153,    -1,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,    -1,   168,
      -1,   170,   171,   172,   173,    -1,    -1,   176,   177,    -1,
      -1,    -1,   181,   182,    -1,    -1,   185,    -1,    -1,    -1,
     189,   190,    -1,    -1,   193,    -1,    -1,    -1,   197,   198,
     199,   200,   201,    -1,    -1,   204,   205,    -1,   207,   208,
     209,   210,   211,   212,   213,    -1,   215,   216,   217,   218,
     219,   220,   221,   222,    -1,   224,    -1,   226,   227,   228,
     229,   230,    -1,   232,   233,   234,    -1,   236,    -1,   238,
     239,    -1,   241,   242,    -1,   244,   245,   246,   247,   248,
     249,   250,   251,    -1,   253,    -1,   255,   256,   257,    -1,
     259,   260,    -1,   262,    -1,   264,    -1,   266,    -1,    -1,
     269,   270,   271,   272,    -1,    -1,   275,   276,   277,   278,
     279,    -1,    -1,   282,   283,   284,   285,   286,   287,    -1,
     289,   290,   291,    -1,    -1,   294,   295,   296,   297,   298,
     299,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
       8,     9,    -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,
      18,    19,    20,    -1,    22,    23,    24,    -1,    26,    -1,
      28,    29,    -1,    31,    32,    33,    34,    -1,   337,    37,
      38,    39,    40,    -1,    42,    43,    44,    45,    -1,    -1,
      -1,    49,    50,    51,    -1,    53,    54,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    -1,    -1,    -1,    66,    67,
      68,    69,    70,    71,    72,    73,    -1,    -1,    76,    77,
      78,    79,    80,    -1,    -1,    -1,    84,    85,    86,    87,
      -1,    89,    90,    -1,    92,    -1,    94,    95,    -1,    97,
      98,    -1,    -1,   101,   102,    -1,   104,    -1,   106,    -1,
      -1,    -1,   110,   111,   112,    -1,    -1,   115,    -1,   117,
      -1,   119,   120,   121,    -1,   123,   124,   125,    -1,    -1,
     128,   129,   130,   131,   132,   133,   134,    -1,    -1,    -1,
     138,    -1,    -1,   141,    -1,   143,   144,   145,    -1,    -1,
     148,    -1,    -1,   151,   152,   153,    -1,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,    -1,
     168,    -1,   170,   171,   172,   173,    -1,    -1,   176,   177,
      -1,    -1,    -1,   181,   182,    -1,    -1,   185,    -1,    -1,
      -1,   189,   190,    -1,    -1,   193,    -1,    -1,    -1,   197,
     198,   199,   200,   201,    -1,    -1,   204,   205,    -1,   207,
     208,   209,   210,   211,   212,   213,    -1,   215,   216,   217,
     218,   219,   220,   221,   222,    -1,   224,    -1,   226,   227,
     228,   229,   230,    -1,   232,   233,   234,    -1,   236,    -1,
     238,   239,    -1,   241,   242,    -1,   244,   245,   246,   247,
     248,   249,   250,   251,    -1,   253,    -1,   255,   256,   257,
      -1,   259,   260,    -1,   262,    -1,   264,    -1,   266,    -1,
      -1,   269,   270,   271,   272,    -1,    -1,   275,   276,   277,
     278,   279,    -1,    -1,   282,   283,   284,   285,   286,   287,
      -1,   289,   290,   291,    -1,     9,   294,   295,   296,   297,
     298,   299,    -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    36,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,   337,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,    -1,
      74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   189,    -1,   191,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   209,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   227,    -1,    -1,    -1,    -1,   232,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     254,   255,    -1,   257,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   266,    -1,    -1,    -1,   270,   271,    -1,    -1,
     274,    -1,    -1,    -1,    -1,    -1,   280,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   309,   310
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned short yystos[] =
{
       0,     3,    11,    12,    13,    24,    42,    44,    45,    49,
      50,    56,    58,    70,    73,    78,    86,    91,    95,    97,
     101,   113,   131,   151,   152,   157,   164,   177,   205,   215,
     219,   222,   224,   231,   236,   239,   245,   269,   276,   278,
     282,   294,   360,   368,   369,   370,   371,   373,   374,   375,
     379,   382,   384,   385,   389,   400,   401,   402,   405,   406,
     410,   413,   415,   424,   425,   431,   454,   458,   464,   469,
     471,   484,   485,   486,   487,   495,   500,   501,   505,   506,
     509,   513,   514,   525,   533,   545,   546,   548,   551,   553,
     554,   557,   560,   568,   569,   570,   571,   572,   575,   576,
     577,   581,   582,   583,   585,   586,   587,   588,   589,   595,
     596,   599,   601,   602,   603,   607,   608,   611,   612,   615,
     616,   617,   618,   619,   620,   264,   296,   574,    68,   114,
     254,   266,   280,   309,   574,     3,     4,     5,     6,     7,
       8,     9,    11,    18,    19,    20,    22,    23,    24,    26,
      28,    29,    31,    32,    33,    34,    37,    38,    39,    40,
      42,    43,    44,    45,    46,    49,    50,    51,    53,    54,
      55,    56,    59,    60,    66,    67,    68,    69,    70,    71,
      72,    73,    76,    77,    78,    79,    80,    84,    85,    86,
      87,    89,    90,    92,    94,    95,    96,    97,    98,    99,
     101,   102,   104,   106,   110,   111,   112,   115,   117,   119,
     120,   121,   123,   124,   125,   128,   129,   130,   131,   132,
     133,   134,   136,   138,   141,   143,   144,   145,   148,   151,
     152,   153,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   168,   170,   171,   172,   173,   174,   176,
     177,   180,   181,   182,   185,   189,   190,   193,   196,   197,
     198,   199,   200,   201,   203,   204,   205,   207,   208,   209,
     210,   211,   212,   213,   215,   216,   217,   218,   219,   220,
     221,   222,   224,   225,   226,   227,   228,   229,   230,   232,
     233,   234,   236,   237,   238,   239,   241,   242,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   255,   256,
     257,   259,   260,   262,   264,   265,   266,   267,   269,   270,
     271,   272,   275,   276,   277,   278,   279,   282,   283,   284,
     285,   286,   287,   289,   290,   291,   294,   295,   296,   297,
     298,   299,   337,   414,   749,   753,   754,   741,   749,   187,
     574,    27,   420,     9,    18,    36,    39,    52,    68,    74,
      84,   112,   114,   153,   189,   191,   227,   255,   257,   266,
     270,   271,   274,   280,   309,   310,   432,   465,   498,   526,
     534,   205,   737,   749,   737,   108,     9,    18,    36,    54,
      68,    84,   110,   114,   124,   189,   209,   226,   227,   232,
     254,   266,   271,   280,   290,   309,   310,   470,   502,   574,
     737,    57,   288,   589,   590,   591,     4,    10,    22,   106,
     108,   122,   170,   207,   216,   303,   344,   352,   510,   511,
     512,   737,   746,    10,    58,    78,    95,   131,   214,   226,
     231,   255,   257,   266,   278,   279,   515,   516,   517,   137,
     169,   186,   725,   733,   735,   749,   757,   339,   742,   747,
     254,   625,   510,   511,   512,   737,   735,   737,    68,   124,
     254,   555,    10,   234,   259,   264,   749,   113,   522,   574,
      10,    82,   627,    53,   153,   165,   234,   259,   264,   390,
     749,    10,   234,   259,   264,   749,   264,   625,   353,   735,
     188,   662,   735,   109,   592,   621,   622,   735,   616,   617,
       0,   364,   591,    93,   135,   192,   273,   315,   628,   629,
     738,   749,   748,   749,   662,   735,   737,   748,   662,   187,
       9,    48,    52,    68,    84,   110,   124,   189,   226,   227,
     232,   254,   266,   271,   290,   507,   735,    21,    25,    27,
      61,   107,   109,   118,   122,   127,   139,   140,   142,   147,
     149,   167,   178,   194,   195,   223,   240,   288,   337,   725,
     733,   743,   751,   753,   755,   737,   360,   236,   266,   738,
     504,   725,   749,   255,   257,   748,   255,   257,    43,   343,
     347,   348,   349,   351,   352,   353,   354,   355,   356,   550,
     698,   699,   749,   218,   386,   749,   737,   504,   748,   735,
     744,   749,   232,   254,   470,    54,   124,   110,   226,   290,
     737,    27,   130,   228,   614,   662,   743,   737,   360,   738,
     743,   748,    43,   550,   737,   737,   378,   748,   145,   503,
     504,   360,   600,   591,   602,   512,   746,   170,   207,   511,
     512,   512,   737,   208,   187,   365,   735,   363,   726,   734,
     735,   511,   512,   737,   512,   737,   360,   597,   737,   735,
      21,   299,   141,   190,   515,   187,    26,    28,    29,    35,
      36,    38,    39,    46,    55,    62,    63,    64,    65,    71,
      72,    85,    96,    99,   100,   102,   133,   134,   136,   154,
     155,   166,   168,   175,   179,   180,   181,   189,   196,   203,
     212,   225,   235,   242,   252,   259,   260,   265,   267,   268,
     274,   280,   286,   337,   338,   341,   342,   343,   345,   351,
     352,   353,   355,   356,   360,   616,   672,   673,   674,   679,
     680,   681,   683,   684,   685,   686,   690,   691,   694,   695,
     700,   702,   704,   719,   724,   725,   727,   728,   733,   743,
     745,   746,   747,   750,   753,    10,   403,   736,   737,   234,
     390,    74,   398,   747,    21,    40,   390,   299,   141,   261,
     347,    21,   299,   141,   141,   573,   735,   360,   735,   236,
     353,   107,   593,   616,   622,    16,   361,   361,   370,   735,
      10,    82,   626,   626,    31,   626,   616,   618,   619,   103,
     150,   184,   633,   650,   236,   401,     7,    86,   383,     7,
      11,    86,   217,    58,   197,   187,   236,   294,   372,   401,
       7,    86,   735,   743,   737,   743,   550,   737,   737,   504,
     360,   444,   360,   491,    41,    26,    28,    29,    38,    39,
      71,    72,   102,   133,   134,   168,   181,   212,   242,   259,
     260,   286,   337,   672,   753,    16,   584,   737,   372,   584,
     372,   504,   491,   363,    21,   387,     8,    23,   472,    16,
     491,   372,   360,   360,   488,   735,   735,   145,   504,   741,
     743,   737,   735,   228,    16,    66,   293,   664,   360,    34,
     220,   412,   672,   360,   535,   504,   360,   187,   187,   365,
     339,   399,   749,   365,   412,   702,   706,   137,   623,   602,
     737,   512,   737,   737,    68,   110,   145,   227,   254,   518,
     734,   517,    74,   285,   360,   604,   615,   353,   740,   749,
     122,   365,   609,   512,   737,   259,   737,   136,   237,   598,
     669,   671,   673,   674,   678,   680,   681,   682,   684,   685,
     690,   691,   750,    16,   104,   556,   556,   148,   103,   187,
     360,   287,   687,   702,   723,   360,   687,   687,   360,   360,
     360,   360,   360,   677,   677,   204,   616,   360,   360,   675,
     360,   360,    38,    39,   687,   702,   360,   360,   676,   360,
     360,   360,   360,   360,   294,   295,   360,   692,   360,   692,
     360,   360,   616,   705,   726,   702,   702,   702,   702,   616,
     696,   702,   706,   747,    39,   360,   688,   360,   747,   122,
     139,   175,   189,   195,   698,   701,   702,    14,    16,    20,
      25,    47,   118,   122,   139,   140,   149,   175,   178,   189,
     191,   240,   343,   347,   348,   349,   351,   352,   353,   354,
     355,   356,   362,   700,   701,   705,   365,   623,   705,   360,
      76,   119,   404,   365,    40,    74,   399,    16,    74,   153,
     337,   338,   352,   397,   461,   462,   463,   691,   746,   747,
     148,    74,   100,   183,   187,   268,   391,   392,   393,   396,
     399,   461,   391,   148,   148,   735,   729,   730,   749,   591,
     616,   360,   594,   618,   618,   630,   631,   702,   618,   211,
     278,    10,   345,   635,   746,   345,   636,   746,   650,   651,
     633,   634,   390,   280,    41,    48,    52,   105,   206,   274,
     442,   443,   559,   559,    52,   559,   559,   262,   261,   662,
     390,   376,   310,   310,   360,   187,   535,   360,   139,   187,
     187,   139,   445,   446,   749,   294,   421,    10,    12,    13,
      14,    15,    16,    17,    30,    35,    36,    41,    47,    48,
      52,    58,    62,    63,    64,    65,    74,    75,    81,    82,
      83,    88,    91,    93,   100,   103,   105,   108,   113,   114,
     116,   126,   135,   137,   146,   150,   154,   155,   169,   175,
     179,   183,   184,   186,   187,   188,   191,   192,   202,   206,
     214,   231,   235,   243,   254,   258,   261,   263,   268,   273,
     274,   280,   281,   292,   293,   337,   492,   493,   752,   753,
     754,   755,   756,   360,    16,   504,     8,   578,   669,   380,
     498,   550,   748,    58,   388,   424,   513,   575,    78,   131,
     278,   473,   474,   360,   376,   149,   433,   434,   435,   436,
     441,   442,   749,   489,   490,   752,   294,   429,   459,   182,
     360,   455,   399,   103,   187,   535,    16,   444,   316,   317,
     318,   613,   103,   702,   353,   547,   669,    16,   122,   128,
     193,   361,   536,   537,   538,   540,   669,   750,   412,   281,
     174,   549,   669,   735,   735,   748,   412,   504,   361,   365,
     112,   153,   254,   255,   257,   624,   735,   737,   736,   523,
     524,   743,   736,   736,   734,   261,   285,   360,   605,   606,
     749,   726,     5,    94,   225,   238,   610,   735,   737,    31,
     671,   361,   365,   670,    69,   117,   160,   163,   229,   298,
     360,   693,   726,   602,   518,   706,   360,   292,   720,   721,
     702,   706,   702,   706,   746,   746,   746,    69,   117,   160,
     163,   229,   298,   337,   339,   708,   710,   746,   746,   746,
     687,   687,   702,   746,   550,   702,   711,   351,   352,   355,
     356,   360,   700,   703,   704,   713,   361,   696,   702,   702,
     706,   714,   259,   259,   746,   746,   702,    30,   108,   146,
     263,   702,   706,   717,   358,   705,   361,   361,   365,   236,
     746,   746,   693,   616,    82,   175,   179,   122,   360,   225,
     360,   695,    10,    15,   243,   360,   616,   695,   697,   702,
     752,   259,   703,   749,   702,   360,   616,   718,    82,   100,
     175,   179,   182,   268,   275,   702,    25,   118,   122,   149,
     240,   360,   702,   261,   702,   702,   702,   702,   702,   702,
     702,   702,   702,   669,   702,   697,   728,   108,   653,    10,
      82,   353,   361,   706,   737,   264,   338,   746,   360,   747,
     211,   233,   394,   365,   394,   361,   365,   653,   705,   588,
     735,   736,   365,    17,    81,   281,   632,   188,   182,   652,
     184,   365,   150,   378,   360,   737,   143,   143,   360,   436,
     749,   737,   749,   558,   737,   254,   748,   217,    59,    60,
      90,   122,   172,   173,   199,   253,   272,   283,   377,   744,
     744,   547,   504,   139,   549,   179,   508,   747,   504,   504,
     508,   361,   365,   185,   108,   261,   416,   361,   365,   347,
     702,   672,   111,   473,    89,   156,   197,   256,   579,   437,
     253,   280,   381,   103,   387,   432,   534,   187,   191,   667,
     668,   749,   504,   361,   365,   669,   361,   365,   347,   311,
      32,    67,   123,   159,   161,   245,   460,   735,   433,   456,
     457,   749,    16,   115,   747,   735,   221,   561,    16,    66,
     615,   361,   672,   361,   365,   540,   726,   739,   749,   365,
     361,   365,   412,   412,   702,   255,   257,   255,   257,   735,
     625,   625,   365,   535,   114,   519,   520,   749,    74,   728,
     731,   732,   361,   365,   705,    94,   238,    94,   238,   225,
     278,   162,   358,   669,   358,   261,   261,   261,   261,   746,
     108,   361,   746,   702,    88,   721,   722,    16,   361,   281,
     361,   361,   361,   361,   365,   361,   108,   361,   361,   361,
     365,   361,   365,   361,   202,   712,   361,   703,   703,   703,
     703,   702,   703,   122,   139,   189,   347,   348,   349,   351,
     352,   353,   354,   355,   356,   362,   700,   361,   361,   361,
     103,   108,   715,   716,   361,   299,   299,   361,   361,    16,
     717,   706,   717,   717,   108,   361,   702,   726,   702,   749,
     361,   361,   108,   179,   616,   550,   616,   299,    14,    92,
     706,   108,   100,   179,   182,   268,   275,   360,    92,   703,
     702,   718,   702,   261,   550,   702,   616,   360,   616,   654,
     656,   657,   662,   663,   725,   743,   329,   639,   706,   706,
     361,   361,   141,   746,   693,    51,   211,   395,   393,   730,
     664,   347,   361,   631,   701,   736,   636,   636,   635,   702,
     443,   360,   360,   445,    86,   236,   411,   412,   412,   261,
     261,   199,   114,   747,   746,   199,   277,   294,   407,   361,
     139,   508,   361,   139,   139,   446,   248,   249,   417,   747,
     493,   189,   352,   461,   494,   539,   540,   701,   747,   361,
     361,   491,   187,   347,   580,   580,   580,   580,    41,    47,
      52,    74,    75,   126,   175,   179,   206,   214,   274,   308,
     438,   439,   440,   689,   746,   378,   271,   254,   735,   474,
     361,   365,   669,   271,   435,   437,   490,   337,   725,   750,
     753,   747,   461,   461,   461,   461,   461,   360,   361,   361,
     365,   615,   466,   725,   737,   749,   261,   281,   527,   539,
     187,   615,   103,   412,   361,   537,   355,   412,   669,   412,
     174,   669,   625,   625,   625,   625,   735,   735,   524,   749,
     294,   365,   521,   361,   365,   285,   615,   606,    94,    94,
     747,   359,   746,   117,   160,   229,   160,   229,   229,   163,
     361,   519,   361,   258,   702,    91,   669,   504,   746,   702,
     702,   746,   702,   715,   703,    82,   175,   182,   703,   703,
     703,   703,   703,   703,   703,   703,   703,   669,   703,   702,
     702,   716,   715,   692,   692,   669,   361,   361,   361,   706,
     359,   366,   705,   688,   747,   695,   361,   704,   703,   702,
     361,   702,   360,   669,   709,   702,    14,    92,    92,   702,
     361,    92,   616,   656,   657,    16,   658,   749,   365,    61,
     109,   127,   142,   147,   167,   223,   336,   659,   358,   642,
     658,    16,   658,   749,   360,    31,   328,   665,   361,   361,
     148,   361,   188,   297,   702,   361,   445,   445,   361,    74,
     175,    74,   175,   247,   250,   737,   737,   747,   378,   747,
     747,   313,   311,   430,   139,   508,   139,   508,   508,   281,
     422,   423,    75,   126,   175,   481,   482,   483,   294,   295,
     689,   735,    74,   746,   747,    74,   747,    74,   737,    74,
     737,   360,   749,   737,   703,    76,   119,    75,   179,   143,
     735,   669,   735,   103,   475,   191,   668,   304,   305,   306,
     307,   428,   689,   433,   125,   452,   457,   749,   284,   468,
     747,   739,   360,    16,    33,    98,   120,   145,   221,   230,
     244,   251,   291,   541,   542,    78,   131,   231,   278,   566,
     615,   412,   271,   735,   735,   735,   735,   113,   520,   732,
     360,   359,   359,   693,   702,   361,   361,   361,   361,   361,
     716,   108,   182,   360,   361,   702,   693,   709,   361,   365,
     703,   702,   702,    92,   702,   361,   749,   360,   656,   142,
     194,   660,   656,   660,   142,   659,   660,   656,   142,   302,
     335,   643,   644,   646,   642,   360,   749,   360,   361,   706,
     337,   707,    30,   330,   331,   332,   333,   334,   666,   735,
     664,   394,   747,   361,   361,   179,   702,   179,   352,   463,
     749,   360,   747,   508,   508,   372,    80,    76,   119,    75,
     483,   482,   110,   110,   108,   480,   702,   175,   439,   444,
     281,   360,    87,   476,    95,   474,   187,   426,   361,   360,
     294,   295,   453,   466,   144,   467,   108,   528,   529,   530,
     531,   740,   743,   749,   543,   747,   187,   230,   399,   179,
      77,   138,   294,   542,   544,   261,   190,   731,   703,   360,
     709,   359,   361,   669,   702,   658,   360,   736,   656,   187,
     281,   661,   656,   142,   656,    31,   645,   747,   301,   647,
     300,   648,   667,   360,   667,   749,   361,   365,   360,   360,
     360,   360,   360,   360,   114,   637,   693,   214,   408,   409,
     446,   418,   747,   524,    16,   552,   735,   481,   361,   158,
     447,   739,   225,   246,   477,   210,   320,   453,   734,   185,
     185,   747,   504,   361,   365,   281,   504,   532,   360,   365,
     179,    77,   138,   187,   491,   735,   361,   709,   361,   736,
     361,   702,   360,   656,   661,   747,   365,   746,   245,   649,
      31,   361,   667,   361,   707,   666,   666,   666,   666,   666,
     666,    31,   116,   638,   735,   361,   365,   319,    27,    79,
     179,   185,   419,   552,    19,   121,   103,   109,   198,   241,
     187,   448,   449,   450,    16,   743,    86,   321,   324,   361,
     664,   531,   504,   736,   747,   129,   179,   664,   361,   361,
     736,   747,    20,   359,   747,   361,   365,   365,   365,   365,
     365,   365,   706,   702,   281,   641,   444,   409,    74,   584,
     584,    87,    78,   278,   187,   450,   187,   449,   110,   189,
     250,   496,   497,   360,   322,   323,   325,   326,   327,   427,
     361,   129,    83,   361,   747,   666,   666,   666,   666,   666,
     666,   303,   312,   640,   447,   747,   747,   747,   225,    34,
     171,   220,   236,   451,   451,   746,   746,   669,   365,   338,
     341,   342,   344,   478,   479,   747,   749,   360,   532,   132,
     567,   365,   361,   365,   365,   365,   365,   358,   358,   314,
     655,   448,    95,     6,    74,   179,   743,   550,   497,   361,
     365,   747,   176,   360,   562,   564,   569,   603,   607,   611,
     615,   339,   339,   339,   339,   339,   746,   746,   139,   481,
     210,   535,   213,   360,   499,   479,   361,   563,   564,   565,
     616,   617,   361,   361,   361,   361,   361,   365,   359,   365,
     656,   743,   549,   361,   364,   746,   747,   360,   361,   565,
     359,   359,   365,   478,   499,   747,   361,   359
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
      yyerror ("parse error: cannot back up");\
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
        case 2:
#line 498 "gram.y"
    { parsetree = yyvsp[0].list; }
    break;

  case 3:
#line 503 "gram.y"
    { if (yyvsp[0].node != (Node *)NULL)
					yyval.list = lappend(yyvsp[-2].list, yyvsp[0].node);
				  else
					yyval.list = yyvsp[-2].list;
				}
    break;

  case 4:
#line 509 "gram.y"
    { if (yyvsp[0].node != (Node *)NULL)
						yyval.list = makeList1(yyvsp[0].node);
					  else
						yyval.list = NIL;
					}
    break;

  case 73:
#line 586 "gram.y"
    { yyval.node = (Node *)NULL; }
    break;

  case 74:
#line 598 "gram.y"
    {
					CreateUserStmt *n = makeNode(CreateUserStmt);
					n->user = yyvsp[-2].str;
					n->options = yyvsp[0].list;
					yyval.node = (Node *)n;
				}
    break;

  case 75:
#line 607 "gram.y"
    {}
    break;

  case 76:
#line 608 "gram.y"
    {}
    break;

  case 77:
#line 620 "gram.y"
    {
					AlterUserStmt *n = makeNode(AlterUserStmt);
					n->user = yyvsp[-2].str;
					n->options = yyvsp[0].list;
					yyval.node = (Node *)n;
				 }
    break;

  case 78:
#line 631 "gram.y"
    {
					AlterUserSetStmt *n = makeNode(AlterUserSetStmt);
					n->user = yyvsp[-2].str;
					n->variable = yyvsp[0].vsetstmt->name;
					n->value = yyvsp[0].vsetstmt->args;
					yyval.node = (Node *)n;
				}
    break;

  case 79:
#line 639 "gram.y"
    {
					AlterUserSetStmt *n = makeNode(AlterUserSetStmt);
					n->user = yyvsp[-1].str;
					n->variable = ((VariableResetStmt *)yyvsp[0].node)->name;
					n->value = NIL;
					yyval.node = (Node *)n;
				}
    break;

  case 80:
#line 660 "gram.y"
    {
					DropUserStmt *n = makeNode(DropUserStmt);
					n->users = yyvsp[0].list;
					yyval.node = (Node *)n;
				}
    break;

  case 81:
#line 671 "gram.y"
    { yyval.list = lappend(yyvsp[-1].list, yyvsp[0].defelt); }
    break;

  case 82:
#line 672 "gram.y"
    { yyval.list = NIL; }
    break;

  case 83:
#line 677 "gram.y"
    {
					yyval.defelt = makeDefElem("password", (Node *)makeString(yyvsp[0].str));
				}
    break;

  case 84:
#line 681 "gram.y"
    {
					yyval.defelt = makeDefElem("encryptedPassword", (Node *)makeString(yyvsp[0].str));
				}
    break;

  case 85:
#line 685 "gram.y"
    {
					yyval.defelt = makeDefElem("unencryptedPassword", (Node *)makeString(yyvsp[0].str));
				}
    break;

  case 86:
#line 689 "gram.y"
    {
					yyval.defelt = makeDefElem("sysid", (Node *)makeInteger(yyvsp[0].ival));
				}
    break;

  case 87:
#line 693 "gram.y"
    {
					yyval.defelt = makeDefElem("createdb", (Node *)makeInteger(TRUE));
				}
    break;

  case 88:
#line 697 "gram.y"
    {
					yyval.defelt = makeDefElem("createdb", (Node *)makeInteger(FALSE));
				}
    break;

  case 89:
#line 701 "gram.y"
    {
					yyval.defelt = makeDefElem("createuser", (Node *)makeInteger(TRUE));
				}
    break;

  case 90:
#line 705 "gram.y"
    {
					yyval.defelt = makeDefElem("createuser", (Node *)makeInteger(FALSE));
				}
    break;

  case 91:
#line 709 "gram.y"
    {
					yyval.defelt = makeDefElem("groupElts", (Node *)yyvsp[0].list);
				}
    break;

  case 92:
#line 713 "gram.y"
    {
					yyval.defelt = makeDefElem("validUntil", (Node *)makeString(yyvsp[0].str));
				}
    break;

  case 93:
#line 718 "gram.y"
    { yyval.list = lappend(yyvsp[-2].list, makeString(yyvsp[0].str)); }
    break;

  case 94:
#line 719 "gram.y"
    { yyval.list = makeList1(makeString(yyvsp[0].str)); }
    break;

  case 95:
#line 733 "gram.y"
    {
					CreateGroupStmt *n = makeNode(CreateGroupStmt);
					n->name = yyvsp[-2].str;
					n->options = yyvsp[0].list;
					yyval.node = (Node *)n;
				}
    break;

  case 96:
#line 745 "gram.y"
    { yyval.list = lappend(yyvsp[-1].list, yyvsp[0].defelt); }
    break;

  case 97:
#line 746 "gram.y"
    { yyval.list = NIL; }
    break;

  case 98:
#line 751 "gram.y"
    {
					yyval.defelt = makeDefElem("userElts", (Node *)yyvsp[0].list);
				}
    break;

  case 99:
#line 755 "gram.y"
    {
					yyval.defelt = makeDefElem("sysid", (Node *)makeInteger(yyvsp[0].ival));
				}
    break;

  case 100:
#line 770 "gram.y"
    {
					AlterGroupStmt *n = makeNode(AlterGroupStmt);
					n->name = yyvsp[-3].str;
					n->action = yyvsp[-2].ival;
					n->listUsers = yyvsp[0].list;
					yyval.node = (Node *)n;
				}
    break;

  case 101:
#line 779 "gram.y"
    { yyval.ival = +1; }
    break;

  case 102:
#line 780 "gram.y"
    { yyval.ival = -1; }
    break;

  case 103:
#line 793 "gram.y"
    {
					DropGroupStmt *n = makeNode(DropGroupStmt);
					n->name = yyvsp[0].str;
					yyval.node = (Node *)n;
				}
    break;

  case 104:
#line 809 "gram.y"
    {
					CreateSchemaStmt *n = makeNode(CreateSchemaStmt);
					/* One can omit the schema name or the authorization id. */
					if (yyvsp[-3].str != NULL)
						n->schemaname = yyvsp[-3].str;
					else
						n->schemaname = yyvsp[-1].str;
					n->authid = yyvsp[-1].str;
					n->schemaElts = yyvsp[0].list;
					yyval.node = (Node *)n;
				}
    break;

  case 105:
#line 821 "gram.y"
    {
					CreateSchemaStmt *n = makeNode(CreateSchemaStmt);
					/* ...but not both */
					n->schemaname = yyvsp[-1].str;
					n->authid = NULL;
					n->schemaElts = yyvsp[0].list;
					yyval.node = (Node *)n;
				}
    break;

  case 106:
#line 832 "gram.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 107:
#line 833 "gram.y"
    { yyval.str = NULL; }
    break;

  case 108:
#line 837 "gram.y"
    { yyval.list = lappend(yyvsp[-1].list, yyvsp[0].node); }
    break;

  case 109:
#line 838 "gram.y"
    { yyval.list = NIL; }
    break;

  case 113:
#line 863 "gram.y"
    {
					VariableSetStmt *n = yyvsp[0].vsetstmt;
					n->is_local = false;
					yyval.node = (Node *) n;
				}
    break;

  case 114:
#line 869 "gram.y"
    {
					VariableSetStmt *n = yyvsp[0].vsetstmt;
					n->is_local = true;
					yyval.node = (Node *) n;
				}
    break;

  case 115:
#line 875 "gram.y"
    {
					VariableSetStmt *n = yyvsp[0].vsetstmt;
					n->is_local = false;
					yyval.node = (Node *) n;
				}
    break;

  case 116:
#line 883 "gram.y"
    {
					VariableSetStmt *n = makeNode(VariableSetStmt);
					n->name = yyvsp[-2].str;
					n->args = yyvsp[0].list;
					yyval.vsetstmt = n;
				}
    break;

  case 117:
#line 890 "gram.y"
    {
					VariableSetStmt *n = makeNode(VariableSetStmt);
					n->name = yyvsp[-2].str;
					n->args = yyvsp[0].list;
					yyval.vsetstmt = n;
				}
    break;

  case 118:
#line 897 "gram.y"
    {
					VariableSetStmt *n = makeNode(VariableSetStmt);
					n->name = "timezone";
					if (yyvsp[0].node != NULL)
						n->args = makeList1(yyvsp[0].node);
					yyval.vsetstmt = n;
				}
    break;

  case 119:
#line 905 "gram.y"
    {
					VariableSetStmt *n = makeNode(VariableSetStmt);
					n->name = "TRANSACTION ISOLATION LEVEL";
					n->args = makeList1(makeStringConst(yyvsp[-1].str, NULL));
					yyval.vsetstmt = n;
				}
    break;

  case 120:
#line 912 "gram.y"
    {
					VariableSetStmt *n = makeNode(VariableSetStmt);
					n->name = "default_transaction_isolation";
					n->args = makeList1(makeStringConst(yyvsp[0].str, NULL));
					yyval.vsetstmt = n;
				}
    break;

  case 121:
#line 919 "gram.y"
    {
					VariableSetStmt *n = makeNode(VariableSetStmt);
					n->name = "client_encoding";
					if (yyvsp[0].str != NULL)
						n->args = makeList1(makeStringConst(yyvsp[0].str, NULL));
					yyval.vsetstmt = n;
				}
    break;

  case 122:
#line 927 "gram.y"
    {
					VariableSetStmt *n = makeNode(VariableSetStmt);
					n->name = "session_authorization";
					n->args = makeList1(makeStringConst(yyvsp[0].str, NULL));
					yyval.vsetstmt = n;
				}
    break;

  case 123:
#line 934 "gram.y"
    {
					VariableSetStmt *n = makeNode(VariableSetStmt);
					n->name = "session_authorization";
					n->args = NIL;
					yyval.vsetstmt = n;
				}
    break;

  case 124:
#line 943 "gram.y"
    { yyval.list = yyvsp[0].list; }
    break;

  case 125:
#line 944 "gram.y"
    { yyval.list = NIL; }
    break;

  case 126:
#line 947 "gram.y"
    { yyval.list = makeList1(yyvsp[0].node); }
    break;

  case 127:
#line 948 "gram.y"
    { yyval.list = lappend(yyvsp[-2].list, yyvsp[0].node); }
    break;

  case 128:
#line 952 "gram.y"
    { yyval.node = makeStringConst(yyvsp[0].str, NULL); }
    break;

  case 129:
#line 954 "gram.y"
    { yyval.node = makeStringConst(yyvsp[0].str, NULL); }
    break;

  case 130:
#line 956 "gram.y"
    { yyval.node = makeAConst(yyvsp[0].value); }
    break;

  case 131:
#line 959 "gram.y"
    { yyval.str = "read committed"; }
    break;

  case 132:
#line 960 "gram.y"
    { yyval.str = "serializable"; }
    break;

  case 133:
#line 964 "gram.y"
    {}
    break;

  case 134:
#line 966 "gram.y"
    {
					elog(ERROR, "SET TRANSACTION/READ ONLY not yet supported");
				}
    break;

  case 135:
#line 970 "gram.y"
    {}
    break;

  case 136:
#line 974 "gram.y"
    { yyval.str = "true"; }
    break;

  case 137:
#line 975 "gram.y"
    { yyval.str = "false"; }
    break;

  case 138:
#line 976 "gram.y"
    { yyval.str = "on"; }
    break;

  case 139:
#line 977 "gram.y"
    { yyval.str = "off"; }
    break;

  case 140:
#line 990 "gram.y"
    {
					yyval.node = makeStringConst(yyvsp[0].str, NULL);
				}
    break;

  case 141:
#line 994 "gram.y"
    {
					yyval.node = makeStringConst(yyvsp[0].str, NULL);
				}
    break;

  case 142:
#line 998 "gram.y"
    {
					A_Const *n = (A_Const *) makeStringConst(yyvsp[-1].str, yyvsp[-2].typnam);
					if (yyvsp[0].ival != INTERVAL_FULL_RANGE)
					{
						if ((yyvsp[0].ival & ~(INTERVAL_MASK(HOUR) | INTERVAL_MASK(MINUTE))) != 0)
							elog(ERROR,
								 "Time zone interval"
								 " must be HOUR or HOUR TO MINUTE");
						n->typename->typmod = INTERVAL_TYPMOD(INTERVAL_FULL_PRECISION, yyvsp[0].ival);
					}
					yyval.node = (Node *)n;
				}
    break;

  case 143:
#line 1011 "gram.y"
    {
					A_Const *n = (A_Const *) makeStringConst(yyvsp[-1].str, yyvsp[-5].typnam);
					if ((yyvsp[-3].ival < 0) || (yyvsp[-3].ival > MAX_INTERVAL_PRECISION))
						elog(ERROR,
							"INTERVAL(%d) precision must be between %d and %d",
							yyvsp[-3].ival, 0, MAX_INTERVAL_PRECISION);

					if ((yyvsp[0].ival != INTERVAL_FULL_RANGE)
						&& ((yyvsp[0].ival & ~(INTERVAL_MASK(HOUR) | INTERVAL_MASK(MINUTE))) != 0))
						elog(ERROR,
							 "Time zone interval"
							 " must be HOUR or HOUR TO MINUTE");

					n->typename->typmod = INTERVAL_TYPMOD(yyvsp[-3].ival, yyvsp[0].ival);

					yyval.node = (Node *)n;
				}
    break;

  case 144:
#line 1028 "gram.y"
    { yyval.node = makeAConst(yyvsp[0].value); }
    break;

  case 145:
#line 1029 "gram.y"
    { yyval.node = NULL; }
    break;

  case 146:
#line 1030 "gram.y"
    { yyval.node = NULL; }
    break;

  case 147:
#line 1034 "gram.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 148:
#line 1035 "gram.y"
    { yyval.str = NULL; }
    break;

  case 149:
#line 1036 "gram.y"
    { yyval.str = NULL; }
    break;

  case 150:
#line 1040 "gram.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 151:
#line 1041 "gram.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 152:
#line 1047 "gram.y"
    {
					VariableShowStmt *n = makeNode(VariableShowStmt);
					n->name = yyvsp[0].str;
					yyval.node = (Node *) n;
				}
    break;

  case 153:
#line 1053 "gram.y"
    {
					VariableShowStmt *n = makeNode(VariableShowStmt);
					n->name = "timezone";
					yyval.node = (Node *) n;
				}
    break;

  case 154:
#line 1059 "gram.y"
    {
					VariableShowStmt *n = makeNode(VariableShowStmt);
					n->name = "TRANSACTION ISOLATION LEVEL";
					yyval.node = (Node *) n;
				}
    break;

  case 155:
#line 1065 "gram.y"
    {
					VariableShowStmt *n = makeNode(VariableShowStmt);
					n->name = "session_authorization";
					yyval.node = (Node *) n;
				}
    break;

  case 156:
#line 1071 "gram.y"
    {
					VariableShowStmt *n = makeNode(VariableShowStmt);
					n->name = "all";
					yyval.node = (Node *) n;
				}
    break;

  case 157:
#line 1080 "gram.y"
    {
					VariableResetStmt *n = makeNode(VariableResetStmt);
					n->name = yyvsp[0].str;
					yyval.node = (Node *) n;
				}
    break;

  case 158:
#line 1086 "gram.y"
    {
					VariableResetStmt *n = makeNode(VariableResetStmt);
					n->name = "timezone";
					yyval.node = (Node *) n;
				}
    break;

  case 159:
#line 1092 "gram.y"
    {
					VariableResetStmt *n = makeNode(VariableResetStmt);
					n->name = "TRANSACTION ISOLATION LEVEL";
					yyval.node = (Node *) n;
				}
    break;

  case 160:
#line 1098 "gram.y"
    {
					VariableResetStmt *n = makeNode(VariableResetStmt);
					n->name = "session_authorization";
					yyval.node = (Node *) n;
				}
    break;

  case 161:
#line 1104 "gram.y"
    {
					VariableResetStmt *n = makeNode(VariableResetStmt);
					n->name = "all";
					yyval.node = (Node *) n;
				}
    break;

  case 162:
#line 1114 "gram.y"
    {
					ConstraintsSetStmt *n = makeNode(ConstraintsSetStmt);
					n->constraints = yyvsp[-1].list;
					n->deferred    = yyvsp[0].boolean;
					yyval.node = (Node *) n;
				}
    break;

  case 163:
#line 1123 "gram.y"
    { yyval.list = NIL; }
    break;

  case 164:
#line 1124 "gram.y"
    { yyval.list = yyvsp[0].list; }
    break;

  case 165:
#line 1128 "gram.y"
    { yyval.boolean = TRUE; }
    break;

  case 166:
#line 1129 "gram.y"
    { yyval.boolean = FALSE; }
    break;

  case 167:
#line 1138 "gram.y"
    {
					CheckPointStmt *n = makeNode(CheckPointStmt);
					yyval.node = (Node *)n;
				}
    break;

  case 168:
#line 1153 "gram.y"
    {
				    AlterStreamStmt *n = makeNode(AlterStreamStmt);
					n->subtype = 'W';
					if(yyvsp[-5].range->catalogname && yyvsp[-5].range->schemaname)
					  n->stream = makeList3(makeString(yyvsp[-5].range->catalogname),
											makeString(yyvsp[-5].range->schemaname),
											makeString(yyvsp[-5].range->relname));
					else if(yyvsp[-5].range->schemaname)
					  {
						n->stream = makeList2(makeString(yyvsp[-5].range->schemaname),
											  makeString(yyvsp[-5].range->relname));
					  }
					else
					  {
						n->stream = makeList1(makeString(yyvsp[-5].range->relname));
					  }
					
					n->wrapper = makeList1(makeString(yyvsp[-2].str));
					n->bindings=yyvsp[-1].list;
					n->info=yyvsp[0].str;
					yyval.node = (Node *)n;
				}
    break;

  case 169:
#line 1177 "gram.y"
    {
				    AlterStreamStmt *n = makeNode(AlterStreamStmt);
					n->subtype = 'w';
										if(yyvsp[-3].range->catalogname && yyvsp[-3].range->schemaname)
					  n->stream = makeList3(makeString(yyvsp[-3].range->catalogname),
											makeString(yyvsp[-3].range->schemaname),
											makeString(yyvsp[-3].range->relname));
					else if(yyvsp[-3].range->schemaname)
					  {
						n->stream = makeList2(makeString(yyvsp[-3].range->schemaname),
											  makeString(yyvsp[-3].range->relname));
					  }
					else
					  {
						n->stream = makeList1(makeString(yyvsp[-3].range->relname));
					  }
					
					n->wrapper = makeList1(makeString(yyvsp[0].str));
					
					yyval.node = (Node *)n;
				}
    break;

  case 170:
#line 1202 "gram.y"
    {
  yyval.list = yyvsp[-1].list;
}
    break;

  case 171:
#line 1206 "gram.y"
    {
  yyval.list = NULL;
}
    break;

  case 172:
#line 1213 "gram.y"
    {
	   yyval.list = lappend(yyvsp[-2].list, yyvsp[0].node);
	}
    break;

  case 173:
#line 1217 "gram.y"
    {
	  yyval.list = makeList1(yyvsp[0].node);
	}
    break;

  case 174:
#line 1223 "gram.y"
    {
		Binding * b = makeNode(Binding);
		b->column=yyvsp[0].node;
		b->isOptional=false;
		b->def=NULL;
		yyval.node=(Node*)b;
	}
    break;

  case 175:
#line 1231 "gram.y"
    {
		Binding *b = makeNode(Binding);
		b->column=yyvsp[-1].node;
		b->isOptional=true;
		b->def=NULL;
		yyval.node=(Node*)b;
	}
    break;

  case 176:
#line 1239 "gram.y"
    {
		Binding *b = makeNode(Binding);
		b->column=yyvsp[-3].node;
		b->isOptional=true;
		b->def=(Value*)makeString(yyvsp[0].str);
		yyval.node=(Node*)b;
	}
    break;

  case 177:
#line 1259 "gram.y"
    {
					AlterTableStmt *n = makeNode(AlterTableStmt);
					n->subtype = 'A';
					n->relation = yyvsp[-3].range;
					n->def = yyvsp[0].node;
					yyval.node = (Node *)n;
				}
    break;

  case 178:
#line 1268 "gram.y"
    {
					AlterTableStmt *n = makeNode(AlterTableStmt);
					n->subtype = 'T';
					n->relation = yyvsp[-4].range;
					n->name = yyvsp[-1].str;
					n->def = yyvsp[0].node;
					yyval.node = (Node *)n;
				}
    break;

  case 179:
#line 1278 "gram.y"
    {
					AlterTableStmt *n = makeNode(AlterTableStmt);
					n->subtype = 'N';
					n->relation = yyvsp[-6].range;
					n->name = yyvsp[-3].str;
					yyval.node = (Node *)n;
				}
    break;

  case 180:
#line 1287 "gram.y"
    {
					AlterTableStmt *n = makeNode(AlterTableStmt);
					n->subtype = 'O';
					n->relation = yyvsp[-6].range;
					n->name = yyvsp[-3].str;
					yyval.node = (Node *)n;
				}
    break;

  case 181:
#line 1296 "gram.y"
    {
					AlterTableStmt *n = makeNode(AlterTableStmt);
					n->subtype = 'S';
					n->relation = yyvsp[-6].range;
					n->name = yyvsp[-3].str;
					n->def = (Node *) yyvsp[0].value;
					yyval.node = (Node *)n;
				}
    break;

  case 182:
#line 1307 "gram.y"
    {
					AlterTableStmt *n = makeNode(AlterTableStmt);
					n->subtype = 'M';
					n->relation = yyvsp[-6].range;
					n->name = yyvsp[-3].str;
					n->def = (Node *) makeString(yyvsp[0].str);
					yyval.node = (Node *)n;
				}
    break;

  case 183:
#line 1317 "gram.y"
    {
					AlterTableStmt *n = makeNode(AlterTableStmt);
					n->subtype = 'D';
					n->relation = yyvsp[-4].range;
					n->name = yyvsp[-1].str;
					n->behavior = yyvsp[0].dbehavior;
					yyval.node = (Node *)n;
				}
    break;

  case 184:
#line 1327 "gram.y"
    {
					AlterTableStmt *n = makeNode(AlterTableStmt);
					n->subtype = 'C';
					n->relation = yyvsp[-2].range;
					n->def = yyvsp[0].node;
					yyval.node = (Node *)n;
				}
    break;

  case 185:
#line 1336 "gram.y"
    {
					AlterTableStmt *n = makeNode(AlterTableStmt);
					n->subtype = 'X';
					n->relation = yyvsp[-4].range;
					n->name = yyvsp[-1].str;
					n->behavior = yyvsp[0].dbehavior;
					yyval.node = (Node *)n;
				}
    break;

  case 186:
#line 1346 "gram.y"
    {
					AlterTableStmt *n = makeNode(AlterTableStmt);
					n->subtype = 'E';
					yyvsp[-3].range->inhOpt = INH_NO;
					n->relation = yyvsp[-3].range;
					yyval.node = (Node *)n;
				}
    break;

  case 187:
#line 1355 "gram.y"
    {
					AlterTableStmt *n = makeNode(AlterTableStmt);
					n->subtype = 'U';
					yyvsp[-3].range->inhOpt = INH_NO;
					n->relation = yyvsp[-3].range;
					n->name = yyvsp[0].str;
					yyval.node = (Node *)n;
				}
    break;

  case 188:
#line 1367 "gram.y"
    {
					/* Treat SET DEFAULT NULL the same as DROP DEFAULT */
					if (exprIsNullConstant(yyvsp[0].node))
						yyval.node = NULL;
					else
						yyval.node = yyvsp[0].node;
				}
    break;

  case 189:
#line 1374 "gram.y"
    { yyval.node = NULL; }
    break;

  case 190:
#line 1378 "gram.y"
    { yyval.dbehavior = DROP_CASCADE; }
    break;

  case 191:
#line 1379 "gram.y"
    { yyval.dbehavior = DROP_RESTRICT; }
    break;

  case 192:
#line 1380 "gram.y"
    { yyval.dbehavior = DROP_RESTRICT; /* default */ }
    break;

  case 193:
#line 1394 "gram.y"
    {
					ClosePortalStmt *n = makeNode(ClosePortalStmt);
					n->portalname = yyvsp[0].str;
					yyval.node = (Node *)n;
				}
    break;

  case 194:
#line 1401 "gram.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 195:
#line 1402 "gram.y"
    { yyval.str = NULL; }
    break;

  case 196:
#line 1418 "gram.y"
    {
					CopyStmt *n = makeNode(CopyStmt);
					n->relation = yyvsp[-7].range;
					n->attlist = yyvsp[-6].list;
					n->is_from = yyvsp[-4].boolean;
					n->filename = yyvsp[-3].str;

					n->options = NIL;
					/* Concatenate user-supplied flags */
					if (yyvsp[-8].defelt)
						n->options = lappend(n->options, yyvsp[-8].defelt);
					if (yyvsp[-5].defelt)
						n->options = lappend(n->options, yyvsp[-5].defelt);
					if (yyvsp[-2].defelt)
						n->options = lappend(n->options, yyvsp[-2].defelt);
					if (yyvsp[0].list)
						n->options = nconc(n->options, yyvsp[0].list);
					yyval.node = (Node *)n;
				}
    break;

  case 197:
#line 1440 "gram.y"
    { yyval.boolean = TRUE; }
    break;

  case 198:
#line 1441 "gram.y"
    { yyval.boolean = FALSE; }
    break;

  case 199:
#line 1450 "gram.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 200:
#line 1451 "gram.y"
    { yyval.str = NULL; }
    break;

  case 201:
#line 1452 "gram.y"
    { yyval.str = NULL; }
    break;

  case 202:
#line 1458 "gram.y"
    { yyval.list = lappend(yyvsp[-1].list, yyvsp[0].defelt); }
    break;

  case 203:
#line 1459 "gram.y"
    { yyval.list = NIL; }
    break;

  case 204:
#line 1465 "gram.y"
    {
					yyval.defelt = makeDefElem("binary", (Node *)makeInteger(TRUE));
				}
    break;

  case 205:
#line 1469 "gram.y"
    {
					yyval.defelt = makeDefElem("oids", (Node *)makeInteger(TRUE));
				}
    break;

  case 206:
#line 1473 "gram.y"
    {
					yyval.defelt = makeDefElem("delimiter", (Node *)makeString(yyvsp[0].str));
				}
    break;

  case 207:
#line 1477 "gram.y"
    {
					yyval.defelt = makeDefElem("null", (Node *)makeString(yyvsp[0].str));
				}
    break;

  case 208:
#line 1486 "gram.y"
    {
					yyval.defelt = makeDefElem("binary", (Node *)makeInteger(TRUE));
				}
    break;

  case 209:
#line 1489 "gram.y"
    { yyval.defelt = NULL; }
    break;

  case 210:
#line 1494 "gram.y"
    {
					yyval.defelt = makeDefElem("oids", (Node *)makeInteger(TRUE));
				}
    break;

  case 211:
#line 1497 "gram.y"
    { yyval.defelt = NULL; }
    break;

  case 212:
#line 1503 "gram.y"
    {
					yyval.defelt = makeDefElem("delimiter", (Node *)makeString(yyvsp[0].str));
				}
    break;

  case 213:
#line 1506 "gram.y"
    { yyval.defelt = NULL; }
    break;

  case 214:
#line 1510 "gram.y"
    {}
    break;

  case 215:
#line 1511 "gram.y"
    {}
    break;

  case 216:
#line 1524 "gram.y"
    {
					CreateStmt *n = makeNode(CreateStmt);
					yyvsp[-5].range->istemp = yyvsp[-7].boolean;
					n->relation = yyvsp[-5].range;
					n->tableElts = yyvsp[-3].list;
					n->inhRelations = yyvsp[-1].list;
					n->constraints = NIL;
					n->hasoids = yyvsp[0].boolean;
					yyval.node = (Node *)n;
				}
    break;

  case 217:
#line 1536 "gram.y"
    {
					/* SQL99 CREATE TABLE OF <UDT> (cols) seems to be satisfied
					 * by our inheritance capabilities. Let's try it...
					 */
					CreateStmt *n = makeNode(CreateStmt);
					yyvsp[-6].range->istemp = yyvsp[-8].boolean;
					n->relation = yyvsp[-6].range;
					n->tableElts = yyvsp[-2].list;
					n->inhRelations = makeList1(yyvsp[-4].range);
					n->constraints = NIL;
					n->hasoids = yyvsp[0].boolean;
					yyval.node = (Node *)n;
				}
    break;

  case 218:
#line 1554 "gram.y"
    {
						 CreateStrmStmt *ns = makeNode(CreateStrmStmt);
						 CreateStmt *n = (CreateStmt *)ns;
						 n->relation = yyvsp[-6].range;
						 n->tableElts = yyvsp[-4].list;
						 n->inhRelations = NIL;
						 n->constraints = NIL;
						 n->relation->istemp = FALSE; /* Not temporary right? */
						 n->hasoids = TRUE; /* Should generate OIDs right? */
						 ns->streamtype = yyvsp[-1].chr;
						 ns->overload_params = yyvsp[0].overload_params;
						 yyval.node = (Node *) n;
					 }
    break;

  case 219:
#line 1570 "gram.y"
    { yyval.overload_params = yyvsp[0].overload_params; }
    break;

  case 220:
#line 1571 "gram.y"
    { yyval.overload_params = makeOverloadParams(OVERLOAD_DROP, NULL); }
    break;

  case 221:
#line 1572 "gram.y"
    { yyval.overload_params = makeOverloadParams(OVERLOAD_BLOCK, NULL); }
    break;

  case 222:
#line 1573 "gram.y"
    { yyval.overload_params = makeOverloadParams(OVERLOAD_BLOCK, NULL); }
    break;

  case 223:
#line 1577 "gram.y"
    { yyval.overload_params = makeOverloadParams(OVERLOAD_COUNTS, NULL); }
    break;

  case 224:
#line 1578 "gram.y"
    { yyval.overload_params = makeOverloadParams(OVERLOAD_REGHIST, NULL); }
    break;

  case 225:
#line 1579 "gram.y"
    { yyval.overload_params = makeOverloadParams(OVERLOAD_MHIST, NULL); }
    break;

  case 226:
#line 1581 "gram.y"
    {
					yyval.overload_params = makeOverloadParams(OVERLOAD_WAVELET, yyvsp[-1].str);
				}
    break;

  case 227:
#line 1584 "gram.y"
    { yyval.overload_params = makeOverloadParams(OVERLOAD_SAMPLE, NULL); }
    break;

  case 228:
#line 1590 "gram.y"
    {
	yyval.chr = RELKIND_ARCHIVEDSTREAM;;
}
    break;

  case 229:
#line 1594 "gram.y"
    {
#ifdef BP_SPILL
	yyval.chr= RELKIND_UNARCHIVEDSTREAM_SPILL;
#else
	yyval.chr= RELKIND_UNARCHIVEDSTREAM_DROP;
#endif
  
	//	$$= RELKIND_UNARCHIVEDSTREAM;
}
    break;

  case 230:
#line 1604 "gram.y"
    {
  yyval.chr= RELKIND_UNARCHIVEDSTREAM_DROP;
}
    break;

  case 231:
#line 1608 "gram.y"
    {
  yyval.chr = RELKIND_UNARCHIVEDSTREAM_SPILL;
}
    break;

  case 232:
#line 1614 "gram.y"
    {
  yyval.str = yyvsp[0].str;
}
    break;

  case 233:
#line 1618 "gram.y"
    {yyval.str = NULL;}
    break;

  case 234:
#line 1622 "gram.y"
    {
  yyval.str = yyvsp[0].str;
}
    break;

  case 235:
#line 1626 "gram.y"
    {yyval.str = NULL;}
    break;

  case 236:
#line 1631 "gram.y"
    {
  CreateWrapperStmt *cws = makeNode(CreateWrapperStmt);
  cws->wrappername=yyvsp[-2].str;
  cws->functions=yyvsp[-1].list;
  cws->info=yyvsp[0].str;
  yyval.node = (Node *) cws;
}
    break;

  case 237:
#line 1644 "gram.y"
    { yyval.boolean = TRUE; }
    break;

  case 238:
#line 1645 "gram.y"
    { yyval.boolean = TRUE; }
    break;

  case 239:
#line 1646 "gram.y"
    { yyval.boolean = TRUE; }
    break;

  case 240:
#line 1647 "gram.y"
    { yyval.boolean = TRUE; }
    break;

  case 241:
#line 1649 "gram.y"
    {
					elog(ERROR,
					"GLOBAL TEMPORARY TABLE is not currently supported");
					yyval.boolean = TRUE;
				}
    break;

  case 242:
#line 1655 "gram.y"
    {
					elog(ERROR,
					"GLOBAL TEMPORARY TABLE is not currently supported");
					yyval.boolean = TRUE;
				}
    break;

  case 243:
#line 1660 "gram.y"
    { yyval.boolean = FALSE; }
    break;

  case 244:
#line 1664 "gram.y"
    { yyval.list = yyvsp[0].list; }
    break;

  case 245:
#line 1665 "gram.y"
    { yyval.list = NIL; }
    break;

  case 246:
#line 1670 "gram.y"
    {
					yyval.list = makeList1(yyvsp[0].node);
				}
    break;

  case 247:
#line 1674 "gram.y"
    {
					yyval.list = lappend(yyvsp[-2].list, yyvsp[0].node);
				}
    break;

  case 248:
#line 1680 "gram.y"
    { yyval.node = yyvsp[0].node; }
    break;

  case 249:
#line 1681 "gram.y"
    { yyval.node = yyvsp[0].node; }
    break;

  case 250:
#line 1682 "gram.y"
    { yyval.node = yyvsp[0].node; }
    break;

  case 251:
#line 1686 "gram.y"
    {
					ColumnDef *n = makeNode(ColumnDef);
					n->colname = yyvsp[-3].str;
					n->typename = yyvsp[-2].typnam;
					n->constraints = yyvsp[-1].list;
					n->is_local = true;

					if (yyvsp[0].str != NULL)
						elog(NOTICE,
							"CREATE TABLE / COLLATE %s not yet implemented; "
							"clause ignored", yyvsp[0].str);

					yyval.node = (Node *)n;
				}
    break;

  case 252:
#line 1703 "gram.y"
    { yyval.list = lappend(yyvsp[-1].list, yyvsp[0].node); }
    break;

  case 253:
#line 1704 "gram.y"
    { yyval.list = NIL; }
    break;

  case 254:
#line 1709 "gram.y"
    {
					switch (nodeTag(yyvsp[0].node))
					{
						case T_Constraint:
							{
								Constraint *n = (Constraint *)yyvsp[0].node;
								n->name = yyvsp[-1].str;
							}
							break;
						case T_FkConstraint:
							{
								FkConstraint *n = (FkConstraint *)yyvsp[0].node;
								n->constr_name = yyvsp[-1].str;
							}
							break;
						default:
							break;
					}
					yyval.node = yyvsp[0].node;
				}
    break;

  case 255:
#line 1729 "gram.y"
    { yyval.node = yyvsp[0].node; }
    break;

  case 256:
#line 1730 "gram.y"
    { yyval.node = yyvsp[0].node; }
    break;

  case 257:
#line 1750 "gram.y"
    {
					Constraint *n = makeNode(Constraint);
					n->contype = CONSTR_NOTNULL;
					n->name = NULL;
					n->raw_expr = NULL;
					n->cooked_expr = NULL;
					n->keys = NULL;
					yyval.node = (Node *)n;
				}
    break;

  case 258:
#line 1760 "gram.y"
    {
					Constraint *n = makeNode(Constraint);
					n->contype = CONSTR_NULL;
					n->name = NULL;
					n->raw_expr = NULL;
					n->cooked_expr = NULL;
					n->keys = NULL;
					yyval.node = (Node *)n;
				}
    break;

  case 259:
#line 1770 "gram.y"
    {
					Constraint *n = makeNode(Constraint);
					n->contype = CONSTR_UNIQUE;
					n->name = NULL;
					n->raw_expr = NULL;
					n->cooked_expr = NULL;
					n->keys = NULL;
					yyval.node = (Node *)n;
				}
    break;

  case 260:
#line 1780 "gram.y"
    {
					Constraint *n = makeNode(Constraint);
					n->contype = CONSTR_TIMESTAMPCOLUMN;
					n->name = NULL;
					n->raw_expr = NULL;
					n->cooked_expr = NULL;
					n->keys = NULL;
					yyval.node = (Node *)n;
				}
    break;

  case 261:
#line 1791 "gram.y"
    {
					Constraint *n = makeNode(Constraint);
					n->contype = CONSTR_PRIMARY;
					n->name = NULL;
					n->raw_expr = NULL;
					n->cooked_expr = NULL;
					n->keys = NULL;
					yyval.node = (Node *)n;
				}
    break;

  case 262:
#line 1801 "gram.y"
    {
					Constraint *n = makeNode(Constraint);
					n->contype = CONSTR_CHECK;
					n->name = NULL;
					n->raw_expr = yyvsp[-1].node;
					n->cooked_expr = NULL;
					n->keys = NULL;
					yyval.node = (Node *)n;
				}
    break;

  case 263:
#line 1811 "gram.y"
    {
					Constraint *n = makeNode(Constraint);
					n->contype = CONSTR_DEFAULT;
					n->name = NULL;
					if (exprIsNullConstant(yyvsp[0].node))
					{
						/* DEFAULT NULL should be reported as empty expr */
						n->raw_expr = NULL;
					}
					else
					{
						n->raw_expr = yyvsp[0].node;
					}
					n->cooked_expr = NULL;
					n->keys = NULL;
					yyval.node = (Node *)n;
				}
    break;

  case 264:
#line 1829 "gram.y"
    {
					FkConstraint *n = makeNode(FkConstraint);
					n->constr_name		= NULL;
					n->pktable			= yyvsp[-3].range;
					n->fk_attrs			= NIL;
					n->pk_attrs			= yyvsp[-2].list;
					n->fk_matchtype		= yyvsp[-1].ival;
					n->fk_upd_action	= (char) (yyvsp[0].ival >> 8);
					n->fk_del_action	= (char) (yyvsp[0].ival & 0xFF);
					n->deferrable		= FALSE;
					n->initdeferred		= FALSE;
					yyval.node = (Node *)n;
				}
    break;

  case 265:
#line 1857 "gram.y"
    {
					Constraint *n = makeNode(Constraint);
					n->contype = CONSTR_ATTR_DEFERRABLE;
					yyval.node = (Node *)n;
				}
    break;

  case 266:
#line 1863 "gram.y"
    {
					Constraint *n = makeNode(Constraint);
					n->contype = CONSTR_ATTR_NOT_DEFERRABLE;
					yyval.node = (Node *)n;
				}
    break;

  case 267:
#line 1869 "gram.y"
    {
					Constraint *n = makeNode(Constraint);
					n->contype = CONSTR_ATTR_DEFERRED;
					yyval.node = (Node *)n;
				}
    break;

  case 268:
#line 1875 "gram.y"
    {
					Constraint *n = makeNode(Constraint);
					n->contype = CONSTR_ATTR_IMMEDIATE;
					yyval.node = (Node *)n;
				}
    break;

  case 269:
#line 1889 "gram.y"
    {
					elog(ERROR, "LIKE in table definitions not yet supported");
					yyval.node = NULL;
				}
    break;

  case 270:
#line 1902 "gram.y"
    {
					switch (nodeTag(yyvsp[0].node))
					{
						case T_Constraint:
							{
								Constraint *n = (Constraint *)yyvsp[0].node;
								n->name = yyvsp[-1].str;
							}
							break;
						case T_FkConstraint:
							{
								FkConstraint *n = (FkConstraint *)yyvsp[0].node;
								n->constr_name = yyvsp[-1].str;
							}
							break;
						default:
							break;
					}
					yyval.node = yyvsp[0].node;
				}
    break;

  case 271:
#line 1922 "gram.y"
    { yyval.node = yyvsp[0].node; }
    break;

  case 272:
#line 1927 "gram.y"
    {
					Constraint *n = makeNode(Constraint);
					n->contype = CONSTR_CHECK;
					n->name = NULL;
					n->raw_expr = yyvsp[-1].node;
					n->cooked_expr = NULL;
					yyval.node = (Node *)n;
				}
    break;

  case 273:
#line 1936 "gram.y"
    {
					Constraint *n = makeNode(Constraint);
					n->contype = CONSTR_UNIQUE;
					n->name = NULL;
					n->raw_expr = NULL;
					n->cooked_expr = NULL;
					n->keys = yyvsp[-1].list;
					yyval.node = (Node *)n;
				}
    break;

  case 274:
#line 1946 "gram.y"
    {
					Constraint *n = makeNode(Constraint);
					n->contype = CONSTR_PRIMARY;
					n->name = NULL;
					n->raw_expr = NULL;
					n->cooked_expr = NULL;
					n->keys = yyvsp[-1].list;
					yyval.node = (Node *)n;
				}
    break;

  case 275:
#line 1957 "gram.y"
    {
					FkConstraint *n = makeNode(FkConstraint);
					n->constr_name		= NULL;
					n->pktable			= yyvsp[-4].range;
					n->fk_attrs			= yyvsp[-7].list;
					n->pk_attrs			= yyvsp[-3].list;
					n->fk_matchtype		= yyvsp[-2].ival;
					n->fk_upd_action	= (char) (yyvsp[-1].ival >> 8);
					n->fk_del_action	= (char) (yyvsp[-1].ival & 0xFF);
					n->deferrable		= (yyvsp[0].ival & 1) != 0;
					n->initdeferred		= (yyvsp[0].ival & 2) != 0;
					yyval.node = (Node *)n;
				}
    break;

  case 276:
#line 1973 "gram.y"
    { yyval.list = yyvsp[-1].list; }
    break;

  case 277:
#line 1974 "gram.y"
    { yyval.list = NIL; }
    break;

  case 278:
#line 1978 "gram.y"
    { yyval.list = makeList1(yyvsp[0].node); }
    break;

  case 279:
#line 1979 "gram.y"
    { yyval.list = lappend(yyvsp[-2].list, yyvsp[0].node); }
    break;

  case 280:
#line 1983 "gram.y"
    {
					yyval.node = (Node *) makeString(yyvsp[0].str);
				}
    break;

  case 281:
#line 1989 "gram.y"
    {
				yyval.ival = FKCONSTR_MATCH_FULL;
			}
    break;

  case 282:
#line 1993 "gram.y"
    {
				elog(ERROR, "FOREIGN KEY/MATCH PARTIAL not yet implemented");
				yyval.ival = FKCONSTR_MATCH_PARTIAL;
			}
    break;

  case 283:
#line 1998 "gram.y"
    {
				yyval.ival = FKCONSTR_MATCH_UNSPECIFIED;
			}
    break;

  case 284:
#line 2002 "gram.y"
    {
				yyval.ival = FKCONSTR_MATCH_UNSPECIFIED;
			}
    break;

  case 285:
#line 2015 "gram.y"
    { yyval.ival = (yyvsp[0].ival << 8) | (FKCONSTR_ACTION_NOACTION & 0xFF); }
    break;

  case 286:
#line 2017 "gram.y"
    { yyval.ival = (FKCONSTR_ACTION_NOACTION << 8) | (yyvsp[0].ival & 0xFF); }
    break;

  case 287:
#line 2019 "gram.y"
    { yyval.ival = (yyvsp[-1].ival << 8) | (yyvsp[0].ival & 0xFF); }
    break;

  case 288:
#line 2021 "gram.y"
    { yyval.ival = (yyvsp[0].ival << 8) | (yyvsp[-1].ival & 0xFF); }
    break;

  case 289:
#line 2023 "gram.y"
    { yyval.ival = (FKCONSTR_ACTION_NOACTION << 8) | (FKCONSTR_ACTION_NOACTION & 0xFF); }
    break;

  case 290:
#line 2026 "gram.y"
    { yyval.ival = yyvsp[0].ival; }
    break;

  case 291:
#line 2029 "gram.y"
    { yyval.ival = yyvsp[0].ival; }
    break;

  case 292:
#line 2033 "gram.y"
    { yyval.ival = FKCONSTR_ACTION_NOACTION; }
    break;

  case 293:
#line 2034 "gram.y"
    { yyval.ival = FKCONSTR_ACTION_RESTRICT; }
    break;

  case 294:
#line 2035 "gram.y"
    { yyval.ival = FKCONSTR_ACTION_CASCADE; }
    break;

  case 295:
#line 2036 "gram.y"
    { yyval.ival = FKCONSTR_ACTION_SETNULL; }
    break;

  case 296:
#line 2037 "gram.y"
    { yyval.ival = FKCONSTR_ACTION_SETDEFAULT; }
    break;

  case 297:
#line 2040 "gram.y"
    { yyval.list = yyvsp[-1].list; }
    break;

  case 298:
#line 2041 "gram.y"
    { yyval.list = NIL; }
    break;

  case 299:
#line 2045 "gram.y"
    { yyval.boolean = TRUE; }
    break;

  case 300:
#line 2046 "gram.y"
    { yyval.boolean = FALSE; }
    break;

  case 301:
#line 2047 "gram.y"
    { yyval.boolean = TRUE; }
    break;

  case 302:
#line 2058 "gram.y"
    {
					/*
					 * When the SelectStmt is a set-operation tree, we must
					 * stuff the INTO information into the leftmost component
					 * Select, because that's where analyze.c will expect
					 * to find it.	Similarly, the output column names must
					 * be attached to that Select's target list.
					 */
					SelectStmt *n = findLeftmostSelect((SelectStmt *) yyvsp[0].node);
					if (n->into != NULL)
						elog(ERROR, "CREATE TABLE AS may not specify INTO");
					yyvsp[-3].range->istemp = yyvsp[-5].boolean;
					n->into = yyvsp[-3].range;
					n->intoColNames = yyvsp[-2].list;
					yyval.node = yyvsp[0].node;
				}
    break;

  case 303:
#line 2077 "gram.y"
    { yyval.list = yyvsp[-1].list; }
    break;

  case 304:
#line 2078 "gram.y"
    { yyval.list = NIL; }
    break;

  case 305:
#line 2082 "gram.y"
    { yyval.list = makeList1(yyvsp[0].node); }
    break;

  case 306:
#line 2083 "gram.y"
    { yyval.list = lappend(yyvsp[-2].list, yyvsp[0].node); }
    break;

  case 307:
#line 2088 "gram.y"
    {
					ColumnDef *n = makeNode(ColumnDef);
					n->colname = yyvsp[0].str;
					n->typename = NULL;
					n->inhcount = 0;
					n->is_local = true;
					n->is_not_null = false;
					n->raw_default = NULL;
					n->cooked_default = NULL;
					n->constraints = NIL;
					n->support = NULL;
					yyval.node = (Node *)n;
				}
    break;

  case 308:
#line 2113 "gram.y"
    {
					CreateSeqStmt *n = makeNode(CreateSeqStmt);
					yyvsp[-1].range->istemp = yyvsp[-3].boolean;
					n->sequence = yyvsp[-1].range;
					n->options = yyvsp[0].list;
					yyval.node = (Node *)n;
				}
    break;

  case 309:
#line 2122 "gram.y"
    { yyval.list = lappend(yyvsp[-1].list, yyvsp[0].defelt); }
    break;

  case 310:
#line 2123 "gram.y"
    { yyval.list = NIL; }
    break;

  case 311:
#line 2127 "gram.y"
    {
					yyval.defelt = makeDefElem("cache", (Node *)yyvsp[0].value);
				}
    break;

  case 312:
#line 2131 "gram.y"
    {
					yyval.defelt = makeDefElem("cycle", (Node *)NULL);
				}
    break;

  case 313:
#line 2135 "gram.y"
    {
					yyval.defelt = makeDefElem("increment", (Node *)yyvsp[0].value);
				}
    break;

  case 314:
#line 2139 "gram.y"
    {
					yyval.defelt = makeDefElem("maxvalue", (Node *)yyvsp[0].value);
				}
    break;

  case 315:
#line 2143 "gram.y"
    {
					yyval.defelt = makeDefElem("minvalue", (Node *)yyvsp[0].value);
				}
    break;

  case 316:
#line 2147 "gram.y"
    {
					yyval.defelt = makeDefElem("start", (Node *)yyvsp[0].value);
				}
    break;

  case 317:
#line 2153 "gram.y"
    { yyval.value = yyvsp[0].value; }
    break;

  case 318:
#line 2154 "gram.y"
    { yyval.value = yyvsp[0].value; }
    break;

  case 319:
#line 2157 "gram.y"
    { yyval.value = makeFloat(yyvsp[0].str); }
    break;

  case 320:
#line 2159 "gram.y"
    {
					yyval.value = makeFloat(yyvsp[0].str);
					doNegateFloat(yyval.value);
				}
    break;

  case 321:
#line 2167 "gram.y"
    {
					yyval.value = makeInteger(yyvsp[0].ival);
				}
    break;

  case 322:
#line 2171 "gram.y"
    {
					yyval.value = makeInteger(yyvsp[0].ival);
					yyval.value->val.ival = - yyval.value->val.ival;
				}
    break;

  case 323:
#line 2188 "gram.y"
    {
				CreatePLangStmt *n = makeNode(CreatePLangStmt);
				n->plname = yyvsp[-4].str;
				n->plhandler = yyvsp[-2].list;
				n->plvalidator = yyvsp[-1].list;
				n->pltrusted = yyvsp[-7].boolean;
				yyval.node = (Node *)n;
			}
    break;

  case 324:
#line 2199 "gram.y"
    { yyval.boolean = TRUE; }
    break;

  case 325:
#line 2200 "gram.y"
    { yyval.boolean = FALSE; }
    break;

  case 326:
#line 2209 "gram.y"
    { yyval.list = makeList1(makeString(yyvsp[0].str)); }
    break;

  case 327:
#line 2210 "gram.y"
    { yyval.list = yyvsp[0].list; }
    break;

  case 328:
#line 2214 "gram.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 329:
#line 2215 "gram.y"
    { yyval.str = ""; }
    break;

  case 330:
#line 2219 "gram.y"
    { yyval.list = yyvsp[0].list; }
    break;

  case 331:
#line 2220 "gram.y"
    { yyval.list = NULL; }
    break;

  case 332:
#line 2225 "gram.y"
    {
					DropPLangStmt *n = makeNode(DropPLangStmt);
					n->plname = yyvsp[-1].str;
					n->behavior = yyvsp[0].dbehavior;
					yyval.node = (Node *)n;
				}
    break;

  case 333:
#line 2234 "gram.y"
    {}
    break;

  case 334:
#line 2235 "gram.y"
    {}
    break;

  case 335:
#line 2250 "gram.y"
    {
					CreateTrigStmt *n = makeNode(CreateTrigStmt);
					n->trigname = yyvsp[-11].str;
					n->relation = yyvsp[-7].range;
					n->funcname = yyvsp[-3].list;
					n->args = yyvsp[-1].list;
					n->before = yyvsp[-10].boolean;
					n->row = yyvsp[-6].boolean;
					memcpy (n->actions, yyvsp[-9].str, 4);
					n->lang = NULL;		/* unused */
					n->text = NULL;		/* unused */
					n->attr = NULL;		/* unused */
					n->when = NULL;		/* unused */

					n->isconstraint  = FALSE;
					n->deferrable	 = FALSE;
					n->initdeferred  = FALSE;
					n->constrrel = NULL;
					yyval.node = (Node *)n;
				}
    break;

  case 336:
#line 2275 "gram.y"
    {
					CreateTrigStmt *n = makeNode(CreateTrigStmt);
					n->trigname = yyvsp[-15].str;
					n->relation = yyvsp[-11].range;
					n->funcname = yyvsp[-3].list;
					n->args = yyvsp[-1].list;
					n->before = FALSE;
					n->row = TRUE;
					memcpy (n->actions, yyvsp[-13].str, 4);
					n->lang = NULL;		/* unused */
					n->text = NULL;		/* unused */
					n->attr = NULL;		/* unused */
					n->when = NULL;		/* unused */

					n->isconstraint  = TRUE;
					n->deferrable = (yyvsp[-9].ival & 1) != 0;
					n->initdeferred = (yyvsp[-9].ival & 2) != 0;

					n->constrrel = yyvsp[-10].range;
					yyval.node = (Node *)n;
				}
    break;

  case 337:
#line 2299 "gram.y"
    { yyval.boolean = TRUE; }
    break;

  case 338:
#line 2300 "gram.y"
    { yyval.boolean = FALSE; }
    break;

  case 339:
#line 2305 "gram.y"
    {
					char *e = palloc (4);
					e[0] = yyvsp[0].chr; e[1] = 0; yyval.str = e;
				}
    break;

  case 340:
#line 2310 "gram.y"
    {
					char *e = palloc (4);
					e[0] = yyvsp[-2].chr; e[1] = yyvsp[0].chr; e[2] = 0; yyval.str = e;
				}
    break;

  case 341:
#line 2315 "gram.y"
    {
					char *e = palloc (4);
					e[0] = yyvsp[-4].chr; e[1] = yyvsp[-2].chr; e[2] = yyvsp[0].chr; e[3] = 0;
					yyval.str = e;
				}
    break;

  case 342:
#line 2323 "gram.y"
    { yyval.chr = 'i'; }
    break;

  case 343:
#line 2324 "gram.y"
    { yyval.chr = 'd'; }
    break;

  case 344:
#line 2325 "gram.y"
    { yyval.chr = 'u'; }
    break;

  case 345:
#line 2330 "gram.y"
    {
					yyval.boolean = yyvsp[0].boolean;
				}
    break;

  case 346:
#line 2336 "gram.y"
    {}
    break;

  case 347:
#line 2337 "gram.y"
    {}
    break;

  case 348:
#line 2341 "gram.y"
    { yyval.boolean = TRUE; }
    break;

  case 349:
#line 2342 "gram.y"
    { yyval.boolean = FALSE; }
    break;

  case 350:
#line 2346 "gram.y"
    { yyval.list = makeList1(yyvsp[0].value); }
    break;

  case 351:
#line 2347 "gram.y"
    { yyval.list = lappend(yyvsp[-2].list, yyvsp[0].value); }
    break;

  case 352:
#line 2348 "gram.y"
    { yyval.list = NIL; }
    break;

  case 353:
#line 2353 "gram.y"
    {
					char buf[64];
					snprintf (buf, sizeof(buf), "%d", yyvsp[0].ival);
					yyval.value = makeString(pstrdup(buf));
				}
    break;

  case 354:
#line 2358 "gram.y"
    { yyval.value = makeString(yyvsp[0].str); }
    break;

  case 355:
#line 2359 "gram.y"
    { yyval.value = makeString(yyvsp[0].str); }
    break;

  case 356:
#line 2360 "gram.y"
    { yyval.value = makeString(yyvsp[0].str); }
    break;

  case 357:
#line 2361 "gram.y"
    { yyval.value = makeString(yyvsp[0].str); }
    break;

  case 358:
#line 2362 "gram.y"
    { yyval.value = makeString(yyvsp[0].str); }
    break;

  case 359:
#line 2366 "gram.y"
    { yyval.range = yyvsp[0].range; }
    break;

  case 360:
#line 2367 "gram.y"
    { yyval.range = NULL; }
    break;

  case 361:
#line 2372 "gram.y"
    { yyval.ival = yyvsp[0].ival; }
    break;

  case 362:
#line 2374 "gram.y"
    {
					if (yyvsp[-1].ival == 0 && yyvsp[0].ival != 0)
						elog(ERROR,
						"INITIALLY DEFERRED constraint must be DEFERRABLE");
					yyval.ival = yyvsp[-1].ival | yyvsp[0].ival;
				}
    break;

  case 363:
#line 2381 "gram.y"
    {
					if (yyvsp[0].ival != 0)
						yyval.ival = 3;
					else
						yyval.ival = 0;
				}
    break;

  case 364:
#line 2388 "gram.y"
    {
					if (yyvsp[0].ival == 0 && yyvsp[-1].ival != 0)
						elog(ERROR,
						"INITIALLY DEFERRED constraint must be DEFERRABLE");
					yyval.ival = yyvsp[-1].ival | yyvsp[0].ival;
				}
    break;

  case 365:
#line 2395 "gram.y"
    { yyval.ival = 0; }
    break;

  case 366:
#line 2399 "gram.y"
    { yyval.ival = 0; }
    break;

  case 367:
#line 2400 "gram.y"
    { yyval.ival = 1; }
    break;

  case 368:
#line 2404 "gram.y"
    { yyval.ival = 0; }
    break;

  case 369:
#line 2405 "gram.y"
    { yyval.ival = 2; }
    break;

  case 370:
#line 2411 "gram.y"
    {
					DropPropertyStmt *n = makeNode(DropPropertyStmt);
					n->relation = yyvsp[-1].range;
					n->property = yyvsp[-3].str;
					n->behavior = yyvsp[0].dbehavior;
					n->removeType = DROP_TRIGGER;
					yyval.node = (Node *) n;
				}
    break;

  case 371:
#line 2433 "gram.y"
    {
					CreateTrigStmt *n = makeNode(CreateTrigStmt);
					n->trigname = yyvsp[-5].str;
					n->args = makeList1(yyvsp[-2].node);
					n->isconstraint  = TRUE;
					n->deferrable = (yyvsp[0].ival & 1) != 0;
					n->initdeferred = (yyvsp[0].ival & 2) != 0;

					elog(ERROR, "CREATE ASSERTION is not yet supported");

					yyval.node = (Node *)n;
				}
    break;

  case 372:
#line 2449 "gram.y"
    {
					DropPropertyStmt *n = makeNode(DropPropertyStmt);
					n->relation = NULL;
					n->property = yyvsp[-1].str;
					n->behavior = yyvsp[0].dbehavior;
					n->removeType = DROP_TRIGGER; /* XXX */
					elog(ERROR, "DROP ASSERTION is not yet supported");
					yyval.node = (Node *) n;
				}
    break;

  case 373:
#line 2470 "gram.y"
    {
					DefineStmt *n = makeNode(DefineStmt);
					n->defType = AGGREGATE;
					n->defnames = yyvsp[-1].list;
					n->definition = yyvsp[0].list;
					yyval.node = (Node *)n;
				}
    break;

  case 374:
#line 2478 "gram.y"
    {
					DefineStmt *n = makeNode(DefineStmt);
					n->defType = OPERATOR;
					n->defnames = yyvsp[-1].list;
					n->definition = yyvsp[0].list;
					yyval.node = (Node *)n;
				}
    break;

  case 375:
#line 2486 "gram.y"
    {
					DefineStmt *n = makeNode(DefineStmt);
					n->defType = TYPE_P;
					n->defnames = yyvsp[-1].list;
					n->definition = yyvsp[0].list;
					yyval.node = (Node *)n;
				}
    break;

  case 376:
#line 2494 "gram.y"
    {
					CompositeTypeStmt *n = makeNode(CompositeTypeStmt);
					RangeVar *r = makeNode(RangeVar);

					/* can't use qualified_name, sigh */
					switch (length(yyvsp[-4].list))
					{
						case 1:
							r->catalogname = NULL;
							r->schemaname = NULL;
							r->relname = strVal(lfirst(yyvsp[-4].list));
							break;
						case 2:
							r->catalogname = NULL;
							r->schemaname = strVal(lfirst(yyvsp[-4].list));
							r->relname = strVal(lsecond(yyvsp[-4].list));
							break;
						case 3:
							r->catalogname = strVal(lfirst(yyvsp[-4].list));
							r->schemaname = strVal(lsecond(yyvsp[-4].list));
							r->relname = strVal(lfirst(lnext(lnext(yyvsp[-4].list))));
							break;
						default:
							elog(ERROR,
								 "Improper qualified name (too many dotted names): %s",
								 NameListToString(yyvsp[-4].list));
							break;
					}
					n->typevar = r;
					n->coldeflist = yyvsp[-1].list;
					yyval.node = (Node *)n;
				}
    break;

  case 377:
#line 2527 "gram.y"
    {
					DefineStmt *n = makeNode(DefineStmt);
					n->defType = CHARACTER;
					n->defnames = yyvsp[-3].list;
					n->definition = yyvsp[-1].list;
					yyval.node = (Node *)n;
				}
    break;

  case 378:
#line 2536 "gram.y"
    {yyval.list=yyvsp[-1].list;}
    break;

  case 379:
#line 2540 "gram.y"
    { yyval.list = makeList1(yyvsp[0].defelt); }
    break;

  case 380:
#line 2542 "gram.y"
    {
  yyval.list = lappend(yyvsp[-2].list,yyvsp[0].defelt);
}
    break;

  case 381:
#line 2548 "gram.y"
    {
  yyval.defelt = makeDefElem(yyvsp[-2].str, (Node*)yyvsp[0].list);
}
    break;

  case 382:
#line 2552 "gram.y"
    {
  yyval.defelt = makeDefElem(yyvsp[-2].str, (Node*)makeList1(makeString(yyvsp[0].str)));
}
    break;

  case 383:
#line 2557 "gram.y"
    { yyval.list = yyvsp[-1].list; }
    break;

  case 384:
#line 2561 "gram.y"
    { yyval.list = makeList1(yyvsp[0].defelt); }
    break;

  case 385:
#line 2562 "gram.y"
    { yyval.list = lappend(yyvsp[-2].list, yyvsp[0].defelt); }
    break;

  case 386:
#line 2566 "gram.y"
    {
					yyval.defelt = makeDefElem(yyvsp[-2].str, (Node *)yyvsp[0].node);
				}
    break;

  case 387:
#line 2570 "gram.y"
    {
					yyval.defelt = makeDefElem(yyvsp[0].str, (Node *)NULL);
				}
    break;

  case 388:
#line 2576 "gram.y"
    { yyval.node = (Node *)yyvsp[0].typnam; }
    break;

  case 389:
#line 2577 "gram.y"
    { yyval.node = (Node *)yyvsp[0].list; }
    break;

  case 390:
#line 2578 "gram.y"
    { yyval.node = (Node *)yyvsp[0].value; }
    break;

  case 391:
#line 2579 "gram.y"
    { yyval.node = (Node *)makeString(yyvsp[0].str); }
    break;

  case 392:
#line 2594 "gram.y"
    {
					CreateOpClassStmt *n = makeNode(CreateOpClassStmt);
					n->opclassname = yyvsp[-8].list;
					n->isDefault = yyvsp[-7].boolean;
					n->datatype = yyvsp[-4].typnam;
					n->amname = yyvsp[-2].str;
					n->items = yyvsp[0].list;
					yyval.node = (Node *) n;
				}
    break;

  case 393:
#line 2606 "gram.y"
    { yyval.list = makeList1(yyvsp[0].node); }
    break;

  case 394:
#line 2607 "gram.y"
    { yyval.list = lappend(yyvsp[-2].list, yyvsp[0].node); }
    break;

  case 395:
#line 2612 "gram.y"
    {
					CreateOpClassItem *n = makeNode(CreateOpClassItem);
					n->itemtype = OPCLASS_ITEM_OPERATOR;
					n->name = yyvsp[-1].list;
					n->args = NIL;
					n->number = yyvsp[-2].ival;
					n->recheck = yyvsp[0].boolean;
					yyval.node = (Node *) n;
				}
    break;

  case 396:
#line 2622 "gram.y"
    {
					CreateOpClassItem *n = makeNode(CreateOpClassItem);
					n->itemtype = OPCLASS_ITEM_OPERATOR;
					n->name = yyvsp[-4].list;
					n->args = yyvsp[-2].list;
					n->number = yyvsp[-5].ival;
					n->recheck = yyvsp[0].boolean;
					yyval.node = (Node *) n;
				}
    break;

  case 397:
#line 2632 "gram.y"
    {
					CreateOpClassItem *n = makeNode(CreateOpClassItem);
					n->itemtype = OPCLASS_ITEM_FUNCTION;
					n->name = yyvsp[-1].list;
					n->args = yyvsp[0].list;
					n->number = yyvsp[-2].ival;
					yyval.node = (Node *) n;
				}
    break;

  case 398:
#line 2641 "gram.y"
    {
					CreateOpClassItem *n = makeNode(CreateOpClassItem);
					n->itemtype = OPCLASS_ITEM_STORAGETYPE;
					n->storedtype = yyvsp[0].typnam;
					yyval.node = (Node *) n;
				}
    break;

  case 399:
#line 2649 "gram.y"
    { yyval.boolean = TRUE; }
    break;

  case 400:
#line 2650 "gram.y"
    { yyval.boolean = FALSE; }
    break;

  case 401:
#line 2653 "gram.y"
    { yyval.boolean = TRUE; }
    break;

  case 402:
#line 2654 "gram.y"
    { yyval.boolean = FALSE; }
    break;

  case 403:
#line 2660 "gram.y"
    {
					RemoveOpClassStmt *n = makeNode(RemoveOpClassStmt);
					n->opclassname = yyvsp[-3].list;
					n->amname = yyvsp[-1].str;
					n->behavior = yyvsp[0].dbehavior;
					yyval.node = (Node *) n;
				}
    break;

  case 404:
#line 2679 "gram.y"
    {
					DropStmt *n = makeNode(DropStmt);
					n->removeType = yyvsp[-2].ival;
					n->objects = yyvsp[-1].list;
					n->behavior = yyvsp[0].dbehavior;
					yyval.node = (Node *)n;
				}
    break;

  case 405:
#line 2688 "gram.y"
    { yyval.ival = DROP_TABLE; }
    break;

  case 406:
#line 2689 "gram.y"
    { yyval.ival = DROP_SEQUENCE; }
    break;

  case 407:
#line 2690 "gram.y"
    { yyval.ival = DROP_VIEW; }
    break;

  case 408:
#line 2691 "gram.y"
    { yyval.ival = DROP_INDEX; }
    break;

  case 409:
#line 2692 "gram.y"
    { yyval.ival = DROP_TYPE; }
    break;

  case 410:
#line 2693 "gram.y"
    { yyval.ival = DROP_DOMAIN; }
    break;

  case 411:
#line 2694 "gram.y"
    { yyval.ival = DROP_CONVERSION; }
    break;

  case 412:
#line 2695 "gram.y"
    { yyval.ival = DROP_SCHEMA; }
    break;

  case 413:
#line 2696 "gram.y"
    { yyval.ival = DROP_STREAM; }
    break;

  case 414:
#line 2697 "gram.y"
    { yyval.ival = DROP_WRAPPER; }
    break;

  case 415:
#line 2701 "gram.y"
    { yyval.list = makeList1(yyvsp[0].list); }
    break;

  case 416:
#line 2702 "gram.y"
    { yyval.list = lappend(yyvsp[-2].list, yyvsp[0].list); }
    break;

  case 417:
#line 2705 "gram.y"
    { yyval.list = makeList1(makeString(yyvsp[0].str)); }
    break;

  case 418:
#line 2706 "gram.y"
    { yyval.list = yyvsp[0].list; }
    break;

  case 419:
#line 2718 "gram.y"
    {
					TruncateStmt *n = makeNode(TruncateStmt);
					n->relation = yyvsp[0].range;
					yyval.node = (Node *)n;
				}
    break;

  case 420:
#line 2740 "gram.y"
    {
					CommentStmt *n = makeNode(CommentStmt);
					n->objtype = yyvsp[-3].ival;
					n->objname = yyvsp[-2].list;
					n->objargs = NIL;
					n->comment = yyvsp[0].str;
					yyval.node = (Node *) n;
				}
    break;

  case 421:
#line 2750 "gram.y"
    {
					CommentStmt *n = makeNode(CommentStmt);
					n->objtype = COMMENT_ON_AGGREGATE;
					n->objname = yyvsp[-5].list;
					n->objargs = makeList1(yyvsp[-3].typnam);
					n->comment = yyvsp[0].str;
					yyval.node = (Node *) n;
				}
    break;

  case 422:
#line 2759 "gram.y"
    {
					CommentStmt *n = makeNode(CommentStmt);
					n->objtype = COMMENT_ON_FUNCTION;
					n->objname = yyvsp[-3].list;
					n->objargs = yyvsp[-2].list;
					n->comment = yyvsp[0].str;
					yyval.node = (Node *) n;
				}
    break;

  case 423:
#line 2769 "gram.y"
    {
					CommentStmt *n = makeNode(CommentStmt);
					n->objtype = COMMENT_ON_OPERATOR;
					n->objname = yyvsp[-5].list;
					n->objargs = yyvsp[-3].list;
					n->comment = yyvsp[0].str;
					yyval.node = (Node *) n;
				}
    break;

  case 424:
#line 2778 "gram.y"
    {
					CommentStmt *n = makeNode(CommentStmt);
					n->objtype = COMMENT_ON_CONSTRAINT;
					n->objname = lappend(yyvsp[-2].list, makeString(yyvsp[-4].str));
					n->objargs = NIL;
					n->comment = yyvsp[0].str;
					yyval.node = (Node *) n;
				}
    break;

  case 425:
#line 2787 "gram.y"
    {
					CommentStmt *n = makeNode(CommentStmt);
					n->objtype = COMMENT_ON_RULE;
					n->objname = lappend(yyvsp[-2].list, makeString(yyvsp[-4].str));
					n->objargs = NIL;
					n->comment = yyvsp[0].str;
					yyval.node = (Node *) n;
				}
    break;

  case 426:
#line 2796 "gram.y"
    {
					/* Obsolete syntax supported for awhile for compatibility */
					CommentStmt *n = makeNode(CommentStmt);
					n->objtype = COMMENT_ON_RULE;
					n->objname = makeList1(makeString(yyvsp[-2].str));
					n->objargs = NIL;
					n->comment = yyvsp[0].str;
					yyval.node = (Node *) n;
				}
    break;

  case 427:
#line 2806 "gram.y"
    {
					CommentStmt *n = makeNode(CommentStmt);
					n->objtype = COMMENT_ON_TRIGGER;
					n->objname = lappend(yyvsp[-2].list, makeString(yyvsp[-4].str));
					n->objargs = NIL;
					n->comment = yyvsp[0].str;
					yyval.node = (Node *) n;
				}
    break;

  case 428:
#line 2817 "gram.y"
    { yyval.ival = COMMENT_ON_COLUMN; }
    break;

  case 429:
#line 2818 "gram.y"
    { yyval.ival = COMMENT_ON_DATABASE; }
    break;

  case 430:
#line 2819 "gram.y"
    { yyval.ival = COMMENT_ON_SCHEMA; }
    break;

  case 431:
#line 2820 "gram.y"
    { yyval.ival = COMMENT_ON_INDEX; }
    break;

  case 432:
#line 2821 "gram.y"
    { yyval.ival = COMMENT_ON_SEQUENCE; }
    break;

  case 433:
#line 2822 "gram.y"
    { yyval.ival = COMMENT_ON_TABLE; }
    break;

  case 434:
#line 2823 "gram.y"
    { yyval.ival = COMMENT_ON_TYPE; }
    break;

  case 435:
#line 2824 "gram.y"
    { yyval.ival = COMMENT_ON_TYPE; }
    break;

  case 436:
#line 2825 "gram.y"
    { yyval.ival = COMMENT_ON_VIEW; }
    break;

  case 437:
#line 2829 "gram.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 438:
#line 2830 "gram.y"
    { yyval.str = NULL; }
    break;

  case 439:
#line 2843 "gram.y"
    {
					FetchStmt *n = makeNode(FetchStmt);
					if (yyvsp[-3].ival == RELATIVE)
					{
						if (yyvsp[-2].ival == 0)
							elog(ERROR,
					"FETCH / RELATIVE at current position is not supported");
						yyvsp[-3].ival = FORWARD;
					}
					if (yyvsp[-2].ival < 0)
					{
						yyvsp[-2].ival = -yyvsp[-2].ival;
						yyvsp[-3].ival = ((yyvsp[-3].ival == FORWARD)? BACKWARD: FORWARD);
					}
					n->direction = yyvsp[-3].ival;
					n->howMany = yyvsp[-2].ival;
					n->portalname = yyvsp[0].str;
					n->ismove = FALSE;
                                        n->iswindow = FALSE;
					yyval.node = (Node *)n;
				}
    break;

  case 440:
#line 2865 "gram.y"
    {
					FetchStmt *n = makeNode(FetchStmt);
                                        n->howMany = 0;
                                        n->direction = FORWARD;
					n->portalname = yyvsp[0].str;
					n->ismove = FALSE;
                                        n->iswindow = TRUE;
					yyval.node = (Node *)n;
				}
    break;

  case 441:
#line 2875 "gram.y"
    {
					FetchStmt *n = makeNode(FetchStmt);
					if (yyvsp[-2].ival < 0)
					{
						n->howMany = -yyvsp[-2].ival;
						n->direction = BACKWARD;
					}
					else
					{
						n->direction = FORWARD;
						n->howMany = yyvsp[-2].ival;
					}
					n->portalname = yyvsp[0].str;
					n->ismove = FALSE;
                                        n->iswindow = FALSE;
					yyval.node = (Node *)n;
				}
    break;

  case 442:
#line 2893 "gram.y"
    {
					FetchStmt *n = makeNode(FetchStmt);
					if (yyvsp[-2].ival == RELATIVE)
					{
						yyvsp[-2].ival = FORWARD;
					}
					n->direction = yyvsp[-2].ival;
					n->howMany = 1;
					n->portalname = yyvsp[0].str;
					n->ismove = FALSE;
                                        n->iswindow = FALSE;
					yyval.node = (Node *)n;
				}
    break;

  case 443:
#line 2907 "gram.y"
    {
					FetchStmt *n = makeNode(FetchStmt);
					n->direction = FORWARD;
					n->howMany = 1;
					n->portalname = yyvsp[0].str;
					n->ismove = FALSE;
                                        n->iswindow = FALSE;
					yyval.node = (Node *)n;
				}
    break;

  case 444:
#line 2917 "gram.y"
    {
					FetchStmt *n = makeNode(FetchStmt);
					n->direction = FORWARD;
					n->howMany = 1;
					n->portalname = yyvsp[0].str;
					n->ismove = FALSE;
                                        n->iswindow = FALSE;
					yyval.node = (Node *)n;
				}
    break;

  case 445:
#line 2927 "gram.y"
    {
					FetchStmt *n = makeNode(FetchStmt);
					if (yyvsp[-2].ival < 0)
					{
						yyvsp[-2].ival = -yyvsp[-2].ival;
						yyvsp[-3].ival = ((yyvsp[-3].ival == FORWARD)? BACKWARD: FORWARD);
					}
					n->direction = yyvsp[-3].ival;
					n->howMany = yyvsp[-2].ival;
					n->portalname = yyvsp[0].str;
					n->ismove = TRUE;
                                        n->iswindow = FALSE;
					yyval.node = (Node *)n;
				}
    break;

  case 446:
#line 2942 "gram.y"
    {
					FetchStmt *n = makeNode(FetchStmt);
					if (yyvsp[-2].ival < 0)
					{
						n->howMany = -yyvsp[-2].ival;
						n->direction = BACKWARD;
					}
					else
					{
						n->direction = FORWARD;
						n->howMany = yyvsp[-2].ival;
					}
					n->portalname = yyvsp[0].str;
					n->ismove = TRUE;
                                        n->iswindow = FALSE;
					yyval.node = (Node *)n;
				}
    break;

  case 447:
#line 2960 "gram.y"
    {
					FetchStmt *n = makeNode(FetchStmt);
					n->direction = yyvsp[-2].ival;
					n->howMany = 1;
					n->portalname = yyvsp[0].str;
					n->ismove = TRUE;
                                        n->iswindow = FALSE;
					yyval.node = (Node *)n;
				}
    break;

  case 448:
#line 2970 "gram.y"
    {
					FetchStmt *n = makeNode(FetchStmt);
					n->direction = FORWARD;
					n->howMany = 1;
					n->portalname = yyvsp[0].str;
					n->ismove = TRUE;
                                        n->iswindow = FALSE;
					yyval.node = (Node *)n;
				}
    break;

  case 449:
#line 2980 "gram.y"
    {
					FetchStmt *n = makeNode(FetchStmt);
					n->direction = FORWARD;
					n->howMany = 1;
					n->portalname = yyvsp[0].str;
					n->ismove = TRUE;
                                        n->iswindow = FALSE;
					yyval.node = (Node *)n;
				}
    break;

  case 450:
#line 2990 "gram.y"
    {
				  /* Scrolling r/i/dstream cursor in time */
				  FetchStmt *n = makeNode(FetchStmt);
				  n->direction = yyvsp[-6].ival;
				  n->howMany = 1;
				  n->ismove = TRUE;
                                  n->iswindow = FALSE;
				  n->portalname = yyvsp[-5].str;

				  n->moveInterval = make_const(makeString(yyvsp[-1].str));
				  n->moveInterval =
				    (Const *) coerce_type((Node *) n->moveInterval,
							  n->moveInterval->consttype,
							  (Oid) INTERVALOID,
							  COERCION_IMPLICIT,
							  COERCE_IMPLICIT_CAST);
				  yyval.node = (Node *) n;
				}
    break;

  case 451:
#line 3010 "gram.y"
    { yyval.ival = FORWARD; }
    break;

  case 452:
#line 3011 "gram.y"
    { yyval.ival = BACKWARD; }
    break;

  case 453:
#line 3012 "gram.y"
    { yyval.ival = RELATIVE; }
    break;

  case 454:
#line 3014 "gram.y"
    {
					elog(NOTICE,
					"FETCH / ABSOLUTE not supported, using RELATIVE");
					yyval.ival = RELATIVE;
				}
    break;

  case 455:
#line 3022 "gram.y"
    { yyval.ival = yyvsp[0].ival; }
    break;

  case 456:
#line 3023 "gram.y"
    { yyval.ival = - yyvsp[0].ival; }
    break;

  case 457:
#line 3025 "gram.y"
    { yyval.ival = 0; }
    break;

  case 458:
#line 3026 "gram.y"
    { yyval.ival = 1; }
    break;

  case 459:
#line 3027 "gram.y"
    { yyval.ival = -1; }
    break;

  case 460:
#line 3030 "gram.y"
    {}
    break;

  case 461:
#line 3031 "gram.y"
    {}
    break;

  case 462:
#line 3043 "gram.y"
    {
					GrantStmt *n = makeNode(GrantStmt);
					n->is_grant = true;
					n->privileges = yyvsp[-5].list;
					n->objtype = (yyvsp[-3].privtarget)->objtype;
					n->objects = (yyvsp[-3].privtarget)->objs;
					n->grantees = yyvsp[-1].list;
					yyval.node = (Node*)n;
				}
    break;

  case 463:
#line 3056 "gram.y"
    {
					GrantStmt *n = makeNode(GrantStmt);
					n->is_grant = false;
					n->privileges = yyvsp[-4].list;
					n->objtype = (yyvsp[-2].privtarget)->objtype;
					n->objects = (yyvsp[-2].privtarget)->objs;
					n->grantees = yyvsp[0].list;
					yyval.node = (Node *)n;
				}
    break;

  case 464:
#line 3069 "gram.y"
    { yyval.list = yyvsp[0].list; }
    break;

  case 465:
#line 3070 "gram.y"
    { yyval.list = makeListi1(ACL_ALL_RIGHTS); }
    break;

  case 466:
#line 3071 "gram.y"
    { yyval.list = makeListi1(ACL_ALL_RIGHTS); }
    break;

  case 467:
#line 3075 "gram.y"
    { yyval.list = makeListi1(yyvsp[0].ival); }
    break;

  case 468:
#line 3076 "gram.y"
    { yyval.list = lappendi(yyvsp[-2].list, yyvsp[0].ival); }
    break;

  case 469:
#line 3082 "gram.y"
    { yyval.ival = ACL_SELECT; }
    break;

  case 470:
#line 3083 "gram.y"
    { yyval.ival = ACL_INSERT; }
    break;

  case 471:
#line 3084 "gram.y"
    { yyval.ival = ACL_UPDATE; }
    break;

  case 472:
#line 3085 "gram.y"
    { yyval.ival = ACL_DELETE; }
    break;

  case 473:
#line 3086 "gram.y"
    { yyval.ival = ACL_RULE; }
    break;

  case 474:
#line 3087 "gram.y"
    { yyval.ival = ACL_REFERENCES; }
    break;

  case 475:
#line 3088 "gram.y"
    { yyval.ival = ACL_TRIGGER; }
    break;

  case 476:
#line 3089 "gram.y"
    { yyval.ival = ACL_EXECUTE; }
    break;

  case 477:
#line 3090 "gram.y"
    { yyval.ival = ACL_USAGE; }
    break;

  case 478:
#line 3091 "gram.y"
    { yyval.ival = ACL_CREATE; }
    break;

  case 479:
#line 3092 "gram.y"
    { yyval.ival = ACL_CREATE_TEMP; }
    break;

  case 480:
#line 3093 "gram.y"
    { yyval.ival = ACL_CREATE_TEMP; }
    break;

  case 481:
#line 3101 "gram.y"
    {
					PrivTarget *n = makeNode(PrivTarget);
					n->objtype = ACL_OBJECT_RELATION;
					n->objs = yyvsp[0].list;
					yyval.privtarget = n;
				}
    break;

  case 482:
#line 3108 "gram.y"
    {
					PrivTarget *n = makeNode(PrivTarget);
					n->objtype = ACL_OBJECT_RELATION;
					n->objs = yyvsp[0].list;
					yyval.privtarget = n;
				}
    break;

  case 483:
#line 3115 "gram.y"
    {
					PrivTarget *n = makeNode(PrivTarget);
					n->objtype = ACL_OBJECT_FUNCTION;
					n->objs = yyvsp[0].list;
					yyval.privtarget = n;
				}
    break;

  case 484:
#line 3122 "gram.y"
    {
					PrivTarget *n = makeNode(PrivTarget);
					n->objtype = ACL_OBJECT_DATABASE;
					n->objs = yyvsp[0].list;
					yyval.privtarget = n;
				}
    break;

  case 485:
#line 3129 "gram.y"
    {
					PrivTarget *n = makeNode(PrivTarget);
					n->objtype = ACL_OBJECT_LANGUAGE;
					n->objs = yyvsp[0].list;
					yyval.privtarget = n;
				}
    break;

  case 486:
#line 3136 "gram.y"
    {
					PrivTarget *n = makeNode(PrivTarget);
					n->objtype = ACL_OBJECT_NAMESPACE;
					n->objs = yyvsp[0].list;
					yyval.privtarget = n;
				}
    break;

  case 487:
#line 3146 "gram.y"
    { yyval.list = makeList1(yyvsp[0].node); }
    break;

  case 488:
#line 3147 "gram.y"
    { yyval.list = lappend(yyvsp[-2].list, yyvsp[0].node); }
    break;

  case 489:
#line 3151 "gram.y"
    {
					PrivGrantee *n = makeNode(PrivGrantee);
					/* This hack lets us avoid reserving PUBLIC as a keyword*/
					if (strcmp(yyvsp[0].str, "public") == 0)
						n->username = NULL;
					else
						n->username = yyvsp[0].str;
					n->groupname = NULL;
					yyval.node = (Node *)n;
				}
    break;

  case 490:
#line 3162 "gram.y"
    {
					PrivGrantee *n = makeNode(PrivGrantee);
					/* Treat GROUP PUBLIC as a synonym for PUBLIC */
					if (strcmp(yyvsp[0].str, "public") == 0)
						n->groupname = NULL;
					else
						n->groupname = yyvsp[0].str;
					n->username = NULL;
					yyval.node = (Node *)n;
				}
    break;

  case 491:
#line 3177 "gram.y"
    {
					elog(ERROR, "grant options are not implemented");
				}
    break;

  case 493:
#line 3185 "gram.y"
    {
					elog(ERROR, "grant options are not implemented");
				}
    break;

  case 495:
#line 3193 "gram.y"
    { yyval.list = makeList1(yyvsp[0].node); }
    break;

  case 496:
#line 3195 "gram.y"
    { yyval.list = lappend(yyvsp[-2].list, yyvsp[0].node); }
    break;

  case 497:
#line 3200 "gram.y"
    {
					FuncWithArgs *n = makeNode(FuncWithArgs);
					n->funcname = yyvsp[-1].list;
					n->funcargs = yyvsp[0].list;
					yyval.node = (Node *)n;
				}
    break;

  case 498:
#line 3220 "gram.y"
    {
					IndexStmt *n = makeNode(IndexStmt);
					n->unique = yyvsp[-9].boolean;
					n->idxname = yyvsp[-7].str;
					n->relation = yyvsp[-5].range;
					n->accessMethod = yyvsp[-4].str;
					n->indexParams = yyvsp[-2].list;
					n->whereClause = yyvsp[0].node;
					yyval.node = (Node *)n;
				}
    break;

  case 499:
#line 3233 "gram.y"
    { yyval.boolean = TRUE; }
    break;

  case 500:
#line 3234 "gram.y"
    { yyval.boolean = FALSE; }
    break;

  case 501:
#line 3238 "gram.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 502:
#line 3240 "gram.y"
    { yyval.str = DEFAULT_INDEX_TYPE; }
    break;

  case 503:
#line 3244 "gram.y"
    { yyval.list = yyvsp[0].list; }
    break;

  case 504:
#line 3245 "gram.y"
    { yyval.list = makeList1(yyvsp[0].ielem); }
    break;

  case 505:
#line 3248 "gram.y"
    { yyval.list = makeList1(yyvsp[0].ielem); }
    break;

  case 506:
#line 3249 "gram.y"
    { yyval.list = lappend(yyvsp[-2].list, yyvsp[0].ielem); }
    break;

  case 507:
#line 3253 "gram.y"
    {
					yyval.ielem = makeNode(IndexElem);
					yyval.ielem->name = NULL;
					yyval.ielem->funcname = yyvsp[-4].list;
					yyval.ielem->args = yyvsp[-2].list;
					yyval.ielem->opclass = yyvsp[0].list;
				}
    break;

  case 508:
#line 3263 "gram.y"
    {
					yyval.ielem = makeNode(IndexElem);
					yyval.ielem->name = yyvsp[-1].str;
					yyval.ielem->funcname = NIL;
					yyval.ielem->args = NIL;
					yyval.ielem->opclass = yyvsp[0].list;
				}
    break;

  case 509:
#line 3273 "gram.y"
    {
					/*
					 * Release 7.0 removed network_ops, timespan_ops, and
					 * datetime_ops, so we suppress it from being passed to
					 * the parser so the default *_ops is used.  This can be
					 * removed in some later release.  bjm 2000/02/07
					 *
					 * Release 7.1 removes lztext_ops, so suppress that too
					 * for a while.  tgl 2000/07/30
					 *
					 * Release 7.2 renames timestamp_ops to timestamptz_ops,
					 * so suppress that too for awhile.  I'm starting to
					 * think we need a better approach.  tgl 2000/10/01
					 */
					if (length(yyvsp[0].list) == 1)
					{
						char   *claname = strVal(lfirst(yyvsp[0].list));

						if (strcmp(claname, "network_ops") != 0 &&
							strcmp(claname, "timespan_ops") != 0 &&
							strcmp(claname, "datetime_ops") != 0 &&
							strcmp(claname, "lztext_ops") != 0 &&
							strcmp(claname, "timestamp_ops") != 0)
							yyval.list = yyvsp[0].list;
						else
							yyval.list = NIL;
					}
					else
						yyval.list = yyvsp[0].list;
				}
    break;

  case 510:
#line 3303 "gram.y"
    { yyval.list = yyvsp[0].list; }
    break;

  case 511:
#line 3304 "gram.y"
    { yyval.list = NIL; }
    break;

  case 512:
#line 3321 "gram.y"
    {
					CreateFunctionStmt *n = makeNode(CreateFunctionStmt);
					n->replace = yyvsp[-7].boolean;
					n->funcname = yyvsp[-5].list;
					n->argTypes = yyvsp[-4].list;
					n->returnType = yyvsp[-2].typnam;
					n->options = yyvsp[-1].list;
					n->withClause = yyvsp[0].list;
					yyval.node = (Node *)n;
				}
    break;

  case 513:
#line 3334 "gram.y"
    { yyval.boolean = TRUE; }
    break;

  case 514:
#line 3335 "gram.y"
    { yyval.boolean = FALSE; }
    break;

  case 515:
#line 3338 "gram.y"
    { yyval.list = yyvsp[-1].list; }
    break;

  case 516:
#line 3339 "gram.y"
    { yyval.list = NIL; }
    break;

  case 517:
#line 3343 "gram.y"
    { yyval.list = makeList1(yyvsp[0].typnam); }
    break;

  case 518:
#line 3344 "gram.y"
    { yyval.list = lappend(yyvsp[-2].list, yyvsp[0].typnam); }
    break;

  case 519:
#line 3348 "gram.y"
    {
					/* We can catch over-specified arguments here if we want to,
					 * but for now better to silently swallow typmod, etc.
					 * - thomas 2000-03-22
					 */
					yyval.typnam = yyvsp[0].typnam;
				}
    break;

  case 520:
#line 3355 "gram.y"
    { yyval.typnam = yyvsp[0].typnam; }
    break;

  case 521:
#line 3358 "gram.y"
    { yyval.boolean = FALSE; }
    break;

  case 522:
#line 3360 "gram.y"
    {
					elog(ERROR,
					"CREATE FUNCTION / OUT parameters are not supported");
					yyval.boolean = TRUE;
				}
    break;

  case 523:
#line 3366 "gram.y"
    {
					elog(ERROR,
					"CREATE FUNCTION / INOUT parameters are not supported");
					yyval.boolean = FALSE;
				}
    break;

  case 524:
#line 3375 "gram.y"
    {
					/* We can catch over-specified arguments here if we want to,
					 * but for now better to silently swallow typmod, etc.
					 * - thomas 2000-03-22
					 */
					yyval.typnam = yyvsp[0].typnam;
				}
    break;

  case 525:
#line 3388 "gram.y"
    { yyval.typnam = yyvsp[0].typnam; }
    break;

  case 526:
#line 3390 "gram.y"
    {
					yyval.typnam = makeNode(TypeName);
					yyval.typnam->names = lcons(makeString(yyvsp[-3].str), yyvsp[-2].list);
					yyval.typnam->pct_type = true;
					yyval.typnam->typmod = -1;
				}
    break;

  case 527:
#line 3401 "gram.y"
    { yyval.list = makeList1(yyvsp[0].defelt); }
    break;

  case 528:
#line 3402 "gram.y"
    { yyval.list = lappend(yyvsp[-1].list, yyvsp[0].defelt); }
    break;

  case 529:
#line 3407 "gram.y"
    {
					yyval.defelt = makeDefElem("as", (Node *)yyvsp[0].list);
				}
    break;

  case 530:
#line 3411 "gram.y"
    {
					yyval.defelt = makeDefElem("language", (Node *)makeString(yyvsp[0].str));
				}
    break;

  case 531:
#line 3415 "gram.y"
    {
					yyval.defelt = makeDefElem("volatility", (Node *)makeString("immutable"));
				}
    break;

  case 532:
#line 3419 "gram.y"
    {
					yyval.defelt = makeDefElem("volatility", (Node *)makeString("stable"));
				}
    break;

  case 533:
#line 3423 "gram.y"
    {
					yyval.defelt = makeDefElem("volatility", (Node *)makeString("volatile"));
				}
    break;

  case 534:
#line 3427 "gram.y"
    {
					yyval.defelt = makeDefElem("strict", (Node *)makeInteger(FALSE));
				}
    break;

  case 535:
#line 3431 "gram.y"
    {
					yyval.defelt = makeDefElem("strict", (Node *)makeInteger(TRUE));
				}
    break;

  case 536:
#line 3435 "gram.y"
    {
					yyval.defelt = makeDefElem("strict", (Node *)makeInteger(TRUE));
				}
    break;

  case 537:
#line 3439 "gram.y"
    {
					yyval.defelt = makeDefElem("security", (Node *)makeInteger(TRUE));
				}
    break;

  case 538:
#line 3443 "gram.y"
    {
					yyval.defelt = makeDefElem("security", (Node *)makeInteger(FALSE));
				}
    break;

  case 539:
#line 3447 "gram.y"
    {
					yyval.defelt = makeDefElem("security", (Node *)makeInteger(TRUE));
				}
    break;

  case 540:
#line 3451 "gram.y"
    {
					yyval.defelt = makeDefElem("security", (Node *)makeInteger(FALSE));
				}
    break;

  case 541:
#line 3456 "gram.y"
    { yyval.list = makeList1(makeString(yyvsp[0].str)); }
    break;

  case 542:
#line 3458 "gram.y"
    {
					yyval.list = makeList2(makeString(yyvsp[-2].str), makeString(yyvsp[0].str));
				}
    break;

  case 543:
#line 3464 "gram.y"
    { yyval.list = yyvsp[0].list; }
    break;

  case 544:
#line 3465 "gram.y"
    { yyval.list = NIL; }
    break;

  case 545:
#line 3481 "gram.y"
    {
					RemoveFuncStmt *n = makeNode(RemoveFuncStmt);
					n->funcname = yyvsp[-2].list;
					n->args = yyvsp[-1].list;
					n->behavior = yyvsp[0].dbehavior;
					yyval.node = (Node *)n;
				}
    break;

  case 546:
#line 3492 "gram.y"
    {
						RemoveAggrStmt *n = makeNode(RemoveAggrStmt);
						n->aggname = yyvsp[-4].list;
						n->aggtype = yyvsp[-2].typnam;
						n->behavior = yyvsp[0].dbehavior;
						yyval.node = (Node *)n;
				}
    break;

  case 547:
#line 3502 "gram.y"
    { yyval.typnam = yyvsp[0].typnam; }
    break;

  case 548:
#line 3503 "gram.y"
    { yyval.typnam = NULL; }
    break;

  case 549:
#line 3508 "gram.y"
    {
					RemoveOperStmt *n = makeNode(RemoveOperStmt);
					n->opname = yyvsp[-4].list;
					n->args = yyvsp[-2].list;
					n->behavior = yyvsp[0].dbehavior;
					yyval.node = (Node *)n;
				}
    break;

  case 550:
#line 3519 "gram.y"
    {
				   elog(ERROR,"parser: argument type missing (use NONE for unary operators)");
				}
    break;

  case 551:
#line 3523 "gram.y"
    { yyval.list = makeList2(yyvsp[-2].typnam, yyvsp[0].typnam); }
    break;

  case 552:
#line 3525 "gram.y"
    { yyval.list = makeList2(NULL, yyvsp[0].typnam); }
    break;

  case 553:
#line 3527 "gram.y"
    { yyval.list = makeList2(yyvsp[-2].typnam, NULL); }
    break;

  case 554:
#line 3532 "gram.y"
    { yyval.list = makeList1(makeString(yyvsp[0].str)); }
    break;

  case 555:
#line 3534 "gram.y"
    { yyval.list = lcons(makeString(yyvsp[-2].str), yyvsp[0].list); }
    break;

  case 556:
#line 3546 "gram.y"
    {
					CreateCastStmt *n = makeNode(CreateCastStmt);
					n->sourcetype = yyvsp[-7].typnam;
					n->targettype = yyvsp[-5].typnam;
					n->func = (FuncWithArgs *) yyvsp[-1].node;
					n->context = (CoercionContext) yyvsp[0].ival;
					yyval.node = (Node *)n;
				}
    break;

  case 557:
#line 3556 "gram.y"
    {
					CreateCastStmt *n = makeNode(CreateCastStmt);
					n->sourcetype = yyvsp[-6].typnam;
					n->targettype = yyvsp[-4].typnam;
					n->func = NULL;
					n->context = (CoercionContext) yyvsp[0].ival;
					yyval.node = (Node *)n;
				}
    break;

  case 558:
#line 3566 "gram.y"
    { yyval.ival = COERCION_IMPLICIT; }
    break;

  case 559:
#line 3567 "gram.y"
    { yyval.ival = COERCION_ASSIGNMENT; }
    break;

  case 560:
#line 3568 "gram.y"
    { yyval.ival = COERCION_EXPLICIT; }
    break;

  case 561:
#line 3573 "gram.y"
    {
					DropCastStmt *n = makeNode(DropCastStmt);
					n->sourcetype = yyvsp[-4].typnam;
					n->targettype = yyvsp[-2].typnam;
					n->behavior = yyvsp[0].dbehavior;
					yyval.node = (Node *)n;
				}
    break;

  case 562:
#line 3594 "gram.y"
    {
					ReindexStmt *n = makeNode(ReindexStmt);
					n->reindexType = yyvsp[-2].ival;
					n->relation = yyvsp[-1].range;
					n->name = NULL;
					n->force = yyvsp[0].boolean;
					yyval.node = (Node *)n;
				}
    break;

  case 563:
#line 3603 "gram.y"
    {
					ReindexStmt *n = makeNode(ReindexStmt);
					n->reindexType = DATABASE;
					n->name = yyvsp[-1].str;
					n->relation = NULL;
					n->force = yyvsp[0].boolean;
					yyval.node = (Node *)n;
				}
    break;

  case 564:
#line 3614 "gram.y"
    { yyval.ival = INDEX; }
    break;

  case 565:
#line 3615 "gram.y"
    { yyval.ival = TABLE; }
    break;

  case 566:
#line 3618 "gram.y"
    {  yyval.boolean = TRUE; }
    break;

  case 567:
#line 3619 "gram.y"
    {  yyval.boolean = FALSE; }
    break;

  case 568:
#line 3632 "gram.y"
    {
					RenameStmt *n = makeNode(RenameStmt);
					n->relation = yyvsp[-5].range;
					n->oldname = yyvsp[-2].str;
					n->newname = yyvsp[0].str;
					if (yyvsp[-2].str == NULL)
						n->renameType = RENAME_TABLE;
					else
						n->renameType = RENAME_COLUMN;
					yyval.node = (Node *)n;
				}
    break;

  case 569:
#line 3644 "gram.y"
    {
					RenameStmt *n = makeNode(RenameStmt);
					n->relation = yyvsp[-3].range;
					n->oldname = yyvsp[-5].str;
					n->newname = yyvsp[0].str;
					n->renameType = RENAME_TRIGGER;
					yyval.node = (Node *)n;
				}
    break;

  case 570:
#line 3654 "gram.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 571:
#line 3655 "gram.y"
    { yyval.str = NULL; }
    break;

  case 572:
#line 3658 "gram.y"
    { yyval.ival = COLUMN; }
    break;

  case 573:
#line 3659 "gram.y"
    { yyval.ival = 0; }
    break;

  case 574:
#line 3670 "gram.y"
    { QueryIsRule=TRUE; }
    break;

  case 575:
#line 3673 "gram.y"
    {
					RuleStmt *n = makeNode(RuleStmt);
					n->replace = yyvsp[-12].boolean;
					n->relation = yyvsp[-4].range;
					n->rulename = yyvsp[-10].str;
					n->whereClause = yyvsp[-3].node;
					n->event = yyvsp[-6].ival;
					n->instead = yyvsp[-1].boolean;
					n->actions = yyvsp[0].list;
					yyval.node = (Node *)n;
					QueryIsRule=FALSE;
				}
    break;

  case 576:
#line 3688 "gram.y"
    { yyval.list = NIL; }
    break;

  case 577:
#line 3689 "gram.y"
    { yyval.list = makeList1(yyvsp[0].node); }
    break;

  case 578:
#line 3690 "gram.y"
    { yyval.list = yyvsp[-1].list; }
    break;

  case 579:
#line 3696 "gram.y"
    { if (yyvsp[0].node != (Node *) NULL)
					yyval.list = lappend(yyvsp[-2].list, yyvsp[0].node);
				  else
					yyval.list = yyvsp[-2].list;
				}
    break;

  case 580:
#line 3702 "gram.y"
    { if (yyvsp[0].node != (Node *) NULL)
					yyval.list = makeList1(yyvsp[0].node);
				  else
					yyval.list = NIL;
				}
    break;

  case 586:
#line 3718 "gram.y"
    { yyval.node = yyvsp[0].node; }
    break;

  case 587:
#line 3719 "gram.y"
    { yyval.node = (Node *)NULL; }
    break;

  case 588:
#line 3723 "gram.y"
    { yyval.ival = CMD_SELECT; }
    break;

  case 589:
#line 3724 "gram.y"
    { yyval.ival = CMD_UPDATE; }
    break;

  case 590:
#line 3725 "gram.y"
    { yyval.ival = CMD_DELETE; }
    break;

  case 591:
#line 3726 "gram.y"
    { yyval.ival = CMD_INSERT; }
    break;

  case 592:
#line 3730 "gram.y"
    { yyval.boolean = TRUE; }
    break;

  case 593:
#line 3731 "gram.y"
    { yyval.boolean = FALSE; }
    break;

  case 594:
#line 3737 "gram.y"
    {
					DropPropertyStmt *n = makeNode(DropPropertyStmt);
					n->relation = yyvsp[-1].range;
					n->property = yyvsp[-3].str;
					n->behavior = yyvsp[0].dbehavior;
					n->removeType = DROP_RULE;
					yyval.node = (Node *) n;
				}
    break;

  case 595:
#line 3757 "gram.y"
    {
					NotifyStmt *n = makeNode(NotifyStmt);
					n->relation = yyvsp[0].range;
					yyval.node = (Node *)n;
				}
    break;

  case 596:
#line 3765 "gram.y"
    {
					ListenStmt *n = makeNode(ListenStmt);
					n->relation = yyvsp[0].range;
					yyval.node = (Node *)n;
				}
    break;

  case 597:
#line 3774 "gram.y"
    {
					UnlistenStmt *n = makeNode(UnlistenStmt);
					n->relation = yyvsp[0].range;
					yyval.node = (Node *)n;
				}
    break;

  case 598:
#line 3780 "gram.y"
    {
					UnlistenStmt *n = makeNode(UnlistenStmt);
					n->relation = makeNode(RangeVar);
					n->relation->relname = "*";
					n->relation->schemaname = NULL;
					yyval.node = (Node *)n;
				}
    break;

  case 599:
#line 3801 "gram.y"
    {
					TransactionStmt *n = makeNode(TransactionStmt);
					n->command = ROLLBACK;
					n->options = NIL;
					yyval.node = (Node *)n;
				}
    break;

  case 600:
#line 3808 "gram.y"
    {
					TransactionStmt *n = makeNode(TransactionStmt);
					n->command = BEGIN_TRANS;
					n->options = NIL;
					yyval.node = (Node *)n;
				}
    break;

  case 601:
#line 3815 "gram.y"
    {
					TransactionStmt *n = makeNode(TransactionStmt);
					n->command = START;
					n->options = yyvsp[0].list;
					yyval.node = (Node *)n;
				}
    break;

  case 602:
#line 3822 "gram.y"
    {
					TransactionStmt *n = makeNode(TransactionStmt);
					n->command = COMMIT;
					n->options = NIL;
					yyval.node = (Node *)n;
				}
    break;

  case 603:
#line 3829 "gram.y"
    {
					TransactionStmt *n = makeNode(TransactionStmt);
					n->command = COMMIT;
					n->options = NIL;
					yyval.node = (Node *)n;
				}
    break;

  case 604:
#line 3836 "gram.y"
    {
					TransactionStmt *n = makeNode(TransactionStmt);
					n->command = ROLLBACK;
					n->options = NIL;
					yyval.node = (Node *)n;
				}
    break;

  case 605:
#line 3845 "gram.y"
    { yyval.list = makeList1(makeStringConst(yyvsp[0].str, NULL)); }
    break;

  case 606:
#line 3846 "gram.y"
    { yyval.list = NIL; }
    break;

  case 607:
#line 3849 "gram.y"
    {}
    break;

  case 608:
#line 3850 "gram.y"
    {}
    break;

  case 609:
#line 3851 "gram.y"
    {}
    break;

  case 610:
#line 3863 "gram.y"
    {
					ViewStmt *n = makeNode(ViewStmt);
					n->replace = yyvsp[-5].boolean;
					n->view = yyvsp[-3].range;
					n->aliases = yyvsp[-2].list;
					n->query = (Query *) yyvsp[0].node;
					yyval.node = (Node *)n;
				}
    break;

  case 611:
#line 3882 "gram.y"
    {
					LoadStmt *n = makeNode(LoadStmt);
					n->filename = yyvsp[0].str;
					yyval.node = (Node *)n;
				}
    break;

  case 612:
#line 3898 "gram.y"
    {
					CreatedbStmt *n = makeNode(CreatedbStmt);
					n->dbname = yyvsp[-2].str;
					n->options = yyvsp[0].list;
					yyval.node = (Node *)n;
				}
    break;

  case 613:
#line 3907 "gram.y"
    { yyval.list = lappend(yyvsp[-1].list, yyvsp[0].defelt); }
    break;

  case 614:
#line 3908 "gram.y"
    { yyval.list = NIL; }
    break;

  case 615:
#line 3913 "gram.y"
    {
					yyval.defelt = makeDefElem("location", (Node *)makeString(yyvsp[0].str));
				}
    break;

  case 616:
#line 3917 "gram.y"
    {
					yyval.defelt = makeDefElem("location", NULL);
				}
    break;

  case 617:
#line 3921 "gram.y"
    {
					yyval.defelt = makeDefElem("template", (Node *)makeString(yyvsp[0].str));
				}
    break;

  case 618:
#line 3925 "gram.y"
    {
					yyval.defelt = makeDefElem("template", NULL);
				}
    break;

  case 619:
#line 3929 "gram.y"
    {
					yyval.defelt = makeDefElem("encoding", (Node *)makeString(yyvsp[0].str));
				}
    break;

  case 620:
#line 3933 "gram.y"
    {
					yyval.defelt = makeDefElem("encoding", (Node *)makeInteger(yyvsp[0].ival));
				}
    break;

  case 621:
#line 3937 "gram.y"
    {
					yyval.defelt = makeDefElem("encoding", NULL);
				}
    break;

  case 622:
#line 3941 "gram.y"
    {
					yyval.defelt = makeDefElem("owner", (Node *)makeString(yyvsp[0].str));
				}
    break;

  case 623:
#line 3945 "gram.y"
    {
					yyval.defelt = makeDefElem("owner", NULL);
				}
    break;

  case 624:
#line 3955 "gram.y"
    {}
    break;

  case 625:
#line 3956 "gram.y"
    {}
    break;

  case 626:
#line 3968 "gram.y"
    {
					AlterDatabaseSetStmt *n = makeNode(AlterDatabaseSetStmt);
					n->dbname = yyvsp[-2].str;
					n->variable = yyvsp[0].vsetstmt->name;
					n->value = yyvsp[0].vsetstmt->args;
					yyval.node = (Node *)n;
				}
    break;

  case 627:
#line 3976 "gram.y"
    {
					AlterDatabaseSetStmt *n = makeNode(AlterDatabaseSetStmt);
					n->dbname = yyvsp[-1].str;
					n->variable = ((VariableResetStmt *)yyvsp[0].node)->name;
					n->value = NIL;
					yyval.node = (Node *)n;
				}
    break;

  case 628:
#line 3994 "gram.y"
    {
					DropdbStmt *n = makeNode(DropdbStmt);
					n->dbname = yyvsp[0].str;
					yyval.node = (Node *)n;
				}
    break;

  case 629:
#line 4010 "gram.y"
    {
					CreateDomainStmt *n = makeNode(CreateDomainStmt);
					n->domainname = yyvsp[-4].list;
					n->typename = yyvsp[-2].typnam;
					n->constraints = yyvsp[-1].list;

					if (yyvsp[0].str != NULL)
						elog(NOTICE,"CREATE DOMAIN / COLLATE %s not yet "
							"implemented; clause ignored", yyvsp[0].str);
					yyval.node = (Node *)n;
				}
    break;

  case 630:
#line 4023 "gram.y"
    {}
    break;

  case 631:
#line 4024 "gram.y"
    {}
    break;

  case 632:
#line 4040 "gram.y"
    {
			  CreateConversionStmt *n = makeNode(CreateConversionStmt);
			  n->conversion_name = yyvsp[-6].list;
			  n->for_encoding_name = yyvsp[-4].str;
			  n->to_encoding_name = yyvsp[-2].str;
			  n->func_name = yyvsp[0].list;
			  n->def = yyvsp[-8].boolean;
			  yyval.node = (Node *)n;
			}
    break;

  case 633:
#line 4060 "gram.y"
    {
				   ClusterStmt *n = makeNode(ClusterStmt);
				   n->relation = yyvsp[0].range;
				   n->indexname = yyvsp[-2].str;
				   yyval.node = (Node*)n;
				}
    break;

  case 634:
#line 4077 "gram.y"
    {
					VacuumStmt *n = makeNode(VacuumStmt);
					n->vacuum = true;
					n->analyze = false;
					n->full = yyvsp[-2].boolean;
					n->freeze = yyvsp[-1].boolean;
					n->verbose = yyvsp[0].boolean;
					n->relation = NULL;
					n->va_cols = NIL;
					yyval.node = (Node *)n;
				}
    break;

  case 635:
#line 4089 "gram.y"
    {
					VacuumStmt *n = makeNode(VacuumStmt);
					n->vacuum = true;
					n->analyze = false;
					n->full = yyvsp[-3].boolean;
					n->freeze = yyvsp[-2].boolean;
					n->verbose = yyvsp[-1].boolean;
					n->relation = yyvsp[0].range;
					n->va_cols = NIL;
					yyval.node = (Node *)n;
				}
    break;

  case 636:
#line 4101 "gram.y"
    {
					VacuumStmt *n = (VacuumStmt *) yyvsp[0].node;
					n->vacuum = true;
					n->full = yyvsp[-3].boolean;
					n->freeze = yyvsp[-2].boolean;
					n->verbose |= yyvsp[-1].boolean;
					yyval.node = (Node *)n;
				}
    break;

  case 637:
#line 4113 "gram.y"
    {
					VacuumStmt *n = makeNode(VacuumStmt);
					n->vacuum = false;
					n->analyze = true;
					n->full = false;
					n->freeze = false;
					n->verbose = yyvsp[0].boolean;
					n->relation = NULL;
					n->va_cols = NIL;
					yyval.node = (Node *)n;
				}
    break;

  case 638:
#line 4125 "gram.y"
    {
					VacuumStmt *n = makeNode(VacuumStmt);
					n->vacuum = false;
					n->analyze = true;
					n->full = false;
					n->freeze = false;
					n->verbose = yyvsp[-2].boolean;
					n->relation = yyvsp[-1].range;
					n->va_cols = yyvsp[0].list;
					yyval.node = (Node *)n;
				}
    break;

  case 639:
#line 4139 "gram.y"
    {}
    break;

  case 640:
#line 4140 "gram.y"
    {}
    break;

  case 641:
#line 4144 "gram.y"
    {}
    break;

  case 642:
#line 4148 "gram.y"
    { yyval.boolean = TRUE; }
    break;

  case 643:
#line 4149 "gram.y"
    { yyval.boolean = FALSE; }
    break;

  case 644:
#line 4152 "gram.y"
    { yyval.boolean = TRUE; }
    break;

  case 645:
#line 4153 "gram.y"
    { yyval.boolean = FALSE; }
    break;

  case 646:
#line 4156 "gram.y"
    { yyval.boolean = TRUE; }
    break;

  case 647:
#line 4157 "gram.y"
    { yyval.boolean = FALSE; }
    break;

  case 648:
#line 4161 "gram.y"
    { yyval.list = yyvsp[-1].list; }
    break;

  case 649:
#line 4162 "gram.y"
    { yyval.list = NIL; }
    break;

  case 650:
#line 4177 "gram.y"
    {
					ExplainStmt *n = makeNode(ExplainStmt);
					n->verbose = yyvsp[-1].boolean;
					n->analyze = FALSE;
					n->query = (Query*)yyvsp[0].node;
					yyval.node = (Node *)n;
				}
    break;

  case 651:
#line 4185 "gram.y"
    {
					ExplainStmt *n = makeNode(ExplainStmt);
					n->verbose = yyvsp[-1].boolean;
					n->analyze = TRUE;
					n->query = (Query*)yyvsp[0].node;
					yyval.node = (Node *)n;
				}
    break;

  case 652:
#line 4193 "gram.y"
    {
					ExplainStmt *n = makeNode(ExplainStmt);
					n->verbose = FALSE;
					n->analyze = FALSE;
					n->query = NULL;
					n->cqeddy = TRUE;
					yyval.node = (Node *)n;
				}
    break;

  case 653:
#line 4212 "gram.y"
    {
					PrepareStmt *n = makeNode(PrepareStmt);
					n->name = yyvsp[-3].str;
					n->argtypes = yyvsp[-2].list;
					n->query = (Query *) yyvsp[0].node;
					yyval.node = (Node *) n;
				}
    break;

  case 654:
#line 4221 "gram.y"
    { yyval.list = yyvsp[-1].list; }
    break;

  case 655:
#line 4222 "gram.y"
    { yyval.list = NIL; }
    break;

  case 656:
#line 4225 "gram.y"
    { yyval.list = makeList1(yyvsp[0].typnam); }
    break;

  case 657:
#line 4227 "gram.y"
    { yyval.list = lappend(yyvsp[-2].list, yyvsp[0].typnam); }
    break;

  case 658:
#line 4238 "gram.y"
    {
					ExecuteStmt *n = makeNode(ExecuteStmt);
					n->name = yyvsp[-2].str;
					n->params = yyvsp[-1].list;
					n->into = yyvsp[0].range;
					yyval.node = (Node *) n;
				}
    break;

  case 659:
#line 4247 "gram.y"
    { yyval.list = yyvsp[-1].list; }
    break;

  case 660:
#line 4248 "gram.y"
    { yyval.list = NIL; }
    break;

  case 661:
#line 4259 "gram.y"
    {
						DeallocateStmt *n = makeNode(DeallocateStmt);
						n->name = yyvsp[0].str;
						yyval.node = (Node *) n;
					}
    break;

  case 662:
#line 4265 "gram.y"
    {
						DeallocateStmt *n = makeNode(DeallocateStmt);
						n->name = yyvsp[0].str;
						yyval.node = (Node *) n;
					}
    break;

  case 668:
#line 4301 "gram.y"
    {
					yyvsp[0].istmt->relation = yyvsp[-1].range;
					yyval.node = (Node *) yyvsp[0].istmt;
				}
    break;

  case 669:
#line 4309 "gram.y"
    {
					yyval.istmt = makeNode(InsertStmt);
					yyval.istmt->cols = NIL;
					yyval.istmt->targetList = yyvsp[-1].list;
					yyval.istmt->selectStmt = NULL;
				}
    break;

  case 670:
#line 4316 "gram.y"
    {
					yyval.istmt = makeNode(InsertStmt);
					yyval.istmt->cols = NIL;
					yyval.istmt->targetList = NIL;
					yyval.istmt->selectStmt = NULL;
				}
    break;

  case 671:
#line 4323 "gram.y"
    {
					yyval.istmt = makeNode(InsertStmt);
					yyval.istmt->cols = NIL;
					yyval.istmt->targetList = NIL;
					yyval.istmt->selectStmt = yyvsp[0].node;
				}
    break;

  case 672:
#line 4330 "gram.y"
    {
					yyval.istmt = makeNode(InsertStmt);
					yyval.istmt->cols = yyvsp[-5].list;
					yyval.istmt->targetList = yyvsp[-1].list;
					yyval.istmt->selectStmt = NULL;
				}
    break;

  case 673:
#line 4337 "gram.y"
    {
					yyval.istmt = makeNode(InsertStmt);
					yyval.istmt->cols = yyvsp[-2].list;
					yyval.istmt->targetList = NIL;
					yyval.istmt->selectStmt = yyvsp[0].node;
				}
    break;

  case 674:
#line 4346 "gram.y"
    { yyval.list = makeList1(yyvsp[0].node); }
    break;

  case 675:
#line 4348 "gram.y"
    { yyval.list = lappend(yyvsp[-2].list, yyvsp[0].node); }
    break;

  case 676:
#line 4353 "gram.y"
    {
					ResTarget *n = makeNode(ResTarget);
					n->name = yyvsp[-1].str;
					n->indirection = yyvsp[0].list;
					n->val = NULL;
					yyval.node = (Node *)n;
				}
    break;

  case 677:
#line 4371 "gram.y"
    {
					DeleteStmt *n = makeNode(DeleteStmt);
					n->relation = yyvsp[-1].range;
					n->whereClause = yyvsp[0].node;
					yyval.node = (Node *)n;
				}
    break;

  case 678:
#line 4380 "gram.y"
    {
					LockStmt *n = makeNode(LockStmt);

					n->relations = yyvsp[-1].list;
					n->mode = yyvsp[0].ival;
					yyval.node = (Node *)n;
				}
    break;

  case 679:
#line 4389 "gram.y"
    { yyval.ival = yyvsp[-1].ival; }
    break;

  case 680:
#line 4390 "gram.y"
    { yyval.ival = AccessExclusiveLock; }
    break;

  case 681:
#line 4393 "gram.y"
    { yyval.ival = AccessShareLock; }
    break;

  case 682:
#line 4394 "gram.y"
    { yyval.ival = RowShareLock; }
    break;

  case 683:
#line 4395 "gram.y"
    { yyval.ival = RowExclusiveLock; }
    break;

  case 684:
#line 4396 "gram.y"
    { yyval.ival = ShareUpdateExclusiveLock; }
    break;

  case 685:
#line 4397 "gram.y"
    { yyval.ival = ShareLock; }
    break;

  case 686:
#line 4398 "gram.y"
    { yyval.ival = ShareRowExclusiveLock; }
    break;

  case 687:
#line 4399 "gram.y"
    { yyval.ival = ExclusiveLock; }
    break;

  case 688:
#line 4400 "gram.y"
    { yyval.ival = AccessExclusiveLock; }
    break;

  case 689:
#line 4415 "gram.y"
    {
					UpdateStmt *n = makeNode(UpdateStmt);
					n->relation = yyvsp[-4].range;
					n->targetList = yyvsp[-2].list;
					n->fromClause = yyvsp[-1].list;
					n->whereClause = yyvsp[0].node;
					yyval.node = (Node *)n;
				}
    break;

  case 690:
#line 4433 "gram.y"
    {
					SelectStmt *n = (SelectStmt *)yyvsp[0].node;
					n->portalname = yyvsp[-4].str;
					n->binary = yyvsp[-3].boolean;
					n->cursorType = CURSORKIND_UNKNOWN;
					yyval.node = yyvsp[0].node;
				}
    break;

  case 691:
#line 4441 "gram.y"
    {				 
				  SelectStmt *n = (SelectStmt *)yyvsp[0].node;
				  n->portalname = yyvsp[-6].str;
				  n->cursorType = yyvsp[-3].cursortype;
				  n->binary = yyvsp[-5].boolean;
				  yyval.node = yyvsp[0].node;
				}
    break;

  case 692:
#line 4450 "gram.y"
    { yyval.cursortype = CURSORKIND_ISTREAM; }
    break;

  case 693:
#line 4451 "gram.y"
    { yyval.cursortype = CURSORKIND_RSTREAM; }
    break;

  case 694:
#line 4452 "gram.y"
    { yyval.cursortype = CURSORKIND_DSTREAM; }
    break;

  case 695:
#line 4455 "gram.y"
    { yyval.boolean = TRUE; }
    break;

  case 696:
#line 4456 "gram.y"
    { yyval.boolean = FALSE; }
    break;

  case 697:
#line 4457 "gram.y"
    { yyval.boolean = FALSE; }
    break;

  case 698:
#line 4458 "gram.y"
    { yyval.boolean = FALSE; }
    break;

  case 699:
#line 4459 "gram.y"
    { yyval.boolean = FALSE; }
    break;

  case 703:
#line 4513 "gram.y"
    { yyval.node = yyvsp[-1].node; }
    break;

  case 704:
#line 4514 "gram.y"
    { yyval.node = yyvsp[-1].node; }
    break;

  case 705:
#line 4524 "gram.y"
    { yyval.node = yyvsp[0].node; }
    break;

  case 706:
#line 4526 "gram.y"
    {
					insertSelectOptions((SelectStmt *) yyvsp[-1].node, yyvsp[0].list, NIL,
										NULL, NULL);
					yyval.node = yyvsp[-1].node;
				}
    break;

  case 707:
#line 4532 "gram.y"
    {
					insertSelectOptions((SelectStmt *) yyvsp[-3].node, yyvsp[-2].list, yyvsp[-1].list,
										nth(0, yyvsp[0].list), nth(1, yyvsp[0].list));
					yyval.node = yyvsp[-3].node;
				}
    break;

  case 708:
#line 4538 "gram.y"
    {
					insertSelectOptions((SelectStmt *) yyvsp[-3].node, yyvsp[-2].list, yyvsp[0].list,
										nth(0, yyvsp[-1].list), nth(1, yyvsp[-1].list));
					yyval.node = yyvsp[-3].node;
				}
    break;

  case 709:
#line 4546 "gram.y"
    { yyval.node = yyvsp[0].node; }
    break;

  case 710:
#line 4547 "gram.y"
    { yyval.node = yyvsp[0].node; }
    break;

  case 711:
#line 4578 "gram.y"
    {
					SelectStmt *n = makeNode(SelectStmt);
					n->distinctClause = yyvsp[-11].list;
					n->targetList = yyvsp[-10].list;
					n->into = yyvsp[-9].range;
					n->intoColNames = NIL;
					n->fromClause = yyvsp[-8].list;


					n->eventClause = yyvsp[-6].node;

/* 					if(!n->eventClause) */
/* 					  elog(NOTICE, "The query has NO event clause"); */
/* 					else */
/* 					  elog(NOTICE, "The query has an event clause"); */

					if(n->eventClause)
					  ((EventExpr *)n->eventClause)->partition = (List *)yyvsp[-7].list;
					else {
					  if(yyvsp[-7].list)
					    elog(ERROR, "The \"Partition By\" clause is currently defined only with the \"Event\" clause");
					}

					n->whereClause = yyvsp[-5].node;
					n->groupClause = yyvsp[-4].list;
					n->havingClause = yyvsp[-3].node;
					n->aggWinClause = yyvsp[-2].agg_window;
					n->total_machines = yyvsp[-1].ival;
					n->resultClause = yyvsp[0].node;
								
					yyval.node = (Node *)n;
					n->isContinuousQuery = FALSE;					
				}
    break;

  case 712:
#line 4612 "gram.y"
    {
					yyval.node = makeSetOp(SETOP_UNION, yyvsp[-1].boolean, yyvsp[-3].node, yyvsp[0].node);
				}
    break;

  case 713:
#line 4616 "gram.y"
    {
					yyval.node = makeSetOp(SETOP_INTERSECT, yyvsp[-1].boolean, yyvsp[-3].node, yyvsp[0].node);
				}
    break;

  case 714:
#line 4620 "gram.y"
    {
					yyval.node = makeSetOp(SETOP_EXCEPT, yyvsp[-1].boolean, yyvsp[-3].node, yyvsp[0].node);
				}
    break;

  case 715:
#line 4625 "gram.y"
    {
			  yyval.node = makeSetOp(SETOP_RECURSIVE_NEXT, TRUE, yyvsp[-2].node, yyvsp[0].node);
			}
    break;

  case 716:
#line 4633 "gram.y"
    {
  yyval.node = makeSetOp(SETOP_RECURSIVE_NEXT, TRUE, yyvsp[-1].node, yyvsp[0].node);
}
    break;

  case 717:
#line 4638 "gram.y"
    {
  yyval.node= makeSetOp(SETOP_RECURSIVE_NEXT, TRUE, yyvsp[0].node, yyvsp[-1].node);
}
    break;

  case 718:
#line 4642 "gram.y"
    {
  yyval.node = yyvsp[0].node;
}
    break;

  case 719:
#line 4649 "gram.y"
    {
	((SelectStmt*)yyvsp[0].node)->resultClause=(Node*)yyvsp[-2].range;
	yyval.node=yyvsp[0].node;
}
    break;

  case 720:
#line 4656 "gram.y"
    { yyval.range = yyvsp[0].range; }
    break;

  case 721:
#line 4657 "gram.y"
    { yyval.range = NULL; }
    break;

  case 722:
#line 4666 "gram.y"
    {
					yyval.range = yyvsp[0].range;
					yyval.range->istemp = true;
				}
    break;

  case 723:
#line 4671 "gram.y"
    {
					yyval.range = yyvsp[0].range;
					yyval.range->istemp = true;
				}
    break;

  case 724:
#line 4676 "gram.y"
    {
					yyval.range = yyvsp[0].range;
					yyval.range->istemp = true;
				}
    break;

  case 725:
#line 4681 "gram.y"
    {
					yyval.range = yyvsp[0].range;
					yyval.range->istemp = true;
				}
    break;

  case 726:
#line 4686 "gram.y"
    {
					elog(ERROR,
						"GLOBAL TEMPORARY TABLE is not currently supported");
					yyval.range = yyvsp[0].range;
					yyval.range->istemp = true;
				}
    break;

  case 727:
#line 4693 "gram.y"
    {
					elog(ERROR,
						"GLOBAL TEMPORARY TABLE is not currently supported");
					yyval.range = yyvsp[0].range;
					yyval.range->istemp = true;
				}
    break;

  case 728:
#line 4700 "gram.y"
    {
					yyval.range = yyvsp[0].range;
					yyval.range->istemp = false;
				}
    break;

  case 729:
#line 4705 "gram.y"
    {
					yyval.range = yyvsp[0].range;
					yyval.range->istemp = false;
				}
    break;

  case 730:
#line 4711 "gram.y"
    {}
    break;

  case 731:
#line 4712 "gram.y"
    {}
    break;

  case 732:
#line 4715 "gram.y"
    { yyval.boolean = TRUE; }
    break;

  case 733:
#line 4716 "gram.y"
    { yyval.boolean = FALSE; }
    break;

  case 734:
#line 4717 "gram.y"
    { yyval.boolean = FALSE; }
    break;

  case 735:
#line 4724 "gram.y"
    { yyval.list = makeList1(NIL); }
    break;

  case 736:
#line 4725 "gram.y"
    { yyval.list = yyvsp[-1].list; }
    break;

  case 737:
#line 4726 "gram.y"
    { yyval.list = NIL; }
    break;

  case 738:
#line 4727 "gram.y"
    { yyval.list = NIL; }
    break;

  case 739:
#line 4731 "gram.y"
    { yyval.list = yyvsp[0].list;}
    break;

  case 740:
#line 4732 "gram.y"
    { yyval.list = NIL; }
    break;

  case 741:
#line 4736 "gram.y"
    { yyval.list = yyvsp[0].list; }
    break;

  case 742:
#line 4740 "gram.y"
    { yyval.list = makeList1(yyvsp[0].sortgroupby); }
    break;

  case 743:
#line 4741 "gram.y"
    { yyval.list = lappend(yyvsp[-2].list, yyvsp[0].sortgroupby); }
    break;

  case 744:
#line 4745 "gram.y"
    {
					yyval.sortgroupby = makeNode(SortGroupBy);
					yyval.sortgroupby->node = yyvsp[-1].node;
					yyval.sortgroupby->useOp = yyvsp[0].list;
				}
    break;

  case 745:
#line 4752 "gram.y"
    { yyval.list = yyvsp[0].list; }
    break;

  case 746:
#line 4754 "gram.y"
    { yyval.list = makeList1(makeString("<")); }
    break;

  case 747:
#line 4756 "gram.y"
    { yyval.list = makeList1(makeString(">")); }
    break;

  case 748:
#line 4758 "gram.y"
    { yyval.list = makeList1(makeString("<"));	/*default*/ }
    break;

  case 749:
#line 4764 "gram.y"
    { yyval.list = makeList2(yyvsp[0].node, yyvsp[-2].node); }
    break;

  case 750:
#line 4766 "gram.y"
    { yyval.list = makeList2(yyvsp[-2].node, yyvsp[0].node); }
    break;

  case 751:
#line 4768 "gram.y"
    { yyval.list = makeList2(NULL, yyvsp[0].node); }
    break;

  case 752:
#line 4770 "gram.y"
    { yyval.list = makeList2(yyvsp[0].node, NULL); }
    break;

  case 753:
#line 4773 "gram.y"
    { elog(ERROR,
					"LIMIT #,# syntax not supported.\n\tUse separate LIMIT and OFFSET clauses."); }
    break;

  case 754:
#line 4779 "gram.y"
    { yyval.list = yyvsp[0].list; }
    break;

  case 755:
#line 4781 "gram.y"
    { yyval.list = makeList2(NULL,NULL); }
    break;

  case 756:
#line 4786 "gram.y"
    {
					Const	*n = makeNode(Const);

					if (yyvsp[0].ival < 0)
						elog(ERROR, "LIMIT must not be negative");

					n->consttype	= INT4OID;
					n->constlen		= sizeof(int4);
					n->constvalue	= Int32GetDatum(yyvsp[0].ival);
					n->constisnull	= FALSE;
					n->constbyval	= TRUE;
					n->constisset	= FALSE;
					n->constiscast	= FALSE;
					yyval.node = (Node *)n;
				}
    break;

  case 757:
#line 4802 "gram.y"
    {
					/* LIMIT ALL is represented as a NULL constant */
					Const	*n = makeNode(Const);

					n->consttype	= INT4OID;
					n->constlen		= sizeof(int4);
					n->constvalue	= (Datum) 0;
					n->constisnull	= TRUE;
					n->constbyval	= TRUE;
					n->constisset	= FALSE;
					n->constiscast	= FALSE;
					yyval.node = (Node *)n;
				}
    break;

  case 758:
#line 4816 "gram.y"
    {
					Param	*n = makeNode(Param);

					n->paramkind	= PARAM_NUM;
					n->paramid		= yyvsp[0].ival;
					n->paramtype	= INT4OID;
					yyval.node = (Node *)n;
				}
    break;

  case 759:
#line 4828 "gram.y"
    {
					Const	*n = makeNode(Const);

					if (yyvsp[0].ival < 0)
						elog(ERROR, "OFFSET must not be negative");

					n->consttype	= INT4OID;
					n->constlen		= sizeof(int4);
					n->constvalue	= Int32GetDatum(yyvsp[0].ival);
					n->constisnull	= FALSE;
					n->constbyval	= TRUE;
					n->constisset	= FALSE;
					n->constiscast	= FALSE;
					yyval.node = (Node *)n;
				}
    break;

  case 760:
#line 4844 "gram.y"
    {
					Param	*n = makeNode(Param);

					n->paramkind	= PARAM_NUM;
					n->paramid		= yyvsp[0].ival;
					n->paramtype	= INT4OID;
					yyval.node = (Node *)n;
				}
    break;

  case 761:
#line 4864 "gram.y"
    { yyval.list = yyvsp[0].list; }
    break;

  case 762:
#line 4865 "gram.y"
    { yyval.list = NIL; }
    break;

  case 763:
#line 4869 "gram.y"
    { yyval.node = yyvsp[0].node; }
    break;

  case 764:
#line 4870 "gram.y"
    { yyval.node = NULL; }
    break;

  case 765:
#line 4875 "gram.y"
    { yyval.list = yyvsp[0].list; }
    break;

  case 766:
#line 4876 "gram.y"
    { yyval.list = NIL; }
    break;

  case 767:
#line 4885 "gram.y"
    {
			elog(DEBUG1, "DECLUST: %d, PLAN: %s", yyvsp[-5].ival, yyvsp[-3].str);
			total_machines = yyvsp[-5].ival;
			elog(DEBUG1, "Looking up ...");
			fdataflow = bsearch(yyvsp[-3].str, fdflow_strs, FP_TOTAL_PLANS,
								sizeof(fdflow_t), fdflow_cmp);
			if (fdataflow)
				{
					is_pair = fdataflow->id;
					elog(DEBUG1, "found plan: %s", fdataflow->name);
				}
			else
				{
					elog(ERROR, "plan not found!");
				}

			if (strcasecmp(yyvsp[-1].str, "push") == 0)
				{
					fdataflow->pushPlan = true;
				}
			
			yyval.ival = yyvsp[-5].ival;
		}
    break;

  case 768:
#line 4909 "gram.y"
    {
			elog(DEBUG1, "DECLUST: %d, PLAN: %s", yyvsp[-3].ival, yyvsp[-1].str);
			total_machines = yyvsp[-3].ival;
			elog(DEBUG1, "Looking up ...");
			fdataflow = bsearch(yyvsp[-1].str, fdflow_strs, FP_TOTAL_PLANS,
								sizeof(fdflow_t), fdflow_cmp);
			if (fdataflow)
				{
					is_pair = fdataflow->id;
					elog(DEBUG1, "found plan: %s", fdataflow->name);
				}
			else
				{
					elog(ERROR, "plan not found!");
				}
			
			yyval.ival = yyvsp[-3].ival;
		}
    break;

  case 769:
#line 4928 "gram.y"
    {
			  elog(DEBUG1, "DECLUST: %d, PLAN: none", yyvsp[-1].ival);
			  total_machines = yyvsp[-1].ival;
			  is_pair = FALSE;
			  yyval.ival = yyvsp[-1].ival;
		  }
    break;

  case 770:
#line 4934 "gram.y"
    { yyval.ival = 1; }
    break;

  case 771:
#line 4941 "gram.y"
    { 
						  AggWindowSpec *spec;
/* 						  elog(ERROR, "At the moment, this feature is disabled."); */
						  spec = palloc(sizeof(AggWindowSpec));
						  elog(DEBUG1, "AGG WINDOW: %d HOP: %d", yyvsp[-3].ival, yyvsp[-1].ival);
						  spec->window_interval = yyvsp[-3].ival;
						  spec->hop_size = yyvsp[-1].ival;
						  yyval.agg_window = spec;
					  }
    break;

  case 772:
#line 4950 "gram.y"
    { yyval.agg_window = NULL; }
    break;

  case 773:
#line 4959 "gram.y"
    {
			   WindowExpr *we = yyvsp[-3].window;
			   
			   if (yyvsp[-2].value != NULL)
				 {
				   we->slidetimeout = yyvsp[-2].value;
				 }
			   else
				 {
				   we->slidetimeout = makeInteger(-1);
				 }
			   
			   if ( yyvsp[-1].str != NULL )
				 we->startat = makeString(yyvsp[-1].str);
			   
			   yyval.window = we;
			 }
    break;

  case 774:
#line 4977 "gram.y"
    { 
			   yyval.window= NULL;
			 }
    break;

  case 775:
#line 4983 "gram.y"
    { 
 				  WindowExpr *we = makeNode(WindowExpr);
				  we->relation = NULL;
				  we->slices = NIL;
				  we->rangeby = makeString(yyvsp[-1].str);
				  if (yyvsp[0].str != NULL) {
					we->slideby = makeString(yyvsp[0].str);
				  }
				  yyval.window = we;
				}
    break;

  case 776:
#line 4994 "gram.y"
    {
				  WindowExpr *we = makeNode(WindowExpr);
				  we->relation = NULL;
				  we->rangeby = we->slideby = NULL;
				  we->slices = yyvsp[0].list;
				  yyval.window = we;
				}
    break;

  case 777:
#line 5003 "gram.y"
    { yyval.list = yyvsp[0].list;  }
    break;

  case 778:
#line 5006 "gram.y"
    { yyval.list = makeList1(makeString(yyvsp[0].str)); }
    break;

  case 779:
#line 5007 "gram.y"
    { yyval.list = lappend(yyvsp[-2].list, makeString(yyvsp[0].str)); }
    break;

  case 780:
#line 5011 "gram.y"
    {
  yyval.str = yyvsp[0].str;
}
    break;

  case 781:
#line 5015 "gram.y"
    {
  yyval.str = NULL;
}
    break;

  case 782:
#line 5021 "gram.y"
    {
  yyval.value = makeInteger(yyvsp[0].ival);
}
    break;

  case 783:
#line 5025 "gram.y"
    {
  yyval.value=NULL;
}
    break;

  case 784:
#line 5030 "gram.y"
    {
  yyval.str = yyvsp[0].str;
}
    break;

  case 785:
#line 5034 "gram.y"
    {
  yyval.str = NULL;
}
    break;

  case 786:
#line 5040 "gram.y"
    {
  yyval.str = yyvsp[0].str;
}
    break;

  case 787:
#line 5044 "gram.y"
    {
  yyval.str= NULL;
}
    break;

  case 788:
#line 5053 "gram.y"
    { yyval.list = yyvsp[0].list; }
    break;

  case 789:
#line 5054 "gram.y"
    { yyval.list = NULL; }
    break;

  case 790:
#line 5058 "gram.y"
    { yyval.list = yyvsp[0].list; }
    break;

  case 791:
#line 5059 "gram.y"
    { yyval.list = NULL; }
    break;

  case 792:
#line 5063 "gram.y"
    { yyval.list = yyvsp[0].list; }
    break;

  case 793:
#line 5064 "gram.y"
    { yyval.list = makeList1(NULL); }
    break;

  case 794:
#line 5076 "gram.y"
    { yyval.list = yyvsp[0].list; }
    break;

  case 795:
#line 5077 "gram.y"
    { yyval.list = NIL; }
    break;

  case 796:
#line 5081 "gram.y"
    { yyval.list = makeList1(yyvsp[0].node); }
    break;

  case 797:
#line 5082 "gram.y"
    { yyval.list = lappend(yyvsp[-2].list, yyvsp[0].node);	}
    break;

  case 798:
#line 5086 "gram.y"
    {
   yyval.node = yyvsp[0].node;
}
    break;

  case 799:
#line 5090 "gram.y"
    {
   yyval.node = NULL;
}
    break;

  case 800:
#line 5103 "gram.y"
    {
				  yyvsp[-1].range->window = (Node*)yyvsp[0].window;
				  if(yyvsp[0].window != NULL)
				    {
				      yyvsp[0].window->relation=yyvsp[-1].range;
				    }
				  yyval.node = (Node *) yyvsp[-1].range;
				}
    break;

  case 801:
#line 5112 "gram.y"
    {
					yyvsp[-2].range->alias = yyvsp[-1].alias;
					yyvsp[-2].range->window=(Node*)yyvsp[0].window;
					if(yyvsp[0].window != NULL)
					  {
					    yyvsp[0].window->relation=yyvsp[-2].range;
					  }
					yyval.node = (Node *) yyvsp[-2].range;
				}
    break;

  case 802:
#line 5122 "gram.y"
    {
					RangeFunction *n = makeNode(RangeFunction);
					n->funccallnode = yyvsp[0].node;
					n->coldeflist = NIL;
					yyval.node = (Node *) n;
				}
    break;

  case 803:
#line 5129 "gram.y"
    {
					RangeFunction *n = makeNode(RangeFunction);
					n->funccallnode = yyvsp[-1].node;
					n->alias = yyvsp[0].alias;
					n->coldeflist = NIL;
					yyval.node = (Node *) n;
				}
    break;

  case 804:
#line 5137 "gram.y"
    {
					RangeFunction *n = makeNode(RangeFunction);
					n->funccallnode = yyvsp[-4].node;
					n->coldeflist = yyvsp[-1].list;
					yyval.node = (Node *) n;
				}
    break;

  case 805:
#line 5144 "gram.y"
    {
					RangeFunction *n = makeNode(RangeFunction);
					Alias *a = makeNode(Alias);
					n->funccallnode = yyvsp[-5].node;
					a->aliasname = yyvsp[-3].str;
					n->alias = a;
					n->coldeflist = yyvsp[-1].list;
					yyval.node = (Node *) n;
				}
    break;

  case 806:
#line 5154 "gram.y"
    {
					RangeFunction *n = makeNode(RangeFunction);
					Alias *a = makeNode(Alias);
					n->funccallnode = yyvsp[-4].node;
					a->aliasname = yyvsp[-3].str;
					n->alias = a;
					n->coldeflist = yyvsp[-1].list;
					yyval.node = (Node *) n;
				}
    break;

  case 807:
#line 5164 "gram.y"
    {
					/*
					 * The SQL spec does not permit a subselect
					 * (<derived_table>) without an alias clause,
					 * so we don't either.  This avoids the problem
					 * of needing to invent a unique refname for it.
					 * That could be surmounted if there's sufficient
					 * popular demand, but for now let's just implement
					 * the spec and see if anyone complains.
					 * However, it does seem like a good idea to emit
					 * an error message that's better than "parse error".
					 */
					elog(ERROR, "sub-SELECT in FROM must have an alias"
						 "\n\tFor example, FROM (SELECT ...) [AS] foo");
					yyval.node = NULL;
				}
    break;

  case 808:
#line 5181 "gram.y"
    {
					RangeSubselect *n = makeNode(RangeSubselect);
					n->subquery = yyvsp[-1].node;
					n->alias = yyvsp[0].alias;
					yyval.node = (Node *) n;
				}
    break;

  case 809:
#line 5188 "gram.y"
    {
					yyval.node = (Node *) yyvsp[0].jexpr;
				}
    break;

  case 810:
#line 5192 "gram.y"
    {
					yyvsp[-2].jexpr->alias = yyvsp[0].alias;
					yyval.node = (Node *) yyvsp[-2].jexpr;
				}
    break;

  case 811:
#line 5218 "gram.y"
    {
					yyval.jexpr = yyvsp[-1].jexpr;
				}
    break;

  case 812:
#line 5222 "gram.y"
    {
					/* CROSS JOIN is same as unqualified inner join */
					JoinExpr *n = makeNode(JoinExpr);
					n->jointype = JOIN_INNER;
					n->isNatural = FALSE;
					n->larg = yyvsp[-3].node;
					n->rarg = yyvsp[0].node;
					n->using = NIL;
					n->quals = NULL;
					yyval.jexpr = n;
				}
    break;

  case 813:
#line 5234 "gram.y"
    {
					/* UNION JOIN is made into 1 token to avoid shift/reduce
					 * conflict against regular UNION keyword.
					 */
					JoinExpr *n = makeNode(JoinExpr);
					n->jointype = JOIN_UNION;
					n->isNatural = FALSE;
					n->larg = yyvsp[-2].node;
					n->rarg = yyvsp[0].node;
					n->using = NIL;
					n->quals = NULL;
					yyval.jexpr = n;
				}
    break;

  case 814:
#line 5248 "gram.y"
    {
					JoinExpr *n = makeNode(JoinExpr);
					n->jointype = yyvsp[-3].jtype;
					n->isNatural = FALSE;
					n->larg = yyvsp[-4].node;
					n->rarg = yyvsp[-1].node;
					if (yyvsp[0].node != NULL && IsA(yyvsp[0].node, List))
						n->using = (List *) yyvsp[0].node; /* USING clause */
					else
						n->quals = yyvsp[0].node; /* ON clause */
					yyval.jexpr = n;
				}
    break;

  case 815:
#line 5261 "gram.y"
    {
					/* letting join_type reduce to empty doesn't work */
					JoinExpr *n = makeNode(JoinExpr);
					n->jointype = JOIN_INNER;
					n->isNatural = FALSE;
					n->larg = yyvsp[-3].node;
					n->rarg = yyvsp[-1].node;
					if (yyvsp[0].node != NULL && IsA(yyvsp[0].node, List))
						n->using = (List *) yyvsp[0].node; /* USING clause */
					else
						n->quals = yyvsp[0].node; /* ON clause */
					yyval.jexpr = n;
				}
    break;

  case 816:
#line 5275 "gram.y"
    {
					JoinExpr *n = makeNode(JoinExpr);
					n->jointype = yyvsp[-2].jtype;
					n->isNatural = TRUE;
					n->larg = yyvsp[-4].node;
					n->rarg = yyvsp[0].node;
					n->using = NIL; /* figure out which columns later... */
					n->quals = NULL; /* fill later */
					yyval.jexpr = n;
				}
    break;

  case 817:
#line 5286 "gram.y"
    {
					/* letting join_type reduce to empty doesn't work */
					JoinExpr *n = makeNode(JoinExpr);
					n->jointype = JOIN_INNER;
					n->isNatural = TRUE;
					n->larg = yyvsp[-3].node;
					n->rarg = yyvsp[0].node;
					n->using = NIL; /* figure out which columns later... */
					n->quals = NULL; /* fill later */
					yyval.jexpr = n;
				}
    break;

  case 818:
#line 5301 "gram.y"
    {
					yyval.alias = makeNode(Alias);
					yyval.alias->aliasname = yyvsp[-3].str;
					yyval.alias->colnames = yyvsp[-1].list;
				}
    break;

  case 819:
#line 5307 "gram.y"
    {
					yyval.alias = makeNode(Alias);
					yyval.alias->aliasname = yyvsp[0].str;
				}
    break;

  case 820:
#line 5312 "gram.y"
    {
					yyval.alias = makeNode(Alias);
					yyval.alias->aliasname = yyvsp[-3].str;
					yyval.alias->colnames = yyvsp[-1].list;
				}
    break;

  case 821:
#line 5318 "gram.y"
    {
					yyval.alias = makeNode(Alias);
					yyval.alias->aliasname = yyvsp[0].str;
				}
    break;

  case 822:
#line 5324 "gram.y"
    { yyval.jtype = JOIN_FULL; }
    break;

  case 823:
#line 5325 "gram.y"
    { yyval.jtype = JOIN_LEFT; }
    break;

  case 824:
#line 5326 "gram.y"
    { yyval.jtype = JOIN_RIGHT; }
    break;

  case 825:
#line 5327 "gram.y"
    { yyval.jtype = JOIN_INNER; }
    break;

  case 826:
#line 5331 "gram.y"
    { yyval.node = NULL; }
    break;

  case 827:
#line 5332 "gram.y"
    { yyval.node = NULL; }
    break;

  case 828:
#line 5344 "gram.y"
    { yyval.node = (Node *) yyvsp[-1].list; }
    break;

  case 829:
#line 5345 "gram.y"
    { yyval.node = yyvsp[0].node; }
    break;

  case 830:
#line 5351 "gram.y"
    {
					/* default inheritance */
					yyval.range = yyvsp[0].range;
					yyval.range->inhOpt = INH_DEFAULT;
					yyval.range->alias = NULL;
				}
    break;

  case 831:
#line 5358 "gram.y"
    {
					/* inheritance query */
					yyval.range = yyvsp[-1].range;
					yyval.range->inhOpt = INH_YES;
					yyval.range->alias = NULL;
				}
    break;

  case 832:
#line 5365 "gram.y"
    {
					/* no inheritance */
					yyval.range = yyvsp[0].range;
					yyval.range->inhOpt = INH_NO;
					yyval.range->alias = NULL;
				}
    break;

  case 833:
#line 5372 "gram.y"
    {
					/* no inheritance, SQL99-style syntax */
					yyval.range = yyvsp[-1].range;
					yyval.range->inhOpt = INH_NO;
					yyval.range->alias = NULL;
				}
    break;

  case 834:
#line 5382 "gram.y"
    {
					FuncCall *n = makeNode(FuncCall);
					n->funcname = yyvsp[-2].list;
					n->args = NIL;
					n->agg_star = FALSE;
					n->agg_distinct = FALSE;
					yyval.node = (Node *)n;
				}
    break;

  case 835:
#line 5391 "gram.y"
    {
					FuncCall *n = makeNode(FuncCall);
					n->funcname = yyvsp[-3].list;
					n->args = yyvsp[-1].list;
					n->agg_star = FALSE;
					n->agg_distinct = FALSE;
					yyval.node = (Node *)n;
				}
    break;

  case 836:
#line 5403 "gram.y"
    { yyval.node = yyvsp[0].node; }
    break;

  case 837:
#line 5404 "gram.y"
    { yyval.node = NULL; }
    break;

  case 838:
#line 5411 "gram.y"
    {
                         EventExpr *n = (EventExpr *)yyvsp[0].node;
                         yyval.node = (Node *)n;
                       }
    break;

  case 839:
#line 5415 "gram.y"
    {yyval.node = NULL;}
    break;

  case 840:
#line 5420 "gram.y"
    {
                                       EventExpr *n = makeNode(EventExpr);
                                       n->stream_name = (RangeVar *)yyvsp[0].range;
                                       n->event_op = EVENT_SIMPLE;
                                       yyval.node = (Node *)n;
                               }
    break;

  case 841:
#line 5427 "gram.y"
    {
                                       EventExpr *n = makeNode(EventExpr);
				       n->event_op = EVENT_ANYONE;
                                       n->lchild = (EventExpr *)yyvsp[-3].node;
                                       n->rchild = (EventExpr *)yyvsp[-1].node;
				       n->time_delay_val = NULL;
                                       yyval.node = (Node *)n;
                               }
    break;

  case 842:
#line 5436 "gram.y"
    {
                                       EventExpr *n = makeNode(EventExpr);
				       n->event_op = EVENT_BOTH;
                                       n->lchild = (EventExpr *)yyvsp[-5].node;
                                       n->rchild = (EventExpr *)yyvsp[-3].node;
				       n->time_delay_val = makeString(yyvsp[-1].str);
                                       yyval.node = (Node *)n;
                               }
    break;

  case 843:
#line 5445 "gram.y"
    {
                                       EventExpr *n = makeNode(EventExpr);
				       n->event_op = EVENT_FOLLOWS;
                                       n->lchild = (EventExpr *)yyvsp[-5].node;
                                       n->rchild = (EventExpr *)yyvsp[-3].node;
				       n->time_delay_val = makeString(yyvsp[-1].str);
                                       yyval.node = (Node *)n;
                               }
    break;

  case 844:
#line 5454 "gram.y"
    {
                                       EventExpr *n = makeNode(EventExpr);
				       n->event_op = EVENT_NOTFOLLOWS;
                                       n->lchild = (EventExpr *)yyvsp[-5].node;
                                       n->rchild = (EventExpr *)yyvsp[-3].node;
				       n->time_delay_val = makeString(yyvsp[-1].str);
                                       yyval.node = (Node *)n;
                               }
    break;

  case 845:
#line 5463 "gram.y"
    {
                                       EventExpr *n = makeNode(EventExpr);
				       n->event_op = EVENT_PRECEDES;
                                       n->lchild = (EventExpr *)yyvsp[-5].node;
                                       n->rchild = (EventExpr *)yyvsp[-3].node;
				       n->time_delay_val = makeString(yyvsp[-1].str);
                                       yyval.node = (Node *)n;
                               }
    break;

  case 846:
#line 5472 "gram.y"
    {
                                       EventExpr *n = makeNode(EventExpr);
				       n->event_op = EVENT_NOTPRECEDES;
                                       n->lchild = (EventExpr *)yyvsp[-5].node;
                                       n->rchild = (EventExpr *)yyvsp[-3].node;
				       n->time_delay_val = makeString(yyvsp[-1].str);
                                       yyval.node = (Node *)n;
                               }
    break;

  case 847:
#line 5487 "gram.y"
    {
					yyval.list = makeList1(yyvsp[0].node);
				}
    break;

  case 848:
#line 5491 "gram.y"
    {
					yyval.list = lappend(yyvsp[-2].list, yyvsp[0].node);
				}
    break;

  case 849:
#line 5497 "gram.y"
    {
					ColumnDef *n = makeNode(ColumnDef);
					n->colname = yyvsp[-1].str;
					n->typename = yyvsp[0].typnam;
					n->constraints = NIL;
					n->is_local = true;
					yyval.node = (Node *)n;
				}
    break;

  case 850:
#line 5518 "gram.y"
    {
					yyval.typnam = yyvsp[-1].typnam;
					yyval.typnam->arrayBounds = yyvsp[0].list;
				}
    break;

  case 851:
#line 5523 "gram.y"
    {
					yyval.typnam = yyvsp[0].typnam;
					yyval.typnam->setof = TRUE;
				}
    break;

  case 852:
#line 5531 "gram.y"
    {  yyval.list = lappend(yyvsp[-2].list, makeInteger(-1)); }
    break;

  case 853:
#line 5533 "gram.y"
    {  yyval.list = lappend(yyvsp[-3].list, makeInteger(yyvsp[-1].ival)); }
    break;

  case 854:
#line 5535 "gram.y"
    {  yyval.list = NIL; }
    break;

  case 855:
#line 5547 "gram.y"
    { yyval.typnam = yyvsp[0].typnam; }
    break;

  case 856:
#line 5548 "gram.y"
    { yyval.typnam = yyvsp[0].typnam; }
    break;

  case 857:
#line 5549 "gram.y"
    { yyval.typnam = yyvsp[0].typnam; }
    break;

  case 858:
#line 5550 "gram.y"
    { yyval.typnam = yyvsp[0].typnam; }
    break;

  case 859:
#line 5551 "gram.y"
    { yyval.typnam = yyvsp[0].typnam; }
    break;

  case 860:
#line 5553 "gram.y"
    {
					yyval.typnam = yyvsp[-1].typnam;
					if (yyvsp[0].ival != INTERVAL_FULL_RANGE)
						yyval.typnam->typmod = INTERVAL_TYPMOD(INTERVAL_FULL_PRECISION, yyvsp[0].ival);
				}
    break;

  case 861:
#line 5559 "gram.y"
    {
					yyval.typnam = yyvsp[-4].typnam;
					if ((yyvsp[-2].ival < 0) || (yyvsp[-2].ival > MAX_INTERVAL_PRECISION))
						elog(ERROR,
						"INTERVAL(%d) precision must be between %d and %d",
							 yyvsp[-2].ival, 0, MAX_INTERVAL_PRECISION);
					yyval.typnam->typmod = INTERVAL_TYPMOD(yyvsp[-2].ival, yyvsp[0].ival);
				}
    break;

  case 862:
#line 5568 "gram.y"
    {
					yyval.typnam = makeNode(TypeName);
					yyval.typnam->names = lcons(makeString(yyvsp[-1].str), yyvsp[0].list);
					yyval.typnam->typmod = -1;
				}
    break;

  case 863:
#line 5585 "gram.y"
    { yyval.typnam = yyvsp[0].typnam; }
    break;

  case 864:
#line 5586 "gram.y"
    { yyval.typnam = yyvsp[0].typnam; }
    break;

  case 865:
#line 5587 "gram.y"
    { yyval.typnam = yyvsp[0].typnam; }
    break;

  case 866:
#line 5588 "gram.y"
    { yyval.typnam = yyvsp[0].typnam; }
    break;

  case 867:
#line 5589 "gram.y"
    { yyval.typnam = yyvsp[0].typnam; }
    break;

  case 868:
#line 5594 "gram.y"
    {
					yyval.typnam = makeTypeName(yyvsp[0].str);
				}
    break;

  case 869:
#line 5605 "gram.y"
    {
					yyval.typnam = SystemTypeName("int4");
				}
    break;

  case 870:
#line 5609 "gram.y"
    {
					yyval.typnam = SystemTypeName("int4");
				}
    break;

  case 871:
#line 5613 "gram.y"
    {
					yyval.typnam = SystemTypeName("int2");
				}
    break;

  case 872:
#line 5617 "gram.y"
    {
					yyval.typnam = SystemTypeName("int8");
				}
    break;

  case 873:
#line 5621 "gram.y"
    {
					yyval.typnam = SystemTypeName("float4");
				}
    break;

  case 874:
#line 5625 "gram.y"
    {
					yyval.typnam = yyvsp[0].typnam;
				}
    break;

  case 875:
#line 5629 "gram.y"
    {
					yyval.typnam = SystemTypeName("float8");
				}
    break;

  case 876:
#line 5633 "gram.y"
    {
					yyval.typnam = SystemTypeName("numeric");
					yyval.typnam->typmod = yyvsp[0].ival;
				}
    break;

  case 877:
#line 5638 "gram.y"
    {
					yyval.typnam = SystemTypeName("numeric");
					yyval.typnam->typmod = yyvsp[0].ival;
				}
    break;

  case 878:
#line 5643 "gram.y"
    {
					yyval.typnam = SystemTypeName("numeric");
					yyval.typnam->typmod = yyvsp[0].ival;
				}
    break;

  case 879:
#line 5648 "gram.y"
    {
					yyval.typnam = SystemTypeName("bool");
				}
    break;

  case 880:
#line 5654 "gram.y"
    {
					if (yyvsp[-1].ival < 1)
						elog(ERROR,
							"precision for FLOAT must be at least 1");
					else if (yyvsp[-1].ival < 7)
						yyval.typnam = SystemTypeName("float4");
					else if (yyvsp[-1].ival < 16)
						yyval.typnam = SystemTypeName("float8");
					else
						elog(ERROR,
							"precision for FLOAT must be less than 16");
				}
    break;

  case 881:
#line 5667 "gram.y"
    {
					yyval.typnam = SystemTypeName("float8");
				}
    break;

  case 882:
#line 5674 "gram.y"
    {
					if (yyvsp[-3].ival < 1 || yyvsp[-3].ival > NUMERIC_MAX_PRECISION)
						elog(ERROR,
							"NUMERIC precision %d must be between 1 and %d",
							 yyvsp[-3].ival, NUMERIC_MAX_PRECISION);
					if (yyvsp[-1].ival < 0 || yyvsp[-1].ival > yyvsp[-3].ival)
						elog(ERROR,
						"NUMERIC scale %d must be between 0 and precision %d",
							 yyvsp[-1].ival,yyvsp[-3].ival);

					yyval.ival = ((yyvsp[-3].ival << 16) | yyvsp[-1].ival) + VARHDRSZ;
				}
    break;

  case 883:
#line 5687 "gram.y"
    {
					if (yyvsp[-1].ival < 1 || yyvsp[-1].ival > NUMERIC_MAX_PRECISION)
						elog(ERROR,
							"NUMERIC precision %d must be between 1 and %d",
							 yyvsp[-1].ival, NUMERIC_MAX_PRECISION);

					yyval.ival = (yyvsp[-1].ival << 16) + VARHDRSZ;
				}
    break;

  case 884:
#line 5696 "gram.y"
    {
					/* Insert "-1" meaning "no limit" */
					yyval.ival = -1;
				}
    break;

  case 885:
#line 5704 "gram.y"
    {
					if (yyvsp[-3].ival < 1 || yyvsp[-3].ival > NUMERIC_MAX_PRECISION)
						elog(ERROR,
							"DECIMAL precision %d must be between 1 and %d",
									yyvsp[-3].ival, NUMERIC_MAX_PRECISION);
					if (yyvsp[-1].ival < 0 || yyvsp[-1].ival > yyvsp[-3].ival)
						elog(ERROR,
							"DECIMAL scale %d must be between 0 and precision %d",
									yyvsp[-1].ival,yyvsp[-3].ival);

					yyval.ival = ((yyvsp[-3].ival << 16) | yyvsp[-1].ival) + VARHDRSZ;
				}
    break;

  case 886:
#line 5717 "gram.y"
    {
					if (yyvsp[-1].ival < 1 || yyvsp[-1].ival > NUMERIC_MAX_PRECISION)
						elog(ERROR,
							"DECIMAL precision %d must be between 1 and %d",
									yyvsp[-1].ival, NUMERIC_MAX_PRECISION);

					yyval.ival = (yyvsp[-1].ival << 16) + VARHDRSZ;
				}
    break;

  case 887:
#line 5726 "gram.y"
    {
					/* Insert "-1" meaning "no limit" */
					yyval.ival = -1;
				}
    break;

  case 888:
#line 5738 "gram.y"
    {
					yyval.typnam = yyvsp[0].typnam;
				}
    break;

  case 889:
#line 5742 "gram.y"
    {
					yyval.typnam = yyvsp[0].typnam;
				}
    break;

  case 890:
#line 5748 "gram.y"
    {
					yyval.typnam = yyvsp[0].typnam;
				}
    break;

  case 891:
#line 5752 "gram.y"
    {
					yyval.typnam->typmod = -1;
					yyval.typnam = yyvsp[0].typnam;
				}
    break;

  case 892:
#line 5760 "gram.y"
    {
					char *typname;

					typname = yyvsp[-3].boolean ? "varbit" : "bit";
					yyval.typnam = SystemTypeName(typname);
					if (yyvsp[-1].ival < 1)
						elog(ERROR, "length for type '%s' must be at least 1",
							 typname);
					else if (yyvsp[-1].ival > (MaxAttrSize * BITS_PER_BYTE))
						elog(ERROR, "length for type '%s' cannot exceed %d",
							 typname, (MaxAttrSize * BITS_PER_BYTE));
					yyval.typnam->typmod = yyvsp[-1].ival;
				}
    break;

  case 893:
#line 5777 "gram.y"
    {
					/* bit defaults to bit(1), varbit to no limit */
					if (yyvsp[0].boolean)
					{
						yyval.typnam = SystemTypeName("varbit");
						yyval.typnam->typmod = -1;
					}
					else
					{
						yyval.typnam = SystemTypeName("bit");
						yyval.typnam->typmod = 1;
					}
				}
    break;

  case 894:
#line 5798 "gram.y"
    {
					yyval.typnam = yyvsp[0].typnam;
				}
    break;

  case 895:
#line 5802 "gram.y"
    {
					yyval.typnam = yyvsp[0].typnam;
				}
    break;

  case 896:
#line 5808 "gram.y"
    {
					yyval.typnam = yyvsp[0].typnam;
				}
    break;

  case 897:
#line 5812 "gram.y"
    {
					/* Length was not specified so allow to be unrestricted.
					 * This handles problems with fixed-length (bpchar) strings
					 * which in column definitions must default to a length
					 * of one, but should not be constrained if the length
					 * was not specified.
					 */
					yyvsp[0].typnam->typmod = -1;
					yyval.typnam = yyvsp[0].typnam;
				}
    break;

  case 898:
#line 5825 "gram.y"
    {
					if ((yyvsp[0].str != NULL) && (strcmp(yyvsp[0].str, "sql_text") != 0))
					{
						char *type;

						type = palloc(strlen(yyvsp[-4].str) + 1 + strlen(yyvsp[0].str) + 1);
						strcpy(type, yyvsp[-4].str);
						strcat(type, "_");
						strcat(type, yyvsp[0].str);
						yyvsp[-4].str = type;
					}

					yyval.typnam = SystemTypeName(yyvsp[-4].str);

					if (yyvsp[-2].ival < 1)
						elog(ERROR, "length for type '%s' must be at least 1",
							 yyvsp[-4].str);
					else if (yyvsp[-2].ival > MaxAttrSize)
						elog(ERROR, "length for type '%s' cannot exceed %d",
							 yyvsp[-4].str, MaxAttrSize);

					/* we actually implement these like a varlen, so
					 * the first 4 bytes is the length. (the difference
					 * between these and "text" is that we blank-pad and
					 * truncate where necessary)
					 */
					yyval.typnam->typmod = VARHDRSZ + yyvsp[-2].ival;
				}
    break;

  case 899:
#line 5856 "gram.y"
    {
					if ((yyvsp[0].str != NULL) && (strcmp(yyvsp[0].str, "sql_text") != 0))
					{
						char *type;

						type = palloc(strlen(yyvsp[-1].str) + 1 + strlen(yyvsp[0].str) + 1);
						strcpy(type, yyvsp[-1].str);
						strcat(type, "_");
						strcat(type, yyvsp[0].str);
						yyvsp[-1].str = type;
					}

					yyval.typnam = SystemTypeName(yyvsp[-1].str);

					/* char defaults to char(1), varchar to no limit */
					if (strcmp(yyvsp[-1].str, "bpchar") == 0)
						yyval.typnam->typmod = VARHDRSZ + 1;
					else
						yyval.typnam->typmod = -1;
				}
    break;

  case 900:
#line 5879 "gram.y"
    { yyval.str = yyvsp[0].boolean ? "varchar": "bpchar"; }
    break;

  case 901:
#line 5881 "gram.y"
    { yyval.str = yyvsp[0].boolean ? "varchar": "bpchar"; }
    break;

  case 902:
#line 5883 "gram.y"
    { yyval.str = "varchar"; }
    break;

  case 903:
#line 5885 "gram.y"
    { yyval.str = yyvsp[0].boolean ? "varchar": "bpchar"; }
    break;

  case 904:
#line 5887 "gram.y"
    { yyval.str = yyvsp[0].boolean ? "varchar": "bpchar"; }
    break;

  case 905:
#line 5889 "gram.y"
    { yyval.str = yyvsp[0].boolean ? "varchar": "bpchar"; }
    break;

  case 906:
#line 5893 "gram.y"
    { yyval.boolean = TRUE; }
    break;

  case 907:
#line 5894 "gram.y"
    { yyval.boolean = FALSE; }
    break;

  case 908:
#line 5898 "gram.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 909:
#line 5899 "gram.y"
    { yyval.str = NULL; }
    break;

  case 910:
#line 5903 "gram.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 911:
#line 5904 "gram.y"
    { yyval.str = NULL; }
    break;

  case 912:
#line 5909 "gram.y"
    {
					if (yyvsp[0].boolean)
						yyval.typnam = SystemTypeName("timestamptz");
					else
						yyval.typnam = SystemTypeName("timestamp");
					/* XXX the timezone field seems to be unused
					 * - thomas 2001-09-06
					 */
					yyval.typnam->timezone = yyvsp[0].boolean;
					if ((yyvsp[-2].ival < 0) || (yyvsp[-2].ival > MAX_TIMESTAMP_PRECISION))
						elog(ERROR,
						"TIMESTAMP(%d)%s precision must be between %d and %d",
							 yyvsp[-2].ival, (yyvsp[0].boolean ? " WITH TIME ZONE": ""), 0,
							 MAX_TIMESTAMP_PRECISION);
					yyval.typnam->typmod = yyvsp[-2].ival;
				}
    break;

  case 913:
#line 5926 "gram.y"
    {
					if (yyvsp[0].boolean)
						yyval.typnam = SystemTypeName("timestamptz");
					else
						yyval.typnam = SystemTypeName("timestamp");
					/* XXX the timezone field seems to be unused
					 * - thomas 2001-09-06
					 */
					yyval.typnam->timezone = yyvsp[0].boolean;
					/* SQL99 specified a default precision of six
					 * for schema definitions. But for timestamp
					 * literals we don't want to throw away precision
					 * so leave this as unspecified for now.
					 * Later, we may want a different production
					 * for schemas. - thomas 2001-12-07
					 */
					yyval.typnam->typmod = -1;
				}
    break;

  case 914:
#line 5945 "gram.y"
    {
					if (yyvsp[0].boolean)
						yyval.typnam = SystemTypeName("timetz");
					else
						yyval.typnam = SystemTypeName("time");
					if ((yyvsp[-2].ival < 0) || (yyvsp[-2].ival > MAX_TIME_PRECISION))
						elog(ERROR,
						"TIME(%d)%s precision must be between %d and %d",
							 yyvsp[-2].ival, (yyvsp[0].boolean ? " WITH TIME ZONE": ""), 0,
							 MAX_TIME_PRECISION);
					yyval.typnam->typmod = yyvsp[-2].ival;
				}
    break;

  case 915:
#line 5958 "gram.y"
    {
					if (yyvsp[0].boolean)
						yyval.typnam = SystemTypeName("timetz");
					else
						yyval.typnam = SystemTypeName("time");
					/* SQL99 specified a default precision of zero.
					 * See comments for timestamp above on why we will
					 * leave this unspecified for now. - thomas 2001-12-07
					 */
					yyval.typnam->typmod = -1;
				}
    break;

  case 916:
#line 5972 "gram.y"
    { yyval.typnam = SystemTypeName("interval"); }
    break;

  case 917:
#line 5976 "gram.y"
    { yyval.boolean = TRUE; }
    break;

  case 918:
#line 5977 "gram.y"
    { yyval.boolean = FALSE; }
    break;

  case 919:
#line 5978 "gram.y"
    { yyval.boolean = FALSE; }
    break;

  case 920:
#line 5982 "gram.y"
    { yyval.ival = INTERVAL_MASK(YEAR); }
    break;

  case 921:
#line 5983 "gram.y"
    { yyval.ival = INTERVAL_MASK(MONTH); }
    break;

  case 922:
#line 5984 "gram.y"
    { yyval.ival = INTERVAL_MASK(DAY); }
    break;

  case 923:
#line 5985 "gram.y"
    { yyval.ival = INTERVAL_MASK(HOUR); }
    break;

  case 924:
#line 5986 "gram.y"
    { yyval.ival = INTERVAL_MASK(MINUTE); }
    break;

  case 925:
#line 5987 "gram.y"
    { yyval.ival = INTERVAL_MASK(SECOND); }
    break;

  case 926:
#line 5989 "gram.y"
    { yyval.ival = INTERVAL_MASK(YEAR) | INTERVAL_MASK(MONTH); }
    break;

  case 927:
#line 5991 "gram.y"
    { yyval.ival = INTERVAL_MASK(DAY) | INTERVAL_MASK(HOUR); }
    break;

  case 928:
#line 5993 "gram.y"
    { yyval.ival = INTERVAL_MASK(DAY) | INTERVAL_MASK(HOUR)
						| INTERVAL_MASK(MINUTE); }
    break;

  case 929:
#line 5996 "gram.y"
    { yyval.ival = INTERVAL_MASK(DAY) | INTERVAL_MASK(HOUR)
						| INTERVAL_MASK(MINUTE) | INTERVAL_MASK(SECOND); }
    break;

  case 930:
#line 5999 "gram.y"
    { yyval.ival = INTERVAL_MASK(HOUR) | INTERVAL_MASK(MINUTE); }
    break;

  case 931:
#line 6001 "gram.y"
    { yyval.ival = INTERVAL_MASK(HOUR) | INTERVAL_MASK(MINUTE)
						| INTERVAL_MASK(SECOND); }
    break;

  case 932:
#line 6004 "gram.y"
    { yyval.ival = INTERVAL_MASK(MINUTE) | INTERVAL_MASK(SECOND); }
    break;

  case 933:
#line 6005 "gram.y"
    { yyval.ival = INTERVAL_FULL_RANGE; }
    break;

  case 934:
#line 6021 "gram.y"
    {
					SubLink *n = makeNode(SubLink);
					n->lefthand = yyvsp[-2].list;
					n->oper = (List *) makeSimpleA_Expr(OP, "=", NULL, NULL);
					n->useor = FALSE;
					n->subLinkType = ANY_SUBLINK;
					n->subselect = yyvsp[0].node;
					yyval.node = (Node *)n;
				}
    break;

  case 935:
#line 6031 "gram.y"
    {
					SubLink *n = makeNode(SubLink);
					n->lefthand = yyvsp[-3].list;
					n->oper = (List *) makeSimpleA_Expr(OP, "<>", NULL, NULL);
					n->useor = TRUE;
					n->subLinkType = ALL_SUBLINK;
					n->subselect = yyvsp[0].node;
					yyval.node = (Node *)n;
				}
    break;

  case 936:
#line 6042 "gram.y"
    {
					SubLink *n = makeNode(SubLink);
					n->lefthand = yyvsp[-3].list;
					n->oper = (List *) makeA_Expr(OP, yyvsp[-2].list, NULL, NULL);
					if (strcmp(strVal(llast(yyvsp[-2].list)), "<>") == 0)
						n->useor = TRUE;
					else
						n->useor = FALSE;
					n->subLinkType = yyvsp[-1].ival;
					n->subselect = yyvsp[0].node;
					yyval.node = (Node *)n;
				}
    break;

  case 937:
#line 6056 "gram.y"
    {
					SubLink *n = makeNode(SubLink);
					n->lefthand = yyvsp[-2].list;
					n->oper = (List *) makeA_Expr(OP, yyvsp[-1].list, NULL, NULL);
					if (strcmp(strVal(llast(yyvsp[-1].list)), "<>") == 0)
						n->useor = TRUE;
					else
						n->useor = FALSE;
					n->subLinkType = MULTIEXPR_SUBLINK;
					n->subselect = yyvsp[0].node;
					yyval.node = (Node *)n;
				}
    break;

  case 938:
#line 6070 "gram.y"
    {
					yyval.node = makeRowExpr(yyvsp[-1].list, yyvsp[-2].list, yyvsp[0].list);
				}
    break;

  case 939:
#line 6074 "gram.y"
    {
					yyval.node = makeRowNullTest(IS_NULL, yyvsp[-2].list);
				}
    break;

  case 940:
#line 6078 "gram.y"
    {
					yyval.node = makeRowNullTest(IS_NOT_NULL, yyvsp[-3].list);
				}
    break;

  case 941:
#line 6082 "gram.y"
    {
					yyval.node = (Node *)makeOverlaps(yyvsp[-2].list, yyvsp[0].list);
				}
    break;

  case 942:
#line 6087 "gram.y"
    {
					/* IS DISTINCT FROM has the following rules for non-array types:
					 * a) the row lengths must be equal
					 * b) if both rows are zero-length, then they are not distinct
					 * c) if any element is distinct, the rows are distinct
					 * The rules for an element being distinct:
					 * a) if the elements are both NULL, then they are not distinct
					 * b) if the elements compare to be equal, then they are not distinct
					 * c) otherwise, they are distinct
					 */
					List *largs = yyvsp[-4].list;
					List *rargs = yyvsp[0].list;
					/* lengths don't match? then complain */
					if (length(largs) != length(rargs))
					{
						elog(ERROR, "Unequal number of entries in row expression");
					}
					/* both are zero-length rows? then they are not distinct */
					else if (length(largs) <= 0)
					{
						yyval.node = (Node *)makeBoolConst(FALSE);
					}
					/* otherwise, we need to compare each element */
					else
					{
						yyval.node = (Node *)makeDistinctExpr(largs, rargs);
					}
				}
    break;

  case 943:
#line 6121 "gram.y"
    { yyval.list = yyvsp[-1].list; }
    break;

  case 944:
#line 6122 "gram.y"
    { yyval.list = makeList1(yyvsp[-1].node); }
    break;

  case 945:
#line 6123 "gram.y"
    { yyval.list = NULL; }
    break;

  case 946:
#line 6124 "gram.y"
    { yyval.list = yyvsp[-1].list; }
    break;

  case 947:
#line 6127 "gram.y"
    { yyval.list = lappend(yyvsp[-2].list, yyvsp[0].node); }
    break;

  case 948:
#line 6130 "gram.y"
    { yyval.ival = ANY_SUBLINK; }
    break;

  case 949:
#line 6131 "gram.y"
    { yyval.ival = ANY_SUBLINK; }
    break;

  case 950:
#line 6132 "gram.y"
    { yyval.ival = ALL_SUBLINK; }
    break;

  case 951:
#line 6135 "gram.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 952:
#line 6136 "gram.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 953:
#line 6139 "gram.y"
    { yyval.str = "+"; }
    break;

  case 954:
#line 6140 "gram.y"
    { yyval.str = "-"; }
    break;

  case 955:
#line 6141 "gram.y"
    { yyval.str = "*"; }
    break;

  case 956:
#line 6142 "gram.y"
    { yyval.str = "/"; }
    break;

  case 957:
#line 6143 "gram.y"
    { yyval.str = "%"; }
    break;

  case 958:
#line 6144 "gram.y"
    { yyval.str = "^"; }
    break;

  case 959:
#line 6145 "gram.y"
    { yyval.str = "<"; }
    break;

  case 960:
#line 6146 "gram.y"
    { yyval.str = ">"; }
    break;

  case 961:
#line 6147 "gram.y"
    { yyval.str = "="; }
    break;

  case 962:
#line 6151 "gram.y"
    { yyval.list = makeList1(makeString(yyvsp[0].str)); }
    break;

  case 963:
#line 6152 "gram.y"
    { yyval.list = yyvsp[-1].list; }
    break;

  case 964:
#line 6157 "gram.y"
    { yyval.list = makeList1(makeString(yyvsp[0].str)); }
    break;

  case 965:
#line 6158 "gram.y"
    { yyval.list = yyvsp[-1].list; }
    break;

  case 966:
#line 6177 "gram.y"
    { yyval.node = yyvsp[0].node; }
    break;

  case 967:
#line 6179 "gram.y"
    { yyval.node = makeTypeCast(yyvsp[-2].node, yyvsp[0].typnam); }
    break;

  case 968:
#line 6181 "gram.y"
    {
					FuncCall *n = makeNode(FuncCall);
					n->funcname = SystemFuncName(yyvsp[0].str);
					n->args = makeList1(yyvsp[-2].node);
					n->agg_star = FALSE;
					n->agg_distinct = FALSE;
					yyval.node = (Node *) n;
				}
    break;

  case 969:
#line 6190 "gram.y"
    {
					FuncCall *n = makeNode(FuncCall);
					n->funcname = SystemFuncName("timezone");
					n->args = makeList2(yyvsp[0].node, yyvsp[-4].node);
					n->agg_star = FALSE;
					n->agg_distinct = FALSE;
					yyval.node = (Node *) n;
				}
    break;

  case 970:
#line 6208 "gram.y"
    { yyval.node = (Node *) makeSimpleA_Expr(OP, "+", NULL, yyvsp[0].node); }
    break;

  case 971:
#line 6210 "gram.y"
    { yyval.node = doNegate(yyvsp[0].node); }
    break;

  case 972:
#line 6212 "gram.y"
    { yyval.node = (Node *) makeSimpleA_Expr(OP, "%", NULL, yyvsp[0].node); }
    break;

  case 973:
#line 6214 "gram.y"
    { yyval.node = (Node *) makeSimpleA_Expr(OP, "^", NULL, yyvsp[0].node); }
    break;

  case 974:
#line 6216 "gram.y"
    { yyval.node = (Node *) makeSimpleA_Expr(OP, "%", yyvsp[-1].node, NULL); }
    break;

  case 975:
#line 6218 "gram.y"
    { yyval.node = (Node *) makeSimpleA_Expr(OP, "^", yyvsp[-1].node, NULL); }
    break;

  case 976:
#line 6220 "gram.y"
    { yyval.node = (Node *) makeSimpleA_Expr(OP, "+", yyvsp[-2].node, yyvsp[0].node); }
    break;

  case 977:
#line 6222 "gram.y"
    { yyval.node = (Node *) makeSimpleA_Expr(OP, "-", yyvsp[-2].node, yyvsp[0].node); }
    break;

  case 978:
#line 6224 "gram.y"
    { yyval.node = (Node *) makeSimpleA_Expr(OP, "*", yyvsp[-2].node, yyvsp[0].node); }
    break;

  case 979:
#line 6226 "gram.y"
    { yyval.node = (Node *) makeSimpleA_Expr(OP, "/", yyvsp[-2].node, yyvsp[0].node); }
    break;

  case 980:
#line 6228 "gram.y"
    { yyval.node = (Node *) makeSimpleA_Expr(OP, "%", yyvsp[-2].node, yyvsp[0].node); }
    break;

  case 981:
#line 6230 "gram.y"
    { yyval.node = (Node *) makeSimpleA_Expr(OP, "^", yyvsp[-2].node, yyvsp[0].node); }
    break;

  case 982:
#line 6232 "gram.y"
    { yyval.node = (Node *) makeSimpleA_Expr(OP, "<", yyvsp[-2].node, yyvsp[0].node); }
    break;

  case 983:
#line 6234 "gram.y"
    { yyval.node = (Node *) makeSimpleA_Expr(OP, ">", yyvsp[-2].node, yyvsp[0].node); }
    break;

  case 984:
#line 6236 "gram.y"
    { yyval.node = (Node *) makeSimpleA_Expr(OP, "=", yyvsp[-2].node, yyvsp[0].node); }
    break;

  case 985:
#line 6239 "gram.y"
    { yyval.node = (Node *) makeA_Expr(OP, yyvsp[-1].list, yyvsp[-2].node, yyvsp[0].node); }
    break;

  case 986:
#line 6241 "gram.y"
    { yyval.node = (Node *) makeA_Expr(OP, yyvsp[-1].list, NULL, yyvsp[0].node); }
    break;

  case 987:
#line 6243 "gram.y"
    { yyval.node = (Node *) makeA_Expr(OP, yyvsp[0].list, yyvsp[-1].node, NULL); }
    break;

  case 988:
#line 6246 "gram.y"
    { yyval.node = (Node *) makeA_Expr(AND, NIL, yyvsp[-2].node, yyvsp[0].node); }
    break;

  case 989:
#line 6248 "gram.y"
    { yyval.node = (Node *) makeA_Expr(OR, NIL, yyvsp[-2].node, yyvsp[0].node); }
    break;

  case 990:
#line 6250 "gram.y"
    { yyval.node = (Node *) makeA_Expr(NOT, NIL, NULL, yyvsp[0].node); }
    break;

  case 991:
#line 6253 "gram.y"
    { yyval.node = (Node *) makeSimpleA_Expr(OP, "~~", yyvsp[-2].node, yyvsp[0].node); }
    break;

  case 992:
#line 6255 "gram.y"
    {
					FuncCall *n = makeNode(FuncCall);
					n->funcname = SystemFuncName("like_escape");
					n->args = makeList2(yyvsp[-2].node, yyvsp[0].node);
					n->agg_star = FALSE;
					n->agg_distinct = FALSE;
					yyval.node = (Node *) makeSimpleA_Expr(OP, "~~", yyvsp[-4].node, (Node *) n);
				}
    break;

  case 993:
#line 6264 "gram.y"
    { yyval.node = (Node *) makeSimpleA_Expr(OP, "!~~", yyvsp[-3].node, yyvsp[0].node); }
    break;

  case 994:
#line 6266 "gram.y"
    {
					FuncCall *n = makeNode(FuncCall);
					n->funcname = SystemFuncName("like_escape");
					n->args = makeList2(yyvsp[-2].node, yyvsp[0].node);
					n->agg_star = FALSE;
					n->agg_distinct = FALSE;
					yyval.node = (Node *) makeSimpleA_Expr(OP, "!~~", yyvsp[-5].node, (Node *) n);
				}
    break;

  case 995:
#line 6275 "gram.y"
    { yyval.node = (Node *) makeSimpleA_Expr(OP, "~~*", yyvsp[-2].node, yyvsp[0].node); }
    break;

  case 996:
#line 6277 "gram.y"
    {
					FuncCall *n = makeNode(FuncCall);
					n->funcname = SystemFuncName("like_escape");
					n->args = makeList2(yyvsp[-2].node, yyvsp[0].node);
					n->agg_star = FALSE;
					n->agg_distinct = FALSE;
					yyval.node = (Node *) makeSimpleA_Expr(OP, "~~*", yyvsp[-4].node, (Node *) n);
				}
    break;

  case 997:
#line 6286 "gram.y"
    { yyval.node = (Node *) makeSimpleA_Expr(OP, "!~~*", yyvsp[-3].node, yyvsp[0].node); }
    break;

  case 998:
#line 6288 "gram.y"
    {
					FuncCall *n = makeNode(FuncCall);
					n->funcname = SystemFuncName("like_escape");
					n->args = makeList2(yyvsp[-2].node, yyvsp[0].node);
					n->agg_star = FALSE;
					n->agg_distinct = FALSE;
					yyval.node = (Node *) makeSimpleA_Expr(OP, "!~~*", yyvsp[-5].node, (Node *) n);
				}
    break;

  case 999:
#line 6298 "gram.y"
    {
					A_Const *c = makeNode(A_Const);
					FuncCall *n = makeNode(FuncCall);
					c->val.type = T_Null;
					n->funcname = SystemFuncName("similar_escape");
					n->args = makeList2(yyvsp[0].node, (Node *) c);
					n->agg_star = FALSE;
					n->agg_distinct = FALSE;
					yyval.node = (Node *) makeSimpleA_Expr(OP, "~", yyvsp[-3].node, (Node *) n);
				}
    break;

  case 1000:
#line 6309 "gram.y"
    {
					FuncCall *n = makeNode(FuncCall);
					n->funcname = SystemFuncName("similar_escape");
					n->args = makeList2(yyvsp[-2].node, yyvsp[0].node);
					n->agg_star = FALSE;
					n->agg_distinct = FALSE;
					yyval.node = (Node *) makeSimpleA_Expr(OP, "~", yyvsp[-5].node, (Node *) n);
				}
    break;

  case 1001:
#line 6318 "gram.y"
    {
					A_Const *c = makeNode(A_Const);
					FuncCall *n = makeNode(FuncCall);
					c->val.type = T_Null;
					n->funcname = SystemFuncName("similar_escape");
					n->args = makeList2(yyvsp[0].node, (Node *) c);
					n->agg_star = FALSE;
					n->agg_distinct = FALSE;
					yyval.node = (Node *) makeSimpleA_Expr(OP, "!~", yyvsp[-4].node, (Node *) n);
				}
    break;

  case 1002:
#line 6329 "gram.y"
    {
					FuncCall *n = makeNode(FuncCall);
					n->funcname = SystemFuncName("similar_escape");
					n->args = makeList2(yyvsp[-2].node, yyvsp[0].node);
					n->agg_star = FALSE;
					n->agg_distinct = FALSE;
					yyval.node = (Node *) makeSimpleA_Expr(OP, "!~", yyvsp[-6].node, (Node *) n);
				}
    break;

  case 1003:
#line 6348 "gram.y"
    {
					NullTest *n = makeNode(NullTest);
					n->arg = yyvsp[-1].node;
					n->nulltesttype = IS_NULL;
					yyval.node = (Node *)n;
				}
    break;

  case 1004:
#line 6355 "gram.y"
    {
					NullTest *n = makeNode(NullTest);
					n->arg = yyvsp[-2].node;
					n->nulltesttype = IS_NULL;
					yyval.node = (Node *)n;
				}
    break;

  case 1005:
#line 6362 "gram.y"
    {
					NullTest *n = makeNode(NullTest);
					n->arg = yyvsp[-1].node;
					n->nulltesttype = IS_NOT_NULL;
					yyval.node = (Node *)n;
				}
    break;

  case 1006:
#line 6369 "gram.y"
    {
					NullTest *n = makeNode(NullTest);
					n->arg = yyvsp[-3].node;
					n->nulltesttype = IS_NOT_NULL;
					yyval.node = (Node *)n;
				}
    break;

  case 1007:
#line 6376 "gram.y"
    {
					BooleanTest *b = makeNode(BooleanTest);
					b->arg = yyvsp[-2].node;
					b->booltesttype = IS_TRUE;
					yyval.node = (Node *)b;
				}
    break;

  case 1008:
#line 6383 "gram.y"
    {
					BooleanTest *b = makeNode(BooleanTest);
					b->arg = yyvsp[-3].node;
					b->booltesttype = IS_NOT_TRUE;
					yyval.node = (Node *)b;
				}
    break;

  case 1009:
#line 6390 "gram.y"
    {
					BooleanTest *b = makeNode(BooleanTest);
					b->arg = yyvsp[-2].node;
					b->booltesttype = IS_FALSE;
					yyval.node = (Node *)b;
				}
    break;

  case 1010:
#line 6397 "gram.y"
    {
					BooleanTest *b = makeNode(BooleanTest);
					b->arg = yyvsp[-3].node;
					b->booltesttype = IS_NOT_FALSE;
					yyval.node = (Node *)b;
				}
    break;

  case 1011:
#line 6404 "gram.y"
    {
					BooleanTest *b = makeNode(BooleanTest);
					b->arg = yyvsp[-2].node;
					b->booltesttype = IS_UNKNOWN;
					yyval.node = (Node *)b;
				}
    break;

  case 1012:
#line 6411 "gram.y"
    {
					BooleanTest *b = makeNode(BooleanTest);
					b->arg = yyvsp[-3].node;
					b->booltesttype = IS_NOT_UNKNOWN;
					yyval.node = (Node *)b;
				}
    break;

  case 1013:
#line 6418 "gram.y"
    { yyval.node = (Node *) makeSimpleA_Expr(DISTINCT, "=", yyvsp[-4].node, yyvsp[0].node); }
    break;

  case 1014:
#line 6420 "gram.y"
    {
					yyval.node = (Node *) makeSimpleA_Expr(OF, "=", yyvsp[-5].node, (Node *) yyvsp[-1].list);
				}
    break;

  case 1015:
#line 6424 "gram.y"
    {
					yyval.node = (Node *) makeSimpleA_Expr(OF, "!=", yyvsp[-6].node, (Node *) yyvsp[-1].list);
				}
    break;

  case 1016:
#line 6428 "gram.y"
    {
					yyval.node = (Node *) makeA_Expr(AND, NIL,
						(Node *) makeSimpleA_Expr(OP, ">=", yyvsp[-4].node, yyvsp[-2].node),
						(Node *) makeSimpleA_Expr(OP, "<=", yyvsp[-4].node, yyvsp[0].node));
				}
    break;

  case 1017:
#line 6434 "gram.y"
    {
					yyval.node = (Node *) makeA_Expr(OR, NIL,
						(Node *) makeSimpleA_Expr(OP, "<", yyvsp[-5].node, yyvsp[-2].node),
						(Node *) makeSimpleA_Expr(OP, ">", yyvsp[-5].node, yyvsp[0].node));
				}
    break;

  case 1018:
#line 6440 "gram.y"
    {
					/* in_expr returns a SubLink or a list of a_exprs */
					if (IsA(yyvsp[0].node, SubLink))
					{
							SubLink *n = (SubLink *)yyvsp[0].node;
							n->lefthand = makeList1(yyvsp[-2].node);
							n->oper = (List *) makeSimpleA_Expr(OP, "=", NULL, NULL);
							n->useor = FALSE;
							n->subLinkType = ANY_SUBLINK;
							yyval.node = (Node *)n;
					}
					else
					{
						Node *n = NULL;
						List *l;
						foreach(l, (List *) yyvsp[0].node)
						{
							Node *cmp;
							cmp = (Node *) makeSimpleA_Expr(OP, "=", yyvsp[-2].node, lfirst(l));
							if (n == NULL)
								n = cmp;
							else
								n = (Node *) makeA_Expr(OR, NIL, n, cmp);
						}
						yyval.node = n;
					}
				}
    break;

  case 1019:
#line 6468 "gram.y"
    {
					/* in_expr returns a SubLink or a list of a_exprs */
					if (IsA(yyvsp[0].node, SubLink))
					{
						SubLink *n = (SubLink *)yyvsp[0].node;
						n->lefthand = makeList1(yyvsp[-3].node);
						n->oper = (List *) makeSimpleA_Expr(OP, "<>", NULL, NULL);
						n->useor = FALSE;
						n->subLinkType = ALL_SUBLINK;
						yyval.node = (Node *)n;
					}
					else
					{
						Node *n = NULL;
						List *l;
						foreach(l, (List *) yyvsp[0].node)
						{
							Node *cmp;
							cmp = (Node *) makeSimpleA_Expr(OP, "<>", yyvsp[-3].node, lfirst(l));
							if (n == NULL)
								n = cmp;
							else
								n = (Node *) makeA_Expr(AND, NIL, n, cmp);
						}
						yyval.node = n;
					}
				}
    break;

  case 1020:
#line 6496 "gram.y"
    {
					SubLink *n = makeNode(SubLink);
					n->lefthand = makeList1(yyvsp[-3].node);
					n->oper = (List *) makeA_Expr(OP, yyvsp[-2].list, NULL, NULL);
					n->useor = FALSE; /* doesn't matter since only one col */
					n->subLinkType = yyvsp[-1].ival;
					n->subselect = yyvsp[0].node;
					yyval.node = (Node *)n;
				}
    break;

  case 1021:
#line 6506 "gram.y"
    {
					/* Not sure how to get rid of the parentheses
					 * but there are lots of shift/reduce errors without them.
					 *
					 * Should be able to implement this by plopping the entire
					 * select into a node, then transforming the target expressions
					 * from whatever they are into count(*), and testing the
					 * entire result equal to one.
					 * But, will probably implement a separate node in the executor.
					 */
					elog(ERROR, "UNIQUE predicate is not yet implemented");
				}
    break;

  case 1022:
#line 6519 "gram.y"
    { yyval.node = yyvsp[0].node; }
    break;

  case 1023:
#line 6532 "gram.y"
    { yyval.node = yyvsp[0].node; }
    break;

  case 1024:
#line 6534 "gram.y"
    { yyval.node = makeTypeCast(yyvsp[-2].node, yyvsp[0].typnam); }
    break;

  case 1025:
#line 6536 "gram.y"
    { yyval.node = (Node *) makeSimpleA_Expr(OP, "+", NULL, yyvsp[0].node); }
    break;

  case 1026:
#line 6538 "gram.y"
    { yyval.node = doNegate(yyvsp[0].node); }
    break;

  case 1027:
#line 6540 "gram.y"
    { yyval.node = (Node *) makeSimpleA_Expr(OP, "%", NULL, yyvsp[0].node); }
    break;

  case 1028:
#line 6542 "gram.y"
    { yyval.node = (Node *) makeSimpleA_Expr(OP, "^", NULL, yyvsp[0].node); }
    break;

  case 1029:
#line 6544 "gram.y"
    { yyval.node = (Node *) makeSimpleA_Expr(OP, "%", yyvsp[-1].node, NULL); }
    break;

  case 1030:
#line 6546 "gram.y"
    { yyval.node = (Node *) makeSimpleA_Expr(OP, "^", yyvsp[-1].node, NULL); }
    break;

  case 1031:
#line 6548 "gram.y"
    { yyval.node = (Node *) makeSimpleA_Expr(OP, "+", yyvsp[-2].node, yyvsp[0].node); }
    break;

  case 1032:
#line 6550 "gram.y"
    { yyval.node = (Node *) makeSimpleA_Expr(OP, "-", yyvsp[-2].node, yyvsp[0].node); }
    break;

  case 1033:
#line 6552 "gram.y"
    { yyval.node = (Node *) makeSimpleA_Expr(OP, "*", yyvsp[-2].node, yyvsp[0].node); }
    break;

  case 1034:
#line 6554 "gram.y"
    { yyval.node = (Node *) makeSimpleA_Expr(OP, "/", yyvsp[-2].node, yyvsp[0].node); }
    break;

  case 1035:
#line 6556 "gram.y"
    { yyval.node = (Node *) makeSimpleA_Expr(OP, "%", yyvsp[-2].node, yyvsp[0].node); }
    break;

  case 1036:
#line 6558 "gram.y"
    { yyval.node = (Node *) makeSimpleA_Expr(OP, "^", yyvsp[-2].node, yyvsp[0].node); }
    break;

  case 1037:
#line 6560 "gram.y"
    { yyval.node = (Node *) makeSimpleA_Expr(OP, "<", yyvsp[-2].node, yyvsp[0].node); }
    break;

  case 1038:
#line 6562 "gram.y"
    { yyval.node = (Node *) makeSimpleA_Expr(OP, ">", yyvsp[-2].node, yyvsp[0].node); }
    break;

  case 1039:
#line 6564 "gram.y"
    { yyval.node = (Node *) makeSimpleA_Expr(OP, "=", yyvsp[-2].node, yyvsp[0].node); }
    break;

  case 1040:
#line 6566 "gram.y"
    { yyval.node = (Node *) makeA_Expr(OP, yyvsp[-1].list, yyvsp[-2].node, yyvsp[0].node); }
    break;

  case 1041:
#line 6568 "gram.y"
    { yyval.node = (Node *) makeA_Expr(OP, yyvsp[-1].list, NULL, yyvsp[0].node); }
    break;

  case 1042:
#line 6570 "gram.y"
    { yyval.node = (Node *) makeA_Expr(OP, yyvsp[0].list, yyvsp[-1].node, NULL); }
    break;

  case 1043:
#line 6572 "gram.y"
    { yyval.node = (Node *) makeSimpleA_Expr(DISTINCT, "=", yyvsp[-4].node, yyvsp[0].node); }
    break;

  case 1044:
#line 6574 "gram.y"
    {
					yyval.node = (Node *) makeSimpleA_Expr(OF, "=", yyvsp[-5].node, (Node *) yyvsp[-1].list);
				}
    break;

  case 1045:
#line 6578 "gram.y"
    {
					yyval.node = (Node *) makeSimpleA_Expr(OF, "!=", yyvsp[-6].node, (Node *) yyvsp[-1].list);
				}
    break;

  case 1046:
#line 6591 "gram.y"
    { yyval.node = (Node *) yyvsp[0].columnref; }
    break;

  case 1047:
#line 6592 "gram.y"
    { yyval.node = yyvsp[0].node; }
    break;

  case 1048:
#line 6594 "gram.y"
    {
					/*
					 * PARAM without field names is considered a constant,
					 * but with 'em, it is not.  Not very consistent ...
					 */
					ParamRef *n = makeNode(ParamRef);
					n->number = yyvsp[-2].ival;
					n->fields = yyvsp[-1].list;
					n->indirection = yyvsp[0].list;
					yyval.node = (Node *)n;
				}
    break;

  case 1049:
#line 6605 "gram.y"
    { yyval.node = yyvsp[-1].node; }
    break;

  case 1050:
#line 6607 "gram.y"
    {
					ExprFieldSelect *n = makeNode(ExprFieldSelect);
					n->arg = yyvsp[-3].node;
					n->fields = yyvsp[-1].list;
					n->indirection = yyvsp[0].list;
					yyval.node = (Node *)n;
				}
    break;

  case 1051:
#line 6615 "gram.y"
    { yyval.node = yyvsp[0].node; }
    break;

  case 1052:
#line 6617 "gram.y"
    {
					FuncCall *n = makeNode(FuncCall);
					n->funcname = yyvsp[-2].list;
					n->args = NIL;
					n->agg_star = FALSE;
					n->agg_distinct = FALSE;
					yyval.node = (Node *)n;
				}
    break;

  case 1053:
#line 6626 "gram.y"
    {
					FuncCall *n = makeNode(FuncCall);
					n->funcname = yyvsp[-3].list;
					n->args = yyvsp[-1].list;
					n->agg_star = FALSE;
					n->agg_distinct = FALSE;
					yyval.node = (Node *)n;
				}
    break;

  case 1054:
#line 6635 "gram.y"
    {
					FuncCall *n = makeNode(FuncCall);
					n->funcname = yyvsp[-4].list;
					n->args = yyvsp[-1].list;
					n->agg_star = FALSE;
					n->agg_distinct = FALSE;
					/* Ideally we'd mark the FuncCall node to indicate
					 * "must be an aggregate", but there's no provision
					 * for that in FuncCall at the moment.
					 */
					yyval.node = (Node *)n;
				}
    break;

  case 1055:
#line 6648 "gram.y"
    {
					FuncCall *n = makeNode(FuncCall);
					n->funcname = yyvsp[-4].list;
					n->args = yyvsp[-1].list;
					n->agg_star = FALSE;
					n->agg_distinct = TRUE;
					yyval.node = (Node *)n;
				}
    break;

  case 1056:
#line 6657 "gram.y"
    {
					/*
					 * For now, we transform AGGREGATE(*) into AGGREGATE(1).
					 *
					 * This does the right thing for COUNT(*) (in fact,
					 * any certainly-non-null expression would do for COUNT),
					 * and there are no other aggregates in SQL92 that accept
					 * '*' as parameter.
					 *
					 * The FuncCall node is also marked agg_star = true,
					 * so that later processing can detect what the argument
					 * really was.
					 */
					FuncCall *n = makeNode(FuncCall);
					A_Const *star = makeNode(A_Const);

					star->val.type = T_Integer;
					star->val.val.ival = 1;
					n->funcname = yyvsp[-3].list;
					n->args = makeList1(star);
					n->agg_star = TRUE;
					n->agg_distinct = FALSE;
					yyval.node = (Node *)n;
				}
    break;

  case 1057:
#line 6682 "gram.y"
    {
					/*
					 * Translate as "'now'::text::date".
					 *
					 * We cannot use "'now'::date" because coerce_type() will
					 * immediately reduce that to a constant representing
					 * today's date.  We need to delay the conversion until
					 * runtime, else the wrong things will happen when
					 * CURRENT_DATE is used in a column default value or rule.
					 *
					 * This could be simplified if we had a way to generate
					 * an expression tree representing runtime application
					 * of type-input conversion functions...
					 */
					A_Const *s = makeNode(A_Const);
					TypeName *d;

					s->val.type = T_String;
					s->val.val.str = "now";
					s->typename = SystemTypeName("text");

					d = SystemTypeName("date");

					yyval.node = (Node *)makeTypeCast((Node *)s, d);
				}
    break;

  case 1058:
#line 6708 "gram.y"
    {
					/*
					 * Translate as "'now'::text::timetz".
					 * See comments for CURRENT_DATE.
					 */
					A_Const *s = makeNode(A_Const);
					TypeName *d;

					s->val.type = T_String;
					s->val.val.str = "now";
					s->typename = SystemTypeName("text");

					d = SystemTypeName("timetz");
					/* SQL99 mandates a default precision of zero for TIME
					 * fields in schemas. However, for CURRENT_TIME
					 * let's preserve the microsecond precision we
					 * might see from the system clock. - thomas 2001-12-07
					 */
					d->typmod = 6;

					yyval.node = (Node *)makeTypeCast((Node *)s, d);
				}
    break;

  case 1059:
#line 6731 "gram.y"
    {
					/*
					 * Translate as "'now'::text::timetz(n)".
					 * See comments for CURRENT_DATE.
					 */
					A_Const *s = makeNode(A_Const);
					TypeName *d;

					s->val.type = T_String;
					s->val.val.str = "now";
					s->typename = SystemTypeName("text");
					d = SystemTypeName("timetz");
					if ((yyvsp[-1].ival < 0) || (yyvsp[-1].ival > MAX_TIME_PRECISION))
						elog(ERROR,
						"CURRENT_TIME(%d) precision must be between %d and %d",
							 yyvsp[-1].ival, 0, MAX_TIME_PRECISION);
					d->typmod = yyvsp[-1].ival;

					yyval.node = (Node *)makeTypeCast((Node *)s, d);
				}
    break;

  case 1060:
#line 6752 "gram.y"
    {
					/*
					 * Translate as "'now'::text::timestamptz".
					 * See comments for CURRENT_DATE.
					 */
					A_Const *s = makeNode(A_Const);
					TypeName *d;

					s->val.type = T_String;
					s->val.val.str = "now";
					s->typename = SystemTypeName("text");

					d = SystemTypeName("timestamptz");
					/* SQL99 mandates a default precision of 6 for timestamp.
					 * Also, that is about as precise as we will get since
					 * we are using a microsecond time interface.
					 * - thomas 2001-12-07
					 */
					d->typmod = 6;

					yyval.node = (Node *)makeTypeCast((Node *)s, d);
				}
    break;

  case 1061:
#line 6775 "gram.y"
    {
					/*
					 * Translate as "'now'::text::timestamptz(n)".
					 * See comments for CURRENT_DATE.
					 */
					A_Const *s = makeNode(A_Const);
					TypeName *d;

					s->val.type = T_String;
					s->val.val.str = "now";
					s->typename = SystemTypeName("text");

					d = SystemTypeName("timestamptz");
					if ((yyvsp[-1].ival < 0) || (yyvsp[-1].ival > MAX_TIMESTAMP_PRECISION))
						elog(ERROR,
						"CURRENT_TIMESTAMP(%d) precision "
						"must be between %d and %d",
							 yyvsp[-1].ival, 0, MAX_TIMESTAMP_PRECISION);
					d->typmod = yyvsp[-1].ival;

					yyval.node = (Node *)makeTypeCast((Node *)s, d);
				}
    break;

  case 1062:
#line 6798 "gram.y"
    {
					/*
					 * Translate as "'now'::text::time".
					 * See comments for CURRENT_DATE.
					 */
					A_Const *s = makeNode(A_Const);
					TypeName *d;

					s->val.type = T_String;
					s->val.val.str = "now";
					s->typename = SystemTypeName("text");

					d = SystemTypeName("time");
					/* SQL99 mandates a default precision of zero for TIME
					 * fields in schemas. However, for LOCALTIME
					 * let's preserve the microsecond precision we
					 * might see from the system clock. - thomas 2001-12-07
					 */
					d->typmod = 6;

					yyval.node = (Node *)makeTypeCast((Node *)s, d);
				}
    break;

  case 1063:
#line 6821 "gram.y"
    {
					/*
					 * Translate as "'now'::text::time(n)".
					 * See comments for CURRENT_DATE.
					 */
					A_Const *s = makeNode(A_Const);
					TypeName *d;

					s->val.type = T_String;
					s->val.val.str = "now";
					s->typename = SystemTypeName("text");
					d = SystemTypeName("time");
					if ((yyvsp[-1].ival < 0) || (yyvsp[-1].ival > MAX_TIME_PRECISION))
						elog(ERROR,
						"LOCALTIME(%d) precision must be between %d and %d",
							 yyvsp[-1].ival, 0, MAX_TIME_PRECISION);
					d->typmod = yyvsp[-1].ival;

					yyval.node = (Node *)makeTypeCast((Node *)s, d);
				}
    break;

  case 1064:
#line 6842 "gram.y"
    {
					/*
					 * Translate as "'now'::text::timestamp".
					 * See comments for CURRENT_DATE.
					 */
					A_Const *s = makeNode(A_Const);
					TypeName *d;

					s->val.type = T_String;
					s->val.val.str = "now";
					s->typename = SystemTypeName("text");

					d = SystemTypeName("timestamp");
					/* SQL99 mandates a default precision of 6 for timestamp.
					 * Also, that is about as precise as we will get since
					 * we are using a microsecond time interface.
					 * - thomas 2001-12-07
					 */
					d->typmod = 6;

					yyval.node = (Node *)makeTypeCast((Node *)s, d);
				}
    break;

  case 1065:
#line 6865 "gram.y"
    {
					/*
					 * Translate as "'now'::text::timestamp(n)".
					 * See comments for CURRENT_DATE.
					 */
					A_Const *s = makeNode(A_Const);
					TypeName *d;

					s->val.type = T_String;
					s->val.val.str = "now";
					s->typename = SystemTypeName("text");

					d = SystemTypeName("timestamp");
					if ((yyvsp[-1].ival < 0) || (yyvsp[-1].ival > MAX_TIMESTAMP_PRECISION))
						elog(ERROR,
						"LOCALTIMESTAMP(%d) precision must be "
						"between %d and %d",
							 yyvsp[-1].ival, 0, MAX_TIMESTAMP_PRECISION);
					d->typmod = yyvsp[-1].ival;

					yyval.node = (Node *)makeTypeCast((Node *)s, d);
				}
    break;

  case 1066:
#line 6888 "gram.y"
    {
					FuncCall *n = makeNode(FuncCall);
					n->funcname = SystemFuncName("current_user");
					n->args = NIL;
					n->agg_star = FALSE;
					n->agg_distinct = FALSE;
					yyval.node = (Node *)n;
				}
    break;

  case 1067:
#line 6897 "gram.y"
    {
					FuncCall *n = makeNode(FuncCall);
					n->funcname = SystemFuncName("session_user");
					n->args = NIL;
					n->agg_star = FALSE;
					n->agg_distinct = FALSE;
					yyval.node = (Node *)n;
				}
    break;

  case 1068:
#line 6906 "gram.y"
    {
					FuncCall *n = makeNode(FuncCall);
					n->funcname = SystemFuncName("current_user");
					n->args = NIL;
					n->agg_star = FALSE;
					n->agg_distinct = FALSE;
					yyval.node = (Node *)n;
				}
    break;

  case 1069:
#line 6915 "gram.y"
    { yyval.node = makeTypeCast(yyvsp[-3].node, yyvsp[-1].typnam); }
    break;

  case 1070:
#line 6917 "gram.y"
    {
					FuncCall *n = makeNode(FuncCall);
					n->funcname = SystemFuncName("date_part");
					n->args = yyvsp[-1].list;
					n->agg_star = FALSE;
					n->agg_distinct = FALSE;
					yyval.node = (Node *)n;
				}
    break;

  case 1071:
#line 6926 "gram.y"
    {
					/* overlay(A PLACING B FROM C FOR D) is converted to
					 * substring(A, 1, C-1) || B || substring(A, C+1, C+D)
					 * overlay(A PLACING B FROM C) is converted to
					 * substring(A, 1, C-1) || B || substring(A, C+1, C+char_length(B))
					 */
					FuncCall *n = makeNode(FuncCall);
					n->funcname = SystemFuncName("overlay");
					n->args = yyvsp[-1].list;
					n->agg_star = FALSE;
					n->agg_distinct = FALSE;
					yyval.node = (Node *)n;
				}
    break;

  case 1072:
#line 6940 "gram.y"
    {
					/* position(A in B) is converted to position(B, A) */
					FuncCall *n = makeNode(FuncCall);
					n->funcname = SystemFuncName("position");
					n->args = yyvsp[-1].list;
					n->agg_star = FALSE;
					n->agg_distinct = FALSE;
					yyval.node = (Node *)n;
				}
    break;

  case 1073:
#line 6950 "gram.y"
    {
					/* substring(A from B for C) is converted to
					 * substring(A, B, C) - thomas 2000-11-28
					 */
					FuncCall *n = makeNode(FuncCall);
					n->funcname = SystemFuncName("substring");
					n->args = yyvsp[-1].list;
					n->agg_star = FALSE;
					n->agg_distinct = FALSE;
					yyval.node = (Node *)n;
				}
    break;

  case 1074:
#line 6962 "gram.y"
    {
					/* TREAT(expr AS target) converts expr of a particular type to target,
					 * which is defined to be a subtype of the original expression.
					 * In SQL99, this is intended for use with structured UDTs,
					 * but let's make this a generally useful form allowing stronger
					 * coersions than are handled by implicit casting.
					 */
					FuncCall *n = makeNode(FuncCall);
					/* Convert SystemTypeName() to SystemFuncName() even though
					 * at the moment they result in the same thing.
					 */
					n->funcname = SystemFuncName(((Value *)llast(yyvsp[-1].typnam->names))->val.str);
					n->args = makeList1(yyvsp[-3].node);
					yyval.node = (Node *)n;
				}
    break;

  case 1075:
#line 6978 "gram.y"
    {
					/* various trim expressions are defined in SQL92
					 * - thomas 1997-07-19
					 */
					FuncCall *n = makeNode(FuncCall);
					n->funcname = SystemFuncName("btrim");
					n->args = yyvsp[-1].list;
					n->agg_star = FALSE;
					n->agg_distinct = FALSE;
					yyval.node = (Node *)n;
				}
    break;

  case 1076:
#line 6990 "gram.y"
    {
					FuncCall *n = makeNode(FuncCall);
					n->funcname = SystemFuncName("ltrim");
					n->args = yyvsp[-1].list;
					n->agg_star = FALSE;
					n->agg_distinct = FALSE;
					yyval.node = (Node *)n;
				}
    break;

  case 1077:
#line 6999 "gram.y"
    {
					FuncCall *n = makeNode(FuncCall);
					n->funcname = SystemFuncName("rtrim");
					n->args = yyvsp[-1].list;
					n->agg_star = FALSE;
					n->agg_distinct = FALSE;
					yyval.node = (Node *)n;
				}
    break;

  case 1078:
#line 7008 "gram.y"
    {
					FuncCall *n = makeNode(FuncCall);
					n->funcname = SystemFuncName("btrim");
					n->args = yyvsp[-1].list;
					n->agg_star = FALSE;
					n->agg_distinct = FALSE;
					yyval.node = (Node *)n;
				}
    break;

  case 1079:
#line 7017 "gram.y"
    {
					FuncCall *n = makeNode(FuncCall);
					A_Const *c = makeNode(A_Const);

					c->val.type = T_String;
					c->val.val.str = NameListToQuotedString(yyvsp[-1].list);

					n->funcname = SystemFuncName("convert_using");
					n->args = makeList2(yyvsp[-3].node, c);
					n->agg_star = FALSE;
					n->agg_distinct = FALSE;
					yyval.node = (Node *)n;
				}
    break;

  case 1080:
#line 7031 "gram.y"
    {
					FuncCall *n = makeNode(FuncCall);
					n->funcname = SystemFuncName("convert");
					n->args = yyvsp[-1].list;
					n->agg_star = FALSE;
					n->agg_distinct = FALSE;
					yyval.node = (Node *)n;
				}
    break;

  case 1081:
#line 7040 "gram.y"
    {
					SubLink *n = makeNode(SubLink);
					n->lefthand = NIL;
					n->oper = NIL;
					n->useor = FALSE;
					n->subLinkType = EXPR_SUBLINK;
					n->subselect = yyvsp[0].node;
					yyval.node = (Node *)n;
				}
    break;

  case 1082:
#line 7050 "gram.y"
    {
					SubLink *n = makeNode(SubLink);
					n->lefthand = NIL;
					n->oper = NIL;
					n->useor = FALSE;
					n->subLinkType = EXISTS_SUBLINK;
					n->subselect = yyvsp[0].node;
					yyval.node = (Node *)n;
				}
    break;

  case 1083:
#line 7067 "gram.y"
    {
					A_Indices *ai = makeNode(A_Indices);
					ai->lidx = NULL;
					ai->uidx = yyvsp[-1].node;
					yyval.list = lappend(yyvsp[-3].list, ai);
				}
    break;

  case 1084:
#line 7074 "gram.y"
    {
					A_Indices *ai = makeNode(A_Indices);
					ai->lidx = yyvsp[-3].node;
					ai->uidx = yyvsp[-1].node;
					yyval.list = lappend(yyvsp[-5].list, ai);
				}
    break;

  case 1085:
#line 7081 "gram.y"
    { yyval.list = NIL; }
    break;

  case 1086:
#line 7085 "gram.y"
    { yyval.list = makeList1(yyvsp[0].node); }
    break;

  case 1087:
#line 7086 "gram.y"
    { yyval.list = lappend(yyvsp[-2].list, yyvsp[0].node); }
    break;

  case 1088:
#line 7097 "gram.y"
    {yyval.list = makeList1(makeString(yyvsp[0].str));}
    break;

  case 1089:
#line 7099 "gram.y"
    {
                                   yyval.list = lcons(makeString(yyvsp[-2].str), yyvsp[0].list);
				}
    break;

  case 1090:
#line 7108 "gram.y"
    {
					A_Const *n = makeNode(A_Const);
					n->val.type = T_String;
					n->val.val.str = yyvsp[-2].str;
					yyval.list = makeList2((Node *) n, yyvsp[0].node);
				}
    break;

  case 1091:
#line 7114 "gram.y"
    { yyval.list = NIL; }
    break;

  case 1092:
#line 7118 "gram.y"
    {
					yyval.list = lappend(yyvsp[-2].list, yyvsp[0].typnam);
				}
    break;

  case 1093:
#line 7122 "gram.y"
    {
					yyval.list = makeList1(yyvsp[0].typnam);
				}
    break;

  case 1094:
#line 7132 "gram.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1095:
#line 7133 "gram.y"
    { yyval.str = "year"; }
    break;

  case 1096:
#line 7134 "gram.y"
    { yyval.str = "month"; }
    break;

  case 1097:
#line 7135 "gram.y"
    { yyval.str = "day"; }
    break;

  case 1098:
#line 7136 "gram.y"
    { yyval.str = "hour"; }
    break;

  case 1099:
#line 7137 "gram.y"
    { yyval.str = "minute"; }
    break;

  case 1100:
#line 7138 "gram.y"
    { yyval.str = "second"; }
    break;

  case 1101:
#line 7139 "gram.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1102:
#line 7149 "gram.y"
    {
					yyval.list = makeList4(yyvsp[-3].node, yyvsp[-2].node, yyvsp[-1].node, yyvsp[0].node);
				}
    break;

  case 1103:
#line 7153 "gram.y"
    {
					yyval.list = makeList3(yyvsp[-2].node, yyvsp[-1].node, yyvsp[0].node);
				}
    break;

  case 1104:
#line 7160 "gram.y"
    { yyval.node = yyvsp[0].node; }
    break;

  case 1105:
#line 7166 "gram.y"
    { yyval.list = makeList2(yyvsp[0].node, yyvsp[-2].node); }
    break;

  case 1106:
#line 7167 "gram.y"
    { yyval.list = NIL; }
    break;

  case 1107:
#line 7183 "gram.y"
    {
					yyval.list = makeList3(yyvsp[-2].node, yyvsp[-1].node, yyvsp[0].node);
				}
    break;

  case 1108:
#line 7187 "gram.y"
    {
					yyval.list = makeList3(yyvsp[-2].node, yyvsp[0].node, yyvsp[-1].node);
				}
    break;

  case 1109:
#line 7191 "gram.y"
    {
					yyval.list = makeList2(yyvsp[-1].node, yyvsp[0].node);
				}
    break;

  case 1110:
#line 7195 "gram.y"
    {
					A_Const *n = makeNode(A_Const);
					n->val.type = T_Integer;
					n->val.val.ival = 1;
					yyval.list = makeList3(yyvsp[-1].node, (Node *)n, yyvsp[0].node);
				}
    break;

  case 1111:
#line 7202 "gram.y"
    {
					yyval.list = yyvsp[0].list;
				}
    break;

  case 1112:
#line 7206 "gram.y"
    { yyval.list = NIL; }
    break;

  case 1113:
#line 7210 "gram.y"
    { yyval.node = yyvsp[0].node; }
    break;

  case 1114:
#line 7213 "gram.y"
    { yyval.node = yyvsp[0].node; }
    break;

  case 1115:
#line 7216 "gram.y"
    { yyval.list = lappend(yyvsp[0].list, yyvsp[-2].node); }
    break;

  case 1116:
#line 7217 "gram.y"
    { yyval.list = yyvsp[0].list; }
    break;

  case 1117:
#line 7218 "gram.y"
    { yyval.list = yyvsp[0].list; }
    break;

  case 1118:
#line 7222 "gram.y"
    {
					SubLink *n = makeNode(SubLink);
					n->subselect = yyvsp[0].node;
					yyval.node = (Node *)n;
				}
    break;

  case 1119:
#line 7227 "gram.y"
    { yyval.node = (Node *)yyvsp[-1].list; }
    break;

  case 1120:
#line 7246 "gram.y"
    {
					CaseExpr *c = makeNode(CaseExpr);
					c->arg = yyvsp[-3].node;
					c->args = yyvsp[-2].list;
					c->defresult = yyvsp[-1].node;
					yyval.node = (Node *)c;
				}
    break;

  case 1121:
#line 7254 "gram.y"
    {
					CaseExpr *c = makeNode(CaseExpr);
					CaseWhen *w = makeNode(CaseWhen);

					w->expr = (Node *) makeSimpleA_Expr(OP, "=", yyvsp[-3].node, yyvsp[-1].node);
					/* w->result is left NULL */
					c->args = makeList1(w);
					c->defresult = yyvsp[-3].node;
					yyval.node = (Node *)c;
				}
    break;

  case 1122:
#line 7265 "gram.y"
    {
					CaseExpr *c = makeNode(CaseExpr);
					List *l;
					foreach (l,yyvsp[-1].list)
					{
						CaseWhen *w = makeNode(CaseWhen);
						NullTest *n = makeNode(NullTest);
						n->arg = lfirst(l);
						n->nulltesttype = IS_NOT_NULL;
						w->expr = (Node *) n;
						w->result = lfirst(l);
						c->args = lappend(c->args, w);
					}
					yyval.node = (Node *)c;
				}
    break;

  case 1123:
#line 7284 "gram.y"
    { yyval.list = makeList1(yyvsp[0].node); }
    break;

  case 1124:
#line 7285 "gram.y"
    { yyval.list = lappend(yyvsp[-1].list, yyvsp[0].node); }
    break;

  case 1125:
#line 7290 "gram.y"
    {
					CaseWhen *w = makeNode(CaseWhen);
					w->expr = yyvsp[-2].node;
					w->result = yyvsp[0].node;
					yyval.node = (Node *)w;
				}
    break;

  case 1126:
#line 7299 "gram.y"
    { yyval.node = yyvsp[0].node; }
    break;

  case 1127:
#line 7300 "gram.y"
    { yyval.node = NULL; }
    break;

  case 1128:
#line 7303 "gram.y"
    { yyval.node = yyvsp[0].node; }
    break;

  case 1129:
#line 7304 "gram.y"
    { yyval.node = NULL; }
    break;

  case 1130:
#line 7313 "gram.y"
    {
					yyval.columnref = makeNode(ColumnRef);
					yyval.columnref->fields = makeList1(makeString(yyvsp[-1].str));
					yyval.columnref->indirection = yyvsp[0].list;
				}
    break;

  case 1131:
#line 7319 "gram.y"
    {
					yyval.columnref = makeNode(ColumnRef);
					yyval.columnref->fields = yyvsp[-1].list;
					yyval.columnref->indirection = yyvsp[0].list;
				}
    break;

  case 1132:
#line 7328 "gram.y"
    { yyval.list = lcons(makeString(yyvsp[-1].str), yyvsp[0].list); }
    break;

  case 1133:
#line 7332 "gram.y"
    { yyval.list = makeList1(makeString(yyvsp[0].str)); }
    break;

  case 1134:
#line 7334 "gram.y"
    { yyval.list = makeList1(makeString("*")); }
    break;

  case 1135:
#line 7336 "gram.y"
    { yyval.list = lcons(makeString(yyvsp[-1].str), yyvsp[0].list); }
    break;

  case 1136:
#line 7349 "gram.y"
    { yyval.list = makeList1(yyvsp[0].target); }
    break;

  case 1137:
#line 7350 "gram.y"
    { yyval.list = lappend(yyvsp[-2].list, yyvsp[0].target); }
    break;

  case 1138:
#line 7355 "gram.y"
    {
					yyval.target = makeNode(ResTarget);
					yyval.target->name = yyvsp[0].str;
					yyval.target->indirection = NIL;
					yyval.target->val = (Node *)yyvsp[-2].node;
				}
    break;

  case 1139:
#line 7362 "gram.y"
    {
					yyval.target = makeNode(ResTarget);
					yyval.target->name = NULL;
					yyval.target->indirection = NIL;
					yyval.target->val = (Node *)yyvsp[0].node;
				}
    break;

  case 1140:
#line 7369 "gram.y"
    {
					ColumnRef *n = makeNode(ColumnRef);
					n->fields = makeList1(makeString("*"));
					n->indirection = NIL;
					yyval.target = makeNode(ResTarget);
					yyval.target->name = NULL;
					yyval.target->indirection = NIL;
					yyval.target->val = (Node *)n;
				}
    break;

  case 1141:
#line 7387 "gram.y"
    { yyval.list = makeList1(yyvsp[0].target); }
    break;

  case 1142:
#line 7388 "gram.y"
    { yyval.list = lappend(yyvsp[-2].list,yyvsp[0].target); }
    break;

  case 1143:
#line 7393 "gram.y"
    {
					yyval.target = makeNode(ResTarget);
					yyval.target->name = yyvsp[-3].str;
					yyval.target->indirection = yyvsp[-2].list;
					yyval.target->val = (Node *)yyvsp[0].node;
				}
    break;

  case 1144:
#line 7402 "gram.y"
    { yyval.list = makeList1(yyvsp[0].target); }
    break;

  case 1145:
#line 7403 "gram.y"
    { yyval.list = lappend(yyvsp[-2].list, yyvsp[0].target); }
    break;

  case 1146:
#line 7407 "gram.y"
    { yyval.target = yyvsp[0].target; }
    break;

  case 1147:
#line 7409 "gram.y"
    {
					InsertDefault *def = makeNode(InsertDefault);
					yyval.target = makeNode(ResTarget);
					yyval.target->name = NULL;
					yyval.target->indirection = NULL;
					yyval.target->val = (Node *)def;
				}
    break;

  case 1148:
#line 7426 "gram.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1149:
#line 7427 "gram.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1150:
#line 7431 "gram.y"
    { yyval.list = makeList1(yyvsp[0].range); }
    break;

  case 1151:
#line 7432 "gram.y"
    { yyval.list = lappend(yyvsp[-2].list, yyvsp[0].range); }
    break;

  case 1152:
#line 7437 "gram.y"
    {
					yyval.range = makeNode(RangeVar);
					yyval.range->catalogname = NULL;
					yyval.range->schemaname = NULL;
					yyval.range->relname = yyvsp[0].str;
				}
    break;

  case 1153:
#line 7444 "gram.y"
    {
					yyval.range = makeNode(RangeVar);
					switch (length(yyvsp[0].list))
					{
						case 2:
							yyval.range->catalogname = NULL;
							yyval.range->schemaname = strVal(lfirst(yyvsp[0].list));
							yyval.range->relname = strVal(lsecond(yyvsp[0].list));
							break;
						case 3:
							yyval.range->catalogname = strVal(lfirst(yyvsp[0].list));
							yyval.range->schemaname = strVal(lsecond(yyvsp[0].list));
							yyval.range->relname = strVal(lfirst(lnext(lnext(yyvsp[0].list))));
							break;
						default:
							elog(ERROR,
							"Improper qualified name "
							"(too many dotted names): %s",
								 NameListToString(yyvsp[0].list));
							break;
					}
				}
    break;

  case 1154:
#line 7469 "gram.y"
    { yyval.list = makeList1(makeString(yyvsp[0].str)); }
    break;

  case 1155:
#line 7471 "gram.y"
    { yyval.list = lappend(yyvsp[-2].list, makeString(yyvsp[0].str)); }
    break;

  case 1156:
#line 7475 "gram.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1157:
#line 7478 "gram.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1158:
#line 7481 "gram.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1159:
#line 7483 "gram.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1160:
#line 7485 "gram.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1161:
#line 7487 "gram.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1162:
#line 7490 "gram.y"
    { yyval.list = makeList1(makeString(yyvsp[0].str)); }
    break;

  case 1163:
#line 7491 "gram.y"
    { yyval.list = yyvsp[0].list; }
    break;

  case 1164:
#line 7494 "gram.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1165:
#line 7501 "gram.y"
    {
					A_Const *n = makeNode(A_Const);
					n->val.type = T_Integer;
					n->val.val.ival = yyvsp[0].ival;
					yyval.node = (Node *)n;
				}
    break;

  case 1166:
#line 7508 "gram.y"
    {
					A_Const *n = makeNode(A_Const);
					n->val.type = T_Float;
					n->val.val.str = yyvsp[0].str;
					yyval.node = (Node *)n;
				}
    break;

  case 1167:
#line 7515 "gram.y"
    {
					A_Const *n = makeNode(A_Const);
					n->val.type = T_String;
					n->val.val.str = yyvsp[0].str;
					yyval.node = (Node *)n;
				}
    break;

  case 1168:
#line 7522 "gram.y"
    {
					A_Const *n = makeNode(A_Const);
					n->val.type = T_BitString;
					n->val.val.str = yyvsp[0].str;
					yyval.node = (Node *)n;
				}
    break;

  case 1169:
#line 7529 "gram.y"
    {
					/* This is a bit constant per SQL99:
					 * Without Feature F511, "BIT data type",
					 * a <general literal> shall not be a
					 * <bit string literal> or a <hex string literal>.
					 */
					A_Const *n = makeNode(A_Const);
					n->val.type = T_BitString;
					n->val.val.str = yyvsp[0].str;
					yyval.node = (Node *)n;
				}
    break;

  case 1170:
#line 7541 "gram.y"
    {
					A_Const *n = makeNode(A_Const);
					n->typename = yyvsp[-1].typnam;
					n->val.type = T_String;
					n->val.val.str = yyvsp[0].str;
					yyval.node = (Node *)n;
				}
    break;

  case 1171:
#line 7549 "gram.y"
    {
					A_Const *n = makeNode(A_Const);
					n->typename = yyvsp[-2].typnam;
					n->val.type = T_String;
					n->val.val.str = yyvsp[-1].str;
					/* precision is not specified, but fields may be... */
					if (yyvsp[0].ival != INTERVAL_FULL_RANGE)
						n->typename->typmod = INTERVAL_TYPMOD(INTERVAL_FULL_PRECISION, yyvsp[0].ival);
					yyval.node = (Node *)n;
				}
    break;

  case 1172:
#line 7560 "gram.y"
    {
					A_Const *n = makeNode(A_Const);
					n->typename = yyvsp[-5].typnam;
					n->val.type = T_String;
					n->val.val.str = yyvsp[-1].str;
					/* precision specified, and fields may be... */
					if ((yyvsp[-3].ival < 0) || (yyvsp[-3].ival > MAX_INTERVAL_PRECISION))
						elog(ERROR,
						"INTERVAL(%d) precision must be between %d and %d",
							 yyvsp[-3].ival, 0, MAX_INTERVAL_PRECISION);
					n->typename->typmod = INTERVAL_TYPMOD(yyvsp[-3].ival, yyvsp[0].ival);
					yyval.node = (Node *)n;
				}
    break;

  case 1173:
#line 7574 "gram.y"
    {
					ParamRef *n = makeNode(ParamRef);
					n->number = yyvsp[-1].ival;
					n->fields = NIL;
					n->indirection = yyvsp[0].list;
					yyval.node = (Node *)n;
				}
    break;

  case 1174:
#line 7582 "gram.y"
    {
					yyval.node = (Node *)makeBoolConst(TRUE);
				}
    break;

  case 1175:
#line 7586 "gram.y"
    {
					yyval.node = (Node *)makeBoolConst(FALSE);
				}
    break;

  case 1176:
#line 7590 "gram.y"
    {
					A_Const *n = makeNode(A_Const);
					n->val.type = T_Null;
					yyval.node = (Node *)n;
				}
    break;

  case 1177:
#line 7597 "gram.y"
    { yyval.ival = yyvsp[0].ival; }
    break;

  case 1178:
#line 7598 "gram.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1179:
#line 7599 "gram.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1180:
#line 7614 "gram.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1181:
#line 7615 "gram.y"
    { yyval.str = pstrdup(yyvsp[0].keyword); }
    break;

  case 1182:
#line 7616 "gram.y"
    { yyval.str = pstrdup(yyvsp[0].keyword); }
    break;

  case 1183:
#line 7621 "gram.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1184:
#line 7622 "gram.y"
    { yyval.str = pstrdup(yyvsp[0].keyword); }
    break;

  case 1185:
#line 7628 "gram.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1186:
#line 7629 "gram.y"
    { yyval.str = pstrdup(yyvsp[0].keyword); }
    break;

  case 1187:
#line 7630 "gram.y"
    { yyval.str = pstrdup(yyvsp[0].keyword); }
    break;

  case 1188:
#line 7636 "gram.y"
    { yyval.str = yyvsp[0].str; }
    break;

  case 1189:
#line 7637 "gram.y"
    { yyval.str = pstrdup(yyvsp[0].keyword); }
    break;

  case 1190:
#line 7638 "gram.y"
    { yyval.str = pstrdup(yyvsp[0].keyword); }
    break;

  case 1191:
#line 7639 "gram.y"
    { yyval.str = pstrdup(yyvsp[0].keyword); }
    break;

  case 1192:
#line 7640 "gram.y"
    { yyval.str = pstrdup(yyvsp[0].keyword); }
    break;

  case 1489:
#line 7992 "gram.y"
    {
					if (QueryIsRule)
						yyval.str = "*OLD*";
					else
						elog(ERROR, "OLD used in non-rule query");
				}
    break;

  case 1490:
#line 7999 "gram.y"
    {
					if (QueryIsRule)
						yyval.str = "*NEW*";
					else
						elog(ERROR, "NEW used in non-rule query");
				}
    break;


    }

/* Line 999 of yacc.c.  */
#line 18930 "y.tab.c"

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
	  yysize += yystrlen ("parse error, unexpected ") + 1;
	  yysize += yystrlen (yytname[yytype]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "parse error, unexpected ");
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
	    yyerror ("parse error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("parse error");
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


#line 8007 "gram.y"


static Node *
makeTypeCast(Node *arg, TypeName *typename)
{
	/*
	 * Simply generate a TypeCast node.
	 *
	 * Earlier we would determine whether an A_Const would
	 * be acceptable, however Domains require coerce_type()
	 * to process them -- applying constraints as required.
	 */
	TypeCast *n = makeNode(TypeCast);
	n->arg = arg;
	n->typename = typename;
	return (Node *) n;
}

static Node *
makeStringConst(char *str, TypeName *typename)
{
	A_Const *n = makeNode(A_Const);

	n->val.type = T_String;
	n->val.val.str = str;
	n->typename = typename;

	return (Node *)n;
}

static Node *
makeIntConst(int val)
{
	A_Const *n = makeNode(A_Const);
	n->val.type = T_Integer;
	n->val.val.ival = val;
	n->typename = SystemTypeName("int4");

	return (Node *)n;
}

static Node *
makeFloatConst(char *str)
{
	A_Const *n = makeNode(A_Const);

	n->val.type = T_Float;
	n->val.val.str = str;
	n->typename = SystemTypeName("float8");

	return (Node *)n;
}

static Node *
makeAConst(Value *v)
{
	Node *n;

	switch (v->type)
	{
		case T_Float:
			n = makeFloatConst(v->val.str);
			break;

		case T_Integer:
			n = makeIntConst(v->val.ival);
			break;

		case T_String:
		default:
			n = makeStringConst(v->val.str, NULL);
			break;
	}

	return n;
}

/* makeDefElem()
 * Create a DefElem node and set contents.
 * Could be moved to nodes/makefuncs.c if this is useful elsewhere.
 */
static DefElem *
makeDefElem(char *name, Node *arg)
{
	DefElem *f = makeNode(DefElem);
	f->defname = name;
	f->arg = arg;
	return f;
}

/* makeBoolConst()
 * Create an A_Const node and initialize to a boolean constant.
 */
static A_Const *
makeBoolConst(bool state)
{
	A_Const *n = makeNode(A_Const);
	n->val.type = T_String;
	n->val.val.str = (state? "t": "f");
	n->typename = SystemTypeName("bool");
	return n;
}

/* makeRowExpr()
 * Generate separate operator nodes for a single row descriptor expression.
 * Perhaps this should go deeper in the parser someday...
 * - thomas 1997-12-22
 */
static Node *
makeRowExpr(List *opr, List *largs, List *rargs)
{
	Node *expr = NULL;
	Node *larg, *rarg;
	char *oprname;

	if (length(largs) != length(rargs))
		elog(ERROR, "Unequal number of entries in row expression");

	if (lnext(largs) != NIL)
		expr = makeRowExpr(opr, lnext(largs), lnext(rargs));

	larg = lfirst(largs);
	rarg = lfirst(rargs);

	oprname = strVal(llast(opr));

	if ((strcmp(oprname, "=") == 0) ||
		(strcmp(oprname, "<") == 0) ||
		(strcmp(oprname, "<=") == 0) ||
		(strcmp(oprname, ">") == 0) ||
		(strcmp(oprname, ">=") == 0))
	{
		if (expr == NULL)
			expr = (Node *) makeA_Expr(OP, opr, larg, rarg);
		else
			expr = (Node *) makeA_Expr(AND, NIL, expr,
									   (Node *) makeA_Expr(OP, opr,
														   larg, rarg));
	}
	else if (strcmp(oprname, "<>") == 0)
	{
		if (expr == NULL)
			expr = (Node *) makeA_Expr(OP, opr, larg, rarg);
		else
			expr = (Node *) makeA_Expr(OR, NIL, expr,
									   (Node *) makeA_Expr(OP, opr,
														   larg, rarg));
	}
	else
	{
		elog(ERROR, "Operator '%s' not implemented for row expressions",
			 oprname);
	}

	return expr;
}

/* makeDistinctExpr()
 * Generate separate operator nodes for a single row descriptor expression.
 * Same comments as for makeRowExpr().
 */
static Node *
makeDistinctExpr(List *largs, List *rargs)
{
	Node *expr = NULL;
	Node *larg, *rarg;

	if (length(largs) != length(rargs))
		elog(ERROR, "Unequal number of entries in row expression");

	if (lnext(largs) != NIL)
		expr = makeDistinctExpr(lnext(largs), lnext(rargs));

	larg = lfirst(largs);
	rarg = lfirst(rargs);

	if (expr == NULL)
		expr = (Node *) makeSimpleA_Expr(DISTINCT, "=", larg, rarg);
	else
		expr = (Node *) makeA_Expr(OR, NIL, expr,
								   (Node *) makeSimpleA_Expr(DISTINCT, "=",
															 larg, rarg));

	return expr;
}

/* makeRowNullTest()
 * Generate separate operator nodes for a single row descriptor test.
 */
static Node *
makeRowNullTest(NullTestType test, List *args)
{
	Node *expr = NULL;
	Node *arg;
	NullTest *n;

	if (lnext(args) != NIL)
		expr = makeRowNullTest(test, lnext(args));

	arg = lfirst(args);

	n = makeNode(NullTest);
	n->arg = arg;
	n->nulltesttype = test;

	if (expr == NULL)
		expr = (Node *)n;
	else if (test == IS_NOT_NULL)
		expr = (Node *) makeA_Expr(OR, NIL, expr, (Node *)n);
	else
		expr = (Node *) makeA_Expr(AND, NIL, expr, (Node *)n);

	return expr;
}

/* makeOverlaps()
 * Create and populate a FuncCall node to support the OVERLAPS operator.
 */
static FuncCall *
makeOverlaps(List *largs, List *rargs)
{
	FuncCall *n = makeNode(FuncCall);
	n->funcname = SystemFuncName("overlaps");
	if (length(largs) == 1)
		largs = lappend(largs, largs);
	else if (length(largs) != 2)
		elog(ERROR, "Wrong number of parameters"
			 " on left side of OVERLAPS expression");
	if (length(rargs) == 1)
		rargs = lappend(rargs, rargs);
	else if (length(rargs) != 2)
		elog(ERROR, "Wrong number of parameters"
			 " on right side of OVERLAPS expression");
	n->args = nconc(largs, rargs);
	n->agg_star = FALSE;
	n->agg_distinct = FALSE;
	return n;
}

/* findLeftmostSelect()
 * Find the leftmost component SelectStmt in a set-operation parsetree.
 */
static SelectStmt *
findLeftmostSelect(SelectStmt *node)
{
	while (node && node->op != SETOP_NONE)
		node = node->larg;
	Assert(node && IsA(node, SelectStmt) && node->larg == NULL);
	return node;
}

/* insertSelectOptions()
 * Insert ORDER BY, etc into an already-constructed SelectStmt.
 *
 * This routine is just to avoid duplicating code in SelectStmt productions.
 */
static void
insertSelectOptions(SelectStmt *stmt,
					List *sortClause, List *forUpdate,
					Node *limitOffset, Node *limitCount)
{
	/*
	 * Tests here are to reject constructs like
	 *	(SELECT foo ORDER BY bar) ORDER BY baz
	 */
	if (sortClause)
	{
		if (stmt->sortClause)
			elog(ERROR, "Multiple ORDER BY clauses not allowed");
		stmt->sortClause = sortClause;
	}
	if (forUpdate)
	{
		if (stmt->forUpdate)
			elog(ERROR, "Multiple FOR UPDATE clauses not allowed");
		stmt->forUpdate = forUpdate;
	}
	if (limitOffset)
	{
		if (stmt->limitOffset)
			elog(ERROR, "Multiple OFFSET clauses not allowed");
		stmt->limitOffset = limitOffset;
	}
	if (limitCount)
	{
		if (stmt->limitCount)
			elog(ERROR, "Multiple LIMIT clauses not allowed");
		stmt->limitCount = limitCount;
	}
}

static Node *
makeSetOp(SetOperation op, bool all, Node *larg, Node *rarg)
{
	SelectStmt *n = makeNode(SelectStmt);

	n->op = op;
	n->all = all;
	n->larg = (SelectStmt *) larg;
	n->rarg = (SelectStmt *) rarg;
	return (Node *) n;
}

/* SystemFuncName()
 * Build a properly-qualified reference to a built-in function.
 */
List *
SystemFuncName(char *name)
{
	return makeList2(makeString("pg_catalog"), makeString(name));
}

/* SystemTypeName()
 * Build a properly-qualified reference to a built-in type.
 *
 * typmod is defaulted, but may be changed afterwards by caller.
 */
TypeName *
SystemTypeName(char *name)
{
	TypeName   *n = makeNode(TypeName);

	n->names = makeList2(makeString("pg_catalog"), makeString(name));
	n->typmod = -1;
	return n;
}

/* parser_init()
 * Initialize to parse one query string
 */
void
parser_init(void)
{
	QueryIsRule = FALSE;
}

/* exprIsNullConstant()
 * Test whether an a_expr is a plain NULL constant or not.
 */
bool
exprIsNullConstant(Node *arg)
{
	if (arg && IsA(arg, A_Const))
	{
		A_Const *con = (A_Const *) arg;

		if (con->val.type == T_Null &&
			con->typename == NULL)
			return TRUE;
	}
	return FALSE;
}

/* doNegate()
 * Handle negation of a numeric constant.
 *
 * Formerly, we did this here because the optimizer couldn't cope with
 * indexquals that looked like "var = -4" --- it wants "var = const"
 * and a unary minus operator applied to a constant didn't qualify.
 * As of Postgres 7.0, that problem doesn't exist anymore because there
 * is a constant-subexpression simplifier in the optimizer.  However,
 * there's still a good reason for doing this here, which is that we can
 * postpone committing to a particular internal representation for simple
 * negative constants.	It's better to leave "-123.456" in string form
 * until we know what the desired type is.
 */
static Node *
doNegate(Node *n)
{
	if (IsA(n, A_Const))
	{
		A_Const *con = (A_Const *)n;

		if (con->val.type == T_Integer)
		{
			con->val.val.ival = -con->val.val.ival;
			return n;
		}
		if (con->val.type == T_Float)
		{
			doNegateFloat(&con->val);
			return n;
		}
	}

	return (Node *) makeSimpleA_Expr(OP, "-", NULL, n);
}

static void
doNegateFloat(Value *v)
{
	char   *oldval = v->val.str;

	Assert(IsA(v, Float));
	if (*oldval == '+')
		oldval++;
	if (*oldval == '-')
		v->val.str = oldval+1;	/* just strip the '-' */
	else
	{
		char   *newval = (char *) palloc(strlen(oldval) + 2);

		*newval = '-';
		strcpy(newval+1, oldval);
		v->val.str = newval;
	}
}

#include "scan.c"

