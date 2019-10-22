CC=g++
INCLUDE= -Ispeech/
LIB= -ljsoncpp -lcurl -lcrypto
bin=VoiceAssistant
src=VoiceAssistant.cc

$(bin):$(src)
	$(CC)  -o $@ $^ $(INCLUDE) $(LIB) -std=c++11 
.PHONY:clean
clean:
	rm -f $(bin)
