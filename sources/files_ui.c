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

static void vscrollbar1_event(kiss_vscrollbar *vscrollbar, SDL_Event *e,
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
static void dirent_read(kiss_textbox *textbox1, kiss_vscrollbar *vscrollbar1)
{
	kiss_dirent *ent;
	kiss_stat s;
	kiss_dir *dir;
	char buf[KISS_MAX_LENGTH], ending[2];

	//kiss_array_free(textbox1->array);
	kiss_array_new(textbox1->array);
	kiss_getcwd(buf, KISS_MAX_LENGTH);
	strcpy(ending, "/");
	//if (buf[0] == 'C') strcpy(ending, "\\");
	//if (!strcmp(buf, "/") || !strcmp(buf, "C:\\")) strcpy(ending, "");
/* 	kiss_string_copy(label_sel->text, (2 * textbox1->rect.w +
		2 * kiss_up.w) / kiss_textfont.advance, buf, ending); */
	dir = kiss_opendir(".");
	while ((ent = kiss_readdir(dir))) {
		if (!ent->d_name) continue;
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


int files_ui(t_rt *rt, t_sdl *sdl)
{
    SDL_Renderer *renderer;
    SDL_Event e;
    kiss_array objects;
    kiss_window window;
    kiss_label label = {0}, label1 = {0};
    kiss_textbox textbox1 = {0};
    kiss_button button = {0};
    kiss_button button2 = {0};
    kiss_button button3 = {0};
    kiss_vscrollbar vscrollbar1 = {0};
    int textbox_width = 250;
	int textbox_height = 250;
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
    kiss_label_new(&label1, &window, "Files", textbox1.rect.x +
		kiss_edge, textbox1.rect.y - kiss_textfont.lineheight);
    label.textcolor.r = 255;

    kiss_textbox_new(&textbox1, &window, 1, &objects, 30,
		3 * kiss_normal.h, textbox_width, textbox_height);

    kiss_vscrollbar_new(&vscrollbar1, &window, textbox1.rect.x +
		textbox_width, textbox1.rect.y, textbox_height);
    
    kiss_button_new(&button, &window, "1 sphr",
                    window.rect.w / 2 - kiss_normal.w / 2 - 20, label.rect.y +50+ kiss_textfont.fontheight + kiss_normal.h);
    kiss_button_new(&button2, &window, "2 sphr",
                    window.rect.w / 2 - kiss_normal.w / 2 - 20, label.rect.y +100 + kiss_textfont.fontheight + kiss_normal.h);
    kiss_button_new(&button3, &window, "hide ",
                    window.rect.w / 2 - kiss_normal.w / 2 - 20, label.rect.y + 150 + kiss_textfont.fontheight + kiss_normal.h);
    window.visible = 1;
    dirent_read(&textbox1, &vscrollbar1);
    while (!quit)
    {
        SDL_Delay(10);
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                quit = 1;
            kiss_window_event(&window, &e, &draw);
            vscrollbar1_event(&vscrollbar1, &e, &textbox1,
				&draw);
            button_event(&button, &e, &draw, &quit, rt, sdl);
            button_event2(&button2, &e, &draw, &quit, rt, sdl);
            button_event3(&button3, &e, &draw, &quit, rt, sdl);
        }

        vscrollbar1_event(&vscrollbar1, NULL, &textbox1, &draw);


        if (!draw)
            continue;
        SDL_RenderClear(renderer);
        kiss_window_draw(&window, renderer);
        kiss_label_draw(&label, renderer);
        kiss_textbox_draw(&textbox1, renderer);
        kiss_vscrollbar_draw(&vscrollbar1, renderer);
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
