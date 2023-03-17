#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "battle.h"

#define TRUE 1
#define FALSE 0

#define TURN_DELAY 2000

Action *current_action;
int press_time;
char info_out[1024];

enum
{
    HEATED,
    NEXT,
    P1_INST,
    P2_INST,
    NUM_MESSAGES
};

char *Messages[] =
    {
        "A Heated Battle",
        "Next",
        "Player 1 Instance",
        "Player 2 Instance",
};


void battle_menu_output(struct nk_context *ctx, char *info_out)
{
    int flags = NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR;
    if (nk_begin(ctx, "top 4", nk_rect(600, 500, 300, 80), flags))
    {
        nk_layout_row_dynamic(ctx, 30, 1);
        nk_label(ctx, info_out, NK_TEXT_LEFT);
    }
    nk_end(ctx);
}

void battle_menu_enemy(struct nk_context *ctx, Character *c)
{
    char *enemy_status;
    SDL_asprintf(&enemy_status, "ENEMY Level %i | HP: %i/%i", c->level, c->hp, c->max_hp);
    int flags = NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR;
    if (nk_begin(ctx, "top 3", nk_rect(600, 50, 200, 80), flags))
    {
        nk_layout_row_dynamic(ctx, 30, 1);

        nk_label(ctx, c->name, NK_TEXT_LEFT);

        nk_label(ctx, enemy_status, NK_TEXT_LEFT);
    }
    nk_end(ctx);
}

void battle_menu_attack(struct nk_context *ctx, Character *c)
{
    int flags = NK_WINDOW_BORDER;
    if (nk_begin(ctx, "top 1", nk_rect(10, 500, 200, 200), flags))
    {
        nk_layout_row_dynamic(ctx, 30, 1);

        nk_label(ctx, c->name, NK_TEXT_LEFT);

        char *ally_status;
        SDL_asprintf(&ally_status, "Level %i | HP: %i/%i", c->level, c->hp, c->max_hp);
        nk_label(ctx, ally_status, NK_TEXT_LEFT);

        if (!current_action)
        {
            for (int i = 0; i < c->n_attacks; i++)
            {
                if (nk_button_label(ctx, c->attacks[i].name)) // render all buttons, and on button press do if statement
                {
                    current_action = &c->attacks[i];
                    press_time = SDL_GetTicks();
                }
            }
        }
    }
    nk_end(ctx);
}
void battle_menu_item(struct nk_context *ctx, Inventory *in)
{
    int flags = NK_WINDOW_BORDER;
    if (nk_begin(ctx, "top 2", nk_rect(300, 500, 200, 200), flags))
    {
        nk_layout_row_dynamic(ctx, 30, 1);

        nk_label(ctx, "ITEMS", NK_TEXT_LEFT);

        for (int i = 0; i < in->num_item; i++)
        {
            if (nk_button_label(ctx, in->item[i].name))
            {
                current_action = &in->item[i]; // fix later  u bum
                press_time = SDL_GetTicks();
            }
        }
    }
    nk_end(ctx);
}

int battle_action_rng(int min, int max)
{
    return rand() % (max - min) + min;
}
/// @brief true = lethal, false is not
/// @param target
/// @param act
/// @return
int battle_action_dmg(Character *target, Action act)
{
    int damage = battle_action_rng(act.min_dam, act.max_dam);
    slog("%s DID THIS %i", act.name, damage);
    target->hp -= damage;
    slog("ENEMY HP%i", target->hp);

    snprintf(info_out, 1024, "%s just took %d damage", target->name, damage);

    if (target->hp <= 0)
    {
        slog("%s has died", target->name);
        return TRUE;
    }
    else
    {
        slog("Continue Battle");
        return FALSE;
    }
}

/// @brief true = end, false = contine
/// @param ctx
/// @param info_out
/// @return
int battle_battle(struct nk_context *ctx, Character *player, Character *enemy, Inventory *inventory)
{
    static int whois_target = 0;
    // strcpy(info_out, Messages[HEATED]);
    // player goes first, meaning target is 0 enemy
    Character *players[] = {enemy, player};

    int cur_time = SDL_GetTicks();
    int time = cur_time - press_time;
    if (current_action)
    {
        // if you kill them
        if (battle_action_dmg(players[whois_target], *current_action))
        {
            whois_target = 0;
            current_action = NULL;
            return FALSE; // return false to end battle
        }

        whois_target = !whois_target;
        current_action = NULL;

        slog(info_out);
        press_time = cur_time;
    }
    else if (time > TURN_DELAY)
    {
        // if player is target, choose enemy attack
        if (whois_target)
        {
            current_action = &enemy->attacks[0]; // chose randomly
        }
        else
        {
            snprintf(info_out, 1024, "%s pick a new attack", player->name);
        }
    }
    gf2d_sprite_draw_image(player->sprite, vector2d(0, 0));
    gf2d_sprite_draw_image(enemy->sprite, vector2d(700, -150));
    battle_menu_attack(ctx, player);
    battle_menu_enemy(ctx, enemy);
    battle_menu_item(ctx, inventory);
    battle_menu_output(ctx, info_out);
    nk_sdl_render(NK_ANTI_ALIASING_ON);
    return TRUE;
}