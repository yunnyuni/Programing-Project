#include <stdio.h>

//-메인 스토리를 따라 진행되는 이야기
//메인 스토리 : 플레이어는 10층 높이의 탑을 탈출하기 위해 층을 내려가 바깥으로 나갈 수 있
//는 1층의 문을 찾기 위한 모험을 한다. (조건문)

//-랜덤하고 다양한 이벤트와 선택지에 따른 대사
//메인 스토리를 진행하면서 종종 랜덤한 확률로 여러 가지의 이벤트와 맞닥뜨리며 이벤트 선택
//지에 따라 분기되는 보상(난수 생성)

//-아이템과 플레이어 간의 상호작용
//스토리 또는 이벤트에 필요한 아이템을 얻고, 때에 따라 사용할 수 있게 하여 스토리 또는 플
//레이어의 상태에 영향을 주는(배열, 포인터)

//메인스토리는 10층 탈출이지만 10층은 별로 아무것도 없는 마왕성의 보스룸과 같은 모습으로 그 층에서 내려가 9층부터 시작
//전투 함수를 하나 만들어서, 적 이름, 적의 피(? ), 등등 입력받아서 전투를 하나로 관리하고
//적의 체력은 층이 내려갈수록 높게 랜덤으로
//유니크(? )몬스터 하나 만들어서 rand값으로 도망치게 하고, 만약 살아있으면 몇층에서 특정 이벤트 진행
//적에게 습격당할수도있고, 적을 발견하면 먼저 공격하고 만약 공격X하고 조용히 지나가면 랜덤한 확률로 그 적에게 습격받음
//함정.. ?

//플레이어 정보 구조체
typedef struct Player {
	int HP; //플레이어 체력
	int MAX_HP; //레벨에 따른 최대 체력
	int EXP; //경험치
	int LEVEL; //플레이어 레벨
	int attack; //플레이어 공격력
	int FLOOR; //플레이어 현재 층
	int money; //돈
}player;

//플레이어 아이템 구조체
typedef struct Item {
	int item_num; //아이템 번호
	char name[10]; //아이템 이름 (한국어로 네글자를 넘지 않는다.)
	int count; //아이템 개수
}item;

void game(player*, item*); //게임 진행
void set_information(player*); //플레이어 정보 설정
void set_state(player*); //경험치에 따른 레벨업과 레벨에 따른 공격력 상승

void store(player*, item*); //상인과의 조우
void quiz(player*, item*); //상인의 퀴즈
void down_floor(player*, item*); //층을 내려가는 변수

int player_action(player*, item*, int); //플레이어의 (전투)행동 결정
void battle_enemy(player*, item*, char[], int); //적과의 전투
int use_item(player*, item*, int, float); //아이템 사용

//엔딩 관련
void goodend(); //1층에서 탈출구를 찾았을 때
void badend(); //베드 엔딩