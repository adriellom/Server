/*-------------------------------------------------------------*
 * Server.cpp                                                  *
 * Historico:	25/05/22	Primera version                    *
 * Autor: Lombardo Leandro Adriel                              *
 *                                                             *
 *-------------------------------------------------------------*/

#include "Server.h"

#pragma comment(lib,"ws2_32.lib")

using namespace std;


/*-------------------------------------------------------------*
 * main()                                                      *
 *                                                             *
 * Entrada:     No posee                                       *
 *                                                             *
 * Salida:      No posee                                       *
 *                                                             *
 * Retorno:     No posee                                       *
 *                                                             *
 * Descripcion: Rutina principal del programa                  *
 *-------------------------------------------------------------*/
int main()
{
	// Variables para la longitud
	int send_len = 0;
	int recv_len = 0;
	int len = 0;
	
	// Variables para bufer de envio y recepcion
	char send_buf[100];
	
	// Variables para el socket
	SOCKET s_server;
	SOCKET s_accept;
	
	// Variables para direccion del servidor y cliente
	SOCKADDR_IN server_addr;
	SOCKADDR_IN accept_addr;
	int port;

	//Variables de la señal
	int i;
	double t;	
	double y;	
	double vp;	// Peak value = VRms*raiz de 2

	initialization();
	
	// Completa la información del servidor
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	cout << "Enter port number for communication" << endl;
	cin >> port;
	server_addr.sin_port = htons(port);
	
	// Crea el socket
	s_server = socket(AF_INET, SOCK_STREAM, 0);
	if (bind(s_server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		cout << "Socket binding failed" << endl;
		WSACleanup();
	}
	else
	{
		cout << "The plug is connected correctly" << endl;
	}
	
	// Establece el socket en estado de escucha
	if (listen(s_server, SOMAXCONN) < 0)
	{
		cout << "Could not set monitoring state" << endl;
		WSACleanup();
	}
	else
	{
		cout << "Set monitoring status to success" << endl;
	}
	cout << "The server is monitoring the connection, please wait..." << endl;
	
	// Acepta solicitud de conexion
	len = sizeof(SOCKADDR);
	s_accept = accept(s_server, (SOCKADDR*)&accept_addr, &len);
	if (s_accept == SOCKET_ERROR)
	{
		cout << "The connection failed" << endl;
		WSACleanup();
		return 0;
	}
	cout << "Connection established, ready to send data" << endl;

	// Calculo de valor pico 
	vp = sqrt(2);
	cout << "value of vp " << vp << endl;
	
	// Bucle para Enviar datos
	while (1)
	{
		for (i = 0 ; i < 100 ; i++) 
		{
			t = i / 100.0;
			y = vp * sin(2 * pi * f * t);
			sprintf_s(send_buf, "%.4f", y);
			
			send_len = send(s_accept, send_buf, 100, 0);
			if (send_len < 0)
			{
				cout << "Failed to send" << endl;
				break;
			}
		}
		// for de delay
		for (i = 0; i < 1000000000; i++)
		{
		}
		if (send_len < 0)
		{
			cout << "The connection with the client was lost" << endl;
			break;
		}
	}
	
	// Cerrar la conexion
	closesocket(s_server);
	closesocket(s_accept);
	
	// Liberar recursos DLL
	WSACleanup();
	return 0;
}

/*-------------------------------------------------------------*
 * initialization()                                            *
 *                                                             *
 * Entrada:     No posee                                       *
 *                                                             *
 * Salida:      No posee                                       *
 *                                                             *
 * Retorno:     No posee                                       *
 *                                                             *
 * Descripcion: Inicializa la libreria de sockets              *
 *-------------------------------------------------------------*/
void initialization(void)
{
	// Inicializa la libreria de sockets
	WORD w_req = MAKEWORD(2, 2);
	
	// Numero de version
	WSADATA wsadata;
	int err;
	err = WSAStartup(w_req, &wsadata);
	if (err != 0)
	{
		cout << "Error initializing socket library" << endl;
	}
	else
	{
		cout << "Sockets library initialized successfully" << endl;
	}
	
	// Verifica el numero de version
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2)
	{
		cout << "Socket library version number does not match" << endl;
		WSACleanup();
	}
	else
	{
		cout << "Socket library version is correct" << endl;
	}
}
