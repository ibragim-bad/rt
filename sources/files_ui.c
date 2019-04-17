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
                  int *quit, t_rt *rt, t_sdl *sdl)
{
    if (kiss_button_event(button, e, draw))
    {
		*quit = 1;
	}
}

int files_ui(t_rt *rt, t_sdl *sdl)
{
    SDL_Renderer *renderer;
    SDL_Event e;
    kiss_array objects;
    kiss_window window;
    kiss_label label = {0};
    kiss_button button = {0};
    kiss_button button2 = {0};
    kiss_button button3 = {0};
    char message[KISS_MAX_LENGTH];
    int draw, quit;
    quit = 0;
    draw = 1;
    kiss_array_new(&objects);
    renderer = kiss_init("RT", &objects, 300, 600);
    if (!renderer)
        return 1;
    kiss_window_new(&window, NULL, 0, 0, 0, kiss_screen_width,
                    kiss_screen_height);
    strcpy(message, "Switch to!");
    kiss_label_new(&label, &window, message,
                   window.rect.w / 2 - strlen(message) *
                                           kiss_textfont.advance / 2,
                   window.rect.h / 2 - (kiss_textfont.fontheight + 2 * kiss_normal.h) / 2);
    label.textcolor.r = 255;
    kiss_button_new(&button, &window, "1 sphere",
                    window.rect.w / 2 - kiss_normal.w / 2 - 20, label.rect.y + kiss_textfont.fontheight + kiss_normal.h);
    kiss_button_new(&button2, &window, "2 spheres",
                    window.rect.w / 2 - kiss_normal.w / 2 - 20, label.rect.y - 50 + kiss_textfont.fontheight + kiss_normal.h);
    kiss_button_new(&button3, &window, "hide menu",
                    window.rect.w / 2 - kiss_normal.w / 2 - 20, label.rect.y + 100 + kiss_textfont.fontheight + kiss_normal.h);
    window.visible = 1;
    while (!quit)
    {
        SDL_Delay(10);
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                quit = 1;
            kiss_window_event(&window, &e, &draw);
            button_event(&button, &e, &draw, &quit, rt, sdl);
            button_event2(&button2, &e, &draw, &quit, rt, sdl);
            button_event3(&button3, &e, &draw, &quit, rt, sdl);
        }
        if (!draw)
            continue;
        SDL_RenderClear(renderer);
        kiss_window_draw(&window, renderer);
        kiss_label_draw(&label, renderer);
        kiss_button_draw(&button, renderer);
        kiss_button_draw(&button2, renderer);
        kiss_button_draw(&button3, renderer);
        SDL_RenderPresent(renderer);
        draw = 0;
    }
    kiss_clean(&objects);
/*  free_args(rt->head_shapes, rt->head_light, rt->head_textures);
	SDL_DestroyWindow(sdl->win);
	SDL_Quit();  */
    return 0;
}
