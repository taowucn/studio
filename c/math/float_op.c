#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <sys/time.h>

typedef unsigned int u_int32_t;

#if 0
typedef union
{
  double value;
  struct
  {
    u_int32_t msw;
    u_int32_t lsw;
  } parts;
} ieee_double_shape_type;
#else
typedef union
{
  double value;
  struct
  {
    u_int32_t lsw;
    u_int32_t msw;
  } parts;
} ieee_double_shape_type;
#endif

#define GET_HIGH_WORD(i,d)                                      \
do {                                                            \
	ieee_double_shape_type gh_u;                                  \
	gh_u.value = (d);                                             \
	(i) = gh_u.parts.msw;                                         \
} while (0)

/* Set the more significant 32 bits of a double from an int.  */

#define SET_HIGH_WORD(d,v)                                      \
do {                                                            \
	ieee_double_shape_type sh_u;                                  \
	sh_u.value = (d);                                             \
	sh_u.parts.msw = (v);                                         \
	(d) = sh_u.value;                                             \
} while (0)

#define GET_LOW_WORD(i,d)                                       \
 do {                                                            \
   ieee_double_shape_type gl_u;                                  \
   gl_u.value = (d);                                             \
   (i) = gl_u.parts.lsw;                                         \
 } while (0)

#define	STRICT_ASSIGN(type, lval, rval) do {	\
	volatile type __lval;			\
						\
	if (sizeof(type) >= sizeof(long double))	\
		(lval) = (rval);		\
	else {					\
		__lval = (rval);		\
		(lval) = __lval;		\
	}					\
} while (0)

#define INSERT_WORDS(d,ix0,ix1)					\
do {								\
  ieee_double_shape_type iw_u;					\
  iw_u.parts.msw = (ix0);					\
  iw_u.parts.lsw = (ix1);					\
  (d) = iw_u.value;						\
} while (0)


static const double
one	= 1.0,
halF[2]	= {0.5,-0.5,},
huge	= 1.0e+300,
o_threshold=  7.09782712893383973096e+02,  /* 0x40862E42, 0xFEFA39EF */
u_threshold= -7.45133219101941108420e+02,  /* 0xc0874910, 0xD52D3051 */
ln2HI[2]   ={ 6.93147180369123816490e-01,  /* 0x3fe62e42, 0xfee00000 */
	     -6.93147180369123816490e-01,},/* 0xbfe62e42, 0xfee00000 */
ln2LO[2]   ={ 1.90821492927058770002e-10,  /* 0x3dea39ef, 0x35793c76 */
	     -1.90821492927058770002e-10,},/* 0xbdea39ef, 0x35793c76 */
invln2 =  1.44269504088896338700e+00, /* 0x3ff71547, 0x652b82fe */
P1   =  1.66666666666666019037e-01, /* 0x3FC55555, 0x5555553E */
P2   = -2.77777777770155933842e-03, /* 0xBF66C16C, 0x16BEBD93 */
P3   =  6.61375632143793436117e-05, /* 0x3F11566A, 0xAF25DE2C */
P4   = -1.65339022054652515390e-06, /* 0xBEBBBD41, 0xC5D26BF1 */
P5   =  4.13813679705723846039e-08; /* 0x3E663769, 0x72BEA4D0 */

static volatile double
twom1000= 9.33263618503218878990e-302;     /* 2**-1000=0x01700000,0*/

static double ieee754_exp(double x)	/* default IEEE double exp */
{
	double y,hi=0.0,lo=0.0,c,t,twopk;
	int32_t k=0,xsb;
	u_int32_t hx;

	GET_HIGH_WORD(hx,x);
	xsb = (hx>>31)&1;		/* sign bit of x */
	hx &= 0x7fffffff;		/* high word of |x| */

    /* filter out non-finite argument */
	if(hx >= 0x40862E42) {			/* if |x|>=709.78... */
            if(hx>=0x7ff00000) {
	        u_int32_t lx;
		GET_LOW_WORD(lx,x);
		if(((hx&0xfffff)|lx)!=0)
		     return x+x; 		/* NaN */
		else return (xsb==0)? x:0.0;	/* exp(+-inf)={inf,0} */
	    }
	    if(x > o_threshold) return huge*huge; /* overflow */
	    if(x < u_threshold) return twom1000*twom1000; /* underflow */
	}

    /* argument reduction */
	if(hx > 0x3fd62e42) {		/* if  |x| > 0.5 ln2 */
	    if(hx < 0x3FF0A2B2) {	/* and |x| < 1.5 ln2 */
		hi = x-ln2HI[xsb]; lo=ln2LO[xsb]; k = 1-xsb-xsb;
	    } else {
		k  = (int)(invln2*x+halF[xsb]);
		t  = k;
		hi = x - t*ln2HI[0];	/* t*ln2HI is exact here */
		lo = t*ln2LO[0];
	    }
	    STRICT_ASSIGN(double, x, hi - lo);
	}
	else if(hx < 0x3e300000)  {	/* when |x|<2**-28 */
	    if(huge+x>one) return one+x;/* trigger inexact */
	}
	else k = 0;

    /* x is now in primary range */
	t  = x*x;
	if(k >= -1021)
	    INSERT_WORDS(twopk,0x3ff00000+(k<<20), 0);
	else
	    INSERT_WORDS(twopk,0x3ff00000+((k+1000)<<20), 0);
	c  = x - t*(P1+t*(P2+t*(P3+t*(P4+t*P5))));
	if(k==0) 	return one-((x*c)/(c-2.0)-x);
	else 		y = one-((lo-(x*c)/(2.0-c))-hi);
	if(k >= -1021) {
	    if (k==1024) return y*2.0*0x1p1023;
	    return y*twopk;
	} else {
	    return y*twopk*twom1000;
	}
}

static double i_exp(double x)
{
	int i,k,m,t;
	int xm=(int)x;
	double sum;
	double e ;
	double ef;
	double z ;
	double sub=x-xm;
	m=1;
	e=1.0;
	ef=1.0;
	t=10;
	z=1;
	sum=1;

	if (xm<0) {
		xm=(-xm);
		for(k=0;k<xm;k++){ef*=2.718281;}
		e/=ef;
	} else {
		for(k=0;k<xm;k++){e*=2.718281;}
	}

	for(i=1;i<t;i++){
		m*=i;
		z*=sub;
		sum+=z/m;
	}

	return sum*e;
}

int main(int argc, char *argv[])
{
	double a = 3.1415926f;
	double x = -3.1415926f;
	double b, c, d, e, f, g;
	int n;

	struct timeval tv1, tv2;

	unsigned int tv_diff1;
	unsigned int tv_diff2;

	b = sqrt(a);
	c = exp(a);
	d = floor(a);
	e = ceil(a);
	f = frexp(a, &n);
	g = fabs(x);
	printf("Original: sqrt=%f, exp=%f, floor=%f, ceil=%f, frexp=%f, n=%d, fabs=%f\n",
			b, c, d, e, f, n, g);

	gettimeofday(&tv1, NULL);
	c = i_exp(a);
	gettimeofday(&tv2, NULL);
	tv_diff1 = (tv2.tv_sec - tv1.tv_sec) * 1000000 +
		tv2.tv_usec - tv1.tv_usec;

	gettimeofday(&tv1, NULL);
	f = ieee754_exp(a);
	gettimeofday(&tv2, NULL);
	tv_diff1 = (tv2.tv_sec - tv1.tv_sec) * 1000000 +
		tv2.tv_usec - tv1.tv_usec;

	printf("My: i_exp=%f(%u), ieee_exp=%f(%u)\n", c, tv_diff1, f, tv_diff2);

	return 0;
}

