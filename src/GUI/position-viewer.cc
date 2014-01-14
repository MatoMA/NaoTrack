/*

g++ -o position-viewer position-viewer.cc -Wl,--export-dynamic `pkg-config --cflags --libs gtk+-3.0`

*/

#include <gtk/gtk.h>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <time.h>
#include <boost/asio.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

typedef boost::asio::ip::tcp tcp;

static const std::string CONF_FILENAME = "config.xml"; // Configuration filename

static float cell_size_h; // size of tiles in horizontal
static float cell_size_v;
static int cell_num_h = 10; // number of tiles in horizontal
static int cell_num_v = 10;
static float color_coeff = 32.0; // color coefficient for labels
static bool isConnected = false;

boost::asio::io_service io_service;
boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;

struct Point{
	int label;
	double x;
	double y;
};

static gboolean time_handler(GtkWidget *widget) {
	if (gtk_widget_get_window(widget) == NULL)
		return FALSE;
	if (isConnected){
		gtk_widget_queue_draw(widget); // emit the draw signal
	}
	return TRUE;
}

static void draw_point(cairo_t *cr, const Point& p){
	double e = p.label * color_coeff / 255.0;
	cairo_set_source_rgb(cr,
			e,
			1 - e,
			e);
	cairo_arc(cr, p.x * cell_size_v, p.y * cell_size_h, 1, 0, 2 * M_PI);
	cairo_fill(cr);
}

static void draw_point_cloud(cairo_t *cr){
	std::string socket_get = "get\n";
	if(isConnected){
		socket_ptr->write_some(boost::asio::buffer(socket_get.c_str(),socket_get.size()));
		size_t len_avail = socket_ptr->available(); // available buffer size
        std::vector<char> data(socket_ptr->available());
        boost::asio::read(*socket_ptr, boost::asio::buffer(data));
        std::istringstream is(std::string(data.begin(),data.end()));
		std::string strLine;
		std::getline(is, strLine);
		int numOfPoints = atoi(strLine.c_str()); // number of points
		// Read points
		Point p;
		boost::char_separator<char> delimiter(" ");
		for (int i = 0; i < numOfPoints; ++i) {
			std::getline(is, strLine);
			// Parse the string line
			boost::tokenizer<boost::char_separator<char>> tok(strLine, delimiter);
			auto t = tok.begin();
			p.label = boost::lexical_cast<int>(*t);
			t++;
			p.x = boost::lexical_cast<double>(*t);
			t++;
			p.y = boost::lexical_cast<double>(*t);
			draw_point(cr, p); // draw point
		}
	}
}

// Establish the connection on socket
static bool socket_connect(std::string host, std::string port){
	try {
		boost::system::error_code error;
		tcp::resolver resolver(io_service);
		tcp::resolver::query query(host,port);
		tcp::resolver::iterator endpoint_it = resolver.resolve(query);
		// Establish socket connection
		socket_ptr.reset(new tcp::socket(io_service));
		socket_ptr->connect(*endpoint_it,error);
		if(error)
			std::cout<<error<<std::endl;
	}catch(std::exception& e){
		std::cout<<e.what()<<std::endl;
		return false;
	}
	if(socket_ptr->is_open())
		return true;
	else
		return false;
}

extern "C" {

gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer data) {
	// Get the actual size of the drawing area
	GtkAllocation alloc;
	gtk_widget_get_allocation(widget, &alloc);
	cell_size_v = alloc.height / cell_num_v;
	cell_size_h = alloc.width / cell_num_h;
	// Draw tiles
	cairo_set_source_rgb(cr, 0.6, 0.6, 0.8);
	for (int i = 0; i < 10; ++i)
		for (int j = 0; j < 10; ++j) {
			cairo_rectangle(cr, i * cell_size_h, j * cell_size_v, cell_size_h,
					cell_size_v);
			cairo_stroke(cr);
		}
	draw_point_cloud(cr);
	return true;
}

gboolean on_button_press(GtkWidget *widget, GdkEvent *event,
		gpointer user_data) {
	std::cout << event->motion.x / cell_size_h << std::endl;
	std::cout << event->motion.y / cell_size_v << std::endl;
	return true;
}

gboolean on_motion_notify(GtkWidget *widget, GdkEvent *event,
		gpointer user_data) {
	GtkWidget* label_info = (GtkWidget*) g_object_get_data(G_OBJECT(widget),
			"label_info");
	// format the data
	std::stringstream ss;
	ss << boost::format("x: %.2f y: %.2f")
					% (event->motion.x / cell_size_h)
					% (event->motion.y / cell_size_v);
	gtk_label_set_text((GtkLabel*) label_info, ss.str().c_str());
	return true;
}

void on_activate_connection(GtkSwitch* widget, gpointer user_data){
	GtkLabel* label_conn_stat = GTK_LABEL(GTK_WIDGET(g_object_get_data(G_OBJECT(widget),"label_connection_status")));
	GtkEntry* entry_host = GTK_ENTRY(GTK_WIDGET(g_object_get_data(G_OBJECT(widget),"entry_host")));
	GtkEntry* entry_port = GTK_ENTRY(GTK_WIDGET(g_object_get_data(G_OBJECT(widget),"entry_port")));

	std::string host(gtk_entry_get_text(entry_host));
	std::string port(gtk_entry_get_text(entry_port));

	gboolean switchOn = gtk_switch_get_active(widget);
	if(!switchOn){
		if(isConnected){
			socket_ptr->close();
			gtk_label_set_text(label_conn_stat,"Disconnected");
		}else // connection failure
		{
			gtk_label_set_markup(label_conn_stat,
					"Disconnected. <span foreground=\"red\">Err: Cannot establish connection</span>");
		}
		gtk_editable_set_editable(GTK_EDITABLE(entry_host),TRUE);
		gtk_editable_set_editable(GTK_EDITABLE(entry_port),TRUE);
		isConnected = false;
	}else{
		isConnected = socket_connect(host,port);
		if(!isConnected){
			gtk_switch_set_active(widget, FALSE);
		}else{
			gtk_label_set_text(label_conn_stat,"Connected");
			gtk_editable_set_editable(GTK_EDITABLE(entry_host),FALSE);
			gtk_editable_set_editable(GTK_EDITABLE(entry_port),FALSE);
		}
	}
}

}

/**
 * MAIN
 */
int main(int argc, char* argv[]) {
	// GTK init
	GtkBuilder * p_builder = NULL;
	GError * p_err = NULL;
	gtk_init(&argc, &argv);
	p_builder = gtk_builder_new();

	if (p_builder != NULL)
		gtk_builder_add_from_file(p_builder, "viewer.glade", &p_err);
	else
		return EXIT_FAILURE;

	if (p_err != NULL) {
		std::cerr << p_err->message << std::endl;
		g_error_free(p_err);
		return EXIT_FAILURE;
	}

	/**
	 * Widget: window
	 */
	GtkWidget * window = GTK_WIDGET(
			gtk_builder_get_object(p_builder, "window"));
	gtk_builder_connect_signals(p_builder, (gpointer) p_builder); // le champ data de chaque calback sera le builder.
	g_signal_connect_swapped(G_OBJECT(window), "destroy",G_CALLBACK(gtk_main_quit), NULL);

	// Load config
	boost::property_tree::ptree pt;
	try{
		boost::property_tree::xml_parser::read_xml(CONF_FILENAME,pt);
		cell_num_h = pt.get<int>("config.cell_number_horizontal");
		cell_num_v = pt.get<int>("config.cell_number_vertical");
		color_coeff = pt.get<float>("config.color_coefficient");
	}catch(boost::property_tree::xml_parser::xml_parser_error& e){
		std::cout << e.what() << std::endl;
		GtkWidget * dialog;
		dialog = gtk_message_dialog_new(GTK_WINDOW(window),
	            GTK_DIALOG_DESTROY_WITH_PARENT,
	            GTK_MESSAGE_ERROR,
	            GTK_BUTTONS_OK,
	            "Error loading configuration file");
		gtk_window_set_title(GTK_WINDOW(dialog), "Error");
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
		return EXIT_FAILURE;
	}

	/**
	 * Widget: drawing area
	 */
	GtkWidget * drawing = GTK_WIDGET(gtk_builder_get_object(p_builder,"drawing_area"));
	g_timeout_add(500, (GSourceFunc) time_handler, (gpointer) window); // Register a timer called every n ms
	time_handler(window); // Call the callback immediately
	gtk_widget_add_events(drawing, GDK_BUTTON_PRESS_MASK|GDK_POINTER_MOTION_MASK);
	g_signal_connect(G_OBJECT(drawing), "button-press-event", G_CALLBACK(on_button_press),NULL);
	g_signal_connect(G_OBJECT(drawing), "motion-notify-event", G_CALLBACK(on_motion_notify), NULL);

	/**
	 * Widget: Labels
	 */
	GtkWidget* label_info = GTK_WIDGET(gtk_builder_get_object(p_builder,"label_info"));
	gtk_label_set_text(GTK_LABEL(label_info),"info");
	g_object_set_data(G_OBJECT(drawing),"label_info",label_info);

	GtkWidget* label_connection = GTK_WIDGET(gtk_builder_get_object(p_builder,"label_connection"));
	GtkWidget* label_conn_stat = GTK_WIDGET(gtk_builder_get_object(p_builder,"label_connection_status"));

	/**
	 * Widget: Entries
	 */
	GtkWidget* entry_host = GTK_WIDGET(gtk_builder_get_object(p_builder,"entry_host"));
	GtkWidget* entry_port = GTK_WIDGET(gtk_builder_get_object(p_builder,"entry_port"));
	gtk_entry_set_text(GTK_ENTRY(entry_host),pt.get<std::string>("config.default_hostname").c_str());
	gtk_entry_set_text(GTK_ENTRY(entry_port),pt.get<std::string>("config.default_port").c_str());

	/**
	 * Widget: Switch connection
	 */
	GtkWidget* switch_connection = GTK_WIDGET(gtk_builder_get_object(p_builder,"switch_connection"));
	g_signal_connect(G_OBJECT(switch_connection), "notify::active", G_CALLBACK(on_activate_connection), NULL);
	g_object_set_data(G_OBJECT(switch_connection),"label_connection_status",label_conn_stat);
	g_object_set_data(G_OBJECT(switch_connection),"entry_host",entry_host);
	g_object_set_data(G_OBJECT(switch_connection),"entry_port",entry_port);

	gtk_widget_show_all(window);
	gtk_main();
	g_object_unref(G_OBJECT(p_builder));

	return EXIT_SUCCESS;
}

