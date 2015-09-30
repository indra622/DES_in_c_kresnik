#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define SIZE 64


//tables
static int parity_drop[56] = { 57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18, 10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36, 63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4 };

static int compression_table[48] = { 14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32 };

static int initial_permutation[64] = { 58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4, 62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8, 57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3, 61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7 };

static int final_permutation[64] = { 40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31, 38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29, 36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27, 34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25 };

static int expansion_pbox[48] = { 32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9, 8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17, 16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25, 24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1 };

static int straight_pbox[32] = { 16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10, 2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25 };

static int sbox[8][16][4] = { { 14, 0, 4, 15, 4, 15, 1, 12, 13, 7, 14, 8, 1, 4, 8, 2, 2, 14, 13, 4, 15, 2, 6, 9, 11, 13, 2, 1, 8, 1, 11, 7, 3, 10, 15, 5, 10, 6, 12, 11, 6, 12, 9, 3, 12, 11, 7, 14, 5, 9, 3, 10, 9, 5, 10, 0, 0, 3, 5, 6, 7, 8, 0, 13 },
{ 15, 3, 0, 13, 1, 13, 14, 8, 8, 4, 7, 10, 14, 7, 11, 1, 6, 15, 10, 3, 11, 2, 4, 15, 3, 8, 13, 4, 4, 14, 1, 2, 9, 12, 5, 11, 7, 0, 8, 6, 2, 1, 12, 7, 13, 10, 6, 12, 12, 6, 9, 0, 0, 9, 3, 5, 5, 11, 2, 14, 10, 5, 15, 9 },
{ 10, 13, 13, 1, 0, 7, 6, 10, 9, 0, 4, 13, 14, 9, 9, 0, 6, 3, 8, 6, 3, 4, 15, 9, 15, 6, 3, 8, 5, 10, 0, 7, 1, 2, 11, 4, 13, 8, 1, 15, 12, 5, 2, 14, 7, 14, 12, 3, 11, 12, 5, 11, 4, 11, 10, 5, 2, 15, 14, 2, 8, 1, 7, 12 },
{ 7, 13, 10, 3, 13, 8, 6, 15, 14, 11, 9, 0, 3, 5, 0, 6, 0, 6, 12, 10, 6, 15, 11, 1, 9, 0, 7, 13, 10, 3, 13, 8, 1, 4, 15, 9, 2, 7, 1, 4, 8, 2, 3, 5, 5, 12, 14, 11, 11, 1, 5, 12, 12, 10, 2, 7, 4, 14, 8, 2, 15, 9, 4, 14 },
{ 2, 14, 4, 11, 12, 11, 2, 8, 4, 2, 1, 12, 1, 12, 11, 7, 7, 4, 10, 1, 10, 7, 13, 14, 11, 13, 7, 2, 6, 1, 8, 13, 8, 5, 15, 6, 5, 0, 9, 15, 3, 15, 12, 0, 15, 10, 5, 9, 13, 3, 6, 10, 0, 9, 3, 4, 14, 8, 0, 5, 9, 6, 14, 3 },
{ 12, 10, 9, 4, 1, 15, 14, 3, 10, 4, 15, 2, 15, 2, 5, 12, 9, 7, 2, 9, 2, 12, 8, 5, 6, 9, 12, 15, 8, 5, 3, 10, 0, 6, 7, 11, 13, 1, 0, 14, 3, 13, 4, 1, 4, 14, 10, 7, 14, 0, 1, 6, 7, 11, 13, 0, 5, 3, 11, 8, 11, 8, 6, 13 },
{ 4, 13, 1, 6, 11, 0, 4, 11, 2, 11, 11, 13, 14, 7, 13, 8, 15, 4, 12, 1, 0, 9, 3, 4, 8, 1, 7, 10, 13, 10, 14, 7, 3, 14, 10, 9, 12, 3, 15, 5, 9, 5, 6, 0, 7, 12, 8, 15, 5, 2, 0, 14, 10, 15, 5, 2, 6, 8, 9, 3, 1, 6, 2, 12 },
{ 13, 1, 7, 2, 2, 15, 11, 1, 8, 13, 4, 14, 4, 8, 1, 7, 6, 10, 9, 4, 15, 3, 12, 10, 11, 7, 14, 8, 1, 4, 2, 13, 10, 12, 0, 15, 9, 5, 6, 12, 3, 6, 10, 9, 14, 11, 13, 0, 5, 0, 15, 3, 0, 14, 3, 5, 12, 9, 5, 6, 7, 2, 8, 11 } };


int* init_perm(int plain[]){

	int result[SIZE] = { 0 };
	int i = 0, j = 0;;

	for (i = 0; i < SIZE; i++){
		if (plain[i] == 1)
			for (j = 0; j < SIZE; j++){ 
				if (i == initial_permutation[j]-1) //테이블에서 찾아서 i값과 맞는 값을 찾아서 그 인덱스에 1을 위치시킨다.
					result[j] = 1;
				else
					continue;
			}
			//result[initial_permutation[i+1]] = 1;
		else
			continue;
	}

	return result;
}


//연산을 위해서 16진수를 2진수로 변환
int* hextobin(char *hex){
	int i, h, len = strlen(hex) - 1, buf[8], j = 0;
	int result[SIZE] = { 0 };
	while (len-->-1){
		//printf("%c: ", *hex);
		h = *hex>96 ? *hex++ - 87 : *hex++ - 48;
		for (i = 7; i >= 0; i--){
			buf[i] = h % 2;
			h = h / 2;
		}
		for (i = 4; i<8; i++, j++){
			//printf("%d", buf[i]);
			result[j] = buf[i];
			
		}
	
	}
	/*for (j = 0; j < SIZE; j++)
		printf("%d", result[j]);*/
	return result;
}

char* bintohex(int* bin){
	int i = 0, j = 0;
	char hex[SIZE/4];
	int temp = 0;
	for (j = 0; j < SIZE / 4; j++){
		for (i = (j*4)+0; i < 4+(j*4); i++){
			if (bin[i] == 1) // 만일 1이 있으면
				if (i % 4 == 0) // 그리고 2^3 자리면
					temp += 8; // 8을 더함
				else if (i % 4 == 1)
					temp += 4;
				else if (i % 4 == 2)
					temp += 2;
				else if (i % 4 == 3)
					temp += 1;
				else
					printf("이진법 16진법으로 바꾸는데 나머지가 4이상이거나 음수가 나왔다. 이상하다.\n");
			else
				continue;
		}

		switch (temp){
			case 0:
			hex[j] = '0';
			break;
			case 1:
			hex[j] = '1';
			break;
			case 2:
			hex[j] = '2';
			break;
			case 3:
			hex[j] = '3';
			break;
			case 4:
			hex[j] = '4';
			break;
			case 5:
			hex[j] = '5';
			break;
			case 6:
			hex[j] = '6';
			break;
			case 7:
				hex[j] = '7';
				break;
			case 8:
				hex[j] = '8';
				break;
			case 9:
				hex[j] = '9';
				break;
			case 10:
				hex[j] = 'a';
				break;
			case 11:
				hex[j] = 'b';
				break;
			case 12:
				hex[j] = 'c';
				break;
			case 13:
				hex[j] = 'd';
				break;
			case 14:
				hex[j] = 'e';
				break;
			case 15:
				hex[j] = 'f';
				break;
			default:
				printf("원래 16진수 변환되어야되는데 0~15 이외의 값이 들어왔다. 이상하다.\n");
		}
		temp = 0; // 한 자리수 만들고 다시 0으로 초기화 시켜줌
	}
	return hex;
}

//expansion pbox
int* exp_pbox(int afterip[]){
	int result[SIZE * 3 / 4] = { 0 }; //48비트 짜리 어레이 만듬
	int i = 0, j=0;


	for (i = 0; i < SIZE / 2 ; i++){
		if (afterip[i] == 1)
			for (j = 0; j < SIZE*3/4; j++){
				if (i == expansion_pbox[j] - 1) //테이블에서 찾아서 i값과 맞는 값을 찾아서 그 인덱스에 1을 위치시킨다.
					result[j] = 1;
				else
					continue;
			}
		else
			continue;
	}

	return result;
}

//왼쪽 부분 자르기
int* division_left(int arr[]){ 
	int result[SIZE / 2] = { 0 };
	int i = 0;

	for (i = 0; i < SIZE / 2; i++){
		result[i] = arr[i];
	}
	
	return result;
}
//오른쪽 부분 자르기
int* division_right(int arr[]){
	int result[SIZE / 2] = { 0 };
	int i = 0;

	for (i = SIZE / 2; i < SIZE; i++){
		result[i-(SIZE/2)] = arr[i];
	}

	return result;
}

int* parityDrop(int key[]){
	int result[56] = { 0 }; //56비트 짜리 어레이 만듬
	int i = 0, j = 0;


	for (i = 0; i < SIZE; i++){
		if (key[i] == 1)
			for (j = 0; j < 56; j++){
				if (i == parity_drop[j] - 1) //테이블에서 찾아서 i값과 맞는 값을 찾아서 그 인덱스에 1을 위치시킨다.
					result[j] = 1;
				else
					continue;
			}
		else
			continue;
	}

	return result;
}

//key generator 만들기
void keyGenerate(char* key){

	




	
}


int main(void){

	int i = 0; // for loop
	char str[SIZE] = "123456abcd132536";
	char key[SIZE] = "aabb09182736ccdd";
	
	////평문 받기
	//printf("input your hex text");
	//scanf_s("%s", &str, sizeof(str));
	////키 받기
	//printf("input your key");
	//scanf_s("%s", &key, sizeof(key));

	//16진수를 2진수로 변환
	int hexInput[SIZE] = { 0 };
	int ipInput[SIZE] = { 0 };
	char output[SIZE / 2] = {NULL};
	int left[SIZE / 2] = { 0 };
	int right[SIZE / 2] = { 0 };
	int pbox[SIZE * 3 / 4] = { 0 };
	int binKey[SIZE];
	int afterparity[56];

	//확인
	printf("=======16진수 변환 input=======\n");
	for (i = 0; i < SIZE; i++){
		hexInput[i] = hextobin(str)[i];
		printf("%d", hexInput[i]);
	}
	//IP
	printf("\n=======after IP=======\n");
	for (i = 0; i < SIZE; i++){
		ipInput[i] = init_perm(hexInput)[i];
		printf("%d", ipInput[i]);
	}
	
	//ip hex로 결과 확인
	printf("\n=======after IP hex =======\n");
	for (i = 0; i < SIZE / 4; i++){
		output[i] = bintohex(ipInput)[i];
	}
	printf("%s", output);
	//나눠져랑
	printf("\n=======left part=======\n");
	for (i = 0; i < SIZE / 2; i++){
		left[i] = division_left(ipInput)[i];
		printf("%d", left[i]);
	}
	printf("\n=======right part=======\n");
	for (i = 0; i < SIZE / 2; i++){
		right[i] = division_right(ipInput)[i];
		printf("%d", right[i]);
	}

	//expansion pbox 돌리기
	printf("\n=======after IP hex =======\n");
	for (i = 0; i < SIZE *3/4; i++){
		pbox[i] = exp_pbox(right)[i];
		printf("%d", pbox[i]);
	}

	//key 구하기 밑밥 _ 패리티 준비

	printf("\n==========key binary==========\n");
	for (i = 0; i < SIZE; i++){
		binKey[i] = hextobin(key)[i];
		printf("%d", binKey[i]);
	}

	printf("\n==========parity drop==========\n");
	for (i = 0; i < 56; i++){
		afterparity[i] = parityDrop(binKey)[i];
		printf("%d", afterparity[i]);
	}

	//여기부터 라운드 모드!!!!!!!!!!!
	
	keyGenerate(key);
	

	system("pause");

	return 0;
}

