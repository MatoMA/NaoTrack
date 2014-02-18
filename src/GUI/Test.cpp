#include "Analyser.hpp"

int main(){
	Analyser analyser;
	std::vector<Point> vertices;

	// params
	analyser.setNbTicks(160);
	analyser.params.setNbSamples(20000);
	analyser.params.setTarget(6.7e-5);
	// connection
	analyser.connectReader("localhost","1024");
//	analyser.connectReader("172.21.3.196","3000");
	analyser.connectPublisher("localhost","1025");

	// read and publish the point cloud
	while(true){
		analyser.readAndPublish();
	}
}




