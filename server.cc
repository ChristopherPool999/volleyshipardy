//Some of this code is from Boost's documentation
#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <unistd.h>

using namespace std;
using boost::asio::ip::tcp;

int main()
{
	try
	{
		//This is code to accept a connection as a server
		boost::asio::io_context io_context;
		tcp::endpoint endpoint(tcp::v4(), 2101); //Change this port to whatever you want
		tcp::acceptor acceptor(io_context, endpoint);
		tcp::iostream stream;
		boost::system::error_code ec;
		cout << "Waiting for a Client to connect...\n";
		acceptor.accept(stream.socket(), ec);
		cout << "Client connected!\n";

		if (!ec)
		{
			while (true) {
				stream << 101 << endl; //101 means Quiz has been answered
				stream << (rand() % 2 ? "SUCCESS" : "FAILURE") << endl;
				int code = 0;
				stream >> code;
				string data;
				stream >> data;
				if (!stream) break;
				cout << "Code: " << code << endl;
				cout << "Data: " << data << endl;
				usleep(333'333);
			}
		}
		else
			cout << "Sadge.\n";
	}
	catch (exception& e)
	{
		cerr << e.what() << endl;
	}
}
