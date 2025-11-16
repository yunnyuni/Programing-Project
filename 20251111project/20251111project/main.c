#include<stdio.h>
#include<stdlib.h>
#include<time.h>

//#define MAX_HP 500;

typedef struct{
	int HP; //체력
	int ATK; //공격력
	int DFN; //방어력
	int PTN; //포션 갯수
}role;

typedef struct {
	int HP;
	int ATK;
}dragon;

void set_state(role*);
void game(role*, role*, role*, role*, dragon*);
int warrior_action(role*, int);
int healer_action(role*, role*, role*, role*, int);
int defender_action(role*, int);
void wizard_action(role*, dragon*);
int attack(int, int);
void potion(role*);
void dragon_attack(role*, role*, role*, role*, dragon*);
void information(role*, role*, role*, role*, dragon*);

int main(){
	srand((unsigned int)time(NULL));

	role warrior; //전사
	role healer; //힐러
	role defender; //탱커
	role wizard; //마법사
	dragon dog = {3000,100};

	role* pwa = &warrior;
	role* ph = &healer;
	role* pd = &defender;
	role* pwi = &wizard;
	dragon* dragon = &dog;

	printf("= = = DRAGON vs YOU = = =\n"
		"- - -「포션을 잘 사용해 보세요 」- - -\n\n"
		"!!!!! 입력할 때 숫자만 입력해 주세요 !!!!!\n\n" //*숫자만 입력해주세요*
		"당신은 강해보이는 드래곤을 조우했습니다.\n"
		"파티의 체력을 잘 관리하며 드래곤을 토벌해 봅시다.\n\n\n");

	set_state(pwa); //전사의 상태 결정
	pwa->ATK += 50;
	set_state(ph); //힐러의 상태 결정
	ph->ATK -= 50;
	ph->PTN += 5;
	set_state(pd); //탱커의 상태 결정
	pd->HP += 50;
	set_state(pwi); //마법사의 상태 결정
	pwi->ATK += 20;

	information(pwa, ph, pd, pwi, dragon);	//랜덤으로 결정된 역할군의 체력과 공격력 표시
	game(pwa, ph, pd, pwi, dragon); //게임 진행
	
	return 0;
}

//역할의 상태 결정
void set_state(role* character) {
	character->HP = 150 + rand() % (200 - 150 + 1);
	character->ATK = 80 + rand() % (150 - 80 + 1);
	character->DFN = 0;
	character->PTN = 5 + rand() % (10 - 5 +1);
}

//게임 반복 진행
void game(role* warrior, role* healer, role* defender, role* wizard, dragon* dog) {
	int round = 0;

	while (1) {
		round++;
		printf("\n\n### Round %d ###\n", round);
		dog->ATK = 100; //마법사 저주를 대비한 용의 공격력 초기화
		if (warrior->HP > 0 && dog->HP > 0){ //역할이 살아있으면 행동
			dog->HP = warrior_action(warrior, dog->HP);
		}
		if (healer->HP > 0 && dog->HP > 0){
			dog->HP = healer_action(healer, warrior, defender, wizard, dog->HP);
		}
		if (defender->HP > 0 && dog->HP > 0){
			dog->HP = defender_action(defender, dog->HP);
		}
		if (wizard->HP > 0 && dog->HP > 0){
			wizard_action(wizard, dog);
		}
		if (dog->HP > 0){ //드래곤이 살아있으면 공격
			dragon_attack(warrior, healer, defender, wizard, dog);
			defender->DFN = 50 + rand() % (100 - 50 + 1); //탱커의 도발 사용 후 방어력 초기화
			dragon_attack(warrior, healer, defender, wizard, dog);
		}

		printf("\n\nRound %d END\n\n\n\n", round);

		if (warrior->HP <= 0)
			printf("\n전사가 사망하였습니다.");
		if (healer->HP <= 0)
			printf("\n힐러가 사망하였습니다.");
		if (defender->HP <= 0)
			printf("\n탱커가 사망하였습니다.");
		if (wizard->HP <= 0)
			printf("\n마법사가 사망하였습니다.");
		if (dog->HP <= 0) { //드래곤 사망시
			printf("\n드래곤 토벌에 성공하였습니다!");
			break;
		}else if (warrior->HP <= 0 && (healer->HP <= 0 && (defender->HP <= 0 && wizard->HP <= 0))) { //파티 전멸시
			printf("\n\n파티가 전멸하였습니다.");
			break;
		}
	}
}

//전사의 행동 결정
int warrior_action(role* player, int dragon_hp) {
	int choice = 0;
	player->DFN = 0;//방어력 초기화

	printf("\n** 드래곤의 현재 상태입니다. **\n"
		"HP : %d\n",dragon_hp);
	printf("\n# # # # # # # # # # # # # # # # #\n\n");
	printf("** 전사의 현재 상태입니다. **\n"
		"HP : %d\n"
		"현재 보유 포션: %d\n\n", player->HP, player->PTN);
	printf("\n전사의 차례입니다.\n"
		"전사의 다음 행동을 선택해 주세요.\n"
		"(1.공격 2.방어 3.회복 포션)\n선택: ");
	scanf("%d", &choice); //행동을 입력받기

	switch (choice) {
	case 1: //공격을 선택했을때 공격함수로
		printf("\n전사가 멋지게 용을 공격합니다.\n");
		dragon_hp = attack(player->ATK, dragon_hp);
		break;
	case 2: //방어를 선택했을때 방어력 설정
		printf("\n전사가 칼로 애써 공격을 막아봅니다.\n");
		player->DFN= 50 + rand() % (100 - 50 + 1);
		break;
	case 3: //회복 포션을 선택했을때 포션함수로
		potion(player);
		break;
	default: //잘못된 입력을 했을 때
		printf("\n전사는 무딘 칼을 갈고있습니다.\n");
	}
	return dragon_hp;
}

//힐러의 행동 결정
int healer_action(role* player, role* warrior, role* defender, role* wizard, int dragon_hp) {
	int choice = 0;
	player->DFN = 0;

	printf("\n** 드래곤의 현재 상태입니다. **\n"
		"HP : %d\n", dragon_hp);
	printf("\n# # # # # # # # # # # # # # # # #\n\n");
	printf("** 힐러의 현재 상태입니다. **\n"
		"HP : %d\n"
		"현재 보유 포션: %d\n\n", player->HP, player->PTN);
	printf("\n힐러의 차례입니다.\n"
		"힐러의 다음 행동을 선택해 주세요.\n"
		"(1.공격 2.방어 3.회복 포션 4.부활)\n선택: ");
	scanf("%d", &choice);

	switch (choice) {
	case 1:
		printf("\n힐러가 스태프를 용에게 내려쳤습니다.\n");
		dragon_hp = attack(player->ATK, dragon_hp);
		break;
	case 2:
		printf("\n힐러가 탱커뒤로 숨었습니다.\n");
		player->DFN = 50 + rand() % (100 - 50 + 1);
		break;
	case 3:
		potion(player);
		break;
	case 4: //부활을 선택했을 때
		printf("\n누구를 선택하시겠습니까? (1.전사 2.탱커 3.마법사): ");
		scanf("%d", &choice); //선택한 역할군이 살아있으면 50의 체력으로 부활
		if (choice == 1) {
			if (warrior->HP <= 0) {
				warrior->HP = 50;
				printf("\n전사가 부활하였습니다.\n");
			}
			else //역할군이 살아있을 때 해당 대사를 출력
				printf("\n전사가 당황스러운 눈으로 힐러를 바라봅니다.\n");
		}
		else if (choice == 2) {
			if (defender->HP <= 0) {
				defender->HP = 50;
				printf("\n탱커가 부활하였습니다.\n");
			}
			else
				printf("\n탱커가 자신의 건강함을 보여줍니다.\n");
		}
		else if (choice == 3) {
			if (wizard->HP <= 0) {
				wizard->HP = 50;
				printf("\n마법사가 부활하였습니다.\n");
			}
			else
				printf("\n마법사는 힐러를 비웃습니다.\n");
		}
		else //잘못된 선택을 했을 때 해당 대사를 출력
			printf("\n세상 어딘가의 존재하는 누군가를 살린 것 같습니다.\n 뿌듯하겠군요.\n");
		break;
	default:
		printf("\n드래곤의 모습에 힐러는 겁을 먹었습니다.\n");
	}
	return dragon_hp;
}

//탱커의 행동 결정
int defender_action(role* player, int dragon_hp) {
	int choice = 0;

	printf("\n** 드래곤의 현재 상태입니다. **\n"
		"HP : %d\n", dragon_hp);
	printf("\n# # # # # # # # # # # # # # # # #\n\n");
	printf("** 탱커의 현재 상태입니다. **\n"
		"HP : %d\n"
		"현재 보유 포션: %d\n\n", player->HP, player->PTN);
	printf("\n탱커의 차례입니다.\n"
		"탱커의 다음 행동을 선택해 주세요.\n"
		"(1.공격 2.방어 3.회복 포션 4.도발)\n선택: ");
	scanf("%d", &choice);

	switch (choice) {
	case 1:
		printf("\n탱커가 방패를 들고 용에게 돌진했습니다.\n");
		dragon_hp = attack(player->ATK, dragon_hp);
		break;
	case 2:
		printf("\n탱커가 자랑스럽게 방패를 사용합니다.\n");
		player->DFN = 50 + rand() % (100 - 50 + 1);
		break;
	case 3:
		potion(player);
		break;
	case 4:
		printf("\n탱커는 멋진 방패를 세우고 드래곤의 시선을 사로잡습니다.\n"
			"멋진 방패에 탱커는 자기 자신마저 만족합니다.\n"
			"이번 공격에서 탱커는 무적이 됩니다.\n");
		player->DFN = 10000;
		break;
	default:
		printf("\n탱커는 방패의 금간 부분을 살펴보고 있습니다.\n");
	}
	return dragon_hp;
}

//마법사의 행동 결정
void wizard_action(role* player, dragon* dog) {
	int choice = 0;
	player->DFN = 0;

	printf("\n** 드래곤의 현재 상태입니다. **\n"
		"HP : %d\n", dog->HP);
	printf("\n# # # # # # # # # # # # # # # # #\n\n");
	printf("** 마법사의 현재 상태입니다. **\n"
		"HP : %d\n"
		"현재 보유 포션: %d\n\n", player->HP, player->PTN);
	printf("\n마법사의 차례입니다.\n"
		"마법사의 다음 행동을 선택해 주세요.\n"
		"(1.공격 2.방어 3.회복 포션 4.저주)\n선택: ");
	scanf("%d", &choice);

	switch (choice) {
	case 1:
		printf("\n마법사가 지팡이를 들고 내려칩니다.\n");
		dog->HP = attack(player->ATK, dog->HP);
		break;
	case 2:
		printf("\n방어 마법을 사용합니다.\n");
		player->DFN = 50 + rand() % (100 - 50 + 1);
		break;
	case 3:
		potion(player);
		break;
	case 4:
		dog->ATK -= 30 + rand() % (80 - 30 + 1);
		printf("\n마법사의 멋진 활약으로 이번턴 동안 드래곤의 공격력이 %d만큼 줄어듭니다.\n", 100-dog->ATK);
		break;
	default:
		printf("\n마법사가 마법 시전에 실패하였습니다.\n");
	}
}

//공격함수
int attack(int attack, int dragon_hp) {
	int attack_point = 0;
	int r = 0;

	r = rand() % 4;
	if (!r) { //플레이어 공격력 ±5 정도로 공격
		printf("\n이럴수가 잘못 휘둘렀습니다.\n");
		attack_point = (attack - 5) + rand() % ((attack + 5) - (attack - 5) + 1);
		printf("용이 %d의 타격을 입었습니다.\n", attack_point - 50);
		dragon_hp -= (attack_point - 50);
	}
	else if (r == 1) {
		printf("! ? ! 치명타 ! ? !\n");
		attack_point = (attack - 5) + rand() % ((attack + 5) - (attack - 5) + 1);
		printf("용이 %d의 타격을 입었습니다.\n", attack_point * 2);
		dragon_hp -= attack_point * 2;
	}
	else { // 1/2확률로 평타
		attack_point = (attack - 5) + rand() % ((attack + 5) - (attack - 5) + 1);
		printf("용이 %d의 타격을 입었습니다.\n", attack_point);
		dragon_hp -= attack_point;
	}
	return dragon_hp;
}

//포션 사용 함수
void potion(role* p) {
	int Heal_point = 0;

	printf("\n회복 포션을 사용합니다.\n");
	if (p->PTN > 0) { //포션을 사용한 역할군의 포션이 있을 때
		//40~80 사이의 회복
		Heal_point = 40 + rand() % (80 - 40 + 1);
		printf("체력이 %d만큼 상승했습니다.\n", Heal_point);
		p->HP += Heal_point;
		p->PTN--;
	}
	else {//포션이 없을때 출력할 대사
		printf("당신은 빈털털이 입니다.\n");
	}
}

//드래곤 공격 함수
void dragon_attack(role* warrior, role* healer, role* defender, role* wizard, dragon* dog) {
	int fireball = (dog->ATK - 10) + rand() % ((dog->ATK + 10) - (dog->ATK - 10) + 1);
	int choice = rand() % 4;
	//드래곤 공격력 ±10 공격
	if (defender->DFN >= 1000) { //탱커가 도발을 했을때
		printf("\n탱커는 가뿐히 공격을 버텨냈습니다.\n");
	}
	else {
		switch (choice) {
		case 0: //전사의 경우
			if (warrior->HP <= 0) { //전사의 피가 0 이하일 때
				printf("\n전사를 공격했지만 이미 전사는 죽어있습니다.\n\n");
				return;
			}
			printf("\n드래곤의 공격이 전사를 향합니다.\n");
			if (warrior->DFN == 0) { //전사의 방어력이 0일때
				warrior->HP -= fireball;
				printf("전사가 %d만큼의 타격을 받았습니다.\n", fireball);
			}
			else if (warrior->DFN < fireball) { //전사의 방어력이 드래곤의 공격보단 작을때
				warrior->HP -= (fireball - warrior->DFN);
				printf("방어를 하여 총 %d만큼의 피해를 입었습니다.\n", fireball - warrior->DFN);
			}
			else { //전사의 방어력이 드래곤의 공격보다 클때
				printf("전사의 칼은 생각 외로 완벽하게 방어했습니다.\n");
			}
			break; 
		case 1: //힐러의 경우
			if (healer->HP <= 0) {
				printf("\n힐러를 공격했지만 이미 힐러는 죽어있습니다.\n\n");
				return;
			}
			printf("\n드래곤의 공격이 힐러를 향합니다.\n");
			if (healer->DFN == 0) {
				healer->HP -= fireball;
				printf("힐러가 %d만큼의 타격을 받았습니다.\n", fireball);
			}
			else if (healer->DFN < fireball) {
				healer->HP -= (fireball - healer->DFN);
				printf("탱커의 방패 덕분에 총 %d만큼의 피해만 입었습니다.\n", fireball - healer->DFN);
			}
			else {
				printf("탱커의 방패는 단단했나 봅니다.\n");
			}
			break; 
		case 2: //탱커의 경우
			if (defender->HP <= 0) {
				printf("\n탱커를 공격했지만 이미 탱커는 죽어있습니다.\n\n");
				return;
			}
			printf("\n드래곤의 공격이 탱커를 향합니다.\n");
			if (defender->DFN == 0) {
				defender->HP -= fireball;
				printf("탱커가 %d만큼의 타격을 받았습니다.\n", fireball);
			}
			else if (defender->DFN < fireball) {
				defender->HP -= (fireball - defender->DFN);
				printf("최고의 방패로 총 %d만큼의 피해만 입었습니다.\n", fireball - defender->DFN);
			}
			else {
				printf("탱커의 방패는 단단했습니다.\n");
			}
			break;
		case 3: //마법사의 경우
			if (wizard->HP <= 0) {
				printf("\n마법사를 공격했지만 이미 마법사는 죽어있습니다.\n\n");
				return;
			}
			printf("\n드래곤의 공격이 마법사를 향합니다.\n");
			if (wizard->DFN == 0) {
				wizard->HP -= fireball;
				printf("마법사가 %d만큼의 타격을 받았습니다.\n", fireball);
			}
			else if (wizard->DFN < fireball) {
				wizard->HP -= (fireball - wizard->DFN);
				printf("방어마법이 도중에 깨져 총 %d만큼의 피해를 입었습니다.\n", fireball - wizard->DFN);
			}
			else {
				printf("마법사의 방어 마법은 완벽했습니다.\n");
			}
			break;
		default:
			printf("이 메시지 뜨면 오류");
		}
	}
}

//정보 출력용
void information(role* warrior, role* healer, role* defender, role* wizard, dragon* dragon) {
	printf("\n전사의 HP:   %4d   전사의 공격력:   %4d\n", warrior->HP,warrior->ATK);
	printf("힐러의 HP:   %4d   힐러의 공격력:   %4d\n", healer->HP,healer->ATK);
	printf("탱커의 HP:   %4d   탱커의 공격력:   %4d\n", defender->HP,defender->ATK);
	printf("마법사의 HP: %4d   마법사의 공격력: %4d\n",wizard->HP,wizard->ATK);
	printf("드래곤의 HP: %4d   드래곤의 공격력: %4d\n",dragon->HP,dragon->ATK);
}