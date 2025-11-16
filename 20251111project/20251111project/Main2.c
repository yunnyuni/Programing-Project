#include<stdio.h>

int scan();
void grade(int);

int main() {
	int count = 0;
	int grade = 0;
	int num = 0;
	int sum = 0;
	float avg = 0;
	int g5 = 0, g56 = 0, g67 = 0, g78 = 0, g89 = 0, g910 = 0, g10 = 0;
	printf("###성적 처리\n\n");
	printf("*** 성적 처리 프로그램 ***\n이 프로그램은 시험 점수를 입력받아서\n합, 평균을 구하고\n히스토그램을 그려줍니다.");

	while (1) {
		printf("시험점수를 입력해 주세요.(-1입력시 종료): ");
		if (scanf("%d", &grade) == -1) {
			break;
		}
		else if (grade >= 50 && grade < 60) {
			g56++;
		}
		else if (grade >= 60 && grade < 70) {
			g67++;
		}
		else if (grade >= 70 && grade < 80) {
			g78++;
		}
		else if (grade >= 80 && grade < 90) {
			g89++;
		}
		else if (grade >= 90 && grade < 100) {
			g910++;
		}
		else if (grade >= 100) {
			g10++;
		}
		else {
			g5++;
		}
		count++;
		sum += grade;
		printf("%d점을 추가했습니다.\n", grade);
		printf("지금까지 %d명의 점수를 입력받았습니다.\n", count);
	}
	avg = (float)sum / (float)count;

	return 0;
}