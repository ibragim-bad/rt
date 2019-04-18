#include "kiss_sdl.h"
#include "rt.h"

void button_event(kiss_button *button, SDL_Event *e, int *draw,
									int *quit, t_rt *rt, t_sdl *sdl)
{
	if (kiss_button_event(button, e, draw))
	{
		//*quit = 1;
		init_rt(rt, "parser/sph2.json");
		create_img(rt, sdl);
	}
}

void button_event2(kiss_button *button, SDL_Event *e, int *draw,
									 int *quit, t_rt *rt, t_sdl *sdl)
{
	if (kiss_button_event(button, e, draw))
	{
		//*quit = 1;
		init_rt(rt, "parser/sphere.json");
		create_img(rt, sdl);
	}
}

void button_event3(kiss_button *button, SDL_Event *e, int *draw,
									 int *quit)
{
	if (kiss_button_event(button, e, draw))
	{
		*quit = 1;
	}
}

static void vscrollbar1_event(kiss_vscrollbar *vscrollbar, SDL_Event *e,
															kiss_textbox *textbox1, int *draw)
{
	int firstline;

	if (kiss_vscrollbar_event(vscrollbar, e, draw) &&
			textbox1->array->length - textbox1->maxlines > 0)
	{
		firstline = (int)((textbox1->array->length -
											 textbox1->maxlines) *
													vscrollbar->fraction +
											0.5);
		if (firstline >= 0)
			textbox1->firstline = firstline;
		*draw = 1;
	}
}

static void text_reset(kiss_textbox *textbox, kiss_vscrollbar *vscrollbar)
{
	qsort(textbox->array->data, textbox->array->length, sizeof(void *),
				kiss_string_compare);
	vscrollbar->step = 0.;
	if (textbox->array->length - textbox->maxlines > 0)
		vscrollbar->step = 1. / (textbox->array->length -
														 textbox->maxlines);
	textbox->firstline = 0;
	textbox->highlightline = -1;
	vscrollbar->fraction = 0.;
}

/* Read directory entries into the textboxes */
static void dirent_read(kiss_textbox *textbox1, kiss_vscrollbar *vscrollbar1, kiss_label *label_sel, t_rtui *ui)
{
	kiss_dirent *ent;
	kiss_stat s;
	kiss_dir *dir;
	char ending[2];

	//kiss_array_free(textbox1->array);
	kiss_array_new(textbox1->array);
	kiss_getcwd(ui->buffer, KISS_MAX_LENGTH);
	strcpy(ending, "/");
	if (ui->buffer[0] == 'C') strcpy(ending, "\\");
	if (!strcmp(ui->buffer, "/") || !strcmp(ui->buffer, "C:\\")) strcpy(ending, "");
 //	kiss_string_copy(label_sel->text, (2 * textbox1->rect.w +
//		2 * kiss_up.w) / kiss_textfont.advance, ui->buffer, ending); 
	dir = kiss_opendir(".");
	//printf("buffer - %s\n", ui->buffer);
	while ((ent = kiss_readdir(dir)))
	{
		if (!ent->d_name)
			continue;
		kiss_getstat(ent->d_name, &s);
		if (kiss_isdir(s))
			kiss_array_appendstring(textbox1->array, 0,
															ent->d_name, "/");
		if (kiss_isreg(s))
			kiss_array_appendstring(textbox1->array, 0,
															ent->d_name, NULL);
	}
	kiss_closedir(dir);
	text_reset(textbox1, vscrollbar1);
}

static void textbox1_event(kiss_textbox *textbox, SDL_Event *e,
													 kiss_vscrollbar *vscrollbar1,kiss_label *label_sel, int *draw, t_rtui	*ui)
{
	int index;

	if (kiss_textbox_event(textbox, e, draw))
	{
		index = textbox->firstline + textbox->selectedline;
		if (strcmp((char *)kiss_array_data(textbox->array, index), ""))
		{
			textbox->selectedline = -1;
			kiss_chdir((char *)kiss_array_data(textbox->array, index));
			dirent_read(textbox, vscrollbar1, label_sel, ui);
			*draw = 1;

			kiss_string_copy(ui->slash,
				KISS_MAX_LABEL,
				ui->buffer, "/");
			kiss_string_copy(ui->file_path,
				KISS_MAX_LABEL,
				ui->slash, (char *)kiss_array_data(textbox->array,
				index));
			kiss_string_copy(label_sel->text,
				KISS_MAX_LABEL,
				(char *) kiss_array_data(textbox->array,
				index), NULL);
			
		}
	}
}

static void button_ok2_event(kiss_button *button, SDL_Event *e,
														 kiss_window *window1, kiss_window *window2,
														 kiss_progressbar *progressbar, int *draw)
{
	if (kiss_button_event(button, e, draw))
	{
		window2->visible = 0;
		window1->focus = 1;
		button->prelight = 0;
		progressbar->fraction = 0.;
		progressbar->run = 0;
		*draw = 1;
	}
}

static void button_ok1_event(kiss_button *button, SDL_Event *e,
														 kiss_window *window1, kiss_window *window2,
														 kiss_progressbar *progressbar, kiss_label *label_sel, kiss_label *label_res, int *draw, t_rtui *ui, t_rt *rt, t_sdl *sdl)
{
	char buf[KISS_MAX_LENGTH];

	if (kiss_button_event(button, e, draw))
	{
	/* 		kiss_string_copy(buf, kiss_maxlength(kiss_textfont,
			window2->rect.w - 2 * kiss_vslider.w,
			label_sel->text, entry->text),
			label_sel->text, entry->text); */
		kiss_string_copy(label_res->text, KISS_MAX_LABEL, 
			"Wait\n", ""); 
		window2->visible = 1;
		window2->focus = 1;
		window1->focus = 0;
		button->prelight = 0;
		progressbar->fraction = 0.;
		progressbar->run = 1;
		*draw = 1;
		init_rt(rt, ui->file_path);
		create_img(rt, sdl);
	}
}

void	ui_drawing(t_rtui *ui)
{

		SDL_RenderClear(ui->renderer);

		kiss_window_draw(&ui->window1, ui->renderer);
		kiss_label_draw(&ui->label1, ui->renderer);
		kiss_textbox_draw(&ui->textbox1, ui->renderer);
		kiss_vscrollbar_draw(&ui->vscrollbar1, ui->renderer);
		kiss_label_draw(&ui->label_sel, ui->renderer);
		kiss_button_draw(&ui->button3, ui->renderer);
		kiss_button_draw(&ui->button_ok1, ui->renderer);


		kiss_window_draw(&ui->window2, ui->renderer);
		kiss_label_draw(&ui->label_res, ui->renderer);
		kiss_progressbar_draw(&ui->progressbar, ui->renderer);
		kiss_button_draw(&ui->button_ok2, ui->renderer);

		SDL_RenderPresent(ui->renderer);
		ui->draw = 0;
}

void	ui_init(t_rtui *ui)
{
	ui->textbox_width = 250;
	ui->textbox_height = 250;
	ui->window2_width = 200;
	ui->window2_height = 168;
	ui->quit = 0;
	ui->draw = 1;
	kiss_array_new(&ui->objects);
	ui->renderer = kiss_init("RT", &ui->objects, 300, 600);
	if (!ui->renderer)
		return ;
	kiss_window_new(&ui->window1, NULL, 0, 0, 0, kiss_screen_width,
									kiss_screen_height);
	kiss_label_new(&ui->label1, &ui->window1, "Files", ui->textbox1.rect.x + kiss_edge, \
									ui->textbox1.rect.y - kiss_textfont.lineheight);
	ui->label.textcolor.r = 255;
		kiss_textbox_new(&ui->textbox1, &ui->window1, 1, &ui->objects, 30,
									 3 * kiss_normal.h, ui->textbox_width, ui->textbox_height);

	kiss_vscrollbar_new(&ui->vscrollbar1, &ui->window1, ui->textbox1.rect.x + ui->textbox_width, ui->textbox1.rect.y, ui->textbox_height);

	kiss_label_new(&ui->label_sel, &ui->window1, "Choose file", ui->textbox1.rect.x + kiss_edge, ui->textbox1.rect.y + ui->textbox_height + kiss_normal.h);
	kiss_button_new(&ui->button3, &ui->window1, "deactive",
									ui->window1.rect.w / 2 - kiss_normal.w / 2 - 20, 500);
	kiss_button_new(&ui->button_ok1, &ui->window1, "render",
									ui->window1.rect.w / 2 - kiss_normal.w / 2 - 20, 550);
	kiss_window_new(&ui->window2, NULL, 1, kiss_screen_width / 2 - ui->window2_width / 2, kiss_screen_height / 2 - ui->window2_height / 2, ui->window2_width, ui->window2_height);
	kiss_label_new(&ui->label_res, &ui->window2, "", ui->window2.rect.x +
		kiss_up.w, ui->window2.rect.y + kiss_vslider.h);
	ui->label_res.textcolor = kiss_blue;
	kiss_progressbar_new(&ui->progressbar, &ui->window2, ui->window2.rect.x + kiss_up.w - kiss_edge, ui->window2.rect.y + ui->window2.rect.h / 2 - kiss_bar.h / 2 - kiss_border, 160);
	kiss_button_new(&ui->button_ok2, &ui->window2, "OK", ui->window2.rect.x + ui->window2.rect.w / 2 - kiss_normal.w / 2,
									ui->progressbar.rect.y + ui->progressbar.rect.h +
											2 * kiss_vslider.h);
	dirent_read(&ui->textbox1, &ui->vscrollbar1, &ui->label_sel, ui);
	ui->window1.visible = 1;
}

int files_ui(t_rt *rt, t_sdl *sdl)
{
	t_rtui	ui;

	ui_init(&ui);
	while (!ui.quit)
	{
		SDL_Delay(10);
		while (SDL_PollEvent(&ui.e))
		{
			if (ui.e.type == SDL_QUIT)
				ui.quit = 1;
			kiss_window_event(&ui.window2, &ui.e, &ui.draw);
			kiss_window_event(&ui.window1, &ui.e, &ui.draw);
			vscrollbar1_event(&ui.vscrollbar1, &ui.e, &ui.textbox1,
												&ui.draw);
			textbox1_event(&ui.textbox1, &ui.e, &ui.vscrollbar1, &ui.label_sel, &ui.draw, &ui);
			button_event3(&ui.button3, &ui.e, &ui.draw, &ui.quit);
			button_ok1_event(&ui.button_ok1, &ui.e, &ui.window1, &ui.window2, &ui.progressbar,
											 &ui.label_sel, &ui.label_res, &ui.draw, &ui, rt, sdl);
			button_ok2_event(&ui.button_ok2, &ui.e, &ui.window1, &ui.window2,
											 &ui.progressbar, &ui.draw);
		}
		
		vscrollbar1_event(&ui.vscrollbar1, NULL, &ui.textbox1, &ui.draw);
		kiss_progressbar_event(&ui.progressbar, NULL, &ui.draw);
		if (!ui.draw)
			continue;
		ui_drawing(&ui);
	}
	kiss_clean(&ui.objects);
	return 0;
}
