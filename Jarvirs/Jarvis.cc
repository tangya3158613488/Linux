#include"Jarvis.hpp"
#include<iostream>
#include<sstream>
#include<memory>
#include<string>
#include<json/json.h>

using namespace std;

//序列化
//int main()
//{
//    Json::Value root;
//    Json::StreamWriterBuilder swb;
//    std::ostringstream os;
//    root["Name"]="李四";
//    root["Age"]=20;
//    root["Lang"]="C++";
//
//    std::unique_ptr<Json::StreamWriter> jsonWriter(swb.newStreamWriter());
//    jsonWriter->write(root,&os);
//    std::string s=os.str();
//    std::cout<<s<<std::endl;
//
//    return 0;
//}
//反序列化
//
//void ParseJson(std::string &s)
//{
//    JSONCPP_STRING errs;
//    Json::Value root;
//    Json::CharReaderBuilder readerBuilder;
//    std::unique_ptr<Json::CharReader> const jsonReader(readerBuilder.newCharReader());
//    bool res=jsonReader->parse(s.data(),s.data()+s.size(),&root,&errs);
//    if(!res || !errs.empty()){
//        std::cerr<<"json parse error:"<<errs<<std::endl;
//        return ;
//    }
//    std::cout<< "Name: "<<root["Name"].asString()<<std::endl;
//    std::cout<<"Age: "<<root["Age"].asInt()<<std::endl;
//    std::cout<<"Lang: "<<root["Lang"].asString()<<std::endl;
//}
//int main()
//{
//    std::string s="{\"Age\" : 23,\"Lang\" : \"C++\",\"Name\" : \"王五\"}";
//    std::cout<<s<<std::endl;
//    ParseJson(s);
//    return 0;
//}

int main()
{
    Jarvis js;
    js.Run();
    return 0;
}
