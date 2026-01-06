#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "./constants.h"

int game_is_running = FALSE;
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;

struct ball
{
    float x;
    float y;
    float width;
    float height;
} ball;

int initialize_window(void)
{
    if (!al_init())
    {
        fprintf(stderr, "Error initializing Allegro\n");
        return FALSE;
    }

    if (!al_install_keyboard())
    {
        fprintf(stderr, "Error installing keyboard\n");
        return FALSE;
    }

    if (!al_init_primitives_addon())
    {
        fprintf(stderr, "Error initializing primitives addon\n");
        return FALSE;
    }

    display = al_create_display(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!display)
    {
        fprintf(stderr, "Error creating display\n");
        return FALSE;
    }

    al_set_window_title(display, "Game Loop - Allegro");

    timer = al_create_timer(FRAME_TARGET_TIME);
    if (!timer)
    {
        fprintf(stderr, "Error creating timer\n");
        al_destroy_display(display);
        return FALSE;
    }

    event_queue = al_create_event_queue();
    if (!event_queue)
    {
        fprintf(stderr, "Error creating event queue\n");
        al_destroy_timer(timer);
        al_destroy_display(display);
        return FALSE;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    fprintf(stderr, "initialize_window: OK\n");
    return TRUE;
}

void setup()
{
    ball.x = 20;
    ball.y = 20;
    ball.width = 15;
    ball.height = 15;
}

void process_input()
{
    ALLEGRO_EVENT event;
    while (al_get_next_event(event_queue, &event))
    {
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            game_is_running = FALSE;
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            {
                game_is_running = FALSE;
            }
        }
    }
}

void update()
{
    static double last_time = 0;
    double current_time = al_get_time();
    float delta_time = (float)(current_time - last_time);
    
    if (last_time == 0)
        delta_time = FRAME_TARGET_TIME;
    
    last_time = current_time;

    ball.x += 70 * delta_time;
    ball.y += 50 * delta_time;
}

void render()
{
    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_draw_filled_rectangle(
        ball.x,
        ball.y,
        ball.x + ball.width,
        ball.y + ball.height,
        al_map_rgb(255, 255, 255));

    al_flip_display();
}

void destroy_window()
{
    if (timer)
        al_destroy_timer(timer);
    if (event_queue)
        al_destroy_event_queue(event_queue);
    if (display)
        al_destroy_display(display);
}

int main()
{
    game_is_running = initialize_window();

    if (!game_is_running)
    {
        return 1;
    }

    setup();
    fprintf(stderr, "main: entering loop\n");

    al_start_timer(timer);

    while (game_is_running)
    {
        process_input();
        update();
        render();
        
        al_rest(0.001); // Small rest to prevent CPU spinning
    }
    
    fprintf(stderr, "main: exiting loop\n");

    destroy_window();

    return 0;
}