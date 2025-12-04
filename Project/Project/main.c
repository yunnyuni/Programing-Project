#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include <windows.h>
#include "adventure.h";

int main() {
	srand((unsigned int)time(NULL));
	
	player kylen; //임시로 주인공 이름을 kylen으로 설정
	item puffed_grain[10] = { {0,"튀밥",0},{2,"마카로니",0},{2,"강냉이",0},{3,"폭탄",0},{4,"지도",0},{5,"티켓",0}, {6,"열쇠",0}}; //아이템 갯수 최대 10개
	player* player = &kylen;
	item* item= puffed_grain;

	char medal[40];
	strcpy(medal, "          [ 업적 목록 ]          \n");
	FILE* file = fopen("Medal.txt", "w"); //파일 초기화
	if (file == NULL) {
		printf("Error opening Medal.txt\n");
		return 1;
	}
	fprintf(file, "%s\n", medal);
	fclose(file);

	set_information(player); //플레이어 초기 설정
	player->FLOOR = 6;

	printf("\n[6층 탈출]\n"
			"당신은 외딴 곳에 있는 웅장한 탑에 갇혔습니다.\n"
			"현재 당신의 위치는 6층, 1층까지 내려가 탑을 탈출 해 봅시다!!!\n"
			"\n┏━━━━━━━━━━ !주의 사항! ━━━━━━━━━━┓\n"
			"┃                                 ┃\n"
			"┃  선택지에있는 숫자만 입력하세요 ┃\n"
			"┃       당신의 선택에 누구도      ┃\n"
			"┃        책임지지 않습니다.       ┃\n"
			"┃                                 ┃\n"
			"┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n"
			"\n━━━━━━━━━━━ 아이템 설명 ━━━━━━━━━━━\n"
			" 튀  밥 : 체력을 100 회복            \n"
			"마카로니: 체력을 랜덤 회복       \n"
			"강 냉 이: 체력을 50 회복           \n"
			" 폭  탄 : 상황에 따라 피해를 준다.   \n"
			" 지  도 : 층을 내려가는 길을 찾는다  \n"
			" 티  켓 : 상점의 가격 할인           \n"
			" 열  쇠 : 어디선가 쓸 일이 생길지도? \n"
			"━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	game(player, item);

	return 0;
}