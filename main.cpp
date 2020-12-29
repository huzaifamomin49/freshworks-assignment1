#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cstdbool>
#include<sstream>
#include<vector>
#include<ctime>
#define MAX_SIZE 1073741824
#define MAX_VAL 16777216
using namespace std;


unsigned long long int fileSize(const char *filename);
bool search(char *filename,string key);
void create(char *filename,string key,long long val, long long timeout);
bool alpha(string key);
string read(char *filename,string key);
void dlete(char *filename,string key);

bool search(char *filename,string key){
    ifstream openfile;
    openfile.open("data.csv",ios::in);
    string getcontent,word;
    vector<string>v1;
     if(openfile.is_open()){
        while(!openfile.eof()){
            v1.clear();
            getline (openfile,getcontent);
            stringstream s(getcontent);
            while(getline(s,word,',')){
                v1.push_back(word);
            }
            if(v1.size()>0 and key==v1[0]){
                openfile.close();
                return true;
            }
        }
        openfile.close();
     }
     else{
         cout << "Opening File error.\n";
    }
    return false;
}


bool alpha(string key){
    for(size_t i=0;i<key.length();i++){
        if(!isalpha(key[i]))
            return false;
    }
    return true;
}


unsigned long long int fileSize(char *filename)
{
    ifstream in_file(filename, ios::binary);
    in_file.seekg(0, ios::end);
    unsigned long long int size = in_file.tellg();
    return (size);
}


void create(char *filename,string key,long long val, long long timeout=0){
    ofstream fout;
    time_t now;
    fout.open("data.csv",ios::app);
    if(search(filename,key))
        cout<<"error: "<<key<<" key already exists.\n";
    else{
        if(alpha(key)){
            if(fileSize(filename)<MAX_SIZE and val<=MAX_VAL){
                if(timeout==0){
                    now=0;
                }
                else{
                    now = time(0)+timeout;
                }
                if(key.length()<=32){
                    fout<<key<<", "<<val<<", "<<now<<"\n";
                    cout<<key<<" sucessfully added into file."<<endl;
                }
            }
            else{
                cout<<"error: Memory limit exceeded!! \n";
            }
        }
        else{
            cout<<"error: Invalind key_name!! key_name must contain only alphabets and no special characters or numbers.\n";
        }
    }
    fout.close();
}

string read(char *filename,string key){
    ifstream fio;
    fio.open(filename);
    vector<string>v1;
    string word,content;
    bool flag=0;
    while(!fio.eof()){
        v1.clear();
        getline(fio,content);
        stringstream s(content);
        while(getline(s,word,',')){
            v1.push_back(word);
        }
        if(v1[0]==key){
            flag=1;
            break;
        }
    }
    if(flag){
        if(stoll(v1[2])==0){
            string ans=v1[0]+" : "+v1[1];
            return ans;
        }
        else{
            if(time(0)<stoll(v1[2])){
                string ans=v1[0]+" : "+v1[1];
                return ans;
            }
            else{
                string ans="error: Time-To-Live of "+key+" has expired";
                return ans;
            }
        }
    }
    else{
        return "error: Key not found !!";
    }
}

void dlete(char *filename,string key){
    fstream fin, fout;
    fin.open(filename, ios::in);
    fout.open("datanew.csv", ios::out);
    int count = 0, i;
    string line, word;
    vector<string> row;
    while (!fin.eof()) {
          row.clear();
          getline(fin, line);
          stringstream s(line);
    
          while (getline(s, word, ',')) {
              row.push_back(word);
          }
    
          size_t row_size = row.size();
          if (key!=row[0]) {
              if (!fin.eof()) {
                  for (i = 0; i < row_size - 1; i++) {
                      fout << row[i] << ", ";
                  }
                  fout << row[row_size - 1] << "\n";
              }
          }
          else {
              count = 1;
          }
          if (fin.eof())
              break;
      }
      if (count == 1)
          cout <<key<<" deleted successfully.\n";
      else
          cout << "error: Key not found.\n";
    
      fin.close();
      fout.close();
      remove(filename);
      rename("datanew.csv",filename);
}

int main()
{
    ofstream fout;
    char filename[]="data.csv";
    fout.open(filename,ios::app);
    string str;
    int val;
    int time;
    while(1){
        cout<<"Press 1. to add data into file.\nPress 2. to read data from file\nPress 3. to delete data from file.\nPress 4. to exit\n";
        int n;
        cin>>n;
        switch(n){
            case 1:
                cout<<endl;
                cout<<"Enter Key : ";
                cin>>str;
                cout<<"Enter value : ";
                cin>>val;
                cout<<"If you don't want to add lifetime of key-value enter 0 otherwise enter time : ";
                cin>>time;
                create(filename,str,val,time);
                cout<<endl;
                break;
            case 2:
                cout<<endl;
                cout<<"Enter key to read from file : ";
                cin>>str;
                cout<<read(filename,str)<<endl;
                cout<<endl;
                break;
            case 3:
                cout<<endl;
                cout<<"Enter key to delete from file : ";
                cin>>str;
                dlete(filename,str);
                cout<<endl;
                break;
            case 4:
                fout.close();
                return 0;
            default:
                cout<<endl;
                cout<<"Please enter valid case.\n";
                cout<<endl;
        }
    }
    return 0;
}

