/**
 * @file
 *
 * @author jeffrey.daily@gmail.com
 *
 * Copyright (c) 2015 Battelle Memorial Institute.
 */
#include "config.h"

#include <stdint.h>
#include <stdlib.h>

#if defined(_MSC_VER)
#include <intrin.h>
#else
#include <emmintrin.h>
#include <smmintrin.h>
#endif

#include "parasail.h"
#include "parasail/memory.h"
#include "parasail/internal_sse.h"

#define SWAP(A,B) { __m128i* tmp = A; A = B; B = tmp; }
#define SWAP3(A,B,C) { __m128i* tmp = A; A = B; B = C; C = tmp; }

#define NEG_INF (INT64_MIN/(int64_t)(2))

#if HAVE_SSE41_MM_EXTRACT_EPI64
#define _mm_extract_epi64_rpl _mm_extract_epi64
#else
static inline int64_t _mm_extract_epi64_rpl(__m128i a, int imm) {
    __m128i_64_t A;
    A.m = a;
    return A.v[imm];
}
#endif

static inline __m128i _mm_cmpgt_epi64_rpl(__m128i a, __m128i b) {
    __m128i_64_t A;
    __m128i_64_t B;
    A.m = a;
    B.m = b;
    A.v[0] = (A.v[0]>B.v[0]) ? 0xFFFFFFFFFFFFFFFF : 0;
    A.v[1] = (A.v[1]>B.v[1]) ? 0xFFFFFFFFFFFFFFFF : 0;
    return A.m;
}

#if HAVE_SSE41_MM_INSERT_EPI64
#define _mm_insert_epi64_rpl _mm_insert_epi64
#else
static inline __m128i _mm_insert_epi64_rpl(__m128i a, int64_t i, int imm) {
    __m128i_64_t A;
    A.m = a;
    A.v[imm] = i;
    return A.m;
}
#endif

static inline __m128i _mm_max_epi64_rpl(__m128i a, __m128i b) {
    __m128i_64_t A;
    __m128i_64_t B;
    A.m = a;
    B.m = b;
    A.v[0] = (A.v[0]>B.v[0]) ? A.v[0] : B.v[0];
    A.v[1] = (A.v[1]>B.v[1]) ? A.v[1] : B.v[1];
    return A.m;
}

#if HAVE_SSE2_MM_SET1_EPI64X
#define _mm_set1_epi64x_rpl _mm_set1_epi64x
#else
static inline __m128i _mm_set1_epi64x_rpl(int64_t i) {
    __m128i_64_t A;
    A.v[0] = i;
    A.v[1] = i;
    return A.m;
}
#endif

static inline int64_t _mm_hmax_epi64_rpl(__m128i a) {
    a = _mm_max_epi64_rpl(a, _mm_srli_si128(a, 8));
    return _mm_extract_epi64_rpl(a, 0);
}


static inline void arr_store(
        __m128i *array,
        __m128i vH,
        int32_t t,
        int32_t seglen,
        int32_t d)
{
    _mm_store_si128(array + (1LL*d*seglen+t), vH);
}

static inline __m128i arr_load(
        __m128i *array,
        int32_t t,
        int32_t seglen,
        int32_t d)
{
    return _mm_load_si128(array + (1LL*d*seglen+t));
}

#define FNAME parasail_sw_trace_striped_sse41_128_64
#define PNAME parasail_sw_trace_striped_profile_sse41_128_64

parasail_result_t* FNAME(
        const char * const restrict s1, const int s1Len,
        const char * const restrict s2, const int s2Len,
        const int open, const int gap, const parasail_matrix_t *matrix)
{
    /* declare local variables */
    parasail_profile_t *profile = NULL;
    parasail_result_t *result = NULL;

    /* validate inputs */
    PARASAIL_CHECK_NULL(s1);
    PARASAIL_CHECK_GT0(s1Len);
    PARASAIL_CHECK_NULL(s2);
    PARASAIL_CHECK_GT0(s2Len);
    PARASAIL_CHECK_GE0(open);
    PARASAIL_CHECK_GE0(gap);
    PARASAIL_CHECK_NULL(matrix);

    /* initialize local variables */
    profile = parasail_profile_create_sse_128_64(s1, s1Len, matrix);
    if (!profile) return NULL;
    result = PNAME(profile, s2, s2Len, open, gap);

    parasail_profile_free(profile);

    return result;
}

parasail_result_t* PNAME(
        const parasail_profile_t * const restrict profile,
        const char * const restrict s2, const int s2Len,
        const int open, const int gap)
{
    /* declare local variables */
    int32_t i = 0;
    int32_t j = 0;
    int32_t k = 0;
    int32_t end_query = 0;
    int32_t end_ref = 0;
    int s1Len = 0;
    const parasail_matrix_t *matrix = NULL;
    int32_t segWidth = 0;
    int32_t segLen = 0;
    __m128i* restrict vProfile = NULL;
    __m128i* restrict pvHStore = NULL;
    __m128i* restrict pvHLoad = NULL;
    __m128i* restrict pvE = NULL;
    __m128i* restrict pvEaStore = NULL;
    __m128i* restrict pvEaLoad = NULL;
    __m128i* restrict pvHT = NULL;
    __m128i* restrict pvHMax = NULL;
    __m128i vGapO;
    __m128i vGapE;
    __m128i vZero;
    int64_t score = 0;
    __m128i vMaxH;
    __m128i vMaxHUnit;
    int64_t maxp = 0;
    parasail_result_t *result = NULL;
    __m128i vTZero;
    __m128i vTIns;
    __m128i vTDel;
    __m128i vTDiag;
    __m128i vTDiagE;
    __m128i vTInsE;
    __m128i vTDiagF;
    __m128i vTDelF;
    __m128i vTMask;
    __m128i vFTMask;

    /* validate inputs */
    PARASAIL_CHECK_NULL(profile);
    PARASAIL_CHECK_NULL(profile->profile64.score);
    PARASAIL_CHECK_NULL(profile->matrix);
    PARASAIL_CHECK_GT0(profile->s1Len);
    PARASAIL_CHECK_NULL(s2);
    PARASAIL_CHECK_GT0(s2Len);
    PARASAIL_CHECK_GE0(open);
    PARASAIL_CHECK_GE0(gap);

    /* initialize stack variables */
    i = 0;
    j = 0;
    k = 0;
    end_query = 0;
    end_ref = 0;
    s1Len = profile->s1Len;
    matrix = profile->matrix;
    segWidth = 2; /* number of values in vector unit */
    segLen = (s1Len + segWidth - 1) / segWidth;
    vProfile = (__m128i*)profile->profile64.score;
    vGapO = _mm_set1_epi64x_rpl(open);
    vGapE = _mm_set1_epi64x_rpl(gap);
    vZero = _mm_setzero_si128();
    score = NEG_INF;
    vMaxH = vZero;
    vMaxHUnit = vZero;
    maxp = INT64_MAX - (int64_t)(matrix->max+1);
    vTZero = _mm_set1_epi64x_rpl(PARASAIL_ZERO);
    vTIns  = _mm_set1_epi64x_rpl(PARASAIL_INS);
    vTDel  = _mm_set1_epi64x_rpl(PARASAIL_DEL);
    vTDiag = _mm_set1_epi64x_rpl(PARASAIL_DIAG);
    vTDiagE= _mm_set1_epi64x_rpl(PARASAIL_DIAG_E);
    vTInsE = _mm_set1_epi64x_rpl(PARASAIL_INS_E);
    vTDiagF= _mm_set1_epi64x_rpl(PARASAIL_DIAG_F);
    vTDelF = _mm_set1_epi64x_rpl(PARASAIL_DEL_F);
    vTMask = _mm_set1_epi64x_rpl(PARASAIL_ZERO_MASK);
    vFTMask= _mm_set1_epi64x_rpl(PARASAIL_F_MASK);

    /* initialize result */
    result = parasail_result_new_trace(segLen, s2Len, 16, sizeof(__m128i));
    if (!result) return NULL;

    /* set known flags */
    result->flag |= PARASAIL_FLAG_SW | PARASAIL_FLAG_STRIPED
        | PARASAIL_FLAG_TRACE
        | PARASAIL_FLAG_BITS_64 | PARASAIL_FLAG_LANES_2;

    /* initialize heap variables */
    pvHStore = parasail_memalign___m128i(16, segLen);
    pvHLoad =  parasail_memalign___m128i(16, segLen);
    pvE = parasail_memalign___m128i(16, segLen);
    pvEaStore = parasail_memalign___m128i(16, segLen);
    pvEaLoad = parasail_memalign___m128i(16, segLen);
    pvHT = parasail_memalign___m128i(16, segLen);
    pvHMax = parasail_memalign___m128i(16, segLen);

    /* validate heap variables */
    if (!pvHStore) return NULL;
    if (!pvHLoad) return NULL;
    if (!pvE) return NULL;
    if (!pvEaStore) return NULL;
    if (!pvEaLoad) return NULL;
    if (!pvHT) return NULL;
    if (!pvHMax) return NULL;

    /* initialize H and E */
    parasail_memset___m128i(pvHStore, vZero, segLen);
    parasail_memset___m128i(pvE, _mm_set1_epi64x_rpl(-open), segLen);
    parasail_memset___m128i(pvEaStore, _mm_set1_epi64x_rpl(-open), segLen);

    for (i=0; i<segLen; ++i) {
        arr_store(result->trace->trace_table, vTDiagE, i, segLen, 0);
    }

    /* outer loop over database sequence */
    for (j=0; j<s2Len; ++j) {
        __m128i vEF_opn;
        __m128i vE;
        __m128i vE_ext;
        __m128i vF;
        __m128i vF_ext;
        __m128i vFa;
        __m128i vFa_ext;
        __m128i vH;
        __m128i vH_dag;
        const __m128i* vP = NULL;

        /* Initialize F value to 0.  Any errors to vH values will be
         * corrected in the Lazy_F loop. */
        vF = _mm_sub_epi64(vZero,vGapO);

        /* load final segment of pvHStore and shift left by 8 bytes */
        vH = _mm_load_si128(&pvHStore[segLen - 1]);
        vH = _mm_slli_si128(vH, 8);

        /* Correct part of the vProfile */
        vP = vProfile + matrix->mapper[(unsigned char)s2[j]] * segLen;

        if (end_ref == j-2) {
            /* Swap in the max buffer. */
            SWAP3(pvHMax,  pvHLoad,  pvHStore)
            SWAP(pvEaLoad,  pvEaStore)
        }
        else {
            /* Swap the 2 H buffers. */
            SWAP(pvHLoad,  pvHStore)
            SWAP(pvEaLoad,  pvEaStore)
        }

        /* inner loop to process the query sequence */
        for (i=0; i<segLen; ++i) {
            vE = _mm_load_si128(pvE + i);

            /* Get max from vH, vE and vF. */
            vH_dag = _mm_add_epi64(vH, _mm_load_si128(vP + i));
            vH_dag = _mm_max_epi64_rpl(vH_dag, vZero);
            vH = _mm_max_epi64_rpl(vH_dag, vE);
            vH = _mm_max_epi64_rpl(vH, vF);
            /* Save vH values. */
            _mm_store_si128(pvHStore + i, vH);

            {
                __m128i vTAll = arr_load(result->trace->trace_table, i, segLen, j);
                __m128i cond_zero = _mm_cmpeq_epi64(vH, vZero);
                __m128i case1 = _mm_cmpeq_epi64(vH, vH_dag);
                __m128i case2 = _mm_cmpeq_epi64(vH, vF);
                __m128i vT = _mm_blendv_epi8(
                        _mm_blendv_epi8(vTIns, vTDel, case2),
                        _mm_blendv_epi8(vTDiag, vTZero, cond_zero),
                        case1);
                _mm_store_si128(pvHT + i, vT);
                vT = _mm_or_si128(vT, vTAll);
                arr_store(result->trace->trace_table, vT, i, segLen, j);
            }
            vMaxH = _mm_max_epi64_rpl(vH, vMaxH);
            vEF_opn = _mm_sub_epi64(vH, vGapO);

            /* Update vE value. */
            vE_ext = _mm_sub_epi64(vE, vGapE);
            vE = _mm_max_epi64_rpl(vEF_opn, vE_ext);
            _mm_store_si128(pvE + i, vE);
            {
                __m128i vEa = _mm_load_si128(pvEaLoad + i);
                __m128i vEa_ext = _mm_sub_epi64(vEa, vGapE);
                vEa = _mm_max_epi64_rpl(vEF_opn, vEa_ext);
                _mm_store_si128(pvEaStore + i, vEa);
                if (j+1<s2Len) {
                    __m128i cond = _mm_cmpgt_epi64_rpl(vEF_opn, vEa_ext);
                    __m128i vT = _mm_blendv_epi8(vTInsE, vTDiagE, cond);
                    arr_store(result->trace->trace_table, vT, i, segLen, j+1);
                }
            }

            /* Update vF value. */
            vF_ext = _mm_sub_epi64(vF, vGapE);
            vF = _mm_max_epi64_rpl(vEF_opn, vF_ext);
            if (i+1<segLen) {
                __m128i vTAll = arr_load(result->trace->trace_table, i+1, segLen, j);
                __m128i cond = _mm_cmpgt_epi64_rpl(vEF_opn, vF_ext);
                __m128i vT = _mm_blendv_epi8(vTDelF, vTDiagF, cond);
                vT = _mm_or_si128(vT, vTAll);
                arr_store(result->trace->trace_table, vT, i+1, segLen, j);
            }

            /* Load the next vH. */
            vH = _mm_load_si128(pvHLoad + i);
        }

        /* Lazy_F loop: has been revised to disallow adjecent insertion and
         * then deletion, so don't update E(i, i), learn from SWPS3 */
        vFa_ext = vF_ext;
        vFa = vF;
        for (k=0; k<segWidth; ++k) {
            __m128i vHp = _mm_load_si128(&pvHLoad[segLen - 1]);
            vHp = _mm_slli_si128(vHp, 8);
            vEF_opn = _mm_slli_si128(vEF_opn, 8);
            vEF_opn = _mm_insert_epi64_rpl(vEF_opn, -open, 0);
            vF_ext = _mm_slli_si128(vF_ext, 8);
            vF_ext = _mm_insert_epi64_rpl(vF_ext, NEG_INF, 0);
            vF = _mm_slli_si128(vF, 8);
            vF = _mm_insert_epi64_rpl(vF, -open, 0);
            vFa_ext = _mm_slli_si128(vFa_ext, 8);
            vFa_ext = _mm_insert_epi64_rpl(vFa_ext, NEG_INF, 0);
            vFa = _mm_slli_si128(vFa, 8);
            vFa = _mm_insert_epi64_rpl(vFa, -open, 0);
            for (i=0; i<segLen; ++i) {
                vH = _mm_load_si128(pvHStore + i);
                vH = _mm_max_epi64_rpl(vH,vF);
                _mm_store_si128(pvHStore + i, vH);
                {
                    __m128i vTAll;
                    __m128i vT;
                    __m128i case1;
                    __m128i case2;
                    __m128i cond;
                    vHp = _mm_add_epi64(vHp, _mm_load_si128(vP + i));
                    vHp = _mm_max_epi64_rpl(vHp, vZero);
                    case1 = _mm_cmpeq_epi64(vH, vHp);
                    case2 = _mm_cmpeq_epi64(vH, vF);
                    cond = _mm_andnot_si128(case1,case2);
                    vTAll = arr_load(result->trace->trace_table, i, segLen, j);
                    vT = _mm_load_si128(pvHT + i);
                    vT = _mm_blendv_epi8(vT, vTDel, cond);
                    _mm_store_si128(pvHT + i, vT);
                    vTAll = _mm_and_si128(vTAll, vTMask);
                    vTAll = _mm_or_si128(vTAll, vT);
                    arr_store(result->trace->trace_table, vTAll, i, segLen, j);
                }
                vMaxH = _mm_max_epi64_rpl(vH, vMaxH);
                /* Update vF value. */
                {
                    __m128i vTAll = arr_load(result->trace->trace_table, i, segLen, j);
                    __m128i cond = _mm_cmpgt_epi64_rpl(vEF_opn, vFa_ext);
                    __m128i vT = _mm_blendv_epi8(vTDelF, vTDiagF, cond);
                    vTAll = _mm_and_si128(vTAll, vFTMask);
                    vTAll = _mm_or_si128(vTAll, vT);
                    arr_store(result->trace->trace_table, vTAll, i, segLen, j);
                }
                vEF_opn = _mm_sub_epi64(vH, vGapO);
                vF_ext = _mm_sub_epi64(vF, vGapE);
                {
                    __m128i vEa = _mm_load_si128(pvEaLoad + i);
                    __m128i vEa_ext = _mm_sub_epi64(vEa, vGapE);
                    vEa = _mm_max_epi64_rpl(vEF_opn, vEa_ext);
                    _mm_store_si128(pvEaStore + i, vEa);
                    if (j+1<s2Len) {
                        __m128i cond = _mm_cmpgt_epi64_rpl(vEF_opn, vEa_ext);
                        __m128i vT = _mm_blendv_epi8(vTInsE, vTDiagE, cond);
                        arr_store(result->trace->trace_table, vT, i, segLen, j+1);
                    }
                }
                if (! _mm_movemask_epi8(
                            _mm_or_si128(
                                _mm_cmpgt_epi64_rpl(vF_ext, vEF_opn),
                                _mm_cmpeq_epi64(vF_ext, vEF_opn))))
                    goto end;
                /*vF = _mm_max_epi64_rpl(vEF_opn, vF_ext);*/
                vF = vF_ext;
                vFa_ext = _mm_sub_epi64(vFa, vGapE);
                vFa = _mm_max_epi64_rpl(vEF_opn, vFa_ext);
                vHp = _mm_load_si128(pvHLoad + i);
            }
        }
end:
        {
        }

        {
            __m128i vCompare = _mm_cmpgt_epi64_rpl(vMaxH, vMaxHUnit);
            if (_mm_movemask_epi8(vCompare)) {
                score = _mm_hmax_epi64_rpl(vMaxH);
                /* if score has potential to overflow, abort early */
                if (score > maxp) {
                    result->flag |= PARASAIL_FLAG_SATURATED;
                    break;
                }
                vMaxHUnit = _mm_set1_epi64x_rpl(score);
                end_ref = j;
            }
        }

        /*if (score == stop) break;*/
    }

    if (score == INT64_MAX) {
        result->flag |= PARASAIL_FLAG_SATURATED;
    }

    if (parasail_result_is_saturated(result)) {
        score = 0;
        end_query = 0;
        end_ref = 0;
    }
    else {
        if (end_ref == j-1) {
            /* end_ref was the last store column */
            SWAP(pvHMax,  pvHStore)
        }
        else if (end_ref == j-2) {
            /* end_ref was the last load column */
            SWAP(pvHMax,  pvHLoad)
        }
        /* Trace the alignment ending position on read. */
        {
            int64_t *t = (int64_t*)pvHMax;
            int32_t column_len = segLen * segWidth;
            end_query = s1Len - 1;
            for (i = 0; i<column_len; ++i, ++t) {
                if (*t == score) {
                    int32_t temp = i / segWidth + i % segWidth * segLen;
                    if (temp < end_query) {
                        end_query = temp;
                    }
                }
            }
        }
    }

    result->score = score;
    result->end_query = end_query;
    result->end_ref = end_ref;

    parasail_free(pvHMax);
    parasail_free(pvHT);
    parasail_free(pvEaLoad);
    parasail_free(pvEaStore);
    parasail_free(pvE);
    parasail_free(pvHLoad);
    parasail_free(pvHStore);

    return result;
}


