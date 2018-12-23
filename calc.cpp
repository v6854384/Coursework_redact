// Реализация разбора программных конструкций (Состоят из нескольких строк)
#include "Calc.h"

bool Stack::down(vector<string> Buf, int &i) // Спуск по списку команд до end или else (соответствующие приоритету программной скобки)
// Возвращается номер строки, где была найдена соотетствующая программная скобка
{
    int prior=0; // приоритет (уровень программных скобок
    for(; i<Buf.size() && prior>=0;i++)
        if(Buf[i].find("while")==0 || Buf[i].find("for")==0 || Buf[i].find("if")==0)
            prior++; // Открытие программной скобки
        else
            if(Buf[i].find("else")==0 && prior==0)
                return true;
            else
                if(Buf[i].find("end")==0)
                    if(prior==0)
                        return true;
                    else
                    {
                        prior--; // Закрытие программной скобки
                        if(prior<0) {Error=true; return false;}
                    }
    Error=true; return false;
}

vector<string> Stack::ForToWhile(vector<string> Buf) // Преобразование for в while
{
    for(int i=0;i<Buf.size();i++) // Преобразование for в while
        if(Buf[i].find("for ")==0)
            {
                string str=Buf[i].substr(4,1000);
                if(str.find("=")==string::npos) {Error=true; cout<<"No '='\n"; Buf.clear(); return Buf;}
                string var=str.substr(0,str.find("="));
                if(var.length()==0) {Error=true; cout<<"No for var\n"; Buf.clear(); return Buf;}
                str=str.substr(str.find("=")+1,1000);
                if(str.find(":")==string::npos) {Error=true; cout<<"No ':'\n"; Buf.clear(); return Buf;}
                Buf[i++]=var +"="+ str.substr(0,str.find(":"));
                str=str.substr(str.find(":")+1,1000);
                string step="1";
                if(count(str.begin(),str.end(),':')==1)
                {
                    step=str.substr(0,str.find(":"));
                    str=str.substr(str.find(":")+1,1000);
                }
                Buf.insert(Buf.begin()+i, "while "+ var +"<="+ str);
                int j=i+1;
                if(!down(Buf,j)) {cout<<"There is not 'end' !!!\n"; Error=true; Buf.clear(); return Buf;}
                if(Buf[j].find("end")==string::npos)
                {
                    cout<<"There is not 'end'\n";
                    Error=true;
                    Buf.clear(); return Buf;
                }
                Buf.insert(Buf.begin()+j, var + "=" + var + "+" + step);
            }
    return Buf;
}

bool Stack::ProgExecute(vector<string> Buf) // Выполнение программы из буфера
{
    int i=0;
    Buf=ForToWhile(Buf); // Преобразование циклов for в цикл while
    vector<int> LineBack;// Стек номеров строк для возвращения по завершению программнго блока
    for(i=0;i<Buf.size();)
        if(Buf[i].find("while")==0)
        {
            {
            if(Buf[i].length()==5) {Error=true; return false;}
            coutF=false;
            if(Expr(Buf[i].substr(5,1000)))
                {LineBack.push_back(i++);/*cout<<"iteratin "<<endl*/}
            else
                {
                if(!down(Buf,++i)) {Error=true; return false;}
                else i++;
                }
            coutF=true;
            }
        }
        else if(Buf[i].find("if")==0)
        {   coutF=false;
            if(Expr(Buf[i].substr(2,1000)))
                {LineBack.push_back(-1); i++;} // флаг завершения блока до else
            else
                {
                i++;
                if(!down(Buf,i)) return false;
                if(Buf[i].find("else")==0)
                    {
                        LineBack.push_back(-1); // флаг завершения блока после else
                        i++;
                    }
                }
            LineBack.push_back(-1);
            coutF=true;
            // Пропустить до end или до else если ложь
        }
        else if(Buf[i].find("end")==0)
        {
            if(LineBack.size()==0) {Error=true; cout<<"End without if, while, for"; return false;}
            if(LineBack.back()>=0)
                {i=LineBack.back(); LineBack.pop_back(); /*cout<<"back to "<<i<<endl;*/}
            else
                if(LineBack.back()<0){LineBack.pop_back();i++;}
        }
        else if(Buf[i].find("else")==0)
        {
            if(LineBack.back()<0)
                {
                    if(!down(Buf,i) || Buf[i].find("end")!=0) return false;
                    i++;
                }
        }
        else {Expr(Buf[i]);i++;}
}

bool Stack::Calc(string str) // Буферизация программы (формирование списка команд)
{
    int i=0;
    while(str[i]==' ')i++;
    str=str.substr(i,10000); // Удаляем пробелы в начале строки
    if(str.find("for")==0 || str.find("while")==0 || str.find("if")==0)
        {
            ProgBuf.push_back(str);
            ProgBracked++;
        }
    else if(str.find("end")==0)
    {
        if(ProgBracked==0) return false;
        ProgBuf.push_back(str);
        ProgBracked--;
        if(ProgBracked==0) // Вывполнение программного блока
        {
            ProgExecute(ProgBuf); // Выполенние программы из списка
            ProgBuf.clear();
            return true;
        }
    }
        else if(ProgBracked==0)
                Expr(str); // Если одна строка, то выполняется
            else
                ProgBuf.push_back(str); // Поместить строку в программынй буфер
    return true;
}
