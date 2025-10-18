/* norasearch.c
 *
 * norasearch(hay text, needle text, minmatch int)
 * Returns int[][] of {offset, count} where count > minmatch.
 *
 * Compiles with PostgreSQL 16+ (C11).
 */

#include "postgres.h"
#include "fmgr.h"
#include "utils/array.h"
#include "utils/builtins.h"
#include "catalog/pg_type_d.h"   /* for INT4OID */

PG_MODULE_MAGIC;
PG_FUNCTION_INFO_V1(norasearch);

Datum
norasearch(PG_FUNCTION_ARGS)
{
    /* Interpret first arg as haystack (big string), second as needle (pattern) */
    text  *hay_text    = PG_GETARG_TEXT_PP(0);
    text  *needle_text = PG_GETARG_TEXT_PP(1);
    int32  minmatch    = PG_GETARG_INT32(2);

    if (minmatch < 0)
        minmatch = 0;

    const char *hay = text_to_cstring(hay_text);
    const char *needle = text_to_cstring(needle_text);

    int hay_len = (int) strlen(hay);
    int needle_len = (int) strlen(needle);

    if (hay_len == 0 || needle_len == 0)
        PG_RETURN_NULL();

    /* We index by byte value 0..255 */
    const int VALS = 256;

    /* Count occurrences to know allocation sizes */
    int *needleCount = (int *) palloc0(sizeof(int) * VALS);
    int *hayCount    = (int *) palloc0(sizeof(int) * VALS);

    for (int i = 0; i < needle_len; i++)
        needleCount[(unsigned char)needle[i]]++;
    for (int i = 0; i < hay_len; i++)
        hayCount[(unsigned char)hay[i]]++;

    /* Allocate arrays of int* for positions (or NULL if none) */
    int **needleIdx = (int **) palloc0(sizeof(int *) * VALS);
    int **hayIdx    = (int **) palloc0(sizeof(int *) * VALS);

    /* prepare writable position counters (reuse counts as current-pos) */
    int *needlePos = (int *) palloc0(sizeof(int) * VALS);
    int *hayPos    = (int *) palloc0(sizeof(int) * VALS);

    for (int v = 0; v < VALS; v++)
    {
        if (needleCount[v] > 0)
            needleIdx[v] = (int *) palloc(sizeof(int) * needleCount[v]);
        if (hayCount[v] > 0)
            hayIdx[v] = (int *) palloc(sizeof(int) * hayCount[v]);

        /* needlePos and hayPos initialized to 0 by palloc0 */
    }

    /* Fill positions */
    for (int i = 0; i < needle_len; i++)
    {
        unsigned char c = (unsigned char)needle[i];
        needleIdx[c][needlePos[c]++] = i;
    }

    for (int i = 0; i < hay_len; i++)
    {
        unsigned char c = (unsigned char)hay[i];
        hayIdx[c][hayPos[c]++] = i;
    }

    /* distancesPositive sized to max possible offset = hay_len - 1.
       We allocate hay_len (so index up to hay_len-1). */
    int maxOffset = hay_len; /* we will ignore offsets >= hay_len */
    int *distancesPositive = (int *) palloc0(sizeof(int) * maxOffset);

    /* Compute offsets: for each byte value v, for each needle position w and hay position x:
       offset = x - w ; if offset >= 0 and offset < maxOffset, increment counter */
    for (int v = 0; v < VALS; v++)
    {
        if (needleIdx[v] != NULL && hayIdx[v] != NULL)
        {
            int nc = needlePos[v]; /* number of positions in needle for v */
            int hc = hayPos[v];    /* number of positions in hay for v */

            for (int a = 0; a < nc; a++)
            {
                int w = needleIdx[v][a]; /* position in needle */
                for (int b = 0; b < hc; b++)
                {
                    int x = hayIdx[v][b]; /* position in hay */
                    int dist = x - w;
                    if (dist >= 0 && dist < maxOffset)
                        distancesPositive[dist]++;
                }
            }
        }
    }

    /* Count how many offsets exceed minmatch */
    int resultCount = 0;
    for (int off = 0; off < maxOffset; off++)
    {
        if (distancesPositive[off] > minmatch)
            resultCount++;
    }

    if (resultCount == 0)
        PG_RETURN_NULL();

    /* Build a 2D array with dimensions [resultCount x 2] */
    int dims[2] = { resultCount, 2 };
    int lbs[2]  = { 1, 1 };

    Datum *elems = palloc_array(Datum, resultCount * 2);
    int pos = 0;
    for (int off = 0; off < maxOffset; off++)
    {
        int cnt = distancesPositive[off];
        if (cnt > minmatch)
        {
            elems[pos++] = Int32GetDatum(off);
            elems[pos++] = Int32GetDatum(cnt);
        }
    }

    ArrayType *result = construct_md_array(
        elems,
        NULL,       /* no nulls */
        2,          /* number of dims */
        dims,
        lbs,
        INT4OID,
        sizeof(int32),
        true,       /* typbyval */
        'i'         /* typalign */
    );

    PG_RETURN_ARRAYTYPE_P(result);
}
