/* errors.h
 * this is a part of itlDragon
 * Copyright (C) 2005-2006, ITLab, Kirill Kornyakov
 * email: kirillkornyakov@yandex.ru
 */
 
#ifndef _ERRORS_H_
#define _ERRORS_H_

typedef int ITLDRAGON_ERROR;

const ITLDRAGON_ERROR OK = 0;

// parser errors: 0xx

// recursive bisection errors: 1xx
const ITLDRAGON_ERROR NUM_OF_NODES_NEGATIVE = 101;
const ITLDRAGON_ERROR NUM_OF_NETS_NEGATIVE  = 102;
const ITLDRAGON_ERROR NUM_OF_PINS_NEGATIVE  = 103;

const ITLDRAGON_ERROR EXIT_AFTER_RB_NEEDED  = 104;

// bin-swapping errors: 2xx

// adjustment step errors: 3xx
const ITLDRAGON_ERROR NOT_ONTO_MACRO        = 301;
const ITLDRAGON_ERROR EQUALIZE_ROW_LEN_ERR  = 302;
// directed bin-swapping errors: 4xx

// cell annealing errors: 5xx
const ITLDRAGON_ERROR CA_EMPTY_BINS         = 501;

// overlap_removing errors: 6xx

// detailed placement errors: 7xx

// legality checking: 8xx
const ITLDRAGON_ERROR EMPTY_BINS            = 801;
const ITLDRAGON_ERROR X_COORDS_UNEQUAL      = 802;
const ITLDRAGON_ERROR Y_COORDS_UNEQUAL      = 803;
const ITLDRAGON_ERROR WRONG_BINS_INDEXATION = 804;
const ITLDRAGON_ERROR LEFT_BORDER_EXCEEDED  = 805;
const ITLDRAGON_ERROR RIGHT_BORDER_EXCEEDED = 806;
const ITLDRAGON_ERROR NOT_ONTO_SITE         = 807;
const ITLDRAGON_ERROR LOWER_BORDER_EXCEEDED = 808;
const ITLDRAGON_ERROR UPPER_BORDER_EXCEEDED = 809;
const ITLDRAGON_ERROR NOT_INTO_ROW          = 810;
const ITLDRAGON_ERROR OVERLAP               = 811;
const ITLDRAGON_ERROR NOT_LEGAL             = 812;
const ITLDRAGON_ERROR BORDER_EXCEEDED       = 813;
const ITLDRAGON_ERROR BINS_ON_MACROS        = 814;
const ITLDRAGON_ERROR ROW_LENGTH_EXCESS     = 815;

// output checking: 9xx
const ITLDRAGON_ERROR EMPTY_DATA_STRUCTURES = 901;

#endif