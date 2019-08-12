#pragma once
#include<iostream>
#include<cstdio>
#include<fstream>
#include<string>
#include<map>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unordered_map>
#include"speech.h"
#include"base/http.h"

#define SPEECH_FILE "temp_file/demo.wav"
#define PLAY_FILE "temp_file/play.mp3"
#define CMD_ETC "command.etc"

//采用图灵机器人，进行智能对话
class InterRobot{
    private:
        std::string url="openapi.tuling123.com/openapi/api/v2";
        std::string api_key="2e92fea2fe204f95b4009b193101a09c";
        std::string user_id="1";
        aip::HttpClient client;
    public:
        InterRobot()
        {}

        Json::Value PostRequest(Json::Value data)
        {
            std::string response;
            Json::Value obj;
            Json::CharReaderBuilder crbuilder;
            int code =this->client.post(url,nullptr,data,nullptr,&response);
            if(code!=CURLcode::CURLE_OK){
                obj[aip::CURL_ERROR_CODE]=code;
                return obj;
            }
            std::string error;
            std::unique_ptr<Json::CharReader> reader(crbuilder.newCharReader());
            reader->parse(response.data(),response.data()+response.size(),&obj,&error);
            return obj;
        }
        std::string Talk(std::string &message)
        {
            Json::Value root;
            Json::Value item1;
            Json::Value item2;
            root["reqType"]=0;
            item1["text"]=message;
            item2["inputText"]=item1;
            item1.clear();
            root["perception"]=item2;
            item2.clear();
            item2["apiKey"]=api_key;
            item2["userId"]=user_id;
            root["userInfo"]=item2;
            item2.clear();

            Json::Value ret=PostRequest(root);
            Json::Value _result=ret["results"];
            Json::Value values=_result[0]["values"];
            std::cout<<"Robot: "<<values["text"].asString()<<std::endl;
            return values["text"].asString();
        }
        ~InterRobot()
        {}
};
//语音识别Speech Recognition
class SpeechRec{
    private:
        std::string app_id="16877199";
        std::string api_key="I3OwS5aZK5nOe3RwHSsxmah8";
        std::string secret_key="DI7Dg05WjcsTqjl0AQkDCI2pleayHEkm";
        aip::Speech *client;
    public:
        SpeechRec()
        {
            client=new aip::Speech(app_id,api_key,secret_key);
        }
        //语音识别Automatic Apeech Recognition
        void ASR(int &err_code,std::string &message)
        {
            std::cout<<std::endl<<"...正在识别..."<<std::endl;
            std::map<std::string,std::string> options;
            options["dev_pid"]="1536";
            options["lan"]="ZH";

            std::string file_content;
            aip::get_file_content(SPEECH_FILE,&file_content);//获取你的语音文件
            //开始识别
            Json::Value result=client->recognize(file_content,"wav",16000,options);
            err_code=result["err_no"].asInt();
            if(err_code==0){
                message=result["result"][0].asString();
            }
            else{
                message="...识别错误...";
            }
        }
        //语音合成Text To Speech
        void TTS(std::string message)
        {
            std::ofstream ofile;
            std::string file_ret;
            std::map<std::string,std::string> options;
            options["spd"]="5";
            options["per"]="0";

            ofile.open(PLAY_FILE,std::ios::out | std::ios::binary);
            //语音合成，将文本转成语音，放到指定目录，形成指定文件
            Json::Value result=client->text2audio(message,options,file_ret);
            if(!file_ret.empty()){
                ofile<<file_ret;
            }
            else{
                std::cout<<"error: "<<result.toStyledString();
            }
            ofile.close();
        }
        ~SpeechRec()
        {
            delete client;
            client=NULL;
        }
};
//Jarvis核心逻辑
class Jarvis{
    private:
        SpeechRec sr;
        InterRobot robot;
        std::unordered_map<std::string,std::string> command_set;
    public:
        //加载命令执行配置文件
        Jarvis()
        {
            char buffer[256];
            std::ifstream in(CMD_ETC);
            if(!in.is_open()){
                std::cerr<<"open file error"<<std::endl;
                exit(1);
            }
            std::string sep=":";
            while(in.getline(buffer,sizeof(buffer))){
                std::string str=buffer;
                std::size_t pos=str.find(sep);
                if(std::string::npos){
                    std::cerr<<"Load Etc Error"<<std::endl;
                    exit(2);
                }
                std::string k=str.substr(0,pos);
                std::string v=str.substr(pos+sep.size());
                k+="。";
                command_set.insert(std::make_pair(k,v));
            }
            std::cout<<"Load Command etc...done"<<std::endl;
            in.close();
        }
        //在Linux中执行嗯指定命令，采用popen
        bool Exec(std::string command,bool print)
        {
            FILE *fp=popen(command.c_str(),"r");
            if(NULL==fp){
                std::cerr<<"popen error"<<std::endl;
                return false;
            }
            if(print){
                char c;
                std::cout<<"----------------start-------------"<<std::endl;
                while(fread(&c,1,1,fp)>0){
                    std::cout<<c;
                }
                std::cout<<std::endl;
                std::cout<<"-----------------end---------------"<<std::endl;
            }
            pclose(fp);
            return true;
        }
        //判断消息是否是需要执行的命令，如果是命令，需要执行，而不需要交给图灵机器人进行对话
        bool MessageIsCommand(std::string _message,std::string &cmd)
        {
            std::unordered_map<std::string,std::string>::iterator iter=command_set.find(_message);
            if(iter!=command_set.end()){
                cmd=iter->second;
                return true;
            }
            cmd ="";
            return false;
        }
        //使用arecord工具进行录音，并进行语音识别，语音转文本
        bool RecordAddASR(std::string &message)
        {
            int err_code=-1;
            std::string record="arecord -t wav -c 1 -r 16000 -d 5 -f S16_LE ";
            record += SPEECH_FILE;
            record += ">/dev/null 2>&1";//不显示输出结果或者消息

            std::cout<<"...请讲话...";
            fflush(stdout);
            if(Exec(record,false)){
                sr.ASR(err_code,message);
                if(err_code==0){
                    return true;
                }
                std::cout<<"语音识别失败..."<<std::endl;
            }
            else{
                std::cout<<"录制失败..."<<std::endl;
            }
            return false;
        }
        bool TTSAndPlay(std::string message)
        {
            //cvlc命令形式和播放
            std::string play="cvlc --play-and-exit ";//播放完毕退
            play += PLAY_FILE;
            play += " >/dev/null 2>&1";
            sr.TTS(message);//语音识别
            Exec(play,false);//执行播放
            return true;
        }
        void Run()
        {
            volatile bool quit=false;
            std::string message;
            while(!quit){
                message="";
                bool ret=RecordAddASR(message);//先进行录音和识别
                if(ret){
                    std::string cmd;
                    std::cout<<"我# "<<message<<std::endl;
                    if(MessageIsCommand(message,cmd)){//判断是否是命令
                        if(message=="退出。"){
                            TTSAndPlay("好吧，期待下次和你聊天");
                            std::cout<<"bye bye...:)"<<std::endl;
                            quit=true;
                        }
                        else{
                             Exec(cmd,true);
                         }
                    }
                    else{
                         //不是命令就交给图灵机器人识别
                         std::string play_message=robot.Talk(message);
                         TTSAndPlay(play_message);
                    }
                }
            }
        }
        ~Jarvis()
        {}
};
