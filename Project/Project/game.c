#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include <windows.h>
#include "adventure.h"

void game(player* player, item* item) {
	int choice; //플레이어 선택 저장
	int event; //랜덤 이벤트 제어 변수
	int heal = 0; //체력 회복에 대한 변수

	while (1) { //게임 진행 반복문

		if (player->FLOOR == 6) { //만약 플레이어가 현재 6층이라면
			printf("\n아늑해 보이지만 인기척이 없어 싸늘합니다.\n"
				"바로 앞 층을 내려가는 계단이 보입니다.\n"
				"   ┏━━━━━━━━┓\n   ┃        ┃\n   ┃   ├┐   ┃\n   ┃   └┘   ┃\n   ┃        ┃\n   ┃        ┃\n━━━┛        ┗━━━\n\n"
				"1. [ 내려 간다 ]\n2. [ 무시 한다 ]\n"
				"입력: ");
			scanf("%d", &choice);
			if (choice == 1) {
				printf("\n한 층을 내려갔습니다.\n");
				player->FLOOR--;
				printf("\n━┓\n ┗━┓    ↘\n   ┗━┓    ↘\n     ┗━┓    ↘\n       ┗━┓\n         ┗━┓\n           ┗━━\n\n");
			}
			else {
				printf("\n당신은 계단을 지나쳤습니다.\n");
				continue;
			}
			printf("\n");
			system("pause");
			system("cls"); //입력을 받아 화면 초기화
		}
		printf("= = = = = = = = = = = = = = = = = = = = = =\n" //플레이어의 정보 출력
			"현재 층 : %d층 | HP : %d / %d | LV : %d | EXP : %d | 공격력 : %d | 돈 : %d\n",
			player->FLOOR, player->HP, player->MAX_HP, player->LEVEL, player->EXP, player->attack, player->money);

		printf("\n당신의 행동을 결정하세요.\n"
			"1. [ 주변 탐색 ]\n2. [  휴   식  ]\n3. [아이템 사용]\n"
			"입력: ");
		scanf("%d", &choice); //플레이어 행동 선택

		switch (choice) { //플레이어가 선택한 행동에 따른 분기
		case 1: //1번을 입력 받았을 때 (이벤트)
			event = rand() % 100; //랜덤값 이용
			if (event % 10 == 0) { //계단 발견 10%
				printf("\n주변을 열심히 탐색하다 아래로 향하는 계단을 발견했습니다.\n");
				down_floor(player, item);
			}
			else if (event % 3 == 0 || event % 5 == 0) { //상인과 마주 37%
				store(player, item);
			}
			else { //적과 마주 53%
				printf("\n적을 마주했습니다.\n");
				battle_enemy(player, item, "몬스터", 0);
			}
			break;
		case 2: //2번을 입력 받았을 때 (휴식)
			printf("\n가만히 앉아 휴식을 취했습니다.\n");
			heal = 10 + rand() % (30 - 10 + 1) * player->LEVEL; //플레이어 레벨에 따른 체력 회복
			if (heal % 8 == 0 || heal % 9 == 0) { //일정한 확률로 체력 회복에 실패
				printf("\n깜빡 잠들어 악몽을 꿧습니다. 제대로 된 휴식을 취하지 못했습니다.\n");
			}else if (heal % 7 == 0) { //일정한 확률로 적의 습격을 받음
				printf("\n적의 습격을 받았습니다.\n");
				battle_enemy(player, item, "슬라임", 1);
			}else {
				printf("\n체력을 회복합니다.\n");
				player->HP += heal;
				if (player->HP > player->MAX_HP) {
					player->HP = player->MAX_HP;
				}
			}
			break;
		case 3: //3번을 선택할 시 아이템 사용 함수로
			use_item(player, item, 1, 0);
			break;
		default:
			printf("\n잘못된입력\n");
		}
		printf("\n");
		system("pause");
		system("cls"); //화면 초기화
	}
}

void set_information(player* player) {
	player->HP = 100; //플레이어 초기 세팅은 1레벨에 맞춰서
	player->MAX_HP = 100;
	player->EXP = 0;
	player->LEVEL = 1;
	player->attack = 10; //1레벨 공격력
	player->money = 0;
}

void set_state(player* player) {
	if (player->EXP >= 1000) { //경험치가 1000 이상일 때
		printf("\nLEVEL UP!!\n");
		player->EXP = player->EXP - 1000; //누적된 경험치 회수
		if (player->LEVEL == 10) { //이미 최대 레벨일 때
			printf("\n(이미 최대 레벨 입니다.)\n");
			char medal[100]; //업적 달성
			strcpy(medal, "[ 한계 돌파 ] 10레벨에서 한번 더 레벨업 하기.\n");
			FILE* file = fopen("Medal.txt", "a");
			if (file == NULL) {
				printf("Error opening Medal.txt\n");
				return;
			}
			fprintf(file, "%s\n", medal);
			fclose(file);
			return;
		}
		player->LEVEL++; //레벨 올려주기
		if (player->LEVEL != 1) {
			player->attack = 10 * ((player->LEVEL - 1) * 1.2); //레벨에 따른 플레이어 공격력 설정
			player->MAX_HP = 100 * player->LEVEL; //레벨에 따른 플레이어 체력 설정
			player->HP = player->MAX_HP; //레벨에 따른 플레이어 체력 설정
			//1-10, 2-12, 3-24, 4-36, 5-48, 6-60, 7-72, 8-84, 9-96, 10-108
		}
	}
}

void down_floor(player* player, item* item) {
	int choice = 0; //플레이어 선택 저장
	printf("\n당신은 아래로 내려가는 계단을 발견했습니다.\n"
		"1. [ 내려 간다 ]\n2. [ 무시 한다 ]\n"
		"입력: ");
	scanf("%d", &choice);
	if (choice == 1) { //플레이어가 내려간다를 선택했을 때
		player->FLOOR--; //층수 낮추기
		if (player->FLOOR == 3) { //만약 내려간 층이 3층이라면
			printf("\n계단을 내려갑니다.\n");
			printf("━┓\n"
				" ┗━┓    ↘\n"
				"   ┗━┓    ↘     1    2    3\n"
				"     ┗━┓    ↘ ┏━━━┓┏━━━┓┏━━━┓\n"
				"       ┗━┓    ┃   ┃┃   ┃┃   ┃\n"
				"         ┗━┓  ┃º  ┃┃º  ┃┃º  ┃\n"
				"           ┗━━┗━━━┛┗━━━┛┗━━━┛\n"
				"\n계단을 내려가자 잠겨있는 세 개의 문이 보입니다.(열쇠 아이템이 필요합니다.)"
				"\n1. [ 1번문 선택]\n2. [ 2번문 선택]\n3. [ 3번문 선택]\n"
				"입력: ");
			scanf("%d", &choice);
			if (choice == 1) { //1번문 선택 시
				printf("\n1번문을 선택했습니다.\n");
				if (item[6].count == 0) { //열쇠 아이템이 없을 때
					printf("\n문은 단단히 잠겨있습니다.\n");
					player->FLOOR++; //이전에 낮췄던 층수 다시 올리기
				}else {
					item[6].count--; //열쇠 아이템 소모
					printf("\n\'열쇠\'를 사용하여 문을 열었습니다.\n무사히 3층으로 도착한 것 같습니다.\n");
				}
			}else if (choice == 2) { //2번 문을 선택했을 때 구덩이 발견
				printf("\n2번문을 선택했습니다.\n");
				if (item[6].count == 0) {
					printf("\n문은 단단히 잠겨있습니다.\n");
					player->FLOOR++;
				}else {
					item[6].count--;
					printf("\n\'열쇠\'를 사용하여 문을 열었습니다.\n문 앞에 구멍과 끊어진 사다리가 보입니다.\n구멍에 빠지시겠습니까?\n");
					if (player->HP <= 250) { //플레이어의 피가 150이하라면 경고해주기
						printf(" (잘못하단 죽을것 같다) ");
					}
					printf("\n1. [ 빠져 본다 ]\n2. [ 지나 간다 ]\n입력: ");
					scanf("%d", &choice);
					if (choice == 1) { //1을 선택했을 때
						player->FLOOR--; //한 층 더 떨어지기
						player->HP -= 250; //플레이어의 체력 - 250
						if (player->HP < 0) { //체력이 0보다 작을 때 0으로 고정
							player->HP = 0;
						}
						if (player->HP <= 0) { //플레이어의 체력이 0일때
							printf("\n당신은 낙사하였습니다.\n");
							badend(); //배드 엔딩으로 진행
						}else {
							printf("\n당신은 사다리를 놓치고 떨어져 다쳤습니다.\n체력이 150 깎입니다.\n");
						}
					}else {
						printf("\n당신은 구멍을 지나치기로 합니다.\n5층의 여정을 시작합니다.\n");
					}
				}
			}
			else if (choice == 3) { //3번 문 선택시 요정과 전투
				printf("\n3번문을 선택했습니다.\n");
				if (item[6].count == 0) {
					printf("\n문은 단단히 잠겨있습니다.\n");
					player->FLOOR++;
				}else {
					item[6].count--;
					printf("\n\'열쇠\'를 사용하여 문을 열었습니다.\n문 앞에는 적이 보입니다. 전투를 피할 수 없을것 같습니다.");
					battle_enemy(player, item, "요정", 1); //요정과 전투 함수로 이동
				}
			}
			else {
				printf("\n당신은 계단을 내려가지 않기로 결정했습니다.\n");
				player->FLOOR++; //낮췄던 층수 올리기
			}
		}
		else if (player->FLOOR == 1) { //만약 내려간 층이 1층이라면
			printf("\n━┓\n ┗━┓    ↘\n   ┗━┓    ↘\n     ┗━┓    ↘\n       ┗━┓\n         ┗━┓\n           ┗━━\n\n");
			printf("한 층을 내려갔습니다.\n내려온 곳은 1층입니다.\n빛이 보이는 탈출구 앞에 아주 강력한 적이 보입니다.\n도전하시겠습니까?\n1. [ 도전 한다 ]\n2. [ 올라 간다 ]\n입력: ");
			scanf("%d", &choice);
			switch (choice) {
			case 1:
				printf("\n당신은 강력한 적에게 도전하기로 했습니다.\n");
				battle_enemy(player, item, "1층의 문지기", 0); //1층의 문지기와 전투 함수로 이동
				printf("\n자리를 벗어나자 뒤에 있던 탈출구가 바로 보입니다.\n"
					"\n┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"
					"┃                                 ┃\n"
					"┃        탈출하시겠습니까?        ┃\n"
					"┃                                 ┃\n"
					"┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n\n"
					"\n1. [ 탈출 한다 ]\n2. [ 탈출 한다 ]\n"
					"입력: ");
				scanf("%d",&choice);
				goodend(); //무엇을 선택하든 엔딩 함수로 이동
				break;
			case 2:
				printf("\n당신은 다시 2층으로 올라가기로 했습니다.\n");
				player->FLOOR++; //다시 올라가기
				break;
			default:
				printf("\n당신은 도전하지 않기로 결정했습니다.\n");
				player->FLOOR++; //범위 외 숫자 선택 시 다시 올라가기
			}
		}else if (player->FLOOR == 0) { //만약 0층이라면? (불가능함)
			printf("\n1층에서 내려간곳은 지하...1층?.\n");
			badend(); //배드엔딩 함수로 이동
		}else { //그 외의 층일 때 계단 그림? 출력
			printf("\n━┓\n ┗━┓    ↘\n   ┗━┓    ↘\n     ┗━┓    ↘\n       ┗━┓\n         ┗━┓\n           ┗━━\n\n");
			printf("한 층을 내려갔습니다.\n");
		}
	}
	else {
		printf("\n당신은 계단을 지나쳤습니다.\n");
	}
}

void store(player* player, item* item) {
	int choice, answer;
	float sale = 1; //변수 선언
	printf("\n로브를 쓰고있는 수상한 상인을 만났습니다.\n"
		"\n상점을 구경하시겠습니까?\n"
		"1. [ 구경 한다 ]\n2. [아이템 사용]\n3. [ 무시 한다 ]\n"
		"입력: ");
	scanf("%d", &choice);
	if (choice == 2) { //2번을 선택했다면
		choice = 1; //1번 선택지로 가도록
		sale = use_item(player, item, 3, sale); //아이템 사용 함수로 이동하여 sale에 반환값 받기
	}
	if (choice == 1) {
		choice = rand() % 4;
		if (sale == 0.5 && choice == 0) { // 세일값이 있고, 랜덤값이 0일 때
			choice = 2; //다른값으로 수정
		}

		printf("\n");
		system("pause");
		system("cls");
		printf("= = = = = = = = = = = = = = = = = = = = = =\n"
			"현재 층 : %d층 | HP : %d / %d | LV : %d | EXP : %d | 공격력 : %d | 돈 : %d\n",
			player->FLOOR, player->HP, player->MAX_HP, player->LEVEL, player->EXP, player->attack, player->money);
		switch (choice) {
		case 0:
			printf("\n상인은 다짜고짜 당신을 공격합니다.\n");
			battle_enemy(player, item, "수상한 상인", 1); //상인과의 전투
			return;
			break;
		case 1:
			printf("\n┏━━━1━━━━━━2━━━━━━3━━┓\n"
				"┃  튀  ┃ 마카 ┃ 강냉 ┃\n┃  밥  ┃ 로니 ┃  이  ┃\n┣━━━━━━╋━━━━━━╋━━━━━━┫\n┃ 1000 ┃ 1000 ┃  500 ┃"
				"\n┗━━━━━━━━━━━━━━━━━━━━┛\n"
				"\n상인이 물품을 보여줍니다.\n"
				"\n상인이 한 물건을 골라보라고 합니다.\n"
				"\n어떤것을 고르겠습니까?\n"
				"1. [ 1번  선택 ]\n2. [ 2번  선택 ]\n3. [ 3번  선택 ]\n4. [ 무시 한다 ]\n"
				"입력: ");
			//선택한 물건에 따라 세일된 가격을 확인하여 플레이어으 소지금에 따라 아이템 획득, 또는 무시
			scanf("%d", &choice);
			if (choice == 1) {
				if (player->money >= 1000*sale) {
					printf("\n튀밥을 구매했습니다!\n");
					player->money -= 1000 * sale;
					item[0].count++;
				}else {
					printf("\n물건을 구매하기엔 돈이 부족합니다.\n");
				}
			}else if(choice == 2) {
				if (player->money >= 1000 * sale) {
					printf("\n마카로니를 구매했습니다!\n");
					player->money -= 1000 * sale;
					item[1].count++;
				}else {
					printf("\n물건을 구매하기엔 돈이 부족합니다.\n");
				}
			}else if (choice == 3) {
				if (player->money >= 500 * sale) {
					printf("\n강냉이를 구매했습니다!\n");
					player->money -= 500 * sale;
					item[2].count++;
				}else {
					printf("\n물건을 구매하기엔 돈이 부족합니다.\n");
				}
			}else if (choice == 4) {
				printf("\n당신은 물건을 보다가 자리를 벗어납니다.\n");
				return;
			}else { //범위 외 입력에 대한 처리
				printf("\n잘못된 입력\n상인이 강냉이를 강매합니다.\n");
				if (player->money != 0) { //0원이 아니면 실행
					player->money -= 500 * sale;
					if (player->money<0) { //0보다 작을 때 0으로 설정
						player->money = 0;
					}
					item[2].count++; //강냉이 아이템 갯수 플러스
					printf("\n당신은 돈을 잃고 강냉이를 얻었습니다.\n");
				}else if (player->money == 0) { //만약 돈이 빵원이면
					printf("\n이런!! 당신은 강냉이를 살 돈조차 없습니다.\n터덜터덜 자리를 떠납니다.\n");
				}
			}
			break;
		case 2:
			printf("\n┏━━━1━━━━━━2━━━━━━3━━┓\n"
				"┃  튀  ┃  폭  ┃  지  ┃\n┃  밥  ┃  탄  ┃  도  ┃\n┣━━━━━━╋━━━━━━╋━━━━━━┫\n┃ 1000 ┃ 1500 ┃ 2000 ┃"
				"\n┗━━━━━━━━━━━━━━━━━━━━┛\n"
				"\n상인이 물품을 보여줍니다.\n"
				"\n상인이 한 물건을 골라보라고 합니다.\n"
				"\n어떤것을 고르겠습니까?\n"
				"1. [ 1번  선택 ]\n2. [ 2번  선택 ]\n3. [ 3번  선택 ]\n4. [ 무시 한다 ]\n"
				"입력: ");
			scanf("%d", &choice);
			if (choice == 1) { //위와 동일 (판매 물품만 다름)
				if (player->money >= 1000 * sale) {
					printf("\n튀밥을 구매했습니다!\n");
					player->money -= 1000 * sale;
					item[0].count++;
				}else {
					printf("\n물건을 구매하기엔 돈이 부족합니다.\n");
				}
			}else if (choice == 2) {
				if (player->money >= 1500 * sale) {
					printf("\n폭탄을 구매했습니다!\n");
					player->money -= 1500 * sale;
					item[3].count++;
				}else {
					printf("\n물건을 구매하기엔 돈이 부족합니다.\n");
				}
			}else if (choice == 3) {
				if (player->money >= 2000 * sale) {
					printf("지도를 구매했습니다!");
					player->money -= 2000 * sale;
					item[4].count++;
				}else {
					printf("\n물건을 구매하기엔 돈이 부족합니다.\n");
				}
			}else if (choice == 4) {
				printf("\n당신은 물품을 감상하다 자리를 벗어납니다.\n");
				return;
			}else {
				printf("\n잘못된 입력\n상인이 강냉이를 강매합니다.\n");
				if (player->money != 0) {
					player->money -= 500 * sale;
					if (player->money < 0) { //0보다 작을 때 0으로 설정
						player->money = 0;
					}
					item[2].count++;
					printf("\n당신은 돈을 잃고 강냉이를 얻었습니다.\n");
				}
				else if (player->money == 0) {
					printf("\n이런!! 당신은 강냉이를 살 돈조차 없습니다.\n터덜터덜 자리를 떠납니다.\n");
				}
			}
			break;
		case 3: //랜덤값이 3이 나왔을 때
			printf("\n상인이 거래를 제안합니다.\n"
				"\n┏━━━━━━━━━ 상인의 제안 ━━━━━━━━━━━┓\n"
				"┃                                 ┃\n"
				"┃   상인과 돈을 걸고 내기를 하여  ┃\n"
				"┃ 승리한다면 특별한 아이템을 획득 ┃\n"
				"┃                                 ┃\n"
				"┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n"
				"\n제안에 응하겠습니까?\n"
				"1. [ 수락 한다 ]\n2. [ 거절 한다 ]\n"
				"입력: ");
			scanf("%d", &choice);
			printf("\n");
			system("pause");
			system("cls");
			printf("= = = = = = = = = = = = = = = = = = = = = =\n"
				"현재 층 : %d층 | HP : %d / %d | LV : %d | EXP : %d | 공격력 : %d | 돈 : %d\n",
				player->FLOOR, player->HP, player->MAX_HP, player->LEVEL, player->EXP, player->attack, player->money);
			if (choice==1){ //플레이어가 상인의 제안에 응했을 때
				quiz(player, item); //퀴즈 함수로 이동
			}else if(choice == 2) { //제안에 거절했을 때
				char medal[100]; //물가상승 업적 달성
				strcpy(medal, "[ 물가상승 ] 상인의 제안을 거절해 바가지를 쓴다.\n");
				FILE* file = fopen("Medal.txt", "a");
				if (file == NULL) {
					printf("Error opening Medal.txt\n");
					return;
				}
				fprintf(file, "%s\n", medal);
				fclose(file);
				printf("\n상인의 얼굴은 보이지 않지만 기분이 나빠진것 같습니다.\n바가지를 씌웁니다.\n"
					"\n┏━━━1━━━━━━2━━━━━━3━━┓\n"
					"┃ 마카 ┃  폭  ┃  지  ┃\n┃ 로니 ┃  탄  ┃  도  ┃\n┣━━━━━━╋━━━━━━╋━━━━━━┫\n┃ 1500 ┃ 2250 ┃ 3000 ┃"
					"\n┗━━━━━━━━━━━━━━━━━━━━┛\n"
					"\n상인이 물품을 보여줍니다.\n"
					"\n상인이 한 물건을 골라보라고 합니다.\n"
					"\n어떤것을 고르겠습니까?\n"
					"1. [ 1번  선택 ]\n2. [ 2번  선택 ]\n3. [ 3번  선택 ]\n"
					"입력: ");
				scanf("%d", &choice);
				if (choice == 1) { //가격이 1.5배로 증가 외에는 위와 동일
					if (player->money >= 1500 * sale) {
						printf("\n마카로니를 구매했습니다!\n");
						player->money -= 1500*sale;
						item[0].count++;
					}else {
						printf("\n물건을 구매하기엔 돈이 부족합니다.\n");
					}
				}else if (choice == 2) {
					if (player->money >= 2250 * sale) {
						printf("\n폭탄을 구매했습니다!\n");
						player->money -= 2250 * sale;
						item[0].count++;
					}else {
						printf("\n물건을 구매하기엔 돈이 부족합니다.\n");
					}
				}else if (choice == 3) {
					if (player->money >= 3000 * sale) {
						printf("\n지도를 구매했습니다!\n");
						player->money -= 3000 * sale;
						item[0].count++;
					}else {
						printf("\n물건을 구매하기엔 돈이 부족합니다.\n");
					}
				}else {
					printf("\n잘못된 입력\n상인이 강냉이를 강매합니다.\n");
					if (player->money != 0) {
						player->money -= 750 * sale;
						item[2].count++;
						printf("\n당신은 돈을 잃고 강냉이를 얻었습니다.\n");
					}
					else if (player->money == 0) {
						printf("\n이런!! 당신은 강냉이를 강매할 돈조차 없습니다.\n터덜터덜 자리를 떠납니다.\n");
					}
				}
			}else{
				printf("\n당신이 선택하기도 전에 상인은 자리를 벗어납니다.\n");
			}
			break;
		default: //랜덤값에서 벗어나면 출력
			printf("이거 출력되면 오류(상점)");
		}
	}
	else if (choice == 3) { // 3(무시하기)를 선택했을 때
		printf("\n당신은 자리를 벗어납니다.\n");
	}
	else {
		printf("\n당신이 선택하기도 전에 상인은 자리를 벗어납니다.\n");
	}
	return;
}

void quiz(player* player, item* item) {
	int choice, answer=0; //행동 선택 변수와, 정답 카운트용 변수
	int principal = player->money; //원금 저장용 변수
	printf("\n당신이 제안을 받아들이자 무언가를 준비합니다.\n"
		"\n┏━━━━━━━━━ 내기의 규칙 ━━━━━━━━━━━┓\n"
		"┃                                 ┃\n"
		"┃ 1. 돈의 1/2이나 전재산을 바친다 ┃\n"
		"┃ 2.  총 다섯번의 퀴즈를 진행한다 ┃\n"
		"┃ 3. 퀴즈를 맞춘 만큼 상품을 지급 ┃\n"
		"┃                                 ┃\n"
		"┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n"
		"\n얼마를 바치실 건가요?\n"
		"1. [   1 / 2   ]\n2. [ 전 재 산  ]\n"
		"입력: ");
	scanf("%d", &choice); //바칠 재산 선택
	if (choice == 1) {
		player->money *= 0.5; //돈의 1/2만 남기기
	}else if(choice ==2){
		player->money = 0; //0원으로
	}else { //다른 선택을 했을 때
		printf("\n상인은 멋대로 전재산을 가져갑니다.\n");
		player->money = 0; //전재산 몰수
	}
	printf("\n첫번째 퀴즈\n\n'1 + 1 =' 의 정답은?\n1. [     2     ]\n2. [  창   문  ]\n입력: ");
	scanf("%d", &choice); //퀴즈에 대한 정답 입력
	if (choice==1||choice==2) {
		answer += 1; //정답 카운트 플러스
	}
	printf("\n두번째 퀴즈\'바나나의 영어 스펠링은 bananana이다.' 의 정답은?\n1. [     O     ]\n2. [     X     ]\n입력: ");
	scanf("%d", &choice);
	if (choice == 2) {
		answer += 1;
	}
	printf("\n세번째 퀴즈\n\n'XYXXXYXXYXXYXYY 에서 'X'의 갯수는?' 의 정답은?\n1. [     8     ]\n2. [     9     ]\n입력: ");
	scanf("%d", &choice);
	if (choice == 2) {
		answer += 1;
	}
	printf("\n네번째 퀴즈\n\n'C언어를 개발한 사람은?' 의 정답은?\n1. [ 데니스 리치 ]\n2. [ 리치 데니스 ]\n입력: ");
	scanf("%d", &choice);
	if (choice == 1) {
		answer += 1;
	}
	if (answer == 0 && player->money) { //정답수에 따른 보상
		char medal[100]; // 신실한 사람 업적 달성
		strcpy(medal, "[ 신실한 사람 ] 상인의 제안에 전재산을 바치고 모든 문제를 틀렸다.\n"); //66?
		FILE* file = fopen("Medal.txt", "a");
		if (file == NULL) {
			printf("Error opening Medal.txt\n");
			return;
		}
		fprintf(file, "%s\n", medal);
		fclose(file);
		printf("\n상인은 당신이 불쌍한지 잔돈을 건네줍니다.\n");
		player->money = 100; //플레이어의 돈을 100원 돌려주기
	}else if (answer == 0) { //위의 제어문을 생각해서, 1/2을 바치고, 정답이 없을 때 실행
		printf("\n당신은 상인이 낸 퀴즈를 모두 틀렸습니다.\n상인이 분노합니다.\n");
		battle_enemy(player, item, "수상한 상인", 1); //상인과의 전투
	}else if (answer == 1) { //아무일도 일어나지 않는다.
		printf("\n당신은 상인이 낸 퀴즈를 하나만 맞췄습니다.\n당신의 지능에 감탄한 상인이 자리를 뜹니다.\n");
	}else if (answer == 2) {
		printf("\n당신은 상인이 낸 퀴즈를 반만 맞췄습니다.\n바쳤던 돈을 일부 또는 전부 돌려받습니다.\n");
		player->money = principal*0.5; //원금의 반을 돌려받음(반만 바치면 전재산)
	}else if (answer == 3) {
		printf("\n당신은 상인이 낸 퀴즈 중 한문제만 틀렸습니다!\n돈을 돌려받고, 아이템을 얻었습니다.\n");
		player->money = principal; //원금으로 복구
		choice = rand() % +7; //랜던값을 정하여
		item[choice].count += 1; //랜덤한 아이템 획득
	}else if (answer == 4) {
		printf("\n당신은 상인이 낸 퀴즈를 모두 맞췄습니다!!!\n돈을 두배로 돌려받고, 특별한 아이템을 얻었습니다.\n");
		player->money = (principal - player->money) * 2; //원금과 비교하여 두배로 돈을 설정
		item[6].count += 1; //열쇠 아이템 획득
	}
}

int player_action(player* player, item* item, int enemy_hp) {
	int choice;
	printf("\n당신의 행동을 결정하세요.(선택은 취소할 수 없습니다.)\n"
		"1. [  공   격  ]\n2. [아이템 사용]\n3. [  도   망  ]\n");
	printf("입력: ");
	scanf("%d", &choice);
	switch (choice) {
	case 1: //1(공격)을 선택 시
		printf("\n적을 공격합니다.");
		choice = rand() % 5; //각각의 숫자가 나올 확률은 1/5
		enemy_hp -= player->attack; //한번 공격
		if (!choice) { //choice가 0이 나오면 1/5확률로
			printf("  !!!적의 약점을 공격했습니다.!!!");
			enemy_hp -= player->attack; //한번 더 공격
		}
		if (enemy_hp < 0) { //적의 체력을 0으로 고정
			enemy_hp = 0;
		}
		return enemy_hp; //변화한 적의 체력 반환
		break;
	case 2: //2(아이템 사용)을 선택 시
		enemy_hp = use_item(player, item, 2, enemy_hp); //아이템 사용함수로 이동, 반환값 받기
		return enemy_hp;
		break;
	case 3:
		printf("\n도망에 성공하였습니다.\n");
		enemy_hp = -10000; //적의 체력을 -10000으로 설정
		return enemy_hp; //적 체력 반환
		break;
	default:
		printf("\n!!!!!!이거 뜨면 오류!!!!!!\n");
	}
}

void battle_enemy(player* player, item* item, char* enemy_name, int isbattle) { //플레이어, 아이템, 몬스터 이름, 전투 상황
	int plus_EXP, plus_money, choice; //변수 선언
	int enemy_HP, enemy_hp, enemy_attack; //적에 관련된 변수 선언
	int min = player->LEVEL * 10; //레벨에 따른 min값
	int max = 12 * (6 - player->FLOOR); //층에 따른 max값
	if (max < min) {
		int tmp = min;
		min = max;
		max = tmp;
	}
	if (strcmp(enemy_name, "1층의 문지기") == 0) { //적의 이름이 1층의 문지기라면
		enemy_attack = 108; //플레이어가 10레벨일 때 체력과 공격력 적용
		enemy_hp = 1000;
	}else {
		enemy_attack = min + rand() % (max - min + 1); //층수에 따른 적의 공격력
		enemy_hp = (enemy_attack * 0.2) * (30 - player->FLOOR); //층수에 따른 적의 체력
	}
	enemy_HP = enemy_hp; //적의 이전 체력을 알기 위한 변수

	printf("\n%s(와)과의 전투를 시작합니다.\n"
		"(%s의 현재 체력: %d)\n"
		"(당신의 현재 체력: %d)\n", enemy_name, enemy_name, enemy_hp, player->HP); //적의 정보 출력
	while (1) { //무한 반복
		printf("\n");
		system("pause");
		system("cls");
		printf("= = = = = = = = = = = = = = = = = = = = = =\n"
			"현재 층 : %d층 | HP : %d / %d | LV : %d | EXP : %d | 공격력 : %d | 돈 : %d\n",
			player->FLOOR, player->HP, player->MAX_HP, player->LEVEL, player->EXP, player->attack, player->money);
		printf("\n%s (와)과의 전투 중 입니다.\n"
			"(%s의 현재 체력: %d)\n", enemy_name, enemy_name, enemy_hp);
		if (isbattle == 0) { //적에게 습격받았을 때 첫 턴 공격 넘거가기
			enemy_HP = enemy_hp;
			enemy_hp = player_action(player, item, enemy_hp); //플레이어 행동 함수 호출
			if (enemy_hp == -10000) { //도망쳤을 때의 몬스터 체력
				printf("\n...\n");
				if (strcmp(enemy_name, "1층의 문지기") == 0) { //적의 이름이 동일할 때
					char medal[100]; //또 다른 방법 업적 달성
					strcpy(medal, "[ 또 다른 방법 ] 1층의 문지기와의 대결에서 도망쳤다.\n");
					FILE* file = fopen("Medal.txt", "a");
					if (file == NULL) {
						printf("Error opening Medal.txt\n");
						return;
					}
					fprintf(file, "%s\n", medal);
					fclose(file);
					printf("\n1층의 문지기를 물리치지 않고 도망쳤습니다.\n");
				}
				return;
			}if (enemy_HP != enemy_hp) { //적의 체력에 변화가 생겼을 때
				printf("\n%s(이)가 총 %d의 데미지를 입었습니다. \n", enemy_name, enemy_HP - enemy_hp);
			}
			if (enemy_hp == 0) { //적의 체력이 0이 됐을 때
				plus_EXP = 200 + rand() % (400 - 200 + 1); //랜덤한 경험치값 저장
				plus_money = 200 + rand() % (300 - 200 + 1); //랜덤한 값의 돈 저장
				printf("\n%s(이)가 처치되었습니다.\n"
					"\n%d만큼의 경험치를 얻었습니다.\n"
					"%d만큼의 코인을 얻었습니다.\n", enemy_name, plus_EXP, plus_money);
				player->EXP += plus_EXP; // 저장된 경험치값 추가
				player->money += plus_money; //저장된 돈값 추가
				set_state(player); //플레이어 상태 설정 함수로 이동
				if (strcmp(enemy_name, "수상한 상인") == 0) { //이름이 동일할 때
					item[6].count += 1; //열쇠 아이템 획득
				}else if (strcmp(enemy_name, "1층의 문지기") == 0) { //이름이 동일힐 때
					char medal[100]; //정정당당한 대결 업적 달성
					strcpy(medal, "[ 정정당당한 대결 ] 1층의 문지기를 물리쳤다.\n");
					FILE* file = fopen("Medal.txt", "a");
					if (file == NULL) {
						printf("Error opening Medal.txt\n");
						return;
					}
					fprintf(file, "%s\n", medal);
					fclose(file);
					printf("\n1층의 문지기를 물리쳐 쓰러트렸습니다.\n");
				}else {
					choice = rand() % 6; //랜덤값 저장
					item[choice].count += 1; //렌덤한 아이템 획득
					printf("\n!!! 아이템 %s(을)를 획득했습니다!\n",item[choice].name);
				}
				break;
			}
		}
		isbattle = 0; //습격 받았을 때 0값으로 돌려놓기
		player->HP -= enemy_attack; //적의 플레이어 공격
		if (player->HP < 0) { //체력이 0보다 작을 때 0으로 고정
			player->HP = 0;
		}
		printf("\n적이 당신을 공격합니다.\n"
			"당신은 총 %d의 데미지를 입었습니다.(당신의 현재 체력: %d)\n", enemy_attack, player->HP);
		if (player->HP <= 0) { //플레이어의체력이 0보다 작아질 때
			printf("\n당신의 체력이 0이 되어 사망하였습니다.\n");
			badend(); //엔딩으로 이동
		}
	}
}

int use_item(player* player, item* item, int place, float value) { //place 1은 아이템 사용(선택지), 2는 전투, 3은 상인
	int choice, Damage; //선택 변수, 데미지변수 선언
	int heal = 0; //체력 회복에 대한 변수 선언 및 초기화
	printf("\n당신은 아이템을 사용합니다.\n현재 보유한 아이템\n");
	for (int i = 0; i < 7; i++) { //아이템 갯수만큼 반복하여 보유한 아이템 목록 출력
		printf("아이템: %8s  %3d개\n", item[i].name, item[i].count);
	}
	printf("\n사용하실 아이템을 선택하세요\n");
	switch (place) { //장소에 따른 아이템 사용
	case 1:
		printf("\n1. [  튀   밥  ]\n2. [ 마카 로니 ]\n3. [ 강 냉 이  ]\n4. [  폭   탄  ]\n5. [  지   도  ]\n입력: ");
		scanf("%d", &choice);
		if (choice == 1) { //아이템을 소유했는지에 따라 아이템 사용이 결정됨
			if (item[0].count == 0) { //아이템이 없을 때
				printf("\n소유한 아이템이 없습니다.\n");
			}else {
				item[0].count -= 1; //아이템이 있을 때 count 줄이기
				printf("\n당신은 튀밥을 먹습니다. (매우 맛있었다!)\n체력이 100 회복됩니다.\n");
				player->HP += 100; //아이템 사용에 대한 체력 회복
				if (player->HP > player->MAX_HP) { //최대 체력이 넘지 않도록
					player->HP = player->MAX_HP;
				}
			}
		}else if (choice == 2) {
			if (item[1].count == 0) {
				printf("\n소유한 아이템이 없습니다.\n");
			}else {
				item[1].count -= 1;
				heal = 100 + rand() % (200 - 100 + 1); //랜덤값 설정
				printf("\n당신은 마카로니를 먹습니다.\n여유로운 휴식시간을 충분히 즐기며 마카로니를 끊임없이 먹습니다\n체력이 %d 회복됩니다.\n", heal);
				player->HP += heal;
				if (player->HP > player->MAX_HP) {
					player->HP = player->MAX_HP;
				}
			}
		}else if (choice == 3) {
			if (item[2].count == 0) {
				printf("\n소유한 아이템이 없습니다.\n");
			}else {
				item[2].count -= 1;
				printf("\n당신은 강냉이를 먹습니다.\n강냉이보다는 튀밥이 더 끌립니다.\n체력이 50 회복됩니다.\n");
				player->HP += 50;
				if (player->HP > player->MAX_HP) {
					player->HP = player->MAX_HP;
				}
				char medal[100]; //몰랐던 취향 업적 달성
				strcpy(medal, "[ 몰랐던 취향 ] 아이템 사용에서 강냉이를 먹었다.\n");
				FILE* file = fopen("Medal.txt", "a");
				if (file == NULL) {
					printf("Error opening Medal.txt\n");
					return 1;
				}
				fprintf(file, "%s\n", medal);
				fclose(file);
			}
		}else if (choice == 4) {
			if (item[3].count == 0) {
				printf("\n소유한 아이템이 없습니다.\n");
			}else {
				item[3].count -= 1;
				char medal[128]; //휴식 대신 고통을 업적 달성
				strcpy(medal, "[ 휴식 대신 고통을 ] 아이템 사용에서 폭탄을 사용하여 피해를 받았다.\n");
				FILE* file = fopen("Medal.txt", "a");
				if (file == NULL) {
					printf("Error opening Medal.txt\n");
					return 1;
				}
				fprintf(file, "%s\n", medal);
				fclose(file);

				Damage = (50 + rand() % (100 - 50 + 1)) * player->LEVEL; //플레이어 레벨에 따른 데미지 저장
				player->HP -= Damage; //체력 감수
				if (player->HP <= 0) {
					player->HP = 1;
				}
				printf("\n폭탄이 당신을 공격합니다.\n당신은 총 %d의 데미지를 입었습니다.(당신의 현재 체력: %d)\n", Damage, player->HP);
			}
		}else if (choice == 5) {
			if (item[4].count == 0) {
				printf("\n소유한 아이템이 없습니다.\n");
			}else {
				item[4].count -= 1;
				printf("\n지도를 펼쳐 휴식시간동안 여유롭게 길을 고민합니다.\n생각 끝에 당신은 계단이 있을 장소를 유추했습니다!\n");
				down_floor(player, item); //계단 발견 함수로 이동
			}
		}else {
			printf("\n잘못된입력\n");
		}
		break;
	case 2: //몬스터와의 전투 상황일 때(아이템 종류를 제외하고 위와 동일)
		printf("1. [  튀   밥  ]\n2. [ 마카 로니 ]\n3. [ 강 냉 이  ]\n4. [  폭   탄  ]\n입력: ");
		scanf("%d", &choice);
		if (choice == 1) {
			if (item[0].count == 0) {
				printf("\n소유한 아이템이 없습니다.\n");
			}else {
				item[0].count -= 1;
				printf("\n당신은 적 앞에서 튀밥을 먹습니다. (매우 맛있었다!)\n체력이 100 회복됩니다.\n");
				player->HP += 100;
				if (player->HP > player->MAX_HP) {
					player->HP = player->MAX_HP;
				}
			}
		}else if (choice == 2) {
			if (item[1].count == 0) {
				printf("\n소유한 아이템이 없습니다.\n");
			}else{
				item[1].count -= 1;
				heal = 50 + rand() % (150 - 50 + 1);
				printf("\n당신은 적 앞에서 마카로니를 먹습니다.\n손이 계속가는 맛이지만 겨우 전투로 돌아간다.\n체력이 %d 회복됩니다.\n", heal);
				player->HP += heal;
				if (player->HP > player->MAX_HP) {
					player->HP = player->MAX_HP;
				}
			}
		}else if (choice == 3) {
			if (item[2].count == 0) {
				printf("\n소유한 아이템이 없습니다.\n");
			}else {
				item[2].count -= 1;
				printf("\n당신은 적 앞에서 강냉이를 먹습니다.\n지금 상황이 마치 영화처럼 느껴집니다.\n체력이 50 회복됩니다.\n");
				player->HP += 50;
				if (player->HP > player->MAX_HP) {
					player->HP = player->MAX_HP;
				}
				char medal[100]; //영화 같은 순간 업적 달성
				strcpy(medal, "[ 영화같은 순간 ] 적과의 전투 도중 팝콘... 아니 강냉이를 먹었다.\n");
				FILE* file = fopen("Medal.txt", "a");
				if (file == NULL) {
					printf("Error opening Medal.txt\n");
					return 1;
				}
				fprintf(file, "%s\n", medal);
				fclose(file);
			}
		}else if (choice == 4) {
			if (item[3].count == 0) {
				printf("\n소유한 아이템이 없습니다.\n");
			}else {
				item[3].count -= 1;
				char medal[50];  //폭탄이다! 업적 달성
				strcpy(medal, "[ 폭탄이다! ] 적에게 폭탄을 사용하였다.\n");
				FILE* file = fopen("Medal.txt", "a");
				if (file == NULL) {
					printf("Error opening Medal.txt\n");
					return 1;
				}
				fprintf(file, "%s\n", medal);
				fclose(file);

				Damage = (50 + rand() % (100 - 50 + 1)) * player->LEVEL; //레벨에 따른 랜덤 데미지 저장
				value -= Damage; //데미지 값을 저장
				if (value < 0) {//0보다 작을 때 0으로 고정
					value = 0;
				}
				printf("\n폭탄을 던져 적에게 총 %d의 데미지를 입혔습니다.\n", Damage);
			}
		}else {
			printf("\n잘못된입력\n");
		}

		break;
	case 3: //상점에서 아이템 종류를 제외하곤 위와 동일
		printf("1. [ 강 냉 이  ]\n2. [  지   도  ]\n3. [  티   켓  ]\n입력: ");
		scanf("%d", &choice);
		if (choice == 1) {
			if (item[2].count == 0) {
				printf("\n소유한 아이템이 없습니다.\n");
			}else {
				item[2].count -= 1;
				printf("\n당신은 축복받은 강냉이를 먹습니다.\n체력이 100 회복됩니다.\n");
				player->HP += 100;
				if (player->HP > player->MAX_HP) {
					player->HP = player->MAX_HP;
				}
				char medal[100]; //강냉이에게 축복을! 업적 달성
				strcpy(medal, "[ 강냉이에게 축복을! ] 상인의 앞에서 강냉이 먹기.\n");
				FILE* file = fopen("Medal.txt", "a");
				if (file == NULL) {
					printf("Error opening Medal.txt\n");
					return 1;
				}
				fprintf(file, "%s\n", medal);
				fclose(file);
			}
		}else if (choice == 2) {
			if (item[4].count == 0) {
				printf("\n소유한 아이템이 없습니다.\n");
			}else {
				item[4].count -= 1;
				printf("\n당신이 지도를 펼치자 상인이 길을 알려줍니다.\n상인이 안내해준 장소에는 계단이 있었습니다!\n");
				down_floor(player, item);
			}
		}else if (choice == 3) {
			if (item[5].count == 0) {
				printf("\n소유한 아이템이 없습니다.\n");
			}else {
				item[5].count -= 1;
				printf("\n상인에게 티켓을 건네주자 아이템의 가격을 할인해준다고 합니다. 오예!\n");
				value = 0.5; //value값을 0.5로 대입
			}
		}else {
			printf("\n잘못된입력\n");
		}
		break;
	default:
		printf("이 메시지가 뜬다면 아이템 사용 오류");
	}
	return value; //몬스터 체력 또는, sale값 반환
}

void goodend() {
	char buffer[100]; //파일 읽기용 변수
	FILE* file = fopen("Medal.txt", "r"); //파일 읽기 열기
	printf("\n");
	system("pause");
	system("cls");
	printf("\n당신은 탈출구를 찾아 밖으로 나갔습니다.\n"
		"\n마침내 지긋지긋한 탑을 벗어났습니다.\n"
		"\n!축하합니다!\n"
		"...\n\n[Good Ending]\n\n");
	if (file == NULL) {
		printf("Error opening Medal.txt\n");
		return;
	}
	while (fgets(buffer, sizeof(buffer), file) != NULL) {
		printf("%s", buffer); //업적 목록 출력
	}
	fclose(file); //파일 닫기

	printf("\n┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"
		"┃                                 ┃\n"
		"┃   플레이 해주셔서 감사합니다.   ┃\n"
		"┃     Thank you for play game.    ┃\n"
		"┃                                 ┃\n"
		"┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n\n");
	exit(0);
}

void badend() {
	char buffer[100];
	FILE* file = fopen("Medal.txt", "r");
	printf("\n");
	system("pause");
	system("cls");
	printf("\n당신은 목숨을 잃었습니다.\n\n눈을 뜨니 어딘가 익숙한 장소가 눈에 뜁니다.\n"
		"\n아늑해 보이지만 인기척이 없어 싸늘합니다.\n"
		"바로 앞 층을 내려가는 계단이 보입니다.\n"
		"   ┏━━━━━━━━┓\n   ┃        ┃\n   ┃   ├┐   ┃\n   ┃   └┘   ┃\n   ┃        ┃\n   ┃        ┃\n━━━┛        ┗━━━\n\n"
		"...\n\n[Bad Ending]\n\n");

	if (file == NULL) {
		printf("Error opening Medal.txt\n");
		return;
	}
	while (fgets(buffer, sizeof(buffer), file) != NULL) {
		printf("%s", buffer);
	}
	fclose(file);
	exit(0);
}