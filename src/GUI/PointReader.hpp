/*
 * PointReader.cpp
 *	Read the point cloud from the server
 *  Created on: Jan 15, 2014
 *      Author: sheng
 */

#include <boost/asio.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include "Point.hpp"

class PointReader {
	typedef boost::asio::ip::tcp tcp;

private:
	boost::asio::io_service io_service;
	boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;

	std::vector<Point> pointCloud;
	bool isConnected;

public:
	PointReader() :
			isConnected(false) {

	}

	virtual ~PointReader() {

	}

	bool isConnetected() {
		return isConnected;
	}

	// Establish the connection on socket
	bool connect(const std::string& host, const std::string& port) {
		try {
			boost::system::error_code error;
			tcp::resolver resolver(io_service);
			tcp::resolver::query query(host, port);
			tcp::resolver::iterator endpoint_it = resolver.resolve(query);
			// Establish socket connection
			socket_ptr.reset(new tcp::socket(io_service));
			socket_ptr->connect(*endpoint_it, error);
			if (error)
				std::cout << error << std::endl;
		} catch (std::exception& e) {
			std::cout << e.what() << std::endl;
			return false;
		}

		if (socket_ptr->is_open()) {
			isConnected = true;
			return true;
		} else
			return false;
	}

	// close the connection
	void close() {
		socket_ptr->close();
	}

	// read points from the server
	void read() {
		try {
			pointCloud.clear();
			std::string socket_get = "get\n";
			if (isConnected) {
				socket_ptr->write_some(
						boost::asio::buffer(socket_get.c_str(),
								socket_get.size()));
				size_t len_avail = socket_ptr->available(); // available buffer size
				std::vector<char> data(socket_ptr->available());
				boost::asio::read(*socket_ptr, boost::asio::buffer(data)); // read the buffer
				std::istringstream is(std::string(data.begin(), data.end()));
				std::string strLine;
				std::getline(is, strLine);
				int numOfPoints = atoi(strLine.c_str()); // number of points
				// Read points
				Point p;
				boost::char_separator<char> delimiter(" ");
				for (int i = 0; i < numOfPoints; ++i) {
					std::getline(is, strLine);
					// Parse the string line
					boost::tokenizer<boost::char_separator<char>> tok(strLine,
							delimiter);
					auto t = tok.begin();
					if (std::distance(t,tok.end())!= 3) // should have label,x and y, otherwise consider it bad data
						continue;
					p.label = boost::lexical_cast<int>(*t);
					t++;
					p.x = boost::lexical_cast<double>(*t);
					t++;
					p.y = boost::lexical_cast<double>(*t);
					// store the point
					pointCloud.push_back(p);
				}
			}
		} catch (std::exception& e) {
			std::cout << e.what() << std::endl;
		}
	}

	std::vector<Point>& getPointCloud() {
		read();
		return pointCloud;
	}
};
