/**
 * @file
 *
 * @author jeffrey.daily@gmail.com
 *
 * Copyright (c) 2015 Battelle Memorial Institute.
 *
 * This file was converted to C code from the raw file found at
 * ftp://ftp.cbi.pku.edu.cn/pub/software/blast/matrices/PAM200, the
 * Center for Bioinformatics, Peking University, China.
 */
#ifndef _PARASAIL_PAM200_H_
#define _PARASAIL_PAM200_H_

#include "parasail.h"
#include "pam_map.h"

#ifdef __cplusplus
extern "C" {
#endif

/* # */
/* # This matrix was produced by "pam" Version 1.0.6 [28-Jul-93] */
/* # */
/* # PAM 200 substitution matrix, scale = ln(2)/3 = 0.231049 */
/* # */
/* # Expected score = -1.23, Entropy = 0.507 bits */
/* # */
/* # Lowest score = -9, Highest score = 18 */
/* # */

static const int parasail_pam200_[] = {
/*        A   R   N   D   C   Q   E   G   H   I   L   K   M   F   P   S   T   W   Y   V   B   Z   X   * */
/* A */   3, -2,  0,  0, -3, -1,  0,  1, -2, -1, -2, -2, -2, -4,  1,  1,  1, -7, -4,  0,  0,  0,  0, -9,
/* R */  -2,  7,  0, -2, -4,  1, -2, -4,  2, -2, -4,  4, -1, -5,  0, -1, -1,  2, -5, -3, -1,  0, -1, -9,
/* N */   0,  0,  3,  3, -5,  1,  2,  0,  2, -2, -4,  1, -2, -4, -1,  1,  0, -5, -2, -2,  3,  1,  0, -9,
/* D */   0, -2,  3,  5, -6,  2,  4,  0,  0, -3, -5,  0, -4, -7, -2,  0,  0, -8, -5, -3,  4,  3, -1, -9,
/* C */  -3, -4, -5, -6, 12, -7, -7, -4, -4, -3, -7, -7, -6, -6, -4,  0, -3, -9,  0, -2, -5, -7, -4, -9,
/* Q */  -1,  1,  1,  2, -7,  5,  3, -2,  3, -3, -2,  1, -1, -6,  0, -1, -1, -6, -5, -3,  1,  4, -1, -9,
/* E */   0, -2,  2,  4, -7,  3,  5,  0,  0, -3, -4,  0, -3, -7, -1,  0, -1, -9, -5, -2,  3,  4, -1, -9,
/* G */   1, -4,  0,  0, -4, -2,  0,  6, -3, -3, -5, -2, -4, -6, -1,  1,  0, -8, -6, -2,  0, -1, -1, -9,
/* H */  -2,  2,  2,  0, -4,  3,  0, -3,  8, -3, -3, -1, -3, -2, -1, -1, -2, -3,  0, -3,  1,  2, -1, -9,
/* I */  -1, -2, -2, -3, -3, -3, -3, -3, -3,  6,  2, -2,  2,  1, -3, -2,  0, -6, -2,  4, -3, -3, -1, -9,
/* L */  -2, -4, -4, -5, -7, -2, -4, -5, -3,  2,  7, -4,  4,  2, -3, -4, -2, -2, -2,  2, -4, -3, -2, -9,
/* K */  -2,  4,  1,  0, -7,  1,  0, -2, -1, -2, -4,  6,  1, -7, -2,  0,  0, -4, -5, -3,  0,  0, -1, -9,
/* M */  -2, -1, -2, -4, -6, -1, -3, -4, -3,  2,  4,  1,  8,  0, -3, -2, -1, -5, -3,  2, -3, -2, -1, -9,
/* F */  -4, -5, -4, -7, -6, -6, -7, -6, -2,  1,  2, -7,  0, 10, -6, -4, -4,  0,  7, -2, -6, -6, -3, -9,
/* P */   1,  0, -1, -2, -4,  0, -1, -1, -1, -3, -3, -2, -3, -6,  7,  1,  0, -7, -6, -2, -1, -1, -1, -9,
/* S */   1, -1,  1,  0,  0, -1,  0,  1, -1, -2, -4,  0, -2, -4,  1,  2,  2, -3, -3, -1,  1, -1,  0, -9,
/* T */   1, -1,  0,  0, -3, -1, -1,  0, -2,  0, -2,  0, -1, -4,  0,  2,  4, -6, -3,  0,  0, -1,  0, -9,
/* W */  -7,  2, -5, -8, -9, -6, -9, -8, -3, -6, -2, -4, -5,  0, -7, -3, -6, 18, -1, -8, -6, -7, -5, -9,
/* Y */  -4, -5, -2, -5,  0, -5, -5, -6,  0, -2, -2, -5, -3,  7, -6, -3, -3, -1, 11, -3, -4, -5, -3, -9,
/* V */   0, -3, -2, -3, -2, -3, -2, -2, -3,  4,  2, -3,  2, -2, -2, -1,  0, -8, -3,  5, -3, -2, -1, -9,
/* B */   0, -1,  3,  4, -5,  1,  3,  0,  1, -3, -4,  0, -3, -6, -1,  1,  0, -6, -4, -3,  3,  2, -1, -9,
/* Z */   0,  0,  1,  3, -7,  4,  4, -1,  2, -3, -3,  0, -2, -6, -1, -1, -1, -7, -5, -2,  2,  4, -1, -9,
/* X */   0, -1,  0, -1, -4, -1, -1, -1, -1, -1, -2, -1, -1, -3, -1,  0,  0, -5, -3, -1, -1, -1, -1, -9,
/* * */  -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9,  1
};

static const parasail_matrix_t parasail_pam200 = {
    "pam200",
    parasail_pam200_,
    parasail_pam_map,
    24,
    18,
    -9,
    NULL
};

#ifdef __cplusplus
}
#endif

#endif /* _PARASAIL_PAM200_H_ */

