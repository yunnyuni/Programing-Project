#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include "head.h"

void set_information(player* player) {
	player->HP = 100;
	player->MAX_HP = 100;
	player->EXP = 0;
	player->LEVEL = 1;
	player->attack = 10; //1레벨 공격력
}

void set_state(player* player) {
	if (player->EXP >=1000) { //경험치가 1000 이상일 때
		printf("\nLEVEL UP!!\n");
		player->EXP = player->EXP - 1000;
		if (player->LEVEL == 10) { //이미 최대 레벨일 때
			printf("\n(이미 최대 레벨 입니다.)\n");
			//한계돌파 업적 달성
			return;
		}
		player->LEVEL++;
	}
	player->attack = 10*((player->LEVEL-1)*1.2); //레벨에 따른 플레이어 공격력 설정
	player->MAX_HP = 100 * player->LEVEL; //레벨에 따른 플레이어 체력 설정
	player->HP = player->MAX_HP; //레벨에 따른 플레이어 체력 설정
	//1-10, 2-12, 3-24, 4-36, 5-48, 6-60, 7-72, 8-84, 9-96, 10-108
}

void player_action(player* player, item* item, int* enemy_hp) {
	int choice;
	printf("\n플레이어의 행동을 결정하세요.\n"
	"1. [  공   격  ]\n2. [아이템 사용]\n3. [  도   망  ]\n");
	while (1) {
		printf("입력: \n");
		scanf("%d", &choice);
		if (choice == 1 || (choice == 2 || choice == 3))
			break;
		printf("\n잘못된 입력입니다.\n");
	}
	switch (choice) {
	case 1:		
		//적을 공격하는 코드 짜기
		break;
	case 2:
		//아이템 사용 코드 짜기
		break;
	case 3:
		if (rand() % 1) {
			printf("\n도망에 성공하였습니다.\n");
			enemy_hp = 0;
		}
		break;
	default:
		printf("\n!!!!!!이거 뜨면 오류!!!!!!\n");
	}
}

void battle_enemy(player* player, item* item, char* enemy_name, int enemy_hp) {
	int enemy_attack = 10 + rand() % ((50 / player->FLOOR) - 10 + 1); //층수에 따른 몬스터 공격력
	printf("\n%s와(과)의 전투를 시작합니다.\n"
		"(%s의 현재 체력: %d)\n"
		"(당신의 현재 체력: %d)\n", enemy_name, enemy_name, enemy_hp, player->HP);
	while (1) {
		int enemy_HP = enemy_hp; //적의 이전 체력을 알기 위한 변수
		player_action(player, item ,enemy_hp); //플레이어 행동 함수 호출
		printf("\n%s가 총 %d의 데미지를 입었습니다. "
			"(%s의 현재 체력: %d)\n", enemy_name, enemy_name, enemy_HP - enemy_hp, enemy_hp);
		printf("\n적이 당신을 공격합니다.\n"
		"당신은 총 %d의 데미지를 입었습니다.(당신의 현재 체력: %d)", enemy_attack, player->HP);
		player->HP -= enemy_attack; //적의 플레이어 공격
		if (player->HP <= 0) {
			badend1();
		}
		else if (enemy_hp <= 0) {
			printf("%s가 처치되었습니다.", enemy_name);
		}
	}
}
int use_item(player* player, item* item, char* item_name) {
	//상황1 - 이벤트 중에 사용할 수 있는 아이템
	// 해당 아이템이 있는지 아이템 목록과 비교, 없으면 실패(0), 있으면 성공(1)
	//상황2 - 전투중에 사용할 수 있는 아이템
	// 아이템 출력
	// 사용할 아이템 번호 선택
	// 아이템 사용에 따른 상태 변화(체력 회복 또는 공격력 강화)
	//상황3 - 아직 미정
}

void badend1() {
	printf("\n플레이어의 체력이 0이 되어 사망하였습니다.\n"
		"\n어딘가 익숙한 장소가 눈에 띈다.\n"
		"...\n\n[Bad Ending 1]\n\n");
	exit(0);
}
void badend2() {
	printf("\n1층에서 내려간곳은 지하...1층?.\n"
		"\n어딘가 익숙한 장소가 눈에 띈다.\n"
		"당신은 10층으로 돌아왔습니다.\n"
		"...\n\n[Bad Ending 2]\n\n");
	exit(0);
}