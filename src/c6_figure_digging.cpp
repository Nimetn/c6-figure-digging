#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <windows.h>
#include <limits>

using namespace std;

struct VectorInt {int *Data, Count;};
struct BOT_Points{
    string Name;
    VectorInt Points;
    int SumAver = 0;
};

class BOT_Group{
private:
    BOT_Points *Data;int Count;

public:
    BOT_Group(char *FN){
        ifstream F(FN);
        F>>Count;
        if(Count<3 || Count>255){
            cerr << "Ошибка: колисечтво участников должно быть от 3 и до 255.\n";
            exit(1);
        }
        Data = new BOT_Points[Count];
        for (int i = 0; i<Count; i++){
            F >> Data[i].Name;
            if(Data[i].Name.length()<1 || Data[i].Name.length()>20){
                cerr << "Ошибка: имя участника должно быть от 1 и до 20 символов.\n";
                exit(1);
            }
            Data[i].Points.Count = 6;
            Data[i].Points.Data = new int[Data[i].Points.Count];

            for(int j=0; j<Data[i].Points.Count; j++){
                if(!(F >> Data[i].Points.Data[j])){
                    cerr << "Ошибка: недостаточно оценок у участника(ов).\n";
                    exit(1);
                }
            }
            F.ignore(numeric_limits<streamsize>::max(), '\n');
            int maxind = GetMaxIndex(i);
            int minind = GetMinIndex(i);

            for(int j=0; j<Data[i].Points.Count;j++){
                if(j != maxind && j != minind){
                    Data[i].SumAver+=Data[i].Points.Data[j];
                }
            }
            Data[i].SumAver/=4;
        }
    }
    void DeleteDubll(){
        for(int i = 0; i<Count;i++){
            for(int j =i+1; j<Count;j++){
                if(Data[i].Name==Data[j].Name){
                    delete[] Data[j].Points.Data;
                    Data[j].Points.Data = nullptr;
                    for(int k = j; k < Count - 1; k++){
                        Data[k] = Data[k + 1];
                    }
                    Count--;
                    j--;
                    if(Count<3){
                        cerr << "Ошибка: после удаления осталось меньше 3 участников\n";
                        exit(1);
                    }
                }
            }
        }
    }
    void DeleteNull(){
        int i = 0;
        while(i < Count){
            bool bad = false;
            for(int j = 0; j < 6; j++){
                if(Data[i].Points.Data[j] < 1 || Data[i].Points.Data[j] > 65535){
                    bad = true;
                    break;
                }
            }

            if(bad){
                delete[] Data[i].Points.Data;
                for(int k = i; k < Count - 1; k++){
                    Data[k] = Data[k + 1];
                }
                Count--;
            } else {
                i++;
            }
        }

        if(Count < 3){
            cerr << "Ошибка: после удаления осталось меньше 3 участников\n";
            exit(1);
        }
    }

    ~BOT_Group(){
        for(int i = 0; i < Count; i++){
            delete[] Data[i].Points.Data;
        }
        delete[] Data;
    }
    void Swap(BOT_Points &a, BOT_Points &b){BOT_Points T = a;a=b;b=T;}
    void SortSumAver() {
        int k = 0;
        bool F = true;
        while (F) {
            F = false;
            for(int i=0;i<Count-k-1;i++){if(Data[i+1].SumAver>Data[i].SumAver){F=true;Swap(Data[i+1],Data[i]);}}
            k++;
            if (k >= Count) break;
        }
    }
    int GetMaxIndex(int numb){
        int Im = 0;
        for(int i=0;i<=Data[numb].Points.Count-1;i++)Im=Data[numb].Points.Data[i]>Data[numb].Points.Data[Im]?i:Im;
        return Im;
    }
    int GetMinIndex(int numb){
        int Im = 0;
        for(int i=0;i<=Data[numb].Points.Count-1;i++)Im=Data[numb].Points.Data[i]<Data[numb].Points.Data[Im]?i:Im;
        return Im;
    }
    void Out(){
        for(int i=0; i<Count; i++){
            cout<<Data[i].Name<<"\t";
            for(int j=0;j<Data[i].Points.Count;j++)cout<<Data[i].Points.Data[j]<<"\t";
            cout<<Data[i].SumAver<<endl;
            cout<<endl;
        }
    }
    void Out(char *FName){
        remove(FName);
        ofstream F(FName);
        for(int i = 0; i < 3; i++){
            F << Data[i].Name << endl;
        }
        F.close();
    }

};



int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    BOT_Group *X = new BOT_Group("robot.txt");
    X->DeleteNull();
    X->DeleteDubll();

    X->SortSumAver();

    X->Out("Winners.txt");
    X->Out();
    delete X;
    return 0;
}
