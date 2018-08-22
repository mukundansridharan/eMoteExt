//-----------------------------------------------------------------------------
//
//                   ** WARNING! ** 
//    This file was generated automatically by a tool.
//    Re-running the tool will overwrite this file.
//    You should copy this file to a custom location
//    before adding any customization in the copy to
//    prevent loss of your changes when the tool is
//    re-run.
//
//-----------------------------------------------------------------------------

#include "MathFuncs.h"
#include "MathFuncs_CMSIS_Support.h"
#include "arm_math.h"

using namespace CMSIS;

/**
 * @ingroup groupSupport
 */

q31_t clip_q63_to_q31(q63_t x)
{
	return ((q31_t) (x >> 32) != ((q31_t) x >> 31)) ? ((0x7FFFFFFF ^ ((q31_t) (x >> 63)))) : (q31_t) x;
}

/**
 * @brief Converts the elements of the floating-point vector to Q31 vector.
 * @param[in]       *pSrc points to the floating-point input vector
 * @param[out]      *pDst points to the Q31 output vector
 * @param[in]       blockSize length of the input vector
 * @return none.
 *
 *\par Description:
 * \par
 * The equation used for the conversion process is:
 *
 * <pre>
 * 	pDst[n] = (q31_t)(pSrc[n] * 2147483648);   0 <= n < blockSize.
 * </pre>
 * <b>Scaling and Overflow Behavior:</b>
 * \par
 * The function uses saturating arithmetic.
 * Results outside of the allowable Q31 range[0x80000000 0x7FFFFFFF] will be saturated.
 *
 * \note In order to apply rounding, the library should be rebuilt with the ROUNDING macro
 * defined in the preprocessor section of project options.
 */

void arm_float_to_q31(
  float32_t * pSrc,
  q31_t * pDst,
  uint32_t blockSize)
{
  float32_t *pIn = pSrc;                         /* Src pointer */
  uint32_t blkCnt;                               /* loop counter */

#ifdef ARM_MATH_ROUNDING

  float32_t in;

#endif /*      #ifdef ARM_MATH_ROUNDING        */

#if defined (ARM_MATH_DSP)

  /* Run the below code for Cortex-M4 and Cortex-M3 */

  /*loop Unrolling */
  blkCnt = blockSize >> 2U;

  /* First part of the processing with loop unrolling.  Compute 4 outputs at a time.
   ** a second loop below computes the remaining 1 to 3 samples. */
  while (blkCnt > 0U)
  {

#ifdef ARM_MATH_ROUNDING

    /* C = A * 32768 */
    /* convert from float to Q31 and then store the results in the destination buffer */
    in = *pIn++;
    in = (in * 2147483648.0f);
    in += in > 0.0f ? 0.5f : -0.5f;
    *pDst++ = clip_q63_to_q31((q63_t) (in));

    in = *pIn++;
    in = (in * 2147483648.0f);
    in += in > 0.0f ? 0.5f : -0.5f;
    *pDst++ = clip_q63_to_q31((q63_t) (in));

    in = *pIn++;
    in = (in * 2147483648.0f);
    in += in > 0.0f ? 0.5f : -0.5f;
    *pDst++ = clip_q63_to_q31((q63_t) (in));

    in = *pIn++;
    in = (in * 2147483648.0f);
    in += in > 0.0f ? 0.5f : -0.5f;
    *pDst++ = clip_q63_to_q31((q63_t) (in));

#else

    /* C = A * 2147483648 */
    /* convert from float to Q31 and then store the results in the destination buffer */
    *pDst++ = clip_q63_to_q31((q63_t) (*pIn++ * 2147483648.0f));
    *pDst++ = clip_q63_to_q31((q63_t) (*pIn++ * 2147483648.0f));
    *pDst++ = clip_q63_to_q31((q63_t) (*pIn++ * 2147483648.0f));
    *pDst++ = clip_q63_to_q31((q63_t) (*pIn++ * 2147483648.0f));

#endif /*      #ifdef ARM_MATH_ROUNDING        */

    /* Decrement the loop counter */
    blkCnt--;
  }

  /* If the blockSize is not a multiple of 4, compute any remaining output samples here.
   ** No loop unrolling is used. */
  blkCnt = blockSize % 0x4U;

  while (blkCnt > 0U)
  {

#ifdef ARM_MATH_ROUNDING

    /* C = A * 2147483648 */
    /* convert from float to Q31 and then store the results in the destination buffer */
    in = *pIn++;
    in = (in * 2147483648.0f);
    in += in > 0.0f ? 0.5f : -0.5f;
    *pDst++ = clip_q63_to_q31((q63_t) (in));

#else

    /* C = A * 2147483648 */
    /* convert from float to Q31 and then store the results in the destination buffer */
    *pDst++ = clip_q63_to_q31((q63_t) (*pIn++ * 2147483648.0f));

#endif /*      #ifdef ARM_MATH_ROUNDING        */

    /* Decrement the loop counter */
    blkCnt--;
  }


#else

  /* Run the below code for Cortex-M0 */

  /* Loop over blockSize number of values */
  blkCnt = blockSize;

  while (blkCnt > 0U)
  {

#ifdef ARM_MATH_ROUNDING

    /* C = A * 2147483648 */
    /* convert from float to Q31 and then store the results in the destination buffer */
    in = *pIn++;
    in = (in * 2147483648.0f);
    in += in > 0 ? 0.5f : -0.5f;
    *pDst++ = clip_q63_to_q31((q63_t) (in));

#else

    /* C = A * 2147483648 */
    /* convert from float to Q31 and then store the results in the destination buffer */
    *pDst++ = clip_q63_to_q31((q63_t) (*pIn++ * 2147483648.0f));

#endif /*      #ifdef ARM_MATH_ROUNDING        */

    /* Decrement the loop counter */
    blkCnt--;
  }

#endif /* #if defined (ARM_MATH_DSP) */

}


/**
 * @brief Converts the elements of the Q31 vector to floating-point vector.
 * @param[in]       *pSrc points to the Q31 input vector
 * @param[out]      *pDst points to the floating-point output vector
 * @param[in]       blockSize length of the input vector
 * @return none.
 *
 * \par Description:
 *
 * The equation used for the conversion process is:
 *
 * <pre>
 * 	pDst[n] = (float32_t) pSrc[n] / 2147483648;   0 <= n < blockSize.
 * </pre>
 *
 */

void arm_q31_to_float(
  q31_t * pSrc,
  float32_t * pDst,
  uint32_t blockSize)
{
  q31_t *pIn = pSrc;                             /* Src pointer */
  uint32_t blkCnt;                               /* loop counter */


#if defined (ARM_MATH_DSP)

  /* Run the below code for Cortex-M4 and Cortex-M3 */

  /*loop Unrolling */
  blkCnt = blockSize >> 2U;

  /* First part of the processing with loop unrolling.  Compute 4 outputs at a time.
   ** a second loop below computes the remaining 1 to 3 samples. */
  while (blkCnt > 0U)
  {
    /* C = (float32_t) A / 2147483648 */
    /* convert from q31 to float and then store the results in the destination buffer */
    *pDst++ = ((float32_t) * pIn++ / 2147483648.0f);
    *pDst++ = ((float32_t) * pIn++ / 2147483648.0f);
    *pDst++ = ((float32_t) * pIn++ / 2147483648.0f);
    *pDst++ = ((float32_t) * pIn++ / 2147483648.0f);

    /* Decrement the loop counter */
    blkCnt--;
  }

  /* If the blockSize is not a multiple of 4, compute any remaining output samples here.
   ** No loop unrolling is used. */
  blkCnt = blockSize % 0x4U;

#else

  /* Run the below code for Cortex-M0 */

  /* Loop over blockSize number of values */
  blkCnt = blockSize;

#endif /* #if defined (ARM_MATH_DSP) */

  while (blkCnt > 0U)
  {
    /* C = (float32_t) A / 2147483648 */
    /* convert from q31 to float and then store the results in the destination buffer */
    *pDst++ = ((float32_t) * pIn++ / 2147483648.0f);

    /* Decrement the loop counter */
    blkCnt--;
  }
}

INT32 Support::ConvertFloatToInt( float param0, HRESULT &hr )
{
    q31_t retVal;
    arm_float_to_q31(&param0, &retVal, 1);
    return retVal;
}

float Support::ConvertIntToFloat( INT32 param0, HRESULT &hr )
{
    float retVal;
    arm_q31_to_float((q31_t*) &param0, &retVal, 1);
    return retVal;
}