// #include <iostream>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <unistd.h>
// #include <cstring>
// #include <SFML/Graphics.hpp>

// int main() {
//     int server_fd, new_socket;
//     struct sockaddr_in address;
//     int opt = 1;
//     int addrlen = sizeof(address);
//     char buffer[1024] = {0};
//     const char* hello = "Hello from server";

//     // Criando o descritor de arquivo do socket
//     if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
//         perror("socket failed");
//         exit(EXIT_FAILURE);
//     }

//     // Anexando o socket à porta 8080
//     if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
//         perror("setsockopt");
//         exit(EXIT_FAILURE);
//     }

//     address.sin_family = AF_INET;
//     address.sin_addr.s_addr = INADDR_ANY; // Aceita conexões de qualquer IP
//     address.sin_port = htons(8080); // Porta 8080

//     // Bind do socket à porta 8080
//     if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
//         perror("bind failed");
//         exit(EXIT_FAILURE);
//     }

//     // Colocando o servidor para escutar conexões
//     if (listen(server_fd, 3) < 0) {
//         perror("listen");
//         exit(EXIT_FAILURE);
//     }

//     // Aceitando conexões
//     if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
//         perror("accept");
//         exit(EXIT_FAILURE);
//     }

//     // Lendo dados do socket
//     read(new_socket, buffer, 1024);
//     std::cout << "Message from client: " << buffer << std::endl;

//     // Enviando resposta para o cliente
//     send(new_socket, hello, strlen(hello), 0);
//     std::cout << "Hello message sent\n";

//     // Fechando o socket
//     close(new_socket);
//     close(server_fd);

//     return 0;
// }

#include <SFML/Graphics.hpp>
#include <iostream>
#include <boost/asio.hpp>

extern "C" {
    #include <libavutil/frame.h>
}

using boost::asio::ip::tcp;

class video_session {
    sf::RenderWindow window_;
    sf::Texture texture_;
    sf::Sprite sprite_;
    tcp::socket socket_;

public:
    video_session(tcp::socket socket) : socket_(std::move(socket)), window_(sf::VideoMode(640, 480), "Video Stream") {
        texture_.create(640, 480);
        sprite_.setTexture(texture_);
    }

    void process_frame(AVFrame* frame) {
        if (frame && frame->data[0]) {  // Garantindo que o frame e o data[0] são válidos.
            texture_.update((const sf::Uint8*)frame->data[0], 640, 480, 0, 0);
        }
        window_.clear();
        window_.draw(sprite_);
        window_.display();
    }
};