/**
 * @file
 *
 * @author jeff.daily@pnnl.gov
 *
 * Copyright (c) 2015 Battelle Memorial Institute.
 */
#include "config.h"

#include <stdlib.h>



#include "parasail.h"
#include "parasail/memory.h"
#include "parasail/internal_neon.h"



#ifdef PARASAIL_TABLE
static inline void arr_store_si128(
        int *array,
        simde__m128i vWH,
        int32_t i,
        int32_t s1Len,
        int32_t j,
        int32_t s2Len)
{
    if (0 <= i+0 && i+0 < s1Len && 0 <= j-0 && j-0 < s2Len) {
        array[1LL*(i+0)*s2Len + (j-0)] = (int8_t)simde_mm_extract_epi8(vWH, 15);
    }
    if (0 <= i+1 && i+1 < s1Len && 0 <= j-1 && j-1 < s2Len) {
        array[1LL*(i+1)*s2Len + (j-1)] = (int8_t)simde_mm_extract_epi8(vWH, 14);
    }
    if (0 <= i+2 && i+2 < s1Len && 0 <= j-2 && j-2 < s2Len) {
        array[1LL*(i+2)*s2Len + (j-2)] = (int8_t)simde_mm_extract_epi8(vWH, 13);
    }
    if (0 <= i+3 && i+3 < s1Len && 0 <= j-3 && j-3 < s2Len) {
        array[1LL*(i+3)*s2Len + (j-3)] = (int8_t)simde_mm_extract_epi8(vWH, 12);
    }
    if (0 <= i+4 && i+4 < s1Len && 0 <= j-4 && j-4 < s2Len) {
        array[1LL*(i+4)*s2Len + (j-4)] = (int8_t)simde_mm_extract_epi8(vWH, 11);
    }
    if (0 <= i+5 && i+5 < s1Len && 0 <= j-5 && j-5 < s2Len) {
        array[1LL*(i+5)*s2Len + (j-5)] = (int8_t)simde_mm_extract_epi8(vWH, 10);
    }
    if (0 <= i+6 && i+6 < s1Len && 0 <= j-6 && j-6 < s2Len) {
        array[1LL*(i+6)*s2Len + (j-6)] = (int8_t)simde_mm_extract_epi8(vWH, 9);
    }
    if (0 <= i+7 && i+7 < s1Len && 0 <= j-7 && j-7 < s2Len) {
        array[1LL*(i+7)*s2Len + (j-7)] = (int8_t)simde_mm_extract_epi8(vWH, 8);
    }
    if (0 <= i+8 && i+8 < s1Len && 0 <= j-8 && j-8 < s2Len) {
        array[1LL*(i+8)*s2Len + (j-8)] = (int8_t)simde_mm_extract_epi8(vWH, 7);
    }
    if (0 <= i+9 && i+9 < s1Len && 0 <= j-9 && j-9 < s2Len) {
        array[1LL*(i+9)*s2Len + (j-9)] = (int8_t)simde_mm_extract_epi8(vWH, 6);
    }
    if (0 <= i+10 && i+10 < s1Len && 0 <= j-10 && j-10 < s2Len) {
        array[1LL*(i+10)*s2Len + (j-10)] = (int8_t)simde_mm_extract_epi8(vWH, 5);
    }
    if (0 <= i+11 && i+11 < s1Len && 0 <= j-11 && j-11 < s2Len) {
        array[1LL*(i+11)*s2Len + (j-11)] = (int8_t)simde_mm_extract_epi8(vWH, 4);
    }
    if (0 <= i+12 && i+12 < s1Len && 0 <= j-12 && j-12 < s2Len) {
        array[1LL*(i+12)*s2Len + (j-12)] = (int8_t)simde_mm_extract_epi8(vWH, 3);
    }
    if (0 <= i+13 && i+13 < s1Len && 0 <= j-13 && j-13 < s2Len) {
        array[1LL*(i+13)*s2Len + (j-13)] = (int8_t)simde_mm_extract_epi8(vWH, 2);
    }
    if (0 <= i+14 && i+14 < s1Len && 0 <= j-14 && j-14 < s2Len) {
        array[1LL*(i+14)*s2Len + (j-14)] = (int8_t)simde_mm_extract_epi8(vWH, 1);
    }
    if (0 <= i+15 && i+15 < s1Len && 0 <= j-15 && j-15 < s2Len) {
        array[1LL*(i+15)*s2Len + (j-15)] = (int8_t)simde_mm_extract_epi8(vWH, 0);
    }
}
#endif

#ifdef PARASAIL_ROWCOL
static inline void arr_store_rowcol(
        int *row,
        int *col,
        simde__m128i vWH,
        int32_t i,
        int32_t s1Len,
        int32_t j,
        int32_t s2Len)
{
    if (i+0 == s1Len-1 && 0 <= j-0 && j-0 < s2Len) {
        row[j-0] = (int8_t)simde_mm_extract_epi8(vWH, 15);
    }
    if (j-0 == s2Len-1 && 0 <= i+0 && i+0 < s1Len) {
        col[(i+0)] = (int8_t)simde_mm_extract_epi8(vWH, 15);
    }
    if (i+1 == s1Len-1 && 0 <= j-1 && j-1 < s2Len) {
        row[j-1] = (int8_t)simde_mm_extract_epi8(vWH, 14);
    }
    if (j-1 == s2Len-1 && 0 <= i+1 && i+1 < s1Len) {
        col[(i+1)] = (int8_t)simde_mm_extract_epi8(vWH, 14);
    }
    if (i+2 == s1Len-1 && 0 <= j-2 && j-2 < s2Len) {
        row[j-2] = (int8_t)simde_mm_extract_epi8(vWH, 13);
    }
    if (j-2 == s2Len-1 && 0 <= i+2 && i+2 < s1Len) {
        col[(i+2)] = (int8_t)simde_mm_extract_epi8(vWH, 13);
    }
    if (i+3 == s1Len-1 && 0 <= j-3 && j-3 < s2Len) {
        row[j-3] = (int8_t)simde_mm_extract_epi8(vWH, 12);
    }
    if (j-3 == s2Len-1 && 0 <= i+3 && i+3 < s1Len) {
        col[(i+3)] = (int8_t)simde_mm_extract_epi8(vWH, 12);
    }
    if (i+4 == s1Len-1 && 0 <= j-4 && j-4 < s2Len) {
        row[j-4] = (int8_t)simde_mm_extract_epi8(vWH, 11);
    }
    if (j-4 == s2Len-1 && 0 <= i+4 && i+4 < s1Len) {
        col[(i+4)] = (int8_t)simde_mm_extract_epi8(vWH, 11);
    }
    if (i+5 == s1Len-1 && 0 <= j-5 && j-5 < s2Len) {
        row[j-5] = (int8_t)simde_mm_extract_epi8(vWH, 10);
    }
    if (j-5 == s2Len-1 && 0 <= i+5 && i+5 < s1Len) {
        col[(i+5)] = (int8_t)simde_mm_extract_epi8(vWH, 10);
    }
    if (i+6 == s1Len-1 && 0 <= j-6 && j-6 < s2Len) {
        row[j-6] = (int8_t)simde_mm_extract_epi8(vWH, 9);
    }
    if (j-6 == s2Len-1 && 0 <= i+6 && i+6 < s1Len) {
        col[(i+6)] = (int8_t)simde_mm_extract_epi8(vWH, 9);
    }
    if (i+7 == s1Len-1 && 0 <= j-7 && j-7 < s2Len) {
        row[j-7] = (int8_t)simde_mm_extract_epi8(vWH, 8);
    }
    if (j-7 == s2Len-1 && 0 <= i+7 && i+7 < s1Len) {
        col[(i+7)] = (int8_t)simde_mm_extract_epi8(vWH, 8);
    }
    if (i+8 == s1Len-1 && 0 <= j-8 && j-8 < s2Len) {
        row[j-8] = (int8_t)simde_mm_extract_epi8(vWH, 7);
    }
    if (j-8 == s2Len-1 && 0 <= i+8 && i+8 < s1Len) {
        col[(i+8)] = (int8_t)simde_mm_extract_epi8(vWH, 7);
    }
    if (i+9 == s1Len-1 && 0 <= j-9 && j-9 < s2Len) {
        row[j-9] = (int8_t)simde_mm_extract_epi8(vWH, 6);
    }
    if (j-9 == s2Len-1 && 0 <= i+9 && i+9 < s1Len) {
        col[(i+9)] = (int8_t)simde_mm_extract_epi8(vWH, 6);
    }
    if (i+10 == s1Len-1 && 0 <= j-10 && j-10 < s2Len) {
        row[j-10] = (int8_t)simde_mm_extract_epi8(vWH, 5);
    }
    if (j-10 == s2Len-1 && 0 <= i+10 && i+10 < s1Len) {
        col[(i+10)] = (int8_t)simde_mm_extract_epi8(vWH, 5);
    }
    if (i+11 == s1Len-1 && 0 <= j-11 && j-11 < s2Len) {
        row[j-11] = (int8_t)simde_mm_extract_epi8(vWH, 4);
    }
    if (j-11 == s2Len-1 && 0 <= i+11 && i+11 < s1Len) {
        col[(i+11)] = (int8_t)simde_mm_extract_epi8(vWH, 4);
    }
    if (i+12 == s1Len-1 && 0 <= j-12 && j-12 < s2Len) {
        row[j-12] = (int8_t)simde_mm_extract_epi8(vWH, 3);
    }
    if (j-12 == s2Len-1 && 0 <= i+12 && i+12 < s1Len) {
        col[(i+12)] = (int8_t)simde_mm_extract_epi8(vWH, 3);
    }
    if (i+13 == s1Len-1 && 0 <= j-13 && j-13 < s2Len) {
        row[j-13] = (int8_t)simde_mm_extract_epi8(vWH, 2);
    }
    if (j-13 == s2Len-1 && 0 <= i+13 && i+13 < s1Len) {
        col[(i+13)] = (int8_t)simde_mm_extract_epi8(vWH, 2);
    }
    if (i+14 == s1Len-1 && 0 <= j-14 && j-14 < s2Len) {
        row[j-14] = (int8_t)simde_mm_extract_epi8(vWH, 1);
    }
    if (j-14 == s2Len-1 && 0 <= i+14 && i+14 < s1Len) {
        col[(i+14)] = (int8_t)simde_mm_extract_epi8(vWH, 1);
    }
    if (i+15 == s1Len-1 && 0 <= j-15 && j-15 < s2Len) {
        row[j-15] = (int8_t)simde_mm_extract_epi8(vWH, 0);
    }
    if (j-15 == s2Len-1 && 0 <= i+15 && i+15 < s1Len) {
        col[(i+15)] = (int8_t)simde_mm_extract_epi8(vWH, 0);
    }
}
#endif

#ifdef PARASAIL_TABLE
#define FNAME parasail_sg_stats_table_diag_neon_128_8
#else
#ifdef PARASAIL_ROWCOL
#define FNAME parasail_sg_stats_rowcol_diag_neon_128_8
#else
#define FNAME parasail_sg_stats_diag_neon_128_8
#endif
#endif

parasail_result_t* FNAME(
        const char * const restrict _s1, const int s1Len,
        const char * const restrict _s2, const int s2Len,
        const int open, const int gap, const parasail_matrix_t *matrix)
{
    const int32_t N = 16; /* number of values in vector */
    const int32_t PAD = N-1;
    const int32_t PAD2 = PAD*2;
    const int32_t s1Len_PAD = s1Len+PAD;
    const int32_t s2Len_PAD = s2Len+PAD;
    int8_t * const restrict s1      = parasail_memalign_int8_t(16, s1Len+PAD);
    int8_t * const restrict s2B     = parasail_memalign_int8_t(16, s2Len+PAD2);
    int8_t * const restrict _H_pr = parasail_memalign_int8_t(16, s2Len+PAD2);
    int8_t * const restrict _HM_pr = parasail_memalign_int8_t(16, s2Len+PAD2);
    int8_t * const restrict _HS_pr = parasail_memalign_int8_t(16, s2Len+PAD2);
    int8_t * const restrict _HL_pr = parasail_memalign_int8_t(16, s2Len+PAD2);
    int8_t * const restrict _F_pr = parasail_memalign_int8_t(16, s2Len+PAD2);
    int8_t * const restrict _FM_pr = parasail_memalign_int8_t(16, s2Len+PAD2);
    int8_t * const restrict _FS_pr = parasail_memalign_int8_t(16, s2Len+PAD2);
    int8_t * const restrict _FL_pr = parasail_memalign_int8_t(16, s2Len+PAD2);
    int8_t * const restrict s2 = s2B+PAD; /* will allow later for negative indices */
    int8_t * const restrict H_pr = _H_pr+PAD;
    int8_t * const restrict HM_pr = _HM_pr+PAD;
    int8_t * const restrict HS_pr = _HS_pr+PAD;
    int8_t * const restrict HL_pr = _HL_pr+PAD;
    int8_t * const restrict F_pr = _F_pr+PAD;
    int8_t * const restrict FM_pr = _FM_pr+PAD;
    int8_t * const restrict FS_pr = _FS_pr+PAD;
    int8_t * const restrict FL_pr = _FL_pr+PAD;
#ifdef PARASAIL_TABLE
    parasail_result_t *result = parasail_result_new_table3(s1Len, s2Len);
#else
#ifdef PARASAIL_ROWCOL
    parasail_result_t *result = parasail_result_new_rowcol3(s1Len, s2Len);
#else
    parasail_result_t *result = parasail_result_new_stats();
#endif
#endif
    int32_t i = 0;
    int32_t j = 0;
    int32_t end_query = 0;
    int32_t end_ref = 0;
    const int8_t NEG_LIMIT = (-open < matrix->min ?
        INT8_MIN + open : INT8_MIN - matrix->min) + 1;
    const int8_t POS_LIMIT = INT8_MAX - matrix->max - 1;
    int8_t score = NEG_LIMIT;
    int8_t matches = NEG_LIMIT;
    int8_t similar = NEG_LIMIT;
    int8_t length = NEG_LIMIT;
    simde__m128i vNegLimit = simde_mm_set1_epi8(NEG_LIMIT);
    simde__m128i vPosLimit = simde_mm_set1_epi8(POS_LIMIT);
    simde__m128i vSaturationCheckMin = vPosLimit;
    simde__m128i vSaturationCheckMax = vNegLimit;
    simde__m128i vNegInf = simde_mm_set1_epi8(NEG_LIMIT);
    simde__m128i vOpen = simde_mm_set1_epi8(open);
    simde__m128i vGap  = simde_mm_set1_epi8(gap);
    simde__m128i vZero = simde_mm_set1_epi8(0);
    simde__m128i vNegInf0 = simde_mm_insert_epi8(vZero, NEG_LIMIT, 15);
    simde__m128i vOne = simde_mm_set1_epi8(1);
    simde__m128i vN = simde_mm_set1_epi8(N);
    simde__m128i vNegOne = simde_mm_set1_epi8(-1);
    simde__m128i vI = simde_mm_set_epi8(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
    simde__m128i vJreset = simde_mm_set_epi8(0,-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-11,-12,-13,-14,-15);
    simde__m128i vMaxH = vNegInf;
    simde__m128i vMaxM = vNegInf;
    simde__m128i vMaxS = vNegInf;
    simde__m128i vMaxL = vNegInf;
    simde__m128i vEndI = vNegInf;
    simde__m128i vEndJ = vNegInf;
    simde__m128i vILimit = simde_mm_set1_epi8(s1Len);
    simde__m128i vILimit1 = simde_mm_subs_epi8(vILimit, vOne);
    simde__m128i vJLimit = simde_mm_set1_epi8(s2Len);
    simde__m128i vJLimit1 = simde_mm_subs_epi8(vJLimit, vOne);

    /* convert _s1 from char to int in range 0-23 */
    for (i=0; i<s1Len; ++i) {
        s1[i] = matrix->mapper[(unsigned char)_s1[i]];
    }
    /* pad back of s1 with dummy values */
    for (i=s1Len; i<s1Len_PAD; ++i) {
        s1[i] = 0; /* point to first matrix row because we don't care */
    }

    /* convert _s2 from char to int in range 0-23 */
    for (j=0; j<s2Len; ++j) {
        s2[j] = matrix->mapper[(unsigned char)_s2[j]];
    }
    /* pad front of s2 with dummy values */
    for (j=-PAD; j<0; ++j) {
        s2[j] = 0; /* point to first matrix row because we don't care */
    }
    /* pad back of s2 with dummy values */
    for (j=s2Len; j<s2Len_PAD; ++j) {
        s2[j] = 0; /* point to first matrix row because we don't care */
    }

    /* set initial values for stored row */
    for (j=0; j<s2Len; ++j) {
        H_pr[j] = 0;
        HM_pr[j] = 0;
        HS_pr[j] = 0;
        HL_pr[j] = 0;
        F_pr[j] = NEG_LIMIT;
        FM_pr[j] = 0;
        FS_pr[j] = 0;
        FL_pr[j] = 0;
    }
    /* pad front of stored row values */
    for (j=-PAD; j<0; ++j) {
        H_pr[j] = 0;
        HM_pr[j] = 0;
        HS_pr[j] = 0;
        HL_pr[j] = 0;
        F_pr[j] = 0;
        FM_pr[j] = 0;
        FS_pr[j] = 0;
        FL_pr[j] = 0;
    }
    /* pad back of stored row values */
    for (j=s2Len; j<s2Len+PAD; ++j) {
        H_pr[j] = 0;
        HM_pr[j] = 0;
        HS_pr[j] = 0;
        HL_pr[j] = 0;
        F_pr[j] = 0;
        FM_pr[j] = 0;
        FS_pr[j] = 0;
        FL_pr[j] = 0;
    }
    H_pr[-1] = 0; /* upper left corner */

    /* iterate over query sequence */
    for (i=0; i<s1Len; i+=N) {
        simde__m128i case1 = vZero;
        simde__m128i case2 = vZero;
        simde__m128i vNH = vZero;
        simde__m128i vNM = vZero;
        simde__m128i vNS = vZero;
        simde__m128i vNL = vZero;
        simde__m128i vWH = vZero;
        simde__m128i vWM = vZero;
        simde__m128i vWS = vZero;
        simde__m128i vWL = vZero;
        simde__m128i vE = vNegInf0;
        simde__m128i vE_opn = vNegInf;
        simde__m128i vE_ext = vNegInf;
        simde__m128i vEM = vZero;
        simde__m128i vES = vZero;
        simde__m128i vEL = vZero;
        simde__m128i vF = vNegInf0;
        simde__m128i vF_opn = vNegInf;
        simde__m128i vF_ext = vNegInf;
        simde__m128i vFM = vZero;
        simde__m128i vFS = vZero;
        simde__m128i vFL = vZero;
        simde__m128i vJ = vJreset;
        simde__m128i vs1 = simde_mm_set_epi8(
                s1[i+0],
                s1[i+1],
                s1[i+2],
                s1[i+3],
                s1[i+4],
                s1[i+5],
                s1[i+6],
                s1[i+7],
                s1[i+8],
                s1[i+9],
                s1[i+10],
                s1[i+11],
                s1[i+12],
                s1[i+13],
                s1[i+14],
                s1[i+15]);
        simde__m128i vs2 = vNegInf;
        const int * const restrict matrow0 = &matrix->matrix[matrix->size*s1[i+0]];
        const int * const restrict matrow1 = &matrix->matrix[matrix->size*s1[i+1]];
        const int * const restrict matrow2 = &matrix->matrix[matrix->size*s1[i+2]];
        const int * const restrict matrow3 = &matrix->matrix[matrix->size*s1[i+3]];
        const int * const restrict matrow4 = &matrix->matrix[matrix->size*s1[i+4]];
        const int * const restrict matrow5 = &matrix->matrix[matrix->size*s1[i+5]];
        const int * const restrict matrow6 = &matrix->matrix[matrix->size*s1[i+6]];
        const int * const restrict matrow7 = &matrix->matrix[matrix->size*s1[i+7]];
        const int * const restrict matrow8 = &matrix->matrix[matrix->size*s1[i+8]];
        const int * const restrict matrow9 = &matrix->matrix[matrix->size*s1[i+9]];
        const int * const restrict matrow10 = &matrix->matrix[matrix->size*s1[i+10]];
        const int * const restrict matrow11 = &matrix->matrix[matrix->size*s1[i+11]];
        const int * const restrict matrow12 = &matrix->matrix[matrix->size*s1[i+12]];
        const int * const restrict matrow13 = &matrix->matrix[matrix->size*s1[i+13]];
        const int * const restrict matrow14 = &matrix->matrix[matrix->size*s1[i+14]];
        const int * const restrict matrow15 = &matrix->matrix[matrix->size*s1[i+15]];
        simde__m128i vIltLimit = simde_mm_cmplt_epi8(vI, vILimit);
        simde__m128i vIeqLimit1 = simde_mm_cmpeq_epi8(vI, vILimit1);
        /* iterate over database sequence */
        for (j=0; j<s2Len+PAD; ++j) {
            simde__m128i vMat;
            simde__m128i vNWH = vNH;
            simde__m128i vNWM = vNM;
            simde__m128i vNWS = vNS;
            simde__m128i vNWL = vNL;
            vNH = simde_mm_srli_si128(vWH, 1);
            vNH = simde_mm_insert_epi8(vNH, H_pr[j], 15);
            vNM = simde_mm_srli_si128(vWM, 1);
            vNM = simde_mm_insert_epi8(vNM, HM_pr[j], 15);
            vNS = simde_mm_srli_si128(vWS, 1);
            vNS = simde_mm_insert_epi8(vNS, HS_pr[j], 15);
            vNL = simde_mm_srli_si128(vWL, 1);
            vNL = simde_mm_insert_epi8(vNL, HL_pr[j], 15);
            vF = simde_mm_srli_si128(vF, 1);
            vF = simde_mm_insert_epi8(vF, F_pr[j], 15);
            vFM = simde_mm_srli_si128(vFM, 1);
            vFM = simde_mm_insert_epi8(vFM, FM_pr[j], 15);
            vFS = simde_mm_srli_si128(vFS, 1);
            vFS = simde_mm_insert_epi8(vFS, FS_pr[j], 15);
            vFL = simde_mm_srli_si128(vFL, 1);
            vFL = simde_mm_insert_epi8(vFL, FL_pr[j], 15);
            vF_opn = simde_mm_subs_epi8(vNH, vOpen);
            vF_ext = simde_mm_subs_epi8(vF, vGap);
            vF = simde_mm_max_epi8(vF_opn, vF_ext);
            case1 = simde_mm_cmpgt_epi8(vF_opn, vF_ext);
            vFM = simde_mm_blendv_epi8(vFM, vNM, case1);
            vFS = simde_mm_blendv_epi8(vFS, vNS, case1);
            vFL = simde_mm_blendv_epi8(vFL, vNL, case1);
            vFL = simde_mm_adds_epi8(vFL, vOne);
            vE_opn = simde_mm_subs_epi8(vWH, vOpen);
            vE_ext = simde_mm_subs_epi8(vE, vGap);
            vE = simde_mm_max_epi8(vE_opn, vE_ext);
            case1 = simde_mm_cmpgt_epi8(vE_opn, vE_ext);
            vEM = simde_mm_blendv_epi8(vEM, vWM, case1);
            vES = simde_mm_blendv_epi8(vES, vWS, case1);
            vEL = simde_mm_blendv_epi8(vEL, vWL, case1);
            vEL = simde_mm_adds_epi8(vEL, vOne);
            vs2 = simde_mm_srli_si128(vs2, 1);
            vs2 = simde_mm_insert_epi8(vs2, s2[j], 15);
            vMat = simde_mm_set_epi8(
                    matrow0[s2[j-0]],
                    matrow1[s2[j-1]],
                    matrow2[s2[j-2]],
                    matrow3[s2[j-3]],
                    matrow4[s2[j-4]],
                    matrow5[s2[j-5]],
                    matrow6[s2[j-6]],
                    matrow7[s2[j-7]],
                    matrow8[s2[j-8]],
                    matrow9[s2[j-9]],
                    matrow10[s2[j-10]],
                    matrow11[s2[j-11]],
                    matrow12[s2[j-12]],
                    matrow13[s2[j-13]],
                    matrow14[s2[j-14]],
                    matrow15[s2[j-15]]
                    );
            vNWH = simde_mm_adds_epi8(vNWH, vMat);
            vWH = simde_mm_max_epi8(vNWH, vE);
            vWH = simde_mm_max_epi8(vWH, vF);
            case1 = simde_mm_cmpeq_epi8(vWH, vNWH);
            case2 = simde_mm_cmpeq_epi8(vWH, vF);
            vWM = simde_mm_blendv_epi8(
                    simde_mm_blendv_epi8(vEM, vFM, case2),
                    simde_mm_adds_epi8(vNWM,
                        simde_mm_and_si128(
                            simde_mm_cmpeq_epi8(vs1,vs2),
                            vOne)),
                    case1);
            vWS = simde_mm_blendv_epi8(
                    simde_mm_blendv_epi8(vES, vFS, case2),
                    simde_mm_adds_epi8(vNWS,
                        simde_mm_and_si128(
                            simde_mm_cmpgt_epi8(vMat,vZero),
                            vOne)),
                    case1);
            vWL = simde_mm_blendv_epi8(
                    simde_mm_blendv_epi8(vEL, vFL, case2),
                    simde_mm_adds_epi8(vNWL, vOne), case1);
            /* as minor diagonal vector passes across the j=-1 boundary,
             * assign the appropriate boundary conditions */
            {
                simde__m128i cond = simde_mm_cmpeq_epi8(vJ,vNegOne);
                vWH = simde_mm_andnot_si128(cond, vWH);
                vWM = simde_mm_andnot_si128(cond, vWM);
                vWS = simde_mm_andnot_si128(cond, vWS);
                vWL = simde_mm_andnot_si128(cond, vWL);
                vE = simde_mm_blendv_epi8(vE, vNegInf, cond);
                vEM = simde_mm_andnot_si128(cond, vEM);
                vES = simde_mm_andnot_si128(cond, vES);
                vEL = simde_mm_andnot_si128(cond, vEL);
            }
            vSaturationCheckMin = simde_mm_min_epi8(vSaturationCheckMin, vWH);
            vSaturationCheckMax = simde_mm_max_epi8(vSaturationCheckMax, vWH);
            vSaturationCheckMax = simde_mm_max_epi8(vSaturationCheckMax, vWM);
            vSaturationCheckMax = simde_mm_max_epi8(vSaturationCheckMax, vWS);
            vSaturationCheckMax = simde_mm_max_epi8(vSaturationCheckMax, vWL);
            vSaturationCheckMax = simde_mm_max_epi8(vSaturationCheckMax, vWL);
            vSaturationCheckMax = simde_mm_max_epi8(vSaturationCheckMax, vJ);
#ifdef PARASAIL_TABLE
            arr_store_si128(result->stats->tables->score_table, vWH, i, s1Len, j, s2Len);
            arr_store_si128(result->stats->tables->matches_table, vWM, i, s1Len, j, s2Len);
            arr_store_si128(result->stats->tables->similar_table, vWS, i, s1Len, j, s2Len);
            arr_store_si128(result->stats->tables->length_table, vWL, i, s1Len, j, s2Len);
#endif
#ifdef PARASAIL_ROWCOL
            arr_store_rowcol(result->stats->rowcols->score_row,   result->stats->rowcols->score_col, vWH, i, s1Len, j, s2Len);
            arr_store_rowcol(result->stats->rowcols->matches_row, result->stats->rowcols->matches_col, vWM, i, s1Len, j, s2Len);
            arr_store_rowcol(result->stats->rowcols->similar_row, result->stats->rowcols->similar_col, vWS, i, s1Len, j, s2Len);
            arr_store_rowcol(result->stats->rowcols->length_row,  result->stats->rowcols->length_col, vWL, i, s1Len, j, s2Len);
#endif
            H_pr[j-15] = (int8_t)simde_mm_extract_epi8(vWH,0);
            HM_pr[j-15] = (int8_t)simde_mm_extract_epi8(vWM,0);
            HS_pr[j-15] = (int8_t)simde_mm_extract_epi8(vWS,0);
            HL_pr[j-15] = (int8_t)simde_mm_extract_epi8(vWL,0);
            F_pr[j-15] = (int8_t)simde_mm_extract_epi8(vF,0);
            FM_pr[j-15] = (int8_t)simde_mm_extract_epi8(vFM,0);
            FS_pr[j-15] = (int8_t)simde_mm_extract_epi8(vFS,0);
            FL_pr[j-15] = (int8_t)simde_mm_extract_epi8(vFL,0);
            /* as minor diagonal vector passes across the i or j limit
             * boundary, extract the last value of the column or row */
            {
                simde__m128i vJeqLimit1 = simde_mm_cmpeq_epi8(vJ, vJLimit1);
                simde__m128i vJgtNegOne = simde_mm_cmpgt_epi8(vJ, vNegOne);
                simde__m128i vJltLimit = simde_mm_cmplt_epi8(vJ, vJLimit);
                simde__m128i cond_j = simde_mm_and_si128(vIltLimit, vJeqLimit1);
                simde__m128i cond_i = simde_mm_and_si128(vIeqLimit1,
                        simde_mm_and_si128(vJgtNegOne, vJltLimit));
                simde__m128i cond_valid_IJ = simde_mm_or_si128(cond_i, cond_j);
                simde__m128i cond_eq = simde_mm_cmpeq_epi8(vWH, vMaxH);
                simde__m128i cond_max = simde_mm_cmpgt_epi8(vWH, vMaxH);
                simde__m128i cond_all = simde_mm_and_si128(cond_max, cond_valid_IJ);
                simde__m128i cond_Jlt = simde_mm_cmplt_epi8(vJ, vEndJ);
                vMaxH = simde_mm_blendv_epi8(vMaxH, vWH, cond_all);
                vMaxM = simde_mm_blendv_epi8(vMaxM, vWM, cond_all);
                vMaxS = simde_mm_blendv_epi8(vMaxS, vWS, cond_all);
                vMaxL = simde_mm_blendv_epi8(vMaxL, vWL, cond_all);
                vEndI = simde_mm_blendv_epi8(vEndI, vI, cond_all);
                vEndJ = simde_mm_blendv_epi8(vEndJ, vJ, cond_all);
                cond_all = simde_mm_and_si128(cond_Jlt, cond_eq);
                cond_all = simde_mm_and_si128(cond_all, cond_valid_IJ);
                vMaxM = simde_mm_blendv_epi8(vMaxM, vWM, cond_all);
                vMaxS = simde_mm_blendv_epi8(vMaxS, vWS, cond_all);
                vMaxL = simde_mm_blendv_epi8(vMaxL, vWL, cond_all);
                vEndI = simde_mm_blendv_epi8(vEndI, vI, cond_all);
                vEndJ = simde_mm_blendv_epi8(vEndJ, vJ, cond_all);
            }
            vJ = simde_mm_adds_epi8(vJ, vOne);
        }
        vI = simde_mm_adds_epi8(vI, vN);
        vSaturationCheckMax = simde_mm_max_epi8(vSaturationCheckMax, vI);
    }

    /* alignment ending position */
    {
        int8_t *t = (int8_t*)&vMaxH;
        int8_t *m = (int8_t*)&vMaxM;
        int8_t *s = (int8_t*)&vMaxS;
        int8_t *l = (int8_t*)&vMaxL;
        int8_t *i = (int8_t*)&vEndI;
        int8_t *j = (int8_t*)&vEndJ;
        int32_t k;
        for (k=0; k<N; ++k, ++t, ++m, ++s, ++l, ++i, ++j) {
            if (*t > score) {
                score = *t;
                matches = *m;
                similar = *s;
                length = *l;
                end_query = *i;
                end_ref = *j;
            }
            else if (*t == score) {
                if (*j < end_ref) {
                    matches = *m;
                    similar = *s;
                    length = *l;
                    end_query = *i;
                    end_ref = *j;
                }
                else if (*j == end_ref && *i < end_query) {
                    matches = *m;
                    similar = *s;
                    length = *l;
                    end_query = *i;
                    end_ref = *j;
                }
            }
        }
    }

    if (simde_mm_movemask_epi8(simde_mm_or_si128(
            simde_mm_cmplt_epi8(vSaturationCheckMin, vNegLimit),
            simde_mm_cmpgt_epi8(vSaturationCheckMax, vPosLimit)))) {
        result->flag |= PARASAIL_FLAG_SATURATED;
        score = 0;
        matches = 0;
        similar = 0;
        length = 0;
        end_query = 0;
        end_ref = 0;
    }

    result->score = score;
    result->end_query = end_query;
    result->end_ref = end_ref;
    result->stats->matches = matches;
    result->stats->similar = similar;
    result->stats->length = length;
    result->flag |= PARASAIL_FLAG_SG | PARASAIL_FLAG_DIAG
        | PARASAIL_FLAG_STATS
        | PARASAIL_FLAG_BITS_8 | PARASAIL_FLAG_LANES_16;
#ifdef PARASAIL_TABLE
    result->flag |= PARASAIL_FLAG_TABLE;
#endif
#ifdef PARASAIL_ROWCOL
    result->flag |= PARASAIL_FLAG_ROWCOL;
#endif

    parasail_free(_FL_pr);
    parasail_free(_FS_pr);
    parasail_free(_FM_pr);
    parasail_free(_F_pr);
    parasail_free(_HL_pr);
    parasail_free(_HS_pr);
    parasail_free(_HM_pr);
    parasail_free(_H_pr);
    parasail_free(s2B);
    parasail_free(s1);

    return result;
}


