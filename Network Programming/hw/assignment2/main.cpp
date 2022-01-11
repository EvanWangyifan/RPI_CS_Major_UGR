#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <algorithm>

extern "C" {
//#include "./unpv13e/lib/unp.h"
#include "unp.h"
}

using namespace std;

const int MAX_CLIENT = 6;
const int BUF_SIZE = 128;

struct Connection
{
    string name;
    int fd;
};

vector<string> dictionary;
vector<Connection> connections;
string secret;

vector<string> readDictionary(char* directory_file);
string generateSecret();
bool checkName(string name);
void handleNewConnection(int fd);
bool handleWordGuess(int fd,string word);
void handleDisconnect(int fd);


// Read the dictionnary into vector of string
vector<string> readDictionary(char* directory_file)
{
    vector<string> dictionary;
    ifstream in(directory_file,ios::in);
    if(!in)
    {
        cerr << "open file error" << endl;
        return dictionary;
    }
    string temp;
    while(in >> temp)
    {
        //cout << temp << endl;
        dictionary.push_back(temp);
    }
    in.close();
    return dictionary;
}

//Make the secret word from srand
string generateSecret()
{
    int index = rand() % dictionary.size();
    cout << dictionary[index] << endl;
    return dictionary[index];
}

//Check if the username is taken
bool checkName(string name)
{
    for(int i = 0;i < connections.size(); ++i)
    {
        if(connections[i].name == name)
        {
            return false;
        }
    }
    return true;
}

//Handle new client
void handleNewConnection(int fd)
{
    char buf[BUF_SIZE];
    char readbuf[BUF_SIZE];
    memset(readbuf,0,BUF_SIZE);
    memset(buf,0,BUF_SIZE);
    //cerr << "new connection" << endl;
    
    //Send the greeting
    sprintf(buf,"Welcome to Guess the Word, please enter your username.\n");
    Writen(fd,buf,strlen(buf));
    read(fd,readbuf,BUF_SIZE);
    readbuf[strlen(readbuf)-1] = '\0';
    //cerr << "readbuf "<< readbuf << endl;
    string name = readbuf;
    //cerr << "name " << name << endl;
    
    //Keep asking client until they find an avaible username
    while(checkName(name) == false)
    {
        memset(buf,0,BUF_SIZE);
        sprintf(buf,"Username %s is already taken, please enter a different username\n", name.c_str());
        Writen(fd,buf,strlen(buf));
        memset(readbuf,0,BUF_SIZE);
        read(fd,readbuf,BUF_SIZE);
        readbuf[strlen(readbuf)-1] = '\0';
        //cerr << "readbuf "<< readbuf << endl;
        name = readbuf;
        //cerr << "name " << name << endl;
    }
    struct Connection connection;
    connection.fd = fd;
    connection.name = name;
    connections.push_back(connection);
    memset(buf,0,BUF_SIZE);
    sprintf(buf,"Let's start playing, %s\n",name.c_str());
    Writen(fd,buf,strlen(buf));
    memset(buf,0,BUF_SIZE);
    sprintf(buf,"There are %lu player(s) playing. The secret word is %lu letter(s).\n",connections.size(),secret.size());
    Writen(fd,buf,strlen(buf));
}

//Check if client find the secret word
bool handleWordGuess(int fd,string word)
{
    //int compare[26] = {0};
    char buf[BUF_SIZE];
    memset(buf,0,BUF_SIZE);
    
    //If the size is wrong
    if(word.size() != secret.size())
    {
        sprintf(buf,"Invalid guess length. The secret word is %lu letter(s).\n",secret.size());
        cerr << word.size() << endl;
        Writen(fd,buf,strlen(buf));
        return false;
    }
    else
    {
        int perfect_match = 0;
        int misplaced_match = 0;
        int* matched = new int[secret.size()];
        memset(matched,0,sizeof(int)*secret.size());
        
        //check if the guess is perfect match
        for(int i = 0;i < secret.size();++i)
        {
            if(secret[i] == word[i])
            {
                perfect_match += 1;
            }
        }
        
        //Find the number of correct letter
        for(int i = 0;i < secret.size();++i)
        {
            //int offset = word[i] - 'a';
            //if(compare[offset] != 1)
            //{
            for(int j = 0;j < secret.size();++j)
            {
                if(word[i] == secret[j] && matched[j] == 0)
                {
                    matched[j] = 1;
                    ++misplaced_match;
                    break;
                }
            }
        }
        string name;
        for(int i = 0;i < connections.size();++i)
        {
            if(connections[i].fd == fd)
            {
                name = connections[i].name;
                break;
            }
        }
        
        //When a player find the secret word, return ture
        //else return falsse
        if(perfect_match == secret.size())
        {
            sprintf(buf,"%s has correctly guessed the word %s\n",name.c_str(),secret.c_str());
            for(int i = 0;i < connections.size();++i)
            {
                Writen(connections[i].fd,buf,strlen(buf));
            }
            return true;
        }
        else
        {
            sprintf(buf,"%s guessed %s: %d letter(s) were correct and %d letter(s) were correctly placed.\n",name.c_str(),word.c_str(),misplaced_match,perfect_match);
            for(int i = 0;i < connections.size();++i)
            {
                Writen(connections[i].fd,buf,strlen(buf));
            }
            return false;
        }
    }
}

//Remove current player from the Connection, and reset the username
void handleDisconnect(int fd)
{
    int i = 0;
    for(i = 0;i < connections.size(); ++i)
    {
        if(connections[i].fd == fd)
        {
            break;
        }
    }
    if(connections.size() > 0)
    {
        //cerr << "disconnecting " << fd << " " << connections[i].name << endl;
        connections.erase(connections.begin()+i);
    }
    else
    {
        cerr << "connection remove error" << endl;
    }
}


int main(int argc,char** argv)
{
    //Check if we got enough command line argument
    if(argc != 5)
    {
        fprintf(stderr,"usage: ./word_guess.out [seed] [port] [dictionary_file] [longest_word_length]\n");
    }
    
    //Read the argument
    int seed = atoi(argv[1]);
    unsigned short port = atoi(argv[2]);
    //longest_word_length = atoi(argv[4]);
    dictionary = readDictionary(argv[3]);
    if(dictionary.empty())
    {
        exit(1);
    }
    
    //Variable initialized here
    srand(seed);
    int sockfd = Socket(AF_INET,SOCK_STREAM,0);
    bool bReuseaddr = true;
    setsockopt(sockfd,SOL_SOCKET ,SO_REUSEADDR,(const char*)&bReuseaddr,sizeof(bool));

    //Making sockets
    struct sockaddr_in servaddr;
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    //Bind the listener
    Bind(sockfd,(SA*)&servaddr,sizeof(servaddr));
    Listen(sockfd,20);

    int client[MAX_CLIENT];
    char buf[BUF_SIZE];
    fd_set ready_set,allset;
    int maxfd = sockfd;
    int maxi = -1;
    for(int i = 0;i < MAX_CLIENT;++i)
    {
        client[i] = -1;
    }

    // clear the descriptor set
    FD_ZERO(&allset);
    FD_SET(sockfd,&allset);
    struct sockaddr_in cliaddr;
    socklen_t clilen;
    secret = generateSecret();
    transform(secret.begin(),secret.end(),secret.begin(),::tolower);
    //cerr << "secret: " << secret << endl;
    
    for(;;)
    {
        ready_set = allset;
        int nready = Select(maxfd+1,&ready_set,NULL,NULL,NULL);
        
        //Check if there is any incoming connection
        if(FD_ISSET(sockfd,&ready_set))
        {
            clilen = sizeof(cliaddr);
            int conn = Accept(sockfd,(SA*)&cliaddr,&clilen);
            int i = 0;
            for(i = 0;i < MAX_CLIENT;++i)
            {
                if(client[i] < 0)
                {
                    client[i] = conn;
                    break;
                }
            }
            if(i == MAX_CLIENT)
            {
                err_quit("too many clients");
            }
            handleNewConnection(conn);
            FD_SET(conn,&allset);
            if(conn > maxfd)
            {
                maxfd = conn;
            }
            if(i > maxi)
            {
                maxi = i;
            }
            if(--nready <= 0)
            {
                continue;
            }
        }
        //cerr << "hello1" << endl;
        int fd = 0;

        //Check if any client response
        for(int i = 0;i <= maxi;++i)
        {
            //cout << "abc" << endl;
            if((fd = client[i]) < 0)
            {
                continue;
            }
            if(FD_ISSET(fd,&ready_set))
            {
                int n;
                memset(buf,0,BUF_SIZE);
                //If client close the connection
                if((n = read(fd,buf,BUF_SIZE)) == 0)
                {
                    //cerr << "disconnecting..." << endl;
                    handleDisconnect(fd);
                    close(fd);
                    FD_CLR(fd,&allset);
                    client[i] = -1;
                }
                else
                {
                    buf[strlen(buf)-1] = '\0';
                    string word = buf;
                    transform(word.begin(),word.end(),word.begin(),::tolower);
                    
                    //When a player find the secret word, close all connection
                    if(handleWordGuess(fd,word))
                    {
                        for(int i = 0;i < connections.size();++i)
                        {
                            close(connections[i].fd);
                            FD_CLR(connections[i].fd,&allset);
                            for(int i = 0;i < MAX_CLIENT;++i)
                            {
                                client[i] = -1;
                            }
                        }
                        connections.clear();
                        maxfd = sockfd;
                        maxi = -1;
                        secret = generateSecret();
                        transform(secret.begin(),secret.end(),secret.begin(),::tolower);
                        //cerr << "secret: " << secret << endl;
                        break;
                    }
                }
                if(--nready <= 0)
                {
                    break;
                }
            }
        }
    }
    close(sockfd);
}
