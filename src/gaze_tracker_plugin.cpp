#include <gaze_tracker_plugin.h>

using namespace gazebo;
using namespace std;


GazeTrackerClient::~GazeTrackerClient()
{
    close(this->sd);
    this->connection.reset();
}

void GazeTrackerClient::Load(physics::WorldPtr _parent, sdf::ElementPtr /*_sdf*/)
{
    this->ConnectToServer();
    this->connection = event::Events::ConnectWorldUpdateBegin(
          boost::bind(&GazeTrackerClient::Update, this));
}

void GazeTrackerClient::Update()
{
    this->ListenToServer();
}

void GazeTrackerClient::ConnectToServer()
{
    if((this->sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Client-socket() error");
        exit(-1);
    }
    else
        printf("Client-socket() OK\n");

    strcpy(this->server, SERVER);
    memset(&serveraddr, 0x00, sizeof(struct sockaddr_in));
    this->serveraddr.sin_family = AF_INET;
    this->serveraddr.sin_port = htons(SERVPORT);


    if((this->serveraddr.sin_addr.s_addr = inet_addr(this->server)) == (unsigned long)INADDR_NONE)
    {
       this->hostp = gethostbyname(server);
       if(this->hostp == (struct hostent *)NULL)
       {
           printf("HOST NOT FOUND --> ");
           printf("h_errno = %d\n",h_errno);
           printf("---This is a client program---\n");
           printf("Command usage: <server name or IP>\n");
           close(sd);
           exit(-1);
       }
       memcpy(&serveraddr.sin_addr, this->hostp->h_addr, sizeof(this->serveraddr.sin_addr));
    }
    if((this->rc = connect(sd, (struct sockaddr *)&serveraddr, sizeof(this->serveraddr))) < 0)
    {
        perror("Client-connect() error");
        close(this->sd);
        exit(-1);
    }
    else printf("Connection established...\n");
}

void GazeTrackerClient::ListenToServer()
{
    this->rc = read(this->sd, &buffer[0], BufferLength);
    if(this->rc < 0)
    {
        perror("Client-read() error");
        close(sd);
        exit(-1);
    }
    else if(this->rc == 0)
    {
        printf("Server program has issued a close()\n");
    }
    printf("Client-read() is OK\n");
    printf("Echoed data from the server: %s\n", buffer);
}

GZ_REGISTER_WORLD_PLUGIN(GazeTrackerClient)
