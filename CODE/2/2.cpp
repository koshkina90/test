// Graphics_from_Key_Results_on_Dif_Freq.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DIPLICATE
#include <iostream>
//#include "stdafx.h"
FILE* fdr, * fdw;
double* recEB, * Result[12], dipole;
int NP,XYZ;
double* Freq, ** Result_in_Point;
int  NPoints, * Points;
int  Nfreq,NRes;
const int max_length{ 10 }; // максимальная длина строки (включая нулевой байт \0)
char FileName[][max_length]
{
    "Re_Ex_Key", "Im_Ex_Key",  "Re_Ey_Key", "Im_Ey_Key", "Re_Ez_Key", "Im_Ez_Key",
    "Re_Bx_Key", "Im_Bx_Key",  "Re_By_Key", "Im_By_Key", "Re_Bz_Key", "Im_Bz_Key"
};

int ReadWrite()
{
    int NF = 12;
    fdr = fopen("result_key", "r");
    if (fdr == NULL)
    {
        printf("Need a result_key file.\n\tDipole size;\nNumber of frequencies - Nfreq;\nNfreq frequency values;\nNumber of receiver positions NP;\nNP x,y,z coordinates of receiver positions;\nRe_Ex, Im_Ex, Re_Ey, Im_Ey, Re_Ez, Im_Ez, Re_Bx, Im_Bx, Re_By, Im_By, Re_Bz, Im_Bz values ​​for all positions for each frequency\n");
        system("pause");
        return 0;
    }
    fscanf(fdr, "%lf", &dipole);
    fscanf(fdr, "%d", &Nfreq);
    Freq = new double[Nfreq];
    for (int i = 0; i < Nfreq; i++)
        fscanf(fdr, "%lf", &Freq[i]);
    fscanf(fdr, "%d", &NP);
    recEB = new double[NP];
    NRes = Nfreq * NP;
    for (int i = 0; i < NF; i++)
        Result[i] = new double[NRes];

    printf("To display the result for E and B along Ox, press 1\n To display the result along Oy, press 2\n To display the result along Oz, press 3\n");
    scanf("%d", &XYZ);

    for (int i = 0; i < NP; i++)
    {
        double tmp[3];
        fscanf(fdr, "%lf%lf%lf", &tmp[0], &tmp[1], &tmp[2]);
        recEB[i] = tmp[XYZ - 1];
    }
   
    for (int i = 0; i < NRes; i++)
    {
        for (int j = 0; j < NF; j++)
            fscanf(fdr, "%lf", &Result[j][i]);

    }
    fclose(fdr);
    fdr = fopen("points", "r");

    if (fdr == NULL)
    {
        printf("Need a points file.\n\tFile format: NPoints - the number of receiver positions in which the frequency result should be output, \n\t\t then NPoints of the numbers of these positions (Numbering from 1)\n");
        system("pause");
        return 0;
    }
    fscanf(fdr, "%d", &NPoints);
    Points = new int[NPoints];
    Result_in_Point = new double* [NPoints];
    for (int i = 0; i < NPoints; i++)
        Result_in_Point[i] = new double[Nfreq];
    for (int i = 0; i < NPoints; i++)
    {
        int tmp;
        fscanf(fdr, "%d", &tmp);
        Points[i] = tmp - 1;
    }
    fclose(fdr);

    system("md Key_Results");
    for (int k = 0; k < Nfreq; k++)
    {
        for (int i = 0; i < NF; i++)
        {
            char tmps[64];
            sprintf(tmps, "Key_Results\\%s_nu%d", FileName[i], k+1);
            fdw = fopen(tmps, "w");
            fprintf(fdw, "\n\n");
            for (int j = 0; j < NP; j++)
            {
                int kj = k * NP + j;
                fprintf(fdw, "%lf\t%e\n", recEB[j], Result[i][kj] * dipole);
            }
            fclose(fdw);
        }
    }
}
int ResultsInPoints()
{
    int NF = 12;
    char tmpPath2[64], tmpPath[64];
    if (NPoints > 0)
    {
       
            sprintf(tmpPath2, "Key_Results");
            sprintf(tmpPath, " md Key_Results\\Results_Key_in_points");
            system(tmpPath);
          
        
        for (int i = 0; i < NF; i++)
        {
            for (int j = 0; j < Nfreq; j++)
            {
                char resfile[120];
                sprintf(resfile, "%s\\%s_nu%d", tmpPath2, FileName[i], j + 1);
                fdr = fopen(resfile, "r");
                if (fdr == NULL)
                {
                    printf("Need a %s file.\n", resfile);
                    system("pause");
                    return 0;
                }
                double tmp[2];
                int k = 0, ip = 0;
                while (fscanf(fdr, "%lf%lf", &tmp[0], &tmp[1]) && ip < NPoints)
                {
                    // fscanf(fdr, "%lf%lf", &tmp[0], &tmp[1]);
                    if (k == Points[ip])
                    {
                        Result_in_Point[ip][j] = tmp[1];
                        ip++;
                    }
                    k++;
                }
                fclose(fdr);
            }
            for (int ip = 0; ip < NPoints; ip++)
            {
                char wresfile[120];

                
                sprintf(wresfile, "Key_Results\\Results_Key_in_points\\%s_P%d", FileName[i], Points[ip] + 1);
               
              

                fdw = fopen(wresfile, "w");
                fprintf(fdw, "\n\n");
                for (int j = 0; j < Nfreq; j++)
                {
                    fprintf(fdw, "%lf\t%e\n", Freq[j], Result_in_Point[ip][j]);
                }

                fclose(fdw);
            }

        }
    }
}


int main()
{
    ReadWrite();
    ResultsInPoints();
}


// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
