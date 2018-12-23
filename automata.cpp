// Реализация конечного автомата

#include "Calc.h"

int Stack::StackCalc(double Var, char Op, char Op2) // Добавление операнда и оператора в стек
// Возвращается коррекция номера позиции (при операции из двух символов коррекция +1
{
    Operands.push_back(Var);
    int correct=0;
    switch(Op)
    {
        case '>': if(Op2=='=') Op='^'; break; // Операция >=
        case '<': if(Op2=='=') Op='v'; break; // Операция <=
        case '!': if(Op2!='=') {Error=true; return 0;} break; // Операция !=
        case '=': if(Op2!='=') {Error=true; return 0;} break; // Операция !=
    }
    int t=Priority;
    if(Op!=0) // Пустая операции (просто добавление операнда в стек операндов
        {
        switch(Op) // Опред
            {
            case   0: t=0; break;
            case '+':
            case '-': t+=1; break;
            case '*':
            case '/': t+=2; break;
            case '>':
            case '<':
            case '=':
            case '!':
            case '^':
            case 'v': t+=3; break;

            default: cout<<"Syntax error"; Error=true;
            }
        }
    while(t<=Priorities.back())
    {
        double var1=Operands.back(); Operands.pop_back();
        double var2=Operands.back(); Operands.pop_back();
        switch(Operators.back()) // Выполнение операции
        {
            case '+': Operands.push_back(var1+var2); break;
            case '-': Operands.push_back(var2-var1); break;
            case '*': Operands.push_back(var1*var2); break;
            case '/': Operands.push_back(var2/var1); break;
            case '>': Operands.push_back(var2>var1); break;
            case '^': Operands.push_back(var2>=var1); break;
            case '<': Operands.push_back(var2<var1);  break;
            case 'v': Operands.push_back(var2<=var1); break;
            case '=': Operands.push_back(var2==var1); break;
            case '!': Operands.push_back(var2!=var1); break;
        }
        Priorities.pop_back();
        Operators.pop_back();
    }
    if(Op!=0)
        {Operators.push_back(Op); Priorities.push_back(t);}
    switch(Op)
    {
        case '^':
        case 'v':
        case '!':
        case '=': return 1;
    }
    return 0;
}

double Stack::Expr(string expr) // Вычисляет результат арифметиеского выражения и возврящает его
// Конечный автомат для разбора рифметического выражения
{
    Error=false;
    string VarBuf, FigureBuf;
    Priorities.clear();
    Priorities.push_back(-1); // Символ конца стека
    S=0; // Перейти в начальное состояние автомата
    Priority=0;
    VarBuf=""; // Буфер для накопления символов идентификатора
    FinVal=""; // переменная для результата

    for(int i=0;i<=expr.length() && S>=0 && !Error;i++)
        {
            while(expr[i]==' ')i++;
            switch(S)
        {
            case 0:
                if(expr[i]>='0' && expr[i]<='9' || expr[i]=='-')
                {S=4;  FigureBuf=expr[i];}
                else if(expr[i]>='a' && expr[i]<='z' || expr[i]>='A' && expr[i]<='Z')
                {S=1; VarBuf=expr[i];}
                else if(expr[i]=='(')
                    {S=2; Priority+=10;}
                else
                    {cout<<"Expression error\n"; return false;}
                break;
            case 1: // Распознание первого идентификатора (переменной, которой надо присваивать значение)
                if(expr[i]>='a' && expr[i]<='z' || expr[i]>='A' && expr[i]<='Z')
                    VarBuf+=expr[i];
                else
                    if(expr[i]==0)
                    {
                    // Вывод переменной на экран
                    if(VarTable.count(VarBuf)==0)
                        {cout<<"Unnoun variable\n"; return false;}
                    else
                        {cout<<VarTable[VarBuf]<<endl; return VarTable[VarBuf];}
                    }
                else
                    if(expr[i]=='+' || expr[i]=='-' || expr[i]=='*' || expr[i]=='/' || expr[i]=='<' || expr[i]=='>' || expr[i]=='!' || expr[i]=='=')
                    {

                    if(expr[i]=='=' && expr[i+1]!='=')
                        {FinVal=VarBuf; if(!VarTable.count(FinVal)) VarTable[FinVal]=0;}
                    else
                        if(VarTable.count(VarBuf)==0)
                            {cout<<"Unnoun variable\n"; return false;}
                        else
                            i+=StackCalc(VarTable[VarBuf],expr[i], expr[i+1]);

                    VarBuf="";
                    S=2;
                    }
                break;
            case 2:
                if(expr[i]>='a' && expr[i]<='z' || expr[i]>='A' && expr[i]<='Z')
                    {S=3; VarBuf=expr[i];}
                else if(expr[i]>='0' && expr[i]<='9' || expr[i]=='-')
                    {S=4; FigureBuf=expr[i];}
                else if(expr[i]=='(')
                    Priority+=10;
                else
                    {cout<<"Expression error 2\n"; return false;}
                //S=-1;
                break;
            case 3: // Распознание идентификатора
                if(expr[i]>='A' && expr[i]<='Z' || expr[i]>='a' && expr[i]<='z' || expr[i]>='0' && expr[i]<='9')
                    VarBuf+=expr[i];
                else
                   if(expr[i]=='+' || expr[i]=='-' || expr[i]=='*' || expr[i]=='/' || expr[i]=='<' || expr[i]=='>' || expr[i]=='!' || expr[i]=='=' || expr[i]==0)
                        {
                        if(VarTable.count(VarBuf)==0) {cout<<"Unnoun variable\n"; return false;}
                        i+=StackCalc(VarTable[VarBuf],expr[i], expr[i+1]);
                        VarBuf="";
                        S=2;
                        }
                   else
                        if(expr[i]==')')
                            {if(VarTable.count(VarBuf)==0) {cout<<"Unnoun variable\n"; return false;}
                            S=7; FigureBuf=to_string(VarTable[VarBuf]); Priority-=10; VarBuf="";}
                        else
                            {cout<<"Expression error 3\n"; return false;}
                break;
            case 4: // распознание константы
                if(expr[i]>='0' && expr[i]<='9')
                    FigureBuf+=expr[i];
                else
                    if(expr[i]=='.')
                        {FigureBuf+=expr[i]; S=5;}
                    else
                        if(expr[i]=='+' || expr[i]=='-' || expr[i]=='*' || expr[i]=='/' || expr[i]=='<' || expr[i]=='>' || expr[i]=='!' || expr[i]=='=' ||  expr[i]==0)
                        {
                            if(expr[i]==0 && Priority!=0)
                            {
                                cout<<"Bracket error\n";
                                return false;
                            }
                            i+=StackCalc(atof(FigureBuf.c_str()),expr[i], expr[i+1]);
                            S=2;
                        }
                    else if(expr[i]==')')
                            {S=7;  Priority-=10;}
                        else
                            S=-1;
                break;
            case 5:
                if(expr[i]>='0' && expr[i]<='9')
                    {FigureBuf+=expr[i]; S=6;}
                else
                    S=-1;
                break;
            case 6: // Распознения числа после десятичной точки
                if(expr[i]>='0' && expr[i]<='9')
                    FigureBuf+=expr[i];
                else
                    if(expr[i]=='+' || expr[i]=='-' || expr[i]=='*' || expr[i]=='/' || expr[i]=='<' || expr[i]=='>' || expr[i]=='!' || expr[i]=='=' ||  expr[i]==0)
                    {
                        i+=StackCalc(atof(FigureBuf.c_str()),expr[i], expr[i+1]);
                        S=2;
                    }
                else
                    S=-1;
                break;
            case 7: // Распознание закрывающихся скобок
                if(Priority<0)
                      {cout<<"Bracket error\n"; return false;}
                if(expr[i]==')')
                {
                    Priority-=10;
                    if(Priority<0)
                      {
                          cout<<"Bracket error\n";
                          return false;
                      }
                }
                else if(expr[i]=='+' || expr[i]=='-' || expr[i]=='*' || expr[i]=='/' || expr[i]=='<' || expr[i]=='>' || expr[i]=='!' || expr[i]=='=' || expr[i]==0)
                    {i+=StackCalc(atof(FigureBuf.c_str()),expr[i], expr[i+1]);S=2;}
                else
                    {
                    cout<<"Expression error\n";
                    return false;
                    }
                break;
        }
    }
    if(S==-1) // Состояние ошибки
        {cout<<"Expression error!!!\n"; return false;}
    if(Priority!=0) // Ошибка в расстановках скобок (не все скобки закрыты
        {cout<<"Bracket error\n"; return false;}
    else if(FinVal=="") // Если на задана переменная для результата, то вывод на экран
        {if(Operands.size()!=0 && coutF) cout<<Operands.back()<<endl;}
    else
      {VarTable[FinVal]=Operands.back();FinVal="";}
    return Operands.back();
    while(Operands.size()!=0) Operands.pop_back(); // Очистка буфер операндов
    return true;
}
