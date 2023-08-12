/// this will test whether SERVER has been defined or not,
/// if it had been it will compile the Server class otherwise the default(client class)
#if defined(SERVER)

class Server :public Comms
{
public:
	int main();
};

/// <summary>
/// the main function of server binds the socket and will accept a conncetion, 
/// with then using a while loop where it will receive data from the client
/// into a buffer, which it prints to the screen then it waits for a reply from the user which
/// it sends back to the client stops when it receives the string QUIT. 
/// </summary>
int Server::main() {
	try {
		bool running = true;
		bindSocket();
		acceptConnection();

		while (running) {
			char buffer[200] = "";
			receiveBuffer(buffer, 200, 0);

			printf(buffer);
			printf("\n");

			/// <summary>
			/// If QUIT received from the client then SHUTDOWN.
			/// </summary>
			if (strcmp(buffer, "QUIT") == 0) {
				printf("Shut down in progress");
				running = false;
			}
			// Otherwise get a reply from the client and sends to the client
			else {
				char buffer[200];
				printf("Enter your reply: ");
				cin.getline(buffer, 200);
				sendBuffer(buffer, 200, 0);
			}
		}
	}
	/// <summary>
	/// catch any exceptions
	/// </summary>
	catch (int e) {
		printf("\nERROR\n");
		//used to convert the error code into string message
		char buffer[200];
		errmsg(e, buffer, 200);
		printf(buffer);
	}

	//this is used to pause the console so that the user/client can view the error
	system("pause");
	/// <summary>
	WSACleanup();

	return 0;
}

int main()
{
	/// <summary>
	/// creates instance of Server and then calls its main(),
	/// which contains the core functionallity of the server
	/// </summary>
	Server myServer;
	return myServer.main();
}
// The Client is derived from comms
// Client::main() connects to the server using the connectSocket() function

// Then it loops to QUIT if the user enters QUIT the client will shut down 
// the progress and exit the loop

// if an exception is thrown then function errmsg(e, buffer, 200) is used to convert the error code to a string message 
// and store it in the buffer. 

// The #else block is used to compile the client code as a default if the server isnt defined

#else 
class Client :public Comms
{
public:
	int main();
};

int Client::main()
{
	try {
		bool running = true;
		connectSocket();

		while (running) {
			char buffer[200];
			printf("Enter your message ");
			cin.getline(buffer, 200);
			sendBuffer(buffer, 200, 0);

			if (strcmp(buffer, "QUIT") == 0) {
				printf("Shut down in progress");
				running = false;
			}
			else {
				receiveBuffer(buffer, 200, 0);
				printf(buffer);
				printf("\n");
			}
		}
	}
	catch (int e) {
		printf("\nERROR\n");
		char buffer[200];
		errmsg(e, buffer, 200);
		printf(buffer);
	}

	system("pause");
	WSACleanup();
	return 0;
}

int main()
{
	Client myClient;
	return myClient.main();
}
#endif
