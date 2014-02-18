#include <vq2.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cmath>
#include <utility>
#include "Point.hpp"
#include "PointReader.hpp"
#include "PointPublisher.hpp"

// This is the GNGT neurons.
typedef vq2::algo::gngt::Unit<Point> Unit;
// Do not forget the vertex initialization functor (3rd argument).
typedef vq2::Graph<Unit, char, Unit::copy_constructor> Graph;
typedef Graph::vertex_type                 Vertex;
typedef Graph::edge_type                 Edge;
typedef Graph::ref_vertex_type             RefVertex;
typedef Graph::ref_edge_type             RefEdge;

// For finding the winner in the GNGT algorithm, we need some
// similarity measure. Here, let us use the squared euclidian
// distance.
class Similarity {
public:
	typedef Point value_type;
	typedef Point sample_type;

	double operator()(const value_type& arg1, const sample_type& arg2) {
		double dx = arg1.x - arg2.x;
		double dy = arg1.y - arg2.y;
		return dx * dx + dy * dy;
	}
};
typedef vq2::unit::Similarity<Unit, Similarity> UnitSimilarity;

// The learning process.
class Learn {
public:
	typedef Point sample_type;
	typedef Point weight_type;
	void operator()(double coef, weight_type& prototype,
			const sample_type& target) {
		prototype.x += coef * (target.x - prototype.x);
		prototype.y += coef * (target.y - prototype.y);
	}
};
typedef vq2::unit::Learn<Unit, Learn> UnitLearn;

// This converts a Point into a 2D position on some A4 sheet paper.
class FigConverter {
public:
	typedef Point position_type;

	void locate(const position_type& pos, double& x_cm, double& y_cm) {
		x_cm = 21.0 / 2 + 2 * pos.x;
		y_cm = 29.7 / 2 + 2 * pos.y;
	}
};
typedef vq2::unit::XfigConverter<Unit, FigConverter> UnitFigConverter;

// This controls the accuracy of the input sampling. THIS IS NOT THE
// NUMBER OF INPUTS (see further).
#define NB_SAMPLES 20000
#define TARGET      1e-4
#define NT          (TARGET*NB_SAMPLES)

// This is the parameter set for GNG-T and evolution
class Params {
private:
	double m_target;
	double m_nbSamples;
public:

	Params():m_target(2e-5),
	         m_nbSamples(20000)
	{
	}

	// GNG-T
	int ageMax(void) {
		return 20;
	}
	double firstLearningRate(void) {
		return .001;
	}
	double secondLearningRate(void) {
		return .2 * firstLearningRate();
	}
	double lambda(void) {
		return .001;
	}
	double infinity(void) {
		return 1e12;
	}

	// Evolution
	void setTarget(double t){
		this->m_target = t;
	}

	double target(void) {
		return m_target;
	}

	void setNbSamples(int n){
		this->m_nbSamples = n;
	}

	int nbSamples(void) {
		return m_nbSamples;
	}
	double lowPassCoef(void) {
		return .4;
	}
	double delta(void) {
		return .75;
	}
	double margin(void) {
		return .2;
	}
};

// Some evolution class is required to control the GNG-T evolution. It fits
// vq2::concept::GNGTEvolution. Let us use the default one here.
typedef vq2::by_default::gngt::Evolution<Params> Evolution;

// Let us gather density manipulations in some class. All the Input
// related stuff are used to generate artificial input sample
// distributions.
class Input {
private:

	typedef vq2::proba::Density<Point> Density;

	class Box: public Density {
	public:
		typedef Point event_type;
		Point min, max;
		double density;
		bool enabled;

		Box(const Point& origin, double width, double height,
				double proba_density) :
				min(origin), max(origin + Point(width, height)), density(
						proba_density), enabled(false) {
		}

		virtual double operator()(const event_type& evt) {
			if (enabled && evt.x >= min.x && evt.x < max.x && evt.y >= min.y
					&& evt.y < max.y)
				return density;
			return 0;
		}
	};

	Box& box1;
	Box& box2;
	Box& noise;
	Density& glue1;
	Density& glue2;

public:

	std::vector<Point> samples;

	Input(void) :
			box1(*(new Box(Point(1, 1), 1, 1, 1))), box2(
					*(new Box(Point(3, 1), 1, 1, .5))), noise(
					*(new Box(Point(0, 0), 5, 3, .05))), glue1(
					vq2::proba::max((Density&) box1, (Density&) box2)), glue2(
					vq2::proba::max(glue1, (Density&) noise)) {
	}

	~Input(void) {
		delete &box1;
		delete &box2;
		delete &noise;
		delete &glue1;
		delete &glue2;
	}

#define INPUT_STAGE_LENGTH     20
	void acquireSamples(int t) {
		Point p;

		int stage = t / INPUT_STAGE_LENGTH;
		int phase = stage % 4;

		switch (phase) {
		case 0:
			box1.enabled = true;
			box2.enabled = false;
			noise.enabled = false;
			break;
		case 1:
			box1.enabled = true;
			box2.enabled = true;
			noise.enabled = false;
			break;
		case 2:
			box1.enabled = true;
			box2.enabled = true;
			noise.enabled = true;
			break;
		case 3:
			box1.enabled = true;
			box2.enabled = false;
			noise.enabled = true;
			break;
		}

		samples.clear();
		for (int i = 0; i < NB_SAMPLES; ++i) {
			p.x = vq2::proba::random::uniform(0, 5);
			p.y = vq2::proba::random::uniform(0, 3);
			if (vq2::proba::random::toss(glue2(p)))
				samples.push_back(p);
		}
	}
};

// These are some functions used inside the main, and defined
// afterwards.
std::string toString(int nb);
void print(int nb, Graph& g, Input& input);
void generateMakefile(void);
void generateGnuplot(void);

#define CAPTURE_RANGE 0.1
// return true is the input point is within the range of reference point
bool isInRange(Point& pRef, Point& pIn, float range) {
	if (hypot(pRef.x - pIn.x, pRef.y - pIn.y) <= range)
		return true;
	return false;
}

// generate random points and take samples with the range of interest points
std::vector<Point> samplePointCloud(float xRange, float yRange, int nSamples,
		std::vector<Point>& pclIn) {
	std::vector<Point> pclOut;
	std::vector<Point> randomSamples;
	Point p;
	// generate random samples
	for (int i = 0; i < nSamples; ++i) {
		p.x = vq2::proba::random::uniform(0, xRange);
		p.y = vq2::proba::random::uniform(0, yRange);
		randomSamples.push_back(p);
	}
	// get the points within the range
	for (Point& p : pclIn) {
		pclOut.push_back(p);
		for (Point& s : randomSamples) {
			if (isInRange(p, s, CAPTURE_RANGE))
				pclOut.push_back(s);
		}
	}
	return pclOut;
}

// Identity. You can also use a lambda function.
const Point& sample_of(const Point& p) {
	return p;
}

class GetVertexFromGraph{
public:
	std::vector<Point> vPoints;
	bool operator()(Vertex& n){
		vPoints.push_back(n.value.prototype());
		return false;
	}
};

class Analyser{
private:
	int nbTicks;

	Graph g;
	Similarity distance;
	UnitSimilarity unit_distance;
	Learn learn;
	UnitLearn unit_learn;
	Evolution evolution;
	Input input;
	std::vector<Point> vertices;
	PointReader pointReader;
	PointPublisher pointPublisher;
public:
	Params params;
	int xRange;
	int yRange;
	Analyser():g(),params(),distance(),
			   unit_distance(distance),learn(),
			   unit_learn(learn), evolution(params),
			   input(),nbTicks(100),
			   xRange(10),yRange(10){
	}

	virtual ~Analyser(){
		if(pointReader.isConnetected())
			pointReader.close();
	}

	// get GNGT generated vertices as points
	std::vector<Point> getVertices(){
		vq2::proba::random::init();
		std::vector<Point> pcl;
		// read until there are some points
		while(pcl.size()==0){
			usleep(5000); // delay for 50ms
			pcl = std::move(pointReader.getPointCloud());
		}
		int tick, e;
		for (tick = 0, e = 0; tick < nbTicks; ++tick) {
			// Let us acquire the input samples for that epoch.
			input.samples = std::move(samplePointCloud(xRange,yRange,params.nbSamples(), pcl));
			for (int per_tick = 0; per_tick < 10; ++per_tick, ++e) {
				std::random_shuffle(input.samples.begin(), input.samples.end());
				vq2::algo::gngt::epoch(params, g, unit_distance, unit_learn,
						evolution, input.samples, sample_of, 1);
			}
		}
		GetVertexFromGraph gp;
		g.for_each_vertex(gp);
		return std::move(gp.vPoints);
	}

	// send vertices "points" to the server
	void readAndPublish(){
		pointPublisher.publish(getVertices());
	}

	// establish the connection of the point reader
	bool connectReader(std::string host, std::string port){
		pointReader.connect(host,port);
		return pointReader.isConnetected();
	}

	// establish the connection of the point publisher
	bool connectPublisher(std::string host, std::string port){
		pointPublisher.connect(host,port);
		return pointReader.isConnetected();
	}

	std::vector<Point> getPoint(){
		return vertices;
	}

	int getNbTicks() const {
		return nbTicks;
	}

	void setNbTicks(int nbTicks) {
		this->nbTicks = nbTicks;
	}
};

