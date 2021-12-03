
#include <iostream>
#include <direct.h>
#include "RailwayTrainRecord.cpp"

using namespace std;

enum UserAction {
    AddRecord,
    ShowAllRecords,
    ShowIncome,
    CloseProgram,
    UknownAction
};
static UserAction AskUserForAction(){
    cout
        <<"[1 - Add record],\n"
        <<"[2 - Show all records],\n"
        <<"[3 - Show income of train]\n"
        <<"[4 - Exit]\n"
        <<"Your command: ";

    string UserAnswer;
    cin>>UserAnswer;

    if(UserAnswer=="1") return UserAction::AddRecord;
    if(UserAnswer=="2") return UserAction::ShowAllRecords;
    if(UserAnswer=="3") return UserAction::ShowIncome;
    if(UserAnswer=="4") return UserAction::CloseProgram;

    return UserAction::UknownAction;
}

int main()
{
    const string FilePathConst = "C:\\testdataRailway\\railwaybase.dat";
    mkdir("C:\\testdataRailway");
    vector<RailwayTrainRecord> AllRecords;

    UserAction action;
    while(action!=UserAction::CloseProgram){
        action=AskUserForAction();

        if(action==UserAction::AddRecord){
            RailwayTrainRecord::CreateAndWrite(FilePathConst);
        }

        else if(action==UserAction::ShowAllRecords){
            for(auto rcd:RailwayTrainRecord::ReadAllFromBinaryFile(FilePathConst)){
                cout<<rcd.GetFullInfoString()<<endl;
            }
        }

        else if(action==UserAction::ShowIncome){
            auto train = RailwayTrainRecord::GetTrainByIdDialog(FilePathConst);
            cout<<train.GetIncomeInfoString()<<endl;
        }

        cout<<endl;
    }
}
