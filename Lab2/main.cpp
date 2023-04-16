#include <xmmintrin.h>
#include <mmintrin.h>
#include <immintrin.h>
#include <dvec.h>
#include <intrin.h>
#include <iostream>

using std::cout;
using std::endl;

__m64 _mm_add(__m64 m1, __m64 m2) {
	__m64 res;
	__asm {
		movq mm0, m1
		movq mm1, m2
		paddsb mm0, mm1
		movq res, mm0
	}
	return res;
}

__m128 _mm_sub(__m128 m1, __m128 m2)
{
	__m128 result;
	__asm
	{
		movups xmm0, m1
		movups xmm1, m2
		subps xmm0, xmm1 //вычитание
		movups result, xmm0
	}
	return result;
}

__declspec(noinline)
void func() {
	void* pvAddressOfReturnAddress = _AddressOfReturnAddress();
	printf_s("%p\n", pvAddressOfReturnAddress);
	printf_s("%p\n", *((void**)pvAddressOfReturnAddress));
	printf_s("%p\n", _ReturnAddress());
}

__m256 __mm_div(__m256 m1, __m256 m2)
{
	__m256 result;
	__asm
	{
		vmovupd ymm0, m1
		vmovupd ymm1, m2
		//vhypot ymm0, ymm1 //деление
		vmovupd result, ymm0
	}
	return result;
}

int main() {

	int arr1[2] = { 6, 4 };
	int arr2[2] = { 7, 5 };

	float farr1[4] = { 1, 2, 4, 8 };
	float farr2[4] = { 5, 6, 7, 8 };

	double sqrtf[2] = { 8.0, 4.0 };

	float res_float[4];

	_asm { //mmx
		movq mm0, arr1
		movq mm1, arr2
		paddd mm0, mm1
		movq arr2, mm0
	}

	cout << "Sum of arr1 and arr2: " << arr2[0] << " " << arr2[1] << endl;
	int ctrl = 0;

	_asm {
		finit
		fstcw ctrl
		and ctrl, 0xFFFE
		fldcw ctrl
	}

	_asm { //sse
		movups xmm0, farr1
		movups xmm1, farr2
		mulps xmm0, xmm1
		movups res_float, xmm0
	}

	cout << "Multiply of farr1 and farr2: ";

	for (auto i : res_float) {
		cout << i << " ";
	}
	cout << endl;

	cout << "Sqrt of sqrtf: ";

	_asm { //sse2
		movups xmm0, sqrtf
		sqrtpd xmm1, xmm0
		movups sqrtf, xmm1
	}

	for (auto i : sqrtf)
		cout << i << " ";
	cout << endl;

	float avx1[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
	float avx2[8] = { 5, 5, 5, 5, 5, 5, 5, 5 };
	float avx_res[8] = {};

	_asm { //avx
		vmovups ymm0, avx1
		vmovups ymm1, avx2
		vsubps ymm3, ymm0, ymm1
		vmovups avx_res, ymm3
		vzeroall
	}

	cout << "Substrction : ";

	for (auto i : avx_res)
		cout << i << " ";

	cout << endl;

	//dvec.h
	F64vec2 v1(1.0, 2.0);
	F64vec2 v2(10.0, 11.0);
	v1 = v1 + v2;
	cout << "Sum of vectors: ";

	for (uint16_t i = 0; i < 2; ++i)
		cout << v1[i] << " ";

	cout << endl;

	//_mm_64

	char arr_mmx_1[8] = { 0b1, 0b10, 0b100, 0b1000, 0b10000, 0b1000000, 0b1 };
	char arr_mmx_2[8] = { 0b11, 0b11, 0b11, 0b11, 0b11, 0b11, 0b11, 0b1111 };
	__m64 m64_1, m64_2;


	_asm {
		movq mm0, arr_mmx_1
		movq m64_1, mm0
		movq mm0, arr_mmx_2
		movq m64_2, mm0
	}

	m64_1 = _mm_add(m64_1, m64_2);

	_asm {
		movq mm0, m64_1
		movq arr_mmx_1, mm0
	}

	cout << "Sum of 64-bit ints: ";
	for (int i = 0; i < 8; ++i)
		cout << (int)arr_mmx_1[i] + 0 << " ";
	cout << endl;

	double arr1_128[4] = { 2, 2, 3, 4 };
	double arr2_128[4] = { 1, 1, 2, 2 };

	__m128 m128_1, m128_2;
	__asm {
		movups xmm0, arr1_128
		movups m128_1, xmm0
		movups xmm0, arr2_128
		movups m128_2, xmm0
	}
	m128_1 = _mm_sub(m128_1, m128_2);
	__asm {
		movups xmm0, m128_1
		movups arr1_128, xmm0
	}

	cout << "Subtraction 128-bit: ";
	for (size_t i = 0; i < 4; ++i)
		cout << (int)arr1_128[i] << " ";
	cout << endl;

	double mas_ymm1[4] = { 12,20,34,134 };
	double mas_ymm2[4] = { 2,2,2,2 };
	__m256 m256_1, m256_2;
	__asm {
		vmovups ymm0, mas_ymm1
		vmovups m256_1, ymm0
		vmovups ymm0, mas_ymm2
		vmovups m256_2, ymm0
	}
	//операция
	m256_1 = __mm_div(m256_1, m256_2);
	__asm {
		vmovups ymm0, m256_1
		vmovups mas_ymm1, ymm0
	}
	std::cout << "\n\nDiv doubles\n";
	for (size_t i = 0; i < 4; i++)
	{
		std::cout << (int)mas_ymm1[i] << ' ';
	}

	//intrin.h

	double im256_1[12] = { 1,2,3,4,5,6,7,8,9,10,11,12 };
	double im256_2[12] = { 1,1,1,2,2,2,3,3,3,4,4,4 };
	double im256_3[12];
	for (size_t i = 0; i < 12; i += 4)
	{
		__m256d x = _mm256_loadu_pd(&im256_1[i]); 
		__m256d y = _mm256_loadu_pd(&im256_2[i]);
		__m256d z = _mm256_add_pd(x, y); 
		_mm256_store_pd(&im256_3[i], z); 
	}
	std::cout << "\n\nsum of im1 and im2\n";
	for (size_t i = 0; i < 12; i++)
	{
		std::cout << (int)im256_3[i] << " ";
	}

	__asm vzeroall; 
	
	__m256 avx_register = _mm256_set_ps(8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0); // инициализируем регистр
	__m256 avx_multiplied = _mm256_mul_ps(avx_register, _mm256_set1_ps(2.0)); // умножаем каждый элемент на 2
	__m256i avx_ints = _mm256_cvttps_epi32(avx_multiplied); // преобразовываем результаты в int

	std::cout << "\n";

	func();

	return 0;
}