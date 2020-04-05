#include <stdlib.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>

struct Windows_s
{
	GtkWidget *MainWindow;
	GtkWidget *container;

	GtkWidget *dest;
	GtkWidget *dest_label;
	GtkWidget *dest_content;

	GtkWidget *bin;
	GtkWidget *bin_label;
	GtkWidget *bin_content;

	GtkWidget *src;
	GtkWidget *src_label;
	GtkWidget *src_content;

	GtkWidget *exe;
	GtkWidget *exe_label;
	GtkWidget *exe_content;

	GtkWidget *flags;
	GtkWidget *flags_label;
	GtkWidget *flags_content;

	GtkWidget *compiler;
	GtkWidget *compiler_label;
	GtkWidget *compiler_content_c;
	GtkWidget *compiler_content_cpp;

	GtkWidget *clean;
	GtkWidget *clean_label;
	GtkWidget *clean_content;

	GtkWidget *execute;
	GtkWidget *execute_label;
	GtkWidget *execute_content;

	GtkWidget *generate;
};

gboolean on_click_button(GtkWidget *button, GdkEventButton *event, gpointer data)
{
	struct Windows_s *win = (struct Windows_s *)data;
	char *path = (char *)gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(win->dest_content));
	char *bin = (char *)gtk_entry_get_text(GTK_ENTRY(win->bin_content));
	char *src = (char *)gtk_entry_get_text(GTK_ENTRY(win->src_content));
	char *exe = (char *)gtk_entry_get_text(GTK_ENTRY(win->exe_content));
	char *flags = (char *)gtk_entry_get_text(GTK_ENTRY(win->flags_content));
	int gcc = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(win->compiler_content_cpp));
	int clean = gtk_switch_get_active(GTK_SWITCH(win->clean_content));
	int execute = gtk_switch_get_active(GTK_SWITCH(win->execute_content));
	char *path_file = (char *)calloc(strlen(path) + 10, sizeof(char));
	snprintf(path_file, strlen(path) + 10, "%s/Makefile", path);
	FILE *out = fopen(path_file, "w+");
	fprintf(out, "TARGET=%s\n", exe);
	fprintf(out, "BIN=%s\n", bin);
	fprintf(out, "SRC=%s\n", src);
	fprintf(out, "\n");
	fprintf(out, "CC=%s\n", gcc ? "g++" : "gcc");
	fprintf(out, "CFLAGS=%s\n", flags);
	fprintf(out, "SRCS=$(wildcard $(SRC)/*.c)\n");
	fprintf(out, "OBJS=$(SRCS:$(SRC/%%.c=$(BIN)/%%.o)\n");
	fprintf(out, "\n");
	fprintf(out, "\n");
	fprintf(out, "$(BIN)/$(TARGET) : $(OBJS)\n");
	fprintf(out, "\t$(CC) $(OBJS) -o $(BIN)/$(TARGET) $(CFLAGS)\n");
	fprintf(out, "\n");
	fprintf(out, "$(BIN)/%%.o : $(SRC)/%%.c\n");
	fprintf(out, "\t$(CC) -c $< -o $@ $(CFLAGS)\n");
	fprintf(out, "\n");
	if (clean)
	{
		fprintf(out, "clean:\n");
		fprintf(out, "\trm -rf $(BIN)/*.o $(BIN)/$(TARGET)\n");
	}

	if (execute)
	{
		fprintf(out, "exe:\n");
		fprintf(out, "\t./$(BIN)/$(TARGET)\n");
	}
	fclose(out);
	free(path_file);
	return FALSE;
}

int main(int argc, char **argv)
{

	struct Windows_s win;

	/* Initialisation de GTK+ */
	gtk_init(&argc, &argv);

	/* Création de la fenêtre */
	win.MainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(win.MainWindow), "MakeFile generator");
	gtk_window_set_default_size(GTK_WINDOW(win.MainWindow), 300, 200);
	gtk_window_set_position(GTK_WINDOW(win.MainWindow), GTK_WIN_POS_CENTER);
	g_signal_connect(G_OBJECT(win.MainWindow), "delete-event", G_CALLBACK(gtk_main_quit), NULL);

	win.container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

	/**
	 *  
	 */

	//Sources
	win.dest = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	win.dest_label = gtk_label_new("Destination");
	win.dest_content = gtk_file_chooser_button_new(("Select a directory"),
												   GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(win.dest_content),
										".");

	gtk_box_pack_start(GTK_BOX(win.dest), win.dest_label, TRUE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(win.dest), win.dest_content, TRUE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(win.container), win.dest, TRUE, FALSE, 0);

	//endsources

	//binary
	win.bin = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	win.bin_label = gtk_label_new("Binary folder");
	win.bin_content = gtk_entry_new();

	gtk_box_pack_start(GTK_BOX(win.bin), win.bin_label, TRUE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(win.bin), win.bin_content, TRUE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(win.container), win.bin, TRUE, FALSE, 0);

	//endsbinary

	//binary
	win.src = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	win.src_label = gtk_label_new("Sources folder");
	win.src_content = gtk_entry_new();

	gtk_box_pack_start(GTK_BOX(win.src), win.src_label, TRUE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(win.src), win.src_content, TRUE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(win.container), win.src, TRUE, FALSE, 0);

	//endsbinary

	//binary
	win.exe = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	win.exe_label = gtk_label_new("Executable name");
	win.exe_content = gtk_entry_new();

	gtk_box_pack_start(GTK_BOX(win.exe), win.exe_label, TRUE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(win.exe), win.exe_content, TRUE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(win.container), win.exe, TRUE, FALSE, 0);

	//endsbinary

	//binary
	win.flags = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	win.flags_label = gtk_label_new("Flags");
	win.flags_content = gtk_entry_new();

	gtk_box_pack_start(GTK_BOX(win.flags), win.flags_label, TRUE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(win.flags), win.flags_content, TRUE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(win.container), win.flags, TRUE, FALSE, 0);

	//endsbinary

	//compiler
	win.compiler = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	win.compiler_label = gtk_label_new("compiler");
	win.compiler_content_c = gtk_radio_button_new_with_label_from_widget(NULL, "gcc");
	win.compiler_content_cpp = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(win.compiler_content_c), "g++");

	gtk_box_pack_start(GTK_BOX(win.compiler), win.compiler_label, TRUE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(win.compiler), win.compiler_content_c, TRUE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(win.compiler), win.compiler_content_cpp, TRUE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(win.container), win.compiler, TRUE, TRUE, 0);

	//endcompiler

	//clean
	win.clean = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	win.clean_label = gtk_label_new("clean section");
	win.clean_content = gtk_switch_new();

	gtk_box_pack_start(GTK_BOX(win.clean), win.clean_label, TRUE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(win.clean), win.clean_content, TRUE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(win.container), win.clean, TRUE, TRUE, 0);

	//endclean

	//execute
	win.execute = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	win.execute_label = gtk_label_new("execute section");
	win.execute_content = gtk_switch_new();

	gtk_box_pack_start(GTK_BOX(win.execute), win.execute_label, TRUE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(win.execute), win.execute_content, TRUE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(win.container), win.execute, TRUE, TRUE, 0);

	//endexecute

	win.generate = gtk_button_new_with_label("Generate");
	gtk_box_pack_start(GTK_BOX(win.container), win.generate, TRUE, TRUE, 0);

	/* END */

	gtk_container_add(GTK_CONTAINER(win.MainWindow), win.container);

	g_signal_connect(win.generate, "button-press-event", (GCallback)on_click_button, &win);

	/* Affichage et boucle évènementielle */
	gtk_widget_show_all(win.MainWindow);
	gtk_main();

	/* On quitte.. */
	return EXIT_SUCCESS;
}
