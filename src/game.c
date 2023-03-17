#include <SDL.h>
#define NK_IMPLEMENTATION
#define NK_SDL_RENDERER_IMPLEMENTATION
#include "simple_logger.h"
#include "simple_json.h"
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"

#include "level.h"
#include "camera.h"

#include "entity.h"
#include "space_bug.h"

#include "world.h"
#include "battle.h"

enum
{
    DELAY,
    ALWAYS,
    NEVER
};

int fullscreen_gui;

#define TRUE 1
#define FALSE 0

Inventory inventory =
    {
        2,
        {{"Heal", -30, -30}, {"Fire", 20, 30}}

};

int main(int argc, char *argv[])
{
    Character Player = battle_load_character("player_data/player.json");
    Character Enemy = battle_load_character("default_enemy/enemy0.json");
    battle_save_data_character("player_data/player.json", &Player);

    slog(Player.attacks[6].name);

    /*variable declarations*/
    int done = 0;
    Level *level;
    const Uint8 *keys;
    Entity *ent;

    int pause = 0;
    Sprite *background;
    // gme_entity_init(1024);
    entity_manager_init(1024);

    int battle_now = 0;

    /*mouse vars*/
    int mx, my;
    float mf = 0;
    Sprite *mouse;
    Color mouseColor = gfc_color8(255, 153, 0, 150);

    /*program initializtion*/
    init_logger("gf2d.log", 0);
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "game",
        1200,
        720,
        1200,
        720,
        vector4d(0, 0, 0, 255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    // gme_entity_manager_init(1024);
    SDL_ShowCursor(SDL_DISABLE);

    struct nk_context *ctx = gf2d_nuklear_init();
    SDL_Event evt;

    /*demo setup*/
    background = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    mouse = gf2d_sprite_load_all("images/pointer.png", 32, 32, 16, 0);
    ent = space_bug_new(vector2d(100, 100));
    level = level_load("config/test.level");
    level_set_active_level(level);

    // TODO in a loop read in json (or whatever) and load all sprites
    // arrays of players (if you have more than one player) and enemys

    // exent = entity_new();
    // exent->sprite = gf2d_sprite_load_all(
    //     "images/space_bug.png",
    //     128,
    //     128,
    //     16,
    //     0);

    battle_now = TRUE;
    while (!done)
    {

        nk_input_begin(ctx);
        while (SDL_PollEvent(&evt))
        {
            if (evt.type == SDL_QUIT)
            {
                done = 1;
                break;
            }
            nk_sdl_handle_event(&evt);
        }
        nk_input_end(ctx);

        SDL_PumpEvents(); // update SDL's internal event structures
        /*handle keyboard and mouse events here*/
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx, &my);
        mf += 0.1;
        if (mf >= 16.0)
            mf = 0;
        //
        entity_think_all();
        entity_update_all();
        camera_world_snap();
        //
        gf2d_graphics_clear_screen(); // clears drawing buffers
                                      // all drawing should happen betweem clear_screen and next_frame
        // backgrounds drawn first
        gf2d_sprite_draw_image(background, vector2d(0, 0));
        // contents next
        // UI elements last

        // SDL_asprintf(&info_out, "Char's %s did %d", current_action->name, (current_action->max_dam, current_action->min_dam));
        // SDL_asprintf(&info_out, "Char's %s did Damage", current_action->name);
        // slog(info_out);
        // slog("Player Turn: %i", turn_player);
        if (keys[SDL_SCANCODE_P])
        {
            battle_now = 1;
            Enemy.hp = Enemy.max_hp;
        }
        if (battle_now)
        {
            Player.sprite = gf2d_sprite_load_image(Player.sprite_path);
            Enemy.sprite = gf2d_sprite_load_image(Enemy.sprite_path);
            battle_now = battle_battle(ctx, &Player, &Enemy, &inventory);
        }
        else
        {
            level_draw(level_get_active_level());
            entity_draw_all();
            camera_world_snap();
        }

        gf2d_sprite_draw(
            mouse,
            vector2d(mx, my),
            NULL,
            NULL,
            NULL,
            NULL,
            &mouseColor,
            (int)mf);

        gf2d_graphics_next_frame(); // render current draw frame and skip to the next frame

        // camera_center_at(mousep);
        // slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
        // slog("Mouse at %i , %i",mx,my);
        if ((pause == 0) && (keys[SDL_SCANCODE_ESCAPE]))
        {
            // pause = 1;
            done = 1; // exit condition
            slog("%i", pause);
        }
        // if(pause == 1)
        // {
        //     if ((keys[SDL_SCANCODE_ESCAPE]))
        //         pause = 0;
        //     slog("%i",pause);
        // }
    }
    level_free(level);
    nk_sdl_shutdown();
    entity_free(ent);
    slog("---==== END ====---");
    return 0;
}

/*eol@eof*/
