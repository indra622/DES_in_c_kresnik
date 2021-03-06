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
//라운드 돌리기용 구조체 선언
typedef struct roundelement{
	int left[SIZE / 2];
	int right[SIZE / 2];
	int leftKey[28];
	int rightKey[28];

}roundelement;

int* init_perm(int plain[]) {
	int result[64] = { 0 }; //48비트 짜리 어레이 만듬
	int i = 0, j = 0;

	for (i = 0; i < 64; i++) {
		result[i] = plain[initial_permutation[i] - 1];
	}
	return result;
}

int* final_perm(int plain[]) {
	int result[64] = { 0 }; //48비트 짜리 어레이 만듬
	int i = 0, j = 0;
	for (i = 0; i < 64; i++) {
		result[i] = plain[final_permutation[i] - 1];
	}
	return result;
}

//연산을 위해서 16진수를 2진수로 변환
int* hextobin(char *hex){
	int i, h, len = strlen(hex) - 1, buf[8], j = 0;
	int result[SIZE] = { 0 };
	while (len-->-1){
		h = *hex>96 ? *hex++ - 87 : *hex++ - 48;
		for (i = 7; i >= 0; i--){
			buf[i] = h % 2;
			h = h / 2;
		}
		for (i = 4; i<8; i++, j++){
			result[j] = buf[i];
		}
	}
	return result;
}
void hexManage(int* bin, int temp, char* hex, int j) {
	int i = 0;
	for (i = (j * 4) + 0; i < 4 + (j * 4); i++) {
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
	
	switch (temp) {
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
}

char* bintohex(int* bin){
	int i = 0, j = 0;
	char hex[SIZE/4];
	int temp = 0;
	for (j = 0; j < SIZE / 4; j++){
		hexManage(bin, temp, hex, j);
		temp = 0;
	}
	return hex;
}
char* bintohexForRoundKey(int* bin) {
	int i = 0, j = 0;
	char hex[12];
	int temp = 0;
	for (j = 0; j < 12; j++) {
		hexManage(bin, temp, hex, j);
		temp = 0; // 한 자리수 만들고 다시 0으로 초기화 시켜줌
	}
	return hex;
}
char* bintohexForRound(int* bin) {
	int i = 0, j = 0;
	char hex[8];
	int temp = 0;
	for (j = 0; j < 8; j++) {
		hexManage(bin, temp, hex, j);
		temp = 0; // 한 자리수 만들고 다시 0으로 초기화 시켜줌
	}
	return hex;
}
//expansion pbox
int* exp_pbox(int afterip[]){
	int result[SIZE * 3 / 4] = { 0 }; //48비트 짜리 어레이 만듬
	int i = 0, j=0;


	for (i = 0; i < 48; i++) {
		result[i] = afterip[expansion_pbox[i]-1];
	}

	return result;
}
//straight pbox
int* str_pbox(int aftersbox[]) {
	int result[32] = { 0 }; 
	int i = 0, j = 0;


	for (i = 0; i < 32; i++) {
		result[i] = aftersbox[straight_pbox[i] - 1];
	}

	return result;
}
//패리티 비트 함수
int* parityDrop(int key[]) {
	int result[56] = { 0 }; 
	int i = 0, j = 0;


	for (i = 0; i < 56; i++) {
		result[i] = key[parity_drop[i] - 1];
	}

	return result;
}
//xor계산 함수 만들기
int* cal_xor(int expansion[], int key[]){
	int i = 0;
	int result[48] = { 0 };

	for (i = 0; i < 48; i++){
		if (expansion[i] != key[i]){
			result[i] = 1;
		}
		else
			result[i] = 0;
	}
	return result;
}
int* cal_xor_next(int left[], int after_f[]) {
	int i = 0;
	int result[32] = { 0 };

	for (i = 0; i < 32; i++) {
		if (left[i] != after_f[i]) {
			result[i] = 1;
		}
		else
			result[i] = 0;
	}
	return result;
}
//라운드에서 이루어져야 할 일
//1. 레프트랑 라이트를 받는다
//2. 키의 레프트랑 라이트를 받는다. 쉬프트도 한다.
//3. 라이트에 익스펜션을 한다.
//4. 키를 compression 한다.
//5. 익스펜션한 라이트랑 키랑 xor한다.
// s-box를 돌린다.
// straight p-box를 돌린다.
//6. xor한거를 라이트에 넣고 라이트를 레프트에 넣는다.
//7. 라이트랑 레프트, 키 라이트랑 레프트를 반환한다.
//8. 이짓을 16번 한다. 끝
//필요한 함수 : compression, xor, left shift, swap.
roundelement round(roundelement re, int num) {

	//나누기
	roundelement result;
	memset(&result, 0, sizeof(result));
	int i = 0, j = 0;
	int tempL = 0, tempR = 0;
	int flag = 0; //flag가 1이면 1번 shift, 2이면 2번 shift
	int pbox[SIZE * 3 / 4] = { 0 };
	int compressing_key[56] = { 0 };
	int compressed_key[48] = { 0 };
	int after_xor[48] = { 0 };
	int after_xor_f[32] = { 0 };
	int division_for_sbox[8][6] = { 0 };
	char roundKeyTest[24] = { NULL };
	int after_sbox[32] = { 0 };
	int after_ssbox[32] = { 0 };
	char roundOutL[12] = { NULL };
	char roundOutR[12] = { NULL };
	
	// 받아온 키를 대입함
	for (i = 0; i < 28; i++) {
		result.leftKey[i] = re.leftKey[i];
		result.rightKey[i] = re.rightKey[i];
	}

	//key를 받아서 shift하기
	if (num == 1 || num == 2 || num == 9 || num == 16) {
		flag = 1;
	}
	else if (num == 3 || num == 4 || num == 5 || num == 6 || num == 7 || num == 8 || num == 10 || num == 11 || num == 12 || num == 13 || num == 14 || num == 15) {
		flag = 2;
	}
	else {
		printf("16라운드 돌려야 되는데 횟수 값이 16이 넘어가거나 음수가 나왔습니다. 이상합니다.");
		exit(1);
	}
	//left shift in right and left
	for (j = 0; j < flag; j++) {
		
		tempL = result.leftKey[0];
		tempR = result.rightKey[0];
		for (i = 0; i < 27; i++) {
			result.leftKey[i] = result.leftKey[i + 1];
			result.rightKey[i] = result.rightKey[i + 1];
		}
		result.leftKey[27] = tempL;
		result.rightKey[27] = tempR;
	}
	//key를 합치기 (compression pbox)
	for (i = 0; i < 56; i++) {
		if (i < 28)
			compressing_key[i] = result.leftKey[i];
		else if (i < 56) {
			compressing_key[i] = result.rightKey[i - 28];
		}
		else {
			printf("뭔가 이상합니다. 키 합치기에서 i값이 음수이거나 지정한 것보다 커요");
		}
	}
	//key compression 끝!
	for (i = 0; i < 48; i++) {
		compressed_key[i] = compressing_key[compression_table[i] - 1];
	}
	//라운드 키 나오는거 확인
	for (i = 0; i < 12; i++) {
		roundKeyTest[i] = bintohexForRoundKey(compressed_key)[i];
	}
	//이제 익스펜션
	//expansion pbox 돌리기
	for (i = 0; i < 48; i++) {
		pbox[i] = exp_pbox(re.right)[i];
	}

	//xor연산한거 저장하기
	for (i = 0; i < 48; i++) {
		after_xor[i] = cal_xor(pbox, compressed_key)[i];
	}
	//저장한거 sbox돌리기

	//일단 after_xor을 6개씩으로 나눕시다
	i = 0;
	j = 0;
	while (i < 48) {
		division_for_sbox[j][i - (j * 6)] = after_xor[i];
		i++;
		if (i % 6 == 0) {
			j++;
		}
	}
	//6자리 수를 분리해서 정리
	//이진법을 십진법으로 바꿈
	for (i = 0; i < 8; i++) {
		int row = 0;
		int col = 0;
		int num = 0;
		int temp = 0;
		int j = 0;

		if (division_for_sbox[i][0] == 1) {
			row += 2;
		}
		if (division_for_sbox[i][1] == 1) {
			col += 8;
		}
		if (division_for_sbox[i][2] == 1) {
			col += 4;
		}
		if (division_for_sbox[i][3] == 1) {
			col += 2;
		}
		if (division_for_sbox[i][4] == 1) {
			col += 1;
		}
		if (division_for_sbox[i][5] == 1) {
			row += 1;
		}

		//sbox[i][col][row] 이 값을 이진법으로 바꿔서 넣어야 함
		temp = sbox[i][col][row];
		for (j = 3; j >= 0; j--) {
			after_sbox[j+(i*4)] = temp % 2;
			temp /= 2;
		}
	}
	//그리고 swap하기

	for (i = 0; i < 32; i++) {
		//straight pbox하기!!!!
		after_ssbox[i] = str_pbox(after_sbox)[i];
		//그리고 얘를 left랑 xor하기
		after_xor_f[i] = cal_xor_next(re.left, after_ssbox)[i];
		//16할땐 스왑 안함
		if (num == 16) {
			result.right[i] = re.right[i];
			result.left[i] = after_xor_f[i];
		}
		else {
			result.left[i] = re.right[i];
			result.right[i] = after_xor_f[i]; //결과값 바로 라이트로 보냄
		}
	}
	for (i = 0; i < 8; i++) {
		roundOutL[i] = bintohexForRound(result.left)[i];
		roundOutR[i] = bintohexForRound(result.right)[i];
	}
	printf("\nleft \t\t right\n");
	printf("%s \t %s", roundOutL, roundOutR);
	printf("\nRound Key :  ");
	printf("%s", roundKeyTest);

	return result;
}


int main(void){

	int i = 0; // for loop
	//char str[SIZE] = "123456abcd132536";
	//char key[SIZE] = "aabb09182736ccdd";
	char str[SIZE] = { NULL };
	char key[SIZE] = { NULL };
	//평문 받기
	printf("=======================================================\n");
	printf("===========DES encryption Tool by hosung===============\n");
	printf("===========please type code in lowercase===============\n");
	printf("=All comments and exception msgs are written by Korean=\n");
	printf("====https://github.com/indra622/DES_in_c_kresnik.git===\n");
	printf("=======================================================\n\n\n");
	printf("input your hex text :");
	scanf_s("%s", &str, sizeof(str));
	//키 받기
	printf("input your key :");
	scanf_s("%s", &key, sizeof(key));

	//16진수를 2진수로 변환
	int hexInput[SIZE] = { 0 };
	int ipInput[SIZE] = { 0 };
	char output[SIZE / 2] = {NULL};
	char ciphered[SIZE / 2] = { NULL };
	
	int left[SIZE / 2] = { 0 };
	int right[SIZE / 2] = { 0 };
	int leftKey[28] = { 0 };
	int rightKey[28] = { 0 };
	int beforeFP[SIZE] = { 0 };
	int afterFP[SIZE] = { 0 };

	int binKey[SIZE];
	int afterparity[56];
	roundelement res;
	roundelement tempRound;
	memset(&res, 0, sizeof(res));

	//확인
	for (i = 0; i < SIZE; i++){
		hexInput[i] = hextobin(str)[i];
	}
	//IP
	for (i = 0; i < SIZE; i++){
		ipInput[i] = init_perm(hexInput)[i];
	}
	//ip hex로 결과 확인
	for (i = 0; i < SIZE / 4; i++){
		output[i] = bintohex(ipInput)[i];
	}
	//나눠져랑
	for (i = 0; i < SIZE; i++){
		if (i < 32)
			left[i] = ipInput[i];
		else if (i < 64)
			right[i - 32] = ipInput[i];
		else {
			printf("레프트 라이트 나눌때 56보다 크거나 음수가 나와버린 것 같습니다.\n");
			exit(1);
		}
	}
	//key 구하기 밑밥 _ 패리티 준비
	for (i = 0; i < SIZE; i++){
		binKey[i] = hextobin(key)[i];
	}
	for (i = 0; i < 56; i++){
		afterparity[i] = parityDrop(binKey)[i];
		if (i < 28)
			leftKey[i] = afterparity[i];
		else if (i < 56)
			rightKey[i-28] = afterparity[i];
		else{
			printf("키 레프트 라이트 나눌때 56보다 크거나 음수가 나와버린 것 같습니다.\n");
			exit(1);
		}
	}
	//여기부터 라운드 모드!!!!!!!!!!!
	//라운드 구조체에 지금까지 한 것들을 넣어 봅시다.
	//키를 넣어 봅시다 아 값도
	for (i = 0; i < 32; i++) {
		res.left[i] = left[i];
		res.right[i] = right[i];
	}
	for (i = 0; i < 28; i++) {
		res.leftKey[i] = leftKey[i];
		res.rightKey[i] = rightKey[i];
	}
	//라운드 돌리기!
	for (i = 1; i < 17; i++) {
		printf("\n==========Round %d=========\n", i);
		res = round(res, i);
	}
	//키 합치기
	for (i = 0; i < SIZE; i++) {
		if (i < SIZE/2)
			beforeFP[i] = res.left[i];
		else if (i < SIZE) {
			beforeFP[i] = res.right[i - 32];
		}
		else {
			printf("뭔가 이상합니다. 마지막 결과 합치기에서 i값이 음수이거나 지정한 것보다 커요");
		}
	}
	//FP
	for (i = 0; i < SIZE; i++) {
		afterFP[i] = final_perm(beforeFP)[i];
	}
	//최종 결과
	printf("\n\n\n=======cipherText=======\n");
	for (i = 0; i < SIZE / 4; i++) {
		ciphered[i] = bintohex(afterFP)[i];
	}
	printf("%s\n Here is the end of the program\n", ciphered);
	
	system("pause");
	return 0;
}