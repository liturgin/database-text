#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

struct CourseWork
{
	char Autor[50];               // Автор
	char Name[50];                // Название работы
	char Discipline[20];          // Предмет
	char DateOfCompletion[20];    // Дата сдачи
	int Assessment;               // Оценка
};
typedef struct CourseWork CourseWork_type;

CourseWork_type AddCourseWork();// Добавление курсовой работы
void ShowCourseWorks(CourseWork_type* CourseWorks, int countCourseWorks); // Вывод данных о курсовых работах
CourseWork_type* SearchCourseWorks(CourseWork_type* CourseWorks, int countCourseWorks, char* searchName); // Поиск по теме
CourseWork_type* SortByAssessment(CourseWork_type* CourseWorks, int countCourseWorks); // Сортировка по оценкам
void WriteToFile(CourseWork_type* CourseWorks, int countCourseWorks, char* filename); // Запись данных в файл
int SizeFromFile(char* filename); // Вычисление записей в файле
CourseWork_type* ReadFromFile(CourseWork_type* CourseWorks, char* filename);//Чтение из файла

int main()
{
	setlocale(LC_ALL, "RUS");
	system("chcp 1251");

	CourseWork_type* CourseWorks = NULL;
	int countCourseWorks = 0, menu;
	char filename[25];

	printf("Введите название файла: ");
	scanf("%s", &filename);

	do
	{
		printf("1 - Добавить курсовую работу\n"
			"2 - Вывод добавленных курсовых работ\n"
			"3 - Найти курсовую работу по теме\n"
			"4 - Отсортировать курсовые работы по баллам\n"
			"5 - Запись данные о работах в файл\n"
			"6 - Чтение данных о работах из файла\n"
			"7 - Сменить файл\n"
			"0 - Выход\n");
		scanf("%d", &menu);

		switch (menu)
		{
		case 1: //Добавление курсовой работы
		{
			CourseWorks = realloc(CourseWorks, ++countCourseWorks * sizeof(CourseWork_type));
			CourseWorks[countCourseWorks - 1] = AddCourseWork();
			break;
		}
		case 2: // Вывод всех записей
		{
			ShowCourseWorks(CourseWorks, countCourseWorks);
			break;
		}
		case 3: //Поиск по записям
		{
			char searchName[25];

			printf("Введите тему для поиска\n");
			scanf("%s", &searchName);

			ShowCourseWorks(SearchCourseWorks(CourseWorks, countCourseWorks, searchName), countCourseWorks);
			break;
		}
		case 4: // Сортировка по оценкам
		{
			ShowCourseWorks(SortByAssessment(CourseWorks, countCourseWorks), countCourseWorks);
			break;
		}
		case 5: //Запись данных в файл
		{
			WriteToFile(CourseWorks, countCourseWorks, filename);
			break;
		}
		case 6: // Чтение данных их файла
		{
			countCourseWorks = SizeFromFile(filename) - 1;
			CourseWorks = realloc(CourseWorks, sizeof(CourseWork_type) * countCourseWorks);
			CourseWorks = ReadFromFile(CourseWorks, filename);
			ShowCourseWorks(CourseWorks, countCourseWorks);
			break;
		}
		case 7:
		{
			printf("Введите название файла: ");
			scanf("%s", &filename);
			break;
		}
		case 0:
			break;
		default:
			printf("Некорректный выбор\n");
		}
	} while (menu != 0);
}

CourseWork_type AddCourseWork()
{
	CourseWork_type newWork;

	printf("Введите автора работы\n");
	scanf("%s", &newWork.Autor);

	printf("Введите название работы\n");
	scanf("%s", &newWork.Name);

	printf("Введите предмет\n");
	scanf("%s", &newWork.Discipline);

	printf("Введите дату сдачи\n");
	scanf("%s", &newWork.DateOfCompletion);

	printf("Введите оценку\n");
	scanf("%d", &newWork.Assessment);

	return newWork;
}

void ShowCourseWorks(CourseWork_type* CourseWorks, int countCourseWorks)
{
	printf("---------------------------------------------------------------------------------------------------------\n");
	printf("|                Тема                   |        Автор        |       Предмет       |   Дата   | Оценка |\n");
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


