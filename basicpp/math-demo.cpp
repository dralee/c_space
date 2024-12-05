/*
 * mathc lib method
 * 2024.12.05 by dralee
 */
#include <cmath>
#include <iostream>
using namespace std;

const double PI = 3.141592653589793;

void demo_1() {
    cout << "\n===========> abs:" << endl;
    cout << abs(-4.7) << endl;
    cout << abs(4.7) << endl;

	// returns the sine of x (x is in radians)
	cout<<"\n===========> sin:"<<endl;
	cout<<"3:"<<sin(3)<<endl;
	cout<<"-3:"<<sin(-3)<<endl;
	cout<<"0:"<<sin(0)<<endl;
	cout<<"PI:"<<sin(PI)<<endl;
	cout<<"PI/2.0:"<<sin(PI/2.0)<<endl;

	// returns the hyperbolic sine of x
	cout<<"\n===========> sinh:"<<endl;
	cout<<"7:"<<sinh(7)<<endl;
	cout<<"56:"<<sinh(56)<<endl;
	cout<<"2.45:"<<sinh(2.45)<<endl;



	// returns the arccosine of x, in radians
    cout << "\n===========> acos:" << endl;
    cout << "0.64:" << acos(0.64) << endl;
    cout << "-0.4:" << acos(-0.4) << endl;
    cout << "0:" << acos(0) << endl;
    cout << "1:" << acos(1) << endl;
    cout << "-1:" << acos(-1) << endl;
    cout << "2:" << acos(2) << endl;

	// returns the hyperbolic arccosine of x
	cout<<"\n===========> acosh:"<<endl;
	cout<<"7:"<<acosh(7)<<endl;
	cout<<"56:"<<acosh(56)<<endl;
	cout<<"2.45:"<<acosh(2.45)<<endl;

	// returns the arcsine of x, in radians
	cout<<"\n===========> asin:"<<endl;
	cout<<"0.64:"<<asin(0.64)<<endl;
	cout<<"-0.4:"<<asin(-0.4)<<endl;
	cout<<"0:"<<asin(0)<<endl;
	cout<<"1:"<<asin(1)<<endl;
	cout<<"-1:"<<asin(-1)<<endl;
	cout<<"2:"<<asin(2)<<endl;

	// returns the hyperbolic arcsine of x
	cout<<"\n===========> asinh:"<<endl;
	cout<<"7:"<<asinh(7)<<endl;
	cout<<"56:"<<asinh(56)<<endl;
	cout<<"2.45:"<<asinh(2.45)<<endl;

	// returns the tangent of x (x is in radians)
	cout<<"\n===========> tan:"<<endl;
	cout<<"3:"<<tan(3)<<endl;
	cout<<"-3:"<<tan(-3)<<endl;
	cout<<"0:"<<tan(0)<<endl;
	cout<<"PI:"<<tan(PI)<<endl;
	cout<<"PI/2.0:"<<tan(PI/2.0)<<endl;

	// returns the hyperbolic tangent of x
	cout<<"\n===========> tanh:"<<endl;
	cout<<"3:"<<tanh(3)<<endl;
	cout<<"-3:"<<tanh(-3)<<endl;
	cout<<"0:"<<tanh(0)<<endl;
	cout<<"1:"<<tan(1)<<endl;
	// returns the arctangent of x as a numeric value between -PI/2 and PI/2 radians
	cout<<"\n===========> atan:"<<endl;
	cout<<"0.5:"<<atan(0.5)<<endl;
	cout<<"-0.5:"<<atan(-0.5)<<endl;
	cout<<"5:"<<atan(5)<<endl;
	cout<<"-5:"<<atan(-5)<<endl;
	cout<<"100:"<<atan(100)<<endl;
	cout<<"-100:"<<atan(-100)<<endl;

	// returns the angle theta from the conversion of rectangular coordinates (x, y) to polar coordinates (r, theta)
	cout<<"\n===========> atan2:"<<endl;
	cout<<"0.5,0.5:"<<atan2(0.5,0.5)<<endl;
	cout<<"-0.5,-0.5:"<<atan2(-0.5,-0.5)<<endl;
	cout<<"5,5:"<<atan2(5,5)<<endl;
	cout<<"-5,-5:"<<atan2(-5,-5)<<endl;
	cout<<"10,10:"<<atan2(10,10)<<endl;
	cout<<"-10,-10:"<<atan2(-10,-10)<<endl;

	// returns the hyperbolic arctangent of x
	cout<<"\n===========> atanh:"<<endl;
	cout<<"0.78:"<<atanh(0.78)<<endl;
	cout<<"0.5:"<<atanh(0.5)<<endl;
	cout<<"1:"<<atanh(1)<<endl;
}

void demo_2() {
	// returns the cube root of x	
	cout<<"\n===========> cbrt:"<<endl;
	cout<<"0:"<<cbrt(0)<<endl;
	cout<<"0.64:"<<cbrt(0.64)<<endl;
	cout<<"1:"<<cbrt(1)<<endl;
	cout<<"27:"<<cbrt(27)<<endl;
	cout<<"-27:"<<cbrt(-27)<<endl;

	// returns the value of x rounded up to its nearest integer
	cout<<"\n===========> ceil:"<<endl;
	cout<<"0.60:"<<ceil(0.60)<<endl;
	cout<<"0.40:"<<ceil(0.40)<<endl;
	cout<<"5:"<<ceil(5)<<endl;
	cout<<"5.1:"<<ceil(5.1)<<endl;
	cout<<"-5.1:"<<ceil(-5.1)<<endl;
	cout<<"-5.9:"<<ceil(-5.9)<<endl;

	// returns the first floating point x with the sign of the second floating point y
	cout<<"\n===========> copysign:"<<endl;
	cout<<"0.5,0.6:"<<copysign(0.5,0.6)<<endl;
	cout<<"-0.5,-0.6:"<<copysign(-0.5,-0.6)<<endl;
	cout<<"5,6:"<<copysign(5,6)<<endl;
	cout<<"-5,-6:"<<copysign(-5,-6)<<endl;

	// returns the cosine of x (x is in radians)
	cout<<"\n===========> cos:"<<endl;
	cout<<"3:"<<cos(3)<<endl;
	cout<<"-3:"<<cos(-3)<<endl;
	cout<<"0:"<<cos(0)<<endl;
	cout<<"PI:"<<cos(PI)<<endl;
	cout<<"PI/2.0:"<<cos(PI/2.0)<<endl;

	// returns the hyperbolic cosine of x
	cout<<"\n===========> cosh:"<<endl;
	cout<<"7:"<<cosh(7)<<endl;
	cout<<"56:"<<cosh(56)<<endl;
	cout<<"2.45:"<<cosh(2.45)<<endl;



}

void demo_3() {
	// returns the value of Ex
	cout<<"\n===========> exp:"<<endl;
	cout<<"0:"<<exp(0)<<endl;
	cout<<"1:"<<exp(1)<<endl;
	cout<<"10:"<<exp(10)<<endl;
	cout<<"4.8:"<<exp(4.8)<<endl;
	cout<<"2.718:"<<exp(2.718)<<endl;

	// returns the value of 2x
	cout<<"\n===========> exp2:"<<endl;
	cout<<"0:"<<exp2(0)<<endl;
	cout<<"1:"<<exp2(1)<<endl;
	cout<<"10:"<<exp2(10)<<endl;
	cout<<"4.8:"<<exp2(4.8)<<endl;
	cout<<"2.718:"<<exp2(2.718)<<endl;

	// returns the value of ex-1
	cout<<"\n===========> expm1:"<<endl;
	cout<<"0:"<<expm1(0)<<endl;
	cout<<"1:"<<expm1(1)<<endl;
	cout<<"10:"<<expm1(10)<<endl;
	cout<<"4.8:"<<expm1(4.8)<<endl;
	cout<<"2.718:"<<expm1(2.718)<<endl;

	// returns the value of the error function at x
	cout<<"\n===========> erf:"<<endl;
	cout<<"0:"<<erf(0)<<endl;
	cout<<"1:"<<erf(1)<<endl;
	cout<<"10:"<<erf(10)<<endl;
	cout<<"4.8:"<<erf(4.8)<<endl;
	cout<<"2.718:"<<erf(2.718)<<endl;

	// returns the value of the complementary error function at x 
	cout<<"\n===========> erfc:"<<endl;
	cout<<"0:"<<erfc(0)<<endl;
	cout<<"1:"<<erfc(1)<<endl;
	cout<<"10:"<<erfc(10)<<endl;
	cout<<"4.8:"<<erfc(4.8)<<endl;
	cout<<"2.718:"<<erfc(2.718)<<endl;

}


void demo_4() {
	// returns the absolute value of a floating x
	cout<<"\n===========> fabs:"<<endl;
	cout<<"-4.7:"<<fabs(-4.7)<<endl;
	cout<<"4.7:"<<fabs(4.7)<<endl;
	cout<<"3:"<<fabs(3)<<endl;

	// returns the positive difference between x and y
	cout<<"\n===========> fdim:"<<endl;
	cout<<"-4.7,4.5:"<<fdim(-4.7,4.5)<<endl;
	cout<<"4.7,-4.5:"<<fdim(4.7,-4.5)<<endl;
	cout<<"0.42,-0.5:"<<fdim(0.42,-0.5)<<endl;
	cout<<"-0.42,-0.8:"<<fdim(-0.42,-0.8)<<endl;

	// returns the value of x rounded down to its nearest integer
	cout<<"\n===========> floor:"<<endl;
	cout<<"0.60:"<<floor(0.60)<<endl;
	cout<<"0.40:"<<floor(0.40)<<endl;
	cout<<"5:"<<floor(5)<<endl;
	cout<<"5.1:"<<floor(5.1)<<endl;
	cout<<"-5.1:"<<floor(-5.1)<<endl;
	cout<<"-5.9:"<<floor(-5.9)<<endl;

	// returns x*y+z without losing precision
	cout<<"\n===========> fma:"<<endl;
	cout<<"0.5,0.75,1.0:"<<fma(0.5,0.75,1.0)<<endl;
	cout<<"3,3,1:"<<fma(3,3,1)<<endl;
	cout<<"2,1.5,1:"<<fma(2,1.5,1)<<endl;

	// returns the highest value of a floating x and y
	cout<<"\n===========> fmax:"<<endl;
	cout<<"2.0,0.25:"<<fmax(2.0,0.25)<<endl;
	cout<<"31.2f,18.0f:"<<fmax(31.2f,18.0f)<<endl;
	cout<<"14,22:"<<fmax(14,22)<<endl;
	cout<<"96,2048:"<<fmax(96,2048)<<endl;

	// returns the lowest value of a floating x and y
	cout<<"\n===========> fmin:"<<endl;
	cout<<"2.0,0.25:"<<fmin(2.0,0.25)<<endl;
	cout<<"31.2f,18.0f:"<<fmin(31.2f,18.0f)<<endl;
	cout<<"14,22:"<<fmin(14,22)<<endl;
	cout<<"96,2048:"<<fmin(96,2048)<<endl;

	// returns the floating point remainder of x/y
	cout<<"\n===========> fmod:"<<endl;
	cout<<"11.0,3.0:"<<fmod(11.0,3.0)<<endl;
	cout<<"16.0f,4.0f:"<<fmod(16.0f,4.0f)<<endl;
	cout<<"31.0,2.5:"<<fmod(31.0,2.5)<<endl;

	// returns the decimal part of x and writes the integer part to the memory at the pointer y
	cout<<"\n===========> modf:"<<endl;
	float f;
	cout<<"11.0:"<<modf(11.0,&f)<<"==>"<<f<<endl;
	cout<<"16.0f:"<<modf(16.0f,&f)<<"==>"<<f<<endl;
	cout<<"31.0:"<<modf(31.0,&f)<<"==>"<<f<<endl;

	// with x expressed as m*2n, returns the value of m (a value between 0.5 and 1.0) and writes the value of n to the memory at the pointer y
	cout<<"\n===========> frexp:"<<endl;
	int r;
	cout<<"2.5:"<<frexp(2.5, &r)<<"==>"<<r<<endl;
	cout<<"52.9:"<<frexp(52.9, &r)<<"=>"<<r<<endl;

	// returns sqrt(x2 +y2) without intermediate overflow or underflow
	cout<<"\n===========> hypot:"<<endl;
	cout<<"3.0f,4.0f:"<<hypot(3.0f, 4.0f)<<endl;
	cout<<"1.0,1.0:"<<hypot(1.0,1.0)<<endl;
	cout<<"1.0,10.0:"<<hypot(1.0,10.0)<<endl;

	// returns the integer part of the floating-point base logarithm of x
	cout<<"\n===========> ilogb:"<<endl;
	cout<<"1:"<<ilogb(1)<<endl;
	cout<<"12.8:"<<ilogb(12.8)<<endl;
	cout<<"-2.9:"<<ilogb(-2.9)<<endl;
	cout<<"10:"<<ilogb(10)<<endl;

	// returns x*2y
	cout<<"\n===========> ldexp:"<<endl;
	cout<<"2,3:"<<ldexp(2,3)<<endl;
	cout<<"3.5,-12:"<<ldexp(3.5,-12)<<endl;

	// returns the value of the gamma function at x
	cout<<"\n===========> tgamma:"<<endl;
	cout<<"2:"<<tgamma(2)<<endl;
	cout<<"5.1:"<<tgamma(5.1)<<endl;
	cout<<"-12.5:"<<tgamma(-12.5)<<endl;
	
	// returns the logarithm of the absolute value of the gamma function at x
	cout<<"\n===========> lgamma:"<<endl;
	cout<<"2:"<<lgamma(2)<<endl;
	cout<<"5.1:"<<lgamma(5.1)<<endl;
	cout<<"-12.5:"<<lgamma(-12.5)<<endl;

	// rounds x to a nearby integer and returns the result as a long long integer
	cout<<"\n===========> llrint:"<<endl;
	cout<<"1.522:"<<llrint(1.522)<<endl;
	cout<<"-2.52:"<<llrint(-2.52)<<endl;
	cout<<"23.12:"<<llrint(23.12)<<endl;

	// rounds x to the nearest integer and returns the result as a long long integer
	cout<<"\n===========> llround:"<<endl;
	cout<<"1.522:"<<llround(1.522)<<endl;
	cout<<"-2.52:"<<llround(-2.52)<<endl;
	cout<<"23.12:"<<llround(23.12)<<endl;

	// rounds x to a nearby integer and returns the result as a long integer
	cout<<"\n===========> lrint:"<<endl;
	cout<<"1.522:"<<lrint(1.522)<<endl;
	cout<<"-2.52:"<<lrint(-2.52)<<endl;
	cout<<"23.12:"<<lrint(23.12)<<endl;

	// rounds x to the nearest integer and returns the result as a long integer
	cout<<"\n===========> lround:"<<endl;
	cout<<"1.522:"<<lround(1.522)<<endl;
	cout<<"-2.52:"<<lround(-2.52)<<endl;
	cout<<"23.12:"<<lround(23.12)<<endl;

	// returns x rounded to a nearby integer
	cout<<"\n===========> rint:"<<endl;
	cout<<"1.522:"<<rint(1.522)<<endl;
	cout<<"-2.52:"<<rint(-2.52)<<endl;
	cout<<"23.12:"<<rint(23.12)<<endl;
	
	// returns x rounded to the nearest integer
	cout<<"\n===========> round:"<<endl;
	cout<<"1.522:"<<round(1.522)<<endl;
	cout<<"-2.52:"<<round(-2.52)<<endl;
	cout<<"23.12:"<<round(23.12)<<endl;

	// returns the natural logarithm of x
	const double E = 2.718281828;
	cout<<"\n===========> log:"<<endl;
	cout<<"6.0f:"<<log(6.0f)<<endl;
	cout<<"E:"<<log(E)<<endl;
	cout<<"2.0:"<<log(2.0)<<endl;
	cout<<"1.0:"<<log(1.0)<<endl;
	cout<<"0.0:"<<log(0.0)<<endl;
	cout<<"-1.0f:"<<log(-1.0f)<<endl;

	// returns the base 10 logarithm of x
	cout<<"\n===========> log10:"<<endl;
	cout<<"120.0f:"<<log10(120.0f)<<endl;
	cout<<"10.0f:"<<log10(10.0f)<<endl;
	cout<<"3.1623:"<<log10(3.1623)<<endl;
	cout<<"1.0:"<<log10(1.0)<<endl;
	cout<<"0.0f:"<<log10(0.0f)<<endl;
	cout<<"-1.0f:"<<log10(-1.0f)<<endl;

	// returns the natural logarithm of x+1
	cout<<"\n===========> log1p:"<<endl;
	cout<<"120.0f:"<<log1p(120.0f)<<endl;
	cout<<"10.0f:"<<log1p(10.0f)<<endl;
	cout<<"3.1623:"<<log1p(3.1623)<<endl;
	cout<<"1.0:"<<log1p(1.0)<<endl;
	cout<<"0.0f:"<<log1p(0.0f)<<endl;
	cout<<"-1.0f:"<<log1p(-1.0f)<<endl;

	// returns the base 2 logarithm of the absolute value of x
	cout<<"\n===========> log2:"<<endl;
	cout<<"64.0:"<<log2(64.0)<<endl;
	cout<<"10.0f:"<<log2(10.0f)<<endl;
	cout<<"3.1623:"<<log2(3.1623)<<endl;
	cout<<"1.0:"<<log2(1.0)<<endl;
	cout<<"0.0f:"<<log2(0.0f)<<endl;
	cout<<"-1.0f:"<<log2(-1.0f)<<endl;

	// returns the floating-point base logarithm of the absolute value of x	
	cout<<"\n===========> logb:"<<endl;
	cout<<"64.0:"<<logb(64.0)<<endl;
	cout<<"10.0f:"<<logb(10.0f)<<endl;
	cout<<"3.1623:"<<logb(3.1623)<<endl;
	cout<<"1.0:"<<logb(1.0)<<endl;
	cout<<"0.0f:"<<logb(0.0f)<<endl;
	cout<<"-1.0f:"<<logb(-1.0f)<<endl;



}

void demo_5() {
	// returns a NaN (Not a Number) value
	cout<<"\n===========> nan:"<<endl;
    char ns[32];	
	cout<<nan(ns)<<"==>"<<ns<<endl;

	// returns x rounded to a nearby integer
	cout<<"\n===========> nearbyint:"<<endl;
	cout<<"1.212:"<<nearbyint(1.212)<<endl;
	cout<<"-1.92:"<<nearbyint(-1.92)<<endl;
	cout<<"-2.32:"<<nearbyint(-2.32)<<endl;

	// returns the closest floating point number to x in the direction of y
	cout<<"\n===========> nextafter:"<<endl;
	cout<<"1.212,1.0f:"<<nextafter(1.212,1.0f)<<endl;
	cout<<"-1.492,-1.0f:"<<nextafter(-1.492,-1.0f)<<endl;
	cout<<"-2.32,-2.1:"<<nextafter(-2.32,-2.1)<<endl;

	// returns the closest floating point number to x in the direction of y
	cout<<"\n===========> nexttoward:"<<endl;
	cout<<"1.212,1.0f:"<<nexttoward(1.212,1.0f)<<endl;
	cout<<"-1.492,-1.0f:"<<nexttoward(-1.492,-1.0f)<<endl;
	cout<<"-2.32,-2.1:"<<nexttoward(-2.32,-2.1)<<endl;

	// returns the value of x to the power of y
	cout<<"\n===========> pow:"<<endl;
	cout<<"2.0f,8.0f:"<<pow(2.0,8.0f)<<endl;
	cout<<"3.0f,4.0f:"<<pow(3.0f,4.0f)<<endl;
	cout<<"9.0,0.5:"<<pow(9.0,0.5)<<endl;
	cout<<"8.0,-1.0:"<<pow(8.0,-1.0)<<endl;
	cout<<"10.0f,-2.0f:"<<pow(10.0f,-2.0f)<<endl;

	// return the remainder of x/y rounded to the nearest integer
	cout<<"\n===========> remainder:"<<endl;
	cout<<"11.0,3.0:"<<remainder(11.0,3.0)<<endl;
	cout<<"16.0f,4.0f:"<<remainder(16.0f,4.0f)<<endl;
	cout<<"31.0,2.5:"<<remainder(31.0,2.5)<<endl;

	// calculates x/y rounded to the nearest integer, writes the result to the memory at the pointer z and returns the remainder.
	cout<<"\n===========> remquo:"<<endl;
	int rq;
	cout<<"3.5,2.0:"<<remquo(3.5,2.0,&rq)<<"==>"<<rq<<endl;
	cout<<"12.3,9.2:"<<remquo(12.3,9.2,&rq)<<"==>"<<rq<<endl;

	// returns x*Ry (R is usually 2)
	cout<<"\n===========> scalbln:"<<endl;
	cout<<"2.5,5:"<<scalbln(2.5, 5)<<endl;
	cout<<"-20.9,-18:"<<scalbln(-20.9, -18)<<endl;

	// returns x*Ry (R is usually 2)
	cout<<"\n===========> scalbn:"<<endl;
	cout<<"2.5,5:"<<scalbn(2.5, 5)<<endl;
	cout<<"-20.9,-18:"<<scalbn(-20.9, -18)<<endl;

	// returns the square root of x	
	cout<<"\n===========> sqrt:"<<endl;
	cout<<"0:"<<sqrt(0)<<endl;
	cout<<"1:"<<sqrt(1)<<endl;
	cout<<"9:"<<sqrt(9)<<endl;
	cout<<"0.64:"<<sqrt(0.64)<<endl;
	cout<<"-25:"<<sqrt(-25)<<endl;

	// returns the integer part of x
	cout<<"\n===========> trunc:"<<endl;
	cout<<"0.60:"<<trunc(0.60)<<endl;
	cout<<"0.40:"<<trunc(0.40)<<endl;
	cout<<"5:"<<trunc(5)<<endl;
	cout<<"5.1:"<<trunc(5.1)<<endl;
	cout<<"-5.1:"<<trunc(-5.1)<<endl;
	cout<<"-5.9:"<<trunc(-5.9)<<endl;

}

int main(int argc, char **argv) {
    demo_1();
    demo_2();
    demo_3();
    demo_4();
    demo_5();

    return 0;
}
