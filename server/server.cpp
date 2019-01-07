#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <fstream>
using namespace std;

int main()
{
    // Tworzenie socketa
    int remain_data=0;
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1)
    {
        cerr << "Nie mogę utworzyć socketów. Przerywam" << endl;
        return -1;
    }

    // Bindowanie ip i portu do socketu
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(8081);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    bind(listening, (sockaddr*)&hint, sizeof(hint));

    // Tell Winsock the socket is for listening
    listen(listening, SOMAXCONN);

    // Oczekiwanie na połącznie
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);

    //socket clienta
    int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

    char host[NI_MAXHOST];      // Nazwa klienta
    char service[NI_MAXSERV];   // Z jakiego ip i portu się podłączył

    memset(host, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXSERV);

    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
    {
        cout << host << " connected na porcie " << service << endl;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host << " connected na porcie " << ntohs(client.sin_port) << endl;
    }

    // Zamykanie nasłuchiwania
    close(listening);

    // Tworzenie buffora
    char buf[4096];

    //Tworzenie szkieletu pliku na nasz odbiór
    const char* fl_name="OdebranyPlik";
    FILE *fr = fopen(fl_name,"a");
    if(fr==NULL)
        printf("Błąd pliku %s",fl_name);
    std::ofstream file(fl_name, std::ios::out|std::ios::binary);

        //odbieranie pliku i zapisanie do szkieletu pliku
        while(int bytesReceived = recv(clientSocket, buf, 4096, 0)){

        if (bytesReceived == -1)
        {
            cerr << "Błąd w otrzymywaniu pliku. Rozłączam." << endl;
        }

        if (bytesReceived == 0)
        {
            cout << "Klient rozłączony " << endl;
        }

        if(bytesReceived>0){ //jesli sa bajty z drugiej strony to zapis do pliku
        file.write(buf,bytesReceived);
        cout << "Pakiet odebrany pomyślnie " << endl;
        }
}
    //zamkniecie pliku i socketu
    fclose(fr);
    close(clientSocket);

    return 0;
}
