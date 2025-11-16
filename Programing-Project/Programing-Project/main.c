//2025-11-04
//1. 발표 평가(교수, 조교 각 5점씩)
//목표/개요 설명 | 1.5 | 프로젝트가 무엇인지 명확히 설명
//기능소개 | 1.5 | 주요 기능을 체계적으로 설명
//시간 관리 | 0.5 | 5분 내 완료
//실제 동작 및 안정성 | 1.5 | 프로그램 정상 작동, 데모 완성도(발표 때 시연이 실패하는 경우 감정)
//(창의성(추가점수)추가 기능 구현 시 가산점)
//2. 코드 평가(조교10점)
//완성도 | 3 | 코드 설계 및 가독성(명확한 변수명, 주석)
//핵심 기능 구현률 | 7 | 계획서 내 명시된 핵심 기능 구현 여부


#include <stdio.h>
#include "head.h";

int main() {
	srand((unsigned int)time(NULL));
	
	player kylen; //임시로 주인공 이름을 kylen으로 설정
	item inventory[20]; //아이템 갯수 최대 10개

	player* player = &kylen;
	item* item= &inventory;

	player->FLOOR = 10;

	void set_information(player);
}