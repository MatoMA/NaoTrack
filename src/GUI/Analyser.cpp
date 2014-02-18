#include "Analyser.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

// config filename
static const std::string CONF_FILENAME = "config.xml";

int main(){
	Analyser analyser;
	// Config
	std::string input_server, input_server_port,
				output_server, output_server_port;
	boost::property_tree::ptree pt;
	try {
		boost::property_tree::xml_parser::read_xml(CONF_FILENAME, pt);
		input_server = pt.get<std::string>("config.input_server");
		input_server_port = pt.get<std::string>("config.input_server_port");
		output_server = pt.get<std::string>("config.output_server");
		output_server_port = pt.get<std::string>("config.output_server_port");
	} catch (boost::property_tree::xml_parser::xml_parser_error& e) {
		// prompt the error when failed to read the config file
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	// params
	analyser.setNbTicks(160);
	analyser.params.setNbSamples(20000);
	analyser.params.setTarget(5e-2);
	// connection
	analyser.connectReader(input_server,input_server_port);
	analyser.connectPublisher(output_server, output_server_port);

	// read and publish the point cloud
	while(true){
		analyser.readAndPublish();
	}
}




