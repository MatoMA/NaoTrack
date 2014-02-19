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
	double target;
	int nb_ticks, nb_samples, x_range, y_range;
	boost::property_tree::ptree pt;
	try {
		boost::property_tree::xml_parser::read_xml(CONF_FILENAME, pt);
		// Connection params
		input_server = pt.get<std::string>("config.analyser.input_server");
		input_server_port = pt.get<std::string>("config.analyser.input_server_port");
		output_server = pt.get<std::string>("config.analyser.output_server");
		output_server_port = pt.get<std::string>("config.analyser.output_server_port");
		// GNGT params
		target = pt.get<double>("config.analyser.target");
		nb_ticks = pt.get<int>("config.analyser.nb_ticks");
		nb_samples = pt.get<double>("config.analyser.nb_samples");
		x_range = pt.get<double>("config.analyser.x_range");
		y_range = pt.get<double>("config.analyser.y_range");
	} catch (boost::property_tree::xml_parser::xml_parser_error& e) {
		// prompt the error when failed to read the config file
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	// params
	analyser.setNbTicks(nb_ticks);
	analyser.params.setNbSamples(nb_samples);
	analyser.params.setTarget(target);
	analyser.xRange = x_range;
	analyser.yRange = y_range;

	// connection
	analyser.connectReader(input_server,input_server_port);
	analyser.connectPublisher(output_server, output_server_port);

	// read and publish the point cloud
	while(true){
		analyser.readAndPublish();
	}
}




