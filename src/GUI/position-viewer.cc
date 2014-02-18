/*

g++ -o position-viewer position-viewer.cc -Wl,--export-dynamic `pkg-config --cflags --libs gtk+-3.0`

*/

#include <gtk/gtk.h>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <time.h>
#include <boost/format.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "Point.hpp"
#include "PointReader.hpp"

// config filename
static const std::string CONF_FILENAME = "config.xml"; // Configuration filename

// configurable values
static float cell_size_h; // size of tiles in horizontal
static float cell_size_v;
static int cell_num_h = 10; // number of tiles in horizontal
static int cell_num_v = 10;
static float color_coeff = 32.0; // color coefficient for labels

// is connected to the server
static bool isConnected = false;

// Point reader
PointReader pointReader;

// timer handler
static gboolean time_handler(GtkWidget *widget) {
	if (gtk_widget_get_window(widget) == NULL)
		return FALSE;
	if (isConnected){
		gtk_widget_queue_draw(widget); // emit the draw signal
	}
	return TRUE;
}

// draw a point
static void draw_point(cairo_t *cr, const Point& p){
	double e = p.label * color_coeff / 255.0;
	cairo_set_source_rgb(cr,
			1-e,
			0,
			1-e);
	// left handed
//	cairo_arc(cr, p.x * cell_size_v, p.y * cell_size_h, 1, 0, 2 * M_PI);
	// right handed
	cairo_arc(cr, p.y * cell_size_h, p.x * cell_size_v, 4, 0, 2 * M_PI);
	cairo_fill(cr);
}

// draw the point cloud
static void draw_point_cloud(cairo_t *cr, const std::vector<Point>& pointCloud){
	for(Point p: pointCloud){
		draw_point(cr, p);
	}
}

extern "C" {

// on draw event handler
gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer data) {
	// Get the actual size of the drawing area
	GtkAllocation alloc;
	gtk_widget_get_allocation(widget, &alloc);
	cell_size_v = alloc.height / cell_num_v;
	cell_size_h = alloc.width / cell_num_h;
	// Draw tiles
	cairo_set_source_rgb(cr, 0.6, 0.6, 0.8);
	for (int i = 0; i < cell_num_h; ++i)
		for (int j = 0; j < cell_num_v; ++j) {
			cairo_rectangle(cr, i * cell_size_h, j * cell_size_v, cell_size_h,
					cell_size_v);
			cairo_stroke(cr);
		}

	// draw the point cloud
	draw_point_cloud(cr,std::move(pointReader.getPointCloud()));
	return true;
}

// event handler when the user clicks in the drawing area
gboolean on_button_press(GtkWidget *widget, GdkEvent *event,
		gpointer user_data) {
/*	left handed
 *  std::cout << "x: "<< event->motion.x / cell_size_h << std::endl;
	std::cout << "y: "<< event->motion.y / cell_size_v << std::endl;*/

	// right handed
	std::cout << "x: "<<event->motion.y / cell_size_v << std::endl;
	std::cout << "y: "<<event->motion.x / cell_size_h << std::endl;
	return true;
}

// event handler when the mouse moves in the drawing area
gboolean on_motion_notify(GtkWidget *widget, GdkEvent *event,
		gpointer user_data) {
	GtkWidget* label_info = (GtkWidget*) g_object_get_data(G_OBJECT(widget),
			"label_info");
	// format the data
	std::stringstream ss;
	// right handed
	ss << boost::format("x: %.2f y: %.2f")
					% (event->motion.y / cell_size_v)
					% (event->motion.x / cell_size_h);
	gtk_label_set_text((GtkLabel*) label_info, ss.str().c_str());
	return true;
}

// event handler for the connection switch
void on_activate_connection(GtkSwitch* widget, gpointer user_data){
	GtkLabel* label_conn_stat = GTK_LABEL(GTK_WIDGET(g_object_get_data(G_OBJECT(widget),"label_connection_status")));
	GtkEntry* entry_host = GTK_ENTRY(GTK_WIDGET(g_object_get_data(G_OBJECT(widget),"entry_host")));
	GtkEntry* entry_port = GTK_ENTRY(GTK_WIDGET(g_object_get_data(G_OBJECT(widget),"entry_port")));

	std::string host(gtk_entry_get_text(entry_host));
	std::string port(gtk_entry_get_text(entry_port));

	gboolean switchOn = gtk_switch_get_active(widget);
	if(!switchOn){ // switch off
		if(isConnected){
			pointReader.close();
			gtk_label_set_text(label_conn_stat,"Disconnected");
		}else // connection failure
		{
			gtk_label_set_markup(label_conn_stat,
					"Disconnected. <span foreground=\"red\">Err: Cannot establish connection</span>");
		}
		gtk_editable_set_editable(GTK_EDITABLE(entry_host),TRUE);
		gtk_editable_set_editable(GTK_EDITABLE(entry_port),TRUE);
		isConnected = false;
	}else{ // switch on
		isConnected = pointReader.connect(host, port);
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
		// prompt the error when failed to read the config file
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

