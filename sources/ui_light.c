/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_sup.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfrankly <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/18 19:05:30 by jfrankly          #+#    #+#             */
/*   Updated: 2019/04/18 19:05:31 by jfrankly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int	light_params_read(t_rtui_min *min_light)
{
	return (0);
} 

void button_event_qlt(t_rtui_min *min_light)
{
	if (kiss_button_event(&min_light->button, &min_light->e, &min_light->draw))
		{
			if (light_params_read(min_light))
				kiss_error("parameters are wrong!");
			min_light->quit = 1;
		}
}

void ui_min_light_init(t_rtui_min *min_light)
{
	min_light->quit = 0;
	min_light->draw = 1;
	kiss_array_new(&min_light->objects);
	min_light->renderer = kiss_init("light", &min_light->objects, 300, 400);
	if (!min_light->renderer)
		return;
	kiss_window_new(&min_light->window, NULL, 0, 0, 0, kiss_screen_width,
					kiss_screen_height);
	kiss_label_new(&min_light->label_x, &min_light->window, "enter x", 10, 5);
	kiss_label_new(&min_light->label_y, &min_light->window, "enter y", 10, 80);
	kiss_label_new(&min_light->label_z, &min_light->window, "enter z", 10, 155);
	kiss_label_new(&min_light->label_intens, &min_light->window, "enter intensity", 8, 230);
	kiss_entry_new(&min_light->en_x, &min_light->window, 1, "0", 10, 40, 280);
	kiss_entry_new(&min_light->en_y, &min_light->window, 1, "1", 10, 115, 280);
	kiss_entry_new(&min_light->en_z, &min_light->window, 1, "2", 10, 190, 280);
	kiss_entry_new(&min_light->en_intens, &min_light->window, 1, "20", 10, 265, 280);
	/*kiss_label_new(&min_light->label, &min_light->window, min_light->message, \
		min_light->window.rect.w / 2 - strlen(min_light->message) * \
		kiss_textfont.advance / 2, \
		min_light->window.rect.h / 2 - (kiss_textfont.fontheight\
		+ 2 * kiss_normal.h) / 2);
	min_light->label.textcolor.r = 255;*/
	kiss_button_new(&min_light->button, &min_light->window, "OK",
					min_light->window.rect.w / 2 - kiss_normal.w / 2, 350);
	min_light->window.visible = 1;
}

void kiss_light_draw(t_rtui_min *min_light)
{
	SDL_RenderClear(min_light->renderer);
	kiss_window_draw(&min_light->window, min_light->renderer);
	//kiss_label_draw(&min_light->label, min_light->renderer);
	kiss_label_draw(&min_light->label_x, min_light->renderer);
	kiss_label_draw(&min_light->label_y, min_light->renderer);
	kiss_label_draw(&min_light->label_z, min_light->renderer);
	kiss_label_draw(&min_light->label_intens, min_light->renderer);
	kiss_button_draw(&min_light->button, min_light->renderer);
	kiss_entry_draw(&min_light->en_x, min_light->renderer);
	kiss_entry_draw(&min_light->en_y, min_light->renderer);
	kiss_entry_draw(&min_light->en_z, min_light->renderer);
	kiss_entry_draw(&min_light->en_intens, min_light->renderer);
	kiss_button_draw(&min_light->button, min_light->renderer);
	SDL_RenderPresent(min_light->renderer);
	min_light->draw = 0;
}

int kiss_light(t_rt *rt)
{
	t_rtui_min min_light;

	//ft_strcpy(min_light_light.message, mes);
	ui_min_light_init(&min_light);
	while (!min_light.quit)
	{
		SDL_Delay(10);
		while (SDL_PollEvent(&min_light.e))
		{
			if (min_light.e.type == SDL_QUIT)
				min_light.quit = 1;
			kiss_window_event(&min_light.window, &min_light.e, &min_light.draw);
			kiss_entry_event(&min_light.en_x, &min_light.e, &min_light.draw);
			kiss_entry_event(&min_light.en_y, &min_light.e, &min_light.draw);
			kiss_entry_event(&min_light.en_z, &min_light.e, &min_light.draw);
			kiss_entry_event(&min_light.en_intens, &min_light.e, &min_light.draw);
			button_event_qlt(&min_light);
		}
		if (!min_light.draw)
			continue;
		kiss_light_draw(&min_light);
	}
	kiss_clean(&min_light.objects);
	return (0);
}
