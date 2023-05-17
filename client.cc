//Some of this code is from Boost's documentation
#include <cstdlib>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
using namespace std;
using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
	try
	{
		tcp::iostream stream("localhost", "2101"); //[1] == host, [2] == port
		if (!stream) //Connection failed
		{
			cout << "Unable to connect: " << stream.error().message() << endl;
			return EXIT_FAILURE;
		}

		while (true) {
			int code = 0;
			stream >> code;
			string data;
			stream >> data;
			if (!stream) break;
			cout << "Code: " << code << endl;
			cout << "Data: " << data << endl;
			stream << 102 << endl;
			stream << "OK" << endl;
		}
	}
	catch (exception& e)
	{
		cout << "Exception: " << e.what() << endl;
	}
}
