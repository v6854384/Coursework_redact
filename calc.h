#ifndef CALC_H_INCLUDED
#define CALC_H_INCLUDED

#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <vector>

using namespace std;

class Stack
{
private:
    bool down(vector<string> Buf, int &i); // Спуск по списку команд до end или else
    vector<char> Operators;
    vector<int> Priorities;
    vector<double> Operands;
    map<string, double> VarTable; // Таблица переменных
    vector<string> ProgBuf; // Буфер строк с вложенной программой
    string FinVal; // Имя переменной, куда записывается результат
    bool coutF=true; // Флаг разрешения вывода финального значения (чтобы не выподить, когда вычисляется логическое выражение в условном операцторе и операцторах цикла)
    int ProgBracked=0; // счетчик программных скобок во время буферизации
    int Priority=0; // Приоритет операции
    int StackCalc(double Var, char Op, char Op2); // Вычислить значение из стека операндов и операций

    bool ProgExecute(vector<string> Buf); // Вополнение программы, хранящейся в буфере (буфер хранит программму с циклами и условными операцторами
    double Expr(string expr); // Вычисление значения одного выражения (либо операцтора присваивания, либо логического выражения в операторе ветвления или циела
    vector<string> ForToWhile(vector<string> Buf);
    int S; // Номер состояния автомата
    bool Error=false; // Флаг синтаксической ошибки
public:
    bool Calc(string str); // Буферизация программы и ее последуюцее выполнение
};

#endif // CALC_H_INCLUDED
