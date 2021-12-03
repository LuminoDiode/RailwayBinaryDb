#include <iostream>
#include <regex>
#include <iomanip>
#include <fstream>

using namespace std;

class RailwayTrainRecord{

private:
    static bool IsNumber(string &str) {return regex_match( str, regex(("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?")));}
    static size_t GetSizeOfTrainId(){return sizeof(RailwayTrainRecord::TrainId);}
    static size_t GetSizeOfDestination(){return sizeof(RailwayTrainRecord::DepartureDestinationPointString);}
    static size_t GetSizeOfPrice(){return sizeof(RailwayTrainRecord::TicketPrice);}
    static size_t GetSizeOfAmount(){return sizeof(RailwayTrainRecord::TicketsSold);}
    static size_t GetSizeOfInstance(){return (GetSizeOfTrainId()+GetSizeOfDestination()+GetSizeOfPrice()+GetSizeOfAmount());}

public:
    int TrainId;
    char DepartureDestinationPointString[30];
    int TicketsSold;
    float TicketPrice;

    static RailwayTrainRecord GetTrainById(int id, string FilePathToSearch){
        for(auto &rcd:ReadAllFromBinaryFile(FilePathToSearch))
            if(rcd.TrainId==id) return rcd;
    }

    static bool TrainWithIdExists(int id, string FilePathToSearch){
        for(auto rcd:ReadAllFromBinaryFile(FilePathToSearch))
            if(rcd.TrainId==id) return true;

        return false;
    }

    static int GetExistingIdDialog(string FilePathToSearch){
        string TempString; int GotId;
        while(true){
            cout<<"Train Id: "; cin>>TempString;
            if(!IsNumber(TempString)) {cout<<"Value does not correspond format requirements"<<"\n"; continue;}
            GotId=stoi(TempString);
            if(!TrainWithIdExists(GotId,FilePathToSearch)) cout<<"Train with id "<<GotId<<" does not exist"<<endl;
            else return GotId;
        }
    }

    static RailwayTrainRecord GetTrainByIdDialog(string FilePathToSearch){
        return RailwayTrainRecord::GetTrainById(GetExistingIdDialog(FilePathToSearch), FilePathToSearch);
    }

    static RailwayTrainRecord CreateUserDialog(){
        auto Out = RailwayTrainRecord();
        string TempString;

        while(true){
            cout<<"Train Id: ";
            cin>>TempString;
            if(!IsNumber(TempString)) cout<<"Value does not correspond format requirements"<<"\n";
            else{Out.TrainId=stoi(TempString); break;}
        }
        while(true){
            cout<<"Train department and destination points as single string: ";
            cin>>TempString;
            if((TempString.size())>30) cout<<"Value does not correspond format requirements"<<"\n";
            else{
                for (unsigned int i = 0; i < TempString.length() && i<30; i++)
                    Out.DepartureDestinationPointString[i] = TempString[i];
                break;
            }
        }
        while(true){
            cout<<"Train tickets sold: ";
            cin>>TempString;
            if(!IsNumber(TempString)) cout<<"Value does not correspond format requirements"<<"\n";
             else{Out.TicketsSold=stoi(TempString);break;}
        }
        while(true){
            cout<<"Train ticket price: ";
            cin>>TempString;
            if(!IsNumber(TempString)) cout<<"Value does not correspond format requirements"<<"\n";
             else{Out.TicketPrice=stof(TempString);break;}
        }

        return Out;
    }

    string GetFullInfoString(){
        stringstream StrStream;
        StrStream << std::fixed << std::setprecision(2)
            <<"Train id: " << this->TrainId << "; "
            <<"Destination: " << DepartureDestinationPointString << "; "
            <<"Tickets sold: " << this->TicketsSold << "; "
            <<"Ticket price: " << this->TicketPrice << ";";

        return StrStream.str();
    }

    string GetIncomeInfoString(){
        stringstream StrStream;
        StrStream << std::fixed << std::setprecision(2)
            <<"Train #" << this->TrainId << " total income: "
            <<(this->TicketPrice)*(this->TicketsSold) << ";";

        return StrStream.str();
    }

    char* ToBinaryRepresentation(){
        char* Data = new char[GetSizeOfInstance()];
        char* CurrentPos = Data;

        memcpy(CurrentPos,&(this->TrainId),GetSizeOfTrainId());
        CurrentPos+=GetSizeOfTrainId();

        memcpy(CurrentPos,&(this->DepartureDestinationPointString),GetSizeOfDestination());
        CurrentPos+=GetSizeOfDestination();

        memcpy(CurrentPos,&(this->TicketPrice),GetSizeOfPrice());
        CurrentPos+=GetSizeOfPrice();

        memcpy(CurrentPos,&(this->TicketsSold),GetSizeOfAmount());

        return Data;
    }

    static RailwayTrainRecord FromBinaryRepresentation(char* Data){
        auto Out = *(new RailwayTrainRecord());
        char* CurrentPos = Data;

        memcpy(&Out.TrainId,CurrentPos,GetSizeOfTrainId());
        CurrentPos+=GetSizeOfTrainId();

        memcpy(&Out.DepartureDestinationPointString,CurrentPos,GetSizeOfDestination());
        CurrentPos+=GetSizeOfDestination();

        memcpy(&Out.TicketPrice,CurrentPos,GetSizeOfPrice());
        CurrentPos+=GetSizeOfPrice();

        memcpy(&Out.TicketsSold,CurrentPos,GetSizeOfAmount());

        return Out;
    }

    static void WriteAllToBinaryFile(vector<RailwayTrainRecord> Records, string FilePath){
        ofstream ofs; ofs.open(FilePath, std::ios::app); // add only

        for (auto &record : Records)
        {
            auto currentInstance = record.ToBinaryRepresentation();
            ofs.write(currentInstance,GetSizeOfInstance());
        }

        ofs.close();
    }

    static vector<RailwayTrainRecord> ReadAllFromBinaryFile(string FilePath){
        ifstream ifs; ifs.open(FilePath, ios::in);
        vector<RailwayTrainRecord> Records;

        size_t OneInstSizeAndStringEndChar = RailwayTrainRecord::GetSizeOfInstance()+1; // +1 - string end char
        char* buf = new char[OneInstSizeAndStringEndChar];

        while(ifs.get(buf,OneInstSizeAndStringEndChar))
            Records.push_back(FromBinaryRepresentation(buf));

        ifs.close();
        return Records;
    }

    static void CreateAndWrite(string FilePath){
        auto created = CreateUserDialog();
        vector<RailwayTrainRecord> List1(1,created);
        WriteAllToBinaryFile(List1, FilePath);
    }
};
