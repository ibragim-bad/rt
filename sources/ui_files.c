#include "kiss_sdl.h"
#include "rt.h"

void	button_event3(kiss_button *button, SDL_Event *e, int *draw,
									 int *quit, t_rt *rt)
{
	if (kiss_button_event(button, e, draw))
	{
		if (rt->win_width)
			*quit = 1;
		else
			kiss_error("choose file");
	}
}

void		button_event_exit(kiss_button *button, SDL_Event *e, int *draw,
									 int *quit, t_rt *rt, t_sdl *sdl)
{
	if (kiss_button_event(button, e, draw))
	{
		*quit = 1;
		
		if (rt->win_width)
			free_args(rt->head_shapes, rt->head_light, rt->head_textures);
		SDL_DestroyWindow(sdl->win);
		SDL_Quit();
		exit(1);
	}
}

static void	vscrollbar1_event(kiss_vscrollbar *vscrollbar, SDL_Event *e,
	kiss_textbox *textbox1, int *draw)
{
	int firstline;

	if (kiss_vscrollbar_event(vscrollbar, e, draw) &&
		textbox1->array->length - textbox1->maxlines > 0) {
		firstline = (int) ((textbox1->array->length - 
			textbox1->maxlines) * vscrollbar->fraction + 0.5);
		if (firstline >= 0) textbox1->firstline = firstline;
		*draw = 1;
	}
}

static void textbox1_event(kiss_textbox *textbox1, SDL_Event *e, t_rtui	*ui)
{
	int index;

	if (kiss_textbox_event(textbox1, e, &ui->draw))
	{
		index = ui->textbox1.firstline + ui->textbox1.selectedline;
		if (ft_strcmp((char *)kiss_array_data(ui->textbox1.array, index), ""))
		{
			ui->textbox1.selectedline = -1;
			kiss_string_copy(ui->slash, KISS_MAX_LABEL, ui->buffer, "/");
			kiss_string_copy(ui->file_path, KISS_MAX_LABEL,
				ui->slash, (char *)kiss_array_data(ui->textbox1.array, index));
			kiss_string_copy(ui->label_sel.text, KISS_MAX_LABEL,
				(char *) kiss_array_data(ui->textbox1.array, index), NULL);
			kiss_chdir((char *)kiss_array_data(ui->textbox1.array, index));
			if (ui->file_path[ft_strlen(ui->file_path) - 1] == '/')
				dirent_read(ui);
			ui->draw = 1;
		}
	}
}
/* static void textbox1_event(t_rtui	*ui)
{
	int index;

	if (kiss_textbox_event(&ui->textbox1, &ui->e, &ui->draw))
	{
		index = ui->textbox1.firstline + ui->textbox1.selectedline;
		if (ft_strcmp((char *)kiss_array_data(ui->textbox1.array, index), ""))
		{
			ui->textbox1.selectedline = -1;
			kiss_chdir((char *)kiss_array_data(ui->textbox1.array, index));
			kiss_string_copy(ui->slash,
				KISS_MAX_LABEL,
				ui->buffer, "/");
			kiss_string_copy(ui->file_path,
				KISS_MAX_LABEL,
				ui->slash, (char *)kiss_array_data(ui->textbox1.array,
				index));
			kiss_string_copy(ui->label_sel.text,
				KISS_MAX_LABEL,
				(char *) kiss_array_data(ui->textbox1.array,
				index), NULL);
			if (ui->file_path[ft_strlen(ui->file_path) - 1] == '/')
				dirent_read(ui);
			ui->draw = 1;
		}
	}
} */

static void button_ok2_event(t_rtui *ui)
{
	if (kiss_button_event(&ui->button_ok2, &ui->e, &ui->draw))
	{
		ui->window2.visible = 0;
		ui->window1.focus = 1;
		ui->button_ok2.prelight = 0;
		ui->progressbar.fraction = 0.;
		ui->progressbar.run = 0;
		ui->draw = 1;
	}
}

static void button_ok1_event(t_rtui *ui, t_rt *rt, t_sdl *sdl)
{
	char buf[KISS_MAX_LENGTH];

	if (kiss_button_event(&ui->button_ok1, &ui->e, &ui->draw))
	{
		if (!(ft_strstr(ui->label_sel.text, ".json")))
		{
			kiss_error("should be \".json\"");
			return ;
		}
	/* 		kiss_string_copy(buf, kiss_maxlength(kiss_textfont,
			window2->rect.w - 2 * kiss_vslider.w,
			label_sel->text, entry->text),
			label_sel->text, entry->text); */
		kiss_string_copy(ui->label_res.text, KISS_MAX_LABEL, 
			"Wait\n", ""); 
/* 		window2->visible = 1;
		window2->focus = 1;
		window1->focus = 0;
		button->prelight = 0;
		progressbar->fraction = 0.;
		progressbar->run = 1;
		*draw = 1; */
		init_rt(rt, ui->file_path);
		create_img(rt, sdl);
	}
}

void	ui_drawing(t_rtui *ui)
{

	SDL_RenderClear(ui->renderer);
	kiss_window_draw(&ui->window1, ui->renderer);
	kiss_textbox_draw(&ui->textbox1, ui->renderer);
	kiss_vscrollbar_draw(&ui->vscrollbar1, ui->renderer);
	kiss_label_draw(&ui->label_sel, ui->renderer);
	kiss_button_draw(&ui->button3, ui->renderer);
	kiss_button_draw(&ui->button_ex, ui->renderer);
	kiss_button_draw(&ui->button_ok1, ui->renderer);
	/* kiss_window_draw(&ui->window2, ui->renderer);
	kiss_label_draw(&ui->label_res, ui->renderer);
	kiss_progressbar_draw(&ui->progressbar, ui->renderer);
	kiss_button_draw(&ui->button_ok2, ui->renderer); */
	SDL_RenderPresent(ui->renderer);
	ui->draw = 0;
}

int		ui_main(t_rt *rt, t_sdl *sdl)
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
			kiss_window_event(&ui.window1, &ui.e, &ui.draw);
			//kiss_window_event(&ui.window2, &ui.e, &ui.draw);
			vscrollbar1_event(&ui.vscrollbar1, &ui.e, &ui.textbox1, &ui.draw);
			textbox1_event(&ui.textbox1, &ui.e, &ui);
			button_event3(&ui.button3, &ui.e, &ui.draw, &ui.quit, rt);
			button_event_exit(&ui.button_ex, &ui.e, &ui.draw, &ui.quit, rt, sdl);
 			button_ok1_event(&ui, rt, sdl);
			//button_ok2_event(&ui); 
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
