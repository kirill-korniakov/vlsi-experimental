/* A Bison parser, made by GNU Bison 2.1.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005 Free Software Foundation, Inc.

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
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

#include "LEF/lex.h"

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0

/* Substitute the variable and function names.  */
#define yyparse lefyyparse
#define yylex   lefyylex
#define yyerror lefyyerror
#define yylval  lefyylval
#define yychar  lefyychar
#define yydebug lefyydebug
#define yynerrs lefyynerrs


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     K_HISTORY = 258,
     K_ABUT = 259,
     K_ABUTMENT = 260,
     K_ACTIVE = 261,
     K_ANALOG = 262,
     K_ARRAY = 263,
     K_AREA = 264,
     K_BLOCK = 265,
     K_BOTTOMLEFT = 266,
     K_BOTTOMRIGHT = 267,
     K_BY = 268,
     K_CAPACITANCE = 269,
     K_CAPMULTIPLIER = 270,
     K_CLASS = 271,
     K_CLOCK = 272,
     K_CLOCKTYPE = 273,
     K_COLUMNMAJOR = 274,
     K_DESIGNRULEWIDTH = 275,
     K_INFLUENCE = 276,
     K_CORE = 277,
     K_CORNER = 278,
     K_COVER = 279,
     K_CPERSQDIST = 280,
     K_CURRENT = 281,
     K_CURRENTSOURCE = 282,
     K_CUT = 283,
     K_DEFAULT = 284,
     K_DATABASE = 285,
     K_DATA = 286,
     K_DIELECTRIC = 287,
     K_DIRECTION = 288,
     K_DO = 289,
     K_EDGECAPACITANCE = 290,
     K_EEQ = 291,
     K_END = 292,
     K_ENDCAP = 293,
     K_FALL = 294,
     K_FALLCS = 295,
     K_FALLT0 = 296,
     K_FALLSATT1 = 297,
     K_FALLRS = 298,
     K_FALLSATCUR = 299,
     K_FALLTHRESH = 300,
     K_FEEDTHRU = 301,
     K_FIXED = 302,
     K_FOREIGN = 303,
     K_FROMPIN = 304,
     K_GENERATE = 305,
     K_GENERATOR = 306,
     K_GROUND = 307,
     K_HEIGHT = 308,
     K_HORIZONTAL = 309,
     K_INOUT = 310,
     K_INPUT = 311,
     K_INPUTNOISEMARGIN = 312,
     K_COMPONENTPIN = 313,
     K_INTRINSIC = 314,
     K_INVERT = 315,
     K_IRDROP = 316,
     K_ITERATE = 317,
     K_IV_TABLES = 318,
     K_LAYER = 319,
     K_LEAKAGE = 320,
     K_LEQ = 321,
     K_LIBRARY = 322,
     K_MACRO = 323,
     K_MATCH = 324,
     K_MAXDELAY = 325,
     K_MAXLOAD = 326,
     K_METALOVERHANG = 327,
     K_MILLIAMPS = 328,
     K_MILLIWATTS = 329,
     K_MINFEATURE = 330,
     K_MUSTJOIN = 331,
     K_NAMESCASESENSITIVE = 332,
     K_NANOSECONDS = 333,
     K_NETS = 334,
     K_NEW = 335,
     K_NONDEFAULTRULE = 336,
     K_NONINVERT = 337,
     K_NONUNATE = 338,
     K_OBS = 339,
     K_OHMS = 340,
     K_OFFSET = 341,
     K_ORIENTATION = 342,
     K_ORIGIN = 343,
     K_OUTPUT = 344,
     K_OUTPUTNOISEMARGIN = 345,
     K_OUTPUTRESISTANCE = 346,
     K_OVERHANG = 347,
     K_OVERLAP = 348,
     K_OFF = 349,
     K_ON = 350,
     K_OVERLAPS = 351,
     K_PAD = 352,
     K_PATH = 353,
     K_PATTERN = 354,
     K_PICOFARADS = 355,
     K_PIN = 356,
     K_PITCH = 357,
     K_PLACED = 358,
     K_POLYGON = 359,
     K_PORT = 360,
     K_POST = 361,
     K_POWER = 362,
     K_PRE = 363,
     K_PULLDOWNRES = 364,
     K_RECT = 365,
     K_RESISTANCE = 366,
     K_RESISTIVE = 367,
     K_RING = 368,
     K_RISE = 369,
     K_RISECS = 370,
     K_RISERS = 371,
     K_RISESATCUR = 372,
     K_RISETHRESH = 373,
     K_RISESATT1 = 374,
     K_RISET0 = 375,
     K_RISEVOLTAGETHRESHOLD = 376,
     K_FALLVOLTAGETHRESHOLD = 377,
     K_ROUTING = 378,
     K_ROWMAJOR = 379,
     K_RPERSQ = 380,
     K_SAMENET = 381,
     K_SCANUSE = 382,
     K_SHAPE = 383,
     K_SHRINKAGE = 384,
     K_SIGNAL = 385,
     K_SITE = 386,
     K_SIZE = 387,
     K_SOURCE = 388,
     K_SPACER = 389,
     K_SPACING = 390,
     K_SPECIALNETS = 391,
     K_STACK = 392,
     K_START = 393,
     K_STEP = 394,
     K_STOP = 395,
     K_STRUCTURE = 396,
     K_SYMMETRY = 397,
     K_TABLE = 398,
     K_THICKNESS = 399,
     K_TIEHIGH = 400,
     K_TIELOW = 401,
     K_TIEOFFR = 402,
     K_TIME = 403,
     K_TIMING = 404,
     K_TO = 405,
     K_TOPIN = 406,
     K_TOPLEFT = 407,
     K_TOPRIGHT = 408,
     K_TOPOFSTACKONLY = 409,
     K_TRISTATE = 410,
     K_TYPE = 411,
     K_UNATENESS = 412,
     K_UNITS = 413,
     K_USE = 414,
     K_VARIABLE = 415,
     K_VERTICAL = 416,
     K_VHI = 417,
     K_VIA = 418,
     K_VIARULE = 419,
     K_VLO = 420,
     K_VOLTAGE = 421,
     K_VOLTS = 422,
     K_WIDTH = 423,
     K_X = 424,
     K_Y = 425,
     K_R90 = 426,
     T_STRING = 427,
     QSTRING = 428,
     NUMBER = 429,
     K_N = 430,
     K_S = 431,
     K_E = 432,
     K_W = 433,
     K_FN = 434,
     K_FS = 435,
     K_FE = 436,
     K_FW = 437,
     K_R0 = 438,
     K_R180 = 439,
     K_R270 = 440,
     K_MX = 441,
     K_MY = 442,
     K_MXR90 = 443,
     K_MYR90 = 444,
     K_USER = 445,
     K_MASTERSLICE = 446,
     K_ENDMACRO = 447,
     K_ENDMACROPIN = 448,
     K_ENDVIARULE = 449,
     K_ENDVIA = 450,
     K_ENDLAYER = 451,
     K_ENDSITE = 452,
     K_CANPLACE = 453,
     K_CANNOTOCCUPY = 454,
     K_TRACKS = 455,
     K_FLOORPLAN = 456,
     K_GCELLGRID = 457,
     K_DEFAULTCAP = 458,
     K_MINPINS = 459,
     K_WIRECAP = 460,
     K_STABLE = 461,
     K_SETUP = 462,
     K_HOLD = 463,
     K_DEFINE = 464,
     K_DEFINES = 465,
     K_DEFINEB = 466,
     K_IF = 467,
     K_THEN = 468,
     K_ELSE = 469,
     K_FALSE = 470,
     K_TRUE = 471,
     K_EQ = 472,
     K_NE = 473,
     K_LE = 474,
     K_LT = 475,
     K_GE = 476,
     K_GT = 477,
     K_OR = 478,
     K_AND = 479,
     K_NOT = 480,
     K_DELAY = 481,
     K_TABLEDIMENSION = 482,
     K_TABLEAXIS = 483,
     K_TABLEENTRIES = 484,
     K_TRANSITIONTIME = 485,
     K_EXTENSION = 486,
     K_PROPDEF = 487,
     K_STRING = 488,
     K_INTEGER = 489,
     K_REAL = 490,
     K_RANGE = 491,
     K_PROPERTY = 492,
     K_VIRTUAL = 493,
     K_BUSBITCHARS = 494,
     K_VERSION = 495,
     K_BEGINEXT = 496,
     K_ENDEXT = 497,
     K_UNIVERSALNOISEMARGIN = 498,
     K_EDGERATETHRESHOLD1 = 499,
     K_CORRECTIONTABLE = 500,
     K_EDGERATESCALEFACTOR = 501,
     K_EDGERATETHRESHOLD2 = 502,
     K_VICTIMNOISE = 503,
     K_NOISETABLE = 504,
     K_EDGERATE = 505,
     K_VICTIMLENGTH = 506,
     K_CORRECTIONFACTOR = 507,
     K_OUTPUTPINANTENNASIZE = 508,
     K_INPUTPINANTENNASIZE = 509,
     K_INOUTPINANTENNASIZE = 510,
     K_CURRENTDEN = 511,
     K_PWL = 512,
     K_ANTENNALENGTHFACTOR = 513,
     K_TAPERRULE = 514,
     K_DIVIDERCHAR = 515,
     K_ANTENNASIZE = 516,
     K_ANTENNAMETALLENGTH = 517,
     K_ANTENNAMETALAREA = 518,
     K_RISESLEWLIMIT = 519,
     K_FALLSLEWLIMIT = 520,
     K_FUNCTION = 521,
     K_BUFFER = 522,
     K_INVERTER = 523,
     K_NAMEMAPSTRING = 524,
     K_NOWIREEXTENSIONATPIN = 525,
     K_WIREEXTENSION = 526,
     K_MESSAGE = 527,
     K_CREATEFILE = 528,
     K_OPENFILE = 529,
     K_CLOSEFILE = 530,
     K_WARNING = 531,
     K_ERROR = 532,
     K_FATALERROR = 533,
     K_RECOVERY = 534,
     K_SKEW = 535,
     K_ANYEDGE = 536,
     K_POSEDGE = 537,
     K_NEGEDGE = 538,
     K_SDFCONDSTART = 539,
     K_SDFCONDEND = 540,
     K_SDFCOND = 541,
     K_MPWH = 542,
     K_MPWL = 543,
     K_PERIOD = 544,
     K_ACCURRENTDENSITY = 545,
     K_DCCURRENTDENSITY = 546,
     K_AVERAGE = 547,
     K_PEAK = 548,
     K_RMS = 549,
     K_FREQUENCY = 550,
     K_CUTAREA = 551,
     K_MEGAHERTZ = 552,
     K_USELENGTHTHRESHOLD = 553,
     K_LENGTHTHRESHOLD = 554,
     K_ANTENNAINPUTGATEAREA = 555,
     K_ANTENNAINOUTDIFFAREA = 556,
     K_ANTENNAOUTPUTDIFFAREA = 557,
     K_ANTENNAAREARATIO = 558,
     K_ANTENNADIFFAREARATIO = 559,
     K_ANTENNACUMAREARATIO = 560,
     K_ANTENNACUMDIFFAREARATIO = 561,
     K_ANTENNAAREAFACTOR = 562,
     K_ANTENNASIDEAREARATIO = 563,
     K_ANTENNADIFFSIDEAREARATIO = 564,
     K_ANTENNACUMSIDEAREARATIO = 565,
     K_ANTENNACUMDIFFSIDEAREARATIO = 566,
     K_ANTENNASIDEAREAFACTOR = 567,
     K_DIFFUSEONLY = 568,
     K_MANUFACTURINGGRID = 569,
     K_ANTENNACELL = 570,
     K_CLEARANCEMEASURE = 571,
     K_EUCLIDEAN = 572,
     K_MAXXY = 573,
     K_USEMINSPACING = 574,
     K_ROWMINSPACING = 575,
     K_ROWABUTSPACING = 576,
     K_FLIP = 577,
     K_NONE = 578,
     K_ANTENNAPARTIALMETALAREA = 579,
     K_ANTENNAPARTIALMETALSIDEAREA = 580,
     K_ANTENNAGATEAREA = 581,
     K_ANTENNADIFFAREA = 582,
     K_ANTENNAMAXAREACAR = 583,
     K_ANTENNAMAXSIDEAREACAR = 584,
     K_ANTENNAPARTIALCUTAREA = 585,
     K_ANTENNAMAXCUTCAR = 586,
     K_SLOTWIREWIDTH = 587,
     K_SLOTWIRELENGTH = 588,
     K_SLOTWIDTH = 589,
     K_SLOTLENGTH = 590,
     K_MAXADJACENTSLOTSPACING = 591,
     K_MAXCOAXIALSLOTSPACING = 592,
     K_MAXEDGESLOTSPACING = 593,
     K_SPLITWIREWIDTH = 594,
     K_MINIMUMDENSITY = 595,
     K_MAXIMUMDENSITY = 596,
     K_DENSITYCHECKWINDOW = 597,
     K_DENSITYCHECKSTEP = 598,
     K_FILLACTIVESPACING = 599,
     K_MINIMUMCUT = 600,
     K_ADJACENTCUTS = 601,
     K_ANTENNAMODEL = 602,
     K_BUMP = 603,
     K_ENCLOSURE = 604,
     K_FROMABOVE = 605,
     K_FROMBELOW = 606,
     K_IMPLANT = 607,
     K_LENGTH = 608,
     K_MAXVIASTACK = 609,
     K_AREAIO = 610,
     K_BLACKBOX = 611,
     K_MAXWIDTH = 612,
     K_MINENCLOSEDAREA = 613,
     K_MINSTEP = 614,
     K_ORIENT = 615,
     K_OXIDE1 = 616,
     K_OXIDE2 = 617,
     K_OXIDE3 = 618,
     K_OXIDE4 = 619,
     K_PARALLELRUNLENGTH = 620,
     K_MINWIDTH = 621,
     K_PROTRUSIONWIDTH = 622,
     K_SPACINGTABLE = 623,
     K_WITHIN = 624,
     K_ABOVE = 625,
     K_BELOW = 626,
     K_CENTERTOCENTER = 627,
     K_CUTSIZE = 628,
     K_CUTSPACING = 629,
     K_DENSITY = 630,
     K_DIAG45 = 631,
     K_DIAG135 = 632,
     K_DIAGMINEDGELENGTH = 633,
     K_DIAGSPACING = 634,
     K_DIAGPITCH = 635,
     K_DIAGWIDTH = 636,
     K_GENERATED = 637,
     K_GROUNDSENSITIVITY = 638,
     K_HARDSPACING = 639,
     K_INSIDECORNER = 640,
     K_LAYERS = 641,
     K_LENGTHSUM = 642,
     K_MICRONS = 643,
     K_MINCUTS = 644,
     K_MINSIZE = 645,
     K_NETEXPR = 646,
     K_OUTSIDECORNER = 647,
     K_PREFERENCLOSURE = 648,
     K_ROWCOL = 649,
     K_ROWPATTERN = 650,
     K_SOFT = 651,
     K_SUPPLYSENSITIVITY = 652,
     K_USEVIA = 653,
     K_USEVIARULE = 654,
     K_WELLTAP = 655,
     K_ARRAYCUTS = 656,
     K_ARRAYSPACING = 657,
     K_ANTENNAAREADIFFREDUCEPWL = 658,
     K_ANTENNAAREAMINUSDIFF = 659,
     K_ANTENNACUMROUTINGPLUSCUT = 660,
     K_ANTENNAGATEPLUSDIFF = 661,
     K_ENDOFLINE = 662,
     K_ENDOFNOTCHWIDTH = 663,
     K_EXCEPTEXTRACUT = 664,
     K_EXCEPTSAMEPGNET = 665,
     K_EXCEPTPGNET = 666,
     K_LONGARRAY = 667,
     K_MAXEDGES = 668,
     K_NOTCHLENGTH = 669,
     K_NOTCHSPACING = 670,
     K_ORTHOGONAL = 671,
     K_PARALLELEDGE = 672,
     K_PARALLELOVERLAP = 673,
     K_PGONLY = 674,
     K_PRL = 675,
     K_TWOEDGES = 676,
     K_TWOWIDTHS = 677,
     IF = 678,
     LNOT = 679,
     UMINUS = 680
   };
#endif
/* Tokens.  */
#define K_HISTORY 258
#define K_ABUT 259
#define K_ABUTMENT 260
#define K_ACTIVE 261
#define K_ANALOG 262
#define K_ARRAY 263
#define K_AREA 264
#define K_BLOCK 265
#define K_BOTTOMLEFT 266
#define K_BOTTOMRIGHT 267
#define K_BY 268
#define K_CAPACITANCE 269
#define K_CAPMULTIPLIER 270
#define K_CLASS 271
#define K_CLOCK 272
#define K_CLOCKTYPE 273
#define K_COLUMNMAJOR 274
#define K_DESIGNRULEWIDTH 275
#define K_INFLUENCE 276
#define K_CORE 277
#define K_CORNER 278
#define K_COVER 279
#define K_CPERSQDIST 280
#define K_CURRENT 281
#define K_CURRENTSOURCE 282
#define K_CUT 283
#define K_DEFAULT 284
#define K_DATABASE 285
#define K_DATA 286
#define K_DIELECTRIC 287
#define K_DIRECTION 288
#define K_DO 289
#define K_EDGECAPACITANCE 290
#define K_EEQ 291
#define K_END 292
#define K_ENDCAP 293
#define K_FALL 294
#define K_FALLCS 295
#define K_FALLT0 296
#define K_FALLSATT1 297
#define K_FALLRS 298
#define K_FALLSATCUR 299
#define K_FALLTHRESH 300
#define K_FEEDTHRU 301
#define K_FIXED 302
#define K_FOREIGN 303
#define K_FROMPIN 304
#define K_GENERATE 305
#define K_GENERATOR 306
#define K_GROUND 307
#define K_HEIGHT 308
#define K_HORIZONTAL 309
#define K_INOUT 310
#define K_INPUT 311
#define K_INPUTNOISEMARGIN 312
#define K_COMPONENTPIN 313
#define K_INTRINSIC 314
#define K_INVERT 315
#define K_IRDROP 316
#define K_ITERATE 317
#define K_IV_TABLES 318
#define K_LAYER 319
#define K_LEAKAGE 320
#define K_LEQ 321
#define K_LIBRARY 322
#define K_MACRO 323
#define K_MATCH 324
#define K_MAXDELAY 325
#define K_MAXLOAD 326
#define K_METALOVERHANG 327
#define K_MILLIAMPS 328
#define K_MILLIWATTS 329
#define K_MINFEATURE 330
#define K_MUSTJOIN 331
#define K_NAMESCASESENSITIVE 332
#define K_NANOSECONDS 333
#define K_NETS 334
#define K_NEW 335
#define K_NONDEFAULTRULE 336
#define K_NONINVERT 337
#define K_NONUNATE 338
#define K_OBS 339
#define K_OHMS 340
#define K_OFFSET 341
#define K_ORIENTATION 342
#define K_ORIGIN 343
#define K_OUTPUT 344
#define K_OUTPUTNOISEMARGIN 345
#define K_OUTPUTRESISTANCE 346
#define K_OVERHANG 347
#define K_OVERLAP 348
#define K_OFF 349
#define K_ON 350
#define K_OVERLAPS 351
#define K_PAD 352
#define K_PATH 353
#define K_PATTERN 354
#define K_PICOFARADS 355
#define K_PIN 356
#define K_PITCH 357
#define K_PLACED 358
#define K_POLYGON 359
#define K_PORT 360
#define K_POST 361
#define K_POWER 362
#define K_PRE 363
#define K_PULLDOWNRES 364
#define K_RECT 365
#define K_RESISTANCE 366
#define K_RESISTIVE 367
#define K_RING 368
#define K_RISE 369
#define K_RISECS 370
#define K_RISERS 371
#define K_RISESATCUR 372
#define K_RISETHRESH 373
#define K_RISESATT1 374
#define K_RISET0 375
#define K_RISEVOLTAGETHRESHOLD 376
#define K_FALLVOLTAGETHRESHOLD 377
#define K_ROUTING 378
#define K_ROWMAJOR 379
#define K_RPERSQ 380
#define K_SAMENET 381
#define K_SCANUSE 382
#define K_SHAPE 383
#define K_SHRINKAGE 384
#define K_SIGNAL 385
#define K_SITE 386
#define K_SIZE 387
#define K_SOURCE 388
#define K_SPACER 389
#define K_SPACING 390
#define K_SPECIALNETS 391
#define K_STACK 392
#define K_START 393
#define K_STEP 394
#define K_STOP 395
#define K_STRUCTURE 396
#define K_SYMMETRY 397
#define K_TABLE 398
#define K_THICKNESS 399
#define K_TIEHIGH 400
#define K_TIELOW 401
#define K_TIEOFFR 402
#define K_TIME 403
#define K_TIMING 404
#define K_TO 405
#define K_TOPIN 406
#define K_TOPLEFT 407
#define K_TOPRIGHT 408
#define K_TOPOFSTACKONLY 409
#define K_TRISTATE 410
#define K_TYPE 411
#define K_UNATENESS 412
#define K_UNITS 413
#define K_USE 414
#define K_VARIABLE 415
#define K_VERTICAL 416
#define K_VHI 417
#define K_VIA 418
#define K_VIARULE 419
#define K_VLO 420
#define K_VOLTAGE 421
#define K_VOLTS 422
#define K_WIDTH 423
#define K_X 424
#define K_Y 425
#define K_R90 426
#define T_STRING 427
#define QSTRING 428
#define NUMBER 429
#define K_N 430
#define K_S 431
#define K_E 432
#define K_W 433
#define K_FN 434
#define K_FS 435
#define K_FE 436
#define K_FW 437
#define K_R0 438
#define K_R180 439
#define K_R270 440
#define K_MX 441
#define K_MY 442
#define K_MXR90 443
#define K_MYR90 444
#define K_USER 445
#define K_MASTERSLICE 446
#define K_ENDMACRO 447
#define K_ENDMACROPIN 448
#define K_ENDVIARULE 449
#define K_ENDVIA 450
#define K_ENDLAYER 451
#define K_ENDSITE 452
#define K_CANPLACE 453
#define K_CANNOTOCCUPY 454
#define K_TRACKS 455
#define K_FLOORPLAN 456
#define K_GCELLGRID 457
#define K_DEFAULTCAP 458
#define K_MINPINS 459
#define K_WIRECAP 460
#define K_STABLE 461
#define K_SETUP 462
#define K_HOLD 463
#define K_DEFINE 464
#define K_DEFINES 465
#define K_DEFINEB 466
#define K_IF 467
#define K_THEN 468
#define K_ELSE 469
#define K_FALSE 470
#define K_TRUE 471
#define K_EQ 472
#define K_NE 473
#define K_LE 474
#define K_LT 475
#define K_GE 476
#define K_GT 477
#define K_OR 478
#define K_AND 479
#define K_NOT 480
#define K_DELAY 481
#define K_TABLEDIMENSION 482
#define K_TABLEAXIS 483
#define K_TABLEENTRIES 484
#define K_TRANSITIONTIME 485
#define K_EXTENSION 486
#define K_PROPDEF 487
#define K_STRING 488
#define K_INTEGER 489
#define K_REAL 490
#define K_RANGE 491
#define K_PROPERTY 492
#define K_VIRTUAL 493
#define K_BUSBITCHARS 494
#define K_VERSION 495
#define K_BEGINEXT 496
#define K_ENDEXT 497
#define K_UNIVERSALNOISEMARGIN 498
#define K_EDGERATETHRESHOLD1 499
#define K_CORRECTIONTABLE 500
#define K_EDGERATESCALEFACTOR 501
#define K_EDGERATETHRESHOLD2 502
#define K_VICTIMNOISE 503
#define K_NOISETABLE 504
#define K_EDGERATE 505
#define K_VICTIMLENGTH 506
#define K_CORRECTIONFACTOR 507
#define K_OUTPUTPINANTENNASIZE 508
#define K_INPUTPINANTENNASIZE 509
#define K_INOUTPINANTENNASIZE 510
#define K_CURRENTDEN 511
#define K_PWL 512
#define K_ANTENNALENGTHFACTOR 513
#define K_TAPERRULE 514
#define K_DIVIDERCHAR 515
#define K_ANTENNASIZE 516
#define K_ANTENNAMETALLENGTH 517
#define K_ANTENNAMETALAREA 518
#define K_RISESLEWLIMIT 519
#define K_FALLSLEWLIMIT 520
#define K_FUNCTION 521
#define K_BUFFER 522
#define K_INVERTER 523
#define K_NAMEMAPSTRING 524
#define K_NOWIREEXTENSIONATPIN 525
#define K_WIREEXTENSION 526
#define K_MESSAGE 527
#define K_CREATEFILE 528
#define K_OPENFILE 529
#define K_CLOSEFILE 530
#define K_WARNING 531
#define K_ERROR 532
#define K_FATALERROR 533
#define K_RECOVERY 534
#define K_SKEW 535
#define K_ANYEDGE 536
#define K_POSEDGE 537
#define K_NEGEDGE 538
#define K_SDFCONDSTART 539
#define K_SDFCONDEND 540
#define K_SDFCOND 541
#define K_MPWH 542
#define K_MPWL 543
#define K_PERIOD 544
#define K_ACCURRENTDENSITY 545
#define K_DCCURRENTDENSITY 546
#define K_AVERAGE 547
#define K_PEAK 548
#define K_RMS 549
#define K_FREQUENCY 550
#define K_CUTAREA 551
#define K_MEGAHERTZ 552
#define K_USELENGTHTHRESHOLD 553
#define K_LENGTHTHRESHOLD 554
#define K_ANTENNAINPUTGATEAREA 555
#define K_ANTENNAINOUTDIFFAREA 556
#define K_ANTENNAOUTPUTDIFFAREA 557
#define K_ANTENNAAREARATIO 558
#define K_ANTENNADIFFAREARATIO 559
#define K_ANTENNACUMAREARATIO 560
#define K_ANTENNACUMDIFFAREARATIO 561
#define K_ANTENNAAREAFACTOR 562
#define K_ANTENNASIDEAREARATIO 563
#define K_ANTENNADIFFSIDEAREARATIO 564
#define K_ANTENNACUMSIDEAREARATIO 565
#define K_ANTENNACUMDIFFSIDEAREARATIO 566
#define K_ANTENNASIDEAREAFACTOR 567
#define K_DIFFUSEONLY 568
#define K_MANUFACTURINGGRID 569
#define K_ANTENNACELL 570
#define K_CLEARANCEMEASURE 571
#define K_EUCLIDEAN 572
#define K_MAXXY 573
#define K_USEMINSPACING 574
#define K_ROWMINSPACING 575
#define K_ROWABUTSPACING 576
#define K_FLIP 577
#define K_NONE 578
#define K_ANTENNAPARTIALMETALAREA 579
#define K_ANTENNAPARTIALMETALSIDEAREA 580
#define K_ANTENNAGATEAREA 581
#define K_ANTENNADIFFAREA 582
#define K_ANTENNAMAXAREACAR 583
#define K_ANTENNAMAXSIDEAREACAR 584
#define K_ANTENNAPARTIALCUTAREA 585
#define K_ANTENNAMAXCUTCAR 586
#define K_SLOTWIREWIDTH 587
#define K_SLOTWIRELENGTH 588
#define K_SLOTWIDTH 589
#define K_SLOTLENGTH 590
#define K_MAXADJACENTSLOTSPACING 591
#define K_MAXCOAXIALSLOTSPACING 592
#define K_MAXEDGESLOTSPACING 593
#define K_SPLITWIREWIDTH 594
#define K_MINIMUMDENSITY 595
#define K_MAXIMUMDENSITY 596
#define K_DENSITYCHECKWINDOW 597
#define K_DENSITYCHECKSTEP 598
#define K_FILLACTIVESPACING 599
#define K_MINIMUMCUT 600
#define K_ADJACENTCUTS 601
#define K_ANTENNAMODEL 602
#define K_BUMP 603
#define K_ENCLOSURE 604
#define K_FROMABOVE 605
#define K_FROMBELOW 606
#define K_IMPLANT 607
#define K_LENGTH 608
#define K_MAXVIASTACK 609
#define K_AREAIO 610
#define K_BLACKBOX 611
#define K_MAXWIDTH 612
#define K_MINENCLOSEDAREA 613
#define K_MINSTEP 614
#define K_ORIENT 615
#define K_OXIDE1 616
#define K_OXIDE2 617
#define K_OXIDE3 618
#define K_OXIDE4 619
#define K_PARALLELRUNLENGTH 620
#define K_MINWIDTH 621
#define K_PROTRUSIONWIDTH 622
#define K_SPACINGTABLE 623
#define K_WITHIN 624
#define K_ABOVE 625
#define K_BELOW 626
#define K_CENTERTOCENTER 627
#define K_CUTSIZE 628
#define K_CUTSPACING 629
#define K_DENSITY 630
#define K_DIAG45 631
#define K_DIAG135 632
#define K_DIAGMINEDGELENGTH 633
#define K_DIAGSPACING 634
#define K_DIAGPITCH 635
#define K_DIAGWIDTH 636
#define K_GENERATED 637
#define K_GROUNDSENSITIVITY 638
#define K_HARDSPACING 639
#define K_INSIDECORNER 640
#define K_LAYERS 641
#define K_LENGTHSUM 642
#define K_MICRONS 643
#define K_MINCUTS 644
#define K_MINSIZE 645
#define K_NETEXPR 646
#define K_OUTSIDECORNER 647
#define K_PREFERENCLOSURE 648
#define K_ROWCOL 649
#define K_ROWPATTERN 650
#define K_SOFT 651
#define K_SUPPLYSENSITIVITY 652
#define K_USEVIA 653
#define K_USEVIARULE 654
#define K_WELLTAP 655
#define K_ARRAYCUTS 656
#define K_ARRAYSPACING 657
#define K_ANTENNAAREADIFFREDUCEPWL 658
#define K_ANTENNAAREAMINUSDIFF 659
#define K_ANTENNACUMROUTINGPLUSCUT 660
#define K_ANTENNAGATEPLUSDIFF 661
#define K_ENDOFLINE 662
#define K_ENDOFNOTCHWIDTH 663
#define K_EXCEPTEXTRACUT 664
#define K_EXCEPTSAMEPGNET 665
#define K_EXCEPTPGNET 666
#define K_LONGARRAY 667
#define K_MAXEDGES 668
#define K_NOTCHLENGTH 669
#define K_NOTCHSPACING 670
#define K_ORTHOGONAL 671
#define K_PARALLELEDGE 672
#define K_PARALLELOVERLAP 673
#define K_PGONLY 674
#define K_PRL 675
#define K_TWOEDGES 676
#define K_TWOWIDTHS 677
#define IF 678
#define LNOT 679
#define UMINUS 680




/* Copy the first part of user declarations.  */
#line 33 "lef.y"

#include <string.h>
#include <stdlib.h>
#include <math.h>
/* # include <sys/types.h> */
/* # include <sys/stat.h> */
/* # include <fcntl.h> */
/* # include <unistd.h> */
/* # ifndef	WIN32 */
/* # include <sys/param.h> */
/* # endif */
#include "FEF_stringhash.h"
#include "lex.h"
#include "lefiDefs.hpp"
#include "lefiUser.hpp"
#include "lefiUtil.hpp"
#include "lefrCallBacks.hpp"
#ifdef WIN32
#include <malloc.h>
#endif

#define LYPROP_ECAP "EDGE_CAPACITANCE"

#define YYINITDEPTH 10000  /* pcr 640902 - initialize the yystacksize to 300 */
                           /* this may need to increase in a design gets */
                           /* larger and a polygon has around 300 sizes */
                           /* 11/21/2003 - incrreased to 500, design from */
                           /* Artisan is greater than 300, need to find a */
                           /* way to dynamically increase the size */
                           /* 2/10/2004 - increased to 1000 for pcr 686073 */
                           /* 3/22/2004 - increased to 2000 for pcr 695879 */
                           /* 9/29/2004 - double the size for pcr 746865 */
                           /* tried to overwrite the yyoverflow definition */
                           /* it is impossible due to the union structure */
                           /* 10/03/2006 - increased to 10000 for pcr 913695 */

#define YYMAXDEPTH 80000

int  lefRetVal;
char *outMsg;

static char lefPropDefType;    /* save the current type of the property */

/* Macro to describe how we handle a callback.
 * If the function was set then call it.
 * If the function returns non zero then there was an error
 * so call the error routine and exit.
 */
#define CALLBACK(func, typ, data) \
    if (!lef_errors) { \
      if (func) { \
        if ((lefRetVal = (*func)(typ, data, lefrUserData)) == 0) { \
        } else { \
	  return lefRetVal; \
        } \
      } \
    }

#define CHKERR() \
    if (lef_errors > 20) { \
      lefError(1011, "Too many syntax errors."); \
      lef_errors = 0; \
      return 1; \
    }

extern int lefNlines, lefNtokens;
extern FILE* lefFile;

/**********************************************************************/
/**********************************************************************/

#define C_EQ 0
#define C_NE 1
#define C_LT 2
#define C_LE 3
#define C_GT 4
#define C_GE 5

extern int lefDumbMode;   // for communicating with parser
extern int lefNoNum;      // likewise, says no numbers for the next N tokens
extern int lefRealNum;    // Next token will be a real number
extern int lefNlToken;    // likewise
extern int lefDefIf;      // likewise
extern int lefNdRule;     // for communicating with parser
extern int lefInPropDef;  // for tracking if inside propertydefinitions

extern int lef_errors;    // from lex.cpph
extern char* Hist_text;   // for BEGINEXT - extension

extern int doneLib;       // keep track if the file is done parsing
extern int ge56almostDone;// lef file has Extension section
extern int ge56done;      // done reading 5.6 and beyond library

extern char* lefrFileName;

static int ignoreVersion = 0; // ignore checking version number

int lefNamesCaseSensitive = TRUE;  // always true in 5.6
int lefReaderCaseSensitive = FALSE;  // default to 0

int lefrShiftCase = 0;     // if user wants to shift to all uppercase

int lefrRelaxMode = FALSE;    // relax mode?

/* XXXXXXXXXXXXX check out these variables */
double lef_save_x, lef_save_y;  // for interpreting (*) notation of LEF/DEF

/* #define STRING_LIST_SIZE 1024 */
/* char string_list[STRING_LIST_SIZE]; */

static char temp_name[258];
static char* layerName;
static char* viaName;
static char* viaRuleName;
static char* nonDefaultRuleName;
static char* siteName;
static char* arrayName;
static char* macroName;
static char* pinName;

static int siteDef, symDef, sizeDef, pinDef, obsDef, origDef;
static int useLenThr;
static int layerCut, layerMastOver, layerRout, layerDir;
static lefiAntennaEnum antennaType = lefiAntennaAR;  /* 5.4 - antenna type */
static int viaRuleLayer;       /* keep track number of layer in a viarule */
static int viaLayer;           /* keep track number of layer in a via */
static int ndLayer;            /* keep track number of layer in a nondefault */
static int numVia;             /* keep track number of via */
static int viaRuleHasDir;      /* viarule layer has direction construct */
static int viaRuleHasEnc;      /* viarule layer has enclosure construct */
static int ndRule = 0;         /* keep track if inside nondefaultrule */
static int ndLayerWidth;       /* keep track if width is set at ndLayer */
static int ndLayerSpace;       /* keep track if spacing is set at ndLayer */
static int isGenerate;         /* keep track if viarule has generate */
static int hasType;            /* keep track of type in layer */
static int hasPitch;           /* keep track of pitch in layer */
static int hasWidth;           /* keep track of width in layer */
static int hasDirection;       /* keep track of direction in layer */
static int hasParallel;        /* keep track of parallelrunlength */
static int hasInfluence;       /* keep track of influence */
static int hasTwoWidths;       /* keep track of twoWidths */
static int hasLayerMincut;     /* keep track of layer minimumcut */
static int hasManufactur = 0;  /* keep track of manufacture is after unit */
static int hasPRP;             /* keep track if path, rect or poly is def */
static int needGeometry;       /* keep track if path/rect/poly is defined */
static int hasViaRule_layer = 0; /* keep track at least viarule or layer */
static int hasSamenet;         /* keep track if samenet is defined in spacing */
static int hasSiteClass = 0;   /* keep track if SITE has CLASS */
static int hasSiteSize = 0;    /* keep track if SITE has SIZE */
static int hasSpCenter = 0;    /* keep track if LAYER SPACING has CENTER */
static int hasSpSamenet = 0;   /* keep track if LAYER SPACING has SAMENET */
static int hasSpParallel = 0;  /* keep track if LAYER SPACING has PARALLEL */
static int hasSpLayer = 0;     /* keep track if LAYER SPACING has LAYER */
static int hasGeoLayer = 0;    /* keep track if Geometries has LAYER */


// the following variables to keep track the number of warnings printed.
static int antennaInoutWarnings = 0;
static int antennaInputWarnings = 0;
static int antennaOutputWarnings = 0;
static int arrayWarnings = 0;
static int caseSensitiveWarnings = 0;
static int correctionTableWarnings = 0;
static int dielectricWarnings = 0;
static int edgeRateThreshold1Warnings = 0;
static int edgeRateThreshold2Warnings = 0;
static int edgeRateScaleFactorWarnings = 0;
static int inoutAntennaWarnings = 0;
static int inputAntennaWarnings = 0;
static int iRDropWarnings = 0;
static int layerWarnings = 0;
static int macroWarnings = 0;
static int maxStackViaWarnings = 0;
static int minFeatureWarnings = 0;
static int noiseMarginWarnings = 0;
static int noiseTableWarnings = 0;
static int nonDefaultWarnings = 0;
static int noWireExtensionWarnings = 0;
static int outputAntennaWarnings = 0;
static int pinWarnings = 0;
static int siteWarnings = 0;
static int spacingWarnings = 0;
static int timingWarnings = 0;
static int unitsWarnings = 0;
static int useMinSpacingWarnings = 0;
static int viaRuleWarnings = 0;
static int viaWarnings = 0;
static double layerCutSpacing = 0;

int spParallelLength;          /* the number of layer parallelrunlength */

lefiNum macroNum;              /* for origin & size callback */

int comp_str(char *s1, int op, char *s2)
{
    int k = strcmp(s1, s2);
    switch (op) {
	case C_EQ: return k == 0;
	case C_NE: return k != 0;
	case C_GT: return k >  0;
	case C_GE: return k >= 0;
	case C_LT: return k <  0;
	case C_LE: return k <= 0;
	}
    return 0;
}
int comp_num(double s1, int op, double s2)
{
    double k = s1 - s2;
    switch (op) {
	case C_EQ: return k == 0;
	case C_NE: return k != 0;
	case C_GT: return k >  0;
	case C_GE: return k >= 0;
	case C_LT: return k <  0;
	case C_LE: return k <= 0;
	}
    return 0;
}
/*
double convert_name2num(char *versionName)
{
    char majorNm[80];
    char minorNm[80];
    char *subMinorNm = NULL;
    char *p1;
    char *versionNm = strdup(versionName);
  
    double major = 0, minor = 0, subMinor = 0;
    double version;

    sscanf(versionNm, "%[^.].%s", majorNm, minorNm);
    if (p1 = strchr(minorNm, '.')) {
       subMinorNm = p1+1; 
       *p1 = '\0';
    }
    major = atof(majorNm);
    minor = atof(minorNm);
    if (subMinorNm)
       subMinor = atof(subMinorNm);

    version = major;
    
    if (minor > 0)
       version = major + minor/10;

    if (subMinor > 0)
       version = version + subMinor/1000;

    lefFree(versionNm);
    return version;
}
*/

int validNum(int values) {
    switch (values) {
        case 100:
        case 200:
        case 1000:
        case 2000:
                return 1; 
        case 10000:
        case 20000:
             if (versionNum < 5.6) {
                if (lefrUnitsCbk) {
                  if (unitsWarnings++ < lefrUnitsWarnings) {
                    outMsg = (char*)lefMalloc(10000);
                    sprintf (outMsg,
                       "Error found when processing LEF file '%s'\nUnit %d is a version 5.6 or later syntax\nYour lef file is defined with version %g",
                    lefrFileName, values, versionNum);
                    lefError(1501, outMsg);
                    lefFree(outMsg);
                  }
                }
                return 0;
             } else
                return 1;
    }
    if (unitsWarnings++ < lefrUnitsWarnings) {
       outMsg = (char*)lefMalloc(10000);
       sprintf (outMsg,
          "The value %d defined for LEF UNITS DATABASE MICRONS is invalid\n. Correct value is 100, 200, 1000, 2000, 10000, or 20000", values);
       lefError(1502, outMsg);
       lefFree(outMsg);
    }
    CHKERR();
    return 0;
}

int zeroOrGt(double values) {
    if (values < 0)
      return 0;
    return 1;
}

void resetVars() {
     hasVer = 0;
     hasNameCase = 0;
     hasBusBit = 0;
     hasDivChar = 0;
     hasManufactur = 0;
     antennaInoutWarnings = 0;
     antennaInputWarnings = 0;
     antennaOutputWarnings = 0;
     arrayWarnings = 0;
     caseSensitiveWarnings = 0;
     correctionTableWarnings = 0;
     dielectricWarnings = 0;
     edgeRateThreshold1Warnings = 0;
     edgeRateThreshold2Warnings = 0;
     edgeRateScaleFactorWarnings = 0;
     inoutAntennaWarnings = 0;
     inputAntennaWarnings = 0;
     iRDropWarnings = 0;
     layerWarnings = 0;
     macroWarnings = 0;
     maxStackViaWarnings = 0;
     minFeatureWarnings = 0;
     noiseMarginWarnings = 0;
     noiseTableWarnings = 0;
     nonDefaultWarnings = 0;
     noWireExtensionWarnings = 0;
     outputAntennaWarnings = 0;
     siteWarnings = 0;
     spacingWarnings = 0;
     timingWarnings = 0;
     unitsWarnings = 0;
     useMinSpacingWarnings = 0;
     viaRuleWarnings = 0;
     viaWarnings = 0;
     return;
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

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 368 "lef.y"
typedef union YYSTYPE {
	double    dval ;
	int       integer ;
	char *    string ;
	lefPOINT  pt;
} YYSTYPE;
/* Line 196 of yacc.c.  */
#line 1285 "lef.tab.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 219 of yacc.c.  */
#line 1297 "lef.tab.c"

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T) && (defined (__STDC__) || defined (__cplusplus))
# include <stddef.h> /* INFRINGES ON USER NAME SPACE */
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if defined (__STDC__) || defined (__cplusplus)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     define YYINCLUDED_STDLIB_H
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2005 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM ((YYSIZE_T) -1)
#  endif
#  ifdef __cplusplus
extern "C" {
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if (! defined (malloc) && ! defined (YYINCLUDED_STDLIB_H) \
	&& (defined (__STDC__) || defined (__cplusplus)))
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if (! defined (free) && ! defined (YYINCLUDED_STDLIB_H) \
	&& (defined (__STDC__) || defined (__cplusplus)))
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifdef __cplusplus
}
#  endif
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short int yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short int) + sizeof (YYSTYPE))			\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
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
   typedef short int yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2175

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  437
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  445
/* YYNRULES -- Number of rules. */
#define YYNRULES  1010
/* YYNRULES -- Number of states. */
#define YYNSTATES  2037

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   680

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned short int yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     434,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     431,   432,   427,   426,     2,   425,     2,   428,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   430,
     435,   433,   436,     2,     2,     2,     2,     2,     2,     2,
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
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     429
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short int yyprhs[] =
{
       0,     0,     3,     7,     8,    13,    17,    21,    22,    25,
      27,    28,    31,    33,    35,    37,    39,    41,    43,    45,
      47,    49,    51,    53,    55,    57,    59,    61,    63,    65,
      67,    69,    71,    73,    75,    77,    79,    81,    83,    85,
      87,    89,    91,    93,    95,    97,    99,   101,   103,   105,
     107,   111,   115,   119,   123,   127,   132,   136,   138,   140,
     142,   144,   146,   148,   153,   155,   156,   159,   164,   169,
     174,   179,   184,   189,   194,   199,   203,   204,   208,   209,
     213,   214,   217,   218,   219,   233,   237,   241,   246,   250,
     255,   259,   264,   268,   272,   276,   280,   281,   288,   289,
     299,   303,   308,   316,   321,   329,   333,   337,   341,   345,
     349,   353,   357,   361,   368,   375,   376,   381,   382,   387,
     392,   397,   398,   399,   409,   410,   411,   421,   425,   426,
     431,   435,   436,   441,   442,   448,   452,   453,   458,   462,
     463,   468,   469,   475,   476,   481,   484,   488,   492,   493,
     494,   504,   508,   512,   516,   520,   524,   528,   532,   536,
     540,   544,   549,   553,   557,   561,   565,   566,   572,   573,
     582,   583,   589,   597,   598,   603,   604,   612,   613,   621,
     625,   629,   630,   636,   637,   639,   640,   643,   644,   647,
     652,   653,   654,   655,   656,   668,   669,   670,   680,   681,
     691,   692,   695,   700,   701,   703,   705,   706,   707,   712,
     715,   716,   719,   720,   723,   724,   727,   728,   731,   733,
     735,   740,   741,   744,   746,   749,   752,   754,   756,   758,
     760,   761,   769,   770,   773,   775,   776,   778,   780,   782,
     784,   785,   786,   787,   800,   801,   802,   808,   809,   815,
     816,   822,   823,   826,   828,   831,   834,   837,   840,   842,
     845,   850,   852,   855,   860,   862,   865,   870,   872,   874,
     876,   878,   880,   882,   884,   886,   888,   890,   891,   894,
     896,   898,   900,   902,   903,   906,   907,   912,   913,   916,
     917,   924,   925,   928,   929,   932,   939,   943,   944,   952,
     953,   958,   960,   963,   967,   971,   972,   973,   974,  1001,
    1002,  1005,  1010,  1015,  1022,  1023,  1028,  1030,  1032,  1035,
    1036,  1039,  1041,  1043,  1047,  1048,  1053,  1055,  1057,  1060,
    1063,  1066,  1069,  1072,  1076,  1081,  1085,  1086,  1090,  1092,
    1094,  1096,  1098,  1100,  1102,  1104,  1106,  1108,  1110,  1112,
    1114,  1116,  1118,  1120,  1122,  1125,  1126,  1131,  1132,  1135,
    1140,  1141,  1149,  1150,  1154,  1155,  1159,  1165,  1166,  1174,
    1175,  1177,  1179,  1182,  1183,  1185,  1187,  1190,  1191,  1196,
    1198,  1201,  1204,  1207,  1210,  1213,  1214,  1217,  1221,  1222,
    1227,  1228,  1231,  1235,  1239,  1244,  1250,  1255,  1261,  1265,
    1269,  1273,  1274,  1278,  1282,  1284,  1287,  1288,  1291,  1297,
    1304,  1306,  1310,  1312,  1315,  1316,  1319,  1323,  1324,  1327,
    1330,  1333,  1338,  1342,  1343,  1344,  1345,  1354,  1356,  1359,
    1360,  1363,  1364,  1367,  1369,  1371,  1373,  1375,  1377,  1379,
    1381,  1385,  1389,  1394,  1395,  1400,  1402,  1405,  1408,  1411,
    1414,  1415,  1416,  1417,  1418,  1431,  1432,  1435,  1439,  1443,
    1448,  1453,  1457,  1461,  1465,  1466,  1470,  1471,  1475,  1476,
    1479,  1485,  1487,  1489,  1491,  1495,  1499,  1503,  1507,  1508,
    1511,  1513,  1515,  1517,  1518,  1523,  1524,  1527,  1528,  1532,
    1535,  1540,  1541,  1546,  1547,  1551,  1552,  1556,  1557,  1560,
    1562,  1564,  1566,  1568,  1570,  1572,  1574,  1576,  1578,  1580,
    1582,  1584,  1586,  1590,  1594,  1596,  1598,  1600,  1602,  1603,
    1608,  1610,  1613,  1617,  1618,  1621,  1623,  1625,  1627,  1630,
    1633,  1636,  1640,  1642,  1645,  1647,  1649,  1652,  1655,  1657,
    1659,  1661,  1663,  1666,  1668,  1670,  1673,  1676,  1678,  1680,
    1682,  1684,  1686,  1688,  1690,  1692,  1694,  1696,  1698,  1700,
    1702,  1704,  1706,  1708,  1710,  1712,  1716,  1721,  1725,  1729,
    1733,  1737,  1741,  1744,  1748,  1753,  1757,  1758,  1763,  1764,
    1769,  1773,  1777,  1779,  1781,  1787,  1791,  1792,  1796,  1797,
    1801,  1802,  1805,  1808,  1812,  1817,  1821,  1826,  1832,  1833,
    1838,  1842,  1844,  1848,  1852,  1856,  1860,  1864,  1868,  1872,
    1876,  1880,  1884,  1888,  1889,  1894,  1895,  1901,  1902,  1908,
    1909,  1915,  1919,  1923,  1927,  1931,  1935,  1939,  1943,  1947,
    1951,  1956,  1960,  1961,  1966,  1971,  1974,  1979,  1984,  1989,
    1993,  1997,  2002,  2007,  2012,  2017,  2022,  2027,  2032,  2037,
    2038,  2043,  2044,  2049,  2050,  2055,  2056,  2061,  2063,  2065,
    2067,  2069,  2071,  2074,  2077,  2080,  2083,  2087,  2091,  2096,
    2100,  2104,  2106,  2107,  2111,  2113,  2115,  2117,  2119,  2121,
    2123,  2125,  2127,  2129,  2131,  2132,  2134,  2136,  2138,  2141,
    2142,  2143,  2151,  2155,  2160,  2167,  2172,  2179,  2187,  2197,
    2199,  2200,  2203,  2204,  2206,  2207,  2210,  2213,  2215,  2217,
    2218,  2221,  2222,  2228,  2229,  2237,  2245,  2257,  2262,  2263,
    2264,  2275,  2276,  2277,  2288,  2295,  2302,  2303,  2306,  2308,
    2315,  2322,  2326,  2329,  2331,  2336,  2337,  2340,  2341,  2342,
    2350,  2351,  2354,  2360,  2361,  2366,  2370,  2372,  2375,  2376,
    2379,  2380,  2385,  2386,  2391,  2392,  2403,  2413,  2417,  2421,
    2426,  2431,  2436,  2441,  2446,  2451,  2456,  2461,  2465,  2473,
    2482,  2489,  2493,  2497,  2501,  2504,  2506,  2508,  2510,  2512,
    2514,  2516,  2518,  2520,  2522,  2524,  2526,  2528,  2530,  2532,
    2534,  2536,  2539,  2545,  2547,  2550,  2551,  2556,  2564,  2566,
    2568,  2570,  2572,  2574,  2576,  2579,  2581,  2584,  2585,  2590,
    2591,  2595,  2596,  2600,  2601,  2604,  2605,  2610,  2611,  2616,
    2617,  2622,  2623,  2628,  2633,  2634,  2639,  2645,  2647,  2650,
    2651,  2654,  2655,  2660,  2661,  2666,  2667,  2670,  2676,  2677,
    2684,  2685,  2692,  2693,  2700,  2701,  2708,  2709,  2716,  2717,
    2719,  2721,  2723,  2726,  2728,  2731,  2735,  2739,  2743,  2747,
    2750,  2754,  2761,  2763,  2767,  2771,  2775,  2779,  2783,  2787,
    2791,  2795,  2799,  2803,  2806,  2810,  2817,  2819,  2821,  2825,
    2829,  2836,  2838,  2840,  2842,  2844,  2846,  2848,  2850,  2852,
    2854,  2856,  2857,  2863,  2864,  2867,  2868,  2874,  2875,  2881,
    2882,  2888,  2889,  2895,  2896,  2902,  2903,  2909,  2910,  2916,
    2917,  2923,  2924,  2929,  2930,  2935,  2937,  2940,  2943,  2944,
    2946,  2949,  2955,  2959,  2960,  2961,  2968,  2969,  2971,  2972,
    2974,  2975,  2979,  2980,  2982,  2983,  2986,  2988,  2989,  2993,
    2995,  2996,  2997,  2998,  3004,  3005,  3012,  3015,  3016,  3022,
    3025,  3031,  3032,  3039,  3042,  3049,  3050,  3052,  3053,  3055,
    3056,  3057,  3061,  3062,  3066,  3071,  3075,  3079,  3083,  3084,
    3092,  3095,  3097,  3100,  3104,  3106,  3107,  3113,  3115,  3118,
    3120,  3123,  3124,  3132,  3134,  3137,  3138,  3146,  3149,  3151,
    3154,  3158,  3160,  3161,  3167,  3169,  3172,  3174,  3177,  3178,
    3186,  3188,  3191,  3195,  3199,  3203,  3207,  3211,  3215,  3216,
    3218
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short int yyrhs[] =
{
     438,     0,    -1,   443,   880,   444,    -1,    -1,   240,   440,
     172,   430,    -1,   260,   173,   430,    -1,   239,   173,   430,
      -1,    -1,   443,   445,    -1,     1,    -1,    -1,    37,    67,
      -1,   439,    -1,   442,    -1,   446,    -1,   454,    -1,   458,
      -1,   555,    -1,   587,    -1,   588,    -1,   441,    -1,   447,
      -1,   794,    -1,   606,    -1,   621,    -1,   620,    -1,   612,
      -1,   643,    -1,   660,    -1,   774,    -1,   798,    -1,   622,
      -1,   809,    -1,   846,    -1,   847,    -1,   849,    -1,   848,
      -1,   850,    -1,   862,    -1,   874,    -1,   875,    -1,   876,
      -1,   877,    -1,   878,    -1,   879,    -1,   448,    -1,   449,
      -1,   450,    -1,   553,    -1,   796,    -1,    77,    95,   430,
      -1,    77,    94,   430,    -1,   270,    95,   430,    -1,   270,
      94,   430,    -1,   314,   174,   430,    -1,   319,   452,   453,
     430,    -1,   316,   451,   430,    -1,   318,    -1,   317,    -1,
      84,    -1,   101,    -1,    95,    -1,    94,    -1,   455,   456,
      37,   158,    -1,   158,    -1,    -1,   456,   457,    -1,   148,
      78,   174,   430,    -1,    14,   100,   174,   430,    -1,   111,
      85,   174,   430,    -1,   107,    74,   174,   430,    -1,    26,
      73,   174,   430,    -1,   166,   167,   174,   430,    -1,    30,
     388,   174,   430,    -1,   295,   297,   174,   430,    -1,   459,
     463,   461,    -1,    -1,    64,   460,   172,    -1,    -1,    37,
     462,   172,    -1,    -1,   464,   463,    -1,    -1,    -1,   402,
     465,   491,   492,   374,   174,   401,   174,   135,   174,   466,
     493,   430,    -1,   156,   540,   430,    -1,   102,   174,   430,
      -1,   102,   174,   174,   430,    -1,   380,   174,   430,    -1,
     380,   174,   174,   430,    -1,    86,   174,   430,    -1,    86,
     174,   174,   430,    -1,   381,   174,   430,    -1,   379,   174,
     430,    -1,   168,   174,   430,    -1,     9,   174,   430,    -1,
      -1,   135,   174,   467,   831,   834,   430,    -1,    -1,   368,
     416,   369,   174,   135,   174,   468,   503,   430,    -1,    33,
     541,   430,    -1,   111,   125,   174,   430,    -1,   111,   125,
     257,   431,   538,   432,   430,    -1,    14,    25,   174,   430,
      -1,    14,    25,   257,   431,   536,   432,   430,    -1,    53,
     174,   430,    -1,   271,   174,   430,    -1,   144,   174,   430,
      -1,   129,   174,   430,    -1,    15,   174,   430,    -1,    35,
     174,   430,    -1,   258,   174,   430,    -1,   256,   174,   430,
      -1,   256,   257,   431,   534,   432,   430,    -1,   256,   431,
     174,   174,   432,   430,    -1,    -1,   237,   469,   532,   430,
      -1,    -1,   290,   521,   470,   522,    -1,   290,   521,   174,
     430,    -1,   291,   292,   174,   430,    -1,    -1,    -1,   291,
     292,   296,   174,   471,   531,   430,   472,   529,    -1,    -1,
      -1,   291,   292,   168,   174,   473,   531,   430,   474,   529,
      -1,   303,   174,   430,    -1,    -1,   304,   475,   516,   430,
      -1,   305,   174,   430,    -1,    -1,   306,   476,   516,   430,
      -1,    -1,   307,   174,   477,   520,   430,    -1,   308,   174,
     430,    -1,    -1,   309,   478,   516,   430,    -1,   310,   174,
     430,    -1,    -1,   311,   479,   516,   430,    -1,    -1,   312,
     174,   480,   520,   430,    -1,    -1,   347,   481,   543,   430,
      -1,   405,   430,    -1,   406,   174,   430,    -1,   404,   174,
     430,    -1,    -1,    -1,   403,   431,   659,   659,   482,   518,
     432,   430,   483,    -1,   332,   174,   430,    -1,   333,   174,
     430,    -1,   334,   174,   430,    -1,   335,   174,   430,    -1,
     336,   174,   430,    -1,   337,   174,   430,    -1,   338,   174,
     430,    -1,   339,   174,   430,    -1,   340,   174,   430,    -1,
     341,   174,   430,    -1,   342,   174,   174,   430,    -1,   343,
     174,   430,    -1,   344,   174,   430,    -1,   357,   174,   430,
      -1,   366,   174,   430,    -1,    -1,   358,   174,   484,   542,
     430,    -1,    -1,   345,   174,   168,   174,   485,   510,   511,
     430,    -1,    -1,   359,   174,   486,   513,   430,    -1,   367,
     174,   353,   174,   168,   174,   430,    -1,    -1,   368,   487,
     495,   430,    -1,    -1,   349,   505,   174,   174,   488,   506,
     430,    -1,    -1,   393,   505,   174,   174,   489,   509,   430,
      -1,   111,   174,   430,    -1,   378,   174,   430,    -1,    -1,
     390,   490,   726,   728,   430,    -1,    -1,   412,    -1,    -1,
     168,   174,    -1,    -1,   494,   493,    -1,   401,   174,   135,
     174,    -1,    -1,    -1,    -1,    -1,   365,   174,   496,   531,
     497,   168,   174,   498,   531,   499,   544,    -1,    -1,    -1,
     422,   168,   174,   550,   174,   500,   531,   501,   547,    -1,
      -1,    21,   168,   174,   369,   174,   135,   174,   502,   551,
      -1,    -1,   504,   503,    -1,   369,   174,   135,   174,    -1,
      -1,   370,    -1,   371,    -1,    -1,    -1,   168,   174,   507,
     508,    -1,   353,   174,    -1,    -1,   409,   174,    -1,    -1,
     168,   174,    -1,    -1,   369,   174,    -1,    -1,   512,   511,
      -1,   350,    -1,   351,    -1,   353,   174,   369,   174,    -1,
      -1,   514,   513,    -1,   515,    -1,   387,   174,    -1,   413,
     174,    -1,   385,    -1,   392,    -1,   139,    -1,   174,    -1,
      -1,   257,   431,   659,   659,   517,   518,   432,    -1,    -1,
     519,   518,    -1,   659,    -1,    -1,   313,    -1,   293,    -1,
     292,    -1,   294,    -1,    -1,    -1,    -1,   295,   174,   523,
     531,   430,   524,   526,   229,   174,   525,   531,   430,    -1,
      -1,    -1,   296,   174,   527,   531,   430,    -1,    -1,   168,
     174,   528,   531,   430,    -1,    -1,   229,   174,   530,   531,
     430,    -1,    -1,   531,   174,    -1,   533,    -1,   532,   533,
      -1,   172,   172,    -1,   172,   173,    -1,   172,   174,    -1,
     535,    -1,   534,   535,    -1,   431,   174,   174,   432,    -1,
     537,    -1,   536,   537,    -1,   431,   174,   174,   432,    -1,
     539,    -1,   538,   539,    -1,   431,   174,   174,   432,    -1,
     123,    -1,    28,    -1,    93,    -1,   191,    -1,   238,    -1,
     352,    -1,    54,    -1,   161,    -1,   376,    -1,   377,    -1,
      -1,   168,   174,    -1,   361,    -1,   362,    -1,   363,    -1,
     364,    -1,    -1,   545,   544,    -1,    -1,   168,   174,   546,
     531,    -1,    -1,   548,   547,    -1,    -1,   168,   174,   550,
     174,   549,   531,    -1,    -1,   420,   174,    -1,    -1,   551,
     552,    -1,   168,   174,   369,   174,   135,   174,    -1,   354,
     174,   430,    -1,    -1,   354,   174,   236,   554,   172,   172,
     430,    -1,    -1,   558,   556,   566,   583,    -1,   163,    -1,
     557,   172,    -1,   557,   172,    29,    -1,   557,   172,   382,
      -1,    -1,    -1,    -1,   164,   560,   172,   430,   373,   174,
     174,   430,   386,   561,   172,   172,   172,   430,   374,   174,
     174,   430,   349,   174,   174,   174,   174,   430,   562,   563,
      -1,    -1,   563,   564,    -1,   394,   174,   174,   430,    -1,
      88,   174,   174,   430,    -1,    86,   174,   174,   174,   174,
     430,    -1,    -1,    99,   565,   172,   430,    -1,   559,    -1,
     567,    -1,   569,   568,    -1,    -1,   568,   569,    -1,   573,
      -1,   577,    -1,   111,   174,   430,    -1,    -1,   237,   570,
     571,   430,    -1,   154,    -1,   572,    -1,   571,   572,    -1,
     172,   174,    -1,   172,   173,    -1,   172,   172,    -1,   574,
     430,    -1,   574,   659,   430,    -1,   574,   659,   576,   430,
      -1,   574,   576,   430,    -1,    -1,    48,   575,   172,    -1,
     175,    -1,   178,    -1,   176,    -1,   177,    -1,   179,    -1,
     182,    -1,   180,    -1,   181,    -1,   183,    -1,   171,    -1,
     184,    -1,   185,    -1,   187,    -1,   189,    -1,   186,    -1,
     188,    -1,   578,   580,    -1,    -1,    64,   579,   172,   430,
      -1,    -1,   580,   581,    -1,   110,   659,   659,   430,    -1,
      -1,   104,   582,   726,   727,   727,   728,   430,    -1,    -1,
      37,   584,   172,    -1,    -1,   164,   586,   172,    -1,   585,
     591,   598,   592,   604,    -1,    -1,   585,    50,   590,   589,
     591,   592,   604,    -1,    -1,    29,    -1,   597,    -1,   591,
     597,    -1,    -1,   593,    -1,   594,    -1,   593,   594,    -1,
      -1,   237,   595,   596,   430,    -1,   594,    -1,   596,   594,
      -1,   172,   172,    -1,   172,   173,    -1,   172,   174,    -1,
     600,   602,    -1,    -1,   598,   599,    -1,   557,   172,   430,
      -1,    -1,    64,   601,   172,   430,    -1,    -1,   602,   603,
      -1,    33,    54,   430,    -1,    33,   161,   430,    -1,   349,
     174,   174,   430,    -1,   168,   174,   150,   174,   430,    -1,
     110,   659,   659,   430,    -1,   135,   174,    13,   174,   430,
      -1,   111,   174,   430,    -1,    92,   174,   430,    -1,    72,
     174,   430,    -1,    -1,    37,   605,   172,    -1,   607,   609,
     608,    -1,   135,    -1,    37,   135,    -1,    -1,   609,   610,
      -1,   611,   172,   172,   174,   430,    -1,   611,   172,   172,
     174,   137,   430,    -1,   126,    -1,   613,   615,   614,    -1,
      61,    -1,    37,    61,    -1,    -1,   615,   616,    -1,   619,
     617,   430,    -1,    -1,   617,   618,    -1,   174,   174,    -1,
     143,   172,    -1,    75,   174,   174,   430,    -1,    32,   174,
     430,    -1,    -1,    -1,    -1,    81,   623,   172,   624,   627,
     628,   625,   626,    -1,    37,    -1,    37,   172,    -1,    -1,
     384,   430,    -1,    -1,   628,   629,    -1,   636,    -1,   555,
      -1,   606,    -1,   633,    -1,   630,    -1,   631,    -1,   632,
      -1,   398,   172,   430,    -1,   399,   172,   430,    -1,   389,
     172,   174,   430,    -1,    -1,   237,   634,   635,   430,    -1,
     633,    -1,   635,   633,    -1,   172,   172,    -1,   172,   173,
      -1,   172,   174,    -1,    -1,    -1,    -1,    -1,    64,   637,
     172,   638,   168,   174,   430,   639,   641,    37,   640,   172,
      -1,    -1,   641,   642,    -1,   135,   174,   430,    -1,   271,
     174,   430,    -1,   111,   125,   174,   430,    -1,    14,    25,
     174,   430,    -1,    35,   174,   430,    -1,   381,   174,   430,
      -1,   644,   648,   646,    -1,    -1,   131,   645,   172,    -1,
      -1,    37,   647,   172,    -1,    -1,   648,   649,    -1,   132,
     174,    13,   174,   430,    -1,   651,    -1,   650,    -1,   654,
      -1,    16,    97,   430,    -1,    16,    22,   430,    -1,    16,
     238,   430,    -1,   142,   652,   430,    -1,    -1,   652,   653,
      -1,   169,    -1,   170,    -1,   171,    -1,    -1,   395,   655,
     656,   430,    -1,    -1,   656,   657,    -1,    -1,   172,   576,
     658,    -1,   174,   174,    -1,   431,   174,   174,   432,    -1,
      -1,   662,   666,   661,   664,    -1,    -1,    68,   663,   172,
      -1,    -1,    37,   665,   172,    -1,    -1,   667,   666,    -1,
     674,    -1,   679,    -1,   680,    -1,   681,    -1,   670,    -1,
     683,    -1,   682,    -1,   684,    -1,   685,    -1,   687,    -1,
     692,    -1,   689,    -1,   693,    -1,   266,   267,   430,    -1,
     266,   268,   430,    -1,   742,    -1,   744,    -1,   751,    -1,
     753,    -1,    -1,   237,   668,   669,   430,    -1,   673,    -1,
     669,   673,    -1,   142,   671,   430,    -1,    -1,   671,   672,
      -1,   169,    -1,   170,    -1,   171,    -1,   172,   174,    -1,
     172,   173,    -1,   172,   172,    -1,    16,   675,   430,    -1,
      24,    -1,    24,   348,    -1,   113,    -1,    10,    -1,    10,
     356,    -1,    10,   396,    -1,   323,    -1,   348,    -1,    97,
      -1,   238,    -1,    97,   676,    -1,    22,    -1,    23,    -1,
      22,   677,    -1,    38,   678,    -1,    56,    -1,    89,    -1,
      55,    -1,   107,    -1,   134,    -1,   355,    -1,    46,    -1,
     145,    -1,   146,    -1,   134,    -1,   315,    -1,   400,    -1,
     108,    -1,   106,    -1,   152,    -1,   153,    -1,    11,    -1,
      12,    -1,    51,   172,   430,    -1,    50,   172,   172,   430,
      -1,   133,   190,   430,    -1,   133,    50,   430,    -1,   133,
      10,   430,    -1,   107,   174,   430,    -1,    88,   659,   430,
      -1,   574,   430,    -1,   574,   659,   430,    -1,   574,   659,
     576,   430,    -1,   574,   576,   430,    -1,    -1,    36,   686,
     172,   430,    -1,    -1,    66,   688,   172,   430,    -1,   690,
     172,   430,    -1,   690,   733,   430,    -1,   131,    -1,   131,
      -1,   132,   174,    13,   174,   430,    -1,   694,   698,   696,
      -1,    -1,   101,   695,   172,    -1,    -1,    37,   697,   172,
      -1,    -1,   698,   699,    -1,   574,   430,    -1,   574,   659,
     430,    -1,   574,   659,   576,   430,    -1,   574,   141,   430,
      -1,   574,   141,   659,   430,    -1,   574,   141,   659,   576,
     430,    -1,    -1,    66,   700,   172,   430,    -1,   107,   174,
     430,    -1,   713,    -1,   159,   716,   430,    -1,   127,   717,
     430,    -1,    65,   174,   430,    -1,   118,   174,   430,    -1,
      45,   174,   430,    -1,   117,   174,   430,    -1,    44,   174,
     430,    -1,   165,   174,   430,    -1,   162,   174,   430,    -1,
     147,   174,   430,    -1,   128,   718,   430,    -1,    -1,    76,
     701,   172,   430,    -1,    -1,    90,   702,   174,   174,   430,
      -1,    -1,    91,   703,   174,   174,   430,    -1,    -1,    57,
     704,   174,   174,   430,    -1,    14,   174,   430,    -1,    70,
     174,   430,    -1,    71,   174,   430,    -1,   111,   174,   430,
      -1,   109,   174,   430,    -1,    27,     6,   430,    -1,    27,
     112,   430,    -1,   121,   174,   430,    -1,   122,   174,   430,
      -1,    63,   172,   172,   430,    -1,   259,   172,   430,    -1,
      -1,   237,   705,   711,   430,    -1,   714,   715,   719,    37,
      -1,   714,    37,    -1,   261,   174,   842,   430,    -1,   263,
     174,   842,   430,    -1,   262,   174,   842,   430,    -1,   264,
     174,   430,    -1,   265,   174,   430,    -1,   324,   174,   842,
     430,    -1,   325,   174,   842,   430,    -1,   330,   174,   842,
     430,    -1,   327,   174,   842,   430,    -1,   326,   174,   842,
     430,    -1,   328,   174,   844,   430,    -1,   329,   174,   844,
     430,    -1,   331,   174,   844,   430,    -1,    -1,   347,   706,
     710,   430,    -1,    -1,   391,   707,   173,   430,    -1,    -1,
     397,   708,   172,   430,    -1,    -1,   383,   709,   172,   430,
      -1,   361,    -1,   362,    -1,   363,    -1,   364,    -1,   712,
      -1,   711,   712,    -1,   172,   174,    -1,   172,   173,    -1,
     172,   172,    -1,    33,    56,   430,    -1,    33,    89,   430,
      -1,    33,    89,   155,   430,    -1,    33,    55,   430,    -1,
      33,    46,   430,    -1,   105,    -1,    -1,    16,   675,   430,
      -1,   130,    -1,     7,    -1,   107,    -1,    52,    -1,    17,
      -1,    31,    -1,    56,    -1,    89,    -1,   138,    -1,   140,
      -1,    -1,     5,    -1,   113,    -1,    46,    -1,   720,   723,
      -1,    -1,    -1,    64,   721,   172,   722,   724,   725,   430,
      -1,   168,   174,   430,    -1,    98,   726,   728,   430,    -1,
      98,    62,   726,   728,   732,   430,    -1,   110,   659,   659,
     430,    -1,   110,    62,   659,   659,   732,   430,    -1,   104,
     726,   727,   727,   727,   728,   430,    -1,   104,    62,   726,
     727,   727,   727,   728,   732,   430,    -1,   729,    -1,    -1,
     723,   720,    -1,    -1,   411,    -1,    -1,   135,   174,    -1,
      20,   174,    -1,   659,    -1,   659,    -1,    -1,   728,   727,
      -1,    -1,   163,   659,   730,   172,   430,    -1,    -1,   163,
      62,   659,   731,   172,   732,   430,    -1,    34,   174,    13,
     174,   139,   174,   174,    -1,   172,   174,   174,   576,    34,
     174,    13,   174,   139,   174,   174,    -1,   172,   174,   174,
     576,    -1,    -1,    -1,   169,   174,    34,   174,   139,   174,
     735,    64,   736,   739,    -1,    -1,    -1,   170,   174,    34,
     174,   139,   174,   737,    64,   738,   739,    -1,   169,   174,
      34,   174,   139,   174,    -1,   170,   174,    34,   174,   139,
     174,    -1,    -1,   739,   740,    -1,   172,    -1,   169,   174,
      34,   174,   139,   174,    -1,   170,   174,    34,   174,   139,
     174,    -1,   743,   719,    37,    -1,   743,    37,    -1,    84,
      -1,   375,   746,   745,    37,    -1,    -1,   745,   746,    -1,
      -1,    -1,    64,   747,   172,   430,   748,   750,   749,    -1,
      -1,   749,   750,    -1,   110,   659,   659,   174,   430,    -1,
      -1,    18,   752,   172,   430,    -1,   754,   756,   755,    -1,
     149,    -1,    37,   149,    -1,    -1,   756,   757,    -1,    -1,
      49,   758,   772,   430,    -1,    -1,   151,   759,   773,   430,
      -1,    -1,   770,    59,   174,   174,   760,   769,   160,   174,
     174,   430,    -1,   770,   765,   157,   771,   227,   174,   174,
     174,   430,    -1,   228,   768,   430,    -1,   229,   766,   430,
      -1,   116,   174,   174,   430,    -1,    43,   174,   174,   430,
      -1,   115,   174,   174,   430,    -1,    40,   174,   174,   430,
      -1,   119,   174,   174,   430,    -1,    42,   174,   174,   430,
      -1,   120,   174,   174,   430,    -1,    41,   174,   174,   430,
      -1,   157,   771,   430,    -1,   206,   207,   174,   208,   174,
     770,   430,    -1,   762,   763,   764,   227,   174,   174,   174,
     430,    -1,   761,   227,   174,   174,   174,   430,    -1,   284,
     173,   430,    -1,   285,   173,   430,    -1,   286,   173,   430,
      -1,   231,   430,    -1,   287,    -1,   288,    -1,   289,    -1,
     207,    -1,   208,    -1,   279,    -1,   280,    -1,   281,    -1,
     282,    -1,   283,    -1,   281,    -1,   282,    -1,   283,    -1,
     226,    -1,   230,    -1,   767,    -1,   766,   767,    -1,   431,
     174,   174,   174,   432,    -1,   174,    -1,   768,   174,    -1,
      -1,   174,   174,   174,   174,    -1,   174,   174,   174,   174,
     174,   174,   174,    -1,   114,    -1,    39,    -1,    60,    -1,
      82,    -1,    83,    -1,   172,    -1,   772,   172,    -1,   172,
      -1,   773,   172,    -1,    -1,   776,   780,   775,   778,    -1,
      -1,     8,   777,   172,    -1,    -1,    37,   779,   172,    -1,
      -1,   780,   781,    -1,    -1,   691,   782,   733,   430,    -1,
      -1,   198,   783,   733,   430,    -1,    -1,   199,   784,   733,
     430,    -1,    -1,   200,   785,   734,   430,    -1,   787,   788,
      37,   172,    -1,    -1,   202,   786,   741,   430,    -1,   203,
     174,   792,    37,   203,    -1,   798,    -1,   201,   172,    -1,
      -1,   788,   789,    -1,    -1,   198,   790,   733,   430,    -1,
      -1,   199,   791,   733,   430,    -1,    -1,   792,   793,    -1,
     204,   174,   205,   174,   430,    -1,    -1,   272,   795,   172,
     433,   807,   802,    -1,    -1,   273,   797,   172,   433,   807,
     802,    -1,    -1,   209,   799,   172,   433,   805,   802,    -1,
      -1,   210,   800,   172,   433,   807,   802,    -1,    -1,   211,
     801,   172,   433,   806,   802,    -1,    -1,   430,    -1,   434,
      -1,   213,    -1,   434,   213,    -1,   214,    -1,   434,   214,
      -1,   805,   426,   805,    -1,   805,   425,   805,    -1,   805,
     427,   805,    -1,   805,   428,   805,    -1,   425,   805,    -1,
     431,   805,   432,    -1,   212,   806,   803,   805,   804,   805,
      -1,   174,    -1,   805,   808,   805,    -1,   805,   224,   805,
      -1,   805,   223,   805,    -1,   807,   808,   807,    -1,   807,
     224,   807,    -1,   807,   223,   807,    -1,   806,   217,   806,
      -1,   806,   218,   806,    -1,   806,   224,   806,    -1,   806,
     223,   806,    -1,   225,   806,    -1,   431,   806,   432,    -1,
     212,   806,   803,   806,   804,   806,    -1,   216,    -1,   215,
      -1,   807,   426,   807,    -1,   431,   807,   432,    -1,   212,
     806,   803,   807,   804,   807,    -1,   173,    -1,   219,    -1,
     220,    -1,   221,    -1,   222,    -1,   217,    -1,   218,    -1,
     433,    -1,   435,    -1,   436,    -1,    -1,   232,   810,   811,
      37,   232,    -1,    -1,   811,   812,    -1,    -1,    67,   813,
     172,   821,   430,    -1,    -1,    58,   814,   172,   821,   430,
      -1,    -1,   101,   815,   172,   821,   430,    -1,    -1,    68,
     816,   172,   821,   430,    -1,    -1,   163,   817,   172,   821,
     430,    -1,    -1,   164,   818,   172,   821,   430,    -1,    -1,
      64,   819,   172,   821,   430,    -1,    -1,    81,   820,   172,
     821,   430,    -1,    -1,   234,   822,   829,   830,    -1,    -1,
     235,   823,   829,   830,    -1,   233,    -1,   233,   173,    -1,
     269,   172,    -1,    -1,   298,    -1,    21,   174,    -1,    21,
     174,   236,   174,   174,    -1,   236,   174,   174,    -1,    -1,
      -1,   417,   174,   369,   174,   826,   827,    -1,    -1,   421,
      -1,    -1,   419,    -1,    -1,   236,   174,   174,    -1,    -1,
     174,    -1,    -1,   832,   831,    -1,   372,    -1,    -1,   126,
     833,   828,    -1,   418,    -1,    -1,    -1,    -1,    64,   835,
     172,   836,   840,    -1,    -1,   346,   174,   369,   174,   837,
     841,    -1,     9,   174,    -1,    -1,   236,   174,   174,   838,
     824,    -1,   299,   174,    -1,   299,   174,   236,   174,   174,
      -1,    -1,   407,   174,   369,   174,   839,   825,    -1,   414,
     174,    -1,   408,   174,   415,   174,   414,   174,    -1,    -1,
     137,    -1,    -1,   410,    -1,    -1,    -1,    64,   843,   172,
      -1,    -1,    64,   845,   172,    -1,   243,   174,   174,   430,
      -1,   244,   174,   430,    -1,   247,   174,   430,    -1,   246,
     174,   430,    -1,    -1,   249,   174,   851,   430,   853,   852,
     802,    -1,    37,   249,    -1,   854,    -1,   853,   854,    -1,
     250,   174,   430,    -1,   855,    -1,    -1,    91,   856,   857,
     430,   858,    -1,   174,    -1,   857,   174,    -1,   859,    -1,
     858,   859,    -1,    -1,   251,   174,   430,   860,   248,   861,
     430,    -1,   174,    -1,   861,   174,    -1,    -1,   245,   174,
     430,   863,   865,   864,   802,    -1,    37,   245,    -1,   866,
      -1,   865,   866,    -1,   250,   174,   430,    -1,   867,    -1,
      -1,    91,   868,   869,   430,   870,    -1,   174,    -1,   869,
     174,    -1,   871,    -1,   870,   871,    -1,    -1,   251,   174,
     430,   872,   252,   873,   430,    -1,   174,    -1,   873,   174,
      -1,   254,   174,   430,    -1,   253,   174,   430,    -1,   255,
     174,   430,    -1,   300,   174,   430,    -1,   301,   174,   430,
      -1,   302,   174,   430,    -1,    -1,   881,    -1,   241,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,   507,   507,   562,   562,   598,   611,   624,   625,   626,
     630,   636,   663,   663,   663,   663,   664,   664,   664,   664,
     664,   665,   665,   666,   666,   666,   666,   666,   666,   666,
     667,   667,   667,   668,   668,   669,   669,   670,   670,   670,
     671,   671,   672,   672,   672,   672,   673,   673,   673,   674,
     677,   690,   708,   718,   729,   736,   751,   755,   756,   759,
     760,   763,   764,   766,   772,   783,   784,   787,   789,   791,
     793,   795,   797,   799,   806,   809,   816,   816,   847,   847,
     903,   907,   912,   918,   911,   935,   941,   946,   951,   955,
     959,   963,   967,   971,   975,   980,   985,   984,  1021,  1020,
    1038,  1052,  1064,  1075,  1087,  1098,  1110,  1122,  1134,  1146,
    1158,  1183,  1210,  1222,  1233,  1245,  1245,  1251,  1250,  1263,
    1278,  1294,  1317,  1293,  1320,  1343,  1319,  1347,  1386,  1385,
    1424,  1463,  1462,  1502,  1501,  1516,  1555,  1554,  1593,  1632,
    1631,  1671,  1670,  1711,  1710,  1749,  1770,  1791,  1813,  1830,
    1812,  1844,  1867,  1890,  1913,  1936,  1959,  1982,  2005,  2027,
    2045,  2063,  2081,  2099,  2117,  2142,  2168,  2167,  2185,  2184,
    2199,  2198,  2205,  2222,  2221,  2248,  2247,  2268,  2267,  2286,
    2304,  2330,  2329,  2348,  2350,  2356,  2358,  2364,  2366,  2369,
    2377,  2398,  2403,  2409,  2376,  2423,  2427,  2422,  2459,  2458,
    2483,  2485,  2488,  2495,  2496,  2497,  2499,  2501,  2500,  2507,
    2523,  2524,  2540,  2541,  2548,  2549,  2565,  2569,  2572,  2591,
    2609,  2627,  2631,  2634,  2638,  2642,  2657,  2658,  2659,  2662,
    2666,  2665,  2678,  2682,  2686,  2691,  2692,  2726,  2727,  2728,
    2732,  2734,  2737,  2731,  2741,  2743,  2742,  2757,  2756,  2773,
    2772,  2777,  2778,  2782,  2783,  2787,  2795,  2803,  2815,  2817,
    2820,  2824,  2825,  2828,  2832,  2833,  2836,  2840,  2841,  2842,
    2843,  2844,  2845,  2848,  2849,  2850,  2851,  2853,  2854,  2861,
    2866,  2871,  2876,  2883,  2887,  2891,  2890,  2900,  2901,  2916,
    2915,  2926,  2927,  2931,  2932,  2935,  2938,  2974,  2974,  3001,
    3001,  3007,  3010,  3019,  3027,  3036,  3038,  3041,  3036,  3063,
    3064,  3067,  3071,  3075,  3079,  3079,  3085,  3086,  3088,  3091,
    3092,  3096,  3098,  3100,  3102,  3102,  3106,  3117,  3118,  3122,
    3132,  3140,  3150,  3159,  3168,  3177,  3187,  3187,  3191,  3192,
    3193,  3194,  3195,  3196,  3197,  3198,  3199,  3200,  3201,  3202,
    3203,  3204,  3205,  3206,  3208,  3211,  3211,  3218,  3220,  3224,
    3228,  3227,  3243,  3243,  3277,  3277,  3287,  3314,  3313,  3349,
    3350,  3368,  3369,  3372,  3374,  3378,  3379,  3382,  3382,  3388,
    3389,  3393,  3401,  3409,  3420,  3451,  3453,  3456,  3459,  3459,
    3465,  3467,  3471,  3490,  3509,  3545,  3547,  3550,  3552,  3554,
    3584,  3612,  3612,  3632,  3635,  3647,  3659,  3661,  3664,  3673,
    3683,  3687,  3690,  3701,  3710,  3712,  3715,  3723,  3725,  3728,
    3731,  3734,  3747,  3758,  3759,  3769,  3758,  3799,  3804,  3825,
    3827,  3846,  3847,  3851,  3852,  3853,  3854,  3855,  3856,  3857,
    3860,  3877,  3896,  3915,  3915,  3921,  3922,  3926,  3934,  3942,
    3953,  3954,  3963,  3967,  3953,  4004,  4006,  4010,  4015,  4018,
    4042,  4065,  4088,  4107,  4113,  4113,  4122,  4122,  4153,  4155,
    4159,  4166,  4168,  4173,  4177,  4178,  4179,  4181,  4184,  4186,
    4190,  4192,  4194,  4197,  4197,  4201,  4203,  4206,  4206,  4210,
    4212,  4216,  4215,  4223,  4223,  4241,  4241,  4263,  4268,  4272,
    4273,  4274,  4275,  4276,  4277,  4279,  4281,  4283,  4284,  4285,
    4287,  4289,  4291,  4293,  4295,  4297,  4299,  4301,  4303,  4303,
    4309,  4310,  4313,  4324,  4326,  4330,  4332,  4334,  4338,  4348,
    4356,  4365,  4373,  4374,  4393,  4394,  4395,  4414,  4433,  4434,
    4446,  4447,  4448,  4471,  4472,  4479,  4482,  4488,  4489,  4490,
    4491,  4492,  4493,  4496,  4497,  4498,  4499,  4518,  4537,  4558,
    4559,  4560,  4561,  4562,  4563,  4565,  4568,  4572,  4581,  4590,
    4600,  4610,  4660,  4664,  4668,  4672,  4677,  4677,  4680,  4680,
    4691,  4703,  4718,  4722,  4725,  4746,  4753,  4753,  4759,  4759,
    4781,  4782,  4786,  4795,  4804,  4813,  4822,  4831,  4840,  4840,
    4849,  4858,  4860,  4862,  4864,  4873,  4882,  4891,  4900,  4909,
    4918,  4927,  4936,  4938,  4938,  4940,  4940,  4949,  4949,  4958,
    4958,  4967,  4976,  4978,  4980,  4989,  4998,  5007,  5016,  5025,
    5034,  5043,  5045,  5045,  5049,  5063,  5078,  5099,  5120,  5141,
    5143,  5145,  5175,  5205,  5235,  5265,  5295,  5325,  5355,  5386,
    5385,  5415,  5415,  5431,  5431,  5447,  5447,  5465,  5470,  5475,
    5480,  5487,  5488,  5492,  5502,  5510,  5520,  5521,  5522,  5523,
    5524,  5526,  5538,  5539,  5544,  5545,  5546,  5547,  5548,  5549,
    5552,  5553,  5554,  5555,  5558,  5559,  5560,  5561,  5563,  5566,
    5567,  5566,  5583,  5595,  5608,  5621,  5633,  5646,  5660,  5673,
    5676,  5677,  5679,  5680,  5695,  5696,  5710,  5725,  5729,  5733,
    5738,  5742,  5742,  5745,  5745,  5751,  5755,  5766,  5779,  5787,
    5778,  5790,  5798,  5789,  5800,  5809,  5819,  5821,  5824,  5827,
    5836,  5846,  5857,  5874,  5886,  5907,  5908,  5911,  5912,  5911,
    5918,  5919,  5922,  5928,  5928,  5931,  5934,  5937,  5951,  5953,
    5958,  5957,  5968,  5968,  5971,  5970,  5974,  5983,  5985,  5987,
    5989,  5991,  5993,  5995,  5997,  5999,  6001,  6003,  6005,  6007,
    6009,  6011,  6013,  6015,  6017,  6021,  6023,  6025,  6029,  6031,
    6033,  6035,  6039,  6041,  6043,  6047,  6049,  6051,  6055,  6057,
    6061,  6063,  6066,  6070,  6072,  6077,  6078,  6080,  6085,  6087,
    6091,  6093,  6095,  6099,  6101,  6105,  6107,  6111,  6110,  6120,
    6120,  6130,  6130,  6154,  6155,  6159,  6159,  6166,  6166,  6173,
    6173,  6180,  6180,  6186,  6189,  6189,  6195,  6201,  6204,  6209,
    6210,  6214,  6214,  6221,  6221,  6231,  6232,  6235,  6239,  6239,
    6243,  6243,  6247,  6247,  6256,  6256,  6265,  6265,  6276,  6277,
    6278,  6281,  6282,  6286,  6287,  6291,  6292,  6293,  6294,  6295,
    6296,  6297,  6299,  6302,  6303,  6304,  6305,  6306,  6307,  6308,
    6309,  6310,  6311,  6312,  6313,  6314,  6316,  6317,  6320,  6326,
    6328,  6337,  6341,  6342,  6343,  6344,  6345,  6346,  6347,  6348,
    6349,  6353,  6352,  6368,  6369,  6373,  6373,  6382,  6382,  6391,
    6391,  6401,  6401,  6410,  6410,  6419,  6419,  6428,  6428,  6437,
    6437,  6448,  6448,  6453,  6453,  6459,  6464,  6469,  6477,  6478,
    6483,  6490,  6497,  6505,  6507,  6506,  6515,  6516,  6524,  6525,
    6533,  6534,  6539,  6540,  6543,  6545,  6547,  6572,  6571,  6596,
    6620,  6622,  6623,  6622,  6639,  6638,  6663,  6687,  6686,  6692,
    6698,  6706,  6705,  6721,  6735,  6752,  6753,  6761,  6762,  6779,
    6780,  6780,  6785,  6785,  6789,  6803,  6816,  6829,  6843,  6842,
    6848,  6861,  6862,  6866,  6873,  6877,  6876,  6882,  6885,  6890,
    6891,  6895,  6894,  6901,  6904,  6909,  6908,  6915,  6928,  6929,
    6933,  6940,  6944,  6943,  6950,  6953,  6958,  6959,  6964,  6963,
    6970,  6973,  6979,  7003,  7027,  7051,  7085,  7119,  7153,  7154,
    7156
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "K_HISTORY", "K_ABUT", "K_ABUTMENT",
  "K_ACTIVE", "K_ANALOG", "K_ARRAY", "K_AREA", "K_BLOCK", "K_BOTTOMLEFT",
  "K_BOTTOMRIGHT", "K_BY", "K_CAPACITANCE", "K_CAPMULTIPLIER", "K_CLASS",
  "K_CLOCK", "K_CLOCKTYPE", "K_COLUMNMAJOR", "K_DESIGNRULEWIDTH",
  "K_INFLUENCE", "K_CORE", "K_CORNER", "K_COVER", "K_CPERSQDIST",
  "K_CURRENT", "K_CURRENTSOURCE", "K_CUT", "K_DEFAULT", "K_DATABASE",
  "K_DATA", "K_DIELECTRIC", "K_DIRECTION", "K_DO", "K_EDGECAPACITANCE",
  "K_EEQ", "K_END", "K_ENDCAP", "K_FALL", "K_FALLCS", "K_FALLT0",
  "K_FALLSATT1", "K_FALLRS", "K_FALLSATCUR", "K_FALLTHRESH", "K_FEEDTHRU",
  "K_FIXED", "K_FOREIGN", "K_FROMPIN", "K_GENERATE", "K_GENERATOR",
  "K_GROUND", "K_HEIGHT", "K_HORIZONTAL", "K_INOUT", "K_INPUT",
  "K_INPUTNOISEMARGIN", "K_COMPONENTPIN", "K_INTRINSIC", "K_INVERT",
  "K_IRDROP", "K_ITERATE", "K_IV_TABLES", "K_LAYER", "K_LEAKAGE", "K_LEQ",
  "K_LIBRARY", "K_MACRO", "K_MATCH", "K_MAXDELAY", "K_MAXLOAD",
  "K_METALOVERHANG", "K_MILLIAMPS", "K_MILLIWATTS", "K_MINFEATURE",
  "K_MUSTJOIN", "K_NAMESCASESENSITIVE", "K_NANOSECONDS", "K_NETS", "K_NEW",
  "K_NONDEFAULTRULE", "K_NONINVERT", "K_NONUNATE", "K_OBS", "K_OHMS",
  "K_OFFSET", "K_ORIENTATION", "K_ORIGIN", "K_OUTPUT",
  "K_OUTPUTNOISEMARGIN", "K_OUTPUTRESISTANCE", "K_OVERHANG", "K_OVERLAP",
  "K_OFF", "K_ON", "K_OVERLAPS", "K_PAD", "K_PATH", "K_PATTERN",
  "K_PICOFARADS", "K_PIN", "K_PITCH", "K_PLACED", "K_POLYGON", "K_PORT",
  "K_POST", "K_POWER", "K_PRE", "K_PULLDOWNRES", "K_RECT", "K_RESISTANCE",
  "K_RESISTIVE", "K_RING", "K_RISE", "K_RISECS", "K_RISERS",
  "K_RISESATCUR", "K_RISETHRESH", "K_RISESATT1", "K_RISET0",
  "K_RISEVOLTAGETHRESHOLD", "K_FALLVOLTAGETHRESHOLD", "K_ROUTING",
  "K_ROWMAJOR", "K_RPERSQ", "K_SAMENET", "K_SCANUSE", "K_SHAPE",
  "K_SHRINKAGE", "K_SIGNAL", "K_SITE", "K_SIZE", "K_SOURCE", "K_SPACER",
  "K_SPACING", "K_SPECIALNETS", "K_STACK", "K_START", "K_STEP", "K_STOP",
  "K_STRUCTURE", "K_SYMMETRY", "K_TABLE", "K_THICKNESS", "K_TIEHIGH",
  "K_TIELOW", "K_TIEOFFR", "K_TIME", "K_TIMING", "K_TO", "K_TOPIN",
  "K_TOPLEFT", "K_TOPRIGHT", "K_TOPOFSTACKONLY", "K_TRISTATE", "K_TYPE",
  "K_UNATENESS", "K_UNITS", "K_USE", "K_VARIABLE", "K_VERTICAL", "K_VHI",
  "K_VIA", "K_VIARULE", "K_VLO", "K_VOLTAGE", "K_VOLTS", "K_WIDTH", "K_X",
  "K_Y", "K_R90", "T_STRING", "QSTRING", "NUMBER", "K_N", "K_S", "K_E",
  "K_W", "K_FN", "K_FS", "K_FE", "K_FW", "K_R0", "K_R180", "K_R270",
  "K_MX", "K_MY", "K_MXR90", "K_MYR90", "K_USER", "K_MASTERSLICE",
  "K_ENDMACRO", "K_ENDMACROPIN", "K_ENDVIARULE", "K_ENDVIA", "K_ENDLAYER",
  "K_ENDSITE", "K_CANPLACE", "K_CANNOTOCCUPY", "K_TRACKS", "K_FLOORPLAN",
  "K_GCELLGRID", "K_DEFAULTCAP", "K_MINPINS", "K_WIRECAP", "K_STABLE",
  "K_SETUP", "K_HOLD", "K_DEFINE", "K_DEFINES", "K_DEFINEB", "K_IF",
  "K_THEN", "K_ELSE", "K_FALSE", "K_TRUE", "K_EQ", "K_NE", "K_LE", "K_LT",
  "K_GE", "K_GT", "K_OR", "K_AND", "K_NOT", "K_DELAY", "K_TABLEDIMENSION",
  "K_TABLEAXIS", "K_TABLEENTRIES", "K_TRANSITIONTIME", "K_EXTENSION",
  "K_PROPDEF", "K_STRING", "K_INTEGER", "K_REAL", "K_RANGE", "K_PROPERTY",
  "K_VIRTUAL", "K_BUSBITCHARS", "K_VERSION", "K_BEGINEXT", "K_ENDEXT",
  "K_UNIVERSALNOISEMARGIN", "K_EDGERATETHRESHOLD1", "K_CORRECTIONTABLE",
  "K_EDGERATESCALEFACTOR", "K_EDGERATETHRESHOLD2", "K_VICTIMNOISE",
  "K_NOISETABLE", "K_EDGERATE", "K_VICTIMLENGTH", "K_CORRECTIONFACTOR",
  "K_OUTPUTPINANTENNASIZE", "K_INPUTPINANTENNASIZE",
  "K_INOUTPINANTENNASIZE", "K_CURRENTDEN", "K_PWL",
  "K_ANTENNALENGTHFACTOR", "K_TAPERRULE", "K_DIVIDERCHAR", "K_ANTENNASIZE",
  "K_ANTENNAMETALLENGTH", "K_ANTENNAMETALAREA", "K_RISESLEWLIMIT",
  "K_FALLSLEWLIMIT", "K_FUNCTION", "K_BUFFER", "K_INVERTER",
  "K_NAMEMAPSTRING", "K_NOWIREEXTENSIONATPIN", "K_WIREEXTENSION",
  "K_MESSAGE", "K_CREATEFILE", "K_OPENFILE", "K_CLOSEFILE", "K_WARNING",
  "K_ERROR", "K_FATALERROR", "K_RECOVERY", "K_SKEW", "K_ANYEDGE",
  "K_POSEDGE", "K_NEGEDGE", "K_SDFCONDSTART", "K_SDFCONDEND", "K_SDFCOND",
  "K_MPWH", "K_MPWL", "K_PERIOD", "K_ACCURRENTDENSITY",
  "K_DCCURRENTDENSITY", "K_AVERAGE", "K_PEAK", "K_RMS", "K_FREQUENCY",
  "K_CUTAREA", "K_MEGAHERTZ", "K_USELENGTHTHRESHOLD", "K_LENGTHTHRESHOLD",
  "K_ANTENNAINPUTGATEAREA", "K_ANTENNAINOUTDIFFAREA",
  "K_ANTENNAOUTPUTDIFFAREA", "K_ANTENNAAREARATIO",
  "K_ANTENNADIFFAREARATIO", "K_ANTENNACUMAREARATIO",
  "K_ANTENNACUMDIFFAREARATIO", "K_ANTENNAAREAFACTOR",
  "K_ANTENNASIDEAREARATIO", "K_ANTENNADIFFSIDEAREARATIO",
  "K_ANTENNACUMSIDEAREARATIO", "K_ANTENNACUMDIFFSIDEAREARATIO",
  "K_ANTENNASIDEAREAFACTOR", "K_DIFFUSEONLY", "K_MANUFACTURINGGRID",
  "K_ANTENNACELL", "K_CLEARANCEMEASURE", "K_EUCLIDEAN", "K_MAXXY",
  "K_USEMINSPACING", "K_ROWMINSPACING", "K_ROWABUTSPACING", "K_FLIP",
  "K_NONE", "K_ANTENNAPARTIALMETALAREA", "K_ANTENNAPARTIALMETALSIDEAREA",
  "K_ANTENNAGATEAREA", "K_ANTENNADIFFAREA", "K_ANTENNAMAXAREACAR",
  "K_ANTENNAMAXSIDEAREACAR", "K_ANTENNAPARTIALCUTAREA",
  "K_ANTENNAMAXCUTCAR", "K_SLOTWIREWIDTH", "K_SLOTWIRELENGTH",
  "K_SLOTWIDTH", "K_SLOTLENGTH", "K_MAXADJACENTSLOTSPACING",
  "K_MAXCOAXIALSLOTSPACING", "K_MAXEDGESLOTSPACING", "K_SPLITWIREWIDTH",
  "K_MINIMUMDENSITY", "K_MAXIMUMDENSITY", "K_DENSITYCHECKWINDOW",
  "K_DENSITYCHECKSTEP", "K_FILLACTIVESPACING", "K_MINIMUMCUT",
  "K_ADJACENTCUTS", "K_ANTENNAMODEL", "K_BUMP", "K_ENCLOSURE",
  "K_FROMABOVE", "K_FROMBELOW", "K_IMPLANT", "K_LENGTH", "K_MAXVIASTACK",
  "K_AREAIO", "K_BLACKBOX", "K_MAXWIDTH", "K_MINENCLOSEDAREA", "K_MINSTEP",
  "K_ORIENT", "K_OXIDE1", "K_OXIDE2", "K_OXIDE3", "K_OXIDE4",
  "K_PARALLELRUNLENGTH", "K_MINWIDTH", "K_PROTRUSIONWIDTH",
  "K_SPACINGTABLE", "K_WITHIN", "K_ABOVE", "K_BELOW", "K_CENTERTOCENTER",
  "K_CUTSIZE", "K_CUTSPACING", "K_DENSITY", "K_DIAG45", "K_DIAG135",
  "K_DIAGMINEDGELENGTH", "K_DIAGSPACING", "K_DIAGPITCH", "K_DIAGWIDTH",
  "K_GENERATED", "K_GROUNDSENSITIVITY", "K_HARDSPACING", "K_INSIDECORNER",
  "K_LAYERS", "K_LENGTHSUM", "K_MICRONS", "K_MINCUTS", "K_MINSIZE",
  "K_NETEXPR", "K_OUTSIDECORNER", "K_PREFERENCLOSURE", "K_ROWCOL",
  "K_ROWPATTERN", "K_SOFT", "K_SUPPLYSENSITIVITY", "K_USEVIA",
  "K_USEVIARULE", "K_WELLTAP", "K_ARRAYCUTS", "K_ARRAYSPACING",
  "K_ANTENNAAREADIFFREDUCEPWL", "K_ANTENNAAREAMINUSDIFF",
  "K_ANTENNACUMROUTINGPLUSCUT", "K_ANTENNAGATEPLUSDIFF", "K_ENDOFLINE",
  "K_ENDOFNOTCHWIDTH", "K_EXCEPTEXTRACUT", "K_EXCEPTSAMEPGNET",
  "K_EXCEPTPGNET", "K_LONGARRAY", "K_MAXEDGES", "K_NOTCHLENGTH",
  "K_NOTCHSPACING", "K_ORTHOGONAL", "K_PARALLELEDGE", "K_PARALLELOVERLAP",
  "K_PGONLY", "K_PRL", "K_TWOEDGES", "K_TWOWIDTHS", "IF", "LNOT", "'-'",
  "'+'", "'*'", "'/'", "UMINUS", "';'", "'('", "')'", "'='", "'\\n'",
  "'<'", "'>'", "$accept", "lef_file", "version", "@1", "dividerchar",
  "busbitchars", "rules", "end_library", "rule", "case_sensitivity",
  "wireextension", "manufacturing", "useminspacing", "clearancemeasure",
  "clearance_type", "spacing_type", "spacing_value", "units_section",
  "start_units", "units_rules", "units_rule", "layer_rule", "start_layer",
  "@2", "end_layer", "@3", "layer_options", "layer_option", "@4", "@5",
  "@6", "@7", "@8", "@9", "@10", "@11", "@12", "@13", "@14", "@15", "@16",
  "@17", "@18", "@19", "@20", "@21", "@22", "@23", "@24", "@25", "@26",
  "@27", "@28", "@29", "layer_arraySpacing_long",
  "layer_arraySpacing_width", "layer_arraySpacing_arraycuts",
  "layer_arraySpacing_arraycut", "sp_options", "@30", "@31", "@32", "@33",
  "@34", "@35", "@36", "layer_spacingtable_opts", "layer_spacingtable_opt",
  "layer_enclosure_type_opt", "layer_enclosure_width_opt", "@37",
  "layer_enclosure_width_except_opt", "layer_preferenclosure_width_opt",
  "layer_minimumcut_within", "layer_minimumcut_options",
  "layer_minimumcut_option", "layer_minstep_options",
  "layer_minstep_option", "layer_minstep_type", "layer_antenna_pwl", "@38",
  "layer_diffusion_ratios", "layer_diffusion_ratio", "layer_antenna_duo",
  "layer_table_type", "layer_frequency", "@39", "@40", "@41",
  "ac_layer_table_opt", "@42", "@43", "dc_layer_table", "@44",
  "number_list", "layer_prop_list", "layer_prop",
  "current_density_pwl_list", "current_density_pwl", "cap_points",
  "cap_point", "res_points", "res_point", "layer_type", "layer_direction",
  "layer_minen_width", "layer_oxide", "layer_sp_parallel_widths",
  "layer_sp_parallel_width", "@45", "layer_sp_TwoWidths",
  "layer_sp_TwoWidth", "@46", "layer_sp_TwoWidthsPRL",
  "layer_sp_influence_widths", "layer_sp_influence_width", "maxstack_via",
  "@47", "via", "@48", "via_keyword", "start_via", "via_viarule", "@49",
  "@50", "@51", "via_viarule_options", "via_viarule_option", "@52",
  "via_option", "via_other_options", "via_more_options",
  "via_other_option", "@53", "via_prop_list", "via_name_value_pair",
  "via_foreign", "start_foreign", "@54", "orientation", "via_layer_rule",
  "via_layer", "@55", "via_geometries", "via_geometry", "@56", "end_via",
  "@57", "viarule_keyword", "@58", "viarule", "viarule_generate", "@59",
  "viarule_generate_default", "viarule_layer_list", "opt_viarule_props",
  "viarule_props", "viarule_prop", "@60", "viarule_prop_list",
  "viarule_layer", "via_names", "via_name", "viarule_layer_name", "@61",
  "viarule_layer_options", "viarule_layer_option", "end_viarule", "@62",
  "spacing_rule", "start_spacing", "end_spacing", "spacings", "spacing",
  "samenet_keyword", "irdrop", "start_irdrop", "end_irdrop", "ir_tables",
  "ir_table", "ir_table_values", "ir_table_value", "ir_tablename",
  "minfeature", "dielectric", "nondefault_rule", "@63", "@64", "@65",
  "end_nd_rule", "nd_hardspacing", "nd_rules", "nd_rule", "usevia",
  "useviarule", "mincuts", "nd_prop", "@66", "nd_prop_list", "nd_layer",
  "@67", "@68", "@69", "@70", "nd_layer_stmts", "nd_layer_stmt", "site",
  "start_site", "@71", "end_site", "@72", "site_options", "site_option",
  "site_class", "site_symmetry_statement", "site_symmetries",
  "site_symmetry", "site_rowpattern_statement", "@73", "site_rowpatterns",
  "site_rowpattern", "@74", "pt", "macro", "@75", "start_macro", "@76",
  "end_macro", "@77", "macro_options", "macro_option", "@78",
  "macro_prop_list", "macro_symmetry_statement", "macro_symmetries",
  "macro_symmetry", "macro_name_value_pair", "macro_class", "class_type",
  "pad_type", "core_type", "endcap_type", "macro_generator",
  "macro_generate", "macro_source", "macro_power", "macro_origin",
  "macro_foreign", "macro_eeq", "@79", "macro_leq", "@80", "macro_site",
  "macro_site_word", "site_word", "macro_size", "macro_pin",
  "start_macro_pin", "@81", "end_macro_pin", "@82", "macro_pin_options",
  "macro_pin_option", "@83", "@84", "@85", "@86", "@87", "@88", "@89",
  "@90", "@91", "@92", "pin_layer_oxide", "pin_prop_list",
  "pin_name_value_pair", "electrical_direction", "start_macro_port",
  "macro_port_class_option", "macro_pin_use", "macro_scan_use",
  "pin_shape", "geometries", "geometry", "@93", "@94", "geometry_options",
  "layer_exceptpgnet", "layer_spacing", "firstPt", "nextPt", "otherPts",
  "via_placement", "@95", "@96", "stepPattern", "sitePattern",
  "trackPattern", "@97", "@98", "@99", "@100", "trackLayers", "layer_name",
  "gcellPattern", "macro_obs", "start_macro_obs", "macro_density",
  "density_layers", "density_layer", "@101", "@102", "density_layer_rects",
  "density_layer_rect", "macro_clocktype", "@103", "timing",
  "start_timing", "end_timing", "timing_options", "timing_option", "@104",
  "@105", "@106", "one_pin_trigger", "two_pin_trigger", "from_pin_trigger",
  "to_pin_trigger", "delay_or_transition", "list_of_table_entries",
  "table_entry", "list_of_table_axis_numbers", "slew_spec", "risefall",
  "unateness", "list_of_from_strings", "list_of_to_strings", "array",
  "@107", "start_array", "@108", "end_array", "@109", "array_rules",
  "array_rule", "@110", "@111", "@112", "@113", "@114", "floorplan_start",
  "floorplan_list", "floorplan_element", "@115", "@116", "cap_list",
  "one_cap", "msg_statement", "@117", "create_file_statement", "@118",
  "def_statement", "@119", "@120", "@121", "dtrm", "then", "else",
  "expression", "b_expr", "s_expr", "relop", "prop_def_section", "@122",
  "prop_stmts", "prop_stmt", "@123", "@124", "@125", "@126", "@127",
  "@128", "@129", "@130", "prop_define", "@131", "@132",
  "opt_range_second", "opt_endofline", "@133", "opt_endofline_twoedges",
  "opt_samenetPGonly", "opt_def_range", "opt_def_value",
  "layer_spacing_opts", "layer_spacing_opt", "@134",
  "layer_spacing_cut_routing", "@135", "@136", "@137", "@138", "@139",
  "spacing_cut_layer_opt", "opt_adjacentcuts_exceptsame", "opt_layer_name",
  "@140", "req_layer_name", "@141", "universalnoisemargin",
  "edgeratethreshold1", "edgeratethreshold2", "edgeratescalefactor",
  "noisetable", "@142", "end_noisetable", "noise_table_list",
  "noise_table_entry", "output_resistance_entry", "@143", "num_list",
  "victim_list", "victim", "@144", "vnoiselist", "correctiontable", "@145",
  "end_correctiontable", "correction_table_list", "correction_table_item",
  "output_list", "@146", "numo_list", "corr_victim_list", "corr_victim",
  "@147", "corr_list", "input_antenna", "output_antenna", "inout_antenna",
  "antenna_input", "antenna_inout", "antenna_output", "extension_opt",
  "extension", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short int yytoknum[] =
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
     635,   636,   637,   638,   639,   640,   641,   642,   643,   644,
     645,   646,   647,   648,   649,   650,   651,   652,   653,   654,
     655,   656,   657,   658,   659,   660,   661,   662,   663,   664,
     665,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,   677,   678,   679,    45,    43,    42,    47,   680,
      59,    40,    41,    61,    10,    60,    62
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned short int yyr1[] =
{
       0,   437,   438,   440,   439,   441,   442,   443,   443,   443,
     444,   444,   445,   445,   445,   445,   445,   445,   445,   445,
     445,   445,   445,   445,   445,   445,   445,   445,   445,   445,
     445,   445,   445,   445,   445,   445,   445,   445,   445,   445,
     445,   445,   445,   445,   445,   445,   445,   445,   445,   445,
     446,   446,   447,   447,   448,   449,   450,   451,   451,   452,
     452,   453,   453,   454,   455,   456,   456,   457,   457,   457,
     457,   457,   457,   457,   457,   458,   460,   459,   462,   461,
     463,   463,   465,   466,   464,   464,   464,   464,   464,   464,
     464,   464,   464,   464,   464,   464,   467,   464,   468,   464,
     464,   464,   464,   464,   464,   464,   464,   464,   464,   464,
     464,   464,   464,   464,   464,   469,   464,   470,   464,   464,
     464,   471,   472,   464,   473,   474,   464,   464,   475,   464,
     464,   476,   464,   477,   464,   464,   478,   464,   464,   479,
     464,   480,   464,   481,   464,   464,   464,   464,   482,   483,
     464,   464,   464,   464,   464,   464,   464,   464,   464,   464,
     464,   464,   464,   464,   464,   464,   484,   464,   485,   464,
     486,   464,   464,   487,   464,   488,   464,   489,   464,   464,
     464,   490,   464,   491,   491,   492,   492,   493,   493,   494,
     496,   497,   498,   499,   495,   500,   501,   495,   502,   495,
     503,   503,   504,   505,   505,   505,   506,   507,   506,   506,
     508,   508,   509,   509,   510,   510,   511,   511,   512,   512,
     512,   513,   513,   514,   514,   514,   515,   515,   515,   516,
     517,   516,   518,   518,   519,   520,   520,   521,   521,   521,
     523,   524,   525,   522,   526,   527,   526,   528,   526,   530,
     529,   531,   531,   532,   532,   533,   533,   533,   534,   534,
     535,   536,   536,   537,   538,   538,   539,   540,   540,   540,
     540,   540,   540,   541,   541,   541,   541,   542,   542,   543,
     543,   543,   543,   544,   544,   546,   545,   547,   547,   549,
     548,   550,   550,   551,   551,   552,   553,   554,   553,   556,
     555,   557,   558,   558,   558,   560,   561,   562,   559,   563,
     563,   564,   564,   564,   565,   564,   566,   566,   567,   568,
     568,   569,   569,   569,   570,   569,   569,   571,   571,   572,
     572,   572,   573,   573,   573,   573,   575,   574,   576,   576,
     576,   576,   576,   576,   576,   576,   576,   576,   576,   576,
     576,   576,   576,   576,   577,   579,   578,   580,   580,   581,
     582,   581,   584,   583,   586,   585,   587,   589,   588,   590,
     590,   591,   591,   592,   592,   593,   593,   595,   594,   596,
     596,   594,   594,   594,   597,   598,   598,   599,   601,   600,
     602,   602,   603,   603,   603,   603,   603,   603,   603,   603,
     603,   605,   604,   606,   607,   608,   609,   609,   610,   610,
     611,   612,   613,   614,   615,   615,   616,   617,   617,   618,
     619,   620,   621,   623,   624,   625,   622,   626,   626,   627,
     627,   628,   628,   629,   629,   629,   629,   629,   629,   629,
     630,   631,   632,   634,   633,   635,   635,   633,   633,   633,
     637,   638,   639,   640,   636,   641,   641,   642,   642,   642,
     642,   642,   642,   643,   645,   644,   647,   646,   648,   648,
     649,   649,   649,   649,   650,   650,   650,   651,   652,   652,
     653,   653,   653,   655,   654,   656,   656,   658,   657,   659,
     659,   661,   660,   663,   662,   665,   664,   666,   666,   667,
     667,   667,   667,   667,   667,   667,   667,   667,   667,   667,
     667,   667,   667,   667,   667,   667,   667,   667,   668,   667,
     669,   669,   670,   671,   671,   672,   672,   672,   673,   673,
     673,   674,   675,   675,   675,   675,   675,   675,   675,   675,
     675,   675,   675,   675,   675,   675,   675,   676,   676,   676,
     676,   676,   676,   677,   677,   677,   677,   677,   677,   678,
     678,   678,   678,   678,   678,   679,   680,   681,   681,   681,
     682,   683,   684,   684,   684,   684,   686,   685,   688,   687,
     689,   689,   690,   691,   692,   693,   695,   694,   697,   696,
     698,   698,   699,   699,   699,   699,   699,   699,   700,   699,
     699,   699,   699,   699,   699,   699,   699,   699,   699,   699,
     699,   699,   699,   701,   699,   702,   699,   703,   699,   704,
     699,   699,   699,   699,   699,   699,   699,   699,   699,   699,
     699,   699,   705,   699,   699,   699,   699,   699,   699,   699,
     699,   699,   699,   699,   699,   699,   699,   699,   699,   706,
     699,   707,   699,   708,   699,   709,   699,   710,   710,   710,
     710,   711,   711,   712,   712,   712,   713,   713,   713,   713,
     713,   714,   715,   715,   716,   716,   716,   716,   716,   716,
     717,   717,   717,   717,   718,   718,   718,   718,   719,   721,
     722,   720,   720,   720,   720,   720,   720,   720,   720,   720,
     723,   723,   724,   724,   725,   725,   725,   726,   727,   728,
     728,   730,   729,   731,   729,   732,   733,   733,   735,   736,
     734,   737,   738,   734,   734,   734,   739,   739,   740,   741,
     741,   742,   742,   743,   744,   745,   745,   747,   748,   746,
     749,   749,   750,   752,   751,   753,   754,   755,   756,   756,
     758,   757,   759,   757,   760,   757,   757,   757,   757,   757,
     757,   757,   757,   757,   757,   757,   757,   757,   757,   757,
     757,   757,   757,   757,   757,   761,   761,   761,   762,   762,
     762,   762,   763,   763,   763,   764,   764,   764,   765,   765,
     766,   766,   767,   768,   768,   769,   769,   769,   770,   770,
     771,   771,   771,   772,   772,   773,   773,   775,   774,   777,
     776,   779,   778,   780,   780,   782,   781,   783,   781,   784,
     781,   785,   781,   781,   786,   781,   781,   781,   787,   788,
     788,   790,   789,   791,   789,   792,   792,   793,   795,   794,
     797,   796,   799,   798,   800,   798,   801,   798,   802,   802,
     802,   803,   803,   804,   804,   805,   805,   805,   805,   805,
     805,   805,   805,   806,   806,   806,   806,   806,   806,   806,
     806,   806,   806,   806,   806,   806,   806,   806,   807,   807,
     807,   807,   808,   808,   808,   808,   808,   808,   808,   808,
     808,   810,   809,   811,   811,   813,   812,   814,   812,   815,
     812,   816,   812,   817,   812,   818,   812,   819,   812,   820,
     812,   822,   821,   823,   821,   821,   821,   821,   824,   824,
     824,   824,   824,   825,   826,   825,   827,   827,   828,   828,
     829,   829,   830,   830,   831,   831,   832,   833,   832,   832,
     834,   835,   836,   834,   837,   834,   834,   838,   834,   834,
     834,   839,   834,   834,   834,   840,   840,   841,   841,   842,
     843,   842,   845,   844,   846,   847,   848,   849,   851,   850,
     852,   853,   853,   854,   854,   856,   855,   857,   857,   858,
     858,   860,   859,   861,   861,   863,   862,   864,   865,   865,
     866,   866,   868,   867,   869,   869,   870,   870,   872,   871,
     873,   873,   874,   875,   876,   877,   878,   879,   880,   880,
     881
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     3,     0,     4,     3,     3,     0,     2,     1,
       0,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     3,     3,     4,     3,     1,     1,     1,
       1,     1,     1,     4,     1,     0,     2,     4,     4,     4,
       4,     4,     4,     4,     4,     3,     0,     3,     0,     3,
       0,     2,     0,     0,    13,     3,     3,     4,     3,     4,
       3,     4,     3,     3,     3,     3,     0,     6,     0,     9,
       3,     4,     7,     4,     7,     3,     3,     3,     3,     3,
       3,     3,     3,     6,     6,     0,     4,     0,     4,     4,
       4,     0,     0,     9,     0,     0,     9,     3,     0,     4,
       3,     0,     4,     0,     5,     3,     0,     4,     3,     0,
       4,     0,     5,     0,     4,     2,     3,     3,     0,     0,
       9,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     4,     3,     3,     3,     3,     0,     5,     0,     8,
       0,     5,     7,     0,     4,     0,     7,     0,     7,     3,
       3,     0,     5,     0,     1,     0,     2,     0,     2,     4,
       0,     0,     0,     0,    11,     0,     0,     9,     0,     9,
       0,     2,     4,     0,     1,     1,     0,     0,     4,     2,
       0,     2,     0,     2,     0,     2,     0,     2,     1,     1,
       4,     0,     2,     1,     2,     2,     1,     1,     1,     1,
       0,     7,     0,     2,     1,     0,     1,     1,     1,     1,
       0,     0,     0,    12,     0,     0,     5,     0,     5,     0,
       5,     0,     2,     1,     2,     2,     2,     2,     1,     2,
       4,     1,     2,     4,     1,     2,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     2,     1,
       1,     1,     1,     0,     2,     0,     4,     0,     2,     0,
       6,     0,     2,     0,     2,     6,     3,     0,     7,     0,
       4,     1,     2,     3,     3,     0,     0,     0,    26,     0,
       2,     4,     4,     6,     0,     4,     1,     1,     2,     0,
       2,     1,     1,     3,     0,     4,     1,     1,     2,     2,
       2,     2,     2,     3,     4,     3,     0,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     0,     4,     0,     2,     4,
       0,     7,     0,     3,     0,     3,     5,     0,     7,     0,
       1,     1,     2,     0,     1,     1,     2,     0,     4,     1,
       2,     2,     2,     2,     2,     0,     2,     3,     0,     4,
       0,     2,     3,     3,     4,     5,     4,     5,     3,     3,
       3,     0,     3,     3,     1,     2,     0,     2,     5,     6,
       1,     3,     1,     2,     0,     2,     3,     0,     2,     2,
       2,     4,     3,     0,     0,     0,     8,     1,     2,     0,
       2,     0,     2,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     4,     0,     4,     1,     2,     2,     2,     2,
       0,     0,     0,     0,    12,     0,     2,     3,     3,     4,
       4,     3,     3,     3,     0,     3,     0,     3,     0,     2,
       5,     1,     1,     1,     3,     3,     3,     3,     0,     2,
       1,     1,     1,     0,     4,     0,     2,     0,     3,     2,
       4,     0,     4,     0,     3,     0,     3,     0,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     1,     1,     1,     1,     0,     4,
       1,     2,     3,     0,     2,     1,     1,     1,     2,     2,
       2,     3,     1,     2,     1,     1,     2,     2,     1,     1,
       1,     1,     2,     1,     1,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     4,     3,     3,     3,
       3,     3,     2,     3,     4,     3,     0,     4,     0,     4,
       3,     3,     1,     1,     5,     3,     0,     3,     0,     3,
       0,     2,     2,     3,     4,     3,     4,     5,     0,     4,
       3,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     0,     4,     0,     5,     0,     5,     0,
       5,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       4,     3,     0,     4,     4,     2,     4,     4,     4,     3,
       3,     4,     4,     4,     4,     4,     4,     4,     4,     0,
       4,     0,     4,     0,     4,     0,     4,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     3,     3,     4,     3,
       3,     1,     0,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     1,     1,     1,     2,     0,
       0,     7,     3,     4,     6,     4,     6,     7,     9,     1,
       0,     2,     0,     1,     0,     2,     2,     1,     1,     0,
       2,     0,     5,     0,     7,     7,    11,     4,     0,     0,
      10,     0,     0,    10,     6,     6,     0,     2,     1,     6,
       6,     3,     2,     1,     4,     0,     2,     0,     0,     7,
       0,     2,     5,     0,     4,     3,     1,     2,     0,     2,
       0,     4,     0,     4,     0,    10,     9,     3,     3,     4,
       4,     4,     4,     4,     4,     4,     4,     3,     7,     8,
       6,     3,     3,     3,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     5,     1,     2,     0,     4,     7,     1,     1,
       1,     1,     1,     1,     2,     1,     2,     0,     4,     0,
       3,     0,     3,     0,     2,     0,     4,     0,     4,     0,
       4,     0,     4,     4,     0,     4,     5,     1,     2,     0,
       2,     0,     4,     0,     4,     0,     2,     5,     0,     6,
       0,     6,     0,     6,     0,     6,     0,     6,     0,     1,
       1,     1,     2,     1,     2,     3,     3,     3,     3,     2,
       3,     6,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     3,     6,     1,     1,     3,     3,
       6,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     5,     0,     2,     0,     5,     0,     5,     0,
       5,     0,     5,     0,     5,     0,     5,     0,     5,     0,
       5,     0,     4,     0,     4,     1,     2,     2,     0,     1,
       2,     5,     3,     0,     0,     6,     0,     1,     0,     1,
       0,     3,     0,     1,     0,     2,     1,     0,     3,     1,
       0,     0,     0,     5,     0,     6,     2,     0,     5,     2,
       5,     0,     6,     2,     6,     0,     1,     0,     1,     0,
       0,     3,     0,     3,     4,     3,     3,     3,     0,     7,
       2,     1,     2,     3,     1,     0,     5,     1,     2,     1,
       2,     0,     7,     1,     2,     0,     7,     2,     1,     2,
       3,     1,     0,     5,     1,     2,     1,     2,     0,     7,
       1,     2,     3,     3,     3,     3,     3,     3,     0,     1,
       1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned short int yydefact[] =
{
       0,     9,     0,  1008,     1,   809,     0,   412,    76,   493,
       0,     0,   423,   464,   404,    64,   301,   364,   842,   844,
     846,   891,     0,     3,  1010,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   838,   840,     0,     0,
       0,     0,     0,     0,     0,    12,    20,    13,     8,    14,
      21,    45,    46,    47,    15,    65,    16,    80,    48,    17,
       0,   299,     0,    18,    19,    23,   406,    26,   414,    25,
      24,    31,    27,   468,    28,   497,    29,   813,    22,    49,
      30,    32,    33,    34,    36,    35,    37,    38,    39,    40,
      41,    42,    43,    44,    10,  1009,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   893,
       0,     0,     0,     0,     0,     0,     0,   968,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    57,     0,    59,    60,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   115,     0,     0,     0,     0,     0,     0,   128,
       0,   131,     0,     0,   136,     0,   139,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   143,   203,     0,     0,     0,     0,     0,   173,
       0,     0,     0,     0,   181,   203,    82,     0,     0,     0,
       0,     0,    80,   302,     0,   369,   388,   385,   371,   390,
       0,     0,     0,     0,   743,   576,   336,     0,     0,   578,
     733,     0,   586,     0,   582,     0,     0,   523,   746,   518,
       0,     0,     0,   491,   497,   503,   499,   500,   501,   502,
     505,   504,   506,   507,   508,   510,     0,   509,   511,   590,
     514,     0,   515,   516,   517,   748,   807,     0,     2,   810,
     422,    77,   494,     0,    51,    50,   424,   465,   365,     0,
       0,     0,     0,     6,     0,     0,   965,   985,   967,   966,
       0,  1003,  1002,  1004,     5,    53,    52,     0,     0,  1005,
    1006,  1007,    54,    56,    62,    61,     0,   297,   296,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    66,     0,
       0,     0,   273,   274,   275,   276,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,   268,   269,   267,   270,
     271,   272,     0,     0,     0,     0,     0,     0,     0,     0,
     238,   237,   239,   117,     0,     0,     0,     0,     0,   133,
       0,     0,     0,     0,   141,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     204,   205,     0,     0,   166,   170,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   183,     0,     0,   145,
       0,    78,    75,    81,   303,   304,   355,     0,   326,   305,
     324,   316,     0,   317,   319,   321,     0,   322,   357,   370,
     367,     0,   372,   373,   384,     0,   410,   403,   407,     0,
       0,     0,   411,   415,   417,     0,   466,     0,   478,   483,
     463,   469,   472,   471,   473,   535,   543,   544,   532,     0,
     540,   534,   541,   538,   539,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   737,   735,   347,   338,   340,
     341,   339,   342,   344,   345,   343,   346,   348,   349,   352,
     350,   353,   351,   572,     0,     0,     0,   498,     0,     0,
       0,   732,   689,     0,     0,     0,     0,     0,     0,   700,
     699,     0,   583,   817,   819,   821,     0,   824,     0,   815,
       0,   814,   829,   827,    11,   421,   429,     0,     0,     0,
       0,   897,   907,   895,   901,   909,   899,   903,   905,   894,
       4,   964,     0,     0,     0,     0,    55,     0,     0,     0,
       0,    63,     0,     0,     0,     0,     0,    95,     0,     0,
     109,   100,   110,   105,     0,    90,     0,    86,     0,     0,
     179,   108,   934,   107,    85,    94,     0,     0,   253,   112,
       0,     0,   111,   106,     0,     0,     0,     0,     0,   127,
     229,     0,     0,   130,     0,   235,   135,     0,   138,     0,
     235,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,     0,   162,   163,     0,   279,   280,   281,   282,     0,
       0,   164,   277,   221,   165,     0,     0,     0,     0,     0,
       0,   180,    93,     0,    88,    92,   707,   709,     0,   184,
     185,     0,   147,   146,     0,     0,     0,     0,     0,   362,
     300,   318,   332,     0,     0,   354,     0,     0,     0,   377,
       0,     0,   374,   375,   386,     0,     0,     0,     0,     0,
       0,     0,     0,   391,   405,     0,   413,   420,     0,     0,
       0,     0,     0,     0,     0,   485,   536,   537,   553,   556,
     554,   555,   557,   558,   545,   533,   563,   564,   560,   559,
     561,   562,   546,   549,   547,   548,   550,   551,   552,   542,
     531,     0,     0,   337,     0,   565,     0,   489,     0,   571,
     587,   570,     0,   569,   568,   567,   525,   526,   527,   522,
     524,     0,     0,   520,   512,   513,     0,     0,   575,   573,
       0,   495,   492,     0,   580,   581,     0,     0,     0,   588,
       0,     0,   619,     0,     0,   598,     0,     0,   613,   615,
     617,   671,     0,     0,     0,     0,     0,     0,     0,     0,
     684,     0,     0,     0,     0,   632,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     649,   655,   651,   653,     0,   585,   591,   601,   672,     0,
       0,   709,     0,     0,     0,     0,     0,   711,     0,   731,
     688,     0,   799,     0,     0,     0,     0,   750,   798,     0,
       0,     0,     0,   752,     0,     0,   778,   779,     0,     0,
       0,   780,   781,     0,     0,     0,   775,   776,   777,   745,
     749,     0,     0,     0,     0,     0,     0,   828,     0,   835,
       0,   811,   808,     0,     0,   431,   862,     0,     0,     0,
     848,   881,     0,     0,   848,     0,   877,   876,     0,     0,
       0,   848,     0,   892,     0,     0,     0,     0,     0,     0,
       0,     0,   992,     0,     0,   988,   991,   975,     0,     0,
     971,   974,   848,   848,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   103,     0,    91,    87,   101,     0,   937,
     936,   939,   940,   934,   255,   256,   257,   116,   254,     0,
       0,   258,     0,   119,     0,   118,   124,   120,   121,     0,
     129,   132,   236,     0,   137,   140,     0,   161,   168,   144,
     175,     0,     0,   228,   226,     0,   227,     0,     0,   221,
     223,     0,     0,     0,   190,     0,   174,    89,     0,   177,
       0,     0,   148,    79,     0,   323,     0,     0,     0,   327,
       0,   320,   335,   333,     0,   360,     0,   358,   373,   389,
     381,   382,   383,     0,     0,   401,   366,   376,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   416,
     418,   475,   474,   476,   467,     0,   480,   481,   482,   477,
     479,     0,   744,   577,   566,   579,     0,     0,   530,   529,
     528,   519,   521,     0,   734,   736,   574,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   680,   681,   682,   683,     0,
     685,   687,   686,     0,     0,   675,   678,   679,   677,   676,
     674,     0,     0,     0,     0,     0,   959,   959,   959,     0,
       0,   959,   959,   959,   959,     0,     0,   959,     0,     0,
       0,     0,     0,     0,   592,     0,     0,   635,     0,   690,
     709,     0,     0,   708,     0,     0,     0,   713,     0,   692,
     701,   747,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   800,   801,   802,     0,     0,   793,     0,     0,
       0,   790,   774,     0,     0,     0,     0,   782,   783,   784,
       0,     0,   788,   789,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   831,   833,
     830,   430,   425,     0,   859,     0,     0,     0,     0,     0,
     849,   850,   843,     0,     0,     0,   845,     0,   873,     0,
       0,     0,   886,   887,   882,   883,   884,   885,     0,     0,
     888,   889,   890,     0,     0,     0,     0,     0,   847,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   848,   989,     0,     0,     0,   848,   972,
     839,   841,     0,    68,    71,    73,    70,    69,    67,    72,
      74,     0,     0,   261,     0,     0,   264,   928,     0,   941,
       0,     0,     0,     0,     0,     0,     0,   935,     0,     0,
     259,     0,   240,   251,   251,     0,   134,   142,   214,   206,
     278,   167,   224,   225,   171,   222,     0,     0,     0,   251,
     291,   182,   710,   212,   186,     0,   232,   356,     0,   331,
     330,   329,   325,   328,   363,   334,     0,     0,     0,   379,
       0,   387,     0,   392,   393,   400,   399,     0,   398,     0,
       0,     0,     0,   419,     0,     0,   484,   486,   490,   584,
     738,   496,   717,   621,   626,   627,   670,   669,   666,     0,
     667,   589,   608,   606,     0,     0,   604,     0,   622,   623,
       0,     0,     0,   600,   625,   624,   607,   605,   628,   629,
     603,   612,   611,   602,   610,   609,     0,     0,   661,   631,
     960,     0,     0,     0,   639,   640,     0,     0,     0,     0,
     962,     0,     0,     0,     0,   657,   658,   659,   660,     0,
       0,     0,     0,   595,     0,   593,     0,     0,     0,   702,
       0,   693,     0,     0,     0,   695,     0,     0,     0,     0,
       0,     0,   803,     0,     0,     0,     0,     0,   805,     0,
     767,     0,   794,   757,     0,   758,   791,   771,   772,   773,
       0,   785,   786,   787,     0,     0,     0,   818,   820,     0,
       0,   822,     0,     0,   825,     0,     0,   836,   816,   812,
     823,     0,     0,   450,     0,   443,     0,     0,     0,   434,
     435,     0,   432,   437,   438,   439,   436,   433,   851,     0,
       0,   860,   856,   855,   857,   858,     0,   879,   878,     0,
     874,   865,   864,   863,   869,   870,   872,   871,   868,   867,
     866,   915,   911,   913,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   994,     0,   990,   987,   986,   977,     0,
     973,   970,   969,   298,     0,     0,   262,     0,     0,   265,
     929,   938,   946,     0,     0,   949,     0,     0,     0,   953,
      97,     0,   113,   114,   251,     0,     0,   230,     0,   216,
       0,     0,     0,     0,    98,     0,   191,     0,     0,     0,
       0,     0,     0,   232,   234,     0,     0,     0,   368,   378,
     380,   402,   396,     0,     0,   394,     0,   408,   470,   487,
       0,     0,   668,     0,   630,   599,   614,     0,     0,   665,
     664,   663,   633,   662,     0,   636,   638,   637,   641,   642,
     645,   644,     0,   646,   647,   643,   648,   650,   656,   652,
     654,   596,     0,   594,   673,   634,   703,   704,     0,     0,
       0,   709,     0,     0,   712,   762,   766,   764,   760,   804,
     751,   761,   759,   763,   765,   806,   753,     0,     0,     0,
       0,   754,     0,     0,     0,     0,     0,   826,     0,     0,
       0,     0,   447,   448,   449,     0,     0,     0,     0,   427,
     426,   852,     0,     0,     0,     0,     0,   916,   930,   930,
     917,   898,   908,   896,   902,   910,   900,   904,   906,   995,
       0,   978,     0,     0,   104,     0,   102,   942,   947,     0,
       0,     0,     0,   260,     0,   252,   125,   122,   232,   215,
     218,   219,     0,     0,   216,   207,   209,   176,   172,   200,
       0,     0,   292,   195,   213,   178,     0,     0,   233,     0,
       0,   359,   397,   395,   409,   488,     0,   740,     0,   620,
     616,   618,   961,   963,   597,     0,     0,     0,     0,   694,
     709,     0,   696,     0,     0,     0,     0,     0,   795,     0,
       0,     0,     0,     0,     0,   832,   834,   451,   445,     0,
       0,   440,   441,   428,   853,     0,     0,     0,     0,     0,
     932,   932,     0,   993,   996,     0,   976,   979,   263,   266,
     955,   918,     0,   944,   951,     0,   241,     0,     0,     0,
       0,   169,   217,   210,     0,     0,   200,     0,     0,   251,
       0,   149,     0,   709,     0,   739,     0,   706,   705,   691,
       0,     0,   697,   714,     0,   792,   770,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   444,   446,   442,
     854,   861,   880,   875,     0,   933,   912,   914,     0,   997,
       0,   980,   956,   943,     0,     0,   919,   948,   950,   957,
     923,     0,   244,     0,   126,   123,   231,     0,     0,   208,
       0,    99,   201,   198,   192,   196,     0,   150,     0,     0,
       0,   741,     0,     0,     0,   768,     0,     0,     0,     0,
     718,   721,   729,   730,   837,     0,   931,   998,   981,   920,
       0,   958,   945,     0,   952,   954,     0,     0,     0,   249,
     220,   211,     0,   293,   251,   287,    83,   306,   361,     0,
       0,     0,   698,   769,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   922,     0,   247,   245,     0,   251,   202,
     199,   193,     0,   197,   287,   187,     0,   742,     0,     0,
     796,     0,   756,   719,   722,   452,     0,     0,     0,     0,
     251,   251,   242,     0,     0,   294,   283,   291,   288,     0,
       0,   187,     0,   716,   715,     0,   755,   726,   726,   455,
    1000,     0,   983,     0,   921,   924,     0,     0,   251,   250,
       0,     0,   194,   283,     0,     0,    84,   188,     0,     0,
     720,   723,     0,  1001,   999,   984,   982,   926,   248,   246,
       0,     0,   285,   284,   289,     0,     0,   797,   728,   727,
       0,     0,   453,     0,     0,     0,     0,   456,   927,   925,
     243,     0,   251,   251,   189,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   286,   290,     0,     0,   461,   454,
       0,   457,   458,   462,   295,     0,   460,   459,     0,     0,
       0,     0,     0,     0,     0,   307,   309,   308,     0,     0,
     314,     0,   310,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   312,   315,   311,     0,   313
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short int yydefgoto[] =
{
      -1,     2,    45,   111,    46,    47,     3,   258,    48,    49,
      50,    51,    52,    53,   132,   135,   296,    54,    55,   137,
     308,    56,    57,    98,   392,   634,   201,   202,   386,  1895,
     562,  1669,   334,   575,  1244,  1748,  1243,  1747,   346,   348,
     585,   351,   353,   590,   369,  1266,  1827,   612,  1248,   613,
     379,  1249,  1263,   384,   630,   951,  1920,  1921,   620,  1259,
    1671,  1864,  1916,  1759,  1865,  1863,  1755,  1756,   372,  1512,
    1753,  1819,  1520,  1509,  1663,  1664,   938,   939,   940,   582,
    1658,  1522,  1523,   923,   343,   915,  1504,  1812,  1938,  1858,
    1911,  1910,  1814,  1888,  1505,   567,   568,   910,   911,  1222,
    1223,  1225,  1226,   332,   316,   932,   609,  1942,  1943,  1982,
    1893,  1894,  1983,  1518,  1890,  1915,    58,   537,    59,   204,
      60,    61,   401,   637,  1896,  2016,  2017,  2022,  2025,   402,
     403,   641,   404,   638,   958,   959,   405,   232,   448,   484,
     407,   408,   635,   645,   967,  1276,   640,   960,    62,   105,
      63,    64,   646,   410,   207,   651,   652,   653,   973,  1280,
     208,   413,   654,   209,   411,   414,   663,   976,  1282,    65,
      66,   417,   210,   418,   419,    67,    68,   422,   211,   423,
     668,   990,   424,    69,    70,    71,   103,   516,  1431,  1620,
     845,  1152,  1432,  1433,  1434,  1435,  1436,  1615,  1719,  1437,
    1611,  1786,  1929,  1988,  1952,  1977,    72,    73,   104,   430,
     672,   212,   431,   432,   433,   674,  1000,   434,   675,  1001,
    1297,  1685,  1093,    74,   486,    75,    99,   732,  1017,   233,
     234,   462,   722,   235,   461,   720,   723,   236,   445,   699,
     684,   692,   237,   238,   239,   240,   241,   242,   243,   447,
     244,   451,   245,   246,   509,   247,   248,   249,   455,   785,
    1026,   490,   786,  1032,  1035,  1036,  1037,  1029,  1064,  1079,
    1081,  1082,  1080,  1359,  1337,  1338,   787,   788,  1088,  1061,
    1049,  1053,   498,   499,   789,  1369,   800,  1577,  1697,   627,
    1262,   948,   500,  1098,  1376,  1579,   489,  1140,  1877,  1927,
    1878,  1928,  1950,  1969,  1143,   250,   251,   252,   727,   466,
     726,  1540,  1765,  1687,   253,   446,   254,   255,   829,   501,
     830,  1106,  1111,  1708,   831,   832,  1130,  1404,  1134,  1120,
    1121,  1118,  1779,   833,  1115,  1383,  1389,    76,   510,    77,
      96,   842,  1146,   256,   511,   840,   834,   835,   836,   838,
     512,   843,  1150,  1421,  1422,  1144,  1417,    78,   124,    79,
     125,    80,   106,   107,   108,  1162,  1440,  1726,   860,   861,
     862,  1183,    81,   109,   272,   529,   866,   864,   869,   867,
     870,   871,   865,   868,  1465,  1628,  1629,  1807,  1854,  1957,
    1979,  1491,  1730,  1796,   902,   903,  1227,  1236,  1493,  1740,
    1809,  1741,  1810,  1803,  1852,  1341,  1554,  1351,  1562,    82,
      83,    84,    85,    86,   280,  1208,   879,   880,   881,  1205,
    1479,  1736,  1737,  1881,  1933,    87,   532,  1203,   874,   875,
     876,  1200,  1474,  1733,  1734,  1880,  1931,    88,    89,    90,
      91,    92,    93,    94,    95
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1440
static const short int yypact[] =
{
     855, -1440,   173,   953, -1440, -1440,    59, -1440, -1440, -1440,
     105,   205, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440,
   -1440, -1440,    25, -1440, -1440,   122,   163,   178,   213,   232,
     246,   264,   301,   330,    73,   229, -1440, -1440,   334,   354,
     357,   363,   125,   194,   386, -1440, -1440, -1440, -1440, -1440,
   -1440, -1440, -1440, -1440, -1440, -1440, -1440,   121, -1440, -1440,
     231, -1440,   172, -1440, -1440, -1440, -1440, -1440, -1440, -1440,
   -1440, -1440, -1440, -1440, -1440,    15, -1440, -1440, -1440, -1440,
   -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440,
   -1440, -1440, -1440, -1440,   152, -1440,   314,  -162,   338,   398,
     410,   -75,   222,   487,   489,   492,   494,   501,   513, -1440,
     269,   525,   536,   295,   323,   352,   394, -1440,   406,   409,
     416,   417,   424,   427,   588,   592,   428,   434,   437,   438,
   -1440, -1440,   458, -1440, -1440,   356,  -163,    72,   597,   759,
     612,   -16,   619,   630,   715,   716,    34,   721,   722,   723,
      21,   724, -1440,   -63,   725,   726,   287,   609,   728, -1440,
     738, -1440,   739,   740, -1440,   743, -1440,   744,   746,   747,
     748,   751,   752,   753,   754,   755,   761,   764,   770,   771,
     772,   773, -1440,   124,   774,   783,   784,   785,   786,   548,
     789,   791,   792,   793, -1440,   124, -1440,   537,   795,   540,
     797,   935,   121,     6,   601,   944, -1440,   911, -1440, -1440,
     101,    88,     7,    96, -1440, -1440, -1440,   804,   805, -1440,
   -1440,  -115, -1440,   806, -1440,   813,   185, -1440, -1440, -1440,
     370,   914,   367, -1440,    15, -1440, -1440, -1440, -1440, -1440,
   -1440, -1440, -1440, -1440, -1440, -1440,   807, -1440, -1440, -1440,
   -1440,   541, -1440, -1440, -1440, -1440,   650,   926, -1440, -1440,
   -1440, -1440, -1440,   564, -1440, -1440, -1440, -1440, -1440,   562,
     563,   566,   990, -1440,   567,   568, -1440, -1440, -1440, -1440,
     570, -1440, -1440, -1440, -1440, -1440, -1440,   577,   578, -1440,
   -1440, -1440, -1440, -1440, -1440, -1440,   571, -1440, -1440,   912,
     942,   628,   862,   948,   938,   951,   864,   729, -1440,   602,
     -86,   603, -1440, -1440, -1440, -1440,   605,   610,   613,   -66,
     -37,    26,   620,   621, -1440,   622, -1440, -1440, -1440, -1440,
   -1440, -1440,   623,   633,   869,   637,   618,   868,   638,   640,
   -1440, -1440, -1440,   885,   -41,   642,    28,   644,    28, -1440,
     655,    28,   656,    28, -1440,   659,   660,   662,   663,   667,
     673,   675,   676,   677,   683,   886,   684,   690,   956,   308,
   -1440, -1440,   949,   699, -1440, -1440,   717,   796,   776,    -8,
     718,   720,   -21,   730,  -115,   972,   749,  -115,   736, -1440,
     742, -1440, -1440, -1440, -1440, -1440, -1440,   977, -1440, -1440,
   -1440, -1440,  1115, -1440, -1440, -1440,   414, -1440, -1440, -1440,
   -1440,   987, -1440,   320,    51,  1035, -1440, -1440, -1440,  1001,
    1114,  1004, -1440, -1440, -1440,    71, -1440,  1003, -1440, -1440,
   -1440, -1440, -1440, -1440, -1440,  -159,    36, -1440,   830,   550,
     -13, -1440, -1440, -1440, -1440,   750,  1007,  1009,  1010,  1011,
     756,  1012,  1013,  1014,   760,  1017,   765,  1178,   775,   787,
     788,   -91,  1029,   790,   822, -1440, -1440, -1440, -1440, -1440,
   -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440,
   -1440, -1440, -1440, -1440,   828,   446,  1166, -1440,    -9,   829,
     678, -1440, -1440,   -48,   -34,   -33,   -32,  1030,  1177, -1440,
   -1440,  1207, -1440, -1440, -1440, -1440,  1043, -1440,  1042, -1440,
    1182, -1440, -1440, -1440, -1440, -1440,   837,   -62,  -121,    14,
     995, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440,
   -1440, -1440,    -2,    19,  -121,  -121, -1440,  1050,  1069,  1086,
    1087, -1440,  1088,  1089,  1090,  1091,  1092, -1440,   838,   839,
   -1440, -1440, -1440, -1440,   841, -1440,   843, -1440,   844,   845,
   -1440, -1440,   -78, -1440, -1440, -1440,   515,  -119, -1440, -1440,
     846,  1101, -1440, -1440,   848,   984,  1106,   851,  1108, -1440,
   -1440,   852,   858, -1440,   860,   976, -1440,   861, -1440,   866,
     976, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440,
   -1440,   867, -1440, -1440,  1124, -1440, -1440, -1440, -1440,   870,
    1125, -1440,  1133,   -72, -1440,  1129,  1130,  1137,  1132,  1140,
     879, -1440, -1440,   881, -1440, -1440, -1440, -1440,  1138, -1440,
    1145,  -115, -1440, -1440,  1142,  1144,   887,  1146,  1148, -1440,
   -1440,   636, -1440,   889,   632,   331,   911,   895,   534, -1440,
    1171,  1307,    99, -1440, -1440,    63,  1172,  1173,  -115,  1174,
    1175,  1176,  1179, -1440, -1440,  1180, -1440, -1440,    -7,   915,
     921,   924,  1183,  1343,   -74, -1440, -1440, -1440, -1440, -1440,
   -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440,
   -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440,
   -1440,   927,   929, -1440,   930, -1440,   931, -1440,  1188, -1440,
   -1440, -1440,  1189, -1440, -1440, -1440, -1440, -1440, -1440, -1440,
   -1440,   547,  -118, -1440, -1440, -1440,  1193,   141, -1440, -1440,
     936, -1440, -1440,  1194, -1440, -1440,  1195,    94,   451, -1440,
    1196,  1197, -1440,  1200,  1199, -1440,  1201,  1202, -1440, -1440,
   -1440, -1440,  1203,  1204,  1205,  1206,  1208,  1209,  1210,   187,
     153,  1212,   629,  1213,  1214, -1440,  1217,  1216,  1218,  1219,
    1220,  1221,  1222,  1223,  1224,  1225,  1226,  1227,  1228,  1229,
   -1440, -1440, -1440, -1440,  -100, -1440, -1440, -1440,   164,  1232,
    -115, -1440,  -115,  -115,  -115,  -115,  -115, -1440,   937, -1440,
    1147,  1242, -1440,  1231,  1233,  1234,  1235, -1440, -1440,  1236,
    1237,  1238,  1243, -1440,   322,  1167, -1440, -1440,  1244,   950,
     986, -1440, -1440,  1246,  1247,  1248, -1440, -1440, -1440, -1440,
   -1440,  1198,   337,   144,  1250,  1250,   478, -1440,   544, -1440,
    1250, -1440, -1440,    55,   993, -1440, -1440,    14,   -62,   -62,
     252, -1440,    14,  -121,    79,    14, -1440, -1440,    14,    14,
     611,   312,   732, -1440,  1252,  1254,  1255,  1256,  1257,  1258,
    1259,  1260, -1440,  1265,    38, -1440, -1440, -1440,  1266,   167,
   -1440, -1440,    79,    79,  1261,  1015,  1016,  1018,  1019,  1020,
    1021,  1022,  1023, -1440,   975, -1440, -1440, -1440,  1024, -1440,
   -1440, -1440,     8,   -78, -1440, -1440, -1440, -1440, -1440,  1267,
     285, -1440,  1002, -1440,  1268, -1440, -1440, -1440, -1440,  -115,
   -1440, -1440, -1440,  1026, -1440, -1440,  1027, -1440, -1440, -1440,
   -1440,  1269,  1028, -1440, -1440,  1270, -1440,  1273,  1031,   -72,
   -1440,  1286,  1324,  1288, -1440,  1289, -1440, -1440,  -112, -1440,
    1290,  1093, -1440, -1440,  1036, -1440,  1038,   565,  -116, -1440,
    1297, -1440, -1440, -1440,  1040, -1440,  -115, -1440,   181, -1440,
   -1440, -1440, -1440,    99,  1041, -1440, -1440, -1440,  1044,  1045,
    1046,  1047,  -115,  1048,  1447,  1322,  1299,  1305,  1306, -1440,
   -1440, -1440, -1440, -1440, -1440,  1308, -1440, -1440, -1440, -1440,
   -1440,  -102, -1440, -1440, -1440, -1440,  1049,  1054, -1440, -1440,
   -1440, -1440, -1440,  1055, -1440, -1440, -1440,  1316,  1153,  1059,
    1060,  1067,  1068,  1070,  1071,  -109,  1327,  1072,  1073,  1330,
    1333,  1076,  1335,  1078,  1079,  1338,  1337,  1339,  1082,  1084,
    1085,  1094,  1095,  1096,  1097, -1440, -1440, -1440, -1440,  1098,
   -1440, -1440, -1440,  1099,  1100, -1440, -1440, -1440, -1440, -1440,
   -1440,  1102,  1103,  1104,  1344,  1105,  1453,  1453,  1453,  1107,
    1109,  1453,  1453,  1453,  1453,  1454,  1454,  1453,  1454,   332,
    1347,  1348,  1350,   -59, -1440,   955,    96, -1440,  1147, -1440,
   -1440,   -50,  -115, -1440,  -115,  -115,  1110, -1440,  1351, -1440,
   -1440, -1440,  1346,  1357,  1362,  1364,  1369,  1368,  1370,  1371,
    1372,  1375, -1440, -1440, -1440,  1118,  1376, -1440,    10,  1377,
     336, -1440, -1440,  1119,  1122,  1123,  1380, -1440, -1440, -1440,
     469,  1381, -1440, -1440,  1399,  1383,  1128,  1131,  1385,  1386,
    1134,  1388,  1389,  1135,    53,  1136,  1395,  1396, -1440, -1440,
   -1440, -1440,    -3,    69, -1440,   183,   -62,   -62,   -62,   -62,
   -1440, -1440, -1440,    69,    50,  -121, -1440,    69, -1440,   556,
     359,   686, -1440, -1440, -1440, -1440, -1440, -1440,   -62,   -62,
   -1440, -1440, -1440,   -62,    14,    14,    14,    14, -1440,  -121,
    -121,  -121,   299,   299,   299,   299,   299,   299,   299,   299,
    1397,  1139,  1325,  -220, -1440,  1398,  1143,  1326,  -220, -1440,
   -1440, -1440,  1149, -1440, -1440, -1440, -1440, -1440, -1440, -1440,
   -1440,  1400,   391, -1440,  1402,   395, -1440,  1158,  1404, -1440,
    1406,  1407,  1408,  1409,  1410,  1411,  1156, -1440,  1413,  1159,
   -1440,  1160, -1440, -1440, -1440,  -115, -1440, -1440,  1230,  -104,
   -1440, -1440, -1440, -1440, -1440, -1440,  1414,  1417,  1239, -1440,
    1181, -1440, -1440,  1424, -1440,  1419,  -115, -1440,  1240, -1440,
   -1440, -1440, -1440, -1440, -1440, -1440,  -115,  -115,  1307, -1440,
     -85, -1440,  1422, -1440, -1440, -1440, -1440,  1165, -1440,  1423,
    1426,  1168,  -101, -1440,  1184,  1153, -1440, -1440, -1440, -1440,
   -1440, -1440,  1562, -1440, -1440, -1440, -1440, -1440, -1440,  1185,
   -1440, -1440, -1440, -1440,  1428,  1186, -1440,  1187, -1440, -1440,
    1190,  1429,  1430, -1440, -1440, -1440, -1440, -1440, -1440, -1440,
   -1440, -1440, -1440, -1440, -1440, -1440,   584,   -95, -1440, -1440,
   -1440,  1191,  1192,  1211, -1440, -1440,  1215,  1241,  1245,  1249,
   -1440,  1251,  1253,  1262,  1263, -1440, -1440, -1440, -1440,  1264,
    1271,  1272,  1274, -1440,  1053, -1440,  1275,  1276,  1568,  1277,
       3, -1440,  -115,  -115,  1572, -1440,  1435,  1278,  1279,  1280,
    1281,  1282, -1440,   -71,  1283,  1284,  1285,  1287, -1440,   -68,
   -1440,  1401, -1440, -1440,  1436, -1440, -1440, -1440, -1440, -1440,
    1437, -1440, -1440, -1440,  1391,  1438,   322, -1440, -1440,  1585,
    1589, -1440,  1590,  1591, -1440,  1425,  1452, -1440, -1440, -1440,
   -1440,  1250,  1250, -1440,   712, -1440,  1455,  1457,  1458, -1440,
   -1440,  1594, -1440, -1440, -1440, -1440, -1440, -1440, -1440,  1420,
     -62, -1440,   374,   374, -1440, -1440,  -121, -1440, -1440,    14,
   -1440,   302,   302,   302, -1440, -1440,   624,   349,  1292,  1292,
    1292,  1459, -1440, -1440,  1462,  1291,  1293,  1294,  1295,  1296,
    1298,  1300,  1301, -1440,    16, -1440, -1440, -1440, -1440,    17,
   -1440, -1440, -1440, -1440,  1461,  1302, -1440,  1463,  1303, -1440,
   -1440, -1440, -1440,  1464,  1465,  1412,  1304,  1309,  1312, -1440,
   -1440,  1310, -1440, -1440, -1440,    18,    22, -1440,  1466,   304,
    1468,  1469,  1311,  1313, -1440,  1470,  1472,  1473,  1475,  1476,
    1314,  1315,  1317,  -115, -1440,  1477,  -115,  1318, -1440, -1440,
   -1440, -1440, -1440,  1320,  1321, -1440,  1323, -1440, -1440, -1440,
    1528,  1478, -1440,  1328, -1440, -1440, -1440,  1329,  1331, -1440,
   -1440, -1440, -1440, -1440,  1481, -1440, -1440, -1440, -1440, -1440,
   -1440, -1440,  1482, -1440, -1440, -1440, -1440, -1440, -1440, -1440,
   -1440, -1440,  1332, -1440, -1440, -1440, -1440,   155,  1483,  1334,
    -115, -1440,  1336,  1572, -1440, -1440, -1440, -1440, -1440, -1440,
   -1440, -1440, -1440, -1440, -1440, -1440, -1440,  1484,  1485,  1486,
    1487, -1440,  1439,  1488,  1489,  1490,  1491, -1440,  1450,  1340,
    1341,  1495, -1440, -1440, -1440,   154,  1494,  1342,  1345,  1497,
   -1440, -1440,  -164,  -129,   859,   267,   298, -1440,  1434,  1434,
   -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440,
    1405, -1440,  1421,  1349, -1440,  1352, -1440, -1440, -1440,  1500,
    1502,  1503,  1506, -1440,    37, -1440, -1440, -1440,  -115, -1440,
   -1440, -1440,  1508,  1353,   304, -1440, -1440, -1440, -1440,  1360,
    1549,  1517, -1440, -1440, -1440, -1440,  1512,  1355, -1440,  1513,
    -115, -1440, -1440, -1440, -1440, -1440,  -115, -1440,  1676, -1440,
   -1440, -1440, -1440, -1440, -1440,  1516,  1521,  1356,  1678, -1440,
   -1440,   -46, -1440,  1358,   189,  1359,  1363,  1522,  1523,  1524,
    1560,  1561,  1564,  1580,  1533, -1440, -1440, -1440, -1440,   -65,
    1365, -1440, -1440, -1440, -1440,  1520,   -62,  -121,    14,  1546,
    1548,  1548,  1563,  1405, -1440,  1565,  1421, -1440, -1440, -1440,
    1598,    24,  1566, -1440, -1440,  1354, -1440,  1507,  1507,  1366,
    1378, -1440, -1440,  1367,  1571,  1373,  1360,  1578,  1581, -1440,
    1603, -1440,  1374, -1440,  -115,  1528,  1582, -1440, -1440, -1440,
    1583,     3, -1440, -1440,  1379, -1440, -1440,  1586,  1593,  1605,
    1595,  1599,  1600,  1604,  1606,  1382,  1609, -1440, -1440, -1440,
   -1440,   302,  1292,   341,  1608, -1440, -1440, -1440,  1384, -1440,
    1387, -1440, -1440, -1440,  1613,  1615, -1440, -1440, -1440,  1390,
    1393,  1616,   -55,  1618, -1440, -1440, -1440,  1620,  1622, -1440,
    1611, -1440, -1440, -1440, -1440,  1472,  1623, -1440,  1415,   -23,
    1625, -1440,  1624,  1640,  1392, -1440,  1394,  1628,  1631,  1632,
    1403,  1416, -1440, -1440, -1440,  1633, -1440, -1440, -1440,  1518,
    1634, -1440, -1440,  1637, -1440, -1440,  1639,  1641,  1587, -1440,
   -1440, -1440,  1644, -1440, -1440,  1651, -1440, -1440, -1440,  1418,
    1646,  1647, -1440, -1440,  1649,  1652,  1427,  1761,  1763,  1431,
    1576,  1584,  1655, -1440,  1467, -1440, -1440,  1656, -1440, -1440,
    1663,  1472,  1660, -1440,  1651,  1440,  1665, -1440,  1661,  1664,
    1666,  1432, -1440, -1440, -1440, -1440,  1668,  1669,  1670,  1671,
   -1440, -1440, -1440,    39,  1673, -1440,  1681,  1181, -1440,  1677,
    1433,  1440,  1667, -1440, -1440,  1679, -1440, -1440, -1440, -1440,
   -1440,    41, -1440,    42, -1440, -1440,    43,    44, -1440, -1440,
    1496,  1680, -1440,  1681,  1682,  1715, -1440, -1440,  1683,  1684,
    1687,  1687,    20, -1440, -1440, -1440, -1440,  1443, -1440, -1440,
      47,  1686, -1440, -1440, -1440,  1692,  1441, -1440, -1440, -1440,
    1827,  1693, -1440,  1743,  1695,  1696,  1698, -1440, -1440, -1440,
   -1440,  1738, -1440, -1440, -1440,  1501,  1700,  1446,  1705,  1704,
    1449,  1451,  1456,  1706,  1472,  1472,  1708,  1460, -1440, -1440,
    1471, -1440, -1440, -1440, -1440,  1709, -1440, -1440,  1474,  1535,
    1711,  1713,  1714,  1717,  1479, -1440, -1440,   -28,  1718,  1719,
   -1440,  1720, -1440,  1721,  1722,  1725,  1724,  1726,  1480,  1492,
    1493,  1728, -1440, -1440, -1440,  1498, -1440
};

/* YYPGOTO[NTERM-NUM].  */
static const short int yypgoto[] =
{
   -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440,
   -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440,
   -1440, -1440, -1440, -1440, -1440, -1440,  1697, -1440, -1440, -1440,
   -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440,
   -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440,
   -1440, -1440, -1440, -1440, -1440, -1440,   -18, -1440, -1440, -1440,
   -1440, -1440, -1440, -1440, -1440, -1440,   133, -1440,  1710, -1440,
   -1440, -1440, -1440, -1440,   242, -1440,   968, -1440, -1440,   145,
   -1440, -1418, -1440,  1361, -1440, -1440, -1440, -1440, -1440, -1440,
   -1440, -1440,   160, -1440, -1220, -1440,  1442, -1440,  1005, -1440,
     689, -1440,   687, -1440, -1440, -1440, -1440,   -30, -1440, -1440,
      23, -1440, -1440,    -1, -1440, -1440, -1440, -1440,   762, -1440,
    1505, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440,
   -1440, -1440,  1319, -1440, -1440,   961, -1440,  -192, -1440,  -402,
   -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440,
   -1440, -1440, -1440, -1440,  1444,   952, -1440,  -627, -1440, -1440,
    -198, -1440, -1440, -1440, -1440, -1440, -1440,   643, -1440,   777,
   -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440,
   -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440,
   -1440, -1440, -1440, -1440, -1440, -1440, -1439, -1440, -1440, -1440,
   -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440,
   -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440,
   -1440, -1440,  -221, -1440, -1440, -1440, -1440, -1440, -1440,  1690,
   -1440, -1440, -1440, -1440, -1440, -1440,  1445, -1440,   840, -1440,
   -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440,
   -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440,
   -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440,
   -1440, -1440, -1440, -1440, -1440,   590, -1440, -1440, -1440, -1440,
   -1440, -1440,   842,  1141, -1440, -1440, -1440, -1440, -1440,  -488,
    -786,  -789, -1440, -1440, -1440, -1358,  -808, -1440, -1440, -1440,
   -1440, -1440,     4, -1440, -1440, -1440, -1440, -1440, -1440,  1448,
   -1440, -1440, -1440,   166, -1440, -1440, -1440, -1440, -1440, -1440,
   -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440,
     814, -1440, -1440,   221,   527, -1440, -1440, -1440, -1440, -1440,
   -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440,
   -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440,
   -1440,  1688, -1440, -1440, -1440,  -814,  -916, -1019,  -516,  -837,
    -515,  -854, -1440, -1440, -1440, -1440, -1440, -1440, -1440, -1440,
   -1440, -1440, -1440, -1440,  -316, -1440, -1440, -1440, -1440, -1440,
   -1440, -1440,   306,   206,  1033, -1440, -1440, -1440, -1440, -1440,
   -1440, -1440, -1440, -1440, -1440,  -202, -1440,  -700, -1440, -1440,
   -1440, -1440, -1440, -1440, -1440, -1440, -1440,  1061, -1440, -1440,
   -1440, -1440,   202, -1440, -1440, -1440, -1440, -1440, -1440,  1065,
   -1440, -1440, -1440, -1440,   209, -1440, -1440, -1440, -1440, -1440,
   -1440, -1440, -1440, -1440, -1440
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -726
static const short int yytable[] =
{
     454,   850,  1091,   854,   643,   791,   793,  1094,  1191,   412,
    1153,   485,   406,   617,   790,  1163,  1582,  1228,  1167,   882,
     883,  1168,  1170,   425,  1506,   977,  1136,  1137,   792,   794,
     796,   213,  1145,   214,  1970,   394,  1536,  1578,   312,  1516,
    1166,  1083,   693,   694,   426,  1804,  1309,  1188,   899,   326,
    1724,   215,   851,   566,   721,  1971,   957,  1972,  2018,   452,
    2019,  1423,   452,   216,  1510,   217,   218,   933,  1210,  1211,
    1295,  2020,  1229,   297,   452,  1202,   695,  1336,   716,   717,
     718,   219,   678,   730,   655,  1724,   299,   648,   548,   872,
    1415,   852,  1147,   669,   696,   996,   997,   998,   300,   220,
    1020,  1589,   301,   221,  1595,  1678,   435,  1424,   554,   302,
     877,   335,   846,  1856,   327,   452,   222,   978,   436,   437,
     438,   697,   223,   656,   452,   420,   452,   576,   452,   872,
     138,  1973,    14,   577,   439,   139,   140,   556,   415,   427,
     452,   452,   452,   657,   328,   313,   224,   225,   226,   428,
     847,   452,   649,   623,   141,  1974,   142,   227,  1050,   321,
      16,   658,   659,   626,   228,   733,   631,   988,   670,  1424,
     679,   549,  1425,     4,   143,  1695,  1718,   452,  1014,   303,
    1086,   680,   681,   304,  1392,   644,   660,   851,   846,   257,
    1639,  1641,  1655,   440,   336,   458,  1655,   676,   110,  1051,
     558,  1087,   580,  1131,  1207,   465,  1021,   144,   322,   441,
    1160,  1655,   329,  1655,  1161,  1953,  1955,  1655,  1655,   661,
     305,  1655,   205,   145,   979,  1703,   855,   416,   802,   856,
     857,   421,   146,    97,  1425,   459,   206,   677,   306,   858,
    1749,  1857,   964,  1045,  1230,   206,   121,  1446,   873,  1511,
     147,  1449,   229,  1148,  1149,   578,   148,  1416,   877,   330,
    1805,  1156,  1157,  1158,  1159,   149,  1052,   298,   260,   878,
    1725,   648,   626,   626,   795,   797,  1046,   150,   133,   100,
    1788,   230,  1438,   559,  1654,   581,  1184,  1185,   873,   151,
    1696,  1975,  1186,  1187,   900,   134,   112,  1165,   784,   101,
     102,  1370,  1090,   808,  1092,  1725,  1372,  1231,  1373,   671,
     853,   907,  1011,   934,  1272,   935,   453,  1191,  1261,   453,
     936,  1310,  1806,   122,   123,  1047,  1424,  1048,  1296,  1537,
    1084,   453,  1154,  1155,   442,  1552,   649,   113,  1164,   719,
     901,   937,   698,  1169,  1171,  1529,  1279,  1454,  1455,  1456,
    1457,   682,   114,   648,  1232,   264,   999,   618,   152,  1590,
     314,   315,  1596,   848,   555,  1787,  2021,   307,   337,   849,
    1132,  1363,   453,   331,  1133,   460,  1352,   153,  1354,   154,
    1371,   453,  1112,   453,  1772,   453,  1426,   115,   395,  1477,
     231,  1425,   155,   557,  1482,  1427,  1428,   453,   453,   453,
     662,  1976,   429,   203,  1113,  1114,   116,  1868,   453,   624,
     952,   156,   157,  1834,   619,  1233,  1234,   878,   649,   443,
     117,   734,  1235,   989,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   453,   965,   683,   982,   118,   848,
    1393,   966,   130,   131,   444,   859,  1640,  1642,  1656,   406,
     294,   295,  1657,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,  1746,   182,  1939,
     183,  1954,  1956,  1958,  1959,   119,  1165,  1980,   184,   185,
     186,  1724,  1447,    16,  1184,  1185,   259,   187,   188,   189,
    1186,  1187,   648,   584,   370,   371,   587,  1022,   589,   190,
     191,   192,   193,  1439,   120,  1165,  1023,  1024,   126,  1160,
     261,   194,  1724,  1161,   195,  1172,  1173,  1174,  1175,  1176,
    1177,  1189,  1190,   196,   197,   198,   199,   200,   127,  1184,
    1185,   128,  1461,  1462,  1463,  1186,  1187,   129,   467,  1825,
    1025,   452,   468,   469,   470,   471,   472,   473,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   649,  1184,  1185,
     136,   686,   687,  1085,  1186,  1187,  1184,  1185,  1464,   626,
     262,   626,  1186,  1095,  1096,  1097,  1184,  1185,   491,   340,
     341,   342,  1186,  1187,   263,   467,  1580,  1581,   452,   468,
     469,   470,   471,   472,   473,   474,   475,   476,   477,   478,
     479,   480,   481,   482,  1727,   492,  1728,  1727,  1156,  1157,
    1158,  1159,  1625,  1609,  1610,  1441,  1302,   467,  1127,  1128,
    1129,   468,   469,   470,   471,   472,   473,   474,   475,   476,
     477,   478,   479,   480,   481,   482,  1055,   463,   464,   493,
    1442,  1443,  1444,  1445,  1891,   494,  1056,  1138,  1139,   216,
    1448,   495,   265,  1530,  1660,  1661,   688,  1662,   689,   266,
    1057,   267,  1451,  1452,   268,   396,   269,  1453,  1913,   605,
     606,   607,   608,   270,  1458,  1459,  1460,  1156,  1157,  1158,
    1159,  1058,  1160,  1366,   216,   271,  1161,   904,   905,   906,
    1936,  1937,   736,  1355,  1356,  1357,  1358,   274,  1245,   273,
     396,  1725,   690,   691,   496,   737,   970,   971,   972,   497,
     275,   738,   397,  1141,  1142,   739,   909,  1239,  1960,  1008,
    1009,  1010,   740,   741,  1165,   276,   216,  1156,  1157,  1158,
    1159,  1180,  1725,  1181,  1182,   742,  1059,  1269,  1270,  1271,
    1680,   743,  1160,   744,   745,  1277,  1161,   397,   746,   747,
    1401,  1402,  1403,   277,   748,   398,  1549,  1550,  1551,  1060,
     287,  1287,  1994,  1995,   288,   399,  1395,  1119,   749,   750,
     412,   309,  1191,  1172,  1173,  1174,  1175,  1176,  1177,  1178,
    1179,   502,   278,   751,   310,   752,   311,   753,  1526,   754,
     398,  1450,  1701,   317,  1700,   755,   756,   483,   453,   757,
     758,  1158,  1159,   467,   318,   759,   760,   468,   469,   470,
     471,   472,   473,   474,   475,   476,   477,   478,   479,   480,
     481,   482,  1221,  1485,   279,   761,  1224,  1488,  1172,  1173,
    1174,  1175,  1176,  1177,  1178,  1179,   281,   762,   400,   282,
     763,  1184,  1185,   764,   642,   453,   283,   284,   503,   504,
     505,   506,   507,   508,   285,    -7,     1,   286,   289,    18,
      19,    20,  1364,    -7,   290,  1342,  1343,   291,   292,  1346,
    1347,  1348,  1349,   400,  1374,  1353,   729,  1466,  1467,  1468,
    1469,  1470,  1471,  1472,  1612,  1613,  1614,    -7,   293,   319,
     320,  1793,    -7,  1539,  1763,   323,   324,   325,   333,   338,
     339,   344,   345,  1172,  1173,  1174,  1175,  1176,  1177,  1189,
    1190,  1771,   347,   349,   350,   765,    -7,   352,   354,    -7,
     355,   356,   357,    -7,  1622,   358,   359,   360,   361,   362,
      -7,  1623,    -7,  1624,  1626,   363,    -7,   766,   364,   767,
     768,   769,   770,   771,   365,   366,   367,   368,   373,  1172,
    1173,  1174,  1175,  1176,  1177,  1189,  1190,   374,   375,   376,
     377,     5,  1572,   380,   378,   381,   382,   383,   387,   388,
     389,   390,   391,   409,  1829,   206,   449,   450,   465,   488,
     456,  1156,  1157,  1158,  1159,     6,    -7,   457,  1441,  1180,
      -7,  1181,  1182,   514,   515,   517,   518,   530,   531,   519,
     533,   536,   772,   773,   774,   775,   776,   777,   778,   779,
     534,   535,   538,    -7,     7,   539,   540,     8,    -7,    -7,
     541,     9,   542,   543,  1507,   780,   546,   520,    10,   544,
      11,   545,   547,   550,    12,   551,  1156,  1157,  1158,  1159,
     552,   566,   571,   553,  1180,  1524,  1181,  1182,   521,   570,
     560,   561,   563,   564,   522,   626,  1527,   523,   524,   574,
     601,   781,   963,   565,    -7,    -7,    -7,   569,   572,   782,
     573,   525,   579,  1724,   583,   783,  1172,  1173,  1174,  1175,
    1176,  1177,  1178,  1179,    13,   586,   588,    -7,    14,   591,
     592,   526,   593,   594,    -7,    -7,    -7,   595,    -7,    -7,
      -7,    -7,    -7,   596,    -7,   597,   598,   599,    -7,    -7,
      -7,    15,  1165,   600,   602,    -7,    16,    17,  1447,  1180,
     603,  1181,  1182,   610,   604,    -7,   467,    -7,    -7,   611,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,   616,   628,   614,   621,   615,
     622,   636,   639,   527,   528,    -7,    -7,    -7,  1165,   647,
     625,   629,    18,    19,    20,  1180,   632,  1181,  1182,    -7,
     664,    -7,   633,   665,    -7,   666,   667,   673,   685,   701,
     700,   702,   703,   704,   706,    21,   705,   707,   708,   710,
     709,   712,    22,    23,    24,   711,    25,    26,    27,    28,
      29,   721,    30,   731,   798,   713,    31,    32,    33,    -7,
    1791,   492,  1792,    34,   799,   837,   839,   714,   715,   841,
     724,   844,   884,    35,   467,    36,    37,   863,   468,   469,
     470,   471,   472,   473,   474,   475,   476,   477,   478,   479,
     480,   481,   482,   885,   801,   493,   802,   803,   804,   805,
     806,   494,   725,    38,    39,    40,   807,   495,   728,   735,
     886,   887,   888,   889,   890,   891,   892,    41,   893,    42,
     894,   895,    43,   896,   897,   912,   898,   909,   913,   914,
     916,   917,   918,   919,  1156,  1157,  1158,  1159,   920,   922,
     921,   924,  1180,  1725,  1181,  1182,   925,   927,   928,   930,
     929,   931,  1524,   941,   942,   943,   944,    44,   945,   946,
     496,   947,   949,   950,   953,   497,   954,   955,   956,   962,
     957,   808,   809,   810,   467,   969,   811,   812,   468,   469,
     470,   471,   472,   473,   474,   475,   476,   477,   478,   479,
     480,   481,   482,   974,   975,   991,   980,   981,   983,   984,
     985,   992,   987,   986,   993,   994,   995,  1002,   813,  1003,
    1004,  1005,  1006,  1007,   814,  1013,  1016,  1099,  1018,  1019,
    1027,  1028,  1030,  1031,  1116,  1033,  1034,  1038,  1039,  1040,
    1041,  1119,  1042,  1043,  1044,  1365,  1054,  1062,  1063,  1065,
    1066,  1101,  1067,  1068,  1069,  1070,  1071,  1072,  1073,  1074,
    1075,  1076,  1077,  1078,  1089,  1102,  1221,  1103,  1104,  1105,
    1107,  1108,  1109,   815,   816,   817,  1122,  1110,  1117,  1123,
    1124,  1125,  1135,  1151,  1192,  1126,  1193,  1194,  1195,  1196,
    1197,  1198,  1199,  1212,  1241,   818,   819,  1524,   820,  1201,
    1206,  1238,  1242,  1250,  1252,  1213,  1214,  1253,  1215,  1216,
    1217,  1218,  1219,  1220,  1256,  1224,  1246,  1247,  1251,  1257,
    1289,  1254,  1258,  1260,  1264,  1764,  1267,  1265,  1268,  1274,
    1275,  1281,  1290,  1291,  1283,  1284,  1285,  1286,  1288,  1292,
    1293,  1298,  1294,  1571,  1299,  1300,   821,   822,  1301,  1303,
    1304,   823,   824,   825,   826,   827,   828,  1305,  1306,  1311,
    1307,  1308,  1312,  1313,  1314,  1315,  1316,  1317,  1318,  1319,
    1320,  1321,  1323,  1322,  1324,  1325,  1336,  1340,  1350,  1360,
    1378,  1361,  1362,  1377,  1326,  1327,  1328,  1329,  1330,  1331,
    1332,  1379,  1333,  1334,  1335,  1339,  1380,  1344,  1381,  1345,
    1375,  1382,  1384,  1830,  1385,  1386,  1387,  1388,  1390,  1397,
    1391,  1394,  1398,  1399,  1400,  1405,  1406,   733,  1407,  1409,
    1410,  1408,  1412,  1413,  1411,  1414,  1418,  1419,  1420,  1475,
    1476,  1473,  1478,  1480,  1484,  1481,  1487,  1490,  1492,  1483,
    1494,  1495,  1496,  1497,  1498,  1499,  1500,  1501,  1513,  1502,
    1503,  1514,  1519,  1521,  1531,  1532,  1541,  1533,  1535,  1508,
    1534,  1517,  1543,  1547,  1548,  1575,  1578,  1583,  1515,  1597,
    1598,  1599,  1601,  1525,  1538,  1542,  1544,  1545,  1600,  1603,
    1546,  1555,  1556,  1604,  1605,  1606,  1608,  1616,  1607,  1617,
    1618,  1619,  1627,  1621,  1630,  1643,  1647,  1645,  1686,  1648,
    1659,  1557,  1665,  1666,  1670,  1558,  1655,  1672,  1649,  1673,
    1674,  1679,  1688,  1692,  1693,  1714,  1732,  1698,  1704,  1705,
    1706,  1707,  1710,  1711,  1712,  1713,  1709,  1717,  1720,  1723,
    1729,  1559,  1735,  1650,  1742,  1560,  1743,  1744,  1651,  1561,
    1745,  1563,  1750,  1564,  1757,  1758,  1760,  1762,  1576,  1766,
    1767,  1770,  1565,  1566,  1567,  1768,  1777,  1778,  1780,  1781,
    1782,  1568,  1569,  1783,  1570,  1573,  1574,  1785,  1584,  1585,
    1586,  1587,  1588,  1591,  1592,  1593,  1676,  1594,  1165,  1784,
    1794,  1631,  1795,  1632,  1633,  1634,  1635,  1652,  1636,  1754,
    1637,  1638,  1644,  1646,  1790,  1802,  1813,  1798,  1826,  1800,
    1808,  1667,  1653,  1668,  1675,  1820,  1862,  1817,  1681,  1677,
    1682,  1683,  1823,  1684,  1882,  1824,  1832,  1833,  1689,  1690,
    1836,  1691,  1694,  1870,  1699,  1838,  1702,  1837,  1811,  1839,
    1715,  1716,  1721,  1840,  1841,  1722,  1818,  1845,  1842,  1871,
    1843,  1738,  1846,  1751,  1739,  1761,  1769,  1849,  1773,  1850,
    1855,  1775,  1859,  1776,  1860,  1789,  1861,  1866,  1816,  1869,
    1851,  1867,  1874,  1821,  1828,  1875,  1876,  1879,  1883,  1835,
    1853,  1884,  1844,  1885,  1847,  1886,  1887,  1848,  1889,  1892,
    1898,  1899,  1872,  1900,  1873,  1903,  1901,  1904,  1906,  1908,
    1912,  1914,  1907,  -724,  1917,  1923,  1909,  1922,  1924,  1948,
    1925,  1919,  1930,  1932,  1934,  1935,  -725,  1940,  1897,  1941,
    1965,  1945,  1986,  1949,  1962,  1966,  1964,  1902,  1967,  1968,
    1981,  1905,  1926,  1946,  1978,  1961,  1984,  1987,  1989,  1990,
    1991,  1985,  1992,  1993,  1997,  1996,  1998,  1999,  2000,  2001,
    2004,  2002,  2005,  2008,  2010,  2011,  2003,  2012,  2013,  1822,
    2006,  2014,  2023,  2024,  2026,  2027,  2028,  2029,  2030,   393,
    2031,  2007,  2035,  1947,  2009,   385,  1752,  1255,  1815,  2015,
    2032,  1486,  1489,  1963,  1429,  1240,  1944,  1918,   650,  1273,
    1278,  1528,  2033,  2034,   487,  1774,  1367,  1553,  2036,  1430,
    1368,  1831,  1951,  1602,  1396,  1731,  1237,  1797,  1801,  1204,
    1209,  1100,  1799,     0,   513,     0,     0,     0,     0,     0,
       0,   926,     0,     0,     0,     0,     0,     0,     0,     0,
     961,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   908,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     968,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1012,     0,     0,
       0,     0,     0,     0,     0,  1015
};

static const short int yycheck[] =
{
     221,   517,   791,   518,   406,   493,   494,   793,   862,   207,
     847,   232,   204,    21,    62,   852,  1374,     9,   855,   534,
     535,   858,   859,    16,  1244,   652,   834,   835,    62,    62,
      62,    16,   840,    18,    14,    29,   137,    34,    54,  1259,
     854,   141,    55,    56,    37,    21,   155,   861,   126,    28,
     214,    36,   173,   172,   172,    35,   172,    37,    86,   174,
      88,    64,   174,    48,   168,    50,    51,   139,   882,   883,
     172,    99,    64,   236,   174,    37,    89,   172,   169,   170,
     171,    66,    46,   485,    33,   214,    14,   172,   174,    91,
      37,   212,    37,    22,   107,   169,   170,   171,    26,    84,
       6,   172,    30,    88,   172,  1523,    10,   172,   174,    37,
      91,   174,   174,   168,    93,   174,   101,    54,    22,    23,
      24,   134,   107,    72,   174,    37,   174,   168,   174,    91,
       9,   111,   135,   174,    38,    14,    15,   174,    37,   132,
     174,   174,   174,    92,   123,   161,   131,   132,   133,   142,
     212,   174,   237,   174,    33,   135,    35,   142,     5,   125,
     163,   110,   111,   384,   149,   174,   387,   174,    97,   172,
     134,   257,   237,     0,    53,    20,  1615,   174,    37,   107,
      16,   145,   146,   111,   174,   406,   135,   173,   174,    37,
     174,   174,   174,    97,   257,    10,   174,   356,   173,    46,
     174,    37,   174,    59,    37,    64,   112,    86,   174,   113,
     430,   174,   191,   174,   434,   174,   174,   174,   174,   168,
     148,   174,    50,   102,   161,  1583,   212,   126,    39,   215,
     216,   143,   111,   174,   237,    50,    64,   396,   166,   225,
    1658,   296,   644,    56,   236,    64,   173,  1163,   250,   353,
     129,  1167,   237,   198,   199,   296,   135,   204,    91,   238,
     236,   425,   426,   427,   428,   144,   113,   430,   430,   250,
     434,   172,   493,   494,   495,   496,    89,   156,    84,   174,
    1719,   266,   213,   257,  1504,   257,   217,   218,   250,   168,
     135,   271,   223,   224,   372,   101,   174,   426,   490,    94,
      95,  1090,   790,   114,   792,   434,  1092,   299,  1094,   238,
     431,   430,   430,   385,   430,   387,   431,  1171,   430,   431,
     392,   430,   298,    94,    95,   138,   172,   140,   430,   430,
     430,   431,   848,   849,   238,   430,   237,   174,   853,   430,
     418,   413,   355,   859,   859,   430,   973,  1184,  1185,  1186,
    1187,   315,   174,   172,   346,   430,   430,   365,   237,   430,
     376,   377,   430,   425,   430,   430,   394,   295,   431,   431,
     226,   430,   431,   352,   230,   190,  1076,   256,  1078,   258,
     430,   431,    60,   431,   430,   431,   389,   174,   382,  1203,
     375,   237,   271,   430,  1208,   398,   399,   431,   431,   431,
     349,   381,   395,   172,    82,    83,   174,   430,   431,   430,
     631,   290,   291,  1771,   422,   407,   408,   250,   237,   323,
     174,   430,   414,   430,   303,   304,   305,   306,   307,   308,
     309,   310,   311,   312,   431,   104,   400,   658,   174,   425,
     430,   110,   317,   318,   348,   431,   430,   430,   430,   641,
      94,    95,   430,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,   343,   344,   345,   430,   347,   430,
     349,   430,   430,   430,   430,   174,   426,   430,   357,   358,
     359,   214,   432,   163,   217,   218,   172,   366,   367,   368,
     223,   224,   172,   348,   370,   371,   351,    46,   353,   378,
     379,   380,   381,   434,   174,   426,    55,    56,   174,   430,
     172,   390,   214,   434,   393,   217,   218,   219,   220,   221,
     222,   223,   224,   402,   403,   404,   405,   406,   174,   217,
     218,   174,   233,   234,   235,   223,   224,   174,   171,  1759,
      89,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   237,   217,   218,
     174,    11,    12,   784,   223,   224,   217,   218,   269,   790,
     172,   792,   223,   794,   795,   796,   217,   218,    37,   292,
     293,   294,   223,   224,   174,   171,  1372,  1373,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,  1623,    64,  1625,  1626,   425,   426,
     427,   428,  1449,  1421,  1422,   432,  1018,   171,   281,   282,
     283,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,     7,   267,   268,    98,
    1156,  1157,  1158,  1159,  1864,   104,    17,   169,   170,    48,
    1165,   110,   430,  1280,   350,   351,   106,   353,   108,   172,
      31,   172,  1178,  1179,   172,    64,   172,  1183,  1888,   361,
     362,   363,   364,   172,  1189,  1190,  1191,   425,   426,   427,
     428,    52,   430,  1085,    48,   172,   434,   172,   173,   174,
    1910,  1911,    14,   361,   362,   363,   364,   172,   919,   430,
      64,   434,   152,   153,   163,    27,   172,   173,   174,   168,
     174,    33,   111,   169,   170,    37,   431,   432,  1938,   172,
     173,   174,    44,    45,   426,   430,    48,   425,   426,   427,
     428,   433,   434,   435,   436,    57,   107,   172,   173,   174,
    1526,    63,   430,    65,    66,   966,   434,   111,    70,    71,
     281,   282,   283,   430,    76,   154,   172,   173,   174,   130,
     172,   982,  1982,  1983,   172,   164,   430,   431,    90,    91,
     968,   174,  1626,   217,   218,   219,   220,   221,   222,   223,
     224,   131,   430,   105,    25,   107,   174,   109,  1276,   111,
     154,   432,  1581,   174,  1580,   117,   118,   430,   431,   121,
     122,   427,   428,   171,   174,   127,   128,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   431,   432,   430,   147,   431,   432,   217,   218,
     219,   220,   221,   222,   223,   224,   430,   159,   237,   430,
     162,   217,   218,   165,   430,   431,   430,   430,   198,   199,
     200,   201,   202,   203,   430,     0,     1,   430,   430,   209,
     210,   211,  1083,     8,   430,  1067,  1068,   430,   430,  1071,
    1072,  1073,  1074,   237,  1095,  1077,   430,  1193,  1194,  1195,
    1196,  1197,  1198,  1199,   172,   173,   174,    32,   430,   174,
     174,  1728,    37,  1295,  1680,   174,   174,   174,   174,   174,
     174,   292,   174,   217,   218,   219,   220,   221,   222,   223,
     224,  1700,   174,   174,   174,   237,    61,   174,   174,    64,
     174,   174,   174,    68,  1440,   174,   174,   174,   174,   174,
      75,  1446,    77,  1449,  1449,   174,    81,   259,   174,   261,
     262,   263,   264,   265,   174,   174,   174,   174,   174,   217,
     218,   219,   220,   221,   222,   223,   224,   174,   174,   174,
     174,     8,  1364,   174,   416,   174,   174,   174,   431,   174,
     430,   174,    37,    29,  1763,    64,   172,   172,    64,   172,
     174,   425,   426,   427,   428,    32,   131,   174,   432,   433,
     135,   435,   436,    67,   430,   433,   433,   430,   430,   433,
     430,   430,   324,   325,   326,   327,   328,   329,   330,   331,
     433,   433,   100,   158,    61,    73,   388,    64,   163,   164,
     158,    68,    74,    85,  1245,   347,   297,    37,    75,    78,
      77,   167,   430,   430,    81,   430,   425,   426,   427,   428,
     430,   172,   174,   430,   433,  1266,   435,   436,    58,   431,
     430,   430,   430,   430,    64,  1276,  1277,    67,    68,   174,
     174,   383,   430,   430,   209,   210,   211,   430,   430,   391,
     430,    81,   430,   214,   430,   397,   217,   218,   219,   220,
     221,   222,   223,   224,   131,   430,   430,   232,   135,   430,
     430,   101,   430,   430,   239,   240,   241,   430,   243,   244,
     245,   246,   247,   430,   249,   430,   430,   430,   253,   254,
     255,   158,   426,   430,   430,   260,   163,   164,   432,   433,
     430,   435,   436,   174,   168,   270,   171,   272,   273,   430,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   369,   174,   430,   430,   353,
     430,   174,    37,   163,   164,   300,   301,   302,   426,   172,
     430,   412,   209,   210,   211,   433,   430,   435,   436,   314,
     135,   316,   430,   172,   319,    61,   172,   174,   348,   172,
     430,   172,   172,   172,   172,   232,   430,   174,   174,   172,
     430,    13,   239,   240,   241,   430,   243,   244,   245,   246,
     247,   172,   249,    37,   174,   430,   253,   254,   255,   354,
    1726,    64,  1727,   260,    37,   172,   174,   430,   430,    37,
     430,   384,   172,   270,   171,   272,   273,   232,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   174,    37,    98,    39,    40,    41,    42,
      43,   104,   430,   300,   301,   302,    49,   110,   430,   430,
     174,   174,   174,   174,   174,   174,   174,   314,   430,   316,
     431,   430,   319,   430,   430,   174,   431,   431,   430,   295,
     174,   430,   174,   431,   425,   426,   427,   428,   430,   313,
     430,   430,   433,   434,   435,   436,   430,   430,   174,   174,
     430,   168,  1523,   174,   174,   168,   174,   354,   168,   430,
     163,   430,   174,   168,   172,   168,   172,   430,   172,   430,
     172,   114,   115,   116,   171,   430,   119,   120,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   172,    37,   430,   174,   174,   174,   174,
     174,   430,   172,   174,   430,   172,    13,   430,   151,   430,
     430,   430,   174,   174,   157,   172,   430,   430,   174,   174,
     174,   174,   172,   174,   207,   174,   174,   174,   174,   174,
     174,   431,   174,   174,   174,   430,   174,   174,   174,   172,
     174,   149,   174,   174,   174,   174,   174,   174,   174,   174,
     174,   174,   174,   174,   172,   174,   431,   174,   174,   174,
     174,   174,   174,   206,   207,   208,   430,   174,   174,   173,
     173,   173,   172,   430,   172,   227,   172,   172,   172,   172,
     172,   172,   172,   172,   432,   228,   229,  1658,   231,   174,
     174,   174,   174,   174,   174,   430,   430,   174,   430,   430,
     430,   430,   430,   430,   168,   431,   430,   430,   430,   135,
      13,   430,   174,   174,   174,  1686,   430,   374,   430,   172,
     430,   430,   150,   174,   430,   430,   430,   430,   430,   174,
     174,   432,   174,   430,   430,   430,   279,   280,   172,   430,
     430,   284,   285,   286,   287,   288,   289,   430,   430,   172,
     430,   430,   430,   430,   174,   172,   430,   172,   430,   430,
     172,   174,   430,   174,   430,   430,   172,    64,    64,   172,
     174,   173,   172,   172,   430,   430,   430,   430,   430,   430,
     430,   174,   430,   430,   430,   430,   174,   430,   174,   430,
     430,   172,   174,  1764,   174,   174,   174,   172,   430,   430,
     174,   174,   430,   430,   174,   174,   157,   174,   430,   174,
     174,   430,   174,   174,   430,   430,   430,   172,   172,   430,
     245,   174,   174,   430,   174,   249,   174,   419,   174,   430,
     174,   174,   174,   174,   174,   174,   430,   174,   174,   430,
     430,   174,   168,   174,   172,   430,    34,   174,   430,   369,
     174,   420,   174,   174,   174,    37,    34,   172,   369,   208,
     174,   174,   174,   373,   430,   430,   430,   430,   227,    34,
     430,   430,   430,    34,    34,    34,   174,   172,   203,   172,
     172,    37,   173,   213,   172,   174,   172,   174,   110,   174,
     174,   430,   174,   174,   174,   430,   174,   174,   236,   174,
     174,   174,   174,   172,   172,   205,   251,   174,   174,   174,
     174,   174,   174,   174,   174,   174,   227,   172,   174,   172,
     236,   430,   251,   369,   174,   430,   174,   174,   369,   430,
     174,   430,   174,   430,   135,   168,   174,   174,   411,    13,
     174,    13,   430,   430,   430,   174,   174,   174,   174,   139,
     139,   430,   430,   139,   430,   430,   430,   174,   430,   430,
     430,   430,   430,   430,   430,   430,   401,   430,   426,   139,
     174,   430,   174,   430,   430,   430,   430,   415,   430,   369,
     430,   430,   430,   430,   214,   137,   229,   174,   135,   174,
     174,   430,   432,   430,   430,   174,   135,   369,   430,   432,
     430,   430,   174,   430,   236,   174,   174,   174,   430,   430,
     174,   430,   430,   139,   430,   160,   430,   174,   414,   174,
     430,   430,   430,   174,   174,   430,   409,   168,   174,   139,
     174,   432,   174,   430,   432,   430,   430,   174,   430,   174,
     174,   432,   174,   430,   174,   430,   174,   174,   432,   174,
     410,   386,   174,   430,   430,   174,   174,   174,   174,   430,
     417,   174,   430,   174,   430,   174,   229,   430,   174,   168,
     174,   174,   430,   174,   430,    64,   174,    64,   252,   174,
     174,   168,   248,   430,   174,   174,   369,   172,   174,   172,
     174,   401,   174,   174,   174,   174,   430,   174,   430,   168,
     135,   174,    25,   174,   174,   172,   174,   430,   174,   172,
     174,   430,   430,   430,   421,   369,   174,   174,   125,   174,
     174,   430,   174,   135,   174,   374,   430,   172,   174,   430,
     174,   430,   174,   174,   349,   174,   430,   174,   174,  1756,
     430,   174,   174,   174,   174,   174,   174,   172,   174,   202,
     174,   430,   174,  1921,   430,   195,  1664,   939,  1748,   430,
     430,  1222,  1225,  1943,  1152,   910,  1917,  1894,   413,   958,
     968,  1278,   430,   430,   234,  1704,  1086,  1337,   430,  1152,
    1088,  1765,  1928,  1406,  1120,  1629,   903,  1731,  1736,   874,
     879,   800,  1733,    -1,   256,    -1,    -1,    -1,    -1,    -1,
      -1,   590,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     641,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   567,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     646,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   722,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   727
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned short int yystos[] =
{
       0,     1,   438,   443,     0,     8,    32,    61,    64,    68,
      75,    77,    81,   131,   135,   158,   163,   164,   209,   210,
     211,   232,   239,   240,   241,   243,   244,   245,   246,   247,
     249,   253,   254,   255,   260,   270,   272,   273,   300,   301,
     302,   314,   316,   319,   354,   439,   441,   442,   445,   446,
     447,   448,   449,   450,   454,   455,   458,   459,   553,   555,
     557,   558,   585,   587,   588,   606,   607,   612,   613,   620,
     621,   622,   643,   644,   660,   662,   774,   776,   794,   796,
     798,   809,   846,   847,   848,   849,   850,   862,   874,   875,
     876,   877,   878,   879,   880,   881,   777,   174,   460,   663,
     174,    94,    95,   623,   645,   586,   799,   800,   801,   810,
     173,   440,   174,   174,   174,   174,   174,   174,   174,   174,
     174,   173,    94,    95,   795,   797,   174,   174,   174,   174,
     317,   318,   451,    84,   101,   452,   174,   456,     9,    14,
      15,    33,    35,    53,    86,   102,   111,   129,   135,   144,
     156,   168,   237,   256,   258,   271,   290,   291,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,   343,
     344,   345,   347,   349,   357,   358,   359,   366,   367,   368,
     378,   379,   380,   381,   390,   393,   402,   403,   404,   405,
     406,   463,   464,   172,   556,    50,    64,   591,   597,   600,
     609,   615,   648,    16,    18,    36,    48,    50,    51,    66,
      84,    88,   101,   107,   131,   132,   133,   142,   149,   237,
     266,   375,   574,   666,   667,   670,   674,   679,   680,   681,
     682,   683,   684,   685,   687,   689,   690,   692,   693,   694,
     742,   743,   744,   751,   753,   754,   780,    37,   444,   172,
     430,   172,   172,   174,   430,   430,   172,   172,   172,   172,
     172,   172,   811,   430,   172,   174,   430,   430,   430,   430,
     851,   430,   430,   430,   430,   430,   430,   172,   172,   430,
     430,   430,   430,   430,    94,    95,   453,   236,   430,    14,
      26,    30,    37,   107,   111,   148,   166,   295,   457,   174,
      25,   174,    54,   161,   376,   377,   541,   174,   174,   174,
     174,   125,   174,   174,   174,   174,    28,    93,   123,   191,
     238,   352,   540,   174,   469,   174,   257,   431,   174,   174,
     292,   293,   294,   521,   292,   174,   475,   174,   476,   174,
     174,   478,   174,   479,   174,   174,   174,   174,   174,   174,
     174,   174,   174,   174,   174,   174,   174,   174,   174,   481,
     370,   371,   505,   174,   174,   174,   174,   174,   416,   487,
     174,   174,   174,   174,   490,   505,   465,   431,   174,   430,
     174,    37,   461,   463,    29,   382,    64,   111,   154,   164,
     237,   559,   566,   567,   569,   573,   574,   577,   578,    29,
     590,   601,   597,   598,   602,    37,   126,   608,   610,   611,
      37,   143,   614,   616,   619,    16,    37,   132,   142,   395,
     646,   649,   650,   651,   654,    10,    22,    23,    24,    38,
      97,   113,   238,   323,   348,   675,   752,   686,   575,   172,
     172,   688,   174,   431,   659,   695,   174,   174,    10,    50,
     190,   671,   668,   267,   268,    64,   746,   171,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   430,   576,   659,   661,   666,   172,   733,
     698,    37,    64,    98,   104,   110,   163,   168,   719,   720,
     729,   756,   131,   198,   199,   200,   201,   202,   203,   691,
     775,   781,   787,   798,    67,   430,   624,   433,   433,   433,
      37,    58,    64,    67,    68,    81,   101,   163,   164,   812,
     430,   430,   863,   430,   433,   433,   430,   554,   100,    73,
     388,   158,    74,    85,    78,   167,   297,   430,   174,   257,
     430,   430,   430,   430,   174,   430,   174,   430,   174,   257,
     430,   430,   467,   430,   430,   430,   172,   532,   533,   430,
     431,   174,   430,   430,   174,   470,   168,   174,   296,   430,
     174,   257,   516,   430,   516,   477,   430,   516,   430,   516,
     480,   430,   430,   430,   430,   430,   430,   430,   430,   430,
     430,   174,   430,   430,   168,   361,   362,   363,   364,   543,
     174,   430,   484,   486,   430,   353,   369,    21,   365,   422,
     495,   430,   430,   174,   430,   430,   659,   726,   174,   412,
     491,   659,   430,   430,   462,   579,   174,   560,   570,    37,
     583,   568,   430,   576,   659,   580,   589,   172,   172,   237,
     557,   592,   593,   594,   599,    33,    72,    92,   110,   111,
     135,   168,   349,   603,   135,   172,    61,   172,   617,    22,
      97,   238,   647,   174,   652,   655,   356,   396,    46,   134,
     145,   146,   315,   400,   677,   348,    11,    12,   106,   108,
     152,   153,   678,    55,    56,    89,   107,   134,   355,   676,
     430,   172,   172,   172,   172,   430,   172,   174,   174,   430,
     172,   430,    13,   430,   430,   430,   169,   170,   171,   430,
     672,   172,   669,   673,   430,   430,   747,   745,   430,   430,
     576,    37,   664,   174,   430,   430,    14,    27,    33,    37,
      44,    45,    57,    63,    65,    66,    70,    71,    76,    90,
      91,   105,   107,   109,   111,   117,   118,   121,   122,   127,
     128,   147,   159,   162,   165,   237,   259,   261,   262,   263,
     264,   265,   324,   325,   326,   327,   328,   329,   330,   331,
     347,   383,   391,   397,   574,   696,   699,   713,   714,   721,
      62,   726,    62,   726,    62,   659,    62,   659,   174,    37,
     723,    37,    39,    40,    41,    42,    43,    49,   114,   115,
     116,   119,   120,   151,   157,   206,   207,   208,   228,   229,
     231,   279,   280,   284,   285,   286,   287,   288,   289,   755,
     757,   761,   762,   770,   783,   784,   785,   172,   786,   174,
     782,    37,   778,   788,   384,   627,   174,   212,   425,   431,
     805,   173,   212,   431,   807,   212,   215,   216,   225,   431,
     805,   806,   807,   232,   814,   819,   813,   816,   820,   815,
     817,   818,    91,   250,   865,   866,   867,    91,   250,   853,
     854,   855,   807,   807,   172,   174,   174,   174,   174,   174,
     174,   174,   174,   430,   431,   430,   430,   430,   431,   126,
     372,   418,   831,   832,   172,   173,   174,   430,   533,   431,
     534,   535,   174,   430,   295,   522,   174,   430,   174,   431,
     430,   430,   313,   520,   430,   430,   520,   430,   174,   430,
     174,   168,   542,   139,   385,   387,   392,   413,   513,   514,
     515,   174,   174,   168,   174,   168,   430,   430,   728,   174,
     168,   492,   659,   172,   172,   430,   172,   172,   571,   572,
     584,   569,   430,   430,   576,   104,   110,   581,   591,   430,
     172,   173,   174,   595,   172,    37,   604,   594,    54,   161,
     174,   174,   659,   174,   174,   174,   174,   172,   174,   430,
     618,   430,   430,   430,   172,    13,   169,   170,   171,   430,
     653,   656,   430,   430,   430,   430,   174,   174,   172,   173,
     174,   430,   673,   172,    37,   746,   430,   665,   174,   174,
       6,   112,    46,    55,    56,    89,   697,   174,   174,   704,
     172,   174,   700,   174,   174,   701,   702,   703,   174,   174,
     174,   174,   174,   174,   174,    56,    89,   138,   140,   717,
       5,    46,   113,   718,   174,     7,    17,    31,    52,   107,
     130,   716,   174,   174,   705,   172,   174,   174,   174,   174,
     174,   174,   174,   174,   174,   174,   174,   174,   174,   706,
     709,   707,   708,   141,   430,   659,    16,    37,   715,   172,
     726,   728,   726,   659,   727,   659,   659,   659,   730,   430,
     720,   149,   174,   174,   174,   174,   758,   174,   174,   174,
     174,   759,    60,    82,    83,   771,   207,   174,   768,   431,
     766,   767,   430,   173,   173,   173,   227,   281,   282,   283,
     763,    59,   226,   230,   765,   172,   733,   733,   169,   170,
     734,   169,   170,   741,   792,   733,   779,    37,   198,   199,
     789,   430,   628,   806,   805,   805,   425,   426,   427,   428,
     430,   434,   802,   806,   807,   426,   802,   806,   806,   805,
     806,   807,   217,   218,   219,   220,   221,   222,   223,   224,
     433,   435,   436,   808,   217,   218,   223,   224,   802,   223,
     224,   808,   172,   172,   172,   172,   172,   172,   172,   172,
     868,   174,    37,   864,   866,   856,   174,    37,   852,   854,
     802,   802,   172,   430,   430,   430,   430,   430,   430,   430,
     430,   431,   536,   537,   431,   538,   539,   833,     9,    64,
     236,   299,   346,   407,   408,   414,   834,   831,   174,   432,
     535,   432,   174,   473,   471,   659,   430,   430,   485,   488,
     174,   430,   174,   174,   430,   513,   168,   135,   174,   496,
     174,   430,   727,   489,   174,   374,   482,   430,   430,   172,
     173,   174,   430,   572,   172,   430,   582,   659,   592,   594,
     596,   430,   605,   430,   430,   430,   430,   659,   430,    13,
     150,   174,   174,   174,   174,   172,   430,   657,   432,   430,
     430,   172,   576,   430,   430,   430,   430,   430,   430,   155,
     430,   172,   430,   430,   174,   172,   430,   172,   430,   430,
     172,   174,   174,   430,   430,   430,   430,   430,   430,   430,
     430,   430,   430,   430,   430,   430,   172,   711,   712,   430,
      64,   842,   842,   842,   430,   430,   842,   842,   842,   842,
      64,   844,   844,   842,   844,   361,   362,   363,   364,   710,
     172,   173,   172,   430,   659,   430,   576,   675,   719,   722,
     728,   430,   727,   727,   659,   430,   731,   172,   174,   174,
     174,   174,   172,   772,   174,   174,   174,   174,   172,   773,
     430,   174,   174,   430,   174,   430,   767,   430,   430,   430,
     174,   281,   282,   283,   764,   174,   157,   430,   430,   174,
     174,   430,   174,   174,   430,    37,   204,   793,   430,   172,
     172,   790,   791,    64,   172,   237,   389,   398,   399,   555,
     606,   625,   629,   630,   631,   632,   633,   636,   213,   434,
     803,   432,   805,   805,   805,   805,   803,   432,   807,   803,
     432,   805,   805,   805,   806,   806,   806,   806,   807,   807,
     807,   233,   234,   235,   269,   821,   821,   821,   821,   821,
     821,   821,   821,   174,   869,   430,   245,   802,   174,   857,
     430,   249,   802,   430,   174,   432,   537,   174,   432,   539,
     419,   828,   174,   835,   174,   174,   174,   174,   174,   174,
     430,   174,   430,   430,   523,   531,   531,   659,   369,   510,
     168,   353,   506,   174,   174,   369,   531,   420,   550,   168,
     509,   174,   518,   519,   659,   373,   726,   659,   604,   430,
     594,   172,   430,   174,   174,   430,   137,   430,   430,   576,
     748,    34,   430,   174,   430,   430,   430,   174,   174,   172,
     173,   174,   430,   712,   843,   430,   430,   430,   430,   430,
     430,   430,   845,   430,   430,   430,   430,   430,   430,   430,
     430,   430,   576,   430,   430,    37,   411,   724,    34,   732,
     727,   727,   732,   172,   430,   430,   430,   430,   430,   172,
     430,   430,   430,   430,   430,   172,   430,   208,   174,   174,
     227,   174,   771,    34,    34,    34,    34,   203,   174,   733,
     733,   637,   172,   173,   174,   634,   172,   172,   172,    37,
     626,   213,   805,   807,   805,   806,   807,   173,   822,   823,
     172,   430,   430,   430,   430,   430,   430,   430,   430,   174,
     430,   174,   430,   174,   430,   174,   430,   172,   174,   236,
     369,   369,   415,   432,   531,   174,   430,   430,   517,   174,
     350,   351,   353,   511,   512,   174,   174,   430,   430,   468,
     174,   497,   174,   174,   174,   430,   401,   432,   518,   174,
     727,   430,   430,   430,   430,   658,   110,   750,   174,   430,
     430,   430,   172,   172,   430,    20,   135,   725,   174,   430,
     727,   728,   430,   732,   174,   174,   174,   174,   760,   227,
     174,   174,   174,   174,   205,   430,   430,   172,   633,   635,
     174,   430,   430,   172,   214,   434,   804,   804,   804,   236,
     829,   829,   251,   870,   871,   251,   858,   859,   432,   432,
     836,   838,   174,   174,   174,   174,   430,   474,   472,   518,
     174,   430,   511,   507,   369,   503,   504,   135,   168,   500,
     174,   430,   174,   727,   659,   749,    13,   174,   174,   430,
      13,   728,   430,   430,   770,   432,   430,   174,   174,   769,
     174,   139,   139,   139,   139,   174,   638,   430,   633,   430,
     214,   805,   807,   806,   174,   174,   830,   830,   174,   871,
     174,   859,   137,   840,    21,   236,   298,   824,   174,   837,
     839,   414,   524,   229,   529,   529,   432,   369,   409,   508,
     174,   430,   503,   174,   174,   531,   135,   483,   430,   728,
     659,   750,   174,   174,   732,   430,   174,   174,   160,   174,
     174,   174,   174,   174,   430,   168,   174,   430,   430,   174,
     174,   410,   841,   417,   825,   174,   168,   296,   526,   174,
     174,   174,   135,   502,   498,   501,   174,   386,   430,   174,
     139,   139,   430,   430,   174,   174,   174,   735,   737,   174,
     872,   860,   236,   174,   174,   174,   174,   229,   530,   174,
     551,   531,   168,   547,   548,   466,   561,   430,   174,   174,
     174,   174,   430,    64,    64,   430,   252,   248,   174,   369,
     528,   527,   174,   531,   168,   552,   499,   174,   547,   401,
     493,   494,   172,   174,   174,   174,   430,   736,   738,   639,
     174,   873,   174,   861,   174,   174,   531,   531,   525,   430,
     174,   168,   544,   545,   550,   174,   430,   493,   172,   174,
     739,   739,   641,   174,   430,   174,   430,   826,   430,   430,
     531,   369,   174,   544,   174,   135,   172,   174,   172,   740,
      14,    35,    37,   111,   135,   271,   381,   642,   421,   827,
     430,   174,   546,   549,   174,   430,    25,   174,   640,   125,
     174,   174,   174,   135,   531,   531,   374,   174,   430,   172,
     174,   430,   430,   430,   174,   174,   430,   430,   174,   430,
     349,   174,   174,   174,   174,   430,   562,   563,    86,    88,
      99,   394,   564,   174,   174,   565,   174,   174,   174,   172,
     174,   174,   430,   430,   430,   174,   430
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


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
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (0)


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (N)								\
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (0)
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
              (Loc).first_line, (Loc).first_column,	\
              (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
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

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr,					\
                  Type, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short int *bottom, short int *top)
#else
static void
yy_stack_print (bottom, top)
    short int *bottom;
    short int *top;
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
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname[yyr1[yyrule]]);
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
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
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
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

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
  const char *yys = yystr;

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
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      size_t yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

#endif /* YYERROR_VERBOSE */



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
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);


# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

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



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
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
    ;
#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short int yyssa[YYINITDEPTH];
  short int *yyss = yyssa;
  short int *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



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
	short int *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short int *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
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
/* Read a look-ahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to look-ahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
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
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
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

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

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
#line 508 "lef.y"
    {
        // 11/16/2001 - Wanda da Rosa - pcr 408334
        // Return 1 if there are errors
        if (lef_errors)
           return 1;
        if (!hasVer)
           lefWarning(2001, "VERSION is a required statement on LEF file with version 5.5 and earlier.\nWithout VERSION defined, the LEF file is technically illegal.\nRefer the LEF/DEF 5.5 and earlier Language Reference manual on how to defined this statement.");
        //only pre 5.6, 5.6 it is obsoleted
        if (!hasNameCase && versionNum < 5.6)
           lefWarning(2002, "NAMESCASESENSITIVE is a required statement on LEF file with version 5.5 and earlier.\nWithout NAMESCASESENSITIVE defined, the LEF file is technically illegal.\nRefer the LEF/DEF 5.5 and earlier Language Referece manual on how to define this statement.");
        if (!hasBusBit && versionNum < 5.6)
           lefWarning(2003, "BUSBITCHARS is a required statement on LEF file with version 5.5 and earlier.\nWithout BUSBITCHARS defined, the LEF file is technically illegal.\nRefer the LEF/DEF 5.5 and earlier Language Referece manual on how to define this statement.");
        if (!hasDivChar && versionNum < 5.6)
           lefWarning(2004, "DIVIDERCHAR is a required statementon LEF file with version 5.5 and earlier.\nWithout DIVIDECHAR defined, the LEF file is technically illegal.\nRefer the LEF/DEF 5.5 and earlier Language Referece manual on how to define this statement.");

       (void)resetVars();
/*
        hasVer = 0;
        hasNameCase = 0;
        hasBusBit = 0;
        hasDivChar = 0;
        hasManufactur = 0;
        antennaInoutWarnings = 0;
        antennaInputWarnings = 0;
        antennaOutputWarnings = 0;
        arrayWarnings = 0;
        caseSensitiveWarnings = 0;
        correctionTableWarnings = 0;
        dielectricWarnings = 0;
        edgeRateThreshold1Warnings = 0;
        edgeRateThreshold2Warnings = 0;
        edgeRateScaleFactorWarnings = 0;
        inoutAntennaWarnings = 0;
        inputAntennaWarnings = 0;
        iRDropWarnings = 0;
        layerWarnings = 0;
        macroWarnings = 0;
        maxStackViaWarnings = 0;
        minFeatureWarnings = 0;
        noiseMarginWarnings = 0;
        noiseTableWarnings = 0;
        nonDefaultWarnings = 0;
        noWireExtensionWarnings = 0;
        outputAntennaWarnings = 0;
        siteWarnings = 0;
        spacingWarnings = 0;
        timingWarnings = 0;
        unitsWarnings = 0;
        useMinSpacingWarnings = 0;
        viaRuleWarnings = 0;
        viaWarnings = 0;
*/
      ;}
    break;

  case 3:
#line 562 "lef.y"
    { lefDumbMode = 1; lefNoNum = 1;;}
    break;

  case 4:
#line 563 "lef.y"
    { 
         versionNum = convert_name2num((yyvsp[-1].string));
         if (versionNum > 5.7) {
            char temp[120];
            sprintf(temp,
               "Lef parser 5.7 does not support lef file with version %s. Parser stops executions.", (yyvsp[-1].string));
            lefError(1503, temp);
            return 1;
         }
/*
         versionNum = $3;         Save the version number for future use */
         if (lefrVersionStrCbk) {
            CALLBACK(lefrVersionStrCbk, lefrVersionStrCbkType, (yyvsp[-1].string));
         } else {
            if (lefrVersionCbk)
               CALLBACK(lefrVersionCbk, lefrVersionCbkType, versionNum);
         }
         if (versionNum > 5.3 && versionNum < 5.4) {
            ignoreVersion = 1;
         }
         use5_3 = use5_4 = 0;
         lef_errors = 0;
         if (hasVer)     /* More than 1 lef file within the open file */
            (void)resetVars();
         hasVer = 1;
         if (versionNum < 5.6) {
            doneLib = 0;
            lefNamesCaseSensitive = lefReaderCaseSensitive;
         } else {
            doneLib = 1;
            lefNamesCaseSensitive = 1;
         }
      ;}
    break;

  case 5:
#line 599 "lef.y"
    {
        if (lefrDividerCharCbk) {
          if (strcmp((yyvsp[-1].string), "") != 0) {
             CALLBACK(lefrDividerCharCbk, lefrDividerCharCbkType, (yyvsp[-1].string));
          } else {
             CALLBACK(lefrDividerCharCbk, lefrDividerCharCbkType, "/");
             lefWarning(2005, "DIVIDERCHAR has an invalid null value. Value is set to default /");
          }
        }
        hasDivChar = 1;
      ;}
    break;

  case 6:
#line 612 "lef.y"
    {
        if (lefrBusBitCharsCbk) {
          if (strcmp((yyvsp[-1].string), "") != 0) {
             CALLBACK(lefrBusBitCharsCbk, lefrBusBitCharsCbkType, (yyvsp[-1].string)); 
          } else {
             CALLBACK(lefrBusBitCharsCbk, lefrBusBitCharsCbkType, "[]"); 
             lefWarning(2006, "BUSBITCHAR has an invalid null value. Value is set to default []");
          }
        }
        hasBusBit = 1;
      ;}
    break;

  case 9:
#line 627 "lef.y"
    { ;}
    break;

  case 10:
#line 630 "lef.y"
    {
        if (versionNum >= 5.6) {
           doneLib = 1;
           ge56done = 1;
        }
      ;}
    break;

  case 11:
#line 637 "lef.y"
    {
        doneLib = 1;
        ge56done = 1;
        if (lefrLibraryEndCbk)
          CALLBACK(lefrLibraryEndCbk, lefrLibraryEndCbkType, 0);
        // 11/16/2001 - Wanda da Rosa - pcr 408334
        // Return 1 if there are errors
/*
        if (lef_errors)
           return 1;
        if (!hasVer)
           lefWarning(2001, "VERSION is a required statement.");
        if (!hasNameCase)
           lefWarning(2002, "NAMESCASESENSITIVE is a required statement.");
        if (!hasBusBit && versionNum < 5.6)
           lefWarning(2003, "BUSBITCHARS is a required statement.");
        if (!hasDivChar && versionNum < 5.6)
           lefWarning(2004, "DIVIDERCHAR is a required statement.");
        hasVer = 0;
        hasNameCase = 0;
        hasBusBit = 0;
        hasDivChar = 0;
        hasManufactur = 0;
*/
      ;}
    break;

  case 50:
#line 678 "lef.y"
    {
            if (versionNum < 5.6) {
              lefNamesCaseSensitive = TRUE;
              if (lefrCaseSensitiveCbk)
                CALLBACK(lefrCaseSensitiveCbk, lefrCaseSensitiveCbkType,
			        lefNamesCaseSensitive);
              hasNameCase = 1;
            } else
              if (lefrCaseSensitiveCbk) /* write warning only if cbk is set */
                 if (caseSensitiveWarnings++ < lefrCaseSensitiveWarnings)
                   lefWarning(2007, "NAMESCASESENSITIVE is obsoleted in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
	  ;}
    break;

  case 51:
#line 691 "lef.y"
    {
            if (versionNum < 5.6) {
	      lefNamesCaseSensitive = FALSE;
              if (lefrCaseSensitiveCbk)
	        CALLBACK(lefrCaseSensitiveCbk, lefrCaseSensitiveCbkType,
			       lefNamesCaseSensitive);
              hasNameCase = 1;
            } else {
              if (lefrCaseSensitiveCbk) { /* write error only if cbk is set */
                if (caseSensitiveWarnings++ < lefrCaseSensitiveWarnings) {
                  lefError(1504, "NAMESCASESENSITIVE is set with OFF.\nStarting version 5.6, NAMESCASENSITIVE is obsoleted,\nif it is defined, it has to have the ON value.\nParser stops executions.");
                  CHKERR();
                }
              }
            }
	  ;}
    break;

  case 52:
#line 709 "lef.y"
    {
      if (versionNum < 5.6) {
        if (lefrNoWireExtensionCbk)
          CALLBACK(lefrNoWireExtensionCbk, lefrNoWireExtensionCbkType, "ON");
      } else
        if (lefrNoWireExtensionCbk) /* write warning only if cbk is set */
           if (noWireExtensionWarnings++ < lefrNoWireExtensionWarnings)
             lefWarning(2008, "NOWIREEXTENSIONATPIN is obsoleted in version 5.6 or later.\nThe NOWIREEXTENSIONATPIN statement will be ignored.");
    ;}
    break;

  case 53:
#line 719 "lef.y"
    {
      if (versionNum < 5.6) {
        if (lefrNoWireExtensionCbk)
          CALLBACK(lefrNoWireExtensionCbk, lefrNoWireExtensionCbkType, "OFF");
      } else
        if (lefrNoWireExtensionCbk) /* write warning only if cbk is set */
           if (noWireExtensionWarnings++ < lefrNoWireExtensionWarnings)
             lefWarning(2008, "NOWIREEXTENSIONATPIN is obsoleted in version 5.6 or later.\nThe NOWIREEXTENSIONATPIN statement will be ignored.");
    ;}
    break;

  case 54:
#line 730 "lef.y"
    {
      if (lefrManufacturingCbk)
        CALLBACK(lefrManufacturingCbk, lefrManufacturingCbkType, (yyvsp[-1].dval));
      hasManufactur = 1;
    ;}
    break;

  case 55:
#line 737 "lef.y"
    {
    if ((strcmp((yyvsp[-2].string), "PIN") == 0) && (versionNum >= 5.6)) {
      if (lefrUseMinSpacingCbk) /* write warning only if cbk is set */
         if (useMinSpacingWarnings++ < lefrUseMinSpacingWarnings)
            lefWarning(2009, "USEMINSPACING PIN is obsoleted in version 5.6 or later.\n The USEMINSPACING PIN statement will be ignored.");
    } else {
        if (lefrUseMinSpacingCbk) {
          lefrUseMinSpacing.lefiUseMinSpacing::set((yyvsp[-2].string), (yyvsp[-1].integer));
          CALLBACK(lefrUseMinSpacingCbk, lefrUseMinSpacingCbkType,
                   &lefrUseMinSpacing);
      }
    }
  ;}
    break;

  case 56:
#line 752 "lef.y"
    { CALLBACK(lefrClearanceMeasureCbk, lefrClearanceMeasureCbkType, (yyvsp[-1].string)); ;}
    break;

  case 57:
#line 755 "lef.y"
    {(yyval.string) = (char*)"MAXXY";;}
    break;

  case 58:
#line 756 "lef.y"
    {(yyval.string) = (char*)"EUCLIDEAN";;}
    break;

  case 59:
#line 759 "lef.y"
    {(yyval.string) = (char*)"OBS";;}
    break;

  case 60:
#line 760 "lef.y"
    {(yyval.string) = (char*)"PIN";;}
    break;

  case 61:
#line 763 "lef.y"
    {(yyval.integer) = 1;;}
    break;

  case 62:
#line 764 "lef.y"
    {(yyval.integer) = 0;;}
    break;

  case 63:
#line 767 "lef.y"
    { 
      if (lefrUnitsCbk)
        CALLBACK(lefrUnitsCbk, lefrUnitsCbkType, &lefrUnits);
    ;}
    break;

  case 64:
#line 773 "lef.y"
    {
      lefrUnits.lefiUnits::clear();
      if (hasManufactur) {
        if (unitsWarnings++ < lefrUnitsWarnings) {
          lefError(1505, "MANUFACTURINGGRID statement was defined before UNITS.\nRefer the LEF Language Reference manual for the order of LEF statements.");
          CHKERR();
        }
      }
    ;}
    break;

  case 67:
#line 788 "lef.y"
    { if (lefrUnitsCbk) lefrUnits.lefiUnits::setTime((yyvsp[-1].dval)); ;}
    break;

  case 68:
#line 790 "lef.y"
    { if (lefrUnitsCbk) lefrUnits.lefiUnits::setCapacitance((yyvsp[-1].dval)); ;}
    break;

  case 69:
#line 792 "lef.y"
    { if (lefrUnitsCbk) lefrUnits.lefiUnits::setResistance((yyvsp[-1].dval)); ;}
    break;

  case 70:
#line 794 "lef.y"
    { if (lefrUnitsCbk) lefrUnits.lefiUnits::setPower((yyvsp[-1].dval)); ;}
    break;

  case 71:
#line 796 "lef.y"
    { if (lefrUnitsCbk) lefrUnits.lefiUnits::setCurrent((yyvsp[-1].dval)); ;}
    break;

  case 72:
#line 798 "lef.y"
    { if (lefrUnitsCbk) lefrUnits.lefiUnits::setVoltage((yyvsp[-1].dval)); ;}
    break;

  case 73:
#line 800 "lef.y"
    { 
      if(validNum((int)(yyvsp[-1].dval))) {
         if (lefrUnitsCbk)
            lefrUnits.lefiUnits::setDatabase("MICRONS", (yyvsp[-1].dval));
      }
    ;}
    break;

  case 74:
#line 807 "lef.y"
    { if (lefrUnitsCbk) lefrUnits.lefiUnits::setFrequency((yyvsp[-1].dval)); ;}
    break;

  case 75:
#line 811 "lef.y"
    { 
      if (lefrLayerCbk)
        CALLBACK(lefrLayerCbk, lefrLayerCbkType, &lefrLayer);
    ;}
    break;

  case 76:
#line 816 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 77:
#line 817 "lef.y"
    { 
      if (lefrHasMaxVS) {   /* 5.5 */
        if (lefrLayerCbk) { /* write error only if cbk is set */
          if (layerWarnings++ < lefrLayerWarnings) {
            lefError(1506, "A MAXVIASTACK statement is defined before the LAYER statement.\nRefer to the LEF Language Reference manual for the order of LEF statements.");
            CHKERR();
          }
        }
      }
      if (lefrLayerCbk)
        lefrLayer.lefiLayer::setName((yyvsp[0].string));
      useLenThr = 0;
      layerCut = 0;
      layerMastOver = 0;
      layerRout = 0;
      layerDir = 0;
      lefrHasLayer = 1;
      //strcpy(layerName, $3);
      layerName = strdup((yyvsp[0].string));
      hasType = 0;
      hasPitch = 0;
      hasWidth = 0;
      hasDirection = 0;
      hasParallel = 0;
      hasInfluence = 0;
      hasTwoWidths = 0;
      lefrHasSpacingTbl = 0;
      lefrHasSpacing = 0;
    ;}
    break;

  case 78:
#line 847 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 79:
#line 848 "lef.y"
    { 
      if (strcmp(layerName, (yyvsp[0].string)) != 0) {
        if (lefrLayerCbk) { /* write error only if cbk is set */
          if (layerWarnings++ < lefrLayerWarnings) {
             outMsg = (char*)lefMalloc(10000);
             sprintf (outMsg,
                "END LAYER name %s is different from the LAYER name %s.\nCorrect the LEF file before rerun it through the LEF parser.", (yyvsp[0].string), layerName);
             lefError(1507, outMsg);
             lefFree(outMsg);
             lefFree(layerName);
             CHKERR(); 
          } else
             lefFree(layerName);
        } else
          lefFree(layerName);
      } else
        lefFree(layerName);
      if (!lefrRelaxMode) {
        if (hasType == 0) {
          if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               lefError(1508, "TYPE statement is a required statement in a LAYER and it is not defined.");
               CHKERR(); 
            }
          }
        }
        if ((layerRout == 1) && (hasPitch == 0)) {
          if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1509, "PITCH statement is a required statement in a LAYER with type ROUTING and it is not defined.");
              CHKERR(); 
            }
          }
        }
        if ((layerRout == 1) && (hasWidth == 0)) {
          if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1510, "WIDTH statement is a required statement in a LAYER with type ROUTING and it is not defined.");
              CHKERR(); 
            }
          }
        }
        if ((layerRout == 1) && (hasDirection == 0)) {
          if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1511, "DIRECTION statement is a required statement in a LAYER with type ROUTING and it is not defined.");
              CHKERR(); 
            }
          }
        }
      }
    ;}
    break;

  case 80:
#line 903 "lef.y"
    { ;}
    break;

  case 81:
#line 908 "lef.y"
    { ;}
    break;

  case 82:
#line 912 "lef.y"
    {
       /* let setArraySpacingCutSpacing to set the data */
    ;}
    break;

  case 83:
#line 918 "lef.y"
    {
      if (lefrLayerCbk) {
         lefrLayer.lefiLayer::setArraySpacingCut((yyvsp[-4].dval));
         lefrLayer.lefiLayer::addArraySpacingArray((yyvsp[-2].dval), (yyvsp[0].dval));
      }
    ;}
    break;

  case 84:
#line 925 "lef.y"
    {
      if (versionNum < 5.7) {
         outMsg = (char*)lefMalloc(10000);
         sprintf(outMsg,
           "ARRAYSPACING is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
         lefError(1685, outMsg);
         lefFree(outMsg);
         CHKERR();
      }
    ;}
    break;

  case 85:
#line 936 "lef.y"
    {
      if (lefrLayerCbk)
         lefrLayer.lefiLayer::setType((yyvsp[-1].string));
      hasType = 1;
    ;}
    break;

  case 86:
#line 942 "lef.y"
    { 
      if (lefrLayerCbk) lefrLayer.lefiLayer::setPitch((yyvsp[-1].dval));
      hasPitch = 1;  
    ;}
    break;

  case 87:
#line 947 "lef.y"
    { 
      if (lefrLayerCbk) lefrLayer.lefiLayer::setPitchXY((yyvsp[-2].dval), (yyvsp[-1].dval));
      hasPitch = 1;  
    ;}
    break;

  case 88:
#line 952 "lef.y"
    { 
      if (lefrLayerCbk) lefrLayer.lefiLayer::setDiagPitch((yyvsp[-1].dval));
    ;}
    break;

  case 89:
#line 956 "lef.y"
    { 
      if (lefrLayerCbk) lefrLayer.lefiLayer::setDiagPitchXY((yyvsp[-2].dval), (yyvsp[-1].dval));
    ;}
    break;

  case 90:
#line 960 "lef.y"
    {
      if (lefrLayerCbk) lefrLayer.lefiLayer::setOffset((yyvsp[-1].dval));
    ;}
    break;

  case 91:
#line 964 "lef.y"
    {
      if (lefrLayerCbk) lefrLayer.lefiLayer::setOffsetXY((yyvsp[-2].dval), (yyvsp[-1].dval));
    ;}
    break;

  case 92:
#line 968 "lef.y"
    {
      if (lefrLayerCbk) lefrLayer.lefiLayer::setDiagWidth((yyvsp[-1].dval));
    ;}
    break;

  case 93:
#line 972 "lef.y"
    {
      if (lefrLayerCbk) lefrLayer.lefiLayer::setDiagSpacing((yyvsp[-1].dval));
    ;}
    break;

  case 94:
#line 976 "lef.y"
    {
      if (lefrLayerCbk) lefrLayer.lefiLayer::setWidth((yyvsp[-1].dval));
      hasWidth = 1;  
    ;}
    break;

  case 95:
#line 981 "lef.y"
    {
      if (lefrLayerCbk) lefrLayer.lefiLayer::setArea((yyvsp[-1].dval));
    ;}
    break;

  case 96:
#line 985 "lef.y"
    {
      hasSpCenter = 0;       /* reset to 0, only once per spacing is allowed */
      hasSpSamenet = 0;
      hasSpParallel = 0;
      hasSpLayer = 0;
      layerCutSpacing = (yyvsp[0].dval);  /* for error message purpose */
      // 11/22/99 - Wanda da Rosa, PCR 283762
      //            Issue an error is this is defined in masterslice
      if (layerMastOver) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1512, "It is illegal to define a SPACING statement in LAYER with TYPE MASTERSLICE or OVERLAP. Parser stops executions.");
              CHKERR();
            }
         }
      }
      // 5.5 either SPACING or SPACINGTABLE, not both for routing layer only
      if (layerRout) {
        if (lefrHasSpacingTbl) {
           if (lefrLayerCbk) { /* write error only if cbk is set */
              if (layerWarnings++ < lefrLayerWarnings) {
                lefWarning(2010, "It is illegal to have both SPACING rules & SPACINGTABLE rules within a ROUTING layer");
              }
           }
        }
        if (lefrLayerCbk)
           lefrLayer.lefiLayer::setSpacingMin((yyvsp[0].dval));
        lefrHasSpacing = 1;
      } else { 
        if (lefrLayerCbk)
           lefrLayer.lefiLayer::setSpacingMin((yyvsp[0].dval));
      }
    ;}
    break;

  case 98:
#line 1021 "lef.y"
    {
      if (lefrLayerCbk)
         lefrLayer.lefiLayer::setSpacingTableOrtho();
      if (lefrLayerCbk) /* due to converting to C, else, convertor produce */
         lefrLayer.lefiLayer::addSpacingTableOrthoWithin((yyvsp[-2].dval), (yyvsp[0].dval));/*bad code*/
    ;}
    break;

  case 99:
#line 1028 "lef.y"
    {
      if (versionNum < 5.7) {
         outMsg = (char*)lefMalloc(10000);
         sprintf(outMsg,
           "SPACINGTABLE ORTHOGONAL is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
         lefError(1694, outMsg);
         lefFree(outMsg);
         CHKERR();
      }
    ;}
    break;

  case 100:
#line 1039 "lef.y"
    {
      layerDir = 1;
      if (!layerRout) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1513, "DIRECTION statement can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setDirection((yyvsp[-1].string));
      hasDirection = 1;  
    ;}
    break;

  case 101:
#line 1053 "lef.y"
    {
      if (!layerRout) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1514, "RESISTANCE statement can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setResistance((yyvsp[-1].dval));
    ;}
    break;

  case 102:
#line 1065 "lef.y"
    {
      if (!layerRout) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1515, "RESISTANCE statement can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
              CHKERR();
            }
         }
      }
    ;}
    break;

  case 103:
#line 1076 "lef.y"
    {
      if (!layerRout) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1516, "CAPACITANCE statement can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setCapacitance((yyvsp[-1].dval));
    ;}
    break;

  case 104:
#line 1088 "lef.y"
    {
      if (!layerRout) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1517, "CAPACITANCE statement can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
              CHKERR();
            }
         }
      }
    ;}
    break;

  case 105:
#line 1099 "lef.y"
    {
      if (!layerRout) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1518, "HEIGHT statement can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setHeight((yyvsp[-1].dval));
    ;}
    break;

  case 106:
#line 1111 "lef.y"
    {
      if (!layerRout) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1519, "WIREEXTENSION statement can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setWireExtension((yyvsp[-1].dval));
    ;}
    break;

  case 107:
#line 1123 "lef.y"
    {
      if (!layerRout && (layerCut || layerMastOver)) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1520, "THICKNESS statement can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setThickness((yyvsp[-1].dval));
    ;}
    break;

  case 108:
#line 1135 "lef.y"
    {
      if (!layerRout) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1521, "SHRINKAGE statement can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setShrinkage((yyvsp[-1].dval));
    ;}
    break;

  case 109:
#line 1147 "lef.y"
    {
      if (!layerRout) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1522, "CAPMULTIPLIER statement can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setCapMultiplier((yyvsp[-1].dval));
    ;}
    break;

  case 110:
#line 1159 "lef.y"
    {
      if (!layerRout) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1523, "EDGECAPACITANCE statement can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setEdgeCap((yyvsp[-1].dval));
    ;}
    break;

  case 111:
#line 1184 "lef.y"
    { /* 5.3 syntax */
      use5_3 = 1;
      if (!layerRout && (layerCut || layerMastOver)) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1525, "ANTENNALENGTHFACTOR statement can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
              CHKERR();
            }
         }
      } else if (versionNum >= 5.4) {
         if (use5_4) {
            if (lefrLayerCbk) { /* write error only if cbk is set */
               if (layerWarnings++ < lefrLayerWarnings) {
                  outMsg = (char*)lefMalloc(10000);
                  sprintf (outMsg,
                    "ANTENNALENGTHFACTOR statement is a version 5.3 or earlier syntax.\nYour lef file with version %g, has both old and new ANTENNALENGTHFACTOR syntax, which is illegal.", versionNum);
                  lefError(1526, outMsg);
                  lefFree(outMsg);
                  CHKERR();
               }
            }
         }
      }

      if (lefrLayerCbk) lefrLayer.lefiLayer::setAntennaLength((yyvsp[-1].dval));
    ;}
    break;

  case 112:
#line 1211 "lef.y"
    {
      if (!layerRout) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1526, "CURRENTDEN statement can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setCurrentDensity((yyvsp[-1].dval));
    ;}
    break;

  case 113:
#line 1223 "lef.y"
    { 
      if (!layerRout) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1526, "CURRENTDEN statement can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
              CHKERR();
            }
         }
      }
    ;}
    break;

  case 114:
#line 1234 "lef.y"
    {
      if (!layerRout) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1526, "CURRENTDEN statement can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setCurrentPoint((yyvsp[-3].dval), (yyvsp[-2].dval));
    ;}
    break;

  case 115:
#line 1245 "lef.y"
    { lefDumbMode = 10000000; lefRealNum = 1; ;}
    break;

  case 116:
#line 1246 "lef.y"
    {
      lefDumbMode = 0;
      lefRealNum = 0;
    ;}
    break;

  case 117:
#line 1251 "lef.y"
    {
      if (layerMastOver) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1527, "ACCURRENTDENSITY statement can't be defined in LAYER with TYPE MASTERSLICE or OVERLAP. Parser stops executions.");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::addAccurrentDensity((yyvsp[0].string));
    ;}
    break;

  case 119:
#line 1264 "lef.y"
    {
      if (layerMastOver) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1527, "ACCURRENTDENSITY statement can't be defined in LAYER with TYPE MASTERSLICE or OVERLAP. Parser stops executions.");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) {
           lefrLayer.lefiLayer::addAccurrentDensity((yyvsp[-2].string));
           lefrLayer.lefiLayer::setAcOneEntry((yyvsp[-1].dval));
      }
    ;}
    break;

  case 120:
#line 1279 "lef.y"
    {
      if (layerMastOver) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1528, "DCCURRENTDENSITY statement can't be defined in LAYER with TYPE MASTERSLICE or OVERLAP. Parser stops executions.");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) {
         lefrLayer.lefiLayer::addDccurrentDensity("AVERAGE");
         lefrLayer.lefiLayer::setDcOneEntry((yyvsp[-1].dval));
      }
    ;}
    break;

  case 121:
#line 1294 "lef.y"
    {
      if (layerMastOver) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1528, "DCCURRENTDENSITY statement can't be defined in LAYER with TYPE MASTERSLICE or OVERLAP. Parser stops executions.");
              CHKERR();
            }
         }
      }
      if (!layerCut) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1529, "CUTAREA statement can only be defined in LAYER with type CUT. Parser stops executions.");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) {
         lefrLayer.lefiLayer::addDccurrentDensity("AVERAGE");
         lefrLayer.lefiLayer::addNumber((yyvsp[0].dval));
      }
    ;}
    break;

  case 122:
#line 1317 "lef.y"
    { if (lefrLayerCbk) lefrLayer.lefiLayer::addDcCutarea(); ;}
    break;

  case 124:
#line 1320 "lef.y"
    {
      if (layerMastOver) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1528, "DCCURRENTDENSITY can't be defined in LAYER with TYPE MASTERSLICE or OVERLAP. Parser stops executions.");
              CHKERR();
            }
         }
      }
      if (!layerRout) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1530, "WIDTH statement can only be defined in LAYER with type ROUTING. Parser stops executions.");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) {
         lefrLayer.lefiLayer::addDccurrentDensity("AVERAGE");
         lefrLayer.lefiLayer::addNumber((yyvsp[0].dval));
      }
    ;}
    break;

  case 125:
#line 1343 "lef.y"
    { if (lefrLayerCbk) lefrLayer.lefiLayer::addDcWidth(); ;}
    break;

  case 127:
#line 1348 "lef.y"
    { /* 5.4 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                 "ANTENNAAREARATIO statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1531, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      } else if (use5_3) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                 "ANTENNAAREARATIO statement is a version 5.4 or earlier syntax.\nYour lef file with version %g, has both old and new ANTENNAAREARATIO syntax, which is illegal.", versionNum);
               lefError(1532, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      }
      if (!layerRout && !layerCut && layerMastOver) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1533, "ANTENNAAREARATIO statement can only be defined in LAYER with TYPE ROUTING or CUT. Parser stops executions.");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setAntennaAreaRatio((yyvsp[-1].dval));
    ;}
    break;

  case 128:
#line 1386 "lef.y"
    { /* 5.4 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                  "ANTENNADIFFAREARATIO statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1532, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      } else if (use5_3) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                 "ANTENNADIFFAREARATIO statement is a version 5.4 or earlier syntax.\nYour lef file with version %g, has both old and new ANTENNADIFFAREARATIO syntax, which is illegal.", versionNum);
               lefError(1533, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      }
      if (!layerRout && !layerCut && layerMastOver) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               lefError(1534, "ANTENNADIFFAREARATIO statement can only be defined in LAYER with TYPE ROUTING or CUT. Parser stops executions.");
               CHKERR();
            }
         }
      }
      antennaType = lefiAntennaDAR; 
    ;}
    break;

  case 130:
#line 1425 "lef.y"
    { /* 5.4 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                  "ANTENNACUMAREARATIO statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1535, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      } else if (use5_3) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                 "ANTENNACUMAREARATIO statement is a version 5.4 or earlier syntax.\nYour lef file with version %g, has both old and new ANTENNACUMAREARATIO syntax, which is illegal.", versionNum);
               lefError(1536, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      }
      if (!layerRout && !layerCut && layerMastOver) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               lefError(1537, "ANTENNACUMAREARATIO statement can only be defined in LAYER with TYPE ROUTING or CUT. Parser stops executions.");
               CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setAntennaCumAreaRatio((yyvsp[-1].dval));
    ;}
    break;

  case 131:
#line 1463 "lef.y"
    {  /* 5.4 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                  "ANTENNACUMDIFFAREARATIO statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1538, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      } else if (use5_3) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                 "ANTENNACUMDIFFAREARATIO statement is a version 5.4 or earlier syntax.\nYour lef file with version %g, has both old and new ANTENNACUMDIFFAREARATIO syntax, which is illegal.", versionNum);
               lefError(1539, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      }
      if (!layerRout && !layerCut && layerMastOver) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1540, "ANTENNACUMDIFFAREARATIO statement can only be defined in LAYER with TYPE ROUTING or CUT. Parser stops executions.");
              CHKERR();
            }
         }
      }
      antennaType = lefiAntennaCDAR;
    ;}
    break;

  case 133:
#line 1502 "lef.y"
    { /* both 5.3  & 5.4 syntax */
      if (!layerRout && !layerCut && layerMastOver) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               lefError(1541, "ANTENNAAREAFACTOR can only be defined in LAYER with TYPE ROUTING or CUT. Parser stops executions.");
               CHKERR();
            }
         }
      }
      /* this does not need to check, since syntax is in both 5.3 & 5.4 */
      if (lefrLayerCbk) lefrLayer.lefiLayer::setAntennaAreaFactor((yyvsp[0].dval));
      antennaType = lefiAntennaAF;
    ;}
    break;

  case 135:
#line 1517 "lef.y"
    { /* 5.4 syntax */
      use5_4 = 1;
      if (!layerRout && (layerCut || layerMastOver)) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               lefError(1542, "ANTENNASIDEAREARATIO can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
               CHKERR();
            }
         }
      }
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                  "ANTENNASIDEAREARATIO statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1543, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      } else if (use5_3) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                 "ANTENNASIDEAREARATIO statement is a version 5.4 or earlier syntax.\nYour lef file with version %g, has both old and new ANTENNASIDEAREARATIO syntax, which is illegal.", versionNum);
               lefError(1544, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setAntennaSideAreaRatio((yyvsp[-1].dval));
    ;}
    break;

  case 136:
#line 1555 "lef.y"
    {  /* 5.4 syntax */
      use5_4 = 1;
      if (!layerRout && (layerCut || layerMastOver)) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               lefError(1545, "ANTENNADIFFSIDEAREARATIO can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
               CHKERR();
            }
         }
      }
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                  "ANTENNADIFFSIDEAREARATIO statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1546, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      } else if (use5_3) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                 "ANTENNADIFFSIDEAREARATIO statement is a version 5.4 or earlier syntax.\nYour lef file with version %g, has both old and new ANTENNADIFFSIDEAREARATIO syntax, which is illegal.", versionNum);
               lefError(1547, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      }
      antennaType = lefiAntennaDSAR;
    ;}
    break;

  case 138:
#line 1594 "lef.y"
    { /* 5.4 syntax */
      use5_4 = 1;
      if (!layerRout && (layerCut || layerMastOver)) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               lefError(1548, "ANTENNACUMSIDEAREARATIO can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
               CHKERR();
            }
         }
      }
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                  "ANTENNACUMSIDEAREARATIO statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1549, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      } else if (use5_3) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                 "ANTENNACUMSIDEAREARATIO statement is a version 5.4 or earlier syntax.\nYour lef file with version %g, has both old and new ANTENNACUMSIDEAREARATIO syntax, which is illegal.", versionNum);
               lefError(1550, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setAntennaCumSideAreaRatio((yyvsp[-1].dval));
    ;}
    break;

  case 139:
#line 1632 "lef.y"
    {  /* 5.4 syntax */
      use5_4 = 1;
      if (!layerRout && (layerCut || layerMastOver)) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               lefError(1551, "ANTENNACUMDIFFSIDEAREARATIO can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
               CHKERR();
            }
         }
      }
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                  "ANTENNACUMDIFFSIDEAREARATIO statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1552, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      } else if (use5_3) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                 "ANTENNACUMDIFFSIDEAREARATIO statement is a version 5.4 or earlier syntax.\nYour lef file with version %g, has both old and new ANTENNACUMDIFFSIDEAREARATIO syntax, which is illegal.", versionNum);
               lefError(1553, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      }
      antennaType = lefiAntennaCDSAR;
    ;}
    break;

  case 141:
#line 1671 "lef.y"
    { /* 5.4 syntax */
      use5_4 = 1;
      if (!layerRout && (layerCut || layerMastOver)) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               lefError(1554, "ANTENNASIDEAREAFACTOR can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
               CHKERR();
            }
         }
      }
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                  "ANTENNASIDEAREAFACTOR statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1555, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      } else if (use5_3) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                 "ANTENNASIDEAREAFACTOR statement is a version 5.4 or earlier syntax.\nYour lef file with version %g, has both old and new ANTENNASIDEAREAFACTOR syntax, which is illegal.", versionNum);
               lefError(1556, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setAntennaSideAreaFactor((yyvsp[0].dval));
      antennaType = lefiAntennaSAF;
    ;}
    break;

  case 143:
#line 1711 "lef.y"
    { /* 5.4 syntax */
      use5_4 = 1;
      if (!layerRout && !layerCut && layerMastOver) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               lefError(1557, "ANTENNAMODEL can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
               CHKERR();
            }
         }
      }
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.5) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                  "ANTENNAMODEL statement is a version 5.5 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1558, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      } else if (use5_3) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                 "ANTENNAMODEL statement is a version 5.4 or earlier syntax.\nYour lef file with version %g, has both old and new ANTENNAMODEL syntax, which is illegal.", versionNum);
               lefError(1559, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      }
      antennaType = lefiAntennaO;
    ;}
    break;

  case 145:
#line 1750 "lef.y"
    {
      if (versionNum < 5.7) {
         outMsg = (char*)lefMalloc(10000);
         sprintf(outMsg,
           "ANTENNACUMROUTINGPLUSCUT is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
         lefError(1686, outMsg);
         lefFree(outMsg);
         CHKERR();
      } else {
         if (!layerRout && !layerCut) {
            if (lefrLayerCbk) { /* write error only if cbk is set */
               if (layerWarnings++ < lefrLayerWarnings) {
                  lefError(1560, "ANTENNACUMROUTINGPLUSCUT can only be defined in LAYER with type ROUTING or CUT. Parser stops executions.");
                  CHKERR();
               }
            }
         }
         if (lefrLayerCbk) lefrLayer.lefiLayer::setAntennaCumRoutingPlusCut();
      }
    ;}
    break;

  case 146:
#line 1771 "lef.y"
    {
      if (versionNum < 5.7) {
         outMsg = (char*)lefMalloc(10000);
         sprintf(outMsg,
           "ANTENNAGATEPLUSDIFF is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
         lefError(1687, outMsg);
         lefFree(outMsg);
         CHKERR();
      } else {
         if (!layerRout && !layerCut) {
            if (lefrLayerCbk) { /* write error only if cbk is set */
               if (layerWarnings++ < lefrLayerWarnings) {
                  lefError(1561, "ANTENNAGATEPLUSDIFF can only be defined in LAYER with type ROUTING or CUT. Parser stops executions.");
                  CHKERR();
               }
            }
         }
         if (lefrLayerCbk) lefrLayer.lefiLayer::setAntennaGatePlusDiff((yyvsp[-1].dval));
      }
    ;}
    break;

  case 147:
#line 1792 "lef.y"
    {
      if (versionNum < 5.7) {
         outMsg = (char*)lefMalloc(10000);
         sprintf(outMsg,
           "ANTENNAAREAMINUSDIFF is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
         lefError(1688, outMsg);
         lefFree(outMsg);
         CHKERR();
      } else {
         if (!layerRout && !layerCut) {
            if (lefrLayerCbk) { /* write error only if cbk is set */
               if (layerWarnings++ < lefrLayerWarnings) {
                  lefError(1562, "ANTENNAAREAMINUSDIFF can only be defined in LAYER with type ROUTING or CUT. Parser stops executions.");
                  CHKERR();
               }
            }
         }
         if (lefrLayerCbk) lefrLayer.lefiLayer::setAntennaAreaMinusDiff((yyvsp[-1].dval));
      }
    ;}
    break;

  case 148:
#line 1813 "lef.y"
    {
      if (!layerRout && !layerCut) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               lefError(1563, "ANTENNAAREADIFFREDUCEPWL can only be defined in LAYER with type ROUTING or CUT. Parser stops executions.");
               CHKERR();
            }
         }
      }
      if (lefrLayerCbk) { /* require min 2 points, set the 1st 2 */
         lefrAntennaPWLPtr=(lefiAntennaPWL*)lefMalloc(sizeof(lefiAntennaPWL));
         lefrAntennaPWLPtr->lefiAntennaPWL::Init();
         lefrAntennaPWLPtr->lefiAntennaPWL::addAntennaPWL((yyvsp[-1].pt).x, (yyvsp[-1].pt).y);
         lefrAntennaPWLPtr->lefiAntennaPWL::addAntennaPWL((yyvsp[0].pt).x, (yyvsp[0].pt).y);
      }
    ;}
    break;

  case 149:
#line 1830 "lef.y"
    {
      if (lefrLayerCbk)
        lefrLayer.lefiLayer::setAntennaPWL(lefiAntennaADR, lefrAntennaPWLPtr);
    ;}
    break;

  case 150:
#line 1834 "lef.y"
    {
      if (versionNum < 5.7) {
        outMsg = (char*)lefMalloc(10000);
        sprintf(outMsg,
          "ANTENNAAREADIFFREDUCEPWL is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
        lefError(1689, outMsg);
        lefFree(outMsg);
        CHKERR();
      }
    ;}
    break;

  case 151:
#line 1845 "lef.y"
    { /* 5.4 syntax */
      if (ignoreVersion) {
         /* do nothing */
         if (lefrLayerCbk) lefrLayer.lefiLayer::setSlotWireWidth((yyvsp[-1].dval));
      } else if (versionNum >= 5.7) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            lefWarning(2011, "SLOTWIREWIDTH is obsoleted in version 5.7 or later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.7 or later.");
         }
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                  "SLOTWIREWIDTH statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1564, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      } else
         if (lefrLayerCbk) lefrLayer.lefiLayer::setSlotWireWidth((yyvsp[-1].dval));
    ;}
    break;

  case 152:
#line 1868 "lef.y"
    { /* 5.4 syntax */
      if (ignoreVersion) {
         /* do nothing */
         if (lefrLayerCbk) lefrLayer.lefiLayer::setSlotWireLength((yyvsp[-1].dval));
      } else if (versionNum >= 5.7) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            lefWarning(2012, "SLOTWIRELENGTH is obsoleted in version 5.7 or later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.7 or later.");
         }
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                  "SLOTWIRELENGTH statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1565, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      } else
         if (lefrLayerCbk) lefrLayer.lefiLayer::setSlotWireLength((yyvsp[-1].dval));
    ;}
    break;

  case 153:
#line 1891 "lef.y"
    { /* 5.4 syntax */
      if (ignoreVersion) {
         /* do nothing */
         if (lefrLayerCbk) lefrLayer.lefiLayer::setSlotWidth((yyvsp[-1].dval));
      } else if (versionNum >= 5.7) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            lefWarning(2013, "SLOTWIDTH is obsoleted in version 5.7 or later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.7 or later.");
         }
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                  "SLOTWIDTH statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1566, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      } else
         if (lefrLayerCbk) lefrLayer.lefiLayer::setSlotWidth((yyvsp[-1].dval));
    ;}
    break;

  case 154:
#line 1914 "lef.y"
    { /* 5.4 syntax */
      if (ignoreVersion) {
         /* do nothing */
         if (lefrLayerCbk) lefrLayer.lefiLayer::setSlotLength((yyvsp[-1].dval));
      } else if (versionNum >= 5.7) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            lefWarning(2014, "SLOTLENGTH is obsoleted in version 5.7 or later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.7 or later.");
         }
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                  "SLOTLENGTH statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1567, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      } else
         if (lefrLayerCbk) lefrLayer.lefiLayer::setSlotLength((yyvsp[-1].dval));
    ;}
    break;

  case 155:
#line 1937 "lef.y"
    { /* 5.4 syntax */
      if (ignoreVersion) {
         /* do nothing */
         if (lefrLayerCbk) lefrLayer.lefiLayer::setMaxAdjacentSlotSpacing((yyvsp[-1].dval));
      } else if (versionNum >= 5.7) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            lefWarning(2015, "MAXADJACENTSLOTSPACING is obsoleted in version 5.7 or later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.7 or later.");
         }
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                  "MAXADJACENTSLOTSPACING statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1568, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      } else
         if (lefrLayerCbk) lefrLayer.lefiLayer::setMaxAdjacentSlotSpacing((yyvsp[-1].dval));
    ;}
    break;

  case 156:
#line 1960 "lef.y"
    { /* 5.4 syntax */
      if (ignoreVersion) {
         /* do nothing */
         if (lefrLayerCbk) lefrLayer.lefiLayer::setMaxCoaxialSlotSpacing((yyvsp[-1].dval));
      } else if (versionNum >= 5.7) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            lefWarning(2016, "MAXCOAXIALSLOTSPACING is obsoleted in version 5.7 or later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.7 or later.");
         }
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                  "MAXCOAXIALSLOTSPACING statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1569, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      } else
         if (lefrLayerCbk) lefrLayer.lefiLayer::setMaxCoaxialSlotSpacing((yyvsp[-1].dval));
    ;}
    break;

  case 157:
#line 1983 "lef.y"
    { /* 5.4 syntax */
      if (ignoreVersion) {
         /* do nothing */
         if (lefrLayerCbk) lefrLayer.lefiLayer::setMaxEdgeSlotSpacing((yyvsp[-1].dval));
      } else if (versionNum >= 5.7) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            lefWarning(2017, "MAXEDGESLOTSPACING is obsoleted in version 5.7 or later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.7 or later.");
         }
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                  "MAXEDGESLOTSPACING statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1570, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      } else
         if (lefrLayerCbk) lefrLayer.lefiLayer::setMaxEdgeSlotSpacing((yyvsp[-1].dval));
    ;}
    break;

  case 158:
#line 2006 "lef.y"
    { /* 5.4 syntax */
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum >= 5.7) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            lefWarning(2018, "SPLITWIREWIDTH is obsoleted in version 5.7 or later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.7 or later.");
         }
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                  "SPLITWIREWIDTH statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1571, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setSplitWireWidth((yyvsp[-1].dval));
    ;}
    break;

  case 159:
#line 2028 "lef.y"
    { /* 5.4 syntax, pcr 394389 */
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                  "MINIMUMDENSITY statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1572, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setMinimumDensity((yyvsp[-1].dval));
    ;}
    break;

  case 160:
#line 2046 "lef.y"
    { /* 5.4 syntax, pcr 394389 */
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                  "MAXIMUMDENSITY statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1573, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setMaximumDensity((yyvsp[-1].dval));
    ;}
    break;

  case 161:
#line 2064 "lef.y"
    { /* 5.4 syntax, pcr 394389 */
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                  "DENSITYCHECKWINDOW statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1574, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setDensityCheckWindow((yyvsp[-2].dval), (yyvsp[-1].dval));
    ;}
    break;

  case 162:
#line 2082 "lef.y"
    { /* 5.4 syntax, pcr 394389 */
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                  "DENSITYCHECKSTEP statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1575, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setDensityCheckStep((yyvsp[-1].dval));
    ;}
    break;

  case 163:
#line 2100 "lef.y"
    { /* 5.4 syntax, pcr 394389 */
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                  "FILLACTIVESPACING statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1576, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setFillActiveSpacing((yyvsp[-1].dval));
    ;}
    break;

  case 164:
#line 2118 "lef.y"
    {
      // 5.5 MAXWIDTH, is for routing layer only
      if (!layerRout) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               lefError(1577, "MAXWIDTH statement can only be defined in LAYER with TYPE ROUTING.  Parser stops executions.");
               CHKERR();
            }
         }
      }
      if (versionNum < 5.5) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                  "MAXWIDTH statement is a version 5.5 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1578, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setMaxwidth((yyvsp[-1].dval));
    ;}
    break;

  case 165:
#line 2143 "lef.y"
    {
      // 5.5 MINWIDTH, is for routing layer only
      if (!layerRout) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1579, "MINWIDTH statement can only be defined in LAYER with TYPE ROUTING.  Parser stops executions.");
              CHKERR();
            }
         }
      }
      if (versionNum < 5.5) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                  "MINWIDTH statement is a version 5.5 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1580, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setMinwidth((yyvsp[-1].dval));
    ;}
    break;

  case 166:
#line 2168 "lef.y"
    {
      if (versionNum < 5.5) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                  "MINENCLOSEDAREA statement is a version 5.5 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1581, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::addMinenclosedarea((yyvsp[0].dval));
    ;}
    break;

  case 168:
#line 2185 "lef.y"
    { /* pcr 409334 */
      if (lefrLayerCbk)
         lefrLayer.lefiLayer::addMinimumcut((int)(yyvsp[-2].dval), (yyvsp[0].dval)); 
      hasLayerMincut = 0;
    ;}
    break;

  case 169:
#line 2192 "lef.y"
    {
      if (!hasLayerMincut) {   /* FROMABOVE nor FROMBELOW is set */
         if (lefrLayerCbk)
             lefrLayer.lefiLayer::addMinimumcutConnect((char*)"");
      }
    ;}
    break;

  case 170:
#line 2199 "lef.y"
    {
      if (lefrLayerCbk) lefrLayer.lefiLayer::addMinstep((yyvsp[0].dval));
    ;}
    break;

  case 171:
#line 2203 "lef.y"
    {
    ;}
    break;

  case 172:
#line 2206 "lef.y"
    {
      if (versionNum < 5.5) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                  "PROTRUSION RULE statement is a version 5.5 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1582, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setProtrusion((yyvsp[-5].dval), (yyvsp[-3].dval), (yyvsp[-1].dval));
    ;}
    break;

  case 173:
#line 2222 "lef.y"
    {
      if (versionNum < 5.5) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                  "SPACINGTABLE statement is a version 5.5 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1583, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      }
      // 5.5 either SPACING or SPACINGTABLE in a layer, not both
      if (lefrHasSpacing && layerRout) {
         if (lefrLayerCbk)  /* write warning only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefWarning(2010, "It is illegal to have both SPACING rules & SPACINGTABLE rules within a ROUTING layer");
            }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::addSpacingTable();
      lefrHasSpacingTbl = 1;
    ;}
    break;

  case 175:
#line 2248 "lef.y"
    {
      if (versionNum < 5.6) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                  "ENCLOSURE statement is a version 5.6 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1584, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      } else {
         if (lefrLayerCbk)
            lefrLayer.lefiLayer::addEnclosure((yyvsp[-2].string), (yyvsp[-1].dval), (yyvsp[0].dval));
      }
    ;}
    break;

  case 177:
#line 2268 "lef.y"
    {
      if (versionNum < 5.6) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                  "PREFERENCLOSURE statement is a version 5.6 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1585, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      } else {
         if (lefrLayerCbk)
            lefrLayer.lefiLayer::addPreferEnclosure((yyvsp[-2].string), (yyvsp[-1].dval), (yyvsp[0].dval));
      }
    ;}
    break;

  case 179:
#line 2287 "lef.y"
    {
      if (versionNum < 5.6) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                  "RESISTANCE statement is a version 5.6 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1586, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      } else {
         if (lefrLayerCbk)
            lefrLayer.lefiLayer::setResPerCut((yyvsp[-1].dval));
      }
    ;}
    break;

  case 180:
#line 2305 "lef.y"
    {
      if (!layerRout) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1587, "DIAGMINEDGELENGTH can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
              CHKERR();
            }
         }
      } else if (versionNum < 5.6) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                  "DIAGMINEDGELENGTH statement is a version 5.6 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1588, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      } else {
         if (lefrLayerCbk)
            lefrLayer.lefiLayer::setDiagMinEdgeLength((yyvsp[-1].dval));
      }
    ;}
    break;

  case 181:
#line 2330 "lef.y"
    {
      // Use the polygon code to retrieve the points for MINSIZE
      lefrGeometriesPtr = (lefiGeometries*)lefMalloc(sizeof(lefiGeometries));
      lefrGeometriesPtr->lefiGeometries::Init();
      lefrDoGeometries = 1;
    ;}
    break;

  case 182:
#line 2337 "lef.y"
    {
      if (lefrLayerCbk) {
         lefrGeometriesPtr->lefiGeometries::addPolygon();
         lefrLayer.lefiLayer::setMinSize(lefrGeometriesPtr);
      }
     lefrDoGeometries = 0;
      lefrGeometriesPtr->lefiGeometries::Destroy();
                                       // Don't need the object anymore
      lefFree(lefrGeometriesPtr);
    ;}
    break;

  case 184:
#line 2351 "lef.y"
    {
        if (lefrLayerCbk)
           lefrLayer.lefiLayer::setArraySpacingLongArray();
    ;}
    break;

  case 186:
#line 2359 "lef.y"
    {
      if (lefrLayerCbk)
         lefrLayer.lefiLayer::setArraySpacingWidth((yyvsp[0].dval));
    ;}
    break;

  case 189:
#line 2370 "lef.y"
    {
      if (lefrLayerCbk)
         lefrLayer.lefiLayer::addArraySpacingArray((yyvsp[-2].dval), (yyvsp[0].dval));
    ;}
    break;

  case 190:
#line 2377 "lef.y"
    { 
      if (hasInfluence) {  // 5.5 - INFLUENCE table must follow a PARALLEL
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1589, "An INFLUENCE table statement was defined before the PARALLELRUNLENGTH table statement.\nINFLUENCE table statement should be defined following the PARALLELRUNLENGTH.\nChange the LEF file and rerun the parser.");
              CHKERR();
            }
         }
      }
      if (hasParallel) { // 5.5 - Only one PARALLEL table is allowed per layer
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1590, "There is multiple PARALLELRUNLENGTH table statements are defined within a layer.\nAccording to the LEF Reference Manual, only one PARALLELRUNLENGTH table statement is allowed per layer.");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::addNumber((yyvsp[0].dval));
      hasParallel = 1;
    ;}
    break;

  case 191:
#line 2398 "lef.y"
    {
      spParallelLength = lefrLayer.lefiLayer::getNumber();
      if (lefrLayerCbk) lefrLayer.lefiLayer::addSpParallelLength();
    ;}
    break;

  case 192:
#line 2403 "lef.y"
    { 
      if (lefrLayerCbk) {
         lefrLayer.lefiLayer::addSpParallelWidth((yyvsp[0].dval));
      }
    ;}
    break;

  case 193:
#line 2409 "lef.y"
    { 
      if (lefrLayer.lefiLayer::getNumber() != spParallelLength) {
         if (lefrLayerCbk) {
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1591, "The number of length in PARALLELRUNLENGTH is not the same as the number of spacing in WIDTH.");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::addSpParallelWidthSpacing();
    ;}
    break;

  case 195:
#line 2423 "lef.y"
    {
      if (lefrLayerCbk) lefrLayer.lefiLayer::addNumber((yyvsp[0].dval));
    ;}
    break;

  case 196:
#line 2427 "lef.y"
    {
      if (hasParallel) { // 5.7 - Either PARALLEL OR TWOWIDTHS per layer
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1592, "A PARALLELRUNLENGTH statement has already defined in the layer.\nOnly either PARALLELRUNLENGTH or TWOWIDTHS is allowed per layer.");
              CHKERR();
            }
         }
      }
      if (hasTwoWidths) { // 5.7 - only 1 TWOWIDTHS per layer
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1593, "A TWOWIDTHS table statement has already defined in the layer.\nOnly one TWOWIDTHS statement is allowed per layer.");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::addSpTwoWidths((yyvsp[-4].dval), (yyvsp[-3].dval));
      hasTwoWidths = 1;
    ;}
    break;

  case 197:
#line 2448 "lef.y"
    {
      if (versionNum < 5.7) {
        outMsg = (char*)lefMalloc(10000);
        sprintf(outMsg,
          "TWOWIDTHS is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
        lefError(1697, outMsg);
        lefFree(outMsg);
        CHKERR();
      } 
    ;}
    break;

  case 198:
#line 2459 "lef.y"
    {
      if (hasInfluence) {  // 5.5 - INFLUENCE table must follow a PARALLEL
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1594, "A INFLUENCE table statement has already defined in the layer.\nOnly one INFLUENCE statement is allowed per layer.");
              CHKERR();
            }
         }
      }
      if (!hasParallel) {  // 5.5 - INFLUENCE must follow a PARALLEL
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1595, "An INFLUENCE table statement has already defined beofre the layer.\nINFLUENCE statement has to be defined after the PARALLELRUNLENGTH table statement in the layer.");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) {
         lefrLayer.lefiLayer::setInfluence();
         lefrLayer.lefiLayer::addSpInfluence((yyvsp[-4].dval), (yyvsp[-2].dval), (yyvsp[0].dval));
      }
    ;}
    break;

  case 202:
#line 2489 "lef.y"
    {
    if (lefrLayerCbk)
       lefrLayer.lefiLayer::addSpacingTableOrthoWithin((yyvsp[-2].dval), (yyvsp[0].dval));
  ;}
    break;

  case 203:
#line 2495 "lef.y"
    {(yyval.string) = (char*)"NULL";;}
    break;

  case 204:
#line 2496 "lef.y"
    {(yyval.string) = (char*)"ABOVE";;}
    break;

  case 205:
#line 2497 "lef.y"
    {(yyval.string) = (char*)"BELOW";;}
    break;

  case 207:
#line 2501 "lef.y"
    {
      if (lefrLayerCbk) {
         lefrLayer.lefiLayer::addEnclosureWidth((yyvsp[0].dval));
      }
    ;}
    break;

  case 209:
#line 2508 "lef.y"
    {
      if (versionNum < 5.7) {
         outMsg = (char*)lefMalloc(10000);
         sprintf(outMsg,
           "LENGTH is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
         lefError(1691, outMsg);
         lefFree(outMsg);
         CHKERR();
      } else {
         if (lefrLayerCbk) {
            lefrLayer.lefiLayer::addEnclosureLength((yyvsp[0].dval));
         }
      }
    ;}
    break;

  case 211:
#line 2525 "lef.y"
    {
      if (versionNum < 5.7) {
         outMsg = (char*)lefMalloc(10000);
         sprintf(outMsg,
           "EXCEPTEXTRACUT is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
         lefError(1690, outMsg);
         lefFree(outMsg);
         CHKERR();
      } else {
         if (lefrLayerCbk) {
            lefrLayer.lefiLayer::addEnclosureExceptEC((yyvsp[0].dval));
         }
      }
    ;}
    break;

  case 213:
#line 2542 "lef.y"
    {
      if (lefrLayerCbk) {
         lefrLayer.lefiLayer::addPreferEnclosureWidth((yyvsp[0].dval));
      }
    ;}
    break;

  case 215:
#line 2550 "lef.y"
    {
      if (versionNum < 5.7) {
        outMsg = (char*)lefMalloc(10000);
        sprintf(outMsg,
          "MINIMUMCUT WITHIN is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
        lefError(1700, outMsg);
        lefFree(outMsg);
        CHKERR();
      } else {
        if (lefrLayerCbk) {
           lefrLayer.lefiLayer::addMinimumcutWithin((yyvsp[0].dval));
        }
      }
    ;}
    break;

  case 218:
#line 2573 "lef.y"
    {
      if (versionNum < 5.5) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                 "FROMABOVE statement is a version 5.5 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1596, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      }
      hasLayerMincut = 1;
      if (lefrLayerCbk)
         lefrLayer.lefiLayer::addMinimumcutConnect((char*)"FROMABOVE");

    ;}
    break;

  case 219:
#line 2592 "lef.y"
    {
      if (versionNum < 5.5) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                 "FROMBELOW statement is a version 5.5 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1597, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      }
      hasLayerMincut = 1;
      if (lefrLayerCbk)
         lefrLayer.lefiLayer::addMinimumcutConnect((char*)"FROMBELOW");
    ;}
    break;

  case 220:
#line 2610 "lef.y"
    {   
      if (versionNum < 5.5) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                 "LENGTH WITHIN statement is a version 5.5 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1598, outMsg);
               lefFree(outMsg);
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk)
         lefrLayer.lefiLayer::addMinimumcutLengDis((yyvsp[-2].dval), (yyvsp[0].dval));
    ;}
    break;

  case 223:
#line 2635 "lef.y"
    {
    if (lefrLayerCbk) lefrLayer.lefiLayer::addMinstepType((yyvsp[0].string));
  ;}
    break;

  case 224:
#line 2639 "lef.y"
    {
    if (lefrLayerCbk) lefrLayer.lefiLayer::addMinstepLengthsum((yyvsp[0].dval));
  ;}
    break;

  case 225:
#line 2643 "lef.y"
    {
    if (versionNum < 5.7) {
      outMsg = (char*)lefMalloc(10000);
      sprintf(outMsg,
        "MAXEDGES is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
      lefError(1685, outMsg);
      lefFree(outMsg);
      CHKERR();
    } else {
      if (lefrLayerCbk) lefrLayer.lefiLayer::addMinstepMaxedges((yyvsp[0].dval));
    }
  ;}
    break;

  case 226:
#line 2657 "lef.y"
    {(yyval.string) = (char*)"INSIDECORNER";;}
    break;

  case 227:
#line 2658 "lef.y"
    {(yyval.string) = (char*)"OUTSIDECORNER";;}
    break;

  case 228:
#line 2659 "lef.y"
    {(yyval.string) = (char*)"STEP";;}
    break;

  case 229:
#line 2663 "lef.y"
    { if (lefrLayerCbk)
          lefrLayer.lefiLayer::setAntennaValue(antennaType, (yyvsp[0].dval)); ;}
    break;

  case 230:
#line 2666 "lef.y"
    { if (lefrLayerCbk) { /* require min 2 points, set the 1st 2 */
          lefrAntennaPWLPtr = (lefiAntennaPWL*)lefMalloc(sizeof(lefiAntennaPWL));
          lefrAntennaPWLPtr->lefiAntennaPWL::Init();
          lefrAntennaPWLPtr->lefiAntennaPWL::addAntennaPWL((yyvsp[-1].pt).x, (yyvsp[-1].pt).y);
          lefrAntennaPWLPtr->lefiAntennaPWL::addAntennaPWL((yyvsp[0].pt).x, (yyvsp[0].pt).y);
        }
      ;}
    break;

  case 231:
#line 2674 "lef.y"
    { if (lefrLayerCbk)
          lefrLayer.lefiLayer::setAntennaPWL(antennaType, lefrAntennaPWLPtr);
      ;}
    break;

  case 234:
#line 2687 "lef.y"
    { if (lefrLayerCbk)
      lefrAntennaPWLPtr->lefiAntennaPWL::addAntennaPWL((yyvsp[0].pt).x, (yyvsp[0].pt).y);
  ;}
    break;

  case 236:
#line 2693 "lef.y"
    { 
        use5_4 = 1;
        if (ignoreVersion) {
           /* do nothing */
        }
        else if ((antennaType == lefiAntennaAF) && (versionNum <= 5.3)) {
           if (lefrLayerCbk) { /* write error only if cbk is set */
              if (layerWarnings++ < lefrLayerWarnings) {
                 outMsg = (char*)lefMalloc(10000);
                 sprintf (outMsg,
                   "ANTENNAAREAFACTOR with DIFFUSEONLY statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
                 lefError(1599, outMsg);
                 lefFree(outMsg);
                 CHKERR();
              }
           }
        } else if (use5_3) {
           if (lefrLayerCbk) { /* write error only if cbk is set */
              if (layerWarnings++ < lefrLayerWarnings) {
                 outMsg = (char*)lefMalloc(10000);
                 sprintf (outMsg,
                   "ANTENNAAREAFACTOR with DIFFUSEONLY statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
                 lefError(1599, outMsg);
                 lefFree(outMsg);
                 CHKERR();
              }
           }
        }
        if (lefrLayerCbk)
          lefrLayer.lefiLayer::setAntennaDUO(antennaType);
      ;}
    break;

  case 237:
#line 2726 "lef.y"
    {(yyval.string) = (char*)"PEAK";;}
    break;

  case 238:
#line 2727 "lef.y"
    {(yyval.string) = (char*)"AVERAGE";;}
    break;

  case 239:
#line 2728 "lef.y"
    {(yyval.string) = (char*)"RMS";;}
    break;

  case 240:
#line 2732 "lef.y"
    { if (lefrLayerCbk) lefrLayer.lefiLayer::addNumber((yyvsp[0].dval)); ;}
    break;

  case 241:
#line 2734 "lef.y"
    { if (lefrLayerCbk) lefrLayer.lefiLayer::addAcFrequency(); ;}
    break;

  case 242:
#line 2737 "lef.y"
    { if (lefrLayerCbk) lefrLayer.lefiLayer::addNumber((yyvsp[0].dval)); ;}
    break;

  case 243:
#line 2739 "lef.y"
    { if (lefrLayerCbk) lefrLayer.lefiLayer::addAcTableEntry(); ;}
    break;

  case 245:
#line 2743 "lef.y"
    {
      if (!layerCut) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1600, "CUTAREA statement can only be defined in LAYER with type CUT.");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::addNumber((yyvsp[0].dval));
    ;}
    break;

  case 246:
#line 2755 "lef.y"
    { if (lefrLayerCbk) lefrLayer.lefiLayer::addAcCutarea(); ;}
    break;

  case 247:
#line 2757 "lef.y"
    {
      if (!layerRout) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1601, "WIDTH can only be defined in LAYER with type ROUTING.");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::addNumber((yyvsp[0].dval));
    ;}
    break;

  case 248:
#line 2769 "lef.y"
    { if (lefrLayerCbk) lefrLayer.lefiLayer::addAcWidth(); ;}
    break;

  case 249:
#line 2773 "lef.y"
    { if (lefrLayerCbk) lefrLayer.lefiLayer::addNumber((yyvsp[0].dval)); ;}
    break;

  case 250:
#line 2775 "lef.y"
    { if (lefrLayerCbk) lefrLayer.lefiLayer::addDcTableEntry(); ;}
    break;

  case 252:
#line 2779 "lef.y"
    { if (lefrLayerCbk) lefrLayer.lefiLayer::addNumber((yyvsp[0].dval)); ;}
    break;

  case 255:
#line 2788 "lef.y"
    {
      if (lefrLayerCbk) {
        char propTp;
        propTp = lefrLayerProp.lefiPropType::propType((yyvsp[-1].string));
        lefrLayer.lefiLayer::addProp((yyvsp[-1].string), (yyvsp[0].string), propTp);
      }
    ;}
    break;

  case 256:
#line 2796 "lef.y"
    {
      if (lefrLayerCbk) {
        char propTp;
        propTp = lefrLayerProp.lefiPropType::propType((yyvsp[-1].string));
        lefrLayer.lefiLayer::addProp((yyvsp[-1].string), (yyvsp[0].string), propTp);
      }
    ;}
    break;

  case 257:
#line 2804 "lef.y"
    {
      char temp[32];
      sprintf(temp, "%.11g", (yyvsp[0].dval));
      if (lefrLayerCbk) {
        char propTp;
        propTp = lefrLayerProp.lefiPropType::propType((yyvsp[-1].string));
        lefrLayer.lefiLayer::addNumProp((yyvsp[-1].string), (yyvsp[0].dval), temp, propTp);
      }
    ;}
    break;

  case 258:
#line 2816 "lef.y"
    { ;}
    break;

  case 259:
#line 2818 "lef.y"
    { ;}
    break;

  case 260:
#line 2821 "lef.y"
    { if (lefrLayerCbk) lefrLayer.lefiLayer::setCurrentPoint((yyvsp[-2].dval), (yyvsp[-1].dval)); ;}
    break;

  case 263:
#line 2829 "lef.y"
    { if (lefrLayerCbk) lefrLayer.lefiLayer::setCapacitancePoint((yyvsp[-2].dval), (yyvsp[-1].dval)); ;}
    break;

  case 265:
#line 2834 "lef.y"
    { ;}
    break;

  case 266:
#line 2837 "lef.y"
    { if (lefrLayerCbk) lefrLayer.lefiLayer::setResistancePoint((yyvsp[-2].dval), (yyvsp[-1].dval)); ;}
    break;

  case 267:
#line 2840 "lef.y"
    {(yyval.string) = (char*)"ROUTING"; layerRout = 1;;}
    break;

  case 268:
#line 2841 "lef.y"
    {(yyval.string) = (char*)"CUT"; layerCut = 1;;}
    break;

  case 269:
#line 2842 "lef.y"
    {(yyval.string) = (char*)"OVERLAP"; layerMastOver = 1;;}
    break;

  case 270:
#line 2843 "lef.y"
    {(yyval.string) = (char*)"MASTERSLICE"; layerMastOver = 1;;}
    break;

  case 271:
#line 2844 "lef.y"
    {(yyval.string) = (char*)"VIRTUAL";;}
    break;

  case 272:
#line 2845 "lef.y"
    {(yyval.string) = (char*)"IMPLANT";;}
    break;

  case 273:
#line 2848 "lef.y"
    {(yyval.string) = (char*)"HORIZONTAL";;}
    break;

  case 274:
#line 2849 "lef.y"
    {(yyval.string) = (char*)"VERTICAL";;}
    break;

  case 275:
#line 2850 "lef.y"
    {(yyval.string) = (char*)"DIAG45";;}
    break;

  case 276:
#line 2851 "lef.y"
    {(yyval.string) = (char*)"DIAG135";;}
    break;

  case 278:
#line 2855 "lef.y"
    {
    if (lefrLayerCbk)
       lefrLayer.lefiLayer::addMinenclosedareaWidth((yyvsp[0].dval));
    ;}
    break;

  case 279:
#line 2862 "lef.y"
    {
    if (lefrLayerCbk)
       lefrLayer.lefiLayer::addAntennaModel(1);
    ;}
    break;

  case 280:
#line 2867 "lef.y"
    {
    if (lefrLayerCbk)
       lefrLayer.lefiLayer::addAntennaModel(2);
    ;}
    break;

  case 281:
#line 2872 "lef.y"
    {
    if (lefrLayerCbk)
       lefrLayer.lefiLayer::addAntennaModel(3);
    ;}
    break;

  case 282:
#line 2877 "lef.y"
    {
    if (lefrLayerCbk)
       lefrLayer.lefiLayer::addAntennaModel(4);
    ;}
    break;

  case 283:
#line 2883 "lef.y"
    { ;}
    break;

  case 284:
#line 2888 "lef.y"
    { ;}
    break;

  case 285:
#line 2891 "lef.y"
    { 
      if (lefrLayerCbk) {
         lefrLayer.lefiLayer::addSpParallelWidth((yyvsp[0].dval));
      }
    ;}
    break;

  case 286:
#line 2897 "lef.y"
    { if (lefrLayerCbk) lefrLayer.lefiLayer::addSpParallelWidthSpacing(); ;}
    break;

  case 287:
#line 2900 "lef.y"
    { ;}
    break;

  case 288:
#line 2902 "lef.y"
    { ;}
    break;

  case 289:
#line 2916 "lef.y"
    {
       if (lefrLayerCbk) lefrLayer.lefiLayer::addNumber((yyvsp[0].dval));
    ;}
    break;

  case 290:
#line 2920 "lef.y"
    {
      if (lefrLayerCbk)
         lefrLayer.lefiLayer::addSpTwoWidths((yyvsp[-4].dval), (yyvsp[-3].dval));
    ;}
    break;

  case 291:
#line 2926 "lef.y"
    { (yyval.dval) = -1; ;}
    break;

  case 292:
#line 2928 "lef.y"
    { (yyval.dval) = (yyvsp[0].dval); ;}
    break;

  case 293:
#line 2931 "lef.y"
    { ;}
    break;

  case 294:
#line 2933 "lef.y"
    { ;}
    break;

  case 295:
#line 2936 "lef.y"
    { if (lefrLayerCbk) lefrLayer.lefiLayer::addSpInfluence((yyvsp[-4].dval), (yyvsp[-2].dval), (yyvsp[0].dval)); ;}
    break;

  case 296:
#line 2939 "lef.y"
    {
      if (!lefrHasLayer) {  /* 5.5 */
        if (lefrMaxStackViaCbk) { /* write error only if cbk is set */
           if (maxStackViaWarnings++ < lefrMaxStackViaWarnings) {
             lefError(1602, "MAXVIASTACK statement has to be defined after the LAYER statement.");
             CHKERR();
           }
        }
      } else if (lefrHasMaxVS) {
        if (lefrMaxStackViaCbk) { /* write error only if cbk is set */
           if (maxStackViaWarnings++ < lefrMaxStackViaWarnings) {
             lefError(1603, "A MAXVIASTACK has already defined.\nOnly one MAXVIASTACK is allowed per lef file.");
             CHKERR();
           }
        }
      } else {
        if (lefrMaxStackViaCbk) {
           lefrMaxStackVia.lefiMaxStackVia::setMaxStackVia((int)(yyvsp[-1].dval));
           CALLBACK(lefrMaxStackViaCbk, lefrMaxStackViaCbkType, &lefrMaxStackVia);
        }
      }
      if (versionNum < 5.5) {
        if (lefrMaxStackViaCbk) { /* write error only if cbk is set */
           if (maxStackViaWarnings++ < lefrMaxStackViaWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                "MAXVIASTACK statement is a version 5.5 and later syntax.\nYour lef file is defined with version %g.", versionNum);
              lefError(1604, outMsg);
              lefFree(outMsg);
              CHKERR();
           }
        }
      }
      lefrHasMaxVS = 1;
    ;}
    break;

  case 297:
#line 2974 "lef.y"
    {lefDumbMode = 2; lefNoNum= 2;;}
    break;

  case 298:
#line 2976 "lef.y"
    {
      if (!lefrHasLayer) {  /* 5.5 */
        if (lefrMaxStackViaCbk) { /* write error only if cbk is set */
           if (maxStackViaWarnings++ < lefrMaxStackViaWarnings) {
              lefError(1602, "MAXVIASTACK statement has to be defined after the LAYER statement.");
              CHKERR();
           }
        }
      } else if (lefrHasMaxVS) {
        if (lefrMaxStackViaCbk) { /* write error only if cbk is set */
           if (maxStackViaWarnings++ < lefrMaxStackViaWarnings) {
             lefError(1603, "A MAXVIASTACK has already defined.\nOnly one MAXVIASTACK is allowed per lef file.");
             CHKERR();
           }
        }
      } else {
        if (lefrMaxStackViaCbk) {
           lefrMaxStackVia.lefiMaxStackVia::setMaxStackVia((int)(yyvsp[-5].dval));
           lefrMaxStackVia.lefiMaxStackVia::setMaxStackViaRange((yyvsp[-2].string), (yyvsp[-1].string));
           CALLBACK(lefrMaxStackViaCbk, lefrMaxStackViaCbkType, &lefrMaxStackVia);
        }
      }
      lefrHasMaxVS = 1;
    ;}
    break;

  case 299:
#line 3001 "lef.y"
    { hasViaRule_layer = 0; ;}
    break;

  case 300:
#line 3002 "lef.y"
    { 
      if (lefrViaCbk)
        CALLBACK(lefrViaCbk, lefrViaCbkType, &lefrVia);
    ;}
    break;

  case 301:
#line 3008 "lef.y"
    { lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 302:
#line 3011 "lef.y"
    {
      /* 0 is nodefault */
      if (lefrViaCbk) lefrVia.lefiVia::setName((yyvsp[0].string), 0);
      viaLayer = 0;
      numVia++;
      //strcpy(viaName, $2);
      viaName = strdup((yyvsp[0].string));
    ;}
    break;

  case 303:
#line 3020 "lef.y"
    {
      /* 1 is default */
      if (lefrViaCbk) lefrVia.lefiVia::setName((yyvsp[-1].string), 1);
      viaLayer = 0;
      //strcpy(viaName, $2);
      viaName = strdup((yyvsp[-1].string));
    ;}
    break;

  case 304:
#line 3028 "lef.y"
    {
      /* 2 is generated */
      if (lefrViaCbk) lefrVia.lefiVia::setName((yyvsp[-1].string), 2);
      viaLayer = 0;
      //strcpy(viaName, $2);
      viaName = strdup((yyvsp[-1].string));
    ;}
    break;

  case 305:
#line 3036 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 306:
#line 3038 "lef.y"
    {lefDumbMode = 3; lefNoNum = 1; ;}
    break;

  case 307:
#line 3041 "lef.y"
    {
       if (versionNum < 5.6) {
         if (lefrViaCbk) { /* write error only if cbk is set */
            if (viaRuleWarnings++ < lefrViaRuleWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                "VIARULE statement is a version 5.6 and later syntax.\nYour lef file is defined with version %g.", versionNum);
              lefError(1605, outMsg);
              lefFree(outMsg);
              CHKERR();
            }
         }
       } else {
         if (lefrViaCbk) lefrVia.lefiVia::setViaRule((yyvsp[-21].string), (yyvsp[-18].dval), (yyvsp[-17].dval), (yyvsp[-13].string), (yyvsp[-12].string), (yyvsp[-11].string),
                         (yyvsp[-8].dval), (yyvsp[-7].dval), (yyvsp[-4].dval), (yyvsp[-3].dval), (yyvsp[-2].dval), (yyvsp[-1].dval));
       }
       viaLayer++;
       hasViaRule_layer = 1;
    ;}
    break;

  case 311:
#line 3068 "lef.y"
    {
       if (lefrViaCbk) lefrVia.lefiVia::setRowCol((int)(yyvsp[-2].dval), (int)(yyvsp[-1].dval));
    ;}
    break;

  case 312:
#line 3072 "lef.y"
    {
       if (lefrViaCbk) lefrVia.lefiVia::setOrigin((yyvsp[-2].dval), (yyvsp[-1].dval));
    ;}
    break;

  case 313:
#line 3076 "lef.y"
    {
       if (lefrViaCbk) lefrVia.lefiVia::setOffset((yyvsp[-4].dval), (yyvsp[-3].dval), (yyvsp[-2].dval), (yyvsp[-1].dval));
    ;}
    break;

  case 314:
#line 3079 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 315:
#line 3080 "lef.y"
    {
       if (lefrViaCbk) lefrVia.lefiVia::setPattern((yyvsp[-1].string));
    ;}
    break;

  case 321:
#line 3097 "lef.y"
    { ;}
    break;

  case 322:
#line 3099 "lef.y"
    { ;}
    break;

  case 323:
#line 3101 "lef.y"
    { if (lefrViaCbk) lefrVia.lefiVia::setResistance((yyvsp[-1].dval)); ;}
    break;

  case 324:
#line 3102 "lef.y"
    { lefDumbMode = 1000000; lefRealNum = 1; ;}
    break;

  case 325:
#line 3103 "lef.y"
    { lefDumbMode = 0;
      lefRealNum = 0;
    ;}
    break;

  case 326:
#line 3107 "lef.y"
    { 
      if (versionNum < 5.6) {
        if (lefrViaCbk) lefrVia.lefiVia::setTopOfStack();
      } else
        if (lefrViaCbk)  /* write warning only if cbk is set */
           if (viaWarnings++ < lefrViaWarnings)
              lefWarning(2019, "TOPOFSTACKONLY is obsoleted in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later");
    ;}
    break;

  case 329:
#line 3123 "lef.y"
    { 
      char temp[32];
      sprintf(temp, "%.11g", (yyvsp[0].dval));
      if (lefrViaCbk) {
         char propTp;
         propTp = lefrViaProp.lefiPropType::propType((yyvsp[-1].string));
         lefrVia.lefiVia::addNumProp((yyvsp[-1].string), (yyvsp[0].dval), temp, propTp);
      }
    ;}
    break;

  case 330:
#line 3133 "lef.y"
    {
      if (lefrViaCbk) {
         char propTp;
         propTp = lefrViaProp.lefiPropType::propType((yyvsp[-1].string));
         lefrVia.lefiVia::addProp((yyvsp[-1].string), (yyvsp[0].string), propTp);
      }
    ;}
    break;

  case 331:
#line 3141 "lef.y"
    {
      if (lefrViaCbk) {
         char propTp;
         propTp = lefrViaProp.lefiPropType::propType((yyvsp[-1].string));
         lefrVia.lefiVia::addProp((yyvsp[-1].string), (yyvsp[0].string), propTp);
      }
    ;}
    break;

  case 332:
#line 3151 "lef.y"
    {
      if (versionNum < 5.6) {
        if (lefrViaCbk) lefrVia.lefiVia::setForeign((yyvsp[-1].string), 0, 0.0, 0.0, -1);
      } else
        if (lefrViaCbk)  /* write warning only if cbk is set */
           if (viaWarnings++ < lefrViaWarnings)
             lefWarning(2020, "FOREIGN in VIA is obsoleted in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
    ;}
    break;

  case 333:
#line 3160 "lef.y"
    {
      if (versionNum < 5.6) {
        if (lefrViaCbk) lefrVia.lefiVia::setForeign((yyvsp[-2].string), 1, (yyvsp[-1].pt).x, (yyvsp[-1].pt).y, -1);
      } else
        if (lefrViaCbk)  /* write warning only if cbk is set */
           if (viaWarnings++ < lefrViaWarnings)
             lefWarning(2020, "FOREIGN in VIA is obsoleted in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
    ;}
    break;

  case 334:
#line 3169 "lef.y"
    {
      if (versionNum < 5.6) {
        if (lefrViaCbk) lefrVia.lefiVia::setForeign((yyvsp[-3].string), 1, (yyvsp[-2].pt).x, (yyvsp[-2].pt).y, (yyvsp[-1].integer));
      } else
        if (lefrViaCbk)  /* write warning only if cbk is set */
           if (viaWarnings++ < lefrViaWarnings)
             lefWarning(2020, "FOREIGN in VIA is obsoleted in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
    ;}
    break;

  case 335:
#line 3178 "lef.y"
    {
      if (versionNum < 5.6) {
        if (lefrViaCbk) lefrVia.lefiVia::setForeign((yyvsp[-2].string), 0, 0.0, 0.0, (yyvsp[-1].integer));
      } else
        if (lefrViaCbk)  /* write warning only if cbk is set */
           if (viaWarnings++ < lefrViaWarnings)
             lefWarning(2020, "FOREIGN in VIA is obsoleted in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
    ;}
    break;

  case 336:
#line 3187 "lef.y"
    {lefDumbMode = 1; lefNoNum= 1;;}
    break;

  case 337:
#line 3188 "lef.y"
    { (yyval.string) = (yyvsp[0].string); ;}
    break;

  case 338:
#line 3191 "lef.y"
    {(yyval.integer) = 0;;}
    break;

  case 339:
#line 3192 "lef.y"
    {(yyval.integer) = 1;;}
    break;

  case 340:
#line 3193 "lef.y"
    {(yyval.integer) = 2;;}
    break;

  case 341:
#line 3194 "lef.y"
    {(yyval.integer) = 3;;}
    break;

  case 342:
#line 3195 "lef.y"
    {(yyval.integer) = 4;;}
    break;

  case 343:
#line 3196 "lef.y"
    {(yyval.integer) = 5;;}
    break;

  case 344:
#line 3197 "lef.y"
    {(yyval.integer) = 6;;}
    break;

  case 345:
#line 3198 "lef.y"
    {(yyval.integer) = 7;;}
    break;

  case 346:
#line 3199 "lef.y"
    {(yyval.integer) = 0;;}
    break;

  case 347:
#line 3200 "lef.y"
    {(yyval.integer) = 1;;}
    break;

  case 348:
#line 3201 "lef.y"
    {(yyval.integer) = 2;;}
    break;

  case 349:
#line 3202 "lef.y"
    {(yyval.integer) = 3;;}
    break;

  case 350:
#line 3203 "lef.y"
    {(yyval.integer) = 4;;}
    break;

  case 351:
#line 3204 "lef.y"
    {(yyval.integer) = 5;;}
    break;

  case 352:
#line 3205 "lef.y"
    {(yyval.integer) = 6;;}
    break;

  case 353:
#line 3206 "lef.y"
    {(yyval.integer) = 7;;}
    break;

  case 354:
#line 3209 "lef.y"
    { ;}
    break;

  case 355:
#line 3211 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 356:
#line 3212 "lef.y"
    {
      if (lefrViaCbk) lefrVia.lefiVia::addLayer((yyvsp[-1].string));
      viaLayer++;
      hasViaRule_layer = 1;
    ;}
    break;

  case 359:
#line 3225 "lef.y"
    { if (lefrViaCbk)
        lefrVia.lefiVia::addRectToLayer((yyvsp[-2].pt).x, (yyvsp[-2].pt).y, (yyvsp[-1].pt).x, (yyvsp[-1].pt).y); ;}
    break;

  case 360:
#line 3228 "lef.y"
    {
      lefrGeometriesPtr = (lefiGeometries*)lefMalloc(sizeof(lefiGeometries));
      lefrGeometriesPtr->lefiGeometries::Init();
      lefrDoGeometries = 1;
    ;}
    break;

  case 361:
#line 3234 "lef.y"
    { 
      lefrGeometriesPtr->lefiGeometries::addPolygon();
      if (lefrViaCbk)
        lefrVia.lefiVia::addPolyToLayer(lefrGeometriesPtr);   // 5.6
      lefrGeometriesPtr->lefiGeometries::clearPolyItems();  // free items fields
      lefFree((char*)(lefrGeometriesPtr)); // Don't need anymore, poly data has
      lefrDoGeometries = 0;                // copied
    ;}
    break;

  case 362:
#line 3243 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1;;}
    break;

  case 363:
#line 3244 "lef.y"
    { 
      // 10/17/2001 - Wanda da Rosa, PCR 404149
      //              Error if no layer in via
      if (!viaLayer) {
         if (lefrViaCbk) {  /* write error only if cbk is set */
            if (viaWarnings++ < lefrViaWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                "A LAYER statement is missing in the VIA %s.\nAt least one LAYERis required per VIA statement.", (yyvsp[0].string));
              lefError(1606, outMsg);
              lefFree(outMsg);
              CHKERR();
            }
         }
      }
      if (strcmp(viaName, (yyvsp[0].string)) != 0) {
         if (lefrViaCbk) { /* write error only if cbk is set */
            if (viaWarnings++ < lefrViaWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                "END VIA name %s is different from the VIA name %s.\nCorrect the LEF file before rerun it through the LEF parser.", (yyvsp[0].string), viaName);
              lefError(1607, outMsg);
              lefFree(outMsg);
              lefFree(viaName);
              CHKERR();
            } else
              lefFree(viaName);
         } else
            lefFree(viaName);
      } else
         lefFree(viaName);
    ;}
    break;

  case 364:
#line 3277 "lef.y"
    { lefDumbMode = 1; lefNoNum = 1;;}
    break;

  case 365:
#line 3278 "lef.y"
    { 
      if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::setName((yyvsp[0].string));
      viaRuleLayer = 0;
      //strcpy(viaRuleName, $3);
      viaRuleName = strdup((yyvsp[0].string));
      isGenerate = 0;
    ;}
    break;

  case 366:
#line 3288 "lef.y"
    {
      // 10/17/2001 - Wanda da Rosa, PCR 404163
      //              Error if layer number is not equal 2.
      // 11/14/2001 - Wanda da Rosa,
      //              Commented out for pcr 411781
      //if (viaRuleLayer != 2) {
         //lefError("VIARULE requires two layers");
         //CHKERR();
      //}
      if (viaRuleLayer == 0 || viaRuleLayer > 2) {
         if (lefrViaRuleCbk) {  /* write error only if cbk is set */
            if (viaRuleWarnings++ < lefrViaRuleWarnings) {
              lefError(1608, "A VIARULE statement requires two layers.");
              CHKERR();
            }
         }
      }
      if (lefrViaRuleCbk)
        CALLBACK(lefrViaRuleCbk, lefrViaRuleCbkType, &lefrViaRule);
      // 2/19/2004 - reset the ENCLOSURE overhang values which may be
      // set by the old syntax OVERHANG -- Not necessary, but just incase
      if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::clearLayerOverhang();
    ;}
    break;

  case 367:
#line 3314 "lef.y"
    {
      isGenerate = 1;
    ;}
    break;

  case 368:
#line 3318 "lef.y"
    {
      // 10/17/2001 - Wanda da Rosa, PCR 404181
      //              Error if layer number is not equal 3.
      // 11/14/2001 - Wanda da Rosa,
      //              Commented out for pcr 411781
      //if (viaRuleLayer != 3) {
         //lefError("VIARULE requires three layers");
         //CHKERR();
      //}
      if (viaRuleLayer == 0) {
         if (lefrViaRuleCbk) {  /* write error only if cbk is set */
            if (viaRuleWarnings++ < lefrViaRuleWarnings) {
              lefError(1609, "A VIARULE GENERATE requires three layers.");
              CHKERR();
            }
         }
      } else if ((viaRuleLayer < 3) && (versionNum >= 5.6)) {
         if (lefrViaRuleCbk)  /* write warning only if cbk is set */
            if (viaRuleWarnings++ < lefrViaRuleWarnings)
              lefWarning(2021, "turn-via is obsoleted in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
      } else {
         if (lefrViaRuleCbk) {
            lefrViaRule.lefiViaRule::setGenerate();
            CALLBACK(lefrViaRuleCbk, lefrViaRuleCbkType, &lefrViaRule);
         }
      }
      // 2/19/2004 - reset the ENCLOSURE overhang values which may be
      // set by the old syntax OVERHANG
      if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::clearLayerOverhang();
    ;}
    break;

  case 370:
#line 3351 "lef.y"
    {
      if (versionNum < 5.6) {
         if (lefrViaRuleCbk) {  /* write error only if cbk is set */
            if (viaRuleWarnings++ < lefrViaRuleWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                "DEFAULT statement is a version 5.6 and later syntax.\nYour lef file is defined with version %g.", versionNum);
              lefError(1605, outMsg);
              lefFree(outMsg);
              CHKERR();
            }
         }
      } else
        if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::setDefault();
    ;}
    break;

  case 377:
#line 3382 "lef.y"
    { lefDumbMode = 10000000; lefRealNum = 1; ;}
    break;

  case 378:
#line 3383 "lef.y"
    { lefDumbMode = 0;
      lefRealNum = 0;
    ;}
    break;

  case 381:
#line 3394 "lef.y"
    {
      if (lefrViaRuleCbk) {
         char propTp;
         propTp = lefrViaRuleProp.lefiPropType::propType((yyvsp[-1].string));
         lefrViaRule.lefiViaRule::addProp((yyvsp[-1].string), (yyvsp[0].string), propTp);
      }
    ;}
    break;

  case 382:
#line 3402 "lef.y"
    {
      if (lefrViaRuleCbk) {
         char propTp;
         propTp = lefrViaRuleProp.lefiPropType::propType((yyvsp[-1].string));
         lefrViaRule.lefiViaRule::addProp((yyvsp[-1].string), (yyvsp[0].string), propTp);
      }
    ;}
    break;

  case 383:
#line 3410 "lef.y"
    {
      char temp[32];
      sprintf(temp, "%.11g", (yyvsp[0].dval));
      if (lefrViaRuleCbk) {
         char propTp;
         propTp = lefrViaRuleProp.lefiPropType::propType((yyvsp[-1].string));
         lefrViaRule.lefiViaRule::addNumProp((yyvsp[-1].string), (yyvsp[0].dval), temp, propTp);
      }
    ;}
    break;

  case 384:
#line 3421 "lef.y"
    {
      // 10/18/2001 - Wanda da Rosa PCR 404181
      //              Make sure the 1st 2 layers in viarule has direction
      // 04/28/2004 - PCR 704072 - DIRECTION in viarule generate is
      //              obsoleted in 5.6
      if (versionNum >= 5.6) {
         if (viaRuleLayer < 2 && !viaRuleHasDir && !viaRuleHasEnc &&
             !isGenerate) {
            if (lefrViaRuleCbk) {  /* write error only if cbk is set */
               if (viaRuleWarnings++ < lefrViaRuleWarnings) {
                  lefError(1606, "VIARULE statement in a layer, requires a DIRECTION construct statement.");
                  CHKERR(); 
               }
            }
         }
      } else {
         if (viaRuleLayer < 2 && !viaRuleHasDir && !viaRuleHasEnc &&
             isGenerate) {
            if (lefrViaRuleCbk) {  /* write error only if cbk is set */
               if (viaRuleWarnings++ < lefrViaRuleWarnings) {
                  lefError(1606, "VIARULE statement in a layer, requires a DIRECTION construct statement.");
                  CHKERR(); 
               }
            }
         }
      }
      viaRuleLayer++;
    ;}
    break;

  case 387:
#line 3457 "lef.y"
    { if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::addViaName((yyvsp[-1].string)); ;}
    break;

  case 388:
#line 3459 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 389:
#line 3460 "lef.y"
    { if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::setLayer((yyvsp[-1].string));
      viaRuleHasDir = 0;
      viaRuleHasEnc = 0;
    ;}
    break;

  case 392:
#line 3472 "lef.y"
    {
      if (viaRuleHasEnc) {
        if (lefrViaRuleCbk) {  /* write error only if cbk is set */
           if (viaRuleWarnings++ < lefrViaRuleWarnings) {
              lefError(1607, "An ENCLOSRE statement has already defined in the layer.\nOnly either DIRECTION or ENCLOSURE can be specified in a layer.");
              CHKERR();
           }
        }
      } else {
        if ((versionNum < 5.6) || (!isGenerate)) {
          if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::setHorizontal();
        } else
          if (lefrViaRuleCbk)  /* write warning only if cbk is set */
             if (viaRuleWarnings++ < lefrViaRuleWarnings)
               lefWarning(2022, "DIRECTION in VIARULE is obsolete in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
      }
      viaRuleHasDir = 1;
    ;}
    break;

  case 393:
#line 3491 "lef.y"
    { 
      if (viaRuleHasEnc) {
        if (lefrViaRuleCbk) { /* write error only if cbk is set */
           if (viaRuleWarnings++ < lefrViaRuleWarnings) {
              lefError(1607, "An ENCLOSRE statement has already defined in the layer.\nOnly either DIRECTION or ENCLOSURE can be specified in a layer.");
              CHKERR();
           }
        }
      } else {
        if ((versionNum < 5.6) || (!isGenerate)) {
          if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::setVertical();
        } else
          if (lefrViaRuleCbk) /* write warning only if cbk is set */
           if (viaRuleWarnings++ < lefrViaRuleWarnings)
              lefWarning(2022, "DIRECTION in VIARULE is obsoleted in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
      }
      viaRuleHasDir = 1;
    ;}
    break;

  case 394:
#line 3510 "lef.y"
    {
      if (versionNum < 5.5) {
         if (lefrViaRuleCbk) { /* write error only if cbk is set */
           if (viaRuleWarnings++ < lefrViaRuleWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                "ENCLOSURE statement is a version 5.5 and later syntax.\nYour lef file is defined with version %g.", versionNum);
              lefError(1608, outMsg);
              lefFree(outMsg);
              CHKERR();
           }
         }
      }
      // 2/19/2004 - Enforced the rule that ENCLOSURE can only be defined
      // in VIARULE GENERATE
      if (!isGenerate) {
         if (lefrViaRuleCbk) { /* write error only if cbk is set */
           if (viaRuleWarnings++ < lefrViaRuleWarnings) {
              lefError(1614, "An ENCLOSURE statement is defined in a VIARULE statement only.\nOVERHANG statement can only be defined in VIARULE GENERATE.");
              CHKERR();
           }
         }
      }
      if (viaRuleHasDir) {
         if (lefrViaRuleCbk) { /* write error only if cbk is set */
           if (viaRuleWarnings++ < lefrViaRuleWarnings) {
              lefError(1609, "A DIRECTION statement has already defined in the layer.\nOnly either DIRECTION or ENCLOSURE can be specified in a layer.");
              CHKERR();
           }
         }
      } else {
         if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::setEnclosure((yyvsp[-2].dval), (yyvsp[-1].dval));
      }
      viaRuleHasEnc = 1;
    ;}
    break;

  case 395:
#line 3546 "lef.y"
    { if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::setWidth((yyvsp[-3].dval),(yyvsp[-1].dval)); ;}
    break;

  case 396:
#line 3548 "lef.y"
    { if (lefrViaRuleCbk)
	lefrViaRule.lefiViaRule::setRect((yyvsp[-2].pt).x, (yyvsp[-2].pt).y, (yyvsp[-1].pt).x, (yyvsp[-1].pt).y); ;}
    break;

  case 397:
#line 3551 "lef.y"
    { if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::setSpacing((yyvsp[-3].dval),(yyvsp[-1].dval)); ;}
    break;

  case 398:
#line 3553 "lef.y"
    { if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::setResistance((yyvsp[-1].dval)); ;}
    break;

  case 399:
#line 3555 "lef.y"
    {
      if (!viaRuleHasDir) {
         if (lefrViaRuleCbk) {  /* write error only if cbk is set */
            if (viaRuleWarnings++ < lefrViaRuleWarnings) {
               lefError(1610, "An OVERHANG statement is defined, but the required DIRECTION statement is not yet defined.\nUpdate the LEF file to define the DIRECTION statement before the OVERHANG.");
               CHKERR();
            }
         }
      }
      // 2/19/2004 - Enforced the rule that OVERHANG can only be defined
      // in VIARULE GENERATE after 5.3
      if ((versionNum > 5.3) && (!isGenerate)) {
         if (lefrViaRuleCbk) {  /* write error only if cbk is set */
            if (viaRuleWarnings++ < lefrViaRuleWarnings) {
               lefError(1611, "An OVERHANG statement is defined in a VIARULE statement only.\nOVERHANG statement can only be defined in VIARULE GENERATE.");
               CHKERR();
            }
         }
      }
      if (versionNum < 5.6) {
        if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::setOverhang((yyvsp[-1].dval));
      } else {
        if (lefrViaRuleCbk)  /* write warning only if cbk is set */
           if (viaRuleWarnings++ < lefrViaRuleWarnings)
              lefWarning(2023, "OVERHANG statement will be translated into similar ENCLOSURE rule");
        // In 5.6 & later, set it to either ENCLOSURE overhang1 or overhang2
        if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::setOverhangToEnclosure((yyvsp[-1].dval));
      }
    ;}
    break;

  case 400:
#line 3585 "lef.y"
    {
      // 2/19/2004 - Enforced the rule that METALOVERHANG can only be defined
      // in VIARULE GENERATE
      if ((versionNum > 5.3) && (!isGenerate)) {
         if (lefrViaRuleCbk) {  /* write error only if cbk is set */
            if (viaRuleWarnings++ < lefrViaRuleWarnings) {
               lefError(1612, "An METALOVERHANG statement is defined in a VIARULE statement only.\nOVERHANG statement can only be defined in VIARULE GENERATE.");
               CHKERR();
            }
         }
      }
      if (versionNum < 5.6) {
        if (!viaRuleHasDir) {
           if (lefrViaRuleCbk) {  /* write error only if cbk is set */
             if (viaRuleWarnings++ < lefrViaRuleWarnings) {
                lefError(1613, "An METALOVERHANG statement is defined, but the required DIRECTION statement is not yet defined.\nUpdate the LEF file to define the DIRECTION statement before the OVERHANG.");
                CHKERR();
             } 
           }
        }
        if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::setMetalOverhang((yyvsp[-1].dval));
      } else
        if (lefrViaRuleCbk)  /* write warning only if cbk is set */
           if (viaRuleWarnings++ < lefrViaRuleWarnings)
             lefWarning(2024, "METALOVERHANG is obsoleted in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
    ;}
    break;

  case 401:
#line 3612 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1;;}
    break;

  case 402:
#line 3613 "lef.y"
    {
      if (strcmp(viaRuleName, (yyvsp[0].string)) != 0) {
        if (lefrViaRuleCbk) {  /* write error only if cbk is set */
           if (viaRuleWarnings++ < lefrViaRuleWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                 "END VIARULE name %s is different from the VIARULE name %s.\nCorrect the LEF file before rerun it through the LEF parser.", (yyvsp[0].string), viaRuleName);
              lefError(1615, outMsg);
              lefFree(outMsg);
              lefFree(viaRuleName);
              CHKERR();
           } else
              lefFree(viaRuleName);
        } else
           lefFree(viaRuleName);
      } else
        lefFree(viaRuleName);
    ;}
    break;

  case 403:
#line 3633 "lef.y"
    { ;}
    break;

  case 404:
#line 3636 "lef.y"
    {
      hasSamenet = 0;
      if ((versionNum < 5.6) || (!ndRule)) {
        if (lefrSpacingBeginCbk)
	  CALLBACK(lefrSpacingBeginCbk, lefrSpacingBeginCbkType, 0);
      } else
        if (lefrSpacingBeginCbk)  /* write warning only if cbk is set */
           if (spacingWarnings++ < lefrSpacingWarnings)
             lefWarning(2025, "SAMENET in NONDEFAULTRULE is obsoleted in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
    ;}
    break;

  case 405:
#line 3648 "lef.y"
    {
      if ((versionNum < 5.6) || (!ndRule)) {
        if ((versionNum <= 5.4) && (!hasSamenet)) {
           lefError(1616, "SAMENET statement is required inside SPACING for any lef file with version 5.4 and earlier, but is not defined in the parsed lef file.");
           CHKERR();
        }
        if (lefrSpacingEndCbk)
           CALLBACK(lefrSpacingEndCbk, lefrSpacingEndCbkType, 0);
      }
    ;}
    break;

  case 408:
#line 3665 "lef.y"
    {
      if ((versionNum < 5.6) || (!ndRule)) {
        if (lefrSpacingCbk) {
          lefrSpacing.lefiSpacing::set((yyvsp[-3].string), (yyvsp[-2].string), (yyvsp[-1].dval), 0);
          CALLBACK(lefrSpacingCbk, lefrSpacingCbkType, &lefrSpacing);
        }
      }
    ;}
    break;

  case 409:
#line 3674 "lef.y"
    {
      if ((versionNum < 5.6) || (!ndRule)) {
        if (lefrSpacingCbk) {
	  lefrSpacing.lefiSpacing::set((yyvsp[-4].string), (yyvsp[-3].string), (yyvsp[-2].dval), 1);
	  CALLBACK(lefrSpacingCbk, lefrSpacingCbkType, &lefrSpacing);
        }
      }
    ;}
    break;

  case 410:
#line 3685 "lef.y"
    { lefDumbMode = 2; lefNoNum = 2; hasSamenet = 1; ;}
    break;

  case 411:
#line 3688 "lef.y"
    { ;}
    break;

  case 412:
#line 3691 "lef.y"
    {
      if (versionNum < 5.4) {
        if (lefrIRDropBeginCbk) 
	  CALLBACK(lefrIRDropBeginCbk, lefrIRDropBeginCbkType, 0);
      } else
        if (lefrIRDropBeginCbk) /* write warning only if cbk is set */
          if (iRDropWarnings++ < lefrIRDropWarnings)
            lefWarning(2026, "IRDROP is obsoleted in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
    ;}
    break;

  case 413:
#line 3702 "lef.y"
    {
      if (versionNum < 5.4) {
        if (lefrIRDropEndCbk)
	  CALLBACK(lefrIRDropEndCbk, lefrIRDropEndCbkType, 0);
      }
    ;}
    break;

  case 416:
#line 3716 "lef.y"
    { 
      if (versionNum < 5.4) {
        if (lefrIRDropCbk)
          CALLBACK(lefrIRDropCbk, lefrIRDropCbkType, &lefrIRDrop);
      }
    ;}
    break;

  case 419:
#line 3729 "lef.y"
    { if (lefrIRDropCbk) lefrIRDrop.lefiIRDrop::setValues((yyvsp[-1].dval), (yyvsp[0].dval)); ;}
    break;

  case 420:
#line 3732 "lef.y"
    { if (lefrIRDropCbk) lefrIRDrop.lefiIRDrop::setTableName((yyvsp[0].string)); ;}
    break;

  case 421:
#line 3735 "lef.y"
    {
    if (versionNum < 5.4) {
       if (lefrMinFeatureCbk) {
         lefrMinFeature.lefiMinFeature::set((yyvsp[-2].dval), (yyvsp[-1].dval));
         CALLBACK(lefrMinFeatureCbk, lefrMinFeatureCbkType, &lefrMinFeature);
       }
    } else
       if (lefrMinFeatureCbk) /* write warning only if cbk is set */
          if (minFeatureWarnings++ < lefrMinFeatureWarnings)
            lefWarning(2027, "MINFEATURE is obsoleted in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
  ;}
    break;

  case 422:
#line 3748 "lef.y"
    {
    if (versionNum < 5.4) {
       if (lefrDielectricCbk)
         CALLBACK(lefrDielectricCbk, lefrDielectricCbkType, (yyvsp[-1].dval));
    } else
       if (lefrDielectricCbk) /* write warning only if cbk is set */
         if (dielectricWarnings++ < lefrDielectricWarnings)
           lefWarning(2028, "DIELECTRIC is obsoleted in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
  ;}
    break;

  case 423:
#line 3758 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1;;}
    break;

  case 424:
#line 3759 "lef.y"
    {
    (void)lefSetNonDefault((yyvsp[0].string));
    if (lefrNonDefaultCbk) lefrNonDefault.lefiNonDefault::setName((yyvsp[0].string));
    ndLayer = 0;
    ndRule = 1;
    numVia = 0;
    //strcpy(nonDefaultRuleName, $3);
    nonDefaultRuleName = strdup((yyvsp[0].string));
  ;}
    break;

  case 425:
#line 3769 "lef.y"
    {lefNdRule = 1;;}
    break;

  case 426:
#line 3770 "lef.y"
    {
    // 10/18/2001 - Wanda da Rosa, PCR 404189
    //              At least 1 layer is required
    if ((!ndLayer) && (!lefrRelaxMode)) {
       if (lefrNonDefaultCbk) { /* write error only if cbk is set */
         if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
            lefError(1617, "NONDEFAULTRULE statement requires at least one LAYER statement.");
            CHKERR();
         }
       }
    }
    if ((!numVia) && (!lefrRelaxMode) && (versionNum < 5.6)) {
       // VIA is no longer a required statement in 5.6
       if (lefrNonDefaultCbk) { /* write error only if cbk is set */
         if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
            lefError(1618, "NONDEFAULTRULE statement requires at least one VIA statement.");
            CHKERR();
         }
       }
    }
    if (lefrNonDefaultCbk) {
      lefrNonDefault.lefiNonDefault::end();
      CALLBACK(lefrNonDefaultCbk, lefrNonDefaultCbkType, &lefrNonDefault);
    }
    ndRule = 0;
    lefDumbMode = 0;
    (void)lefUnsetNonDefault();
  ;}
    break;

  case 427:
#line 3800 "lef.y"
    {
      if ((nonDefaultRuleName) && (*nonDefaultRuleName != '\0'))
        lefFree(nonDefaultRuleName);
    ;}
    break;

  case 428:
#line 3805 "lef.y"
    {
      if (strcmp(nonDefaultRuleName, (yyvsp[0].string)) != 0) {
        if (lefrNonDefaultCbk) { /* write error only if cbk is set */
          if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
             outMsg = (char*)lefMalloc(10000);
             sprintf (outMsg,
                "END NONDEFAULTRULE name %s is different from the NONDEFAULTRULE name %s.\nCorrect the LEF file before rerun it through the LEF parser.", (yyvsp[0].string), nonDefaultRuleName);
             lefError(1619, outMsg);
             lefFree(nonDefaultRuleName);
             lefFree(outMsg);
             CHKERR();
          } else
             lefFree(nonDefaultRuleName);
        } else
           lefFree(nonDefaultRuleName);
      } else
        lefFree(nonDefaultRuleName);
    ;}
    break;

  case 430:
#line 3828 "lef.y"
    {
       if (versionNum < 5.6) {
          if (lefrNonDefaultCbk) { /* write error only if cbk is set */
            if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                 "HARDSPACING statement is a version 5.6 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1620, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
          }
       } else 
          if (lefrNonDefaultCbk)
             lefrNonDefault.lefiNonDefault::setHardspacing();
    ;}
    break;

  case 440:
#line 3861 "lef.y"
    {
       if (versionNum < 5.6) {
          if (lefrNonDefaultCbk) { /* write error only if cbk is set */
             outMsg = (char*)lefMalloc(10000);
             sprintf (outMsg,
               "USEVIA statement is a version 5.6 and later syntax.\nYour lef file is defined with version %g.", versionNum);
             lefError(1621, outMsg);
             lefFree(outMsg);
             CHKERR();
          }
       } else {
          if (lefrNonDefaultCbk)
             lefrNonDefault.lefiNonDefault::addUseVia((yyvsp[-1].string));
       }
    ;}
    break;

  case 441:
#line 3878 "lef.y"
    {
       if (versionNum < 5.6) {
          if (lefrNonDefaultCbk) { /* write error only if cbk is set */
             if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
                outMsg = (char*)lefMalloc(10000);
                sprintf (outMsg,
                  "USEVIARULE statement is a version 5.6 and later syntax.\nYour lef file is defined with version %g.", versionNum);
                lefError(1622, outMsg);
                lefFree(outMsg);
                CHKERR();
             }
          }
       } else {
          if (lefrNonDefaultCbk)
             lefrNonDefault.lefiNonDefault::addUseViaRule((yyvsp[-1].string));
       }
    ;}
    break;

  case 442:
#line 3897 "lef.y"
    {
       if (versionNum < 5.6) {
          if (lefrNonDefaultCbk) { /* write error only if cbk is set */
             if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
                outMsg = (char*)lefMalloc(10000);
                sprintf (outMsg,
                  "MINCUTS statement is a version 5.6 and later syntax.\nYour lef file is defined with version %g.", versionNum);
                lefError(1623, outMsg);
                lefFree(outMsg);
                CHKERR();
             }
          }
       } else {
          if (lefrNonDefaultCbk)
             lefrNonDefault.lefiNonDefault::addMinCuts((yyvsp[-2].string), (int)(yyvsp[-1].dval));
       }
    ;}
    break;

  case 443:
#line 3915 "lef.y"
    { lefDumbMode = 10000000; lefRealNum = 1; ;}
    break;

  case 444:
#line 3916 "lef.y"
    { lefDumbMode = 0;
      lefRealNum = 0;
    ;}
    break;

  case 447:
#line 3927 "lef.y"
    {
      if (lefrNonDefaultCbk) {
         char propTp;
         propTp = lefrNondefProp.lefiPropType::propType((yyvsp[-1].string));
         lefrNonDefault.lefiNonDefault::addProp((yyvsp[-1].string), (yyvsp[0].string), propTp);
      }
    ;}
    break;

  case 448:
#line 3935 "lef.y"
    {
      if (lefrNonDefaultCbk) {
         char propTp;
         propTp = lefrNondefProp.lefiPropType::propType((yyvsp[-1].string));
         lefrNonDefault.lefiNonDefault::addProp((yyvsp[-1].string), (yyvsp[0].string), propTp);
      }
    ;}
    break;

  case 449:
#line 3943 "lef.y"
    {
      if (lefrNonDefaultCbk) {
         char temp[32];
         char propTp;
         sprintf(temp, "%.11g", (yyvsp[0].dval));
         propTp = lefrNondefProp.lefiPropType::propType((yyvsp[-1].string));
         lefrNonDefault.lefiNonDefault::addNumProp((yyvsp[-1].string), (yyvsp[0].dval), temp, propTp);
      }
    ;}
    break;

  case 450:
#line 3953 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1;;}
    break;

  case 451:
#line 3954 "lef.y"
    {
    if (lefrNonDefaultCbk) lefrNonDefault.lefiNonDefault::addLayer((yyvsp[0].string));
    ndLayer++;
    //strcpy(layerName, $3);
    layerName = strdup((yyvsp[0].string));
    ndLayerWidth = 0;
    ndLayerSpace = 0;
  ;}
    break;

  case 452:
#line 3963 "lef.y"
    { 
    ndLayerWidth = 1;
    if (lefrNonDefaultCbk) lefrNonDefault.lefiNonDefault::addWidth((yyvsp[-1].dval));
  ;}
    break;

  case 453:
#line 3967 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1;;}
    break;

  case 454:
#line 3968 "lef.y"
    {
    if (strcmp(layerName, (yyvsp[0].string)) != 0) {
      if (lefrNonDefaultCbk) { /* write error only if cbk is set */
         if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
            outMsg = (char*)lefMalloc(10000);
            sprintf (outMsg,
               "END LAYER name %s is different from the LAYER name %s.\nCorrect the LEF file before rerun it through the LEF parser.", (yyvsp[-9].string), layerName);
            lefError(1624, outMsg);
            lefFree(outMsg);
            lefFree(layerName);
            CHKERR();
         } else
            lefFree(layerName);
      } else
         lefFree(layerName);
    } else
      lefFree(layerName);
    if (!ndLayerWidth) {
      if (lefrNonDefaultCbk) { /* write error only if cbk is set */
         if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
            lefError(1625, "A WIDTH statement is required in the LAYER statement in NONDEFULTRULE.");
            CHKERR();
         }
      }
    }
    if (!ndLayerSpace && versionNum < 5.6) {   // 5.6, SPACING is optional
      if (lefrNonDefaultCbk) { /* write error only if cbk is set */
         if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
            lefError(1626, "A SPACING statement is required in the LAYER statement in NONDEFULTRULE.");
            CHKERR();
         }
      }
    }
  ;}
    break;

  case 457:
#line 4011 "lef.y"
    {
      ndLayerSpace = 1;
      if (lefrNonDefaultCbk) lefrNonDefault.lefiNonDefault::addSpacing((yyvsp[-1].dval));
    ;}
    break;

  case 458:
#line 4016 "lef.y"
    { if (lefrNonDefaultCbk)
         lefrNonDefault.lefiNonDefault::addWireExtension((yyvsp[-1].dval)); ;}
    break;

  case 459:
#line 4019 "lef.y"
    {
      if (ignoreVersion) {
         if (lefrNonDefaultCbk)
            lefrNonDefault.lefiNonDefault::addResistance((yyvsp[-1].dval));
      } else if (versionNum < 5.4) {
         if (lefrNonDefaultCbk) { /* write error only if cbk is set */
            if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                 "RESISTANCE RPERSQ statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1627, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      } else if (versionNum > 5.5) {  // obsolete in 5.6
         if (lefrNonDefaultCbk) /* write warning only if cbk is set */
            if (nonDefaultWarnings++ < lefrNonDefaultWarnings)
              lefWarning(2029, "RESISTANCE RPERSQ is obsoleted in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
      } else if (lefrNonDefaultCbk)
         lefrNonDefault.lefiNonDefault::addResistance((yyvsp[-1].dval));
    ;}
    break;

  case 460:
#line 4043 "lef.y"
    {
      if (ignoreVersion) {
         if (lefrNonDefaultCbk)
            lefrNonDefault.lefiNonDefault::addCapacitance((yyvsp[-1].dval));
      } else if (versionNum < 5.4) {
         if (lefrNonDefaultCbk) { /* write error only if cbk is set */
            if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                 "CAPACITANCE CPERSQDIST statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1628, outMsg);
               lefFree(outMsg);
               CHKERR();
            }
         }
      } else if (versionNum > 5.5) { // obsolete in 5.6
         if (lefrNonDefaultCbk) /* write warning only if cbk is set */
            if (nonDefaultWarnings++ < lefrNonDefaultWarnings)
              lefWarning(2030, "CAPACITANCE CPERSQDIST is obsoleted in version 5.6. and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
      } else if (lefrNonDefaultCbk)
         lefrNonDefault.lefiNonDefault::addCapacitance((yyvsp[-1].dval));
    ;}
    break;

  case 461:
#line 4066 "lef.y"
    {
      if (ignoreVersion) {
         if (lefrNonDefaultCbk)
            lefrNonDefault.lefiNonDefault::addEdgeCap((yyvsp[-1].dval));
      } else if (versionNum < 5.4) {
         if (lefrNonDefaultCbk) { /* write error only if cbk is set */
            if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                 "EDGECAPACITANCE statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1629, outMsg);
               lefFree(outMsg);
              CHKERR();
            }
         }
      } else if (versionNum > 5.5) {  // obsolete in 5.6
         if (lefrNonDefaultCbk) /* write warning only if cbk is set */
            if (nonDefaultWarnings++ < lefrNonDefaultWarnings)
              lefWarning(2031, "EDGECAPACITANCE is obsoleted in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
      } else if (lefrNonDefaultCbk)
         lefrNonDefault.lefiNonDefault::addEdgeCap((yyvsp[-1].dval));
    ;}
    break;

  case 462:
#line 4089 "lef.y"
    {
      if (versionNum < 5.6) {  // 5.6 syntax
         if (lefrNonDefaultCbk) { /* write error only if cbk is set */
            if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                 "DIAGWIDTH statement is a version 5.6 and later syntax.\nYour lef file is defined with version %g.", versionNum);
               lefError(1630, outMsg);
               lefFree(outMsg);
               CHKERR(); 
            }
         }
      } else {
         if (lefrNonDefaultCbk)
            lefrNonDefault.lefiNonDefault::addDiagWidth((yyvsp[-1].dval));
      }
    ;}
    break;

  case 463:
#line 4108 "lef.y"
    { 
      if (lefrSiteCbk)
        CALLBACK(lefrSiteCbk, lefrSiteCbkType, &lefrSite);
    ;}
    break;

  case 464:
#line 4113 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1;;}
    break;

  case 465:
#line 4114 "lef.y"
    { 
      if (lefrSiteCbk) lefrSite.lefiSite::setName((yyvsp[0].string));
      //strcpy(siteName, $3);
      siteName = strdup((yyvsp[0].string));
      hasSiteClass = 0;
      hasSiteSize = 0;
    ;}
    break;

  case 466:
#line 4122 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1;;}
    break;

  case 467:
#line 4123 "lef.y"
    {
      if (strcmp(siteName, (yyvsp[0].string)) != 0) {
        if (lefrSiteCbk) { /* write error only if cbk is set */
           if (siteWarnings++ < lefrSiteWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                 "END SITE name %s is different from the SITE name %s.\nCorrect the LEF file before rerun it through the LEF parser.", (yyvsp[0].string), siteName);
              lefError(1631, outMsg);
              lefFree(outMsg);
              lefFree(siteName);
              CHKERR();
           } else
              lefFree(siteName);
        } else
           lefFree(siteName);
      } else {
        lefFree(siteName);
        if (lefrSiteCbk) { /* write error only if cbk is set */
          if (hasSiteClass == 0) {
             lefError(1632, "A CLASS statement is required in the SITE statement.");
             CHKERR();
          }
          if (hasSiteSize == 0) {
             lefError(1633, "A SIZE  statement is required in the SITE statement.");
             CHKERR();
          }
        }
      }
    ;}
    break;

  case 470:
#line 4160 "lef.y"
    {
/* Workaround for pcr 640902
*/
      if (lefrSiteCbk) lefrSite.lefiSite::setSize((yyvsp[-3].dval),(yyvsp[-1].dval));
      hasSiteSize = 1;
    ;}
    break;

  case 471:
#line 4167 "lef.y"
    { ;}
    break;

  case 472:
#line 4169 "lef.y"
    { 
      if (lefrSiteCbk) lefrSite.lefiSite::setClass((yyvsp[0].string));
      hasSiteClass = 1;
    ;}
    break;

  case 473:
#line 4174 "lef.y"
    { ;}
    break;

  case 474:
#line 4177 "lef.y"
    {(yyval.string) = (char*)"PAD"; ;}
    break;

  case 475:
#line 4178 "lef.y"
    {(yyval.string) = (char*)"CORE"; ;}
    break;

  case 476:
#line 4179 "lef.y"
    {(yyval.string) = (char*)"VIRTUAL"; ;}
    break;

  case 477:
#line 4182 "lef.y"
    { ;}
    break;

  case 480:
#line 4191 "lef.y"
    { if (lefrSiteCbk) lefrSite.lefiSite::setXSymmetry(); ;}
    break;

  case 481:
#line 4193 "lef.y"
    { if (lefrSiteCbk) lefrSite.lefiSite::setYSymmetry(); ;}
    break;

  case 482:
#line 4195 "lef.y"
    { if (lefrSiteCbk) lefrSite.lefiSite::set90Symmetry(); ;}
    break;

  case 483:
#line 4197 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1;;}
    break;

  case 484:
#line 4199 "lef.y"
    { ;}
    break;

  case 487:
#line 4206 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1;;}
    break;

  case 488:
#line 4207 "lef.y"
    { if (lefrSiteCbk) lefrSite.lefiSite::addRowPattern((yyvsp[-2].string), (yyvsp[-1].integer)); ;}
    break;

  case 489:
#line 4211 "lef.y"
    { (yyval.pt).x = (yyvsp[-1].dval); (yyval.pt).y = (yyvsp[0].dval); ;}
    break;

  case 490:
#line 4213 "lef.y"
    { (yyval.pt).x = (yyvsp[-2].dval); (yyval.pt).y = (yyvsp[-1].dval); ;}
    break;

  case 491:
#line 4216 "lef.y"
    { 
      if (lefrMacroCbk)
        CALLBACK(lefrMacroCbk, lefrMacroCbkType, &lefrMacro);
      lefrDoSite = 0;
    ;}
    break;

  case 493:
#line 4223 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1;;}
    break;

  case 494:
#line 4224 "lef.y"
    {
      siteDef = 0;
      symDef = 0;
      sizeDef = 0; 
      pinDef = 0; 
      obsDef = 0; 
      origDef = 0;
      lefrMacro.lefiMacro::clear();      
      if (lefrMacroBeginCbk || lefrMacroCbk) {
        // some reader may not have MacroBeginCB, but has MacroCB set
        lefrMacro.lefiMacro::setName((yyvsp[0].string));
        CALLBACK(lefrMacroBeginCbk, lefrMacroBeginCbkType, (yyvsp[0].string));
      }
      //strcpy(macroName, $3);
      macroName = strdup((yyvsp[0].string));
    ;}
    break;

  case 495:
#line 4241 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1;;}
    break;

  case 496:
#line 4242 "lef.y"
    {
      if (strcmp(macroName, (yyvsp[0].string)) != 0) {
        if (lefrMacroEndCbk) { /* write error only if cbk is set */
           if (macroWarnings++ < lefrMacroWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                 "END MACRO name %s is different from the MACRO name %s.\nCorrect the LEF file before rerun it through the LEF parser.", (yyvsp[0].string), macroName);
              lefError(1634, outMsg);
              lefFree(outMsg);
              lefFree(macroName);
              CHKERR();
           } else
              lefFree(macroName);
        } else
           lefFree(macroName);
      } else
        lefFree(macroName);
      if (lefrMacroEndCbk)
        CALLBACK(lefrMacroEndCbk, lefrMacroEndCbkType, (yyvsp[0].string));
    ;}
    break;

  case 504:
#line 4278 "lef.y"
    { ;}
    break;

  case 505:
#line 4280 "lef.y"
    { ;}
    break;

  case 506:
#line 4282 "lef.y"
    { ;}
    break;

  case 509:
#line 4286 "lef.y"
    { ;}
    break;

  case 510:
#line 4288 "lef.y"
    { ;}
    break;

  case 511:
#line 4290 "lef.y"
    { ;}
    break;

  case 512:
#line 4292 "lef.y"
    { if (lefrMacroCbk) lefrMacro.lefiMacro::setBuffer(); ;}
    break;

  case 513:
#line 4294 "lef.y"
    { if (lefrMacroCbk) lefrMacro.lefiMacro::setInverter(); ;}
    break;

  case 514:
#line 4296 "lef.y"
    { ;}
    break;

  case 515:
#line 4298 "lef.y"
    { ;}
    break;

  case 516:
#line 4300 "lef.y"
    { ;}
    break;

  case 517:
#line 4302 "lef.y"
    { ;}
    break;

  case 518:
#line 4303 "lef.y"
    {lefDumbMode = 1000000; lefRealNum = 1;;}
    break;

  case 519:
#line 4304 "lef.y"
    { lefDumbMode = 0;
        lefRealNum = 0;
      ;}
    break;

  case 522:
#line 4314 "lef.y"
    {
      if (siteDef) { /* SITE is defined before SYMMETRY */
          /* pcr 283846 suppress warning */
          if (lefrMacroCbk) /* write warning only if cbk is set */
             if (macroWarnings++ < lefrMacroWarnings)
               lefWarning(2032, "A SITE statement is defined before SYMMETRY statement.\nTo avoid this warning in the future, define SITE after SYMMETRY");
      }
      symDef = 1;
    ;}
    break;

  case 525:
#line 4331 "lef.y"
    { if (lefrMacroCbk) lefrMacro.lefiMacro::setXSymmetry(); ;}
    break;

  case 526:
#line 4333 "lef.y"
    { if (lefrMacroCbk) lefrMacro.lefiMacro::setYSymmetry(); ;}
    break;

  case 527:
#line 4335 "lef.y"
    { if (lefrMacroCbk) lefrMacro.lefiMacro::set90Symmetry(); ;}
    break;

  case 528:
#line 4339 "lef.y"
    {
      char temp[32];
      sprintf(temp, "%.11g", (yyvsp[0].dval));
      if (lefrMacroCbk) {
         char propTp;
         propTp = lefrMacroProp.lefiPropType::propType((yyvsp[-1].string));
         lefrMacro.lefiMacro::setNumProperty((yyvsp[-1].string), (yyvsp[0].dval), temp,  propTp);
      }
    ;}
    break;

  case 529:
#line 4349 "lef.y"
    {
      if (lefrMacroCbk) {
         char propTp;
         propTp = lefrMacroProp.lefiPropType::propType((yyvsp[-1].string));
         lefrMacro.lefiMacro::setProperty((yyvsp[-1].string), (yyvsp[0].string), propTp);
      }
    ;}
    break;

  case 530:
#line 4357 "lef.y"
    {
      if (lefrMacroCbk) {
         char propTp;
         propTp = lefrMacroProp.lefiPropType::propType((yyvsp[-1].string));
         lefrMacro.lefiMacro::setProperty((yyvsp[-1].string), (yyvsp[0].string), propTp);
      }
    ;}
    break;

  case 531:
#line 4366 "lef.y"
    {
       if (lefrMacroCbk) lefrMacro.lefiMacro::setClass((yyvsp[-1].string));
       if (lefrMacroClassTypeCbk)
          CALLBACK(lefrMacroClassTypeCbk, lefrMacroClassTypeCbkType, (yyvsp[-1].string));
    ;}
    break;

  case 532:
#line 4373 "lef.y"
    {(yyval.string) = (char*)"COVER"; ;}
    break;

  case 533:
#line 4375 "lef.y"
    { (yyval.string) = (char*)"COVER BUMP";
      if (versionNum < 5.5) {
        if (lefrMacroCbk) { /* write error only if cbk is set */
           if (macroWarnings++ < lefrMacroWarnings) {
              if (lefrRelaxMode)
                 lefWarning(2033, "The statement COVER BUMP is a LEF verion 5.5 syntax.\nYour LEF file is version 5.4 or earlier which is illegal but will be allowed\nbecause this application does not enforce strict version checking.\nOther tools that enforce strict checking will have a syntax error when reading this file.\nYou can change the VERSION statement in this LEF file to 5.5 or higher to stop this warning.");
              else {
                 outMsg = (char*)lefMalloc(10000);
                 sprintf (outMsg,
                    "COVER BUMP statement is a version 5.5 and later syntax.\nYour lef file is defined with version %g.", versionNum);
                 lefError(1635, outMsg);
                 lefFree(outMsg);
                 CHKERR();
              }
           }
        }
      }
    ;}
    break;

  case 534:
#line 4393 "lef.y"
    {(yyval.string) = (char*)"RING"; ;}
    break;

  case 535:
#line 4394 "lef.y"
    {(yyval.string) = (char*)"BLOCK"; ;}
    break;

  case 536:
#line 4396 "lef.y"
    { (yyval.string) = (char*)"BLOCK BLACKBOX";
      if (versionNum < 5.5) {
        if (lefrMacroCbk) { /* write error only if cbk is set */
           if (macroWarnings++ < lefrMacroWarnings) {
             if (lefrRelaxMode)
                lefWarning(2034, "The statement BLOCK BLACKBOX is a LEF verion 5.5 syntax.\nYour LEF file is version 5.4 or earlier which is illegal but will be allowed\nbecause this application does not enforce strict version checking.\nOther tools that enforce strict checking will have a syntax error when reading this file.\nYou can change the VERSION statement in this LEF file to 5.5 or higher to stop this warning.");
              else {
                 outMsg = (char*)lefMalloc(10000);
                 sprintf (outMsg,
                    "BLOCK BLACKBOX statement is a version 5.5 and later syntax.\nYour lef file is defined with version %g.", versionNum);
                 lefError(1636, outMsg);
                 lefFree(outMsg);
                 CHKERR();
              }
           }
        }
      }
    ;}
    break;

  case 537:
#line 4415 "lef.y"
    {
      if (ignoreVersion) {
        (yyval.string) = (char*)"BLOCK SOFT";
      } else if (versionNum < 5.6) {
        if (lefrMacroCbk) { /* write error only if cbk is set */
           if (macroWarnings++ < lefrMacroWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                 "BLOCK SOFT statement is a version 5.6 and later syntax.\nYour lef file is defined with version %g.", versionNum);
              lefError(1637, outMsg);
              lefFree(outMsg);
              CHKERR();
           }
        }
      }
      else
        (yyval.string) = (char*)"BLOCK SOFT";
    ;}
    break;

  case 538:
#line 4433 "lef.y"
    {(yyval.string) = (char*)"NONE"; ;}
    break;

  case 539:
#line 4435 "lef.y"
    {
        if (versionNum < 5.7) {
          outMsg = (char*)lefMalloc(10000);
          sprintf(outMsg,
            "BUMP is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
          lefError(1698, outMsg);
          lefFree(outMsg);
          CHKERR();
      } else
        (yyval.string) = (char*)"BUMP";
      ;}
    break;

  case 540:
#line 4446 "lef.y"
    {(yyval.string) = (char*)"PAD"; ;}
    break;

  case 541:
#line 4447 "lef.y"
    {(yyval.string) = (char*)"VIRTUAL"; ;}
    break;

  case 542:
#line 4449 "lef.y"
    {  sprintf(temp_name, "PAD %s", (yyvsp[0].string));
        (yyval.string) = temp_name; 
        if (versionNum < 5.5) {
           if (strcmp("AREAIO", (yyvsp[0].string)) != 0) {
             sprintf(temp_name, "PAD %s", (yyvsp[0].string));
             (yyval.string) = temp_name; 
           } else if (lefrMacroCbk) { 
             if (macroWarnings++ < lefrMacroWarnings) {
               if (lefrRelaxMode)
                  lefWarning(2035, "The statement PAD AREAIO is a LEF verion 5.5 syntax.\nYour LEF file is version 5.4 or earlier which is illegal but will be allowed\nbecause this application does not enforce strict version checking.\nOther tools that enforce strict checking will have a syntax error when reading this file.\nYou can change the VERSION statement in this LEF file to 5.5 or higher to stop this warning.");
               else {
                  outMsg = (char*)lefMalloc(10000);
                  sprintf (outMsg,
                     "PAD AREAIO statement is a version 5.5 and later syntax.\nYour lef file is defined with version %g.", versionNum);
                  lefError(1638, outMsg);
                  lefFree(outMsg);
                  CHKERR();
               }
            }
          }
        }
      ;}
    break;

  case 543:
#line 4471 "lef.y"
    {(yyval.string) = (char*)"CORE"; ;}
    break;

  case 544:
#line 4473 "lef.y"
    {(yyval.string) = (char*)"CORNER";
      /* This token is NOT in the spec but has shown up in 
       * some lef files.  This exception came from LEFOUT
       * in 'frameworks'
       */
      ;}
    break;

  case 545:
#line 4480 "lef.y"
    {sprintf(temp_name, "CORE %s", (yyvsp[0].string));
      (yyval.string) = temp_name;;}
    break;

  case 546:
#line 4483 "lef.y"
    {sprintf(temp_name, "ENDCAP %s", (yyvsp[0].string));
      (yyval.string) = temp_name;;}
    break;

  case 547:
#line 4488 "lef.y"
    {(yyval.string) = (char*)"INPUT";;}
    break;

  case 548:
#line 4489 "lef.y"
    {(yyval.string) = (char*)"OUTPUT";;}
    break;

  case 549:
#line 4490 "lef.y"
    {(yyval.string) = (char*)"INOUT";;}
    break;

  case 550:
#line 4491 "lef.y"
    {(yyval.string) = (char*)"POWER";;}
    break;

  case 551:
#line 4492 "lef.y"
    {(yyval.string) = (char*)"SPACER";;}
    break;

  case 552:
#line 4493 "lef.y"
    {(yyval.string) = (char*)"AREAIO";;}
    break;

  case 553:
#line 4496 "lef.y"
    {(yyval.string) = (char*)"FEEDTHRU";;}
    break;

  case 554:
#line 4497 "lef.y"
    {(yyval.string) = (char*)"TIEHIGH";;}
    break;

  case 555:
#line 4498 "lef.y"
    {(yyval.string) = (char*)"TIELOW";;}
    break;

  case 556:
#line 4500 "lef.y"
    { 
      if (ignoreVersion) {
        (yyval.string) = (char*)"SPACER";
      } else if (versionNum < 5.4) {
        if (lefrMacroCbk) { /* write error only if cbk is set */
           if (macroWarnings++ < lefrMacroWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                 "SPACER statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
              lefError(1639, outMsg);
              lefFree(outMsg);
              CHKERR();
           }
        }
      }
      else
        (yyval.string) = (char*)"SPACER";
    ;}
    break;

  case 557:
#line 4519 "lef.y"
    { 
      if (ignoreVersion) {
        (yyval.string) = (char*)"ANTENNACELL";
      } else if (versionNum < 5.4) {
        if (lefrMacroCbk) { /* write error only if cbk is set */
           if (macroWarnings++ < lefrMacroWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                 "ANTENNACELL statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
              lefError(1640, outMsg);
              lefFree(outMsg);
              CHKERR();
           }
        }
      }
      else
        (yyval.string) = (char*)"ANTENNACELL";
    ;}
    break;

  case 558:
#line 4538 "lef.y"
    { 
      if (ignoreVersion) {
        (yyval.string) = (char*)"WELLTAP";
      } else if (versionNum < 5.6) {
        if (lefrMacroCbk) { /* write error only if cbk is set */
           if (macroWarnings++ < lefrMacroWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                 "WELLTAP statement is a version 5.6 and later syntax.\nYour lef file is defined with version %g.", versionNum);
              lefError(1641, outMsg);
              lefFree(outMsg);
              CHKERR();
           }
        }
      }
      else
        (yyval.string) = (char*)"WELLTAP";
    ;}
    break;

  case 559:
#line 4558 "lef.y"
    {(yyval.string) = (char*)"PRE";;}
    break;

  case 560:
#line 4559 "lef.y"
    {(yyval.string) = (char*)"POST";;}
    break;

  case 561:
#line 4560 "lef.y"
    {(yyval.string) = (char*)"TOPLEFT";;}
    break;

  case 562:
#line 4561 "lef.y"
    {(yyval.string) = (char*)"TOPRIGHT";;}
    break;

  case 563:
#line 4562 "lef.y"
    {(yyval.string) = (char*)"BOTTOMLEFT";;}
    break;

  case 564:
#line 4563 "lef.y"
    {(yyval.string) = (char*)"BOTTOMRIGHT";;}
    break;

  case 565:
#line 4566 "lef.y"
    { if (lefrMacroCbk) lefrMacro.lefiMacro::setGenerator((yyvsp[-1].string)); ;}
    break;

  case 566:
#line 4569 "lef.y"
    { if (lefrMacroCbk) lefrMacro.lefiMacro::setGenerate((yyvsp[-2].string), (yyvsp[-1].string)); ;}
    break;

  case 567:
#line 4573 "lef.y"
    {
      if (versionNum < 5.6) {
        if (lefrMacroCbk) lefrMacro.lefiMacro::setSource("USER");
      } else
        if (lefrMacroCbk) /* write warning only if cbk is set */
           if (macroWarnings++ < lefrMacroWarnings)
             lefWarning(2036, "SOURCE is obsoleted in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
    ;}
    break;

  case 568:
#line 4582 "lef.y"
    {
      if (versionNum < 5.6) {
        if (lefrMacroCbk) lefrMacro.lefiMacro::setSource("GENERATE");
      } else
        if (lefrMacroCbk) /* write warning only if cbk is set */
           if (macroWarnings++ < lefrMacroWarnings)
             lefWarning(2037, "SOURCE is obsoleted in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
    ;}
    break;

  case 569:
#line 4591 "lef.y"
    {
      if (versionNum < 5.6) {
        if (lefrMacroCbk) lefrMacro.lefiMacro::setSource("BLOCK");
      } else
        if (lefrMacroCbk) /* write warning only if cbk is set */
           if (macroWarnings++ < lefrMacroWarnings)
             lefWarning(2037, "SOURCE is obsoleted in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
    ;}
    break;

  case 570:
#line 4601 "lef.y"
    {
      if (versionNum < 5.4) {
        if (lefrMacroCbk) lefrMacro.lefiMacro::setPower((yyvsp[-1].dval));
      } else
        if (lefrMacroCbk) /* write warning only if cbk is set */
           if (macroWarnings++ < lefrMacroWarnings)
             lefWarning(2038, "MACRO POWER is obsoleted in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
    ;}
    break;

  case 571:
#line 4611 "lef.y"
    { 
       if (origDef) { /* Has multiple ORIGIN defined in a macro, stop parsing*/
          if (lefrMacroCbk) { /* write error only if cbk is set */
             if (macroWarnings++ < lefrMacroWarnings) {
                lefError(1642, "ORIGIN statement has defined more than once in a MACRO statement.\nOnly one ORIGIN statement can be defined in a Macro.\nParser stops executions.");
               CHKERR();
             }
          }
       }
       origDef = 1;
       if (siteDef) { /* SITE is defined before ORIGIN */
          /* pcr 283846 suppress warning */
          if (lefrMacroCbk) /* write warning only if cbk is set */
             if (macroWarnings++ < lefrMacroWarnings)
               lefWarning(2039, "A SITE statement is defined before ORIGIN statement.\nTo avoid this warning in the future, define SITE after ORIGIN");
       }
       if (pinDef) { /* PIN is defined before ORIGIN */
          /* pcr 283846 suppress warning */
          if (lefrMacroCbk) /* write warning only if cbk is set */
             if (macroWarnings++ < lefrMacroWarnings)
               lefWarning(2040, "A PIN statement is defined before ORIGIN statement.\nTo avoid this warning in the future, define PIN after ORIGIN");
       }
       if (obsDef) { /* OBS is defined before ORIGIN */
          /* pcr 283846 suppress warning */
          if (lefrMacroCbk) /* write warning only if cbk is set */
             if (macroWarnings++ < lefrMacroWarnings)
               lefWarning(2041, "A OBS statement is defined before ORIGIN statement.\nTo avoid this warning in the future, define OBS after ORIGIN");
       }
       /* 11/22/99 - Wanda da Rosa. PCR 283846 
                     can be defined any order.
       if (symDef)  * SYMMETRY is defined before ORIGIN *
          lefWarning("SYMMETRY is defined before ORIGIN.");
       */
       /* Add back it back in per Nora request on PCR 283846 */
       /* 1/14/2000 - Wanda da Rosa, PCR 288770
       if (sizeDef)  * SIZE is defined before ORIGIN *
          lefWarning("SIZE is defined before ORIGIN.");
       */
      
       /* Workaround for pcr 640902 */
       if (lefrMacroCbk) lefrMacro.lefiMacro::setOrigin((yyvsp[-1].pt).x, (yyvsp[-1].pt).y);
       if (lefrMacroOriginCbk) {
          macroNum.x = (yyvsp[-1].pt).x; 
          macroNum.y = (yyvsp[-1].pt).y; 
          CALLBACK(lefrMacroOriginCbk, lefrMacroOriginCbkType, macroNum);
       }
    ;}
    break;

  case 572:
#line 4661 "lef.y"
    { if (lefrMacroCbk)
      lefrMacro.lefiMacro::addForeign((yyvsp[-1].string), 0, 0.0, 0.0, -1);
    ;}
    break;

  case 573:
#line 4665 "lef.y"
    { if (lefrMacroCbk)
      lefrMacro.lefiMacro::addForeign((yyvsp[-2].string), 1, (yyvsp[-1].pt).x, (yyvsp[-1].pt).y, -1);
    ;}
    break;

  case 574:
#line 4669 "lef.y"
    { if (lefrMacroCbk)
      lefrMacro.lefiMacro::addForeign((yyvsp[-3].string), 1, (yyvsp[-2].pt).x, (yyvsp[-2].pt).y, (yyvsp[-1].integer));
    ;}
    break;

  case 575:
#line 4673 "lef.y"
    { if (lefrMacroCbk)
      lefrMacro.lefiMacro::addForeign((yyvsp[-2].string), 0, 0.0, 0.0, (yyvsp[-1].integer));
    ;}
    break;

  case 576:
#line 4677 "lef.y"
    { lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 577:
#line 4678 "lef.y"
    { if (lefrMacroCbk) lefrMacro.lefiMacro::setEEQ((yyvsp[-1].string)); ;}
    break;

  case 578:
#line 4680 "lef.y"
    { lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 579:
#line 4681 "lef.y"
    {
      if (versionNum < 5.6) {
        if (lefrMacroCbk) lefrMacro.lefiMacro::setLEQ((yyvsp[-1].string));
      } else
        if (lefrMacroCbk) /* write warning only if cbk is set */
           if (macroWarnings++ < lefrMacroWarnings)
             lefWarning(2042, "LEQ in MACRO is obsoleted in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
    ;}
    break;

  case 580:
#line 4692 "lef.y"
    {
      if (lefrMacroCbk) {
        lefrMacro.lefiMacro::setSiteName((yyvsp[-1].string));
/* For later than 5.6 release
        lefrSitePatternPtr = (lefiSitePattern*)lefMalloc(
                              sizeof(lefiSitePattern));
        lefrSitePatternPtr->lefiSitePattern::Init();
        lefrSitePatternPtr->lefiSitePattern::setSiteName($2);
*/
      }
    ;}
    break;

  case 581:
#line 4704 "lef.y"
    {
      if (lefrMacroCbk) {
        /* also set site name in the variable siteName_ in lefiMacro */
        /* this, if user wants to use method siteName will get the name also */
        /* Does not work, it will only set with the last sitename, if multiple
           SITEs are defined.  Therefore, data will not be correct
        lefrMacro.lefiMacro::setSitePatternName(
            lefrSitePatternPtr->lefiSitePattern::name());
        */
	lefrMacro.lefiMacro::setSitePattern(lefrSitePatternPtr);
	lefrSitePatternPtr = 0;
      }
    ;}
    break;

  case 582:
#line 4719 "lef.y"
    { lefDumbMode = 1; lefNoNum = 1; siteDef = 1;
        if (lefrMacroCbk) lefrDoSite = 1; ;}
    break;

  case 583:
#line 4723 "lef.y"
    { lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 584:
#line 4726 "lef.y"
    { 
      if (siteDef) { /* SITE is defined before SIZE */
         /* pcr 283846 suppress warning
         if (siteWarnings++ < lefrSiteWarnings)
           lefWarning("SITE is defined before SIZE.");
         return 1; 
         */
      }
      sizeDef = 1;
      if (lefrMacroCbk) lefrMacro.lefiMacro::setSize((yyvsp[-3].dval), (yyvsp[-1].dval));
      if (lefrMacroSizeCbk) {
         macroNum.x = (yyvsp[-3].dval); 
         macroNum.y = (yyvsp[-1].dval); 
         CALLBACK(lefrMacroSizeCbk, lefrMacroSizeCbkType, macroNum);
      }
    ;}
    break;

  case 585:
#line 4747 "lef.y"
    { 
      if (lefrPinCbk)
        CALLBACK(lefrPinCbk, lefrPinCbkType, &lefrPin);
      lefrPin.lefiPin::clear();
    ;}
    break;

  case 586:
#line 4753 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1; pinDef = 1;;}
    break;

  case 587:
#line 4754 "lef.y"
    { if (lefrPinCbk) lefrPin.lefiPin::setName((yyvsp[0].string));
      //strcpy(pinName, $3);
      pinName = strdup((yyvsp[0].string));
    ;}
    break;

  case 588:
#line 4759 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1;;}
    break;

  case 589:
#line 4760 "lef.y"
    {
      if (strcmp(pinName, (yyvsp[0].string)) != 0) {
        if (lefrMacroCbk) { /* write error only if cbk is set */
           if (macroWarnings++ < lefrMacroWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                 "END PIN name %s is different from the PIN name %s.\nCorrect the LEF file before rerun it through the LEF parser.", (yyvsp[0].string), pinName);
              lefError(1643, outMsg);
              lefFree(outMsg);
              lefFree(pinName);
              CHKERR();
           } else
              lefFree(pinName);
        } else
           lefFree(pinName);
      } else
        lefFree(pinName);
    ;}
    break;

  case 590:
#line 4781 "lef.y"
    { ;}
    break;

  case 591:
#line 4783 "lef.y"
    { ;}
    break;

  case 592:
#line 4787 "lef.y"
    {
      if (versionNum < 5.6) {
        if (lefrPinCbk) lefrPin.lefiPin::addForeign((yyvsp[-1].string), 0, 0.0, 0.0, -1);
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             lefWarning(2043, "FOREIGN in MACRO PIN is obsoleted in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
    ;}
    break;

  case 593:
#line 4796 "lef.y"
    {
      if (versionNum < 5.6) {
        if (lefrPinCbk) lefrPin.lefiPin::addForeign((yyvsp[-2].string), 1, (yyvsp[-1].pt).x, (yyvsp[-1].pt).y, -1);
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             lefWarning(2043, "FOREIGN in MACRO PIN is obsoleted in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
    ;}
    break;

  case 594:
#line 4805 "lef.y"
    {
      if (versionNum < 5.6) {
        if (lefrPinCbk) lefrPin.lefiPin::addForeign((yyvsp[-3].string), 1, (yyvsp[-2].pt).x, (yyvsp[-2].pt).y, (yyvsp[-1].integer));
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             lefWarning(2043, "FOREIGN in MACRO PIN is obsoleted in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
    ;}
    break;

  case 595:
#line 4814 "lef.y"
    {
      if (versionNum < 5.6) {
        if (lefrPinCbk) lefrPin.lefiPin::addForeign((yyvsp[-2].string), 0, 0.0, 0.0, -1);
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             lefWarning(2043, "FOREIGN in MACRO PIN is obsoleted in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
    ;}
    break;

  case 596:
#line 4823 "lef.y"
    {
      if (versionNum < 5.6) {
        if (lefrPinCbk) lefrPin.lefiPin::addForeign((yyvsp[-3].string), 1, (yyvsp[-1].pt).x, (yyvsp[-1].pt).y, -1);
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             lefWarning(2043, "FOREIGN in MACRO PIN is obsoleted in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
    ;}
    break;

  case 597:
#line 4832 "lef.y"
    {
      if (versionNum < 5.6) {
        if (lefrPinCbk) lefrPin.lefiPin::addForeign((yyvsp[-4].string), 1, (yyvsp[-2].pt).x, (yyvsp[-2].pt).y, (yyvsp[-1].integer));
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             lefWarning(2043, "FOREIGN in MACRO PIN is obsoleted in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
    ;}
    break;

  case 598:
#line 4840 "lef.y"
    { lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 599:
#line 4841 "lef.y"
    {
      if (versionNum < 5.6) {
        if (lefrPinCbk) lefrPin.lefiPin::setLEQ((yyvsp[-1].string));
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             lefWarning(2044, "LEQ in MACRO PIN is obsoleted in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
   ;}
    break;

  case 600:
#line 4850 "lef.y"
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setPower((yyvsp[-1].dval));
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             lefWarning(2045, "MACRO POWER is obsoleted in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
    ;}
    break;

  case 601:
#line 4859 "lef.y"
    { if (lefrPinCbk) lefrPin.lefiPin::setDirection((yyvsp[0].string)); ;}
    break;

  case 602:
#line 4861 "lef.y"
    { if (lefrPinCbk) lefrPin.lefiPin::setUse((yyvsp[-1].string)); ;}
    break;

  case 603:
#line 4863 "lef.y"
    { ;}
    break;

  case 604:
#line 4865 "lef.y"
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setLeakage((yyvsp[-1].dval));
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             lefWarning(2046, "MACRO LEAKAGE is obsoleted in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, r emove this statement from the LEF file with version 5.4 or later.");
    ;}
    break;

  case 605:
#line 4874 "lef.y"
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setRiseThresh((yyvsp[-1].dval));
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             lefWarning(2047, "MACRO RISETHRESH is obsoleted in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
    ;}
    break;

  case 606:
#line 4883 "lef.y"
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setFallThresh((yyvsp[-1].dval));
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             lefWarning(2048, "MACRO FALLTHRESH is obsoleted in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
    ;}
    break;

  case 607:
#line 4892 "lef.y"
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setRiseSatcur((yyvsp[-1].dval));
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             lefWarning(2049, "MACRO RISESATCUR is obsoleted in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
    ;}
    break;

  case 608:
#line 4901 "lef.y"
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setFallSatcur((yyvsp[-1].dval));
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             lefWarning(2050, "MACRO FALLSATCUR is obsoleted in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
    ;}
    break;

  case 609:
#line 4910 "lef.y"
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setVLO((yyvsp[-1].dval));
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             lefWarning(2051, "MACRO VLO is obsoleted in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
    ;}
    break;

  case 610:
#line 4919 "lef.y"
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setVHI((yyvsp[-1].dval));
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             lefWarning(2052, "MACRO VHI is obsoleted in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
    ;}
    break;

  case 611:
#line 4928 "lef.y"
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setTieoffr((yyvsp[-1].dval));
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             lefWarning(2053, "MACRO TIEOFFR is obsoleted in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
    ;}
    break;

  case 612:
#line 4937 "lef.y"
    { if (lefrPinCbk) lefrPin.lefiPin::setShape((yyvsp[-1].string)); ;}
    break;

  case 613:
#line 4938 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1;;}
    break;

  case 614:
#line 4939 "lef.y"
    { if (lefrPinCbk) lefrPin.lefiPin::setMustjoin((yyvsp[-1].string)); ;}
    break;

  case 615:
#line 4940 "lef.y"
    {lefDumbMode = 1;;}
    break;

  case 616:
#line 4941 "lef.y"
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setOutMargin((yyvsp[-2].dval), (yyvsp[-1].dval));
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             lefWarning(2054, "MACRO OUTPUTNOISEMARGIN is obsoleted in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
    ;}
    break;

  case 617:
#line 4949 "lef.y"
    {lefDumbMode = 1;;}
    break;

  case 618:
#line 4950 "lef.y"
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setOutResistance((yyvsp[-2].dval), (yyvsp[-1].dval));
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             lefWarning(2055, "MACRO OUTPUTRESISTANCE is obsoleted in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
    ;}
    break;

  case 619:
#line 4958 "lef.y"
    {lefDumbMode = 1;;}
    break;

  case 620:
#line 4959 "lef.y"
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setInMargin((yyvsp[-2].dval), (yyvsp[-1].dval));
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             lefWarning(2056, "MACRO INPUTNOISEMARGIN is obsoleted in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
    ;}
    break;

  case 621:
#line 4968 "lef.y"
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setCapacitance((yyvsp[-1].dval));
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             lefWarning(2057, "MACRO CAPACITANCE is obsoleted in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
    ;}
    break;

  case 622:
#line 4977 "lef.y"
    { if (lefrPinCbk) lefrPin.lefiPin::setMaxdelay((yyvsp[-1].dval)); ;}
    break;

  case 623:
#line 4979 "lef.y"
    { if (lefrPinCbk) lefrPin.lefiPin::setMaxload((yyvsp[-1].dval)); ;}
    break;

  case 624:
#line 4981 "lef.y"
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setResistance((yyvsp[-1].dval));
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             lefWarning(2058, "MACRO RESISTANCE is obsoleted in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
    ;}
    break;

  case 625:
#line 4990 "lef.y"
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setPulldownres((yyvsp[-1].dval));
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             lefWarning(2059, "MACRO PULLDOWNRES is obsoleted in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
    ;}
    break;

  case 626:
#line 4999 "lef.y"
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setCurrentSource("ACTIVE");
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             lefWarning(2060, "MACRO CURRENTSOURCE is obsoleted in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
    ;}
    break;

  case 627:
#line 5008 "lef.y"
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setCurrentSource("RESISTIVE");
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             lefWarning(2061, "MACRO CURRENTSOURCE is obsoleted in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
    ;}
    break;

  case 628:
#line 5017 "lef.y"
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setRiseVoltage((yyvsp[-1].dval));
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             lefWarning(2062, "MACRO RISEVOLTAGETHRESHOLD is obsoleted in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
    ;}
    break;

  case 629:
#line 5026 "lef.y"
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setFallVoltage((yyvsp[-1].dval));
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             lefWarning(2063, "MACRO FALLVOLTAGETHRESHOLD is obsoleted in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
    ;}
    break;

  case 630:
#line 5035 "lef.y"
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setTables((yyvsp[-2].string), (yyvsp[-1].string));
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             lefWarning(2064, "MACRO IV_TABLES is obsoleted in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
    ;}
    break;

  case 631:
#line 5044 "lef.y"
    { if (lefrPinCbk) lefrPin.lefiPin::setTaperRule((yyvsp[-1].string)); ;}
    break;

  case 632:
#line 5045 "lef.y"
    {lefDumbMode = 1000000; lefRealNum = 1;;}
    break;

  case 633:
#line 5046 "lef.y"
    { lefDumbMode = 0;
      lefRealNum = 0;
    ;}
    break;

  case 634:
#line 5050 "lef.y"
    {
      lefDumbMode = 0;
      hasGeoLayer = 0;
      if (lefrPinCbk) {
	lefrPin.lefiPin::addPort(lefrGeometriesPtr);
	lefrGeometriesPtr = 0;
	lefrDoGeometries = 0;
      }
      if ((needGeometry) && (needGeometry != 2))  // if the last LAYER in PORT
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             lefWarning(2065, "Either PATH, RECT or POLYGON statement is a required in MACRO/PIN/PORT.");
    ;}
    break;

  case 635:
#line 5067 "lef.y"
    {
      // Since in start_macro_port it has call the Init method, here
      // we need to call the Destroy method.
      // Still add a null pointer to set the number of port
      if (lefrPinCbk) {
        lefrPin.lefiPin::addPort(lefrGeometriesPtr);
        lefrGeometriesPtr = 0;
        lefrDoGeometries = 0;
      }
      hasGeoLayer = 0;
    ;}
    break;

  case 636:
#line 5079 "lef.y"
    {  /* a pre 5.4 syntax */
      use5_3 = 1;
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum >= 5.4) {
        if (use5_4) {
           if (lefrPinCbk) { /* write error only if cbk is set */
             if (pinWarnings++ < lefrPinWarnings) {
                outMsg = (char*)lefMalloc(10000);
                sprintf (outMsg,
                   "ANTENNASIZE statement is a version 5.3 and earlier syntax.\nYour lef file is defined with version %g.", versionNum);
                lefError(1644, outMsg);
                lefFree(outMsg);
                CHKERR();
             }
           }
        }
      }
      if (lefrPinCbk) lefrPin.lefiPin::addAntennaSize((yyvsp[-2].dval), (yyvsp[-1].string));
    ;}
    break;

  case 637:
#line 5100 "lef.y"
    {  /* a pre 5.4 syntax */
      use5_3 = 1;
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum >= 5.4) {
        if (use5_4) {
           if (lefrPinCbk) { /* write error only if cbk is set */
              if (pinWarnings++ < lefrPinWarnings) {
                 outMsg = (char*)lefMalloc(10000);
                 sprintf (outMsg,
                    "ANTENNAMETALAREA statement is a version 5.3 and earlier syntax.\nYour lef file is defined with version %g.", versionNum);
                 lefError(1645, outMsg);
                 lefFree(outMsg);
                 CHKERR();
              }
           }
        }
      }
      if (lefrPinCbk) lefrPin.lefiPin::addAntennaMetalArea((yyvsp[-2].dval), (yyvsp[-1].string));
    ;}
    break;

  case 638:
#line 5121 "lef.y"
    { /* a pre 5.4 syntax */ 
      use5_3 = 1;
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum >= 5.4) {
        if (use5_4) {
           if (lefrPinCbk) { /* write error only if cbk is set */
              if (pinWarnings++ < lefrPinWarnings) {
                 outMsg = (char*)lefMalloc(10000);
                 sprintf (outMsg,
                    "ANTENNAMETALLENGTH statement is a version 5.3 and earlier syntax.\nYour lef file is defined with version %g.", versionNum);
                 lefError(1646, outMsg);
                 lefFree(outMsg);
                 CHKERR();
              }
           }
        }
      }
      if (lefrPinCbk) lefrPin.lefiPin::addAntennaMetalLength((yyvsp[-2].dval), (yyvsp[-1].string));
    ;}
    break;

  case 639:
#line 5142 "lef.y"
    { if (lefrPinCbk) lefrPin.lefiPin::setRiseSlewLimit((yyvsp[-1].dval)); ;}
    break;

  case 640:
#line 5144 "lef.y"
    { if (lefrPinCbk) lefrPin.lefiPin::setFallSlewLimit((yyvsp[-1].dval)); ;}
    break;

  case 641:
#line 5146 "lef.y"
    { /* 5.4 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum < 5.4) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                 "ANTENNAPARTIALMETALAREA statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
              lefError(1647, outMsg);
              lefFree(outMsg);
              CHKERR();
           }
        }
      } else if (use5_3) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                 "ANTENNAPARTIALMETALAREA statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
              lefError(1647, outMsg);
              lefFree(outMsg);
              CHKERR();
           }
        }
      }
      if (lefrPinCbk) lefrPin.lefiPin::addAntennaPartialMetalArea((yyvsp[-2].dval), (yyvsp[-1].string));
    ;}
    break;

  case 642:
#line 5176 "lef.y"
    { /* 5.4 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum < 5.4) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                 "ANTENNAPARTIALMETALSIDEAREA statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
              lefError(1648, outMsg);
              lefFree(outMsg);
              CHKERR();
           }
        }
      } else if (use5_3) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                 "ANTENNAPARTIALMETALSIDEAREA statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
              lefError(1648, outMsg);
              lefFree(outMsg);
              CHKERR();
           }
        }
      }
      if (lefrPinCbk) lefrPin.lefiPin::addAntennaPartialMetalSideArea((yyvsp[-2].dval), (yyvsp[-1].string));
    ;}
    break;

  case 643:
#line 5206 "lef.y"
    { /* 5.4 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum < 5.4) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                 "ANTENNAPARTIALCUTAREA statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
              lefError(1649, outMsg);
              lefFree(outMsg);
              CHKERR();
           }
        }
      } else if (use5_3) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                 "ANTENNAPARTIALCUTAREA statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
              lefError(1649, outMsg);
              lefFree(outMsg);
              CHKERR();
           }
        }
      }
      if (lefrPinCbk) lefrPin.lefiPin::addAntennaPartialCutArea((yyvsp[-2].dval), (yyvsp[-1].string));
    ;}
    break;

  case 644:
#line 5236 "lef.y"
    { /* 5.4 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum < 5.4) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                 "ANTENNADIFFAREA statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
              lefError(1650, outMsg);
              lefFree(outMsg);
              CHKERR();
           }
        }
      } else if (use5_3) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                 "ANTENNADIFFAREA statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
              lefError(1650, outMsg);
              lefFree(outMsg);
              CHKERR();
           }
        }
      }
      if (lefrPinCbk) lefrPin.lefiPin::addAntennaDiffArea((yyvsp[-2].dval), (yyvsp[-1].string));
    ;}
    break;

  case 645:
#line 5266 "lef.y"
    { /* 5.4 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum < 5.4) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                 "ANTENNAGATEAREA statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
              lefError(1651, outMsg);
              lefFree(outMsg);
              CHKERR();
           }
        }
      } else if (use5_3) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                 "ANTENNAGATEAREA statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
              lefError(1651, outMsg);
              lefFree(outMsg);
              CHKERR();
           }
        }
      }
      if (lefrPinCbk) lefrPin.lefiPin::addAntennaGateArea((yyvsp[-2].dval), (yyvsp[-1].string));
    ;}
    break;

  case 646:
#line 5296 "lef.y"
    { /* 5.4 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum < 5.4) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                 "ANTENNAMAXAREACAR statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
              lefError(1652, outMsg);
              lefFree(outMsg);
              CHKERR();
           }
        }
      } else if (use5_3) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                 "ANTENNAMAXAREACAR statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
              lefError(1652, outMsg);
              lefFree(outMsg);
              CHKERR();
           }
        }
      }
      if (lefrPinCbk) lefrPin.lefiPin::addAntennaMaxAreaCar((yyvsp[-2].dval), (yyvsp[-1].string));
    ;}
    break;

  case 647:
#line 5326 "lef.y"
    { /* 5.4 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum < 5.4) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                 "ANTENNAMAXSIDEAREACAR statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
              lefError(1653, outMsg);
              lefFree(outMsg);
              CHKERR();
           }
        }
      } else if (use5_3) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                 "ANTENNAMAXSIDEAREACAR statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
              lefError(1653, outMsg);
              lefFree(outMsg);
              CHKERR();
           }
        }
      }
      if (lefrPinCbk) lefrPin.lefiPin::addAntennaMaxSideAreaCar((yyvsp[-2].dval), (yyvsp[-1].string));
    ;}
    break;

  case 648:
#line 5356 "lef.y"
    { /* 5.4 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum < 5.4) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                 "ANTENNAMAXCUTCAR statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
              lefError(1654, outMsg);
              lefFree(outMsg);
              CHKERR();
           }
        }
      } else if (use5_3) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                 "ANTENNAMAXCUTCAR statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
              lefError(1654, outMsg);
              lefFree(outMsg);
              CHKERR();
           }
        }
      }
      if (lefrPinCbk) lefrPin.lefiPin::addAntennaMaxCutCar((yyvsp[-2].dval), (yyvsp[-1].string));
    ;}
    break;

  case 649:
#line 5386 "lef.y"
    { /* 5.5 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum < 5.5) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                 "ANTENNAMODEL statement is a version 5.5 and later syntax.\nYour lef file is defined with version %g.", versionNum);
              lefError(1655, outMsg);
              lefFree(outMsg);
              CHKERR();
           }
        }
      } else if (use5_3) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                 "ANTENNAMODEL statement is a version 5.5 and later syntax.\nYour lef file is defined with version %g.", versionNum);
              lefError(1655, outMsg);
              lefFree(outMsg);
              CHKERR();
           }
        }
      }
    ;}
    break;

  case 651:
#line 5415 "lef.y"
    {lefDumbMode = 2; lefNoNum = 2; ;}
    break;

  case 652:
#line 5416 "lef.y"
    {
      if (versionNum < 5.6) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                 "NETEXPR statement is a version 5.6 and later syntax.\nYour lef file is defined with version %g.", versionNum);
              lefError(1656, outMsg);
              lefFree(outMsg);
              CHKERR();
           }
        }
      } else
        if (lefrPinCbk) lefrPin.lefiPin::setNetExpr((yyvsp[-1].string));
    ;}
    break;

  case 653:
#line 5431 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 654:
#line 5432 "lef.y"
    {
      if (versionNum < 5.6) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                 "SUPPLYSENSITIVITY statement is a version 5.6 and later syntax.\nYour lef file is defined with version %g.", versionNum);
              lefError(1657, outMsg);
              lefFree(outMsg);
              CHKERR();
           }
        }
      } else
        if (lefrPinCbk) lefrPin.lefiPin::setSupplySensitivity((yyvsp[-1].string));
    ;}
    break;

  case 655:
#line 5447 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 656:
#line 5448 "lef.y"
    {
      if (versionNum < 5.6) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                 "GROUNDSENSITIVITY statement is a version 5.6 and later syntax.\nYour lef file is defined with version %g.", versionNum);
              lefError(1658, outMsg);
              lefFree(outMsg);
              CHKERR();
           }
        }
      } else
        if (lefrPinCbk) lefrPin.lefiPin::setGroundSensitivity((yyvsp[-1].string));
    ;}
    break;

  case 657:
#line 5466 "lef.y"
    {
    if (lefrPinCbk)
       lefrPin.lefiPin::addAntennaModel(1);
    ;}
    break;

  case 658:
#line 5471 "lef.y"
    {
    if (lefrPinCbk)
       lefrPin.lefiPin::addAntennaModel(2);
    ;}
    break;

  case 659:
#line 5476 "lef.y"
    {
    if (lefrPinCbk)
       lefrPin.lefiPin::addAntennaModel(3);
    ;}
    break;

  case 660:
#line 5481 "lef.y"
    {
    if (lefrPinCbk)
       lefrPin.lefiPin::addAntennaModel(4);
    ;}
    break;

  case 663:
#line 5493 "lef.y"
    { 
      char temp[32];
      sprintf(temp, "%.11g", (yyvsp[0].dval));
      if (lefrPinCbk) {
         char propTp;
         propTp = lefrPinProp.lefiPropType::propType((yyvsp[-1].string));
         lefrPin.lefiPin::setNumProperty((yyvsp[-1].string), (yyvsp[0].dval), temp, propTp);
      }
    ;}
    break;

  case 664:
#line 5503 "lef.y"
    {
      if (lefrPinCbk) {
         char propTp;
         propTp = lefrPinProp.lefiPropType::propType((yyvsp[-1].string));
         lefrPin.lefiPin::setProperty((yyvsp[-1].string), (yyvsp[0].string), propTp);
      }
    ;}
    break;

  case 665:
#line 5511 "lef.y"
    {
      if (lefrPinCbk) {
         char propTp;
         propTp = lefrPinProp.lefiPropType::propType((yyvsp[-1].string));
         lefrPin.lefiPin::setProperty((yyvsp[-1].string), (yyvsp[0].string), propTp);
      }
    ;}
    break;

  case 666:
#line 5520 "lef.y"
    {(yyval.string) = (char*)"INPUT";;}
    break;

  case 667:
#line 5521 "lef.y"
    {(yyval.string) = (char*)"OUTPUT";;}
    break;

  case 668:
#line 5522 "lef.y"
    {(yyval.string) = (char*)"OUTPUT TRISTATE";;}
    break;

  case 669:
#line 5523 "lef.y"
    {(yyval.string) = (char*)"INOUT";;}
    break;

  case 670:
#line 5524 "lef.y"
    {(yyval.string) = (char*)"FEEDTHRU";;}
    break;

  case 671:
#line 5527 "lef.y"
    {
      if (lefrPinCbk) {
	lefrDoGeometries = 1;
        hasPRP = 0;
	lefrGeometriesPtr = (lefiGeometries*)lefMalloc( sizeof(lefiGeometries));
	lefrGeometriesPtr->lefiGeometries::Init();
      }
      needGeometry = 0;  // don't need rect/path/poly define yet
      hasGeoLayer = 0;   // make sure LAYER is set before geometry
    ;}
    break;

  case 673:
#line 5540 "lef.y"
    { if (lefrDoGeometries)
        lefrGeometriesPtr->lefiGeometries::addClass((yyvsp[-1].string)); ;}
    break;

  case 674:
#line 5544 "lef.y"
    {(yyval.string) = (char*)"SIGNAL";;}
    break;

  case 675:
#line 5545 "lef.y"
    {(yyval.string) = (char*)"ANALOG";;}
    break;

  case 676:
#line 5546 "lef.y"
    {(yyval.string) = (char*)"POWER";;}
    break;

  case 677:
#line 5547 "lef.y"
    {(yyval.string) = (char*)"GROUND";;}
    break;

  case 678:
#line 5548 "lef.y"
    {(yyval.string) = (char*)"CLOCK";;}
    break;

  case 679:
#line 5549 "lef.y"
    {(yyval.string) = (char*)"DATA";;}
    break;

  case 680:
#line 5552 "lef.y"
    {(yyval.string) = (char*)"INPUT";;}
    break;

  case 681:
#line 5553 "lef.y"
    {(yyval.string) = (char*)"OUTPUT";;}
    break;

  case 682:
#line 5554 "lef.y"
    {(yyval.string) = (char*)"START";;}
    break;

  case 683:
#line 5555 "lef.y"
    {(yyval.string) = (char*)"STOP";;}
    break;

  case 684:
#line 5558 "lef.y"
    {(yyval.string) = (char*)""; ;}
    break;

  case 685:
#line 5559 "lef.y"
    {(yyval.string) = (char*)"ABUTMENT";;}
    break;

  case 686:
#line 5560 "lef.y"
    {(yyval.string) = (char*)"RING";;}
    break;

  case 687:
#line 5561 "lef.y"
    {(yyval.string) = (char*)"FEEDTHRU";;}
    break;

  case 689:
#line 5566 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 690:
#line 5567 "lef.y"
    {
      if ((needGeometry) && (needGeometry != 2)) // 1 LAYER follow after another
        if (layerWarnings++ < lefrLayerWarnings) {
          /* geometries is called by MACRO/OBS & MACRO/PIN/PORT */
          if (obsDef)
             lefWarning(2076, "Either PATH, RECT or POLYGON statement is a required in MACRO/OBS.");
          else
             lefWarning(2065, "Either PATH, RECT or POLYGON statement is a required in MACRO/PIN/PORT.");
        }
      if (lefrDoGeometries)
        lefrGeometriesPtr->lefiGeometries::addLayer((yyvsp[0].string));
      needGeometry = 1;    // within LAYER it requires either path, rect, poly
      hasGeoLayer = 1;
    ;}
    break;

  case 692:
#line 5584 "lef.y"
    { 
      if (lefrDoGeometries) {
        if (hasGeoLayer == 0) {   /* LAYER statement is missing */
           if (macroWarnings++ < lefrMacroWarnings) {
              lefError(1701, "A LAYER statement is missing in Geometry.\nLAYER is a required statement before any geometry can be defined.");
              CHKERR();
           }
        } else
           lefrGeometriesPtr->lefiGeometries::addWidth((yyvsp[-1].dval)); 
      } 
    ;}
    break;

  case 693:
#line 5596 "lef.y"
    { if (lefrDoGeometries) {
        if (hasGeoLayer == 0) {   /* LAYER statement is missing */
           if (macroWarnings++ < lefrMacroWarnings) {
              lefError(1701, "A LAYER statement is missing in Geometry.\nLAYER is a required statement before any geometry can be defined.");
              CHKERR();
           }
        } else
           lefrGeometriesPtr->lefiGeometries::addPath();
      }
      hasPRP = 1;
      needGeometry = 2;
    ;}
    break;

  case 694:
#line 5609 "lef.y"
    { if (lefrDoGeometries) {
        if (hasGeoLayer == 0) {   /* LAYER statement is missing */
           if (macroWarnings++ < lefrMacroWarnings) {
              lefError(1701, "A LAYER statement is missing in Geometry.\nLAYER is a required statement before any geometry can be defined.");
              CHKERR();
           }
        } else
           lefrGeometriesPtr->lefiGeometries::addPathIter();
      } 
      hasPRP = 1;
      needGeometry = 2;
    ;}
    break;

  case 695:
#line 5622 "lef.y"
    { if (lefrDoGeometries) {
        if (hasGeoLayer == 0) {   /* LAYER statement is missing */
           if (macroWarnings++ < lefrMacroWarnings) {
              lefError(1701, "A LAYER statement is missing in Geometry.\nLAYER is a required statement before any geometry can be defined.");
              CHKERR();
           }
        } else
           lefrGeometriesPtr->lefiGeometries::addRect((yyvsp[-2].pt).x, (yyvsp[-2].pt).y, (yyvsp[-1].pt).x, (yyvsp[-1].pt).y);
      }
      needGeometry = 2;
    ;}
    break;

  case 696:
#line 5634 "lef.y"
    { if (lefrDoGeometries) {
        if (hasGeoLayer == 0) {   /* LAYER statement is missing */
           if (macroWarnings++ < lefrMacroWarnings) {
              lefError(1701, "A LAYER statement is missing in Geometry.\nLAYER is a required statement before any geometry can be defined.");
              CHKERR();
           }
        } else
           lefrGeometriesPtr->lefiGeometries::addRectIter((yyvsp[-3].pt).x, (yyvsp[-3].pt).y, (yyvsp[-2].pt).x,
                                                          (yyvsp[-2].pt).y);
      }
      needGeometry = 2;
    ;}
    break;

  case 697:
#line 5647 "lef.y"
    {
      if (lefrDoGeometries) {
        if (hasGeoLayer == 0) {   /* LAYER statement is missing */
           if (macroWarnings++ < lefrMacroWarnings) {
              lefError(1701, "A LAYER statement is missing in Geometry.\nLAYER is a required statement before any geometry can be defined.");
              CHKERR();
           }
        } else
           lefrGeometriesPtr->lefiGeometries::addPolygon();
      }
      hasPRP = 1;
      needGeometry = 2;
    ;}
    break;

  case 698:
#line 5661 "lef.y"
    { if (lefrDoGeometries) {
        if (hasGeoLayer == 0) {   /* LAYER statement is missing */
           if (macroWarnings++ < lefrMacroWarnings) {
              lefError(1701, "A LAYER statement is missing in Geometry.\nLAYER is a required statement before any geometry can be defined.");
              CHKERR();
           }
        } else
           lefrGeometriesPtr->lefiGeometries::addPolygonIter();
      }
      hasPRP = 1;
      needGeometry = 2;
    ;}
    break;

  case 699:
#line 5674 "lef.y"
    { ;}
    break;

  case 703:
#line 5681 "lef.y"
    {
      if (versionNum < 5.7) {
        outMsg = (char*)lefMalloc(10000);
        sprintf(outMsg,
          "EXCEPTPGNET is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
        lefError(1699, outMsg);
        lefFree(outMsg);
        CHKERR();
      } else {
       if (lefrDoGeometries)
        lefrGeometriesPtr->lefiGeometries::addLayerExceptPgNet();
      }
    ;}
    break;

  case 705:
#line 5697 "lef.y"
    { if (lefrDoGeometries) {
        if (zeroOrGt((yyvsp[0].dval)))
           lefrGeometriesPtr->lefiGeometries::addLayerMinSpacing((yyvsp[0].dval));
        else {
           outMsg = (char*)lefMalloc(10000);
           sprintf (outMsg,
              "THE SPACING statement has the value %g in MACRO OBS.\nValue has to be 0 or greater.", (yyvsp[0].dval));
           lefError(1659, outMsg);
           lefFree(outMsg);
           CHKERR();
        }
      }
    ;}
    break;

  case 706:
#line 5711 "lef.y"
    { if (lefrDoGeometries) {
        if (zeroOrGt((yyvsp[0].dval)))
           lefrGeometriesPtr->lefiGeometries::addLayerRuleWidth((yyvsp[0].dval));
        else {
           outMsg = (char*)lefMalloc(10000);
           sprintf (outMsg,
              "THE DESIGNRULEWIDTH statement has the value %g in MACRO OBS.\nValue has to be 0 or greater.", (yyvsp[0].dval));
           lefError(1660, outMsg);
           lefFree(outMsg);
           CHKERR();
        }
      }
    ;}
    break;

  case 707:
#line 5726 "lef.y"
    { if (lefrDoGeometries)
        lefrGeometriesPtr->lefiGeometries::startList((yyvsp[0].pt).x, (yyvsp[0].pt).y); ;}
    break;

  case 708:
#line 5730 "lef.y"
    { if (lefrDoGeometries)
        lefrGeometriesPtr->lefiGeometries::addToList((yyvsp[0].pt).x, (yyvsp[0].pt).y); ;}
    break;

  case 711:
#line 5742 "lef.y"
    {lefDumbMode = 1;;}
    break;

  case 712:
#line 5743 "lef.y"
    { if (lefrDoGeometries)
        lefrGeometriesPtr->lefiGeometries::addVia((yyvsp[-3].pt).x, (yyvsp[-3].pt).y, (yyvsp[-1].string)); ;}
    break;

  case 713:
#line 5745 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1;;}
    break;

  case 714:
#line 5747 "lef.y"
    { if (lefrDoGeometries)
        lefrGeometriesPtr->lefiGeometries::addViaIter((yyvsp[-4].pt).x, (yyvsp[-4].pt).y, (yyvsp[-2].string)); ;}
    break;

  case 715:
#line 5752 "lef.y"
    { if (lefrDoGeometries)
         lefrGeometriesPtr->lefiGeometries::addStepPattern((yyvsp[-5].dval), (yyvsp[-3].dval), (yyvsp[-1].dval), (yyvsp[0].dval)); ;}
    break;

  case 716:
#line 5757 "lef.y"
    {
      if (lefrDoSite) {
	lefrSitePatternPtr = (lefiSitePattern*)lefMalloc(
				   sizeof(lefiSitePattern));
	lefrSitePatternPtr->lefiSitePattern::Init();
	lefrSitePatternPtr->lefiSitePattern::set((yyvsp[-10].string), (yyvsp[-9].dval), (yyvsp[-8].dval), (yyvsp[-7].integer), (yyvsp[-5].dval), (yyvsp[-3].dval),
	  (yyvsp[-1].dval), (yyvsp[0].dval));
	}
    ;}
    break;

  case 717:
#line 5767 "lef.y"
    {
      if (lefrDoSite) {
	lefrSitePatternPtr = (lefiSitePattern*)lefMalloc(
				   sizeof(lefiSitePattern));
	lefrSitePatternPtr->lefiSitePattern::Init();
	lefrSitePatternPtr->lefiSitePattern::set((yyvsp[-3].string), (yyvsp[-2].dval), (yyvsp[-1].dval), (yyvsp[0].integer), -1, -1,
	  -1, -1);
	}
    ;}
    break;

  case 718:
#line 5779 "lef.y"
    { 
      if (lefrDoTrack) {
	lefrTrackPatternPtr = (lefiTrackPattern*)lefMalloc(
				sizeof(lefiTrackPattern));
	lefrTrackPatternPtr->lefiTrackPattern::Init();
	lefrTrackPatternPtr->lefiTrackPattern::set("X", (yyvsp[-4].dval), (int)(yyvsp[-2].dval), (yyvsp[0].dval));
      }    
    ;}
    break;

  case 719:
#line 5787 "lef.y"
    {lefDumbMode = 1000000000;;}
    break;

  case 720:
#line 5788 "lef.y"
    { lefDumbMode = 0;;}
    break;

  case 721:
#line 5790 "lef.y"
    { 
      if (lefrDoTrack) {
	lefrTrackPatternPtr = (lefiTrackPattern*)lefMalloc(
                                    sizeof(lefiTrackPattern));
	lefrTrackPatternPtr->lefiTrackPattern::Init();
	lefrTrackPatternPtr->lefiTrackPattern::set("Y", (yyvsp[-4].dval), (int)(yyvsp[-2].dval), (yyvsp[0].dval));
      }    
    ;}
    break;

  case 722:
#line 5798 "lef.y"
    {lefDumbMode = 1000000000;;}
    break;

  case 723:
#line 5799 "lef.y"
    { lefDumbMode = 0;;}
    break;

  case 724:
#line 5801 "lef.y"
    { 
      if (lefrDoTrack) {
	lefrTrackPatternPtr = (lefiTrackPattern*)lefMalloc(
                                    sizeof(lefiTrackPattern));
	lefrTrackPatternPtr->lefiTrackPattern::Init();
	lefrTrackPatternPtr->lefiTrackPattern::set("X", (yyvsp[-4].dval), (int)(yyvsp[-2].dval), (yyvsp[0].dval));
      }    
    ;}
    break;

  case 725:
#line 5810 "lef.y"
    { 
      if (lefrDoTrack) {
	lefrTrackPatternPtr = (lefiTrackPattern*)lefMalloc(
                                    sizeof(lefiTrackPattern));
	lefrTrackPatternPtr->lefiTrackPattern::Init();
	lefrTrackPatternPtr->lefiTrackPattern::set("Y", (yyvsp[-4].dval), (int)(yyvsp[-2].dval), (yyvsp[0].dval));
      }    
    ;}
    break;

  case 728:
#line 5825 "lef.y"
    { if (lefrDoTrack) lefrTrackPatternPtr->lefiTrackPattern::addLayer((yyvsp[0].string)); ;}
    break;

  case 729:
#line 5828 "lef.y"
    {
      if (lefrDoGcell) {
	lefrGcellPatternPtr = (lefiGcellPattern*)lefMalloc(
                                    sizeof(lefiGcellPattern));
	lefrGcellPatternPtr->lefiGcellPattern::Init();
	lefrGcellPatternPtr->lefiGcellPattern::set("X", (yyvsp[-4].dval), (int)(yyvsp[-2].dval), (yyvsp[0].dval));
      }    
    ;}
    break;

  case 730:
#line 5837 "lef.y"
    {
      if (lefrDoGcell) {
	lefrGcellPatternPtr = (lefiGcellPattern*)lefMalloc(
                                    sizeof(lefiGcellPattern));
	lefrGcellPatternPtr->lefiGcellPattern::Init();
	lefrGcellPatternPtr->lefiGcellPattern::set("Y", (yyvsp[-4].dval), (int)(yyvsp[-2].dval), (yyvsp[0].dval));
      }    
    ;}
    break;

  case 731:
#line 5847 "lef.y"
    { 
      if (lefrObstructionCbk) {
	lefrObstruction.lefiObstruction::setGeometries(lefrGeometriesPtr);
	lefrGeometriesPtr = 0;
	lefrDoGeometries = 0;
        CALLBACK(lefrObstructionCbk, lefrObstructionCbkType, &lefrObstruction);
      }
      lefDumbMode = 0;
      hasGeoLayer = 0;       /* reset */
    ;}
    break;

  case 732:
#line 5865 "lef.y"
    {
       /* The pointer has malloced in start, need to free manually */
       if (lefrGeometriesPtr) {
          lefrGeometriesPtr->lefiGeometries::Destroy();
          lefFree(lefrGeometriesPtr);
       }
       hasGeoLayer = 0;
    ;}
    break;

  case 733:
#line 5875 "lef.y"
    {
      obsDef = 1;
      if (lefrObstructionCbk) {
	lefrDoGeometries = 1;
	lefrGeometriesPtr = (lefiGeometries*)lefMalloc(
	    sizeof(lefiGeometries));
	lefrGeometriesPtr->lefiGeometries::Init();
	}
      hasGeoLayer = 0;
    ;}
    break;

  case 734:
#line 5887 "lef.y"
    { 
      if (versionNum < 5.6) {
        if (lefrDensityCbk) { /* write error only if cbk is set */
           if (macroWarnings++ < lefrMacroWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                 "DENSITY statement is a version 5.6 and later syntax.\nYour lef file is defined with version %g.", versionNum);
              lefError(1661, outMsg);
              lefFree(outMsg);
              CHKERR();
           }
        }
      } else
        if (lefrDensityCbk) {
          CALLBACK(lefrDensityCbk, lefrDensityCbkType, &lefrDensity);
          lefrDensity.lefiDensity::clear();
        }
      lefDumbMode = 0;
    ;}
    break;

  case 737:
#line 5911 "lef.y"
    { lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 738:
#line 5912 "lef.y"
    {
      if (lefrDensityCbk)
        lefrDensity.lefiDensity::addLayer((yyvsp[-1].string));
    ;}
    break;

  case 742:
#line 5923 "lef.y"
    {
      if (lefrDensityCbk)
        lefrDensity.lefiDensity::addRect((yyvsp[-3].pt).x, (yyvsp[-3].pt).y, (yyvsp[-2].pt).x, (yyvsp[-2].pt).y, (yyvsp[-1].dval)); 
    ;}
    break;

  case 743:
#line 5928 "lef.y"
    { lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 744:
#line 5929 "lef.y"
    { if (lefrMacroCbk) lefrMacro.lefiMacro::setClockType((yyvsp[-1].string)); ;}
    break;

  case 745:
#line 5932 "lef.y"
    { ;}
    break;

  case 746:
#line 5935 "lef.y"
    { /* XXXXX for macros */ ;}
    break;

  case 747:
#line 5938 "lef.y"
    {
    if (versionNum < 5.4) {
      if (lefrTimingCbk && lefrTiming.lefiTiming::hasData())
        CALLBACK(lefrTimingCbk, lefrTimingCbkType, &lefrTiming);
      lefrTiming.lefiTiming::clear();
    } else {
      if (lefrTimingCbk) /* write warning only if cbk is set */
        if (timingWarnings++ < lefrTimingWarnings)
          lefWarning(2066, "MACRO TIMING is obsoleted in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
      lefrTiming.lefiTiming::clear();
    }
  ;}
    break;

  case 750:
#line 5958 "lef.y"
    {
    if (versionNum < 5.4) {
      if (lefrTimingCbk && lefrTiming.lefiTiming::hasData())
        CALLBACK(lefrTimingCbk, lefrTimingCbkType, &lefrTiming);
    }
    lefDumbMode = 1000000000;
    lefrTiming.lefiTiming::clear();
    ;}
    break;

  case 751:
#line 5967 "lef.y"
    { lefDumbMode = 0;;}
    break;

  case 752:
#line 5968 "lef.y"
    {lefDumbMode = 1000000000;;}
    break;

  case 753:
#line 5969 "lef.y"
    { lefDumbMode = 0;;}
    break;

  case 754:
#line 5971 "lef.y"
    { if (lefrTimingCbk) lefrTiming.lefiTiming::addRiseFall((yyvsp[-3].string),(yyvsp[-1].dval),(yyvsp[0].dval)); ;}
    break;

  case 755:
#line 5973 "lef.y"
    { if (lefrTimingCbk) lefrTiming.lefiTiming::addRiseFallVariable((yyvsp[-2].dval),(yyvsp[-1].dval)); ;}
    break;

  case 756:
#line 5976 "lef.y"
    { if (lefrTimingCbk) {
	if ((yyvsp[-7].string)[0] == 'D' || (yyvsp[-7].string)[0] == 'd') /* delay */
	  lefrTiming.lefiTiming::addDelay((yyvsp[-8].string), (yyvsp[-5].string), (yyvsp[-3].dval), (yyvsp[-2].dval), (yyvsp[-1].dval));
	else
	  lefrTiming.lefiTiming::addTransition((yyvsp[-8].string), (yyvsp[-5].string), (yyvsp[-3].dval), (yyvsp[-2].dval), (yyvsp[-1].dval));
      }
    ;}
    break;

  case 757:
#line 5984 "lef.y"
    { ;}
    break;

  case 758:
#line 5986 "lef.y"
    { ;}
    break;

  case 759:
#line 5988 "lef.y"
    { if (lefrTimingCbk) lefrTiming.lefiTiming::setRiseRS((yyvsp[-2].dval),(yyvsp[-1].dval)); ;}
    break;

  case 760:
#line 5990 "lef.y"
    { if (lefrTimingCbk) lefrTiming.lefiTiming::setFallRS((yyvsp[-2].dval),(yyvsp[-1].dval)); ;}
    break;

  case 761:
#line 5992 "lef.y"
    { if (lefrTimingCbk) lefrTiming.lefiTiming::setRiseCS((yyvsp[-2].dval),(yyvsp[-1].dval)); ;}
    break;

  case 762:
#line 5994 "lef.y"
    { if (lefrTimingCbk) lefrTiming.lefiTiming::setFallCS((yyvsp[-2].dval),(yyvsp[-1].dval)); ;}
    break;

  case 763:
#line 5996 "lef.y"
    { if (lefrTimingCbk) lefrTiming.lefiTiming::setRiseAtt1((yyvsp[-2].dval),(yyvsp[-1].dval)); ;}
    break;

  case 764:
#line 5998 "lef.y"
    { if (lefrTimingCbk) lefrTiming.lefiTiming::setFallAtt1((yyvsp[-2].dval),(yyvsp[-1].dval)); ;}
    break;

  case 765:
#line 6000 "lef.y"
    { if (lefrTimingCbk) lefrTiming.lefiTiming::setRiseTo((yyvsp[-2].dval),(yyvsp[-1].dval)); ;}
    break;

  case 766:
#line 6002 "lef.y"
    { if (lefrTimingCbk) lefrTiming.lefiTiming::setFallTo((yyvsp[-2].dval),(yyvsp[-1].dval)); ;}
    break;

  case 767:
#line 6004 "lef.y"
    { if (lefrTimingCbk) lefrTiming.lefiTiming::addUnateness((yyvsp[-1].string)); ;}
    break;

  case 768:
#line 6006 "lef.y"
    { if (lefrTimingCbk) lefrTiming.lefiTiming::setStable((yyvsp[-4].dval),(yyvsp[-2].dval),(yyvsp[-1].string)); ;}
    break;

  case 769:
#line 6008 "lef.y"
    { if (lefrTimingCbk) lefrTiming.lefiTiming::addSDF2Pins((yyvsp[-7].string),(yyvsp[-6].string),(yyvsp[-5].string),(yyvsp[-3].dval),(yyvsp[-2].dval),(yyvsp[-1].dval)); ;}
    break;

  case 770:
#line 6010 "lef.y"
    { if (lefrTimingCbk) lefrTiming.lefiTiming::addSDF1Pin((yyvsp[-5].string),(yyvsp[-3].dval),(yyvsp[-2].dval),(yyvsp[-2].dval)); ;}
    break;

  case 771:
#line 6012 "lef.y"
    { if (lefrTimingCbk) lefrTiming.lefiTiming::setSDFcondStart((yyvsp[-1].string)); ;}
    break;

  case 772:
#line 6014 "lef.y"
    { if (lefrTimingCbk) lefrTiming.lefiTiming::setSDFcondEnd((yyvsp[-1].string)); ;}
    break;

  case 773:
#line 6016 "lef.y"
    { if (lefrTimingCbk) lefrTiming.lefiTiming::setSDFcond((yyvsp[-1].string)); ;}
    break;

  case 774:
#line 6018 "lef.y"
    { /* XXXXX */ ;}
    break;

  case 775:
#line 6022 "lef.y"
    { (yyval.string) = (char*)"MPWH";;}
    break;

  case 776:
#line 6024 "lef.y"
    { (yyval.string) = (char*)"MPWL";;}
    break;

  case 777:
#line 6026 "lef.y"
    { (yyval.string) = (char*)"PERIOD";;}
    break;

  case 778:
#line 6030 "lef.y"
    { (yyval.string) = (char*)"SETUP";;}
    break;

  case 779:
#line 6032 "lef.y"
    { (yyval.string) = (char*)"HOLD";;}
    break;

  case 780:
#line 6034 "lef.y"
    { (yyval.string) = (char*)"RECOVERY";;}
    break;

  case 781:
#line 6036 "lef.y"
    { (yyval.string) = (char*)"SKEW";;}
    break;

  case 782:
#line 6040 "lef.y"
    { (yyval.string) = (char*)"ANYEDGE";;}
    break;

  case 783:
#line 6042 "lef.y"
    { (yyval.string) = (char*)"POSEDGE";;}
    break;

  case 784:
#line 6044 "lef.y"
    { (yyval.string) = (char*)"NEGEDGE";;}
    break;

  case 785:
#line 6048 "lef.y"
    { (yyval.string) = (char*)"ANYEDGE";;}
    break;

  case 786:
#line 6050 "lef.y"
    { (yyval.string) = (char*)"POSEDGE";;}
    break;

  case 787:
#line 6052 "lef.y"
    { (yyval.string) = (char*)"NEGEDGE";;}
    break;

  case 788:
#line 6056 "lef.y"
    { (yyval.string) = (char*)"DELAY"; ;}
    break;

  case 789:
#line 6058 "lef.y"
    { (yyval.string) = (char*)"TRANSITION"; ;}
    break;

  case 790:
#line 6062 "lef.y"
    { ;}
    break;

  case 791:
#line 6064 "lef.y"
    { ;}
    break;

  case 792:
#line 6067 "lef.y"
    { if (lefrTimingCbk) lefrTiming.lefiTiming::addTableEntry((yyvsp[-3].dval),(yyvsp[-2].dval),(yyvsp[-1].dval)); ;}
    break;

  case 793:
#line 6071 "lef.y"
    { if (lefrTimingCbk) lefrTiming.lefiTiming::addTableAxisNumber((yyvsp[0].dval)); ;}
    break;

  case 794:
#line 6073 "lef.y"
    { if (lefrTimingCbk) lefrTiming.lefiTiming::addTableAxisNumber((yyvsp[0].dval)); ;}
    break;

  case 795:
#line 6077 "lef.y"
    { ;}
    break;

  case 796:
#line 6079 "lef.y"
    { if (lefrTimingCbk) lefrTiming.lefiTiming::addRiseFallSlew((yyvsp[-3].dval),(yyvsp[-2].dval),(yyvsp[-1].dval),(yyvsp[0].dval)); ;}
    break;

  case 797:
#line 6081 "lef.y"
    { if (lefrTimingCbk) lefrTiming.lefiTiming::addRiseFallSlew((yyvsp[-6].dval),(yyvsp[-5].dval),(yyvsp[-4].dval),(yyvsp[-3].dval));
      if (lefrTimingCbk) lefrTiming.lefiTiming::addRiseFallSlew2((yyvsp[-2].dval),(yyvsp[-1].dval),(yyvsp[0].dval)); ;}
    break;

  case 798:
#line 6086 "lef.y"
    { (yyval.string) = (char*)"RISE"; ;}
    break;

  case 799:
#line 6088 "lef.y"
    { (yyval.string) = (char*)"FALL"; ;}
    break;

  case 800:
#line 6092 "lef.y"
    { (yyval.string) = (char*)"INVERT"; ;}
    break;

  case 801:
#line 6094 "lef.y"
    { (yyval.string) = (char*)"NONINVERT"; ;}
    break;

  case 802:
#line 6096 "lef.y"
    { (yyval.string) = (char*)"NONUNATE"; ;}
    break;

  case 803:
#line 6100 "lef.y"
    { if (lefrTimingCbk) lefrTiming.lefiTiming::addFromPin((yyvsp[0].string)); ;}
    break;

  case 804:
#line 6102 "lef.y"
    { if (lefrTimingCbk) lefrTiming.lefiTiming::addFromPin((yyvsp[0].string)); ;}
    break;

  case 805:
#line 6106 "lef.y"
    { if (lefrTimingCbk) lefrTiming.lefiTiming::addToPin((yyvsp[0].string)); ;}
    break;

  case 806:
#line 6108 "lef.y"
    { if (lefrTimingCbk) lefrTiming.lefiTiming::addToPin((yyvsp[0].string)); ;}
    break;

  case 807:
#line 6111 "lef.y"
    {
      if (lefrArrayCbk)
	CALLBACK(lefrArrayCbk, lefrArrayCbkType, &lefrArray);
      lefrArray.lefiArray::clear();
      lefrSitePatternPtr = 0;
      lefrDoSite = 0;
   ;}
    break;

  case 809:
#line 6120 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1;;}
    break;

  case 810:
#line 6121 "lef.y"
    {
      if (lefrArrayCbk) {
	lefrArray.lefiArray::setName((yyvsp[0].string));
	CALLBACK(lefrArrayBeginCbk, lefrArrayBeginCbkType, (yyvsp[0].string));
      }
      //strcpy(arrayName, $3);
      arrayName = strdup((yyvsp[0].string));
    ;}
    break;

  case 811:
#line 6130 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1;;}
    break;

  case 812:
#line 6131 "lef.y"
    {
      if (lefrArrayCbk && lefrArrayEndCbk)
	CALLBACK(lefrArrayEndCbk, lefrArrayEndCbkType, (yyvsp[0].string));
      if (strcmp(arrayName, (yyvsp[0].string)) != 0) {
        if (lefrArrayCbk) { /* write error only if cbk is set */
           if (arrayWarnings++ < lefrArrayWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                 "END ARRAY name %s is different from the ARRAY name %s.\nCorrect the LEF file before rerun it through the LEF parser.", (yyvsp[0].string), arrayName);
              lefError(1662, outMsg);
              lefFree(outMsg);
              lefFree(arrayName);
              CHKERR();
           } else
              lefFree(arrayName);
        } else
           lefFree(arrayName);
      } else
        lefFree(arrayName);
    ;}
    break;

  case 813:
#line 6154 "lef.y"
    { ;}
    break;

  case 814:
#line 6156 "lef.y"
    { ;}
    break;

  case 815:
#line 6159 "lef.y"
    { if (lefrArrayCbk) lefrDoSite = 1; lefDumbMode = 1; ;}
    break;

  case 816:
#line 6161 "lef.y"
    {
      if (lefrArrayCbk) {
	lefrArray.lefiArray::addSitePattern(lefrSitePatternPtr);
      }
    ;}
    break;

  case 817:
#line 6166 "lef.y"
    {lefDumbMode = 1; if (lefrArrayCbk) lefrDoSite = 1; ;}
    break;

  case 818:
#line 6168 "lef.y"
    {
      if (lefrArrayCbk) {
	lefrArray.lefiArray::addCanPlace(lefrSitePatternPtr);
      }
    ;}
    break;

  case 819:
#line 6173 "lef.y"
    {lefDumbMode = 1; if (lefrArrayCbk) lefrDoSite = 1; ;}
    break;

  case 820:
#line 6175 "lef.y"
    {
      if (lefrArrayCbk) {
	lefrArray.lefiArray::addCannotOccupy(lefrSitePatternPtr);
      }
    ;}
    break;

  case 821:
#line 6180 "lef.y"
    { if (lefrArrayCbk) lefrDoTrack = 1; ;}
    break;

  case 822:
#line 6181 "lef.y"
    {
      if (lefrArrayCbk) {
	lefrArray.lefiArray::addTrack(lefrTrackPatternPtr);
      }
    ;}
    break;

  case 823:
#line 6187 "lef.y"
    {
    ;}
    break;

  case 824:
#line 6189 "lef.y"
    { if (lefrArrayCbk) lefrDoGcell = 1; ;}
    break;

  case 825:
#line 6190 "lef.y"
    {
      if (lefrArrayCbk) {
	lefrArray.lefiArray::addGcell(lefrGcellPatternPtr);
      }
    ;}
    break;

  case 826:
#line 6196 "lef.y"
    {
      if (lefrArrayCbk) {
	lefrArray.lefiArray::setTableSize((int)(yyvsp[-3].dval));
      }
    ;}
    break;

  case 827:
#line 6202 "lef.y"
    { ;}
    break;

  case 828:
#line 6205 "lef.y"
    { if (lefrArrayCbk) lefrArray.lefiArray::addFloorPlan((yyvsp[0].string)); ;}
    break;

  case 829:
#line 6209 "lef.y"
    { ;}
    break;

  case 830:
#line 6211 "lef.y"
    { ;}
    break;

  case 831:
#line 6214 "lef.y"
    { lefDumbMode = 1; if (lefrArrayCbk) lefrDoSite = 1; ;}
    break;

  case 832:
#line 6216 "lef.y"
    {
      if (lefrArrayCbk)
	lefrArray.lefiArray::addSiteToFloorPlan("CANPLACE",
	lefrSitePatternPtr);
    ;}
    break;

  case 833:
#line 6221 "lef.y"
    { if (lefrArrayCbk) lefrDoSite = 1; lefDumbMode = 1; ;}
    break;

  case 834:
#line 6223 "lef.y"
    {
      if (lefrArrayCbk)
	lefrArray.lefiArray::addSiteToFloorPlan("CANNOTOCCUPY",
	lefrSitePatternPtr);
     ;}
    break;

  case 835:
#line 6231 "lef.y"
    { ;}
    break;

  case 836:
#line 6233 "lef.y"
    { ;}
    break;

  case 837:
#line 6236 "lef.y"
    { if (lefrArrayCbk) lefrArray.lefiArray::addDefaultCap((int)(yyvsp[-3].dval), (yyvsp[-1].dval)); ;}
    break;

  case 838:
#line 6239 "lef.y"
    {lefDumbMode=1;lefNlToken=TRUE;;}
    break;

  case 839:
#line 6240 "lef.y"
    { lefAddStringMessage((yyvsp[-3].string), (yyvsp[-1].string)); ;}
    break;

  case 840:
#line 6243 "lef.y"
    {lefDumbMode=1;lefNlToken=TRUE;;}
    break;

  case 841:
#line 6244 "lef.y"
    { ;}
    break;

  case 842:
#line 6247 "lef.y"
    {lefDumbMode=1;lefNlToken=TRUE;;}
    break;

  case 843:
#line 6248 "lef.y"
    {
      if (versionNum < 5.6)
        lefAddNumDefine((yyvsp[-3].string), (yyvsp[-1].dval));
      else
        if (lefrArrayCbk) /* write warning only if cbk is set */
           if (arrayWarnings++ < lefrArrayWarnings)
             lefWarning(2067, "DEFINE is obsoleted in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
    ;}
    break;

  case 844:
#line 6256 "lef.y"
    {lefDumbMode=1;lefNlToken=TRUE;;}
    break;

  case 845:
#line 6257 "lef.y"
    {
      if (versionNum < 5.6)
        lefAddStringDefine((yyvsp[-3].string), (yyvsp[-1].string));
      else
        if (lefrArrayCbk) /* write warning only if cbk is set */
           if (arrayWarnings++ < lefrArrayWarnings)
             lefWarning(2068, "DEFINES is obsoleted in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
    ;}
    break;

  case 846:
#line 6265 "lef.y"
    {lefDumbMode=1;lefNlToken=TRUE;;}
    break;

  case 847:
#line 6266 "lef.y"
    {
      if (versionNum < 5.6)
        lefAddBooleanDefine((yyvsp[-3].string), (yyvsp[-1].integer));
      else
        if (lefrArrayCbk) /* write warning only if cbk is set */
           if (arrayWarnings++ < lefrArrayWarnings)
             lefWarning(2069, "DEFINEB is obsoleted in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
    ;}
    break;

  case 849:
#line 6277 "lef.y"
    {lefNlToken = FALSE;;}
    break;

  case 850:
#line 6278 "lef.y"
    {lefNlToken = FALSE;;}
    break;

  case 855:
#line 6291 "lef.y"
    {(yyval.dval) = (yyvsp[-2].dval) + (yyvsp[0].dval); ;}
    break;

  case 856:
#line 6292 "lef.y"
    {(yyval.dval) = (yyvsp[-2].dval) - (yyvsp[0].dval); ;}
    break;

  case 857:
#line 6293 "lef.y"
    {(yyval.dval) = (yyvsp[-2].dval) * (yyvsp[0].dval); ;}
    break;

  case 858:
#line 6294 "lef.y"
    {(yyval.dval) = (yyvsp[-2].dval) / (yyvsp[0].dval); ;}
    break;

  case 859:
#line 6295 "lef.y"
    {(yyval.dval) = -(yyvsp[0].dval);;}
    break;

  case 860:
#line 6296 "lef.y"
    {(yyval.dval) = (yyvsp[-1].dval);;}
    break;

  case 861:
#line 6298 "lef.y"
    {(yyval.dval) = ((yyvsp[-4].integer) != 0) ? (yyvsp[-2].dval) : (yyvsp[0].dval);;}
    break;

  case 862:
#line 6299 "lef.y"
    {(yyval.dval) = (yyvsp[0].dval);;}
    break;

  case 863:
#line 6302 "lef.y"
    {(yyval.integer) = comp_num((yyvsp[-2].dval),(yyvsp[-1].integer),(yyvsp[0].dval));;}
    break;

  case 864:
#line 6303 "lef.y"
    {(yyval.integer) = (yyvsp[-2].dval) != 0 && (yyvsp[0].dval) != 0;;}
    break;

  case 865:
#line 6304 "lef.y"
    {(yyval.integer) = (yyvsp[-2].dval) != 0 || (yyvsp[0].dval) != 0;;}
    break;

  case 866:
#line 6305 "lef.y"
    {(yyval.integer) = comp_str((yyvsp[-2].string),(yyvsp[-1].integer),(yyvsp[0].string));;}
    break;

  case 867:
#line 6306 "lef.y"
    {(yyval.integer) = (yyvsp[-2].string)[0] != 0 && (yyvsp[0].string)[0] != 0;;}
    break;

  case 868:
#line 6307 "lef.y"
    {(yyval.integer) = (yyvsp[-2].string)[0] != 0 || (yyvsp[0].string)[0] != 0;;}
    break;

  case 869:
#line 6308 "lef.y"
    {(yyval.integer) = (yyvsp[-2].integer) == (yyvsp[0].integer);;}
    break;

  case 870:
#line 6309 "lef.y"
    {(yyval.integer) = (yyvsp[-2].integer) != (yyvsp[0].integer);;}
    break;

  case 871:
#line 6310 "lef.y"
    {(yyval.integer) = (yyvsp[-2].integer) && (yyvsp[0].integer);;}
    break;

  case 872:
#line 6311 "lef.y"
    {(yyval.integer) = (yyvsp[-2].integer) || (yyvsp[0].integer);;}
    break;

  case 873:
#line 6312 "lef.y"
    {(yyval.integer) = !(yyval.integer);;}
    break;

  case 874:
#line 6313 "lef.y"
    {(yyval.integer) = (yyvsp[-1].integer);;}
    break;

  case 875:
#line 6315 "lef.y"
    {(yyval.integer) = ((yyvsp[-4].integer) != 0) ? (yyvsp[-2].integer) : (yyvsp[0].integer);;}
    break;

  case 876:
#line 6316 "lef.y"
    {(yyval.integer) = 1;;}
    break;

  case 877:
#line 6317 "lef.y"
    {(yyval.integer) = 0;;}
    break;

  case 878:
#line 6321 "lef.y"
    {
      (yyval.string) = (char*)lefMalloc(strlen((yyvsp[-2].string))+strlen((yyvsp[0].string))+1);
      strcpy((yyval.string),(yyvsp[-2].string));
      strcat((yyval.string),(yyvsp[0].string));
    ;}
    break;

  case 879:
#line 6327 "lef.y"
    { (yyval.string) = (yyvsp[-1].string); ;}
    break;

  case 880:
#line 6329 "lef.y"
    {
      lefDefIf = TRUE;
      if ((yyvsp[-4].integer) != 0) {
	(yyval.string) = (yyvsp[-2].string);	
      } else {
	(yyval.string) = (yyvsp[0].string);
      }
    ;}
    break;

  case 881:
#line 6338 "lef.y"
    { (yyval.string) = (yyvsp[0].string); ;}
    break;

  case 882:
#line 6341 "lef.y"
    {(yyval.integer) = C_LE;;}
    break;

  case 883:
#line 6342 "lef.y"
    {(yyval.integer) = C_LT;;}
    break;

  case 884:
#line 6343 "lef.y"
    {(yyval.integer) = C_GE;;}
    break;

  case 885:
#line 6344 "lef.y"
    {(yyval.integer) = C_GT;;}
    break;

  case 886:
#line 6345 "lef.y"
    {(yyval.integer) = C_EQ;;}
    break;

  case 887:
#line 6346 "lef.y"
    {(yyval.integer) = C_NE;;}
    break;

  case 888:
#line 6347 "lef.y"
    {(yyval.integer) = C_EQ;;}
    break;

  case 889:
#line 6348 "lef.y"
    {(yyval.integer) = C_LT;;}
    break;

  case 890:
#line 6349 "lef.y"
    {(yyval.integer) = C_GT;;}
    break;

  case 891:
#line 6353 "lef.y"
    { 
      if (lefrPropBeginCbk)
        CALLBACK(lefrPropBeginCbk, lefrPropBeginCbkType, 0);
      lefInPropDef = 1;  /* set flag as inside propertydefinitions */
    ;}
    break;

  case 892:
#line 6359 "lef.y"
    { 
      if (lefrPropEndCbk)
        CALLBACK(lefrPropEndCbk, lefrPropEndCbkType, 0);
      lefRealNum = 0;     /* just want to make sure it is reset */
      lefInPropDef = 0;   /* reset flag */
    ;}
    break;

  case 893:
#line 6368 "lef.y"
    { ;}
    break;

  case 894:
#line 6370 "lef.y"
    { ;}
    break;

  case 895:
#line 6373 "lef.y"
    {lefDumbMode = 1; lefrProp.lefiProp::clear(); ;}
    break;

  case 896:
#line 6375 "lef.y"
    { 
      if (lefrPropCbk) {
        lefrProp.lefiProp::setPropType("library", (yyvsp[-2].string));
        CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
      }
      lefrLibProp.lefiPropType::setPropType((yyvsp[-2].string), lefPropDefType);
    ;}
    break;

  case 897:
#line 6382 "lef.y"
    {lefDumbMode = 1; lefrProp.lefiProp::clear(); ;}
    break;

  case 898:
#line 6384 "lef.y"
    { 
      if (lefrPropCbk) {
        lefrProp.lefiProp::setPropType("componentpin", (yyvsp[-2].string));
        CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
      }
      lefrCompProp.lefiPropType::setPropType((yyvsp[-2].string), lefPropDefType);
    ;}
    break;

  case 899:
#line 6391 "lef.y"
    {lefDumbMode = 1; lefrProp.lefiProp::clear(); ;}
    break;

  case 900:
#line 6393 "lef.y"
    { 
      if (lefrPropCbk) {
        lefrProp.lefiProp::setPropType("pin", (yyvsp[-2].string));
        CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
      }
      lefrPinProp.lefiPropType::setPropType((yyvsp[-2].string), lefPropDefType);
      
    ;}
    break;

  case 901:
#line 6401 "lef.y"
    {lefDumbMode = 1; lefrProp.lefiProp::clear(); ;}
    break;

  case 902:
#line 6403 "lef.y"
    { 
      if (lefrPropCbk) {
        lefrProp.lefiProp::setPropType("macro", (yyvsp[-2].string));
        CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
      }
      lefrMacroProp.lefiPropType::setPropType((yyvsp[-2].string), lefPropDefType);
    ;}
    break;

  case 903:
#line 6410 "lef.y"
    {lefDumbMode = 1; lefrProp.lefiProp::clear(); ;}
    break;

  case 904:
#line 6412 "lef.y"
    { 
      if (lefrPropCbk) {
        lefrProp.lefiProp::setPropType("via", (yyvsp[-2].string));
        CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
      }
      lefrViaProp.lefiPropType::setPropType((yyvsp[-2].string), lefPropDefType);
    ;}
    break;

  case 905:
#line 6419 "lef.y"
    {lefDumbMode = 1; lefrProp.lefiProp::clear(); ;}
    break;

  case 906:
#line 6421 "lef.y"
    { 
      if (lefrPropCbk) {
        lefrProp.lefiProp::setPropType("viarule", (yyvsp[-2].string));
        CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
      }
      lefrViaRuleProp.lefiPropType::setPropType((yyvsp[-2].string), lefPropDefType);
    ;}
    break;

  case 907:
#line 6428 "lef.y"
    {lefDumbMode = 1; lefrProp.lefiProp::clear(); ;}
    break;

  case 908:
#line 6430 "lef.y"
    { 
      if (lefrPropCbk) {
        lefrProp.lefiProp::setPropType("layer", (yyvsp[-2].string));
        CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
      }
      lefrLayerProp.lefiPropType::setPropType((yyvsp[-2].string), lefPropDefType);
    ;}
    break;

  case 909:
#line 6437 "lef.y"
    {lefDumbMode = 1; lefrProp.lefiProp::clear(); ;}
    break;

  case 910:
#line 6439 "lef.y"
    { 
      if (lefrPropCbk) {
        lefrProp.lefiProp::setPropType("nondefaultrule", (yyvsp[-2].string));
        CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
      }
      lefrNondefProp.lefiPropType::setPropType((yyvsp[-2].string), lefPropDefType);
    ;}
    break;

  case 911:
#line 6448 "lef.y"
    { lefRealNum = 0 ;}
    break;

  case 912:
#line 6449 "lef.y"
    { 
      if (lefrPropCbk) lefrProp.lefiProp::setPropInteger();
      lefPropDefType = 'I'
    ;}
    break;

  case 913:
#line 6453 "lef.y"
    { lefRealNum = 1 ;}
    break;

  case 914:
#line 6454 "lef.y"
    { 
      if (lefrPropCbk) lefrProp.lefiProp::setPropReal();
      lefPropDefType = 'R';
      lefRealNum = 0;
    ;}
    break;

  case 915:
#line 6460 "lef.y"
    {
      if (lefrPropCbk) lefrProp.lefiProp::setPropString();
      lefPropDefType = 'S'
    ;}
    break;

  case 916:
#line 6465 "lef.y"
    {
      if (lefrPropCbk) lefrProp.lefiProp::setPropQString((yyvsp[0].string));
      lefPropDefType = 'Q'
    ;}
    break;

  case 917:
#line 6470 "lef.y"
    {
      if (lefrPropCbk) lefrProp.lefiProp::setPropNameMapString((yyvsp[0].string));
      lefPropDefType = 'S'
    ;}
    break;

  case 918:
#line 6477 "lef.y"
    { ;}
    break;

  case 919:
#line 6479 "lef.y"
    {
      if (lefrLayerCbk)
        lefrLayer.lefiLayer::setSpacingRangeUseLength();
    ;}
    break;

  case 920:
#line 6484 "lef.y"
    {
      if (lefrLayerCbk) {
        lefrLayer.lefiLayer::setSpacingRangeInfluence((yyvsp[0].dval));
        lefrLayer.lefiLayer::setSpacingRangeInfluenceRange(-1, -1);
      }
    ;}
    break;

  case 921:
#line 6491 "lef.y"
    {
      if (lefrLayerCbk) {
        lefrLayer.lefiLayer::setSpacingRangeInfluence((yyvsp[-3].dval));
        lefrLayer.lefiLayer::setSpacingRangeInfluenceRange((yyvsp[-1].dval), (yyvsp[0].dval));
      }
    ;}
    break;

  case 922:
#line 6498 "lef.y"
    {
      if (lefrLayerCbk)
        lefrLayer.lefiLayer::setSpacingRangeRange((yyvsp[-1].dval), (yyvsp[0].dval));
    ;}
    break;

  case 923:
#line 6505 "lef.y"
    { ;}
    break;

  case 924:
#line 6507 "lef.y"
    {
      if (lefrLayerCbk)
        lefrLayer.lefiLayer::setSpacingParSW((yyvsp[-2].dval), (yyvsp[0].dval));
    ;}
    break;

  case 926:
#line 6515 "lef.y"
    { ;}
    break;

  case 927:
#line 6517 "lef.y"
    {
      if (lefrLayerCbk)
        lefrLayer.lefiLayer::setSpacingParTwoEdges();
    ;}
    break;

  case 928:
#line 6524 "lef.y"
    { ;}
    break;

  case 929:
#line 6526 "lef.y"
    {
      if (lefrLayerCbk)
        lefrLayer.lefiLayer::setSpacingSamenetPGonly();
    ;}
    break;

  case 930:
#line 6533 "lef.y"
    { ;}
    break;

  case 931:
#line 6535 "lef.y"
    {  if (lefrPropCbk) lefrProp.lefiProp::setRange((yyvsp[-1].dval), (yyvsp[0].dval)); ;}
    break;

  case 932:
#line 6539 "lef.y"
    { ;}
    break;

  case 933:
#line 6541 "lef.y"
    { if (lefrPropCbk) lefrProp.lefiProp::setNumber((yyvsp[0].dval)); ;}
    break;

  case 936:
#line 6548 "lef.y"
    {
      if (lefrLayerCbk) {
         if (hasSpCenter) {
           if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1663, "A CENTERTOCENTER statement has already defined in SPACING\nCENTERTOCENTER can only be defined once per LAYER CUT SPACING.");
              CHKERR();
           }
        }
        hasSpCenter = 1;
        if (versionNum < 5.6) {
           if (layerWarnings++ < lefrLayerWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                 "CENTERTOCENTER statement is a version 5.6 and later syntax.\nYour lef file is defined with version %g.", versionNum);
              lefError(1664, outMsg);
              lefFree(outMsg);
              CHKERR();
           }
        }
        if (lefrLayerCbk)
          lefrLayer.lefiLayer::setSpacingCenterToCenter();
      }
    ;}
    break;

  case 937:
#line 6572 "lef.y"
    {
      if (lefrLayerCbk) {
        if (hasSpSamenet) {
           if (layerWarnings++ < lefrLayerWarnings) {
              lefError(1665, "A SAMENET statement has already defined in SPACING\nSAMENET can only be defined once per LAYER CUT SPACING.");
              CHKERR();
           }
        }
        hasSpSamenet = 1;
        if (lefrLayerCbk)
          lefrLayer.lefiLayer::setSpacingSamenet();
       }
    ;}
    break;

  case 938:
#line 6586 "lef.y"
    {
      if (versionNum < 5.7) {
        outMsg = (char*)lefMalloc(10000);
        sprintf(outMsg,
          "SAMENET is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
        lefError(1684, outMsg);
        lefFree(outMsg);
        CHKERR();
      }
    ;}
    break;

  case 939:
#line 6597 "lef.y"
    {
      if (versionNum < 5.7) {
        outMsg = (char*)lefMalloc(10000);
        sprintf(outMsg,
          "PARALLELOVERLAP is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
        lefError(1680, outMsg);
        lefFree(outMsg);
        CHKERR(); 
      } else {
        if (lefrLayerCbk) {
          if (hasSpParallel) {
             if (layerWarnings++ < lefrLayerWarnings) {
                lefError(1666, "A PARALLELOVERLAP statement has already defined in SPACING\nPARALLELOVERLAP can only be defined once per LAYER CUT SPACING.");
                CHKERR();
             }
          }
          hasSpParallel = 1;
          if (lefrLayerCbk)
            lefrLayer.lefiLayer::setSpacingParallelOverlap();
        }
      }
    ;}
    break;

  case 941:
#line 6622 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 942:
#line 6623 "lef.y"
    {
      if (lefrLayerCbk)
{
        if (versionNum < 5.7) {
           if (hasSpSamenet) {    /* 5.6 and earlier does not allow */
              if (layerWarnings++ < lefrLayerWarnings) {
                 lefError(1667, "A SAMENET statement has already defined in SPACING\nEither SAMENET or LAYER can be defined, but not both.");
                 CHKERR();
              }
           }
        }
        lefrLayer.lefiLayer::setSpacingName((yyvsp[0].string));
      }
    ;}
    break;

  case 944:
#line 6639 "lef.y"
    {
      if (lefrLayerCbk) {
        if (versionNum < 5.5) {
           if (layerWarnings++ < lefrLayerWarnings) {
              outMsg = (char*)lefMalloc(10000);
              sprintf (outMsg,
                 "ADJACENTCUTS statement is a version 5.5 and later syntax.\nYour lef file is defined with version %g.", versionNum);
              lefError(1668, outMsg);
              lefFree(outMsg);
              CHKERR();
           }
        }
        if (versionNum < 5.7) {
           if (hasSpSamenet) {    /* 5.6 and earlier does not allow */
              if (layerWarnings++ < lefrLayerWarnings) {
                 lefError(1669, "A SAMENET statement has already defined in SPACING\nEither SAMENET or ADJACENTCUTS can be defined, but not both.");
                 CHKERR();
              }
           }
        }
        lefrLayer.lefiLayer::setSpacingAdjacent((int)(yyvsp[-2].dval), (yyvsp[0].dval));
      }
    ;}
    break;

  case 946:
#line 6664 "lef.y"
    {
      if (versionNum < 5.7) {
        outMsg = (char*)lefMalloc(10000);
        sprintf(outMsg,
          "AREA is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
        lefError(1693, outMsg);
        lefFree(outMsg);
        CHKERR();
      } else {
        if (lefrLayerCbk) {
          if (versionNum < 5.7) {
             if (hasSpSamenet) {    /* 5.6 and earlier does not allow */
                if (layerWarnings++ < lefrLayerWarnings) {
                   lefError(1670, "A SAMENET statement has already defined in SPACING\nEither SAMENET or AREA can be defined, but not both.");
                   CHKERR();
                }
             }
          }
          lefrLayer.lefiLayer::setSpacingArea((yyvsp[0].dval));
        }
      }
    ;}
    break;

  case 947:
#line 6687 "lef.y"
    {
      if (lefrLayerCbk)
        lefrLayer.lefiLayer::setSpacingRange((yyvsp[-1].dval), (yyvsp[0].dval));
    ;}
    break;

  case 949:
#line 6693 "lef.y"
    {
      if (lefrLayerCbk) {
        lefrLayer.lefiLayer::setSpacingLength((yyvsp[0].dval));
      }
    ;}
    break;

  case 950:
#line 6699 "lef.y"
    {
      if (lefrLayerCbk) {
        lefrLayer.lefiLayer::setSpacingLength((yyvsp[-3].dval));
        lefrLayer.lefiLayer::setSpacingLengthRange((yyvsp[-1].dval), (yyvsp[0].dval));
      }
    ;}
    break;

  case 951:
#line 6706 "lef.y"
    {
      if (lefrLayerCbk)
        lefrLayer.lefiLayer::setSpacingEol((yyvsp[-2].dval), (yyvsp[0].dval));
    ;}
    break;

  case 952:
#line 6711 "lef.y"
    {
      if (versionNum < 5.7) {
        outMsg = (char*)lefMalloc(10000);
        sprintf(outMsg,
          "ENDOFLINE is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
        lefError(1681, outMsg);
        lefFree(outMsg);
        CHKERR();
      }
    ;}
    break;

  case 953:
#line 6722 "lef.y"
    {
      if (versionNum < 5.7) {
        outMsg = (char*)lefMalloc(10000);
        sprintf(outMsg,
          "NOTCHLENGTH is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
        lefError(1682, outMsg);
        lefFree(outMsg);
        CHKERR();
      } else {
        if (lefrLayerCbk)
          lefrLayer.lefiLayer::setSpacingNotchLength((yyvsp[0].dval));
      }
    ;}
    break;

  case 954:
#line 6736 "lef.y"
    {
      if (versionNum < 5.7) {
        outMsg = (char*)lefMalloc(10000);
        sprintf(outMsg,
          "ENDOFNOTCHWIDTH is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
        lefError(1696, outMsg);
        lefFree(outMsg);
        CHKERR();
      } else {
        if (lefrLayerCbk)
          lefrLayer.lefiLayer::setSpacingEndOfNotchWidth((yyvsp[-4].dval), (yyvsp[-2].dval), (yyvsp[0].dval));
      }
    ;}
    break;

  case 955:
#line 6752 "lef.y"
    {;}
    break;

  case 956:
#line 6754 "lef.y"
    {
      if (lefrLayerCbk)
        lefrLayer.lefiLayer::setSpacingLayerStack();
    ;}
    break;

  case 957:
#line 6761 "lef.y"
    {;}
    break;

  case 958:
#line 6763 "lef.y"
    {
      if (versionNum < 5.7) {
        outMsg = (char*)lefMalloc(10000);
        sprintf(outMsg,
          "EXCEPTSAMEPGNET is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
        lefError(1683, outMsg);
        lefFree(outMsg);
        CHKERR();
      } else {
        if (lefrLayerCbk)
          lefrLayer.lefiLayer::setSpacingAdjacentExcept();
      }
    ;}
    break;

  case 959:
#line 6779 "lef.y"
    { (yyval.string) = 0; ;}
    break;

  case 960:
#line 6780 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 961:
#line 6781 "lef.y"
    { (yyval.string) = (yyvsp[0].string); ;}
    break;

  case 962:
#line 6785 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 963:
#line 6786 "lef.y"
    { (yyval.string) = (yyvsp[0].string); ;}
    break;

  case 964:
#line 6790 "lef.y"
    {
      if (versionNum < 5.4) {
        if (lefrNoiseMarginCbk) {
          lefrNoiseMargin.low = (yyvsp[-2].dval);
          lefrNoiseMargin.high = (yyvsp[-1].dval);
          CALLBACK(lefrNoiseMarginCbk, lefrNoiseMarginCbkType, &lefrNoiseMargin);
        }
      } else
        if (lefrNoiseMarginCbk) /* write warning only if cbk is set */
          if (noiseMarginWarnings++ < lefrNoiseMarginWarnings)
            lefWarning(2070, "UNIVERSALNOISEMARGIN is obsoleted in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
    ;}
    break;

  case 965:
#line 6804 "lef.y"
    {
      if (versionNum < 5.4) {
        if (lefrEdgeRateThreshold1Cbk) {
          CALLBACK(lefrEdgeRateThreshold1Cbk,
          lefrEdgeRateThreshold1CbkType, (yyvsp[-1].dval));
        }
      } else
        if (lefrEdgeRateThreshold1Cbk) /* write warning only if cbk is set */
          if (edgeRateThreshold1Warnings++ < lefrEdgeRateThreshold1Warnings)
            lefWarning(2071, "EDGERATETHRESHOLD1 is obsoleted in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
    ;}
    break;

  case 966:
#line 6817 "lef.y"
    {
      if (versionNum < 5.4) {
        if (lefrEdgeRateThreshold2Cbk) {
	  CALLBACK(lefrEdgeRateThreshold2Cbk,
	  lefrEdgeRateThreshold2CbkType, (yyvsp[-1].dval));
        }
      } else
        if (lefrEdgeRateThreshold2Cbk) /* write warning only if cbk is set */
          if (edgeRateThreshold2Warnings++ < lefrEdgeRateThreshold2Warnings)
            lefWarning(2072, "EDGERATETHRESHOLD2 is obsoleted in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
    ;}
    break;

  case 967:
#line 6830 "lef.y"
    {
      if (versionNum < 5.4) {
        if (lefrEdgeRateScaleFactorCbk) {
	  CALLBACK(lefrEdgeRateScaleFactorCbk,
	  lefrEdgeRateScaleFactorCbkType, (yyvsp[-1].dval));
        }
      } else
        if (lefrEdgeRateScaleFactorCbk) /* write warning only if cbk is set */
          if (edgeRateScaleFactorWarnings++ < lefrEdgeRateScaleFactorWarnings)
            lefWarning(2073, "EDGERATESCALEFACTOR is obsoleted in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
    ;}
    break;

  case 968:
#line 6843 "lef.y"
    { if (lefrNoiseTableCbk) lefrNoiseTable.lefiNoiseTable::setup((int)(yyvsp[0].dval)); ;}
    break;

  case 969:
#line 6845 "lef.y"
    { ;}
    break;

  case 970:
#line 6849 "lef.y"
    {
    if (versionNum < 5.4) {
      if (lefrNoiseTableCbk)
        CALLBACK(lefrNoiseTableCbk, lefrNoiseTableCbkType, &lefrNoiseTable);
    } else
      if (lefrNoiseTableCbk) /* write warning only if cbk is set */
        if (noiseTableWarnings++ < lefrNoiseTableWarnings)
          lefWarning(2074, "NOISETABLE is obsoleted in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
  ;}
    break;

  case 973:
#line 6867 "lef.y"
    { if (lefrNoiseTableCbk)
         {
            lefrNoiseTable.lefiNoiseTable::newEdge();
            lefrNoiseTable.lefiNoiseTable::addEdge((yyvsp[-1].dval));
         }
    ;}
    break;

  case 974:
#line 6874 "lef.y"
    { ;}
    break;

  case 975:
#line 6877 "lef.y"
    { if (lefrNoiseTableCbk) lefrNoiseTable.lefiNoiseTable::addResistance(); ;}
    break;

  case 977:
#line 6883 "lef.y"
    { if (lefrNoiseTableCbk)
    lefrNoiseTable.lefiNoiseTable::addResistanceNumber((yyvsp[0].dval)); ;}
    break;

  case 978:
#line 6886 "lef.y"
    { if (lefrNoiseTableCbk)
    lefrNoiseTable.lefiNoiseTable::addResistanceNumber((yyvsp[0].dval)); ;}
    break;

  case 981:
#line 6895 "lef.y"
    { if (lefrNoiseTableCbk)
	lefrNoiseTable.lefiNoiseTable::addVictimLength((yyvsp[-1].dval)); ;}
    break;

  case 982:
#line 6898 "lef.y"
    { ;}
    break;

  case 983:
#line 6902 "lef.y"
    { if (lefrNoiseTableCbk)
    lefrNoiseTable.lefiNoiseTable::addVictimNoise((yyvsp[0].dval)); ;}
    break;

  case 984:
#line 6905 "lef.y"
    { if (lefrNoiseTableCbk)
    lefrNoiseTable.lefiNoiseTable::addVictimNoise((yyvsp[0].dval)); ;}
    break;

  case 985:
#line 6909 "lef.y"
    { if (lefrCorrectionTableCbk)
    lefrCorrectionTable.lefiCorrectionTable::setup((int)(yyvsp[-1].dval)); ;}
    break;

  case 986:
#line 6912 "lef.y"
    { ;}
    break;

  case 987:
#line 6916 "lef.y"
    {
    if (versionNum < 5.4) {
      if (lefrCorrectionTableCbk)
        CALLBACK(lefrCorrectionTableCbk, lefrCorrectionTableCbkType,
               &lefrCorrectionTable);
    } else
      if (lefrCorrectionTableCbk) /* write warning only if cbk is set */
        if (correctionTableWarnings++ < lefrCorrectionTableWarnings)
          lefWarning(2075, "CORRECTIONTABLE is obsoleted in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
  ;}
    break;

  case 990:
#line 6934 "lef.y"
    { if (lefrCorrectionTableCbk)
         {
            lefrCorrectionTable.lefiCorrectionTable::newEdge();
            lefrCorrectionTable.lefiCorrectionTable::addEdge((yyvsp[-1].dval));
         }
    ;}
    break;

  case 991:
#line 6941 "lef.y"
    { ;}
    break;

  case 992:
#line 6944 "lef.y"
    { if (lefrCorrectionTableCbk)
  lefrCorrectionTable.lefiCorrectionTable::addResistance(); ;}
    break;

  case 993:
#line 6947 "lef.y"
    { ;}
    break;

  case 994:
#line 6951 "lef.y"
    { if (lefrCorrectionTableCbk)
    lefrCorrectionTable.lefiCorrectionTable::addResistanceNumber((yyvsp[0].dval)); ;}
    break;

  case 995:
#line 6954 "lef.y"
    { if (lefrCorrectionTableCbk)
    lefrCorrectionTable.lefiCorrectionTable::addResistanceNumber((yyvsp[0].dval)); ;}
    break;

  case 998:
#line 6964 "lef.y"
    { if (lefrCorrectionTableCbk)
     lefrCorrectionTable.lefiCorrectionTable::addVictimLength((yyvsp[-1].dval)); ;}
    break;

  case 999:
#line 6967 "lef.y"
    { ;}
    break;

  case 1000:
#line 6971 "lef.y"
    { if (lefrCorrectionTableCbk)
	lefrCorrectionTable.lefiCorrectionTable::addVictimCorrection((yyvsp[0].dval)); ;}
    break;

  case 1001:
#line 6974 "lef.y"
    { if (lefrCorrectionTableCbk)
	lefrCorrectionTable.lefiCorrectionTable::addVictimCorrection((yyvsp[0].dval)); ;}
    break;

  case 1002:
#line 6980 "lef.y"
    { /* 5.3 syntax */
        use5_3 = 1;
        if (ignoreVersion) {
           /* do nothing */
        } else if (versionNum > 5.3) {
           /* A 5.3 syntax in 5.4 */
           if (use5_4) {
              if (lefrInputAntennaCbk) { /* write warning only if cbk is set */
                if (inputAntennaWarnings++ < lefrInputAntennaWarnings) {
                   outMsg = (char*)lefMalloc(10000);
                   sprintf (outMsg,
                      "INPUTPINANTENNASIZE statement is a version 5.3 or earlier syntax.\nYour lef file with version %g, has both old and new INPUTPINANTENNASIZE syntax, which is illegal.", versionNum);
                   lefError(1671, outMsg);
                   lefFree(outMsg);
                   CHKERR();
                }
              }
           }
        }
        if (lefrInputAntennaCbk)
          CALLBACK(lefrInputAntennaCbk, lefrInputAntennaCbkType, (yyvsp[-1].dval));
    ;}
    break;

  case 1003:
#line 7004 "lef.y"
    { /* 5.3 syntax */
        use5_3 = 1;
        if (ignoreVersion) {
           /* do nothing */
        } else if (versionNum > 5.3) {
           /* A 5.3 syntax in 5.4 */
           if (use5_4) {
              if (lefrOutputAntennaCbk) { /* write warning only if cbk is set */
                if (outputAntennaWarnings++ < lefrOutputAntennaWarnings) {
                   outMsg = (char*)lefMalloc(10000);
                   sprintf (outMsg,
                      "OUTPUTPINANTENNASIZE statement is a version 5.3 or earlier syntax.\nYour lef file with version %g, has both old and new OUTPUTPINANTENNASIZE syntax, which is illegal.", versionNum);
                   lefError(1672, outMsg);
                   lefFree(outMsg);
                   CHKERR();
                }
              }
           }
        }
        if (lefrOutputAntennaCbk)
          CALLBACK(lefrOutputAntennaCbk, lefrOutputAntennaCbkType, (yyvsp[-1].dval));
    ;}
    break;

  case 1004:
#line 7028 "lef.y"
    { /* 5.3 syntax */
        use5_3 = 1;
        if (ignoreVersion) {
           /* do nothing */
        } else if (versionNum > 5.3) {
           /* A 5.3 syntax in 5.4 */
           if (use5_4) {
              if (lefrInoutAntennaCbk) { /* write warning only if cbk is set */
                if (inoutAntennaWarnings++ < lefrInoutAntennaWarnings) {
                   outMsg = (char*)lefMalloc(10000);
                   sprintf (outMsg,
                      "INOUTPINANTENNASIZE statement is a version 5.3 or earlier syntax.\nYour lef file with version %g, has both old and new INOUTPINANTENNASIZE syntax, which is illegal.", versionNum);
                   lefError(1673, outMsg);
                   lefFree(outMsg);
                   CHKERR();
                }
              }
           }
        }
        if (lefrInoutAntennaCbk)
          CALLBACK(lefrInoutAntennaCbk, lefrInoutAntennaCbkType, (yyvsp[-1].dval));
    ;}
    break;

  case 1005:
#line 7052 "lef.y"
    { /* 5.4 syntax */
        /* 11/12/2002 - this is obsoleted in 5.5, suppose should be ingored */
        /* 12/16/2002 - talked to Dave Noice, leave them in here for debugging*/
        use5_4 = 1;
        if (ignoreVersion) {
           /* do nothing */
        } else if (versionNum < 5.4) {
           if (lefrAntennaInputCbk) { /* write warning only if cbk is set */
             if (antennaInputWarnings++ < lefrAntennaInputWarnings) {
               outMsg = (char*)lefMalloc(10000);
               sprintf (outMsg,
                  "ANTENNAINPUTGATEAREA statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.\nEither update your VERSION number or use the 5.3 syntax.", versionNum);
               lefError(1674, outMsg);
               lefFree(outMsg);
               CHKERR();
             }
           }
        } else if (use5_3) {
           if (lefrAntennaInputCbk) { /* write warning only if cbk is set */
             if (antennaInputWarnings++ < lefrAntennaInputWarnings) {
                outMsg = (char*)lefMalloc(10000);
                sprintf (outMsg,
                   "ANTENNAINPUTGATEAREA statement is a version 5.4 or later syntax.\nYour lef file with version %g, has both old and new ANTENNAINPUTGATEAREA syntax, which is illegal.", versionNum);
                lefError(1675, outMsg);
                lefFree(outMsg);
               CHKERR();
             }
           }
        }
        if (lefrAntennaInputCbk)
          CALLBACK(lefrAntennaInputCbk, lefrAntennaInputCbkType, (yyvsp[-1].dval));
    ;}
    break;

  case 1006:
#line 7086 "lef.y"
    { /* 5.4 syntax */
        /* 11/12/2002 - this is obsoleted in 5.5, & will be ignored */
        /* 12/16/2002 - talked to Dave Noice, leave them in here for debugging*/
        use5_4 = 1;
        if (ignoreVersion) {
           /* do nothing */
        } else if (versionNum < 5.4) {
           if (lefrAntennaInoutCbk) { /* write warning only if cbk is set */
              if (antennaInoutWarnings++ < lefrAntennaInoutWarnings) {
                 outMsg = (char*)lefMalloc(10000);
                 sprintf (outMsg,
                    "ANTENNAINOUTDIFFAREA statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.\nEither update your VERSION number or use the 5.3 syntax.", versionNum);
                 lefError(1676, outMsg);
                 lefFree(outMsg);
                 CHKERR();
              }
           }
        } else if (use5_3) {
           if (lefrAntennaInoutCbk) { /* write warning only if cbk is set */
              if (antennaInoutWarnings++ < lefrAntennaInoutWarnings) {
                 outMsg = (char*)lefMalloc(10000);
                 sprintf (outMsg,
                    "ANTENNAINOUTDIFFAREA statement is a version 5.4 or later syntax.\nYour lef file with version %g, has both old and new ANTENNAINOUTDIFFAREA syntax, which is illegal.", versionNum);
                 lefError(1677, outMsg);
                 lefFree(outMsg);
                 CHKERR();
              }
           }
        }
        if (lefrAntennaInoutCbk)
          CALLBACK(lefrAntennaInoutCbk, lefrAntennaInoutCbkType, (yyvsp[-1].dval));
    ;}
    break;

  case 1007:
#line 7120 "lef.y"
    { /* 5.4 syntax */
        /* 11/12/2002 - this is obsoleted in 5.5, & will be ignored */
        /* 12/16/2002 - talked to Dave Noice, leave them in here for debugging*/
        use5_4 = 1;
        if (ignoreVersion) {
           /* do nothing */
        } else if (versionNum < 5.4) {
           if (lefrAntennaOutputCbk) { /* write warning only if cbk is set */
              if (antennaOutputWarnings++ < lefrAntennaOutputWarnings) {
                 outMsg = (char*)lefMalloc(10000);
                 sprintf (outMsg,
                    "ANTENNAOUTPUTDIFFAREA statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.\nEither update your VERSION number or use the 5.3 syntax.", versionNum);
                 lefError(1678, outMsg);
                 lefFree(outMsg);
                 CHKERR();
              }
           }
        } else if (use5_3) {
           if (lefrAntennaOutputCbk) { /* write warning only if cbk is set */
              if (antennaOutputWarnings++ < lefrAntennaOutputWarnings) {
                 outMsg = (char*)lefMalloc(10000);
                 sprintf (outMsg,
                    "ANTENNAOUTPUTDIFFAREA statement is a version 5.4 or later syntax.\nYour lef file with version %g, has both old and new ANTENNAOUTPUTDIFFAREA syntax, which is illegal.", versionNum);
                 lefError(1679, outMsg);
                 lefFree(outMsg);
                 CHKERR();
              }
           }
        }
        if (lefrAntennaOutputCbk)
          CALLBACK(lefrAntennaOutputCbk, lefrAntennaOutputCbkType, (yyvsp[-1].dval));
    ;}
    break;

  case 1010:
#line 7157 "lef.y"
    { 
        if (lefrExtensionCbk)
          CALLBACK(lefrExtensionCbk, lefrExtensionCbkType, Hist_text);
        if (versionNum >= 5.6)
           ge56almostDone = 1;
    ;}
    break;


      default: break;
    }

/* Line 1126 of yacc.c.  */
#line 13007 "lef.tab.c"

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
	  int yytype = YYTRANSLATE (yychar);
	  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
	  YYSIZE_T yysize = yysize0;
	  YYSIZE_T yysize1;
	  int yysize_overflow = 0;
	  char *yymsg = 0;
#	  define YYERROR_VERBOSE_ARGS_MAXIMUM 5
	  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
	  int yyx;

#if 0
	  /* This is so xgettext sees the translatable formats that are
	     constructed on the fly.  */
	  YY_("syntax error, unexpected %s");
	  YY_("syntax error, unexpected %s, expecting %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s or %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
#endif
	  char *yyfmt;
	  char const *yyf;
	  static char const yyunexpected[] = "syntax error, unexpected %s";
	  static char const yyexpecting[] = ", expecting %s";
	  static char const yyor[] = " or %s";
	  char yyformat[sizeof yyunexpected
			+ sizeof yyexpecting - 1
			+ ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
			   * (sizeof yyor - 1))];
	  char const *yyprefix = yyexpecting;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 1;

	  yyarg[0] = yytname[yytype];
	  yyfmt = yystpcpy (yyformat, yyunexpected);

	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
		  {
		    yycount = 1;
		    yysize = yysize0;
		    yyformat[sizeof yyunexpected - 1] = '\0';
		    break;
		  }
		yyarg[yycount++] = yytname[yyx];
		yysize1 = yysize + yytnamerr (0, yytname[yyx]);
		yysize_overflow |= yysize1 < yysize;
		yysize = yysize1;
		yyfmt = yystpcpy (yyfmt, yyprefix);
		yyprefix = yyor;
	      }

	  yyf = YY_(yyformat);
	  yysize1 = yysize + yystrlen (yyf);
	  yysize_overflow |= yysize1 < yysize;
	  yysize = yysize1;

	  if (!yysize_overflow && yysize <= YYSTACK_ALLOC_MAXIMUM)
	    yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg)
	    {
	      /* Avoid sprintf, as that infringes on the user's name space.
		 Don't have undefined behavior even if the translation
		 produced a string with the wrong number of "%s"s.  */
	      char *yyp = yymsg;
	      int yyi = 0;
	      while ((*yyp = *yyf))
		{
		  if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		    {
		      yyp += yytnamerr (yyp, yyarg[yyi++]);
		      yyf += 2;
		    }
		  else
		    {
		      yyp++;
		      yyf++;
		    }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    {
	      yyerror (YY_("syntax error"));
	      goto yyexhaustedlab;
	    }
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror (YY_("syntax error"));
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
        {
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
        }
      else
	{
	  yydestruct ("Error: discarding", yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (0)
     goto yyerrorlab;

yyvsp -= yylen;
  yyssp -= yylen;
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
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


      yydestruct ("Error: popping", yystos[yystate], yyvsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token. */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

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
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK;
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 7164 "lef.y"


