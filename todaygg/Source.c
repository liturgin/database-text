#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

struct CourseWork
{
	char Autor[50];               // �����
	char Name[50];                // �������� ������
	char Discipline[20];          // �������
	char DateOfCompletion[20];    // ���� �����
	int Assessment;               // ������
};
typedef struct CourseWork CourseWork_type;

CourseWork_type AddCourseWork();// ���������� �������� ������
void ShowCourseWorks(CourseWork_type* CourseWorks, int countCourseWorks); // ����� ������ � �������� �������
CourseWork_type* SearchCourseWorks(CourseWork_type* CourseWorks, int countCourseWorks, char* searchName); // ����� �� ����
CourseWork_type* SortByAssessment(CourseWork_type* CourseWorks, int countCourseWorks); // ���������� �� �������
void WriteToFile(CourseWork_type* CourseWorks, int countCourseWorks, char* filename); // ������ ������ � ����
int SizeFromFile(char* filename); // ���������� ������� � �����
CourseWork_type* ReadFromFile(CourseWork_type* CourseWorks, char* filename);//������ �� �����

int main()
{
	setlocale(LC_ALL, "RUS");
	system("chcp 1251");

	CourseWork_type* CourseWorks = NULL;
	int countCourseWorks = 0, menu;
	char filename[25];

	printf("������� �������� �����: ");
	scanf("%s", &filename);

	do
	{
		printf("1 - �������� �������� ������\n"
			"2 - ����� ����������� �������� �����\n"
			"3 - ����� �������� ������ �� ����\n"
			"4 - ������������� �������� ������ �� ������\n"
			"5 - ������ ������ � ������� � ����\n"
			"6 - ������ ������ � ������� �� �����\n"
			"7 - ������� ����\n"
			"0 - �����\n");
		scanf("%d", &menu);

		switch (menu)
		{
		case 1: //���������� �������� ������
		{
			CourseWorks = realloc(CourseWorks, ++countCourseWorks * sizeof(CourseWork_type));
			CourseWorks[countCourseWorks - 1] = AddCourseWork();
			break;
		}
		case 2: // ����� ���� �������
		{
			ShowCourseWorks(CourseWorks, countCourseWorks);
			break;
		}
		case 3: //����� �� �������
		{
			char searchName[25];

			printf("������� ���� ��� ������\n");
			scanf("%s", &searchName);

			ShowCourseWorks(SearchCourseWorks(CourseWorks, countCourseWorks, searchName), countCourseWorks);
			break;
		}
		case 4: // ���������� �� �������
		{
			ShowCourseWorks(SortByAssessment(CourseWorks, countCourseWorks), countCourseWorks);
			break;
		}
		case 5: //������ ������ � ����
		{
			WriteToFile(CourseWorks, countCourseWorks, filename);
			break;
		}
		case 6: // ������ ������ �� �����
		{
			countCourseWorks = SizeFromFile(filename) - 1;
			CourseWorks = realloc(CourseWorks, sizeof(CourseWork_type) * countCourseWorks);
			CourseWorks = ReadFromFile(CourseWorks, filename);
			ShowCourseWorks(CourseWorks, countCourseWorks);
			break;
		}
		case 7:
		{
			printf("������� �������� �����: ");
			scanf("%s", &filename);
			break;
		}
		case 0:
			break;
		default:
			printf("������������ �����\n");
		}
	} while (menu != 0);
}

CourseWork_type AddCourseWork()
{
	CourseWork_type newWork;

	printf("������� ������ ������\n");
	scanf("%s", &newWork.Autor);

	printf("������� �������� ������\n");
	scanf("%s", &newWork.Name);

	printf("������� �������\n");
	scanf("%s", &newWork.Discipline);

	printf("������� ���� �����\n");
	scanf("%s", &newWork.DateOfCompletion);

	printf("������� ������\n");
	scanf("%d", &newWork.Assessment);

	return newWork;
}

void ShowCourseWorks(CourseWork_type* CourseWorks, int countCourseWorks)
{
	printf("---------------------------------------------------------------------------------------------------------\n");
	printf("|                ����                   |        �����        |       �������       |   ����   | ������ |\n");
	printf("---------------------------------------------------------------------------------------------------------\n");

	for (int i = 0; i < countCourseWorks; i++)
	{
		if (strcmp(CourseWorks[i].Name, "-") == 0) break;
		printf("|%39s|%21s|%21s|%10s|%8d|\n", CourseWorks[i].Name, CourseWorks[i].Autor, CourseWorks[i].Discipline, CourseWorks[i].DateOfCompletion, CourseWorks[i].Assessment);
		printf("---------------------------------------------------------------------------------------------------------\n");
	}
}

CourseWork_type* SearchCourseWorks(CourseWork_type* CourseWorks, int countCourseWorks, char* searchName)
{
	CourseWork_type* foundedWorks = malloc(sizeof(CourseWork_type));
	int countFoundWorks = 0;

	for (int i = 0; i < countCourseWorks; i++)
	{
		if (strcmp(CourseWorks[i].Name, searchName) == 0)
		{
			foundedWorks = realloc(foundedWorks, sizeof(CourseWork_type) * (countFoundWorks + 2));
			foundedWorks[countFoundWorks++] = CourseWorks[i];
		}
	}
	strcpy(foundedWorks[countFoundWorks].Name, "-");

	return foundedWorks;
}

CourseWork_type* SortByAssessment(CourseWork_type* CourseWorks, int countCourseWorks)
{
	CourseWork_type temp;

	for (int i = 0; i < countCourseWorks - 1; i++)
		for (int j = 0; j < countCourseWorks - i - 1; j++)
		{
			if (CourseWorks[j].Assessment > CourseWorks[j + 1].Assessment)
			{
				temp = CourseWorks[j];
				CourseWorks[j] = CourseWorks[j + 1];
				CourseWorks[j + 1] = temp;
			}
		}

	return CourseWorks;
}

void WriteToFile(CourseWork_type* CourseWorks, int countCourseWorks, char* filename)
{
	FILE* fileWriter = fopen(filename, "w");

	for (int i = 0; i < countCourseWorks; i++)
	{
		fprintf(fileWriter, "%s %s %s %s %d\n", CourseWorks[i].Name, CourseWorks[i].Autor, CourseWorks[i].Discipline, CourseWorks[i].DateOfCompletion, CourseWorks[i].Assessment);
	}

	fclose(fileWriter);
}

int SizeFromFile(char* filename)
{
	FILE* fileReader = fopen(filename, "r");
	char buffer;
	int countWorks = 0;

	while (!feof(fileReader))
	{
		fscanf(fileReader, "%c", &buffer);
		if (buffer == '\n')
			countWorks++;
	}

	return countWorks;
}

CourseWork_type* ReadFromFile(CourseWork_type* CourseWorks, char* filename)
{
	FILE* fileReader = fopen(filename, "r");
	int countWorks = SizeFromFile(filename);

	for (int i = 0; i < countWorks; i++)
	{
		fscanf(fileReader, "%s %s %s %s %d\n", &CourseWorks[i].Name, &CourseWorks[i].Autor, &CourseWorks[i].Discipline, &CourseWorks[i].DateOfCompletion, &CourseWorks[i].Assessment);
	}
	fclose(fileReader);

	return CourseWorks;
}


