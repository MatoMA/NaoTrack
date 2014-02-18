/*
 * PointPublisher.cpp
 *
 *  Created on: Feb 4, 2014
 *      Author: sheng
 */

#include <boost/asio.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <string>
#include "Point.hpp"

class PointPublisher{
	typedef boost::asio::ip::tcp tcp;

private:
	boost::asio::io_service io_service;
	boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;

	std::vector<Point> pointCloud;
	bool isConnected;

public:
	PointPublisher():isConnected(false){

	}

	virtual ~PointPublisher(){

	}

	bool isConnetected(){
		return isConnected;
	}

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
	void close(){
		socket_ptr->close();
	}

	// publish the point cloud
	void publish(const std::vector<Point>&& pc){
		std::string str_put = "put ";
		std::string str_blank = " ";
		std::string str_br = " \n";
		std::string s;
		try {
			for(Point p:pc){
				s = str_put +
					std::to_string(p.label) +
					str_blank +
					std::to_string(p.x) +
					str_blank +
					std::to_string(p.y) +
					str_br;
				std::cout<<"publishing "<<s<<std::endl;
				socket_ptr->write_some(
									boost::asio::buffer(s.c_str(),s.size()));
			}
		} catch (std::exception& e) {
			std::cout << e.what() << std::endl;
		}
	}

	void setPointCloud(std::vector<Point>& pc){
		this->pointCloud = std::move(pc);
	}

	std::vector<Point>& getPointCloud(){
		return pointCloud;
	}
};



