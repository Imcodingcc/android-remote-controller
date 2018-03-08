#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <unistd.h>
#include <reader.h>
#include <value.h>
#include <string.h>
#include <iostream>
#include <cstdlib>
#include <uinput.h>
#include <signleTouch.h>

using namespace std;

int main(int argc, char *argv[]) {
    int fd = createTouchScreen();
    int server_sockfd;
    int client_sockfd;
    int len;
    struct sockaddr_in my_addr;
    struct sockaddr_in remote_addr;
    Json::Reader reader;
    Json::Value root;
    int sin_size;
    char buf[BUFSIZ];
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = INADDR_ANY;
    my_addr.sin_port = htons(10151);

    if ((server_sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return 1;
    }

    if (bind(server_sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) < 0) {
        perror("bind");
        return 1;
    }

    listen(server_sockfd, 5);

    sin_size = sizeof(struct sockaddr_in);
    while (1) {
        if ((client_sockfd = accept(server_sockfd, (struct sockaddr *) &remote_addr,
                                    (socklen_t *) &sin_size)) < 0) {
            perror("accept");
            return 1;
        }
        printf("accept client %s\n", inet_ntoa(remote_addr.sin_addr));
        len = send(client_sockfd, "Welcome to my server\n", 21, 0);

        while ((len = recv(client_sockfd, buf, BUFSIZ, 0)) > 0) {
            buf[len] = '\0';
            printf("%s\n", buf);
            string a = buf;
            reader.parse(a, root);
            int code = root["code"].asInt();
            if (code == 0) {
                int x = root["x"].asInt();
                int y = root["y"].asInt();
                nvr_execute_down(fd, x, y);
            } else if (code == 1) {
                int x = root["x"].asInt();
                int y = root["y"].asInt();
                nvr_execute_move(fd, x, y);
                //handleTouch(TOUCHDOWN, 0, 0, ROTATING_90, 1);
            }else if(code == 2){
                nvr_execute_up(fd);
            }
        }
        close(client_sockfd);
    }
}