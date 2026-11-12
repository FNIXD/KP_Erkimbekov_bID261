/*
 * Курсовой проект. Подзадача 1.
 * Табулирование функций Y(x) и V(x) (вариант 8)
 *
 * Y(x) = e^x + 5 + cos(0.001x)
 *
 * V(x) = 2cos^3(x^2) + sin^2(x^3) - 0.08*phi(x), где
 *          | x^3 + 0.2,  x < 1
 * phi(x) = |
 *          | x + x,      x >= 1
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <locale.h>

#define MAX_X_Y      700.0   /* при x > 709 число e^x не помещается в double */
#define MAX_X_V      1000.0  /* при больших |x| sin и cos от x^2, x^3 теряют точность */
#define MAX_POINTS   200     /* максимальное число строк таблицы */
#define GRAPH_WIDTH  60      /* ширина графика в символах */

double phi(double x);
double func_y(double x);
double func_v(double x);
int is_valid(int choice, double x);
double func_value(int choice, double x);
void print_table(int choice, double xmin, double dx, int n);
void print_graph(int choice, double xmin, double dx, int n);

int main(void)
{
    int choice;
    int action;
    double xmin, xmax, dx;
    int n;

    setlocale(LC_ALL, ".UTF8");

    do
    {
        printf("\n=== Табулирование функции ===\n");
        printf("1 - Y(x) = e^x + 5 + cos(0.001x)\n");
        printf("2 - V(x) = 2cos^3(x^2) + sin^2(x^3) - 0.08*phi(x)\n");
        printf("0 - выход\n");
        printf("Ваш выбор: ");
        if (scanf("%d", &choice) != 1)
        {
            printf("Ошибка: нужно ввести число\n");
            return 1;
        }

        if (choice == 1 || choice == 2)
        {
            printf("Введите начало и конец промежутка (xmin xmax): ");
            if (scanf("%lf %lf", &xmin, &xmax) != 2)
            {
                printf("Ошибка: нужно ввести два числа\n");
                return 1;
            }
            printf("Введите шаг табуляции dx: ");
            if (scanf("%lf", &dx) != 1)
            {
                printf("Ошибка: нужно ввести число\n");
                return 1;
            }

            if (xmin >= xmax)
                printf("Ошибка: xmin должен быть меньше xmax\n");
            else if (dx <= 0)
                printf("Ошибка: шаг должен быть больше нуля\n");
            else if ((xmax - xmin) / dx >= MAX_POINTS)
                printf("Слишком много точек (больше %d). Увеличьте шаг\n", MAX_POINTS);
            else
            {
                n = (int)((xmax - xmin) / dx + 1e-9) + 1;

                printf("1 - таблица значений\n");
                printf("2 - график\n");
                printf("Ваш выбор: ");
                if (scanf("%d", &action) != 1)
                {
                    printf("Ошибка: нужно ввести число\n");
                    return 1;
                }

                switch (action)
                {
                case 1:
                    print_table(choice, xmin, dx, n);
                    break;
                case 2:
                    print_graph(choice, xmin, dx, n);
                    break;
                default:
                    printf("Нет такого пункта меню\n");
                }
            }
        }
        else if (choice != 0)
            printf("Нет такого пункта меню\n");
    } while (choice != 0);

    printf("Работа программы завершена.\n");

    return 0;
}

double phi(double x)
{
    double result;

    if (x < 1)
        result = x * x * x + 0.2;
    else
        result = x + x;

    return result;
}

double func_y(double x)
{
    double result;

    result = exp(x) + 5 + cos(0.001 * x);

    return result;
}

double func_v(double x)
{
    double c, s;
    double result;

    c = cos(x * x);
    s = sin(x * x * x);
    result = 2 * c * c * c + s * s - 0.08 * phi(x);

    return result;
}

int is_valid(int choice, double x)
{
    int ok;

    if (choice == 1)
        ok = (x <= MAX_X_Y);
    else
        ok = (fabs(x) <= MAX_X_V);

    return ok;
}

double func_value(int choice, double x)
{
    double result;

    if (choice == 1)
        result = func_y(x);
    else
        result = func_v(x);

    return result;
}

void print_table(int choice, double xmin, double dx, int n)
{
    int i;
    double x, y;

    printf("\nТаблица значений функции %s\n", (choice == 1) ? "Y(x)" : "V(x)");
    printf("+------------+-----------------+\n");
    printf("|      x     |      %s       |\n", (choice == 1) ? "Y(x)" : "V(x)");
    printf("+------------+-----------------+\n");
    for (i = 0; i < n; i++)
    {
        x = xmin + i * dx;
        if (is_valid(choice, x))
        {
            y = func_value(choice, x);
            printf("| %10.4f | %15.6g |\n", x, y);
        }
        else
            printf("| %10.4f | %15s |\n", x, "---");
    }
    printf("+------------+-----------------+\n");
    printf("--- : x вне области допустимых значений\n");
}

void print_graph(int choice, double xmin, double dx, int n)
{
    int i, col;
    int found = 0;
    int point_col, axis_col;
    double x, y, ymin, ymax;

    ymin = 0;
    ymax = 0;
    for (i = 0; i < n; i++)
    {
        x = xmin + i * dx;
        if (is_valid(choice, x))
        {
            y = func_value(choice, x);
            if (found == 0 || y < ymin)
                ymin = y;
            if (found == 0 || y > ymax)
                ymax = y;
            found = 1;
        }
    }

    if (found == 0)
    {
        printf("Функцию нельзя вычислить ни в одной точке промежутка\n");
        return;
    }

    axis_col = -1;
    if (ymin <= 0 && ymax >= 0 && ymax > ymin)
        axis_col = (int)((0 - ymin) / (ymax - ymin) * (GRAPH_WIDTH - 1) + 0.5);

    printf("\nГрафик функции %s (ось x - вниз, ось y - вправо)\n", (choice == 1) ? "Y(x)" : "V(x)");
    printf("y от %g до %g, '|' - линия y = 0\n\n", ymin, ymax);

    for (i = 0; i < n; i++)
    {
        x = xmin + i * dx;

        point_col = -1;
        if (is_valid(choice, x))
        {
            y = func_value(choice, x);
            if (ymax > ymin)
                point_col = (int)((y - ymin) / (ymax - ymin) * (GRAPH_WIDTH - 1) + 0.5);
            else
                point_col = GRAPH_WIDTH / 2;
        }

        printf("%10.4f :", x);
        for (col = 0; col < GRAPH_WIDTH; col++)
        {
            if (col == point_col)
                putchar('*');
            else if (col == axis_col)
                putchar('|');
            else
                putchar(' ');
        }
        printf("\n");
    }
}