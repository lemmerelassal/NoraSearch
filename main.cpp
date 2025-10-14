#include "postgres.h"
#include "fmgr.h"
#include "utils/array.h"
#include "utils/builtins.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

// Declare the SQL-callable function
PG_FUNCTION_INFO_V1(norasearch);

Datum norasearch(PG_FUNCTION_ARGS);

Datum
norasearch(PG_FUNCTION_ARGS)
{
    text *strText = PG_GETARG_TEXT_PP(0);
    text *substrText = PG_GETARG_TEXT_PP(1);
    int32 minmatch = PG_GETARG_INT32(2);

    // Convert PostgreSQL text to C strings
    char *str = text_to_cstring(strText);
    char *substr = text_to_cstring(substrText);

    // Preallocate arrays (512 ASCII possible characters)
    int indexSub[512][512];
    int lenSub[512] = {0};
    int indexStr[512][512];
    int lenStr[512] = {0};
    int distancesPositive[512] = {0};

    int len1 = strlen(str);
    int len2 = strlen(substr);

    // Fill indexSub
    for (int i = 0; i < len2; i++) {
        unsigned char c = (unsigned char)substr[i];
        indexSub[c][lenSub[c]++] = i;
    }

    // Fill indexStr
    for (int i = 0; i < len1; i++) {
        unsigned char c = (unsigned char)str[i];
        indexStr[c][lenStr[c]++] = i;
    }

    // Count matching offsets
    for (int ch = 0; ch < 512; ch++) {
        if (lenSub[ch] > 0) {
            for (int a = 0; a < lenSub[ch]; a++) {
                int w = indexSub[ch][a];
                for (int b = 0; b < lenStr[ch]; b++) {
                    int x = indexStr[ch][b];
                    int dist = x - w;
                    if (dist >= 0 && dist < 512)
                        distancesPositive[dist]++;
                }
            }
        }
    }

    // Collect results
    int results[512][2];
    int nResults = 0;

    for (int offset = 0; offset < 512; offset++) {
        int count = distancesPositive[offset];
        if (count > minmatch) {
            results[nResults][0] = offset;
            results[nResults][1] = count;
            nResults++;
        }
    }

    // Build a 2D PostgreSQL array result (int4[][])
    if (nResults == 0)
        PG_RETURN_NULL();

    int ndim = 2;
    int dims[2] = {nResults, 2};
    int lbs[2] = {1, 1};

    int *flat = (int *) palloc(sizeof(int) * nResults * 2);
    for (int i = 0; i < nResults; i++) {
        flat[i * 2] = results[i][0];
        flat[i * 2 + 1] = results[i][1];
    }

    ArrayType *resultArray = construct_md_array(
        (Datum *)flat,
        NULL,
        ndim,
        dims,
        lbs,
        INT4OID,
        sizeof(int),
        true, // int is pass-by-value
        'i'
    );

    PG_RETURN_ARRAYTYPE_P(resultArray);
}
