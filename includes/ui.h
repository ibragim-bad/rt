#ifndef UI_H
# define UI_H

typedef struct		s_rtui_min
{
	SDL_Renderer	*renderer;
	SDL_Event		e;
	kiss_array		objects;
	kiss_window		window;
	kiss_label 		label;
	kiss_button		button;
	kiss_entry		en_x;
	kiss_entry		en_y;
	kiss_entry		en_z;
	kiss_entry		en_intens;
	kiss_label		label_x;
	kiss_label		label_y;
	kiss_label		label_z;
	kiss_label		label_intens;
	t_vec3			pos_light;
	char 			message[KISS_MAX_LENGTH];
	int				draw;
	int				quit;
}					t_rtui_min;


typedef struct		s_rtui
{
	SDL_Renderer	*renderer;
	SDL_Event		e;
	kiss_array		objects;
	kiss_array		a1;
	kiss_window		window1;
	kiss_window		window2;
	kiss_window		window3;
	char			buffer[KISS_MAX_LABEL];
	char			slash[KISS_MAX_LABEL];
	char			file_path[KISS_MAX_LABEL];
	int				draw;
	int				quit;
	int				textbox_width;
	int				textbox_height;
	int				window2_width;
	int				window2_height;
	int				window3_width;
	int				window3_height;
	kiss_label		label;
	kiss_label		label3;
	kiss_label		label_buf;
	kiss_label		label_sel;
	kiss_label		label_res;
	kiss_button 	button3;
	kiss_button		button_light;
	kiss_button		button_light_ok;
	kiss_button 	button_ex;
	kiss_button 	button_ok1;
	kiss_button 	button_ok2;
	kiss_vscrollbar vscrollbar1;
	//kiss_progressbar progressbar;
	kiss_textbox	textbox1;
}					t_rtui;

#endif
