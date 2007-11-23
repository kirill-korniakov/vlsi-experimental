/* errors.h
 * this is a part of itlDragon
 * Copyright (C) 2005-2006, ITLab, Kirill Kornyakov
 * email: kirillkornyakov@yandex.ru
 */
 
#ifndef _ERRORS_H_
#define _ERRORS_H_

typedef int MULTIPLACER_ERROR;

const MULTIPLACER_ERROR OK = 0;

// parser errors: 0xx

// recursive bisection errors: 1xx
const MULTIPLACER_ERROR NUM_OF_NODES_NEGATIVE = 101;
const MULTIPLACER_ERROR NUM_OF_NETS_NEGATIVE  = 102;
const MULTIPLACER_ERROR NUM_OF_PINS_NEGATIVE  = 103;

const MULTIPLACER_ERROR EXIT_AFTER_RB_NEEDED  = 104;

// bin-swapping errors: 2xx

// adjustment step errors: 3xx
const MULTIPLACER_ERROR NOT_ONTO_MACRO        = 301;
const MULTIPLACER_ERROR EQUALIZE_ROW_LEN_ERR  = 302;
// directed bin-swapping errors: 4xx

// cell annealing errors: 5xx
const MULTIPLACER_ERROR CA_EMPTY_BINS         = 501;

// overlap_removing errors: 6xx

// detailed placement errors: 7xx

// legality checking: 8xx
const MULTIPLACER_ERROR EMPTY_BINS            = 801;
const MULTIPLACER_ERROR X_COORDS_UNEQUAL      = 802;
const MULTIPLACER_ERROR Y_COORDS_UNEQUAL      = 803;
const MULTIPLACER_ERROR WRONG_BINS_INDEXATION = 804;
const MULTIPLACER_ERROR LEFT_BORDER_EXCEEDED  = 805;
const MULTIPLACER_ERROR RIGHT_BORDER_EXCEEDED = 806;
const MULTIPLACER_ERROR NOT_ONTO_SITE         = 807;
const MULTIPLACER_ERROR LOWER_BORDER_EXCEEDED = 808;
const MULTIPLACER_ERROR UPPER_BORDER_EXCEEDED = 809;
const MULTIPLACER_ERROR NOT_INTO_ROW          = 810;
const MULTIPLACER_ERROR OVERLAP               = 811;
const MULTIPLACER_ERROR NOT_LEGAL             = 812;
const MULTIPLACER_ERROR BORDER_EXCEEDED       = 813;
const MULTIPLACER_ERROR BINS_ON_MACROS        = 814;
const MULTIPLACER_ERROR ROW_LENGTH_EXCESS     = 815;

// output checking: 9xx
const MULTIPLACER_ERROR EMPTY_DATA_STRUCTURES = 901;

#endif